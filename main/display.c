#include "display.h"
#include "driver/i2c_master.h"
#include "esp_log.h"
#include "esp_timer.h"
#include <string.h>
#include <stdio.h>

static const char *TAG = "display";

#if CFG_ENABLE_OLED

/* SH1106 commands */
#define SH1106_SETCONTRAST          0x81
#define SH1106_DISPLAYALLON_RESUME  0xA4
#define SH1106_DISPLAYALLON         0xA5
#define SH1106_NORMALDISPLAY        0xA6
#define SH1106_INVERTDISPLAY        0xA7
#define SH1106_DISPLAYOFF           0xAE
#define SH1106_DISPLAYON            0xAF
#define SH1106_SETPAGEADDR          0xB0
#define SH1106_SETCOLADDR_LOW       0x00
#define SH1106_SETCOLADDR_HIGH      0x10
#define SH1106_SETSTARTLINE         0x40

static i2c_master_bus_handle_t s_i2c_bus = NULL;
static i2c_master_dev_handle_t s_dev = NULL;
static uint8_t s_fb[OLED_PAGES][OLED_WIDTH];
static bool s_initialized = false;
static int64_t s_last_activity_us = 0;

static const uint8_t s_init_cmds[] = {
    SH1106_DISPLAYOFF,
    0x00, 0x10,             /* Column address = 0 */
    SH1106_SETSTARTLINE | 0, /* Start line 0 */
    0xB0,                   /* Page address 0 */
    0x81, 0xCF,             /* Contrast 207 */
    0xA1,                   /* Segment remap (left-right flipped) */
    0xA6,                   /* Normal (non-inverted) */
    0xA8, 0x3F,             /* Multiplex ratio 64 */
    0xD3, 0x00,             /* Display offset 0 */
    0xD5, 0x80,             /* Oscillator freq */
    0xD9, 0xF1,             /* Pre-charge period */
    0xDA, 0x12,             /* COM pins hardware config */
    0xDB, 0x40,             /* VCOM deselect level */
    SH1106_DISPLAYALLON_RESUME,
    SH1106_INVERTDISPLAY,   /* Reverse for white-on-dark */
    SH1106_DISPLAYON,
};

static esp_err_t sh1106_write_cmd(const uint8_t *data, size_t len)
{
    return i2c_master_transmit(s_dev, data, len, -1);
}

static esp_err_t sh1106_write_data(const uint8_t *data, size_t len)
{
    /* SH1106 data: prefix with 0x40 (Co=0, D/C#=1) */
    uint8_t buf[129];  /* 1 prefix + max 128 data */
    buf[0] = 0x40;
    memcpy(buf + 1, data, len);
    return i2c_master_transmit(s_dev, buf, len + 1, -1);
}

static void sh1106_refresh(void)
{
    for (int page = 0; page < OLED_PAGES; page++) {
        uint8_t cmds[3] = {
            SH1106_SETPAGEADDR | page,
            SH1106_SETCOLADDR_LOW  | 0x02,   /* SH1106 has 2-pixel column offset */
            SH1106_SETCOLADDR_HIGH | 0x00,
        };
        sh1106_write_cmd(cmds, 3);
        sh1106_write_data(s_fb[page], OLED_WIDTH);
    }
}

esp_err_t display_init(void)
{
    /* I2C bus config */
    i2c_master_bus_config_t bus_cfg = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .i2c_port = OLED_I2C_PORT,
        .sda_io_num = GPIO_I2C_SDA,
        .scl_io_num = GPIO_I2C_SCL,
        .flags.enable_internal_pullup = true,
    };
    esp_err_t ret = i2c_new_master_bus(&bus_cfg, &s_i2c_bus);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "I2C bus init failed: %s", esp_err_to_name(ret));
        return ret;
    }

    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = OLED_I2C_ADDR,
        .scl_speed_hz = OLED_I2C_FREQ_HZ,
    };
    ret = i2c_master_bus_add_device(s_i2c_bus, &dev_cfg, &s_dev);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "I2C device add failed: %s", esp_err_to_name(ret));
        return ret;
    }

    /* Send init sequence */
    ret = sh1106_write_cmd(s_init_cmds, sizeof(s_init_cmds));
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "SH1106 init failed: %s", esp_err_to_name(ret));
        return ret;
    }

    /* Clear framebuffer */
    memset(s_fb, 0, sizeof(s_fb));
    sh1106_refresh();

    s_initialized = true;
    s_last_activity_us = esp_timer_get_time();
    ESP_LOGI(TAG, "SH1106 OLED initialized: %dx%d, I2C 0x%02X",
             OLED_WIDTH, OLED_HEIGHT, OLED_I2C_ADDR);
    return ESP_OK;
}

/* ---- Simple 5x7 font (ASCII 32-127) ---- */
static const uint8_t font5x7[96][5] = {
    {0x00,0x00,0x00,0x00,0x00}, /* space */
    {0x00,0x00,0x5F,0x00,0x00}, /* ! */
    {0x00,0x07,0x00,0x07,0x00}, /* " */
    {0x14,0x7F,0x14,0x7F,0x14}, /* # */
    {0x24,0x2A,0x7F,0x2A,0x12}, /* $ */
    {0x23,0x13,0x08,0x64,0x62}, /* % */
    {0x36,0x49,0x55,0x22,0x50}, /* & */
    {0x00,0x05,0x03,0x00,0x00}, /* ' */
    {0x00,0x1C,0x22,0x41,0x00}, /* ( */
    {0x00,0x41,0x22,0x1C,0x00}, /* ) */
    {0x08,0x2A,0x1C,0x2A,0x08}, /* * */
    {0x08,0x08,0x3E,0x08,0x08}, /* + */
    {0x00,0x50,0x30,0x00,0x00}, /* , */
    {0x08,0x08,0x08,0x08,0x08}, /* - */
    {0x00,0x30,0x30,0x00,0x00}, /* . */
    {0x20,0x10,0x08,0x04,0x02}, /* / */
    {0x3E,0x51,0x49,0x45,0x3E}, /* 0 */
    {0x00,0x42,0x7F,0x40,0x00}, /* 1 */
    {0x42,0x61,0x51,0x49,0x46}, /* 2 */
    {0x21,0x41,0x45,0x4B,0x31}, /* 3 */
    {0x18,0x14,0x12,0x7F,0x10}, /* 4 */
    {0x27,0x45,0x45,0x45,0x39}, /* 5 */
    {0x3C,0x4A,0x49,0x49,0x30}, /* 6 */
    {0x01,0x71,0x09,0x05,0x03}, /* 7 */
    {0x36,0x49,0x49,0x49,0x36}, /* 8 */
    {0x06,0x49,0x49,0x29,0x1E}, /* 9 */
    {0x00,0x36,0x36,0x00,0x00}, /* : */
    {0x00,0x56,0x36,0x00,0x00}, /* ; */
    {0x00,0x08,0x14,0x22,0x41}, /* < */
    {0x14,0x14,0x14,0x14,0x14}, /* = */
    {0x41,0x22,0x14,0x08,0x00}, /* > */
    {0x02,0x01,0x51,0x09,0x06}, /* ? */
    {0x3E,0x41,0x5D,0x55,0x1E}, /* @ */
    {0x7E,0x11,0x11,0x11,0x7E}, /* A */
    {0x7F,0x49,0x49,0x49,0x36}, /* B */
    {0x3E,0x41,0x41,0x41,0x22}, /* C */
    {0x7F,0x41,0x41,0x22,0x1C}, /* D */
    {0x7F,0x49,0x49,0x49,0x41}, /* E */
    {0x7F,0x09,0x09,0x01,0x01}, /* F */
    {0x3E,0x41,0x41,0x51,0x32}, /* G */
    {0x7F,0x08,0x08,0x08,0x7F}, /* H */
    {0x00,0x41,0x7F,0x41,0x00}, /* I */
    {0x20,0x40,0x41,0x3F,0x01}, /* J */
    {0x7F,0x08,0x14,0x22,0x41}, /* K */
    {0x7F,0x40,0x40,0x40,0x40}, /* L */
    {0x7F,0x02,0x04,0x02,0x7F}, /* M */
    {0x7F,0x04,0x08,0x10,0x7F}, /* N */
    {0x3E,0x41,0x41,0x41,0x3E}, /* O */
    {0x7F,0x09,0x09,0x09,0x06}, /* P */
    {0x3E,0x41,0x51,0x21,0x5E}, /* Q */
    {0x7F,0x09,0x19,0x29,0x46}, /* R */
    {0x46,0x49,0x49,0x49,0x31}, /* S */
    {0x01,0x01,0x7F,0x01,0x01}, /* T */
    {0x3F,0x40,0x40,0x40,0x3F}, /* U */
    {0x1F,0x20,0x40,0x20,0x1F}, /* V */
    {0x7F,0x20,0x18,0x20,0x7F}, /* W */
    {0x63,0x14,0x08,0x14,0x63}, /* X */
    {0x03,0x04,0x78,0x04,0x03}, /* Y */
    {0x61,0x51,0x49,0x45,0x43}, /* Z */
};

static void draw_char(int x, int y, char c)
{
    if (c < 32 || c > 127) c = '?';
    const uint8_t *glyph = font5x7[c - 32];
    for (int col = 0; col < 5; col++) {
        if (x + col >= OLED_WIDTH) break;
        uint8_t bits = glyph[col];
        if (y < 8) {
            s_fb[y / 8][x + col] |= (bits << (y % 8));
            if (y % 8 > 1) {
                s_fb[y / 8 + 1][x + col] |= (bits >> (8 - y % 8));
            }
        }
    }
}

static void draw_string(int x, int y, const char *str)
{
    while (*str) {
        draw_char(x, y, *str);
        x += 6;
        str++;
    }
}

static void draw_hline(int x, int y, int len)
{
    if (y < 0 || y >= OLED_HEIGHT) return;
    for (int i = 0; i < len && (x + i) < OLED_WIDTH; i++) {
        s_fb[y / 8][x + i] |= (1 << (y % 8));
    }
}

static void draw_bar(int x, int y, int width, int height, uint8_t pct)
{
    int fill = (width * pct) / 100;
    for (int row = 0; row < height; row++) {
        draw_hline(x, y + row, width);
    }
    /* Clear right portion */
    for (int row = 0; row < height; row++) {
        for (int col = fill; col < width; col++) {
            if (x + col < OLED_WIDTH) {
                s_fb[y / 8 + row][x + col] &= ~(1 << (y % 8));
            }
        }
    }
}

void display_update(const display_state_t *state)
{
    if (!s_initialized || !state) return;

    /* Check sleep timeout */
    int64_t now = esp_timer_get_time();
    if (now - s_last_activity_us > OLED_TIMEOUT_MS * 1000) {
        sh1106_write_cmd((uint8_t[]){SH1106_DISPLAYOFF}, 1);
        return;
    }
    sh1106_write_cmd((uint8_t[]){SH1106_DISPLAYON}, 1);

    memset(s_fb, 0, sizeof(s_fb));

    /* Row 0: Status bar */
    char line[32];

    if (state->estop_active) {
        snprintf(line, sizeof(line), "E-STOP!");
        draw_string(0, 0, line);
    } else if (state->failsafe_active) {
        snprintf(line, sizeof(line), "FAILSAFE");
        draw_string(0, 0, line);
    } else if (state->rc_signal_ok) {
        uint16_t steer = state->rc_steer_us;
        const char *dir;
        if (steer < 1460) dir = "  <<";
        else if (steer > 1540) dir = ">>";
        else dir = " --";
        snprintf(line, sizeof(line), "%s %u%% %s",
                 state->engine_running ? "RUN" : "STOP",
                 state->throttle_pct, dir);
        draw_string(0, 0, line);
    } else {
        draw_string(0, 0, "NO SIG");
    }

    /* Separator */
    draw_hline(0, 9, OLED_WIDTH);

    /* Row 10-20: RC channel bars */
    draw_string(0, 11, "S");  /* Steering */
    uint8_t steer_pct = (state->rc_steer_us > RC_CENTER_US) ?
        50 + (state->rc_steer_us - RC_CENTER_US) * 50 / (RC_MAX_VALID_US - RC_CENTER_US) :
        50 - (RC_CENTER_US - state->rc_steer_us) * 50 / (RC_CENTER_US - RC_MIN_VALID_US);
    if (steer_pct > 100) steer_pct = 100;
    draw_bar(10, 11, 30, 3, steer_pct);
    snprintf(line, sizeof(line), "%.0f'", state->steering_angle_deg);
    draw_string(44, 11, line);

    draw_string(0, 16, "T");  /* Throttle */
    draw_bar(10, 16, 30, 3, state->throttle_pct);
    snprintf(line, sizeof(line), "%u%%", state->throttle_pct);
    draw_string(44, 16, line);

    draw_string(0, 21, "B");  /* Brake */
    draw_bar(10, 21, 30, 3, state->brake_pct);
    snprintf(line, sizeof(line), "%u%%", state->brake_pct);
    draw_string(44, 21, line);

    /* Separator */
    draw_hline(0, 26, OLED_WIDTH);

    /* Row 28+: Engine info */
#if CFG_ENABLE_RPM
    snprintf(line, sizeof(line), "RPM: %u", state->rpm);
    draw_string(0, 28, line);
#else
    draw_string(0, 28, "Engine:");
    draw_string(48, 28, state->engine_state_str);
#endif

    /* Row 36+: Steering angle graphical bar */
    draw_string(0, 37, "ST:");
    int bar_center = 60;
    int bar_width = 60;
    int bar_half = bar_width / 2;
    draw_hline(bar_center - bar_half, 38, bar_width);
    /* Tick mark at center */
    s_fb[38 / 8][bar_center] ^= (1 << (38 % 8));
    s_fb[38 / 8][bar_center] ^= (1 << (38 % 8));

    /* Steering position indicator */
    float angle_ratio = state->steering_angle_deg / STEERING_MAX_ANGLE_DEG;
    if (angle_ratio > 1.0f) angle_ratio = 1.0f;
    if (angle_ratio < -1.0f) angle_ratio = -1.0f;
    int pos_x = bar_center + (int)(angle_ratio * bar_half);
    /* Draw a small arrow/cursor */
    for (int dy = -1; dy <= 1; dy++) {
        if (pos_x >= 0 && pos_x < OLED_WIDTH) {
            s_fb[(38 + dy) / 8][pos_x] |= (1 << ((38 + dy) % 8));
        }
    }

    /* Bottom line: connection status */
    draw_hline(0, 47, OLED_WIDTH);
    snprintf(line, sizeof(line), "%s  %s",
             state->rc_signal_ok ? "RC:OK" : "RC:--",
             state->engine_state_str);
    draw_string(0, 49, line);

    /* Firmware version */
    draw_string(100, 55, FIRMWARE_VERSION);

    sh1106_refresh();
}

void display_set_power(bool on)
{
    if (!s_initialized) return;
    uint8_t cmd = on ? SH1106_DISPLAYON : SH1106_DISPLAYOFF;
    sh1106_write_cmd(&cmd, 1);
}

void display_notify_activity(void)
{
    if (!s_initialized) return;
    s_last_activity_us = esp_timer_get_time();
}

#else /* !CFG_ENABLE_OLED */

esp_err_t display_init(void)
{
    ESP_LOGI(TAG, "OLED display disabled (CFG_ENABLE_OLED=0)");
    return ESP_OK;
}

void display_update(const display_state_t *state) { (void)state; }
void display_set_power(bool on) { (void)on; }
void display_notify_activity(void) {}

#endif /* CFG_ENABLE_OLED */

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

#define CMD_BUF_SZ  32

/*
 * SH1106 I2C protocol: every transaction starts with a control byte.
 *   0x00 = next bytes are commands  (D/C#=0)
 *   0x40 = next bytes are data      (D/C#=1)
 */
static esp_err_t sh1106_write_cmd(const uint8_t *data, size_t len)
{
    uint8_t buf[CMD_BUF_SZ];
    buf[0] = 0x00;
    memcpy(buf + 1, data, len);
    return i2c_master_transmit(s_dev, buf, len + 1, -1);
}

static esp_err_t sh1106_write_data(const uint8_t *data, size_t len)
{
    /* Max 128 bytes per SH1106 page — 1 prefix + 128 data = 129 */
    uint8_t buf[129];
    buf[0] = 0x40;
    memcpy(buf + 1, data, len);
    return i2c_master_transmit(s_dev, buf, len + 1, -1);
}

/* Helper: send single-byte command */
static esp_err_t sh1106_cmd1(uint8_t cmd)  { return sh1106_write_cmd(&cmd, 1); }
/* Helper: send two-byte command-register pair */
static esp_err_t sh1106_cmd2(uint8_t cmd, uint8_t val) {
    uint8_t d[2] = {cmd, val}; return sh1106_write_cmd(d, 2);
}

static void sh1106_refresh(void)
{
    for (int page = 0; page < OLED_PAGES; page++) {
        sh1106_cmd1(SH1106_SETPAGEADDR | page);
        sh1106_cmd1(0x02);           /* lower column = 2 (SH1106 2-pixel offset) */
        sh1106_cmd1(0x10);           /* upper column = 0 */
        sh1106_write_data(s_fb[page], OLED_WIDTH);
    }
}

esp_err_t display_init(void)
{
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
        ESP_LOGE(TAG, "I2C bus: %s", esp_err_to_name(ret));
        return ret;
    }

    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = OLED_I2C_ADDR,
        .scl_speed_hz = OLED_I2C_FREQ_HZ,
    };
    ret = i2c_master_bus_add_device(s_i2c_bus, &dev_cfg, &s_dev);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "I2C device: %s", esp_err_to_name(ret));
        return ret;
    }

    /* ---- SH1106 init sequence (each command as separate I2C transaction) ---- */
    ESP_ERROR_CHECK_WITHOUT_ABORT(sh1106_cmd1(SH1106_DISPLAYOFF));
    /* Column address — value encoded in command byte itself (0x00-0x0F / 0x10-0x1F) */
    ESP_ERROR_CHECK_WITHOUT_ABORT(sh1106_cmd1(0x00));     /* lower column = 0 */
    ESP_ERROR_CHECK_WITHOUT_ABORT(sh1106_cmd1(0x10));     /* upper column = 0 */
    ESP_ERROR_CHECK_WITHOUT_ABORT(sh1106_cmd1(SH1106_SETSTARTLINE | 0x00));
    ESP_ERROR_CHECK_WITHOUT_ABORT(sh1106_cmd1(SH1106_SETPAGEADDR | 0x00));
    ESP_ERROR_CHECK_WITHOUT_ABORT(sh1106_cmd2(SH1106_SETCONTRAST, 0xCF));
    ESP_ERROR_CHECK_WITHOUT_ABORT(sh1106_cmd1(0xA1));   /* segment remap */
    ESP_ERROR_CHECK_WITHOUT_ABORT(sh1106_cmd1(0xC8));   /* COM scan reverse */
    ESP_ERROR_CHECK_WITHOUT_ABORT(sh1106_cmd1(SH1106_NORMALDISPLAY));
    ESP_ERROR_CHECK_WITHOUT_ABORT(sh1106_cmd2(0xA8, 0x3F));   /* multiplex 64 */
    ESP_ERROR_CHECK_WITHOUT_ABORT(sh1106_cmd2(0xD3, 0x00));   /* display offset 0 */
    ESP_ERROR_CHECK_WITHOUT_ABORT(sh1106_cmd2(0xD5, 0x80));   /* oscillator */
    ESP_ERROR_CHECK_WITHOUT_ABORT(sh1106_cmd2(0xD9, 0xF1));   /* pre-charge */
    ESP_ERROR_CHECK_WITHOUT_ABORT(sh1106_cmd2(0xDA, 0x12));   /* COM pins */
    ESP_ERROR_CHECK_WITHOUT_ABORT(sh1106_cmd2(0xDB, 0x40));   /* VCOM deselect */
    ESP_ERROR_CHECK_WITHOUT_ABORT(sh1106_cmd1(SH1106_DISPLAYALLON_RESUME));
    ESP_ERROR_CHECK_WITHOUT_ABORT(sh1106_cmd1(SH1106_INVERTDISPLAY)); /* white-on-dark */
    ESP_ERROR_CHECK_WITHOUT_ABORT(sh1106_cmd1(SH1106_DISPLAYON));

    memset(s_fb, 0, sizeof(s_fb));
    sh1106_refresh();

    s_initialized = true;
    s_last_activity_us = esp_timer_get_time();
    ESP_LOGI(TAG, "SH1106 OLED ready: %dx%d I2C 0x%02X",
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
    int page = y / 8;
    int shift = y % 8;
    for (int col = 0; col < 5; col++) {
        if (x + col >= OLED_WIDTH) break;
        uint8_t bits = glyph[col];
        s_fb[page][x + col] |= (bits << shift);
        if (shift > 1 && page + 1 < OLED_PAGES) {
            s_fb[page + 1][x + col] |= (bits >> (8 - shift));
        }
    }
}

static void draw_string(int x, int y, const char *str)
{
    while (*str) { draw_char(x, y, *str); x += 6; str++; }
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
    for (int row = 0; row < height; row++) draw_hline(x, y + row, width);
    for (int row = 0; row < height; row++) {
        for (int col = fill; col < width; col++) {
            if (x + col < OLED_WIDTH) s_fb[y / 8 + row][x + col] &= ~(1 << (y % 8));
        }
    }
}

void display_update(const display_state_t *state)
{
    if (!s_initialized || !state) return;

    int64_t now = esp_timer_get_time();
    if (now - s_last_activity_us > OLED_TIMEOUT_MS * 1000) {
        sh1106_cmd1(SH1106_DISPLAYOFF);
        return;
    }
    sh1106_cmd1(SH1106_DISPLAYON);

    memset(s_fb, 0, sizeof(s_fb));

    char line[32];

#define BAR_X       10
#define BAR_W       100
#define BAR_H       6
#define VAL_X       (BAR_X + BAR_W + 4)
#define ROW_S       3
#define ROW_T       15
#define ROW_B       27
#define ROW_BOT     45

    /* Steering bar */
    draw_string(0, ROW_S, "S");
    uint8_t steer_pct = (state->rc_steer_us > RC_CENTER_US) ?
        50 + (state->rc_steer_us - RC_CENTER_US) * 50 / (RC_MAX_VALID_US - RC_CENTER_US) :
        50 - (RC_CENTER_US - state->rc_steer_us) * 50 / (RC_CENTER_US - RC_MIN_VALID_US);
    if (steer_pct > 100) steer_pct = 100;
    draw_bar(BAR_X, ROW_S, BAR_W, BAR_H, steer_pct);
    snprintf(line, sizeof(line), "%.0f'", state->steering_angle_deg);
    draw_string(VAL_X, ROW_S, line);

    /* Throttle bar */
    draw_string(0, ROW_T, "T");
    if (state->throttle_pct > 0) {
        draw_bar(BAR_X, ROW_T, BAR_W, BAR_H, state->throttle_pct);
    } else {
        /* Empty outline */
        for (int r = 0; r < BAR_H; r++) draw_hline(BAR_X, ROW_T + r, BAR_W);
    }
    snprintf(line, sizeof(line), "%u%%", state->throttle_pct);
    draw_string(VAL_X, ROW_T, line);

    /* Brake bar (fill from right side = pct% of bar) */
    draw_string(0, ROW_B, "B");
    if (state->brake_pct > 0) {
        int fill = (BAR_W * state->brake_pct) / 100;
        for (int r = 0; r < BAR_H; r++) {
            for (int c = BAR_X; c < BAR_X + fill; c++) {
                if (c < OLED_WIDTH) s_fb[(ROW_B + r) / 8][c] |= (1 << ((ROW_B + r) % 8));
            }
        }
        /* Outline full width */
        for (int r = 0; r < BAR_H; r++) draw_hline(BAR_X, ROW_B + r, BAR_W);
    } else {
        for (int r = 0; r < BAR_H; r++) draw_hline(BAR_X, ROW_B + r, BAR_W);
    }
    snprintf(line, sizeof(line), "%u%%", state->brake_pct);
    draw_string(VAL_X, ROW_B, line);

    /* Bottom status line */
    if (state->estop_active) {
        draw_string(0, ROW_BOT, "E-STOP!");
    } else if (state->failsafe_active) {
        draw_string(0, ROW_BOT, "FAILSAFE");
    } else {
        snprintf(line, sizeof(line), "%s  %s",
                 state->rc_signal_ok ? "RC:OK" : "RC:--",
                 state->engine_state_str);
        draw_string(0, ROW_BOT, line);
    }
    snprintf(line, sizeof(line), FIRMWARE_VERSION);
    draw_string(90, ROW_BOT + 8, line);

    sh1106_refresh();
}

void display_set_power(bool on)
{
    if (!s_initialized) return;
    sh1106_cmd1(on ? SH1106_DISPLAYON : SH1106_DISPLAYOFF);
}

void display_notify_activity(void)
{
    if (!s_initialized) return;
    s_last_activity_us = esp_timer_get_time();
}

void display_show_calibration(const char *axis_label, const char *status,
                               int bars_filled, int bars_empty)
{
    if (!s_initialized) return;
    s_last_activity_us = esp_timer_get_time();
    sh1106_cmd1(SH1106_DISPLAYON);

    memset(s_fb, 0, sizeof(s_fb));

    draw_string(0, 0, "CALIBRATION");
    draw_string(0, 14, axis_label);
    draw_string(0, 26, status);

    int bar_y = 40;
    int bar_x = 0;
    int seg_w = (OLED_WIDTH - 4) / 20;

    for (int i = 0; i < bars_filled; i++) {
        for (int r = 0; r < 8; r++) {
            for (int c = 0; c < seg_w - 2; c++) {
                int px = bar_x + i * seg_w + c;
                if (px < OLED_WIDTH) s_fb[(bar_y + r) / 8][px] |= (1 << ((bar_y + r) % 8));
            }
        }
    }
    for (int i = 0; i < 20; i++) {
        for (int c = 0; c < seg_w - 2; c++) {
            int px = bar_x + i * seg_w + c;
            if (px < OLED_WIDTH) s_fb[bar_y / 8][px] |= (1 << (bar_y % 8));
            if (px < OLED_WIDTH) s_fb[(bar_y + 7) / 8][px] |= (1 << ((bar_y + 7) % 8));
        }
    }

    sh1106_refresh();
}

#else /* !CFG_ENABLE_OLED */

esp_err_t display_init(void)
{
    ESP_LOGI(TAG, "OLED disabled (CFG_ENABLE_OLED=0)");
    return ESP_OK;
}

void display_update(const display_state_t *state) { (void)state; }
void display_set_power(bool on) { (void)on; }
void display_notify_activity(void) {}
void display_show_calibration(const char *a, const char *s, int f, int e) { (void)a; (void)s; (void)f; (void)e; }

#endif /* CFG_ENABLE_OLED */

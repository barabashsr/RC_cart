## UserManual

# DSY-RS Series

## Low-voltageServoDrive

## Usermanual(V 1. 0 )

© 2024 AllRightsReserved

Address: 15 - 4 ,# 799 HushanRoad,Jiangning,Nanjing,China

Tel: 0086 - 2587156578

Web:www.omc-stepperonline.com

Sales:sales@stepperonline.com

Support:technical@stepperonline.com


## Chapter 1 SelectionofServoSystem


- Content Content
- Chapter 1 SelectionofServoSystem
   - 1 1 Drivespecifications.............................................................................................................................
   - 1 2 Servomotorsanddrivesmodeldescription...................................................................................
   - 1 3 Servodriveandservomotormatchinglist.....................................................................................
   - 1 4 Matchingmotorparameters..............................................................................................................
   - 1 5 Matchingcable....................................................................................................................................
- Chapter 2 DimensionsandProductInstallation
   - 2 1 Servodrivedimensions.....................................................................................................................
   - 2 2 Servodriveinstallation.....................................................................................................................
- Chapter 3 ServoDriveandMotorConnection
   - 3 1 Systemstructurediagram...............................................................................................................
   - 3 2 Maincircuit.........................................................................................................................................
   - 3 3 Powercableconnector....................................................................................................................
   - 3 4 Encoderconnector............................................................................................................................
   - 3 5 DIDOPort...........................................................................................................................................
   - 3 6 Communicationsignalport..............................................................................................................
- Chapter 4 Control
   - 4 1 Basiccontrol......................................................................................................................................
   - 4 2 Positioncontrolmode.......................................................................................................................
   - 4 3 Speedcontrolmode.........................................................................................................................
   - 4 4 Torquecontrolmode....................................................................................................................... Chapter 1 SelectionofServoSystem
   - 4 5 Mixedmodeswitching....................................................................................................................
- Chapter 5 Operationperformanceadjustment
   - 5 1 Overview...........................................................................................................................................
   - 5 2 InertiaIdentification.........................................................................................................................
   - 5 3 Gainadjustment..............................................................................................................................
   - 5 4 Torquefeedforward.........................................................................................................................
   - 5 5 Commandfilteradjustment...........................................................................................................
   - 5 6 Adjustmentparametersindifferentmode...................................................................................
   - 5 7 Mechanicalresonancesuppression............................................................................................
- Chapter 6 AlarmandProcessing
   - 6 1 Faultdiagnosisandtreatment......................................................................................................
   - 6 2 Reasonsforwarningandtreatment............................................................................................
- Chapter 7 ParameterList
   - 7 1 Parametergroupnumber..............................................................................................................
   - 7 2 Eachgroupofparameters.............................................................................................................
- AppendixA:Alarmlightinformationdescription
- AppendixB:RS- 485 Communicationaddresscalculationdescription
   - 1 1 Communicationsignalport............................................................................................................
   - 1 2 WiringinstructionsandEMCprecautions..................................................................................
   - 1 3 Communicationrelatedparameter...........................................................................................
   - 1 4 ModBusProtocol.............................................................................................................................


## Chapter 1 SelectionofServoSystem

1. 5 485 Precautionsforonsitecommunication............................................................................. 176

**AppendixC:Brakeresistorconfiguration** ............................................. 180

**AppendixD:Versionchangelog** ..................................................... 181


Chapter 1 SelectionofServoSystem

## Chapter 1 SelectionofServo System

### 1 1 Drivespecifications.............................................................................................................................

#### B

```
as
ic
S
p
ec
ifi
ca
tio
n
s
```
```
Model DSY- 200 RS DSY- 400 RS DSY- 750 RS DSY- 1200 RS
```
```
ContinuousOutput
```
```
Current （ Arms ）
```
#### 7. 5 15 25 40

```
MaximumOutput
```
```
Current （ Arms ）
```
#### 24 48 80 120

```
PowerSupplyInput 24 VDC~ 70 VDC
```
```
Environm
```
```
ent
```
```
Temperature
```
```
OperationTemperature： 0 °C～ 55 °C(IfEnvironmenttemperatureover
```
```
55 °C,plzlowerit)
```
```
StorageTemperature:- 40 °C～ 80 °C
```
```
Humidity 0 ～ 90 %RH(Nodewenvironment)
```
```
Elevation Elevation≤ 1000 m
```
```
Vibration/Strike 5. 88 m/s 2 / 19. 6 m/s 2
```
```
IPGrade/
```
```
Contamination
```
```
IP 20 /ContaminationRating 2
```
```
ControlMethod MOSFETSVPWMSpacevectorpulsewidthmodulationalgorithm
```
```
Encoder 17 - bitmagneticencoder； 23 - bitopticalencoder
```
```
Input/Out
```
```
putPorts
```
```
IOSignal
```
```
Input 3 pcs(General)
```
```
Output 1 GeneralDOOutput, 1 BrakeOutput
```
```
Network
```
```
RS 485 ModbusCommunication
```
```
RS- 232 ConnectwithPCtodebugservodrive
```

Chapter 1 SelectionofServoSystem

```
ProtectionFunction
```
```
Overcurrent,Overload,Overvoltage,LowVoltage,Over
```
```
speed,Overheat,Encodererror,Communicationerror,
```
```
Positiondeviation
```
```
ControlMode
```
```
PositionControlMode;SpeedControlMode;Torque
```
```
ControlMode;Mixed-modeSwitching
```
```
Position
```
```
Control
```
```
Mode
```
```
LocationCommandFormat
```
#### PULSE/DIR

#### CW/CCW

```
A,BPhaseCrossPulse
```
```
Functioncodeparameterssetting
```
```
SmoothingFilter Smoothingprocessforpositioncommandtoensuremotorrunmore
stably
```
```
ElectronicGear Twosetsofelectronicgearratiosavailable
```
```
FeedforwardCompensation 0 ～ 100 %defaultresolutionratio: 1 %)
```
**Speed/Tor**

```
que
```
```
Control
```
```
Mode
```
```
CommandForm
```
```
Internalspeed/torquecommand
```
```
Functioncodeparameterssetting
```
```
Speed
```
```
Change
```
```
Rate
```
```
Currentcontrol
```
```
Precision
```
```
± 5 %(Ratedtorque)
```
```
OverloadCapacity 300 %motorratedtorque
```
```
Temperature
```
```
Fluctuation
```
```
25 ± 25 °C(Ratedspeed)
```
```
Common
```
```
Spec
```
```
Self-tuningFunction Identifyinginertia
```
```
Over-distance(OT)Prevent
```
```
Function
```
```
SlowingdowntostopwhenP-OT、N-OT
```
```
ErrorFeedbackandRecord Redandgreendouble-holeindicator, 9 setofrecords
```

Chapter 1 SelectionofServoSystem

### 1 2 Servomotorsanddrivesmodeldescription...................................................................................

**1. 2. 1 DriveModelDescription**

```
DS Y - 200 RS
```
```
Controlmethod:RS 485
```
```
Power ： 200 / 400 / 750 / 1200 W
```
```
Productrange
```
```
ProductCategories
```
```
Figure 1. 1 Drivenamingrule
```

Chapter 1 SelectionofServoSystem

**1. 2. 2 Servomotormodeldescription**

**DS Y 40 - 100 H 2 A 2 M 17 S**

```
Encoderturns:
S:Single-turn
```
```
Encoderaccuracy: 17 bit
```
```
EncoderType:
```
```
M:Magnetic E:Optical
```
```
PlugType: 3 :Aviationplug
```
```
2 :Plasticplug 1 :DirectPlug
```
```
Brake:A:withoutbrake
```
```
B:withbrake
```
```
InputVoltage:
```
```
1 : 24 v 2 : 48 v
```
```
Inertia:
```
```
H:high/M:medium/L:low
```
```
Power: 100 / 200 / 400 / 750 / 1000 W
```
```
Motorflangesize: 40 / 60 / 80 mm
```
```
Productrange
```
```
ProductCategories
```
```
Figure 1. 2 Motornamingrules
```
### 1 3 Servodriveandservomotormatchinglist.....................................................................................

```
Table 1. 2 Servodriveandmotormatchingtable
```
```
ServoMotorModels Flange Power
```
```
DriveModel
Matchingcable
DSY-xxRS
```
```
Small
```
```
inertia
```
#### DSY 40 - 100 L 2 A 2 M 17 S 40 100 W 200

```
P 1 - DS-xx
```
```
E 2 - DS-xx
```
#### DSY 40 - 100 L 2 B 2 M 17 S 40 100 W 200

#### DSY 60 - 200 L 2 A 2 M 17 S 60 200 W 200

#### DSY 60 - 200 L 2 B 2 M 17 S 60 200 W 200


Chapter 1 SelectionofServoSystem

```
ServoMotorModels Flange Power
```
```
DriveModel
Matchingcable
DSY-xxRS
```
#### DSY 60 - 400 L 2 A 2 M 17 S 60 400 W 400

#### DSY 60 - 400 L 2 B 2 M 17 S 60 400 W 400

#### DSY 80 - 750 L 2 A 2 M 17 S 80 750 W 800

```
P 2 - DS-xx
```
```
E 2 - DS-xx
```
#### DSY 80 - 750 L 2 B 2 M 17 S 80 750 W 800

#### DSY 80 - 1000 L 2 A 2 M 17 S 80 1000 W 1200

#### DSY 80 - 1000 L 2 B 2 M 17 S 80 1000 W 1200

### 1 4 Matchingmotorparameters..............................................................................................................

```
Table 1. 3 Motorparametertable
```
```
Motorspec.
```
```
Ratedoutput
```
```
KW
```
```
Ratedtorque
```
```
Nm
```
```
Max.Torque
```
```
Nm
```
```
Ratedcurrent
```
```
Arms
```
```
Rated
```
```
Speed
```
```
min-^1
```
```
Rotor
```
```
Inertia
```
```
Kgm^2 。 10 -^4
```
#### DSY 40 - 100 L 2 A 2 M 17 S 0. 1 0. 318 0. 954 3. 1 3000 0. 046

#### DSY 40 - 100 L 2 B 2 M 17 S 0. 1 0. 318 0. 954 3. 1 3000 0. 048

#### DSY 60 - 200 L 2 A 2 M 17 S 0. 2 0. 64 1. 91 5. 2 3000 0. 27

#### DSY 60 - 200 L 2 B 2 M 17 S 0. 2 0. 64 1. 91 5. 2 3000 0. 29

#### DSY 60 - 400 L 2 A 2 M 17 S 0. 4 1. 27 3. 81 10. 2 3000 0. 49

#### DSY 60 - 400 L 2 B 2 M 17 S 0. 4 1. 27 3. 81 10. 2 3000 0. 51

#### DSY 80 - 750 L 2 A 2 M 17 S 0. 75 2. 39 7. 17 18. 5 3000 1. 42

#### DSY 80 - 750 L 2 B 2 M 17 S 0. 75 2. 39 7. 17 18. 5 3000 1. 62

#### DSY 80 - 1000 L 2 A 2 M 17 S 1. 0 3. 2 9. 6 24. 5 3000 1. 92

#### DSY 80 - 1000 L 2 B 2 M 17 S 1. 0 3. 2 9. 6 24. 5 3000 2. 12


Chapter 1 SelectionofServoSystem

### 1 5 Matchingcable....................................................................................................................................

Thenamingrulesformotorpowerlinesareasfollows：

```
Figure 1. 3 Powerlinenamingrules
```
Thestructurediagramofthemotorpowercableisshowninthetablebelow：

```
1 .P 1 AnPuConnectorCable（LeadLengthL= 1. 5 mˎ 3 mˎ 5 m）
```
```
2 .P 2 CircularConnectorCable（LeadLengthL= 1. 5 mˎ 3 mˎ 5 m）
```
```
Figure 1. 4 Powercablestructurediagram
```

Chapter 1 SelectionofServoSystem

Themotorpowerlinepinsaredefinedasfollows

```
Conn. 1 Pindefinition Conn. 2
```
```
Pincolor
```
```
1 U U Red
```
```
2 V V Yellow
```
```
3 W W Blue
```
```
4 PE PE Yellowgreen
```
Thenamingrulesformotorencoderlinesareasfollows:

```
Figure 1. 5 Encoderlinenamingrules
```
Thestructurediagramoftheencodercableisshowninthetablebelow:

```
Figure 1. 6 Encodercablestructurediagram(L= 1. 5 mˎ 3 mˎ 5 m)
```

Chapter 1 SelectionofServoSystem

Themotorencoderlinepinsaredefinedasfollows:

```
Conn. 1 Pindefinition Conn. 2 Pincolor
```
```
7 5 V 1 Blue
```
```
8 0 V 2 Orange
```
```
3 SD+ 5 Brown
```
```
4 SD- 6 Green
```
```
Shell PE Shell /
```
```
Battery /
```
#### 3 BAT+

#### 4 BAT-


## Chapter 2 DimensionsandProductInstallation

## Chapter 2 DimensionsandProductInstallation

### 2 1 Servodrivedimensions.....................................................................................................................

```
Figure 2. 1 ThepictureofDSY- 200 RS/DSY- 400 RSdrivestructure
```

Chapter 2 DimensionsandProductInstallation

```
Figure 2. 2 ThepictureofDSY- 750 RSdrivestructure
```
Figure 2. 3 ThepictureofDSY- 1200 RSdrivestructure


Chapter 2 DimensionsandProductInstallation

### 2 2 Servodriveinstallation.....................................................................................................................

```
Figure 2. 4 Servodriveinstallation
```

## Chapter 3 ServoDriveandMotorConnection

## Chapter 3 Servo Drive andMotorConnection

### 3 1 Systemstructurediagram...............................................................................................................

Thenames,functionsandspecificationsofeachpartofthedriveareasfollows.

```
Table 3. 1 Servodrivemaincircuitterminalnameandfunction
```
```
No. Name Description
```
```
1 DC+、DC- Inputcontrolcircuitpoweraccordingtospecifications
```
#### 2

#### RB+,RB-

```
RegenerativeResistor
```
```
Terminal
```
```
ExternalBrakeResistorConnectionPort
```
```
3 Motorpowercable Connectservomotorthree-phaseandgroundwire
```
```
5 Communicationport ConnectedtoPCviaRS 232 tomonitorthedrive,testandchangeparameter,etc
```
```
6 Communicationcontrol ConnectedtoRS 485 communicationdevice
```
```
7 Controlport ConnecttohostcontrollerforIOsignalcontrol
```
```
8 Encoderfeedbackport Connectedtothemotorencodercable
```
### 3 2 Maincircuit.........................................................................................................................................

Thefunctions,namesandspecificationsofeachpartofthemaincircuitareasfollows.

```
Table 3. 2 Servodrivemaincircuitterminalfunctionspecifications
```

Chapter 3 ServoDriveandMotorConnection

```
Name Terminalmark Function,specification
```
```
Powerinputterminal DC+、DC- Powercircuitofthepowersupplyaccordingtotheinputspecifications.
```
```
Regenerativeresistor
```
```
connectionterminal
```
#### RB+,RB-

```
TheexternalregenerativeresistorbetweentheRB+withRB-.
```
```
Fortheexternalbrakeresistor,pleasebuyseparately.
```
ServomotorPowercable
connectionterminal

```
U、V、W ConnectedtotheU,V,Wofthemotor.
```
```
GroundTerminal PE
```
```
Twogroundingterminalsareconnectedtothepowergroundingterminaland
themotorgroundingterminal.
```
```
Besuretogroundtheentiresystem.
```
### 3 3 Powercableconnector....................................................................................................................

```
Table 3. 3 Powercableconnector
```
```
Terminalpin Connectorpicture
```
```
5 Pincircularconnector
```
```
PinNO. Signalname
```
#### 1 U

#### 2 V

#### 3 W

#### 4 PE

```
4 PinAnPuconnector
```
```
PinNO. Signalname
```
#### 1 U

#### 2 V

#### 3 W

#### 4 PE


Chapter 3 ServoDriveandMotorConnection

### 3 4 Encoderconnector............................................................................................................................

Pindefinition Picture

#### 1 2 3 4 5

#### SD+ SD-

#### 6 7 8 9 PE

#### + 5 V GND PE

### 3 5 DIDOPort...........................................................................................................................................

```
Definition PIN Function
```
```
Generalinputandoutputsignal
```
#### BRK+ 1

```
Brakeoutput
BRK- 2
```
```
DI 1 3 Digitalinput 1
```
```
DI 2 4 Digitalinput 2
```
```
DI 3 5 Digitalinput 3
```
```
ICOM 6 DIpowersupplyinputport， 24 V
```
#### DO+ 7

```
Digitaloutput 1
DO- 8
```
#### PULSE+ 9

```
Pulsesignalinput（ 5 V~ 24 V,capatible）
PULSE- 10
```
#### SIGH+ 11

```
Directionsignalinput（ 5 V~ 24 V,capatible）
SIGH- 12
```
**3. 5. 1 Digitalinputcircuit**

TakeDI 1 asanexample,DI 1 ~DI 3 isthesame.

**a) Whenthehostcomputerisrelayoutput:**


Chapter 3 ServoDriveandMotorConnection

**b) Whenthehostcomputerisopencollectoroutput:**

```
Note ：
```
ThemixingofNPNandPNPinputsisnotsupported.

**3. 5. 2 Digitaloutputcircuit**

**a) Whenthehostcomputerisarelayinput**


Chapter 3 ServoDriveandMotorConnection

```
Note ：
```
```
Whenthehostcomputerisrelayinput,besuretoconnectthefreewheelingdiodeinthecorrectdirection,otherwisetheDOport
```
```
maybedamaged.
```
**b) Whenthehostcomputerisoptocouplerinput**

```
Note ：
```
1. Besuretoconnectacurrentlimitingresistor.

2. Themaximumallowablevoltageandcurrentoftheinternaloptocouplercircuitoftheservodriverareasfollows:

```
Voltage:DC 30 V(maximum)
```
```
Current:DC 50 mA(maximum)
```
### 3 6 Communicationsignalport..............................................................................................................

**3. 6. 1 CN 1 /CN 2 BusportCN 1 /CN 2**

CN 1 andCN 2 isthe 485 communicationconnector.

```
Table 3. 4 communicationconnectorpindefinition
```

Chapter 3 ServoDriveandMotorConnection

Pin Signalname Direction Terminal

```
1 RS 485 + Output
```
```
2 RS 485 - Output
```
```
3 GND Output
```
```
4 - - unused
```
```
5 - - unused
```
```
6 - - unused
```
```
7 - - unused
```
```
8 - - unused
```

Chapter 3 ServoDriveandMotorConnection

**3. 6. 2 PostcomputercommunicationportCN 3**

```
Table 3. 5 communicationconnectorpindefinition
```
```
Pin Definition Function Terminal
```
```
1 - -
```
```
2 - -
```
```
3 RS 232 - TXD RS 232 communication
```
```
port
```
```
4 GND
```
```
5 GND
```
```
6 RS 232 - RXD
```
```
7 - -
```
```
8 - -
```
```
shell PE Shielding
```

## Chapter 4 Control

## Chapter 4 Control

### 4 1 Basiccontrol......................................................................................................................................

```
Figure 4. 1 Servosettingprocess
```
```
4. 1. 1 Checkbeforerunning
```
```
Thefollowingchecksarerequiredbeforetheservodriverandservomotorrun：
```

Chapter 4 Control

```
Table 5. 1 Checkstepsbeforeoperation
```
```
Recor No. Description
```
```
Wiring
```
```
□ 1 Thecontrolcircuitpowerinputterminals(DC+,DC-)oftheservodrivemustbeconnectedcorrectly.
```
#### □ 2

```
Theoutputterminals(U,V,W)ofthemaincircuitoftheservodriverandthemaincircuitcables(U,V,W)
```
```
oftheservomotormusthavethesamephaseandbecorrectlyconnected.
```
#### □ 3

```
Thewiringofeachcontrolsignalcableoftheservodriveriscorrect:theexternalsignalcablessuchas
```
```
brakeandovertravelprotectionhavebeenreliablyconnected.
```
```
□ 4 Servodrivesandservomotorsmustbereliablygrounded.
```
```
□ 5 Thestressofallcablesiswithinthespecifiedrange.
```
```
□ 6 Thewiringterminalshavebeeninsulated.
```
```
Conditionandmachine
```
#### □ 1

```
Therearenowirestubs,metalfilingsandotherforeignobjectsinsideandoutsidetheservodrivethatmay
```
```
causeshortcircuitsinthesignalwiresandpowerwires.
```
```
□ 2 Servodrivesandexternalbrakingresistorsarenotplacedoncombustibleobjects.
```
```
□ 3 Theinstallationoftheservomotor,theconnectionoftheshaftandthemachinemustbereliable.
```
```
□ 4 Theservomotorandconnectedmachinerymustbeinoperablecondition.
```
**4. 1. 2 Poweron**

```
1 ） Turnonthecontrolloop(DC+,DC-)
```

Chapter 4 Control

```
●Afterthecontrolcircuitpowersupplyandthemaincircuitpowersupplyareconnected,thebusvoltageindicator
```
showsnoabnormalityandwaitforthehostcomputertogivetheservoenablesignal.

```
●Ifthedriver'sredalarmlightison,pleasecheckthewiringandeliminatethecauseofthefault.
```
```
4. 1. 3 JOGoperation
```
```
PleaseusetheJOGmodeinthehostcomputersoftwaretoconfirmwhethertheservomotorcanrotatenormally,and
```
thereisnoabnormalvibrationandabnormalsoundwhenrotating.TheJOGmodecontrolpanelisshowninFigure 4. 2.

```
Figure 4. 2 JOGmodeoperationinterface
```
```
（theleftoneistheEnglishinterface,andtherightoneistheChineseinterface)
```
```
TheJOGsettingprocessisshowninthefigurebelow.
```

Chapter 4 Control

```
Figure 4. 3 JOGcontrolprocess
```
**4. 1. 4 Rotationdirectionselection**

Bysetting"rotationdirectionselection(P 00. 01 )",therotationdirectionofthemotorcanbechangedwithoutchangingthe

polarityoftheinputcommand

☆Associatedfunctioncode:

```
Function
```
```
code
```
```
Name SettingRange Unit Function Type
```
```
Effective
```
```
time
```
```
Default
```
```
setting
```
```
P 00. 01
```
```
Directionof
rotatio
```
```
0 - - CCWdirectionisthe
forward
direction(counterclockwise)
```
```
1 - - CWdirectionistheforward
direction(clockwise)
```
-

```
Setthemotor
forward/reverse
```
```
Settingafter
motorstop
```
```
Re-power 0
```
**Note:**

Whentherotationdirectionselection(P 00. 01 )ischanged,theoutputpulseformoftheservodriverandthepositiveornegativeofthe

monitoringparameterswillnotchange.

**4. 1. 5 BrakeSetting**

Thebrakeisamechanismthatpreventstheservomotorshaftfrommovingwhentheservodriverisinanon-runningstate,

tokeepthemotorlockedinposition,sothatthemovingpartsofthemachinewillnotmoveduetoitsownweightor

externalforce.

```
Figure 4. 4 Brakeapplicationschematic
```
```
Note:
```
- Thebrakemechanismbuiltintotheservomotorisanon-energizedfixedspecialmechanism,whichcannotbeusedforbraking

purposes,andisonlyusedtokeeptheservomotorinastoppedstate.


Chapter 4 Control

- Thebrakecoilhasnopolarity.
- Aftertheservomotorstops,theservoenable(S-ON)shouldbeturnedoff.
- Whenthemotorwiththebuilt-inbrakerunning,thebrakemaymakeaclickingsound,butithasnoeffectonthefunction.
- Whenthebrakecoilisenergized(thebrakeisreleased),magneticfluxleakagemayoccurattheshaftendandotherparts.Be

carefulwhenusinginstrumentssuchasmagneticsensorsnearthemotor.

```
a ） Brakewiring
```
```
Theconnectionofthebrakeinputsignalhasnopolarity,andtheuserneedstopreparea 24 Vpowersupply.The
```
standardwiringexampleofthebrakesignalBKandthebrakepowersupplyisasfollows:

```
Figure 4. 5 BrakeWiringDiagram
```
**Note:**

```
●Thelengthofthemotorbrakecableneedstofullyconsiderthevoltagedropcausedbythecableresistance,andthebrakeneedsto
```
ensurethattheinputvoltageisatleast 21. 6 V.

```
●Itisbestnottosharethepowersupplywithotherelectricalappliances,soastopreventthevoltageorcurrentfromreducingdueto
```
theworkofotherelectricalappliancesandeventuallycausethebraketomalfunction.

```
●Whenconnectingtherelay,pleasebesuretoconnectthefreewheelingdiode(recommendedultra-fastrecoverydiode, 1 A, 1000 V
```
model:UF 1010 G),andthedirectioniscorrect,otherwisetheDOportmaybedamaged.

```
●Itisrecommendedtousecablesabove 0. 5 mm^2.
```
```
●BrakeistheHomealpowerconsumptioncomponent.Brakeswithdifferentpowersareinstalledaccordingtotheratedtorqueofthe
```
motor.Theoutputcurrentof 24 Vpowersupplyinsidetheservodrivercandirectlydrivethemotorlockanddrivecurrent 1 A.


Chapter 4 Control

```
b ） Brakesoftwaresetting
```
```
Foraservomotorwithabrake,thesecondDOterminaloftheservodrivemustbeconfiguredasfunction 11
```
(FunOUT. 11 :BK,brakeoutput),andtheeffectivelogicoftheDOterminalmustbedetermined.

```
☆Associatedfunctioncode:
```
```
Code Name FunctionName Function
```
```
FunOUT. 11 BK Brakeoutput
```
```
Invalid,thebrakepowerisdisconnected,thebrakeacts,andthemotoris
```
```
inaposition-lockedstate;
```
```
Valid,thepowerofthebrakeisturnedon,thebrakeisreleased,andthe
```
```
motorcanrotate.
```
**4. 1. 6 Brakesetting**

```
Whenthetorqueofthemotorisintheoppositedirectiontotherotationalspeed,theenergyistransmittedbacktothe
```
driverfromthemotorend,whichcausesthebusvoltagetoincrease.Whenitreachesthebrakingpoint,theenergycan

onlybeconsumedthroughthebrakingresistor.Atthistime,thebrakingenergymustbeconsumedaccordingtothe

brakingrequirements,otherwisetheservodrivewillbedamaged.DS 3 seriesdriversneedanexternalbrakingresistor,

andtheconnectionofthebrakingresistorisshowninFigure 4. 6.

```
Figure 4. 6 Wiringdiagramofbrakingresistor
```
☆Associatedfunctioncode:

```
Functio
```
```
ncode
```
```
Name SettingRange Unit Function Type
```
```
Effective
```
```
time
```
```
Model
```

Chapter 4 Control

#### P 0018

```
Energyconsumption
```
```
resistancesetting
```
```
0 - -Usebuilt-inpower
```
```
consumptionresistor.
```
```
1 - -Useexternal
```
```
power-consumingresistors
```
```
andcoolnaturally.
```
```
2 - -Useexternal
```
```
power-consumingresistors
```
```
andforceair-cooled.
```
```
3 - -Noenergy
```
```
consumptionresistor,
```
```
relyingoncapacitance
```
```
absorption.
```
#### 1 0

```
Effective
```
```
immediately
```
```
Settingafter
```
```
motorstop
```
#### PST

#### P 0019

```
Externalresistor
```
```
powercapacity
```
#### 1 ～ 65535 1 W

```
Model
```
```
parameters
```
```
Effective
```
```
immediately
```
```
Settingafter
```
```
motorstop
```
#### PST

#### P 0020

```
Externalresistance
```
```
value
```
#### 1 ～ 1000 1 Ω

```
Model
```
```
parameters
```
```
Effective
```
```
immediately
```
```
Settingafter
```
```
motorstop
```
#### PST

#### P 0021

```
Externalresistance
```
```
heatingtimeconstant
```
```
1000 ~ 65535 1 ms
```
```
Model
```
```
parameters
```
```
Effective
```
```
immediately
```
```
Settingafter
```
```
motorstop
```
#### PST

#### P 0022

```
Energyconsumption
```
```
brakingstartvoltage
```
#### 0 ~ 410 1 V

```
Model
```
```
parameters
```
```
Effective
```
```
immediately
```
```
Setwhen
```
```
running
```
#### PST

**4. 1. 7 Servooperation**

```
Whentheservodriverisintheenablestate,theservodriverisintherunningstateatthistime;butbecausethereis
```
nocommandinputatthistime,theservomotordoesnotrotateandisinthelockedstate.Afterthecommandisinput,the

servomotorrotates.


Chapter 4 Control

```
Table 4. 2 Servooperationinstructions
```
```
No. Description
```
```
1
```
```
Whenrunningforthefirsttime,setappropriatecommandstomakethemotorrotateatalowspeed,
```
```
andconfirmwhetherthemotorrotationiscorrect.
```
```
2
```
```
Observewhetherthedirectionofrotationofthemotoriscorrect.Ifitisfoundthatthedirectionofthe
```
```
motorisoppositetotheexpectedone,pleasechecktheinputcommandsignalandcommanddirection
```
```
settingsignal.
```
```
3
```
```
Iftherotationdirectionofthemotoriscorrect,youcanusethehostcomputersoftwaretoobservethe
```
```
actualspeedP 18. 01 ofthemotor,theaverageloadrateP 18. 02 andotherparameters.
```
```
4
```
```
Aftercheckingtheoperationstatusofthemotorabove,youcanadjusttherelevantparameterstomake
```
```
themotorworkintheexpectedworkingcondition.
```
```
5 RefertoChapter 5 "OperationPerformanceAdjustment"todebugtheservodriver.
```
**1** ） **PowerOnTimingChart**


Chapter 4 Control

**Note:**

```
* 1 :Theresettimeisdeterminedbytheestablishmenttimeofthemicroprocessor+ 5 Vpowersupply;
```
```
* 2 :Above 0 ms,itmeansthatthetimeisdeterminedbytheactualmomentwhenthemainpowersupplyisswitchedon;
```
```
* 3 :Whenthecontrolpowersupplyandthepowersupplyarepoweredonatthesametime,thistimeisthesameasthetimefromthe
```
completionofmicroprocessinginitializationtotheeffectivetimeofRdy;

```
* 4 :Forthedelaytimeoftheactionofthebrakemechanicalpart,pleaserefertotherelevantspecificationsofthemotor;
```
```
* 5 :WhenDOfunction 11 (FunOUT. 11 :BK)isnotallocated,P 00. 14 hasnoeffect.
```
```
2 ） ShutdownwhenservoenableOFF,warningorfaultoccurstimingdiagram
```
```
a ） ServoenableOFFNon-brake:coasttostop,keepfreerunningstate(P 00. 10 = 0 )
```

Chapter 4 Control

**b** ） **ServoenableOFFNon-brake:Stopatzerospeedandkeepinfreerunningstate(P 00. 10 = 1 )**

**c** ） **ServoenableOFFwithbrake:forcedtostopatzerospeed,keepfreestate**

```
Note:
```
* 1 :Forthedelaytimeoftheactionofthebrakemechanicalpart,pleaserefertotherelevantspecificationsofthemotor.

**d** ） **TypeIfaulty:freestop,keepfreerunningstate(P 00. 11 = 0 )**


Chapter 4 Control

**Note:**

* 1 ：WhenDOfunction 11 (FunOUT. 11 :BK)isnotassigned,P 00. 16 andP 00. 17 havenoeffect.

* 2 ：Forthedelaytimeoftheactionofthebrakecontactpart,pleaserefertotherelevantspecificationsofthemotor.

**e** ） **TypeIIfaulty**

**f** ） **Overtravelstopwarningnon-holdingbrake:freestop,keepfreerunningstate(P 00. 13 = 0 )**

**g** ） **Overtravel,brakestopwarning:stopatzerospeed,keeppositionlocked(P 00. 13 = 1 )**


Chapter 4 Control

**h** ） **Overtravelstopwarningnon-brake:stopatzerospeed,keepfreerunningstate(P 00. 13 = 2 )**

**i** ） **Overtravelstopwarningwithbrake:stopatzerospeed,keepfreerunningstate(P 00. 13 = 0 orP 00. 13 = 2 )**

**Note:**

* 1 ：Forthedelaytimeoftheactionofthebrakemechanicalpart,pleaserefertotherelevantspecificationsofthemotor.


Chapter 4 Control

**j** ） **Non-shutdownwarning** ：

Exceptforemergencybrake,forwardovertravelwarning,andreverseovertravelwarning,otherwarningshavenoeffect

onthecurrentstatusoftheservo,asshownbelow.

```
k ） Faultreset:
```
**Note** ：

* 1 ：DIfaultresetsignal(FunIN. 2 :ALM-RST)isvalidwhenedgechanges;

* 2 ：Forthedelaytimeoftheactionofthebrakecontactpart,pleaserefertotherelevantspecificationsofthemotor;

* 3 ：WhenDOfunction 11 (FunOUT. 11 :BK)isnotassigned,P 00. 14 hasnoeffect.


Chapter 4 Control

### 4 2 Positioncontrolmode.......................................................................................................................

```
4. 2. 1 FunctionalDiagramofPositionControl
```
```
Figure 4. 7 Positioncontroldiagram
```
```
Thepositionmodeisacommonworkingmodeoftheservodrive,anditsmainstepsareasfollows:
```
1. Correctlyconnectthepowersupplyoftheservomaincircuitandcontrolcircuit,aswellasthemotorpowercableand

encodercable.

2. Disconnectthemotorfromtheload,andconductaservoJOGtestrunthroughthehostcomputertoconfirmwhether

themotorcanrunnormally.

3. Configurepulsedirectioninput,pulsecommandinputandnecessaryDI/DOsignals,suchasservoenable,

positioningcompletion,etc.

4. Makerelevantsettingsofthepositionmode.SettheDI/DOusedaccordingtotheactualsituation,andrefertoGroup

P 02 forthefunctioncode.Inaddition,functionssuchasreturntoHomearesometimessetasneeded.

5. Enabletheservo,andcontroltherotationoftheservomotorthroughthepositioncommandissuedbythehost

computer.Firstmakethemotorrotateatalowspeed,andconfirmwhetherthedirectionofrotationandtheelectronicgear

ratioarenormal,andthenadjustthegain.


Chapter 4 Control

**4. 2. 2 Positioncontrolmoderelatedfunctioncodesetting**

```
Parametersettinginpositioncontrolmode,includingmodeselection,commandpulseform,electronicgearratio,
```
DI/DO,etc.

**4. 2. 2. 1 Positioncommandinputsetting**

**a)Sourceofpositioncommand**

```
SetthefunctioncodeP 04 - 00 = 0 ,thepositioncommandcomesfromthelow-speedpulsecommand,anditcanalso
```
besettoothervaluesaccordingtotheactualsituation.

```
Function
```
```
code
```
```
Name SettingRange Unit Default
setting
```
```
Type Effective
time
```
```
Mode
```
```
P 04 00
```
```
Mainposition
```
```
commandsource
```
```
0 - -lowspeedpulsecommand
```
```
1 - -reserved
```
```
2 - -stepamountgiven
```
```
4 - -multi-segmentposition
```
```
commandgiven
```
```
5 - -communicationgiven
```
```
6 - -reserved
```
```
1 0
```
```
Effective
```
```
immediately
```
```
Setting after
motorstop P
```
```
b)Positioncommanddirectionswitching
```
```
BysettingtheDIfunctionFunIN. 24 ,youcanuseDItocontrolthedirectionswitchingofthepositioncommandtomeet
```
thesituationwherethedirectionneedstobeswitched.

```
Code Name Functionname Settingrange Mark
```
```
FunIN. 24 POS_DIR Positioncommanddirection
```
```
Invalid:nocommutation
```
```
Valid:reversing
```
```
Therecommendedsettingforthelogic
```
```
selectionofrelevantterminalsis:edge
```
```
effective.
```
```
c)Pulsecommandformselection
```
```
SetthefunctioncodeP 04 - 21 toselecttheformoftheexternalpulsecommand,includingthreeformsof"direction+
```
pulse(positiveandnegativelogic)","orthogonalpulse"and"CW+CCW"


Chapter 4 Control

Function

code

```
Name SettingRange Unit
```
```
Default
```
```
setting
```
```
Type
```
```
Effective
time
```
```
Mode
```
P 04 21 Pulseshape

```
0 - -pulse+direction,positive
logic.(Defaults)
```
```
1 - -direction+pulse,negative
logic
```
```
2 - -Aphase+Bphaseorthogonal
pulse,positivelogic
```
```
3 - -Aphase+Bphaseorthogonal
pulse,negativelogic
```
```
4 - -CCW+CW,positivelogic
```
```
5 - -CCW+CW,negativelogic
```
```
1 0
```
```
Re-pow
er
```
```
Setting
aftermotor
stop
```
```
P
```
Theprinciplesofthethreepulsecommandformsareasfollows.

```
Table 4. 3 Principleofpulsecommandform
```
```
P 04 - 21 Polarity
```
```
Pulsecommand
```
```
form
```
```
Forward Reverse
```
#### 0

```
Positive
```
```
logic
```
```
Direction+pulse
```
#### 2

```
Positive
```
```
logic
```
```
ABphase
```
```
quadraturepulse
```

Chapter 4 Control

```
P 04 - 21 Polarity
```
```
Pulsecommand
```
```
form
```
```
Forward Reverse
```
#### 4

```
Positive
```
```
logic
```
#### CCW+CW

#### 1

```
Negative
```
```
logic
```
```
Direction+pulse
```
#### 3

```
Negative
```
```
logic
```
```
ABphase
```
```
quadraturepulse
```
#### 5

```
Negative
```
```
logic
```
#### CCW+CW

**d)Pulseinhibitinput**

BysettingtheDIfunctionFunIN. 12 ,thepulsecommandinputisprohibited.

```
Code Name
```
```
Function
```
```
name
```
```
Settingrange Mark
```
```
FunIN. 12 INHIBIT Pulseinhibit
```
```
Invalid:allowcommandpulseinput
```
```
Valid:inhibitcommandpulseinput
```
```
Positioncommandsareprohibited,
```
```
includinginternalandexternal
```
```
positioncommands.Thelogic
```
```
selectionofthecorresponding
```
```
terminalmustbesetas:levelisvalid.
```

Chapter 4 Control

```
4. 2. 2. 2 Electronicgearratiosetting
```
Settheelectronicgearratioaccordingtotheactualsituationofthemachineandthehostcomputer.

```
Functioncode Name SettingRange Min.Unit
```
```
Default
```
```
setting
```
```
Type Effectivetime Mode
```
#### P 04 05

```
Thenumberof
```
```
commandpulses
```
```
requiredforone
```
```
revolutionofthemotor
```
```
0 ~ 1048576 1 P/Rev 0 Re-power
```
```
Settingafter
```
```
motorstop
```
#### P

#### P 04 07

```
Electronicgear 1
```
```
molecule
```
#### 1 ~ 1073741824 1 4

```
Motor
resolution
```
```
Setwhen
running
```
#### P

#### P 04 09

```
Electronicgear 1
denominator
```
#### 1 ~ 1073741824 1 1 10000

```
Setwhen
running
```
#### P

#### P 04 11

```
electronicgear 2
molecule
```
#### 1 ~ 1073741824 1 4

```
Motor
resolution
```
```
Setwhen
running
```
#### P

#### P 04 13

```
Electronicgear 2
```
```
denominator
```
#### 1 ~ 1073741824 1 1 10000

```
Setwhen
running
```
#### P

Theschematicdiagramoftheelectronicgearratioisasfollows:

```
Figure 4. 8 Principleofelectronicgearratio
```
```
a） WhenP 04 - 05 = 0 ,themotorandtheloadareconnectedthroughthereductiongear,assumingthatthereduction
```
ratiobetweenthemotorshaftandthemechanicalsideoftheloadisn/m(themotorshaftrotatesmcircles,andtheload

shaftrotatesncircles),thecalculationoftheelectronicgearratioisasfollows.

```
electronicgearratio
```
```
B
A
```
```
=
```
```
P 04 − 07
P 04 − 09
```
```
=
```
```
encoderresolution
displacementoftheloadshaftforonerotation(commandunit)×
```
```
m
n
```
```
Thistypeofdriversupportsupto 2 setsofelectronicgearratios,andtheelectronicgearratioswitchingfunction
```
FunIN. 23 canbeusedtocompletethegearratioselection.

```
b) WhenP 04 - 05 ≠ 0 ，
```

Chapter 4 Control

```
electronicgearratio
```
```
B
A=
```
```
encoderresolution
P 04 − 05
```
```
Atthistime,thetwosetsofelectronicgearratioandelectronicgearswitchingfunctionareinvalid.
```
```
c) Theelectronicgearratiosettingrangeis
```
```
0. 001 ×encoderresolution
10000 <
```
```
B
A<
```
```
4000 ×encoderresolution
10000 ，otherwise,fault
```
```
Er. 49 (electronicgearratiosettingfault)willoccur.
```
**4. 2. 2. 3 Positiondeviationclearfunction**

Positiondeviation=(positioncommand-positionfeedback)(encoderunit)

Thepositiondeviationclearingfunctionmeansthatthedrivecanclearthepositiondeviationtozerowhencertain

conditionsaremet(P 04. 22 ).

```
☆Associatedfunctioncode:
```
```
Code Name Settingrange Unit Function Type Effectivetime
```
```
Default
setting
```
#### P 04. 22

```
Position
deviation
clearfunction
```
```
0 - -Clearpositiondeviation
pulsewhenfaultoccursor
servoOFF
```
```
1 - -Cleartheposition
deviation
```
```
pulseonlywhenafault
occurs
```
```
2 - -ClearedbyDIinput
```
```
function(PERR-CLR)
```
#### -

```
Settingthe
```
```
conditionsfor
```
```
clearingposition
```
```
deviation.
```
```
Setting
```
```
after
```
```
motorstop
```
```
Effective
```
```
immediately
```
#### 0

WhenP 04. 22 = 2 ,oneDIterminaloftheservodriveshouldbeconfiguredasfunction 5 (FunIN. 5 :PERR_CLR,clear

positiondeviation),anddeterminethevalidlogicoftheDIterminal.

```
☆Associatedfunctioncode:
```
```
Code Name Functionname Function
```
```
FunIN. 5
```
```
Pulsedeviationclear
```
```
function
```
```
Clearposition
```
```
deviation
```
```
Valid,clearpositiondeviation;
```
```
Invalid,noclearoperation.
```

Chapter 4 Control

```
Thesettingmethodisasfollows.
```
```
Table 4. 4 Positiondeviationclearsetting
```
```
Settingvalue Clearcondition Cleartime
```
#### P 04. 22 = 0

```
Clearthe position deviation when the
```
```
servoisOFForoccuringthetype 1 fault.
```
#### P 04. 22 = 1

```
Clearpositiondeviationpulsesonlywhen
```
```
afaultoccurs.
```
#### P 04. 22 = 2

```
Clearthepositiondeviationbythesignal
```
```
inputbyDI(function 5 ).
```
**4. 2. 2. 4 Positioningcomplete/approachfunction**

```
Thepositioningcompletionfunctionmeansthatthepositiondeviationmeetstheconditionssetbytheuser(P 04. 24 ),
```
anditcanbeconsideredthatthepositioningiscompletedinthepositioncontrolmode.Atthistime,theservodrivecan

outputapositioningcompletion(COIN)signal,andthehostcomputercanconfirmthatthepositioningoftheservodriveis


Chapter 4 Control

completedafterreceivingthissignal.

```
Itsfunctionalprincipleisshowninthefigurebelow:
```
```
Figure 4. 9 Positioningcompletionfunctiondescription
```
```
Whenthepositiondeviationsatisfiesthecondition(P 04. 23 ),theservodrivercanalsooutputthepositioningapproach
```
(NEAR)signal.Usually,thehostcomputercanreceivethepositioningapproachsignalbeforeconfirmingthatthe

positioningiscompleted,soastoprepareforthepositioningcompletionoperation.

```
Beforeusingthepositioningcompletion/approachfunction,settheoutputconditionsandthresholdsforpositioning
```
completion/approach.

```
☆ Associatedfunctioncode:
```
```
Code Name Settingrange Unit Function Type
```
```
Effective
```
```
time
```
```
Default
setting
```
```
P 04. 23
```
```
Conditionfor
positioning
completed
```
```
0 - -Outputwhentheabsolute - Settheconditionsfor
```
```
Setting
after
motor
```
```
Effective^0
```

Chapter 4 Control

```
/proximityoutput
positiondeviationvalueissmallerthan
```
```
thethresholdofpositioningcompleted/
```
```
proximity;
```
```
1 - -Outputwhentheabsolute
```
```
positiondeviationvalueissmallerthan
```
```
thethresholdofpositioningcompleted/
```
```
proximityandthefilteredposition
```
```
commandis 0 ;
```
```
2 - -Outputwhentheabsoluteposition
```
```
deviationvalueissmallerthanthe
```
```
thresholdofpositioningcompleted/
```
```
proximityandpositioncommandis 0.
```
```
positioningcompleted
```
```
(COIN)/proximity
```
```
(NEAR)tobevalid.
```
```
stop
immediately
```
P 04. 24

```
Thresholdof
```
```
positioning
```
```
completed
```
```
1 ~ 65535 1 P
```
```
Setthethresholdofthe
```
```
absolutevalueofthe
```
```
positiondeviationwhen
```
```
thepositioning
```
```
completed(COIN)is
```
```
valid
```
```
Setting
after
motor
stop
```
```
Effective
```
```
immediately
```
```
According
```
```
toencode
```
```
type
```
P 04. 25

```
Thresholdof
positioning
proximity
```
```
1 ~ 65535 1 P
```
```
Setthethresholdofthe
```
```
absolutevalueofthe
```
```
positiondeviationwhen
```
```
thepositioningproximity
```
```
(NEAR)isvalid
```
```
Setting
after
motor
stop
```
```
Effective
```
```
immediately
```
```
65535
```
```
Note:
```
- Thethresholdofpositioningproximity(P 04. 25 )generallyneedstobegreaterthanthepositioningcompletedthreshold(P 04. 24 ).


Chapter 4 Control

- Thepositioningcompletionthreshold(P 04. 24 )onlyreflectsthethresholdoftheabsolutevalueofthepositiondeviationwhenthe

positioningcompletedisvalid,andhasnothingtodowiththepositioningaccuracy.

- Ifthespeedfeedforwardgain(P 07. 20 )issettoolargeorrunsatlowspeed,theabsolutevalueofthepositiondeviationwillbesmall.

IftheP 04. 24 issettoolarge,thepositioningcompletedwillalwaysbevalid.Therefore,inordertoimprovetheeffectivenessofpositioning

completed,pleasereducethesettingvalueofP 04. 24

- Whenthepositioningcompletedthreshold(P 04. 24 )issmallandthepositiondeviationissmall,theoutputconditionsofthe

positioningcompleted/proximitysignalcanbechangedbysettingP 04. 23.

- Whentheservoenable(S-ON)isinvalid,theoutputofthepositioningcompletedsignal(COIN)andpositioningproximitysignal

(NEAR)isinvalid.

```
Whenusingthepositioningcompletedandpositioningproximityfunctions,thetwoDOterminalsoftheservodriver
```
shouldbeconfiguredasDOfunction 7 (FunOUT. 7 :COIN,positioningcompleted)andDOfunction 8 (FunOUT. 8 :NEAR,

positioningproximity),anddeterminethevalidlogicofthecorrespondingDOterminal.

```
☆Associatedfunctioncode:
```
```
Code Name FunctionName Function
```
```
FunOut. 7 COIN Positioncompleted
```
```
Valid, inthepositioncontrolmode,theabsolutevalueofthe
```
```
positiondeviationmeetsthesettingconditionsofP 04. 24 ,
```
```
indicatingthattheservopositioningiscompleted.
```
```
Invalid, inpositioncontrolmode,theservoisintheprocess
```
```
ofcompletingpositioning.
```
```
FunOut. 8 NEAR Positionproximity
```
```
Valid, inthepositioncontrolmode,theabsolutevalueofthe
```
```
positiondeviationmeetsthesettingconditionsofP 04. 25 ,
```
```
indicatingthattheservopositioningisclose.
```
```
Invalid, inthepositioncontrolmode,theservoisinthe
```
```
processofpositioningandproximity.
```

Chapter 4 Control

```
4. 2. 2. 5 InterruptPositioning
```
```
1 ） Description
```
Inthepositioncontrolmode,ifinterruptpositioningistriggeredwhentheS-ONsignalisactive,the

servodriveabortscurrentoperationandturnstoexecutingthepresetpositionreference,whichistosay,

afterinterruptpositioningistriggered,themotorexecutesthepositionreferencetriggeredbyinterrupt

positioning,withmotordirectionunchanged.

Wheninterruptpositioningisinprogress,theservodrivedoesnotrespondtoanyotherinternal/

externalpositionreferences(includinganotherinterruptpositioningcommand).Inthiscase,theinput

position reference counter (P 18. 15 ) counts the interrupt positioning reference only.After interrupt

positioningisdone,theservodrivemayormaynotrespondtootherpositionreferencesdependingon

the setpoint of P 16. 06 (Interruptpositioningcancel signal). Thepositionreferencesreceivedduring

interruptpositioningwillbeabandoned.

After interrupt positioningisdone,theservo driveoutputs theinterruptpositioningcompleted

(FunOUT. 18 ：XINT_DONE) signalandpositioningcompleted(FunOUT. 7 ：COIN)signal,whilethehost

controller,uponreceivingXINT_DONEsignal,acknowledgesinterruptpositioningisdone.TheXINT_DONE

signaloutputisnotrelatedtotheS-ONsignalorthelogicofDI 9.

Interruptpositioningiseffectiveonlywhenthefollowingconditionsaremet:

●Themotorspeedishigherthanorequalto 10 RPMbeforeinterruptpositioningistriggered,orthe

setpoints ofP 16. 03 (Constantoperatingspeedininterruptpositioning) andP 16. 01 (Displacementof

interruptpositioning)arenot 0.

●TheDIassignedwithFunIN. 31 (Interruptpositioninginhibited)isnotusedorthelogicofthisDIis

inactive.

```
2 ） Parameterssetting
```
```
☆Relatedparameters：
```
```
Code Name Settingrange Unit Function Type Effectivetime Default
```
P 16. 00

```
Interruptpositioning
```
```
selection
```
```
0 - -Disable
```
```
1 - -Enable
```
-

```
Usedto
```
```
enable/disable
```
```
interruptpositioning.
```
```
Settingafter
motorstop Re-power^0
```

Chapter 4 Control

P 16. 01

```
Displacementof
```
```
interruptpositioning
```
```
0 ~ 1073741824
```
```
Reference
```
```
unit
```
```
Definesthe
```
```
displacementof
```
```
interruptpositioning
```
```
Setwhen
running
```
```
Effective
```
```
immediately
```
```
10000
```
P 16. 03

```
Constantoperating
```
```
speedininterrupt
```
```
positioning
```
```
0 ~ 6000 rpm
```
```
Definesthemaximum
```
```
motorspeedin
```
```
interruptpositioning,
```
```
whichisunrelatedto
```
```
theelectronicgear
```
```
ratio.
```
```
Setwhen
running
```
```
Effective
```
```
immediately
```
```
200
```
```
P 16. 04
```
```
Accelerationtimeof
```
```
interruptpositioning
```
```
0 ~ 1000 ms
```
```
Definesthetimefor
```
```
themotorto
```
```
acceleratefrom 0
```
```
RPMto 1000 RPM.
```
```
Setwhen
running
```
```
Effective
```
```
immediately
```
```
10
```
```
P 16. 05
```
```
Decelerationtimeof
```
```
interruptpositioning
```
```
0 ~ 1000 ms
```
```
Definesthetimefor
```
```
themotorto
```
```
acceleratefrom 0
```
```
RPMto 1000 RPM
```
```
Setwhen
running
```
```
Effective
```
```
immediately
```
```
10
```

Chapter 4 Control

P 16. 06

```
Interruptpositioning
```
```
cancelsignalEnable
```
```
0 —Disable
```
```
1 —Enable
```
-

```
Definesthecondition
```
```
forrespondingto
```
```
otherposition
```
```
referencesafter
```
```
interruptpositioningis
```
```
done.WhenP 16. 06 is
```
```
setto 1 ,interrupt
```
```
positioningmustbe
```
```
canceledbyaDI
```
```
assignedwith
```
```
FunIN. 30 (Interrupt
```
```
positioningcancel.
```
```
Setwhen
running
```
```
Effective
```
```
immediately
```
```
1
```
```
☆ Relatedfunctioncode：
```
```
Code Name Functionname Function
```
```
FunIN. 30 XINT_FINISH
```
```
Interruptpositioning
```
```
cancel
```
```
Active: Theinterruptpositioningstateisunlocked,whichmeanstheservo
```
```
drivecanrespondtootherpositionreferences.
```
```
Inactive: Theinterruptpositioningstateislocked,whichmeanstheservo
```
```
drivecannotrespondtootherpositionreferences.
```
```
FunIN. 31 XINT_DISABLE
```
```
Interruptpositioning
```
```
inhibit
```
```
Active: Interruptpositioninginhibited;
```
```
Inactive: Interruptpositioningpermitted
```
```
FunOut. 18 XINT_DONE
```
```
Interruptpositioning
```
```
completed
```
```
Active: Interruptpositioningcompletedduringpositioncontrol.
```
```
Inactive: Interruptpositioningnotcompletedduringpositioncontrol.
```

Chapter 4 Control

Theconstantoperatingspeedforinterruptpositioningisshowninthefollowingfigure.

```
Figure 4. 10 Motoroperatingcurveduringinterruptpositioning
```
```
Table 4. 4 ActivelogicofDI 8 duringinterruptpositioning
```
#### P 16. 03

```
MotorSpeedbeforeTriggeringInterrupt
```
```
Positioning
```
```
InterruptPositioning
```
```
ConstantOperatingSpeedinInterrupt
```
```
Positioning
```
#### 0

```
< 10 Inactive -
```
```
≥ 10 Active
```
```
MotorSpeedbeforeTriggeringInterrupt
```
```
Positioning
```
```
1 ~ 6000 - Active P 16. 03
```
**4. 2. 2. 6 HomingFunction**

**Note:**

Thehomingtriggersignalisdisabledwheninterruptpositioningormulti-positionreferenceisinprogress.

```
1 ） Descriptionofthehomingfunction
```
```
Home(ormechanicalhome):IndicatesthepositionofthehomeswitchormotorZsignaldependingonthesettingof
```
P 16 - 09 (Homingmode).

```
Zeroposition(ortargetpoint):Indicatesthesumofthehomeplustheoffset(definedbyP 16 - 14 ).WhenP 16 - 14
```
(Homeoffset)issetto 0 ,zeropositionsharesthesamepositionwiththehome.


Chapter 4 Control

```
Inthepositioncontrolmode,whenhomingistriggeredaftertheS-ONsignalisactivated,themotorstartssearching
```
forthezeropoint.Whenhomingisinprogress,theservodrivedoesnotrespondtootherpositionreferences(including

anotherhomingtriggersignal)untilhomingisdone.

```
Homing:Theservodrive,uponreceivingthehomingtriggersignal,locatestherelativepositionofthemotorshaftand
```
themechanicalhomebasedonthepre-setmechanicalhomeandstartssearchingforthehomefirst.Thentheservodrive

executestheoffsetbasedonthehome,afterwhichitreachesthezeropoint.Thehomingmodeusuallyappliesininitial

searchofthezeropoint.

```
Afterthehomingfunctionisdoneexecuting,themotorpositionabsolutevalue(P 18 - 07 )isconsistentwiththehome
```
offset(P 16 - 14 ).

```
Afterhomingisdone,theservodriveoutputsthehomingcompletedsignal(FunOUT. 19 ：HomeAttain).Thehost
```
controller,uponreceivingthesignal,acknowledgesthehomingisdone.HomeAttainisindependentofthecontrolmodeor

theoperatingstateoftheservodrive

```
Table 4. 5 Homingtriggermode
```
```
Mode Homingtriggermode(P 16 - 08 )
```
```
HomingDirection,
```
```
DecelerationPoint,
```
```
Home
```
```
Trigger
```
```
Signal
```
```
TotalMotorDisplacement
```
```
Homing 0 :Disabled - - -
```

Chapter 4 Control

```
1 :Homingenabledthrough
```
```
theHomingStartsignalinputted
```
```
fromDI
```
```
Determinedby
```
```
P 16 - 09
```
```
HomingStart
```
```
signal
```
```
Determinedbythehomecoordinate
```
```
andoffsetdisplacement.
```
```
2 :Homingenabledimmediately
```
```
uponpower-on
```
#### S-ON

```
3 :Homingexecutedimmediately S-ON
```
```
4 :Currentpositionasthehome S-ON
```
```
5 :DI-triggeredpositionasthe
```
```
home
```
#### S-ON

```
6 :Hostcontrollerhoming S-ON
```
```
Note:
```
```
Theelectronicgearing,themovingaveragefilterandthelow-passfilterareinactiveduringhoming.
```
```
2 ） Homing
```
```
Note:
```
- Setmechanicallimitswitchesbeforeenablinghoming.Forhominguponhit-and-stop,settheoffsettoavaluewithinthetravel

rangetopreventthemachinefromcollisionduetohigh-speedoperationduringhoming.

- Whenthemotorhitsthelimitswitchduringhoming,theservodrivereportsEr. 86 (Forwardovertravel)orEr. 87 (Reverse

overtravel),andthenitreturnsautomaticallyandcontinuestosearchforthemechanicalhome.Seethefollowingexamplesforthe

homingmode:

**HomingMode 1 :Homingbyreverselimitswitchandzeropositionpulsesignal** （ **0 x 6098 = 1** ）

**1** ） **Duringhoming,thereverselimitswitchsignalisinactive.**


Chapter 4 Control

**Description:** Whenhoming,thereverselimitswitchsignalisinvalid.Themotorsearchesforthe

reverselimitswitchsignalatahigherspeeduntiltherisingedgeofthelimitswitchsignalisdetected,

thendecelerates,reverses,andrunsforwardatalowerspeed.Whenthefallingedgeofthereverselimit

signalisdetected,themotorstartstosearchfortheZsignal,andstopsrunningaftertheZsignalis

found.

**2** ） **Whenhoming,thereverselimitswitchsignalisvalid**

**Description:** Whenhoming,thereverselimitswitchsignalisvalid.Themotordirectlyrunsforwardata

lowersetspeed.Whenthefallingedgeofthereverselimitsignalisdetected,themotorstartstosearch

fortheZsignal,andstopsrunningaftertheZsignalisfound.

**Homingmode 2 :hoimgwithpositivelimitswitchandzeropulsesignal( 0 x 6098 = 2 )**

**1** ） **Whenhoming,thepositivelimitswitchsignalisinvalid**


Chapter 4 Control

**Description:** Whenhoming,thepositivelimitswitchsignalisinvalid.Themotorsearchesforthe

positivelimitswitchsignalatahigherspeedintheforwarddirectionuntiltherisingedgeofthelimit

switchsignalisdetected,thendecelerates,reverses,andrunsinthereversedirectionatalowerspeed.

Whenthefallingedgeofthepositivelimitsignalisdetected,themotorstartstosearchfortheZsignal,

andstopsrunningaftertheZsignalisfound.

**2** ） **Whenhoming,thepositivelimitswitchsignalisvalid**

**Description:** Whenhoming,thepositivelimitswitchsignalisvalid.Themotordirectlyrunsinthe

reversedirectionatalowersetspeed.Whenthefallingedgeofthepositivelimitsignalisdetected,the

motorstartstosearchfortheZsignalofthemotor,andstopsrunningaftertheZsignalisfound.

**Homingmode 3 :homingwithHomeswitchandzeropulsesignal( 0 x 6098 = 3 )**

**1** ） **Whenhoming,theHomeswitchsignalisinvalid**


Chapter 4 Control

**Description:** Whenhoming,theHomeswitchsignalisinvalid.ThemotorsearchesfortheHome

switchsignalatahigherspeedintheforwarddirectionuntiltherisingedgeoftheHomeswitchsignalis

detected,thendecelerates,reverses,andrunsinthereversedirectionatalowerspeed.Whenthe

fallingedgeoftheHomesignalisdetected,themotorstartstosearchfortheZsignal,andstopsrunning

aftertheZsignalisfound.

**2** ） **Whenhoming,thehomeswitchsignalisvalid**

**Description:** Whenhoming,theHomeswitchsignalisvalid.Themotordirectlyrunsinthereverse

directionatalowersetspeed.WhenthefallingedgeoftheHomeswitchsignalisdetected,themotor

startstosearchfortheZsignal,andstopsrunningaftertheZsignalisfound.

**Homingmode 4 :homingwithHomeswitchandzeropulsesignal( 0 x 6098 = 4 )**

**1** ） **Whenhoming,theHomeswitchsignalisinvalid**


Chapter 4 Control

**Description:** Whenhoming,theHomeswitchsignalisinvalid.ThemotorsearchesfortheHome

switchsignalintheforwarddirectionatalowerspeeduntiltherisingedgeoftheHomeswitchsignalis

detected.ThenitstartstosearchforthemotorZsignalandstopsrunningaftertheZsignalisfound.

**2** ） **Whenhoming,theHomeswitchsignalisvalid**

**Description:** Whenhoming,theHomeswitchsignalisvalid.Themotordirectlyrunsinthereverse

direction ata higher setspeed. When the falling edge ofthe Homeswitchsignal isdetected,it

decelerates,reverses,andrunsforwardatalowerspeed.WhentherisingedgeoftheHomesignalis

detected,itstartstosearchforthemotorZsignalandstopsrunningaftertheZsignalisfound.

**Homingmode 5 :homingwithHomeswitchandzeropulsesignal( 0 x 6098 = 5 )**

**1** ） **Whenhoming,theHomeswitchsignalisinvalid**


Chapter 4 Control

**Description:** Whenhoming,theHomeswitchsignalisinvalid.ThemotorsearchesfortheHome

switchsignalinreverseatahigherspeed.WhentherisingedgeoftheHomeswitchsignalisdetected,it

decelerates,reverses,andrunsforwardatalowersetspeed.WhenthefallingedgeoftheHomeswitch

signalisdetected,itstartstosearchforthemotorZsignalandstopsrunningaftertheZsignalisfound.

**2** ） **Whenhoming,theHomeswitchsignalisvalid**

**Description:** Whenhoming,theHomeswitchsignalisvalid.Themotordirectlyrunsforwardata

lowersetspeed.WhenthefallingedgeoftheHomeswitchsignalisdetected,themotorstartstosearch

fortheZsignal,andstopsrunningaftertheZsignalisfound.

**Homingmode 6 :homingwithHomeswitchandzeropulsesignal( 0 x 6098 = 6 )**

**1** ） **Whenhoming,theHomeswitchsignalisinvalid**


Chapter 4 Control

**Description:** Whenhoming,theHomeswitchsignalisinvalid.ThemotorsearchesfortheHome

switchsignalinreverseatalowerspeeduntiltherisingedgeoftheHomeswitchsignalisdetected.

ThenitstartssearchingforthemotorZsignalandstopsrunningaftertheZsignalisfound.

**2** ） **Whenhoming,theHomeswitchsignalisvalid**

**Description:** Whenhoming,theHomeswitchsignalisvalid.Themotordirectlyrunsforwardata

highersetspeed.WhenthefallingedgeoftheHomeswitchsignalisdetected,itdecelerates,reverses,

andrunsinthereversedirectionatalowerspeed.WhentherisingedgeoftheHomesignalisdetected,

itstartstosearchforthemotorZsignal(fallingedge),andstopsrunningaftertheZsignalisfound.

**Homingmode 7 :homingwithHomeswitch,positivelimitsignalandzeropulsesignal( 0 x 6098 =**

**7 )**

**1 ) TheHomesignalisinvalidwhenstartingthehoming,andthepositivelimitswitch isnot**

**encountered.**


Chapter 4 Control

**Description:** Whenstartinghoming,theHomesignalisinvalid.Themotorrunsforwardatahigher

speedanddoesnotencountertheforwardlimitsignal.WhentherisingedgeoftheHomesignalis

detected,itdecelerates,reverses,andrunsinthereversedirectionatalowersetspeed.Whenthe

fallingedgeoftheHomesignalisencountered,itstartstosearchforthemotorZsignalandstops

runningaftertheZsignalisfound.

**2 ) TheHomesignalisinvalidwhenstartingthehoming,andthepositivelimitswitchis**

**encountered.**

**Description:** Whenhoming,thehomesignalisinvalid.Themotorrunsforwardatahighspeed.

Whenencounteringtheforwardlimitsignal,itdeceleratesandrunsinthereversedirectionatahigh

speed.Afterdetectingtherisingedgeofthehomesignal,itdeceleratesandcontinuestoruninthe

reversedirectionatalowersetspeed.Whenencounteringthefallingedgeofthehomesignal,itstartsto


Chapter 4 Control

searchforthemotorZsignalandstopsrunningafterfindingtheZsignal.

**3 ) Thehomesignalisvalidwhenhoming**

**Description:** Whenstartingthehoming,thehomesignalisvalid.Themotordirectlyrunsinthe

reversedirectionatalowersetspeed.Whenthefallingedgeofthehomesignalisdetected,itstartsto

searchforthemotorZsignalandstopsrunningaftertheZsignalisfound.

**Homingmode 8 :homingwithhomeswitch,positivelimitsignalandzeropulsesignal( 0 x 6098 =**

**8 )**

**1 ) Thehomesignalisinvalid whenstarting thehoming,andthepositivelimitswitch isnot**

**encountered.**

**Description:** Whenhoming,thehomesignalisinvalid.Themotorrunsforwardatahigherspeed

anddoesnotencountertheforwardlimitsignal.Whentherisingedgeofthehomesignalisdetected,it

decelerates,reverses,andrunsinthereversedirectionatalowersetspeed.Whenthefallingedgeof

thehomesignalisencountered,itreversesandrunsforwardatalowersetspeed.Whentherisingedge


Chapter 4 Control

ofthehomesignalisdetected,itstartstosearchforthemotorZsignalandstopsrunningaftertheZ

signalisfound.

**2 ) Thehomesignalisinvalidwhenstartingthehoming,andthepositivelimitswitchis**

**encountered.**

**Description:** Whenhoming,thehomesignalisinvalid.Themotorrunsforwardatahighspeed.

Whenencounteringtheforwardlimitsignal,itdeceleratesandrunsinthereversedirectionatahigh

speed.Afterdetectingtherisingedgeofthehomesignal,itdeceleratesandcontinuestoruninthe

reversedirectionatalower setspeed.Whenencountering thefallingedge of thehomesignal,it

reversesandrunsforwardatalowersetspeed.Whendetectingtherisingedgeofthehomesignal,it

startstosearchforthemotorZsignalandstopsrunningafterfindingtheZsignal.

**3 ) Thehomesignalisvalidwhenhoming**


Chapter 4 Control

**Description:** Whenstartingthehoming,thehomesignalisvalid.Themotordirectlyrunsinthereverse

directionatalowersetspeed.Whenthefallingedgeofthehomesignalisdetected,itreversesandruns

forwardatalowersetspeed.Whentherisingedgeofthehomesignalisdetected,itstartstosearchfor

themotorZsignalandstopsrunningaftertheZsignalisfound.

**Homingmode 9 :homingwithhomeswitch,positivelimitsignalandzeropulsesignal( 0 x 6098 =**

**9 )**

**1 ) Thehomesignalisinvalid whenstarting thehoming,andthepositivelimitswitch isnot**

**encountered.**

**Description:** Whenhoming,thehomesignalisinvalid.Themotorrunsforwardatahigherspeed

anddoesnotencountertheforwardlimitsignal.Whentherisingedgeofthehomesignalisdetected,it

deceleratesandcontinuestorunforwardatalowersetspeed.Whenthefallingedgeofthehomesignal


Chapter 4 Control

isencountered,itreversesandrunsinthereversedirectionatalowersetspeed.Whentherisingedge

ofthehomesignalisdetected,itstartstosearchforthemotorZsignalandstopsrunningaftertheZ

signalisfound.

**2 )Thehomesignalisinvalidwhenstartingthehoming,andthepositivelimitswitchis**

**encountered.**

**Description:** Whenhoming,thehomesignalisinvalid.Themotorrunsforwardatahigherspeed.

Whenencounteringtheforwardlimitsignal,itdeceleratesandrunsinthereversedirectionatahigh

speed.Afterdetectingtherisingedgeofthehomesignal,itdecelerates,reverses,andrunsforwardata

lowersetspeed.Whenencounteringthefallingedgeofthehomesignal,itreversesandcontinuestorun

inthereversedirectionatalowersetspeed.Whentherisingedgeofthehomesignalisdetected,it

startstosearchforthemotorZsignalandstopsrunningafterfindingtheZsignal.

**3 )Thehomesignalisvalidwhenhoming**


Chapter 4 Control

**Description:** Whenstartingthehoming,thehomesignalisvalid.Themotordirectlyrunsforwardat

alowersetspeed.Whenthefallingedgeofthehomesignalisdetected,itreversesandcontinuestorun

inthereversedirectionatalowersetspeed.Whentherisingedgeofthehomesignalisdetected,it

startstosearchforthemotorZsignalandstopsrunningaftertheZsignalisfound.

**Homing mode 10 :homingwithhomeswitch,positivelimit signaland zero pulsesignal**

**( 0 x 6098 = 10 )**

**1** ） **Thehomesignalisinvalidwhenstartingthehoming,andthepositivelimitswitchisnot**

**encountered.**

**Description:** Whenhoming,thehomesignalisinvalid.Themotorrunsforwardatahigherspeed

anddoesnotencountertheforwardlimitsignal.Whentherisingedgeofthehomesignalisdetected,it


Chapter 4 Control

deceleratesandcontinuestorunforwardatalowersetspeed.Whenthefallingedgeofthehomesignal

isencountered,itstartstosearchforthemotorZsignalandstopsrunningaftertheZsignalisfound.

**2 ) Thehomesignalisinvalidwhenstartingthehoming,andthepositivelimitswitchis**

**encountered.**

**Description:** Whenhoming,thehomesignalisinvalid.Themotorrunsforwardatahighspeed.

Whenencounteringtheforwardlimitsignal,itdeceleratesandrunsinthereversedirectionatahigh

speed.Afterdetectingtherisingedgeofthehomesignal,itdecelerates,reverses,andcontinuestorun

forwardatalowersetspeed.Whenencounteringthefallingedgeofthehomesignal,itstartstosearch

forthemotorZsignalandstopsrunningafterfindingtheZsignal.

**3 ) Thehomesignalisvalidwhenhoming**

**Description:** Whenstartingthehoming,thehomesignalisvalid.Themotordirectlyrunsforwardat

alowersetspeed.Whenthefallingedgeofthehomesignalisdetected,itstartstosearchforthemotor


Chapter 4 Control

ZsignalandstopsrunningaftertheZsignalisfound.

**Homing mode 11 :homing with homeswitch,reverselimit signal and zeropulsesignal**

**( 0 x 6098 = 11 )**

**1 )Thehomesignalisinvalid when startingthehoming,andnoreverselimit switch is**

**encountered.**

**Description:** Whenhoming,thehomesignalisinvalid.Themotorrunsinthereversedirectionata

higherspeedanddoesnotencounterthereverselimitsignal.Whentherisingedgeofthehomesignalis

detected,itdecelerates,reverses,andrunsforwardatalowersetspeed.Whenthefallingedgeofthe

homesignalisencountered,itstartstosearchforthemotorZsignalandstopsrunningaftertheZsignal

isfound.

**2** ） **Thehomesignalisinvalidwhenhoming,andthereverselimitswitchisencountered.**


Chapter 4 Control

**Description:** Whenhoming,thehomesignalisinvalid.Themotorrunsinthereversedirectionata

higherspeed.Whenencounteringthereverselimitsignal,itdeceleratesandreverses,andrunsinthe

forwarddirectionatahighersetspeed.Afterdetectingtherisingedgeofthehomesignal,itdecelerates

andcontinuestorunintheforwarddirectionatalowersetspeed.Whenencounteringthefallingedgeof

thehomesignal,itstartstosearchforthemotorZsignalandstopsrunningafterfindingtheZsignal.

**3 )Thehomesignalisvalidwhenhoming**

**Description:** Whenstartingthehoming,thehomesignalisvalid.Themotordirectlyrunsforwardat

alowersetspeed.Whenthefallingedgeofthehomesignalisdetected,itstartstosearchforthemotor

ZsignalandstopsrunningaftertheZsignalisfound.


Chapter 4 Control

**Homing mode 12 : homing withhome switch,reverselimit signal and zeropulsesignal**

**( 0 x 6098 = 12 )**

**1 ) Thehomesignalisinvalidwhen startingthehoming,andnoreverselimit switch is**

**encountered.**

**Description:** Whenhoming,thehomesignalisinvalid.Themotorrunsinthereversedirectionata

higherspeedanddoesnotencounterthereverselimitsignal.Whentherisingedgeofthehomesignalis

detected,itdecelerates,reverses,andrunsforwardatalowersetspeed.Whenthefallingedgeofthe

homesignalisencountered,itreversesagainandrunsinthereversedirectionatalowersetspeed.

Whentherisingedgeofthehomesignalisdetected,itstartstosearchforthemotorZsignalandstops

runningaftertheZsignalisfound.

**2 ) Thehomesignalisinvalidwhenhoming,andthereverselimitswitchisencountered.**


Chapter 4 Control

**Description:** Whenhoming,thehomesignalisinvalid.Themotorrunsinthereversedirectionata

higherspeed.Whenencounteringthereverselimitsignal,itdecelerates,reverses,andrunsforwardata

higherspeed.Afterdetectingtherisingedgeofthehomesignal,itdeceleratesandcontinuestorun

forwardatalowersetspeed.Whenencounteringthefallingedgeofthehomesignal,itdecelerates,

reverses,andrunsinthereversedirectionatalowersetspeed.Whendetectingtherisingedgeofthe

homesignal,itstartstosearchforthemotorZsignalandstopsrunningafterfindingtheZsignal.

**3 ) Thehomesignalisvalidwhenhoming**

**Description:** Whenstartingfromzero,thehomesignalisvalid.Themotordirectlyrunsforwardata

lowersetspeed.Whenthefallingedgeofthehomesignalisdetected,itdecelerates,reverses,andruns

inthereversedirectionatalowersetspeed.Whentherisingedgeofthehomesignalisdetected,it

startstosearchforthemotorZsignalandstopsrunningaftertheZsignalisfound.


Chapter 4 Control

**Homing mode 13 : homing withhome switch,reverselimit signal and zeropulsesignal**

**( 0 x 6098 = 13 )**

**1 ) Thehomesignalisinvalidwhen startingthehoming,andnoreverselimit switch is**

**encountered.**

**Description:** Whenhoming,thehomesignalisinvalid.Themotorrunsinthereversedirectionata

higherspeedanddoesnotencounterthereverselimitsignal.Whentherisingedgeofthehomesignalis

detected,itdeceleratesandcontinuestoruninthereversedirectionatalowersetspeed.Whenthe

fallingedgeofthehomesignalisencountered,itreversesandrunsforwardatalowersetspeed.When

therisingedgeofthehomesignalisdetected,itstartstosearchforthemotorZsignalandstopsrunning

aftertheZsignalisfound.

**2 ) Thehomesignalisinvalidwhenhoming,andthereverselimitswitchisencountered.**


Chapter 4 Control

**Description:** Whenhoming,thehomesignalisinvalid.Themotorrunsinthereversedirectionata

higherspeed.Whenencounteringthereverselimitsignal,itdecelerates,reverses,andrunsforwardata

highersetspeed.Afterdetectingtherisingedgeofthehomesignal,itdecelerates,reverses,andrunsin

thereversedirectionatalowersetspeed.Whenencounteringthefallingedgeofthehomesignal,it

deceleratesagainandreverses,andrunsforwardatalowersetspeed.Whentherisingedgeofthe

homesignalisdetected,itstartstosearchforthemotorZsignalandstopsrunningaftertheZsignalis

found.

**3 ) Thehomesignalisvalidwhenhoming**

**Description:** Whenstartingthehoming,thehomesignalisvalid.Themotordirectlyrunsinthe

reversedirection at a lower set speed.Whenthefalling edge of thehomesignal isdetected,it

deceleratesandreverses,andrunsforwardatalowersetspeed.Whentherisingedgeofthehome


Chapter 4 Control

signalisdetected,itstartstosearchforthemotorZsignal,andstopsrunningaftertheZsignalisfound.

**Homing mode 14 : homing withhome switch,reverselimit signal and zeropulsesignal**

**( 0 x 6098 = 14 )**

**1 ) Thehomesignalisinvalidwhen startingthehoming,andnoreverselimit switch is**

**encountered.**

**Description:** Whenhoming,thehomesignalisinvalid.Themotorrunsinthereversedirectionata

higherspeedanddoesnotencounterthereverselimitsignal.Whentherisingedgeofthehomesignalis

detected,itdeceleratesandcontinuestoruninthereversedirectionatalowersetspeed.Whenthe

fallingedgeofthehomesignalisencountered,itstartstosearchforthemotorZsignalandstops

runningaftertheZsignalisfound.

**2 ) Thehomesignalisinvalidwhenhoming,andthereverselimitswitchisencountered.**


Chapter 4 Control

**Description:** Whenhoming,thehomesignalisinvalid.Themotorrunsinthereversedirectionata

higherspeed.Whenencounteringthereverselimitsignal,itdecelerates,reverses,andrunsforwardata

highersetspeed.Afterdetectingtherisingedgeofthehomesignal,itdecelerates,reverses,andrunsin

thereversedirectionatalowersetspeed.Whenencounteringthefallingedgeofthehomesignal,it

startstosearchforthemotorZsignalandstopsrunningafterfindingtheZsignal.

**3 ) Thehomesignalisvalidwhenhoming**

**Description:** Whenstartingthehoming,thehomesignalisvalid.Themotordirectlyrunsinthe

reversedirectionatalowersetspeed.Whenthefallingedgeofthehomesignalisdetected,itstartsto

searchforthemotorZsignalandstopsrunningaftertheZsignalisfound.

**Homingmethod 15 , 16 :**

**Reserve**


Chapter 4 Control

**Homingmode 17 :Homingwithreverselimitsignal( 6098 = 17 )**

**1 ) Whenhoming,thereverselimitsignalisinvalid.**

**Description:** Whenstartingfromzero,thereverselimitsignalisinvalid.Themotorrunsinthe

reversedirectionatahighersetspeed.Whentherisingedgeofthereverselimitsignalisdetected,it

deceleratesandreverses,andrunsforwardatalowersetspeed.Whenthefallingedgeofthereverse

limitsignalisdetected,itstopsrunning.

**2 ) Whenhomingandstarting,thereverselimitsignalisvalid**

**Description:** Whenstartingfromzero,thereverselimitsignalisvalid.Themotordirectlyruns

forwardatalowersetspeedandstopswhenthefallingedgeofthereverselimitsignalisdetected.

**Homingmode 18 :Homingwithpositivelimitsignal( 6098 = 18 )**

**1 ) Whenhoming,thepositivelimitsignalisinvalid**


Chapter 4 Control

**Description:** Whenstartingfromzero,thepositivelimitsignalisinvalid.Themotorrunsforwardata

higher setspeed.Whenthe risingedge of thepositivelimitsignal is detected,it decelerates and

reverses,andrunsinthereversedirectionatalowersetspeed.Whenthefallingedgeofthepositive

limitsignalisdetected,itstopsrunning.


Chapter 4 Control

**2 ) Whenhoming,thepositivelimitsignalisvalid**

**Description:** Whenstartingfromzero,thepositivelimitsignalisvalid.Themotordirectlyrunsinthe

reversedirectionatalowersetspeed,andstopswhenthefallingedgeofthepositivelimitsignalis

detected.

**Homingmethod 19 :homingwithhomesignal( 6098 = 19 )**

**1 ) Whenhoming,thehomesignalisinvalid**

**Description:** Whenstartingfromhoming,thehomesignalisinvalid.Themotorrunsforwardata

highersetspeed.Whentherisingedgeofthehomesignalisdetected,itdeceleratesandreverses,and

runsinthereversedirectionatalowersetspeed.Whenthefallingedgeofthehomesignalisdetected,it

stopsrunning.

**2 ) Whenhoming,thehomesignalisvalid**


Chapter 4 Control

**Description:** Whenstartingthehoming,thehomesignalisvalid.Themotordirectlyrunsinthe

reversedirectionatalowersetspeed,andstopswhenthefallingedgeofthehomesignalisdetected.


Chapter 4 Control

**Homingmethod 20 :homingwithhomesignal( 6098 = 20 )**

**1 ) Whenhoming,thehomesignalisinvalid**

**Description:** Whenstartingthehoming,thehomesignalisinvalid.Themotorrunsforwardata

lowerspeedandstopswhentherisingedgeofthehomesignalisdetected.

**2 ) Whenhoming,thehomesignalisvalid**

**Description:** Whenstartingfromzero,thehomesignalisvalid.Themotorfirstrunsinthereverse

directionatahighersetspeed.Whenthefallingedgeofthehomesignalisdetected,itdeceleratesand

reverses,andrunsforwardatalowersetspeed.Whentherisingedgeofthehomesignalisdetected,it

stops.

**Homingmethod 21 :homingwithhomesignal( 6098 = 21 )**

**1 ) Whenhoming,thehomesignalisinvalid**


Chapter 4 Control

**Description:** Whenstartingfromhoming,thehomesignalisinvalid.Themotorrunsinthereverse

directionatahighersetspeed.Whentherisingedgeofthehomesignalisdetected,itdeceleratesand

reverses,andrunsforwardatalowersetspeed.Whenthefallingedgeofthehomesignalisdetected,it

stops.

**2 ) Whenhoming,thehomesignalisvalid**

**Description:** Whenstartingfromzero,thehomesignalisvalid.Themotordirectlyrunsforwardata

lowersetspeedandstopswhenthefallingedgeofthehomesignalisdetected.

**Homingmethod 22 :Homingwithhomesignal( 6098 = 22 )**

**1 ) Whenhoming,thehomesignalisinvalid**


Chapter 4 Control

**Description:** Whenstartingthehoming,thehomesignalisinvalid.Themotorrunsinthereverse

directionatalowerspeedandstopswhentherisingedgeofthehomesignalisdetected.

**2 ) Whenhoming,thehomesignalisvalid**

**Description:** Whenstartingfromhoming,thehomesignalisvalid.Themotorfirstrunsforwardata

highersetspeed.Whenthefallingedgeofthehomesignalisdetected,itdeceleratesandreverses,

runninginthereversedirectionatalowersetspeed.Whentherisingedgeofthehomesignalis

detected,itstopsrunning.

**Homingmode 23 :homingwithhomesignalandpositivelimitsignal( 6098 = 23 )**

**1 ) Whenhoming,thehomesignalisinvalidandnopositivelimitswitchisencountered**


Chapter 4 Control

**Description:** Whenhoming,thehomesignalisinvalid.Themotor runsforwardatahigher set

speedanddoesnotencountertheforwardlimitswitch.Whentherisingedgeofthehomesignalis

detected,itdeceleratesandreverses,runsinthereversedirectionatalowersetspeed,andstopswhen

thefallingedgeofthehomesignalisdetected.

**2 ) Whenhoming,thehomesignalisinvalidandencountersthepositivelimitswitch.**

**Description:** Whenhoming,thehomesignalisinvalid.Themotor runsforwardatahigher set

speed.Whenitencounterstheforwardlimitswitch,itdeceleratesandreverses,andrunsinthereverse

directionatahighersetspeed.Whentherisingedgeofthehomesignalisdetected,itdeceleratesand

continuestoruninthereversedirectionatalowersetspeed.Whenthefallingedgeofthehomesignalis

detected,itstopsrunning.


Chapter 4 Control

**3 )Whenhoming,thehomesignalisvalid**

**Description:** Whenstartingthehoming,thehomesignalisvalid.Themotorrunsinthereverse

directionatalowersetspeedandstopswhenthefallingedgeofthehomesignalisdetected.

**Homingmode 24 :homingwithhomesignalandpositivelimitsignal( 6098 = 24 )**

**1** ） **Whenhoming,thehomesignalisinvalidandnopositivelimitswitchisencountered**

**Description:** Whenhoming,thehomesignalisinvalid.Themotor runsforwardatahigher set

speedanddoesnotencountertheforwardlimitswitch.Whentherisingedgeofthehomesignalis

detected,itdeceleratesandreverses,andrunsinthereversedirectionatalowersetspeed.Whenthe

fallingedgeofthehomesignalisdetected,itdeceleratesandreversesagain,andrunsforwardata

lowersetspeed.Whentherisingedgeofthehomesignalisdetected,itstopsrunning.


Chapter 4 Control

**2** ） **Whenhoming,thehomesignalisinvalidandencountersthepositivelimitswitch.**

**Description:** Whenhoming,thehomesignalisinvalid.Themotor runsforwardatahigher set

speed.Whenencounteringtheforwardlimitswitch,itdeceleratesandreverses,andrunsinthereverse

directionatahighersetspeed.Whentherisingedgeofthehomesignalisdetected,itdeceleratesand

continuestoruninthereversedirectionatalowersetspeed.Whenthefallingedgeofthehomesignalis

detected,itdeceleratesandreversesagain,andrunsforwardatalowersetspeed.Whentherising

edgeofthehomesignalisdetected,itstopsrunning.

**3** ） **Whenhoming,thehomesignalisvalid**

**Description:** Whenhoming,thehomesignalisvalid.Themotorrunsinthereversedirectionata

lowersetspeed.Whenthefallingedgeofthehomesignalisdetected,itdeceleratesandreversesagain,


Chapter 4 Control

andrunsforwardatalowersetspeed.Whentherisingedgeofthehomesignalisdetected,itstops

running.

**Homingmode 25 :homingwithhomesignalandpositivelimitsignal( 6098 = 25 )**

**1** ） **Whenhoming,thehomesignalisinvalidandnopositivelimitswitchisencountered**

**Description:** Whenhoming,thehomesignalisinvalid.Themotor runsforwardatahigher set

speedanddoesnotencountertheforwardlimitswitch.Whentherisingedgeofthehomesignalis

detected,itdeceleratesandcontinuestorunforwardatalowersetspeed.Whenthefallingedgeofthe

homesignalisdetected,itdeceleratesagainandrunsinthereversedirectionatalowersetspeed.

Whentherisingedgeofthehomesignalisdetected,itstopsrunning.

**2** ） **Whenhoming,thehomesignalisinvalidandencountersthepositivelimitswitch.**


Chapter 4 Control

**Description:** Whenhoming,thehomesignalisinvalid.Themotor runsforwardatahigher set

speed.Whenencounteringtheforwardlimitswitch,itdeceleratesandreverses,andrunsinthereverse

directionatahighersetspeed.Whentherisingedgeofthehomesignalisdetected,itdeceleratesand

reverses,andrunsforwardatalowersetspeed.Whenthefallingedgeofthehomesignalisdetected,it

deceleratesandreversesagain,andrunsinthereversedirectionatalowersetspeed.Whentherising

edgeofthehomesignalisdetected,itstopsrunning.

**3** ） **Whenhoming,thehomesignalisvalid**

**Description:** Whenhoming,thehomesignalisvalid.Themotorrunsforwardatalowersetspeed.

Whenthefallingedgeofthehomesignalisdetected,itdeceleratesandreverses,runninginthereverse

directionatalowersetspeed.Whentherisingedgeofthehomesignalisdetected,itstopsrunning.


Chapter 4 Control

**Homingmode 26 :homingwithhomesignalandpositivelimitsignal( 6098 = 26 )**

**1** ） **Whenhoming,thehomesignalisinvalidandnopositivelimitswitchisencountered**

**Description:** Whenhoming,thehomesignalisinvalid.Themotor runsforwardatahigher set

speedanddoesnotencountertheforwardlimitswitch.Whentherisingedgeofthehomesignalis

detected,itdeceleratesandcontinuestorunforwardatalowersetspeed.Whenthefallingedgeofthe

homesignalisdetected,itstopsrunning.

**2** ） **Whenhoming,thehomesignalisinvalidandencountersthepositivelimitswitch.**

**Description:** Whenhoming,thehomesignalisinvalid.Themotor runsforwardatahigher set

speed.Whenencounteringtheforwardlimitswitch,itdeceleratesandreverses,andrunsinthereverse

directionatahighersetspeed.Whentherisingedgeofthehomesignalisdetected,itdeceleratesand

reverses,andrunsforwardatalowersetspeed.Whenthefallingedgeofthehomesignalisdetected,it


Chapter 4 Control

stopsrunning.

**3** ） **Whenhoming,thehomesignalisvalid**

**Description:** Whenhoming,thehomesignalisvalid.Themotorrunsforwardatalowersetspeed

andstopswhenthefallingedgeofthehomesignalisdetected.

**Homingmode 27 :homingwithhomesignalandreverselimitsignal( 6098 = 27 )**

**1** ） **Whenhoming,thehomesignalisinvalidandnoreverselimitswitchisencountered**

**Description:** Whenhoming,thehomesignalisinvalid.Themotorrunsinthereversedirectionata

highersetspeedanddoesnotencounterthereverselimitswitch.Whentherisingedgeofthehome

signalisdetected,itdeceleratesandreverses,andrunsforwardatalowersetspeed.Whenthefalling

edgeofthehomesignalisdetected,itstopsrunning.

**2** ） **Whenhoming,thehomesignalisinvalidandencountersareverselimitswitch**


Chapter 4 Control

**Description:** Whenhoming,thehomesignalisinvalid.Themotorrunsinthereversedirectionata

highersetspeed.Whenitencountersthereverselimitswitch,itdeceleratesandreverses,andruns

forwardatahighersetspeed.Whentherisingedgeofthehomesignalisdetected,itdeceleratesand

continuestorunforwardatalowersetspeed.Whenthefallingedgeofthehomesignalisdetected,it

stopsrunning.

**3** ） **Whenhoming,thehomesignalisvalid**

**Description:** Whenhoming,thehomesignalisvalid.Themotorrunsforwardatalowersetspeed

andstopswhenthefallingedgeofthehomesignalisdetected.

**Homingmode 28 :homingwithhomesignalandreverselimitsignal( 6098 = 28 )**

**1** ） **Whenhoming,thehomesignalisinvalidandnoreverselimitswitchisencountered**


Chapter 4 Control

**Description:** Whenhoming,thehomesignalisinvalid.Themotorrunsinthereversedirectionata

highersetspeedanddoesnotencounterthereverselimitswitch.Whentherisingedgeofthehome

signalisdetected,itdeceleratesandreverses,andrunsforwardatalowersetspeed.Whenthefalling

edgeofthehomesignalisdetected,itdeceleratesandreversesagain,andrunsinthereversedirection

atalowersetspeed.Whentherisingedgeofthehomesignalisdetected,itstopsrunning.

**2** ） **Whenhoming,thehomesignalisinvalidandencountersareverselimitswitch**

**Description:** Whenhoming,thehomesignalisinvalid.Themotorrunsinthereversedirectionata

highersetspeed.Whenencounteringthereverselimitswitch,itdeceleratesandreverses,andruns

forwardatahighersetspeed.Whentherisingedgeofthehomesignalisdetected,itdeceleratesand

continuestorunforwardatalowersetspeed.Whenthefallingedgeofthehomesignalisdetected,it

deceleratesandreversesagain,andrunsinthereversedirectionatalowersetspeed.Whentherising

edgeofthehomesignalisdetected,itstopsrunning.


Chapter 4 Control

**3** ） **Whenhoming,thehomesignalisvalid**

**Description:** Whenhoming,thehomesignalisvalid.Themotorrunsforwardatalowersetspeed.

Whenthefallingedgeofthehomesignalisdetected,itdeceleratesandreverses,runninginthereverse

directionatalowersetspeed.Whentherisingedgeofthehomesignalisdetected,itstopsrunning.

**Homingmode 29 :homingwithhomesignalandreverselimitsignal( 6098 = 29 )**

**1** ） **Whenhoming,thehomesignalisinvalidandnoreverselimitswitchisencountered**

**Description:** Whenhoming,thehomesignalisinvalid.Themotorrunsinthereversedirectionata

highersetspeedanddoesnotencounterthereverselimitswitch.Whentherisingedgeofthehome

signalisdetected,itdeceleratesandcontinuestoruninthereversedirectionatalowersetspeed.When

thefallingedgeofthehomesignalisdetected,itdeceleratesandreversesagain,runsforwardatalower

setspeed,andstopswhentherisingedgeofthehomesignalisdetected.

**2** ） **Whenhoming,thehomesignalisinvalidandencountersthepositivelimitswitch.**


Chapter 4 Control

**Description:** Whenhoming,thehomesignalisinvalid.Themotorrunsinthereversedirectionata

highersetspeed.Whenencounteringthereverselimitswitch,itdeceleratesandreverses,andruns

forwardatahighersetspeed.Whentherisingedgeofthehomesignalisdetected,itdeceleratesand

reverses,andrunsinthereversedirectionatalowersetspeed.Whenthefallingedgeofthehome

signalisdetected,itdeceleratesandreversesagain,andrunsforwardatalowersetspeed.Whenthe

risingedgeofthehomesignalisdetected,itstopsrunning.

**3** ） **Whenhoming,thehomesignalisvalid**

**Description:** Whenhoming,thehomesignalisvalid.Themotorrunsinthereversedirectionata

lowersetspeed.Whenthefallingedgeofthehomesignalisdetected,itdeceleratesandreverses,and

runsforwardatalowersetspeed.Whentherisingedgeofthehomesignalisdetected,itstops.


Chapter 4 Control

**Homingmode 30 :homingwithhomesignalandreverselimitsignal( 6098 = 30 )**

**1** ） **Whenhoming,thehomesignalisinvalidandnoreverselimitswitchisencountered**

**Description:** Whenhoming,thehomesignalisinvalid.Themotorrunsinthereversedirectionata

highersetspeedanddoesnotencounterthereverselimitswitch.Whentherisingedgeofthehome

signalisdetected,itdeceleratesandcontinuestoruninthereversedirectionatalowersetspeed.When

thefallingedgeofthehomesignalisdetected,itstopsrunning.

**2** ） **Whenhoming,thehomesignalisinvalidandencountersareverselimitswitch**

**Description:** Whenhoming,thehomesignalisinvalid.Themotorrunsinthereversedirectionata

highersetspeed.Whenitencountersthereverselimitswitch,itdeceleratesandreverses,andruns

forwardatahighersetspeed.Whentherisingedgeofthehomesignalisdetected,itdeceleratesand

reverses,andrunsinthereversedirectionatalowersetspeed.Whenthefallingedgeofthehome

signalisdetected,itstopsrunning.


Chapter 4 Control

**3** ） **Whenhoming,thehomesignalisvalid**

**Description:** Whenstartingthehoming,thehomesignalisvalid.Themotorrunsinthereverse

directionatalowersetspeedandstopswhenthefallingedgeofthehomesignalisdetected.

**Homingmethod 31 :(usedinconjunctionwithhomingmethod 32 )**

**Description:** Whenthemotorusesanabsolutepositionencoder,performabsolutepositionhoming

(theabsolutepositionrecordedbyhomingmethod 32 ).

**Homingmethod 32 :(usedinconjunctionwithhomingmethod 31 )**

**Description:** Whenthemotorusesanabsolutepositionencoder,thecurrentabsolutepositionis

recordedasthezeropointforabsolutepositionreturn.

**Homingmode 33 , 34 :Homingwithmotorzeropositionpulsesignal( 6098 = 33 , 34 )**


Chapter 4 Control

**Description:**

**Homingmode 33 :Themotorrunsinthereversedirectionatasetlowerspeedandstops**

**whenitencountersthemotorZsignal;**

**Homingmode 34 :Themotorrunsintheforwarddirectionatasetlowerspeedandstops**

**whenitencountersthemotorZsignal;**

**Homingmethod 35 :currentpointhoming**

**Description:** Withbit 4 ofcontrolword 6040 asthetriggersignal,themotordoesnotmoveanddirectly

setsthehomingcompletionstate.

**4. 2. 2. 7 Internalmulti-positionfunction**

Themulti-segmentpositionoperationfunctionmeansthattheservodriverstores 16 segmentsof

positioninstructions.Thedisplacement,maximumoperatingspeed,accelerationanddecelerationtime

ofeachsegmentcanbesetseparately.Thewaitingtimeandconnectionmethodbetweeneachsegment

canalsobeselectedaccordingtoactualneeds.Thesettingprocessisasfollows:

```
Figure 4. 11 Multi-segmentpositioncommandsourcesettingprocess
```

Chapter 4 Control

**1 )Setthemulti-positionoperationmode**

☆Relatedfunctioncode:

Function

code

```
Name Settingrange Function
```
```
Setting
```
```
method
```
```
Effectivetime
```
```
Factory
```
```
setting
```
P 13. 00

```
Multi-stage
```
```
position
```
```
operationmode
```
```
0 - Singlerunends
```
```
andstops
```
```
1 - Cyclerun
```
```
2 - DIswitchrun
```
```
Setthe
```
```
connection
```
```
between
```
```
paragraphs
```
```
Shutdown
```
```
setting
```
```
Effective
```
```
immediately
```
```
1
```
P 13. 01

```
Specifythe
```
```
startingsegment
```
```
1 ～ 16
```
```
Setthe
```
```
startingpoint
```
```
ofmultiple
```
```
position
```
```
commands
```
```
Shutdown
```
```
setting
```
```
Effective
```
```
immediately
```
```
1
```
P 13. 02

```
Specifytheend
```
```
segment
```
```
1 ～ 16
```
```
Set the end
```
```
segment of
```
```
multiple
```
```
position
```
```
instructions
```
```
Shut down
```
```
setting
```
```
Effective
```
```
immediately
```
```
16
```
P 13. 03

```
Residue
```
```
processing
```
```
method
```
```
0 - Continuerunning
```
```
theunfinished
```
```
segment
```
```
1 - Restartrunning
```
```
fromthefirst
```
```
Settheservo
```
```
enabletoON,
```
```
andthe
```
```
starting
```
```
segment
```
```
numberwhen
```
```
Shut down
```
```
setting
```
```
Effective
```
```
immediately
```
```
0
```

Chapter 4 Control

```
segment the
```
```
multi-segmen
```
```
tposition
```
```
operationis
```
```
interrupted
```
```
andresumed
```
P 13. 04 Waitingtimeunit

```
0 - ms
```
```
1 - s
```
```
Setthe
```
```
waitingtime
```
```
unit
```
```
Shutdown
```
```
setting
```
```
Effective
```
```
immediately
```
```
0
```
P 13. 05

```
Position control
```
```
method
```
```
0 - Relative
```
```
positioncommand
```
```
1 - Absolute
```
```
positioncommand
```
```
Setthe
```
```
displacement
```
```
instruction
```
```
type
```
```
Shutdown
```
```
setting
```
```
Effective
```
```
immediately
```
```
0
```
**a)Stopattheendofasinglerun**

```
Figure 4. 12 Operationcurveofshutdownattheendofasingleoperation
```
V 1 max、V 2 max：Themaximumoperatingspeedofthefirstsegment(thestartingsegmentspecifiedby

P 1301 )andthemaximumoperatingspeedofthesecondsegment(thestartingsegmentincreasing

segment).


Chapter 4 Control

S 1 、S 2 ：Thefirstsegment(thestartingsegmentspecifiedbyP 1301 )isdisplaced,andthesecond

segment(thestartingsegmentandtheincreasingsegment)isdisplaced.

ModeDescription:

◆ThefirstsegmentoftheoperationisthestartingsegmentspecifiedbyP 1301 ,andthelast

segmentoftheoperationistheendingsegmentspecifiedbyP 1302.

◆Thesegmentnumberautomaticallyincreasesandswitches.

◆Waitingtimecanbesetbetweeneachsegment.

◆Theinternalmulti-segmentpositionenable(FunIN. 29 :PSEC_EN)signalislevel-valid.

◆Aftereachsegmentofoperationiscompleted,thepositioningcompletionsignalisvalid.

◆During operation, the internal multi-stage position enable is OFF, the servo abandons the

unfinisheddisplacementofthisstageandstops.Afterthestop,thepositioningcompletionsignalisvalid.

◆Turnontheinternalmulti-stagepositionenableagain,andtheservowillruninthecorresponding

stageaccordingtoP 13. 03 setting.

◆Duringacertainperiodofoperation,theservoisenabledandturnedoff.Themotorstopsinthe

servoOFFmode.Afterthestop,thepositioningisinvalid.

◆Duringacertainoperation,thereverseDI(FunIN. 24 :POS_DIR)logicswitchingoftheposition

commandhasnoeffectontheoperationdirectionofthissection.

**b)Cycleoperation(P 13. 00 = 1 )**


Chapter 4 Control

```
Figure 4. 13 Cyclicoperationcurve
```
V 1 max、V 2 max：Themaximumoperatingspeedofthefirstsegment(thestartingsegmentspecified

byP 1301 )andthemaximumoperatingspeedofthesecondsegment(thestartingsegmentincreasing

segment).

S 1 、S 2 ：Thefirstsegment(thestartingsegmentspecifiedbyP 1301 )isdisplaced,andthesecond

segment(thestartingsegmentandtheincreasingsegment)isdisplaced.

ModeDescription:

◆Cyclicoperation,thestartingsegmentnumberofeachroundissetbyparameterP 1301 ;

◆Thesegmentnumberautomaticallyincreasesandswitches;

◆Waitingtimecanbesetbetweeneachsegment;

◆Theinternalmulti-segmentpositionenable(FunIN. 29 :PSEC_EN)signalislevel-valid;ifvalid,the

loopoperationstateismaintained.

◆Aftereachsegmentiscompleted,thepositioningcompletionsignalisvalid;

◆Duringoperation,turnoffthemulti-stagepositioncommandenable,theservoabandonsthe

unfinisheddisplacementofthisstageandstops.Afterthestop,thepositioningcompletionsignalisvalid;

◆Turnonthemulti-segmentpositioncommandagain,andtheservowillruninthecorresponding

segmentaccordingtoP 13. 03 setting;

◆Duringacertainperiodofoperation,theservoisenabledandturnedoff.Themotorstopsinthe

servoOFFmode.Afterthestop,thepositioningisinvalid.

◆Duringacertainoperation,thereverseDI(FunIN. 24 :POS_DIR)logicswitchingoftheposition

commandhasnoeffectontheoperationdirectionofthissection.

**c)DIswitchoperation(H 11 - 00 = 2 )**


Chapter 4 Control

```
Figure 4. 14 Multi-stagepositiontimingdiagram
```
Whenthemulti-segmentpositionoperationmodeissettoDIswitchingoperation,pleaseconfigure

theDIterminaloftheservodrivetofunction 6 ~ 9 (FunIN. 6 :CMD 1 ~FunIN. 9 :CMD 4 ,multi-segment

operationcommandswitching)andconfirmtheeffectivelogicoftheDIterminal.

☆Relatedfunctioncode:

```
Code Name Functionname Function
```
```
FunIN. 6 CMD 1
```
```
Multi-stageoperation
```
```
commandswitching 1
```
```
Themulti-segmentnumberisa 4 - bitbinarynumber.Thecorresponding
```
```
relationshipbetweenCMD 1 ~CMD 4 andthesegmentnumberisasfollows:(DI
```
```
terminallogicislevel-effective,whentheinputleveliseffective,theCMDvalue
```
```
is 1 ,otherwiseitis 0 )
```
#### CMD 4 CMD 3 CMD 2 CMD 1 段号

#### 0 0 0 0 1

#### 0 0 0 1 2

#### ......

#### 1 1 1 0 15

#### 1 1 1 1 16

```
FunIN. 7 CMD 2
```
```
Multi-stageoperation
```
```
commandswitching 2
```
```
FunIN. 8 CMD 3
```
```
Multi-stageoperation
```
```
commandswitching 3
```
```
FunIN. 9 CMD 4
```
```
Multi-stageoperation
```
```
commandswitching 4
```

Chapter 4 Control

Figure 4. 15 OperationcurveofDIswitchingoperation

Vxmax、Vymax：Maximumoperatingspeedofsegmentxandsegmenty;

Sx、Sy：displacementofsegmentxandsegmenty;

ModeDescription:

◆Whenrunningthecurrentsegmentnumber,youcansetthenextrunningsegmentnumber.After

completingthepositioncommandsetforthecurrentsegmentnumber,themotorstops;then

immediatelyrunsthenextrunningsegmentnumber;

◆ThesegmentnumberisdeterminedbytheDIterminallogic;

◆Thereisnowaitingtimebetweeneachsegment,andtheintervaltimeisdeterminedbythehost

computercommanddelay;

◆Theinternalmulti-segmentpositionenable(FunIN. 29 :PSEC_EN)signalislevel-valid;ifvalid,the

loopoperationstateismaintained;

◆Aftereachsegmentiscompleted,thepositioningcompletionsignalisvalid;

◆Duringoperation,turnoffthemulti-stagepositioncommandenable,theservoabandonsthe

unfinisheddisplacementofthisstageandstops.Afterthestop,thepositioningcompletionsignalisvalid;

◆IftheservoisenabledOFFduringacertainperiodofoperation,themotorstopsintheservoOFF

stopmode.Afterthestopiscompleted,thepositioningcompletionisinvalid;


Chapter 4 Control

◆Duringacertainoperation,thereverseDI(FunIN. 24 :POS_DIR)logicswitchingoftheposition

commandhasnoeffectontheoperationdirectionofthissection.

**2 )Positioncontrolmethod**

**a)Incrementalpositioncontrol**

Therelativedisplacementofeachexecutionoftheincremental positioncontrol methodis the

positionincrementofthetargetpositionrelativetothecurrentpositionofthemotor.

**b)Absolutepositioncontrol**

Theabsolutepositioncontrolmodeisbasedonthesuccessfulmotorzeroing.Ifthemotorzeroingis

notsuccessful,thatis,themotorhomeisnotzeroedbeforerunningtheabsolutepositionfunction,the

multi-segmentpositionfunctionofthemotorwillnotbeexecuted,andthemotorwillenterthelockaxis

waitingstate.

Absolutedisplacementisthepositionincrementofthetargetpositionrelativetothemotorhome.

Forexample,ifthemechanicalhomeoffset(P 1614 )afterthemotorreturnstozeroissetto 1000 ,then

theabsolutepositioncounter(P 1807 )whenthemotorstopsafterthehomingisalso 1000 .Afterthe

absolutepositionoffsetisexecuted,thevalueofthemotorabsolutepositioncounter(P 1807 )isthe

motormechanicalhomeoffset(P 1614 )plustheabsolutedisplacementincrementofthecorresponding

segment(forexample,thefirstsegmentP 1308 is 20000 ),sotheabsolutepositionofthemotorafter

executionis 20000 + 1000.

**3 )Multi-stagepositionoperationcurvesetting**

The multi-stage position operation function can set 16 different position commands. The

displacement,maximumoperatingspeed,accelerationanddecelerationtimeofeachstageandthe

waitingtimebetweeneachstagecanbesetseparately.Takethefirststageasanexample:


Chapter 4 Control

☆Relatedfunctioncode:

```
Function
```
```
code
```
```
Name Settingrange Unit Function
```
```
Setting
```
```
method
```
```
Effectivetime
```
```
Factory
```
```
setting
```
```
P 13. 09
```
```
Thefirst
```
```
stageof
```
```
movement
```
- 1073741824

```
~ 1073741824
```
```
Comma
```
```
ndunit
```
```
Setthetotal
```
```
position
```
```
command
```
```
ofthefirst
```
```
segment
```
```
Running
```
```
setting
```
```
Effective
```
```
immediately
```
```
10000
```
```
P 13. 10
```
```
Maximum
```
```
operating
```
```
speedofthe
```
```
first
```
```
displacement
```
```
1 ~ 6000 rpm
```
```
Setthe
```
```
maximum
```
```
running
```
```
speedof
```
```
thefirst
```
```
stage
```
```
Running
```
```
setting
```
```
Effective
```
```
immediatel
```
```
200
```
```
P 13. 11
```
```
Thefirst
```
```
stageof
```
```
displacement
```
```
acceleration
```
```
and
```
```
deceleration
```
```
time
```
```
0 ~ 65535 ms(s)
```
```
Setthetime
```
```
forthe
```
```
motorto
```
```
change
```
```
speedfrom
```
```
0 rpmto
```
```
1000 rpmin
```
```
thefirst
```
```
stageof
```
```
multi-stage
```
```
position.
```
```
Running
```
```
setting
```
```
Effective
```
```
immediately
```
```
10
```
```
P 13. 12
```
```
Waitingtime
```
```
afterthefirst
```
```
displacement
```
```
iscompleted
```
```
0 ~ 10000 ms(s)
```
```
Setthe
```
```
waitingtime
```
```
afterthe
```
```
first
```
```
segment
```
```
positioning
```
```
is
```
```
completed
```
```
Running
```
```
setting
```
```
Effective
```
```
immediately
```
```
10
```
Accordingtotheabovesettings,theactualoperationcurveofthemotorisshowninthefigure

below:


Chapter 4 Control

```
Figure 4. 16 Thefirstsectionofthemotoroperationcurve
```
Therefore, the actual acceleration time t to P 13. 10 (maximum running speed of the first

displacementsegment)is:

```
(P 13. 11 )
1000
```
```
(P 13. 10 )
t 
```
**4 )Internalmulti-segmentpositioncommandenable**

Whenselectingtheinternal multi-segmentpositioncommandasthepositioncommandsource,

pleaseconfigureoneDIterminaloftheservodriveasfunction 29 (FunIN. 29 :internalmulti-segment

positionenable)andconfirmtheeffectivelogicoftheDIterminal.

☆Relatedfunctioncode:

```
Code Name Functionname Function
```
```
FunIN. 29 PSEC_EN
```
```
Internalmulti-stageposition
```
```
enable
```
```
Ifvalid,theservomotorrunsmultiplepositioncommands;
```
```
Ifinvalid,theservomotorisinalockedstate;
```
### 4 3 Speedcontrolmode.........................................................................................................................

**4. 3. 1 Speedcontroldiagram**


Chapter 4 Control

```
Figure 4. 17 Speedcontroldiagram
```
Themainusestepsofspeedcontrolmodeareasfollows:

1. Correctlyconnectthepowersupplyofservomaincircuitandcontrolcircuit,aswellas

themotorpowercableandencodercable.

2. DisconnectthemotorandloadandperformtheservoJOGtestrunthroughthehost

computertocomfirmwetherthemotorcanrunasnormal.

3. PleaserefertospeedmodewiringdescriptiontoconnectthenecessaryDI/DOsignal

andanaloginstructioninDIDOterminal.

4. makerelatesettingsofspeedmode.

5. Enableservo,firstlymakethemotorrotateatalowspeedandjudgewetherthe

rotationdirectionofmotorisnormal,thenadjustgain.

**4. 3. 2 Internalmulti-segmentspeedfunction**

Theservodriver hasmulti-segemntspeedrunningfunction.Itrefers theinternalofservodrive

storage 16 segmentsspeedinstruction,themaximumspeedofeachsegmentandrunningtimecanbe

setrespectively.Andequippedwith 4 setsofaccelerationanddecelerationtimestochoosefrom.The

settingprocessareasfollows:


Chapter 4 Control

```
Figure 4. 18 Multi-segmentsettingprocessdiagram
```
① **Settingmulti-segmentrunningmode**

☆Relativefunctioncode：

Functioncode Name Settingarrange Unit Function

```
Setting
```
```
method
```
```
Effective
```
```
time
```
```
Factory
```
```
settings
```
#### P 14. 00

```
Multi-segment
```
```
speed
```
```
running
```
```
method
```
```
0 - Stopattheendofsignal
```
```
run(P 14. 01 Segments
```
```
selection)
```
```
1 - Loopoperation(P 14. 01
```
```
Segmentselection)
```
```
2 - Switchingthrough
```
```
externalDI
```
#### -

```
Setting
```
```
multi-segments
```
```
speedinstruction
```
```
operationmethod
```
```
Shutdown
```
```
settings
```
```
Effective
```
```
immediately 1
```
P 14. 01 Speed

```
commandend
```
```
segment
```
```
selection
```
#### 1 ~ 16

- Settingthenumber

```
ofsegmentsfor
```
```
multi-segmentspeed
```
```
command
```
```
Shutdown
```
```
settings
```
```
Effective
```
```
immediately
1
```
#### P 14. 02

```
Running time^0 - Sec
```
- Selectingtimeunit

```
forthe
```
```
Operation Effective 1
```

Chapter 4 Control

```
unitselection 1 - Min multi-segmentspeed
```
```
commandoperation
```
```
settings immediately
```
TakeP 14. 01 = 2 asexampletoillustrateeachmode.

**●Signaloperationendshutdown(P 14. 00 = 0 )**

SettingfunctioncodeP 14. 00 to 0 ,selectsignalrunningshutdownmethod.Aftersettingfunction

codeP 14. 01 andP 14. 02 accordingtoexecutetotalsegmentsandimplementtimeunitindividually,and

settingparameterssuchascorrespondingcommandvalue,operationtime,accelerateanddecelerate

timeaccordingtorequirement.ThedriverwillrunningasthemethodoffromthefirstsegmenttotheN

segmentaccordingtosegmentcode,untilfinishthelastsegmentandthenshutdown.

```
Table 4. 6 Thesignalrunningendupshutdowndescription
```
```
Modedescription Operationcurve
```
◆Runningoneround

◆Segmentnumberautomatic

incrementswitching

```
◆V 1 max、V 2 max：Firstsegment,secondsegmentcommandspeed
```
```
◆t 1 ：Firstsegmentactualaccelerateanddeceleratetime
```
```
◆t 3 、t 5 ：Thesecondtimeaccelerate,deceleratetime
```
```
Therunningtimeofacertainsegment:Thespeedchangingtimefromthelastsegmentspeed
```
```
commandswitchingtothissegmentspeedcommand+thissegmentuniformrunningtime(such
```
```
as:inthisfigure,thefirstsegmentrunningtimeist 1 +t 2 ,thesecondrunningtimeist 3 +t 4 ,andso
```
```
on.)
```
```
Donotsetacertainsegmentrunningtimeto 0 ,driverwillskipthisspeedcommand,executenext
```
```
segment;
```
```
Theactualrotatespeedofmotorreachesuptothesettingmaximumrunningspeedofthissegment,
```

Chapter 4 Control

```
thespeedreachessignalisvalid.
```
★Glossary：

Thedriveruns completelyonce themulti-segment speed commandtotalsegmentssettingby

P 14. 01 iscalledcompletearoundoperation.

● **Cyclicoperation(P 14. 00 = 1 )**

SettingfunctioncodeP 14. 00 to 1 ,andselecttheloopoperationmethod.Aftersettingfunctioncode

P 14. 01 ,P 14. 02 respectivelyaccordingtotheexecutiontotalsegmentnumberandexecutiontimeunit,

andsettingparameterscommandvalue,runningtime,accelerationanddecelerationtimecorresponding

segment according to requirements. The moduel will set the running time and acceleration and

decelerationtimeofeachsegment,thedrivewillrunfrom 1 stsegmenttoNthsegmentaccordingto

segmentcode.Afterrunningthelastsegment,itwillautomaticallyjumptothe 1 stsegmentforcyclic

operation.

```
Table 4. 7 Cyclicoperationdescription
```
```
Modedescription Operationcurve
```
◆Cyclicoperation,Thestarting

segmentnumberofeachround

is 1 ;

◆Segmentnumberautomatic

incrementswitching;

◆TheServoenableisvalidand

alwayskeepthecyclic

operationstatus.

```
◆V 1 max、V 2 max：Themaximumrunningspeedoffirstsegment,secondsegment;
```
```
◆Acertainsegmentrunningtime:Thespeedchangingtimeofpreviousspeedcommandswitching
```
```
tothissegmentspeedcommand+thissegmentuniformrunningtime(Forexample:inthisfigure,
```
```
thefirstsegmentrunningtimeist 1 +t 2 ,thesecondrunningtimeist 3 +t 4 ,andsoon.)
```
```
◆Donotsetacertainsegmentrunningtimeto 0 ,driverwillskipthisspeedcommand,executenext
```
```
segment;
```
```
◆Theactualrotatespeedofmotorreachesthemaximumrunningspeedsettingofthissegment,
```

Chapter 4 Control

```
thespeedreachessignalisvalid.
```
**●DISwitchingoperation(P 14. 00 = 2 )**

SettingthefunctioncodeP 14. 00 to 2 ,selecttheexternalDIswitchingmethod.Setcorresponding

segmentcommandvalueaccordingtorequirements,thedriverwillselectruncorrespondingsegment

speedinstructionaccordingtothecombinationofON/OFFofexternalDI(CMDx).

```
Table 4. 8 DISwitchingoperationdescription
```
```
Modedescription Runningcurve
```
◆Aslongasthesegment

numberisupdated,itcan

continuetorun.

◆Thesegmentnumberis

determinedbyDIterminal

logic;

◆Theintervalbetween

segmentsisdeterminedby

thehostcomputer

commanddelaytime;

◆Multi-segmentposition

enableisvalidforedge

changing.

```
x，y：Segmentnumber,thelogicrelationshipbetweensegmentnumberandDIterminalis
```
```
descriptionasfollows;
```
```
Therunningtimeofacertainsectionisnotaffectedbythefunctioncodesettingvalue,duringa
```
```
certainsectionspeedinstructionrunning,ifthesegmentnumberischanged,itwillswitchtoa
```
```
newsegmentnumberforrunningatonce;Theactualrotatespeedreachesthissegmentsetting
```
```
maximumrunningspeed,thespeedreachessignalisvalid.
```
Whensettingthemulti-segmentspeedrunningmethodtoDIswitchingrunning,mustconfigurethe

ServodriveDIterminalasfunction 6 ~ 9 (FunIN. 6 ~FunIN. 9 Multi-segmentoperationcommandswitching),

andcomfirmthatDIterminalvalidlogic.


Chapter 4 Control

☆Relativefunctioncode：

```
Code Name FunctionName Function
```
FunIN. 6 CMD 1

```
Multi-segmentrunninginstruction
```
```
switching
```
```
Multi-segmentnumberis 4 - bitbinarynumber,therelationshipbetween
```
```
CMD 1 ~CMD 4 andsegmentnumberareasfollows:
```
```
CMD 4 CMD 3 CMD 2 CMD 1 Segment
```
```
Number
```
#### 0 0 0 0 1

#### 0 0 0 1 2

#### ......

#### 1 1 1 1 16

```
DICMDvalueis 1 whenterminalinputlevelisvalid,oris 0.
```
FunIN. 7 CMD 2

```
Multi-segmentrunninginstruction
```
```
switching
```
FunIN. 8 CMD 3

```
Multi-segmentrunninginstruction
```
```
switching
```
FunIN. 9 CMD 4

```
Multi-segmentrunninginstruction
```
```
switching
```

Chapter 4 Control

```
Figure 4. 19 Multi-segmentspeedcurveexamples
```
②Multi-segmentSpeedRunningCurveSettings

Takethefirstsegmentspeedinstructionasexamples,therelativefunctioncodeisasbelow:

☆Relativefunctioncode：

Function

```
code
```
```
Name Settingrange Unit Function
```
```
Setting
```
```
method
```
```
Effective
```
```
time
```
```
Factory
```
```
settings
```
#### P 14. 03

```
Accelerateand
```
```
deceleratetime
```
```
0 ~ 65535 ms
```
```
SettingAccelerateand
```
```
deceleratetime
```
```
Shutdown
```
```
settings
```
```
Effective
```
```
immediately 0
```
#### P 14. 04

```
Accelerateand
```
```
deceleratetime
```
```
0 ~ 65535 ms
```
```
Shutdown
```
```
settings
```
```
Effective
```
```
immediately 0
```
P 14. (^05) Accelerateand 0 ~ 65535 ms Shutdown Effective 0


Chapter 4 Control

```
deceleratetime settings immediately
```
#### P 14. 06

```
Accelerateand
```
```
deceleratetime
```
```
0 ~ 65535 ms
```
```
Shutdown
```
```
settings
```
```
Effective
```
```
immediately 0
```
#### P 14. 07

```
Thefirstsegment
```
```
speedcommand
```
- 6000 ~ 6000 rpm

```
Settingthefirstsegment
```
```
speedinstructionvalue
```
```
Shutdown
```
```
settings
```
```
Effective
```
```
immediately 0
```
#### P 14. 08

```
Thefirstsegment
```
```
speedcommand
```
#### 0 ~ 65535

```
0. 1 s
```
```
(min)
```
```
Settingthefirstsegment
```
```
instructionrunningtime
```
```
Shutdown
```
```
settings
```
```
Effective
```
```
immediately 50
```
#### P 14. 09

```
Thefirstsegment
```
```
accelerateand
```
```
deceleratetime
```
```
0 - Noaccelerate
```
```
anddecelerate
```
```
time
```
```
1 - Accelerateand
```
```
deceleratetime
```
```
2 - Accelerateand
```
```
deceleratetime
```
```
3 - Accelerateand
```
```
deceleratetime
```
```
4 - Accelerateand
```
```
deceleratetime
```
#### -

```
Selectthefirstsegment
```
```
accelerateanddecelerate
```
```
method
```
```
Shutdown
```
```
settings
```
```
Effective
```
```
immediately
```
#### 0

Amongthemulti-segmentspeedinstructionparameters,except 1 ~ 16 segmentscommandvalue


Chapter 4 Control

andcommandrunningtime,thereare 4 setsaccelerationanddecelerationtimetochoosefrom,the

defaultmethodisnoaccelerateanddeceleratetime.Takemulti-segmentP 14. 01 = 1 signalrunningend

asexample,toillustratetheactualaccelerateanddeceleratetimeandrunningtime:

```
Figure 4. 20 Multi-segmentspeedcurveexample
```
Asshownabove,thissegmentspeedinstructionisV 1 ,actualtheacceleratetimet 1 is:

t 1 =

V 1

1000

×Theaccelerationtimesettingofthissegmentspeed

Actualdecelerationtime:

t 2 =

V 1

1000

×Thedecelerationtimesettingofthissegmentspeed

Runningtime:Thespeedchangingtimefrompreviousspeedinstructionswitchingtothissegment

speedcommand+thissegmentuniformrunningtime,shownasthefiguret 3.

### 4. 4 Torquecontrolmode


Chapter 4 Control

```
Figure 4. 21 Torquecontroldiagra
```
Themainusingstepsoftorquecontrolmodeareasfollows:

1. Correctlyconnectthepowersupplyoftheservomaincircuitandcontrolcircuit,aswell

asthemotorpowercableandencodercable.

2. Disconnectthemotorfromtheload,andconductaservoJOGtestrunthroughthe

hostcomputertoconfirmwhetherthemotorcanrunnormally.

3. ConnectnecessaryDI/DOandtorquecommandsource,speedlimit,etc.

4. Makerelatedsettingsoftorquemode.

5. Enableservo,setalowerspeedlimitvalue,imposeaforwardorreversetorque

commandtoservo,andconfirmwethertherotationdirectionisright,therotationspeedislimited

correctly,ifnormal,youcanstartusingit.

### 4 5 Mixedmodeswitching....................................................................................................................

Themixedcontrolmodeisreferto,whentheservoenableisON,theservostatusis“run”,the


Chapter 4 Control

workingmodeofservodrivercanbeswitchingindifferentcontrolmode.

Themixedcontrolmodeisdividedintothreetypesasbelow:

●Torquemode↔speedmode

●Speedmode↔positionmode

●Torquemode↔positionmode

SettingfunctioncodeP 00. 00 throughpanelorhoster,theservodriverwillworkinginmixedcontrol

mode.

☆Relativefunctioncode：

```
Function
```
```
code
```
```
Name Settingrange Function
```
```
Setting
```
```
method
```
```
Effective
```
```
time
```
```
Factory
```
```
settings
```
#### P 00. 00

```
Control
```
```
mode
```
```
selection
```
```
0 - Positionmode
```
```
1 - Speedmode
```
```
2 - Torquemode
```
```
3 - Torquemode↔Speedmode
```
```
4 - Speedmode↔Positionmode
```
```
5 - Torquemode↔Positionmode
```
```
Settingservodrivercontrol
```
```
mode
```
```
Shutdown
```
```
settings
```
```
Effective
```
```
immediately
```
#### 0

Pleasesettingthe servodriver’sparameters individuallyindifferentcontrol modeaccordingto

mechanicalstructureandindex.

WhenP 00. 00 = 3 / 4 / 5 ,pleaseconfigureoneDIterminalofservodrivertofunction 10 (FunIN. 10 ：

MODE_SWITCH，Modeswitching),andconfirmthevalidlogicofDIterminal.

☆Relativefunctioncode：


Chapter 4 Control

```
Code Name
```
```
Function
```
```
name
```
```
Functios
```
FunIN. 10 MODE_SWITCH

```
Mode
```
```
switching
```
```
Appliedinmixedcontrolmode,whentheservostatusis“run”,
```
```
settingthedrivercurrentcontrolmode
```
```
P 00. 00 Terminallogic Controlmode
```
#### 3

```
Invalid Torquemode
```
```
Valid Speedmode
```
#### 4

```
Valid Speedmode
```
```
Valid Positionmode
```
#### 5

```
Invalid Torquemode
```
```
Valid Positionmode
```

## Chapter 5 Operationperformanceadjustment

## Chapter 5 Operationperformanceadjustment

### 5 1 Overview...........................................................................................................................................

The Servo drive needto drivethe motor as quickly andaccurately as possible totrack the

instructionsfromhostorinternalsettings.Inordertomeettherequirement,theservogainmustbe

reasonablyadjustment.

```
Figure 5. 1 Gainsettingexample
```
Positionloopgain: 40. 0 Hz Speed​ ​ loopgain: 200. 0 Hz Speed​ ​ loopgain: 200. 0 Hz

```
Speed​ loopgain: 200. 0 Hz Speed​ ​ loopgain: 25. 0 Hz Speed​ ​ loopgain: 25. 0 Hz
```
```
Speed​ loopintegraltimeconstant Speed​ ​ loopintegraltimeconstant Speed​ loopintegraltime
```
constant

```
: 100. 00 ms : 50. 00 ms : 50. 00 ms
```
```
Speed​ ​ feedforwardgain: 0 Speed​ ​ feedforwardgain: 0 Speed​ feedforwardgain: 50. 0 %
```
```
Loadinertiaratio: 30 Loadinertiaratio: 30 Loadinertiaratio: 30
```
Theservogainissettingbythecombinationofmultipleparameters(Positionloop,speedloopgain,

filter,loadrotateinertiaratio,etc.),whichinfluenceeachother.Therefore,thesettingofservogainmust

consideratethebalancebetweeneachparametersettingvalue.

**Note:** Beforeperforminggainadjustment,itisrecommendedtoperformajogtestruntoconfirmthatthemotorcanoperatenormally.

Thenormalprocessofgainadjustmentisshownasthefigurebelow:


Chapter 5 Operationperformanceadjustment

Figure 5. 2 Gainadjustmentprocess

### 5 2 InertiaIdentification.........................................................................................................................

Loadinertiaratio=

```
mechanicalloadttotalrotationinertia
Motor−selfrotationinertia
```
Loadinertiaratioistheimportantparameterofservosystem,correctlysetloadinertiaratioisbenefit

forquicklycompletetest.Theloadinertiaratiocanbesetmanuallyorautomaticallyidentifiedthrough

theinertiaidentificationfunctionoftheservodrive.

Usethe“Rotatinginertiaidentificationfunction(P 11. 03 )”torealizeinertiaidentificationbyoperating

thefunctioncodeofservodrivehostcomputertorotatethemotor.

**Note:**

```
Tousetheinertiaidentificationfunction,inordertoaccuratelycalculatetheloadinertiaratio,thefollowingconditionsmustbe
```
met:quirements:

- Theactualmotormaximumrotationspeedisupto 200 rpm.
- Whentheactualmotoracceleratesanddecelerates,theaccelerationisabove 3000 rpm/s.


Chapter 5 Operationperformanceadjustment

- Theloadtorqueisrelativelystableandcannotchangestrenuously.
- Theactualloadinertiaratiodoeanotexceed 120 times.

Iftheactualloadinertiaratioislargeandthedrivegainislow,themotorwillmoveslowlyandcannot

meetthemotor’smaximumspeedandaccerlationrequirements.Inthis case,thespeedloopgain

(P 07. 01 )canbeincreasedandtheinertiaidentificationcanbeperformedagain.

Ifvibrationoccursduringtheidentificationprocess,theinertiaidentificationshouldbestopped

immediately andthe gainshould be reduced.In addition,when the backlashofthe transmission

mechanismislarge,theinertiaidentificationmayfail.Beforeperformingtheofflineinertiaidentification,

firstcomfirmthefollowing:

**1 )Themotormovableprocessshouldmeettherequirementbellowing**

Thereisamovablestrokeofmorethan 1 turnintheforwardandreversedirectionsbetweenthe

mechanicallimitswitches:Beforeperformingtheofflineinertiaidentification,pleasemakesurethat a

limitswitchisinstalledonthemachineandsurethemotorhasamovablestrokeofmorethan 1 turnin

theforwardandreversedirectionstopreventovertravelduringtheinertiaidentificationprocessand

causeanaccident!

**2 )EstimatedloadinertiaratioP 00. 05 value**

IfP 00. 05 isthedefaultvalue( 1. 00 )andtheactualloadinertiaratioisgreaterthan 30. 00 ,themotor

maymoveslowlycausingidentificationfailure.Inthiscase,thefollowingtwomeasurescanbetaken:

a)ThepresetP 00. 05 isalargerinitialvalue:itisrecommendedtouse 5. 00 timesasthestarting

valueforthepresetvalue,andgraduallyincreaseituntilthepaneldisplayvalueisupdatedduringthe

identificationprocess.

b)Appropriatelyincreasethedriverigiditylevel(P 00. 04 )sothattheactualmotorspeedcanreach

themaximumspeedofinertiaidentification(P 08. 20 ).

InertiaidentificationisdividedintopositiveandnegativetrianglewavemodeandJOGmode,and

themotorrotationneedstoberealizedthroughthehostcomputerfunctioncode.


Chapter 5 Operationperformanceadjustment

```
Table 5. 1 Inertiaidentificationmethod
```
Project Positiveandnegativetrianglewaveform(P 08. 23 = 0 ) JOGmode(P 08. 23 = 1 )

Instruction

form

Maxspeed P 08. 20 P 08. 20

Accelerati

on and

deceleratio

ntime

#### P 08. 21 P 08. 21

Interval

time

```
P 08. 22 Thetimeintervalbetweentwokeyoperations
```
Motor

rotation

times

```
ViewP 08. 24 HumanControl
```
Button

Descriptio

n

```
Longpresstheinertiaidentificationforwardbutton:the
```
```
motorfirstrotatesforwardandthenreverses
```
```
Longpresstheinertiaidentificationreversebutton:the
```
```
motorwillfirstreverseandthenforward
```
```
Releasethebutton:stopatzerospeedandkeepthe
```
```
positionlocked
```
```
Longpresstheinertiaidentificationforwardbutton:
```
```
themotorrunsforward
```
```
Longpresstheinertiaidentificationreversebutton:
```
```
themotorreverses
```
```
Releasethe button: zero speed stop,keep the
```
```
positionlocked
```
Applicable

occasions

```
Whenthemotorstrokeisshort.
```
```
Themotorhasalongstrokeandcanbemanually
```
```
controlled.
```

Chapter 5 Operationperformanceadjustment

```
Figure 5. 3 Inertiaidentificationprocess
```
☆Relatedfunctioncode:

```
Function
```
```
code
```
```
Name Settingrange
```
```
Minimum
```
```
unit
```
```
Function
```
```
Factory
```
```
setting
```
```
Effective
```
```
time
```
```
Setting
```
```
method
```
#### P 08. 20

```
Inertia
```
```
identification
```
```
maximum
```
```
speed
```
```
200 ~ 1000 1 rpm
```
```
Setthemaximumspeed
```
```
commandforinertia
```
```
identification
```
#### 500

```
Effective
```
```
immediately
```
```
Shutdown
```
```
setting
```
#### P 08. 21

```
Inertia
```
```
identification
```
```
acceleration
```
```
and
```
```
deceleration
```
```
time
```
```
50 ~ 800 1 ms
```
```
Setthetimeforthemotorto
```
```
acceleratefrom 0 rpmtothe
```
```
maximumspeedofinertia
```
```
identification(P 08. 20 )during
```
```
inertiaidentification.
```
#### 100

```
Effective
```
```
immediately
```
```
Shutdown
```
```
setting
```

Chapter 5 Operationperformanceadjustment

### 5 3 Gainadjustment..............................................................................................................................

**5. 3. 1 PIParametersAdjustmentPI**

Whentheautomaticgaincannotachievetheexpectedresult,youcanmanuallyminuteadjustthe

gain.Tooptimizetheresultthroughmoredetailedadjustment.Theservosystemiscomposedofthree

controlloops,fromoutsidetoinside,theyarepositionloop,speedloopandcurrentloop,thebasic

controlblockdiagramisshowninfigure 5. 4.

```
Figure 5. 4 ManualGainBasicDescriptionDiagram
```
Thedefaultcurrentloopgainofservodriveisalreadyensuredthesufficientresponsiveness,

generallydoesnotneedtobeadjusted,onlypositionloopgain,speedloopgainandotherauxiliarygain

needtobeadjusted.Therefore,whenadjustgaininpositioncontrolmode,inordertoensuresystem

stability,whileincreasingthepositionloopgain,itisnecessarytoimprovespeedgain,andensurethat

theresponseofpositionloopislowerthanspeedloop,thebasicgainparametersadjustmentmethod

areasfollows.

Table 5. 2 Gainadjustmentmethod

```
Steps
```
```
Function
code Name AdjustmentDescription
```
#### 1 P 07. 01

```
Speed
loop
gain
```
```
◆Parametersfunction:
```
```
Determinesthemaximumfrequencyofchangingspeedcommandsthatthespeedloopcan
follow.
```
```
Underthepremisethattheloadinertiaratio(P 00. 05 )issetcorrectly,itcanbeconsideredthatthe
maximumfollowfrequencyofspeedloop=P 07. 01
```

Chapter 5 Operationperformanceadjustment

```
◆Adjustmentmethod:
```
```
Withintherangewherenoiseandvibrationdonotoccurs,increasingthisparametercan
acceleratepositioningtimeandbringbetterspeedstabilityandfollowability.Ifnoiseoccurs,
reducetheparametersettingvalue.
```
#### 2 P 07. 02

```
Speed
loop
integral
```
```
time
```
```
◆Parametersfunction:
```
```
Eliminatespeedloopdeviation.
```
```
◆Adjustmentmethod:
```
```
Itisrecommendtotakethevaluefromthefollowingrelationship:
```
```
500 ≤P 07. 01 ×P 07. 02 ≤ 1000
```
```
Forexample,whenthespeedloopgainP 07. 01 = 40. 0 Hz,thespeedloopintegratedtimeconstant
shouldmeet:
```
```
12. 50 ms≤P 07. 02 ≤ 25. 00 ms。
```
```
Decreasingthesetvaluecanstrengthentheintegraleffectandspeedupthepositioningtime,but
asetvaluethatistoosmallcaneasilycausemechanicalvibration.Ifthesettingvalueistoohigh,
thespeedloopdeviationwillneverbereturnedtozero.
```
```
WhenP 07. 02 = 512. 00 ms，integralisinvalid.
```
#### 3 P 07. 00

```
Position
loop
gain
```
```
◆Parametersfunction:
```
```
Determinesthemaximumfrequencyofpositioninstructionthatthepositionloopcanfollow.
```
```
Positionloopmaximumfollowingfrequency=P 07. 00
```
```
◆Adjustmentmethod:
```
```
Inordertoensuresystemstability,itshouldbeensuredthatthemaximumfollowingfrequencyof
speedloopis 3 - 5 timesthemaximumfollowingfrequencyofpositionloop.Therefore:
```
```
5
P 07. 00
```
```
2 P 07. 01
3 
```
```


```
```

```
```
Forexample,whenthespeedloopgainP 07. 01 = 40 .Hz,positonloopgainshouldmeet:
```
```
50. 2 Hz≤P 07. 00 ≤ 83. 7 Hz。
```
```
Accordingthepositiontimetoadjust,increasingthisparametercanacceleratepositioningtime
andimprovetheabilityofmotortoresistexternaldisturbancewhenitisstationary.
```
```
Ifthesettingvalueistoohighmaycausingsystemunstableandoccursvibration.
```

Chapter 5 Operationperformanceadjustment

#### 4 P 07. 04

```
Torque
comma
nd
```
```
filtering
```
```
◆Parametersfunction:
```
```
Eliminatehigh-frequencynoiseandsuppressmechanicalresonance.
```
```
◆Adjustmentmethod:
```
```
Itshouldbeensuredthatthecutofffrequencyofthetorqueinstructionfilterishigherthan 4 times
highestfollowingfrequencyofspeedloop,therefore:
```
```
(P 07. 01 ) 4
2 P 07. 04
```
```
1000
 

```
```
Forexample,whenthespeedloopgainP 07. 01 = 40. 0 Hz,thetorqueinstructionfilterc=time
constantshouldmeet:
```
```
P 07. 04 ≤ 1. 00 ms。
```
```
WhenvibrationoccurswhenP 07. 01 isincreased,thevibarationcanbesuppressedbyP 07. 04 ;
```
```
Ifthesettingvalueistoohigh,itwillcausetheresponsivemessdecreaseofcurrentloop.
```
```
Thevibrationinmotorshutdownstatusneedtobesuppressed,youcantrytoincreaseP 07. 01 ,
decreseP 07. 04 ;
```
```
Thevibrationofthemotoristoolargewhenitisstopped.Youcantrytoreducethesettingvalueof
P 07. 04.
```
**5. 3. 2 SpeedFeedforwardGain**

```
Figure 5. 5 Speedfeedforwardcontroloperationdiagram
```
Thevelocityforwardisonlyapplicabletopositioncontrolmode.Usingvelocityfeedforwardfunction

canimproveresponseofspeedcommand,reducethepositiondeviationatafixedspeed.

Theoperationstepsofspeedfeedforward:


Chapter 5 Operationperformanceadjustment

**a** ） **Settingspeedfeedforwardsignalsource**

SetP 07. 19 (Speedfeedforwardcontrolselection)toanon- 0 value,thespeedfeedforwardfunction

takeseffectandthecorrespondingsignalsourceisselected.

```
Function
code
```
```
Name Settingvalue Remark
```
#### P 07. 19

```
Velocity
feedforward
control
selection
```
```
0 - Nospeedfeedforward -
```
```
1 - Internalspeed
feedforward
```
```
Usethe speedinformation correspondingto the positioncommand
(encoderunit)asthesourceofthespeedfeedforwardsignal.
```
```
4 - EtherCTASpeed
feedforward EtherCTA
```
```
Usethe 60 B 1 inputfromthemasterstationasspeedfeedforward.
```
**b** ） **Setthespeedfeedforwardparameters**

Includingthespeedfeedforwardgain(P 07. 20 )andspeedfeedforwardfiltertimeconstant(P 07. 21 ).

```
Table 5. 3 Speedfeedforwardadjustment
```
```
Function
code Name
```
```
Adjustmentdescription
```
#### P 07. 20

```
Speed
```
```
feefforward
```
```
gain ◆Parametersfunction：
```
```
IncreasingP 07. 20 canimproveresponse,butmaygeneratespeedovershootduringaccelerationand
deceleration;
```
```
ReducingP 07. 21 ,cansuppressthespeedovershootoccurinaccelerationanddeceleration;Increasing
P 07. 21 cansuppressthenoisecausedbythepositioncommandupdatecyclebeinglongerthanthe
drivecontrolcycle,thepulsefrequencyofthepositioncommandbeinguneven,andsuppressingthejitter
ofthepositioningcompletionsignal;
```
```
◆Adjustmentmethod:
```
```
First,settingP 07. 21 toafixedvalue;thengraduallyincreaseP 07. 20 fromzerostart,untilthespeed
feedforwardtakeseffectatacertainsettingvalue.Whenadjusting,P 07. 20 andP 07. 21 shouldbe
adjustedrepeatedlythetofindasettingwithgoodbalance.
```
#### P 07. 21

```
Speed
```
```
feedforward
```
```
filter time
```
```
constant
```

Chapter 5 Operationperformanceadjustment

### 5 4 Torquefeedforward.........................................................................................................................

```
Figure 5. 6 Torquefeedforwardcontroloperationdiagram
```
**Onlyapplicabletonon-torquecontrolmodesituations.** Thepositioncontrolmodeusestorque

feedforward,whichcanimprovethetorquecommandresponseandreducethepositiondeviationat

fixedaccelerationanddeceleration;thespeedcontrolmodeusestorquefeedforward,whichcan

improvethetorquecommandresponseandreducethespeeddeviationatfixedspeed.

Torquefeedforwardfunctionoperationsteps:

**a** ） **Settingtorquefeeforwardsignalsource**

SetP 07. 22 (Torquefeedforwardcontrolselection)toanon- 0 value,thetorquefeedforwardfunction

takeseffectandthecorrespondingsignalsourceisselected.

```
Function
code
```
```
Name Settingvalue Remark
```
#### P 07. 22

```
Torque
feedforward
control
selection
```
```
0 - Notorquefeedforward -
```
```
1 - Internaltorquefeedforward
```
```
Usingtheaccelerationinformationcorrespondingtospeedinstruction
asthesourceofthetorquefeedforwardsignal.
```
```
Inpositioncontrolmode,thespeedinstructioncomesfromtheoutputof
thepositioncontroller.
```

Chapter 5 Operationperformanceadjustment

**b** ） **Settingtorquefeedforwardparameters**

```
Function
code Name Adjustmentdescription
```
#### P 07. 23

```
Torque
```
```
feedforwardgain
```
```
◆Parametersfunction:
```
```
IncreasingtheP 07. 23 canimprovetheresponse,butmayoccursovershootwhenaccelerate
anddecelerate；
```
```
ReducingtheP 07. 24 cansuppresstheovershootoccursinaccelerateanddecelerate;
IncreasingP 07. 24 cansuppressthenoise;
```
```
◆Adjustmentmethod:
```
```
Whenadjusting,firstkeepP 07. 24 asthedefaultvalue;thengraduallyincreasethe
```
```
settingvalueofP 07. 23 from 0 untilthetorquefeedforwardtakeseffectatacertainsetting
```
```
value.
```
```
Whenadjusting,P 07. 23 andP 07. 24 shouldbeadjustedrepeatedlytofindasettingwithgood
balance.
```
#### P 07. 24

```
Torque
```
```
feedforwardfilter
```
```
timeconstant
```
### 5 5 Commandfilteradjustment...........................................................................................................

**5. 5. 1 Positioncommandsmoothingfilter**

Settingthetimeconstantoftheoncedelayfilterforthepositioncommand.

```
Figure 5. 7 Positioncommandsmoothingfilterdiagram
```
☆Relatedfunctioncode:

Function
code

```
Name Settingrange Minimumunit Factory
setting
```
```
Effectivetime Category Mode
```
```
P 04 03 Positioncommand
smoothingfilter
```
```
0 ～ 65535 0. 1 ms 0 Effective
immediately
```
```
Shutdown
setting
```
#### P


Chapter 5 Operationperformanceadjustment

**5. 5. 2 PositioncommandFIRfilter**

SettingthetimeconstantofFIRfilterforpositioncommand.

```
Figure 5. 8 PositioncommandFIRfilterdiagram
```
Function
code Name Settingrange Minimumunit

```
Factory
setting Effectivetime Category
```
```
Mode
```
```
P 04 04 PositioncommandFIRfilter 0 .～ 1280 0. 1 ms 0
```
```
Effective
immediately
```
```
Shutdown
setting P
```
### 5 6 Adjustmentparametersindifferentmode...................................................................................

Parameters adjustment under differentcontrol mode must followthe consequenceof “Inertia

identification”==>“AutomaticGainAdjustment”==>“ManualGainAdjustment”.

**5. 6. 1 Adjustmentparametersinpositionmode**

1 ）Throughinertiaidentification,obtainloadinertiaratioP 00. 05 :

2 ）Thegainparameterinpositionmode:

①Thefirstgain:

```
Functioncode Name Function Defaultvalue
```
```
P 07. 00 Positionloopgain Setpositionloopproportionalgain 48. 0 Hz
```
```
P 07. 01 Speedloopgain Setspeedloopproportionalgain 50. 0 Hz
```

Chapter 5 Operationperformanceadjustment

```
P 07. 02 Speedloopintegraltimeconstant
```
```
Settheintegraltimeconstantofthespeed
```
```
loop
```
```
12. 00 ms
```
```
P 07. 04 Torquecommandfiltertimeconstant
```
```
Setthetorquecommandfiltertime
```
```
constant
```
```
1. 26 ms
```
②Thesecondgain:

```
Function
```
```
code
```
```
Name Function Defaultvalue
```
P 07. 05 Settheintegraltimeconstantofthespeedloop Setpositionloopproportionalgain 38. 0 Hz

P 07. (^06) Thesecondspeedloopgain Setspeedloopproportionalgain 18. 0 Hz
P 07. 07 Thesecondspeedloopintegraltimeconstant Settheintegraltimeconstantofthespeedloop 512. 00 ms
P 07. 09 Thesecondtorquecommandfiltertimeconstant Setthetorquecommandfiltertime
constant
1. 26 ms
P 07. 10 DIfunctionGAIN-SWITCHswitchingaction
selection
SettheGAIN-SWITCHswitchingaction
selection

#### 0

P 07. 11 Gainswitchingmode Setthegainswitchingcondition 0

P 07. 12 Gainswitchingdelay Setthedelaytimeforgainswitching 5. 0 ms

P 07. 13 Gainswitchinglevel Setsthelevelatwhichgainswitches 50

P 07. 14 Gainswitchinghysteresis Setthehysteresisofgainswitching 30

P 07. 15 Positiongainswitchingtime Settheswitchingtimeofthepositionloopgain 3. 0 ms


Chapter 5 Operationperformanceadjustment

PublicGain:

```
Function
```
```
code
```
```
Name Function
```
```
Default
```
```
value
```
```
P 07. 03 Speed​ feedbackfiltering Setthespeedfeedbackfiltertime 0. 00 ms
```
```
P 07. 16 Pseudo-differentialfeedforwardcontrol
```
```
coefficient
```
```
SettingthecoefficientsofthePDFF
```
```
controller
```
```
100. 0 %
```
```
P 07. 20 Speed​ ​ feedforwardgain Setthespeedfeedforwardgain 0. 0 %
```
```
P 07. 21 Speed​ ​ feedforwardfiltertimeconstant Setthefiltertimeconstantofthespeed
```
```
feedforwardsignal
```
```
0. 50 ms
```
```
P 07. 23 Torquefeedforwardgain Settingthetorquefeedforwardgain 0. 0 %
```
```
P 07. 24 Torquefeedforwardfiltertimeconstant Setthefiltertimeconstantofthetorque
```
```
feedforwardsignal
```
```
0. 50 ms
```
3 )BysettingtherigiditylevelofP 00. 04 andautomaticadjustmentgain,obtainthefirstgain(or

secondgain).Iftherigidconnectionequipmentoftheleadscrewandrackencounterswhistlingwhen

therigiditylevelisincreasing,canopentheinstantaneousspeedobserver(P 08. 39 ),inordertoimprove

rigiditylevel.

4 ）Mannuallyfine-tunethefollowinggains,ifthereareproblemssuchasmechanicalwhistling,

mechanicalshaking,andtipjitter,youcanusenotchandmodelcompensationcontroltooptimize:

```
Functioncode Name Function
```
```
P 07. 00 Positionloopgain Setthepositionloopproportionalgain
```
```
P 07. 01 Speedloopgain Setthespeedloopproportionalgain
```
```
P 07. 02 Speedloopintegraltimeconstant Settheintegraltimeconstantofthespeedloop
```
P 07. (^04) Torquecommandfiltertimeconstant Setthetorquecommandfiltertimeconstant


Chapter 5 Operationperformanceadjustment

```
P 07. 20 Speedfeedforwardgain Setthespeedfeedforwardgain
```
```
P 07. 21 Speed​ ​ feedforwardfiltertime Setthespeedfeedforwardcommandfilter
```
**5. 6. 2 Parametersadjustmentinthespeedmode**

Theparametersadjustmentinspeedcontrolmodeissameasthatinpositionmode,exceptforthe

positionloopgain(P 07. 00 ,P 07. 05 ),pleaseadjustaccordingto 5. 6. 1 “Parametersadjustmentinposition

mode”.

**5. 6. 3 Parametersadjustmentintorquecontrolmode**

Parametersadjustmentintorquecontrolmode:

Whentheactualspeedreachesthespeedlimitvalue,theadjustmentmethodisthesameas 6. 7. 2

"Parameteradjustmentinspeedmode";

Whentheactualspeeddonotreachesthespeedlimitvalue,exceptforspeedloopgain(P 07. 01 ,

P 07. 06 )andspeedloopintegraltimeconstant(P 07. 02 ,P 07. 07 ),theadjustmentmethodissameas

6. 5. 2 “Parameteradjustmentinspeedmode”.

### 5 7 Mechanicalresonancesuppression............................................................................................

Mechanical system has a certain resonance frequency. When the servo gain is increased,

resonancemay occurnearthemechanicalresonancefrequency,causingthegaintobeunableto

continuetoincrease.Therearetwowaystosuppressmechanicalresonance:

**1** ） **Torquecommandfilter(P 07. 04** ， **P 07. 09 )**

Bysettingfiltertimeconstant,thetorquecommandisattenuatedinhigh-frequencyabovethecutoff

frequencytoachievethepurposeofsuppressingmechanicalresonance.

**2** ） **Notchfilter:**

Thenotchfiltercanachievethepurposeofsuppressingmechanicalresonancebyreducingthegain

atspecificfrequencies.Aftercorrectlysetnotchfilter,thevibrationcangeteffectivelysuppress,andtryto


Chapter 5 Operationperformanceadjustment

increaseservogaincontinuously.TheprimcipleofNotchfilterisasshownbelow.

```
Figure 5. 9 Notchfilterprinciple
```
Theservodrivehastwosetsofnotches,eachofthemhave 3 parameters,dividedintoNotchfilter

frequency,widthlevelanddepthlevel.Thisnotchisamanualnotch,andeachparameterissetmanually

bytheuser.

**Notchfilterwidthanddepth**

Thenotchwidthisusedtoexpresstheratioofthenotchwidthtonotchcenterfrequency:

Thenotchwidth=

fH−fL

fT

Among:

fT：Notchcenterfrequency,thatistosay,mechanicalresonancefrequency

fH-fL：Thenotchwidthrepresentsthefrequencybandwidthwithanamplitudeattenuationrateof- 3 dB

relativetothenotchcenterfrequency.

Thenotchdepthlevelrepresentstherelationshipoftheratiooftheinputtooutputatcenter

frequency.Whenthenotchdepthlevelis 0 ,theinputiscompletelybesuppressedatcenterfrequency;


Chapter 5 Operationperformanceadjustment

Whenthenotchdepthlevelis 100 ,theinputiscompletelypassatcenterfrequency.Therefore,the

settinglevelofnotchdepthissmaller,thedepthofnotchismoredeeper,thesuppressiontomechanical

resonanceismorestronger,butthismaycauseunstablesystem,shouldbeattentionwhenusing.

```
Table 5. 4 Parameterssettingofnotch
```
```
Notchwidth
```
```
Bandwidth/Center
```
```
frequency
```
```
Notchdepth
```
```
Ratioofinputto
```
```
output
```
```
[dB]Gain
```
```
Represent
```
#### 0 0. 5 0 0 - ∞

#### 1 0. 59 1 0. 01 - 40

#### 2 0. 71 2 0. 02 - 34

#### 3 0. 84 3 0. 03 - 30. 5

#### 4 1 4 0. 04 - 28

#### 5 1. 19 5 0. 05 - 26

#### 6 1. 41 6 0. 06 - 24. 4

#### 7 1. 68 7 0. 07 - 23. 1

#### 8 2 8 0. 08 - 21. 9

#### 9 0. 09 - 20. 9

#### 10 0. 1 - 20

#### 15 0. 15 - 16. 5

#### 20 0. 2 - 14

#### 25 0. 25 - 12

#### 30 0. 3 - 10. 5


Chapter 5 Operationperformanceadjustment

#### 35 0. 35 - 9. 1

#### 40 0. 4 - 8

#### 45 0. 45 - 6. 9

#### 50 0. 5 - 6

#### 60 0. 6 - 4. 4

#### 70 0. 7 - 3. 1

#### 80 0. 8 - 1. 9

#### 90 0. 9 - 0. 9

#### 100 1 0

```
Figure 5. 9 Notchfrequencycharacteristic
```
**Notchusingsteps**

1 Analyzeresonancefrequency;

Whenusingamanualnotch,youneedtosetthefrenquencyofthenotchtotheactualresonant


Chapter 5 Operationperformanceadjustment

frequencythatoccurs.Themethodtoobtainresonancefrequency:BysettingP 08. 00 = 3 ,theresonant

frequencyisautomaticallytestedwhentheservoisrunning,andthetestresultsaresavedinP 08. 01.

Afterthetestiscompleted,besuretosetP 08. 00 to 0.

2 EntertheofEntertheresonantfrequencyobtainedinstep①intothenotchparametersofthe

selectedgroup,andenterthewidthlevelanddepthlevelofthenotchgroup.

3 Iftheresonanceissuppressed,itmeansthenotchiseffective,youcancontinuetoadjustthe

gain.Afterthegainisincreased,ifoccoursnewresonance,youcanrepeatsteps①~②;

④Ifthevibrationcannotbeeliminatedinalongtime,pleaseclosetheservofunctionintime.

☆Relatedfunctioncode:

```
Function
code
```
```
Name Settingrange Minimum
unit
```
```
Factory
setting
```
```
Effectivetime Category Mode
```
```
P 08 00 Adaptivefiltermode 0 ~ 5 1 0 Effective
immediately
```
```
Running
setting
```
#### PST

```
P 08 01 Resonancefrequency - 1 Hz 0 N/A
```
```
Display
parampara
```
#### PST

```
P 08 02 1 stnotchfilterfrequency
(manual)
```
```
10 ~ 4000 1 Hz 4000 Effective
immediately
```
```
Running
setting
```
#### PST

```
P 08 03 1 stnotchfilterwidth 0 ~ 8 1 2 Effective
immediately
```
```
Running
setting
```
#### PST

```
P 08 04 1 stnotchdepth 0 ~ 100 1 50
```
```
Effective
immediately
```
```
Running
setting
```
#### PST

```
P 08 05 2 ndnotchfilterfrequency
(manual)
```
```
10 ~ 4000 1 Hz 4000 Effective
immediately
```
```
Running
setting
```
#### PST

```
P 08 06 2 ndnotchfilterwidth 0 ~ 8 1 2 Effective
immediately
```
```
Running
setting
```
#### PST

```
P 08 07 2 ndnotchdepth 0 ~ 100 1 50
```
```
Effective
immediately
```
```
Running
setting
```
#### PST


## Chapter 6 AlarmandProcessing

## Chapter 6 AlarmandProcessing

Servodrivealarmlevelisdividedintotwolevels

```
Table 6 - 1 Alarmlevel
```
```
Alarmlevel Name Decription
```
```
Level 1 Fault
```
```
Theservodrivehasaseriousalarmandcannotworknormally.Itneedstobeshutdowntocheak.
```
```
DOterminaloutputsALMsingal.
```
```
Level 2 Warning
```
```
Theservodrivewarnsthatthedevicewillnotbedamagedforthetimebeing,butifitisnothandled
```
```
correctlyintime,itmaycauseahighlevelfaultoutput.
```
```
DOterminaloutputsWARNsingal.
```
### 6 1 Faultdiagnosisandtreatment......................................................................................................

Faultscanbedividedinto:

a) Non-resettableNO. 1 fault;

b) ResettableNO. 1 fault;

c) ResettableNO. 2 fault.

Amongthem,“Resettable”meansthataftertroubleshooting,P 11 - 01 canbeset to 1 or theDI

functioncanbeconfiguredFunIN. 2 ：ALM-RSTalarmresettocleartheservofaultstate.

“Non-resettable”meansthatafterthefaultishandled,driveneedstobere-powered.

TheNO. 1 andNO. 2 faultshavedifferentstopmodes.TheNO. 1 faultisfreetostopandkeeps

runningfreely.TheNO. 2 faultstopmodeissetbyP 00 - 12.

TheresetmethodofNO. 1 ,NO 2 resettablefault:firstturnofftheservoenablesignal(S-ONissetto

OFF),thensetP 11 - 01 = 1 oruseDIfunction 2.


Chapter 6 AlarmandProcessing

**6. 1. 1 Non-resettableNO. 1 fault**

```
Table 6 - 2 Non-resettableNO. 1 fault
```
```
Faultnumber
```
```
Er_
```
```
Faultname Causeoffault
```
```
Stop
```
```
method
```
```
whenfault
```
```
occurs
```
```
Whetherthe
```
```
faultcanbe
```
```
reset
```
```
1 Systemparameterexception Abnormaldataofservointernal parameters No. 1 NO
```
```
2 Productmodelselectionfailure Invaliddrivemodel No. 1 NO
```
```
3 Motormodelselectionfailure P 01. 00 FunctioncodeSetaninvalidmotorcode No. 1 NO
```
```
4 Faultinparameterstorage
```
```
1. Parameterstoragedevicefailure
2. Parametersreadandwritetoofrequently
3. Controlpowersupplyinstability
4. Drivefailure
```
```
No. 1 NO
```
```
5 FPGAfault
```
```
1 ． FPGAinitializationexception
2 ． FPGAlogicversionisabnormal
3 ． FPGAdetectedanexception
```
```
No. 1 NO
```
```
6 Encodermatchingfault
```
```
Inabsolutevaluesystemmode(P 00. 06 ≠ 0 ),an
```
```
incrementalorsingle-turnabsolutevalue
```
```
encodermotorisused
```
```
No. 1 NO
```
#### 7

```
Controlpowersupply
```
```
undervoltage
```
```
Controlcircuitpowersupplyundervoltage No. 1 NO
```
```
9 OvercurrentA
```
```
1. Thedriveormotorparametersareincorrect
2. UVWphase-to-phaseshortcircuit
3. Motorburnedout
4. Motorisshortedtoground
5. Drivefailure
```
#### NO. 1 NO

```
10 OvercurrentB
```
```
1. Servomotorwiringisnotnormal
2. Softwaredetectstheovercurrentofpower
transistor
```
```
No. 1 NO
```
```
11 Encoderdisconnection
```
```
IncrementalencoderA/B/Zthree-phaseline
```
```
break
```
```
No. 1 NO
```
```
13 Encodercheckexception
```
```
1 .Thebusencoderdatacheckisabnormal
```
```
2 .Theincrementalencoderzerocheckis
abnormal
```
```
No. 1 NO
```
#### 14

```
Motorinitialangledetection
```
```
abnormal
```
```
Theencodersignalinterfereswhenthe
```
```
incrementalencoderispoweredon
```
```
No. 1 NO
```
```
15 Motoroutofcontrol
```
```
1. MotorUVWphasesequencesettingerror
2. UVWwiringerror
3. Initialpositionorencoderparametersetting
error
```
```
No. 1 NO
```
```
16 Currentsamplingfault ThecurrentA/Dsamplingcircuitisfaulty No. 1 NO
```
```
18 Motordataverificationfailure
```
```
1. Nomotorparametersarewrittenintothe
encoderEEPROM
2. Motorparameterverificationerror
```
```
No. 1 NO
```

Chapter 6 AlarmandProcessing

**6. 1. 2 ResettableNO. 1 fault**

```
Table 6 - 3 ResettableNO. 1 fault
```
```
Faultnumber
```
```
Er_
```
```
Faultname Causeoffault
```
```
Stopmethod
```
```
whenfault
```
```
occurs
```
```
Whetherthe
```
```
faultcanbe
```
```
reset
```
```
20 Overvoltage ThemaincircuitDCvoltageabnormallyhigh. No. 1 Yes
```
```
21 Undervoltage ThemaincircuitDCvoltagelow. No. 1 Yes
```
```
22 Overspeed
```
```
1. Speedcommandexceedsthemaximum
speedvalue
2. UVWphasesequenceerror
3. Speedresponsetoohigh,severely
exceedadjustmentrange
4. Drivefailure
```
```
No. 1 Yes
```
#### 25

```
Abnormalmotorrotation
```
```
duringbootstrapcharging
```
```
Abnormalmotorrotationduringbootstrap
```
```
charging
```
```
No. 1 Yes
```
#### 27

```
DIterminalparameter
```
```
settingfault
```
```
DifferentDIareassignedwithsame
```
```
functionrepeatedly.
```
```
No. 1 Yes
```
#### 28

```
DOterminalparameter
```
```
settingfault
```
```
DifferentDOareassignedwithsameoutput. No. 1 Yes
```
#### 30

```
Referenceposition
```
```
failure
```
```
WhenusingthePTPfunction,P 04. 00 = 5
```
```
isnotset.
```
```
No. 1 Yes
```
**6. 1. 3 ResettableNO. 2 fault**

```
Table 6 - 4 ResettableNO. 2 fault
```
```
Faultnumber
```
```
Er_
```
```
Faultname Causeoffault
```
```
Stop
```
```
method
```
```
whenfault
```
```
occurs
```
```
Whether
```
```
thefault
```
```
canbe
```
```
reset
```
#### 43

```
Positiondeviationistoo
```
```
large
```
```
IntheservoONstate,thepositiondeviation
```
```
exceedsthepositiondeviationvalue(P 09. 09 )
```
```
No. 2 Yes
```
```
46 Driveoverload
```
```
1 .Theloadoperationexceedstheinversetime
```
```
curveofthedrive.
```
```
2 .TheUVWoutputmaybelackofphaseor
```
```
phasesequenceconnectedincorrectly.
```
```
No. 2 Yes
```
```
47 Motoroverload
```
```
1 .Theloadoperationexceedstheinversetime
```
```
curveofthemotor.
```
```
2 .TheUVWoutputmaybelackofphaseor
```
```
phasesequenceconnectedincorrectly.
```
```
N 0. 2 Yes
```
```
48 Motorstall
```
```
1 .Themechanicalpositionisstuck,causing
```
```
themotorcurrenttocontinuouslyincrease
```
```
abnormally.
```
```
N 0. 2 Yes
```

Chapter 6 AlarmandProcessing

**Faultnumber**

```
Er_
```
```
Faultname Causeoffault
```
```
Stop
```
```
method
```
```
whenfault
```
```
occurs
```
```
Whether
```
```
thefault
```
```
canbe
```
```
reset
```
```
2 .Inconsistentresponseofdual-drivemotors
```
```
withgantrystructure.
```
#### 49

```
Electronicgearsetting
```
```
error
```
```
Electronicgearratioexceedsspecification
```
```
range[ 0. 001 , 4000 ]
```
```
No. 2 Yes
```
#### 50

```
Heatsink(fan)
```
```
overheating
```
```
Servounitheatsink(fan)exceedsthesetfault
```
```
value
```
```
No. 2 Yes
```
```
51 Encoderbatteryfailure Nobatteryorbatteryvoltagebelow 2. 6 V No. 2 Yes
```
#### 52

```
Encodermulti-turn
```
```
counterror
```
```
Absoluteencodermulti-turncounterror No. 2 Yes
```
#### 53

```
Encodermulti-turn
```
```
countoverflow
```
```
Absoluteencodermulti-turncountoverflow No. 2 Yes
```
```
54 Softlimitsettingerror
```
```
1 .Thepositionbetweenpositive/negative
```
```
limitistooshort
```
```
2 .Whenthemotorrunsinthe
```
```
counterclockwisedirectionasthepositive
```
```
direction,thepositivelimitvalueissmaller
```
```
thanthenegativelimitvalue
```
```
3 .Whenthemotorrunsintheclockwise
```
```
directionasthepositivedirection,the
```
```
negativelimitvalueissmallerthanthe
```
```
positivelimitvalue
```
```
No. 2 Yes
```
### 6 2 Reasonsforwarningandtreatment............................................................................................

```
Table 6 - 5 Reasonsforwarningandtreatment
```
```
Faultnumber
```
```
Er_
```
```
Warningname Causeofwarning
```
```
81 Driveoverloadwarning Warningwhen 80 %ofthedriveoverloadlimitvalueisreached.
```
```
82 Motoroverloadwarning
```
```
Thewarningbeforethemotorisabouttofail,thewarningvalueis
```
```
determinedbyP 09 - 05.
```
#### 83

```
Changedparametersneedto
```
```
bere-poweredtotakeeffect
```
```
Changedparametersthatneedtobere-powered.
```
```
84 Resetencoderwarning
```
```
Inenabledstate,resetsencoderwhenhostcomputersending
```
```
commands.
```
```
86 Forwardovertravelwarning
```
```
PositiveovertravelswitchPOTterminalisvalid.
```
```
Themotorpositionexceedsthemaximumsoftlimit.
```
```
87 Negativeovertravelwarning
```
```
NegativeovertravelswitchNOTterminalisvalid.
```
```
Themotorpositionexceedsthesoftlimitminimum.
```

Chapter 6 AlarmandProcessing

**Note:** Theresetmethodofwarning:setP 11 - 01 = 1 oruseDIfunction 2.

#### 88

```
Dividedpulseoutputsetting
```
```
fault
```
```
Thenumberofencoderdividedpulsesdoesnotmeetsetconditionor
```
```
range.
```
```
89 AI 1 \AI 2 zerodrifttoolarge AI 1 \AI 2 zerodrifttoolarge.
```
#### 90

```
Externalregenerativeresistor
```
```
overload
```
```
Externalregenerativeresistorpoweristoosmall.
```
#### 91

```
Externalregenerativeresistor
```
```
valueistoosmall
```
```
Theexternalregenerativeresistorislessthantheminimumrequirement
```
```
bythedrive;
```
```
Orparametersettingerror.
```
```
94 DIemergencybrake ExternalemergencybrakeE_STOPterminaltrigger.
```
```
95 Absoluteencoderbatterylow Batteryvoltageislowerthan 3. 2 V.
```
```
96 Homingtimeout
```
```
1 .Homeswitchfailure.
```
```
2 .Timelimittofindhomeistooshort.
```
```
3 .Thehighspeedforsearchinghomesignalistoosmall.
```
#### 97

```
Mechanicalhomepoint
```
```
offseterror
```
```
1 .WhenthehomingmodeparameterP 16 - 09 = 6 orP 16 - 09 = 8 or
```
```
P 16 - 09 = 14 ,themechanicalhomeoffsetparameterP 16 - 14 issettoa
```
```
valuegreaterthan 0.
```
```
2 .WhenthehomingmodeparameterP 16 - 09 = 7 orP 16 - 09 = 9 orP 16 -
```
```
09 = 15 ,themechanicalhomeoffsetparameterP 16 - 14 issettolessthan
```
```
0.
```
#### 99

```
Multi-segmentposition
```
```
referencepositionfault
```
```
1 .Inthemulti-segmentabsolutepositionoperationmode,thesystem
```
```
doesnotperformthehomingoperationorthedriveisnotsettothe
```
```
absolutevaluesystem.
```
```
2 .Inthemulti-segmentincrementalpositionoperationmode,whenthe
```
```
functioncodeofP 13. 07 issetto 1 ,thesystemdoesnotperformthezero
```
```
returnoperation.
```
#### 100

```
Thehomeingmodeis
```
```
incorrectlyset
```
```
Thehomeingmodeisset 0 ,Orthemodeisnotsupported.
```

## Chapter 7 ParameterList

## Chapter 7 ParameterList

### 7 1 Parametergroupnumber..............................................................................................................

```
Parametergroup
number
```
```
Parametergroupdescription
```
```
P 00 Basiccontrolparameters
```
```
P 01 Servomotorparameters
```
```
P 02 Digitalinputandoutputparameters
```
```
P 04 Positioncontrolparameters
```
```
P 05 Speedcontrolparameters
```
```
P 06 Torquecontrolparameters
```
```
P 07 Gainparameters
```
```
P 08 Advancedadjustmentparameters
```
```
P 09 Faultandprotectionparameters
```
```
P 10 Communicationparameters
```
```
P 11 Auxiliaryfunctionparameters
```
```
P 12 Keyboarddisplayparameters
```
```
P 13 Multi-segmentpositionfunctionparameters
```
```
P 14 Multi-speedfunctionparameters
```
```
P 16 Specialfunctionparameters
```
```
P 17 Driveparameters
```
```
P 18 Displayparameters
```
### 7 2 Eachgroupofparameters.............................................................................................................

**P 00 group:basiccontrolparameters**

```
Function
code
```
```
Name Settingrange Min.
unit
```
```
Default
setting
```
```
Effective
time
```
```
Type Mode
```
```
P 00 00 Controlmode
selection
```
```
0 - positionmode
1 - speedmode
2 - torquemode
```
```
1 0 Effectiveimm
ediately
```
```
Setting
after
motorstop
```
#### PST

```
P 00 01 Directionof
rotation
```
```
0 - CCWdirectionistheforward
direction
(counterclockwise)
1 - CWdirectionistheforward
direction
(clockwise)
```
```
1 0 Re-power
```
```
Setting
after
motor
stop
```
#### PST

#### P 00 02

```
Pulseoutput
positive
direction
definition
```
```
0 - CCWdirectionisforwarddirection
(thecorrespondingmotorrotation
directionwhenthepulseoutputOA
leadstheOB)
1 - CWdirectionisforwarddirection
(reversemode,OAlagOB)
```
```
1 0 Re-power
```
```
Setting
after
motorstop
```
#### PST

```
P 00 04 Rigiditylevel
setting
```
```
0 ~ 31 1 11 Effective
immediately
```
```
Setwhen
running
```
#### PST

```
P 00 05 Inertiaratio 0 ~ 3000 0. 01 100 Effective
immediately
```
```
Setwhen
running
```
#### PST


Chapter 7 ParameterList

```
Function
code Name Settingrange
```
```
Min.
unit
```
```
Default
setting
```
```
Effective
time Type Mode
```
#### P 00 06

```
Absolutevalue
systemselection
```
```
0 - incrementalpositionmode
1 - Absolutepositionlinearmode
（Ifhomingcompletedwhenthe
absolutevalueencoderisused,
powerontheencoderimmediately.If
thevalueof 6064 isnot 0 afterthe
encoderispoweredon,setthe
absolutevaluemodetothismode.）
2 - Absolutevalueposition
rotationmode
（Maskmulti-circledataoverflow
errors,Inthemulti-turndata
```
- 32768 ~ 32767 hasbeenrunning
cycle.）

```
1 0 Re-power
```
```
Setting
after
motorstop
```
#### PST

#### P 00 07

```
System
maximumspeed
```
```
0 ~ 10000 1 rpm 4500
```
```
Effective
immediately
```
```
Setting
after
motorstop
```
#### PST

P 00 10 ServoOFFstop
mode

```
0 - Freewheelstop,keepfreerunning
state
1 - Stopatzerospeed(deceleration
timeiscontrolledbyP 05. 06 ),
keepfreerunningstate
```
```
1 1 Effective
immediately
```
```
Setting
after
motorstop
```
#### PST

#### P 00 11

```
FaultNo. 1 stop
modeselection
```
```
0 - Freewheelstop,keepfreerunning
state
1 - Resaved
```
#### 1 0

```
Effective
immediately
```
```
Setting
after
motorstop
```
#### PST

#### P 00 12

```
FaultNo. 2 stop
modeselection
```
```
0 - Freewheelstop,keepfreerunning
state
1 - Stopatzerospeed(deceleration
timeiscontrolledbyP 05. 06 ),
keepfreerunningstate
```
#### 1 1

```
Effective
immediately
```
```
Setting
after
motorstop
```
#### PST

P 00 13 Stopmode
whenovertravel

```
0 - freerunningstop.
```
```
1 - Deceleratethemotorwiththe
```
```
settorqueoftheemergencystop
```
```
torqueasthemaximumvalue,
```
```
andthenentertheservolock
```
```
state.
```
```
2 - Deceleratethemotorwiththe
```
```
settorqueoftheemergency
```
```
stoptorqueasthemaximum
```
```
value,andthenenterfree
```
```
runningstate.
```
```
1 1 Effective
immediately
```
```
Setting
after
motorstop
```
#### PST

#### P 00 14

```
Brakeoutput
ONtocommand
receivingdelay
```
```
0 ～ 10000 1 ms 200 Effective
immediately
```
```
Setwhen
running
```
#### PST

#### P 00 15

```
Intherotation
state,No. 2 fault
occurs,orthe
servoenableis
OFF,thebrake
outputOFF
delay;
Inthestatic
state,brake
outputOFF
tothemotornot
energizeddelay.
```
```
10 ～ 10000 1 ms 200
```
```
Effective
immediately
```
```
Setwhen
running PST
```
#### P 00 16

```
Rotatingstate,
thespeed
thresholdwhen
thebrakeoutput
isOFF
```
```
0 ～ 1000 1 rpm 50
```
```
Effective
immediately
```
```
Setwhen
running PST
```

Chapter 7 ParameterList

```
Function
code Name Settingrange
```
```
Min.
unit
```
```
Default
setting
```
```
Effective
time Type Mode
```
#### P 00 17

```
NO. 1 fault
occursin
rotatingstate,
delaybetween
servoenable
OFFandbrake
outputOFF
```
```
0 ～ 10000 1 ms 500 Effective
immediately
```
```
Setwhen
running
```
#### PST

#### P 00 18

```
Energy
consumption
resistance
setting
```
```
0 - Usebuilt-inpowerconsumption
```
```
resistor
```
```
1 - Useexternalpower-consuming
```
```
resistorsandcoolnaturally
```
```
2 - Useexternalpower-consuming
```
```
resistorsandforceair-cooled
```
```
3 - Noenergyconsumptionresistor,
```
```
relyingoncapacitance
```
```
Absorption(Thebraketubeisalways
```
```
closedatthistime)
```
```
1 0 Effective
immediately
```
```
Setting
after
motorstop
```
#### PST

#### P 00 19

```
Externalresistor
powercapacity
```
#### 1 ～ 65535 1 W

```
Model
parame
t
ers
```
```
Effective
immediately
```
```
Setting
after
motorstop
```
#### PST

#### P 00 20

```
External
resistancevalue^1 ～^10001 Ω
```
```
Model
parame
ters
```
```
Effective
immediately
```
```
Setting
after
motorstop
```
#### PST

#### P 00 21

```
External
resistance
heating
timeconstant
```
```
1000 ~ 65535 1 ms
```
```
Model
parame
t
ers
```
```
Effective
immediately
```
```
Setting
after
motorstop
```
#### PST

#### P 00 22

```
Energy
consumption
brakingstart
voltage
```
#### 0 ~ 1000 1 V

```
Model
parame
t
ers
```
```
Effective
immediately
```
```
Setwhen
running PST
```
#### P 00 37

```
Pulseincrement
threshold^0 ～^20011
```
```
Effective
immediately
```
```
Setwhen
running PS
```
#### P 00 38

```
Continuous
pulseless
receptioncycle
number
```
```
1 ～ 200 1 3 Effective
immediately
```
```
Setwhen
running
```
#### PS

**P 01 group:Servomotorparameters**

```
Function
code
```
```
Name Settingrange Min.unit Default
setting
```
```
Effective
time
```
```
Type Mode
```
#### P 01 00

```
Motormodel
code^0 ~^655351101 Re-power
```
```
Settingafter
motorstop PST
```
#### P 01 01

```
Motorpowerline
phasesequence
direction
```
```
0 ：CCW(Theopposite
electromotiveforceUisahead
ofV)
0 ：CCW(Theopposite
electromotiveforceVisahead
ofU)
```
```
1 0 Re-power
```
```
Settingafter
motorstop PST
```
```
P 01 02 Ratedvoltage 1 - 1000 1 48 Re-power
```
```
Settingafter
motorstop PST
```
```
P 01 03 Ratedpower 0 ~ 65535 0. 01 Kw Re-power Settingafter
motorstop
```
#### PST

```
P 01 04 Ratedcurrent 1 ～ 10000 0. 01 A Re-power
```
```
Settingafter
motorstop PST
```
```
P 01 05 Ratedtorque 0 ~ 65535 0. 01 Nm Re-power
```
```
Settingafter
motorstop
```
#### PST

```
P 01 08 Maxspeed 0 ~ 9000 1 rpm Re-power Settingafter
motorstop
```
#### PST

```
P 01 09 Rotorinertia 0 ~ 10000
```
#### 0. 01

```
kgcm^2 Re-power
```
```
Settingafter
motorstop PST
```

Chapter 7 ParameterList

```
Function
code Name Settingrange Min.unit
```
```
Default
setting
```
```
Effective
time Type Mode
```
#### P 01 10

```
Numberofpole
pairsof
Permanent
magnet
synchronous
motor
```
#### 1 ~ 50

```
1 pole
pair Re-power
```
```
Settingafter
motorstop PST
```
#### P 01 11

```
statorresistance
Rs^1 ~^655350.^001 Ω Re-power
```
```
Settingafter
motorstop PST
```
```
P 01 12 Q-axis
inductanceLq
```
```
1 ~ 65535 0. 01 mH Re-power Settingafter
motorstop
```
#### PST

#### P 01 13

```
D-axis
inductanceLd^1 ~^655350.^01 mH Re-power
```
```
Settingafter
motorstop PST
```
```
P 01 14 BackEMF 1 ~ 65535
```
#### 0. 01

```
mV/rpm Re-power
```
```
Settingafter
motorstop PST
```
```
P 01 15 Torquefactor 1 ~ 65535 0.^001
Nm/A
```
```
Re-power Settingafter
motorstop
```
#### PST

```
P 01 18 Encoder
selection
```
```
0 : 2500 linesencoder
1 : 17 bitincrementalencoder
2 : 17 bitabsoluteencoder
3 : 23 bitincremental
encoder
4 : 23 bitabsoluteencoder
```
```
Re-power Settingafter
motorstop
```
#### PST

```
P 01 20 Encoder
resolution
```
```
1 ~ 1073741824 Re-power Settingafter
motorstop
```
#### PST

#### P 01 22

```
Zcorresponding
Electricalangle^0 ~^36000.^1
```
```
o Re-power Settingafter
motorstop PST
```
#### P 01 23

```
Urisingedge
corresponding
electricalangle
```
```
0 ~ 3600 0. 1 o Re-power
```
```
Settingafter
motorstop PST
```
```
P 01 24 FPGAupload
motormodel
```
```
0 ～ 65535 Effective
immediately
```
```
Onlyread
parameter
```
#### PST

**P 02 group:digitalterminalinputandoutputparameters**

```
Function
code
```
```
Name Settingrange Min.
unit
```
```
Default
setting
```
```
Effective
time
```
```
Type Mode
```
#### P 02 00

```
FunINLsignal
unassigned
state(HEX)
```
```
0 ~ 0 xFFFF
Bit 0 - correspondstoFunIN. 1 ;
Bit 1 - correspondstoFunIN. 2 ;
......
Bit 15 - correspondstoFunIN. 16
```
```
1 0 Re-power
```
```
Setwhen
running PST
```
#### P 02 01

```
DI 1 terminal
function
selection
```
```
Inputfunctioncode: 0 , 1 - 45
0 :nodefinition
1 ~ 45 :FunIN. 1 to 45 (refertothe
DIDObasicfunction
codingtable)
```
```
1 1 Effective
immediately
```
```
Set
when
running
```
#### PST

#### P 02 02

```
DI 2 terminal
function
selection
```
```
Inputfunctioncode: 0 , 1 - 45
0 :nodefinition
1 ~ 45 :FunIN. 1 to 45 (refertothe
DIDObasicfunction
codingtable)
```
#### 1 2

```
Effective
immediately
```
```
Set
when
running
```
#### PST

#### P 02 03

```
DI 3 terminal
function
selection
```
```
Inputfunctioncode: 0 , 1 - 45
0 :nodefinition
1 ~ 45 :FunIN. 1 to 45 (refertothe
DIDObasicfunction
codingtable)
```
```
1 10 Effective
immediately
```
```
Set
when
running
```
#### PST

#### P 02 10

```
FunINHsignal
unassigned
state(HEX)
```
```
0 ~ 0 xFFFF
```
```
Bit 0 - correspondstoFunIN. 17 ;
```
```
Bit 1 - correspondstoFunIN. 18 ;
```
```
......
```
```
Bit 15 - correspondstoFunIN. 32
```
```
1 0 Re-power
```
```
Setting
after
motor
stop
```
#### PST


Chapter 7 ParameterList

```
Function
code
```
```
Name Settingrange Min.
unit
```
```
Default
setting
```
```
Effective
time
```
```
Type Mode
```
P 02 11 DI^1 terminal
logicselection

```
Inputpolarity: 0 - 4
```
```
0 - lowleveleffective
```
```
1 - highleveleffective
```
```
2 - risingedgeisvalid
```
```
3 - fallingedgeisvalid
```
```
4 - risingandfallingedgesare
```
```
valid
```
```
1 0 Effective
immediately
```
```
Set
when
running
```
#### PST

#### P 02 12

```
DI 2 terminal
logic
selection
```
```
Inputpolarity: 0 - 4
```
```
0 - lowleveleffective
```
```
1 - highleveleffective
```
```
2 - risingedgeisvalid
```
```
3 - fallingedgeisvalid
```
```
4 - risingandfallingedgesare
```
```
valid
```
```
1 0 Effective
immediately
```
```
Set
when
running
```
#### PST

#### P 02 13

```
DI 3 terminal
logic
selection
```
```
Inputpolarity: 0 - 4
```
```
0 - lowleveleffective
```
```
1 - highleveleffective
```
```
2 - risingedgeisvalid
```
```
3 - fallingedgeisvalid
```
```
4 - risingandfallingedgesare
```
```
valid
```
#### 1 0

```
Effective
immediately
```
```
Set
when
running
```
#### PST

#### P 02 21

```
DO 1 terminal
function
selection
```
```
Outputcode： 1 ～ 25
```
```
0 ：nodefinition
```
```
1 ～ 25 ：FunOUT. 1 ～ 25 refer
```
```
toDIDOfunctionselection
```
```
codedefinition
```
#### 1 1

```
Effective
immediately
```
```
Setting
after
motor
stop
```
#### PST

#### P 02 22

```
DO 2 terminal
function
selection
```
```
TheDOcanonlybesetto
```
```
FunOUT 11 (Lockreleasesignal
```
```
output)
```
#### 1 11

```
Effective
immediately
```
```
Setting
after
motor
stop
```
#### PST

#### P 02 31

```
DO 1 terminal
logiclevel
selection
```
```
Outputpolarityinversion
```
```
setting: 0 - 1
```
```
0 :conductwhenactive
```
```
(normallyopen)
```
```
1 :notconductwhenactive
```
```
(normallyclosed)
```
#### 1 0

```
Effective
immediately
```
```
Setting
after
motor
stop
```
#### PST

#### P 02 32

```
DO 2 terminal
logiclevel
selection
```
```
Outputpolarityinversion
```
```
setting: 0 - 1
```
```
0 :conductwhenactive
```
```
(normallyopen)
```
```
1 :notconductwhenactive
```
```
(normallyclosed)
```
#### 1 0

```
Effective
immediately
```
```
Setting
after
motor
stop
```
#### PST


Chapter 7 ParameterList

**P 04 group:positioncontrolparameters**

```
Function
code Name Settingrange
```
```
Min.
unit
```
```
Default
setting
```
```
Effective
time Type Mode
```
#### P 04 00

```
Mainposition
commandA
source
```
```
0 - lowspeedpulsecommand
1 - highspeedpulsecommand
2 - stepamountgiven
4 - multi-segmentpositioncommand
given
5 - giventhroughcommunication
```
#### 1 0

```
Effective
immediately
```
```
Settingafter
Motorstop P
```
```
P 04 02 Stepamount - 9999 ～ 9999 1 Unit 50
```
```
Effective
immediately
```
```
Settingafter
Motorstop P
```
#### P 04 03

```
Position
command
smoothingfilter
```
```
0 ～ 65535 0. 1 ms 0
```
```
Effective
immediately
```
```
Settingafter
Motorstop P
```
#### P 04 04

```
Position
command
FIRfilter
```
```
0 ～ 1280 0. 1 ms 0
```
```
Effective
immediately
```
```
Setting
after
motor
stop
```
#### P

#### P 04 05

```
Numberof
instruction
unitrequired
foroneround
( 32 - bit)(Only
PTP)
```
```
16 ～ 1073741824 1 Unit/T
urn
```
```
0 Re-power
```
```
Setting
after
motor
stop
```
#### P

#### P 04 07

```
Electronicgear
1
numerator( 32
bit)
```
#### 1 ～ 1073741824 1

```
Motor
resoluti
on
```
```
Effective
immediately
```
```
Set
when
running
```
#### P

#### P 04 09

```
Electronicgear
1
denominator
( 32 bit)
```
#### 1 ～ 1073741824 1 10000

```
Effective
immediately
```
```
Set
when
running
```
#### P

#### P 04 11

```
Electronicgear
2
numerator( 32
bit)
```
#### 1 ～ 1073741824 1

```
Motor
resoluti
on
```
```
Effective
immediately
```
```
Set
when
running
```
#### P

#### P 04 13

```
Electronicgear
2
denominator
( 32 bit)
```
```
1 ～ 1073741824 1 10000 Effective
immediately
```
```
Set
when
running
```
#### P

```
P 04 21 Pulseshape
```
```
0 - pulse+direction,positive
logic.(Defaults)
1 - direction+pulse,negativelogic
2 - Aphase+Bphaseorthogonal
pulse,positivelogic
3 - Aphase+Bphaseorthogonal
pulse,negativelogic
4 - CCW+CW,positivelogic
5 - CCW+CW,negativelogic
```
```
1 0 Re-power
```
```
Setting
after
motor
stop
```
#### P

#### P 04 22

```
Position
deviation
clearfunction
```
```
0 - Clearpositiondeviationpulse
whenfaultoccursorservoOFF
1 - Clearthepositiondeviation
pulseonlywhenafaultoccurs
2 - ClearedbyDIinputfunction
(PERR-CLR)
```
```
1 0 Effective
immediately
```
```
Setting
after
motor
stop
```
#### P

#### P 04 23

```
Positioning
complete
(COIN)output
```
```
0 - Outputwhentheabsolute
positiondeviationvalueissmaller
thanpositioningcompletionrange.
1 - Outputwhentheabsolute
positiondeviationvalueissmaller
thanpositioningcompletionrange
andthefilteredpositioncommand
is 0.
2 - Outputwhentheabsolute
positiondeviationvalueissmaller
thanpositioningcompletionrange
andpositioncommandis 0.
```
#### 1 0

```
Effective
immediately
```
```
Setting
after
motor
stop
```
#### P


Chapter 7 ParameterList

```
Function
code
```
```
Name Settingrange Min.
unit
```
```
Default
setting
```
```
Effective
time
```
```
Type Mode
```
#### P 04 24

```
Positioning
completion
range
```
#### 1 ～ 65535 1 P

```
2500 line
motor: 7 P
17 - bit
motor:
100 P
23 - bit
motor:
1000 P
```
```
Effective
immediately
```
```
Setting
after
motor
stop
```
#### P

```
P 04 25 Positioning
closerange
```
```
1 ～ 65535 1 P 65535 Effective
immediately
```
```
Setting
after
motor
stop
```
#### P

**P 05 group:speedcontrolparameters**

```
Function
code Name Settingrange
```
```
Min.
unit
```
```
Default
setting
```
```
Effective
time Type Mode
```
#### P 05 00

```
Mainspeed
commandA
source
```
```
0 - digitvalue(P 0503 )
1 - reserved
2 - reserved
```
#### 1 0

```
Effective
immediately
```
```
Setting
after
motor
stop
```
#### S

#### P 05 01

```
Auxiliaryspeed
commandB
source
```
```
0 - digitvalue(P 0503 )
1 - reserved
2 - reserved
3 - multi-speedcommand
```
#### 1 3

```
Effective
immediately
```
```
Setting
after
motor
stop
```
#### S

#### P 05 02

```
Speedcommand
selection
```
```
0 - mainspeedcommandA
source
2 - auxiliaryspeedcommandB
source
3 - A/Bswitching
```
#### 1 0

```
Effective
immediately
```
```
Setting
after
motor
stop
```
#### S

#### P 05 03

```
Speedcommand
keyboardsetting -^9000 ～^90001 rpm^200
```
```
Effective
immediately
```
```
Settingwhen
running S
```
```
P 05 04 Jogspeedsetting 0 ～ 9000 1 rpm 200 Effective
immediately
```
```
Settingwhen
running
```
#### S

```
P 05 05 Speedcommand
accelerationtime
```
```
0 ～ 10000 1 ms 50 Effective
immediately
```
```
Reserved S
```
```
P 05 06 Speedcommand
decelerationtime
```
```
0 ～ 10000 1 ms 50 Effective
immediately
```
```
Reserved S
```
```
P 05 07 Speedcommand
limitselection
```
```
0 - forwardP 05. 08 ,reverse
P 05. 09 ,internallimit(default)
1 - reserved
2 - reserved
```
```
1 0 Effective
immediately
```
```
Settingwhen
running
```
#### S

```
P 05 08 Forwardspeed
limit
```
```
0 ～ 9000 1 rpm 6000 Effective
immediately
```
```
Settingwhen
running
```
#### S

```
P 05 09 Backwardspeed
limit
```
```
0 ～ 9000 1 rpm 6000 Effective
immediately
```
```
Settingwhen
running
```
#### S

#### P 05 14

```
Speeddirection
selection
```
```
0 - directionunchanged
1 - directionreversal
2 - directionisdeterminedbyDI
function 25
3 - directionisdeterminedbyDI
function 40 / 41
```
#### 1 2

```
Effective
immediately
```
```
Setting
after
motor
stop
```
#### S

#### P 05 15

```
Zerofixedspeed
fixedvalue
```
```
0 ～ 6000 1 rpm 10
```
```
Effective
immediately
```
```
Settingwhen
running
```
#### S

#### P 05 16

```
Speedthreshold
of
motorrunning
signaloutput
```
```
0 ～ 1000 1 rpm 20
```
```
Effective
immediately
```
```
Settingwhen
running
```
#### PS

```
P 05 17 Speeduniform
signalwidth
```
```
0 ～ 100 1 rpm 10 Effective
immediately
```
```
Settingwhen
running
```
#### PS

#### P 05 18

```
Speedreaches
thespecified
value
```
```
0 ～ 6000 1 rpm 1000
```
```
Effective
immediately
```
```
Settingwhen
running PST
```
#### P 05 20

```
Zerospeed
judgment
threshold
```
```
0 ～ 6000 1 rpm 10 N/A
```
```
Settingwhen
running PST
```
**P 06 group:torquecontrolparameters**

```
Function
code
```
```
Name Settingrange Min.
unit
```
```
Default
setting
```
```
Effective
time
```
```
Type Mode
```

Chapter 7 ParameterList

```
Function
code
```
```
Name Settingrange Min.
unit
```
```
Default
setting
```
```
Effective
time
```
```
Type Mode
```
#### P 06 00

```
Maintorque
commandA
source
```
```
0 - Digitgiven(P 06 - 05 )
1 - Reserved
```
```
1 0 Effective
immediately
```
```
Setting
after
motor
stop
```
#### T

#### P 06 02

```
Torque
command
selection
```
```
0 - MaintorquecommandAsource
1 - AuxiliaryspeedcommandB
source
2 - CommandA+Bsource
3 - A/Bswitching
```
#### 1 0

```
Effective
immediately
```
```
Setting
after
motor
stop
```
#### T

#### P 06 04

```
Torque
command
filtertimein
torquemode
```
```
0 ～ 65535 0. 01 ms 0
```
```
Effective
immediately
```
```
Setwhen
running T
```
#### P 06 05

```
Torque
command
keyboardsetting
```
- 3000 ～ 3000 (basedonratedmotor
torque)

#### 0. 1 % 0

```
Effective
immediately
```
```
Setwhen
running T
```
#### P 06 06

```
Torquelimit
source
```
```
0 - Positiveandnegativeinternal
torquelimit(default)
1 - Positiveandreverseexternal
torquelimit(selectedwithP_CL,
N_CL)
```
#### 1 0

```
Effective
immediately
```
```
Setwhen
running
```
#### PST

```
P 06 08 Forwardinternal
torquelimit
```
```
0 ~ 5000 (basedonratedmotor
torque）
```
```
0. 1 % 3000 Effective
immediately
```
```
Setwhen
running
```
#### PST

#### P 06 09

```
Backward
internal
torquelimit
```
```
0 ~ 5000 (basedonratedmotor
torque）
```
```
0. 1 % 3000 Effective
immediately
```
```
Setwhen
running
```
#### PST

#### P 06 10

```
Forwardside
external
torquelimit
```
```
0 ~ 5000 (basedonratedmotor
torque）
```
```
0. 1 % 3000 Effective
immediately
```
```
Setwhen
running
```
#### PST

#### P 06 11

```
Backwardside
externaltorque
limit
```
```
0 ~ 5000 (basedonratedmotor
torque）^0.^1 %^3000
```
```
Effective
immediately
```
```
Setwhen
running PST
```
#### P 06 13

```
Speedlimit
source
selectionfor
torquecontrol
```
```
0 - Internalspeedlimit(P 06. 15 ,
P 06. 16 setting)
1 - Reserved
```
```
1 0 Effective
immediately
```
```
Setwhen
running
```
#### T

#### P 06 15

```
Positivespeed
limit
Duringtorque
control
```
```
0 ～ 9000 1 3000 Effective
immediately
```
```
Setwhen
running
```
#### T

#### P 06 16

```
Negetivespeed
limit
Duringtorque
control
```
#### 0 ～ 9000 1 3000

```
Effective
immediately
```
```
Setwhen
running T
```
#### P 06 17

```
Torquearrival
command
referencevalue
```
#### 0 ～ 5000

```
( 1000 correspondstotherated
torqueofthemotor)
```
#### 0. 1 % 0

```
Effective
immediately
```
```
Setwhen
running
```
#### PST

#### P 06 18

```
Torquearrival
effectiveoffset
threshold
```
#### 0 ～ 5000

```
( 1000 correspondstotherated
torqueofthemotor)
```
```
0. 1 % 200 Effective
immediately
```
```
Setwhen
running
```
#### PST

#### P 06 19

```
Torquearrival
ineffectiveoffset
threshold
```
#### 0 ～ 5000

```
( 1000 correspondstotherated
torqueofthemotor)
```
#### 0. 1 % 100

```
Effective
immediately
```
```
Setwhen
running PST
```
#### P 06 20

```
Speedlimit
window
intorquemode
```
```
1 ~ 900 1 ms 50 Effective
immediately
```
```
Setwhen
running
```
#### PST

#### P 06 21

```
Multi-segment
torque
command 1
```
- 3000 ～ 3000 (basedonmotor
ratedtorque）

```
0. 1 % 0 Effective
immediately
```
```
Setwhen
running
```
#### T

#### P 06 22

```
Multi-segment
torquecommand
2
```
- 3000 ～ 3000 (basedonmotorrated
torque）

#### 0. 1 % 0

```
Effective
immediately
```
```
Setwhen
running T
```
#### P 06 23

```
Multi-segment
```
```
torque
command 3
```
- 3000 ～ 3000 (basedonmotor
ratedtorque）

#### 0. 1 % 0

```
Effective
immediately
```
```
Setwhen
running
```
#### T

**P 07 group:gainparameter**

```
Function
code
```
```
Name Settingrange
```
```
Min.
unit
```
```
Default
setting
```
```
Effective
time
```
```
Type Mode
```

Chapter 7 ParameterList

Function
code

```
Name Settingrange Min.
unit
```
```
Default
setting
```
```
Effective
time
```
```
Type Mode
```
#### P 07 00

```
Positionloop
gain 1
```
#### 10 ～ 20000 0. 1 HZ 320

```
Effective
immediately
```
```
Setwhen
running
```
#### P

#### P 07 01

```
Speedloopgain
1 10 ～^200000.^1 HZ^180
```
```
Effective
immediately
```
```
Setwhen
running PS
```
#### P 07 02

```
Speedloop
integral
time 1
```
```
15 ～ 512 0. 01 ms 3100 Effective
immediately
```
```
Setwhen
running
```
#### PS

#### P 07 03

```
Speeddetection
filter 1 0 ～^2000.^01 ms^20
```
```
Effective
immediately
```
```
Setwhen
running PST
```
#### P 07 04

```
Torque
command
filtering 1
```
```
0 ～ 10000 0. 01 ms 126 Effective
immediately
```
```
Setwhen
running
```
#### PST

#### P 07 05

```
Positionloop
gain 2 10 ～^200000.^1 HZ^380
```
```
Effective
immediately
```
```
Setwhen
running P
```
```
P 07 06
```
```
Speedloopgain
2 10 ～^200000.^1 HZ^180
```
```
Effective
immediately
```
```
Setwhen
running PS
```
#### P 07 07

```
Speed
loopintegral
time 2
```
```
15 ～ 51200 0. 01 ms 51200 Effective
immediately
```
```
Setwhen
running
```
#### PS

#### P 07 08

```
Speeddetection
filter 2 0 ～^2000.^01 ms^0
```
```
Effective
immediately
```
```
Setwhen
running PST
```
#### P 07 09

```
Torque
command
filtering 2
```
```
0 ～ 10000 0. 01 ms 126 Effective
immediately
```
```
Setwhen
running
```
#### PST

#### P 07 10

```
DIfunction
GAINSWITCH
switching
actionselection
```
```
0 - SpeedloopregulatorP( 1 )/
PI( 0 )switch,gainisfixedtofirst
group.
1 - Firstgain( 0 ),secondgain( 1 )
switching.
```
#### 1 0

```
Effective
immediately
```
```
Setwhen
running PS
```
#### P 07 11

```
Gainswitching
mode
```
```
0 - Firstgainfixed
1 - Secondgainfixed
2 - UsingDIinput(GAINSWITCH)
3 - Torquecommandgreater
4 - Speedcommandchange
greater
5 - Speedcommandgreater
6 - Positiondeviationgreater(P)
7 - Receivepositioncommand(P)
8 - Positioningisnotcompleted
(P)
9 - Actualspeedisgreater(P)
10 - Receivepositioncommand
and
actualspeed(P)
11 - Speedloopcontrollerwith
PDFFcontrol(PS)
12 - Reserved
13 - Speedloopcontrollerwith
improvedPIcontrol(PS)
```
#### 1 0

```
Effective
immediately
```
```
Setwhen
running
```
#### PS

#### P 07 12

```
Gainswitching
delay^0 ~^100000.^1 ms^50
```
```
Effective
immediately
```
```
Setwhen
running PS
```
```
P 07 13 Gainswitching
level
```
```
0 ~ 20000 （unit:accordingtothe
gainswitchingmode
description）
```
```
1 50 Effective
immediately
```
```
Setwhen
running
```
#### PS

#### P 07 14

```
Hysteresistime
ofgainswitching
```
```
0 ~ 20000 （unit:accordingtothe
gainswitchingmode
description）
```
#### 1 33

```
Effective
immediately
```
```
Setwhen
running PS
```
#### P 07 15

```
Positiongain
switchingtime^0 ~^100000.^1 ms^33
```
```
Effective
immediately
```
```
Setwhen
running PS
```
```
P 07 16 Speedregulator
PDFFcoefficient
```
```
0 ~ 1000 0. 1 % 700 Effective
immediately
```
```
Setwhen
running
```
#### PS

#### P 07 17

```
Improvedspeed
PI
controllevel
```
#### 2 ~ 9 1 5

```
Effective
immediately
```
```
Setwhen
running PS
```
#### P 07 18

```
Anti-integration
saturation
coefficient
```
#### 0 ~ 1000 0. 001 820

```
Effective
immediately
```
```
Setwhen
running
```
#### PS

#### P 07 19

```
Speed
feedforward
controlselection
```
```
0 - Nospeedfeedforward
1 - Internalspeedfeedforward
```
```
1 1 Effective
immediately
```
```
Setwhen
running
```
#### P


Chapter 7 ParameterList

```
Function
code
```
```
Name Settingrange Min.
unit
```
```
Default
setting
```
```
Effective
time
```
```
Type Mode
```
#### P 07 20

```
Speed
feedforward
gain
```
#### 0 ～ 1000 0. 1 % 0

```
Effective
immediately
```
```
Setwhen
running P
```
#### P 07 21

```
Speed
feedforward
filtertime
parameter
```
```
0 ～ 6400 0. 01 ms 50 Effective
immediately
```
```
Setwhen
running
```
#### P

#### P 07 22

```
Torque
feedforward
selection
```
```
0 - Notorquefeedforward
1 - Internaltorquefeedforward
```
#### 1 1

```
Effective
immediately
```
```
Setwhen
running
```
#### PS

#### P 07 23

```
Torque
feedforward
gain
```
#### 0 ～ 1000 0. 1 % 0

```
Effective
immediately
```
```
Setwhen
running
```
#### PS

#### P 07 24

```
Torque
feedforward
filtertime
parameter
```
```
0 ～ 6400 0. 01 ms 50
```
```
Effective
immediately
```
```
Setwhen
running PS
```
**P 08 group:advancedadjustmentparameters**

```
Function
code Name Settingrange
```
```
Min.
unit
```
```
Default
setting
```
```
Effective
time Type Mode
```
```
P 08 00
```
```
Adaptivefilter
mode^0 ~^510
```
```
Effective
immediately
```
```
Setwhen
running PST
```
```
P 08 01
```
```
Resonance
frequency^0 ~^655351 Hz^0 N/A
```
```
Display
parameters PST
```
#### P 08 02

```
The 1 stnotch
filter
frequency
(manual)
```
```
10 ~ 4000 1 Hz 4000
```
```
Effective
immediately
```
```
Setwhen
running PST
```
#### P 08 03

```
The 1 stnotch
filter
width
```
```
0 ~ 8 1 8 Effective
immediately
```
```
Setwhen
running
```
#### PST

#### P 08 04

```
The 1 stnotch
filter
depth
```
#### 0 ~ 100 1 50

```
Effective
immediately
```
```
Setwhen
running PST
```
#### P 08 05

```
The 2 ndnotch
filter
frequency
(manual)
```
```
10 ~ 4000 1 Hz 4000 Effective
immediately
```
```
Setwhen
running
```
#### PST

#### P 08 06

```
The 2 ndnotch
filter
width
```
#### 0 ~ 8 1 8

```
Effective
immediately
```
```
Setwhen
running
```
#### PST

#### P 08 07

```
The 2 ndnotch
filter
depth
```
#### 0 ~ 100 1 50

```
Effective
immediately
```
```
Setwhen
running
```
#### PST

#### P 08 15

```
Dampingfilter
switch
```
#### 0 - OFF

#### 1 - ON^10

```
Effective
immediately
```
```
Setting
after
motor
stop
```
#### PS

#### P 08 16

```
Dampingfilter
frequency
```
```
10 ~ 2000 0. 1 Hz 2000
```
```
Effective
immediately
```
```
Setting
after
motor
stop
```
#### PS

```
P 08 17 Dampingfilter
selection
```
```
0 - DampingfilterA
1 - DampingfilterB
```
```
1 1 Effective
immediately
```
```
Setting
after
motor
stop
```
#### PS

```
P 08 18 FilterAwidth 0 ~ 200 1 4
```
```
Effective
immediately
```
```
Setting
after
motor
stop
```
#### PS

```
P 08 19 FilterBgain 0 ~ 100 1 100
```
```
Effective
immediately
```
```
Setting
after
motor
stop
```
#### PS

#### P 08 20

```
Offlineinertia
Identification
maximumspeed
```
```
200 ~ 1000 1 rpm 500 Effective
immediately
```
```
Setting
after
motor
stop
```
#### PST

```
P 08 21 Offlineinertia
Identification
```
```
50 ~ 800 1 ms 100 Effective
immediately
```
```
Setting
after
```
#### PST


Chapter 7 ParameterList

Function
code

```
Name Settingrange Min.
unit
```
```
Default
setting
```
```
Effective
time
```
```
Type Mode
```
```
accelerationand
deceleration
time
```
```
motor
stop
```
#### P 08 22

```
Waitingtime
afterasingle
offlineinertia
identificationis
completed
```
```
100 ~ 10000 1 ms 800
```
```
Effective
immediately
```
```
Setting
after
motor
stop
```
#### PST

#### P 08 23

```
Inertia
identification
modeselection
```
```
0 - Offlineinertiaidentification:
speedcommandispositiveand
negativetrianglewave
1 - Offlineinertiaidentification:
SpeedcommandisJOGmode
```
#### 1 0

```
Effective
immediately
```
```
Setting
after
motor
stop
```
#### PST

#### P 08 24

```
Motorrotation
round
forcomplete
singleoffline
inertia
identificatio
```
#### 0 ~ 65535 0. 01 R 83 N/A

```
Display
parameter
```
#### PST

#### P 08 26

```
Highfrequency
vibration
suppression
controlswitch
```
#### 0 - OFF

#### 1 - ON^10

```
Effective
immediately
```
```
Setting
after
motor
stop
```
#### PS

#### P 08 27

```
Highfrequency
vibration
suppression
frequency
```
```
10 ~ 4000 1 Hz 4000
```
```
Effective
immediately
```
```
Setwhen
running PS
```
#### P 08 28

```
Highfrequency
vibration
suppression
gain
compensation
```
#### 0 ~ 1000 0. 01 100

```
Effective
immediately
```
```
Setwhen
running PS
```
#### P 08 29

```
Highfrequency
vibration
suppression
damping
coefficient 1
```
```
0 ~ 1000 0. 01 180 Effective
immediately
```
```
Setwhen
running
```
#### PS

#### P 08 30

```
Highfrequency
vibration
suppression
frequency
compensation 1
```
- 1000 ~ 1000 1 Hz 0

```
Effective
immediately
```
```
Setwhen
running PS
```
#### P 08 31

```
Highfrequency
vibration
suppression
frequency
compensation 2
```
- 1000 ~ 1000 1 Hz 0 Effective
    immediately

```
Setwhen
running
```
#### PS

#### P 08 32

```
Highfrequency
vibration
suppression
damping
coefficient 2
```
#### 0 ~ 1000 0. 01 0

```
Effective
immediately
```
```
Setwhen
running PS
```
#### P 08 33

```
Anti-disturbance
compensation
switch
```
#### 0 - OFF

#### 1 - ON^10

```
Effective
immediately
```
```
Setting
after
motor
stop
```
#### PS

#### P 08 36

```
Disturbance
compensation
factor
```
```
0 ~ 10000 0. 1 % 0 Effective
immediately
```
```
Setwhen
running
```
#### PS

#### P 08 39

```
Momentary
speed
compensation
switch
```
#### 0 - OFF

#### 1 - ON

#### 1 0

```
Effective
immediately
```
```
Setting
after
motor
stop
```
#### PS

#### P 08 40

```
Instantaneous
speed
compensation
gain
```
```
0 ~ 1000 1 Hz 0 Effective
immediately
```
```
Setwhen
running
```
#### PS

#### P 08 41

```
Instantaneous
speed
compensation
gain
compensation
```
```
0 ~ 1000 0. 01 600 Effective
immediately
```
```
Setwhen
running
```
#### PS


Chapter 7 ParameterList

```
Function
code
```
```
Name Settingrange Min.
unit
```
```
Default
setting
```
```
Effective
time
```
```
Type Mode
```
#### P 08 45

```
Model
compensation
switch
```
```
0 - Turnoffmodelcompensation
1 - Rigidmodel
2 - Secondordervectormodel
```
#### 1 0

```
Effective
immediately
```
```
Setwhen
running PS
```
#### P 08 46

```
Model
compensation
gain
```
```
10 ~ 20000 0. 1 /s 300 Effective
immediately
```
```
Setting
after
motor
stop
```
#### PS

#### P 08 48

```
Model
Compensation
Forward
Compensation
Coefficient
```
```
0 ~ 10000 0. 1 % 1000 Effective
immediately
```
```
Setting
after
motor
stop
```
#### PS

#### P 08 49

```
Model
Compensation
Reverse
Compensation
Coefficient
```
#### 0 ~ 10000 0. 1 % 1000

```
Effective
immediately
```
```
Setting
after
motor
stop
```
#### PS

#### P 08 50

```
Model
compensation
vibration
suppression
frequencyA
```
```
0 ~ 2500 0. 1 Hz 500 Effective
immediately
```
```
Setting
after
motor
stop
```
#### PS

#### P 08 51

```
Model
compensation
vibration
suppression
frequencyR
```
```
0 ~ 2500 0. 1 Hz 500
```
```
Effective
immediately
```
```
Setting
after
motor
stop
```
#### PS

#### P 08 52

```
Model
Compensation
Speed
Compensation
Coefficient
```
```
0 ~ 10000 0. 1 % 1000 Effective
immediately
```
```
Setting
after
motor
stop
```
#### PS

**P 09 group:failureandprotection**

```
Function
code Name Settingrange
```
```
Min.
unit
```
```
Default
setting
```
```
Effective
time Type Mode
```
#### P 09 02

```
Undervoltage
detection
delay
```
```
100 ~ 20000 0. 1 ms 700 Effective
immediately
```
```
Setwhen
running
```
#### PST

#### P 09 04

```
Motoroutof
control
protection
```
```
0 - Protectionopen
1 - Protectionoff
```
```
1 0 Effective
immediately
```
```
Setwhen
running
```
#### PST

#### P 09 05

```
Overload
warningvalue
```
#### 1 ～ 100 1 % 90

```
Effective
immediately
```
```
Setwhen
running
```
#### PST

#### P 09 06

```
Motoroverload
protectionfactor^10 ～^3001 %^100
```
```
Effective
immediately
```
```
Setwhen
running PST
```
#### P 09 07

```
Undervoltage
protection
point
```
```
50 ～ 100 （ 100 correspondstothe
defaultundervoltagepoint）
```
```
1 % 100 Effective
immediately
```
```
Setwhen
running
```
#### PST

#### P 09 08

```
Overspeedfault
point
```
```
50 ～ 120 （ 100 correspondsto
maximummotorspeed）
```
#### 1 % 120

```
Effective
immediately
```
```
Setwhen
running
```
#### PST

#### P 09 09

```
Position
deviation
excessive
threshold( 32 bit)
```
#### 1 ~ 1073741824 1 P

#### 2500

```
line
motor:
32767 P
17 - bit
motor:
393216
23 - bit
motor:
251658
24
```
```
Effective
immediately
```
```
Setwhen
running PST
```
#### P 09 12

```
Low-speed
command
pulseinputpin
filter
time
```
```
0 - 500 8. 33 ns 45 Re-power
```
```
Setting
after
motor
stop
```
#### P


Chapter 7 ParameterList

```
Function
code
```
```
Name Settingrange Min.
unit
```
```
Default
setting
```
```
Effective
time
```
```
Type Mode
```
#### P 09 16

```
Averageload
rate
overload
threshold
```
```
100 ~ 3000 0. 1 % 1100 Effective
immediately
```
```
Setting
after
motor
stop
```
#### PST

#### P 09 17

```
Multi-segment
position/speed
DIinput
delaydetection
time
```
```
0 ~ 1000 1 ms 3
```
```
Effective
immediately
```
```
Setwhen
running
```
#### PS

#### P 09 23

```
Motorstallover
temperature
protection
time
```
```
10 ~ 65535 1 ms 200
```
```
Effective
immediately
```
```
Setting
after
motor
stop
```
#### PST

#### P 09 24

```
Motorstallover
temperature
protection
enable
```
```
0 - Shieldlockedrotorover
temperatureprotectionmonitoring
detection
1 - Turnonthemotorstalland
over-temperatureprotection
monitoringanddetection
```
```
1 0 Effective
immediately
```
```
Setwhen
running
```
#### PST

#### P 09 25

```
Motoroverload
protection
enable
```
```
0 - Openmotoroverloadand
averageloadrateoverload
detection
1 - Openmotoroverload,shield
theaverageloadrateoverload
detection
2 - Shieldmotoroverload,open
averageloadrateoverload
detection
3 - Shieldmotoroverloadand
averageloadrateoverload
detection
```
#### 1 0

```
Effective
immediately
```
```
Setting
after
motor
stop
```
#### PST

#### P 09 29

```
Averageload
rate
protectiontime
```
```
10 ~ 65535 160 ms 300
```
```
Effective
immediately
```
```
Setting
after
motor
stop
```
#### PST

**P 10 group:communicationparameters**

```
Function
code
```
```
Name Settingrange
```
```
Min.
unit
```
```
Default
setting
```
```
Effective
time
```
```
Type Mode
```
#### P 10 00

```
Communication
address^0 ~^247 ，^0 isbroadcastaddress^11
```
```
Effective
immediately
```
```
Setwhen
running PST
```
#### P 10 02

```
Modbusbaud
ratesetting
```
#### 0 - 2400

#### 1 - 4800

#### 2 - 9600

#### 3 - 19200

#### 4 - 38400

#### 5 - 57600

#### 6 - 115200

#### 1 6

```
Effective
immediately
```
```
Setwhen
running PST
```
#### P 10 03

```
Modbusdata
format
```
```
0 - Noparity, 2 stopbits
1 - Evenparity, 1 stopbit
2 - Oddparity, 1 stopbit
3 - Noparity, 1 stopbit
```
#### 1 0

```
Effective
immediately
```
```
Setwhen
running PST
```
#### P 10 04

```
Updatewrited
communication
functioncodeto
EEPROM?
```
```
0 - DonotupdateEEPROM
1 - UpdatetoEEPROMexceptP 11
groupandP 18 group
```
```
1 0 Effective
immediately
```
```
Setwhen
running
```
#### PST

#### P 10 05

#### RS 232

```
communication
baudratesetting
```
#### 0 - 2400

#### 1 - 4800

#### 2 - 9600

#### 3 - 19200

#### 4 - 38400

#### 5 - 57600

#### 6 - 115200

```
1 6 Effective
immediately
```
```
Setwhen
running
```
#### PST

#### P 10 06

#### RS 485

```
communication
address
selection
```
```
0 - DipswitchSettings
1 - Hostcomputercommunication
Settings
```
```
1 0 Re-power
```
```
Setting
after
motor
stop
```
#### PST


Chapter 7 ParameterList

**P 11 group:auxiliaryfunctionparameters**

```
Function
code Name Settingrange
```
```
Min.
unit
```
```
Default
setting
```
```
Effective
time Type Mode
```
```
P 11 01 Faultreset
```
```
0 - Nooperation
1 - Faultreset^10
```
```
Effective
immediately
```
```
Setting
after
motor
stop
```
#### PST

```
P 11 02 Softreset
```
```
0 - Nooperation
1 - Faultreset
```
#### 1 0

```
Effective
immediately
```
```
Setting
after
motor
stop
```
#### PST

#### P 11 03

```
Inertia
recognition
function
```
```
Effectiveuponenter 1 0
```
```
Effective
immediately
```
```
Setwhen
running PST
```
#### P 11 06

```
Absolute
encoderreset
```
```
0 - Nooperation.
1 - Absoluteencoderrelated
warninganderrorclearing.
2 - Absoluteencodermultiturndata
reset.
```
```
1 0 Re-power
```
```
Setting
after
motor
stop
```
#### PST

#### P 11 07

```
Absolutevalue
systemsoftlimit
setting
```
```
0 - nooperation
1 - Thecurrentpositionisset
tothenegativelimit
2 - Thecurrentpositionissetasthe
positivelimit
```
```
1 0 Effective
immediately
```
```
Setwhen
running
```
#### PST

#### P 11 09

```
System
initialization
function
```
```
0 - Nooperation
1 - Restorefactorysetting
(exceptP 1 andP 17 parameters)
2 - Clearthefaultrecord
```
```
1 0 Effective
immediately
```
```
Setting
after
motor
stop
```
#### PST

#### P 11 10

```
DIDOforced
input
andoutput
enable
```
```
0 - Nooperation
1 - ForcedDIenable
2 - ForcedDOenable
3 - ForcedDIDOenabled
```
#### 1 0

```
Effective
immediately
```
```
Setwhen
running PST
```
#### P 11 11

```
SetDIforced
input^0 -^0 x^01 FF^1511
```
```
Effective
immediately
```
```
Setwhen
running PST
```
```
P 11 12
```
```
SetDOforced
output^0 -^0 x^001 F^10
```
```
Effective
immediately
```
```
Setwhen
running PST
```
```
P 11 13
```
```
Emergencystop
settings
```
```
0 – Nooperation
1 - Emergencystop^10
```
```
Effective
immediately
```
```
Setwhen
running PST
```
**P 12 group:keyboarddisplayparameters**

```
Function
code
```
```
Name Settingrange Min.
unit
```
```
Default
setting
```
```
Effective
time
```
```
Type Mode
```
#### P 12 00

```
LEDwarning
display
selection
```
```
0 - LEDimmediatelyoutputs
warningmessage
1 - LEDdoesnotoutput
warningmessage
```
#### 1 0

```
Effective
immediately
```
```
Setwhen
running PST
```
#### P 12 01

```
Defaultdisplay
settings^0 ~^10011
```
```
Effective
immediately
```
```
Setwhen
running PST
```
#### P 12 03

```
Speeddisplay
filter
time
```
```
0 ~ 10000 0. 1 ms 50 Effective
immediately
```
```
Setwhen
running
```
#### PST

#### P 12 11

```
Non-standard
version
number
```
#### VV.B.DD 1 0 N/A

```
Display
parameter
```
#### P 12 12

```
Softwareversion
number VV.B.DD^10 N/A
```
```
Display
parameter
```
```
P 12 13
```
```
FPGAversion
number VV.B.DD^10 N/A
```
```
Display
parameter
```
```
P 12 14
```
```
Productseries
code PP.XXX^13000 N/A
```
```
Display
parameter
```
**P 13 group:multi-segmentposition**

```
Function
code Name Settingrange
```
```
Min.
unit
```
```
Default
setting
```
```
Effective
time Type
```
```
Mod
e
```

Chapter 7 ParameterList

```
Function
code
```
```
Name Settingrange Min.
unit
```
```
Default
setting
```
```
Effective
time
```
```
Type Mod
e
```
#### P 13 00

```
Multi-segmen
t
position
operation
mode
```
```
0 - Stopaftertheendofsingle
operation(P 13. 01 forsegment
numberselection)
1 - Cycleoperation(P 13. 01 for
segmentnumberselection)
2 - DIswitchingoperation
(selectedbyDI)
```
#### 1 1

```
Effective
immediately
```
```
Setting
after
motorstop
```
#### P

#### P 13 01

```
Specify
starting
segment
```
```
1 ～ 16 1 1 Effective
immediately
```
```
Setting
after
motorstop
```
#### P

#### P 13 02

```
Specifyend
segment^1
```
```
～ 16 1 16 Effective
immediately
```
```
Setting
after
motorstop
```
#### P

#### P 13 03

```
Interrupted
position
handle
setting
```
```
Validintheotherthreemodes
exceptDImode
0 :Continuetorunthesegmentthat
hasnotfinished
1 :Restartfromthefirstsegment
```
#### 1 0

```
Effective
immediately
```
```
Setting
after
motorstop
```
#### P

P 13 04 Waitingtime
unit

```
0 - ms
1 - s
```
```
1 0 Effective
immediately
```
```
Setting
after
motorstop
```
#### P

#### P 13 05

```
Position
control
method
```
```
0 - Incrementalpositioncontrol
1 - Absolutepositioncontrol
```
```
1 0 Effective
immediately
```
```
Setting
after
motorstop
```
#### P

#### P 13 08

```
Segment 1 :
movement
displacement
( 32 bits)
```
- 1073741824 ～ 1073741824 1 Unit 10000

```
Effective
immediately
```
```
Setwhen
running
```
#### P

#### P 13 10

```
Segment 1 :
maximum
running
speed
```
```
0 ～ 9000 1 rpm 200 Effective
immediately
```
```
Setwhen
running
```
#### P

#### P 13 11

```
Segment 1 :
acceleration/
deceler
ationtime
```
```
0 ~ 65535 1 ms 1
```
```
Effective
immediately
```
```
Setwhen
running P
```
#### P 13 12

```
Segment 1 :
waiting
timeafter
completion
```
#### 0 ～ 10000

```
1 ms(s
)^1000
```
```
Effective
immediately
```
```
Setwhen
running P
```
#### P 13 13

```
Segment 2 :
movement
displacement
( 32 bits)
```
- 1073741824 ～ 1073741824 1 Unit 10000 Effective
    immediately

```
Setwhen
running
```
#### P

#### P 13 15

```
Segment 2 :
maximum
running
speed
```
```
0 ～ 9000 1 rpm 200 Effective
immediately
```
```
Setwhen
running
```
#### P

#### P 13 16

```
Segment 2 :
acceleration/
deceler
ationtime
```
```
0 ~ 65535 1 ms 1
```
```
Effective
immediately
```
```
Setwhen
running P
```
#### P 13 17

```
Segment 2 :
waiting
timeafter
completion
```
#### 0 ～ 10000

```
1 ms(s
)
```
#### 1000

```
Effective
immediately
```
```
Setwhen
running
```
#### P

#### P 13 18

```
Segment 3 :
movement
displacement
( 32 bits)
```
- 1073741824 ～ 1073741824 1 Unit 10000 Effective
    immediately

```
Setwhen
running
```
#### P

#### P 13 20

```
Segment 3 :
maximum
running
speed
```
```
0 ～ 9000 1 rpm 200
```
```
Effective
immediately
```
```
Setwhen
running P
```
#### P 13 21

```
Segment 3 :
acceleration/
deceler
ationtime
```
```
0 ~ 65535 1 ms 1
```
```
Effective
immediately
```
```
Setwhen
running P
```
#### P 13 22

```
Segment 3 :
waiting
timeafter
completion
```
```
0 ～ 10000 1 ms(s
)
```
```
1000 Effective
immediately
```
```
Setwhen
running
```
#### P


Chapter 7 ParameterList

```
Function
code
```
```
Name Settingrange Min.
unit
```
```
Default
setting
```
```
Effective
time
```
```
Type Mod
e
```
#### P 13 23

```
Segment 4 :
movement
displacement
( 32 bits)
```
- 1073741824 ～ 1073741824 1 Unit 10000 Effective
    immediately

```
Setwhen
running
```
#### P

#### P 13 25

```
Segment 4 :
maximum
running
speed
```
```
0 ～ 9000 1 rpm 200 Effective
immediately
```
```
Setwhen
running
```
#### P

#### P 13 26

```
Segment 4 :
acceleration/
deceler
ationtime
```
```
0 ~ 65535 1 ms 1
```
```
Effective
immediately
```
```
Setwhen
running P
```
#### P 13 27

```
Segment 4 :
waiting
timeafter
completion
```
#### 0 ～ 10000

```
1 ms(s
)
```
#### 1000

```
Effective
immediately
```
```
Setwhen
running
```
#### P

#### P 13 28

```
Segment 5 :
movement
displacement
( 32 bits)
```
- 1073741824 ～ 1073741824 1 Unit 10000 Effective
    immediately

```
Setwhen
running
```
#### P

#### P 13 30

```
Segment 5 :
maximum
running
speed
```
```
0 ～ 9000 1 rpm 200
```
```
Effective
immediately
```
```
Setwhen
running P
```
#### P 13 31

```
Segment 5 :
acceleration/
deceler
ationtime
```
```
0 ~ 65535 1 ms 1
```
```
Effective
immediately
```
```
Setwhen
running
```
#### P

#### P 13 32

```
Segment 5 :
waiting
timeafter
completion
```
```
0 ～ 10000 1 ms(s
)
```
```
1000 Effective
immediately
```
```
Setwhen
running
```
#### P

#### P 13 33

```
Segment 6 :
movement
displacement
( 32 bits)
```
- 1073741824 ～ 1073741824 1 Unit 10000

```
Effective
immediately
```
```
Setwhen
running P
```
#### P 13 35

```
Segment 6 :
maximum
running
speed
```
```
0 ～ 9000 1 rpm 200
```
```
Effective
immediately
```
```
Setwhen
running P
```
#### P 13 36

```
Segment 6 :
acceleration/
deceler
ationtime
```
```
0 ~ 65535 1 ms 1 Effective
immediately
```
```
Setwhen
running
```
#### P

#### P 13 37

```
Segment 6 :
waiting
timeafter
completion
```
```
0 ～ 10000 1 ms(s
)
```
```
1000 Effective
immediately
```
```
Setwhen
running
```
#### P

#### P 13 38

```
Segment 7 :
movement
displacement
( 32 bits)
```
- 1073741824 ～ 1073741824 1 Unit 10000

```
Effective
immediately
```
```
Setwhen
running P
```
#### P 13 40

```
Segment 7 :
maximum
running
speed
```
```
0 ～ 9000 1 rpm 200
```
```
Effective
immediately
```
```
Setwhen
running
```
#### P

#### P 13 41

```
Segment 7 :
acceleration/
deceler
ationtime
```
```
0 ~ 65535 1 ms 1 Effective
immediately
```
```
Setwhen
running
```
#### P

#### P 13 42

```
Segment 7 :
waiting
timeafter
completion
```
#### 0 ～ 10000

```
1 ms(s
)^1000
```
```
Effective
immediately
```
```
Setwhen
running P
```
#### P 13 43

```
Segment 8 :
movement
displacement
( 32 bits)
```
- 1073741824 ～ 1073741824 1 Unit 10000

```
Effective
immediately
```
```
Setwhen
running P
```
#### P 13 45

```
Segment 8 :
maximum
running
speed
```
```
0 ～ 9000 1 rpm 200 Effective
immediately
```
```
Setwhen
running
```
#### P


Chapter 7 ParameterList

```
Function
code
```
```
Name Settingrange Min.
unit
```
```
Default
setting
```
```
Effective
time
```
```
Type Mod
e
```
#### P 13 46

```
Segment 8 :
acceleration/
deceler
ationtime
```
```
0 ~ 65535 1 ms 1 Effective
immediately
```
```
Setwhen
running
```
#### P

#### P 13 47

```
Segment 8 :
waiting
timeafter
completion
```
```
0 ～ 10000 1 ms(s
)
```
```
1000 Effective
immediately
```
```
Setwhen
running
```
#### P

#### P 13 48

```
Segment 9 :
movement
displacement
( 32 bits)
```
- 1073741824 ～ 1073741824 1 Unit 10000

```
Effective
immediately
```
```
Setwhen
running P
```
#### P 13 50

```
Segment 9 :
maximum
running
speed
```
```
0 ～ 9000 1 rpm 200
```
```
Effective
immediately
```
```
Setwhen
running
```
#### P

#### P 13 51

```
Segment 9 :
acceleration/
deceler
ationtime
```
```
0 ~ 65535 1 ms 1 Effective
immediately
```
```
Setwhen
running
```
#### P

#### P 13 52

```
Segment 9 :
waiting
timeafter
completion
```
#### 0 ～ 10000

```
1 ms(s
)^1000
```
```
Effective
immediately
```
```
Setwhen
running P
```
#### P 13 53

```
Segment 10 :
movement
displacement
( 32 bits)
```
- 1073741824 ～ 1073741824 1 Unit 10000

```
Effective
immediately
```
```
Setwhen
running
```
#### P

#### P 13 55

```
Segment 10 :
maximum
running
speed
```
```
0 ～ 9000 1 rpm 200 Effective
immediately
```
```
Setwhen
running
```
#### P

#### P 13 56

```
Segment 10 :
acceleration/
deceler
ationtime
```
```
0 ~ 65535 1 ms 1
```
```
Effective
immediately
```
```
Setwhen
running P
```
#### P 13 57

```
Segment 10 :
waitingtime
after
completion
```
#### 0 ～ 10000

```
1 ms(s
)^1000
```
```
Effective
immediately
```
```
Setwhen
running P
```
#### P 13 58

```
Segment 11 :
movement
displacement
( 32 bits)
```
- 1073741824 ～ 1073741824 1 Unit 10000 Effective
    immediately

```
Setwhen
running
```
#### P

#### P 13 60

```
Segment 11 :
maximum
running
speed
```
```
0 ～ 9000 1 rpm 200 Effective
immediately
```
```
Setwhen
running
```
#### P

#### P 13 61

```
Segment 11 :
acceleration/
deceler
ationtime
```
```
0 ~ 65535 1 ms 1
```
```
Effective
immediately
```
```
Setwhen
running P
```
#### P 13 62

```
Segment 11 :
waitingtime
after
completion
```
#### 0 ～ 10000

```
1 ms(s
)
```
#### 1000

```
Effective
immediately
```
```
Setwhen
running
```
#### P

#### P 13 63

```
Segment 12 :
movement
displacement
( 32 bits)
```
- 1073741824 ～ 1073741824 1 Unit 10000 Effective
    immediately

```
Setwhen
running
```
#### P

#### P 13 65

```
Segment 12 :
maximum
running
speed
```
```
0 ～ 9000 1 rpm 200
```
```
Effective
immediately
```
```
Setwhen
running P
```
#### P 13 66

```
Segment 12 :
acceleration/
deceler
ationtime
```
```
0 ~ 65535 1 ms 1
```
```
Effective
immediately
```
```
Setwhen
running P
```
#### P 13 67

```
Segment 12 :
waitingtime
after
completion
```
```
0 ～ 10000 1 ms(s
)
```
```
1000 Effective
immediately
```
```
Setwhen
running
```
#### P


Chapter 7 ParameterList

```
Function
code
```
```
Name Settingrange Min.
unit
```
```
Default
setting
```
```
Effective
time
```
```
Type Mod
e
```
#### P 13 68

```
Segment 13 :
movement
displacement
( 32 bits)
```
- 1073741824 ～ 1073741824 1 Unit 10000 Effective
    immediately

```
Setwhen
running
```
#### P

#### P 13 70

```
Segment 13 :
maximum
running
speed
```
```
0 ～ 9000 1 rpm 200 Effective
immediately
```
```
Setwhen
running
```
#### P

#### P 13 71

```
Segment 13 :
acceleration/
deceler
ationtime
```
```
0 ~ 65535 1 ms 1
```
```
Effective
immediately
```
```
Setwhen
running P
```
#### P 13 72

```
Segment 13 :
waitingtime
after
completion
```
#### 0 ～ 10000

```
1 ms(s
)
```
#### 1000

```
Effective
immediately
```
```
Setwhen
running
```
#### P

#### P 13 73

```
Segment 14 :
movement
displacement
( 32 bits)
```
- 1073741824 ～ 1073741824 1 Unit 10000 Effective
    immediately

```
Setwhen
running
```
#### P

#### P 13 75

```
Segment 14 :
maximum
running
speed
```
```
0 ～ 9000 1 rpm 200
```
```
Effective
immediately
```
```
Setwhen
running P
```
#### P 13 76

```
Segment 14 :
acceleration/
deceler
ationtime
```
```
0 ~ 65535 1 ms 1
```
```
Effective
immediately
```
```
Setwhen
running
```
#### P

#### P 13 77

```
Segment 14 :
waitingtime
after
completion
```
```
0 ～ 10000 1 ms(s
)
```
```
1000 Effective
immediately
```
```
Setwhen
running
```
#### P

#### P 13 78

```
movement
displacement
( 32 bits)
```
- 1073741824 ～ 1073741824 1 Unit 10000

```
Effective
immediately
```
```
Setwhen
running P
```
#### P 13 80

```
Segment 15 :
maximum
running
speed
```
```
0 ～ 9000 1 rpm 200
```
```
Effective
immediately
```
```
Setwhen
running P
```
#### P 13 81

```
Segment 15 :
acceleration/
deceler
ationtime
```
```
0 ~ 65535 1 ms 1
```
```
Effective
immediately
```
```
Setwhen
running
```
#### P

#### P 13 82

```
Segment 15 :
waitingtime
after
completion
```
```
0 ～ 10000 1 ms(s
)
```
```
1000 Effective
immediately
```
```
Setwhen
running
```
#### P

#### P 13 83

```
Segment 16 :
movement
displacement
( 32 bits)
```
- 1073741824 ～ 1073741824 1 Unit 10000

```
Effective
immediately
```
```
Setwhen
running P
```
#### P 13 85

```
Segment 16 :
maximum
running
speed
```
```
0 ～ 9000 1 rpm 200
```
```
Effective
immediately
```
```
Setwhen
running P
```
#### P 13 86

```
Segment 16 :
acceleration/
deceler
ationtime
```
```
0 ~ 65535 1 ms 1 Effective
immediately
```
```
Setwhen
running
```
#### P

#### P 13 87

```
Segment 16 :
waitingtime
after
completion
```
```
0 ～ 10000 1 ms(s
)
```
```
1000 Effective
immediately
```
```
Setwhen
running
```
#### P

**P 14 groupMulti-speedcommand**

```
Function
code
```
```
Name Settingrange Min.
unit
```
```
Default
setting
```
```
Effective
time
```
```
Type Mode
```

Chapter 7 ParameterList

```
Function
code
```
```
Name Settingrange Min.
unit
```
```
Default
setting
```
```
Effective
time
```
```
Type Mode
```
#### P 14 00

```
Multi-speed
command
operation
mode
```
```
0 - Stopaftertheendofsingle
operation(P 1401 forsegment
numberselection)
1 - Cycleoperation(P 1401 for
segmentnumberselection)
2 - DIswitchingoperation
```
```
1 1 Effective
immediately
```
```
Setting
after
motorstop
```
#### S

#### P 14 01

```
Speedcommand
endsegment
selection
```
```
1 ～ 16 1 16 Effective
immediately
```
```
Setting
after
motorstop
```
#### S

P 14 02 Runningtimeunit
selection

```
0 - s
1 - min
```
```
1 0 Effective
immediately
```
```
Setting
after
motorstop
```
#### S

#### P 14 03

```
Acceleration/dec
ele
rationtime 1
```
```
0 - 10000 1 ms 0 Effective
immediately
```
```
Setting
after
motorstop
```
#### S

#### P 14 04

```
Acceleration/dec
ele
rationtime 2
```
```
0 - 10000 1 ms 0
```
```
Effective
immediately
```
```
Setting
after
motorstop
```
#### S

#### P 14 05

```
Acceleration/dec
ele
rationtime 3
```
```
0 - 10000 1 ms 0
```
```
Effective
immediately
```
```
Setting
after
motorstop
```
#### S

#### P 14 06

```
Acceleration/dec
ele
rationtime 4
```
```
0 - 10000 1 ms 0
```
```
Effective
immediately
```
```
Setting
after
motorstop
```
#### S

#### P 14 07

```
Speedsegment
1 -^9000 ～+^90001 rpm^0
```
```
Effective
immediately
```
```
Setting
after
motorstop
```
#### S

#### P 14 08

```
Runningtimeof
speedsegment 1
```
#### 0 ～ 65535

```
0. 1 s(mi
n)
```
#### 50

```
Effective
immediately
```
```
Setting
after
motorstop
```
#### S

#### P 14 09

```
Speedupand
down
timeofspeed
segment 1
```
```
0 - zeroacc./dec.time
1 - acc./dec.time 1
2 - acc./dec.time 2
3 - acc./dec.time 3
4 - acc./dec.time 4
```
```
1 0 Effective
immediately
```
```
Setting
after
motorstop
```
#### S

#### P 14 10

```
Speedsegment
2 -^9000
```
```
～+ 9000 1 rpm 0 Effective
immediately
```
```
Setting
after
motorstop
```
#### S

#### P 14 11

```
Runningtimeof
speedsegment 2 0 ～^65535
```
```
0. 1 s(mi
n)^50
```
```
Effective
immediately
```
```
Setting
after
motorstop
```
#### S

#### P 14 12

```
Speedupand
down
timeofspeed
segment 2
```
```
0 - zeroacc./dec.time
1 - acc./dec.time 1
2 - acc./dec.time 2
3 - acc./dec.time 3
4 - acc./dec.time 4
```
#### 1 0

```
Effective
immediately
```
```
Setting
after
motorstop
```
#### S

P 14 13 Speedsegment
3

- 9000 ～+ 9000 1 rpm 0 Effective
    immediately

```
Setting
after
motorstop
```
#### S

P 14 14 Runningtimeof
speedsegment 3

```
0 ～ 65535 0.^1 s(mi
n)
```
```
50 Effective
immediately
```
```
Setting
after
motorstop
```
#### S

#### P 14 15

```
Speedupand
down
timeofspeed
segment 3
```
```
0 - zeroacc./dec.time
1 - acc./dec.time 1
2 - acc./dec.time 2
3 - acc./dec.time 3
4 - acc./dec.time 4
```
#### 1 0

```
Effective
immediately
```
```
Setting
after
motorstop
```
#### S

#### P 14 16

```
Speedsegment
4 -^9000 ～+^90001 rpm^0
```
```
Effective
immediately
```
```
Setting
after
motorstop
```
#### S

#### P 14 17

```
Runningtimeof
speedsegment 4
```
#### 0 ～ 65535

```
0. 1 s(mi
n)
```
#### 50

```
Effective
immediately
```
```
Setting
after
motorstop
```
#### S

#### P 14 18

```
Speedupand
down
timeofspeed
segment 4
```
```
0 - zeroacc./dec.time
1 - acc./dec.time 1
2 - acc./dec.time 2
3 - acc./dec.time 3
4 - acc./dec.time 4
```
```
1 0 Effective
immediately
```
```
Setting
after
motorstop
```
#### S

#### P 14 19

```
Speedsegment
5 -^9000 ～+^90001 rpm^0
```
```
Effective
immediately
```
```
Setting
after
motorstop
```
#### S


Chapter 7 ParameterList

```
Function
code
```
```
Name Settingrange Min.
unit
```
```
Default
setting
```
```
Effective
time
```
```
Type Mode
```
#### P 14 20

```
Runningtimeof
speedsegment 5 0
```
```
～ 65535 0.^1 s(mi
n)^50
```
```
Effective
immediately
```
```
Setting
after
motorstop
```
#### S

#### P 14 21

```
Speedupand
down
timeofspeed
segment 5
```
```
0 - zeroacc./dec.time
1 - acc./dec.time 1
2 - acc./dec.time 2
3 - acc./dec.time 3
4 - acc./dec.time 4
```
#### 1 0

```
Effective
immediately
```
```
Setting
after
motorstop
```
#### S

P 14 22 Speedsegment
6

- 9000 ～+ 9000 1 rpm 0 Effective
    immediately

```
Setting
after
motorstop
```
#### S

P 14 23 Runningtimeof
speedsegment 6

```
0 ～ 65535 0.^1 s(mi
n)
```
```
50 Effective
immediately
```
```
Setting
after
motorstop
```
#### S

#### P 14 24

```
Speedupand
down
timeofspeed
segment 6
```
```
0 - zeroacc./dec.time
1 - acc./dec.time 1
2 - acc./dec.time 2
3 - acc./dec.time 3
4 - acc./dec.time 4
```
#### 1 0

```
Effective
immediately
```
```
Setting
after
motorstop
```
#### S

#### P 14 25

```
Speedsegment
7 -^9000 ～+^90001 rpm^0
```
```
Effective
immediately
```
```
Setting
after
motorstop
```
#### S

#### P 14 26

```
Runningtimeof
speedsegment 7 0 ～^65535
```
```
0. 1 s(mi
n)^50
```
```
Effective
immediately
```
```
Setting
after
motorstop
```
#### S

#### P 14 27

```
Speedupand
down
timeofspeed
segment 7
```
```
0 - zeroacc./dec.time
1 - acc./dec.time 1
2 - acc./dec.time 2
3 - acc./dec.time 3
4 - acc./dec.time 4
```
```
1 0 Effective
immediately
```
```
Setting
after
motorstop
```
#### S

#### P 14 28

```
Speedsegment
8 -^9000 ～+^90001 rpm^0
```
```
Effective
immediately
```
```
Setting
after
motorstop
```
#### S

#### P 14 29

```
Runningtimeof
speedsegment 8 0 ～^65535
```
```
0. 1 s(mi
n)^50
```
```
Effective
immediately
```
```
Setting
after
motorstop
```
#### S

#### P 14 30

```
Runningtimeof
speedsegment 8
```
```
0 - zeroacc./dec.time
1 - acc./dec.time 1
2 - acc./dec.time 2
3 - acc./dec.time 3
4 - acc./dec.time 4
```
#### 1 0

```
Effective
immediately
```
```
Setting
after
motorstop
```
#### S

P 14 31 Speedsegment
9

- 9000 ～+ 9000 1 rpm 0 Effective
    immediately

```
Setting
after
motorstop
```
#### S

P 14 32 Runningtimeof
speedsegment 9

```
0 ～ 65535 0.^1 s(mi
n)
```
```
50 Effective
immediately
```
```
Setting
after
motorstop
```
#### S

#### P 14 33

```
Speedupand
down
timeofspeed
segment 9
```
```
0 - zeroacc./dec.time
1 - acc./dec.time 1
2 - acc./dec.time 2
3 - acc./dec.time 3
4 - acc./dec.time 4
```
#### 1 0

```
Effective
immediately
```
```
Setting
after
motorstop
```
#### S

#### P 14 34

```
Speedsegment
10
```
- 9000 ～+ 9000 1 rpm 0

```
Effective
immediately
```
```
Setting
after
motorstop
```
#### S

#### P 14 35

```
Runningtimeof
speedsegment
10
```
#### 0 ～ 65535

```
0. 1 s(mi
n)
```
#### 50

```
Effective
immediately
```
```
Setting
after
motorstop
```
#### S

#### P 14 36

```
Speedupand
down
timeofspeed
segment 10
```
```
0 - zeroacc./dec.time
1 - acc./dec.time 1
2 - acc./dec.time 2
3 - acc./dec.time 3
4 - acc./dec.time 4
```
```
1 0 Effective
immediately
```
```
Setting
after
motorstop
```
#### S

#### P 14 37

```
Speedsegment
11 -^9000 ～+^90001 rpm^0
```
```
Effective
immediately
```
```
Setting
after
motorstop
```
#### S

#### P 14 38

```
Runningtimeof
speedsegment
11
```
#### 0 ～ 65535

```
0. 1 s(mi
n)^50
```
```
Effective
immediately
```
```
Setting
after
motorstop
```
#### S


Chapter 7 ParameterList

```
Function
code
```
```
Name Settingrange Min.
unit
```
```
Default
setting
```
```
Effective
time
```
```
Type Mode
```
#### P 14 39

```
Speedupand
down
timeofspeed
segment 11
```
```
0 - zeroacc./dec.time
1 - acc./dec.time 1
2 - acc./dec.time 2
3 - acc./dec.time 3
4 - acc./dec.time 4
```
#### 1 0

```
Effective
immediately
```
```
Setting
after
motorstop
```
#### S

#### P 14 40

```
Speedsegment
12
```
- 9000 ～+ 9000 1 rpm 0

```
Effective
immediately
```
```
Setting
after
motorstop
```
#### S

#### P 14 41

```
Runningtimeof
speedsegment
12
```
```
0 ～ 65535 0.^1 s(mi
n)
```
```
50 Effective
immediately
```
```
Setting
after
motorstop
```
#### S

#### P 14 42

```
Speedupand
down
timeofspeed
segment 12
```
```
0 - zeroacc./dec.time
1 - acc./dec.time 1
2 - acc./dec.time 2
3 - acc./dec.time 3
4 - acc./dec.time 4
```
```
1 0 Effective
immediately
```
```
Setting
after
motorstop
```
#### S

#### P 14 43

```
Speedsegment
13 -^9000 ～+^90001 rpm^0
```
```
Effective
immediately
```
```
Setting
after
motorstop
```
#### S

#### P 14 44

```
Runningtimeof
speedsegment
13
```
#### 0 ～ 65535

```
0. 1 s(mi
n)^50
```
```
Effective
immediately
```
```
Setting
after
motorstop
```
#### S

#### P 14 45

```
Speedupand
down
timeofspeed
segment 13
```
```
0 - zeroacc./dec.time
1 - acc./dec.time 1
2 - acc./dec.time 2
3 - acc./dec.time 3
4 - acc./dec.time 4
```
```
1 0 Effective
immediately
```
```
Setting
after
motorstop
```
#### S

```
P 14 46 Speedsegment
14
```
- 9000 ～+ 9000 1 rpm 0 Effective
    immediately

```
Setting
after
motorstop
```
#### S

#### P 14 47

```
Runningtimeof
speedsegment
14
```
#### 0 ～ 65535

```
0. 1 s(mi
n)^50
```
```
Effective
immediately
```
```
Setting
after
motorstop
```
#### S

#### P 14 48

```
Speedupand
down
timeofspeed
segment 14
```
```
0 - zeroacc./dec.time
1 - acc./dec.time 1
2 - acc./dec.time 2
3 - acc./dec.time 3
4 - acc./dec.time 4
```
#### 1 0

```
Effective
immediately
```
```
Setting
after
motorstop
```
#### S

```
P 14 49 Speedsegment
15
```
- 9000 ～+ 9000 1 rpm 0 Effective
    immediately

```
Setting
after
motorstop
```
#### S

#### P 14 50

```
Runningtimeof
speedsegment
15
```
```
0 ～ 65535 0.^1 s(mi
n)
```
```
50 Effective
immediately
```
```
Setting
after
motorstop
```
#### S

#### P 14 51

```
Speedupand
down
timeofspeed
segment 15
```
```
0 - zeroacc./dec.time
1 - acc./dec.time 1
2 - acc./dec.time 2
3 - acc./dec.time 3
4 - acc./dec.time 4
```
#### 1 0

```
Effective
immediately
```
```
Setting
after
motorstop
```
#### S

#### P 14 52

```
Speedsegment
16 -^9000 ～+^90001 rpm^0
```
```
Effective
immediately
```
```
Setting
after
motorstop
```
#### S

#### P 14 53

```
Runningtimeof
speedsegment
16
```
#### 0 ～ 65535

```
0. 1 s(mi
n)
```
#### 50

```
Effective
immediately
```
```
Setting
after
motorstop
```
#### S

#### P 14 54

```
Speedupand
down
timeofspeed
segment 16
```
```
0 - zeroacc./dec.time
1 - acc./dec.time 1
2 - acc./dec.time 2
3 - acc./dec.time 3
4 - acc./dec.time 4
```
```
1 0 Effective
immediately
```
```
Setting
after
motorstop
```
#### S

**P 16 group:specialfunctionparameters**

```
Function
code
```
```
Name Settingrange
```
```
Min.
unit
```
```
Default
setting
```
```
Effectivetime Type Mode
```
```
P 16 00 Interruptfixed
lengthenable
```
```
0 - Disable
1 - Enable
```
```
1 0 Effective
immediately
```
```
Setting
after
motorstop
```
#### P

#### P 16 01

```
Interruptfixed
length 1
displacement
```
```
0 ~ 1073741824 1 Unit 10000 Effective
immediately
```
```
Setwhen
running
```
#### P


Chapter 7 ParameterList

```
Function
code
```
```
Name Settingrange Min.
unit
```
```
Default
setting
```
```
Effectivetime Type Mode
```
#### P 16 03

```
Interruptfixed
length 1
constant
runningspeed
```
```
0 ~ 9000 1 rpm 200 Effective
immediately
```
```
Setwhen
running
```
#### P

#### P 16 04

```
Interruptfixed
length
acceleration
time
```
```
0 ~ 1000 1 ms 200 Effective
immediately
```
```
Setwhen
running
```
#### P

#### P 16 05

```
Interruptfixed
length
deceleration
time
```
```
0 ~ 1000 1 ms 200
```
```
Effective
immediately
```
```
Setwhen
running P
```
#### P 16 06

```
Fixedlength
lock
releasesignal
enable
```
#### 0 ~ 1 1 1

```
Effective
immediately
```
```
Setwhen
running
```
#### P

#### P 16 08

```
Homingenable
control
```
```
0 - TurnofftheHomingfunction.
1 - EnabletheHoming
functionbyinputtingthe
HomingStartsignalthroughDI.
2 - StartreturntoHome
immediatelyafterpower-on.
3 - StartreturntoHome
immediately.
4 - Takethecurrentpositionas
theHome.
5 - SettheHomethroughDI
trigger.
6 - hostcomputerhoming
```
#### 1 0

```
Effective
immediately
```
```
Setting
after
motorstop
```
#### P


Chapter 7 ParameterList

```
Function
code
```
```
Name Settingrange Min.
unit
```
```
Default
setting
```
```
Effectivetime Type Mode
```
P 16 09 Homingmode

```
0 - Forwardhoming,deceleration
andhomepointsarehome
switch;
1 - Backwardhoming,
decelerationandhomepoints
arehomeswitch;
2 - Forwardhoming,
decelerationandhomepoints
aremotorZsignal;
3 - Backwardhoming,
decelerationandhomepoints
aremotorZsignal;
4 - Forwardhoming,
decelerationpointisHome
switch,homeismotorZsignal;
5 - Backwardhoming,
decelerationpointisHome
switch,homeismotorZsignal;
6 - Forwardhoming,
decelerationandhomepoints
areforwardovertravelswitch;
7 - Backwardhoming,
decelerationandhomepoints
arebackwardovertravel
switch;
8 - Forwardhoming,
decelerationpointisforward
overtravelswitch,homeis
motorZsignal;
9 - Backwardhoming,
decelerationpointisbackward
overtravelswitch,homeis
motorZsignal;
10 - Forwardhoming,no
decelerationpoint,homeis
homeswitch;
11 - Backwardhoming,no
decelerationpoint,homeis
homeswitch;
12 - Forwardhoming,no
decelerationpoint,homeis
motorZsignal;
13 - Backwardhoming,no
decelerationpoint,homeis
motorZsignal;
14 - Borwardhoming,no
decelerationpoint,homeis
forwardovertravelswitch;
15 - Backwardhoming,no
decelerationpoint,homeis
backwardovertravelswitch;
16 - Absolutevaluehoming;
17 - Indexingabsolutevalue
homing；
```
#### 1 0

```
Effective
immediately
```
```
Setting
after
motorstop
```
#### P

#### P 16 10

```
High-speedfor
searchinghome
switchsignal
```
```
10 - 3000 1 rpm 100
```
```
Effective
immediately
```
```
Setting
after
motorstop
```
#### P

#### P 16 11

```
Low-speedfor
searchinghome
switchsignal
```
```
10 - 1000 1 rpm 10
```
```
Effective
immediately
```
```
Setting
after
motorstop
```
#### P

#### P 16 12

```
Limittheaccel.
/decel.timefor
searchinghome
```
```
0 - 65535 1 ms 1000 Effective
immediately
```
```
Setting
after
motorstop
```
#### P

P 16 13 Limittimefor
findinghome

```
0 - 65535 1 ms 10000 Effective
immediately
```
```
Setting
after
motorstop
```
#### P

#### P 16 14

```
Mechanical
homeoffset( 32
bits)
```
- 1073741824 ~ 1073741824 1 Unit 0 Effective
    immediately

```
Setting
after
motorstop
```
#### P


Chapter 7 ParameterList

```
Function
code
```
```
Name Settingrange Min.
unit
```
```
Default
setting
```
```
Effectivetime Type Mode
```
#### P 16 28

```
Theorigin
correspondsto
theabsolute
positionofthe
encoder
```
```
0 ~ 4294967295 1 inc 0
```
```
Effective
immediately
```
```
Setwhen
running P
```
#### P 16 30

```
Theorigin
correspondsto
thenumberof
encoderturns
```
```
0 ~ 32767 1 inc 0
```
```
Effective
immediately
```
```
Setwhen
running P
```
#### P 16 31

```
Thereturnto
zeroprocess
position
reacheswait
count
```
```
0 ~ 65535 1 ms 500
```
```
Effective
immediately
```
```
Setwhen
running P
```
#### P 16 37

```
Interruptfixed
length 2
displacement
```
- 1073741824 ~ 1073741824 1 Unit 0

```
Effective
immediately
```
```
Setwhen
running P
```
#### P 16 39

```
Interruptfixed
length 2
constant
runningspeed
```
```
0 ~ 9000 1 rpm 200
```
```
Effective
immediately
```
```
Setwhen
running P
```
**P 18 group:displayparameters**

```
Function
code Name Settingrange
```
```
Min.
unit
```
```
Default
setting
```
```
Effective
time Type Mode
```
```
P 18 00 Servostatus
```
```
Rdy、Run、Err. 00 ~ 99
（error）,AL. 00 ~ 10 (warning）
```
#### 1 - N/A

```
Display
parameter PST
```
#### P 18 01

```
Motorspeed
feedback( 32
bit)
```
- 9000 ~ 9000 1 rpm - N/A Display
    parameter

#### PST

#### P 18 02

```
averageload
rate
```
#### 0 ~ 3000 0. 1 % - N/A

```
Display
parameter
```
#### PST

#### P 18 03

```
Speed
command -^9000 ~^90001 rpm - N/A
```
```
Display
parameter PST
```
#### P 18 04

```
Internaltorque
command
(relative
toratedtorque)
```
#### - 5000 ~ 5000 0. 1 % - N/A

```
Display
parameter PST
```
#### P 18 05

```
Phasecurrent
RMS
```
#### 0 ~ 10000 0. 01 A - N/A

```
Display
parameter
```
#### PST

#### P 18 06

```
Busvoltage
value^0 ~^100000.^1 V - N/A
```
```
Display
parameter PST
```
#### P 18 07

```
Absolute
position
counter( 32 bit)
```
- 1073741824 ~ 1073741824 1 Unit - N/A Display
    parameter

#### PST

```
P 18 09 Electricalangle 0 ~ 3600 0. 1 ° - N/A
```
```
Display
parameter
```
#### PST

#### P 18 10

```
Mechanical
angle
(relativeto
encoder
zero)
```
#### 0 ~ 3600 0. 1 ° - N/A

```
Display
parameter PST
```
#### P 18 11

```
Busencoder
communication
checkerror
counter
```
- 1 - N/A Display
    parameter

#### PST

#### P 18 12

```
Corresponding
speed
information
ofinputposition
command
```
- 9000 ~ 9000 1 rpm - N/A Display
    parameter

#### PST

#### P 18 13

```
Position
deviation
counter( 32 bit)
```
#### - 1073741824 ~ 1073741824 1 P - N/A

```
Display
parameter PST
```
#### P 18 15

```
Inputcommand
Pulsecounter
( 32
bit)
```
- 1073741824 ~ 1073741824 1 Unit - N/A Display
    parameter

#### PST

```
P 18 17 Feedbackpulse
counter( 32 bit)
```
- 1073741824 ~ 1073741824 1 P - N/A Display
    parameter

#### PST


Chapter 7 ParameterList

```
Function
code
```
```
Name Settingrange Min.
unit
```
```
Default
setting
```
```
Effective
time
```
```
Type Mode
```
#### P 18 19

```
Position
deviation
counter
command
unit( 32 bit)
```
- 1073741824 ~ 1073741824 1 Unit - N/A

```
Display
parameter PST
```
#### P 18 21

```
Digitalinput
signal
monitoring
```
#### - - - N/A

```
Display
parameter
```
#### PST

#### P 18 22

```
Recordthe
U-phasezero
currentvalue
during
shutdown
```
- 32768 ~ 32767 0. 01 A - N/A Display
    parameter

#### PST

#### P 18 23

```
Digitaloutput
signal
monitoring
```
#### - - - N/A

```
Display
parameter PST
```
#### P 18 24

```
Recordthe
V-phasezero
currentvalue
during
shutdown
```
#### - 32768 ~ 32767 0. 01 A - N/A

```
Display
parameter
```
#### PST

#### P 18 25

```
Totalpower-on
time
( 32 - bit)
```
```
0 - 4294967295 0. 1 s - N/A Display
parameter
```
#### PST

#### P 18 27

```
U-Phasezero
samplingvalue^0 ~^655351 - N/A
```
```
Display
parameter PST
```
P 18 28

```
V-Phasezero
samplingvalue^0 ~^655351 - N/A
```
```
Display
parameter PST
```
#### P 18 29

```
U-phase
current
samplingvalue
```
```
0 ~ 65535 1 - N/A Display
parameter
```
#### PST

#### P 18 30

```
V-phasecurrent
samplingvalue^0 ~^655351 - N/A
```
```
Display
parameter PST
```
#### P 18 31

```
Module
temperature
value
```
- 1 °C - N/A Display
    parameter

#### PST

#### P 18 32

```
Absolute
encoder
single-round
data
```
- pulse - N/A

```
Display
parameter PST
```
#### P 18 34

```
Absolute
encoder
multi-round
data
```
- turn - N/A Display
    parameter

#### PST

P 18 35 Maximumload
rate

```
0 ~ 3000 0. 1 % - N/A Display
parameter
```
#### PST

#### P 18 40

```
Displayoffault
record
```
```
0 - Currentfault
1 - Thefirstfaultbeforecurrent
fault
2 - Thesecondfaultbefore
currentfault
......
9 - The 9 thfaultbeforecurrent
fault
```
#### 1 0

```
Effective
immediately
```
```
Setwhen
running PST
```
P 18 41 Errorcode - - - N/A Display
parameter

#### PST

#### P 18 42

```
Selectedfault
timestamp( 32
bit)
```
- 0. 1 s - N/A

```
Display
parameter PST
```
P 18 44 In-timespeedat
selectedfault

- 1 rpm - N/A Display
    parameter

#### PST

#### P 18 45

```
In-timecurrent
Uat
Selectedfault
```
#### - 0. 01 A - N/A

```
Display
parameter PST
```
#### P 18 46

```
In-timecurrent
Vat
selectedfault
```
#### - 0. 01 A N/A

```
Display
parameter PST
```
P 18 47 Busvoltageat
selectedfault

- 0. 1 V - N/A Display
    parameter

#### PST

#### P 18 48

```
Inputterminal
status
atselectedfault
```
#### - - - N/A

```
Display
parameter PST
```

Chapter 7 ParameterList

```
Function
code
```
```
Name Settingrange Min.
unit
```
```
Default
setting
```
```
Effective
time
```
```
Type Mode
```
#### P 18 49

```
Outputterminal
statusat
selectedfault
```
#### - - - N/A

```
Display
parameter PST
```

AppendixA

## AppendixA:Alarmlightinformationdescription

```
Fault Decription
```
```
Redlightstate
```
```
Flicker
```
```
frequency
```
```
Flicker
```
```
number
```
```
Flicker
```
```
times
```
```
Cyclic
```
```
flicker
```
Voltagefault Overvoltage,Undervoltage(Themaincircuit) 1 HZ 1 3 seconds YES

Currentfault Overcurrent(OvercurrentA,OvercurrentB) 1 HZ 2 3 seconds YES

Overload/ Motor

stall

```
Driveoverload,Motoroverload,Motorstall 1 HZ 3 3 seconds YES
```
Parameter
settingerror

```
DI/DOterminalparametersettingfault，Softlimit
```
```
settingerro
```
```
1 HZ 4 3 seconds YES
```
Encodererror

```
Encodermatchingfault，Encoderdisconnection，
```
```
Encodercheckexception
```
```
1 HZ 5 3 seconds YES
```
Motorverification

erro

```
Motodataverificationfailure 1 HZ 6 3 seconds YES
```
Remainingfaults Faultsotherthantheabovefaults Stayon

Nofaults - Stayoff


AppendixB

## AppendixB:RS- 485 Communicationaddresscalculationdescription

### 1 1 Communicationsignalport............................................................................................................

DS 3 seriesservocrystalportPhysicalinterface,supportstandardModbusprotocol,CN 3 andCN 4

allcancommunicate,crystalportpindescriptionisasfollows:

Pin Definition Description

3 RS^485 _GND RS 485 signalPE

1 RS 485 + RS 485 signalling+

2 RS 485 - RS 485 signalling-

Connectorhousing PE groundconnection

### 1 2 WiringinstructionsandEMCprecautions..................................................................................

**1. 2. 1 ThissectiondescribeshowtomakeRS 485 networkcables**

Ifthenetworkcablecrystalheadismadeaccordingtothe 568 Aor 568 Bstandard,onlytheblueand

whiteandbluecablesaredrawnoutattheotherend,anditcanbeusedasanRS 485 communication

cable(usingtwisted-pairnetworkcables,thecommunicationdistanceisupto 300 m).Thespecificwiring

diagramisasfollows：


AppendixB

Networkplug 568 Bserialconnectionmethod

**1. 2. 2 Multi-computerCommunication**

Intheprojectengineeringenvironment,whenmultipleservosneedtocommunicateatthesame

time, 485 busisrecommendedtousethehand-in-handbusstructure.

The shorter the branch length between the bus and the node, the shorter the better. It is

recommendedthatthebranchlengthnotexceed 3 m.Thegeneralbusstructureisasfollows:

a) Recommendedscheme:Jointhestructurehandinhand

b) Generalscheme:branchlineconnectionstructure


AppendixB

c) Errorscheme:Starjoinstructure

**1. 2. 3 EMClayoutrequirements**

1 )Layoutrequirements:Interferencesourcesareisolatedfromsensitivedevices


AppendixB

3 ) GroundPEconnectionrequirements:

a.ThegroundcablemustbethickerthantheAWG 12.

b.GroundcableThePEconnectstothegroundterminalofanodeorthegroundbarofthe

cabinetwherethenodeislocated

c.ThedistancebetweenthePEandthebusisgreaterthan 5 cm

4 ) Cablelayoutrequirements：


AppendixB

a.KeeptheRS 485 busatleast 1 cmawayfromthemetalbackplaneofthecontrolcabinet.

b.KeeptheRS 485 busatleast 5 cmawayfromthegroundcable.

c. Keepadistanceofatleast 20 cmbetweentheRS 485 busandthefieldstrongcurrent

cable.

d.ThedistancebetweentheRS 485 busandthefieldmotorpowerlineshouldbeatleast

50 cm.

### 1 3 Communicationrelatedparameter...........................................................................................

Function

code

Name Description

Setting

range

Default

setting

P 1000 Communicationaddress

0 :broadcastaddress

1 ~ 247 :Slaveaddress

0 ～ 247 1

P 1002 Modbusbaudratesetting

0 - 2400

1 - 4800

2 - 9600

3 - 19200

4 - 38400

5 - 57600

6 - 115200

0 ～ 6 6


AppendixB

P 1003 Modbusdataformat

0 - Noparity, 2 stopbits

1 - Evenparity, 1 stopbit

2 - Oddparity, 1 stopbit

3 - Noparity, 1 stopbit

0 ～ 3 0

P 1004

Updatewrited

communicationfunction

codetoEEPROM?

0 - DonotupdateEEPROM

1 - Update to EEPROM except

P 11 groupandP 18 group

0 ～ 1 0

Explain:

1 )Inthecaseoflongtransmissioncableandseriousinterference,theservocommunicationline

shouldbeshieldedtwistedpaircable,andtheModbusdataformatshouldbesettooddcheckoreven

checkasfaraspossible.

2 )Theservocanonlydotheslavestation,the 0 communicationaddressisusedforthemaster

stationbroadcast,theservocommunicationaddressisprohibitedtosetto 0.

Whenmultipleservoscommunicateonthenetwork,eachdrivehasauniquecommunicationaddress,

otherwisethecommunicationwillbeabnormal.RS 485 communication,canconnect 32 servosatthe

sametime,ifyouwanttoconnectmoreservos,youmustaddamplifier,upto 247 servos.

5 ) Thebaudrateofservocommunicationmustbeconsistentwiththatofhostcomputer,otherwise

itiseasytohaveabnormalcommunication.

6 ) Servoprovides threetypesof dataverificationmethods:oddcheck (P 1003 : 2 ),evencheck

(P 1003 : 1 ),nocheck(P 1003 : 0 \ 3 )):

A) Oddcheckorevencheck

Theactualtransmissionofdataperframeis 11 bits, 1 startbit, 8 databit, 1 checkbit,and 1 endbit.

10 9 8 ～ 1 0

Endbit Checkbit Databit Startbit


AppendixB

B）Nocheck:Thedataframehastwoformats

①Theactualtransmissionofdataperframeis 11 bits, 1 startingbit, 8 databits,and 2 endbits.

10 ～ 9 8 ～ (^10)
Endbit Databit Startbit
②Theactualtransmissionofdataperframeis 11 bits, 1 startingbit, 8 databits,and 1 endbits.
10 9 8 ～ 1 0
Invalidbit Endbit Databit Startbit
Thedataframeformatoftheuppercomputermustconformtotheaboveformat,otherwiseitcannot
communicatewiththedriver.
5 ）TheP 1004 address parameter takes effect immediately,andits changevalueissavedto
EEPROM.Duringthecommunicationtask,ifnopowerfailureisrequiredtosaveparameters,setP 1004
to 0 .Otherwise,ifthefunctioncodevalueischangedinlargequantitiesforalongtimeandsavedtothe
EEPROM,theEEPROMwillbedamagedandtheserverEr. 04 (parameterstorage)willfail.

### 1 4 ModBusProtocol.............................................................................................................................

DS 3 series servo driver embedded standard Modbus RTU communication protocol, support

Modbusmasterstationreadandwriteasingle,multipleparametersoperation;Amongthem,thethree

mostcommonlyusedcommandscansatisfythecontroller'sall-roundcontroloftheservodrive,andthe

specificfunctioncodeisasfollows

Functioncode（CMD) Decription


AppendixB

16 # (^03) Readsingleormultipledriverregisterparameters
16 # 06 Readsingledriverregisterparameters
16 # 10 Readmultipledriverregisterparameters
TheModbuscommunicationaddresscorrespondingtotheinternalparametergroupoftheservo
meetsthefollowingrelation:P
MmNn
=Mm* 256 +Nn,This datais adecimalnumber,Mmindicatesthe
parametergroupandNnindicatesthecorrespondingserveraddressundertheparametergroup.For
example, theP 1801 address in theservo corresponds tothe Modbus communication address of
4609 ( 18 * 256 + 01 ),whichisconvertedtothehexadecimaladdressof 16 # 1201.
WhenPLCandtouchscreenMODBUSprogrammingofsomemanufacturers,theregisteraddress
isnotequaltotheactualregisteraddress,butisequaltotheactualregisteraddressplus 1 ,Becausethe
standardMODBUSinstructionregisterstartsat 1 ,thedeviceregisteraddressstartsat 0 .Therefore,
whenthiskindofPLCandtouchscreenareaddressedbyMODBUScommunicationwiththeservo,the
programmer must clarify the addressing method to correctly read and write the servo address
parameters,andwhenitisunclear,youcanconsulttheproductmanufacturer.
**1. 4. 1 Readparameter** （ **16 # 03** ）
Thefollowingrequestframerepresentsreading 2 - worddatastartingwiththeP 1801 (motorspeed
feedback)parameterfromadrivewithaddress 01.
Requestframe：
**Format**
Slave
Adr
CMD
Start
AdrH
Start
AdrL
No.of
RegsH
No.of
RegsL
CRC
1 Byte 1 Byte 2 Bytes 2 Bytes 2 Bytes
**Example** 16 # 01 16 # 03 16 # 12 16 # 01 16 # 00 16 # 02 XXXX XXXX
SlaveAdr
Drivecommunicationaddress,thatis,theP 1000 communicationaddresssetvalue,which
needstobeconvertedtoahexadecimalnumber.
CMD Functioncode, 16 # 03 isreadfunctioncodeoperation.


AppendixB

```
StartAdrH/L
```
```
Thestartaddressmustbeconvertedtoahexadecimalnumber.Inthisexample,P 1801
```
```
correspondstothedecimalModbusaddressnumberof 4609 andisconvertedtothe
```
```
hexadecimalnumberof 16 # 1201 .Therefore,thehighandlowvaluesare 16 # 12 and 16 # 01.
```
```
No.ofRegsH/L
```
```
Thenumberofreadparametersishighorlow,andthestartaddressvaluemustbeconverted
```
```
toahexadecimalnumber.Inthisexample,therearetwoparameters( 16 # 0002 ),sothehigh
```
```
valueis 16 # 00 ,andthelowvalueis 16 # 02.
```
```
CRC CRCcheckword.
```
Ifthereadissuccessful,thereplyframe:

```
Format
```
```
SlaveAdr CMD
```
```
Data
Length
```
```
Data 0 Data 1 ...
```
```
Data
n× 2 - 2
```
```
Data
n× 2 - 1
```
```
CRC
```
```
1 Byte 1 Byte 1 Byte 2 Bytes ... 2 Bytes 2 Bytes
```
```
Example 16 # 01 16 # 03 16 # 04 16 # 00 16 # 00 / 16 # 00 16 # 00 XXX XXX
```
**1. 4. 2 Writesingleparameter** （ **16 # 06** ）

Caserequirementsundertheframe,saidtothecorrespondenceaddressof 01 drive,asingle

```
SlaveAdr
```
```
Drivecommunicationaddress,thatis,theP 1000 communicationaddresssetvalue,
```
```
whichneedstobeconvertedtoahexadecimalnumber.
```
```
CMD Functioncode, 16 # 03 isreadfunctioncodeoperation.
```
```
DataLength Databytelength,equaltothenumberofreadparameters(No.ofRegs)* 2.
```
```
Data 0 /Data 1 Readthestartparametervalue 8 bitshigher/Startparametervalue 8 bitslower.
```
```
...
```
```
Datan* 2 - 1 Readthelastparametervalue 8 bitslower.
```
```
CRC CRCcheckword.
```

AppendixB

parameterP 0503 (speedinstructionsetkeyboard)writingnewvaluein 500.

Requestframe：

```
Format
```
```
Slave
```
```
Adr
```
```
CMD
```
```
Reg
```
```
AdrH
```
```
Reg
```
```
AdrL
```
```
Preset
```
```
DataH
```
```
Preset
```
```
DataL
```
```
CRC
```
```
1 Byte 1 Byte 2 Bytes 2 Bytes 2 Bytes
```
```
Example 16 # 01 16 # 06 16 # 01 16 # 44 16 # 01 16 #F 4 XXXX XXXX
```
```
SlaveAdr
```
```
Drivecommunicationaddress,thatis,theP 1000 communicationaddresssetvalue,which
```
```
needstobeconvertedtoahexadecimalnumber.
```
```
CMD Functioncode, 16 # 06 istowriteasinglenumberofoperations.
```
```
RegAdrH/L
```
```
Thevalueofthestartaddressmustbeconvertedtoahexadecimalnumber.Inthisexample,
```
```
P 0503 correspondstothedecimalModbusaddressnumberof 1283 andisconvertedto
```
```
16 # 0503 inhexadecimal.Therefore,thehighvalueis 16 # 05 andthelowvalueis 16 # 03.
```
```
PresetDataH/L
```
```
Whenthedataiswrittenwithhighorlowbytes,thedatamustbeconvertedintohexadecimal
```
```
numbers.Inthisexample, 500 isconvertedinto 16 # 01 F 4 inhexadecimal,sothehighbyteis
```
```
16 # 01 andthelowbyteis 16 #F 4.
```
```
CRC CRCcheckword.
```
Ifreadsuccessfully,replyframe:

```
Format
```
```
Slave
```
```
Adr
```
```
CMD
```
```
Reg
```
```
AdrH
```
```
Reg
```
```
AdrL
```
```
Preset
```
```
DataH
```
```
Preset
```
```
DataL
```
```
CRC
```
```
1 Byte 1 Byte 2 Bytes 2 Bytes 2 Bytes
```
```
Example 16 # 01 16 # 06 16 # 01 16 # 44 16 # 01 16 #F 4 XXXX XXXX
```

AppendixB

**1. 4. 3 Writemultiplearguments** （ **16 # 10** ）

Thefollowingrequestframeindicatesthatnewvalues 200 and 300 arecontinuouslywrittentothetwo

parametersP 0505 /P 0506 (speedinstructionaccelerationanddecelerationtime)tothecommunication

addressofdrive 01.

Requestframe：

```
Format
```
```
SlaveAdr CMD
```
```
Start
```
```
AdrH
```
```
Start
```
```
AdrL
```
```
No.of
RegsH
```
```
No.of
```
```
RegsL
```
```
Data
Length
```
```
Data 0 Data 1 ...
```
```
Data
n× 2 - 2
```
```
Data
n× 2 - 1
```
```
CRC
```
```
1 Byte 1 Byte 2 Bytes 2 Bytes 1 Byte 2 Bytes ... 2 Bytes 2 Bytes
```
```
Example 16 # 01 16 # 10 16 # 05 16 # 05 16 # 00 16 # 02 16 # 04 16 # 00 16 #C 8
```
```
/
/
```
```
16 # 01 16 # 2 C XXXX XXXX
```
Ifmultiplewritesaresuccessful,thereplyframe:

SlaveAdr

Drivecommunicationaddress,thatis,theP 1000 communicationaddressset

value,whichneedstobeconvertedtoahexadecimalnumber.

CMD Functioncode, 16 # 10 istowritemultipleparameteroperations.

StartAdrH/L

Thevalueofthestartaddressmustbeconvertedintoahexadecimalnumber.

Inthisexample,P 0505 correspondstothedecimalModbusaddressnumber

of 1285 ,andisconvertedintoahexadecimalnumberof 16 # 0505 ,sothehigh

valueis 16 # 05 andthelowvalueis 16 # 05.

No.ofRegsH/L

Thenumberofwriteparametersishigh/low,andthestartaddressvaluemust

be converted to a hexadecimal number. In this example, there are two

parameters( 16 # 0002 ),sothehighvalueis 16 # 00 ,andthelowvalueis 16 # 02.

DataLength

Thelengthofdata bytes,equal to thenumber ofwriteparameters (No.of

Regs)* 2 ,mustbeconvertedtohexadecimal.

Data 0 /Data 1

Thestartparametervalueis 8 bitshigherorthestartparametervalueis 8 bits

lower. 200 correspondsto 16 #C 8.

...

Datan* 2 - 1 Readthelastparametervalueis 8 bitslower; 300 correspondsto 16 # 012 C.

CRC CRCcheckword.


AppendixB

```
Format
```
SlaveAdr CMD

Start

AdrH

Start

AdrL

```
No.of
RegsH
```
```
No.of
RegsL
```
CRC

1 Byte 1 Byte 2 Bytes 2 Bytes 2 Bytes

**Example** 16 # 01 16 # 10 16 # 05 16 # 05 16 # 00 16 # 02 XXXX XXXX

**1. 4. 4 Errorcode**

The driver receives the MODBUS communication message from the master station without

transmissionerror,buttheslavestationdoesnotanswercorrectly.Inthiscase,theslavestationwill

replywithanabnormalresponsemessage.

Exceptionresponseframeformat:Inthisexample,indicatestheexceptionresponseframewhen

writingasingledriveregisterparameter.

```
Format
```
SlaveAdr CMD ErrorH ErrorL CRC

1 Byte 1 Byte 2 Bytes 2 Bytes

**Example** 16 # 01 16 # 86 16 # 00 16 # 02 XXXX XXXX

SlaveAdr

Drivecommunicationaddress,thatis,theP 1000 communicationaddressset

value,whichneedstobeconvertedtoahexadecimalnumber.

CMD Functioncode, 16 # 10 istowritemultipleparameteroperations.

StartAdrH/L

The value of the start address must be converted into a hexadecimal

number.Inthisexample,P 0505 correspondstothedecimalModbusaddress

numberof 1285 ,andisconvertedintoahexadecimalnumberof 16 # 0505 ,so

thehighvalueis 16 # 05 andthelowvalueis 16 # 05.

No.ofRegsH/L

Thenumberofwriteparametersishighorlow.Thestartaddressmustbe

converted to a hexadecimal number. In this example, there are two

parameters ( 16 # 0002 ),sothehighvalueis 16 # 00 andthelowvalue is

16 # 02.

CRC CRCcheckword.


AppendixB

SlaveAdr Drivecommunicationaddress,theP 1000 communicationaddresssetvalue,which

needstobeconvertedtoahexadecimalnumber.

CMD Functioncode:Thehighestbit( 7 thbit)ofthefunctioncodeofthemasterstationis

setto 1 intheabnormalresponseframe.Inthisexample,therequestframeofthe

masterstationiswrittenintoasingleparameterbit 16 # 06 ,andthehighestpositionof

thefunctioncodeofthemasterstationissetto 1 intheabnormalresponseframe,

whichis 16 # 86.

ErrorH/L Theerrorcodetakes 2 bytes.

CRC CRCcheckword.

Errorcodesaredescribedinthefollowingtable:

Errorcode Name Decription

16 # 0001 Illegalcommandcode Thefunctioncodecommandisincorrect.

16 # 0002 Illegaldataaddress

Thewriteparameterrangeisincorrectorcontainsreserved

parameters,andtheparametersareread-only.

16 # 0003 Illegaldata

Theparametergroupaddressexceedstherangethatcan

besetbytheserver.

16 # 0004

Theslavestationdevice

isfaulty

The communication interface of the slave server is

abnormalandtheCRCcheckerroroccurs.

16 # 0006

Busy, refused to

execute.

TheservoslaveisperformingEEPROMstorage,orison

enable.

**1. 4. 5 Functioncodewithsignednumber**

Whenwritingsingleormultipleparameters,thedatamaybesigned.Inthiscase,thewrittendata

needstobeconvertedtothecorrespondingcomplement.

1 ） 16 bitfunctioncode

a)Whenthedataispositiveor 0 ,complement=sourcecode.

b)Fornegativedatabits,thecomplement= 0 xFFFF-thecomplementoftheabsolutevalueofthe


AppendixB

data+ 0 x 0001.

Forexample,inthe 16 - digit 100 codebit 0 x 0064 ,thecomplementis: 0 x 0064 ；

16 - digit- 100 complement: 0 xFFFF- 0 x 0064 + 0 x 0001 =FF 9 C

2 ） 32 - bitfunctioncode

a)Whenthedataispositiveor 0 ,complement=sourcecode.

b)Fornegativedatabits,thecomplement= 0 xFFFFFFFF-thecomplementoftheabsolutevalue

ofthedata+ 0 x 00000001

Forexample,ifthe 32 - bit 100 codebitis 0 x 00000064 ,thecomplementis: 0 x 00000064 ；

32 bits- 100 complement: 0 xFFFFFFFF- 0 x 00000064 + 0 x 00000001 =FFFFFF 9 C.

**1. 4. 6 Addressesofparametersthatcanbereadandwrittenby**

**MODBUS**

TheserveraddressthatcanbereadandwrittenthroughtheMODBUScommunicationtask,seethe

parametergroupaddresstable;

Parametergroupaddresstable:

```
Parametergroup
number
```
```
Parametergroupdescription
```
```
P 00 Basiccontrolparameters
```
```
P 01 Servomotorparameters
```
```
P 02 Digitalinputandoutputparameters
```
```
P 04 Positioncontrolparameters
```
```
P 05 Speedcontrolparameters
```
```
P 06 Torquecontrolparameters
```
```
P 07 Gainparameters
```
```
P 08 Advancedadjustmentparameters
```
```
P 09 Faultandprotectionparameters
```
```
P 10 Communicationparameters
```
```
P 11 Auxiliaryfunctionparameters
```
```
P 12 Keyboarddisplayparameters
```
```
P 13 Multi-segmentpositionfunctionparameters
```
```
P 14 Multi-speedfunctionparameters
```
```
P 16 Specialfunctionparameters
```
```
P 17 Driveparameters
```
```
P 18 Displayparameters
```
Ifthecommunicationtaskistowritemultipledatainarow,payattentiontowhethertheparameter


AppendixB

addressesinthewritetaskareconsecutiveaddressesintheservoparametergroup.Iftheyarenot

consecutive,thecommunicationtaskwillbeaffected.Inthiscase,youneedtosplitthetask.Whether

theaddressiscontinuouscanbeseeninthedetailedservoaddressparametertableorthroughthe

servohostcomputer.

IfyouwritedatatoP 02 groupparameters,write 12 numbersstartingfromP 0201 toP 0212 ,thewrite

taskfails.IntheP 02 servoparametergroup,theaddressesP 0201 toP 0212 arediscontinuous,andthe

hiddenaddressP 0209 affectsthecommunicationtask.AddawritetaskfromP 0201 toP 0208 asawrite

taskandP 0210 toP 0212 asanotherwritetask.

When usingPLCtowrite the MODBUScommunicationtask program,weshould pay special

attentiontotheabovephenomenon,otherwisetherewillbeacommunicationtaskfailure.

### 1. 5 485 Precautionsforonsitecommunication

**1. 5. 1 Terminalresistancetreatment**

Whenthe 485 RScommunicationisunstable,theaccessterminal resistancecanbeconsideredto

eliminatethereflectedsignalinthecommunicationcable.

a．Onlytheresistorsmatchedonbothendsofthecommunicationnetworkcanbeconnected.

b．Itisrecommendedthatthemasterstationbeplacedatoneendofthebus.

c．Inthecaseofpowerfailure,afterconnectingtheterminalresistance,youcanuseamultimeter

ohmtomeasuretheresistancebetween 485 buses,andthenormalmeasurementvalueis 60 Ω.Ifthe

displayedvalueislessthan 50 ohms,checkwhetherothernodeshaveaddedmatchingresistorsand

disconnectthem.If 0 Ohmisdisplayed,checkwhetherthenodeisshort-circuitedordamaged.


AppendixB

**1. 5. 2 GNDwiringpointprocessing**

FornodesthatdonothaveaGNDconnectionpoint,refertothefollowingmethodsforhandling

communicationcables:

a． Findoutwhetherthereisareferencegroundsharedwiththe 485 circuitonotherportsofthis

node.Ifthereis,GNDisconnectedtothis referenceground.Notethattheshieldlayercannotbe

connectedtothereferenceground,otherwisethe 485 portwillbedamaged.

b． Checkwhetherthereisareferencegroundsharedwiththe 485 circuitonthisnodeboard.If

thereis,GNDisconnectedtothisreferenceground.Notethattheshieldlayercannotbeconnectedto

thereferenceground,otherwisethe 485 portwillbedamaged.

c． Ifthereferencegroundofthe 485 circuitcannotbefound,suspendtheGNDcableandensure

thatthegroundcableisreliablyconnectedtothePE.

**1. 5. 3 Systemexternalinterferenceprocessing**

Toeffectivelysuppresstheimpactofexternalinterferenceonthecommunication,youcantakethe

followingmeasures.


AppendixB

a．Wrapamagneticloopatposition 1 tosuppressexternalinterferencefromthesystem;

b．Wrapamagneticloopatposition 2 tosuppressexternalinterferencefromthesystem.

**1. 5. 4 Driverinterferenceprocessing**

Whenthedriverisrunning,itwillalsoaffectthecommunication.Inordertoeffectivelysuppressthe

interferenceofthedriveritself,youcanrefertothefollowingmeasures.

a． Addafiltermagneticringatposition 1 .ThreeUVWwires(excludinggroundwirePE)pass

throughthemagneticringatthesametime.Threeturnsarerecommended.

b．Wrapamagneticringatposition 2 ,addafiltermagneticringatposition 2 ,andthepowercable

(excludinggroundPE)passesthroughthemagneticringatthesametime.Itisrecommendedthatyou

windthreeturns.

Processforlocatingonsiteproblems:


AppendixB


DS 3 - A 1 PSeriesUsermanualV 1. 0 AppendixC

## AppendixC:Brakeresistorconfiguration

Whentheservomotorisinthestateofdecelerationbraking,themotorwillbeinthe

stateofgeneratingelectricityandfeedtheelectricenergybacktotheservodriverbus

circuit.Whenthedriverbusvoltagedetectionvalueishigherthanthesetthreshold,the

driverwillreporttheovervoltagewarning,andtheenergygeneratedbythemotorneedsto

beconsumedbyanexternalbrakeresistor.ConnectthebrakeresistancethroughRB+

andRB- at thepower end, and correctly set the brake resistancevalue andbrake

resistancepower.Theresistancevalueoftheoptionalbrakeresistancecannotbelower

thantherecommendedresistancevalue.Therecommendedparameters ofthebrake

resistanceareshowninthefollowingtable:

```
Mode Brakeresistancevalue(Ω) Brakeresistancepower（W）
```
#### DSY- 200 RS 10 100

#### DSY- 400 RS 10 100

#### DSY- 750 RS 5 100

#### DSY- 1200 RS 3. 5 200


AppendixD

## AppendixD:Versionchangelog

Date Modifiedversion Contentofchange

2024. 12 V 1. 0 Firstrelease



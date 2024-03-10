#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include "CST816S.h"
#include "SensorQMI8658.hpp"
#include <lvgl.h>
#include <ui.h>
#include <Arduino_Helpers.h>
#include <AH/Timing/MillisMicrosTimer.hpp>
#include "mcp2515_can.h"

//GPIOs available : 15 16 17 18 21 33 (from center pair outside)
#define CAN_MISO 15 // Goes to D5
#define CAN_MOSI 16 // Goes to D4
#define CAN_CLK 17 // Goes to D6
#define CAN_CS 18 //Goes to D7
#define CAN_INT 33

mcp2515_can CAN(CAN_CS);
SPIClass CANSPI(HSPI); //Pins need to be set in the begin() in setup()
unsigned char RxBuffer[8];
unsigned char TxBuffer[8] = {0x02,0x01,0x00,0x00,0x00,0x00,0x00,0x00};
unsigned char len = 0;
unsigned long FrameID = 0x7E8;

unsigned char OBD_length;
unsigned char OBD_mode;
unsigned char OBD_command;
unsigned char byteA;
unsigned char byteB;
unsigned char byteC;
unsigned char byteD;
unsigned char byteE;

byte requestID = 0x05;

//Touch and QMI I2C setup
#define TP_INT 5
#define TP_SDA 6
#define TP_SCL 7
#define TP_RST 13
#define QMI_INT2 3
#define QMI_INT1 4
#define QMI_SDA 6
#define QMI_SCL 7

//For the QMI IMU
#define USE_WIRE
#define SENSOR_SDA 6
#define SENSOR_SCL 7

//Not sure if useful
#define TFT_HOR_RES 240
#define TFT_VER_RES 240

#define DRAW_BUF_SIZE (TFT_HOR_RES * TFT_VER_RES / 4 * (LV_COLOR_DEPTH / 8))
uint32_t draw_buf[DRAW_BUF_SIZE];

CST816S touch(TP_SDA, TP_SCL, TP_RST, TP_INT);
SensorQMI8658 qmi;
IMUdata acc;
IMUdata gyr;

//Revise this
int intakeTemp;
int absBaroPressure = 100;
int intakeManifoldPressure = 100;
int boostPressure;
int engineCoolantTemp;
float controlModuleVoltage = 12.0;

//Timers
Timer<millis> refreshValues = 150;
Timer<millis> tickerLVGL = 5;
Timer<millis> OBDrequestDelay = 20;

#if LV_USE_LOG != 0
void my_print( lv_log_level_t level, const char * buf )
{
    LV_UNUSED(level);
    Serial.println(buf);
    Serial.flush();
}
#endif


void touchRead(lv_indev_t *indev, lv_indev_data_t *data)
{
  if(touch.available()) {
    data->state = LV_INDEV_STATE_PRESSED;
    data->point.x = touch.data.x;
    data->point.y = touch.data.y;
  }
  else {
    data->state = LV_INDEV_STATE_RELEASED;
  }
}


void setup() {
  CANSPI.begin(CAN_CLK,CAN_MISO,CAN_MOSI,CAN_CS);
  CAN.setSPI(&CANSPI);
  CAN.begin(CAN_500KBPS,MCP_16MHz);


  Serial.begin(115200);
  String LVGL_Arduino = "Hello Arduino! ";
  LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();
  Serial.println(LVGL_Arduino);
  lv_init();

  #if LV_USE_LOG != 0
    lv_log_register_print_cb( my_print );
  #endif

  lv_display_t * disp;
  disp = lv_tft_espi_create(TFT_HOR_RES, TFT_VER_RES, draw_buf, sizeof(draw_buf));

  lv_indev_t *indev = lv_indev_create();
  lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
  lv_indev_set_read_cb(indev,touchRead);



  if (!qmi.begin(Wire, QMI8658_L_SLAVE_ADDRESS, SENSOR_SDA, SENSOR_SCL)) {
    Serial.println("Failed to find QMI8658 - check your wiring!");
    while (1) {
      delay(1000);
    }
  }
  
  #pragma region 
  Serial.print("Device ID:");
  Serial.println(qmi.getChipID(), HEX);
  

  qmi.configAccelerometer(
    /*
      * ACC_RANGE_2G
      * ACC_RANGE_4G
      * ACC_RANGE_8G
      * ACC_RANGE_16G
      * */
    SensorQMI8658::ACC_RANGE_4G,
    /*
      * ACC_ODR_1000H
      * ACC_ODR_500Hz
      * ACC_ODR_250Hz
      * ACC_ODR_125Hz
      * ACC_ODR_62_5Hz
      * ACC_ODR_31_25Hz
      * ACC_ODR_LOWPOWER_128Hz
      * ACC_ODR_LOWPOWER_21Hz
      * ACC_ODR_LOWPOWER_11Hz
      * ACC_ODR_LOWPOWER_3H
    * */
    SensorQMI8658::ACC_ODR_1000Hz,
    /*
    *  LPF_MODE_0     //2.66% of ODR
    *  LPF_MODE_1     //3.63% of ODR
    *  LPF_MODE_2     //5.39% of ODR
    *  LPF_MODE_3     //13.37% of ODR
    * */
    SensorQMI8658::LPF_MODE_0,
    // selfTest enable
    true);
  qmi.configGyroscope(
    /*
    * GYR_RANGE_16DPS
    * GYR_RANGE_32DPS
    * GYR_RANGE_64DPS
    * GYR_RANGE_128DPS
    * GYR_RANGE_256DPS
    * GYR_RANGE_512DPS
    * GYR_RANGE_1024DPS
    * */
    SensorQMI8658::GYR_RANGE_64DPS,
    /*
      * GYR_ODR_7174_4Hz
      * GYR_ODR_3587_2Hz
      * GYR_ODR_1793_6Hz
      * GYR_ODR_896_8Hz
      * GYR_ODR_448_4Hz
      * GYR_ODR_224_2Hz
      * GYR_ODR_112_1Hz
      * GYR_ODR_56_05Hz
      * GYR_ODR_28_025H
      * */
    SensorQMI8658::GYR_ODR_896_8Hz,
    /*
    *  LPF_MODE_0     //2.66% of ODR
    *  LPF_MODE_1     //3.63% of ODR
    *  LPF_MODE_2     //5.39% of ODR
    *  LPF_MODE_3     //13.37% of ODR
    * */
    SensorQMI8658::LPF_MODE_3,
    // selfTest enable
    true);
  qmi.enableGyroscope();
  qmi.enableAccelerometer();
  qmi.dumpCtrlRegister();
  #pragma endregion

  pinMode(TFT_BL,OUTPUT);
  digitalWrite(TFT_BL,HIGH);


  //Touch controller setup to Serial
  touch.begin();
  #pragma region 
  Serial.print(touch.data.version);
  Serial.print("\t");
  Serial.print(touch.data.versionInfo[0]);
  Serial.print("-");
  Serial.print(touch.data.versionInfo[1]);
  Serial.print("-");
  Serial.println(touch.data.versionInfo[2]);
  #pragma endregion
  ui_init();

  Serial.println( "Setup done" );

}

void loop() {
  //Dump the accelerometer data to serial
  #ifdef DUMP_QMI_TO_SERIAL
    if (qmi.getDataReady()) {
      if (qmi.getAccelerometer(acc.x, acc.y, acc.z)) {
        Serial.print("{ACCEL: ");
        Serial.print(acc.x);
        Serial.print(",");
        Serial.print(acc.y);
        Serial.print(",");
        Serial.print(acc.z);
        Serial.println("}");
      }
    }
  #endif

  if(CAN_MSGAVAIL == CAN.checkReceive()) {
    CAN.readMsgBuf(&len, RxBuffer);
    if(CAN.getCanId()==FrameID) {
      OBD_length  = RxBuffer[0];
      OBD_mode    = RxBuffer[1];
      OBD_command = RxBuffer[2];
      byteA       = RxBuffer[3];
      byteB       = RxBuffer[4];
      byteC       = RxBuffer[5];
      byteD       = RxBuffer[6];
      byteE       = RxBuffer[7];
      if((OBD_mode-0x40)==0x01) { //Check we are in the correct mode
        switch (OBD_command)
        {
        case 0x05: //Engine Coolant Temperature : engineCoolantTemp
          engineCoolantTemp = ((int)byteA - 40);
          break;
        case 0x0B:
          intakeManifoldPressure = (int)byteA;
          break;
        case 0x0F :
          intakeTemp = ((int)byteA - 40);
          break;
        case 0x33 : 
          absBaroPressure = (int)byteA;
          break;
        case 0x42 : //Control module voltage... arc is in mV
          controlModuleVoltage = (256*(int)byteA + (int)byteB)/1000.0;
          break;
        default:
          break;
        }
        boostPressure = intakeManifoldPressure - absBaroPressure;
      }
    }
  }

  //current placeholder for engineCoolantTemp, should be a CAN.available()
  //engineCoolantTemp = 30 + millis()%13;
  //engineCoolantTemp = 30 + (int)(50*sin(2*PI*millis()/5000));
  if(refreshValues) {
    //start by refreshing the existing components
    lv_arc_set_value(ui_coolantArc,engineCoolantTemp);
    lv_label_set_text_fmt(ui_coolantVal, "%03d",engineCoolantTemp);

    lv_arc_set_value(ui_iatArc,intakeTemp);
    lv_label_set_text_fmt(ui_iatVal,"%03d",intakeTemp);

    lv_arc_set_value(ui_boostArc,boostPressure);
    lv_label_set_text_fmt(ui_boostVal,"%03d",boostPressure);

    lv_arc_set_value(ui_voltageArc,(int32_t)(controlModuleVoltage*1000));
    lv_label_set_text_fmt(ui_voltageVal,"%02.1f",controlModuleVoltage);
    Serial.print("Control Module Voltage : ");
    Serial.println(controlModuleVoltage);

    // Then send the requested messages
    // TxBuffer[2] = 0x05; //Engine coolant temperature first
    // CAN.sendMsgBuf(0x7DF,0,8,TxBuffer);
    // TxBuffer[2] = 0x0B; //Intake Manifold Pressure
    // CAN.sendMsgBuf(0x7DF,0,8,TxBuffer);
    // TxBuffer[2] = 0x0F; //Intake Temp
    // CAN.sendMsgBuf(0x7DF,0,8,TxBuffer);
    // TxBuffer[2] = 0x33; //Absolute Barometric Pressure
    // CAN.sendMsgBuf(0x7DF,0,8,TxBuffer);
    // TxBuffer[2] = 0x42; // Control Module Voltage
    // CAN.sendMsgBuf(0x7DF,0,8,TxBuffer);
  }

  if (OBDrequestDelay) {
    switch (requestID)
    {
    case 0x05:
      TxBuffer[2] = 0x05; //Engine coolant temperature first
      CAN.sendMsgBuf(0x7DF,0,8,TxBuffer);
      requestID = 0x0B;
      break;
    case 0x0B:
      TxBuffer[2] = 0x0B; //Engine coolant temperature first
      CAN.sendMsgBuf(0x7DF,0,8,TxBuffer);
      requestID = 0x0F;
      break;
    case 0x0F:
      TxBuffer[2] = 0x0F; //Engine coolant temperature first
      CAN.sendMsgBuf(0x7DF,0,8,TxBuffer);
      requestID = 0x33;
      break;
    case 0x33:
      TxBuffer[2] = 0x33; //Engine coolant temperature first
      CAN.sendMsgBuf(0x7DF,0,8,TxBuffer);
      requestID = 0x42;
      break;
    case 0x42:
      TxBuffer[2] = 0x42; //Engine coolant temperature first
      CAN.sendMsgBuf(0x7DF,0,8,TxBuffer);
      requestID = 0x05;
      break;
    default:
      requestID = 0x05;
      break;
    }
  }
  //Stuff for LVGL. Should be able to do something better than delays
  if(tickerLVGL) {
    lv_task_handler();
    lv_tick_inc(5);
  }

}
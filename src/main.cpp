#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include "CST816S.h"
#include "SensorQMI8658.hpp"
#include <lvgl.h>
#include <ui.h>

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

#define DRAW_BUF_SIZE (TFT_HOR_RES * TFT_VER_RES / 10 * (LV_COLOR_DEPTH / 8))
uint32_t draw_buf[DRAW_BUF_SIZE];

CST816S touch(TP_SDA, TP_SCL, TP_RST, TP_INT);
SensorQMI8658 qmi;
IMUdata acc;
IMUdata gyr;

int oilTemp;
long boostPressure;
int waterTemp;

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

  //current placeholder for waterTemp
  waterTemp = 30 + millis()%30;

  lv_arc_set_value(ui_coolantArc,waterTemp);
  lv_label_set_text_fmt(ui_coolantVal, "%03u",waterTemp);
  
  //Stuff for LVGL. Should be able to do something better than delays
  lv_task_handler();
  lv_tick_inc(5);
  delay(5);

}
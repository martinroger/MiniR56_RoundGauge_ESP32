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
//static lv_draw_buf_t draw_buf;
//static lv_color_t buf[240*240/10];

//TFT_eSPI tft = TFT_eSPI(TFT_WIDTH,TFT_HEIGHT);
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
  
  pinMode(TFT_BL,OUTPUT);
  digitalWrite(TFT_BL,HIGH);

  //These GPIOs are available
  // pinMode(15,OUTPUT);
  // digitalWrite(15,HIGH);
  // pinMode(16,OUTPUT);
  // digitalWrite(16,LOW);
  // pinMode(17,OUTPUT);
  // digitalWrite(17,HIGH);
  // pinMode(18,OUTPUT);
  // digitalWrite(18,LOW);
  // pinMode(21,OUTPUT);
  // digitalWrite(21,HIGH);
  // pinMode(33,OUTPUT);
  // digitalWrite(33,LOW);

  //Touch controller setup to Serial
  touch.begin();
  Serial.print(touch.data.version);
  Serial.print("\t");
  Serial.print(touch.data.versionInfo[0]);
  Serial.print("-");
  Serial.print(touch.data.versionInfo[1]);
  Serial.print("-");
  Serial.println(touch.data.versionInfo[2]);

  ui_init();

  Serial.println( "Setup done" );

}

void loop() {
  // put your main code here, to run repeatedly:


  // oilTemp = 50+25*(int)sin(2*PI*millis());
  //waterTemp = 70 + (int)(10.0*sin(2*PI*millis()));
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
   waterTemp = 70 + millis()%30;
   Serial.println(waterTemp);

  lv_arc_set_value(ui_coolantArc,waterTemp);

  lv_task_handler();
  lv_tick_inc(5);
  delay(5);



/*   if (touch.available()) {
    Serial.print(touch.gesture());
    Serial.print("\t");
    Serial.print(touch.data.points);
    Serial.print("\t");
    Serial.print(touch.data.event);
    Serial.print("\t");
    Serial.print(touch.data.x);
    Serial.print("\t");
    Serial.println(touch.data.y);

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

      if (qmi.getGyroscope(gyr.x, gyr.y, gyr.z)) {
        Serial.print("{GYRO: ");
        Serial.print(gyr.x);
        Serial.print(",");
        Serial.print(gyr.y );
        Serial.print(",");
        Serial.print(gyr.z);
        Serial.println("}");
      }
      Serial.printf("\t\t\t\t > %lu  %.2f *C\n", qmi.getTimestamp(), qmi.getTemperature_C());
    }
  } */

}
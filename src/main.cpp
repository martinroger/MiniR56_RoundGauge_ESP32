#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include "CST816S.h"
#include "SensorQMI8658.hpp"

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


TFT_eSPI tft = TFT_eSPI(TFT_WIDTH,TFT_HEIGHT);
CST816S touch(TP_SDA, TP_SCL, TP_RST, TP_INT);
SensorQMI8658 qmi;
IMUdata acc;
IMUdata gyr;

TFT_eSprite background = TFT_eSprite(&tft);


void setup() {
  Serial.begin(115200);
  #ifdef SERIALDEBUG
    while(!Serial.available()) {}
  #endif
  if (!qmi.begin(Wire, QMI8658_L_SLAVE_ADDRESS, SENSOR_SDA, SENSOR_SCL)) {
    Serial.println("Failed to find QMI8658 - check your wiring!");
    while (1) {
      delay(1000);
    }
  }
  #ifdef SERIALDEBUG
    Serial.print("Device ID:");
    Serial.println(qmi.getChipID(), HEX);
  #endif
  qmi.configAccelerometer(
    /*
      * ACC_RANGE_2G
      * ACC_RANGE_4G
      * ACC_RANGE_8G
      * ACC_RANGE_16G
      * */
    SensorQMI8658::ACC_RANGE_2G,
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
    SensorQMI8658::ACC_ODR_62_5Hz,
    /*
    *  LPF_MODE_0     //2.66% of ODR
    *  LPF_MODE_1     //3.63% of ODR
    *  LPF_MODE_2     //5.39% of ODR
    *  LPF_MODE_3     //13.37% of ODR
    * */
    SensorQMI8658::LPF_MODE_2,
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

  tft.begin();
  //tft.initDMA();

  tft.fillScreen(TFT_WHITE);

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

  //Touch screen setup to Serial
  touch.begin();
  #ifdef SERIALDEBUG
    Serial.print(touch.data.version);
    Serial.print("\t");
    Serial.print(touch.data.versionInfo[0]);
    Serial.print("-");
    Serial.print(touch.data.versionInfo[1]);
    Serial.print("-");
    Serial.println(touch.data.versionInfo[2]);
  #endif

  background.createSprite(240,240);
  background.fillSprite(TFT_DARKCYAN);
  background.setTextColor(TFT_WHITE);
  background.setCursor(20,120,2);
  background.print("X:");background.print(0.5);background.print(" Y:");background.print(0.5);background.print(" Z:");background.print(0.5);
  background.pushSprite(0,0);

}

void loop() {
  // put your main code here, to run repeatedly:
  #ifdef SERIALDEBUG
  if (touch.available()) {
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
  }
  #endif
  if(qmi.getDataReady()) {
    if(qmi.getAccelerometer(acc.x,acc.y,acc.z)) {
      background.fillSprite(TFT_DARKCYAN);
      background.setTextColor(TFT_CYAN);
      background.setCursor(20,120,2);
      background.print("X:");background.print(acc.x);background.print(" Y:");background.print(acc.y);background.print(" Z:");background.print(acc.z);
      background.fillSmoothCircle(120+(int32_t)(acc.y*80),120-(int32_t)(acc.x*80),10,TFT_RED);
      background.pushSprite(0,0);
    }
  }
  delay(5);

}
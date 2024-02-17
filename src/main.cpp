#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include "CST816S.h"

TFT_eSPI tft = TFT_eSPI(TFT_WIDTH,TFT_HEIGHT);

void setup() {
  // put your setup code here, to run once:

  pinMode(TFT_BL,OUTPUT);
  digitalWrite(TFT_BL,HIGH);
  tft.begin();
  tft.fillScreen(TFT_WHITE);

  //These GPIOs are available
  pinMode(15,OUTPUT);
  digitalWrite(15,HIGH);
  pinMode(16,OUTPUT);
  digitalWrite(16,LOW);
  pinMode(17,OUTPUT);
  digitalWrite(17,HIGH);
  pinMode(18,OUTPUT);
  digitalWrite(18,LOW);
  pinMode(21,OUTPUT);
  digitalWrite(21,HIGH);
  pinMode(33,OUTPUT);
  digitalWrite(33,LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  tft.fillScreen(TFT_RED);
  delay(1000);
  tft.fillScreen(TFT_BLUE);
  delay(1000);
  tft.fillScreen(TFT_GREEN);
  delay(1000);
}
#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include "CST816S.h"

TFT_eSPI tft = TFT_eSPI(TFT_WIDTH,TFT_HEIGHT);
// put function declarations here:
int myFunction(int, int);

void setup() {
  // put your setup code here, to run once:
  int result = myFunction(2, 3);
  pinMode(TFT_BL,OUTPUT);
  digitalWrite(TFT_BL,HIGH);
  tft.begin();
  tft.fillScreen(TFT_WHITE);
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

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}
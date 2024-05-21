#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include "CST816S.h"
#include <lvgl.h>
#include <Arduino_Helpers.h>
#include <AH/Timing/MillisMicrosTimer.hpp>



//Touch and QMI I2C setup
#define TP_INT 5
#define TP_SDA 6
#define TP_SCL 7
#define TP_RST 13


//Not sure if useful
#define TFT_HOR_RES 240
#define TFT_VER_RES 240

#define DRAW_BUF_SIZE (TFT_HOR_RES * TFT_VER_RES / 4 * (LV_COLOR_DEPTH / 8))
uint32_t draw_buf[DRAW_BUF_SIZE];

CST816S touch(TP_SDA, TP_SCL, TP_RST, TP_INT);


//Timers
Timer<millis> tickerLVGL = 5;
Timer<millis> slowTicker = 1000;


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


void ui_init() {
  lv_obj_t* label = lv_label_create(lv_scr_act());
  lv_label_set_text(label,"Hi"),
  lv_obj_align(label,LV_ALIGN_CENTER,0,0);

}

void setup() {



  Serial.begin(115200);
  lv_init();

  #if LV_USE_LOG != 0
    lv_log_register_print_cb( my_print );
  #endif

  lv_display_t * disp;
  disp = lv_tft_espi_create(TFT_HOR_RES, TFT_VER_RES, draw_buf, sizeof(draw_buf));

  lv_indev_t *indev = lv_indev_create();
  lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
  lv_indev_set_read_cb(indev,touchRead);

  pinMode(TFT_BL,OUTPUT);
  digitalWrite(TFT_BL,HIGH);


  //Touch controller setup to Serial
  touch.begin();

  ui_init();
  Serial.println( "Setup done" );
}

void loop() {

  //Stuff for LVGL. Should be able to do something better than delays
  if(tickerLVGL) {
    lv_task_handler();
    lv_tick_inc(5);
  }

}
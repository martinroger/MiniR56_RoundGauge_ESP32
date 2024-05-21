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

uint8_t targetVal = 50;

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


lv_obj_t* label;
lv_obj_t* valueArc;

//Callback for updating the arc
void animateArc(void *var, int32_t value) {
  lv_arc_set_value(valueArc,value);

}

void labelValueChanged(lv_event_t* e) {
  lv_event_code_t event_code = lv_event_get_code(e);
  if(event_code==LV_EVENT_VALUE_CHANGED) {
    //lv_arc_set_value(valueArc,targetVal); //Direct update of the arc value
    //Doing it via an animation :
    lv_anim_t arcAnim;
    lv_anim_init(&arcAnim);
    lv_anim_set_var(&arcAnim,valueArc);
    lv_anim_set_values(&arcAnim,lv_arc_get_value(valueArc),targetVal);
    lv_anim_set_duration(&arcAnim,500);
    lv_anim_set_exec_cb(&arcAnim, animateArc);

    lv_anim_set_path_cb(&arcAnim,lv_anim_path_linear);
    lv_anim_start(&arcAnim);
  }
  //Serial.println((uint32_t)event_code);
}

void ui_init() {
  label = lv_label_create(lv_screen_active());
  lv_label_set_text(label,"Hi"),
  lv_obj_align(label,LV_ALIGN_CENTER,0,0);

  valueArc = lv_arc_create(lv_screen_active());
  lv_obj_align(valueArc,LV_ALIGN_CENTER,0,0);
  lv_obj_set_size(valueArc,200,200);
  /*
  lv_obj_clear_flag(valueArc,LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE |
                      LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SNAPPABLE | LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC |
                      LV_OBJ_FLAG_SCROLL_MOMENTUM | LV_OBJ_FLAG_SCROLL_CHAIN);
  */

  lv_obj_set_style_bg_opa(valueArc,0,LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_arc_set_range(valueArc,0,255);
  lv_arc_set_value(valueArc,targetVal);
  

  lv_obj_add_event_cb(label,labelValueChanged,LV_EVENT_VALUE_CHANGED,NULL);

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

  // if(slowTicker) {
  //   lv_label_set_text_fmt(label,"%d",targetVal);
  // }

  if(Serial.available()) {
    targetVal = Serial.read();
    lv_label_set_text_fmt(label,"%d",targetVal);
    //Send this to trigger the arc update. Technically could also just update the arc + animate
    lv_obj_send_event(label,LV_EVENT_VALUE_CHANGED,NULL);
  }

}
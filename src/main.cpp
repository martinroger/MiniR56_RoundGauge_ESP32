#include <Arduino.h>
// #include <Arduino_Helpers.h>
// #include <AH/Timing/MillisMicrosTimer.hpp>
// #include "lv_conf.h"

#include "lvgl_port.h"
#include <ui.h>

#ifndef TFT_BL
#define TFT_BL 5
#endif

#ifndef SCREEN_ID_MAIN
#define SCREEN_ID_MAIN 1
#endif

// Display buffer preparation

#define DRAW_BUF_SIZE (TFT_HOR_RES * TFT_VER_RES / 4 * (LV_COLOR_DEPTH / 8))
uint32_t draw_buf[DRAW_BUF_SIZE];

// Touch initialisation
// #define TP_INT 4
// #define TP_SDA 1
// #define TP_SCL 3
// #define TP_RST -1
// CST816S touch(TP_SDA, TP_SCL, TP_RST, TP_INT);
// void touchRead(lv_indev_t *indev, lv_indev_data_t *data)
// {

//   if(touch.available()) {
//     data->state = LV_INDEV_STATE_PRESSED;
//     data->point.x = touch.data.x;
//     data->point.y = touch.data.y;
//   }
//   else {
//     data->state = LV_INDEV_STATE_RELEASED;
//   }
// }

// Timers
#ifndef TICKS
#define TICKS 5
#endif
// Timer<millis> tickerLVGL      =   TICKS;    //LVGL 5ms ticker
// Timer<millis> refreshValues   =   100;  //Values refresh interval on the screens
// Timer<millis> OBDrequestDelay =   100;   //Interval for requests over OBD
#ifndef DISP_VALUES_REFRESH_INTERVAL
#define DISP_VALUES_REFRESH_INTERVAL 100
#endif
#ifndef OBD_QUERY_REFRESH_INTERVAL
#define OBD_QUERY_REFRESH_INTERVAL 100
#endif
unsigned long lastLVGLTicked = 0;
unsigned long lastDispValuesRefreshed = 0;

// Vehicle variables
bool indicatorsOn = true;
bool highBeamOn = true;
bool lowFuelOn = true;
bool overTemperatureOn = true;
bool brakesOn = true;
bool absOn = true;
bool lowCoolantOn = true;
bool batteryOn = true;
bool lowOilOn = true;
bool milOn = true;
bool airbagOn = true;
uint32_t speed = 0;
uint32_t rpm = 0;
uint8_t fuelLevel = 50;
uint8_t coolant = 88;

#define BRIGHTNESS 215

// First read flags

bool screenON = false;

void generateValues()
{
    speed = 120 + 120 * sin(millis() / 5000);
    rpm = 3500 + 3500 * sin(millis() / 10000);
    fuelLevel = 50 + 50 * sin(millis() / 15000);
    coolant = 88 + 12 * sin(millis() / 20000);
    indicatorsOn = (millis() / 1000) % 2 == 0;
    highBeamOn = random(0, 100) > 50;
    lowFuelOn = fuelLevel < 20;
    overTemperatureOn = coolant > 95;
    brakesOn = random(0, 100) > 50;
    absOn = random(0, 100) > 50;
    lowCoolantOn = random(0, 100) > 50;
    batteryOn = random(0, 100) > 50;
    lowOilOn = random(0, 100) > 50;
    milOn = random(0, 100) > 50;
    airbagOn = random(0, 100) > 50;
}

#if LV_USE_LOG != 0
void my_print(lv_log_level_t level, const char *buf)
{
    LV_UNUSED(level);
    Serial.println(buf);
    Serial.flush();
}
#endif

void setup()
{

    // For Debug
    Serial.begin(115200);

    generateValues();

    // Touch startup
    //   touch.begin();

    // ESP Panel
    //   ESP_Panel *panel = new ESP_Panel();
    //   panel->init();

    //   panel->begin();

    // LV startup sequence
    //   lv_init();
    displayInit();
#if LV_USE_LOG != 0
    lv_log_register_print_cb(my_print);
#endif
    //   lv_display_t * disp;
    //   disp = lv_display_create(TFT_HOR_RES,TFT_VER_RES);
    //   //disp = lv_tft_espi_create(TFT_HOR_RES, TFT_VER_RES, draw_buf, sizeof(draw_buf));
    //   lv_indev_t *indev = lv_indev_create();
    //   lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    //   lv_indev_set_read_cb(indev,touchRead);
    //   lv_display_set_rotation(disp,LV_DISPLAY_ROTATION_270);
    // Draw screens
    ui_init();

    // Debug
    Serial.println("Setup done");
}

void loop()
{

    generateValues();

    // Refresh the items in the UI
    if ((millis() - lastDispValuesRefreshed) > DISP_VALUES_REFRESH_INTERVAL)
    {
        lastDispValuesRefreshed = millis();
        lv_arc_set_value(objects.speed_arc,speed);
        lv_arc_set_value(objects.rpm_arc,rpm);
        lv_bar_set_value(objects.fuel_bar,fuelLevel,LV_ANIM_OFF);
        lv_bar_set_value(objects.coolant_bar,coolant,LV_ANIM_OFF);
        lv_label_set_text_fmt(objects.fuel_level,"%03d",fuelLevel);
        lv_label_set_text_fmt(objects.speed,"%03d",speed);
        lv_label_set_text_fmt(objects.rpm,"%04d",rpm);
        lv_label_set_text_fmt(objects.coolant,"%03d",coolant);
        lv_obj_set_style_image_opa(objects.low_fuel_tt, lowFuelOn ? LV_OPA_COVER : LV_OPA_TRANSP, LV_STATE_DEFAULT);
        lv_obj_set_style_image_opa(objects.over_temperature_tt, overTemperatureOn ? LV_OPA_COVER : LV_OPA_TRANSP, LV_STATE_DEFAULT);
        lv_obj_set_style_image_opa(objects.brakes_tt, brakesOn ? LV_OPA_COVER : LV_OPA_TRANSP, LV_STATE_DEFAULT);
        lv_obj_set_style_image_opa(objects.abs_tt, absOn ? LV_OPA_COVER : LV_OPA_TRANSP, LV_STATE_DEFAULT);
        lv_obj_set_style_image_opa(objects.low_coolant_tt, lowCoolantOn ? LV_OPA_COVER : LV_OPA_TRANSP, LV_STATE_DEFAULT);
        lv_obj_set_style_image_opa(objects.battery_tt, batteryOn ? LV_OPA_COVER : LV_OPA_TRANSP, LV_STATE_DEFAULT);
        lv_obj_set_style_image_opa(objects.low_oil_tt, lowOilOn ? LV_OPA_COVER : LV_OPA_TRANSP, LV_STATE_DEFAULT);
        lv_obj_set_style_image_opa(objects.mil_tt, milOn ? LV_OPA_COVER : LV_OPA_TRANSP, LV_STATE_DEFAULT);
        lv_obj_set_style_image_opa(objects.hi_beam_tt, highBeamOn ? LV_OPA_COVER : LV_OPA_TRANSP, LV_STATE_DEFAULT);
        lv_obj_set_style_image_opa(objects.indicators_tt, indicatorsOn ? LV_OPA_COVER : LV_OPA_TRANSP, LV_STATE_DEFAULT);
        lv_obj_set_style_image_opa(objects.airbag_tt, airbagOn ? LV_OPA_COVER : LV_OPA_TRANSP, LV_STATE_DEFAULT);
        
        // Update shit here
    }
    lv_timer_handler();

    // Initial screenON
    if (!screenON)
    {
        uint8_t tempbrightness = 0;
        while (tempbrightness < BRIGHTNESS)
        {
            backLight->setBrightness(tempbrightness * 100 / 255);
            // analogWrite(TFT_BL,tempbrightness);
            delay(3);
            tempbrightness++;
        }
        // analogWrite(TFT_BL,BRIGHTNESS);
        screenON = true;
    }
}
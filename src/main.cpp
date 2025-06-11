#include <Arduino.h>
// #include <Arduino_Helpers.h>
// #include <AH/Timing/MillisMicrosTimer.hpp>
// #include "lv_conf.h"
#include <esp_display_panel.hpp>
#include <lvgl.h>
#include "lvgl_v9_port.h"
#include <ui.h>

using namespace esp_panel::drivers;
using namespace esp_panel::board;

#ifndef SCREEN_ID_MAIN
#define SCREEN_ID_MAIN 1
#endif

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
bool indicatorsOn, p_indicatorsOn = true;
bool highBeamOn, p_highBeamOn = true;
bool lowFuelOn, p_lowFuelOn = true;
bool overTemperatureOn, p_overTemperatureOn = true;
bool brakesOn, p_brakesOn = true;
bool absOn, p_absOn = true;
bool lowCoolantOn, p_lowCoolantOn = true;
bool batteryOn, p_batteryOn = true;
bool lowOilOn, p_lowOilOn = true;
bool milOn, p_milOn = true;
bool airbagOn, p_airbagOn = true;
uint32_t speed, p_speed = 0;
uint32_t rpm, p_rpm = 0;
uint8_t fuelLevel, p_fuelLevel = 50;
uint8_t coolant, p_coolant = 88;

#define BRIGHTNESS 215

// First read flags

bool screenON = false;

void generateValues()
{
    speed = 120 + 120 * sin((float)millis() / 5000.0);
    rpm = 100 * (uint8_t)((3500 + 3500 * sin((float)millis() / 10000.0)) / 100);
    fuelLevel = 50 + 50 * sin((float)millis() / 15000.0);
    coolant = 88 + 12 * sin((float)millis() / 20000.0);
    indicatorsOn = (millis() / 800) % 2 == 0;
    highBeamOn = (millis() / 1100) % 2 == 0;
    lowFuelOn = fuelLevel < 20;
    overTemperatureOn = coolant > 95;
    brakesOn = (millis() / 1200) % 2 == 0;
    absOn = (millis() / 1250) % 2 == 0;
    lowCoolantOn = (millis() / 1300) % 2 == 0;
    batteryOn = (millis() / 1400) % 2 == 0;
    lowOilOn = (millis() / 1425) % 2 == 0;
    milOn = (millis() / 1500) % 2 == 0;
    airbagOn = (millis() / 1382) % 2 == 0;
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

    Board *board = new Board();
    board->init();
    auto lcd = board->getLCD();
    lcd->configFrameBufferNumber(LVGL_PORT_BUFFER_NUM);
#if ESP_PANEL_DRIVERS_BUS_ENABLE_RGB && CONFIG_IDF_TARGET_ESP32S3
    auto lcd_bus = lcd->getBus();
    /**
     * As the anti-tearing feature typically consumes more PSRAM bandwidth, for the ESP32-S3, we need to utilize the
     * "bounce buffer" functionality to enhance the RGB data bandwidth.
     * This feature will consume `bounce_buffer_size * bytes_per_pixel * 2` of SRAM memory.
     */
    if (lcd_bus->getBasicAttributes().type == ESP_PANEL_BUS_TYPE_RGB)
    {
        static_cast<BusRGB *>(lcd_bus)->configRGB_BounceBufferSize(lcd->getFrameWidth() * 10);
    }
#endif

    assert(board->begin());

    lvgl_port_init(board->getLCD(), board->getTouch());

    lvgl_port_lock(-1);
    ui_init();
    lvgl_port_unlock();
    generateValues();

    // #if LV_USE_LOG != 0
    //     lv_log_register_print_cb(my_print);
    // #endif

    // Debug
    Serial.println("Setup done");
}

void loop()
{

    generateValues();

    // Refresh the items in the UI
    if ((millis() - lastDispValuesRefreshed) > DISP_VALUES_REFRESH_INTERVAL)
    {

        if (lvgl_port_lock(2))
        {
            lastDispValuesRefreshed = millis();
            if (p_speed != speed)
            {
                lv_arc_set_value(objects.speed_arc, speed);
                lv_label_set_text_fmt(objects.speed, "%03d", speed);
                p_speed = speed;
            }
            if (p_rpm != rpm)
            {
                lv_arc_set_value(objects.rpm_arc, rpm);
                lv_label_set_text_fmt(objects.rpm, "%04d", rpm);
                p_rpm = rpm;
            }
            if (p_fuelLevel != fuelLevel)
            {
                lv_bar_set_value(objects.fuel_bar, fuelLevel, LV_ANIM_OFF);
                lv_label_set_text_fmt(objects.fuel_level, "%03d", fuelLevel);
                p_fuelLevel = fuelLevel;
            }
            if (p_coolant != coolant)
            {
                lv_bar_set_value(objects.coolant_bar, coolant, LV_ANIM_OFF);
                lv_label_set_text_fmt(objects.coolant, "%03d", coolant);
                p_coolant = coolant;
            }
            if (p_absOn != absOn)
            {
                lv_obj_set_style_image_opa(objects.abs_tt, absOn ? LV_OPA_COVER : LV_OPA_TRANSP, LV_STATE_DEFAULT);
                p_absOn = absOn;
            }
            if (p_lowFuelOn != lowFuelOn)
            {
                lv_obj_set_style_image_opa(objects.low_fuel_tt, lowFuelOn ? LV_OPA_COVER : LV_OPA_TRANSP, LV_STATE_DEFAULT);
                p_lowFuelOn = lowFuelOn;
            }
            if (p_overTemperatureOn != overTemperatureOn)
            {
                lv_obj_set_style_image_opa(objects.over_temperature_tt, overTemperatureOn ? LV_OPA_COVER : LV_OPA_TRANSP, LV_STATE_DEFAULT);
                p_overTemperatureOn = overTemperatureOn;
            }
            if (p_brakesOn != brakesOn)
            {
                lv_obj_set_style_image_opa(objects.brakes_tt, brakesOn ? LV_OPA_COVER : LV_OPA_TRANSP, LV_STATE_DEFAULT);
                p_brakesOn = brakesOn;
            }
            if (p_lowCoolantOn != lowCoolantOn)
            {
                lv_obj_set_style_image_opa(objects.low_coolant_tt, lowCoolantOn ? LV_OPA_COVER : LV_OPA_TRANSP, LV_STATE_DEFAULT);
                p_lowCoolantOn = lowCoolantOn;
            }
            if (p_batteryOn != batteryOn)
            {
                lv_obj_set_style_image_opa(objects.battery_tt, batteryOn ? LV_OPA_COVER : LV_OPA_TRANSP, LV_STATE_DEFAULT);
                p_batteryOn = batteryOn;
            }
            if (p_lowOilOn != lowOilOn)
            {
                lv_obj_set_style_image_opa(objects.low_oil_tt, lowOilOn ? LV_OPA_COVER : LV_OPA_TRANSP, LV_STATE_DEFAULT);
                p_lowOilOn = lowOilOn;
            }
            if (p_milOn != milOn)
            {
                lv_obj_set_style_image_opa(objects.mil_tt, milOn ? LV_OPA_COVER : LV_OPA_TRANSP, LV_STATE_DEFAULT);
                p_milOn = milOn;
            }
            if (p_highBeamOn != highBeamOn)
            {
                lv_obj_set_style_image_opa(objects.hi_beam_tt, highBeamOn ? LV_OPA_COVER : LV_OPA_TRANSP, LV_STATE_DEFAULT);
                p_highBeamOn = highBeamOn;
            }
            if (p_indicatorsOn != indicatorsOn)
            {
                lv_obj_set_style_image_opa(objects.indicators_tt, indicatorsOn ? LV_OPA_COVER : LV_OPA_TRANSP, LV_STATE_DEFAULT);
                p_indicatorsOn = indicatorsOn;
            }
            if (p_airbagOn != airbagOn)
            {
                lv_obj_set_style_image_opa(objects.airbag_tt, airbagOn ? LV_OPA_COVER : LV_OPA_TRANSP, LV_STATE_DEFAULT);
                p_airbagOn = airbagOn;
            }
            lvgl_port_unlock();
        }

        // lv_obj_set_style_image_opa(objects.over_temperature_tt, overTemperatureOn ? LV_OPA_COVER : LV_OPA_TRANSP, LV_STATE_DEFAULT);
        // lv_obj_set_style_image_opa(objects.brakes_tt, brakesOn ? LV_OPA_COVER : LV_OPA_TRANSP, LV_STATE_DEFAULT);
        // lv_obj_set_style_image_opa(objects.low_coolant_tt, lowCoolantOn ? LV_OPA_COVER : LV_OPA_TRANSP, LV_STATE_DEFAULT);
        // lv_obj_set_style_image_opa(objects.battery_tt, batteryOn ? LV_OPA_COVER : LV_OPA_TRANSP, LV_STATE_DEFAULT);
        // lv_obj_set_style_image_opa(objects.low_oil_tt, lowOilOn ? LV_OPA_COVER : LV_OPA_TRANSP, LV_STATE_DEFAULT);
        // lv_obj_set_style_image_opa(objects.mil_tt, milOn ? LV_OPA_COVER : LV_OPA_TRANSP, LV_STATE_DEFAULT);
        // lv_obj_set_style_image_opa(objects.hi_beam_tt, highBeamOn ? LV_OPA_COVER : LV_OPA_TRANSP, LV_STATE_DEFAULT);
        // lv_obj_set_style_image_opa(objects.indicators_tt, indicatorsOn ? LV_OPA_COVER : LV_OPA_TRANSP, LV_STATE_DEFAULT);
        // lv_obj_set_style_image_opa(objects.airbag_tt, airbagOn ? LV_OPA_COVER : LV_OPA_TRANSP, LV_STATE_DEFAULT);

        // Update shit here
    }
    // lv_timer_handler();

    // // Initial screenON
    // if (!screenON)
    // {
    //     uint8_t tempbrightness = 0;
    //     while (tempbrightness < BRIGHTNESS)
    //     {
    //         backLight->setBrightness(tempbrightness * 100 / 255);
    //         // analogWrite(TFT_BL,tempbrightness);
    //         delay(3);
    //         tempbrightness++;
    //     }
    //     // analogWrite(TFT_BL,BRIGHTNESS);
    //     screenON = true;
}

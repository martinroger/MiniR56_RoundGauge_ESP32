#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _objects_t {
    lv_obj_t *coolant_scr;
    lv_obj_t *boost_scr;
    lv_obj_t *iat_scr;
    lv_obj_t *voltage_scr;
    lv_obj_t *boost_scr_arc;
    lv_obj_t *boost_scr_can;
    lv_obj_t *boost_scr_currentvalue;
    lv_obj_t *boost_scr_max;
    lv_obj_t *boost_scr_min;
    lv_obj_t *boost_scr_name;
    lv_obj_t *boost_scr_unit;
    lv_obj_t *coolant_scr_arc;
    lv_obj_t *coolant_scr_can;
    lv_obj_t *coolant_scr_currentvalue;
    lv_obj_t *coolant_scr_max;
    lv_obj_t *coolant_scr_maxarc;
    lv_obj_t *coolant_scr_min;
    lv_obj_t *coolant_scr_minarc;
    lv_obj_t *coolant_scr_name;
    lv_obj_t *coolant_scr_unit;
    lv_obj_t *iat_scr_arc;
    lv_obj_t *iat_scr_can;
    lv_obj_t *iat_scr_currentvalue;
    lv_obj_t *iat_scr_max;
    lv_obj_t *iat_scr_min;
    lv_obj_t *iat_scr_name;
    lv_obj_t *iat_scr_unit;
    lv_obj_t *voltage_scr_arc;
    lv_obj_t *voltage_scr_can;
    lv_obj_t *voltage_scr_currentvalue;
    lv_obj_t *voltage_scr_max;
    lv_obj_t *voltage_scr_min;
    lv_obj_t *voltage_scr_name;
    lv_obj_t *voltage_scr_unit;
} objects_t;

extern objects_t objects;

enum ScreensEnum {
    SCREEN_ID_COOLANT_SCR = 1,
    SCREEN_ID_BOOST_SCR = 2,
    SCREEN_ID_IAT_SCR = 3,
    SCREEN_ID_VOLTAGE_SCR = 4,
};

void create_screen_coolant_scr();
void tick_screen_coolant_scr();

void create_screen_boost_scr();
void tick_screen_boost_scr();

void create_screen_iat_scr();
void tick_screen_iat_scr();

void create_screen_voltage_scr();
void tick_screen_voltage_scr();

void create_screens();
void tick_screen(int screen_index);


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/
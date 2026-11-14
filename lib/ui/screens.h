#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include "vars.h"
#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _objects_t {
    lv_obj_t *coolant_scr;
    lv_obj_t *boost_scr;
    lv_obj_t *iat_scr;
    lv_obj_t *maf_scr;
    lv_obj_t *hpfp_scr;
    lv_obj_t *oil_tscr;
    lv_obj_t *boost_scr_arc;
    lv_obj_t *boost_scr_can;
    lv_obj_t *boost_scr_currentvalue;
    lv_obj_t *boost_scr_key;
    lv_obj_t *boost_scr_kl15;
    lv_obj_t *boost_scr_max;
    lv_obj_t *boost_scr_maxarc;
    lv_obj_t *boost_scr_min;
    lv_obj_t *boost_scr_minarc;
    lv_obj_t *boost_scr_name;
    lv_obj_t *boost_scr_unit;
    lv_obj_t *coolant_scr_arc;
    lv_obj_t *coolant_scr_can;
    lv_obj_t *coolant_scr_currentvalue;
    lv_obj_t *coolant_scr_key;
    lv_obj_t *coolant_scr_kl15;
    lv_obj_t *coolant_scr_max;
    lv_obj_t *coolant_scr_maxarc;
    lv_obj_t *coolant_scr_min;
    lv_obj_t *coolant_scr_minarc;
    lv_obj_t *coolant_scr_name;
    lv_obj_t *coolant_scr_unit;
    lv_obj_t *hpfp_scr_arc;
    lv_obj_t *hpfp_scr_can;
    lv_obj_t *hpfp_scr_currentvalue;
    lv_obj_t *hpfp_scr_key;
    lv_obj_t *hpfp_scr_kl15;
    lv_obj_t *hpfp_scr_max;
    lv_obj_t *hpfp_scr_maxarc;
    lv_obj_t *hpfp_scr_min;
    lv_obj_t *hpfp_scr_minarc;
    lv_obj_t *hpfp_scr_name;
    lv_obj_t *hpfp_scr_unit;
    lv_obj_t *iat_scr_arc;
    lv_obj_t *iat_scr_can;
    lv_obj_t *iat_scr_currentvalue;
    lv_obj_t *iat_scr_key;
    lv_obj_t *iat_scr_kl15;
    lv_obj_t *iat_scr_max;
    lv_obj_t *iat_scr_maxarc;
    lv_obj_t *iat_scr_min;
    lv_obj_t *iat_scr_minarc;
    lv_obj_t *iat_scr_name;
    lv_obj_t *iat_scr_unit;
    lv_obj_t *maf_scr_arc;
    lv_obj_t *maf_scr_can;
    lv_obj_t *maf_scr_currentvalue;
    lv_obj_t *maf_scr_key;
    lv_obj_t *maf_scr_kl15;
    lv_obj_t *maf_scr_max;
    lv_obj_t *maf_scr_maxarc;
    lv_obj_t *maf_scr_min;
    lv_obj_t *maf_scr_minarc;
    lv_obj_t *maf_scr_name;
    lv_obj_t *maf_scr_unit;
    lv_obj_t *oil_tscr_arc;
    lv_obj_t *oil_tscr_can;
    lv_obj_t *oil_tscr_currentvalue;
    lv_obj_t *oil_tscr_key;
    lv_obj_t *oil_tscr_kl15;
    lv_obj_t *oil_tscr_max;
    lv_obj_t *oil_tscr_maxarc;
    lv_obj_t *oil_tscr_min;
    lv_obj_t *oil_tscr_minarc;
    lv_obj_t *oil_tscr_name;
    lv_obj_t *oil_tscr_unit;
    lv_obj_t *reset_boost_min_max_area;
    lv_obj_t *reset_coolant_min_max_area;
    lv_obj_t *reset_hpfp_min_max_area;
    lv_obj_t *reset_iat_min_max_area;
    lv_obj_t *reset_maf_min_max_area;
    lv_obj_t *reset_oil_tmin_max_area;
} objects_t;

extern objects_t objects;

enum ScreensEnum {
    SCREEN_ID_COOLANT_SCR = 1,
    SCREEN_ID_BOOST_SCR = 2,
    SCREEN_ID_IAT_SCR = 3,
    SCREEN_ID_MAF_SCR = 4,
    SCREEN_ID_HPFP_SCR = 5,
    SCREEN_ID_OIL_TSCR = 6,
};

void create_screen_coolant_scr();
void tick_screen_coolant_scr();

void create_screen_boost_scr();
void tick_screen_boost_scr();

void create_screen_iat_scr();
void tick_screen_iat_scr();

void create_screen_maf_scr();
void tick_screen_maf_scr();

void create_screen_hpfp_scr();
void tick_screen_hpfp_scr();

void create_screen_oil_tscr();
void tick_screen_oil_tscr();

void create_screens();
void tick_screen(int screen_index);


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/
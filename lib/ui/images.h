#ifndef EEZ_LVGL_UI_IMAGES_H
#define EEZ_LVGL_UI_IMAGES_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

extern const lv_img_dsc_t img_turn_indicators;
extern const lv_img_dsc_t img_airbag_srs;
extern const lv_img_dsc_t img_brakes;
extern const lv_img_dsc_t img_low_oil;
extern const lv_img_dsc_t img_hi_beams;
extern const lv_img_dsc_t img_low_coolant;
extern const lv_img_dsc_t img_battery;
extern const lv_img_dsc_t img_abs;
extern const lv_img_dsc_t img_mil;
extern const lv_img_dsc_t img_over_temperature;
extern const lv_img_dsc_t img_low_fuel;

#ifndef EXT_IMG_DESC_T
#define EXT_IMG_DESC_T
typedef struct _ext_img_desc_t {
    const char *name;
    const lv_img_dsc_t *img_dsc;
} ext_img_desc_t;
#endif

extern const ext_img_desc_t images[11];


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_IMAGES_H*/
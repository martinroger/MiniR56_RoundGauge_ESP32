#ifndef EEZ_LVGL_UI_ACTIONS_H
#define EEZ_LVGL_UI_ACTIONS_H

#include <lvgl.h>

extern void action_go_to_next_screen(lv_event_t * e);
extern void action_reset_coolant_min_max(lv_event_t * e);
extern void action_reset_boost_min_max(lv_event_t * e);
extern void action_reset_iat_min_max(lv_event_t * e);
extern void action_reset_voltage_min_max(lv_event_t * e);
extern void action_reset_maf_min_max(lv_event_t * e);
extern void action_reset_hpfp_min_max(lv_event_t * e);
extern void action_reset_oil_tmin_max(lv_event_t * e);


#endif /*EEZ_LVGL_UI_ACTIONS_H*/
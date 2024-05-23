#ifndef EEZ_LVGL_UI_ACTIONS_H
#define EEZ_LVGL_UI_ACTIONS_H

#include <lvgl.h>

extern void action_go_to_next_screen(lv_event_t * e);
extern void action_refresh_boost_scr_arc(lv_event_t * e);
extern void action_refresh_coolant_scr_arc(lv_event_t * e);
extern void action_refresh_iat_scr_arc(lv_event_t * e);
extern void action_refresh_voltage_scr_arc(lv_event_t * e);


#endif /*EEZ_LVGL_UI_ACTIONS_H*/
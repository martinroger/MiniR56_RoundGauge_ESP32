#include "ui.h"
#include "images.h"

static int16_t currentScreen = -1;

static lv_obj_t *getLvglObjectFromIndex(int32_t index) {
    if (index == -1) {
        return 0;
    }
    return ((lv_obj_t **)&objects)[index];
}

static const void *getLvglImageByName(const char *name) {
    for (size_t imageIndex = 0; imageIndex < sizeof(images) / sizeof(ext_img_desc_t); imageIndex++) {
        if (strcmp(images[imageIndex].name, name) == 0) {
            return images[imageIndex].img_dsc;
        }
    }
    return 0;
}

void loadScreen(enum ScreensEnum screenId) {
    currentScreen = screenId - 1;
    lv_obj_t *screen = getLvglObjectFromIndex(currentScreen);
    lv_screen_load_anim(screen, LV_SCR_LOAD_ANIM_FADE_IN, 200, 0, false);
}

void ui_init() {
    create_screens();
    loadScreen(SCREEN_ID_MAIN);
}

void ui_tick() {
    tick_screen(currentScreen);
}

void animateTargetArc(lv_obj_t* targetArc, int32_t targetValue) {
    lv_anim_t arcAnim;
    lv_anim_init(&arcAnim);
    lv_anim_set_var(&arcAnim,targetArc);
    lv_anim_set_values(&arcAnim,lv_arc_get_value(targetArc),targetValue);
    lv_anim_set_duration(&arcAnim,250);
    lv_anim_set_exec_cb(&arcAnim,(lv_anim_exec_xcb_t)lv_arc_set_value);
    lv_anim_set_path_cb(&arcAnim,lv_anim_path_ease_in_out);
    lv_anim_start(&arcAnim);
}

void action_go_to_next_screen(lv_event_t * e) {
    if(currentScreen==3) {
        loadScreen(1);
    }
    else {
        loadScreen(currentScreen+2);
    }
}

void updateCoolantArc(int32_t value) {
    animateTargetArc(objects.coolant_scr_arc,value);
}

void updateCoolantLabel(int32_t value) {
    lv_label_set_text_fmt(objects.coolant_scr_currentvalue,"%03d",value);
}

void updateBoostArc(int32_t value) {
    animateTargetArc(objects.boost_scr_arc,value);
}

void updateBoostLabel(int32_t value) {
    lv_label_set_text_fmt(objects.boost_scr_currentvalue,"%03d",value);
}

void updateIatArc(int32_t value) {
    animateTargetArc(objects.iat_scr_arc,value);
}

void updateIatLabel(int32_t value) {
    lv_label_set_text_fmt(objects.iat_scr_currentvalue,"%03d",value);
}

void updateVoltageArc(float value) {
    animateTargetArc(objects.voltage_scr_arc,(int32_t)(value*1000));
}

void updateVoltageLabel(float value) {
    lv_label_set_text_fmt(objects.voltage_scr_currentvalue,"%02.1f",value);
}


void setCanState(bool canState) {
    if(canState) {
        lv_obj_remove_state(objects.coolant_scr_can,LV_STATE_DISABLED);
        lv_obj_remove_state(objects.boost_scr_can,LV_STATE_DISABLED);
        lv_obj_remove_state(objects.iat_scr_can,LV_STATE_DISABLED);
        lv_obj_remove_state(objects.voltage_scr_can,LV_STATE_DISABLED);
    }
    else {
        lv_obj_add_state(objects.coolant_scr_can,LV_STATE_DISABLED);
        lv_obj_add_state(objects.boost_scr_can,LV_STATE_DISABLED);
        lv_obj_add_state(objects.iat_scr_can,LV_STATE_DISABLED);
        lv_obj_add_state(objects.voltage_scr_can,LV_STATE_DISABLED);
    }
}

// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.4
// LVGL version: 8.3.6
// Project name: SquareLine_Project

#ifndef _SQUARELINE_PROJECT_UI_H
#define _SQUARELINE_PROJECT_UI_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined __has_include
#if __has_include("lvgl.h")
#include "lvgl.h"
#elif __has_include("lvgl/lvgl.h")
#include "lvgl/lvgl.h"
#else
#include "lvgl.h"
#endif
#else
#include "lvgl.h"
#endif

#include "ui_helpers.h"
#include "ui_events.h"
// SCREEN: ui_coolantTemp
void ui_coolantTemp_screen_init(void);
void ui_event_coolantTemp(lv_event_t * e);
extern lv_obj_t * ui_coolantTemp;
extern lv_obj_t * ui_coolantArc;
extern lv_obj_t * ui_coolantVal;
extern lv_obj_t * ui_coolantUnit;
extern lv_obj_t * ui_coolantLabel;
// SCREEN: ui_boostPressure
void ui_boostPressure_screen_init(void);
void ui_event_boostPressure(lv_event_t * e);
extern lv_obj_t * ui_boostPressure;
extern lv_obj_t * ui_boostArc;
extern lv_obj_t * ui_boostVal;
extern lv_obj_t * ui_boostUnit;
extern lv_obj_t * ui_boostLabel;
// SCREEN: ui_intakeTemp
void ui_intakeTemp_screen_init(void);
void ui_event_intakeTemp(lv_event_t * e);
extern lv_obj_t * ui_intakeTemp;
extern lv_obj_t * ui_iatArc;
extern lv_obj_t * ui_iatVal;
extern lv_obj_t * ui_iatUnit;
extern lv_obj_t * ui_iatLabel;
// SCREEN: ui_moduleVoltage
void ui_moduleVoltage_screen_init(void);
void ui_event_moduleVoltage(lv_event_t * e);
extern lv_obj_t * ui_moduleVoltage;
extern lv_obj_t * ui_voltageArc;
extern lv_obj_t * ui_voltageVal;
extern lv_obj_t * ui_voltageUnit;
extern lv_obj_t * ui_voltageLabel;
extern lv_obj_t * ui____initial_actions0;



LV_FONT_DECLARE(ui_font_center60);
LV_FONT_DECLARE(ui_font_unit16);

void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
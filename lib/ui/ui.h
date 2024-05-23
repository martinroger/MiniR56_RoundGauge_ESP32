#ifndef EEZ_LVGL_UI_GUI_H
#define EEZ_LVGL_UI_GUI_H

#include <lvgl.h>
#include "screens.h"

#ifdef __cplusplus
extern "C" {
#endif

void ui_init();
void ui_tick();

void loadScreen(enum ScreensEnum screenId);

void updateCoolantArc(int32_t value);
void updateCoolantLabel(int32_t value);

void updateBoostArc(int32_t value);
void updateBoostLabel(int32_t value);

void updateIatArc(int32_t value);
void updateIatLabel(int32_t value);


void updateVoltageArc(float value);
void updateVoltageLabel(float value);


#ifdef __cplusplus
}
#endif

#endif // EEZ_LVGL_UI_GUI_H
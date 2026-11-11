#ifndef EEZ_LVGL_UI_GUI_H
#define EEZ_LVGL_UI_GUI_H

#include <lvgl.h>
#include "screens.h"


#ifdef __cplusplus
extern "C" {
#endif

void ui_init();
void ui_tick();

void loadScreen(enum ScreensEnum screenId,bool animate);

void resetCoolantMinMax(int32_t value);
void updateCoolantMinMax(double minValue, double maxValue);
void updateCoolantScr(double value);

void resetBoostMinMax(int32_t value);
void updateBoostMinMax(double minValue, double maxValue);
void updateBoostScr(double value);

void resetIatMinMax(int32_t value);
void updateIatMinMax(double minValue, double maxValue);
void updateIatScr(double value);

/*void resetVoltageMinMax(int32_t value);
void updateVoltageMinMax(int32_t minValue, int32_t maxValue);
void updateVoltageScr(int32_t value);*/

void resetMAFMinMax(int32_t value);
void updateMAFMinMax(double minValue, double maxValue);
void updateMAFScr(double value);

void resetHPFPMinMax(int32_t value);
void updateHPFPMinMax(double minValue, double maxValue);
void updateHPFPScr(double value);

void resetOilTMinMax(int32_t value);
void updateOilTMinMax(double minValue, double maxValue);
void updateOilTScr(double value);

void setCanState(bool canState);
void setIgnitionState(bool ignitionOn);
void setKeyPresence(bool keyPresence);


#ifdef __cplusplus
}
#endif

#endif // EEZ_LVGL_UI_GUI_H
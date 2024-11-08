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
void updateCoolantMinMax(int32_t minValue, int32_t maxValue);
void updateCoolantScr(int32_t value);

void resetBoostMinMax(int32_t value);
void updateBoostMinMax(int32_t minValue, int32_t maxValue);
void updateBoostScr(int32_t value);

void resetIatMinMax(int32_t value);
void updateIatMinMax(int32_t minValue, int32_t maxValue);
void updateIatScr(int32_t value);

/*void resetVoltageMinMax(int32_t value);
void updateVoltageMinMax(int32_t minValue, int32_t maxValue);
void updateVoltageScr(int32_t value);*/

void resetMAFMinMax(int32_t value);
void updateMAFMinMax(int32_t minValue, int32_t maxValue);
void updateMAFScr(int32_t value);

void resetHPFPMinMax(int32_t value);
void updateHPFPMinMax(int32_t minValue, int32_t maxValue);
void updateHPFPScr(int32_t value);

void resetOilTMinMax(int32_t value);
void updateOilTMinMax(int32_t minValue, int32_t maxValue);
void updateOilTScr(int32_t value);

void setCanState(bool canState);
void setIgnitionState(bool ignitionOn);
void setKeyPresence(bool keyPresence);


#ifdef __cplusplus
}
#endif

#endif // EEZ_LVGL_UI_GUI_H
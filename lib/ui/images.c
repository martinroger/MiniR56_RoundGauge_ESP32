#include "images.h"

const ext_img_desc_t images[11] = {
    { "turnIndicators", &img_turn_indicators },
    { "airbagSRS", &img_airbag_srs },
    { "brakes", &img_brakes },
    { "lowOil", &img_low_oil },
    { "hiBeams", &img_hi_beams },
    { "lowCoolant", &img_low_coolant },
    { "battery", &img_battery },
    { "abs", &img_abs },
    { "mil", &img_mil },
    { "overTemperature", &img_over_temperature },
    { "lowFuel", &img_low_fuel },
};

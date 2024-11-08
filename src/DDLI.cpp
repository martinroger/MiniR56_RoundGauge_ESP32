#include <Arduino.h>
#include <DDLI.h>

DID* DDLI[9] = {
    &_580C_RPM,
    &_58F0_HPFPPressure,
    &_5ABC_LuftMasse,
    &_58DD_drosselKlappePressure,
    &_580D_speed,
    &_4402_oilTemperature,
    &_581F_engineTemp_2,
    &_5805_engineTemp_1,
    &_580F_IAT
};

// DDLI[0] =   &_580C_RPM;
// DDLI[1] =   &_58F0_HPFPPressure;
// DDLI[2] =   &_5ABC_LuftMasse;
// DDLI[3] =   &_58DD_drosselKlappePressure;
// DDLI[4] =   &_580D_speed;
// DDLI[5] =   &_4402_oilTemperature;
// DDLI[6] =   &_581F_engineTemp_2;
// DDLI[7] =   &_5805_engineTemp_1;
// DDLI[8] =   &_580F_IAT;

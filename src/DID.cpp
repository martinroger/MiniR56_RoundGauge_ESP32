#include <Arduino.h>
#include <DID.h>
 
DID _4402_oilTemperature = {
    .identifier =   {0x44,0x02},
    .memorySize =   1,
    .position   =   1,
    .mul        =   1,
    .div        =   1,
    .add        =   -60,
    .value      =   0
};
DID _5805_engineTemp_1 = {
    .identifier =   {0x58,0x05},
    .memorySize =   1,
    .position   =   1,
    .mul        =   3,
    .div        =   4,
    .add        =   -48,
    .value      =   0
};
DID _580C_RPM = {
    .identifier =   {0x58,0x0C},
    .memorySize =   1,
    .position   =   1,
    .mul        =   40,
    .div        =   1,
    .add        =   0,
    .value      =   0
};
DID _580D_speed = {
    .identifier =   {0x58,0x0D},
    .memorySize =   1,
    .position   =   1,
    .mul        =   5,
    .div        =   4,
    .add        =   0,
    .value      =   0
};
DID _580F_IAT = {
    .identifier =   {0x58,0x0F},
    .memorySize =   1,
    .position   =   1,
    .mul        =   3,
    .div        =   4,
    .add        =   -48,
    .value      =   0
};
DID _58DD_drosselKlappePressure = {
    .identifier =   {0x58,0xDD},
    .memorySize =   2,
    .position   =   1,
    .mul        =   5,
    .div        =   64,
    .add        =   0,
    .value      =   0
};
DID _58F0_HPFPPressure = {
    .identifier =   {0x58,0xF0},
    .memorySize =   2,
    .position   =   1,
    .mul        =   1,
    .div        =   2000,
    .add        =   0,
    .value      =   0
};
DID _5ABC_LuftMasse = {
    .identifier =   {0x5A,0xBC},
    .memorySize =   2,
    .position   =   1,
    .mul        =   1,
    .div        =   10,
    .add        =   0,
    .value      =   0
};
DID _581F_engineTemp_2 = {
    .identifier =   {0x58,0x1F},
    .memorySize =   1,
    .position   =   1,
    .mul        =   3,
    .div        =   4,
    .add        =   -48,
    .value      =   0
};
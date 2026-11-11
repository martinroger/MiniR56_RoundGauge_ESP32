#include <Arduino.h>

struct _DID
{
    byte identifier[2] ;
    byte memorySize    ;
    byte position      ;
    int32_t mul        ;
    int32_t div        ;
    int32_t add        ;
    double value       ;
};
typedef struct _DID DID;
//Known DIDs
extern DID _4402_oilTemperature;
extern DID _5805_engineTemp_1;
extern DID _580C_RPM;
extern DID _580D_speed;
extern DID _580F_IAT;
extern DID _58DD_drosselKlappePressure;
extern DID _58F0_HPFPPressure;
extern DID _5ABC_LuftMasse;
extern DID _581F_engineTemp_2;
//DIDs to be tested

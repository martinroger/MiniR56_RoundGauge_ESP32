#include <Arduino.h>
#include "DID.h"
#define DDLI_POS_580C_RPM       0
#define DDLI_POS_58F0_HPFP      1
#define DDLI_POS_5ABC_MAF       2
#define DDLI_POS_58DD_BOOST     3
#define DDLI_POS_580D_SPEED     4
#define DDLI_POS_4402_OILT      5
#define DDLI_POS_581F_ENGINET_2 6
#define DDLI_POS_5805_ENGINET_1 7
#define DDLI_POS_580F_IAT       8
#define NUM_DIDS 9 //Make sure this is actually the case !

extern DID* DDLI[NUM_DIDS] ;

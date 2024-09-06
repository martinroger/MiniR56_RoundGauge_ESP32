#pragma once
#include "Arduino.h"
#include "kwpFrame.h"

#define CAN_TX 16
#define CAN_RX 15

enum SIDReq : byte
{
    _$23_readMemoryByAddress = 0x23
    //Others
};

//TODO : also add SIDResp enums, frameType enums
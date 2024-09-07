#pragma once
#include "Arduino.h"
#include <ESP32-TWAI-CAN.hpp>

enum frameType : byte
{
    singleFrame         =   0x00,
    firstFrame          =   0x10,
    continuationFrame   =   0x20,
    flowControlFrame    =   0x30
};

class kwpFrame
{
private:
    /* data */
public:
    bool frameComplete  = false;
    byte Buffer[255]    = {0x00};
    byte length         =   0x00;
    byte cursor         =   0x00;
    byte target         =   0x00;
    byte source         =   0x00;
    byte SID            =   0x00;
    //kwpFrame(/* args */);
    //~kwpFrame();
};

/*Methods :
- processCanFrame ? 


*/
// kwpFrame::kwpFrame(/* args */)
// {
// }

// kwpFrame::~kwpFrame()
// {
// }

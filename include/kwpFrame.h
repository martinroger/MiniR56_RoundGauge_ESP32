#pragma once
#include <Arduino.h>
#include "driver/twai.h"

#pragma region DEFINES
#ifndef ECU_ID
    #define ECU_ID 0xF1
#endif
#ifndef TARGET_ID
    #define TARGET_ID 0x12
#endif
#pragma endregion

#pragma region UTILS

#pragma endregion

#pragma region ENUMS


#pragma endregion

#pragma region CLASS DEFINITION


class kwpFrame
{
    public:
        byte target                 =   TARGET_ID;          //Usually the first CAN data byte
        byte sender                 =   ECU_ID;             //Usually the last byte of the CAN frame identifier
        byte SID                    =   0x00;               //Determined only once in RX, set in TX
        uint16_t length             =   2;                  //Length of payload + SID byte
        uint16_t bufferLength       =   1;                  //Length of useful payload after the SID
        byte cursor                 =   0;                  //Indicate currently writable byte in payload Buffer. Always less than bufferLength
        byte buffer[255]            =   {0x00};             //255 bytes RX or TX buffer, indexable with cursor
        bool rxComplete             =   true;               //Indicates if the frame is completely received or not. Useful only in RX
        bool multiFrame             =   false;              //MultiCAN frame (or not) for both TX and RX

        kwpFrame(   byte _target, 
                    byte _sender, 
                    byte _SID, 
                    uint16_t _length, 
                    uint16_t _bufferLength, 
                    bool _rxComplete = true,
                    bool _multiFrame = false);
        kwpFrame(   byte _target, 
                    byte _sender, 
                    byte _SID,
                    uint16_t _dataBufLength,
                    const byte _dataBuf[], 
                    bool _rxComplete = true,
                    bool _multiFrame = false);
        kwpFrame(   byte _target, 
                    byte _sender); 
        kwpFrame() = default;
        
        void setMetadaData(twai_message_t* canMetaFrame);
        void calculateMetaData();
        bool appendCanFrameBuffer(twai_message_t* canFrame, uint8_t startPos = 2, uint8_t endPos = 7);
        void reset(byte _target = TARGET_ID, byte _sender = ECU_ID);
};


#pragma endregion

#pragma region PRESET OBJECTS

#pragma endregion
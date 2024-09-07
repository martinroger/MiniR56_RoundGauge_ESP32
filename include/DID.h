//List of DIDs that have a specific structure
#pragma once
#include "Arduino.h"



struct DID
{
//private:
    /* data */
//public:
    //DID(/* args */);
    //~DID();
    byte identifier[2]  =   {0x00};
    byte memorySize     =   1;
    byte position       =   1;
    int32_t mul         =   1;
    int32_t div         =   1;
    int32_t add         =   0;
    //different value type ?
};



// DID::DID(/* args */)
// {
// }

// DID::~DID()
// {
// }

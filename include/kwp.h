#pragma once
#include "Arduino.h"
#include "kwpFrame.h"
#include "DID.h"

//CAN interface management
#define CAN_TX 16
#define CAN_RX 15

//Utils
template <typename T>
T swap_endian(T u)
{
    static_assert (CHAR_BIT == 8, "CHAR_BIT != 8");

    union
    {
        T u;
        unsigned char u8[sizeof(T)];
    } source, dest;

    source.u = u;

    for (size_t k = 0; k < sizeof(T); k++)
        dest.u8[k] = source.u8[sizeof(T) - k - 1];

    return dest.u;
}

/*Usage : 
tempTrackIndex = swap_endian<uint32_t>(*((uint32_t*)&byteArray[3]));
*/

//ENUMS

enum SIDReq : byte
{
    // _$10_
    // _$11_
    // _$12_
    // _$13_
    // _$14_
    // _$17_
    // _$18_
    // _$1A_
    // _$20_
    _$21_readDataByLocalIdentifier          =   0x21,
    _$22_readDataByCommonIdentifier         =   0x22,
    _$23_readMemoryByAddress                =   0x23,
    // _$26_
    // _$27_
    _$2C_dynamicallyDefineLocalIdentifier   =   0x2C
    // _$2E_
    // _$2F_
    // _$30_
    // _$31_
    // _$32_
    // _$33_
    // _$34_
    // _$35_
    // _$36_
    // _$37_
    // _$38_
    // _$39_
    // _$3A_
    // _$3B_
    // _$3D_
    // _$3E_
};

enum SIDResp : byte
{
    _$61_readDataByLocalIdentifier          =   0x61,
    _$62_readDataByCommonIdentifier         =   0x62,
    _$63_readMemoryByAddress                =   0x63,
    _$6C_dynamicallyDefineLocalIdentifier   =   0x6C,
    _$7F_negativeResponse                   =   0x7F
};


/*TODO
- properties
    - waitingForFCFrame
    - processIncomingFrame
    - sendKWPFrame (overloaded)
    - registerDDLI (array of DIDs)
    - clear DDLI
    - read DDLI
    - read CI


*/
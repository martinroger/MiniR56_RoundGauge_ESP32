#include "kwp.h"

/*
class kwpFrame
{
private:
public:
    bool frameComplete  = false;
    byte Buffer[255]    = {0x00};
    byte length         =   0x00;
    byte cursor         =   0x00;
    byte target         =   0x00;
    byte source         =   0x00;
    byte SID            =   0x00;

};

class kwp
{
private:
    bool multiFramePendingOut   =   false;
    bool multiFramePendingIn    =   false;
    void copyToKWPFrame(CanFrame* sourceFrame, kwpFrame* targetFrame, uint8_t startByte, uint8_t endByte);

public:
    KWPState state = KWP_START;
    DID DDLI[8];
    uint8_t activeDIDCount = 0;

    void reset();
    bool clearDDLI();
    bool setDDLI(DID* arrayDID, uint8_t numberOfActiveDIDs);
    bool parseDDLI(kwpFrame* containerFrame, DID* targetDDLI, uint8_t activeDIDCount);
    bool parseDDLI(kwpFrame* containerFrame);
    void processCANFrame(CanFrame* incomingCANFrame,kwpFrame* containerFrame);
    bool sendKWPFrame(kwpFrame* frameToSend);
    bool readDDLIReq();
};
*/


/// @brief Private function to copy segments of a CAN Frame to the Buffer of a KWP Frame
/// @param sourceFrame CAN Frame to copy the data from.
/// @param targetFrame KWP frame which defines the cursor and Buffer array
/// @param startByte byte position (0-7) from which to copy data, included
/// @param endByte byte position (0-7) at which to stop the data copy, included
void kwp::copyToKWPFrame(CanFrame *sourceFrame, kwpFrame *targetFrame, uint8_t startByte, uint8_t endByte)
{
    //Do some basic checks on the plausibility of startByte and endByte
    //Copy from startByte included in sourceFrame, up to endByte included in sourceFrame, to the cursor position in targetFrame Buffer
    //Advance the internal frame cursor to the next byte to write
}

/// @brief Private function to copy the right-hand segment of a CAN Frame to the Buffer of a KWP Frame
/// @param sourceFrame CAN Frame to copy the data from.
/// @param targetFrame KWP frame which defines the cursor and Buffer array
/// @param startByte byte position (0-7) from which to copy data, included
void kwp::copyToKWPFrame(CanFrame *sourceFrame, kwpFrame *targetFrame, uint8_t startByte)
{
    //Do the same as the other but until the end of the CanFrame
    //Should check for edge case conditions
    if((sourceFrame->data_length_code) >= startByte+1)
    {
        copyToKWPFrame(sourceFrame,targetFrame,startByte,(sourceFrame->data_length_code)-1);
    }
}

/// @brief Resets the KWP engine to its starting state
void kwp::reset()
{
    multiFramePendingIn     =   false;
    multiFramePendingOut    =   false;
    state                   =   KWP_START;
    activeDIDCount          =   0;
}

/// @brief Sends a KWP Frame to clear the DDLI reference
/// @return True when successfully sent, False if there is an issue. Does not check for successful response
bool kwp::clearDDLI()
{
    //Instantiate reset KWP Frame
    //Send KWP Frame
    //If KWP Frame is incomplete, set multiFramePendingOut to true and hand back over to kwp on next refresh
    //Depending on feedback from CAN sender, return true or false
    // KWP state management will be done in the RxHandler
    return false;
}

/// @brief Sends a KWP Frame to set up the DDLI content
/// @param arrayDID pointer to an array of DIDs that define the content (and sequence) of the DDLI
/// @param numberOfActiveDIDs number of DIDs in the DDLI array that need to be considered. Has to be >0
/// @return True when successfully sent, False if there is an issue. Does not check for successful response
bool kwp::setDDLI(DID* arrayDID, uint8_t numberOfActiveDIDs) 
{
    return false;
}

/// @brief 
/// @param containerFrame 
/// @param targetDDLI 
/// @param activeDIDCount 
/// @return 
bool kwp::parseDDLI(kwpFrame* containerFrame, DID* targetDDLI, uint8_t activeDIDCount)
{
    return false;
}

/// @brief 
/// @param containerFrame 
/// @return 
bool kwp::parseDDLI(kwpFrame *containerFrame)
{
    return false;
}

/// @brief 
/// @param incomingCANFrame 
/// @param containerFrame 
void kwp::processCANFrame(CanFrame* incomingCANFrame,kwpFrame* containerFrame)
{

}

/// @brief 
/// @param frameToSend 
/// @return 
bool kwp::sendKWPFrame(kwpFrame* frameToSend)
{
    return false;
}

/// @brief 
/// @return 
bool kwp::readDDLIReq()
{
    return false;
}

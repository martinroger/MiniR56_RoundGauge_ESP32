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

/// @brief Transfers a kwpFrame to the scaled values in a DDLI array of DIDs
/// @param containerFrame Complete kwpFrame that corresponds to the targetDDLI sectioning
/// @param targetDDLI Array of DIDs that defines the sectioning and scaling of the data package in the containerFrame
/// @param activeDIDCount Number of entries (up to 8) to consider in targetDDLI
/// @return True if parsing is successful, false otherwise
bool kwp::parseDDLI(kwpFrame* containerFrame, DID* targetDDLI, uint8_t activeDIDCount)
{
    //Check activeDIDCount > 0
    //Check kwpFrame is complete
    //Go from targetDDLI[0] to targetDDLI[activeDIDCount-1] and update targetDDLI[i].value with the mul/div + add operation
    return false;
}

/// @brief Transfers a kwpFrame to the intrinsice DDLI array of the current kwp class instance
/// @param containerFrame Complete kwpFrame that contains the DDLI
/// @return True if parsing is successful, false otherwise
bool kwp::parseDDLI(kwpFrame *containerFrame)
{
    parseDDLI(containerFrame,DDLI,activeDIDCount);
    return false;
}

/// @brief Handles incoming CAN frames and performs state operation depending on the content
/// @param incomingCANFrame Incoming KWP-type CAN frame (validate before calling this method)
/// @param containerFrame Target kwpFrame. Can be incomplete
void kwp::processCANFrame(CanFrame* incomingCANFrame,kwpFrame* containerFrame)
{
    //Check if multiFramePendingIn, and if yes then check the kwpFrame is incomplete, and then check the frame type
    //  if both are correct, then process the canFrame, copying at the cursor with taking into account the length and cursor position
    //Check frame type
    //Should it fire back FC Frames if needed ?
}

/// @brief Attempts to send a KWP Frame over CAN
/// @param frameToSend 
/// @return True if successfully sent, false is there was a communication error or timeout
bool kwp::sendKWPFrame(kwpFrame* frameToSend)
{
    //This should manage the sending cursor, as well as the choice to raise the multiFramePendingOut flag and chosing between SF,FF,CF
    //FC Frame reception should be managed somewhere else?
    return false;
}

/// @brief Send a simple request to read the 0xF0 DDLI. 
/// @return True on successful request sent, false if transmit error
bool kwp::readDDLIReq()
{
    return false;
}

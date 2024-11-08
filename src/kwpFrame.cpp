#include "kwpFrame.h"

/// @brief Base constructor with direct assignment
/// @param _target ID of the target ECU for this KWP Frame
/// @param _sender ID of the sender ECU for this KWP Frame
/// @param _SID SID of the KWP Frame
/// @param _length Total payload length, including SID
/// @param _bufferLength Total payload length, excluding SID
/// @param _rxComplete Flag of completed reception (true by default)
/// @param _multiFrame MultiFrame flag. False by default.
kwpFrame::kwpFrame( byte _target, 
                    byte _sender, 
                    byte _SID, 
                    uint16_t _length, 
                    uint16_t _bufferLength, 
                    bool _rxComplete, 
                    bool _multiFrame) 
                    :
                    target(_target),
                    sender(_sender),
                    SID(_SID),
                    length(_length),
                    bufferLength(_bufferLength),
                    rxComplete(_rxComplete),
                    multiFrame(_multiFrame)
{}

/// @brief Base constructor with direct assignment
/// @param _target ID of the target ECU for this KWP Frame
/// @param _sender ID of the sender ECU for this KWP Frame
/// @param _SID SID of the KWP Frame
/// @param _dataBufLength Length of the base buffer to copy
/// @param _dataBuf Base buffer to copy to the payload buffer, excluding SID
/// @param _rxComplete Flag of completed reception (true by default)
/// @param _multiFrame MultiFrame flag. False by default.
kwpFrame::kwpFrame( byte _target, 
                    byte _sender, 
                    byte _SID, 
                    uint16_t _dataBufLength, 
                    const byte _dataBuf[], 
                    bool _rxComplete,
                    bool _multiFrame)
                    :
                    target(_target),
                    sender(_sender),
                    SID(_SID),
                    length(_dataBufLength+1),
                    bufferLength(_dataBufLength),
                    rxComplete(_rxComplete),
                    multiFrame(_multiFrame)
{
    for (int i = 0; i < _dataBufLength; i++)
    {
        buffer[i] = _dataBuf[i];
    }
}

/// @brief Minimal constructor with target and sender ID
/// @param _target ID of the target ECU for this KWP Frame
/// @param _sender ID of the sender ECU for this KWP Frame
kwpFrame::kwpFrame( byte _target, 
                    byte _sender)
                    :
                    target(_target),
                    sender(_sender)
{
}

/// @brief Parses Metadata on first reception frame when in an RX position. Unused in TX
/// @param canMetaFrame FirstFrame or SingleFrame (no Flow Control) to get the metadata from
void kwpFrame::setMetadaData(twai_message_t *canMetaFrame)
{
    target = canMetaFrame->data[0];
    sender = canMetaFrame->identifier & 0xFF;
    switch ((canMetaFrame->data[1]) & 0xF0)
    {
    case 0x00: //SingleFrame
        multiFrame = false;
        break;
    case 0x10: //FirstFrame
        multiFrame = true;
        break;
    default:
        multiFrame = false;
        break;
    }
}

/// @brief After constructing a KWP Frame, calculates all metadata variants
void kwpFrame::calculateMetaData()
{
    if(length>6) multiFrame = true;
    else multiFrame = false;
}

/// @brief Appends a segment of a CAN Frame buffer to an existing kwpFrame payload buffer
/// @param canFrame Donor CAN Frame, referenced by pointer
/// @param startPos Start position, by default 2
/// @param endPos End position, by default 7
/// @return True if the append is possible, false if sanity checks are failed
bool kwpFrame::appendCanFrameBuffer(twai_message_t *canFrame, uint8_t startPos, uint8_t endPos)
{
    //Some sanity check
    if( (startPos>endPos)   || 
        (endPos>7)          || 
        (canFrame==nullptr) || 
        ((cursor + (endPos-startPos+1)) > bufferLength)) return false;
    else
    {
        for (int i = startPos; i < endPos+1; i++)
        {
            buffer[cursor] = canFrame->data[i];
            cursor++;
        }
        return true;
    }
}

/// @brief Resets frame to a fresh state
void kwpFrame::reset(byte _target, byte _sender)
{
    target          =   _target;
    sender          =   _sender;
    SID             =   0x00;
    length          =   2;
    bufferLength   =   1;
    cursor          =   0;
    rxComplete      =   true;
    for (int i = 0; i < 255; i++)
    {
        buffer[i] = 0x00;
    }
}

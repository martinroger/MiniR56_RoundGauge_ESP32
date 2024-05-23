#include <ESP32-TWAI-CAN.hpp>

#define CAN_TX 16
#define CAN_RX 15

CanFrame rxFrame;

#define CAN_TIME_OUT 2000
unsigned long lastConnected = 0;
bool canState = false;

unsigned long FrameID = 0x7E8;
unsigned char OBD_length;
unsigned char OBD_mode;
unsigned char OBD_command;
unsigned char byteA;
unsigned char byteB;
unsigned char byteC;
unsigned char byteD;
unsigned char byteE;

byte requestID = 0x05;

void sendObdFrame(uint8_t obdId) {
    CanFrame obdFrame = { 0 };
    obdFrame.identifier = 0x7DF; // Default OBD2 address;
    obdFrame.extd = 0;
    obdFrame.data_length_code = 8;
    obdFrame.data[0] = 0x02;
    obdFrame.data[1] = 0x01;    // Mode 1
    obdFrame.data[2] = obdId;
    obdFrame.data[3] = 0xAA;    // Best to use 0xAA (0b10101010) instead of 0
    obdFrame.data[4] = 0xAA;    // CAN works better this way as it needs
    obdFrame.data[5] = 0xAA;    // to avoid bit-stuffing
    obdFrame.data[6] = 0xAA;
    obdFrame.data[7] = 0xAA;
      // Accepts both pointers and references 
    ESP32Can.writeFrame(obdFrame,0);  // timeout defaults to 1 ms
}

void canStart() {
    ESP32Can.setPins(CAN_TX,CAN_RX);
    ESP32Can.setSpeed(ESP32Can.convertSpeed(500));
    ESP32Can.begin(); 
}
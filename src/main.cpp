#include <Arduino.h>
//#include <Arduino_Helpers.h>
//#include <AH/Timing/MillisMicrosTimer.hpp>
//#include "lv_conf.h"

#include "lvgl_port.h"
#include <ui.h>
#include "obdHandler.h"

#ifndef TFT_BL
  #define TFT_BL 5
#endif

#ifndef SCREEN_ID_MAIN
  #define SCREEN_ID_MAIN 1
#endif

//Display buffer preparation

#define DRAW_BUF_SIZE (TFT_HOR_RES * TFT_VER_RES / 4 * (LV_COLOR_DEPTH / 8))
uint32_t draw_buf[DRAW_BUF_SIZE];

// Touch initialisation
// #define TP_INT 4
// #define TP_SDA 1
// #define TP_SCL 3
// #define TP_RST -1
// CST816S touch(TP_SDA, TP_SCL, TP_RST, TP_INT);
// void touchRead(lv_indev_t *indev, lv_indev_data_t *data)
// {
  
//   if(touch.available()) {
//     data->state = LV_INDEV_STATE_PRESSED;
//     data->point.x = touch.data.x;
//     data->point.y = touch.data.y;
//   }
//   else {
//     data->state = LV_INDEV_STATE_RELEASED;
//   }
// }

//Timers
#ifndef TICKS
  #define TICKS 5
#endif
//Timer<millis> tickerLVGL      =   TICKS;    //LVGL 5ms ticker
//Timer<millis> refreshValues   =   100;  //Values refresh interval on the screens 
//Timer<millis> OBDrequestDelay =   100;   //Interval for requests over OBD
#ifndef  DISP_VALUES_REFRESH_INTERVAL
  #define DISP_VALUES_REFRESH_INTERVAL 100
#endif
#ifndef OBD_QUERY_REFRESH_INTERVAL
  #define OBD_QUERY_REFRESH_INTERVAL 100
#endif
unsigned long lastLVGLTicked = 0;
unsigned long lastDispValuesRefreshed = 0;
unsigned long lastOBDRequestSent = 0;


//Ignition and key presence alerts
bool ignitionOn   = false;  //True when ignition is on
bool keyPresence  = false;  //True when authorized key is inserted

//Vehicle variables
int32_t intakeTemp;
int32_t absBaroPressure;
int32_t intakeManifoldPressure;
int32_t boostPressure;
int32_t engineCoolantTemp;
int32_t controlModuleVoltage;
#define BRIGHTNESS 215

//First read flags
bool intakeTemp_FR = false;
bool absBaroPressure_FR = false;
bool intakeManifoldPressure_FR = false;
bool boostPressure_FR = false;
bool engineCoolantTemp_FR = false;
bool controlModuleVoltage_FR = false;
bool screenON = false;

#ifdef TEST_GENERATOR
void generateValues() {
  if(millis()>1000) {
    intakeTemp_FR             =   true;
    absBaroPressure_FR        =   true;
    intakeManifoldPressure_FR =   true;
    boostPressure_FR          =   true;
    engineCoolantTemp_FR      =   true;
    controlModuleVoltage_FR   =   true;
  }

  intakeTemp    =   (int32_t)(127*(1+0.6*sin((2*PI/10000)*millis()))-40);
  boostPressure =   (int32_t)(127*(1+0.6*sin((2*PI/10000)*millis())));
  engineCoolantTemp   = (int32_t)(127*(1+0.6*sin((2*PI/10000)*millis()))-40);
  controlModuleVoltage = (int32_t)((12.0+3.0*sin((2*PI/10000)*millis()))*1000.0);
  ignitionOn = ((sin((2*PI/2000)*millis()))<0);
  keyPresence = ((sin((2*PI/3000)*millis()))<0);
}
#else
void parseCANFrame() {
  if(ESP32Can.readFrame(rxFrame,0)) {
    switch (rxFrame.identifier) {
      case OBD_RESP_ID:
        OBD_length  = rxFrame.data[0];
        OBD_mode    = rxFrame.data[1];
        OBD_command = rxFrame.data[2];
        byteA       = rxFrame.data[3];
        byteB       = rxFrame.data[4];
        byteC       = rxFrame.data[5];
        byteD       = rxFrame.data[6];
        byteE       = rxFrame.data[7];

        if((OBD_mode-0x40)==0x01) { //Check we are in the correct mode
          switch (OBD_command)  {
            case 0x05: //Engine Coolant Temperature : engineCoolantTemp
              engineCoolantTemp = ((int)byteA - 40);
              engineCoolantTemp_FR = true;
              break;
            case 0x0B:
              intakeManifoldPressure = (int)byteA;
              intakeManifoldPressure_FR = true;
              boostPressure_FR = absBaroPressure_FR && intakeManifoldPressure_FR;
              break;
            case 0x0F :
              intakeTemp = ((int)byteA - 40);
              intakeTemp_FR = true;
              break;
            case 0x33 : 
              absBaroPressure = (int)byteA;
              absBaroPressure_FR = true;
              boostPressure_FR = absBaroPressure_FR && intakeManifoldPressure_FR;
              break;
            case 0x42 : //Control module voltage... unit is mV !
              controlModuleVoltage = (256*(int)byteA + (int)byteB);
              controlModuleVoltage_FR = true;
              break;
            default:
              break;
          }
          boostPressure = intakeManifoldPressure - absBaroPressure;
        }
        lastConnected = millis();
        break;
      case 0x130: //Ignition and key detection
        keyPresence = ((rxFrame.data[0] & 0xF0) == 0x40); //True if key present
        ignitionOn  = ((rxFrame.data[0] & 0x0F) == 0x05); //True if ignition on
        lastConnected = millis();
        break;
      default:
        break;
    }
  }
}
#endif

#if LV_USE_LOG != 0
void my_print( lv_log_level_t level, const char * buf )
{
    LV_UNUSED(level);
    Serial.println(buf);
    Serial.flush();
}
#endif

void setup() {
  
  //Initialise min-max indicators
  engineCoolantTemp_max = -40;
  engineCoolantTemp_min = 215;

  boostPressure_max = -255;
  boostPressure_min = 255;

  intakeTemp_max = -40;
  intakeTemp_min = 215;

  controlModuleVoltage_max = 0;
  controlModuleVoltage_min = 24000;

  //For Debug
  Serial.begin(115200);

  //CAN start
  canStart();

  #ifdef TEST_GENERATOR
  generateValues();
  #endif

  //Touch startup
//   touch.begin();

  //ESP Panel
//   ESP_Panel *panel = new ESP_Panel();
//   panel->init();

//   panel->begin();

  //LV startup sequence
//   lv_init();
	displayInit();
	#if LV_USE_LOG != 0
		lv_log_register_print_cb( my_print );
	#endif
//   lv_display_t * disp;
//   disp = lv_display_create(TFT_HOR_RES,TFT_VER_RES);
//   //disp = lv_tft_espi_create(TFT_HOR_RES, TFT_VER_RES, draw_buf, sizeof(draw_buf));
//   lv_indev_t *indev = lv_indev_create();
//   lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
//   lv_indev_set_read_cb(indev,touchRead);
//   lv_display_set_rotation(disp,LV_DISPLAY_ROTATION_270);
  //Draw screens
  ui_init();

  //Debug
  Serial.println( "Setup done" );
}

void loop() {

  //Sends the OBD query only if the CAN is sending something as a keepalive first and if the key is present.
  if((millis()-lastOBDRequestSent > OBD_QUERY_REFRESH_INTERVAL) && canState && keyPresence) {
    sendObdFrame(requestID);
    //Move to the next requestID... would be better to use an enum list
    switch (requestID)  {
      case 0x05:
        requestID = 0x0B;
        break;
      case 0x0B:
        requestID = 0x0F;
        break;
      case 0x0F:
        requestID = 0x33;
        break;
      case 0x33:
        requestID = 0x42;
        break;
      case 0x42:
        requestID = 0x05;
        break;
      default:
        requestID = 0x05;
        break;
    }
	lastOBDRequestSent = millis();
  }
  
  //Parse a CAN Frame if available
  #ifdef TEST_GENERATOR
  generateValues();
  #else
  parseCANFrame();
  #endif

  //Update the canState in case of silent connection
  if(millis()-lastConnected>2000) {
    canState = false;
    }
  else {
    canState = true;
  }

  //Refresh the items in the UI
  if((millis()-lastDispValuesRefreshed)>DISP_VALUES_REFRESH_INTERVAL) {
    lastDispValuesRefreshed = millis();
	  setCanState(canState);
    setIgnitionState(ignitionOn);
    setKeyPresence(keyPresence);
    
    //Coolant Screen
    if(engineCoolantTemp_FR) {
    updateCoolantScr(engineCoolantTemp);
      if((engineCoolantTemp_min>engineCoolantTemp) || (engineCoolantTemp_max<engineCoolantTemp))  {
        engineCoolantTemp_max = max(engineCoolantTemp_max,engineCoolantTemp);
        engineCoolantTemp_min = min(engineCoolantTemp_min,engineCoolantTemp);
        updateCoolantMinMax(engineCoolantTemp_min,engineCoolantTemp_max);
      }
    }
    
    //Boost Screen
    if(boostPressure_FR) {
      updateBoostScr(boostPressure);
      if((boostPressure_min>boostPressure) || (boostPressure_max<boostPressure))  {
        boostPressure_max = max(boostPressure_max,boostPressure);
        boostPressure_min = min(boostPressure_min,boostPressure);
        updateBoostMinMax(boostPressure_min,boostPressure_max);
      }
    }

    //IAT Screen
    if(intakeTemp_FR) {
      updateIatScr(intakeTemp);
      if((intakeTemp_min>intakeTemp) || (intakeTemp_max<intakeTemp))  {
        intakeTemp_max = max(intakeTemp_max,intakeTemp);
        intakeTemp_min = min(intakeTemp_min,intakeTemp);
        updateIatMinMax(intakeTemp_min,intakeTemp_max);
      }
    }

    //Voltage Screen
    if(controlModuleVoltage_FR) {
      updateVoltageScr(controlModuleVoltage);
      if((controlModuleVoltage_min>controlModuleVoltage) || (controlModuleVoltage_max<controlModuleVoltage))  {
        controlModuleVoltage_max = max(controlModuleVoltage_max,controlModuleVoltage);
        controlModuleVoltage_min = min(controlModuleVoltage_min,controlModuleVoltage);
        updateVoltageMinMax(controlModuleVoltage_min,controlModuleVoltage_max);
      }
    }
  }
  
  //Loop LVGL
  // if((millis()-lastLVGLTicked)>TICKS) {
  //   lastLVGLTicked = millis();
	// lv_task_handler();
  //   //lv_tick_inc(TICKS);
  // }
  //could use lv_timer_handler();
  lv_timer_handler();

  //Initial screenON
  if(!screenON) {
    uint8_t tempbrightness = 0;
    while (tempbrightness<BRIGHTNESS) {
      backLight->setBrightness(tempbrightness*100/255);
      //analogWrite(TFT_BL,tempbrightness);
      delay(3);
      tempbrightness++;
    }
    //analogWrite(TFT_BL,BRIGHTNESS);
    screenON = true;
  }
}
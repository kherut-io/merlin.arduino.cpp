#ifndef Merlin_h
#define Merlin_h

#include <ESP8266HTTPClient.h>
#include <WiFiManager.h>
 
#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
  #include "pins_arduino.h"
  #include "WConstants.h"
#endif

namespace Merlin {
  class Client {
    char _deviceName[64];
    char _merlinBrainName[64];
    char _localIP[32];
    unsigned short int _merlinBrainPort;
    bool _resetSettings;
    WiFiManager* _wm;
    HTTPClient* _hc;

    public:
      Client();

      void setUp(char*, unsigned short int = 8080, char* = "", bool = false);
      void runWFM();
      void runHC();
      char* requestND();
      char* getDeviceName();
      char* getIP();
  };
}
 
#endif

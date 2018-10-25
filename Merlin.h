#ifndef Merlin_h
#define Merlin_h

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
    WiFiManager* _wm;

    public:
      Client();

      void setUp(char*, unsigned short int = 8080, char* = "");
      void runWFM();
      char* getDeviceName();
      char* getIP();
  };
}
 
#endif

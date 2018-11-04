/*
  merlin.ino - Server example

  created 2018
  by Adam K. Pisula (Kherut.io)

  Licensed under GNU General Public License v3.0

  https://github.com/kherut-io/merlin.ino
*/

#ifndef Merlin_h
#define Merlin_h

#include <ESP8266HTTPClient.h>
#include <WiFiManager.h>
#include <WiFiClient.h>
 
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
    /*WiFiServer _tcpServer;
    WiFiClient _tcpClient;*/

    public:
      Client();

      void setUp(char*, unsigned short int = 8080, bool = false, char* = "");
      void runWFM();
      void runHC();
      char* requestND();
      char* getDeviceName();
      char* getIP();
  };
  
  class Server {
    char _deviceName[64];
    char _merlinBrainName[64];
    char _localIP[32];
    unsigned short int _merlinBrainPort;
    bool _resetSettings;
    WiFiManager* _wm;
    HTTPClient* _hc;
    WiFiServer* _tcpServer;
    WiFiClient _tcpClient;
    int _tcpPort;
    int _httpCode;

    public:
      Server();

      void setUp(char*, unsigned short int = 8080, bool = false, char* = "");
      void runWFM();
      void runHC();
      void requestND();
      void begin();
      void connect();
      bool connected();
      int available();
      char read();
      char* getDeviceName();
      char* getIP();
  };
}
 
#endif

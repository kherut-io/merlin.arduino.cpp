#ifndef Merlin_h
#define Merlin_h
 
#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
  #include "pins_arduino.h"
  #include "WConstants.h"
#endif

class Client {
  char _deviceName[256];

  public:
    Client(const char*, unsigned short int = 8080, char* = "Merlin_");
    char* getDeviceName();
};

class Server {
  public:
    Server(const char*, unsigned short int = 8080);
};
 
#endif

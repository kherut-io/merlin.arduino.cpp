#include "Merlin.h"
#include <ESP8266WiFi.h>
#include <WiFiManager.h>

//CLIENT
Client::Client(const char* merlinBrainName, unsigned short int merlinBrainPort, const char* deviceName) {
    if(strcmp(_deviceName, "Merlin_") != 0) {
        char chip_id[32];

        sprintf(chip_id, "%d", system_get_chip_id());
        strcat(_deviceName, chip_id);
    }
}

char* Client::getDeviceName() {
    return _deviceName;
}

//SERVER
Server::Server(const char* merlinBrainName, unsigned short int merlinBrainPort) {
    
}
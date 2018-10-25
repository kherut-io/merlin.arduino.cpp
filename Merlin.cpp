#include "Merlin.h"
#include <ESP8266WiFi.h>
#include <WiFiManager.h>

//CLIENT
Merlin::Client::Client() {
    char deviceName[64] = "Merlin_";
    char chip_id[32];

    sprintf(chip_id, "%d", system_get_chip_id());
    strcat(deviceName, chip_id);

    strcpy(deviceName, _deviceName);

    Serial.println("Created");
}

void Merlin::Client::setUp(char* merlinBrainName, unsigned short int merlinBrainPort, char* deviceName) {
    if(strcmp(deviceName, "") != 0)
        strcpy(deviceName, _deviceName);
    
    strcpy(merlinBrainName, _merlinBrainName);
    _merlinBrainPort = merlinBrainPort;
}

void Merlin::Client::runWFM() {
    _wm = new WiFiManager();

    _wm->autoConnect(_deviceName);

    char conv[3];
    IPAddress ip = WiFi.localIP();

    for(int i = 0; i < 4; i++) {
        if(i > 0)
            strcat(_localIP, ".");

        sprintf(conv, "%s", ip[i]);

        strcat(_localIP, conv);
    }
}

char* Merlin::Client::getDeviceName() {
    return _deviceName;
}

char* Merlin::Client::getIP() {
    return _localIP;
}
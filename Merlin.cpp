#include "Merlin.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiManager.h>
#include <string.h>

//CLIENT
Merlin::Client::Client() {
    char deviceName[64] = "Merlin_";
    char chip_id[32];

    sprintf(chip_id, "%d", system_get_chip_id());
    strcat(deviceName, chip_id);

    strcpy(_deviceName, deviceName);

    Serial.println("Created");
}

void Merlin::Client::setUp(char* merlinBrainName, unsigned short int merlinBrainPort, char* deviceName, bool resetSettings) {
    if(strcmp(deviceName, "") != 0)
        strcpy(_deviceName, deviceName);
        
    strcpy(_merlinBrainName, merlinBrainName);
    _merlinBrainPort = merlinBrainPort;
    _resetSettings = resetSettings;
}

void Merlin::Client::runWFM() {
    _wm = new WiFiManager();

    if(_resetSettings)
        _wm->resetSettings();

    _wm->autoConnect(_deviceName);

    char conv[3];
    char ipCopy[32];
    bzero(ipCopy, 32);

    IPAddress ip = WiFi.localIP();

    for(int i = 0; i < 4; i++) {
        bzero(conv, 3);

        if(i > 0)
            strcat(ipCopy, ".");

        sprintf(conv, "%d", ip[i]);

        strcat(ipCopy, conv);
    }

    strcpy(_localIP, ipCopy);
}

void Merlin::Client::runHC() {
    _hc = new HTTPClient();

    char url[256];
    bzero(url, 256);

    char mbPortStr[8];
    bzero(mbPortStr, 8);

    sprintf(mbPortStr, "%hu", _merlinBrainPort);

    strcat(url, "http://");
    strcat(url, _merlinBrainName);
    strcat(url, ":");
    strcat(url, mbPortStr);
    strcat(url, "/control/devices/new/");
    strcat(url, _localIP);
    strcat(url, "/");
    strcat(url, _deviceName);
    strcat(url, "/CLIENT");

    _hc->begin(url);
}

char* Merlin::Client::requestND() {
    int code = _hc->GET();
    char content[256];
    bzero(content, 256);

    if(code > 0)
        strcpy(content, _hc->getString().c_str());

    _hc->end();

    return content;
}

char* Merlin::Client::getDeviceName() {
    return _deviceName;
}

char* Merlin::Client::getIP() {
    return _localIP;
}
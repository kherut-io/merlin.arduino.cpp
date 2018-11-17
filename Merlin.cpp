/*
  merlin.ino - Server example

  created 2018
  by Adam K. Pisula (Kherut.io)

  Licensed under GNU General Public License v3.0

  https://github.com/kherut-io/merlin.ino
*/

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

void Merlin::Client::setUp(char* merlinBrainName, unsigned short int merlinBrainPort, bool resetSettings, char* deviceName) {
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
    strcat(url, "/control/device/new/");
    strcat(url, _localIP);
    strcat(url, "/");
    strcat(url, _deviceName);
    strcat(url, "/CLIENT");

    /////
    Serial.println(url);

    _hc->begin(url);
}

char* Merlin::Client::requestND() {
    int code = _hc->GET();

    /////
    Serial.print("HTTP code: ");
    Serial.println(code);
    Serial.print("HTTP code (string): ");
    Serial.println(_hc->errorToString(code).c_str());

    char content[256];
    bzero(content, 256);

    if(code > 0)
        strcpy(content, _hc->getString().c_str());

    _hc->end();

    Serial.print("(LIB) Response: ");
    Serial.println(content);

    return content;
}

char* Merlin::Client::getDeviceName() {
    return _deviceName;
}

char* Merlin::Client::getIP() {
    return _localIP;
}

//SERVER
Merlin::Server::Server() {
    char deviceName[64] = "Merlin_";
    char chip_id[32];

    sprintf(chip_id, "%d", system_get_chip_id());
    strcat(deviceName, chip_id);

    strcpy(_deviceName, deviceName);

    Serial.println("Created");
}

void Merlin::Server::setUp(char* merlinBrainName, unsigned short int merlinBrainPort, bool resetSettings, char* deviceName) {
    if(strcmp(deviceName, "") != 0)
        strcpy(_deviceName, deviceName);
        
    strcpy(_merlinBrainName, merlinBrainName);
    _merlinBrainPort = merlinBrainPort;
    _resetSettings = resetSettings;
}

void Merlin::Server::runWFM() {
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

void Merlin::Server::runHC() {
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
    strcat(url, "/control/device/new/");
    strcat(url, _localIP);
    strcat(url, "/");
    strcat(url, _deviceName);
    strcat(url, "/SERVER");

    _hc->begin(url);
}

void Merlin::Server::requestND() {
    _httpCode = _hc->GET();

    char content[256];
    bzero(content, 256);

    if(_httpCode > 0)
        strcpy(content, _hc->getString().c_str());

    _hc->end();

    _tcpPort = atoi(content);
}

void Merlin::Server::begin() {
    _tcpServer = new WiFiServer(_tcpPort);
    _tcpServer->begin();
}

void Merlin::Server::connect() {
    _tcpClient = _tcpServer->available();
}

bool Merlin::Server::connected() {
    return _tcpClient.connected();
}

int Merlin::Server::available() {
    return _tcpClient.available();
}

char Merlin::Server::read() {
    return _tcpClient.read();
}

char* Merlin::Server::getDeviceName() {
    return _deviceName;
}

char* Merlin::Server::getIP() {
    return _localIP;
}
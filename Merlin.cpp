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

void Merlin::Client::_append(char* s, char c) {
    int len = strlen(s);

    s[len] = c;
    s[len + 1] = '\0';
}

char* Merlin::Client::_split(char* str, char delimiter, int index) {
    char* ret = (char*) malloc(strlen(str) * sizeof(char));
    bzero(ret, strlen(str));

    int delimitersPassed = 0;

    for(int i = 0; i < strlen(str); i++) {
        if(str[i] == delimiter) {
            if(index == delimitersPassed)
                return ret;

            delimitersPassed++;

            memset(ret, 0, sizeof ret);
        }

        if(delimitersPassed == index && str[i] != delimiter)
            _append(ret, str[i]);
    }

    return ret;
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
}

void Merlin::Client::requestND() {
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

    _httpCode = _hc->GET();

    char content[256];
    bzero(content, 256);

    if(_httpCode > 0)
        strcpy(content, _hc->getString().c_str());

    _hc->end();

    char portStr[6];
    bzero(portStr, 6);

    strcpy(_id, _split(content, '!', 0));
    strcpy(portStr, _split(content, '!', 1));

    _tcpPort = atoi(portStr);
}

void Merlin::Client::begin() {
    _tcpServer = new WiFiServer(_tcpPort);
    _tcpServer->begin();
}

void Merlin::Client::connect() {
    _tcpClient.connect(_merlinBrainName, _tcpPort);
}

void Merlin::Client::status(char* value) {
    char url[256];
    bzero(url, 256);

    char mbPortStr[8];
    bzero(mbPortStr, 8);

    sprintf(mbPortStr, "%hu", _merlinBrainPort);

    strcat(url, "http://");
    strcat(url, _merlinBrainName);
    strcat(url, ":");
    strcat(url, mbPortStr);
    strcat(url, "/control/devices/status/");
    strcat(url, _id);
    strcat(url, "/");
    strcat(url, value);

    _hc->begin(url);

    _httpCode = _hc->GET();

    char content[256];
    bzero(content, 256);

    if(_httpCode != 200) {
        if(strcmp(content, "OK") != 0) {
            Serial.print("HTTP code: ");
            Serial.println(_httpCode);
            Serial.print("Response: ");
            Serial.println(content);
        }
    }

    _hc->end();
}

int Merlin::Client::readBytes(char* buffer, int length) {
    int i;

    for(i = 0; available() > 0 && i < length; i++)
        buffer[i] = read();

    return i;
}

bool Merlin::Client::connected() {
    return _tcpClient.connected();
}

int Merlin::Client::available() {
    return _tcpClient.available();
}

int Merlin::Client::write(char val) {
    return _tcpClient.write(val);
}

int Merlin::Client::write(String str) {
    int len = str.length();

    char buf[len + 1];
    bzero(buf, len + 1);

    for(int i = 0; i < len; i++)
        buf[i] = str[i];

    return write(buf, len);
}

int Merlin::Client::write(char* buf, int len) {
    return _tcpClient.write(buf, len);
}

char Merlin::Client::read() {
    return _tcpClient.read();
}

char* Merlin::Client::getDeviceName() {
    return _deviceName;
}

char* Merlin::Client::getIP() {
    return _localIP;
}

char* Merlin::Client::getID() {
    return _id;
}

String Merlin::Client::readString() {
    String ret;

    while(available() > 0)
        ret.concat(read());

    return ret;
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

void Merlin::Server::_append(char* s, char c) {
    int len = strlen(s);

    s[len] = c;
    s[len + 1] = '\0';
}

char* Merlin::Server::_split(char* str, char delimiter, int index) {
    char* ret = (char*) malloc(strlen(str) * sizeof(char));
    bzero(ret, strlen(str));

    int delimitersPassed = 0;

    for(int i = 0; i < strlen(str); i++) {
        if(str[i] == delimiter) {
            if(index == delimitersPassed)
                return ret;

            delimitersPassed++;

            memset(ret, 0, sizeof ret);
        }

        if(delimitersPassed == index && str[i] != delimiter)
            _append(ret, str[i]);
    }

    return ret;
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
}

void Merlin::Server::requestND() {
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
    strcat(url, "/SERVER");

    _hc->begin(url);

    _httpCode = _hc->GET();

    char content[256];
    bzero(content, 256);

    if(_httpCode > 0)
        strcpy(content, _hc->getString().c_str());

    _hc->end();

    char portStr[6];
    bzero(portStr, 6);

    strcpy(_id, _split(content, '!', 0));
    strcpy(portStr, _split(content, '!', 1));

    _tcpPort = atoi(portStr);
}

void Merlin::Server::begin() {
    _tcpServer = new WiFiServer(_tcpPort);
    _tcpServer->begin();
}

void Merlin::Server::connect() {
    _tcpClient = _tcpServer->available();
}

void Merlin::Server::status(char* value) {
    char url[256];
    bzero(url, 256);

    char mbPortStr[8];
    bzero(mbPortStr, 8);

    sprintf(mbPortStr, "%hu", _merlinBrainPort);

    strcat(url, "http://");
    strcat(url, _merlinBrainName);
    strcat(url, ":");
    strcat(url, mbPortStr);
    strcat(url, "/control/devices/status/");
    strcat(url, _id);
    strcat(url, "/");
    strcat(url, value);

    _hc->begin(url);

    _httpCode = _hc->GET();

    char content[256];
    bzero(content, 256);

    if(_httpCode != 200) {
        if(strcmp(content, "OK") != 0) {
            Serial.print("HTTP code: ");
            Serial.println(_httpCode);
            Serial.print("Response: ");
            Serial.println(content);
        }
    }

    _hc->end();
}

int Merlin::Server::readBytes(char* buffer, int length) {
    int i;

    for(i = 0; available() > 0 && i < length; i++)
        buffer[i] = read();

    return i;
}

bool Merlin::Server::connected() {
    return _tcpClient.connected();
}

int Merlin::Server::available() {
    return _tcpClient.available();
}

int Merlin::Server::write(char val) {
    return _tcpClient.write(val);
}

int Merlin::Server::write(String str) {
    int len = str.length();

    char buf[len + 1];
    bzero(buf, len + 1);

    for(int i = 0; i < len; i++)
        buf[i] = str[i];

    return write(buf, len);
}

int Merlin::Server::write(char* buf, int len) {
    return _tcpClient.write(buf, len);
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

char* Merlin::Server::getID() {
    return _id;
}

String Merlin::Server::readString() {
    String ret;

    while(available() > 0)
        ret.concat(read());

    return ret;
}
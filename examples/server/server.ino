/*
  merlin.ino - Server example

  created 2018
  by Adam Pisula

  This example code is in the public domain.

  https://github.com/kherut-io/merlin.ino
*/

#define blue 2
#define white 14
#define button 16

#include <Merlin.h>

//Declare Server globally so it's possible to access it in both setup() and loop()
Merlin::Server s;

bool statusClicked = false;
String str = "";

void setup() {
  //Config
  pinMode(white, OUTPUT);
  pinMode(button, INPUT_PULLUP);

  Serial.begin(115200);
  
  //Wait for the user to press (or not) the button
  delay(3500);
  bool btnState = !digitalRead(button);

  //If he does, reset settings and thereby run WiFiManager
  s.setUp("laptop-adampisula", 8080, btnState);
  s.runWFM();

  delay(500);

  //That part is self-explanatory, I think
  Serial.println(s.getDeviceName());
  Serial.println(s.getIP());

  //Run HttpClient
  s.runHC();
  //Retrieve port to connect to via TCP
  s.requestND();
  
  //Print device ID
  Serial.println(s.getID());
  
  //Really?
  s.begin();
}

void loop() {
  //Wait for a TCP client to connect
  if (!s.connected()) {
    s.connect();
  }
  
  else {
    //If buffer's not empty, print it
    if(s.available() > 0) {
      str = s.readString();
      Serial.println(str);
      str.toUpperCase();
      
      s.write(String("ok_") + str);
    }
  }

  if(!digitalRead(button) && !statusClicked) {
    statusClicked = true;

    s.status("OK");
  }
}

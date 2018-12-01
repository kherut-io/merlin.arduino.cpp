/*
  merlin.ino - Client example

  created 2018
  by Adam Pisula

  This example code is in the public domain.

  https://github.com/kherut-io/merlin.ino
*/

#define blue 2
#define white 14
#define button 5

#include <Merlin.h>

//Declare Client globally so it's possible to access it in both setup() and loop()
Merlin::Client c;

bool statusSent = false;
String str = "";

void setup() {
  //Config
  pinMode(white, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(button, INPUT_PULLUP);

  Serial.begin(115200);
  
  //Wait for the user to press (or not) the button
  delay(3500);
  bool btnState = !digitalRead(button);

  //If they do, reset settings and thereby run WiFiManager
  digitalWrite(blue, !btnState);

  c.setUp("laptop-adampisula", 8080, btnState);
  c.runWFM();

  delay(500);

  //That part is self-explanatory, I think
  Serial.println(c.getDeviceName());
  Serial.println(c.getIP());

  //Run HttpClient
  c.runHC();
  //Retrieve port to connect to via TCP
  c.requestND();
  
  //Print device ID
  Serial.println(c.getID());
  
  //Really?
  c.begin();

  Serial.println();
}

void loop() {
  //Wait for a TCP client to connect
  if (!c.connected()) {
    delay(350);
    
    Serial.print(".");

    c.connect();
  }
  
  else {
    if(Serial.available() > 0) {
      str = Serial.readString();

      c.write(str);

      str = "";
    }

    while(c.available() > 0)
      Serial.write(c.read());
  }

  if(!digitalRead(button)) {
    if(!statusSent) {
      statusSent = true;
      c.status("OK");
    }

    if(!c.connected()) {
      c.requestND();

      statusSent = false;
    }

    delay(350);
  }
}

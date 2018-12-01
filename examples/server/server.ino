/*
  merlin.ino - Server example

  created 2018
  by Adam Pisula

  This example code is in the public domain.

  https://github.com/kherut-io/merlin.ino
*/

#define blue 2
#define white 14
#define button 5

#include <Merlin.h>

//Declare Server globally so it's possible to access it in both setup() and loop()
Merlin::Server s;

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

  Serial.println();
}

void loop() {
  //Wait for a TCP client to connect
  if (!s.connected()) {
    delay(350);
    
    Serial.print(".");

    s.connect();
  }
  
  else {
    //If buffer's not empty, print it
    if(s.available() > 0) {
      str = s.readString();
      
      Serial.print(str);
      
      str.toUpperCase();
      
      s.write(String("ok_") + str);
    }
  }

  if(!digitalRead(button)) {
    if(!statusSent) {
      statusSent = true;
      s.status("OK");
    }

    if(!s.connected()) {
      s.requestND();

      statusSent = false;
    }

    delay(350);
  }
}

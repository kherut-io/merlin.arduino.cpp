#include <Merlin.h>

Merlin::Client c;

void setup() {
  pinMode(5, INPUT_PULLUP);
  delay(3500);

  bool btnState = !digitalRead(5);

  Serial.begin(115200);

  Serial.print("Reset settings: ");
  Serial.println(btnState);

  c.setUp("orangepizero", 8080, "", btnState);
  c.runWFM();

  Serial.println("Connected.");

  Serial.println(c.getDeviceName());
  Serial.println(c.getIP());

  c.runHC();

  Serial.print("Response: ");
  Serial.println(c.requestND());

  pinMode(2, OUTPUT);
  
}

void loop() {
  digitalWrite(2, HIGH);
  delay(350);
  digitalWrite(2, LOW);
  delay(350);
}
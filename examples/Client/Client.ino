#include <Merlin.h>

Merlin::Client c;

void setup() {
  Serial.begin(115200);

  c.setUp("orangepizero", 8080, "", true);
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
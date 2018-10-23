#include <Merlin.h>

void setup() {
  delay(5000);

  Serial.begin(115200);

  Client* c = new Client("orangepizero", 8080);
  Server s;

  pinMode(2, OUTPUT);
}

void loop() {
  digitalWrite(2, HIGH);
  delay(350);
  digitalWrite(2, LOW);
  delay(350);
}

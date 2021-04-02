//Kapadokya GSM Shield : http://bit.ly/GSMshield
//İnceleme ve proje videosu: https://youtu.be/zijlJ2-lzhg

#include <SoftwareSerial.h>
SoftwareSerial Kapadokya(10, 11); // RX, TX

void setup() {
  Serial.begin(9600);
  Kapadokya.begin(9600);
}

void loop() {
  if (Kapadokya.available()) {
    Serial.write(Kapadokya.read());
  }
  if (Serial.available()) {
    Kapadokya.write(Serial.read());
  }
}

//Kapadokya GSM Shield : http://bit.ly/GSMshield
//İnceleme ve proje videosu: https://youtu.be/zijlJ2-lzhg
//Kapadokya GSM Shield ile DTMF videosu: https://youtu.be/yGVz60JAioU
#include <KapadokyaGSM.h>
KapadokyaGSM Kapadokya;
#define telNumarasi "+905431231212" // Numarayı değiştirmeyi unutmayın :) 
#define LED_pin 13
extern SoftwareSerial SIM;

void setup() {
  pinMode(LED_pin, OUTPUT);
  Kapadokya.basla();
  delay(2000);
  Serial.begin(9600);
  delay(2000);
  Kapadokya.tumSmsSil();
  delay(2000);
  SIM.println("ATS0=1");
  delay(2000);
  SIM.println("AT+DDET=1");
  delay(2000);
  Serial.println("Sistem Calisiyor");
  Kapadokya.smsGonder(telNumarasi, "Sistem acildi.");
}

void loop() {
  String gelen = "";
  if (SIM.available()) {
    while (SIM.available()) {
      delay(13);
      char c = SIM.read();
      gelen += c;
    }
    Serial.print("gelen:"); Serial.println(gelen);
    if (gelen.indexOf("DTMF: 1") != -1) {
      Serial.println("1'e basildi, LED acildi");
      digitalWrite(LED_pin, HIGH);
      Kapadokya.smsGonder(telNumarasi, "LED Acildi");
      Serial.println("LED acildi SMS gonderildi");
    }
    else if (gelen.indexOf("DTMF: 0") != -1) {
      Serial.println("0'a basildi, LED kapandi");
      digitalWrite(LED_pin, LOW);
      Kapadokya.smsGonder(telNumarasi, "LED Kapatildi");
      Serial.println("LED kapandi SMS gonderildi");
    }
  }
}

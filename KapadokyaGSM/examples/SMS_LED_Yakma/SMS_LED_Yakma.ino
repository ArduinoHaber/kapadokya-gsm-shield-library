//Kapadokya GSM Shield : http://bit.ly/GSMshield
//İnceleme ve proje videosu: https://youtu.be/zijlJ2-lzhg

#include <KapadokyaGSM.h>
KapadokyaGSM Kapadokya;
String smsMetni, smsNumarasi;
#define gonderilecekTelNumarasi  "+905321112233" // Numarayı değiştirmeyi unutmayın :) 
#define LED_pin 13

void setup() {
  pinMode(LED_pin, OUTPUT);
  Kapadokya.basla();
  delay(4000);
  Serial.begin(9600);
  delay(2000);
  Kapadokya.tumSmsSil();
  delay(2000);
  Serial.println("Sistem Calisiyor");
  Kapadokya.smsGonder(gonderilecekTelNumarasi, "Sistem acildi.");
}

void loop() {
  smsMetni = Kapadokya.smsOku(1);
  if (smsMetni.indexOf("OK") != -1) {
    if (smsMetni.length() > 7) {
      smsNumarasi = Kapadokya.numaraliSmsOku(1);
      Serial.println(smsNumarasi);
      smsMetni.toUpperCase();
      
      if (smsMetni.indexOf("LEDAC") != -1) {
        Serial.println("LED ACILDI");
        digitalWrite(LED_pin, HIGH);
        Kapadokya.smsGonder(gonderilecekTelNumarasi, "LED Acildi");
      }
      else if (smsMetni.indexOf("LEDKAPAT") != -1) {
        Serial.println("LED KAPATILDI");
        digitalWrite(LED_pin, LOW);
        Kapadokya.smsGonder(gonderilecekTelNumarasi, "LED Kapatildi");
      }
      else {
        Serial.println("Tanimsiz SMS............");
      }
      Kapadokya.tumSmsSil();
    }
  }
}

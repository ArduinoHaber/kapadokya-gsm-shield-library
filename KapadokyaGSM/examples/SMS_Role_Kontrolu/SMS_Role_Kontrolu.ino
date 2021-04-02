//Kapadokya GSM Shield : http://bit.ly/GSMshield
//İnceleme ve proje videosu: https://youtu.be/zijlJ2-lzhg

#include <KapadokyaGSM.h> 
KapadokyaGSM Kapadokya;
String smsMetni, smsNumarasi;
#define gonderilecekTelNumarasi  "+905321112233" // Numarayı değiştirmeyi unutmayın :) 
#define LED_pin 13
#define role1 2
#define role2 3

void setup() {
  pinMode(LED_pin, OUTPUT);
  pinMode(role1, OUTPUT);
  pinMode(role2, OUTPUT);
  digitalWrite(role1, HIGH);
  digitalWrite(role2, HIGH);
  Serial.begin(9600);
  Kapadokya.basla();
  delay(2000);
  pinMode(LED_pin, OUTPUT);
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
      ///////////////////////////////LED///////////////////////////////
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

      ///////////////////////////////RÖLE1///////////////////////////////
      else if (smsMetni.indexOf("ROLE1AC") != -1) {
        Serial.println("Role1 Acildi");
        digitalWrite(role1, !HIGH);
        Kapadokya.smsGonder(gonderilecekTelNumarasi, "Role1 Acildi");
      }
      else if (smsMetni.indexOf("ROLE1KAPAT") != -1) {
        Serial.println("Role1 Kapatildi");
        digitalWrite(role1, !LOW);
        Kapadokya.smsGonder(gonderilecekTelNumarasi, "Role1 Kapatildi");
      }

      ///////////////////////////////RÖLE2///////////////////////////////
      else if (smsMetni.indexOf("ROLE2AC") != -1) {
        Serial.println("Role2 Acildi");
        digitalWrite(role2, !HIGH);
        Kapadokya.smsGonder(gonderilecekTelNumarasi, "Role2 Acildi");
      }
      else if (smsMetni.indexOf("ROLE2KAPAT") != -1) {
        Serial.println("Role2 Kapatildi");
        digitalWrite(role2, !LOW);
        Kapadokya.smsGonder(gonderilecekTelNumarasi, "Role2 Kapatildi");
      }

      ///////////////////////////////TEST MESAJI///////////////////////////////
      else if (smsMetni.indexOf("NBR") != -1) {
        Serial.println("nbr mesajı geldi");
        Kapadokya.smsGonder(gonderilecekTelNumarasi, "iyilik senden naber");
      }

      ///////////////////////////////TANIMLI OLMAYAN MESAJ///////////////////////////////
      else {
        Serial.println("Tanimsiz SMS............");
      }
      Kapadokya.tumSmsSil();
    }
  }
}

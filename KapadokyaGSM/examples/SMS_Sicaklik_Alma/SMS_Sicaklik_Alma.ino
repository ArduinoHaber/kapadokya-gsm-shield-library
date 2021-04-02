//Kapadokya GSM Shield : http://bit.ly/GSMshield
//İnceleme ve proje videosu: https://youtu.be/zijlJ2-lzhg

#include <KapadokyaGSM.h>
KapadokyaGSM Kapadokya;
String smsMetni, smsNumarasi;
#define gonderilecekTelNumarasi  "+905321112233" // Numarayı değiştirmeyi unutmayın :) 
#define LM35_pin A0

void setup() {
  Kapadokya.basla();
  delay(2000);
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

      if (smsMetni.indexOf("HAVA KAC DERECE?") != -1) {
        Serial.println("Hava SMSi Geldi");
        String Sicaklik = String(analogRead(LM35_pin) * 0.48828125);
        String Sicaklik2 = "Hava " + Sicaklik + " derece.";
        char SicaklikSMS2[] = "";
        Sicaklik2.toCharArray(SicaklikSMS2, 30);
        Serial.println(SicaklikSMS2);
        Kapadokya.smsGonder(gonderilecekTelNumarasi, SicaklikSMS2);
      }
      else {
        Serial.println("Tanimsiz SMS............");
      }
      Kapadokya.tumSmsSil();
    }
  }
}

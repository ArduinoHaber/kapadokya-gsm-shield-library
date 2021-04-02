//Kapadokya GSM Shield : http://bit.ly/GSMshield
//İnceleme ve proje videosu: https://youtu.be/zijlJ2-lzhg

#include <KapadokyaGSM.h>
KapadokyaGSM Kapadokya;
String smsMetni, smsNumarasi;
#define TelNumarasi  "+905321112233" // Numarayı değiştirmeyi unutmayın :) 

void setup() {
  Kapadokya.basla();
  delay(2000);
  Serial.begin(9600);
  delay(2000);
  Kapadokya.tumSmsSil();
  delay(2000);
  Serial.println("Sistem Calisiyor");
  Kapadokya.smsGonder(TelNumarasi, "Sistem acildi.");
}

void loop() {
  Serial.print(TelNumarasi);
  Serial.println(" araniyor...");
  Kapadokya.numaraAra(TelNumarasi);
  delay(100000);//100sn bekle
}

//Kapadokya GSM Shield : http://bit.ly/GSMshield
//İnceleme ve proje videosu: https://youtu.be/zijlJ2-lzhg

//kodu yükledikten sonra gps modülünü bağlamak gerekiyor

#include <KapadokyaGSM.h>
KapadokyaGSM Kapadokya;
String smsMetni, smsNumarasi;
#define gonderilecekTelNumarasi  "+905321112233" // Numarayı değiştirmeyi unutmayın :) 
#define LED_pin 13

#define GPS Serial
#include <TinyGPS.h>
TinyGPS gps;

double enl;
double boy;
String konum;
char konum2[255];

void setup() {
  Kapadokya.basla();
  delay(2000);
  pinMode(LED_pin, OUTPUT);
  GPS.begin(9600);
  delay(2000);
  Kapadokya.tumSmsSil();
  delay(2000);
  Kapadokya.smsGonder(gonderilecekTelNumarasi, "Sistem acildi.");
}

void loop() {
  smsMetni = Kapadokya.smsOku(1);
  if (smsMetni.indexOf("OK") != -1) {
    if (smsMetni.length() > 7) {
      smsNumarasi = Kapadokya.numaraliSmsOku(1);
      smsMetni.toUpperCase();

      if (smsMetni.indexOf("LEDAC") != -1) {
        digitalWrite(LED_pin, HIGH);
        Kapadokya.smsGonder(gonderilecekTelNumarasi, "LED Acildi");
      }
      else if (smsMetni.indexOf("LEDKAPAT") != -1) {
        digitalWrite(LED_pin, LOW);
        Kapadokya.smsGonder(gonderilecekTelNumarasi, "LED Kapatildi");
      }
      else if (smsMetni.indexOf("NERDESIN") != -1 || smsMetni.indexOf("NEREDESIN") != -1 || smsMetni.indexOf("KONUM") != -1) {
        delay(1000);

        bool yeniVeri = false;
        for (unsigned long start = millis(); millis() - start < 1000;) {
          while (GPS.available()) {
            char c = GPS.read();
            if (gps.encode(c))
              yeniVeri = true;
          }
        }

        if (yeniVeri) {
          float flat, flon;
          unsigned long age;
          gps.f_get_position(&flat, &flon, &age);
          enl = flat;
          boy = flon;
          konum = "www.google.com.tr/maps/place/" + String(enl, 6) + "," + String(boy, 6);
          konum.toCharArray(konum2, 100);
          delay(1000);
        }
        delay(1000);
        char konum2[255];
        konum.toCharArray(konum2, 100);
        Kapadokya.smsGonder(gonderilecekTelNumarasi, konum2);
      }
      else {
      }
      Kapadokya.tumSmsSil();
    }
  }
}

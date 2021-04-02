// videoda detaylı anlatım: https://www.youtube.com/channel/UCVAJfMAgBurMjEcW3bZcc8A
// GSM: https://www.robolinkmarket.com/kapadokya-gsm-mini-imei-kayitli-gsm-modul


#include <KeyMatrix.h> // Arduino IDE kütüphane yöneticisinden yüklenebilir
KEY_MATRIX_PHONE(keypad, 17, 18, 19, 2, 14, 15, 16); // 3x4=12 butonlu keypad baülantıları
String  numara = "+9"; // Ülke alan kodu
String  numara2 = "";

#include <Adafruit_GFX.h> // Arduino IDE kütüphane yöneticisinden yüklenebilir
#include <Adafruit_PCD8544.h> // Arduino IDE kütüphane yöneticisinden yüklenebilir
Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3); // 5110 ekranı bağlantıları
// pin 7 - SCLK
// pin 6 - DIN
// pin 5 - D/C
// pin 4 - CS - CE
// pin 3 - RST

#include <KapadokyaGSM.h>
KapadokyaGSM Kapadokya;
String smsMetni, smsNumarasi;

void setup() {
  Serial.begin(9600);
  display.begin();
  display.setContrast(50);
  display.clearDisplay();
  display.display();

  Kapadokya.basla();
  delay(5000);
  Kapadokya.tumSmsSil();
  delay(2000);
  Serial.println("Sistem Calisiyor");
  Kapadokya.sesAc();
}

void loop() {
  if (keypad.pollEvent()) {
    Serial.print("evnt:"); Serial.println(event_to_string(keypad.event));
  }
}


String event_to_string(const KeyMatrixEvent &ev) {
  String s;
  switch (ev.type) {
    case KM_NONE:
      s = "KM_NONE (";
      break;
    case KM_KEYUP:
      s = "KM_KEYUP (";
      return "0";
      break;
    case KM_KEYDOWN:
      s = "KM_KEYDOWN (";
      delay(100);
      break;
    case KM_TEXT:
      s = "KM_TEXT (";
      break;
    case KM_MODE:
      s = "KM_MODE (";
      break;
  }
  if (ev.type == KM_MODE) {
    switch (ev.c) {
      case KM_MODE_LOWER:
        s += "KM_MODE_LOWER";
        break;
      case KM_MODE_UPPER:
        s += "KM_MODE_UPPER";
        break;
      case
          KM_MODE_NUM:
        s += "KM_MODE_NUM";
        break;
    }
  }
  else {
    s += ev.c;
    //Serial.println(s);
  }
  s += ")";
  if (ev.c == '#') {
    if ( Kapadokya.aramaDurumuAl() == 3) {
      Kapadokya.aramaCevapla();
    }
    else {
      Serial.print("ARA :"); Serial.println(numara);
      char *num = numara.c_str();;
      Kapadokya.numaraAra(num);
    }

    numara = "+9";
    numara2 = "";
    display.clearDisplay();
    display.display();
  }
  else if (ev.c == '*') {
    Serial.print("ARAMA BİTİR");
    Kapadokya.aramaBitir();
    numara = "+9";
    numara2 = "";
  }
  else {
    numara += ev.c;
    numara2 += ev.c;
    Serial.print("ev.c:"); Serial.println(ev.c);
    Serial.print("numr:"); Serial.println(numara);
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(BLACK);
    display.setCursor(10, 20);
    display.println(numara2);
    display.display();
  }
  return s;
}

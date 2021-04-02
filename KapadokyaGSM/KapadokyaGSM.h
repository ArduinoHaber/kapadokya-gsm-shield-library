#ifndef KapadokyaGSM_h
#define KapadokyaGSM_h
#include <SoftwareSerial.h>
#include "Arduino.h"

#define RX_PIN 10
#define TX_PIN 11	

class KapadokyaGSM{									
  private:
	int _timeout;
	String _tampon;
		String _serialOku();
  	
  public:

 	void basla();	
	void reset();
	void sesAc();
 	// arama metodları
 	bool aramaCevapla(); 	
 	void numaraAra(char* number);
 	bool aramaBitir();
 	uint8_t aramaDurumuAl();   
 	//sms metodları
	bool smsGonder(char* number,char* text);	 
	String smsOku(uint8_t index); 
	String numaraliSmsOku(uint8_t index);
	bool tumSmsSil();    

	void sinyalKalitesi();
	void telefonOzelligiAyarla();
	void tasiyiciProfiliniAc();
	void tasiyiciProfiliniKapat();
	//void RTCzaman(int *day,int *month, int *year,int *hour,int *minute, int *second);  
	String RTCzaman();  
	String tarih(); 
	bool RTCguncelle(int utc);  
};

#endif 
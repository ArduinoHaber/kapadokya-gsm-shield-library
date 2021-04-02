#include "Arduino.h"
#include "KapadokyaGSM.h"
#include <SoftwareSerial.h>

SoftwareSerial SIM(RX_PIN,TX_PIN);

void KapadokyaGSM::basla(){
	SIM.begin(9600);
  _tampon.reserve(255);
}

//
//PRIVATE METHODS
//
String KapadokyaGSM::_serialOku(){
  _timeout=0;
  while  (!SIM.available() && _timeout < 12000  ) 
  {
    delay(13);
    _timeout++;
  }
  if (SIM.available()) {
 	return SIM.readString();
  }
}


//
//PUBLIC METHODS
//


void KapadokyaGSM::sesAc(){
  SIM.print("AT+CRSL=100\r\n");// çalma sesi yüksekliği %100
  delay(100);
  SIM.print("AT+CLVL=100\r\n"); // konuşma sesi yüksekliği %100
  delay(100);
}


void KapadokyaGSM::reset(){
  SIM.print(F("AT\r\n"));
  while (_serialOku().indexOf("OK")==-1 ){
    SIM.print(F("AT\r\n"));
  }
  while (_serialOku().indexOf("SMS")==-1 ){
  }
}


void KapadokyaGSM::telefonOzelligiAyarla(){
  SIM.print (F("AT+CFUN=1\r\n"));
}


void KapadokyaGSM::sinyalKalitesi(){
/*Response
+CSQ: <rssi>,<ber>Parameters
<rssi>
0 -115 dBm or less
1 -111 dBm
2...30 -110... -54 dBm
31 -52 dBm or greater
99 not known or not detectable
<ber> (in percent):
0...7 As RXQUAL values in the table in GSM 05.08 [20]
subclause 7.2.4
99 Not known or not detectable 
*/
  SIM.print (F("AT+CSQ\r\n"));
  Serial.println(_serialOku());
}


void KapadokyaGSM::tasiyiciProfiliniAc(){
  SIM.print (F(" AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\" \r\n" ));_tampon=_serialOku();  
  SIM.print (F(" AT+SAPBR=3,1,\"APN\",\"internet\" \r\n" ));_tampon=_serialOku(); 
  SIM.print (F(" AT+SAPBR=1,1 \r\n"));delay(1200);_tampon=_serialOku();
  SIM.print (F(" AT+SAPBR=2,1\r\n "));delay(3000);_tampon=_serialOku();
}


void KapadokyaGSM::tasiyiciProfiliniKapat(){
  SIM.print (F("AT+SAPBR=0,1\r\n "));
  delay(1500);
}


bool KapadokyaGSM::aramaCevapla(){
   SIM.print (F("ATA\r\n"));
   _tampon=_serialOku();
   if ( (_tampon.indexOf("OK") )!=-1 ) return true;  
   else return false;
}


void  KapadokyaGSM::numaraAra(char* number){
  SIM.print (F("ATD"));
  SIM.print (number);
  SIM.print (F(";\r\n"));
}


uint8_t KapadokyaGSM::aramaDurumuAl(){
/*
  dönüş değerleri:
 
 0 Ready (MT allows commands from TA/TE)
 2 Unknown (MT is not guaranteed to respond to tructions)
 3 Ringing (MT is ready for commands from TA/TE, but the ringer is active)
 4 Call in progress
*/
  SIM.print (F("AT+CPAS\r\n"));
  _tampon=_serialOku();  
  return _tampon.substring(_tampon.indexOf("+CPAS: ")+7,_tampon.indexOf("+CPAS: ")+9).toInt();
}



bool KapadokyaGSM::aramaBitir(){
  SIM.print (F("ATH\r\n"));
  _tampon=_serialOku();
  if ( (_tampon.indexOf("OK") ) != -1)
   return true;
  else 
  return false;
}




bool KapadokyaGSM::smsGonder(char* number,char* text){

    SIM.print (F("AT+CMGF=1\r")); 
    _tampon=_serialOku();
    SIM.print (F("AT+CMGS=\""));  
    SIM.print (number);           
    SIM.print(F("\"\r"));       
    _tampon=_serialOku(); 
    SIM.print (text);
    SIM.print ("\r"); 
    _tampon=_serialOku();
    SIM.print((char)26);
    _tampon=_serialOku();
    if (((_tampon.indexOf("CMGS") ) != -1 ) ){
      return true;
    }
    else {
      return false;
    }
}


String KapadokyaGSM::numaraliSmsOku(uint8_t index){
  _tampon=smsOku(index);
  Serial.println(_tampon.length());
  if (_tampon.length() > 10) {
    uint8_t _idx1=_tampon.indexOf("+CMGR:");
    _idx1=_tampon.indexOf("\",\"",_idx1+1);
    return _tampon.substring(_idx1+3,_tampon.indexOf("\",\"",_idx1+4));
  }
  else{
    return "";
  }
}



String KapadokyaGSM::smsOku(uint8_t index){
  SIM.print (F("AT+CMGF=1\r")); 
  if (( _serialOku().indexOf("ER")) ==-1) {
    SIM.print (F("AT+CMGR="));
    SIM.print (index);
    SIM.print("\r");
    _tampon=_serialOku();
    if (_tampon.indexOf("CMGR:")!=-1){
      return _tampon;
    }
    else return "";    
    }
  else
    return "";
}

bool KapadokyaGSM::tumSmsSil(){ 
  SIM.print(F("at+cmgda=\"del all\"\n\r"));
  _tampon=_serialOku();
  if (_tampon.indexOf("OK")!=-1) {return true;}else {return false;}
}


String KapadokyaGSM::RTCzaman(){
//void KapadokyaGSM::RTCzaman(int *day,int *month, int *year,int *hour,int *minute, int *second){
  SIM.print(F("at+cclk?\r\n"));
  // if respond with ERROR try one more time. 
  _tampon=_serialOku();
  if ((_tampon.indexOf("ERR"))!=-1){
    delay(50);
    SIM.print(F("at+cclk?\r\n"));
  } 
  //String yyear,mmonth,dday,hhour,mminute,ssecond;
  if ((_tampon.indexOf("ERR"))==-1){
    _tampon=_tampon.substring(_tampon.indexOf("\"")+1,_tampon.lastIndexOf("\"")-1);  
    /*
    *year=_tampon.substring(0,2).toInt();
    *month= _tampon.substring(3,5).toInt();
    *day=_tampon.substring(6,8).toInt();
    *hour=_tampon.substring(9,11).toInt();
    *minute=_tampon.substring(12,14).toInt();
    *second=_tampon.substring(15,17).toInt();
    */
   /*
    yyear=_tampon.substring(0,2);
    mmonth= _tampon.substring(3,5);
    dday=_tampon.substring(6,8);
    hhour=_tampon.substring(9,11);
    mminute=_tampon.substring(12,14);
    ssecond=_tampon.substring(15,17);
    */
    /*
    Serial.print("yyear: ");Serial.println(yyear);
    Serial.print("mmont: ");Serial.println(mmonth);
    Serial.print("ddayy: ");Serial.println(dday);
    Serial.print("hhour: ");Serial.println(hhour);
    Serial.print("mminu: ");Serial.println(mminute);
    Serial.print("sseco: ");Serial.println(ssecond);
    */
   //String tarihSaat =_tampon.substring(9,11)+":"+_tampon.substring(12,14)+":"+_tampon.substring(15,17)+" "+_tampon.substring(6,8)+"."+_tampon.substring(3,5)+"."+_tampon.substring(0,2);
    //String tarihSaat = hhour+":"+mminute+":"+ssecond+" "+dday+"."+mmonth+"."+yyear;
    //String tarihSaat = String(hhour)+":"+String(mminute)+" "+String(dday)+"."+String(mmonth)+"."+String(yyear);
    //Serial.println(tarihSaat);
    //return tarihSaat;
    return _tampon.substring(9,11)+":"+_tampon.substring(12,14)+":"+_tampon.substring(15,17)+" "+_tampon.substring(6,8)+"."+_tampon.substring(3,5)+"."+_tampon.substring(0,2);
 }
}

String KapadokyaGSM::tarih() {
  SIM.print(F("AT+CIPGSMLOC=2,1\r\n "));
  _tampon=_serialOku();

  if (_tampon.indexOf("OK")!=-1 ){
    return _tampon.substring(_tampon.indexOf(":")+2,(_tampon.indexOf("OK")-4));
  } 
  else
  return "0";      
}

bool KapadokyaGSM::RTCguncelle(int utc){
  
  tasiyiciProfiliniAc();
  _tampon=tarih();
  tasiyiciProfiliniKapat();
  
  _tampon=_tampon.substring(_tampon.indexOf(",")+1,_tampon.length());
  String dt=_tampon.substring(0,_tampon.indexOf(","));
  String tm=_tampon.substring(_tampon.indexOf(",")+1,_tampon.length()) ;

  int hour = tm.substring(0,2).toInt();
  int day = dt.substring(8,10).toInt();

  hour=hour+utc;

  String tmp_hour;
  String tmp_day;

  if (hour<0){
    hour+=24;
    day-=1;
  }
  if (hour<10){
      tmp_hour="0"+String(hour);
  }
  else{
    tmp_hour=String(hour);
  }
  if (day<10){
    tmp_day="0"+String(day);    
  }
  else{
    tmp_day=String(day);
  }
  Serial.println("at+cclk=\""+dt.substring(2,4)+"/"+dt.substring(5,7)+"/"+tmp_day+","+tmp_hour+":"+tm.substring(3,5)+":"+tm.substring(6,8)+"-03\"\r\n");
  SIM.print("at+cclk=\""+dt.substring(2,4)+"/"+dt.substring(5,7)+"/"+tmp_day+","+tmp_hour+":"+tm.substring(3,5)+":"+tm.substring(6,8)+"-03\"\r\n");
  if ( (_serialOku().indexOf("ER"))!=-1) {
    return false;
    }
    else return true;
  }
#include "Arduino.h"
#include <SoftwareSerial.h>
#include "esp.h"


 /*************************Class Constructor*****************************
DESC:       Stores all the pins used to control the ESP
PARAMETERS: Rx Pin, Tx Pin, Enable Pin
RETURNS:    Nothing
************************************************************************/
ESP::ESP(uint8_t rx, uint8_t tx, uint8_t enable){
  _Rx = rx;
  _Tx = tx;
  _Enable = enable;
}

/***********************************************************************
DESC:       Set's up the SSID and Password of your Router
PARAMETERS: SSID, Password
RETURNS:    Nothing
************************************************************************/
void ESP::init(char* SSID, char* PASS){
  _SSID = SSID;
  _PASSWD = PASS;
} 

/***********************************************************************
DESC:       Brings the ESP out of deep sleep & starts the Serial
PARAMETERS: True or false
RETURNS:    Nothing
************************************************************************/
void ESP::enabled(boolean _ESPOn){
  if(_ESPOn){
    pinMode(_Enable, OUTPUT);
    digitalWrite(_Enable, HIGH);
    Serial.println("enabled");
    ESPSerial = new SoftwareSerial(_Rx, _Tx); // RX, TX
    ESPSerial->begin(9600);
    ESPSerial->setTimeout(5000);
  } else {
    digitalWrite(_Enable, LOW);
    pinMode(_Enable, OUTPUT);
    Serial.println("disabled");
    ESPSerial->end();
  }
}


/***********************************************************************
DESC:       Set's up the SSID and Password of your Router
PARAMETERS: SSID, Password
RETURNS:    Nothing
************************************************************************/
void ESP::start(void){
  while(responce()){
    delay(5000);
  }
  delay(1000);
  //connect to the wifi
  boolean connected = false;
  for (int i = 0; i < 5; i++){
    if (connect()){
      connected = true;
      break;
    }
  }
  if (!connected){
    while (1);
  }
  delay(5000);
  //print the ip addr
  /*ESPSerial->println("AT+CIFSR");
  Serial.println("ip address:");
  while (ESPSerial->available())
    Serial.write(ESPSerial->read());*/
  //set the single connection mode
  ESPSerial->println("AT+CIPMUX=0");
}

/***********************************************************************
DESC:       Waits on a responce from the ESP
PARAMETERS: Nothing
RETURNS:    boolean, Ready or not
************************************************************************/
boolean ESP::responce(){
  ESPSerial->println("AT+RST");
  delay(1000);
  if (ESPSerial->find("ready")){
    Serial.println("Module is ready");
    return false;
  }
  else {
    Serial.println("Module have no response.");
    return true;
    //while (1);
  }
}


/***********************************************************************
DESC:       Connects the ESP to your network
PARAMETERS: Nothing
RETURNS:    boolean, Connected or not
************************************************************************/
boolean ESP::connect(void){
  ESPSerial->println("AT+CWMODE=1");
  String cmd = "AT+CWJAP=\"";
  cmd += _SSID;
  cmd += "\",\"";
  cmd += _PASSWD;
  cmd += "\"";
  Serial.println(cmd);
  ESPSerial->println(cmd);
  delay(2000);
  if (ESPSerial->find("OK"))
  {
    Serial.println("OK, Connected to WiFi.");
    return true;
  } else
  {
    Serial.println("Can not connect to the WiFi.");
    return false;
  } 
}

/***********************************************************************
DESC:       Enables and set's up ESP, Posts your data
PARAMETERS: your data to post
RETURNS:    boolean, Posted or not
************************************************************************/
boolean ESP::post(char* _data){
  enabled(true);
  start();
  return postData(_data);
}
    
/***********************************************************************
DESC:       Connects to Xively, creates the PUT and post's your data
PARAMETERS: the data to post
RETURNS:    boolean, Posted or not
************************************************************************/   
boolean ESP::postData(char* _data2Post){
  int length = strlen(_data2Post);
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += "api.xively.com";
  cmd += "\",80";
  Serial.println(cmd);
  ESPSerial->println(cmd);
  delay(2000);
  if(ESPSerial->find("Error")){
    return false;
  }

  cmd = "PUT http://api.xively.com/v2/feeds/***********your FEED************.csv HTTP/1.0\r\n";
  cmd += "Host: api.xively.com\r\n";
  cmd += "X-PachubeApiKey: ************************your KEY*********************\r\n"; 
  cmd += "Content-Length: " + String(length) + "\r\n\r\n";
  cmd += _data2Post;
  
  Serial.println(cmd);
  ESPSerial->print("AT+CIPSEND=");
  ESPSerial->println(cmd.length());
  if(ESPSerial->find(">")){
    ESPSerial->print(cmd);
  }else{
    ESPSerial->println("AT+CIPCLOSE");
  }
  Serial.println("Posted to Xively");
  Serial.println(_data2Post);
  enabled(false);
  return true;
}


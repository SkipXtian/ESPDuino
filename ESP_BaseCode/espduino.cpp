#include "Arduino.h"
#include <SoftwareSerial.h>
#include "espduino.h"

/*************************Class Constructor*****************************
 * DESC:		Sets all the pins used to control the ESP and creates
 *				an internal SoftwareSerial object
 * PARAMETERS:
 * 				unit8_t rxPin		- software serial rx pin
 *				uint8_t txPin		- software serial tx pin
 *				uint8_t enablePin	- ESP enable pin
 *				boolean debug		- hardware serial debugging (on/off)
 * RETURNS:
 *				None
 ***********************************************************************/
 ESPduino::ESPduino(uint8_t rxPin, uint8_t txPin, uint8_t enablePin, boolean debug){
  _rxPin = rxPin;
  _txPin = txPin;
  _enablePin = enablePin;
  _debugEnabled = debug;
  
  // Instantiate a new SoftwareSerial object for the ESP module
  _ESPSerial = new SoftwareSerial(_rxPin, _txPin);
}

/***********************************************************************
 * DESC:       Set up the SSID and Password of your WiFi Router
 * PARAMETERS:
 *				char* ssid			- your WiFi ssid
 *				char* ssidPassword	- your WiFi password
 * RETURNS:
 *				None
 ***********************************************************************/
 void ESPduino::setup(char* ssid, char* password){
  _ssid = ssid;
  _ssidPassword = password;
}

/***********************************************************************
 * DESC:       Outputs string to hardware serial if debugging is enabled
 * PARAMETERS:
 *				String s	- String to output to hardware serial
 * RETURNS:
 *				None
 ***********************************************************************/
 void ESPduino::_debug(String s){
  if(!_debugEnabled) {
    
    return;
  }
  Serial.println(s);  
}
// Overloaded method for single characters
void ESPduino::_debug(char s){
  if(!_debugEnabled) {
    
    return;
  }
  Serial.write(s); 
}
// Overloaded method for character arrays with type conversion
void ESPduino::_debug(char* s){
	_debug(String(s));
}


/***********************************************************************
 * DESC:       Get the current state of the ESP module
 * PARAMETERS:
 *				None
 * RETURNS:
 *				Boolean
 ***********************************************************************/
 boolean ESPduino::getState(){

  return _ESPState;
}

/***********************************************************************
 * DESC:       Set the current state of the ESP module
 * PARAMETERS:
 *				Boolean state
 * RETURNS:
 *				None
 ***********************************************************************/
 void ESPduino::setState(boolean state){

  // Are we already in the given state?
  if(_ESPState == state){
    
   return;
 }
  // Set the internal state
 _ESPState = state;
 
  // Enable or disable the ESP accordingly
 if(_ESPState){
   _enableESP();
 } else {
   _disableESP();
 }
}

/***********************************************************************
 * DESC:       Brings the ESP out of deep sleep, start SoftwareSerial
 * PARAMETERS:
 *				None
 * RETURNS:
 *				None
 ***********************************************************************/
 void ESPduino::_enableESP(){
  pinMode(_enablePin, OUTPUT);
  digitalWrite(_enablePin, HIGH);  
  _debug("enabled");  
  _ESPSerial->begin(9600);
  _ESPSerial->setTimeout(5000);
}

/***********************************************************************
 * DESC:       Disable the ESP module
 * PARAMETERS:
 *				None
 * RETURNS:
 *				None
 ***********************************************************************/
 void ESPduino::_disableESP(){
  digitalWrite(_enablePin, LOW);
  pinMode(_enablePin, OUTPUT);
  _debug("disabled");
  _ESPSerial->end();
}

/***********************************************************************
 * DESC:       Attempts connection to the WiFi via the ESP module
 * PARAMETERS:
 *				None
 * RETURNS:
 *				None
 ***********************************************************************/
 void ESPduino::_start(void){

  // Attempt to gain a response from the ESP module
  while(_reset() == false){ 
  
    // No response, toggle the ESP enable line
	_disableESP();
	_enableESP();
  
    // Wait before trying again
    delay(1000);
  }
  
  // Attempt to connect to the WiFi
  boolean connected = false;
  for (int i = 0; i < 5; i++){
    if (_connect()){
      connected = true;
	  
      break;
    }
  }
  
  // This needs work... :)
  if (!connected){
    while (1);
  }
  //delay(5000);
  
  // Print the ip address
  /*_ESPSerial->println("AT+CIFSR");
  _debug("ip address:");
  while (_ESPSerial->available())
    Serial.write(_ESPSerial->read());*/
   
  // Sets single connection mode
  String cmd = "AT+CIPMUX=0";
  _debug(cmd);
   _ESPSerial->println(cmd);
}

/***********************************************************************
 * DESC:       Resets the ESP and awaits for a response
 * PARAMETERS:
 *				None
 * RETURNS:
 *				boolean	- true = response, false = no response
 ***********************************************************************/
 boolean ESPduino::_reset(){
 
  // Reset the ESP module
  String cmd = "AT+RST";  
  _debug(cmd);
  _ESPSerial->println(cmd);  
  delay(2000);

  // Search for a ready status
  if (_ESPSerial->find("ready")){
    _debug("Module is ready");
    
    return true;
  }
  else {
    _debug("Module did not respond");
    
    return false;    
  }
}

/***********************************************************************
 * DESC:       Attempts to connect the ESP to your WiFi network
 * PARAMETERS:
 *				None
 * RETURNS:
 *				boolean	- connection successful or not
 ***********************************************************************/
 boolean ESPduino::_connect(void){
 
  // Set STA mode
  _ESPSerial->println("AT+CWMODE=1");
  
  // Construct authentication string
  String cmd = "AT+CWJAP=\"";
  cmd += _ssid;
  cmd += "\",\"";
  cmd += _ssidPassword;
  cmd += "\"";
  _debug(cmd);
  
  // Attempt connection
  _ESPSerial->println(cmd);
  delay(5000);
  
  // Search for an OK response
  if (_ESPSerial->find("OK"))
  {
    _debug("OK, Connected to WiFi");
    
    return true;
  } else
  {
    _debug("Could not connect to WiFi");
    
    return false;
  } 
}

/***********************************************************************
 * DESC:       Attempts a GET request
 * PARAMETERS:
 *        char* ip           	    - IP Address
 *        int port                - TCP IP port
 *        char* uri               - GET URI
 *        int* httpResponseCode   - pointer to httpResponseCode
 *        char* httpResponseData  - pointer to httpResponseData
 * RETURNS:
 *        boolean - GET successful or not
 ***********************************************************************/
boolean ESPduino::get(char* ip, int port, char* uri) {//, int* httpResponseCode, char* httpResponseData) {

  // Check whether TCP is already connected  
  if(_tcpConnected() != true) {

    // Reset the WiFi connection
    _start();

  } else {

    // Close TCP connection
    _tcpClose();
  }

  // Attempt a TCP Connection
  if(_tcpConnect(ip, port) != true) {
      _debug("TCP connection failed, aborting GET");

      return false;
  }

  // Construct GET string
  String getStr("GET ");
  getStr += String(uri);
  getStr += "\r\n\r\n";

  // Attempt GET
  _ESPSerial->print("AT+CIPSEND=");
  _ESPSerial->println(getStr.length());
   
  if(_ESPSerial->find(">")) {
	  _debug(getStr);
    _ESPSerial->print(getStr);

  } else {
    _tcpClose();

    return false;
  }

  // GET ok.
  return true;
}

/***********************************************************************
 * DESC:       Determine whether there is a TCP connection already
 * PARAMETERS:
 *        None
 * RETURNS:
 *        boolean - connected or not
 ***********************************************************************/
boolean ESPduino::_tcpConnected() {

  // Get the current TCP Connection Status
  String getStatus = "AT+CIPSTATUS";
  _debug("Get TCP connection status");
  _debug(getStatus);  
  _ESPSerial->println(getStatus);
  
  // Search for the correct status
  if(!_ESPSerial->find("STATUS:3")) {
    _debug("TCP not connected");

    return false;
  }

  _debug("TCP is already connected");

  return true;
}

/***********************************************************************
 * DESC:       Attempts a TCP connection
 * PARAMETERS:
 *        char* ip               - IP Address
 *        int port               - TCP IP port
 * RETURNS:
 *        boolean - connection successful or not
 ***********************************************************************/
boolean ESPduino::_tcpConnect(char* ip, int port) {

  // Attempt TCP connection
  String tcpStr = "AT+CIPSTART=\"TCP\",\"";
  tcpStr += ip;
  tcpStr += "\",80";
  _debug(tcpStr);
  _ESPSerial->println(tcpStr);    
  delay(2000); 
  
  if(_ESPSerial->find("Error")){
    _debug("TCP connection error!");

    return false;
  }

  _debug("TCP connected");

  return true;
}


/***********************************************************************
 * DESC:       Close a TCP connection
 * PARAMETERS:
 *        None
 * RETURNS:
 *        None
 ***********************************************************************/
void ESPduino::_tcpClose(void) {

  // Close TCP connection
  _debug("Closing TCP connection");
  _ESPSerial->println("AT+CIPCLOSE");
  _dump();
}

/***********************************************************************
 * DESC:       Flush the SoftwareSerial buffer
 * PARAMETERS:
 *        None
 * RETURNS:
 *        None
 ***********************************************************************/
void ESPduino::_flush(void) {

  while (_ESPSerial->available())
    _ESPSerial->read();
  _ESPSerial->flush();
}

/***********************************************************************
 * DESC:       Dump the SoftwareSerial buffer
 * PARAMETERS:
 *        None
 * RETURNS:
 *        None
 ***********************************************************************/
void ESPduino::_dump(void) {

  while (_ESPSerial->available())
    _debug(_ESPSerial->read());
}

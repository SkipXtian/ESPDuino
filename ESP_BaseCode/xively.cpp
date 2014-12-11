#include "Arduino.h"
#include "xively.h"

/***********************************************************************
 * DESC:		Enables and starts the ESP module, connects to Xively,
 *				creates the PUT string and puts your data.
 * PARAMETERS:
 *				char* feed	- Xively feed
 *				char* key	- Xively key
 *				char* data	- data to put
 * RETURNS:
 *				boolean		- successful or not
 ***********************************************************************/
 boolean xively::put(char* feed, char* key, char* data){
 
  // Enable and start the ESP module
   setState(true);
  _start();

  // Construct TCP connection string
  String tcpCmd = "AT+CIPSTART=\"TCP\",\"";
  tcpCmd += "api.xively.com";
  tcpCmd += "\",80";
  _debug(tcpCmd);
  
  // Attempt to open TCP connection
  _ESPSerial->println(tcpCmd);
  delay(2000);
  if(_ESPSerial->find("Error")){
  
    return false;
  }  

  // Construct XIVELY put string
  String putStr = "PUT http://api.xively.com/v2/feeds/";
  putStr += feed;
  putStr += ".csv HTTP/1.0\r\n";
  putStr += "Host: api.xively.com\r\n";
  putStr += "X-PachubeApiKey: ";
  putStr += key;
  putStr += "\r\n";
  putStr += "Content-Length: " + String(strlen(data)) + "\r\n\r\n";
  putStr += data;  
  _debug(putStr);
  
  // Send the PUT string
  _ESPSerial->print("AT+CIPSEND=");
  _ESPSerial->println(putStr.length());
  if(_ESPSerial->find(">")){
    _ESPSerial->print(putStr);
  }else{
	 // Failed!
	 _ESPSerial->println("AT+CIPCLOSE");
	
	return false;
  }
  
  // Should we close the TCP connection here?
  //_ESPSerial->println("AT+CIPCLOSE");
  
  _debug("Posted to Xively");
  _debug(data);
  
  // Disable the ESP module
  setState(false);
  
  return true;
}
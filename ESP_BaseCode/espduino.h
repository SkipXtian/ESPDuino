#ifndef _ESPduino_H_
#define _ESPduino_H_

#include "Arduino.h"
#include <SoftwareSerial.h>
#include <espHardwarePins.h>

class ESPduino
{
  public:
	/** Methods **/
	
	// Constructor
    ESPduino(uint8_t rxPin, uint8_t txPin, uint8_t enablePin, boolean debug);
	
	// ESP WiFi settings
    void setup(char* ssid, char* ssidPassword);
    
	// These methods need to be implemented...
	boolean get(char* ip, char* uri); //, int* httpResponseCode, char* httpResponseData);
	// boolean put(char* ip, char* uri, int* httpResponseCode, char* httpResponseData);
	// boolean post(char* ip, char* uri, char* data, int* httpResponseCode, char* httpResponseData);
	// boolean delete(char* ip, char* uri, int* httpResponseCode, char* httpResponseData);
	
	// Enable / Disable the ESP module
	boolean getState();
	void setState(boolean state);

  protected:
	/** Methods **/
	
	void _enableESP();
	void _disableESP();    
	void _start(void);
    boolean _connect(void);    
    boolean _noResponse(void);    
	
	// Hardware serial debugging
	void _debug(char* s);
	void _debug(String s);
	
	/** Variables **/
    SoftwareSerial *_ESPSerial;
	
	// ESP hardware pins
    uint8_t _enablePin;
    uint8_t _rxPin;
    uint8_t _txPin;
	
	// ESP settings and data
    char* _ssid;
    char* _ssidPassword;	
	
	// Other internal
	boolean _debugEnabled;
	boolean _ESPState;
};
#endif

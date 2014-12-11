#ifndef _xively_H_
#define _xively_H_

#include "Arduino.h"
#include <espduino.h>


class xively : public ESPduino
{
  public:

  	// Superclass constructor
  	xively(uint8_t rxPin, uint8_t txPin, uint8_t enablePin, boolean debug) : ESPduino(rxPin, txPin, enablePin, debug) {}

  	// Overridden put method for xively
	boolean put(char* feed, char* key, char* data);	
};
#endif
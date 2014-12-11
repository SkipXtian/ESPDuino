#ifndef _ESP_H_
#define _ESP_H_

#include "Arduino.h"
#include <SoftwareSerial.h>

class ESP{
  public:
    ESP(uint8_t rx, uint8_t tx, uint8_t enable);//void);
    void init(char* SSID, char* PASS);
    boolean post(char* _data);
    
  private:
    void enabled(boolean _ESPOn);
    boolean connect(void);
    void start(void);
    boolean responce(void);
    boolean postData(char* data2Post);
    SoftwareSerial *ESPSerial;
    uint8_t _Enable;
    uint8_t _Rx;
    uint8_t _Tx;
    char* _SSID;
    char* _PASSWD;
    char* _data2Post;

};
#endif

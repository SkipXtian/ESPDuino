/********************************************************************
**                   ESPDuino Example Code                         **
**                                                                 **
**  NOTE: Change your SSID & Password below. The Post still needs  **
**  work, You'll have to edit it (bottom of esp.cpp) to post where **
**  you want it. eg your feed, key for Xively or your server etc   **
**                                                                 **
** This example sleeps the ESPDuino and the ESP8266 for multiples  **
**           of 8 seconds, then posts your data.                   **
**                                                                 **
** PLEASE TEACH ME TO CODE WELL! I MIGHT BE DOING A LOT INCORRECT! **
*********************************************************************/

//TODO: Error Checking, Set up post properly

#include <SoftwareSerial.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include "pinDef_v1.h"
#include "esp.h"

volatile int sleep_count = 0;  
byte sleep_total = 7;  //Number of times to sleep (sleeps for 8 seconds at a time)

#define SSID  "**************yourSSID*********"
#define PASS  "************yourPassword*********"

ESP wifi(ESP_RXD, ESP_TXD, ESP_ENB);

void setup(){
  Serial.begin(9600);  
  Serial.println("ESP8266 Battery Demo");  
  delay(250);  //Delay fro Serial to complete before we Sleep
  wifi.init(SSID, PASS);
}

void loop(){
  if(sleep_count >= sleep_total) { 
    sleep_count = 0;
    wdt_disable();  //Disable Watchdog timer
    wifi.post("Bat_lvl,700");
  } 
  Serial.println("Sleeping");
  delay(250);  //Delay fro Serial to complete before we Sleep
  gotoSleep();  //Sleeps the uC for 8 seconds
}

  

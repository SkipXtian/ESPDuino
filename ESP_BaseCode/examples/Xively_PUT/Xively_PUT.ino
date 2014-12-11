/********************************************************************
**                ESPDuino - Xively Example Code                   **
**                                                                 **
**  NOTE: Change your SSID & Password below. The PUT still needs   **
**  work? <- maybe, someone please check with their xively account **
**                                                                 **
** This example sleeps the ESPDuino and the ESP8266 for multiples  **
**           of 8 seconds, then PUTs your data.                    **
**                                                                 **
*********************************************************************/

// TODO: Error Checking, Set up PUT properly
#include <SoftwareSerial.h>
#include <xively.h>
#include <sleep.ino>

// Number of times to sleep (sleeps for 8 seconds at a time)
byte sleep_total = 7;

// WiFi Settings
#define WIFI_SSID  "**************yourSSID*********"
#define WIFI_PASSWORD  "************yourPassword*********"

// Xively Settings
char xivelyFeed[] = "your-feed";
char xivelyKey[] = "your-key";

// Create an Xively object (inherited from ESPDuino), named wifi with serial debugging on
xively wifi(ESP_RXD, ESP_TXD, ESP_ENB, true);

// Main Setup function
void setup() {

  // Start hardware serial debugging
  Serial.begin(9600);
  Serial.println("ESP8266 Battery Demo");

  // Delay for Serial output to complete before we Sleep
  delay(250);  
  
  // WiFi settings
  wifi.setup(WIFI_SSID, WIFI_PASSWORD);
}

// Main Loop function
void loop() {

  if(sleep_count >= sleep_total) {

    // Disable Watchdog timer
    wdt_disable();

    // Reset the Sleep Counter
    sleep_count = 0;

    // Attempt to PUT data to xively
    if(wifi.put(xivelyFeed, xivelyKey, "Bat_lvl,700")) {

      Serial.println("PUT ok.");

    } else {

      Serial.println("PUT failed!");

    }
  } 

  // Debug
  Serial.println("Sleeping");

  // Delay for Serial output to complete before we Sleep
  delay(250);

  // Sleeps the Arduino μC for 8 seconds
  gotoSleep();
} 
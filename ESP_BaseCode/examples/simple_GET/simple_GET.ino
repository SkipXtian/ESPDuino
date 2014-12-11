/********************************************************************
**              ESPDuino - Simple GET Example Code                 **
**                                                                 **
**  NOTE: Change your SSID & Password below.                       **
**                                                                 **
*********************************************************************/

#include <SoftwareSerial.h>
#include <espduino.h>

// WiFi Settings
#define WIFI_SSID  "**************yourSSID*********"
#define WIFI_PASSWORD  "************yourPassword*********"

// Create an ESPduino object, named wifi with serial debugging on
ESPduino wifi(ESP_RXD, ESP_TXD, ESP_ENB, true);

// Main Setup function
void setup() {

  // Start hardware serial debugging
  Serial.begin(9600);
  Serial.println("ESP8266 GET Demo");

  // Delay for Serial output to complete before we Sleep
  delay(250);  
  
  // WiFi settings
  wifi.setup(WIFI_SSID, WIFI_PASSWORD);
}

// Main Loop function
void loop() {

  // http GET variables
  char ip[] = "10.1.1.1";
  char uri[] = "/this/is/a/get";
  int httpResponseCode;
  char httpResponseData[256];

  if(wifi.get(ip, uri)){ //, &httpResponseCode, httpResponseData)) {

    Serial.println("GET ok.");

  } else {

    Serial.println("GET failed!");
  }

  // Output Debug information
  Serial.print("httpResponseCode: ");
  Serial.println(httpResponseCode);
  Serial.println("httpResponseData: ");
  Serial.println(httpResponseData);


  // Wait 10 seconds before looping
  delay(10000);
} 
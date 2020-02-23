/*
  Making an ESP8266 module the access point(hotspot)
  SERVER
  default IP address of our ESP AP: 192.168.4.1 
*/
#include <ESP8266WiFi.h>

#define   BUTTON_PIN    4
#define   LED_STRIP     2
#define   NUMPIXELS     53 

#define WIFI_NAME  "pressurePlate"
#define WIFI_PW    "play-a-game-with-me"

void setup()
{
  Serial.begin(115200);
  Serial.println();

  Serial.print("Setting soft-AP ... ");
  boolean result = WiFi.softAP(WIFI_NAME, WIFI_PW);
  if(result == true)
  {
    Serial.println("Ready");
  }
  else
  {
    Serial.println("Failed!");
  }
}

void loop()
{
  Serial.printf("Stations connected = %d\n", WiFi.softAPgetStationNum());
  delay(3000);
}

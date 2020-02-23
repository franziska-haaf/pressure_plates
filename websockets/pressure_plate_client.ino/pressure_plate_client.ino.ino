/*
  Making an ESP8266 module access the server
  CLIENT
*/
#include <ESP8266WiFi.h>

#define   BUTTON_PIN    4
#define   LED_STRIP     2
#define   NUMPIXELS     53 

#define WIFI_NAME  "pressurePlate"
#define WIFI_PW    "play-a-game-with-me"

void setup() {
  Serial.begin(9600);         // Start the Serial communication to send messages to the computer
  delay(10);
  Serial.println('\n');
  
  WiFi.begin(WIFI_NAME, WIFI_PW);             // Connect to the network
  Serial.print("Connecting to ");
  Serial.print(WIFI_NAME); Serial.println(" ...");

  int i = 0;
  while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
    delay(1000);
    Serial.print(++i); Serial.print(' ');
  }

  Serial.println('\n');
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());         // Send the IP address of the ESP8266 to the computer
}

void loop() { 
  
  }

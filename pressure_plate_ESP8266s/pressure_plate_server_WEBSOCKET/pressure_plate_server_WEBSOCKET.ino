/*
 * WebSocketServer.ino
 *
 *  Created on: 22.05.2015
 *
 */

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsServer.h>
#include <Adafruit_NeoPixel.h>

#define   BUTTON_PIN    4
#define   LED_STRIP     2
#define   NUMPIXELS     53 

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, LED_STRIP, NEO_GRB + NEO_KHZ800);

#define USE_SERIAL  Serial1
#define WIFI        "Vogsphere"
#define WIFI_PW     "soschnellwirdmanzumvogonen"

ESP8266WiFiMulti WiFiMulti;
WebSocketsServer webSocket = WebSocketsServer(81);

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status
int colorState = 0;

uint32_t pink = strip.Color(255, 0, 234);
uint32_t orange = strip.Color(255, 162, 0);
uint32_t turquoise = strip.Color(0, 255, 145);
uint32_t red = strip.Color(255, 0, 0);
uint32_t green = strip.Color(0, 255, 0);
uint32_t blue = strip.Color(0, 0, 255);
uint32_t yellow = strip.Color(255, 255, 0);
uint32_t colors [7] = {pink, orange, turquoise, red, green, blue, yellow};

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

    switch(type) {
        case WStype_DISCONNECTED:
            USE_SERIAL.printf("[%u] Disconnected!\n", num);
            break;
        case WStype_CONNECTED:
            {
                IPAddress ip = webSocket.remoteIP(num);
                USE_SERIAL.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        
        // send message to client
        webSocket.sendTXT(num, "Connected");
            }
            break;
        case WStype_TEXT:
            USE_SERIAL.printf("[%u] get Text: %s\n", num, payload);

            // send message to client
            // webSocket.sendTXT(num, "message here");

            // send data to all connected clients
            // webSocket.broadcastTXT("message here");
            break;
        case WStype_BIN:
            USE_SERIAL.printf("[%u] get binary length: %u\n", num, length);
            hexdump(payload, length);

            // send message to client
            // webSocket.sendBIN(num, payload, length);
            break;
    }

}

void setup() {
    USE_SERIAL.begin(115200);
    strip.begin();
    strip.show();
    strip.setBrightness(150); // Set BRIGHTNESS (max = 255)
    changeColor();
    pinMode(BUTTON_PIN, INPUT_PULLUP);  

    //Serial.setDebugOutput(true);
    USE_SERIAL.setDebugOutput(true);

    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

    for(uint8_t t = 4; t > 0; t--) {
        USE_SERIAL.printf("[SETUP] BOOT WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }

    WiFiMulti.addAP(WIFI, WIFI_PW);

    while(WiFiMulti.run() != WL_CONNECTED) {
        delay(100);
    }

    webSocket.begin();
    webSocket.onEvent(webSocketEvent); 

    webSocket.broadcastTXT("Pressureplate Server");
}

void loop() {
    webSocket.loop();

    // read the state of the pushbutton value:
    buttonState = digitalRead(BUTTON_PIN);
  
    if (buttonState == LOW) {   
      // turn LED on:   
      USE_SERIAL.printf("Color change");
      webSocket.broadcastTXT("My color got changed!");
      changeColor();
      }
       else {
    }
    delay(1000);
}

void changeColor(){
  colorState++;
  if(colorState >= 7 ){
    colorState = 0;
  }
  
   strip.fill( colors[colorState], 0, strip.numPixels() - 1);
   strip.show();
}

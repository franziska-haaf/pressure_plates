/*
   WebSocketClient.ino

    Created on: 24.05.2015

*/

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <Adafruit_NeoPixel.h>
#include <WebSocketsClient.h>

#include <Hash.h>

#define   BUTTON_PIN    4
#define   LED_STRIP     2
#define   NUMPIXELS     53

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, LED_STRIP, NEO_GRB + NEO_KHZ800);

#define USE_SERIAL       Serial1
#define WIFI            "Vogsphere"
#define WIFI_PW         "soschnellwirdmanzumvogonen"
#define PORT            81

ESP8266WiFiMulti WiFiMulti;
WebSocketsClient webSocket;

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

char serverAdress = "192.168.0.123"; //todo where is this coming from?

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {

  switch (type) {
    case WStype_DISCONNECTED:
      USE_SERIAL.printf("[WSc] Disconnected!\n");
      break;
    case WStype_CONNECTED: {
        USE_SERIAL.printf("[WSc] Connected to url: %s\n", payload);

        // send message to server when Connected
        webSocket.sendTXT("Connected");
      }
      break;
    case WStype_TEXT:
      USE_SERIAL.printf("[WSc] get text: %s\n", payload);

      // send message to server
      // webSocket.sendTXT("message here");
      break;
    case WStype_BIN:
      USE_SERIAL.printf("[WSc] get binary length: %u\n", length);
      hexdump(payload, length);

      // send data to server
      // webSocket.sendBIN(payload, length);
      break;
  }

}

void setup() {
  // USE_SERIAL.begin(921600);
  USE_SERIAL.begin(115200);

  strip.begin();
  strip.show();
  strip.setBrightness(150); // Set BRIGHTNESS to about 1/5 (max = 255)
  changeColor();
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  //Serial.setDebugOutput(true);
  USE_SERIAL.setDebugOutput(true);

  USE_SERIAL.println();
  USE_SERIAL.println();
  USE_SERIAL.println();

  for (uint8_t t = 4; t > 0; t--) {
    USE_SERIAL.printf("[SETUP] BOOT WAIT %d...\n", t);
    USE_SERIAL.flush();
    delay(1000);
  }

  WiFiMulti.addAP(WIFI, WIFI_PW);

  //WiFi.disconnect();
  while (WiFiMulti.run() != WL_CONNECTED) {
    delay(100);
  }

  // server address, port and URL
  webSocket.begin(serverAdress, PORT, "/");

  // event handler
  webSocket.onEvent(webSocketEvent);

  // use HTTP Basic Authorization this is optional remove if not needed
  // webSocket.setAuthorization("user", "Password");

  // try ever 5000 again if connection has failed
  webSocket.setReconnectInterval(5000);

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

void changeColor() {
  colorState++;
  if (colorState >= 7 ) {
    colorState = 0;
  }

  strip.fill( colors[colorState], 0, strip.numPixels() - 1);
  strip.show();
}

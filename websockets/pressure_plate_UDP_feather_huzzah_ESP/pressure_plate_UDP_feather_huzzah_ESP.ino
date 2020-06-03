/*
   Code for Pressure Plate Nr. 01 (the e-paper ESP8266)

   Send UDP packages via terminal:
   echo -n "hello" >/dev/udp/192.168.179.23/4210 (or any other IP that the ESP has)
   --> echo -n "hello" >/dev/udp/IP/PORT
*/

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Adafruit_NeoPixel.h>
#include <time.h>

#define SERIAL_BAUD_NUM   115200

#define   BUTTON_PIN          4
#define   LED_STRIP           5
#define   NUMPIXELS           47 // 48 for plate 01, 47 for plate 02
#define   NUMPIXELS_COUNTER   5
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS + NUMPIXELS_COUNTER, LED_STRIP, NEO_GRB + NEO_KHZ800);

int buttonState = 0;         // variable for buttonReading the pushbutton status

uint32_t black = strip.Color(0, 0, 0);
uint32_t white = strip.Color(255, 255, 255);

uint32_t pink = strip.Color(255, 0, 234);           // 0
uint32_t orange = strip.Color(255, 162, 0);         // 1
uint32_t turquoise = strip.Color(0, 255, 145);      // 2
uint32_t red = strip.Color(255, 0, 0);              // 3
uint32_t green = strip.Color(0, 255, 0);            // 4
uint32_t blue = strip.Color(0, 0, 255);             // 5
uint32_t yellow = strip.Color(255, 255, 0);         // 6
uint32_t colors [4] = {pink, green, blue, yellow};
const int amountOfColors = 4;
int currentColor = 0; //can be: 0 - 4

WiFiUDP Udp;
unsigned int localUdpPort = 4210;  // local port to listen on
char incomingPacket[255];  // buffer for incoming packets

#include "wifiAccessData.h"
const char* usedIP = otherESPIP;

time_t lastTimeStepped;

int lastButtonState = 0;
int lastDebounceTime = 0;
int debounceDelay = 20;

void setup() {
  Serial.begin(SERIAL_BAUD_NUM);

  strip.begin();
  strip.show();
  strip.setBrightness(50); //150 Set BRIGHTNESS to about 1/5 (max = 255)
  setToRandomColor();
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  Serial.println();

  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");

  Udp.begin(localUdpPort);
  Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);
  Serial.print("chipId: ");
  Serial.println(ESP.getChipId()); 

  // Initialize Time
  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");
}

void loop() {
  rotateColors();
  lighCounterLEDs();

  //Button reading with debounce mechanism
  int buttonReading = digitalRead(BUTTON_PIN);
  if (buttonReading != lastButtonState) {
    lastDebounceTime = millis();// reset the debouncing timer
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (buttonReading != buttonState) { // if the button state has changed:
      buttonState = buttonReading;
      plateGotActivated();
    }
  } 
  lastButtonState = buttonReading; // save the buttonReading

  receivePackage();
}

void receivePackage() {
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    // receive incoming UDP packets
    Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int len = Udp.read(incomingPacket, 255);
    if (len > 0) {
      incomingPacket[len] = 0;
    }
    //------ Read content of package
    Serial.printf("received %s\n", incomingPacket);
    if ((strcmp(incomingPacket, "0") == 0)) {
      decodeBooleanPackage();
    }
    else if((strcmp(incomingPacket, "1") == 0)){
      decodeBooleanPackage();
      }
    else if ((strcmp(incomingPacket, "3") == 0)) { //game over flag = 3
      Serial.printf("game over");
      resetWinningCounter();
      looserLights();
    }
    else if ((strcmp(incomingPacket, "4") == 0)) { //reset flag = 4
      Serial.printf("reset");
      resetWinningCounter();
    }
    else {
      decodeColorAndTimestampPackage();
    }
  }
}

/**
   Add a win to the counter

   Check if the counter reached a 5.
   If yes
   - notify the other plate
   - play the winning sequence
   - reset to 0

   If no
   - add a number to the counter
*/
int winningCounter = 0;
int winningCounterMax = 5;
void handleWinningCounter() {
  if (winningCounter == winningCounterMax) {
    lighCounterLEDs();
    winnerLights();
    sendOtherPlateGameOver();
    resetWinningCounter();
  }
  Serial.print("counter: " + winningCounter);
  Serial.println(winningCounter);
}

void resetWinningCounter() {
  winningCounter = 0;
  strip.fill(black, NUMPIXELS-1, NUMPIXELS_COUNTER);
  strip.show();
}

void lighCounterLEDs() {
  if (winningCounter > 0) {
    strip.fill(white, NUMPIXELS-1, winningCounter);
    strip.show();
  }
}

void plateGotActivated() {
  if (buttonState == HIGH) {
    lastTimeStepped = time(nullptr);
    Serial.println(lastTimeStepped);

    sendTimestampAndColorToOtherPlate(lastTimeStepped);
  }
}

void decodeBooleanPackage() {
  if ((strcmp(incomingPacket, "1") == 0)) {
    winningCounter++;
    handleWinningCounter();
  }
  else if ((strcmp(incomingPacket, "0") == 0)) {
    winningCounter--;
    handleWinningCounter();
  }
}

void decodeColorAndTimestampPackage() {
  //------ Split into color and timestamp
  //[0]        color
  //[1]-[10]   timestamp
  char receivedColorString = incomingPacket[0];
  int receivedColor = receivedColorString - 48;
  Serial.printf("received color %d\n", receivedColor);

  char receivedTimestampString [10];
  for (int j = 1; j <= 10; j++) {
    receivedTimestampString[j - 1] = incomingPacket[j];
  }
  char *bufferString;
  time_t receivedTimestamp = strtoul(receivedTimestampString, &bufferString, 10);
  Serial.printf("received timestamp %ld\n", receivedTimestamp);

  //------ Check color
  if (receivedColor == currentColor) {
    if (checkIfMyTimestampIsEarlier(receivedTimestamp)) {
      sendOtherPlateItLost();
      winningCounter++;
      handleWinningCounter();
    }
    else {
      sendOtherPlateItWon();
      winningCounter--;
      handleWinningCounter();
    }
  }
  else {
    sendOtherPlateItLost();
    winningCounter++;
    handleWinningCounter();
  }
}

bool checkIfMyTimestampIsEarlier(time_t receivedTimestamp) {
  Serial.printf("comparing timestamps %ld and %ld\n", lastTimeStepped, receivedTimestamp);
  double diffedTime = difftime(lastTimeStepped, receivedTimestamp);
  Serial.print("time diff:");
  Serial.println(diffedTime);
  return diffedTime > 0;
}

void sendOtherPlateItWon() {
  Udp.beginPacket(usedIP, localUdpPort);
  Udp.write("1");
  Udp.endPacket();
}

void sendOtherPlateItLost() {
  Udp.beginPacket(usedIP, localUdpPort);
  Udp.write("0");
  Udp.endPacket();
}

void sendOtherPlateGameOver() {
  Udp.beginPacket(usedIP, localUdpPort);
  Udp.write("3");
  Udp.endPacket();
}

void sendTimestampToOtherPlate(time_t timestamp) {
  //time_t value    long int    1583315675
  char timestampBuffer [15];
  snprintf (timestampBuffer, 15, "%ld", timestamp);
  Serial.printf("send %s\n", timestampBuffer);

  Udp.beginPacket(usedIP, localUdpPort);
  Udp.write(timestampBuffer);
  Udp.endPacket();
}

void sendTimestampAndColorToOtherPlate(time_t timestamp) {
  //time_t value    long int    1583315675
  char timestampBuffer [15];
  snprintf (timestampBuffer, 15, "%ld", timestamp);
  //create char of timestamp + current color
  char packageToSend[15];
  sprintf(packageToSend, "%d%s", currentColor, timestampBuffer);

  Serial.println("send package:");
  Serial.println(packageToSend);
  Serial.println("to IP:");
  Serial.println(usedIP);

  Udp.beginPacket(usedIP, localUdpPort);
  Udp.write(packageToSend);
  Udp.endPacket();
}

/**
     Rotate all the colors every x seconds.
     Using millis(), this won' stop the program
*/
int waitTime = 5000; //5 seconds
unsigned long timeNow = 0;
void rotateColors() {
  if ((unsigned long)(millis() - timeNow) > waitTime) {
    timeNow = millis();
    setToRandomColor();
  }
}

int resetTime = 30000; //30 seconds
void sendOtherPlateReset() {
  if ((unsigned long)(millis() - lastTimeStepped) > resetTime) {
    Udp.beginPacket(usedIP, localUdpPort);
    Udp.write("4");
    Udp.endPacket();
  }
  resetWinningCounter();
}

void setToRandomColor() {
  int randNumber = random(0, amountOfColors);
  strip.fill( colors[randNumber], 0, NUMPIXELS - 1);
  currentColor = randNumber;
  strip.show();
}

void winnerLights() {
  for (int rounds = 5; rounds >= 0; rounds--) {
    for (int i = 0; i <= amountOfColors; i++) {
      strip.fill( colors[i], 0, NUMPIXELS - 1);
      strip.show();
      delay(50);
    }
  }
  strip.fill( colors[currentColor], 0, NUMPIXELS - 1);
  strip.show();
}

void looserLights() {
  strip.fill(red, 0, NUMPIXELS - 1);
  strip.show();
  delay(10);
  for (int i = 255; i >= 0; i--) {
    strip.fill( strip.Color(i, 0, 0), 0, NUMPIXELS - 1);
    strip.show();
    delay(5);
  }
  strip.fill( colors[currentColor], 0, NUMPIXELS - 1);
  strip.show();
}

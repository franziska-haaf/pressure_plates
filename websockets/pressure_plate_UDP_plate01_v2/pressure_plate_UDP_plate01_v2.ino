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

#define OTHER_PLATE_IP    "192.168.179.24"
#define SERIAL_BAUD_NUM   74880

#define   BUTTON_PIN    4
#define   LED_STRIP     2
#define   NUMPIXELS     53

#define STEPPED  "S"

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, LED_STRIP, NEO_GRB + NEO_KHZ800);

int buttonState = 0;         // variable for reading the pushbutton status

uint32_t black = strip.Color(0, 0, 0);

uint32_t pink = strip.Color(255, 0, 234);
uint32_t orange = strip.Color(255, 162, 0);
uint32_t turquoise = strip.Color(0, 255, 145);
uint32_t red = strip.Color(255, 0, 0);
uint32_t green = strip.Color(0, 255, 0);
uint32_t blue = strip.Color(0, 0, 255);
uint32_t yellow = strip.Color(255, 255, 0);
uint32_t colors [7] = {pink, orange, turquoise, red, green, blue, yellow};

const char* ssid = "OnePlus 5T";
const char* password = "TeddyIstBraun";

WiFiUDP Udp;
unsigned int localUdpPort = 4210;  // local port to listen on
char incomingPacket[255];  // buffer for incoming packets
char replyPacket[] = "Hi there! Got the message :-)";  // a reply string to send back

uint32_t lastSteppedColor;
char lastSteppedTimestamp [7];

void setup() {
  Serial.begin(SERIAL_BAUD_NUM);

  strip.begin();
  strip.show();
  strip.setBrightness(150); // Set BRIGHTNESS to about 1/5 (max = 255)
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

  // Initialize Time
  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");
}

void loop() {
  rotateColors();


  // read the state of the pushbutton value:
  buttonState = digitalRead(BUTTON_PIN);
  //-----------------------------GOT STEPPED ON: SEND PACKAGE
  if (buttonState == LOW) {
    Serial.println("Got stepped on");
    //------Save Timestamp
    //Get the current timestamp
    time_t now;
    struct tm * timeinfo;
    time(&now);
    timeinfo = localtime(&now);
    char timestamp [7]; //problem: the hour/min/sec can be 1-9 aswell, making the timestamp [] shorter!
    sprintf(timestamp, "%ld%ld%ld\0", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
    Serial.println("Timestamp:");
    Serial.println(timestamp);
    Serial.println("Old Timestamp:");
    Serial.println(lastSteppedTimestamp);

    boolean nowIsLaterThenBefore = calculateTimeDifference(timestamp,lastSteppedTimestamp);

    //Copy new timestamp into old one
    char * ptrtimestamp = timestamp;
    char * ptrtimestampOld = lastSteppedTimestamp;
    while (*ptrtimestampOld++ = *ptrtimestamp++);
   
    //------ Tell other plate
    sendPackage(now);
    //todo rest of code
  }

  delay(1000);

  //-----------------------------OTHER ONE GOT STEPPED ON: RECEIVE PACKAGES
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    // receive incoming UDP packets
    Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int len = Udp.read(incomingPacket, 255);
    if (len > 0) {
      incomingPacket[len] = 0;
    }
    Serial.printf("UDP packet contents: %s\n", incomingPacket);

    // send back a reply, to the IP address and port we got the packet from
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(replyPacket);
    Udp.endPacket();
  }
}

/**
   Calculate the difference of two char timestamps.

   true     firstTimeStamp is bigger then secondTimeStamp
   false    secondTimeStamp is bigger then firstTimeStamp
*/
boolean calculateTimeDifference(char firstTimeStamp [], char secondTimeStamp []) {
  int firstTimestampInt, secondTimestampInt;
  sscanf(firstTimeStamp, "%d", &firstTimestampInt);
  sscanf(secondTimeStamp, "%d", &secondTimestampInt);
  Serial.println("firstTimeStamp > secondTimeStamp:");
  Serial.println(firstTimestampInt > secondTimestampInt);
  return firstTimestampInt > secondTimestampInt;
}

void checkMessage(char package) {
  //if (package.startsWith(STEPPED)) {
  //get the timeout out of the message
  //myString.substring(from, to)

  // }
}

/**
   Send a the handed text to the other plate
*/
void sendPackage(time_t time) {
  Udp.beginPacket(OTHER_PLATE_IP, localUdpPort);
  Udp.write("Stepped");
  Udp.endPacket();
}

/**
     Rotate all the colors every 30 seconds.
     Using millis(), this won' stop the program
*/
int waitTime = 30000; //30 seconds
unsigned long timeNow = 0;
void rotateColors() {
  if ((unsigned long)(millis() - timeNow) > waitTime) {
    timeNow = millis();
    Serial.println("Change color");
    setToRandomColor();
  }
}

void setToRandomColor() {
  int randNumber = random(0, 7);
  strip.fill( colors[randNumber], 0, strip.numPixels() - 1);
  strip.show();
}

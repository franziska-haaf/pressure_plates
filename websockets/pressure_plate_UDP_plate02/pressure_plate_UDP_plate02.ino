/*
 * Code for Pressure Plate Nr. 02 (the feather ESP8266)
 * 
 * Send UDP packages via terminal:
 * echo -n "hello" >/dev/udp/192.168.179.23/4210 (or any other IP that the ESP has)
 * --> echo -n "hello" >/dev/udp/IP/PORT
 */

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#define OTHER_PLATE_IP  "192.168.179.22"

const char* ssid = "OnePlus 5T";
const char* password = "TeddyIstBraun";

WiFiUDP Udp;
unsigned int localUdpPort = 4210;  // local port to listen on
char incomingPacket[255];  // buffer for incoming packets
char replyPacket[] = "Hi there! Got the message :-)";  // a reply string to send back


void setup(){
  Serial.begin(115200);
  Serial.println();

  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");

  Udp.begin(localUdpPort);
  Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);
}


void loop(){

    Udp.beginPacket("192.168.43.101", 4210); //todo ip adress
    Udp.write("Hey you!");
    Udp.endPacket();
  
}

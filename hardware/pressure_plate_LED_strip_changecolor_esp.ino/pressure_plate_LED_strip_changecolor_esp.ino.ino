//The ESP got some other pins..

#include <Adafruit_NeoPixel.h>

#define   BUTTON_PIN    4
#define   LED_STRIP     2
#define   NUMPIXELS     53 //stimmt?

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, LED_STRIP, NEO_GRB + NEO_KHZ800);

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

void setup() {
  Serial.begin(9600);
  strip.begin();
  strip.show();
  strip.setBrightness(150); // Set BRIGHTNESS to about 1/5 (max = 255)
  changeColor();
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  // read the state of the pushbutton value:
  buttonState = digitalRead(BUTTON_PIN);

  if (buttonState == LOW) {
    // turn LED on:
    Serial.println("Color change");
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

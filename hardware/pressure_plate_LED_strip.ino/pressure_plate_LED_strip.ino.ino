#include <Adafruit_NeoPixel.h>

#define BUTTON_PIN  8
#define LED_PIN     2
#define NUMPIXELS   8

Adafruit_NeoPixel ledStripe = Adafruit_NeoPixel(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

int buttonState = 0;         
int colorState = 0;

uint32_t pink = ledStripe.Color(255, 0, 234);
uint32_t orange = ledStripe.Color(255, 162, 0);
uint32_t turquoise = ledStripe.Color(0, 255, 145);
uint32_t red = ledStripe.Color(255, 0, 0);
uint32_t green = ledStripe.Color(0, 255, 0);
uint32_t blue = ledStripe.Color(0, 0, 255);
uint32_t yellow = ledStripe.Color(255, 255, 0);
uint32_t colors [7] = {pink, orange, turquoise, red, green, blue, yellow};

void setup() {
  Serial.begin(9600);

  ledStripe.begin();
   
  ledStripe.show();
  pinMode(LED_PIN, OUTPUT);     
  pinMode(BUTTON_PIN, INPUT_PULLUP);    
}

void loop(){
  // read the state of the pushbutton value:
  buttonState = digitalRead(BUTTON_PIN);
  
  // check if the pushbutton is pressed.
  // if it is, the buttonState is LOW:
  if (buttonState == LOW) {   
    // turn LED on:   
    Serial.println("Color change");
    changeColor();
  }
  delay(1000);
}

void changeColor(){
  colorState++;
  if(colorState >= 7 ){
    colorState = 0;
  }
  
   ledStripe.fill( colors[colorState] , 0, ledStripe.numPixels() - 1);
   ledStripe.show();
}

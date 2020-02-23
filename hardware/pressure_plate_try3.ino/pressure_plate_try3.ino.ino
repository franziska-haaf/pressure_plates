//Here is an example of code that you could use with this switch.
const int buttonPin = 8;     // the number of the pushbutton pin
const int ledPin =  2;      // the number of the LED pin

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);     
  pinMode(buttonPin, INPUT_PULLUP);    
}

void loop(){
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  // check if the pushbutton is pressed.
  // if it is, the buttonState is LOW:
  if (buttonState == LOW) {    
    // turn LED on:   
    Serial.println("LED on");
    digitalWrite(ledPin, HIGH); 
  }
  else {
    // turn LED off:
    Serial.println("LED off");
    digitalWrite(ledPin, LOW);
  }
}

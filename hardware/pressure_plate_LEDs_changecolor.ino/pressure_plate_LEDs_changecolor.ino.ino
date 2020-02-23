//Here is an example of code that you could use with this switch.
const int buttonPin = 8;     // the number of the pushbutton pin
const int ledPin1 =  3;      // the number of the LED pin
const int ledPin2 =  4;      // the number of the LED pin


// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status
int ledToShine = 1;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin1, OUTPUT);     
  pinMode(ledPin2, OUTPUT);     
  pinMode(buttonPin, INPUT_PULLUP);    
}

void loop(){
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  // check if the pushbutton is pressed.
  if (buttonState == LOW) {  
    delay(500);
    if(buttonState == LOW){
      //still low?
       if(ledToShine == 1){
       ledToShine++;
      }
      else{
       ledToShine--;
      }
    }
    
  }
  else {
     if(ledToShine == 1){
      digitalWrite(ledPin1, HIGH);
      digitalWrite(ledPin2, LOW);
      }
     if(ledToShine == 2){
      digitalWrite(ledPin1, LOW);
      digitalWrite(ledPin2, HIGH);
     }
  }
}

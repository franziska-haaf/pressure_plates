#include <EEPROM.h> //Internal space

#define PRESSURE_PLATE_PIN    8
#define LED_PIN_1             3
#define LED_PIN_2             4

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status
int ledToShine = 1;
int EEPROMadress = 0;       

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN_1, OUTPUT);     
  pinMode(LED_PIN_2, OUTPUT);     
  pinMode(PRESSURE_PLATE_PIN, INPUT_PULLUP);  
  
  int pressedAmount = EEPROM.read(0);

  Serial.print(pressedAmount);
  Serial.print("\n");
}

void loop(){
  // read the state of the pushbutton value:
  buttonState = digitalRead(PRESSURE_PLATE_PIN);

  // check if the pushbutton is pressed.
  if (buttonState == LOW) {  
    delay(500); //evtl ein count stattdessen?
    if(buttonState == LOW){
      //still low?
       if(ledToShine == 1){
       ledToShine++;
       //Write to EEPROM
       int pressedAmount = EEPROM.read(0);
       EEPROM.write(0, pressedAmount + 1);
      }
      else{
       ledToShine--;
      }
    }
    
  }
  else {
     if(ledToShine == 1){
      digitalWrite(LED_PIN_1, HIGH);
      digitalWrite(LED_PIN_2, LOW);
      }
     if(ledToShine == 2){
      digitalWrite(LED_PIN_1, LOW);
      digitalWrite(LED_PIN_2, HIGH);
     }
  }
}

void countPressed(){
  int pressedAmount = EEPROM.read(EEPROMadress);
  Serial.print("pressedAmount");
  
  //If the data in the EEPROM address is 255, meaning an overflow will happening, start to use a new address
  if(pressedAmount >= 255){
     EEPROMadress = EEPROMadress++;
    if (EEPROMadress == EEPROM.length()) {
      EEPROMadress = 0;
      Serial.print("EEPROM is full");
   }
  }
  else{
    EEPROM.write(EEPROMadress, pressedAmount + 1);
  }
}

 void readPressed(){
  Serial.print("Read: " + EEPROMadress);
  int pressedAmount = 0;
   for(int i = 0; i <= EEPROMadress; i++){
    pressedAmount = EEPROM.read(i);
    Serial.print("Address " + EEPROMadress);
    Serial.print(pressedAmount); 
   }
 }

#define SERIAL_BAUD_NUM   115200
#define   BUTTON_PIN    4

int buttonState = 0;        

void setup() {
  Serial.begin(SERIAL_BAUD_NUM);

  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  int sensorValue = digitalRead(BUTTON_PIN);
  Serial.println(sensorValue);
}

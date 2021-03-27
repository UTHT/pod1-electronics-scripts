int analogPin = A0;
float voltage, current;
int resistance = 100;

void setup() {
  Serial.begin(19200);
}

void loop() {
  voltage = analogRead(analogPin);
  voltage = voltage*5/1023.0;
  current = voltage / resistance;
  
  Serial.println(current);  
  delay(100);
}

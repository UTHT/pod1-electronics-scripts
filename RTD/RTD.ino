//For use with 220ohm load
void setup() {
  Serial.begin (9600);
  pinMode(A0, INPUT);

}

void loop() {
 int temperature =  map(analogRead (A0),180, 901, -40, 300);
 Serial.print (temperature);
 delay (500);
}

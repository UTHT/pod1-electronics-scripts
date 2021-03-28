 const int sensorPin = A0;    // select the input pin for the pressure sensor
int sensorValue = 0;  // variable to store the value coming from the sensor

void setup() {
  pinMode(A0, INPUT);
  Serial.begin(115200);
}

double map(const int x, const double in_min, const double in_max, const double out_min, const double out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void loop() {
  // read the value from the sensor:
  sensorValue = analogRead(sensorPin);
  
  // TODO Calibrate from 0 psi to 200 psi
  // Currently sensorValue at 0 psi is 176 - 177
  Serial.print(sensorValue);
  Serial.print('\t');
  Serial.println(map(sensorValue, 176.0, 1023.0, 0, 200.0), 5);
  
}

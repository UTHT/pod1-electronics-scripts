#define pulse_pin 2
int ontime,offtime,duty;
float freq,period;

void setup() {
  Serial.begin(9600);                    // connect to the serial port
  Serial.println("Frequency Counter");
  pinMode(pulse_pin,INPUT);
}

float map_float(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void loop() {
  ontime = pulseIn(pulse_pin,HIGH);
  offtime = pulseIn(pulse_pin,LOW);
  period = ontime+offtime;
  freq = 1000000.0/period;
  duty = (ontime/period)*100;
  float flow =  map_float(freq,0, 100, 0.26, 5.28);  //measured in gallons per minute(gpm)
  if(flow < 0.27) {
    Serial.println('Flow is at or below minimum value of 0.26 gpm');
  }
  else {
    Serial.println(flow);
  }
  delay(200);
}

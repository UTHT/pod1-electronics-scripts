//Wenglor Photoelectric OPT2002 with Arduino
//test code from Alex Zeng

int analogPin = A3;		//measurements (6; analog output)
int error_pin = 11;		//Error Output Message (1; Contamination/Error output(NO))

//this is a weird one, I am not quite sure how this works
int ELD_pin = 10;		//some sorta control(5; Emitted Light disengageable)

float multiplier = 2;	// !!! this value depends on the resistor
float distance = 0;		//output distance
float counts = 0;
float volts = 0;
const float voltRF = 5; 	//we need to calibrate the Reference of the ADC based of our Arduino, more details in the Doc

String error = "";

void setup()
{  
  Serial.begin(115200);           // !!! setup serial, we will need to decide on a baud
  
  //set up pins
  //&&& pinMode(ELD_pin, INPUT);
  pinMode(error_pin, OUTPUT);
  pinMode(analogPin, OUTPUT);
}

void loop()
{
  //&&& digitalWrite(ELD_pin,LOW);
  digitalWrite(error_pin,LOW);
  
  //turn on the sensors
  //&&& digitalWrite(ELD_pin,HIGH);
  digitalWrite(error_pin,HIGH);
  
  error = String(digitalRead(error_pin));		//read if there is any errors
  
  //recieve the messurement from the sensor (math base off the Characteristic Curves)
  counts = analogRead(analogPin);
  volts = counts * voltRF;
  distance = 30 + 5 * (volts * multiplier);		
  
  //presenting the information
  Serial.println(error);
  Serial.println(distance);		//in mm
  delay(10);
  
  //storing the height in file for future analysis
}


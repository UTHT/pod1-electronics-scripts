# UTHT Electronics Scripts

Libraries and test sketches for the various electronics (sensors, etc.).

# Sensors

## SPTD25-20-1000H

Monitors the pressure of the braking exhaust valves, alerting the team in case of any irregularity. 

Outputs: Pressure (psig), range: 0 to 1000 psig

## SPT25-20-0200A

Monitors the pressure of the braking exhaust valves, alerting the team in case of any irregularity. 

Outputs: Pressure (psi), range: 0 to 200 psi

## RTD

Measures the temperatures near a) the front wheel, b) the center of the pod, and c) the rear wheel.

Outputs: Temperature (Celcius), range: -40 to 150 C

## TMP006

Measures the temperature around the center of the pod. Sensor is intended to be used in auxiliary, to obtain diagnostic data for pod improvement.

Outputs:
- Die temperature (Celcius), range: unspecified in official documentation
- Object temperature (Celcius) (calculated from die temp and die voltage), range: -40 to 125 C

## OPT2002

Measures the distancee between the bottom of the pod’s frame to the top of the track I-beam. Used to inform emergency stopping procedures.

Outputs:
- Distance [mm], range: 30[mm] to 80[mm]
- An indication if there exists an error(or if it cannot detect an object within the range) 

## VFS500-5-1001

Monitors the flowrate of the watercooling loop.

Outputs:
- Frequency (Hz)
- Flow rate (GPM), range: 0.26 to 5.28 GPM

## MPU9250

9-axis accelerometer, gyroscope, and magnetic field sensor. Sensor is intended to be used in auxiliary, obtained data for pod improvement.

Outputs:
- Acceleration X, Y, Z (milli-gees), range: ±2000 mg (default, configurable to up to +-16000 mg)
- Gyro values X, Y, Z (deg/sec), range: ±250 deg/sec (default, configurable to up to +-2000 deg/sec)
- Magnetic values X, Y, Z (milli-Gauss), range: ±48000 mG
- Sensor temperature (C), range: -40 to 85 C

# Actuators

## Brakes

Engages brakes via digital output control of a MOSFET driving a solenoid (NORMALLY OPEN, i.e. digital low = open solenoid = brakes engaged). Once engaged, brakes do not disengage. Fails safe (to engaged).
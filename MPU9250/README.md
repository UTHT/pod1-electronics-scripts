# MPU9250

## Sensor Information

### Sensor Output (TODO: add return value ranges)
- Acceleration (x,y,z) [milli-g]
- Gyro values (x,y,z) [deg/sec]
- Mag field values (x,y,z) [milliGauss/sec]
- Sensor temperature [Celsius]
- Vibration frequency of selected axis [mg/Hz]

### Sensor Use
Sensor is intended to be used in auxiliary, obtained data for pod improvement.
- Acceleration data measures relative location. Values also used for vibration analysis.
- Gyroscope data measures orientation
- Magnetic field data *currently unused*
- Sensor temperature monitors sensor health
- Vibration frequency measures the dominant vibrations experienced by the pod, data can be used to mitigate vibrations to improve the accuracy of the main IMU "Vector Nav VN200"

### Code Description
1. During setup, the sensor checks communication with the Arduino by reading the WHO_AM_I register. Sensor aborts if connection is not established.
2. Sensor performs self test to calibrate its return values. The manufacturer's calibration methods are used as documented in the MPU9250 Library.
3. Sensor stores accel, gyro, mag field values for x-, y-, z-axis in their respective registers using getData(), this occurs max 1000 times/sec.
4. If FFT is configured to enabled in "ENABLE_FFT", the accel datum for the selected axis is stored in an array "vReal" for vibration analysis.
5. Once the array is filled with enough samples, vibration analysis is performed through computeFFT(vReal) and stored in variable "peak"
6. Every 500ms the most recent accel, gyro, mag field, sensor temp, and FFT "peak" is printed through printData().

### Supporting Documentation
> Documentation for the **MPU9250** can be found [here](https://drive.google.com/drive/folders/1T1scCzBrcOk7fL052PqYYgnkAevgHrp9?usp=sharing).


## Todo

- [ ] Setup, testing, and expected output documentation (screenshot, pictures, logs/stack traces, etc.);
- [ ] A list of dependencies (and links to those);
- [ ] Everything else that is listed under [the master `README`](../README.md).

# Development

## Dependencies

- ...

## Testing

### Setup

...

### Procedure

...

### Expected Output

...

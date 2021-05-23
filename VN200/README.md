# VectorNav VN200

## Sensor Information

### Sensor Output
- Position (GPS)
- Rotation [°/s, (Euler angles)], range: -2000[°/s] to +2000[°/s]
- Velocity [m/s], range: 0[m/s] to 500[m/s]
- Acceleration [g], range: -16[g] to +16[g]
- Angular velocity [°/s, (Euler angles)]
- Magnetic field [Gauss], range: -2.5[Gauss] to +2.5[Gauss]
- Pressure [mbar], range: 10[mbar] to 1200[mbar]

### Sensor Use
- This sensor has an industrial-grade 9-axis GNSS and inertial navigation system; three-axes of each: accelerometer, gyro, and magnetometer, along with a pressure sensor and 32-bit processor. This will be used for navigation of the pod and will have an associated control system.

### Code Description
1. We are using Serial TTL (RX)  from Arduino Due UART (TX) to communicate with the sensor, alone with existing online libraries and softwares. 

> Documentation for the **VectorNav VN200** can be found [here](https://drive.google.com/drive/folders/1EaSJbGR74E2HPFfagwd0tmVs1x8UhGS6?usp=sharing).


## Todo
- [ ] Setup, testing, and expected output documentation (screenshot, pictures, logs/stack traces, etc.);
- [ ] A list of dependencies (and links to those);
- [ ] Everything else that is listed under [the master `README`](../README.md)

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

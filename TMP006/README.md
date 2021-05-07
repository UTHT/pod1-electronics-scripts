# TMP006

## Sensor Information

### Sensor Output
- Die temperature [Celsius], range: unspecified in official documentation
- Object temperature [Celsius] (calculated from die temp and die voltage), range: -40[C] to 125[C]

### Sensor Use
Sensor is intended to be used in auxiliary, to obtain data for pod improvement.
- Measure the temperature around the center of the pod

### Code Description
1. Sensor checks connection with Arduino
2. Returns the object temperature and die temperature according to the configured sampling rate
3. The sensor can be put to sleep or wake up through serial command

### Supporting Documentation
> Documentation for the **TMP006** can be found [here](https://drive.google.com/drive/folders/14W78F2MIxz7_K_xL1_n9KY-3X8qnph70?usp=sharing).


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

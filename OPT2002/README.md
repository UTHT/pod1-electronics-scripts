# Wenglor OPT2002

## Sensor Information

### Sensor Output
- Distance [mm], range: 30[mm] to 80[mm]
- An indication if there exists an error 

### Sensor Use
- The OPT2002 measures the gap between the bottom of the pod’s frame to the top of the I beam. There will be a control system associated with this sensor and its readings will be used to tune the pod and emergency stop the pod. Check the user manuel for how to use the sensor(turn it on/off) and what the LED means. 

### Code Description
1. Distance is calculated by proportionally mapping the analogy input from the arduino(4-20mA) over the distance range(30-80mm).
2. OPT2002 has a PNP output, turn positive when an error has occurred. The red F-LED will also lights up to indicate that the error output has been activated.
3. The code will indicate if there exist an error(or if it cannot detect an object within the range), if not, the calculated distance in mm. 


> Documentation for the **Wenglor OPT2002** can be found [here](https://drive.google.com/drive/folders/1m1LtClotZ9bE37aDwrzZTo6NHY5s_LML?usp=sharing), the user manuel can be found [here](https://cdn.automationdirect.com/static/manuals/lasersensorsmanual/opt2001-6.pdf) (refer to the information for the OPT2001).


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

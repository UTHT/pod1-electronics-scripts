# Find the drive link (with more details) here:
https://drive.google.com/drive/folders/1EaSJbGR74E2HPFfagwd0tmVs1x8UhGS6

# VN-200 Vector Nav INS (IMU) Sensor Summary

The VN-200 is a miniature, surface-mount, high-performance GPS-Aided
Inertial Navigation System (GPS/INS)

The VN-200 combines a set of 3-axis accelerometers, 
3-axis gyros, 3-axis magnetometer, a barometric pressure sensor, 
a 50-channel L1 GPS receiver, as well as a 32-bit
processor into a miniature surface-mount or standalone module.

## Getting Started

Software Information: 
    Desktop Software Download link: https://www.vectornav.com/support/downloads
    C/C++ Library Download link: https://www.vectornav.com/support/downloads

    https://forum.arduino.cc/index.php?topic=480057.0
    Sample project: https://github.com/bolderflight/VectorNav
    (SPI Communication ONLY, do not use if considering other forms of communication, i.e. TTL & USB)
    We will probably use USB
    FAQs: https://www.vectornav.com/support/faq/vn200-questions

### Prerequisites

    Link for the complete user manual - https://www.vectornav.com/docs/default-source/documentation/vn-200-documentation/vn-200-user-manual-(um004).pdf?sfvrsn=3a9ee6b9_35

    Interface and tech sheet link - 
    https://www.vectornav.com/docs/default-source/documentation/vn-200-documentation/PB-12-0003.pdf?sfvrsn=749ee6b9_15


### Limitations of the VN-200

    The VN-200 INS filter requires a GPS 3D fix in order to maintain specified accuracies over time.  If the GPS fix lost, the INS filter will continue to run; however, the position estimates will quickly degrade (on the order of 50 to 60 meters in a minute) as will the velocity estimates.  Without a GPS fix, the INS filter is essentially integrating noisy accelerometer measurements, which degrade in accuracy very quickly.  We typically recommend a maximum GPS outage of about 30 to 60 seconds without resetting the INS filter.
    The INS filter requires motion to maintain inertial alignment and will only be able to maintain the heading estimate for a short duration as the sensor platform comes to a rest.  The duration that the INS filter can maintain heading estimates within specification is typically about 3 minutes.  Prolonged stationary operation requires reliance on the magnetic measurements to bound the heading estimate errors. 

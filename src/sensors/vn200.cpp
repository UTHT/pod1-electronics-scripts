#include <sensors/vn200.h>

#include <Arduino.h>
#include "sensors.h"
#include "types.h"
#include "int.h"
#include "nocopy.h"
#include "packetfinder.h"
#include "export.h"
#include "registers.h"

#include <utils/base.h>
#include <sensors/sensor.h>

// Namespaces
using namespace vn::protocol::uart;
using namespace vn::xplat;

// Declarations
bool check_sync_byte(void);
unsigned short calculate_imu_crc(unsigned char data[], unsigned int length);

static const char* labels[20] = {
    "deg [Yaw]",        "deg [Pitch]",      "deg [Roll]", 
    "rad/sec [X]",      "rad/sec [Y]",      "rad/sec [Z]",
    "deg [Latitude]",   "deg [Longitude]",  "deg [Altitude]", 
    "m/sec [X]",        "m/sec [Y]",        "m/sec [Z]",
    "m/s^2 [X]",        "m/s^2 [Y]",        "m/s^2 [Z]", 
    "Gauss [X]",        "Gauss [Y]",        "Gauss [Z]",
    "deg C",            "kPa"
};

static const SensorDataSetup datasetup = {
    .numdata = 20,
    .labels = labels
};

VN200::VN200(void) : Sensor(SENSOR_VN200, VN200_NAME, &datasetup, VN200_DELTA) {
    this->vn200 = vn::sensors::VnSensor();
}

errorlevel_t VN200::initialize(void) {
    // Start Serial1 for IMU communication and connect
    Serial1.begin(VN200_BAUD);
    vn200.connect(VN200_PORT, VN200_BAUD);

    // Check connectivity
    if(!vn200.verifySensorConnectivity()) {
        return ERR_FATAL;
    }

    // Make sure no generic async output is registered
    vn200.writeAsyncDataOutputType(VNOFF);

    // Sensor Register Configuration
	vn::sensors::BinaryOutputRegister bor(
		ASYNCMODE_PORT1, // Async Port 1 is equivalent to Register 75
		200,             // IMU Rate: 800(default) / 200 = 4 [Hz]
		COMMONGROUP_YAWPITCHROLL
		| COMMONGROUP_ANGULARRATE
		| COMMONGROUP_POSITION
		| COMMONGROUP_VELOCITY
		| COMMONGROUP_ACCEL
		| COMMONGROUP_MAGPRES,
		TIMEGROUP_NONE,
		IMUGROUP_NONE,
		GPSGROUP_NONE,
		ATTITUDEGROUP_NONE,
		INSGROUP_NONE,
        GPSGROUP_NONE);
    
    // Sensor should asynchronously output messages at the indicated IMU rate
    vn200.writeBinaryOutput1(bor);

    return ERR_NONE;
}

errorlevel_t VN200::read(float* data, uint8_t numdata) {
    // Check if new IMU data is available
    if (Serial1.available() > 4) {
        check_sync_byte(); // Every new binary message is at least 4 bytes long
    }

    // If sync byte is detected, read the rest of the data
    if (check_sync_byte()) {
        unsigned char in[100];
        Serial1.readBytes(in, 97);

        checksum.b[0] = in[97];
        checksum.b[1] = in[96];

        if (calculate_imu_crc(in, 96) == checksum.s) {
            for (int i = 0; i < 4; i++)
            {
                yaw.b[i] = in[4 + i]; // Attitude
                pitch.b[i] = in[8 + i];
                roll.b[i] = in[12 + i];
                W_x.b[i] = in[16 + i]; // Angular rate
                W_y.b[i] = in[20 + i];
                W_z.b[i] = in[24 + i];
                lati.b[i] = in[28 + i]; // Position
                lati.b[i + 4] = in[32 + i];
                longi.b[i] = in[36 + i];
                longi.b[i + 4] = in[40 + i];
                alti.b[i] = in[44 + i];
                alti.b[i + 4] = in[48 + i];
                v_x.b[i] = in[52 + i]; // Velocity
                v_y.b[i] = in[56 + i];
                v_z.b[i] = in[60 + i];
                a_x.b[i] = in[64 + i]; // Acceleration
                a_y.b[i] = in[68 + i];
                a_z.b[i] = in[72 + i];
                m_x.b[i] = in[76 + i]; // Magnetometer
                m_y.b[i] = in[80 + i];
                m_z.b[i] = in[84 + i];
                temp.b[i] = in[88 + i]; // Temperature
                pres.b[i] = in[92 + i]; // Pressure
            }
        }
    }

    data[0].value = yaw.f;
    data[1].value = pitch.f;
    data[2].value = roll.f;
    data[3].value = W_x.f;
    data[4].value = W_y.f;
    data[5].value = W_z.f;
    data[6].value = (float)lati.d;
    data[7].value = (float)longi.d;
    data[8].value = (float)alti.d;
    data[9].value = v_x.f;
    data[10].value = v_y.f;
    data[11].value = v_z.f;
    data[12].value = a_x.f;
    data[13].value = a_y.f;
    data[14].value = a_z.f;
    data[15].value = m_x.f;
    data[16].value = m_y.f;
    data[17].value = m_z.f;
    data[18].value = temp.f;
    data[19].value = pres.f;

    return ERR_NONE;
}

// Check for the sync byte (0xFA)
bool check_sync_byte(void) {
    for (int i = 0; i < 6; i++)
    { // not sure why 6 specifically, entire function copied from reference
        Serial1.readBytes(in, 1);
        if (in[0] == 0xFA)
        {
            return true;
        }
    }
}

// Calculate the 16-bit CRC for the given ASCII or binary message.
// Function taken from User Manual Section 3.8.3
unsigned short calculate_imu_crc(unsigned char data[], unsigned int length) {
    unsigned int i;
    unsigned short crc = 0;
    for (i = 0; i < length; i++)
    {
        crc = (unsigned char)(crc >> 8) | (crc << 8);
        crc ^= data[i];
        crc ^= (unsigned char)(crc & 0xff) >> 4;
        crc ^= crc << 12;
        crc ^= (crc & 0x00ff) << 5;
    }
    return crc;
}
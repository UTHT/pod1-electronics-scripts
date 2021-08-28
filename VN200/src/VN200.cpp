#include "VN200.h"
#include "Sensor.h"

using namespace std;
using namespace vn::math;
using namespace vn::sensors;
using namespace vn::protocol::uart;
using namespace vn::xplat;

const char* arr[20] = {"deg [Yaw]",    "deg [Pitch]",  "deg [Roll]", 
                        "rad/sec [X]",  "rad/sec [Y]",  "rad/sec [Z]",
                        "deg [Lati]",   "deg [Longi]",  "deg [Alti]", 
                        "m/sec [X]",    "m/sec [Y]",    "m/sec [Z]",
                        "m/s^2 [X]",    "m/s^2 [Y]",    "m/s^2 [Z]", 
                        "Gauss [X]",    "Gauss [Y]",    "Gauss [Z]",
                        "C [Temp]",     "kPa"};
t_datasetup datasetup = {20, arr};

bool imu_sync_detected = false;
unsigned char in[100]; // byte array


typedef union { unsigned short s; unsigned char b[2]; } VNChecksum;
typedef union { float f; unsigned char b[4]; } VNData;
typedef union { double d; unsigned char b[8]; } VNBigData;

VNData yaw, pitch, roll;    // YawPitchRoll (Attitude)  -> Binary Group 1, Bit Offset 3

VNData W_x, W_y, W_z;       // AngularRate              -> Binary Group 1, Bit Offset 5

VNBigData lati, longi, alti;// AngularRate              -> Binary Group 1, Bit Offset 5

VNData v_x, v_y, v_z;       // Velocity                 -> Binary Group 1, Bit Offset 7

VNData a_x, a_y, a_z;       // Acceleration             -> Binary Group 1, Bit Offset 8

VNData m_x, m_y, m_z;       // Magnetic field           -> Binary Group 1, Bit Offset 10
VNData temp, pres;          // Temperature and pressure (same offset)

// Checksum
VNChecksum checksum;

VN200::VN200(arduino_t arduino) : Sensor(S_VN200, arduino, datasetup, 250)
{
    this->VNSensor = VnSensor();
}

errorlevel_t VN200::init() // TODO initialize buffer
{
    Serial1.begin(115200); // Start Serial1 for IMU communication

    // Connect sensor to Arduino
    const string SensorPort = "COM3"; // ** Adjust as needed **
    const uint32_t SensorBaudrate = 115200;

    VNSensor.connect(SensorPort, SensorBaudrate);

    // Check connectivity
    if (VNSensor.verifySensorConnectivity())
    {
        #if DEBUG
            String mn = VNSensor.readModelNumber();
            String fv = VNSensor.readFirmwareVersion();
            Serial.println("Sensor connection established");
            Serial.println("Model Number: " + mn);
            Serial.println("Firmware Version: " + fv);
        #endif
    }
    else
    {
        #if DEBUG
            Serial.println("Sensor connection unsuccessful");
        #endif
        return ERR_FAIL;
    }

    // Make sure no generic async output is registered
    VNSensor.writeAsyncDataOutputType(VNOFF);

    // Sensor Register Configuration

	BinaryOutputRegister bor(
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
		INSGROUP_NONE);

    VNSensor.writeBinaryOutput1(bor); // Sensor should asynchronously output messages at the indicated IMU rate

    return ERR_NONE;
}

// Check for the sync byte (0xFA)
void check_sync_byte()
{
    for (int i = 0; i < 6; i++)
    { // not sure why 6 specifically, entire function copied from reference
        Serial1.readBytes(in, 1);
        if (in[0] == 0xFA)
        {
            imu_sync_detected = true;
            break;
        }
    }
}

// Calculate the 16-bit CRC for the given ASCII or binary message.
// Function taken from User Manual Section 3.8.3
unsigned short calculate_imu_crc(unsigned char data[], unsigned int length)
{
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

// Read the IMU bytes
void read_imu_data()
{
    Serial1.readBytes(in, 97);

    checksum.b[0] = in[97];
    checksum.b[1] = in[96];

    if (calculate_imu_crc(in, 96) == checksum.s)
    {
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

        return ERR_NONE;
    }
}

errorlevel_t VN200::read(t_datum *data, uint8_t numdata)
{
    imu_sync_detected = false;

    if (numdata != 20) {
        return ERR_FAIL;
    }

    // Check if new IMU data is available
    if (Serial1.available() > 4) {
        check_sync_byte(); // Every new binary message is at least 4 bytes long
    }

    // If sync byte is detected, read the rest of the data
    if (imu_sync_detected) {
        read_imu_data();
    }

    data[0].data = yaw.f;
    data[1].data = pitch.f;
    data[2].data = roll.f;
    data[3].data = W_x.f;
    data[4].data = W_y.f;
    data[5].data = W_z.f;
    data[6].data = (float)lati.d;
    data[7].data = (float)longi.d;
    data[8].data = (float)alti.d;
    data[9].data = v_x.f;
    data[10].data = v_y.f;
    data[11].data = v_z.f;
    data[12].data = a_x.f;
    data[13].data = a_y.f;
    data[14].data = a_z.f;
    data[15].data = m_x.f;
    data[16].data = m_y.f;
    data[17].data = m_z.f;
    data[18].data = temp.f;
    data[19].data = pres.f;

    return ERR_NONE;
}
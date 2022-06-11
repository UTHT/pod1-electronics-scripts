#ifndef UTHT_SENSORS_VN200_H_
#define UTHT_SENSORS_VN200_H_

#include <Arduino.h>

#include "sensors.h"
#include "thread.h"

#include <sensors/sensor.h>

#define VN200_NAME      "VN200"
#define VN200_DELTA     250
#define VN200_BAUD      115200
#define VN200_PORT      ""
#define VN200_TIMEOUT   500
#define VN200_IMURATE   200
#define VN200_ASYNCPORT 1

typedef union { unsigned short s; unsigned char b[2]; } VNChecksum;
typedef union { float f; unsigned char b[4]; } VNData;
typedef union { double d; unsigned char b[8]; } VNBigData;

enum CommonGroup {
	COMMONGROUP_NONE				= 0x0000,	///< None.
	COMMONGROUP_TIMESTARTUP			= 0x0001,	///< TimeStartup.
	COMMONGROUP_TIMEGPS				= 0x0002,	///< TimeGps.
	COMMONGROUP_TIMESYNCIN			= 0x0004,	///< TimeSyncIn.
	COMMONGROUP_YAWPITCHROLL		= 0x0008,	///< YawPitchRoll.
	COMMONGROUP_QUATERNION			= 0x0010,	///< Quaternion.
	COMMONGROUP_ANGULARRATE			= 0x0020,	///< AngularRate.
	COMMONGROUP_POSITION			= 0x0040,	///< Position.
	COMMONGROUP_VELOCITY			= 0x0080,	///< Velocity.
	COMMONGROUP_ACCEL				= 0x0100,	///< Accel.
	COMMONGROUP_IMU					= 0x0200,	///< Imu.
	COMMONGROUP_MAGPRES				= 0x0400,	///< MagPres.
	COMMONGROUP_DELTATHETA			= 0x0800,	///< DeltaTheta.
	COMMONGROUP_INSSTATUS			= 0x1000,	///< InsStatus.
	COMMONGROUP_SYNCINCNT			= 0x2000,	///< SyncInCnt.
	COMMONGROUP_TIMEGPSPPS			= 0x4000	///< TimeGpsPps.
};

class VN200 : public Sensor {
  public:
  /**
   * Constructor.
   */
    VN200(void);
  private:
    errorlevel_t initialize(void) override;

    errorlevel_t read(float* data, uint8_t numdata) override;

    // Sensor interface
    vn::sensors::VnSensor vn200;

    // Attitude -> Binary Group 1, Bit Offset 3
    VNData yaw, pitch, roll;

    // Angular Rate -> Binary Group 1, Bit Offset 5
    VNData W_x, W_y, W_z;

    // Angular Rate -> Binary Group 1, Bit Offset 5
    VNBigData lati, longi, alti;

    // Velocity -> Binary Group 1, Bit Offset 7
    VNData v_x, v_y, v_z;

    // Acceleration -> Binary Group 1, Bit Offset 8
    VNData a_x, a_y, a_z;

    // Magnetic field -> Binary Group 1, Bit Offset 10
    VNData m_x, m_y, m_z;

    // Temperature and pressure (same offset)
    VNData temp, pres;

    // Checksum
    VNChecksum checksum;
};

#endif
#include <sensors/mpu9250.h>

#include <Arduino.h>
#include <Wire.h>

#include <utils/base.h>
#include <sensors/sensor.h>

static const char* labels[9] = {
  "mg [X]", 
  "mg [Y]", 
  "mg [Z]", 
  "deg/sec [X]", 
  "deg/sec [Y]", 
  "deg/sec [Z]", 
  "mG [X]", 
  "mG [Y]", 
  "mG [Z]",
};

static const SensorDataSetup datasetup = {
  .numdata = 9,
  .labels = labels
};

MPU9250::MPU9250(uint8_t address) : Sensor(SENSOR_MPU9250, &datasetup, MPU9250_DELTA) {
  this->address = address;
}

errorlevel_t MPU9250::initialize(void) {
  // Start interface
  Wire.begin();

  // Check communication
  byte c = readByte(WHO_AM_I_MPU9250);
  if (c != 0x71) {
    return ERR_FATAL;
  }

  // SELFTEST
  // float selftest[6] = {0, 0, 0, 0, 0, 0};
  // selfTest(selftest);

  // CALIBRATION
  calibrate();
  
  // INITIALIZATION
  initMPU9250();
  initAK8963();

  // Get sensor resolutions, only need to do this once
  aRes = MPU9250_ASCALE == AFS_2G ? (2.0f / 32768.0f) : MPU9250_ASCALE == AFS_4G ? (4.0f / 32768.0f) : MPU9250_ASCALE == AFS_8G ? (8.0f / 32768.0f) : (16.0f / 32768.0f);
  gRes = MPU9250_GSCALE == GFS_250DPS ? (250.0f / 32768.0f) : MPU9250_GSCALE == GFS_500DPS ? (500.0f / 32768.0f) : MPU9250_GSCALE == GFS_1000DPS ? (1000.0f / 32768.0f) : (2000.0f / 32768.0f);
  mRes = MPU9250_MSCALE == MFS_14BITS ? (10.0f * 4912.0f / 8190.0f) : (10.0f * 4912.0f / 32760.0f);

  calibrateMag();

  return ERR_NONE;
}

errorlevel_t MPU9250::read(float* data, uint8_t numdata) {
  // Check if data ready
  if (readByte(INT_STATUS) & 0x01) {
    uint8_t accelRaw[6];
    readBytes(address, ACCEL_XOUT_H, 6, &accelRaw[0]);
    data[0] = ((float)(((int16_t)accelRaw[0] << 8) | accelRaw[1]) * aRes - accelBias[0]) * 1000;
    data[1] = ((float)(((int16_t)accelRaw[2] << 8) | accelRaw[3]) * aRes - accelBias[1]) * 1000;
    data[2] = ((float)(((int16_t)accelRaw[4] << 8) | accelRaw[5]) * aRes - accelBias[2]) * 1000;

    uint8_t gyroRaw[6];
    readBytes(address, GYRO_XOUT_H, 6, &gyroRaw[0]);
    data[3] = (float)(((int16_t)gyroRaw[0] << 8) | gyroRaw[1]) * gRes - gyroBias[0];
    data[4] = (float)(((int16_t)gyroRaw[2] << 8) | gyroRaw[3]) * gRes - gyroBias[1];
    data[5] = (float)(((int16_t)gyroRaw[4] << 8) | gyroRaw[5]) * gRes - gyroBias[2];

    // x/y/z gyro register data, ST2 register stored here, must read ST2 at end
    // of data acquisition
    uint8_t magRaw[7];
    // Wait for magnetometer data ready bit to be set
    if (readByte(AK8963_ADDRESS, AK8963_ST1) & 0x01)
    {
      // Read the six raw data and ST2 registers sequentially into data array
      readBytes(AK8963_ADDRESS, AK8963_XOUT_L, 7, &magRaw[0]);
      uint8_t c = magRaw[6]; // End data read by reading ST2 register
      // Check if magnetic sensor overflow set, if not then report data
      if (!(c & 0x08)) {
        // Turn the MSB and LSB into a signed 16-bit value
        data[6] = (float)(((int16_t)magRaw[1] << 8) | magRaw[0]) * mRes * magCalibration[0] - magBias[0];
        // Data stored as little Endian
        data[7] = (float)(((int16_t)magRaw[3] << 8) | magRaw[2]) * mRes * magCalibration[1] - magBias[1];
        data[8] = (float)(((int16_t)magRaw[5] << 8) | magRaw[4]) * mRes * magCalibration[2] - magBias[2];
      }
    }
  }

  // TODO: other error conditions?
  return ERR_NONE;
}

void MPU9250::writeByte(uint8_t r, uint8_t d) {
  writeByte(address, r, d);
  return;
}

void MPU9250::writeByte(uint8_t address, uint8_t r, uint8_t d) {
  Wire.setClock(MPU9250_I2C_CLK);			// Reset to the desired speed, in case other devices required a slowdown
  Wire.beginTransmission(address);  	// Initialize the Tx buffer
  Wire.write(r);      		// Put slave register address in Tx buffer
  Wire.write(d);                 		// Put data in Tx buffer
  Wire.endTransmission();           		// Send the Tx buffer
  // TODO: Fix this to return something meaningful
  // return NULL; // In the meantime fix it to return the right type
  return;
}

uint8_t MPU9250::readByte(uint8_t r) {
  return readByte(address, r);
}

uint8_t MPU9250::readByte(uint8_t address, uint8_t r) {
  // Initialize the Tx buffer
  Wire.beginTransmission(address);
  // Put slave register address in Tx buffer
  Wire.write(r);
  // Send the Tx buffer, but send a restart to keep connection alive
  Wire.endTransmission(false);
  // Read one byte from slave register address
  Wire.requestFrom(address, (uint8_t) 1);
  // Fill Rx buffer with result
  return Wire.read();
}

uint8_t MPU9250::readBytes(uint8_t r, uint8_t num, uint8_t* dest) {
  return readBytes(address, r, num, dest);
}

uint8_t MPU9250::readBytes(uint8_t address, uint8_t r, uint8_t num, uint8_t* dest) {
  // Initialize the Tx buffer
  Wire.beginTransmission(address);
  // Put slave register address in Tx buffer
  Wire.write(r);
  // Send the Tx buffer, but send a restart to keep connection alive
  Wire.endTransmission(false);

  uint8_t i = 0;
  // Read bytes from slave register address
  Wire.requestFrom(address, num);
  while (Wire.available()) {
    // Put read results in the Rx buffer
    dest[i++] = Wire.read();
  }

  return i; // Return number of bytes written
}

void MPU9250::selfTest(float* destination) {
  uint8_t rawData[6] = {0, 0, 0, 0, 0, 0};
  uint8_t selfTest[6];
  int32_t gAvg[3] = {0}, aAvg[3] = {0}, aSTAvg[3] = {0}, gSTAvg[3] = {0};
  float factoryTrim[6];
  uint8_t FS = GFS_250DPS;
   
  writeByte(SMPLRT_DIV, 0x00);    // Set gyro sample rate to 1 kHz
  writeByte(CONFIG, 0x02);        // Set gyro sample rate to 1 kHz and DLPF to 92 Hz
  writeByte(GYRO_CONFIG, FS<<3);  // Set full scale range for the gyro to 250 dps
  writeByte(ACCEL_CONFIG2, 0x02); // Set accelerometer rate to 1 kHz and bandwidth to 92 Hz
  writeByte(ACCEL_CONFIG, FS<<3); // Set full scale range for the accelerometer to 2 g

  for (int ii = 0; ii < 200; ii++) {  // get average current values of gyro and acclerometer
  
    readBytes(ACCEL_XOUT_H, 6, &rawData[0]);        // Read the six raw data registers into data array
    aAvg[0] += (int16_t)(((int16_t)rawData[0] << 8) | rawData[1]) ;  // Turn the MSB and LSB into a signed 16-bit value
    aAvg[1] += (int16_t)(((int16_t)rawData[2] << 8) | rawData[3]) ;  
    aAvg[2] += (int16_t)(((int16_t)rawData[4] << 8) | rawData[5]) ; 
  
    readBytes(GYRO_XOUT_H, 6, &rawData[0]);       // Read the six raw data registers sequentially into data array
    gAvg[0] += (int16_t)(((int16_t)rawData[0] << 8) | rawData[1]) ;  // Turn the MSB and LSB into a signed 16-bit value
    gAvg[1] += (int16_t)(((int16_t)rawData[2] << 8) | rawData[3]) ;  
    gAvg[2] += (int16_t)(((int16_t)rawData[4] << 8) | rawData[5]) ; 
  }

  // Get average of 200 values and store as average current readings
  for (int ii =0; ii < 3; ii++) {
    aAvg[ii] /= 200;
    gAvg[ii] /= 200;
  }

  // Configure the accelerometer for self-test
  // Enable self test on all three axes and set accelerometer range to +/- 2 g
  writeByte(ACCEL_CONFIG, 0xE0);
  // Enable self test on all three axes and set gyro range to +/- 250 degrees/s
  writeByte(GYRO_CONFIG,  0xE0);
  delay(25);  // Delay a while to let the device stabilize

  // Get average self-test values of gyro and acclerometer
  for (int ii = 0; ii < 200; ii++) {
    // Read the six raw data registers into data array
    readBytes(ACCEL_XOUT_H, 6, &rawData[0]);
    // Turn the MSB and LSB into a signed 16-bit value
    aSTAvg[0] += (int16_t)(((int16_t)rawData[0] << 8) | rawData[1]) ;
    aSTAvg[1] += (int16_t)(((int16_t)rawData[2] << 8) | rawData[3]) ;
    aSTAvg[2] += (int16_t)(((int16_t)rawData[4] << 8) | rawData[5]) ;

    // Read the six raw data registers sequentially into data array
    readBytes(GYRO_XOUT_H, 6, &rawData[0]);
    // Turn the MSB and LSB into a signed 16-bit value
    gSTAvg[0] += (int16_t)(((int16_t)rawData[0] << 8) | rawData[1]) ;
    gSTAvg[1] += (int16_t)(((int16_t)rawData[2] << 8) | rawData[3]) ;
    gSTAvg[2] += (int16_t)(((int16_t)rawData[4] << 8) | rawData[5]) ;
  }

  // Get average of 200 values and store as average self-test readings
  for (int ii =0; ii < 3; ii++) {
    aSTAvg[ii] /= 200;
    gSTAvg[ii] /= 200;
  }

  // Configure the gyro and accelerometer for normal operation
  writeByte(ACCEL_CONFIG, 0x00);
  writeByte(GYRO_CONFIG,  0x00);
  delay(25);  // Delay a while to let the device stabilize

  // Retrieve accelerometer and gyro factory Self-Test Code from USR_Reg
  // X-axis accel self-test results
  selfTest[0] = readByte(SELF_TEST_X_ACCEL);
  // Y-axis accel self-test results
  selfTest[1] = readByte(SELF_TEST_Y_ACCEL);
  // Z-axis accel self-test results
  selfTest[2] = readByte(SELF_TEST_Z_ACCEL);
  // X-axis gyro self-test results
  selfTest[3] = readByte(SELF_TEST_X_GYRO);
  // Y-axis gyro self-test results
  selfTest[4] = readByte(SELF_TEST_Y_GYRO);
  // Z-axis gyro self-test results
  selfTest[5] = readByte(SELF_TEST_Z_GYRO);

  // Retrieve factory self-test value from self-test code reads
  // FT[Xa] factory trim calculation
  factoryTrim[0] = (float)(2620/1<<FS)*(pow(1.01 ,((float)selfTest[0] - 1.0) ));
  // FT[Ya] factory trim calculation
  factoryTrim[1] = (float)(2620/1<<FS)*(pow(1.01 ,((float)selfTest[1] - 1.0) ));
  // FT[Za] factory trim calculation
  factoryTrim[2] = (float)(2620/1<<FS)*(pow(1.01 ,((float)selfTest[2] - 1.0) ));
  // FT[Xg] factory trim calculation
  factoryTrim[3] = (float)(2620/1<<FS)*(pow(1.01 ,((float)selfTest[3] - 1.0) ));
  // FT[Yg] factory trim calculation
  factoryTrim[4] = (float)(2620/1<<FS)*(pow(1.01 ,((float)selfTest[4] - 1.0) ));
  // FT[Zg] factory trim calculation
  factoryTrim[5] = (float)(2620/1<<FS)*(pow(1.01 ,((float)selfTest[5] - 1.0) ));

  // Report results as a ratio of (STR - FT)/FT; the change from Factory Trim
  // of the Self-Test Response
  // To get percent, must multiply by 100
  for (int i = 0; i < 3; i++) {
    // Report percent differences
    destination[i] = 100.0 * ((float)(aSTAvg[i] - aAvg[i])) / factoryTrim[i]
      - 100.;
    // Report percent differences
    destination[i+3] = 100.0*((float)(gSTAvg[i] - gAvg[i]))/factoryTrim[i+3]
      - 100.;
  }
}

void MPU9250::calibrate(void) {
  uint8_t data[12]; // data array to hold accelerometer and gyro x, y, z, data
  uint16_t ii, packet_count, fifo_count;
  int32_t gyro_bias[3]  = {0, 0, 0}, accel_bias[3] = {0, 0, 0};

  // reset device
  // Write a one to bit 7 reset bit; toggle reset device
  writeByte(PWR_MGMT_1, READ_FLAG);
  delay(100);

  // get stable time source; Auto select clock source to be PLL gyroscope
  // reference if ready else use the internal oscillator, bits 2:0 = 001
  writeByte(PWR_MGMT_1, 0x01);
  writeByte(PWR_MGMT_2, 0x00);
  delay(200);

  // Configure device for bias calculation
  // Disable all interrupts
  writeByte(INT_ENABLE, 0x00);
  // Disable FIFO
  writeByte(FIFO_EN, 0x00);
  // Turn on internal clock source
  writeByte(PWR_MGMT_1, 0x00);
  // Disable I2C master
  writeByte(I2C_MST_CTRL, 0x00);
  // Disable FIFO and I2C master modes
  writeByte(USER_CTRL, 0x00);
  // Reset FIFO and DMP
  writeByte(USER_CTRL, 0x0C);
  delay(15);

  // Configure MPU6050 gyro and accelerometer for bias calculation
  // Set low-pass filter to 188 Hz
  writeByte(CONFIG, 0x01);
  // Set sample rate to 1 kHz
  writeByte(SMPLRT_DIV, 0x00);
  // Set gyro full-scale to 250 degrees per second, maximum sensitivity
  writeByte(GYRO_CONFIG, 0x00);
  // Set accelerometer full-scale to 2 g, maximum sensitivity
  writeByte(ACCEL_CONFIG, 0x00);

  uint16_t  gyrosensitivity  = 131;   // = 131 LSB/degrees/sec
  uint16_t  accelsensitivity = 16384; // = 16384 LSB/g

  // Configure FIFO to capture accelerometer and gyro data for bias calculation
  writeByte(USER_CTRL, 0x40);  // Enable FIFO
  // Enable gyro and accelerometer sensors for FIFO  (max size 512 bytes in
  // MPU-9150)
  writeByte(FIFO_EN, 0x78);
  delay(40);  // accumulate 40 samples in 40 milliseconds = 480 bytes

  // At end of sample accumulation, turn off FIFO sensor read
  // Disable gyro and accelerometer sensors for FIFO
  writeByte(FIFO_EN, 0x00);
  // Read FIFO sample count
  readBytes(FIFO_COUNTH, 2, &data[0]);
  fifo_count = ((uint16_t)data[0] << 8) | data[1];
  // How many sets of full gyro and accelerometer data for averaging
  packet_count = fifo_count/12;

  for (ii = 0; ii < packet_count; ii++)
  {
    int16_t accel_temp[3] = {0, 0, 0}, gyro_temp[3] = {0, 0, 0};
    // Read data for averaging
    readBytes(FIFO_R_W, 12, &data[0]);
    // Form signed 16-bit integer for each sample in FIFO
    accel_temp[0] = (int16_t) (((int16_t)data[0] << 8) | data[1]  );
    accel_temp[1] = (int16_t) (((int16_t)data[2] << 8) | data[3]  );
    accel_temp[2] = (int16_t) (((int16_t)data[4] << 8) | data[5]  );
    gyro_temp[0]  = (int16_t) (((int16_t)data[6] << 8) | data[7]  );
    gyro_temp[1]  = (int16_t) (((int16_t)data[8] << 8) | data[9]  );
    gyro_temp[2]  = (int16_t) (((int16_t)data[10] << 8) | data[11]);

    // Sum individual signed 16-bit biases to get accumulated signed 32-bit
    // biases.
    accel_bias[0] += (int32_t) accel_temp[0];
    accel_bias[1] += (int32_t) accel_temp[1];
    accel_bias[2] += (int32_t) accel_temp[2];
    gyro_bias[0]  += (int32_t) gyro_temp[0];
    gyro_bias[1]  += (int32_t) gyro_temp[1];
    gyro_bias[2]  += (int32_t) gyro_temp[2];
  }
  // Sum individual signed 16-bit biases to get accumulated signed 32-bit biases
  accel_bias[0] /= (int32_t) packet_count;
  accel_bias[1] /= (int32_t) packet_count;
  accel_bias[2] /= (int32_t) packet_count;
  gyro_bias[0]  /= (int32_t) packet_count;
  gyro_bias[1]  /= (int32_t) packet_count;
  gyro_bias[2]  /= (int32_t) packet_count;

  // Sum individual signed 16-bit biases to get accumulated signed 32-bit biases
  if (accel_bias[2] > 0L)
  {
    accel_bias[2] -= (int32_t) accelsensitivity;
  }
  else
  {
    accel_bias[2] += (int32_t) accelsensitivity;
  }

  // Construct the gyro biases for push to the hardware gyro bias registers,
  // which are reset to zero upon device startup.
  // Divide by 4 to get 32.9 LSB per deg/s to conform to expected bias input
  // format.
  data[0] = (-gyro_bias[0]/4  >> 8) & 0xFF;
  // Biases are additive, so change sign on calculated average gyro biases
  data[1] = (-gyro_bias[0]/4)       & 0xFF;
  data[2] = (-gyro_bias[1]/4  >> 8) & 0xFF;
  data[3] = (-gyro_bias[1]/4)       & 0xFF;
  data[4] = (-gyro_bias[2]/4  >> 8) & 0xFF;
  data[5] = (-gyro_bias[2]/4)       & 0xFF;

  // Push gyro biases to hardware registers
  writeByte(XG_OFFSET_H, data[0]);
  writeByte(XG_OFFSET_L, data[1]);
  writeByte(YG_OFFSET_H, data[2]);
  writeByte(YG_OFFSET_L, data[3]);
  writeByte(ZG_OFFSET_H, data[4]);
  writeByte(ZG_OFFSET_L, data[5]);

  // Output scaled gyro biases for display in the main program
  gyroBias[0] = (float) gyro_bias[0]/(float) gyrosensitivity;
  gyroBias[1] = (float) gyro_bias[1]/(float) gyrosensitivity;
  gyroBias[2] = (float) gyro_bias[2]/(float) gyrosensitivity;

  // Construct the accelerometer biases for push to the hardware accelerometer
  // bias registers. These registers contain factory trim values which must be
  // added to the calculated accelerometer biases; on boot up these registers
  // will hold non-zero values. In addition, bit 0 of the lower byte must be
  // preserved since it is used for temperature compensation calculations.
  // Accelerometer bias registers expect bias input as 2048 LSB per g, so that
  // the accelerometer biases calculated above must be divided by 8.

  // A place to hold the factory accelerometer trim biases
  int32_t accel_bias_reg[3] = {0, 0, 0};
  // Read factory accelerometer trim values
  readBytes(XA_OFFSET_H, 2, &data[0]);
  accel_bias_reg[0] = (int32_t) (((int16_t)data[0] << 8) | data[1]);
  readBytes(YA_OFFSET_H, 2, &data[0]);
  accel_bias_reg[1] = (int32_t) (((int16_t)data[0] << 8) | data[1]);
  readBytes(ZA_OFFSET_H, 2, &data[0]);
  accel_bias_reg[2] = (int32_t) (((int16_t)data[0] << 8) | data[1]);

  // Define mask for temperature compensation bit 0 of lower byte of
  // accelerometer bias registers
  uint32_t mask = 1uL;
  // Define array to hold mask bit for each accelerometer bias axis
  uint8_t mask_bit[3] = {0, 0, 0};

  for (ii = 0; ii < 3; ii++)
  {
    // If temperature compensation bit is set, record that fact in mask_bit
    if ((accel_bias_reg[ii] & mask))
    {
      mask_bit[ii] = 0x01;
    }
  }

  // Construct total accelerometer bias, including calculated average
  // accelerometer bias from above
  // Subtract calculated averaged accelerometer bias scaled to 2048 LSB/g
  // (16 g full scale)
  accel_bias_reg[0] -= (accel_bias[0]/8);
  accel_bias_reg[1] -= (accel_bias[1]/8);
  accel_bias_reg[2] -= (accel_bias[2]/8);

  data[0] = (accel_bias_reg[0] >> 8) & 0xFF;
  data[1] = (accel_bias_reg[0])      & 0xFF;
  // preserve temperature compensation bit when writing back to accelerometer
  // bias registers
  data[1] = data[1] | mask_bit[0];
  data[2] = (accel_bias_reg[1] >> 8) & 0xFF;
  data[3] = (accel_bias_reg[1])      & 0xFF;
  // Preserve temperature compensation bit when writing back to accelerometer
  // bias registers
  data[3] = data[3] | mask_bit[1];
  data[4] = (accel_bias_reg[2] >> 8) & 0xFF;
  data[5] = (accel_bias_reg[2])      & 0xFF;
  // Preserve temperature compensation bit when writing back to accelerometer
  // bias registers
  data[5] = data[5] | mask_bit[2];

  // Apparently this is not working for the acceleration biases in the MPU-9250
  // Are we handling the temperature correction bit properly?
  // Push accelerometer biases to hardware registers
  writeByte(XA_OFFSET_H, data[0]);
  writeByte(XA_OFFSET_L, data[1]);
  writeByte(YA_OFFSET_H, data[2]);
  writeByte(YA_OFFSET_L, data[3]);
  writeByte(ZA_OFFSET_H, data[4]);
  writeByte(ZA_OFFSET_L, data[5]);

  // Output scaled accelerometer biases for display in the main program
  accelBias[0] = (float)accel_bias[0]/(float)accelsensitivity;
  accelBias[1] = (float)accel_bias[1]/(float)accelsensitivity;
  accelBias[2] = (float)accel_bias[2]/(float)accelsensitivity;
}

void MPU9250::initMPU9250(void) {
  // wake up device
  // Clear sleep mode bit (6), enable all sensors
  writeByte(PWR_MGMT_1, 0x00);
  delay(100); // Wait for all registers to reset

  // Get stable time source
  // Auto select clock source to be PLL gyroscope reference if ready else
  writeByte(PWR_MGMT_1, 0x01);
  delay(200);

  // Configure Gyro and Thermometer
  // Disable FSYNC and set thermometer and gyro bandwidth to 41 and 42 Hz,
  // respectively;
  // minimum delay time for this setting is 5.9 ms, which means sensor fusion
  // update rates cannot be higher than 1 / 0.0059 = 170 Hz
  // DLPF_CFG = bits 2:0 = 011; this limits the sample rate to 1000 Hz for both
  // With the MPU9250, it is possible to get gyro sample rates of 32 kHz (!),
  // 8 kHz, or 1 kHz
  writeByte(CONFIG, 0x03);

  // Set sample rate = gyroscope output rate/(1 + SMPLRT_DIV)
  // Use a 200 Hz rate; a rate consistent with the filter update rate
  // determined inset in CONFIG above.
  writeByte(SMPLRT_DIV, 0x04);

  // Set gyroscope full scale range
  // Range selects FS_SEL and AFS_SEL are 0 - 3, so 2-bit values are
  // left-shifted into positions 4:3

  // get current GYRO_CONFIG register value
  uint8_t c = readByte(GYRO_CONFIG);
  // c = c & ~0xE0; // Clear self-test bits [7:5]
  c = c & ~0x02; // Clear Fchoice bits [1:0]
  c = c & ~0x18; // Clear AFS bits [4:3]
  c = c | MPU9250_GSCALE << 3; // Set full scale range for the gyro
  // Set Fchoice for the gyro to 11 by writing its inverse to bits 1:0 of
  // GYRO_CONFIG
  // c =| 0x00;
  // Write new GYRO_CONFIG value to register
  writeByte(GYRO_CONFIG, c);

  // Set accelerometer full-scale range configuration
  // Get current ACCEL_CONFIG register value
  c = readByte(ACCEL_CONFIG);
  // c = c & ~0xE0; // Clear self-test bits [7:5]
  c = c & ~0x18;  // Clear AFS bits [4:3]
  c = c | MPU9250_ASCALE << 3; // Set full scale range for the accelerometer
  // Write new ACCEL_CONFIG register value
  writeByte(ACCEL_CONFIG, c);

  // Set accelerometer sample rate configuration
  // It is possible to get a 4 kHz sample rate from the accelerometer by
  // choosing 1 for accel_fchoice_b bit [3]; in this case the bandwidth is
  // 1.13 kHz
  // Get current ACCEL_CONFIG2 register value
  c = readByte(ACCEL_CONFIG2);
  c = c & ~0x0F; // Clear accel_fchoice_b (bit 3) and A_DLPFG (bits [2:0])
  c = c | 0x03;  // Set accelerometer rate to 1 kHz and bandwidth to 41 Hz
  // Write new ACCEL_CONFIG2 register value
  writeByte(ACCEL_CONFIG2, c);
  // The accelerometer, gyro, and thermometer are set to 1 kHz sample rates,
  // but all these rates are further reduced by a factor of 5 to 200 Hz because
  // of the SMPLRT_DIV setting

  // Configure Interrupts and Bypass Enable
  // Set interrupt pin active high, push-pull, hold interrupt pin level HIGH
  // until interrupt cleared, clear on read of INT_STATUS, and enable
  // I2C_BYPASS_EN so additional chips can join the I2C bus and all can be
  // controlled by the Arduino as master.
  writeByte(INT_PIN_CFG, 0x22);
  // Enable data ready (bit 0) interrupt
  writeByte(INT_ENABLE, 0x01);
  delay(100);
}

void MPU9250::initAK8963(void) {
  // First extract the factory calibration for each magnetometer axis
  uint8_t rawData[3];  // x/y/z gyro calibration data stored here
  // TODO: Test this!! Likely doesn't work
  writeByte(AK8963_ADDRESS, AK8963_CNTL, 0x00); // Power down magnetometer
  delay(10);
  writeByte(AK8963_ADDRESS, AK8963_CNTL, 0x0F); // Enter Fuse ROM access mode
  delay(10);

  // Read the x-, y-, and z-axis calibration values
  readBytes(AK8963_ADDRESS, AK8963_ASAX, 3, &rawData[0]);

  // Return x-axis sensitivity adjustment values, etc.
  magCalibration[0] =  (float)(rawData[0] - 128)/256. + 1.;
  magCalibration[1] =  (float)(rawData[1] - 128)/256. + 1.;
  magCalibration[2] =  (float)(rawData[2] - 128)/256. + 1.;
  writeByte(AK8963_ADDRESS, AK8963_CNTL, 0x00); // Power down magnetometer
  delay(10);

  // Configure the magnetometer for continuous read and highest resolution.
  // Set Mscale bit 4 to 1 (0) to enable 16 (14) bit resolution in CNTL
  // register, and enable continuous mode data acquisition Mmode (bits [3:0]),
  // 0010 for 8 Hz and 0110 for 100 Hz sample rates.

  // Set magnetometer data resolution and sample ODR
  writeByte(AK8963_ADDRESS, AK8963_CNTL, MFS_16BITS << 4 | M_8HZ);
  delay(10);
}

void MPU9250::calibrateMag() {
  uint16_t ii = 0, sample_count = 0;
  int32_t mag_bias[3]  = {0, 0, 0}, mag_scale[3] = {0, 0, 0};
  int16_t mag_max[3]  = {-32768, -32768, -32768}, mag_min[3]  = {32767, 32767, 32767}, mag_temp[3] = {0, 0, 0};

  // shoot for ~fifteen seconds of mag data
  // at 8 Hz ODR, new mag data is available every 125 ms
  if (MPU9250_MMODE == M_8HZ)
  {
    sample_count = 128;
  }
  // at 100 Hz ODR, new mag data is available every 10 ms
  if (MPU9250_MMODE == M_100HZ)
  {
    sample_count = 1500;
  }

  for (ii = 0; ii < sample_count; ii++)
  {
    uint8_t rawData[7];
    // Wait for magnetometer data ready bit to be set
    if (readByte(AK8963_ADDRESS, AK8963_ST1) & 0x01)
    {
      // Read the six raw data and ST2 registers sequentially into data array
      readBytes(AK8963_ADDRESS, AK8963_XOUT_L, 7, &rawData[0]);
      uint8_t c = rawData[6]; // End data read by reading ST2 register
      if (!(c & 0x08)) {
        mag_temp[0] = ((int16_t)rawData[1] << 8) | rawData[0];
        mag_temp[1] = ((int16_t)rawData[3] << 8) | rawData[2];
        mag_temp[2] = ((int16_t)rawData[5] << 8) | rawData[4];
      }
    }

    for (int jj = 0; jj < 3; jj++)
    {
      if (mag_temp[jj] > mag_max[jj])
      {
        mag_max[jj] = mag_temp[jj];
      }
      if (mag_temp[jj] < mag_min[jj])
      {
        mag_min[jj] = mag_temp[jj];
      }
    }

    if (MPU9250_MMODE == M_8HZ)
    {
      delay(135); // At 8 Hz ODR, new mag data is available every 125 ms
    }
    if (MPU9250_MMODE == M_100HZ)
    {
      delay(12);  // At 100 Hz ODR, new mag data is available every 10 ms
    }
  }

  // Get hard iron correction
  // Get 'average' x mag bias in counts
  mag_bias[0]  = (mag_max[0] + mag_min[0]) / 2;
  // Get 'average' y mag bias in counts
  mag_bias[1]  = (mag_max[1] + mag_min[1]) / 2;
  // Get 'average' z mag bias in counts
  mag_bias[2]  = (mag_max[2] + mag_min[2]) / 2;

  // Save mag biases in G for main program
  magBias[0] = (float)mag_bias[0] * mRes * magCalibration[0];
  magBias[1] = (float)mag_bias[1] * mRes * magCalibration[1];
  magBias[2] = (float)mag_bias[2] * mRes * magCalibration[2];

  // Get soft iron correction estimate
  // Get average x axis max chord length in counts
  mag_scale[0]  = (mag_max[0] - mag_min[0]) / 2;
  // Get average y axis max chord length in counts
  mag_scale[1]  = (mag_max[1] - mag_min[1]) / 2;
  // Get average z axis max chord length in counts
  mag_scale[2]  = (mag_max[2] - mag_min[2]) / 2;

  float avg_rad = mag_scale[0] + mag_scale[1] + mag_scale[2];
  avg_rad /= 3.0;

  magScale[0] = avg_rad / ((float)mag_scale[0]);
  magScale[1] = avg_rad / ((float)mag_scale[1]);
  magScale[2] = avg_rad / ((float)mag_scale[2]);
}
#include "quaternionFilters.h" // Currently Unused.
#include "MPU9250.h"
#define AHRS false          // Set to false for basic data read
#define SerialDebug true    // Set to true to get Serial output for debugging
#define I2Cclock 400000     // Options: 100k, 400k
#define I2Cport Wire
#define MPU9250_ADDRESS 104 // I2C Address: 0x68
MPU9250 mpu9250(MPU9250_ADDRESS, I2Cport, I2Cclock);

// Vibration analysis
#include "arduinoFFT.h"
#define ENABLE_FFT true     // Set to true for FFT calculations
#define FFT_AXIS 0          // Axis setting: 0=x, 1=y, 2=z;
#define SAMPLES 256 // Must be a power of 2
#define SAMPLING_FREQUENCY 1000
arduinoFFT FFT = arduinoFFT();
double vReal[SAMPLES];
double vImag[SAMPLES];
int count = 0;
double peak = 0.0;
double vibrationFreq;


void setup()
{
  Wire.begin();
  Serial.begin(9600);

  while(!Serial){};

  // Read the WHO_AM_I register, this is a good test of communication
  byte c = mpu9250.readByte(MPU9250_ADDRESS, WHO_AM_I_MPU9250);
  Serial.print(F("MPU9250 WHOAMI: 0x"));
  Serial.print(c, HEX);
  Serial.print(F("? "));
  Serial.print(0x71, HEX);
  Serial.println(F("! "));

  if (c != 0x71) // WHO_AM_I should always be 0x71
  {
    Serial.print("Failed to connect to MPU9250: 0x");
    Serial.println(c, HEX);

    // Communication failed, stop here
    Serial.println(F("Communication failed, abort!"));
    Serial.flush();
    abort();
  }
  else
  {
    Serial.println(F("MPU9250 Online!"));

    // Start by performing self test and reporting values
    mpu9250.MPU9250SelfTest(mpu9250.selfTest);
    Serial.println(F("SELF-TEST:"));
    Serial.print(F("X acceleration trim within : "));
    Serial.print(mpu9250.selfTest[0],1); Serial.println("% of factory");
    Serial.print(F("Y acceleration trim within : "));
    Serial.print(mpu9250.selfTest[1],1); Serial.println("% of factory");
    Serial.print(F("Z acceleration trim within : "));
    Serial.print(mpu9250.selfTest[2],1); Serial.println("% of factory");
    Serial.print(F("X gyro trim within : "));
    Serial.print(mpu9250.selfTest[3],1); Serial.println("% of factory");
    Serial.print(F("Y gyro trim within : "));
    Serial.print(mpu9250.selfTest[4],1); Serial.println("% of factory");
    Serial.print(F("Z gyro trim within : "));
    Serial.print(mpu9250.selfTest[5],1); Serial.println("% of factory");

    Serial.println(F("Loading MPU9250 biases and calibrating..."));
    // Calibrate gyro and accelerometers, load biases in bias registers
    mpu9250.calibrateMPU9250(mpu9250.gyroBias, mpu9250.accelBias);

    Serial.println("Initializing MPU9250 for active read...");
    mpu9250.initMPU9250();

    // Read the WHO_AM_I register of the magnetometer, this is a good test of
    // communication
    byte d = mpu9250.readByte(AK8963_ADDRESS, WHO_AM_I_AK8963);
    Serial.print(F("AK8963 WHOAMI: 0x"));
    Serial.print(c, HEX);
    Serial.print(F("? "));
    Serial.print(0x71, HEX);
    Serial.println(F("! "));
    if (d != 0x48)
    {
      Serial.print("Failed to connect to AK8963: 0x");
      Serial.println(c, HEX);

      // Communication failed, stop here
      Serial.println(F("Communication failed, abort!"));
      Serial.flush();
      abort();
    }
    
    // Get magnetometer calibration from AK8963 ROM
    Serial.println("Initializing AK8963 for active read...");
    mpu9250.initAK8963(mpu9250.factoryMagCalibration);
    // Initialize device for active mode read of magnetometer
    
    if (SerialDebug)
    {
      //  Serial.println("Calibration values: ");
      Serial.print("Mag X-Axis factory sensitivity adjustment value ");
      Serial.println(mpu9250.factoryMagCalibration[0], 2);
      Serial.print("Mag Y-Axis factory sensitivity adjustment value ");
      Serial.println(mpu9250.factoryMagCalibration[1], 2);
      Serial.print("Mag Z-Axis factory sensitivity adjustment value ");
      Serial.println(mpu9250.factoryMagCalibration[2], 2);
    }

    // Get sensor resolutions, only need to do this once
    mpu9250.getAres();
    mpu9250.getGres();
    mpu9250.getMres();

    // The next call delays for 4 seconds, and then records about 15 seconds of
    // data to calculate bias and scale.
    Serial.print(F("Calculating magnet bias and scale, takes 20 seconds..."));
    mpu9250.magCalMPU9250(mpu9250.magBias, mpu9250.magScale);
    Serial.println("AK8963 mag biases (mG):");
    Serial.println(mpu9250.magBias[0]);
    Serial.println(mpu9250.magBias[1]);
    Serial.println(mpu9250.magBias[2]);

    Serial.println("AK8963 mag scale (mG):");
    Serial.println(mpu9250.magScale[0]);
    Serial.println(mpu9250.magScale[1]);
    Serial.println(mpu9250.magScale[2]);

    if(SerialDebug)
    {
      Serial.println("Magnetometer:");
      Serial.print("X-Axis sensitivity adjustment value ");
      Serial.println(mpu9250.factoryMagCalibration[0], 2);
      Serial.print("Y-Axis sensitivity adjustment value ");
      Serial.println(mpu9250.factoryMagCalibration[1], 2);
      Serial.print("Z-Axis sensitivity adjustment value ");
      Serial.println(mpu9250.factoryMagCalibration[2], 2);
    }
    Serial.println("Data start!");
    Serial.println("Data start!");

  if(ENABLE_FFT)
  {
    // Collecting data for fourier transform
    // Configured for x-axis (Due only has enough memeory for one axis).
    // For other axes, set FFT_AXIS to 1 for y-axis, to 2 for z-axis
    if     (FFT_AXIS == 0) vReal[count] = (double) mpu9250.ax;
    else if(FFT_AXIS == 1) vReal[count] = (double) mpu9250.ay;
    else                   vReal[count] = (double) mpu9250.az;
    count++;
    if(count > SAMPLES)
    {
      peak = computeFFT(vReal);
      count = 0;
    }
  }
}

void loop()
{
  // Check if data ready
  if (mpu9250.readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01)
  {

    //Read Acceleration Raw
    mpu9250.readAccelData(mpu9250.accelCount);

    //Calculate acceleration in Gs
    mpu9250.ax = (float)mpu9250.accelCount[0] * mpu9250.aRes - mpu9250.accelBias[0];
    mpu9250.ay = (float)mpu9250.accelCount[1] * mpu9250.aRes - mpu9250.accelBias[1];
    mpu9250.az = (float)mpu9250.accelCount[2] * mpu9250.aRes - mpu9250.accelBias[2];

    //Read Gyro Raw
    mpu9250.readGyroData(mpu9250.gyroCount);

    //Calculate Gyro deg/sec
    mpu9250.gx = (float)mpu9250.gyroCount[0] * mpu9250.gRes;
    mpu9250.gy = (float)mpu9250.gyroCount[1] * mpu9250.gRes;
    mpu9250.gz = (float)mpu9250.gyroCount[2] * mpu9250.gRes;

    //Read Mag Raw
    mpu9250.readMagData(mpu9250.magCount);

    // Calculate Mag milligauss (mG)
    mpu9250.mx = (float)mpu9250.magCount[0] * mpu9250.mRes
               * mpu9250.factoryMagCalibration[0] - mpu9250.magBias[0];
    mpu9250.my = (float)mpu9250.magCount[1] * mpu9250.mRes
               * mpu9250.factoryMagCalibration[1] - mpu9250.magBias[1];
    mpu9250.mz = (float)mpu9250.magCount[2] * mpu9250.mRes
               * mpu9250.factoryMagCalibration[2] - mpu9250.magBias[2];
  }

  // Must be called before updating quaternions!
//  mpu9250.updateTime();

/*
 * Name: printData
 * Function: Displays the calculated data for accel, gyro, mag, sensor temp, and vib frequency
 */
void printData() {
  // Print acceleration values in milligs
  Serial.print("X-acceleration: "); Serial.print(1000 * mpu9250.ax);
  Serial.print(" mg          ");
  Serial.print("Y-acceleration: "); Serial.print(1000 * mpu9250.ay);
  Serial.print(" mg          ");
  Serial.print("Z-acceleration: "); Serial.print(1000 * mpu9250.az);
  Serial.println(" mg ");

  // Print gyro values in degree/sec
  Serial.print("X-gyro rate: "); Serial.print(mpu9250.gx, 3);
  Serial.print(" degrees/sec    ");
  Serial.print("Y-gyro rate: "); Serial.print(mpu9250.gy, 3);
  Serial.print(" degrees/sec    ");
  Serial.print("Z-gyro rate: "); Serial.print(mpu9250.gz, 3);
  Serial.println(" degrees/sec");

  // Print mag values in milligauss/sec     [Currently Unused]
  Serial.print("X-mag field: "); Serial.print(mpu9250.mx);
  Serial.print(" mG              ");
  Serial.print("Y-mag field: "); Serial.print(mpu9250.my);
  Serial.print(" mG              ");
  Serial.print("Z-mag field: "); Serial.print(mpu9250.mz);
  Serial.println(" mG");

  mpu9250.tempCount = mpu9250.readTempData(); // Read the adc values
  mpu9250.temperature = ((float) mpu9250.tempCount) / 333.87 + 21.0; // Temperature in degrees Centigrade
  Serial.print("Temperature: ");  Serial.print(mpu9250.temperature, 1);
  Serial.println(" degrees C");

  if(ENABLE_FFT)
  {
    // Arduino Due only has enough memory to process enough data for one axis of Fast Fourier Transform
    if     (FFT_AXIS == 0) Serial.print("X-Axis ");
    else if(FFT_AXIS == 1) Serial.print("Y-Axis ");
    else                   Serial.print("Z-Axis ");
    Serial.print("vibration frequency: "); Serial.print(peak);
    Serial.println(" mg/Hz");
  }
  Serial.println();
}

  if (!AHRS)
  {
    mpu9250.delt_t = millis() - mpu9250.count;
    if (mpu9250.delt_t > 500)
    {
      if(SerialDebug)
      {
        // Print acceleration values in milligs
        Serial.print("X-acceleration: "); Serial.print(1000 * mpu9250.ax);
        Serial.print(" mg          ");
        Serial.print("Y-acceleration: "); Serial.print(1000 * mpu9250.ay);
        Serial.print(" mg          ");
        Serial.print("Z-acceleration: "); Serial.print(1000 * mpu9250.az);
        Serial.println(" mg ");

        // Print gyro values in degree/sec
        Serial.print("X-gyro rate: "); Serial.print(mpu9250.gx, 3);
        Serial.print(" degrees/sec    ");
        Serial.print("Y-gyro rate: "); Serial.print(mpu9250.gy, 3);
        Serial.print(" degrees/sec    ");
        Serial.print("Z-gyro rate: "); Serial.print(mpu9250.gz, 3);
        Serial.println(" degrees/sec");

        // Print mag values in milligauss/sec     [Currently Unused]
        Serial.print("X-mag field: "); Serial.print(mpu9250.mx);
        Serial.print(" mG              ");
        Serial.print("Y-mag field: "); Serial.print(mpu9250.my);
        Serial.print(" mG              ");
        Serial.print("Z-mag field: "); Serial.print(mpu9250.mz);
        Serial.println(" mG");
   
        mpu9250.tempCount = mpu9250.readTempData();  // Read the adc values
        // Temperature in degrees Centigrade
        mpu9250.temperature = ((float) mpu9250.tempCount) / 333.87 + 21.0;
        // Print temperature in degrees Centigrade
        Serial.print("Temperature: ");  Serial.print(mpu9250.temperature, 1);
        Serial.println(" degrees C");

        // Arduino Due only has enough memory to process enough data for one axis of Fast Fourier Transform
        Serial.print("X-vib freq: "); Serial.print(peakx);
        Serial.println(" mg/Hz              ");
//        Serial.print("Y-vib freq: "); Serial.print(peaky);
//        Serial.print(" mg/Hz              ");
//        Serial.print("Z-vib freq: "); Serial.print(peakz);
//        Serial.println(" mg/Hz");

        Serial.println();

      }
      mpu9250.count = millis();
      
    } // if (mpu9250.delt_t > 500)
  } // if (!AHRS)
  else // [Currently Unused, from line 241 - 325]
  {
    // Serial print and/or display at 0.5 s rate independent of data rates
    mpu9250.delt_t = millis() - mpu9250.count;

    // Update LCD once per half-second independent of read rate
    if (mpu9250.delt_t > 500)
    {
      if(SerialDebug)
      {
        Serial.print(" ax = ");  Serial.print((int)1000 * mpu9250.ax);
        Serial.print(" ay = "); Serial.print((int)1000 * mpu9250.ay);
        Serial.print(" az = "); Serial.print((int)1000 * mpu9250.az);
        Serial.println(" mg");

        Serial.print(" gx = ");  Serial.print(mpu9250.gx, 2);
        Serial.print(" gy = "); Serial.print(mpu9250.gy, 2);
        Serial.print(" gz = "); Serial.print(mpu9250.gz, 2);
        Serial.println(" deg/s");

        Serial.print(" mx = ");  Serial.print((int)mpu9250.mx);
        Serial.print(" my = "); Serial.print((int)mpu9250.my);
        Serial.print(" mz = "); Serial.print((int)mpu9250.mz);
        Serial.println(" mG");

        Serial.print(" q0 = ");  Serial.print(*getQ());
        Serial.print(" qx = "); Serial.print(*(getQ() + 1));
        Serial.print(" qy = "); Serial.print(*(getQ() + 2));
        Serial.print(" qz = "); Serial.println(*(getQ() + 3));
      }

// Define output variables from updated quaternion---these are Tait-Bryan
// angles, commonly used in aircraft orientation. In this coordinate system,
// the positive z-axis is down toward Earth. Yaw is the angle between Sensor
// x-axis and Earth magnetic North (or true North if corrected for local
// declination, looking down on the sensor positive yaw is counterclockwise.
// Pitch is angle between sensor x-axis and Earth ground plane, toward the
// Earth is positive, up toward the sky is negative. Roll is angle between
// sensor y-axis and Earth ground plane, y-axis up is positive roll. These
// arise from the definition of the homogeneous rotation matrix constructed
// from quaternions. Tait-Bryan angles as well as Euler angles are
// non-commutative; that is, the get the correct orientation the rotations
// must be applied in the correct order which for this configuration is yaw,
// pitch, and then roll.
// For more see
// http://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
// which has additional links.
      mpu9250.yaw   = atan2(2.0f * (*(getQ()+1) * *(getQ()+2) + *getQ()
                    * *(getQ()+3)), *getQ() * *getQ() + *(getQ()+1)
                    * *(getQ()+1) - *(getQ()+2) * *(getQ()+2) - *(getQ()+3)
                    * *(getQ()+3));
      mpu9250.pitch = -asin(2.0f * (*(getQ()+1) * *(getQ()+3) - *getQ()
                    * *(getQ()+2)));
      mpu9250.roll  = atan2(2.0f * (*getQ() * *(getQ()+1) + *(getQ()+2)
                    * *(getQ()+3)), *getQ() * *getQ() - *(getQ()+1)
                    * *(getQ()+1) - *(getQ()+2) * *(getQ()+2) + *(getQ()+3)
                    * *(getQ()+3));
      mpu9250.pitch *= RAD_TO_DEG;
      mpu9250.yaw   *= RAD_TO_DEG;

      // Declination of SparkFun Electronics (40°05'26.6"N 105°11'05.9"W) is
      //   8° 30' E  ± 0° 21' (or 8.5°) on 2016-07-19
      // - http://www.ngdc.noaa.gov/geomag-web/#declination
      mpu9250.yaw  -= 8.5;
      mpu9250.roll *= RAD_TO_DEG;

      
      if(SerialDebug)
      {
        Serial.print("Yaw, Pitch, Roll: ");
        Serial.print(mpu9250.yaw, 2);
        Serial.print(", ");
        Serial.print(mpu9250.pitch, 2);
        Serial.print(", ");
        Serial.println(mpu9250.roll, 2);

        Serial.print("rate = ");
        Serial.print((float)mpu9250.sumCount / mpu9250.sum, 2);
        Serial.println(" Hz");
      }
      mpu9250.count = millis();
      mpu9250.sumCount = 0;
      mpu9250.sum = 0;
    } // if (mpu9250.delt_t > 500)
  } // if (AHRS)


  // Collecting data for fourier transform
  vRealx[count] = (double) mpu9250.ax;
  vRealy[count] = (double) mpu9250.ay;
  vRealz[count] = (double) mpu9250.az;
  vImag[count] = 0;

  count++;
  
  // Fourier transform to detemine the frequency of vibration using the acclerometer readings
  // IMPORTANT: Arduino only has enough memory to run fourier transform for one axis, otherwise sensor will bootloop
  if(count > SAMPLES)
  {
    FFT.Windowing(vRealx, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(vRealx, vImag, SAMPLES, FFT_FORWARD);
    FFT.ComplexToMagnitude(vRealx, vImag, SAMPLES);
    peakx = FFT.MajorPeak(vRealx, SAMPLES, SAMPLING_FREQUENCY);
    //Serial.print("X-vib freq: "); Serial.print(peakx);
    //Serial.println(" mg/Hz");
        
//    FFT.Windowing(vRealy, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
//    FFT.Compute(vRealy, vImag, SAMPLES, FFT_FORWARD);
//    FFT.ComplexToMagnitude(vRealy, vImag, SAMPLES);
//    peaky = FFT.MajorPeak(vRealy, SAMPLES, SAMPLING_FREQUENCY);
//    Serial.print("Y-vib freq: "); Serial.print(peaky);
//    Serial.println(" mg/Hz");
//        
//    FFT.Windowing(vRealz, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
//    FFT.Compute(vRealz, vImag, SAMPLES, FFT_FORWARD);
//    FFT.ComplexToMagnitude(vRealz, vImag, SAMPLES);
//    peakz = FFT.MajorPeak(vRealz, SAMPLES, SAMPLING_FREQUENCY);
//    Serial.print("Z-vib freq: "); Serial.print(peakz);
//    Serial.println(" mg/Hz");

    count = 0;
  }
  
}

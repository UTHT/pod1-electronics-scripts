#include <sensors/tmp006.h>

#include <Arduino.h>

#include <utils/base.h>
#include <sensors/sensor.h>

#include <Wire.h>

static const char* labels[2] = {
  "deg C [Object]", 
  "deg C [Die]"
};

static const SensorDataSetup datasetup = {
  .numdata = 2,
  .labels = labels
};

void write16(uint8_t address, uint8_t a, uint16_t d);
uint16_t read16(uint8_t address, uint8_t a);

TMP006::TMP006(uint8_t address) : Sensor(SENSOR_TMP006, &datasetup, TMP006_DELTA){
    this->address = address;
}

errorlevel_t TMP006::initialize(void) {
  Wire.begin();

  write16(address, TMP006_CONFIG, TMP006_CFG_MODEON | TMP006_CFG_DRDYEN | TMP006_SAMPLERATE);

  uint16_t mid, did;
  mid = read16(address, TMP006_MANID);
  did = read16(address, TMP006_DEVID);

  if (mid != 0x5449 || did != 0x67) return ERR_FATAL;
  return ERR_NONE;
}

// TODO Calibrate temperature readings
errorlevel_t TMP006::read(float* data, uint8_t numdata) {
  int16_t Traw = read16(address, TMP006_TAMB) >> 2;
  int16_t Vraw = read16(address, TMP006_VOBJ);
  double TdieK = ((double)Traw) * 0.03125 + 273.15; // Convert to Kelvin
  double Vobj = ((double)Vraw) * 156.25 / 1000000000; //156.25 nV per LSB, nV -> V

  double tdie_tref = TdieK - TMP006_TREF;
  double S = (1 + TMP006_A1 * tdie_tref + TMP006_A2 * tdie_tref * tdie_tref) * TMP006_S0 / 100000000000000;

  double Vos = TMP006_B0 + TMP006_B1 * tdie_tref + TMP006_B2 * tdie_tref * tdie_tref;
  double fVobj = (Vobj - Vos) + TMP006_C2 * (Vobj - Vos) * (Vobj - Vos);
  double TobjC = sqrt(sqrt(TdieK * TdieK * TdieK * TdieK + fVobj / S)) - 273.15;

  // Copy buffer
  data[0] = TobjC;
  data[1] = Traw * 0.03125;

  // TODO: other error conditions?
  return ERR_NONE;
}

/**
 * Write uint16_t bits of data to a register
 * @param address I2C Address
 * @param a Register addreess
 * @param d Data to write
 */
void write16(uint8_t address, uint8_t a, uint16_t d) {
  Wire.beginTransmission(address); // start transmission to device

  Wire.write(a);      // sends register address to read from
  Wire.write(d >> 8); // write data
  Wire.write(d);      // write data

  Wire.endTransmission(); // end transmission
}

/**
 * @brief  Read uint16_t bits of data from a register
 * @param address I2C address
 * @param a Register address
 * @returns uint16_t The data read from the register
 */
uint16_t read16(uint8_t address, uint8_t a) {
  uint16_t ret;

  Wire.beginTransmission(address); // start transmission to device
  Wire.write(a); // sends register address to read from
  Wire.endTransmission(); // end transmission

  Wire.requestFrom(address, (uint8_t)2); // send data n-bytes read

  ret = Wire.read(); // receive DATA
  ret <<= 8;
  ret |= Wire.read(); // receive DATA

  return ret;
}
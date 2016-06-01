/*
 * Bme280Sensor.h
 *
 *  Created on: 31 maj 2016
 *      Author: pkasprow
 */

#ifndef BME280SENSOR_H_
#define BME280SENSOR_H_

#include "ISensorDevice.h"
#include "AdapterBme280.h"

#include "mraa/i2c.hpp"

#ifdef __cplusplus
extern "C" {
#endif
  #include "bme280.h"
#ifdef __cplusplus
}
#endif

class Bme280Sensor : public ISensorDevice
{

public:

  enum class DeviceAddress
  {
    ADDR_77h = 0x77,
    ADDR_76h = 0x76
  };

  Bme280Sensor(mraa::I2c & i2cDriver, DeviceAddress address);
  virtual ~Bme280Sensor();

  virtual void acquire(std::vector<sgm::PhysQuantity> quantities, std::vector<sgm::SgmProcessData> & data);

  virtual void acquire(std::vector<sgm::SgmProcessData> & data);

  virtual std::vector<sgm::PhysQuantity> & queryCapabilities();

private:

  static const uint16_t temperatureDivider;

  static const uint16_t pressureDivider;

  static const uint16_t humidityDivider;

  void measure(sgm::PhysQuantity quantity, std::vector<sgm::SgmProcessData> & data);

  void measurePressure(std::vector<sgm::SgmProcessData> & data);

  void measureTemperature(std::vector<sgm::SgmProcessData> & data);

  void measureHumidity(std::vector<sgm::SgmProcessData> & data);

  AdapterBme280 itsBmeAdapter;

  bme280_t itsBme280Config;

  std::vector<sgm::PhysQuantity> itsCapabilities{sgm::PhysQuantity::PRESSURE, sgm::PhysQuantity::TEMPERATURE, sgm::PhysQuantity::HUMIDITY};

};

#endif /* BME280SENSOR_H_ */

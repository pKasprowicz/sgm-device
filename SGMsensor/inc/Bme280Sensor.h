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
  Bme280Sensor(mraa::I2c & i2cDriver);
  virtual ~Bme280Sensor();

  virtual void acquire(std::vector<sgm::PhysQuantity> quantities, std::vector<sgm::SgmProcessData> & data);
  virtual std::vector<sgm::PhysQuantity> & queryCapabilities();

private:

  AdapterBme280 itsBmeAdapter;

  bme280_t itsBme280Config;

};

#endif /* BME280SENSOR_H_ */

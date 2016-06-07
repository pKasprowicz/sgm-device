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

  Bme280Sensor(mraa::I2c & i2cDriver, DeviceAddress address, sgm::MeasurementPoint measPoint);

  Bme280Sensor(const Bme280Sensor &) = delete;

  Bme280Sensor & operator =(const Bme280Sensor &) = delete;

  virtual ~Bme280Sensor();


  bool isReady() override;


  virtual void acquire(std::vector<sgm::SgmProcessData> & data);

  virtual void acquire(std::vector<sgm::PhysQuantity> && quantities, std::vector<sgm::SgmProcessData> & data);

  virtual void acquire(sgm::PhysQuantity & quanty, sgm::SgmProcessData & data) override;


  virtual std::vector<sgm::PhysQuantity> & queryCapabilities();

  sgm::MeasurementPoint queryMeasurementPoint() override;

private:

  static const uint16_t temperatureDivider;

  static const uint16_t pressureDivider;

  static const uint16_t humidityDivider;

  void measure(sgm::PhysQuantity quantity, std::vector<sgm::SgmProcessData> & data);

  sgm::SgmProcessData && measurePressure();

  sgm::SgmProcessData &&  measureTemperature();

  sgm::SgmProcessData && measureHumidity();

  AdapterBme280 itsBmeAdapter;

  bme280_t itsBme280Config;

  std::vector<sgm::PhysQuantity> itsCapabilities{sgm::PhysQuantity::PRESSURE, sgm::PhysQuantity::TEMPERATURE, sgm::PhysQuantity::HUMIDITY};

  sgm::MeasurementPoint itsMeasurementPoint;

};

#endif /* BME280SENSOR_H_ */

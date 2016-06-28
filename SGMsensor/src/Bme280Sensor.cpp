/*
 * Bme280Sensor.cpp
 *
 *  Created on: 31 maj 2016
 *      Author: pkasprow
 */

#include <Bme280Sensor.h>

#include "Logger.h"

#include <ctime>
#include <thread>


const uint16_t Bme280Sensor::temperatureDivider{100U};

const uint16_t Bme280Sensor::pressureDivider{100U};

const uint16_t Bme280Sensor::humidityDivider{1024U};

Bme280Sensor::Bme280Sensor(mraa::I2c & i2cDriver, DeviceAddress address, sgm::MeasurementPoint measPoint) :
itsBmeAdapter(i2cDriver),
itsMeasurementPoint(measPoint)
{

  itsBme280Config.bus_read   = &AdapterBme280::read;
  itsBme280Config.bus_write  = &AdapterBme280::write;
  itsBme280Config.delay_msec = &AdapterBme280::delay;
  itsBme280Config.dev_addr   = static_cast<uint8_t>(address);

  bme280_init(&itsBme280Config);

  BME280_RETURN_FUNCTION_TYPE result{0U};

  result += bme280_set_soft_rst();
  result += bme280_set_power_mode(BME280_SLEEP_MODE);

  result += bme280_set_oversamp_humidity(BME280_OVERSAMP_1X);
  result += bme280_set_oversamp_pressure(BME280_OVERSAMP_2X);
  result += bme280_set_oversamp_temperature(BME280_OVERSAMP_4X);

  result += bme280_set_standby_durn(BME280_STANDBY_TIME_1_MS);

  result += bme280_set_power_mode(BME280_FORCED_MODE);

}

Bme280Sensor::~Bme280Sensor()
{
  (void)bme280_set_power_mode(BME280_SLEEP_MODE);
}

void Bme280Sensor::acquire(std::vector<sgm::SgmProcessData> & data)
{
//  signed int dummy{0U};
//
//  (void)bme280_read_uncomp_pressure_temperature_humidity(&dummy, &dummy, &dummy);

  measure(sgm::PhysQuantity::TEMPERATURE, data);
  measure(sgm::PhysQuantity::PRESSURE, data);
  measure(sgm::PhysQuantity::HUMIDITY, data);
}

void Bme280Sensor::acquire(std::vector<sgm::PhysQuantity> && quantities,
    std::vector<sgm::SgmProcessData>& data)
{
  for (auto physQuantity : quantities)
  {
    measure(physQuantity, data);
  }
}

void Bme280Sensor::acquire(sgm::PhysQuantity& quanty, sgm::SgmProcessData& data)
{
  //TODO implement
}

std::vector<sgm::PhysQuantity>& Bme280Sensor::queryCapabilities()
{
  return itsCapabilities;
}

void Bme280Sensor::measure(sgm::PhysQuantity quantity,
    std::vector<sgm::SgmProcessData>& data)
{

  signed int pressureUncomp{0U};
  signed int temperatureUncomp{0U};
  signed int humidityUncomp{0U};

  BME280_RETURN_FUNCTION_TYPE result{0U};
  result = bme280_get_forced_uncomp_pressure_temperature_humidity(&pressureUncomp, &temperatureUncomp, &humidityUncomp);
  switch(quantity)
  {
    case sgm::PhysQuantity::PRESSURE:
      data.push_back(measurePressure(pressureUncomp));
      break;

    case sgm::PhysQuantity::TEMPERATURE:
      data.push_back(measureTemperature(temperatureUncomp));
      break;

    case sgm::PhysQuantity::HUMIDITY:
      data.push_back(measureHumidity(humidityUncomp));
      break;

    default:
      break;
  }
}

sgm::SgmProcessData Bme280Sensor::measurePressure(signed int uncompVal)
{

  signed int actualPressure = bme280_compensate_pressure_int32(uncompVal);
  return std::move(sgm::SgmProcessData
  {
    0U,
    itsMeasurementPoint,
    sgm::PhysQuantity::PRESSURE,
    sgm::Unit::hPa,
    actualPressure,
    pressureDivider,
    std::time(nullptr),
    0U
  });
}

sgm::SgmProcessData Bme280Sensor::measureTemperature(signed int uncompVal)
{
  signed int actualTemperature = bme280_compensate_temperature_int32(uncompVal);
  return std::move(sgm::SgmProcessData
  {
    0U,
    itsMeasurementPoint,
    sgm::PhysQuantity::TEMPERATURE,
    sgm::Unit::Celsius,
    actualTemperature,
    temperatureDivider,
    std::time(nullptr),
    0U
  });
}

bool Bme280Sensor::isReady()
{

  return true;
}

sgm::SgmProcessData Bme280Sensor::measureHumidity(signed int uncompVal)
{
  signed int actualHumidity = bme280_compensate_humidity_int32(uncompVal);
  return std::move(sgm::SgmProcessData
  {
    0U,
    itsMeasurementPoint,
    sgm::PhysQuantity::HUMIDITY,
    sgm::Unit::Percent,
    actualHumidity,
    humidityDivider,
    std::time(nullptr),
    0U
  });
}

sgm::MeasurementPoint Bme280Sensor::queryMeasurementPoint()
{
  return itsMeasurementPoint;
}

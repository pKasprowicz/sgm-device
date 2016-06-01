/*
 * Bme280Sensor.cpp
 *
 *  Created on: 31 maj 2016
 *      Author: pkasprow
 */

#include <Bme280Sensor.h>

#include "Logger.h"

const uint16_t Bme280Sensor::temperatureDivider{100U};

const uint16_t Bme280Sensor::pressureDivider{100U};

const uint16_t Bme280Sensor::humidityDivider{1024U};

Bme280Sensor::Bme280Sensor(mraa::I2c & i2cDriver, DeviceAddress address) :
itsBmeAdapter(i2cDriver)
{

  itsBme280Config.bus_read   = &AdapterBme280::read;
  itsBme280Config.bus_write  = &AdapterBme280::write;
  itsBme280Config.delay_msec = &AdapterBme280::delay;
  itsBme280Config.dev_addr   = static_cast<uint8_t>(address);

  bme280_init(&itsBme280Config);

  BME280_RETURN_FUNCTION_TYPE result{0U};

  result += bme280_set_power_mode(BME280_NORMAL_MODE);
  result += bme280_set_oversamp_humidity(BME280_OVERSAMP_1X);
  result += bme280_set_oversamp_pressure(BME280_OVERSAMP_2X);
  result += bme280_set_oversamp_temperature(BME280_OVERSAMP_4X);

  result += bme280_set_standby_durn(BME280_STANDBY_TIME_250_MS);
}

Bme280Sensor::~Bme280Sensor()
{
  (void)bme280_set_power_mode(BME280_SLEEP_MODE);
  // TODO Auto-generated destructor stub
}

void Bme280Sensor::acquire(std::vector<sgm::PhysQuantity> quantities,
    std::vector<sgm::SgmProcessData>& data)
{
  for (auto physQuantity : quantities)
  {
    measure(physQuantity, data);
  }
}

std::vector<sgm::PhysQuantity>& Bme280Sensor::queryCapabilities()
{
  return itsCapabilities;
}

void Bme280Sensor::measure(sgm::PhysQuantity quantity,
    std::vector<sgm::SgmProcessData>& data)
{
  switch(quantity)
  {
    case sgm::PhysQuantity::PRESSURE:
      measurePressure(data);
      break;

    case sgm::PhysQuantity::TEMPERATURE:
      measureTemperature(data);
      break;

    case sgm::PhysQuantity::HUMIDITY:
      measureHumidity(data);
      break;

    default:
      break;
  }

}

void Bme280Sensor::measurePressure(std::vector<sgm::SgmProcessData>& data)
{
}

void Bme280Sensor::measureTemperature(std::vector<sgm::SgmProcessData>& data)
{
}

void Bme280Sensor::acquire(std::vector<sgm::SgmProcessData>& data)
{
  signed int pressureUncomp{0U};
  signed int temperatureUncomp{0U};
  signed int humidityUncomp{0U};

  BME280_RETURN_FUNCTION_TYPE result{0U};
  bme280_read_uncomp_pressure_temperature_humidity(&pressureUncomp, &temperatureUncomp, &humidityUncomp);

  signed int actualTemperature = bme280_compensate_temperature_int32(temperatureUncomp);
  data.push_back(sgm::SgmProcessData
      {
        0U,
        sgm::MeasurementPoint::NONE,
        sgm::PhysQuantity::TEMPERATURE,
        sgm::Unit::Celsius,
        actualTemperature,
        temperatureDivider,
        0U
      });
  signed int actualPressure = bme280_compensate_pressure_int32(pressureUncomp);
  data.push_back(sgm::SgmProcessData
        {
          0U,
          sgm::MeasurementPoint::NONE,
          sgm::PhysQuantity::PRESSURE,
          sgm::Unit::hPa,
          actualPressure,
          pressureDivider,
          0U
        });

  signed int actualHumidity = bme280_compensate_humidity_int32(humidityUncomp);
  data.push_back(sgm::SgmProcessData
        {
          0U,
          sgm::MeasurementPoint::NONE,
          sgm::PhysQuantity::HUMIDITY,
          sgm::Unit::Percent,
          actualHumidity,
          humidityDivider,
          0U
        });

  SGM_LOG_DEBUG("Temperature readout : %4.2f Celsius", actualTemperature / 100.0f);
  SGM_LOG_DEBUG("Pressure readout    : %4.2f hPa",     actualPressure / 100.0f);
  SGM_LOG_DEBUG("Humidity readout    : %4.2f percent", (actualHumidity / 1024.0f) );

}

void Bme280Sensor::measureHumidity(std::vector<sgm::SgmProcessData>& data)
{
}

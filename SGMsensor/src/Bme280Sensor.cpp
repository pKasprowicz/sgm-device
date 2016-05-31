/*
 * Bme280Sensor.cpp
 *
 *  Created on: 31 maj 2016
 *      Author: pkasprow
 */

#include <Bme280Sensor.h>

Bme280Sensor::Bme280Sensor(mraa::I2c & i2cDriver) :
itsBmeAdapter(i2cDriver)
{


  bme280_init(&itsBme280Config);

}

Bme280Sensor::~Bme280Sensor()
{
  // TODO Auto-generated destructor stub
}

void Bme280Sensor::acquire(std::vector<sgm::PhysQuantity> quantities,
    std::vector<sgm::SgmProcessData>& data)
{
}

std::vector<sgm::PhysQuantity>& Bme280Sensor::queryCapabilities()
{
}

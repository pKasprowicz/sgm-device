/*
 * AdapterBme280.cpp
 *
 *  Created on: 31 maj 2016
 *      Author: pkasprow
 */

#include "AdapterBme280.h"

#include <cstring>

AdapterBme280 * AdapterBme280::instance{nullptr};

AdapterBme280::AdapterBme280(mraa::I2c & i2cRef) throw (std::exception) :
itsI2cDevice(i2cRef)
{
  if (AdapterBme280::instance != nullptr)
  {
    throw std::runtime_error("Cannot create more instances of AdapterBme280");
  }

  AdapterBme280::instance = this;

}

AdapterBme280::~AdapterBme280()
{
  AdapterBme280::instance = nullptr;
}

s8 AdapterBme280::read(u8 devAddress, u8 regAddress, u8* readBuffer,
    u8 lengthToRead)
{
  if (nullptr == AdapterBme280::instance)
  {
    throw std::runtime_error("No instance of AdapterBme280");
  }

  instance->itsI2cDevice.address(devAddress);

  return instance->itsI2cDevice.readBytesReg(regAddress, readBuffer, lengthToRead);
}

s8 AdapterBme280::write(u8 devAddress, u8 regAddress, u8* writeBuffer,
    u8 lengthToWrite)
{
  uint8_t buffer[lengthToWrite + 1];
  buffer[0] = regAddress;
  std::memcpy(&buffer[1], writeBuffer, lengthToWrite);

  if (nullptr == AdapterBme280::instance)
  {
    throw std::runtime_error("No instance of AdapterBme280");
  }

  instance->itsI2cDevice.address(devAddress);

  return instance->itsI2cDevice.write(buffer, lengthToWrite + 1);
}

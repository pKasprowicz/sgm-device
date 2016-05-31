/*
 * AdapterBme280.h
 *
 *  Created on: 31 maj 2016
 *      Author: pkasprow
 */

#ifndef ADAPTERBME280_H_
#define ADAPTERBME280_H_

#include <mraa/i2c.hpp>

#ifdef __cplusplus
extern "C" {
#endif
  #include "bme280.h"
#ifdef __cplusplus
}
#endif

class AdapterBme280
{
public:
  AdapterBme280(mraa::I2c & i2cRef) throw (std::exception);
  virtual ~AdapterBme280();

  static s8 read(u8 devAddress, u8 regAddress, u8 * readBuffer, u8 lengthToRead);
  static s8 write(u8 devAddress, u8 regAddress, u8 * writeBuffer, u8 lengthToWrite);

private:

  static AdapterBme280 * instance;

  mraa::I2c & itsI2cDevice;
};

#endif /* ADAPTERBME280_H_ */

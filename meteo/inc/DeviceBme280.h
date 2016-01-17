/*
 * DeviceBme280.h
 *
 *  Created on: 16 sty 2016
 *      Author: pkasprow
 */

#ifndef DEVICEBME280_H_
#define DEVICEBME280_H_

#include <mraa/i2c.hpp>

class DeviceBme280
{
public:
	DeviceBme280(mraa::I2c i2cBus);
	virtual ~DeviceBme280();
};

#endif /* DEVICEBME280_H_ */

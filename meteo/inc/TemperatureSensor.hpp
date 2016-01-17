/*
 * TemperatureSensor.hpp
 *
 *  Created on: 15 sty 2016
 *      Author: pkasprow
 */

#ifndef TEMPERATURESENSOR_HPP_
#define TEMPERATURESENSOR_HPP_

#include "ISensor.hpp"

class TemperatureSensor : public ISensor
{

public:

	TemperatureSensor();

	virtual float getCurrentValue();
	virtual float getMaxValue();
	virtual float getMinValue();


private:
	DeviceBme280 sensorDevice;

};



#endif /* TEMPERATURESENSOR_HPP_ */

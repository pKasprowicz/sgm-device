/*
 * SensorService.h
 *
 *  Created on: 5 cze 2016
 *      Author: pkasprow
 */

#ifndef SENSORSERVICEAPPLICATION_H_
#define SENSORSERVICEAPPLICATION_H_

#include "Bme280Sensor.h"
#include "SensorHub.h"

#include "mraa/i2c.hpp"

class SensorServiceApplication
{
public:
  SensorServiceApplication();

  SensorServiceApplication(const SensorServiceApplication &) = delete;
  SensorServiceApplication & operator =(const SensorServiceApplication &) = delete;

  virtual ~SensorServiceApplication();

  void run();

private:

  void executeAcquisitionRoutine();

  static const int i2cBusNumber;

  mraa::I2c itsI2cDriver;

  Bme280Sensor itsBmeSensor;

  SensorHub itsSensorHub;

};

#endif /* SENSORSERVICEAPPLICATION_H_ */

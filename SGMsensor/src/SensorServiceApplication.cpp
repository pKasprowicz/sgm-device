/*
 * SensorService.cpp
 *
 *  Created on: 5 cze 2016
 *      Author: pkasprow
 */

#include <SensorServiceApplication.h>
#include "ClientSocket.h"

#include "Logger.h"

#include <stdexcept>

const int SensorServiceApplication::i2cBusNumber{1};

SensorServiceApplication::SensorServiceApplication() :
  itsI2cDriver(i2cBusNumber),
  itsBmeSensor(itsI2cDriver, Bme280Sensor::DeviceAddress::ADDR_77h, sgm::MeasurementPoint::AIR),
  itsSensorHub()

{
  // TODO Auto-generated constructor stub

}

SensorServiceApplication::~SensorServiceApplication()
{
  // TODO Auto-generated destructor stub
}

void SensorServiceApplication::run()
{
  itsSensorHub.registerSensor(itsBmeSensor);

  SGM_LOG_INFO("Starting acquisition routine...");
  executeAcquisitionRoutine();
  SGM_LOG_INFO("Acquisition routine finished!");

}

void SensorServiceApplication::executeAcquisitionRoutine()
{
  std::vector<sgm::SgmProcessData> measurementsData;

  itsSensorHub.acquireAll(measurementsData);

  try
  {
    ClientSocket<sgm::SgmProcessData> sinkSocket;
    for (auto measurement : measurementsData)
    {
      if (sizeof(measurement) > sinkSocket.sendPacket(&measurement))
      {
        SGM_LOG_WARN("Possible socket transmission error");
      }


    }
  }
  catch(std::runtime_error & ex)
  {
    SGM_LOG_ERROR("Unhandled exception caught: %s", ex.what());
  }



}

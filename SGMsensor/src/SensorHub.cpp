/*
 * SensorManager.cpp
 *
 *  Created on: 5 cze 2016
 *      Author: pkasprow
 */

#include <SensorHub.h>

SensorHub::SensorHub()
{
  // TODO Auto-generated constructor stub

}

SensorHub::~SensorHub()
{
  // TODO Auto-generated destructor stub
}

void SensorHub::acquireAll(std::vector<sgm::SgmProcessData>& measurementData)
{
  for (auto sensor : itsSensorVector)
  {
    sensor->acquire(measurementData);
  }
}

void SensorHub::acquireOneSensor(IdTuple & id, sgm::SgmProcessData & measurement)
{
  itsSensorList[id]->acquire(id.second, measurement);
}

SensorHub::RegistrationResult SensorHub::registerSensor(ISensorDevice & sensor)
{
  RegistrationResult regResult = RegistrationResult::DEVICE_UNAVAILABLE;

  if (!sensor.isReady())
  {
    return RegistrationResult::DEVICE_UNAVAILABLE;
  }

  sgm::MeasurementPoint measPoint = sensor.queryMeasurementPoint();

  for (auto physQuantity : sensor.queryCapabilities())
  {
    auto result = itsSensorList.emplace(IdTuple(measPoint, physQuantity), &sensor);
    if (!result.second)
    {
      regResult = RegistrationResult::DEVICE_OVERLAPPED;
    }

  }

  itsSensorVector.push_back(&sensor);

  return regResult;
}

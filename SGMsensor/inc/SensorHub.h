/*
 * SensorHub.h
 *
 *  Created on: 5 cze 2016
 *      Author: pkasprow
 */

#ifndef SENSORHUB_H_
#define SENSORHUB_H_

#include "ISensorDevice.h"

#include "SGMDataType.h"

#include <map>
#include <utility>

class SensorHub
{
public:

  using IdTuple = std::pair<sgm::MeasurementPoint, sgm::PhysQuantity>;

  enum class RegistrationResult
  {
    REGISTERED,
    DEVICE_OVERLAPPED,
    DEVICE_UNAVAILABLE,
    REGISTRATION_ERROR
  };

  SensorHub();
  ~SensorHub();

  void acquireAll(std::vector<sgm::SgmProcessData> & sensorList);

  void acquireOneSensor(IdTuple & id, sgm::SgmProcessData & measurement);

  RegistrationResult registerSensor(ISensorDevice & sensor);

private:


  std::map<IdTuple, ISensorDevice * > itsSensorList;

  std::vector<ISensorDevice *> itsSensorVector;
};

#endif /* SENSORHUB_H_ */

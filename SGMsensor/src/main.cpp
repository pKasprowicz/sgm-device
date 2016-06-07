#include "AdapterBme280.h"
#include "Bme280Sensor.h"

#include "ClientSocket.h"
#include "SGMDataType.h"

#include "mraa/i2c.hpp"

#include <iostream>
#include <vector>
#include <thread>

#include "bme280.h"

using namespace std;


int main() {
  /*
   * Setup your example here, code that should run once
   */
  ClientSocket<sgm::SgmProcessData> socket;
  mraa::I2c i2cDriver(1);

  Bme280Sensor bmeSensor(i2cDriver, Bme280Sensor::DeviceAddress::ADDR_77h, sgm::MeasurementPoint::AIR);

  while(1)
  {

  }

  return 0;
}

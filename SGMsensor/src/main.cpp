#include "AdapterBme280.h"
#include "Bme280Sensor.h"

#include "SGMDataType.h"

#include "mraa/i2c.hpp"

#include <iostream>
#include <vector>
#include <thread>

#include "bme280.h"

using namespace std;


int main() {
  /* Setup your example here, code that should run once
   */

  mraa::I2c i2cDriver(1);

  Bme280Sensor bmeSensor(i2cDriver, Bme280Sensor::DeviceAddress::ADDR_77h);

  std::vector<sgm::SgmProcessData> data;

  while(1)
  {

    std::vector<sgm::SgmProcessData> data;
    bmeSensor.acquire(data);
    std::this_thread::sleep_for(std::chrono::seconds(1));

  }

  return 0;
}

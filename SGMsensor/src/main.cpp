#include "AdapterBme280.h"

#include "mraa/i2c.hpp"

#include <iostream>

#include "bme280.h"

using namespace std;


int main() {
  /* Setup your example here, code that should run once
   */

  mraa::I2c i2cDriver(1);
  AdapterBme280 bmeToI2cAdapter(i2cDriver);

  return 0;
}

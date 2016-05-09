/*
 * main.cpp
 *
 *  Created on: 15 mar 2016
 *      Author: pkasprow
 */

extern"C"
{
#include <unistd.h>
}

#include "SharedMemory.h"

#include "PowerManager.h"
#include "BGS2CMux.h"

#include <Logger.h>

#include <thread>

//TODO turn off digital interface when process is killed

class ModemQueryMock : public IModemQuery
{
public:
  virtual Result run()
  {
    return Result::OK_RAW;
  }

  virtual ~ModemQueryMock()
  {

  }
};

int main() {

  mraa_init();

  BGS2CMux cMuxDriver;

  mraa::Uart uart1(0);
  uart1.setBaudRate(57600);
  uart1.setMode(8, mraa::UART_PARITY_NONE, 1);
  uart1.setFlowcontrol(false, true);
  uart1.setTimeout(10, 10, 10);

  SharedMemory sharedMem;
  if (SharedMemory::Result::INIT_ERROR == sharedMem.init())
  {
    SGM_LOG_ERROR("Error during shared memory initialization");
    return -1;
  }

  PowerManager powerManager(sharedMem, cMuxDriver, uart1);


  powerManager.run();

}

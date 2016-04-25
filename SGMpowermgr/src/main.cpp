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

#include <SharedMemory.h>

#include "PowerManager.h"

#include <Logger.h>

#include <thread>

//TODO turn off digital interface when process is killed

class ModemQueryMock : public IModemQuery
{
public:
  virtual Result run()
  {
    return Result::OK;
  }

  virtual ~ModemQueryMock()
  {

  }
};

void isr_mock(void * data)
{
  int * value = reinterpret_cast<int *>(data);

  SGM_LOG_DEBUG("Executed custom power pin isr with parameter: %d", *value);
}

int main() {

  mraa_init();

  SharedMemory sharedMem;
  if (SharedMemory::Result::INIT_ERROR == sharedMem.init())
  {
    SGM_LOG_ERROR("Error during shared memory initialization");
    return -1;
  }

  PowerManager powerManager(sharedMem);
  ModemQueryMock queryMock;


  powerManager.run();

}

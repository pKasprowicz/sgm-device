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
#include <Logger.h>


int main() {
  SharedMemory sharedMem;

  SharedMemory::Result result = sharedMem.init();

  if (SharedMemory::Result::INIT_OK == result)
  {
    SGM_LOG_INFO("Shared Memory initialized successfully!");
  }
  else
  {
    return -1;
  }

  SharedMemory::SharedData & sData = sharedMem.getDataInstance();

  bool flag = false;

  for (;;) {

    flag = (flag ? false : true);

    sData.startAccess();

    SGM_LOG_INFO("Setting flag to %d", flag);

    sData.setModemReady(flag);

    sData.endAccess();

    sleep(5);
  }

  return 0;
}

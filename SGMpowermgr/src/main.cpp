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

//TODO turn off digital interface when process is killed

int main() {

  SharedMemory SharedMem;

  if (SharedMemory::Result::INIT_ERROR == SharedMem.init())
  {
    SGM_LOG_ERROR("Error during shared memory initialization");
  }



  return 0;
}

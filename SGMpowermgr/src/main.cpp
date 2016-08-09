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
#include "ShutdownDriver.h"

#include <Logger.h>

#include <thread>
#include <signal.h>

//TODO turn off digital interface when process is killed


int main() {

  sigset_t PowerMgrSignalSet;

  sigemptyset (&PowerMgrSignalSet);
  sigaddset(&PowerMgrSignalSet, SIGTERM);
  sigaddset(&PowerMgrSignalSet, SIGHUP);

  sigprocmask(SIG_SETMASK, &PowerMgrSignalSet, NULL);

  mraa_init();

  mraa::Uart uart1(0);
  uart1.setBaudRate(57600);
  uart1.setMode(8, mraa::UART_PARITY_NONE, 1);
  uart1.setFlowcontrol(false, true);
  uart1.setTimeout(10, 10, 10);

  BGS2CMux cMuxDriver(uart1);

  ShutdownDriver shutdownDriver(cMuxDriver);

  SharedMemory sharedMem;
  if (SharedMemory::Result::INIT_ERROR == sharedMem.init())
  {
    SGM_LOG_ERROR("Error during shared memory initialization");
    return -1;
  }

  PowerManager powerManager(sharedMem, cMuxDriver, uart1, shutdownDriver);

  powerManager.run();

}

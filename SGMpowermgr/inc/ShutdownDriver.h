/*
 * ShutdownDriver.h
 *
 *  Created on: 5 lip 2016
 *      Author: pkasprow
 */

#ifndef SHUTDOWNDRIVER_H_
#define SHUTDOWNDRIVER_H_

#include "ICMuxDriver.h"
#include "AtCommand.h"

#include "AtCommand.h"
#include "bgs2/at/Smso.h"
#include "bgs2/PlainAtInterpreter.h"

#include <mraa/uart.hpp>

class ShutdownDriver
{
  //TODO we need a thread safe access wrapper for mraa uarts
public:

  enum class ShutdownState
  {
    NO_SHUTDOWN,
    NORMAL_SHUTDOWN,
    FORCE_SHUTDOWN
  };

  ShutdownDriver(ICMuxDriver & cmuxDriver);
  virtual ~ShutdownDriver();

  bool executeShutdown();

  ShutdownState getShutdownState()
  {
    return itsShutdownInProgressFlag;
  }

private:

  void beginShutdownRequestListening();
  void shutdown();

  ICMuxDriver & itsCmuxDriver;
  ShutdownState itsShutdownInProgressFlag{ShutdownState::NO_SHUTDOWN};

  AtCommand<bgs2::PlainAtInterpreter, bgs2::Smso> shutdownCommand;

};

#endif /* SHUTDOWNDRIVER_H_ */

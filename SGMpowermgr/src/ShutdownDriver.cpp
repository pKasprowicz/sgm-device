/*
 * ShutdownDriver.cpp
 *
 *  Created on: 5 lip 2016
 *      Author: pkasprow
 */

#include <ShutdownDriver.h>

#include <signal.h>

#include <thread>

ShutdownDriver::ShutdownDriver(ICMuxDriver & cmuxDriver)
: itsCmuxDriver(cmuxDriver),
  shutdownCommand()
{
  beginShutdownRequestListening();
}

ShutdownDriver::~ShutdownDriver()
{
  // TODO Auto-generated destructor stub
}

bool ShutdownDriver::executeShutdown()
{
  if (ShutdownState::NO_SHUTDOWN == itsShutdownInProgressFlag)
  {
    shutdown();
  }
}

void ShutdownDriver::beginShutdownRequestListening()
{
  std::thread signalThread([this](){

      sigset_t PowerMgrSignalSet;

      sigemptyset(&PowerMgrSignalSet);
      sigaddset(&PowerMgrSignalSet, SIGTERM);
      sigaddset(&PowerMgrSignalSet, SIGHUP);

      int capturedSignal{0};
      sigwait(&PowerMgrSignalSet, &capturedSignal);

      if (SIGTERM == capturedSignal)
      {
        SGM_LOG_INFO("Exit signal captured, attempting to execute soft shutdown...");
        shutdown();
      }

    });

  signalThread.detach();
}

void ShutdownDriver::shutdown()
{
  mraa::Uart & currentUart = itsCmuxDriver.getCurrentUart();

  if (IAtCommand::Result::AT_SENT != shutdownCommand.sendAt(currentUart, IAtCommand::CommandType::AT_EXECUTE))
  {
    SGM_LOG_FATAL("Unable to execute clean shutdown, exiting by force!");
    itsShutdownInProgressFlag = ShutdownState::FORCE_SHUTDOWN;
  }
  itsShutdownInProgressFlag = ShutdownState::NORMAL_SHUTDOWN;
}

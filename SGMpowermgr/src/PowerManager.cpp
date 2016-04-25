/*
 * PowerStateMachine.cpp
 *
 *  Created on: 19 kwi 2016
 *      Author: pkasprow
 */

#include "PowerManager.h"

#include "Logger.h"
#include "SGMfixtures.h"

#include <thread>

void PowerManager::onPowerIndChange(void * data)
{
  PowerManager & instance = *reinterpret_cast<PowerManager *>(data);
  ModemPowerController::PowerState currentPowerState = instance.itsPowerController.getPowerState();

  SGM_LOG_DEBUG("PowerManager::onPowerIndChange() : power state changed event triggered");

  {
    std::unique_lock<std::mutex> eventLock(instance.itsLockingMutex);

    if (currentPowerState != storedPowerState)
    {
      eventLock.unlock();
      storedPowerState = currentPowerState;
      itsCondVariable.notify_all();
    }
  }

}

PowerManager::PowerManager(SharedMemory & sharedMem) :
    itsModemQuery(),
    itsPowerController(sharedMem, itsModemQuery, std::bind(&PowerManager::onPowerIndChange, this)),
    itsModemCMux()
{

}

void PowerManager::run()
{
  std::thread powerControllerThread(std::ref(itsPowerController));

  determineInitialConditions();

  itsPowerController.turnOn();

  while(true)
  {
    std::unique_lock<std::mutex> lock(itsLockingMutex);
    itsCondVariable.wait(lock);


    processStateMachine(computeEvent());

  }

  powerControllerThread.join();
}

PowerManager::~PowerManager()
{
}

void PowerManager::processStateMachine(Event evType)
{
  SGM_LOG_DEBUG("void PowerManager::processStateMachine() : event tick");

  switch(storedPowerState)
  {
  case ModemPowerController::PowerState::ENABLED:
    break;

  case ModemPowerController::PowerState::DISABLED_POWERED:
    break;

  case ModemPowerController::PowerState::DISABLED_UNPOWERED:
    break;

  case ModemPowerController::PowerState::UNDEFINED:
    SGM_LOG_FATAL("PowerManager::processStateMachine() : UNDEFINED state requested to be processed. Recovering...");
    break;
  }

}

PowerManager::Event PowerManager::computeEvent()
{
  Event retVal = Event::UNDEFINED;

  switch(itsDeviceState)
  {
  case DeviceState::MODEM_OFF:
    if (ModemPowerController::PowerState::ENABLED == storedPowerState)
    {
      retVal = Event::ASYNC_TURN_ON;
    }
    else
    {
      SGM_LOG_WARN("PowerManager::computeEvent() : event %d has no effect", storedPowerState);
    }
    break;

  case DeviceState::MODEM_READY:
  case DeviceState::MODEM_CMUX:
    if ( (ModemPowerController::PowerState::DISABLED_POWERED == storedPowerState)
      || (ModemPowerController::PowerState::DISABLED_UNPOWERED == storedPowerState) )
    {
      retVal = Event::ASYNC_TURN_OFF;
    }
    break;
  }

}

void PowerManager::determineInitialConditions()
{
  storedPowerState =  itsPowerController.getPowerState();

  switch(storedPowerState)
  {
    case ModemPowerController::PowerState::ENABLED:
      break;

    case ModemPowerController::PowerState::DISABLED_POWERED:
    case ModemPowerController::PowerState::DISABLED_UNPOWERED:
      break;

    default:
      break;
  }
}

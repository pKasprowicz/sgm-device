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

    instance.storedPowerState.set(currentPowerState);

    if (instance.storedPowerState.hasChanged())
    {
      eventLock.unlock();
      instance.itsCondVariable.notify_all();
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

  switch(storedPowerState.get())
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

  if (!itsDeviceState.hasChanged())

  switch(itsDeviceState.get())
  {
  case DeviceState::MODEM_OFF:
    if (storedPowerState.is(ModemPowerController::PowerState::ENABLED))
    {
      retVal = Event::ASYNC_TURN_ON;
    }
    else
    {
      SGM_LOG_WARN("PowerManager::computeEvent() : event %d has no effect", storedPowerState.get());
    }
    break;

  case DeviceState::MODEM_READY:
  case DeviceState::MODEM_CMUX:
    if ( (storedPowerState.is(ModemPowerController::PowerState::DISABLED_POWERED))
      || (storedPowerState.is(ModemPowerController::PowerState::DISABLED_UNPOWERED)) )
    {
      retVal = Event::ASYNC_TURN_OFF;
    }
    break;

  default:
    SGM_LOG_ERROR("PowerManager::computeEvent() : device not in defined state");
    break;

  }

  return retVal;

}

void PowerManager::determineInitialConditions()
{
  storedPowerState.set(itsPowerController.getPowerState());

  switch(storedPowerState.get())
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

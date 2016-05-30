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
    std::unique_lock<std::mutex> eventLock(instance.eventMutex);

    instance.storedPowerState.set(currentPowerState);

    if (instance.storedPowerState.hasChanged())
    {
      eventLock.unlock();
      SGM_LOG_INFO("PowerManager::onPowerIndChange : old power state : %d", instance.storedPowerState.getHistory());
      SGM_LOG_INFO("PowerManager::onPowerIndChange : new power state : %d", currentPowerState);
      instance.itsCondVariable.notify_all();
    }
  }

}

PowerManager::PowerManager(SharedMemory & sharedMem, ICMuxDriver & cMuxDriver, mraa::Uart & uartPort) :
    itsModemQuery(uartPort),
    itsSharedMemory(sharedMem),
    itsPowerController(sharedMem, itsModemQuery, std::bind(&PowerManager::onPowerIndChange, this)),
    itsModemCMux(cMuxDriver)
{

}

void PowerManager::run()
{
  std::thread powerControllerThread(std::ref(itsPowerController));

  determineInitialConditions();

  SGM_LOG_INFO("PowerManager initialized, turning modem on...")
  itsPowerController.turnOn();

//  try
//  {
//    std::thread([this]()
//    {
//      waitForIncomingRequest();
//    });
//  }
//  catch(std::exception & ex)
//  {
//    SGM_LOG_FATAL("Error when starting request waiting thread");
//  }

  SGM_LOG_INFO("Monitoring of incoming requests started");


  while(true)
  {
    std::unique_lock<std::mutex> lock(eventMutex);
    itsCondVariable.wait(lock);

    {
      std::lock_guard<std::mutex> processingGuard(processingMutex);
      processIncomingEvent(computeEvent());
    }

  }

  powerControllerThread.join();
}

PowerManager::~PowerManager()
{
}

void PowerManager::processIncomingEvent(Event evType)
{
  SGM_LOG_DEBUG("PowerManager::processStateMachine() : event tick");

  SharedMemory::SharedData & sData = itsSharedMemory.getDataInstance();

  switch(evType)
  {
  case Event::ASYNC_TURN_OFF:
    sData.startAccess();
    sData.setCmuxReady(false);
    sData.endAccess();
    itsModemCMux.turnOff();
    itsDeviceState.set(DeviceState::MODEM_OFF);
    break;

  case Event::ASYNC_TURN_ON:
    if (ICMuxDriver::Result::MUX_ON == itsModemCMux.turnOn())
    {
      SGM_LOG_DEBUG("PowerManager::processStateMachine : CMUX turned on");
      sData.startAccess();
      sData.setCmuxReady(true);
      sData.endAccess();
      itsDeviceState.set(DeviceState::MODEM_CMUX);
    }
    else
    {
      SGM_LOG_DEBUG("PowerManager::processStateMachine : could not turn the CMUX on");
      itsDeviceState.set(DeviceState::MODEM_FAILED);
    }
    break;


  case Event::NONE:
    SGM_LOG_DEBUG("PowerManager::processStateMachine : no-change event occured");
    break;

  case Event::UNDEFINED:
    SGM_LOG_ERROR("PowerManager::processStateMachine : undefined event raised");
    break;
  }

  SGM_LOG_INFO("Event processed; current device state : %d", itsDeviceState.get());

}

PowerManager::Event PowerManager::computeEvent()
{
  Event retVal = Event::UNDEFINED;

  if (!itsDeviceState.hasChanged())
  {
    return Event::NONE;
  }

  switch(itsDeviceState.get())
  {
  case DeviceState::MODEM_OFF:
    if (storedPowerState.is(ModemPowerController::PowerState::ENABLED))
    {
      retVal = Event::ASYNC_TURN_ON;
    }
    else
    {
      SGM_LOG_WARN("PowerManager::computeEvent() : no event triggered in state %d", storedPowerState.get());
      retVal = Event::NONE;
    }
    break;

  case DeviceState::MODEM_READY:
  case DeviceState::MODEM_CMUX:
    if ( (storedPowerState.is(ModemPowerController::PowerState::DISABLED_POWERED))
      || (storedPowerState.is(ModemPowerController::PowerState::DISABLED_UNPOWERED)) )
    {
      retVal = Event::ASYNC_TURN_OFF;
    }
    else
    {
      SGM_LOG_WARN("PowerManager::computeEvent() : event has no effect in state %d", storedPowerState.get());
      retVal = Event::NONE;
    }
    break;

  default:
    SGM_LOG_ERROR("PowerManager::computeEvent() : device not in defined state");
    break;

  }

  SGM_LOG_INFO("PowerManager::computeEvent : computed event %d", retVal);
  return retVal;

}

void PowerManager::determineInitialConditions()
{
  SGM_LOG_DEBUG("PowerManager::determineInitialConditions() : entering function");
  storedPowerState.set(itsPowerController.getPowerState());

  switch(storedPowerState.get())
  {
    case ModemPowerController::PowerState::ENABLED:
      itsDeviceState.set(DeviceState::MODEM_READY);
      SGM_LOG_INFO("PowerManager::determineInitialConditions() : initializing modem to %d state", DeviceState::MODEM_READY);
      break;

    case ModemPowerController::PowerState::DISABLED_POWERED:
    case ModemPowerController::PowerState::DISABLED_UNPOWERED:
      itsDeviceState.set(DeviceState::MODEM_OFF);
      SGM_LOG_INFO("PowerManager::determineInitialConditions() : initializing modem to %d state", DeviceState::MODEM_OFF);
      break;

    default:
      break;
  }
}

void PowerManager::waitForIncomingRequest()
{
}

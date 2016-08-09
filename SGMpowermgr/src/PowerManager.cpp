/*
 * PowerStateMachine.cpp
 *
 *  Created on: 19 kwi 2016
 *      Author: pkasprow
 */

#include "PowerManager.h"

#include "Logger.h"
#include "SGMfixtures.h"

#include "systemd/sd-daemon.h"

#include <thread>
#include <signal.h>

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

PowerManager::PowerManager(SharedMemory & sharedMem, ICMuxDriver & cMuxDriver, mraa::Uart & uartPort, ShutdownDriver & shdnDriver) :
    itsModemQuery(uartPort),
    itsSharedMemory(sharedMem),
    itsPowerController(sharedMem, itsModemQuery, std::bind(&PowerManager::onPowerIndChange, this)),
    itsModemCMux(cMuxDriver),
    itsShutdownDriver(shdnDriver)
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
      if (DeviceState::SHUTDOWN == processIncomingEvent(computeEvent()))
      {
        break;
      }
    }

  }

  powerControllerThread.join();
}

PowerManager::~PowerManager()
{
}

PowerManager::DeviceState PowerManager::processIncomingEvent(Event evType)
{
  SGM_LOG_DEBUG("PowerManager::processStateMachine() : event tick");

  SharedMemory::SharedData & sData = itsSharedMemory.getDataInstance();

  switch(evType)
  {

  case Event::SERVICE_SHUTDOWN:
//    TODO perhaps we need to have some method to deinitialize resources from
//    lower layer and process shutdown in a way that we call destructors
    return DeviceState::SHUTDOWN;

  case Event::TERMINATE:
    (void)itsPowerController.turnOffHw();
    return DeviceState::SHUTDOWN;

  case Event::ASYNC_TURN_OFF:
    sData.startAccess();
    sData.setCmuxReady(false);
    sData.endAccess();
    itsModemCMux.turnOff();
    itsDeviceState.set(DeviceState::MODEM_OFF);
    break;

  case Event::ASYNC_TURN_ON:
    SGM_LOG_INFO("Modem turned on, waiting to stabilize...");
    std::this_thread::sleep_for(std::chrono::seconds(5));
    if (ICMuxDriver::Result::MUX_ON == itsModemCMux.turnOn())
    {
      SGM_LOG_DEBUG("PowerManager::processStateMachine : CMUX turned on");
      sData.startAccess();
      sData.setCmuxReady(true);
      sData.endAccess();
      itsDeviceState.set(DeviceState::MODEM_CMUX);
      sd_notify(0, "READY=1");
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

  auto retVal = itsDeviceState.get();

  SGM_LOG_INFO("Event processed; current device state : %d", retVal);
  return retVal;

}

PowerManager::Event PowerManager::computeEvent()
{
  Event retVal = Event::UNDEFINED;

  if (!itsDeviceState.hasChanged())
  {
    //TODO detection of request
    retVal = itsEventRequest;
    itsEventRequest = Event::NONE;
    return retVal;
  }

  switch(itsDeviceState.get())
  {
  case DeviceState::MODEM_OFF:
    if (ShutdownDriver::ShutdownState::NORMAL_SHUTDOWN == itsShutdownDriver.getShutdownState())
    {
      SGM_LOG_INFO("Shutting down power manager");
      return Event::SERVICE_SHUTDOWN;
    }
    if (ShutdownDriver::ShutdownState::NORMAL_SHUTDOWN == itsShutdownDriver.getShutdownState())
    {
      SGM_LOG_FATAL("Unable to perform gentle shutdown, terminating Power Manager");
      return Event::TERMINATE;
    }

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

void PowerManager::executeShutdownRoutine()
{

}

/*
 * PowerStateMachine.h
 *
 *  Created on: 19 kwi 2016
 *      Author: pkasprow
 */

#ifndef POWERSTATEMACHINE_H_
#define POWERSTATEMACHINE_H_

#include "ICMuxDriver.h"

#include "ModemPowerController.h"
#include "ShutdownDriver.h"

#include "SharedMemory.h"
#include "ModemPresenceQuery.h"
#include "HistoricalValue.h"

#include <mraa/uart.hpp>

class PowerManager
{
public:

  enum class PowerState
  {
    MODEM_OFF,
    MODEM_ON_CMUX_OFF,
    MODEM_ON_CMUX_ON,
  };

  PowerManager(SharedMemory & sharedMem, ICMuxDriver & cMuxDriver, mraa::Uart & uartPort, ShutdownDriver & shdnDriver);

  PowerManager(const PowerManager &) = delete;
  PowerManager(const PowerManager &&) = delete;
  PowerManager & operator = (const PowerManager &) = delete;

  static void onPowerIndChange(void * data);

  void run();
  virtual ~PowerManager();

private:

  enum class DeviceState
  {
    UNDEFINED,
    MODEM_READY,
    MODEM_CMUX,
    MODEM_OFF,
    MODEM_FAILED,
    SHUTDOWN
  };

  enum class Event
  {
    REQ_TURN_ON,
    REQ_TURN_OFF,
    ASYNC_TURN_ON,
    ASYNC_TURN_OFF,
    SERVICE_SHUTDOWN,
    TERMINATE,
    NONE,
    UNDEFINED
  };

  Event computeEvent();

  DeviceState processIncomingEvent(Event evType);

  void determineInitialConditions();

  void waitForIncomingRequest();

  void executeShutdownRoutine();

  HistoricalValue<DeviceState> itsDeviceState{DeviceState::UNDEFINED};

  HistoricalValue<ModemPowerController::PowerState> storedPowerState{ModemPowerController::PowerState::UNDEFINED};

  ModemPresenceQuery itsModemQuery;
  SharedMemory & itsSharedMemory;
  ModemPowerController itsPowerController;
  ICMuxDriver & itsModemCMux;
  ShutdownDriver & itsShutdownDriver;


  std::condition_variable itsCondVariable;
  std::mutex eventMutex;
  std::mutex processingMutex;
  Event itsEventRequest{Event::NONE};

};

#endif /* POWERSTATEMACHINE_H_ */

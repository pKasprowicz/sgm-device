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

  PowerManager(SharedMemory & sharedMem, ICMuxDriver & cMuxDriver, mraa::Uart & uartPort);

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
    MODEM_FAILED
  };

  enum class Event
  {
    REQ_TURN_ON,
    REQ_TURN_OFF,
    ASYNC_TURN_ON,
    ASYNC_TURN_OFF,
    NONE,
    UNDEFINED
  };

  Event computeEvent();

  void processIncomingEvent(Event evType);

  void determineInitialConditions();

  void waitForIncomingRequest();

  HistoricalValue<DeviceState> itsDeviceState{DeviceState::UNDEFINED};

  HistoricalValue<ModemPowerController::PowerState> storedPowerState{ModemPowerController::PowerState::UNDEFINED};

  ModemPresenceQuery itsModemQuery;
  SharedMemory & itsSharedMemory;
  ModemPowerController itsPowerController;
  ICMuxDriver & itsModemCMux;


  std::condition_variable itsCondVariable;
  std::mutex eventMutex;
  std::mutex processingMutex;

};

#endif /* POWERSTATEMACHINE_H_ */

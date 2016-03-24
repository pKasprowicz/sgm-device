/*
 * ModemPowerController.h
 *
 *  Created on: 20 mar 2016
 *      Author: pkasprow
 */

#ifndef MODEMPOWERCONTROLLER_H_
#define MODEMPOWERCONTROLLER_H_

#include <IModemQuery.h>
#include <SharedMemory.h>

#include <mraa/gpio.hpp>

#include <mutex>
#include <condition_variable>

class ModemPowerController
{
public:

  enum class PowerState
  {
    UNDEFINED,
    DISABLED_POWERED,
    ENABLED,
    DISABLED_UNPOWERED
  };

  typedef void (*IsrHandler)(void * data);

  ModemPowerController(SharedMemory & sharedMem, IModemQuery & powerQuery, IsrHandler powerChangeHandler);
  ModemPowerController(const ModemPowerController &) = delete;
  ModemPowerController& operator=(const ModemPowerController &) = delete;

  virtual ~ModemPowerController();

  PowerState turnOn();
  PowerState turnOff(bool keepPower = false);
  PowerState reset();

  PowerState getPowerState()
  {
    return itsPowerState;
  }

  void operator()();

private:

  void stabilizePowerState();

  static void onPowerIndChange(void * data);

  void powerIndChangeHandler();

  mraa::Gpio modemPowerSetPin;
  mraa::Gpio modemEnablePin;
  mraa::Gpio modemPowerIndPin;
  mraa::Gpio battStatusPin;

  IModemQuery & itsModemStatusQuery;
  SharedMemory & itsSharedMemory;
  PowerState itsPowerState;
  IsrHandler itsIsrHandler;

  std::mutex itsLockingMutex;
  std::condition_variable itsCondVar;

  bool powerStateChanged;

  static const int PowerStatusPinNumber   = 45; //GP45
  static const int BatteryStatusPinNumber = 0;
  static const int ModemPowerPinNumber    = 46; //GP47
  static const int ModemEnablePinNumber   = 38; //GP43

};

#endif /* MODEMPOWERCONTROLLER_H_ */

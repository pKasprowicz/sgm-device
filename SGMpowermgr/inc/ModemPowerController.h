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
  typedef void * IsrData;

  ModemPowerController(SharedMemory & sharedMem, IModemQuery & powerQuery, std::function<void(void)> isrCallback);
  ModemPowerController(const ModemPowerController &) = delete;
  ModemPowerController& operator=(const ModemPowerController &) = delete;

  virtual ~ModemPowerController();

  PowerState turnOn();
  PowerState turnOffHw(bool keepPower = false);
  PowerState resetHw();

  PowerState turnOffSoft(bool keepPower = false);
  PowerState resetSoft();

  PowerState getPowerState()
  {
    return itsPowerState;
  }

  void operator()();

private:

  void stabilizePowerState();

  static void powerIndicationIsr(void * data);

  void powerIndChangeHandler();

  mraa::Gpio modemPowerSetPin;
  mraa::Gpio modemEnablePin;
  mraa::Gpio modemPowerIndPin;
  mraa::Gpio battStatusPin;

  IModemQuery & itsModemStatusQuery;
  SharedMemory & itsSharedMemory;
  PowerState itsPowerState;
  std::function<void(void)> itsIsrCallback;

  std::mutex itsLockingMutex;
  std::condition_variable itsCondVar;

  bool powerStateChanged{false};

  static const int PowerStatusPinNumber   = 45; //GP45
  static const int BatteryStatusPinNumber = 0;
  static const int ModemPowerPinNumber    = 46; //GP47
  static const int ModemEnablePinNumber   = 38; //GP43

};

#endif /* MODEMPOWERCONTROLLER_H_ */

/*
 * ModemPowerController.cpp
 *
 *  Created on: 20 mar 2016
 *      Author: pkasprow
 */

#define MODEM_IGNITION  (1)
#define MODEM_LINE_IDLE (0)

#define MODEM_POWER_ON  (1)
#define MODEM_POWER_OFF (0)

/*
 * modem operation line states
 * 0 = modem activated
 * 1 = modem deactivated
 */
#define MODEM_IND_POWER_ON  (0)
#define MODEM_IND_POWER_OFF (1)

#include <ModemPowerController.h>
#include <Logger.h>

#include<unistd.h>

#include <chrono>
#include <thread>

ModemPowerController::ModemPowerController(SharedMemory & sharedMem, IModemQuery & powerQuery, std::function<void(void)> isrCallback) :
  modemPowerSetPin(ModemPowerPinNumber),
  modemEnablePin(ModemEnablePinNumber),
  //modemPowerIndPin(PowerStatusPinNumber),
  /*
   * TODO temporary constructor
   */
  modemPowerIndPin(45, true, true),
  battStatusPin(BatteryStatusPinNumber),
  itsModemStatusQuery(powerQuery),
  itsSharedMemory(sharedMem),
  itsPowerState(PowerState::UNDEFINED),
  itsIsrCallback(isrCallback),
  powerStateChanged(false)
{
  battStatusPin.dir(mraa::DIR_IN);
  modemPowerIndPin.dir(mraa::DIR_IN);

  if (MODEM_IND_POWER_ON == modemPowerIndPin.read())
  {
    /*
     * preserve current control lines status
     */
    modemEnablePin.dir(mraa::DIR_OUT);
    modemPowerSetPin.dir(mraa::DIR_OUT_HIGH);

    stabilizePowerState();
  }
  else
  {
    SGM_LOG_DEBUG("ModemPowerController::ModemPowerController() : modem disabled, not performing initial setup");
    modemPowerSetPin.dir(mraa::DIR_OUT_LOW);
    modemEnablePin.dir(mraa::DIR_OUT_LOW);
    itsPowerState = PowerState::DISABLED_UNPOWERED;
  }

  modemPowerIndPin.isr(mraa::EDGE_BOTH,  powerIndicationIsr, this);
}

ModemPowerController::~ModemPowerController()
{
}

void ModemPowerController::stabilizePowerState()
{
  SGM_LOG_DEBUG("ModemPowerController::stabilizePowerState()");

  const int modemPowerIndication  = modemPowerIndPin.read();

  SharedMemory::SharedData & sharedData = itsSharedMemory.getDataInstance();

  IModemQuery::Result modemQueryResult = IModemQuery::Result::NOK;

  switch (modemPowerIndication)
  {
  case MODEM_IND_POWER_ON:
    modemQueryResult = itsModemStatusQuery.run();

    SGM_LOG_DEBUG("ModemPowerController::stabilizePowerState() : MODEM_IND_POWER_ON");

    if (IModemQuery::Result::OK == modemQueryResult)
    {
      itsPowerState = PowerState::ENABLED;
      modemEnablePin.write(MODEM_LINE_IDLE);
      modemPowerSetPin.write(MODEM_POWER_ON);

      sharedData.startAccess();
      sharedData.setModemReady(true);
      sharedData.endAccess();

      return;
    }
    else
    {
      SGM_LOG_INFO("Modem powered but not responding. Unable to determine operation. Performing hard shutdown...");

      /*
       * Writing to power pins occurs after mutex acquisition in order to protect clients against writing to the socket when modem is
       * shutting down. That could cause unnecessary fault.
       */
      sharedData.startAccess();
      sharedData.setModemReady(false);
      sharedData.endAccess();

      modemPowerSetPin.write(MODEM_IND_POWER_OFF);
      modemEnablePin.write(MODEM_LINE_IDLE);
      sleep(1);
      modemPowerSetPin.write(MODEM_IND_POWER_OFF);
      itsPowerState = PowerState::DISABLED_UNPOWERED;
      SGM_LOG_INFO("Modem brought back to clean off state");
      return;
    }

    break;

  case MODEM_IND_POWER_OFF:

    SGM_LOG_DEBUG("ModemPowerController::stabilizePowerState() : MODEM_IND_POWER_OFF");

    sharedData.startAccess();
    sharedData.setModemReady(false);
    sharedData.endAccess();

    modemPowerSetPin.write(MODEM_IND_POWER_OFF);
    modemEnablePin.write(MODEM_LINE_IDLE);

    itsPowerState = PowerState::DISABLED_POWERED;
    break;

  }

}

ModemPowerController::PowerState ModemPowerController::turnOn()
{
  std::unique_lock<std::mutex>(itsLockingMutex);

  modemEnablePin.write(MODEM_IGNITION);
  modemPowerSetPin.write(MODEM_POWER_ON);

  return PowerState::ENABLED;
}

ModemPowerController::PowerState ModemPowerController::turnOffHw(bool keepPower)
{
//TODO implement keep power

  std::unique_lock<std::mutex>(itsLockingMutex);
  modemPowerSetPin.write(MODEM_POWER_OFF);

  return PowerState::DISABLED_POWERED;
}

ModemPowerController::PowerState ModemPowerController::resetHw()
{
  std::unique_lock<std::mutex>(itsLockingMutex);
  modemPowerSetPin.write(MODEM_POWER_OFF);
  modemEnablePin.write(MODEM_LINE_IDLE);
  sleep(1);
  modemPowerSetPin.write(MODEM_POWER_ON);
  modemEnablePin.write(MODEM_IGNITION);

  sleep(1);

  if (MODEM_IND_POWER_ON == modemPowerIndPin.read())
  {
    return PowerState::ENABLED;
  }

  SGM_LOG_WARN("Modem not responding after 1s, still waiting...");
  sleep(1);

  if (MODEM_IND_POWER_OFF == modemPowerIndPin.read())
  {
    SGM_LOG_ERROR("Unable to turn on the modem");
    return PowerState::UNDEFINED;
  }

  return PowerState::ENABLED;
}

ModemPowerController::PowerState ModemPowerController::turnOffSoft(bool keepPower)
{
  //send AT^SMSO and wait for ^SHUTDOWN URC
  return PowerState::DISABLED_POWERED;
}

ModemPowerController::PowerState ModemPowerController::resetSoft()
{
  //send AT^SMSO and wait for ^SHUTDOWN URC
  return PowerState::ENABLED;
}

void ModemPowerController::operator ()()
{
  while(1)
  {
    std::unique_lock<std::mutex> lk(itsLockingMutex);

    itsCondVar.wait(lk, [this]
                         {
                           return (true == this->powerStateChanged);
                         });

    powerIndChangeHandler();

    powerStateChanged = false;

    SGM_LOG_INFO("ModemPowerController::operator () : Power state change event handled");
  }
}

void ModemPowerController::powerIndicationIsr(void * data)
{
  SGM_LOG_DEBUG("ModemPowerController::powerIndicationIsr() : entering function");
  ModemPowerController & powerController = *(reinterpret_cast<ModemPowerController *>(data));

  std::unique_lock<std::mutex> eventLock(powerController.itsLockingMutex);
  powerController.powerStateChanged = true;

  SGM_LOG_DEBUG("ModemPowerController::powerIndicationIsr() : notifying involved thread");
  eventLock.unlock();

  powerController.itsCondVar.notify_all();
}

void ModemPowerController::powerIndChangeHandler()
{
    SGM_LOG_DEBUG("ModemPowerController::powerIndChangeHandler() : entering function");
    int modemPowerIndicationValue = modemPowerIndPin.read();
    SharedMemory::SharedData sharedData = itsSharedMemory.getDataInstance();

    sharedData.startAccess();
    switch (modemPowerIndicationValue)
    {
    case MODEM_IND_POWER_OFF:
      modemEnablePin.write(MODEM_LINE_IDLE);
      modemPowerSetPin.write(MODEM_POWER_OFF);

      sharedData.setModemReady(false);
      itsPowerState = PowerState::DISABLED_POWERED;
      SGM_LOG_INFO("ModemPowerController::powerIndChangeHandler : Modem disabled and powered");
      break;

    case MODEM_IND_POWER_ON:
      sleep(1);
      modemEnablePin.write(MODEM_LINE_IDLE);
      itsPowerState = PowerState::ENABLED;
      sharedData.setModemReady(true);
      SGM_LOG_INFO("ModemPowerController::powerIndChangeHandler : Modem powered and ready");
      break;
    }
    sharedData.endAccess();
    itsIsrCallback();
}

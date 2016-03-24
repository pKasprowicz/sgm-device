/*
 * ModemPowerController.cpp
 *
 *  Created on: 20 mar 2016
 *      Author: pkasprow
 */

#define MODEM_LOGICAL_ENABLE  (1)
#define MODEM_LOGICAL_DISABLE (0)

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

std::mutex ModemPowerController::ItsAccessMutex();

ModemPowerController::ModemPowerController(SharedMemory & sharedMem, IModemQuery & powerQuery, IsrHandler powerChangeHandler) :
  modemPowerSetPin(ModemPowerPinNumber),
  modemEnablePin(ModemEnablePinNumber),
  //modemPowerIndPin(PowerStatusPinNumber),
  /*
   * TODO temporary constructor
   */
  modemPowerIndPin(134, true, true),
  battStatusPin(BatteryStatusPinNumber),
  itsModemStatusQuery(powerQuery),
  itsSharedMemory(sharedMem),
  itsPowerState(PowerState::UNDEFINED),
  itsIsrHandler(powerChangeHandler)
{
  battStatusPin.dir(mraa::DIR_IN);
  modemPowerIndPin.dir(mraa::DIR_IN);

  if (1 == modemPowerIndPin.read())
  {
    /*
     * preserve current control lines status
     */
    modemPowerSetPin.dir(mraa::DIR_OUT);
    modemEnablePin.dir(mraa::DIR_OUT);

    stabilizePowerState();
  }
  else
  {
    modemPowerSetPin.dir(mraa::DIR_OUT_LOW);
    modemEnablePin.dir(mraa::DIR_OUT_LOW);
    itsPowerState = PowerState::DISABLED_UNPOWERED;
  }

  modemPowerIndPin.isr(mraa::EDGE_BOTH,  onPowerIndChange, this);
}

ModemPowerController::~ModemPowerController()
{
}

/*
 * Brings the modem to defined state.
 * Useful during (re)start and initialization of Power Manager.
 *
 * Spec:
 * * if modem is powered the modem query checks if the modem is responding
 * * should the query fail, the modem is restarted and the shared memory is updated
 * * if modem is powered and responding control lines are updated correspondingly and shared memory updated
 * * if modem is not powered it makes sure that control lines are in proper state and
 *   shared memory data is being synchronized
 */

void ModemPowerController::stabilizePowerState()
{
  const int modemPowerIndication  = modemPowerIndPin.read();

  SharedMemory::SharedData & sharedData = itsSharedMemory.getDataInstance();

  IModemQuery::Result modemQueryResult = IModemQuery::Result::NOK;

  switch (modemPowerIndication)
  {
  case MODEM_IND_POWER_ON:
    modemQueryResult = itsModemStatusQuery.run();

    if (IModemQuery::Result::OK == modemQueryResult)
    {
      itsPowerState = PowerState::ENABLED;
      modemPowerSetPin.write(MODEM_IND_POWER_ON);
      modemEnablePin.write(MODEM_LOGICAL_ENABLE);

      sharedData.startAccess();
      sharedData.setModemReady(true);
      sharedData.endAccess();

      return;
    }
    else
    {
      itsPowerState = PowerState::UNDEFINED;
      SGM_LOG_INFO("Modem powered but not responding. Unable to determine operation. Shutting down...");

      /*
       * Writing to power pins occurs after mutex acquisition in order to protect clients against writing to the socket when modem is
       * shutting down. That could cause unnecessary fault.
       */
      sharedData.startAccess();
      modemPowerSetPin.write(MODEM_IND_POWER_OFF);
      modemEnablePin.write(MODEM_LOGICAL_DISABLE);
      sharedData.setModemReady(false);
      sharedData.endAccess();

      itsPowerState = PowerState::DISABLED_UNPOWERED;

      return;
    }

    break;

  case MODEM_IND_POWER_OFF:
    modemEnablePin.write(MODEM_LOGICAL_DISABLE);

    sharedData.startAccess();
    sharedData.setModemReady(false);
    sharedData.endAccess();

    itsPowerState = PowerState::DISABLED_UNPOWERED;
    break;

  }

}

ModemPowerController::PowerState ModemPowerController::turnOn()
{
  if (PowerState::UNDEFINED == itsPowerState)
  {
    return PowerState::UNDEFINED;
  }

  if (PowerState::ENABLED == itsPowerState)
  {
    return PowerState::ENABLED;
  }

  SGM_LOG_INFO("Turning on the modem");

  modemPowerSetPin.write(MODEM_POWER_ON);
  modemEnablePin.write(MODEM_LOGICAL_ENABLE);

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

ModemPowerController::PowerState ModemPowerController::turnOff(bool keepPower)
{
  modemPowerSetPin.write(MODEM_POWER_OFF);

  PowerState expectedResult = PowerState::DISABLED_UNPOWERED;

  if (keepPower)
  {
    modemEnablePin.write(MODEM_LOGICAL_DISABLE);
    sleep(1);
    modemPowerSetPin.write(MODEM_POWER_ON);
    expectedResult = PowerState::DISABLED_POWERED;
  }

  if (MODEM_IND_POWER_OFF == modemPowerIndPin.read())
  {
    return expectedResult;
  }

  return PowerState::UNDEFINED;

}

ModemPowerController::PowerState ModemPowerController::reset()
{
  modemPowerSetPin.write(MODEM_POWER_OFF);
  modemEnablePin.write(MODEM_LOGICAL_DISABLE);
  sleep(1);
  modemPowerSetPin.write(MODEM_POWER_ON);
  modemEnablePin.write(MODEM_LOGICAL_ENABLE);

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

void ModemPowerController::onPowerIndChange(void * data)
{
  ModemPowerController * powerController = reinterpret_cast<ModemPowerController *>(data);

  int modemPowerIndicationValue = powerController->modemPowerIndPin.read();
  int modemPowerSetValue        = powerController->modemPowerSetPin.read();
  SharedMemory::SharedData sharedData = powerController->itsSharedMemory.getDataInstance();

  (void)(powerController->itsIsrHandler)(static_cast<void *>(&modemPowerIndicationValue));

  /*
   * Modem enable line should always be brought down to be ready to generate rising edge (tunr-on signal)
   */
  powerController->modemEnablePin.write(MODEM_LOGICAL_DISABLE);

  sharedData.startAccess();
  switch (modemPowerIndicationValue)
  {
  case MODEM_IND_POWER_OFF:
    SGM_LOG_INFO("Modem deactivated");
    switch (modemPowerSetValue)
    {
    case MODEM_POWER_OFF:
      powerController->itsPowerState = ModemPowerController::PowerState::DISABLED_UNPOWERED;
      SGM_LOG_INFO("Modem is unpowered");
      break;
    case MODEM_POWER_ON:
      powerController->itsPowerState = ModemPowerController::PowerState::DISABLED_POWERED;
      SGM_LOG_INFO("Modem remains powered");
      break;
    default:
      /*
       * That should not ever happen
       */
      powerController->itsPowerState = ModemPowerController::PowerState::UNDEFINED;
      break;
    }
    sharedData.setModemReady(false);
    SGM_LOG_INFO("Modem disabled");
    break;

  case MODEM_IND_POWER_ON:
    sharedData.setModemReady(true);
    SGM_LOG_INFO("Modem powered and ready");
    break;
  }
  sharedData.endAccess();

}

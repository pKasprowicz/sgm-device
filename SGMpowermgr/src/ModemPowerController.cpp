/*
 * ModemPowerController.cpp
 *
 *  Created on: 20 mar 2016
 *      Author: pkasprow
 */

#define MODEM_LOGICAL_ENABLE  (1)
#define MODEM_LOGICAL_DISABLE (0)

#define MODEM_POWER_ON  (0)
#define MODEM_POWER_OFF (1)

#define MODEM_IND_POWER_ON  (0)
#define MODEM_IND_POWER_OFF (1)

#include <ModemPowerController.h>
#include <Logger.h>

ProcessSharedFlag<bool> ModemPowerController::itsPowerIndChageFlag(false);

ModemPowerController::ModemPowerController(SharedMemory & sharedMem, IModemQuery & powerQuery, IsrHandler powerChangeHandler) :
  modemPowerSetPin(ModemPowerPinNumber),
  modemEnablePin(ModemEnablePinNumber),
  modemPowerIndPin(PowerStatusPinNumber),
  battStatusPin(BatteryStatusPinNumber),
  itsModemStatusQuery(powerQuery),
  itsSharedMemory(sharedMem),
  itsPowerState(PowerState::UNDEFINED),
  itsIsrHandler(powerChangeHandler)
{
  battStatusPin.dir(mraa::DIR_IN);
  modemPowerIndPin.dir(mraa::DIR_IN);
  modemPowerIndPin.isr(mraa::EDGE_BOTH,  onPowerIndChange, this);

  if (1 == modemPowerIndPin.read())
  {
    modemPowerSetPin.dir(mraa::DIR_OUT);
    modemEnablePin.dir(mraa::DIR_OUT);

    stabilizePowerState();
  }
  else
  {
    modemPowerSetPin.dir(mraa::DIR_OUT_HIGH);
    modemEnablePin.dir(mraa::DIR_OUT_LOW);
    itsPowerState = PowerState::UNPOWERED;
  }
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
      itsPowerState = PowerState::POWERED_ENABLED;
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

      itsPowerState = PowerState::UNPOWERED;

      return;
    }

    break;

  case MODEM_IND_POWER_OFF:
    modemEnablePin.write(MODEM_LOGICAL_DISABLE);

    sharedData.startAccess();
    sharedData.setModemReady(false);
    sharedData.endAccess();

    itsPowerState = PowerState::UNPOWERED;
    break;

  }

}


void ModemPowerController::onPowerIndChange(void * data)
{
  ModemPowerController * powerController = reinterpret_cast<ModemPowerController *>(data);

  int modemPowerIndication = powerController->modemPowerIndPin.read();
  SharedMemory::SharedData sharedData = powerController->itsSharedMemory.getDataInstance();

  (void)(powerController->itsIsrHandler)(static_cast<void *>(&modemPowerIndication));

  sharedData.startAccess();
  switch (modemPowerIndication)
  {
  case MODEM_IND_POWER_OFF:
    sharedData.setModemReady(false);
    break;

  case MODEM_IND_POWER_ON:
    sharedData.setModemReady(true);
    break;
  }
  sharedData.endAccess();

}

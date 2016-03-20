/*
 * ModemPowerController.cpp
 *
 *  Created on: 20 mar 2016
 *      Author: pkasprow
 */

#define MODEM_LOGICAL_ENABLE (1)
#define MODEM_LOGICAL_DISABLE (0)

#define MODEM_POWER_ON (0)
#define MODEM_POWER_OFF (1)

#include <ModemPowerController.h>


ModemPowerController::ModemPowerController(SharedMemory & sharedMem, IModemQuery & powerQuery, IsrHandler powerChangeHandler) :
  modemPowerIndPin(PowerStatusPinNumber),
  battStatusPin(BatteryStatusPinNumber),
  modemPowerPin(ModemPowerPinNumber),
  modemEnablePin(ModemEnablePinNumber),
  itsPowerQuery(powerQuery),
  itsSharedMemory(sharedMem),
  itsPowerState(PowerState::UNDEFINED)
{
  battStatusPin.dir(mraa::DIR_IN);
  modemPowerIndPin.dir(mraa::DIR_IN);
  modemPowerIndPin.isr(mraa::EDGE_BOTH,  onPowerIndChange, NULL);

  if (1 == modemPowerIndPin.read())
  {
    modemPowerPin.dir(mraa::DIR_OUT);
    modemEnablePin.dir(mraa::DIR_OUT);

    stabilizePowerState();
  }
  else
  {
    modemPowerPin.dir(mraa::DIR_OUT_HIGH);
    modemEnablePin.dir(mraa::DIR_OUT_LOW);
    itsPowerState = PowerState::UNPOWERED;
  }
}

ModemPowerController::~ModemPowerController()
{
}

void ModemPowerController::stabilizePowerState()
{
  const int modemPower  = modemPowerPin.read();
  const int modemEnable = modemEnablePin.read();

  IModemQuery::Result modemQueryResult = IModemQuery::Result::NOK;

  switch (modemPower)
  {
  case MODEM_POWER_ON:
    modemQueryResult = itsPowerQuery.run();

    if (IModemQuery::Result::OK == modemQueryResult)
      {
        itsPowerState = PowerState::POWERED_ENABLED;
        return;
      }

    break;

  case MODEM_POWER_OFF:
    modemEnablePin.write(MODEM_LOGICAL_DISABLE);
    itsPowerState == PowerState::UNPOWERED;
    break;
  }

  if ((MODEM_POWER_ON == modemPower) && (MODEM_LOGICAL_ENABLE == modemEnable))
  {
    IModemQuery::Result result = itsPowerQuery.run();

    if (IModemQuery::Result::OK == result)
    {
      itsPowerState = PowerState::POWERED_ENABLED;
      return;
    }
  }


  if (IModemQuery::Result::OK == modemQueryResult)
  {
    itsPowerState = PowerState::POWERED_ENABLED;
  }
  else
  {
    modemEnablePin.write(0);
    modemPowerPin.write(0);
  }
}


void ModemPowerController::onPowerIndChange(void * data)
{

}

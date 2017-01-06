/*
 * ModemPresenceQuery.cpp
 *
 *  Created on: 19 kwi 2016
 *      Author: pkasprow
 */

#include "ModemPresenceQuery.h"

#include "AtGenericResponse.h"

#include <unistd.h>

ModemPresenceQuery::ModemPresenceQuery(mraa::Uart & uartPort)
: commandAT(),
  itsUartPort(uartPort)
{
  // TODO Auto-generated constructor stub

}

ModemPresenceQuery::~ModemPresenceQuery()
{
  // TODO Auto-generated destructor stub
}

IModemQuery::Result ModemPresenceQuery::run()
{
  if (isCmuxPotentiallyActive())
  {
    SGM_LOG_DEBUG("ModemPresenceQuery::run : ttyGSM device detected, potentially CMUX is active");
    return IModemQuery::Result::NOK;
  }


  if (IAtCommand::Result::AT_SENT != commandAT.sendAt(itsUartPort, IAtCommand::CommandType::AT_EXECUTE))
  {
    SGM_LOG_ERROR("Error when sending query command");
    return IModemQuery::Result::NOK;
  }
    SGM_LOG_INFO("Querying for modem availability");
    std::unique_ptr<AtGenericResponse> resp = commandAT.getResponse();

    if (resp->getReturnCode() != AtGenericResponse::ReturnCode::OK)
    {
      SGM_LOG_ERROR("Modem serial port not responding");
      return IModemQuery::Result::NOK;
    }

  return IModemQuery::Result::OK_RAW;
}

bool ModemPresenceQuery::isCmuxPotentiallyActive()
{
 if (0 == access("/dev/ttyGSM2", F_OK))
 {
   return true;
 }
 else
 {
   return false;
 }
}

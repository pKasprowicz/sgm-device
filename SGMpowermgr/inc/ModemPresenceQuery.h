/*
 * ModemPresenceQuery.h
 *
 *  Created on: 19 kwi 2016
 *      Author: pkasprow
 */

#ifndef MODEMPRESENCEQUERY_H_
#define MODEMPRESENCEQUERY_H_

#include "IModemQuery.h"

#include "AtCommand.h"
#include <IAtCommand.h>
#include "bgs2/PlainAtInterpreter.h"
#include "bgs2/at/At.h"

#include <mraa/uart.hpp>

class ModemPresenceQuery : public IModemQuery
{
public:

  ModemPresenceQuery(mraa::Uart & uartPort);
  virtual ~ModemPresenceQuery();

  virtual Result run();

private:
  AtCommand<bgs2::PlainAtInterpreter, bgs2::At>commandAT;
  mraa::Uart & itsUartPort;

  bool isCmuxPotentiallyActive();
};

#endif /* MODEMPRESENCEQUERY_H_ */

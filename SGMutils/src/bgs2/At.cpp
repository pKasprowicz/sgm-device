/*
 * At.cpp
 *
 *  Created on: 31 mar 2016
 *      Author: pkasprow
 */

#include "bgs2/At.h"
#include "IAtCommand.h"

namespace bgs2
{

  At::At()
  {
    // TODO Auto-generated constructor stub

  }

IAtCommand::Result At::sendAt(mraa::Uart& _uart, int timeout)
{
  _uart.write(COMMAND_STRING, sizeof(COMMAND_STRING) - 1);


  uint32_t rxPos = 0U;
  size_t rxBytes = 0;
  while(_uart.dataAvailable(100))
  {
    rxPos += _uart.read(&rxBuffer[rxPos], RESPONSE_SIZE);
  }
  std::string response{rxBuffer, rxPos};
  return IAtCommand::Result::AT_OK;
}

const AtResponse& At::getResponse()
{
}

  At::~At()
  {
    // TODO Auto-generated destructor stub
  }


}



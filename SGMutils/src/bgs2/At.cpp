/*
 * At.cpp
 *
 *  Created on: 31 mar 2016
 *      Author: pkasprow
 */

#include "bgs2/At.h"

namespace bgs2
{

const char At::CommandString[]{"AT\r\n"};

  At::At()
  {
    // TODO Auto-generated constructor stub

  }

IAtCommand::Result At::sendAt(mraa::Uart& _uart, int timeout)
{
  _uart.write(CommandString, sizeof(CommandString));


  std::string response =_uart.read(&rxBuffer[0], ResponseSize);
}

const AtResponse& At::getResponse()
{
}

  At::~At()
  {
    // TODO Auto-generated destructor stub
  }


}



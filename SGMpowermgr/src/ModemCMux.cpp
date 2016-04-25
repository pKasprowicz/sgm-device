/*
 * ModemCMux.cpp
 *
 *  Created on: 19 kwi 2016
 *      Author: pkasprow
 */

#include "ModemCMux.h"

ModemCMux::ModemCMux()
{
  // TODO Auto-generated constructor stub

}

ModemCMux::~ModemCMux()
{
  // TODO Auto-generated destructor stub
}

ModemCMux::Result ModemCMux::turnOn()
{
  SGM_LOG_DEBUG("ModemCMux::turnOn() : Attempting to turn the CMUX on");
  return ModemCMux::Result::MUX_ON;
}

ModemCMux::Result ModemCMux::turnOff()
{
  SGM_LOG_DEBUG("ModemCMux::turnOff() : Attempting to turn the CMUX off");
  return ModemCMux::Result::MUX_OFF;
}

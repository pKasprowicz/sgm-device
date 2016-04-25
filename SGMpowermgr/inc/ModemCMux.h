/*
 * ModemCMux.h
 *
 *  Created on: 19 kwi 2016
 *      Author: pkasprow
 */

#ifndef MODEMCMUX_H_
#define MODEMCMUX_H_

#include "Logger.h"

class ModemCMux
{
public:

  enum class Result
  {
    MUX_ON,
    MUX_OFF,
    MUX_ERROR
  };

  ModemCMux();
  virtual ~ModemCMux();

  Result turnOn();
  Result turnOff();

private:

};

#endif /* MODEMCMUX_H_ */

/*
 * ModemPresenceQuery.h
 *
 *  Created on: 19 kwi 2016
 *      Author: pkasprow
 */

#ifndef MODEMPRESENCEQUERY_H_
#define MODEMPRESENCEQUERY_H_

#include "IModemQuery.h"

class ModemPresenceQuery : public IModemQuery
{
public:

  ModemPresenceQuery();
  virtual ~ModemPresenceQuery();

  virtual Result run();
};

#endif /* MODEMPRESENCEQUERY_H_ */

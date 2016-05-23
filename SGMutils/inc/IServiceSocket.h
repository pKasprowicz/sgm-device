/*
 * IServiceSocket.h
 *
 *  Created on: 23 maj 2016
 *      Author: pkasprow
 */

#ifndef ISERVICESOCKET_H_
#define ISERVICESOCKET_H_

#include <stdint.h>
#include <stddef.h>

class IServiceSocket
{
public:
  virtual ~IServiceSocket()
  {

  }

  virtual int receive(uint8_t * rxBuffer, const size_t rxSize) throw() = 0;
  virtual int send() throw() = 0;

private:
};

#endif /* ISERVICESOCKET_H_ */

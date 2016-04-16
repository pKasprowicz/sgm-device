/*
 * IAtCommand.h
 *
 *  Created on: 31 mar 2016
 *      Author: pkasprow
 */

#ifndef IATCOMMAND_H_
#define IATCOMMAND_H_

#include <mraa/uart.hpp>

#include <AtResponse.h>

#define RX_BUF_SIZE 200

class IAtCommand
{
public:

  enum class Result
  {
    AT_SENT,
    AT_UART_ERROR,
    AT_UNKNOWN_ERROR
  };

  enum class CommandType
  {
      AT_EXECUTE,
      AT_READ,
      AT_TEST,
      AT_WRITE
  };

  IAtCommand()
  {

  }
  virtual ~IAtCommand()
  {

  }

  virtual Result sendAt(mraa::Uart & _uart, CommandType cmdType, int timeout = 0) = 0;
  virtual const AtResponse & getResponse() = 0;

private:


};



#endif /* IATCOMMAND_H_ */

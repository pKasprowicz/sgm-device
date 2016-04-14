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

#define RX_BUF_SIZE 100

class IAtCommand
{
public:

  enum class Result
  {
    AT_OK,
    AT_ERROR,
    AT_ERROR_CMS,
    AT_ERROR_CME
  };

  IAtCommand()
  {

  }
  virtual ~IAtCommand()
  {

  }

  virtual Result sendAt(mraa::Uart & _uart, int timeout = 0) = 0;
  virtual const AtResponse & getResponse() = 0;

private:


};



#endif /* IATCOMMAND_H_ */

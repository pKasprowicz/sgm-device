/*
 * AtCommand.h
 *
 *  Created on: 16 kwi 2016
 *      Author: pkasprow
 */

#ifndef ATCOMMAND_H_
#define ATCOMMAND_H_

#include <IAtCommand.h>

#include <Logger.h>

#include<mraa/uart.hpp>

template<typename ResponseInterpreter, typename AtSpec> class AtCommand : public IAtCommand
{
  private:
    static char rxBuffer[RX_BUF_SIZE];

  public:

    AtCommand<ResponseInterpreter, AtSpec>() :
      itsAtSpec(AtSpec())
    {

    }

    AtCommand<ResponseInterpreter, AtSpec>(AtSpec && _spec) :
      itsAtSpec{_spec}
    {

    }

    AtCommand(const AtCommand &) = delete;
    AtCommand & operator = (const AtCommand &) = delete;

    virtual Result sendAt(mraa::Uart & _uart, CommandType cmdType, int timeout = 0)
    {
      int commandLength = itsAtSpec.renderCommand(&rxBuffer[0], CommandType::AT_EXECUTE);

      if(0 > commandLength)
      {
       return IAtCommand::Result::AT_UART_ERROR;
       SGM_LOG_ERROR("Could not get properly rendered AT command");
      }

      if (_uart.write(&rxBuffer[0], commandLength) != commandLength)
      {
        return IAtCommand::Result::AT_UART_ERROR;
        SGM_LOG_ERROR("Unable to send AT command");
      }

      unsigned int readBytes = 0U;

      while (_uart.dataAvailable(100))
      {
        readBytes += _uart.read(&rxBuffer[readBytes], RX_BUF_SIZE);
      }

      rxBuffer[readBytes] = '\0';
      ++readBytes;
      return IAtCommand::Result::AT_SENT;
    }

    virtual const AtResponse & getResponse()
    {
      return ResponseInterpreter::getResponse(rxBuffer);
    }

  private:

    AtSpec itsAtSpec;
};

template<typename ResponseInterpreter, typename AtSpec> char AtCommand<ResponseInterpreter, AtSpec>::rxBuffer[RX_BUF_SIZE];
#endif /* ATCOMMAND_H_ */

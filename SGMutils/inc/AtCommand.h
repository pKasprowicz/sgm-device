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

#include <memory>

template<typename ResponseInterpreter, typename AtSpec> class AtCommand : public IAtCommand
{
  private:
    static char txRxBuffer[RX_BUF_SIZE];

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
      int commandLength = itsAtSpec.renderCommand(&txRxBuffer[0], cmdType);

      if(0 > commandLength)
      {
       SGM_LOG_ERROR("Could not get properly rendered AT command");
       return IAtCommand::Result::AT_UART_ERROR;
      }

      if (_uart.write(&txRxBuffer[0], commandLength) != commandLength)
      {
        SGM_LOG_ERROR("Unable to send AT command");
        return IAtCommand::Result::AT_UART_ERROR;
      }

      /*
       * TODO somewhere here we need to start polling for incoming characters in case
       * of commands which takes long to process i.e. +COPS
       */

      unsigned int readBytes = 0U;

      while (_uart.dataAvailable(100))
      {
        readBytes += _uart.read(&txRxBuffer[readBytes], RX_BUF_SIZE);
      }

      txRxBuffer[readBytes] = '\0';
      ++readBytes;
      return IAtCommand::Result::AT_SENT;
    }

    virtual std::unique_ptr<AtGenericResponse> getResponse()
    {
      return ResponseInterpreter::getResponse(txRxBuffer, lastSentCommandType);
    }

  private:

    AtSpec itsAtSpec;
    CommandType lastSentCommandType = CommandType::AT_INVALID;
};

template<typename ResponseInterpreter, typename AtSpec> char AtCommand<ResponseInterpreter, AtSpec>::txRxBuffer[RX_BUF_SIZE];
#endif /* ATCOMMAND_H_ */

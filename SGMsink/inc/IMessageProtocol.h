/*
 * IMessageProtocol.h
 *
 *  Created on: 16 maj 2016
 *      Author: pkasprow
 */

#ifndef IMESSAGEPROTOCOL_H_
#define IMESSAGEPROTOCOL_H_

#include "INetworkProvider.h"

#include "SGMDataType.h"

class IMessageProtocol
{
public:

  enum class Result
  {
    MESSAGE_SENT,
    ERROR_PROTOCOL,
    ERROR_SOCKET,
    INPUT_DATA_INVALID,
    ERROR_UNKNOWN
  };

  virtual ~IMessageProtocol()
  {

  }

  virtual Result sendMessage(sgm::SgmProcessData & dataToSend) = 0;

  virtual bool connect() = 0;

  virtual bool disconnect() = 0;

  virtual void onNetworkStatusChange(INetworkProvider::NetworkStatus status) = 0;

private:
};



#endif /* IMESSAGEPROTOCOL_H_ */

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
    ERROR_UNKNOWN
  };

  virtual ~IMessageProtocol()
  {

  }

  virtual Result sendMessage(SgmDataType & dataToSend) = 0;

  virtual void onNetworkStatusChange(INetworkProvider::NetworkStatus status) = 0;

private:
};



#endif /* IMESSAGEPROTOCOL_H_ */

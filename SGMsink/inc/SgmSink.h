/*
 * SinkStateMachine.h
 *
 *  Created on: 7 mar 2016
 *      Author: pkasprow
 */

#ifndef SGMSINK_H_
#define SGMSINK_H_


#include "PppConnection.h"

#include "Logger.h"
#include "IServiceSocket.h"
#include "IMessageProtocol.h"
#include "INetworkProvider.h"
#include "SGMDataType.h"
#include "SinkSocket.h"

#include <string>

class SgmSink
  {
    public:
      SgmSink(IMessageProtocol & sinkProtoRef, INetworkProvider & pppConnRef);
      virtual ~SgmSink();

      void tick();

      void operator() ();

      enum class SinkState
      {
        NO_CONNECTION,
        CONNECTED,
        LISTENING_CONNECTED,
        LISTENING_DISCONNECTED
      };

      enum class ProcessingState
      {
        SENDING_ERROR,

      };

      struct SendingException : public std::exception
      {

        const char * what () const throw ()
        {
          return "Error when sending data";
        }

      };

    private:

      void processMessageQueue();

      static const uint32_t ReconnectTimeout;

      static const uint32_t MaxListenIterationsCount{5U};

      uint32_t listenIterationsCount;

      SinkState itsCurrentState;

      IMessageProtocol & itsSinkProtocol;

      INetworkProvider & itsPppConnection;

      sgm::SgmProcessData itsSgmDataPacket;
      SinkSocket<sgm::SgmProcessData> itsSinkSocket;

  };

#endif /* SGMSINK_H_ */

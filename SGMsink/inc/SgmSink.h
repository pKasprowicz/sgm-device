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

#include "mqtt/client.h"

#include <string>

class SgmSink
  {
    public:
      SgmSink(mqtt::client & mqttClientRef, PppConnection & pppConnRef);
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

    private:

      void processMessageQueue();

      static const uint32_t ReconnectTimeout;

      SinkState itsCurrentState;

      mqtt::client & itsMqttClient;
      PppConnection & itsPppConnection;

  };

#endif /* SGMSINK_H_ */

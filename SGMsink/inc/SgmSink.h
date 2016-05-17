/*
 * SinkStateMachine.h
 *
 *  Created on: 7 mar 2016
 *      Author: pkasprow
 */

#ifndef SGMSINK_H_
#define SGMSINK_H_

#include "mqtt/client.h"

#include "Logger.h"

#include <string>

class SgmSink
  {
    public:
      SgmSink();
      virtual ~SgmSink();

      void tick();

      void operator() ();

      enum class SinkState
      {
        NO_CONNECTION,
        MODEM_PRESENT,
        CONN_ESTABLISHED,
        ACTIVE,
        IDLE,
        ERROR_TCPIP,
        ERROR_PPP,
        RESTART_REQUESTED
      };

    private:

      std::string itsServerURI{"tcp://iot.eclipse.org:1883"};
      std::string itsClientId{"SGM#1"};

      mqtt::client itsMqttClient;

      SinkState itsCurrentState;

  };

#endif /* SGMSINK_H_ */

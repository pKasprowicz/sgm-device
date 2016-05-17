/*
 * SinkStateMachine.cpp
 *
 *  Created on: 7 mar 2016
 *      Author: pkasprow
 */

#include "SgmSink.h"

#include <stdlib.h>

SgmSink::SgmSink() :
itsMqttClient(itsServerURI, itsClientId)
{

}

SgmSink::~SgmSink()
{
	// TODO Auto-generated destructor stub
}

void SgmSink::tick()
{
  switch(itsCurrentState)
  {
    case SinkState::ACTIVE:
      break;

    case SinkState::CONN_ESTABLISHED:
      break;

    case SinkState::ERROR_PPP:
      break;

    case SinkState::ERROR_TCPIP:
      break;

    case SinkState::IDLE:
      break;

    case SinkState::MODEM_PRESENT:
      break;

      /*
       * Initial state of the SinkSM
       */
    case SinkState::NO_CONNECTION:
      break;

    case SinkState::RESTART_REQUESTED:
      break;
  }
}

void SgmSink::operator ()()
{
  const char message[] = "SGMmessage";
  std::string topic{"SGMtopic"};

  itsMqttClient.connect();
  if (!itsMqttClient.is_connected())
  {
    SGM_LOG_ERROR("Could not connect to MQTT server. Stopping");
    exit(-1);
  }
  itsMqttClient.publish(topic, message, sizeof(message), 1, false);
  itsMqttClient.disconnect();
}

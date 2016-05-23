/*
 * SinkStateMachine.cpp
 *
 *  Created on: 7 mar 2016
 *      Author: pkasprow
 */

#include "SgmSink.h"
#include "SinkSocket.h"

#include "SGMDataType.h"

#include <stdlib.h>
#include <thread>

const uint32_t SgmSink::ReconnectTimeout{10U};

SgmSink::SgmSink(mqtt::client & mqttClientRef, PppConnection & pppConnRef) :
  itsCurrentState(SinkState::NO_CONNECTION),
  itsMqttClient(mqttClientRef),
  itsPppConnection(pppConnRef)
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
    case SinkState::NO_CONNECTION:
    {

      auto status = itsPppConnection.connect();

      if (INetworkProvider::NetworkStatus::DEVICE_UNAVAILABLE == status)
      {
        SGM_LOG_ERROR("Network unavailable. waiting %d s for reconnection", ReconnectTimeout);
        std::this_thread::sleep_for(std::chrono::seconds(ReconnectTimeout));
      }
      if (INetworkProvider::NetworkStatus::CONNECTED != status)
      {
        SGM_LOG_WARN("Could not connect to Internet. Retrying...");
        std::this_thread::sleep_for(std::chrono::seconds(ReconnectTimeout));
      }
      else
      {
        SGM_LOG_INFO("Connected to internet!");
        itsCurrentState = SinkState::CONNECTED;
      }

      //TODO count iterations and kill if exceeded
      break;
    }

    case SinkState::CONNECTED:
    {
      itsMqttClient.connect();

      if (!itsMqttClient.is_connected())
      {
        SGM_LOG_ERROR("Could not connect to MQTT server. Resuming in %d seconds", ReconnectTimeout);
        std::this_thread::sleep_for(std::chrono::seconds(ReconnectTimeout));
      }
      else
      {
        itsCurrentState = SinkState::CONNECTED;
      }
      break;
    }

    case SinkState::LISTENING_CONNECTED:
      SGM_LOG_INFO("Connected to MQTT server. Processing started...");
      processMessageQueue();
      break;

    case SinkState::LISTENING_DISCONNECTED:
      break;

    default:
      break;
  }
}

void SgmSink::operator ()()
{
//  const char message[] = "SGMmessage";
//  std::string topic{"SGMtopic"};
//
//  itsMqttClient.connect();
//  if (!itsMqttClient.is_connected())
//  {
//    SGM_LOG_ERROR("Could not connect to MQTT server. Stopping");
//    exit(-1);
//  }
//  itsMqttClient.publish(topic, message, sizeof(message), 1, false);
//  itsMqttClient.disconnect();

  while(true)
  {
    tick();
  }
}

void SgmSink::processMessageQueue()
{
  SinkSocket<SgmDataType> socket();
}

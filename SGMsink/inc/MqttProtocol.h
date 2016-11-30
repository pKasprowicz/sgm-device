/*
 * MqttProtocol.h
 *
 *  Created on: 16 maj 2016
 *      Author: pkasprow
 */

#ifndef MQTTPROTOCOL_H_
#define MQTTPROTOCOL_H_

#include "IMessageProtocol.h"
#include "MQTTClient.h"

#include "mqtt/client.h"

#include <mutex>

class MqttProtocol : public IMessageProtocol
{
public:

  enum class ConnectionState
  {
    CONNECTED,
    DISCONNECTED
  };

  MqttProtocol(const std::string& serverURI, const std::string& clientId);
  virtual ~MqttProtocol();

  virtual IMessageProtocol::Result sendMessage(sgm::SgmProcessData & dataToSend);

  virtual bool connect();

  virtual bool disconnect();

  virtual void onNetworkStatusChange(INetworkProvider::NetworkStatus status);

private:

  bool prepareTopic(sgm::SgmProcessData & data);
  int prepareMessage(sgm::SgmProcessData & data);

  mqtt::client itsMqttClient;
  std::mutex itsEventMutex;
  bool isNetworkReady{false};

  std::string itsTopicBuffer;
  const std::string mqttClientId;
  uint8_t itsMesssageBuffer[100];
};

#endif /* MQTTPROTOCOL_H_ */

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

class MqttProtocol : public INetworkProvider
{
public:
  MqttProtocol();
  virtual ~MqttProtocol();

  virtual IMessageProtocol::Result sendMessage(SgmDataType & dataToSend);

  virtual void onNetworkStatusChange(INetworkProvider::NetworkStatus status);
};

#endif /* MQTTPROTOCOL_H_ */

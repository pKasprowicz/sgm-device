/*
 * MqttClient.h
 *
 *  Created on: 23.12.2016
 *      Author: pkasprow
 */

#ifndef SGMSINK_SRC_MQTTCLIENT_H_
#define SGMSINK_SRC_MQTTCLIENT_H_

extern "C"
{
	#include <MQTTClient.h>
}

#include <string>

class MqttClient{
public:

	enum class Result
	{
		CONNECTED,
		CONNECTION_ERROR,
		DISCONNECTED,
		PUBLISH_SUCCESS,
		PUBLISH_ERROR,
		ERROR
	};

	MqttClient(std::string & address, std::string & clientId);
	virtual ~MqttClient();

	Result connect();
	Result disconnect();

	Result publishMessage(std::string & topic, uint8_t * message, uint16_t messageSize, uint8_t qos, bool retained);

private:
	 MQTTClient itsClientHandle;
	 MQTTClient_deliveryToken itsDeliveryToken;
	 MQTTClient_connectOptions itsConnectionOptions = MQTTClient_connectOptions_initializer;
	 MQTTClient_message itsPubMessage = MQTTClient_message_initializer;

	 bool isConnected = false;

	static const uint32_t PublishTimeout{1000};
};

#endif /* SGMSINK_SRC_MQTTCLIENT_H_ */

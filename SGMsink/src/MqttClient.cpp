/*
 * MqttClient.cpp
 *
 *  Created on: 23.12.2016
 *      Author: pkasprow
 */

#include "MqttClient.h"

MqttClient::MqttClient(std::string & address, std::string & clientId)
{
	MQTTClient_create(&itsClientHandle,
					const_cast<char *>(address.c_str()),
					const_cast<char *>(clientId.c_str()),
					MQTTCLIENT_PERSISTENCE_NONE,
					NULL);

	itsConnectionOptions.keepAliveInterval = 40;
	itsConnectionOptions.cleansession = 1;
}

MqttClient::~MqttClient()
{
	MQTTClient_destroy(&itsClientHandle);
}

MqttClient::Result MqttClient::connect()
{

	if(isConnected)
	{
		return Result::CONNECTED;
	}

	if(MQTTClient_connect(itsClientHandle, &itsConnectionOptions) != MQTTCLIENT_SUCCESS)
	{
		return Result::CONNECTION_ERROR;
	}

	isConnected = true;
	return Result::CONNECTED;
}

MqttClient::Result MqttClient::disconnect()
{
	MQTTClient_disconnect(itsClientHandle, 2000);
	isConnected = false;

	return Result::DISCONNECTED;
}

MqttClient::Result MqttClient::publishMessage(std::string &topic, uint8_t *message, uint16_t messageSize, uint8_t qos,
											  bool retained)
{
	int rc = MQTTCLIENT_SUCCESS;

	itsPubMessage = { {'M', 'Q', 'T', 'M'}, 0, 0, NULL, 0, 0, 0, 0 };
	itsPubMessage.payload = message;
	itsPubMessage.payloadlen = messageSize;
	itsPubMessage.qos = qos;

	MQTTClient_publish(itsClientHandle, const_cast<char *>(topic.c_str()), messageSize, message, qos, retained, &itsDeliveryToken);
	rc = MQTTClient_waitForCompletion(itsClientHandle, itsDeliveryToken, PublishTimeout);

	if (MQTTCLIENT_FAILURE == rc)
	{
		return Result::PUBLISH_ERROR;
	}
	else
	{
		return Result::PUBLISH_SUCCESS;
	}
}

void MqttClient::yield()
{
	MQTTClient_yield();
}
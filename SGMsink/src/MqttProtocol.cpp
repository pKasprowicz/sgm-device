/*
 * MqttProtocol.cpp
 *
 *  Created on: 16 maj 2016
 *      Author: pkasprow
 */

#include "MqttProtocol.h"

#include "Logger.h"

#include <cstring>
#include <map>

MqttProtocol::MqttProtocol(const std::string& serverURI, const std::string& clientId)
: itsMqttClient(serverURI,clientId),
  itsTopicBuffer{""},
  mqttClientId(clientId)
{
  // TODO Auto-generated constructor stub

}

MqttProtocol::~MqttProtocol()
{
  // TODO Auto-generated destructor stub
}

IMessageProtocol::Result MqttProtocol::sendMessage(sgm::SgmProcessData& dataToSend)
{

  auto retVal = IMessageProtocol::Result::MESSAGE_SENT;

  if (!prepareTopic(dataToSend))
  {
    return IMessageProtocol::Result::INPUT_DATA_INVALID;
  }

  int messageSize = prepareMessage(dataToSend);

  if (messageSize < 0)
  {
      return IMessageProtocol::Result::INPUT_DATA_INVALID;
  }

  std::unique_lock<std::mutex> lock(itsEventMutex);

  if (isNetworkReady)
  {
    itsMqttClient.publish(itsTopicBuffer, itsMesssageBuffer, messageSize, 1, true);
  }
  else
  {
    SGM_LOG_WARN("Network unavailable at the moment");
    retVal = IMessageProtocol::Result::ERROR_PROTOCOL;
  }
  lock.unlock();

  return retVal;
}

bool MqttProtocol::disconnect()
{
  itsMqttClient.disconnect();
  return !itsMqttClient.is_connected();
}

void MqttProtocol::onNetworkStatusChange(INetworkProvider::NetworkStatus status)
{
  SGM_LOG_DEBUG("MqttProtocol::onNetworkStatusChange() : locking mutex");
  std::lock_guard<std::mutex> lock(itsEventMutex);
  switch(status)
  {
  case INetworkProvider::NetworkStatus::CONNECTED:
    SGM_LOG_DEBUG("MqttProtocol::onNetworkStatusChange() : network ready");
    isNetworkReady = true;
    break;

  default:
    SGM_LOG_DEBUG("MqttProtocol::onNetworkStatusChange() : network unavailable");
    isNetworkReady = false;
    break;
  }
}

bool MqttProtocol::connect()
{
  try
  {
  itsMqttClient.connect();
  }
  catch(mqtt::exception & ex)
  {
    SGM_LOG_ERROR("MQTT stack exception thrown");
    return false;
  }
  SGM_LOG_DEBUG("MqttProtocol::connect() : connected");
  return itsMqttClient.is_connected();
}

//TODO think of a better parser (objective)
bool MqttProtocol::prepareTopic(sgm::SgmProcessData& data)
{
  itsTopicBuffer.assign("sgm/");

  bool isMapperFound = false;

  SGM_LOG_DEBUG("MqttProtocol::prepareTopic : %d / %d", data.physQuantityType, data.measPoint);
  SGM_LOG_DEBUG("MqttProtocol::prepareTopic : %s / %s", sgm::physicalStringMap[data.physQuantityType].c_str(), sgm::measPointStringMap[data.measPoint].c_str());

  for (auto & unitMapper : sgm::unitMap)
  {
    if ((unitMapper.first == data.measPoint) && (unitMapper.second == data.physQuantityType))
    {
      isMapperFound = true;
      itsTopicBuffer += sgm::measPointStringMap.at(unitMapper.first);
      itsTopicBuffer += "/";
      itsTopicBuffer += sgm::physicalStringMap.at(unitMapper.second);
      break;
    }
  }

  return isMapperFound;
}

int MqttProtocol::prepareMessage(sgm::SgmProcessData& data)
{
  size_t pos = 0U;


  std::memcpy(&itsMesssageBuffer[pos], reinterpret_cast<void *>(&data.value), sizeof(data.value));
  pos += sizeof(data.value);

  SGM_LOG_DEBUG("MqttProtocol::prepareMessage : raw value : %d", data.value);

  std::memcpy(&itsMesssageBuffer[pos], reinterpret_cast<void *>(&data.divider), sizeof(data.divider));
  pos += sizeof(data.divider);

  SGM_LOG_DEBUG("MqttProtocol::prepareMessage : divider: %d", data.divider);

  std::memcpy(&itsMesssageBuffer[pos], reinterpret_cast<void *>(&data.unit), sizeof(data.unit));
  pos += sizeof(data.unit);

  std::memcpy(&itsMesssageBuffer[pos], reinterpret_cast<void *>(&data.timestamp), sizeof(data.timestamp));
  pos += sizeof(data.timestamp);

  return pos;
}

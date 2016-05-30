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
#include <functional>

const uint32_t SgmSink::ReconnectTimeout{10U};

SgmSink::SgmSink(IMessageProtocol & sinkProtoRef, INetworkProvider & pppConnRef) :
  itsCurrentState(SinkState::NO_CONNECTION),
  itsSinkProtocol(sinkProtoRef),
  itsPppConnection(pppConnRef)
{
  itsPppConnection.registerUserCallback(std::bind(&IMessageProtocol::onNetworkStatusChange, &itsSinkProtocol, std::placeholders::_1));
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
      if (!itsSinkProtocol.connect())
      {
        SGM_LOG_ERROR("Could not connect to MQTT server. Resuming in %d seconds", ReconnectTimeout);
        std::this_thread::sleep_for(std::chrono::seconds(ReconnectTimeout));
      }
      else
      {
        SGM_LOG_INFO("Connected to sink protocol!");
        itsCurrentState = SinkState::LISTENING_CONNECTED;
      }
      break;
    }

    case SinkState::LISTENING_CONNECTED:
      SGM_LOG_INFO("Connected to MQTT server. Processing started...");
      try
      {
        processMessageQueue();
        std::this_thread::sleep_for(std::chrono::seconds(10U));
      }
      catch (SendingException & ex)
      {
        (void)itsSinkProtocol.disconnect();
        (void)itsPppConnection.disconnect();
        SGM_LOG_FATAL("Fatal error when processing messages (socket). Waiting %d seconds for resuming...", ReconnectTimeout);
        std::this_thread::sleep_for(std::chrono::seconds(ReconnectTimeout));

        itsCurrentState = SinkState::NO_CONNECTION;
      }
      catch (std::runtime_error & ex)
      {
        SGM_LOG_FATAL("Fatal error when processing messages (runtime error). Waiting %d seconds for resuming...", ReconnectTimeout);
        std::this_thread::sleep_for(std::chrono::seconds(ReconnectTimeout));
      }
      break;

    case SinkState::LISTENING_DISCONNECTED:
      std::this_thread::sleep_for(std::chrono::seconds(ReconnectTimeout));
      break;

    default:
      break;
  }
}

void SgmSink::operator ()()
{
  while(true)
  {
    tick();
  }
}

void SgmSink::processMessageQueue()  throw ()
{
  size_t rxSize = 0U;
  auto sendingResult = IMessageProtocol::Result::ERROR_PROTOCOL;

  SinkSocket<sgm::SgmProcessData> sinkSocket;
  while (true)
  {
    rxSize = sinkSocket.receive(& itsSgmDataPacket);
    if (0U == rxSize)
    {
      break;
    }
    if (sizeof(sgm::SgmProcessData) > rxSize )
    {
      throw std::runtime_error("Bad packet received : size mismatch");
    }

    //TODO validation

    sendingResult = itsSinkProtocol.sendMessage(itsSgmDataPacket);

    switch(sendingResult)
    {
    case IMessageProtocol::Result::MESSAGE_SENT:
      SGM_LOG_INFO("Message sent!");
      break;

    case IMessageProtocol::Result::ERROR_DATA_INVALID:
      SGM_LOG_ERROR("Data invalid - packet dropped");
      break;

    case IMessageProtocol::Result::ERROR_SOCKET:
      SGM_LOG_ERROR("Error when sending - internal socket error");
      throw SendingException();
      break;

    case IMessageProtocol::Result::ERROR_PROTOCOL:
      SGM_LOG_ERROR("Error on sending message - protocol fault");
      itsCurrentState = SinkState::LISTENING_DISCONNECTED;
      break;

    case IMessageProtocol::Result::ERROR_UNKNOWN:
      SGM_LOG_FATAL("Unknown error when sending message");
      throw SendingException();
      break;

    default:
      SGM_LOG_FATAL("Unhandled return code");
      throw SendingException();
      break;
    }
  }
  //TODO check crc of the packet
}

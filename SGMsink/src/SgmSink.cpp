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
  listenIterationsCount{0U},
  itsCurrentState(SinkState::NO_CONNECTION),
  itsSinkProtocol(sinkProtoRef),
  itsPppConnection(pppConnRef)
{
  SGM_LOG_DEBUG("SgmSink constructor called");
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
        listenIterationsCount = 0U;
        itsCurrentState = SinkState::LISTENING_CONNECTED;
      }
      break;
    }

    case SinkState::LISTENING_CONNECTED:
      SGM_LOG_INFO("Listening for incoming packets, attempt %d / %d", listenIterationsCount+1, MaxListenIterationsCount);
      try
      {
        processMessageQueue();
        std::this_thread::sleep_for(std::chrono::seconds(10U));
        ++listenIterationsCount;
      }
      catch (SendingException & ex)
      {
        (void)itsSinkProtocol.disconnect();
        SGM_LOG_FATAL("Fatal error when processing messages. Waiting %d seconds for resuming...", ReconnectTimeout);
        std::this_thread::sleep_for(std::chrono::seconds(ReconnectTimeout));

        listenIterationsCount = 0U;
        itsCurrentState = SinkState::NO_CONNECTION;
      }
      catch (std::runtime_error & ex)
      {
        SGM_LOG_FATAL("Fatal error when processing messages (runtime error). Waiting %d seconds for resuming...", ReconnectTimeout);
        std::this_thread::sleep_for(std::chrono::seconds(ReconnectTimeout));
      }
      break;

    case SinkState::LISTENING_DISCONNECTED:
      //TODO rework the code to utilize this state
      std::this_thread::sleep_for(std::chrono::seconds(ReconnectTimeout));
      ++listenIterationsCount;
      break;

    default:
      break;
  }
}

void SgmSink::operator ()()
{
  SGM_LOG_INFO("SgmSink main thread started");
  while(listenIterationsCount < MaxListenIterationsCount)
  {
    tick();
  }

  SGM_LOG_INFO("Idle time exceeded, exiting...");
  itsSinkProtocol.disconnect();
  SGM_LOG_INFO("Protocol disconnected..");
  itsPppConnection.disconnect();
  SGM_LOG_INFO("PPP connection finished...");
  std::this_thread::sleep_for(std::chrono::seconds(5));
}

void SgmSink::processMessageQueue()
{
  int rxSize = 0;

  SGM_LOG_DEBUG("SgmSink::processMessageQueue()");

  int rc = itsSinkSocket.connect();
  if ((EWOULDBLOCK == rc) || (EAGAIN == rc))
  {
    SGM_LOG_INFO("Not processing any messages");
    return;
  }
  else
  {
    listenIterationsCount = 0U;
  }

  while (true)
  {
    rxSize = itsSinkSocket.receive(& itsSgmDataPacket);
    if ((EAGAIN == rxSize) || (EWOULDBLOCK == rxSize) || (0 == rxSize))
    {
      itsSinkSocket.disconnect();
      itsSinkProtocol.yield();
      return;
    }
    if (sizeof(sgm::SgmProcessData) > rxSize )
    {
      throw std::runtime_error("Bad packet received : size mismatch");
    }

    //TODO validation

    auto sendingResult = itsSinkProtocol.sendMessage(itsSgmDataPacket);

    switch(sendingResult)
    {
    case IMessageProtocol::Result::MESSAGE_SENT:
      SGM_LOG_INFO("Message sent!");
      break;

    case IMessageProtocol::Result::INPUT_DATA_INVALID:
      SGM_LOG_ERROR("Data invalid - packet dropped");
      break;

    case IMessageProtocol::Result::NO_CONNECTION:
      SGM_LOG_ERROR("The client is not connected");
      throw SendingException();


    default:
      SGM_LOG_FATAL("Unhandled return code - ignoring");
      break;
    }
  }
  //TODO check crc of the packet
}

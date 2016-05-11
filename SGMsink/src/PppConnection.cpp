/*
 * PppConnection.cpp
 *
 *  Created on: 11 maj 2016
 *      Author: pkasprow
 */

#include <PppConnection.h>

#include "Logger.h"

#include <stdlib.h>
PppConnection::PppConnection()
{
  callbackList.reserve(InitialCallbacksCount);
}

PppConnection::~PppConnection()
{
  // TODO Auto-generated destructor stub
}

void PppConnection::registerUserCallback(NetworkStatusCallback netCallback)
{
  if (InitialCallbacksCount == callbackList.size())
  {
    SGM_LOG_WARN("Exceeding default amount of callbacks for the PppConnection manager");
  }

  callbackList.push_back(std::forward<NetworkStatusCallback>(netCallback));
}

INetworkProvider::NetworkStatus PppConnection::connect()
{
  auto retVal = INetworkProvider::NetworkStatus::CONNECTION_ERROR;

  if (!isDeviceAvailable())
  {
    (void)stateMachineTick(Event::DEVICE_LOST);
    return INetworkProvider::NetworkStatus::DEVICE_UNAVAILABLE;
  }

  if(itsCurrentState == State::CONNECTED)
  {
    return INetworkProvider::NetworkStatus::CONNECTED;
  }

  if (State::CONNECTED == stateMachineTick(Event::CONNECT_REQUEST))
  {
    retVal = INetworkProvider::NetworkStatus::CONNECTED;
  }

  return retVal;
}

INetworkProvider::NetworkStatus PppConnection::disconnect()
{
  auto retVal = INetworkProvider::NetworkStatus::CONNECTION_ERROR;

  if (!isDeviceAvailable())
  {
    (void)stateMachineTick(Event::DEVICE_LOST);
    return INetworkProvider::NetworkStatus::DISCONNECTED;
  }

  if(itsCurrentState == State::NO_CONNECTION)
  {
    return INetworkProvider::NetworkStatus::DISCONNECTED;
  }

  if (State::CONNECTED == stateMachineTick(Event::DISCONNECT_REQUEST))
  {
    retVal = INetworkProvider::NetworkStatus::DISCONNECTED;
  }

  return retVal;
}

INetworkProvider::NetworkStatus PppConnection::queryStatus()
{
  auto status = INetworkProvider::NetworkStatus::UNKNOWN_ERROR;

  switch (itsCurrentState)
  {
  case State::CONNECTED:
    status = INetworkProvider::NetworkStatus::CONNECTED;
    break;

  case State::DEVICE_UNAVAILABLE:
    status = INetworkProvider::NetworkStatus::DEVICE_UNAVAILABLE;
    break;

  case State::NO_CONNECTION:
    status = INetworkProvider::NetworkStatus::DISCONNECTED;
    break;

  case State::CONNECTION_LOST:
    status = INetworkProvider::NetworkStatus::DEVICE_UNAVAILABLE;
    break;
  }

  return status;
}

bool PppConnection::isDeviceAvailable()
{
}

PppConnection::State PppConnection::stateMachineTick(Event ev)
{
  std::lock_guard<std::mutex> eventLock(itsEventMutex);

  switch (ev)
  {
  case Event::CONNECT_REQUEST:
    if (itsCurrentState != State::NO_CONNECTION)
    {
      return itsCurrentState;
    }

    if(itsPppEndpoint.open())
    {
      itsCurrentState = State::CONNECTED;
      startPowerMonitor();
    }
    else
    {
      SGM_LOG_ERROR("Could not establish ppp connection");
      itsCurrentState = State::NO_CONNECTION;
    }

    break;

  case Event::DISCONNECT_REQUEST:
    if(
        (itsCurrentState == State::CONNECTED) ||
        (itsCurrentState == State::CONNECTION_LOST)
        )
    {
      pausePowerMonitor();
    }
    break;

  case Event::DEVICE_LOST:
    break;

  case Event::DEVICE_RESTORED:
    break;
  }

  return itsCurrentState;
}

void PppConnection::notifyByCallback(INetworkProvider::NetworkStatus & status)
{
  for (auto callback : callbackList)
  {
    callback(status);
  }
}

void PppConnection::startPowerMonitor()
{
}

void PppConnection::pausePowerMonitor()
{
}

void PppConnection::powerMonitorThread()
{
}

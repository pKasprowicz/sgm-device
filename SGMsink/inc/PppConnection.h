/*
 * PppConnection.h
 *
 *  Created on: 11 maj 2016
 *      Author: pkasprow
 */

#ifndef PPPCONNECTION_H_
#define PPPCONNECTION_H_

#include "INetworkProvider.h"
#include "PppEndpoint.h"

#include <vector>
#include <mutex>

class PppConnection :public INetworkProvider
{
public:
  PppConnection();

  PppConnection(const PppConnection &) = delete;
  PppConnection & operator =(const PppConnection &) = delete;

  virtual ~PppConnection();

  virtual void registerUserCallback(NetworkStatusCallback netCallback);

  virtual INetworkProvider::NetworkStatus connect();
  virtual INetworkProvider::NetworkStatus disconnect();

  virtual INetworkProvider::NetworkStatus queryStatus();

private:

  enum class State
  {
    NO_CONNECTION,
    CONNECTED,
    DEVICE_UNAVAILABLE,
    CONNECTION_LOST
  };

  enum class Event
  {
    CONNECT_REQUEST,
    DISCONNECT_REQUEST,
    DEVICE_LOST,
    DEVICE_RESTORED
  };

  enum class ErrorCode
  {
    PPPD_ERROR,
    PING_ERROR,
    INTERNAL_ERROR
  };

  static const unsigned int InitialCallbacksCount = 5U;


  State itsCurrentState = State::NO_CONNECTION;

  PppEndpoint itsPppEndpoint;

  std::vector<INetworkProvider::NetworkStatusCallback> callbackList;

  std::mutex itsEventMutex;
  std::mutex itsMonitoringMutex;

  bool isDeviceAvailable();
  State stateMachineTick(Event ev);
  void notifyByCallback(INetworkProvider::NetworkStatus & status);

  inline void startPowerMonitor();
  inline void pausePowerMonitor();

  void powerMonitorThread();
};

#endif /* PPPCONNECTION_H_ */

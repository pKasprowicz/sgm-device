/*
 * INetworkProvider.h
 *
 *  Created on: 9 maj 2016
 *      Author: pkasprow
 */

#ifndef INETWORKPROVIDER_H_
#define INETWORKPROVIDER_H_

#include <functional>

class INetworkProvider
{
public:

  enum class NetworkStatus
  {
    CONNECTED,
    DISCONNECTED,
    CONNECTION_ERROR,
    TOO_MANY_ATTEMPTS,
    DEVICE_UNAVAILABLE,
    UNKNOWN_ERROR
  };

  typedef std::function<void(NetworkStatus &)> NetworkStatusCallback;

  virtual ~INetworkProvider()
  {

  }

  virtual void registerUserCallback(NetworkStatusCallback) = 0;

  virtual NetworkStatus connect() = 0;
  virtual NetworkStatus disconnect() = 0;

  virtual NetworkStatus queryStatus() = 0;

private:

};



#endif /* INETWORKPROVIDER_H_ */

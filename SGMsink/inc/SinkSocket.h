/*
 * SinkSocket.h
 *
 *  Created on: 23 maj 2016
 *      Author: pkasprow
 */

#ifndef SINKSOCKET_H_
#define SINKSOCKET_H_

#include "Logger.h"

#include <sys/un.h>
#include <sys/socket.h>
#include <systemd/sd-daemon.h>
#include "unistd.h"
#include <fcntl.h>

#include <stdexcept>
#include <cstring>

template<typename PacketClass> class SinkSocket
{
public:
  SinkSocket()
  {

  }
  virtual ~SinkSocket();

  virtual bool isConnected()
  {
    return (acceptedSocketDescriptor > -1);
  }

  virtual int connect();
  virtual void disconnect();

  virtual int receive(PacketClass * rxPacket) throw();
  virtual int send() throw();

private:

  int setFdBlocking(int fd, int blocking) {
    /* Save the current flags */
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1)
    {
      return 0;
    }

    if (blocking)
    {
      flags &= ~O_NONBLOCK;
    }
    else
    {
      flags |= O_NONBLOCK;
    }
    return fcntl(fd, F_SETFL, flags) != -1;
  }

  void translateByteStreamToStructure(PacketClass * packet);

  uint8_t rxBuffer[2 * sizeof(PacketClass)];

  struct sockaddr_un itsSocketAddress;
  size_t itsSocketAddressLength;
  int socketDescriptor{-1};
  int acceptedSocketDescriptor{-1};

};

template<typename PacketClass> SinkSocket<PacketClass>::~SinkSocket()
{
  SGM_LOG_DEBUG("SinkSocket : Closing socket %d", acceptedSocketDescriptor);
  close(acceptedSocketDescriptor);
}

template<typename PacketClass>
int SinkSocket<PacketClass>::receive(PacketClass * rxPacket) throw ()
{

  if (socketDescriptor < 0U)
  {
         SGM_LOG_ERROR("Error during socket connection, restarting daemon");
         throw std::runtime_error("Error during socket accept() call");
  }

  int rxSize = 0;
  rxSize = recv(acceptedSocketDescriptor, rxBuffer, sizeof(rxBuffer), 0);

  if (rxSize < 0)
  {
    if ((EAGAIN == errno) || (EWOULDBLOCK == errno))
    {
      SGM_LOG_ERROR("No data available on socket");
      return errno;
    }
    else
    {
      SGM_LOG_ERROR("Unable to receive packet : error %d", errno);
      return 0;
    }
  }

  if (0 == rxSize)
  {
    return 0;
  }

  translateByteStreamToStructure(rxPacket);

  SGM_LOG_DEBUG("SinkSocket<PacketClass>::receive() : Received %d bytes\n", rxSize);

  return rxSize;
}

template<typename PacketClass>
int SinkSocket<PacketClass>::connect()
{
  int socketsCount = 0;
  while ((socketsCount = sd_listen_fds(0)) == 0)
  {

  }
  SGM_LOG_DEBUG("Found %d sockets\n", socketsCount);
  for (socketDescriptor = SD_LISTEN_FDS_START; socketDescriptor < SD_LISTEN_FDS_START + socketsCount; ++socketDescriptor)
  {
    if (sd_is_socket_unix(socketDescriptor, SOCK_SEQPACKET, -1, NULL, 0))
    {
      SGM_LOG_DEBUG("SinkSocket<PacketClass>::SinkSocket() : Socket found, descriptor : %d", socketDescriptor);
      break;
    }
    if(socketDescriptor == (SD_LISTEN_FDS_START + socketsCount - 1))
    {
      SGM_LOG_DEBUG("SinkSocket<PacketClass>::SinkSocket() : Could not find socket");
      throw std::runtime_error("Could not find socket");
    }
  }

  itsSocketAddressLength = sizeof(itsSocketAddress);

  setFdBlocking(socketDescriptor, false);

  acceptedSocketDescriptor = accept(socketDescriptor, reinterpret_cast<struct sockaddr *>(&itsSocketAddress), &itsSocketAddressLength);
  if (acceptedSocketDescriptor < 0)
  {
    if ((EAGAIN == errno) || (EWOULDBLOCK == errno))
    {
      SGM_LOG_INFO("No awaiting connections");
      return errno;
    }
    else
    {
      SGM_LOG_DEBUG("SinkSocket<PacketClass>::SinkSocket() : Unable to accept connection : error %d", errno);
      throw std::runtime_error("Connection could not be accepted");
    }
  }
  else
  {
    SGM_LOG_INFO("Socket connection established; id = %d", acceptedSocketDescriptor);
  }

}

template<typename PacketClass>
void SinkSocket<PacketClass>::disconnect()
{
  close(acceptedSocketDescriptor);
  acceptedSocketDescriptor = -1;
}

template<typename PacketClass>
int SinkSocket<PacketClass>::send() throw ()
{
  return 0;
}

template<typename PacketClass>
void SinkSocket<PacketClass>::translateByteStreamToStructure(
    PacketClass * packet)
{
  std::memcpy(packet, rxBuffer, sizeof(PacketClass));
}

#endif /* SINKSOCKET_H_ */

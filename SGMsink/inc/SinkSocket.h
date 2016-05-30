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

#include <stdexcept>
#include <cstring>

template<typename PacketClass> class SinkSocket
{
public:
  SinkSocket() throw (std::runtime_error);
  virtual ~SinkSocket();

  virtual int receive(PacketClass * rxPacket) throw();
  virtual int send() throw();

private:

  void translateByteStreamToStructure(PacketClass * packet);

  uint8_t rxBuffer[2 * sizeof(PacketClass)];

  struct sockaddr_un itsSocketAddress;
  size_t itsSocketAddressLength;
  int socketDescriptor{-1};
  int acceptedSocketDescriptor{-1};

};

template<typename PacketClass> SinkSocket<PacketClass>::~SinkSocket()
{
  SGM_LOG_DEBUG("Closing socket %d", acceptedSocketDescriptor);
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

  int rxSize = 0U;
  rxSize = recv(acceptedSocketDescriptor, rxBuffer, sizeof(rxBuffer), 0U);

  if (rxSize < 0)
  {
    SGM_LOG_ERROR("Unable to receive packet : error %d", errno);
  }

  translateByteStreamToStructure(rxPacket);

  SGM_LOG_DEBUG("SinkSocket<PacketClass>::receive() : Received %d bytes\n", rxSize);

  return rxSize;
}

template<typename PacketClass>
SinkSocket<PacketClass>::SinkSocket() throw (std::runtime_error)
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
      SGM_LOG_DEBUG("SinkSocket<PacketClass>::SinkSocket() : Socket found : %d", socketDescriptor);
      break;
    }
    if(socketDescriptor == (SD_LISTEN_FDS_START + socketsCount - 1))
    {
      SGM_LOG_DEBUG("SinkSocket<PacketClass>::SinkSocket() : Could not find socket");
      throw std::runtime_error("Could not find socket");
    }
  }

  itsSocketAddressLength = sizeof(itsSocketAddress);
  acceptedSocketDescriptor = accept(socketDescriptor, reinterpret_cast<struct sockaddr *>(&itsSocketAddress), &itsSocketAddressLength);
  if (acceptedSocketDescriptor < 0)
  {
    SGM_LOG_DEBUG("SinkSocket<PacketClass>::SinkSocket() : Unable to accept connection : error %d", errno);
    throw std::runtime_error("Connection could not be accepted");
  }
  else
  {
    SGM_LOG_INFO("Socket connection established; id = %d", acceptedSocketDescriptor);
  }

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

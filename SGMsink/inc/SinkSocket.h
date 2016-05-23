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

#include <stdexcept>


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

  struct sockaddr_un remoteSocket;
  int socketFileDescriptor{-1};
  int socketDescriptor{-1};

};

template<typename PacketClass> SinkSocket<PacketClass>::~SinkSocket()
{

}

template<typename PacketClass>
int SinkSocket<PacketClass>::receive(PacketClass * rxPacket) throw ()
{
  auto socketAddressLength = sizeof(struct sockaddr_un);
  socketDescriptor = accept(socketFileDescriptor, reinterpret_cast<sockaddr *>(&remoteSocket), &socketAddressLength);

  if (socketDescriptor < 0U)
  {
         SGM_LOG_ERROR("Error during socket connection, restarting daemon");
         throw std::runtime_error("Error during socket accept() call");
  }
  SGM_LOG_INFO("Connection accepted");

  unsigned int rxSize = 0U;
  rxSize = recv(socketDescriptor, rxBuffer, sizeof(rxBuffer), 0U);

  translateByteStreamToStructure(rxPacket);

  SGM_LOG_DEBUG("Received %d bytes\n", rxSize);



  return rxSize;
}

template<typename PacketClass>
SinkSocket<PacketClass>::SinkSocket() throw (std::runtime_error)
{
  {
    int socketsCount = 0;
    while ((socketsCount = sd_listen_fds(0)) == 0)
    {

    }
    SGM_LOG_DEBUG("Found %d sockets\n", socketsCount);
    for (socketFileDescriptor = SD_LISTEN_FDS_START; socketFileDescriptor < SD_LISTEN_FDS_START + socketsCount; ++socketFileDescriptor)
    {
      if (sd_is_socket_unix(socketFileDescriptor, SOCK_SEQPACKET, -1, NULL, 0))
      {
              break;
      }
      if(socketFileDescriptor == (SD_LISTEN_FDS_START + socketsCount - 1))
      {
              throw std::runtime_error("Could not find socket");
      }
    }
  }

  listen(socketFileDescriptor, 3);

}

template<typename PacketClass>
int SinkSocket<PacketClass>::send() throw ()
{

}

template<typename PacketClass>
void SinkSocket<PacketClass>::translateByteStreamToStructure(
    PacketClass * packet)
{
}

#endif /* SINKSOCKET_H_ */

#ifndef CLIENTSOCKET_H_
#define CLIENTSOCKET_H_

#include "Logger.h"

#include <sys/un.h>
#include <sys/socket.h>
#include "unistd.h"

#include <stdexcept>
#include <cstring>

template<typename PacketClass> class ClientSocket
{
public:
  ClientSocket() throw (std::runtime_error);
  virtual ~ClientSocket();

  virtual int sendPacket(PacketClass * rxPacket) throw();

private:

  void translateByteStreamToStructure(PacketClass * packet);

  std::string itsSocketPath{"/home/root/sgm/sgm.sckt"};

  struct sockaddr_un itsSocketAddress;
  int socketDescriptor{-1};

};

template<typename PacketClass> ClientSocket<PacketClass>::~ClientSocket()
{
  SGM_LOG_DEBUG("Closing socket %d", socketDescriptor);
  close(socketDescriptor);
}

template<typename PacketClass>
ClientSocket<PacketClass>::ClientSocket() throw (std::runtime_error)
{
  socketDescriptor = socket(AF_UNIX, SOCK_SEQPACKET, 0);

  itsSocketAddress.sun_family = AF_UNIX;
  strcpy(itsSocketAddress.sun_path, itsSocketPath.c_str());

  int connectResult = connect(socketDescriptor, (struct sockaddr *)&itsSocketAddress, sizeof(itsSocketAddress));

  if (-1 == connectResult)
  {
    throw std::runtime_error("error during socket connection");
  }
}

template<typename PacketClass>
int ClientSocket<PacketClass>::sendPacket(PacketClass * rxPacket) throw ()
{
  int txBytes = send(socketDescriptor, reinterpret_cast<void *>(rxPacket), sizeof(PacketClass), 0);

  return txBytes;
}

#endif /* CLIENTSOCKET_H_ */

/*
 * SocketReader.cpp
 *
 *  Created on: 10 mar 2016
 *      Author: pkasprow
 */

#include "SocketReader.h"
#include "Logging.h"

int  SocketReader::socketFileDescriptor = 0;
bool SocketReader::isSocketInitialized  = false;

SocketReader::SocketReader()
{
  // TODO Auto-generated constructor stub

}

SocketReader::~SocketReader()
{
  // TODO Auto-generated destructor stub
}

SocketReader::Result SocketReader::init()
{
  SGM_LOG_DEBUG("Initialization started");

  if (true == isSocketInitialized)
  {
    SGM_LOG_DEBUG("isSocketInitialized == true");
    return Result::SOCKET_ALREADY_INITIALIZED;
  }

  {
    int socketsCount = 0;
    while ((socketsCount = sd_listen_fds(0)) == 0U)
    {

    }

    sd_journal_print(LOG_ERR, "Found %d sockets\n", socketsCount);
    for (socketFileDescriptor = SD_LISTEN_FDS_START;
        socketFileDescriptor < SD_LISTEN_FDS_START + socketsCount; ++socketFileDescriptor)
    {
      if (sd_is_socket_unix(socketFileDescriptor, SOCK_SEQPACKET, -1, NULL, 0))
      {
        break;
      }
      if (socketFileDescriptor == (SD_LISTEN_FDS_START + socketsCount - 1))
      {
        sd_journal_print(LOG_ERR, "No socket found\n");
        return Result::SOCKET_NOT_FOUND;
      }
    }

  }

}

SocketReader::Result SocketReader::deInit()
{
  return Result::SOCKET_CLOSED;
}

int SocketReader::receive()
{
  uint8_t rxBuffer[255];
  unsigned int rxSize = 0U;
  struct sockaddr_un client;
  socklen_t sockaddr_len = sizeof(struct sockaddr_un);
  unsigned int socketDescriptor = 0U;
  while (1)
  {
    rxSize = 0U;
    SGM_LOG_INFO("Starting listening on socket");
    listen(socketFileDescriptor, 3);
    socketDescriptor = accept(socketFileDescriptor, reinterpret_cast<sockaddr *>(&client),
        &sockaddr_len);
    if (socketDescriptor < 0U)
    {
      sd_journal_print(LOG_INFO,
          "Error during socket connection, restarting daemon\n");
      break;
    }
    sd_journal_print(LOG_INFO, "Connection accepted\n");
    unsigned int currentRxSize = 0U;
    while ((currentRxSize = recv(socketDescriptor, rxBuffer, sizeof(rxBuffer),
        0U)) > 0U)
    {
      rxSize += currentRxSize;
    }
    if (0U == currentRxSize)
    {
      sd_journal_print(LOG_INFO, "Client disconnected\n");
    }
    sd_journal_print(LOG_INFO, "Received %d bytes\n", rxSize);

  }

}

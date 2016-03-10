/*
 * SocketReader.h
 *
 *  Created on: 10 mar 2016
 *      Author: pkasprow
 */

#ifndef SOCKETREADER_H_
#define SOCKETREADER_H_

extern "C"
{
  #include <sys/socket.h>
  #include <sys/un.h>
  #include <systemd/sd-daemon.h>
}

class SocketReader
{
public:

  enum class Result;

  SocketReader ();
  virtual ~SocketReader ();
  SocketReader::Result init();
  SocketReader::Result deInit();

  int receive();

  enum class Result
  {
    SOCKET_OPENED,
    SOCKET_CLOSED,
    SOCKET_NOT_FOUND,
    SOCKET_TOO_MANY_DESCRIPTORS,
    SOCKET_ALREADY_INITIALIZED
  };

private:
  static int socketFileDescriptor;
  static bool isSocketInitialized;
};

#endif /* SOCKETREADER_H_ */

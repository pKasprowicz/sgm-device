/*
 * ModemCMux.h
 *
 *  Created on: 19 kwi 2016
 *      Author: pkasprow
 */

#ifndef BGS2CMUX_H_
#define BGS2CMUX_H_

#include "ICMuxDriver.h"
#include "Logger.h"

class BGS2CMux : public ICMuxDriver
{
public:

  BGS2CMux();
  virtual ~BGS2CMux();

  virtual ICMuxDriver::Result turnOn();
  virtual ICMuxDriver::Result turnOff();

private:

  int serial_fd{-1};

  ICMuxDriver::Result cMuxOn();

  int send_at_command(int serial_fd, const char *command);
  int get_major(const char *driver);
  int make_nodes(int major, const char *basename, int number_nodes);
  void remove_nodes(const char *basename, int number_nodes);

  void performEarlyCleanup();
};

#endif /* BGS2CMUX_H_ */

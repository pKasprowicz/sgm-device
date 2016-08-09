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

#include <memory>

#include <mraa/uart.hpp>

class BGS2CMux : public ICMuxDriver
{
  //TODO rewrite using libmraa UART
public:

  BGS2CMux(mraa::Uart & uartRef);
  virtual ~BGS2CMux();

  ICMuxDriver::Result turnOn() override;
  ICMuxDriver::Result turnOff() override;

  mraa::Uart & getCurrentUart() override;
  Result getState() override;

private:

  int serial_fd{-1};
  mraa::Uart & itsRawUart;
  std::unique_ptr<mraa::Uart> itsMuxUart;

  ICMuxDriver::Result cMuxOn();

  int send_at_command(int serial_fd, const char *command);
  int get_major(const char *driver);
  int make_nodes(int major, const char *basename, int number_nodes);
  void remove_nodes(const char *basename, int number_nodes);

  void performEarlyCleanup();
  ICMuxDriver::Result itsCurrentCMuxState{ICMuxDriver::Result::MUX_OFF};
};

#endif /* BGS2CMUX_H_ */

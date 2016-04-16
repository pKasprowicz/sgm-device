/*
 * Cops.cpp
 *
 *  Created on: 16 kwi 2016
 *      Author: pkasprow
 */

#include <bgs2/at/Cops.h>

#include <Logger.h>

#include <string.h>



#define COPS_COMMAND "AT+COPS"

namespace bgs2
{

  Cops::Cops() :
      mode(0U),
      format(0U),
      networkId(0U),
      networkName{0U}
  {
  }

  Cops::Cops(int _networkId) :
      mode(1U),
      format(2U),
      networkId(_networkId),
      networkName{0U}
  {
  }

  Cops::Cops(char _networkName[]) :
      mode{1U},
      format{2U},
      networkId{0U},
      networkName{0U}
  {
    memcpy(static_cast<void *>(&networkName[0]), static_cast<void *>(&_networkName[0]), sizeof(_networkName));
  }

  int Cops::renderCommand(char* txBuffer, IAtCommand::CommandType cmdType)
  {
    std::string command{COPS_COMMAND};

    switch(cmdType)
    {
      case IAtCommand::CommandType::AT_READ:
        break;

      case IAtCommand::CommandType::AT_WRITE:
        prepareWriteCommand(command);
        break;
    }
  }

  bgs2::Cops::~Cops()
  {
  }

  void Cops::prepareWriteCommand(std::string& command)
  {
    command += "=";
    switch(mode)
    {
      case 0:
        command += "0\r\n";
        return;

      case 1:
        switch(format)
        {
          case 0: //long alphanumeric
            command += "1,0," + networkId;
            break;

          case 2:
            command += "1,2,";
            command += networkName;
            break;

          default:
            SGM_LOG_ERROR("AT+COPS : unsupported <format>");
            break;
        }
        break;

        default:
          SGM_LOG_ERROR("AT+COPS : unknown or unsupported <mode>");
          break;
    }

    command += "\r\n";
  }

} /* namespace bgs2 */

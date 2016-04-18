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

  Cops::Cops(unsigned int _networkId) :
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
    std::string commandText{COPS_COMMAND};

    bool isCommandRendered = false;

    switch(cmdType)
    {
      case IAtCommand::CommandType::AT_READ:
        isCommandRendered = prepareReadCommand(commandText);
        break;

      case IAtCommand::CommandType::AT_WRITE:
        isCommandRendered = prepareWriteCommand(commandText);
        break;

      case IAtCommand::CommandType::AT_EXECUTE:
        SGM_LOG_FATAL("+COPS : exec command unspecified");
        break;

      case IAtCommand::CommandType::AT_TEST:
        SGM_LOG_WARN("+COPS : test command not supported");
        break;

      default:
        SGM_LOG_FATAL("+COPS : command has not been sent yet to ME");
        break;
    }


    if(isCommandRendered)
    {
      int renderedCommandLength = commandText.length();
      commandText.copy(txBuffer, renderedCommandLength);
      SGM_LOG_DEBUG("+COPS : rendered command : %s", commandText.c_str());
      return renderedCommandLength;
    }

    return 0U;

  }

  bgs2::Cops::~Cops()
  {
  }

  bool Cops::prepareWriteCommand(std::string& command)
  {
    bool retVal = true;

    command += "=";
    switch(mode)
    {
      case 0:
        command += "0\r\n";
        return retVal;

      case 1:
        switch(format)
        {
          case 0: //long alphanumeric
            command += "1,0,";
            command += networkName;
            break;

          case 2:
            command += "1,2,";
            command.append(std::to_string(networkId));
            break;

          default:
            SGM_LOG_ERROR("AT+COPS : unsupported <format>");
            retVal = false;
            break;
        }
        break;

        default:
          SGM_LOG_ERROR("AT+COPS : unknown or unsupported <mode>");
          retVal = false;
          break;
    }

    command += "\r\n";
  }

bool Cops::prepareReadCommand(std::string& command)
{
  command += "=?\r\n";
  return true;
}

} /* namespace bgs2 */

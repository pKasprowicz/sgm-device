/*
 * At.cpp
 *
 *  Created on: 16 kwi 2016
 *      Author: pkasprow
 */

#include <bgs2/at/Smso.h>

#include <IAtCommand.h>
#include <Logger.h>

#include <string.h>

#define AT_COMMAND "AT^SMSO"

namespace bgs2
{
  int Smso::renderCommand(char * txBuffer, IAtCommand::CommandType cmdType)
  {
    int retVal = -1;

    switch(cmdType)
    {
      case IAtCommand::CommandType::AT_WRITE:
        SGM_LOG_ERROR("AT Error : no write command available");
        break;

      case IAtCommand::CommandType::AT_EXECUTE:
        memcpy(static_cast<void *>(txBuffer), AT_COMMAND"\r\n", sizeof(AT_COMMAND"\r\n") - 1);
        retVal = sizeof(AT_COMMAND"\r\n") - 1;
        break;

      case IAtCommand::CommandType::AT_READ:
        SGM_LOG_ERROR("AT Error : no query command available");
        break;

      default:
        break;
    }

    return retVal;
  }

}


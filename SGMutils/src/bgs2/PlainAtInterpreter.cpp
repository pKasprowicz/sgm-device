/*
 * PlainAtInterpreter.cpp
 *
 *  Created on: 16 kwi 2016
 *      Author: pkasprow
 */

#include "bgs2/PlainAtInterpreter.h"
#include <regex>

#define EXEC_REGEX "\r\n(OK|ERROR)\r\n"

AtResponse && PlainAtInterpreter::getResponse(const char * rxBuf)
{
  AtResponse resp;

  std::regex atExecRegex(EXEC_REGEX);
  std::cmatch base_match;

  if (std::regex_match(rxBuf, base_match, atExecRegex))
  {
    resp.responseCorrect = true;
  }
  else
  {
    return std::move(resp);
  }

  if (2U == base_match[1].length())
  {
    resp.commandReturnCode = AtResponse::ReturnCode::OK;
  }
  else
  {
    resp.commandReturnCode = AtResponse::ReturnCode::ERROR;
  }

  return std::move(resp);
}

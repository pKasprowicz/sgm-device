/*
 * PlainAtInterpreter.cpp
 *
 *  Created on: 16 kwi 2016
 *      Author: pkasprow
 */

#include "bgs2/PlainAtInterpreter.h"
#include <regex>

#define EXEC_REGEX  "([^]*?)" \
                    "\r\n(OK|ERROR)\r\n"

namespace bgs2
{

  std::unique_ptr<AtGenericResponse> PlainAtInterpreter::getResponse(const char * rxBuf, IAtCommand::CommandType cmdType)
  {
    std::regex atExecRegex(EXEC_REGEX);
    std::cmatch base_match;

    bool isResponseCorrect = false;
    AtGenericResponse::ReturnCode commandReturnCode{AtGenericResponse::ReturnCode::ERROR};

    if (std::regex_match(rxBuf, base_match, atExecRegex))
    {
      isResponseCorrect = true;
    }
    else
    {
      return std::unique_ptr<AtGenericResponse>(new AtGenericResponse());
    }

    if (2U == base_match[2].length())
    {
      commandReturnCode = AtGenericResponse::ReturnCode::OK;
    }

    return std::unique_ptr<AtGenericResponse>(new AtGenericResponse(isResponseCorrect, commandReturnCode, 0));
  }

}


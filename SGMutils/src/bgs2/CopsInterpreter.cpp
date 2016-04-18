/*
 * CopsInterpreter.cpp
 *
 *  Created on: 18 kwi 2016
 *      Author: pkasprow
 */

#include "bgs2/CopsInterpreter.h"

#include <bgs2/at/CopsResponse.h>

namespace bgs2
{



} /* namespace bgs2 */

std::unique_ptr<AtGenericResponse> bgs2::CopsInterpreter::getResponse(const char* rxBuf, IAtCommand::CommandType cmdType)
{

  switch(cmdType)
  {

  }

  std::unique_ptr<AtGenericResponse> response{new CopsResponse()};

}

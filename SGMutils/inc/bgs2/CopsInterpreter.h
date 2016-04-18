/*
 * CopsInterpreter.h
 *
 *  Created on: 18 kwi 2016
 *      Author: pkasprow
 */

#ifndef BGS2_COPSINTERPRETER_H_
#define BGS2_COPSINTERPRETER_H_

#include "AtGenericResponse.h"
#include "IAtCommand.h"

#include <memory>


#define READ_REGEX  "\r\n([^]*?)\r\n" \
                    "\r\n(OK|ERROR)\r\n"
#define WRITE_REGEX

namespace bgs2
{

  class CopsInterpreter
  {
    public:

    static std::unique_ptr<AtGenericResponse> getResponse(const char * rxBuf, IAtCommand::CommandType cmdType);

    private:
  };

} /* namespace bgs2 */

#endif /* BGS2_COPSINTERPRETER_H_ */

/*
 * PlainAtInterpreter.h
 *
 *  Created on: 16 kwi 2016
 *      Author: pkasprow
 */

#ifndef BGS2_PLAINATINTERPRETER_H_
#define BGS2_PLAINATINTERPRETER_H_

#include <AtGenericResponse.h>
#include <IAtCommand.h>

#include <memory>

namespace bgs2
{

  class PlainAtInterpreter
  {
    public:

    static std::unique_ptr<AtGenericResponse> getResponse(const char * rxBuf, IAtCommand::CommandType cmdType);

    private:

  };

}


#endif /* BGS2_PLAINATINTERPRETER_H_ */

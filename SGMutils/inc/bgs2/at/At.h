/*
 * At.h
 *
 *  Created on: 31 mar 2016
 *      Author: pkasprow
 */

#ifndef BGS2_AT_H_
#define BGS2_AT_H_

#include <IAtCommand.h>

#include <mraa/uart.hpp>

namespace bgs2
{

  class At
  {
    public:

      int renderCommand(char * txBuffer, IAtCommand::CommandType cmdType);

    private:

  };

}

#endif /* BGS2_AT_H_ */

/*
 * Smso.h
 *
 *  Created on: 5 lip 2016
 *      Author: pkasprow
 */

#ifndef BGS2_AT_SMSO_H_
#define BGS2_AT_SMSO_H_


#include <IAtCommand.h>

#include <mraa/uart.hpp>

namespace bgs2
{

  class Smso
  {
    public:

      int renderCommand(char * txBuffer, IAtCommand::CommandType cmdType);

    private:

  };

}


#endif /* BGS2_AT_SMSO_H_ */

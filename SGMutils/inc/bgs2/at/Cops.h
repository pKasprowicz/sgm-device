/*
 * Cops.h
 *
 *  Created on: 16 kwi 2016
 *      Author: pkasprow
 */

#ifndef BGS2_AT_COPS_H_
#define BGS2_AT_COPS_H_

#include <IAtCommand.h>

namespace bgs2
{

  class Cops
  {
    public:
      Cops();
      Cops(int _networkId);
      Cops(char _networkName[]);

      int renderCommand(char * txBuffer, IAtCommand::CommandType cmdType);

      virtual ~Cops();

    private:

      void prepareWriteCommand(std::string & command);

      int mode;
      int format;
      int networkId;
      char networkName[10];
  };

} /* namespace bgs2 */

#endif /* BGS2_AT_COPS_H_ */

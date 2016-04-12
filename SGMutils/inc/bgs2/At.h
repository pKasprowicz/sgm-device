/*
 * At.h
 *
 *  Created on: 31 mar 2016
 *      Author: pkasprow
 */

#ifndef BGS2_AT_H_
#define BGS2_AT_H_

#include <IAtCommand.h>

namespace bgs2
{

  class At : public IAtCommand
  {
  public:

    At();

    virtual Result sendAt(mraa::Uart & _uart, int timeout = 0);
    virtual const AtResponse & getResponse();

    virtual ~At();

  private:

    char rxBuffer[RX_BUF_SIZE];


    static const char CommandString[];
    static const int ResponseSize = 4U;
  };

}


#endif /* BGS2_AT_H_ */

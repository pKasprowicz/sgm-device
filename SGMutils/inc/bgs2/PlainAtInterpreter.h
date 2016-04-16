/*
 * PlainAtInterpreter.h
 *
 *  Created on: 16 kwi 2016
 *      Author: pkasprow
 */

#ifndef BGS2_PLAINATINTERPRETER_H_
#define BGS2_PLAINATINTERPRETER_H_

#include <AtResponse.h>

class PlainAtInterpreter
{
  public:

    static AtResponse && getResponse(const char * rxBuf);

  private:

};

#endif /* BGS2_PLAINATINTERPRETER_H_ */

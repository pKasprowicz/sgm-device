/*
 * CopsResponse.h
 *
 *  Created on: 18 kwi 2016
 *      Author: pkasprow
 */

#ifndef BGS2_AT_COPSRESPONSE_H_
#define BGS2_AT_COPSRESPONSE_H_

#include "AtGenericResponse.h"

#include <string>

namespace bgs2
{

class CopsResponse : public AtGenericResponse
{
public:

  CopsResponse();

  CopsResponse(const CopsResponse & ) = delete;
  CopsResponse & operator = (const CopsResponse &) = delete;

  ~CopsResponse();

  unsigned int networkMode;
  unsigned int networkFormat;

  std::string & getOperatorName();

private:
  std::string operatorName;

};

} /* namespace bgs2 */

#endif /* BGS2_AT_COPSRESPONSE_H_ */

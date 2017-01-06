/*
 * CopsResponse.cpp
 *
 *  Created on: 18 kwi 2016
 *      Author: pkasprow
 */

#include "bgs2/at/CopsResponse.h"

#include <string>

namespace bgs2
{

CopsResponse::CopsResponse() : AtGenericResponse(),
    networkMode{0U},
    networkFormat{0U},
    operatorName{""}
{
  // TODO Auto-generated constructor stub

}

CopsResponse::~CopsResponse()
{
  // TODO Auto-generated destructor stub
}

} /* namespace bgs2 */

std::string& bgs2::CopsResponse::getOperatorName()
{
  return operatorName;
}

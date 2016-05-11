/*
 * PppEndpoint.cpp
 *
 *  Created on: 11 maj 2016
 *      Author: pkasprow
 */

#include <PppEndpoint.h>

#include "Logger.h"

#include <stdlib.h>

PppEndpoint::PppEndpoint()
{
  // TODO Auto-generated constructor stub

}

PppEndpoint::~PppEndpoint()
{
  // TODO Auto-generated destructor stub
}

bool PppEndpoint::open()
{
  int returncode = system("pppd");

  switch(returncode)
  {
  case 0:
    SGM_LOG_INFO("PPP connectio has been successfully established");
    return true;
    break;

  default:
    SGM_LOG_ERROR("Error on launching pppd daemon");
    return false;
    break;
  }
}

bool PppEndpoint::close()
{
  system("killall pppd");
  return true;
}

bool PppEndpoint::verifyConnection()
{
  return true;
}

bool PppEndpoint::performFailsafeCleanup()
{
}

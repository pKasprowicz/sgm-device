/*
 * ISensorDevice.h
 *
 *  Created on: 31 maj 2016
 *      Author: pkasprow
 */

#include "SGMDataType.h"
#include <vector>

#ifndef ISENSORDEVICE_H_
#define ISENSORDEVICE_H_

class ISensorDevice
{
public:

  virtual ~ISensorDevice()
  {

  }

  virtual void acquire(std::vector<sgm::PhysQuantity> quantities, std::vector<sgm::SgmProcessData> & data) = 0;
  virtual void acquire(std::vector<sgm::SgmProcessData> & data) = 0;
  virtual std::vector<sgm::PhysQuantity> & queryCapabilities() = 0;

private:
};



#endif /* ISENSORDEVICE_H_ */

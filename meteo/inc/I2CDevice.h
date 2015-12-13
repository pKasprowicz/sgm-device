/*
 * SystemFile.h
 *
 *  Created on: 16 kwi 2015
 *      Author: wro00354
 */

#ifndef INC_I2CDEVICE_H_
#define INC_I2CDEVICE_H_

#include <string>
#include "i2c-dev.h"

class I2Cdevice
{
public:
	I2Cdevice(std::string path, uint8_t devAddr);

	int32_t readByte(uint8_t deviceRegister, int8_t & buf);
	int32_t readWord(uint8_t deviceRegister, int16_t & buf);

	int32_t writeByte(uint8_t deviceRegister, const int8_t buf);
	int32_t writeWord(uint8_t deviceRegister, const int16_t buf);

private:

	int16_t swapOrder(const int16_t val);

	std::string devicePath;
	int posixFileDescriptor;
	uint8_t i2cDevAddress;
};



#endif /* INC_I2CDEVICE_H_ */

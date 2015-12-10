/*
 * SystemFile.cpp
 *
 *  Created on: 16 kwi 2015
 *      Author: wro00354
 */

#include<fcntl.h>
#include <I2CDevice.h>
#include<unistd.h>
#include <stdlib.h>


I2Cdevice::I2Cdevice(std::string path, uint8_t devAddr) :
i2cDevAddress(devAddr)
{
	devicePath = path;
}


int32_t I2Cdevice::readByte(uint8_t deviceRegister, int8_t & buf) {

	posixFileDescriptor = open(devicePath.c_str(), O_RDWR);
	int32_t result;

	ioctl(posixFileDescriptor, I2C_SLAVE, i2cDevAddress);
	result = i2c_smbus_read_byte_data(posixFileDescriptor, deviceRegister);
	close(posixFileDescriptor);

	if (result > -1)
	{
		buf = static_cast<uint8_t>(result);
		return 0;
	}

	else
	{
		return -1;
	}

}

int32_t I2Cdevice::readWord(uint8_t deviceRegister, int16_t& buf) {
	posixFileDescriptor = open(devicePath.c_str(), O_RDWR);
	int32_t result;

	ioctl(posixFileDescriptor, I2C_SLAVE, i2cDevAddress);
	result = i2c_smbus_read_word_data(posixFileDescriptor, deviceRegister);
	close(posixFileDescriptor);

	if (result > -1)
	{
		buf = swapOrder(result);
		return 0;
	}

	else
	{
		return -1;
	}
}

int32_t I2Cdevice::writeByte(uint8_t deviceRegister, const int8_t buf) {
	posixFileDescriptor = open(devicePath.c_str(), O_RDWR);
	int32_t result;

	ioctl(posixFileDescriptor, I2C_SLAVE, i2cDevAddress);
	result = i2c_smbus_write_byte_data(posixFileDescriptor, deviceRegister, buf);

	close(posixFileDescriptor);

	return result;
}

int32_t I2Cdevice::writeWord(uint8_t deviceRegister, const int16_t buf) {
	posixFileDescriptor = open(devicePath.c_str(), O_RDWR);
	int32_t result;

	ioctl(posixFileDescriptor, I2C_SLAVE, i2cDevAddress);
	result = i2c_smbus_write_word_data(posixFileDescriptor, deviceRegister, buf);

	close(posixFileDescriptor);

	return result;
}

int16_t I2Cdevice::swapOrder(const int16_t val) {
	uint16_t temp = (val & 0x00FF) << 8U;
	temp |= (val & 0xFF00) >> 8U;

	return temp;
}

#ifndef DEVICEID_H_
#define DEVICEID_H_

#include <string>


class DeviceId
{
  public:

  	DeviceId() = delete;

  	static void getDeviceId(std::string & devId);

  private:

  	static const char * SerialNumberFilePath;
  	static const char * DefaultSerialNumber;
  	static const uint8_t SerialNumberSize = 16U;
};

#endif /* DEVICEID_H_ */
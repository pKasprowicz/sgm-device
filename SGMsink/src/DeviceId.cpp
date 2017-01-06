#include "DeviceId.h"

#include "Logger.h"

#include <fcntl.h>
#include <unistd.h>

const char * DeviceId::SerialNumberFilePath = "/factory/serial_number";
const char * DeviceId::DefaultSerialNumber = "SGM_DUMMY_DEVICE";

void DeviceId::getDeviceId(std::string & devId)
{
    int bytesRead = 0;
    int fd = -1;
    char idBuffer[SerialNumberSize];
    if ((fd = open(SerialNumberFilePath, O_RDONLY)) != -1)
    {
        bytesRead = read(fd, const_cast<char *>(idBuffer), SerialNumberSize);
        close(fd);
    }

    if (SerialNumberSize != static_cast<uint8_t>(bytesRead))
    {
        devId.assign(DefaultSerialNumber);
        SGM_LOG_WARN("Could not acquire device serial number!");
    }
    else
    {
        devId.assign(idBuffer, SerialNumberSize);
    }

}
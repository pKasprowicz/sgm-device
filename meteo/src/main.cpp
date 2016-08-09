#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <i2c.hpp>
#include <sys/socket.h>
#include <sys/un.h>

#include <SharedMemory.h>
#include <AtCommand.h>
#include <AtGenericResponse.h>
#include <IAtCommand.h>

#include <bgs2/at/At.h>
#include <bgs2/PlainAtInterpreter.h>
#include <bgs2/at/Cops.h>
#include <bgs2/at/CopsResponse.h>
#include <bgs2/CopsInterpreter.h>
#include "SGMDataType.h"
#include "Logger.h"

#include <mraa/uart.hpp>


#include <fcntl.h>
using namespace std;

#define SOCKET_PATH "/home/root/sgm/sgm.sckt"

void socketTest()
{

  sgm::SgmProcessData data;
  data.value = 0xA5A55A5A;
  data.crc = 0x11223344;
  data.measPoint = sgm::MeasurementPoint::AIR;
  data.physQuantityType = sgm::PhysQuantity::TEMPERATURE;

  struct sockaddr_un remote;
  int s = socket(AF_UNIX, SOCK_SEQPACKET, 0);
  int len;

  remote.sun_family = AF_UNIX;
  strcpy(remote.sun_path, SOCKET_PATH);
  len = strlen(remote.sun_path) + sizeof(remote.sun_family);
  int connectResult = connect(s, (struct sockaddr *)&remote, sizeof(remote));
  if (connectResult == -1)
  {
    int cause = errno;
    perror("connect");
    exit(1);
  }

  unsigned int txBytes = send(s, reinterpret_cast<void *>(&data), sizeof(data), 0U);
  printf("Sent %d bytes\n", txBytes);

}

int main() {

//  SharedMemory sharedMem;
//    sharedMem.init();
//
//    SharedMemory::SharedData & sData = sharedMem.getDataInstance();
//
//    socketTest();

//	mraa::Uart uart1(0);
//	uart1.setBaudRate(57600);
//	uart1.setMode(8, mraa::UART_PARITY_NONE, 1);
//	uart1.setFlowcontrol(false, true);
//	uart1.setTimeout(10, 10, 10);
//
//	AtCommand<bgs2::PlainAtInterpreter, bgs2::At>commandAT;
//
//	IAtCommand & atc = commandAT;
//	IAtCommand::Result res = atc.sendAt(uart1, IAtCommand::CommandType::AT_EXECUTE, 100);
//
//	std::unique_ptr<AtGenericResponse> resp = commandAT.getResponse();
//
//	if (resp->getReturnCode() != AtGenericResponse::ReturnCode::OK)
//	{
//	  return -1;
//	}
//
//	AtCommand<bgs2::PlainAtInterpreter, bgs2::Cops> networkCommand(bgs2::Cops{26001});
//
//	res = networkCommand.sendAt(uart1, IAtCommand::CommandType::AT_WRITE, 100);
//
//	std::unique_ptr<AtGenericResponse> response2 = networkCommand.getResponse();
//
//	if (response2->getReturnCode() != AtGenericResponse::ReturnCode::OK)
//	  {
//	    return -1;
//	  }

    system("/home/root/cmux");

    sleep(3);

    int fd = open("/dev/ttyGSM2", O_RDWR | O_NONBLOCK);
    write(fd, "at^smso\r\n", 9);
    int err = errno;
    printf("%d",fd);
    printf("%d",errno);
    close(fd);

    system("killall cmux");

  return 0;
}

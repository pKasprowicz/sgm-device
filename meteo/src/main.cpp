#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <i2c.hpp>
#include <sys/socket.h>
#include <sys/un.h>

#include <SharedMemory.h>

using namespace std;

#define SOCKET_PATH "/home/root/sgm/sgm.sckt"

int main() {

  SharedMemory sharedMem;
    sharedMem.init();

    SharedMemory::SharedData & sData = sharedMem.getDataInstance();

    while(1)
    {
      sData.startAccess();

      if (sData.getModemReady())
      {
        cout << "Flag value true" << endl;
      }
      else
      {
        cout << "Flag value false" << endl;
      }

      sData.endAccess();
      sleep(7);
    }

	const char message[] = "srutu-tutu";

	struct sockaddr_un remote;
	int s = socket(AF_UNIX, SOCK_SEQPACKET, 0);
	int len;

	remote.sun_family = AF_UNIX;
	strcpy(remote.sun_path, SOCKET_PATH);
	len = strlen(remote.sun_path) + sizeof(remote.sun_family);
	if (connect(s, (struct sockaddr *)&remote, len) == -1)
	{
		perror("connect");
		exit(1);
	}

	unsigned int txBytes = send(s, message, sizeof(message), 0U);
	printf("Sent %d bytes\n", txBytes);


  return 0;
}

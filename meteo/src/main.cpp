#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <i2c.hpp>
#include <sys/socket.h>
#include <sys/un.h>
using namespace std;

#define SOCKET_PATH "/home/root/sgm/sgm.sckt"

int main() {

	const char message[] = "srutu-tutu";

	struct sockaddr_un remote;
	int s = socket(AF_UNIX, SOCK_STREAM, 0);
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

#include "AtModem.h"

#include "termios.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

const char * ATModem::terminalPath  = "/dev/ttyO4";

void ATModem::init() {
	struct termios config;
	int result = 0;
	fileDescriptor = open(terminalPath, O_RDWR | O_NOCTTY | O_NDELAY);
	result = tcgetattr(fileDescriptor, &config);

	config.c_iflag &= ~(IGNBRK | BRKINT | ICRNL |
	                     INLCR | PARMRK | INPCK | ISTRIP | IXON);

	config.c_oflag = 0;
	config.c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | ISIG);

	config.c_cflag &= ~(CSIZE | PARENB);
	config.c_cflag |= CS8;

	config.c_cc[VMIN]  = 1;
	config.c_cc[VTIME] = 0;

	result = cfsetispeed(&config, B9600);
	result = cfsetospeed(&config, B9600);
	result = tcsetattr(fileDescriptor, TCSAFLUSH, &config);

	close(fileDescriptor);

}

int ATModem::sendRawAt(std::string tx, std::string & rx) {
	char response[50] = {0x00};
	int result;

	fileDescriptor = open(terminalPath, O_RDWR | O_NOCTTY | O_NDELAY);

		result = write(fileDescriptor, tx.c_str(), tx.length());
		sleep(1);
		do
		{
			result = read(fileDescriptor, response, sizeof(response));
		} while ((result == -1) && (errno == EAGAIN));
		rx.assign(response);

		close(fileDescriptor);

		return result;
}



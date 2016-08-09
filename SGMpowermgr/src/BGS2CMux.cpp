/*
 * ModemCMux.cpp
 *
 *  Created on: 19 kwi 2016
 *      Author: pkasprow
 */

#include <BGS2CMux.h>

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <termios.h>
#include <net/if.h>
#include <linux/types.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <signal.h>

#include <thread>
/**
* gsmmux.h provides n_gsm line dicipline structures and functions.
* It should be kept in sync with your kernel release.
*/
#include "gsmmux.h"

/* n_gsm ioctl */
#ifndef N_GSM0710
# define N_GSM0710  21
#endif

/* attach a line discipline ioctl */
#ifndef TIOCSETD
# define TIOCSETD 0x5423
#endif

/* serial port of the modem */
#define SERIAL_PORT "/dev/ttyMFD1"

/* line speed */
#define LINE_SPEED  B115200

/* maximum transfert unit (MTU), value in bytes */
#define MTU 512

/**
* whether or not to create virtual TTYs for the multiplex
* 0 : do not create
* 1 : create
*/
#define CREATE_NODES  1

/* number of virtual TTYs to create (most modems can handle up to 4) */
#define NUM_NODES 4

/* name of the virtual TTYs to create */
#define BASENAME_NODES  "/dev/ttyGSM"

/* name of the driver, used to get the major number */
#define DRIVER_NAME "gsmtty"

/**
* whether or not to print debug messages to stderr
* 0 : debug off
* 1 : debug on
*/
#define DEBUG 1

/**
* whether or not to detach the program from the terminal
* 0 : do not daemonize
* 1 : daemonize
*/
#define DAEMONIZE 0

 /* size of the reception buffer which gets data from the serial line */
#define SIZE_BUF  256

/**
* Function raised by signal catching
*/
void signal_callback_handler(int signum) {
  return;
}

BGS2CMux::BGS2CMux(mraa::Uart & uartRef)
: itsRawUart(uartRef),
  itsMuxUart(nullptr)
{
  // TODO Auto-generated constructor stub

}

BGS2CMux::~BGS2CMux()
{
  // TODO Auto-generated destructor stub
}

BGS2CMux::Result BGS2CMux::turnOn()
{
  SGM_LOG_DEBUG("ModemCMux::turnOn() : Attempting to turn the CMUX on");
//  if (ICMuxDriver::Result::MUX_ON == cMuxOn())
//  {
//    itsCurrentCMuxState = ICMuxDriver::Result::MUX_ON;
//    std::string uartPath("/dev/ttyGSM2");
//    SGM_LOG_INFO("Modem CMUX has been successfully established!!");
//  }
//  else
//  {
//    itsCurrentCMuxState = ICMuxDriver::Result::MUX_ERROR;
//    SGM_LOG_ERROR("Could not establish CMUX connection");
//  }
  std::thread t1([this](){
    cMuxOn();
  });
  t1.detach();
  itsCurrentCMuxState = ICMuxDriver::Result::MUX_ON;
  return ICMuxDriver::Result::MUX_ON;
}

BGS2CMux::Result BGS2CMux::turnOff()
{
  SGM_LOG_DEBUG("ModemCMux::turnOff() : Attempting to turn the CMUX off");
  remove_nodes(BASENAME_NODES, NUM_NODES);
//  close(serial_fd);
  itsCurrentCMuxState = ICMuxDriver::Result::MUX_OFF;
  SGM_LOG_INFO("Modem CMUX is now turned off");
  return BGS2CMux::Result::MUX_OFF;
}

ICMuxDriver::Result BGS2CMux::cMuxOn()
{
  int major;
  struct termios tio;
  int ldisc = N_GSM0710;
  struct gsm_config gsm;
  char atcommand[40];

  performEarlyCleanup();

  /* print global parameters */
  SGM_LOG_INFO("SERIAL_PORT = " SERIAL_PORT);

  /* open the serial port */
  serial_fd = open(SERIAL_PORT, O_RDWR | O_NOCTTY | O_NDELAY);
  if (serial_fd == -1)
  {
    SGM_LOG_ERROR("Cannot open " SERIAL_PORT);
    return ICMuxDriver::Result::MUX_ERROR;
  }

  /* get the current attributes of the serial port */
  if (tcgetattr(serial_fd, &tio) == -1)
  {
    SGM_LOG_ERROR("Cannot get line attributes");
    return ICMuxDriver::Result::MUX_ERROR;
  }

  /* set the new attrbiutes */
  tio.c_iflag = 0;
  tio.c_oflag = 0;
  tio.c_cflag = CS8 | CREAD | CLOCAL;
  tio.c_cflag |= CRTSCTS;
  tio.c_lflag = 0;
  tio.c_cc[VMIN] = 1;
  tio.c_cc[VTIME] = 0;

  /* write the speed of the serial line */
  if (cfsetospeed(&tio, LINE_SPEED) < 0 || cfsetispeed(&tio, LINE_SPEED) < 0)
  {
    SGM_LOG_ERROR("Cannot set line speed");
    return ICMuxDriver::Result::MUX_ERROR;
  }

  /* write the attributes */
  if (tcsetattr(serial_fd, TCSANOW, &tio) == -1)
  {
    SGM_LOG_ERROR("Cannot set line attributes");
    return ICMuxDriver::Result::MUX_ERROR;
  }

  /**
  * Send AT commands to put the modem in CMUX mode.
  * This is vendor specific and should be changed
  * to fit your modem needs.
  * The following matches Quectel M95.
  */
  if (send_at_command(serial_fd, "AT\\Q3\r\n") == -1)
  {
    SGM_LOG_ERROR("AT\\Q3\\r\\n: bad response");
    return ICMuxDriver::Result::MUX_ERROR;
  }
  //  if (send_at_command(serial_fd, "AT+GMM\r") == -1)
  //    warnx("AT+GMM: bad response");
  if (send_at_command(serial_fd, "AT\r") == -1)
  {
    SGM_LOG_ERROR("AT: bad response");
    return ICMuxDriver::Result::MUX_ERROR;
  }
  if (send_at_command(serial_fd, "AT+IPR=115200\r") == -1)
  {
    SGM_LOG_ERROR("AT+IPR=115200: bad response");
    return ICMuxDriver::Result::MUX_ERROR;
  }
  sprintf(atcommand, "AT+CMUX=0\r");
  if (send_at_command(serial_fd, atcommand) == -1)
  {
    SGM_LOG_ERROR("Cannot enable modem CMUX");
    return ICMuxDriver::Result::MUX_ERROR;
  }

  /* use n_gsm line discipline */
  sleep(2);
  if (ioctl(serial_fd, TIOCSETD, &ldisc) < 0)
  {
    SGM_LOG_ERROR("Cannot set line dicipline. Is 'n_gsm' module registred?");
    return ICMuxDriver::Result::MUX_ERROR;
  }

  /* get n_gsm configuration */
  if (ioctl(serial_fd, GSMIOC_GETCONF, &gsm) < 0)
  {
    return ICMuxDriver::Result::MUX_ERROR;
  }
  /* set and write new attributes */
  gsm.initiator = 1;
  gsm.encapsulation = 0;
  gsm.mru = MTU;
  gsm.mtu = MTU;
  gsm.t1 = 10;
  gsm.n2 = 3;
  gsm.t2 = 30;
  gsm.t3 = 10;

  if (ioctl(serial_fd, GSMIOC_SETCONF, &gsm) < 0)
  {
    SGM_LOG_ERROR("Cannot set GSM multiplex parameters");
    return ICMuxDriver::Result::MUX_ERROR;
  }

  SGM_LOG_DEBUG("Line dicipline set");

  /* create the virtual TTYs */
  if (CREATE_NODES) {
    int created;

    if ((major = get_major(DRIVER_NAME)) < 0)
    {
      SGM_LOG_ERROR("Cannot get major number");
      return ICMuxDriver::Result::MUX_ERROR;
    }

    if ((created = make_nodes(major, BASENAME_NODES, NUM_NODES)) < NUM_NODES)
    {
      SGM_LOG_WARN("Cannot create all nodes, only %d/%d have been created.", created, NUM_NODES);
    }
  }

  close(serial_fd);
  return ICMuxDriver::Result::MUX_ON;
}

int BGS2CMux::send_at_command(int serial_fd, const char* command)
{
  char buf[SIZE_BUF];
    int r;

    tcflush(serial_fd,TCIFLUSH);

    /* write the AT command to the serial line */
    if (write(serial_fd, command, strlen(command)) <= 0)
      SGM_LOG_ERROR("Cannot write to %s", SERIAL_PORT);

    /* wait a bit to allow the modem to rest */
    sleep(1);

    /* read the result of the command from the modem */
    memset(buf, 0, sizeof(buf));
    r = read(serial_fd, buf, sizeof(buf));
    if (r == -1)
      SGM_LOG_ERROR("Cannot read %s", SERIAL_PORT);

    /* if there is no result from the modem, return failure */
    if (r == 0) {
      SGM_LOG_DEBUG("%s : No response", command);
      return -1;
    }

    /* if we have a result and want debug info, strip CR & LF out from the output */
    if (DEBUG) {
      unsigned int i;
      char bufp[SIZE_BUF];
      memcpy(bufp, buf, sizeof(buf));
      for(i = 0; i < strlen(bufp); i++) {
        if (bufp[i] == '\r' || bufp[i] == '\n')
          bufp[i] = ' ';
      }
    }

    /* if the output shows "OK" return success */
    if (strstr(buf, "OK\r") != NULL) {
      return 0;
    }

    return -1;
}

int BGS2CMux::get_major(const char* driver)
{
  FILE *fp;
  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  char device[20];
  int major = -1;

  /* open /proc/devices file */
  if ((fp = fopen("/proc/devices", "r")) == NULL)
  {
    SGM_LOG_ERROR("Cannot open /proc/devices");
    return EXIT_FAILURE;
  }

  /* read the file line by line */
  while ((major == -1) && (read = getline(&line, &len, fp)) != -1) {

    /* if the driver name string is found in the line, try to get the major */
    if (strstr(line, driver) != NULL) {
      if (sscanf(line,"%d %s\n", &major, device) != 2)
      {
        major = -1;
      }
    }

    /* free the line before getting a new one */
    if (line) {
      free(line);
      line = NULL;
    }
  }

  /* close /proc/devices file */
  fclose(fp);

  return major;
}

int BGS2CMux::make_nodes(int major, const char* basename, int number_nodes)
{
  int minor, created = 0;
    dev_t device;
    char node_name[15];
    mode_t oldmask;

    /* set a new mask to get 666 mode and stores the old one */
    oldmask = umask(0);

    for (minor=1; minor<number_nodes+1; minor++)
    {

      /* append the minor number to the base name */
      sprintf(node_name, "%s%d", basename, minor);

      /* store a device info with major and minor */
      device = makedev(major, minor);

      /* create the actual character node */
      if (mknod(node_name, S_IFCHR | 0666, device) != 0)
      {
        SGM_LOG_WARN("Cannot create %s", node_name);
      }
      else
      {
        created++;
        SGM_LOG_DEBUG("Created %s", node_name);
      }

    }

    /* revert the mask to the old one */
    umask(oldmask);

    return created;
}

void BGS2CMux::remove_nodes(const char* basename, int number_nodes)
{
  int node;
  char node_name[15];

  for (node = 1; node < number_nodes + 1; node++)
  {
    /* append the minor number to the base name */
    sprintf(node_name, "%s%d", basename, node);

    /* unlink the actual character node */
    SGM_LOG_DEBUG("Removing %s", node_name);

    if (unlink(node_name) == -1)
    {
      SGM_LOG_ERROR("Cannot remove %s", node_name);
    }
  }

}

mraa::Uart & BGS2CMux::getCurrentUart()
{
  switch(itsCurrentCMuxState)
  {
  case ICMuxDriver::Result::MUX_OFF:
  case ICMuxDriver::Result::MUX_ERROR:
      return itsRawUart;

  case ICMuxDriver::Result::MUX_ON:
    if (itsMuxUart == nullptr)
    {
      SGM_LOG_INFO("Opening /dev/ttyGSM2...");
      try
      {
      itsMuxUart = std::unique_ptr<mraa::Uart>(new mraa::Uart("/dev/ttyGSM2"));
      }
      catch(std::exception & e)
      {
        SGM_LOG_FATAL("Unable to open /dev/ttyGSM2");
        throw e;
      }
    }
    return *itsMuxUart;
  }
}

ICMuxDriver::Result BGS2CMux::getState()
{
  return itsCurrentCMuxState;
}

void BGS2CMux::performEarlyCleanup()
{
  if (0 == access("/dev/ttyGSM1", F_OK))
  {
    SGM_LOG_INFO("Early initialization detected existing CMUX devices. Removing...");
    remove_nodes(BASENAME_NODES, NUM_NODES);
  }
}

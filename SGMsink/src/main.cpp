
#include "PppConnection.h"
#include "INetworkProvider.h"
#include "SgmSink.h"

#include "Logger.h"
#include "SharedMemory.h"

#include <stdio.h>
#include <stdlib.h>

#include <systemd/sd-journal.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/un.h>
#include<time.h>
#include <csignal>

void sig_handler(int sig);

std::string sinkServerURI{"tcp://iot.eclipse.org:1883"};
std::string sinkClientId{"SGM#1"};

int main()
{

  std::signal(SIGHUP, sig_handler);
  std::signal(SIGTERM, sig_handler);
  sd_journal_print(LOG_INFO, "SGMdatasink service started!");

  SharedMemory sharedMem;
  if (SharedMemory::Result::INIT_ERROR == sharedMem.init())
  {
    SGM_LOG_ERROR("Error during shared memory initialization");
    return -1;
  }

  mqtt::client mqttClient(sinkServerURI, sinkClientId);
  PppConnection pppConn(sharedMem);
  SgmSink sgmSink(mqttClient, pppConn);


  while(1);

}

void sig_handler(int sig) {

  switch (sig)
  {
  case SIGHUP:
          sd_journal_print(LOG_INFO, "Received SIGHUP\n");
          abort();
          break;
  case SIGTERM:
          sd_journal_print(LOG_INFO, "Received SIGTERM\n");
          abort();
          break;
  default:
          sd_journal_print(LOG_INFO, "wasn't expecting that!\n");
          abort();
          break;
  }
}


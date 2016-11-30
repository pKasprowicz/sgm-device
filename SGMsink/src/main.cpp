
#include "PppConnection.h"
#include "INetworkProvider.h"
#include "SgmSink.h"
#include "MqttProtocol.h"
#include "DeviceId.h"

#include "Logger.h"
#include "SharedMemory.h"

#include <systemd/sd-journal.h>
#include <sys/socket.h>
#include <sys/un.h>
#include<time.h>
#include <csignal>
#include <thread>

void sig_handler(int sig);

std::string sinkServerURI{"tcp://51.255.203.130:1883"};
std::string sinkClientId{""};

int main()
{

  //TODO read config value and pass data via c'tors

  std::signal(SIGHUP, sig_handler);
  std::signal(SIGTERM, sig_handler);
  std::signal(SIGKILL, sig_handler);
  sd_journal_print(LOG_INFO, "SGMdatasink service started!");

  DeviceId::getDeviceId(sinkClientId);

  SGM_LOG_INFO("Acquired device id: %s", sinkClientId.c_str());

  SharedMemory sharedMem;
  if (SharedMemory::Result::INIT_ERROR == sharedMem.init())
  {
    SGM_LOG_ERROR("Error during shared memory initialization");
    return -1;
  }

  MqttProtocol mqttProto(sinkServerURI, sinkClientId);
  PppConnection pppConn(sharedMem);
  SgmSink sgmSink(mqttProto, pppConn);

  std::thread sinkThread(sgmSink);

  sinkThread.join();

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

    case SIGKILL:
      sd_journal_print(LOG_INFO, "Received SIGTERM\n");
      abort();
      break;

    default:
      sd_journal_print(LOG_INFO, "wasn't expecting that!\n");
      abort();
      break;
  }
}


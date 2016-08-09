#include "SensorServiceApplication.h"

#include "Logger.h"

#include <csignal>

void signalHandler(int signal)
{
  switch(signal)
  {
  case SIGKILL:
    break;

  default:
    break;

  }
}

int main() {


  SensorServiceApplication app;
  SGM_LOG_INFO("Sensor service started!");

  app.run();


  SGM_LOG_INFO("Shutting down sensor service...");
  return 0;
}

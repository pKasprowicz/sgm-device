#include "SensorServiceApplication.h"

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

  app.run();

  return 0;
}

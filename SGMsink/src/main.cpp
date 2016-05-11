#include <stdio.h>
#include <stdlib.h>

#include <systemd/sd-journal.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/un.h>
#include<time.h>
#include <csignal>

void sig_handler(int sig);

int main()
{

  std::signal(SIGHUP, sig_handler);
  std::signal(SIGTERM, sig_handler);
  sd_journal_print(LOG_INFO, "SGMdatasink service started!");

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


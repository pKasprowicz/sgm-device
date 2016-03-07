#ifdef __cplusplus
extern "C"{
#endif

#include <stdio.h>
#include <stdlib.h>
#include <systemd/sd-daemon.h>
#include <systemd/sd-journal.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/un.h>
#include<time.h>
#include<pthread.h>

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C"{
#endif

static pthread_t thread1, thread2;

#ifdef __cplusplus
}
#endif

extern "C" void * receive_thread(void * args)
{
  sd_journal_print(LOG_INFO, "Daemon started\n");
          unsigned int socketFD = 0U;

          {
            unsigned int socketsCount = 0U;
            while ((socketsCount = sd_listen_fds(0)) == 0U)
            {

            }
            sd_journal_print(LOG_ERR, "Found %d sockets\n", socketsCount);
            for (socketFD = SD_LISTEN_FDS_START; socketFD < SD_LISTEN_FDS_START + socketsCount; ++socketFD)
            {
                    if (sd_is_socket_unix(socketFD, SOCK_STREAM, -1, NULL, 0))
                    {
                            break;
                    }
                    if(socketFD == (SD_LISTEN_FDS_START + socketsCount - 1U))
                    {
                            sd_journal_print(LOG_ERR, "No socket found\n");
                            return NULL;
                    }
            }

          }

          uint8_t rxBuffer[255];
          unsigned int rxSize = 0U;
          struct sockaddr_un client;
          socklen_t sockaddr_len = sizeof(struct sockaddr_un);
          unsigned int socketDescriptor = 0U;
          while(1)
          {
                  rxSize = 0U;
                  sd_journal_print(LOG_INFO, "Starting listening on socket %d ...", socketFD);
                  listen(socketFD, 3);
                  socketDescriptor = accept(socketFD, reinterpret_cast<sockaddr *>(&client), &sockaddr_len);
                  if (socketDescriptor < 0U)
                  {
                          sd_journal_print(LOG_INFO, "Error during socket connection, restarting daemon\n");
                          break;
                  }
                  sd_journal_print(LOG_INFO, "Connection accepted\n");
                  unsigned int currentRxSize = 0U;
                  while((currentRxSize = recv(socketDescriptor, rxBuffer, sizeof(rxBuffer), 0U)) > 0U )
                  {
                          rxSize += currentRxSize;
                  }
                  if (0U == currentRxSize)
                  {
                          sd_journal_print(LOG_INFO, "Client disconnected\n");
                  }
                  sd_journal_print(LOG_INFO, "Received %d bytes\n", rxSize);

          }

          return NULL;
}

extern "C" void * processing_thread(void * args)
{

}

extern "C" void sig_handler(int sig);

int main() {

        signal(SIGHUP, sig_handler);
        signal(SIGTERM, sig_handler);
        sd_journal_print(LOG_INFO, "SGMdatasink service started!");
        pthread_create(&thread1, NULL, receive_thread, NULL);
        pthread_join(thread1, NULL);

}

void sig_handler(int sig) {
        switch (sig) {
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


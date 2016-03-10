#ifdef __cplusplus
extern "C"{
#endif

#include <stdio.h>
#include <stdlib.h>

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


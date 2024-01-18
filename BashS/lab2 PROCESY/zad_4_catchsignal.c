#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

void signal_handler(int signal) {
    const char* signal_name = strsignal(signal);
    printf("Received signal: %s (%d)\n", signal_name, signal);
}

int main() {
    pid_t my_pid = getpid();
    printf("My PID: %d\n", my_pid);

    for (int i = 1; i < NSIG; ++i) {
        if (i != SIGKILL && i != SIGSTOP) {
            signal(i, signal_handler);
        }
    }

    printf("Waiting for signals (kill -9 to end, kill -19 to stop)...\n");
    while (1) {     // petla nieskonczona
        sleep(1);
    }

    return 0;
}
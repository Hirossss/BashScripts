#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <PID> <signal>\n", argv[0]);
        return 1;
    }

    int pid = atoi(argv[1]);
    int signal_number = atoi(argv[2]);

    if (kill(pid, signal_number) == 0) {
        printf("Signal %d sent to process with PID %d\n", signal_number, pid);
    } else {
        fprintf(stderr, "Error sending signal.\n");
        return 1;
    }

    return 0;
}
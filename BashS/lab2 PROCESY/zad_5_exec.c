#include <stdio.h>
#include <unistd.h>

int main() {
    printf("Hello, I'm exec program!\n");
    pid_t exec_pid = getpid();
    printf("Exec PID: %d\n", exec_pid);
    sleep(10);

    execlp("./another", "another", (char *)NULL);   // execlp() to jest replacement
    perror("exec failed");
    return 0;
}
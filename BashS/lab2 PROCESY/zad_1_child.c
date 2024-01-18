#include <stdio.h>
#include <unistd.h>

int main() {
    pid_t parent_pid = getpid();
    printf("Parent PID: %d\n", parent_pid);

    printf("\nPress Enter to create a child process...");
    getchar();

    pid_t child_pid = fork();

    if (child_pid == 0) {   // proces dziecka
        pid_t parent_of_child_pid = getppid();
        printf("\nChild PID: %d\n", getpid());
        printf("Parent of Child (PPID): %d\n", parent_of_child_pid);
    } else if (child_pid > 0) {     // proces rodzica
        printf("\nParent PID (still): %d\n", getpid());
    } else {
        fprintf(stderr, "Forking failed!\n");
        return 1;
    }

    printf("Press Enter to exit...");
    getchar();

    return 0;
}
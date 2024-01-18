#include <stdio.h>
#include <unistd.h>

int main() {
    pid_t parent_pid = getpid();
    printf("Parent PID: %d\n", parent_pid);

    printf("Press Enter to create a child process...");
    getchar();

    pid_t child_pid = fork();

    if (child_pid == 0) {
        pid_t parent_of_child_pid = getppid();
        printf("Child PID: %d\n", getpid());
        printf("Parent of Child (PPID): %d\n", parent_of_child_pid);

        printf("Press Enter to create a grandchild process...");
        getchar();

        pid_t grandchild_pid = fork();

        if (grandchild_pid == 0) {      // proces wnuczka
            pid_t parent_of_grandchild_pid = getppid();
            printf("Grandchild PID: %d\n", getpid());
            printf("Parent of Grandchild (PPID): %d\n", parent_of_grandchild_pid);
        } else if (grandchild_pid > 0) {    // proces dziecka
            sleep(1);
            printf("Press Enter to exit child...");
            getchar();
        } else {
            fprintf(stderr, "Grandchild forking failed!\n");
        }
    } else if (child_pid > 0) { // proces rodzica
        sleep(1);
        printf("\nParent PID (still): %d\n", getpid());
        printf("Press Enter to exit parent...");
        getchar();
    } else {
        fprintf(stderr, "Child forking failed!\n");
    }

    return 0;
}
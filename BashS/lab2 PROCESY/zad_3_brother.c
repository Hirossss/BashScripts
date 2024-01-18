#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t parent_pid = getpid();
    printf("Parent PID: %d\n", parent_pid);
    getchar();

    pid_t child1_pid = fork();

    if (child1_pid == 0) {
        pid_t parent_of_child1_pid = getppid();
        printf("Child 1 PID: %d\n", getpid());
        printf("Parent of Child 1 (PPID): %d\n", parent_of_child1_pid);
        getchar();

        pid_t child2_pid = fork();

        if (child2_pid == 0) {
            pid_t parent_of_child2_pid = getppid();
            printf("Child 2 PID: %d\n", getpid());
            printf("Parent of Child 2 (PPID): %d\n", parent_of_child2_pid);
            getchar(); 

            pid_t grandchild_pid = fork();

            if (grandchild_pid == 0) {
                pid_t parent_of_grandchild_pid = getppid();
                printf("Grandchild PID: %d\n", getpid());
                printf("Parent of Grandchild (PPID): %d\n", parent_of_grandchild_pid);
                getchar(); 
            } else if (grandchild_pid > 0) {
                wait(NULL);     // czekamy az wyzszy w instancji skonczy prace
                getchar();
            } else {
                fprintf(stderr, "Grandchild forking failed!\n");
            }
        } else if (child2_pid > 0) {
            wait(NULL); 
            getchar(); 
        } else {
            fprintf(stderr, "Child 2 forking failed!\n");
        }
    } else if (child1_pid > 0) {
        wait(NULL); 
        getchar(); 
    } else {
        fprintf(stderr, "Child 1 forking failed!\n");
    }

    return 0;
}
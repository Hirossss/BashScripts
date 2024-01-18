#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("Hello, I'm system program!\n");
    getchar(); 

    int result = system("ls -al /var/log/");

    if (result == -1) {
        perror("Failed to execute the system command");
    }

    return 0;
}
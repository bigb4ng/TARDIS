#define _DEFAULT_SOURCE

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/auxv.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char *argv[], char *envp[]) {
    pid_t child = fork();
    if (child == 0) {
        char *args[] = {"/bin/sleep", "10", NULL};
        execve(args[0], &args, NULL);
        exit(EXIT_SUCCESS);
    }

    wait(NULL);
    exit(EXIT_SUCCESS);
}
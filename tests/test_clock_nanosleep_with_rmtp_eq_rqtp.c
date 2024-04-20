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
    struct timespec time = {.tv_sec = 10, .tv_nsec = 0};
    clock_nanosleep(0, 0, &time, &time);

    return 0;
}
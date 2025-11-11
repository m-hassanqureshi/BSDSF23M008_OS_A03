#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "shell.h"

void execute(char **args) {
    if (args[0] == NULL) return;

    pid_t pid = fork();
    if (pid == 0) { // child
        if (execvp(args[0], args) == -1)
            perror("HQshell");
        exit(EXIT_FAILURE);
    } else if (pid > 0) { // parent
        int status;
        waitpid(pid, &status, 0);
    } else {
        perror("fork");
    }
}

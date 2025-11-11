#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include "shell.h"

void execute(char **args) {
    if (args[0] == NULL) return;

    pid_t pid = fork();
    if (pid == 0) {
        execute_with_redirection(args);
        perror("execvp");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        waitpid(pid, NULL, 0);
    } else {
        perror("fork");
    }
}

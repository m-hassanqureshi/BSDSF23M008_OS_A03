#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "shell.h"

/* Helper to execute with < and > redirection */
int execute_with_redirection(char **args) {
    int in_fd = -1, out_fd = -1;
    int i = 0;

    for (; args[i] != NULL; i++) {
        if (strcmp(args[i], "<") == 0) {
            args[i] = NULL;
            in_fd = open(args[i + 1], O_RDONLY);
            if (in_fd < 0) { perror("open input"); return -1; }
            dup2(in_fd, STDIN_FILENO);
            close(in_fd);
        } 
        else if (strcmp(args[i], ">") == 0) {
            args[i] = NULL;
            out_fd = open(args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (out_fd < 0) { perror("open output"); return -1; }
            dup2(out_fd, STDOUT_FILENO);
            close(out_fd);
        }
    }

    execvp(args[0], args);
    perror("execvp");
    exit(1);
}

/* Handle simple single-pipe command */
int execute_with_pipes(char *line) {
    char *left = strtok(line, "|");
    char *right = strtok(NULL, "|");

    if (!right) return 0; // no pipe present

    int pipefd[2];
    pipe(pipefd);

    pid_t pid1 = fork();
    if (pid1 == 0) {
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]); close(pipefd[1]);
        char **left_args = tokenize(left);
        if (!handle_builtin(left_args))
            execute_with_redirection(left_args);
        free_tokens(left_args);
        exit(0);
    }

    pid_t pid2 = fork();
    if (pid2 == 0) {
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[1]); close(pipefd[0]);
        char **right_args = tokenize(right);
        if (!handle_builtin(right_args))
            execute_with_redirection(right_args);
        free_tokens(right_args);
        exit(0);
    }

    close(pipefd[0]);
    close(pipefd[1]);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    return 1;
}

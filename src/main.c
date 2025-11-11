#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/wait.h>
#include "shell.h"

int main() {
    char *line, *cmd;
    char **args;

    while (1) {
        reap_jobs(); // clean up finished background jobs

        line = read_cmd();
        if (strlen(line) == 0) { free(line); continue; }
        if (strncmp(line, "if ", 3) == 0 && strstr(line, "fi")) {
            // Pass entire block intact to the if handler
            handle_if_structure(line);
            free(line);
            continue;
        }

        // Otherwise, process normally with ; chaining
        cmd = strtok(line, ";");
        while (cmd != NULL) {
            char *trim = trim_whitespace(cmd);

            // check if-then-else-fi (multi-line form)
            if (handle_if_structure(trim)) {
                cmd = strtok(NULL, ";");
                continue;
            }

            // detect background process
            int background = 0;
            int len = strlen(trim);
            if (len > 0 && trim[len - 1] == '&') {
                background = 1;
                trim[len - 1] = '\0';
                trim = trim_whitespace(trim);
            }

            if (strchr(trim, '|')) {
                execute_with_pipes(trim);
            } else {
                args = tokenize(trim);
                if (!handle_builtin(args)) {
                    pid_t pid = fork();
                    if (pid == 0) {
                        execute_with_redirection(args);
                        perror("execvp");
                        exit(1);
                    } else if (pid > 0) {
                        if (background) {
                            printf("[BG Started] PID %d: %s\n", pid, trim);
                            add_job(pid, trim);
                        } else {
                            waitpid(pid, NULL, 0);
                        }
                    } else {
                        perror("fork");
                    }
                }
                free_tokens(args);
            }
            cmd = strtok(NULL, ";");
        }
        free(line);
    }
}

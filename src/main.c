#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>
#include "shell.h"

int main() {
    char *line;
    char *cmd;
    char **args;

    while (1) {
        reap_jobs(); // clean zombies
        line = read_cmd();
        if (strlen(line) == 0) { free(line); continue; }

        // Handle !n recall
        if (line[0] == '!' && isdigit(line[1])) {
            int idx = atoi(&line[1]);
            char *cmd_from_hist = get_history_cmd(idx);
            if (cmd_from_hist) {
                printf("Re-executing: %s\n", cmd_from_hist);
                free(line);
                line = cmd_from_hist;
            } else { free(line); continue; }
        }

        // Command chaining (;)
        cmd = strtok(line, ";");
        while (cmd != NULL) {
            char *trim = strdup(cmd);
            while (isspace(*trim)) trim++;

            // Background execution (&)
            int background = 0;
            if (trim[strlen(trim) - 1] == '&') {
                background = 1;
                trim[strlen(trim) - 1] = '\0';
            }

            if (strchr(trim, '|')) {
                execute_with_pipes(trim);
            } else {
                args = tokenize(trim);
                if (!handle_builtin(args)) {
                    pid_t pid = fork();
                    if (pid == 0) {
                        execute_with_redirection(args);
                        exit(0);
                    } else if (pid > 0) {
                        if (background) {
                            printf("[BG Started] PID %d: %s\n", pid, trim);
                            add_job(pid, trim);
                        } else {
                            int status;
                            waitpid(pid, &status, 0);
                        }
                    } else perror("fork");
                }
                free_tokens(args);
            }
            free(trim);
            cmd = strtok(NULL, ";");
        }
        free(line);
    }
    return 0;
}

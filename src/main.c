#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "shell.h"

int main() {
    char *line;
    char **args;

    while (1) {
        line = read_cmd();
        if (strlen(line) == 0) { free(line); continue; }

        // !n recall
        if (line[0] == '!' && isdigit(line[1])) {
            int idx = atoi(&line[1]);
            char *cmd_from_hist = get_history_cmd(idx);
            if (cmd_from_hist) {
                printf("Re-executing: %s\n", cmd_from_hist);
                free(line);
                line = cmd_from_hist;
            } else {
                free(line);
                continue;
            }
        }

        // check for pipe
        if (strchr(line, '|')) {
            execute_with_pipes(line);
            free(line);
            continue;
        }

        args = tokenize(line);

        if (!handle_builtin(args))
            execute(args);

        free_tokens(args);
        free(line);
    }
    return 0;
}

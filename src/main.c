#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"

int main() {
    char *line;
    char **args;

    while (1) {
        display_prompt();
        line = read_cmd();
        if (strlen(line) == 0) { free(line); continue; }
        args = tokenize(line);

        if (!handle_builtin(args)) {
            execute(args);
        }

        free_tokens(args);
        free(line);
    }
    return 0;
}

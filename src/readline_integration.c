#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "shell.h"

// Display prompt and get user input via GNU Readline
char *read_cmd() {
    char *line = readline("HQshell:- ");
    if (!line) {      // Ctrl-D pressed
        printf("\n");
        exit(0);
    }

    if (*line) {      // non-empty → add to history
        add_history(line);
    }
    return line;
}

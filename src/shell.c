#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"

void display_prompt() {
    // prompt handled by readline(), but kept for compatibility
}
char **tokenize(char *line) {
    int bufsize = MAX_TOKENS, position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;

    if (!tokens) {
        fprintf(stderr, "allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, " \t\r\n");
    while (token != NULL) {
        tokens[position++] = token;
        if (position >= bufsize) {
            bufsize += MAX_TOKENS;
            tokens = realloc(tokens, bufsize * sizeof(char*));
        }
        token = strtok(NULL, " \t\r\n");
    }
    tokens[position] = NULL;
    return tokens;
}

void free_tokens(char **args) {
    free(args);
}
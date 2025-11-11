#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"

void display_prompt() {
    printf("HQshell:- ");
    fflush(stdout);
}

char *read_cmd() {
    char *line = malloc(MAX_CMD_LEN);
    if (!fgets(line, MAX_CMD_LEN, stdin)) {
        printf("\n");
        exit(0);
    }
    line[strcspn(line, "\n")] = '\0';
    return line;
}

char **tokenize(char *line) {
    char **tokens = malloc(MAX_TOKENS * sizeof(char *));
    char *token;
    int pos = 0;
    token = strtok(line, " ");
    while (token != NULL) {
        tokens[pos++] = strdup(token);
        token = strtok(NULL, " ");
    }
    tokens[pos] = NULL;
    return tokens;
}

void free_tokens(char **args) {
    for (int i = 0; args[i]; i++)
        free(args[i]);
    free(args);
}

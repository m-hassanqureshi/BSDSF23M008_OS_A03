#ifndef SHELL_H
#define SHELL_H

#define MAX_TOKENS 64
#define MAX_CMD_LEN 1024

void display_prompt();
char *read_cmd();
char **tokenize(char *line);
void execute(char **args);
void free_tokens(char **args);
int handle_builtin(char **args);

#endif

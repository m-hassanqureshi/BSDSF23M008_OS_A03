#ifndef SHELL_H
#define SHELL_H

#define MAX_TOKENS 64
#define MAX_CMD_LEN 1024
#define HISTORY_SIZE 20

void display_prompt();
char *read_cmd();              // now wraps readline()
char **tokenize(char *line);
void execute(char **args);
void free_tokens(char **args);
int handle_builtin(char **args);
void add_history(const char *cmd);
void show_history();
char *get_history_cmd(int index);

#endif

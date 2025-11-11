#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <ctype.h>
#include <signal.h>

#define MAX_TOKENS 64
#define MAX_CMD_LEN 1024
#define HISTORY_SIZE 20
#define MAX_JOBS 32

// helper
char *trim_whitespace(char *str);

// base
void display_prompt();
char *read_cmd();
char **tokenize(char *line);
void free_tokens(char **args);
void execute(char **args);
int handle_builtin(char **args);

// history
void add_history(const char *cmd);
void show_history();
char *get_history_cmd(int index);

// I/O + pipes
int execute_with_redirection(char **args);
int execute_with_pipes(char *line);

// multitasking
void add_job(pid_t pid, const char *cmd);
void reap_jobs();
void list_jobs();

// control flow
int handle_if_structure(char *line);

typedef struct VarNode {
    char name[64];
    char value[256];
    struct VarNode *next;
} VarNode;

void set_variable(const char *name, const char *value);
char *get_variable(const char *name);
void print_all_variables();
void unset_variable(const char *name);
void expand_variables(char **args);
#endif

#ifndef SHELL_H
#define SHELL_H

// ========== STANDARD LIBRARIES ==========
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>        // fork(), execvp(), pid_t
#include <sys/types.h>     // pid_t, open()
#include <sys/wait.h>      // wait(), waitpid(), macros
#include <fcntl.h>         // open(), O_CREAT, O_TRUNC
#include <ctype.h>         // isdigit()
#include <signal.h>        // signal handling (future use)

// ========== CONSTANTS ==========
#define MAX_TOKENS 64
#define MAX_CMD_LEN 1024
#define HISTORY_SIZE 20
#define MAX_JOBS 32

// ========== FUNCTION PROTOTYPES ==========

// shell core
void display_prompt();
char *read_cmd();
char **tokenize(char *line);
void execute(char **args);
void free_tokens(char **args);

// builtins and history
int handle_builtin(char **args);
void add_history(const char *cmd);
void show_history();
char *get_history_cmd(int index);

// redirection & pipes
int execute_with_redirection(char **args);
int execute_with_pipes(char *line);

// multitasking
void add_job(pid_t pid, const char *cmd);
void reap_jobs();
void list_jobs();

// control flow
int handle_if_structure();

// variables
typedef struct VarNode {
    char name[64];
    char value[256];
    struct VarNode *next;
} VarNode;

void set_variable(const char *name, const char *value);
char *get_variable(const char *name);
void print_all_variables();
void expand_variables(char **args);

#endif

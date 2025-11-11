#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shell.h"

int handle_builtin(char **args) {
    if (args[0] == NULL) return 0;

    // exit command
    if (strcmp(args[0], "exit") == 0) {
        printf("Exiting HQshell...\n");
        exit(0);
    }

    // cd command
    else if (strcmp(args[0], "cd") == 0) {
        if (args[1] == NULL) {
            fprintf(stderr, "cd: missing argument\n");
        } else if (chdir(args[1]) != 0) {
            perror("cd");
        }
        return 1;
    }

    // help command
    else if (strcmp(args[0], "help") == 0) {
        printf("HQShell Built-in Commands:\n");
        printf("  cd <dir>    - Change directory\n");
        printf("  help        - Show this help message\n");
        printf("  exit        - Exit the shell\n");
        printf("  jobs        - Show background jobs (placeholder)\n");
        printf("  set         - Show all shell variables\n");  
        return 1;
    }
    else if (strcmp(args[0], "jobs") == 0) {
    list_jobs();
    return 1;
    }

    else if (strcmp(args[0], "history") == 0) {
    show_history();
    return 1;   
    }
    if (strcmp(args[0], "set") == 0) {
        print_all_variables();
        return 1;
    }
    if (strcmp(args[0], "unset") == 0 && args[1]) {
        unset_variable(args[1]);
        return 1;
    }
    return 0;  // not a built-in command
}

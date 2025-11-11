#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>
#include <unistd.h>
#include "shell.h"

// Helper: remove leading/trailing spaces and newlines safely
static char *clean_str(char *s) {
    while (isspace(*s)) s++;
    if (*s == 0) return s;
    char *end = s + strlen(s) - 1;
    while (end > s && isspace(*end)) end--;
    *(end + 1) = '\0';
    return s;
}

// -----------------------------------------------------------
// handle_if_structure()
// Supports:
//   if CMD; then CMD; fi
//   if CMD; then CMD; else CMD; fi
//   Handles redirection, pipes, and mixed spacing (;then, then, etc.)
// -----------------------------------------------------------
int handle_if_structure(char *line) {
    if (strncmp(line, "if ", 3) != 0)
        return 0; // not an if-statement

    // Support both " then" and ";then"
    char *then_part = strstr(line, " then");
    if (!then_part) then_part = strstr(line, ";then");

    char *else_part = strstr(line, " else");
    if (!else_part) else_part = strstr(line, ";else");

    char *fi_part = strstr(line, " fi");
    if (!fi_part) fi_part = strstr(line, ";fi");

    if (!then_part || !fi_part) {
        fprintf(stderr, "Syntax error: missing 'then' or 'fi'\n");
        return 1; // handled but invalid
    }

    // --- Split the command into parts ---
    *then_part = '\0';
    then_part += (then_part[0] == ';') ? 5 : 6; // skip ";then" or " then"

    if (else_part)
        *else_part = '\0';
    *fi_part = '\0';

    char *condition = clean_str(line + 3);             // after "if "
    char *then_cmd  = clean_str(then_part);
    char *else_cmd  = else_part ? clean_str(else_part + ((else_part[0] == ';') ? 5 : 6)) : NULL;

    // --- Execute the condition ---
    pid_t pid = fork();
    int status;

    if (pid == 0) {
        // Use system shell for robust parsing (handles >, |, etc.)
        execl("/bin/sh", "sh", "-c", condition, NULL);
        perror("execl (if condition)");
        exit(1);
    } 
    else if (pid > 0) {
        waitpid(pid, &status, 0);
        int exit_code = WEXITSTATUS(status);

        if (exit_code == 0) {
            printf("[IF Success → THEN]\n");
            if (strlen(then_cmd) > 0)
                system(then_cmd);
        } else {
            printf("[IF Failed → ELSE]\n");
            if (else_cmd && strlen(else_cmd) > 0)
                system(else_cmd);
        }
    } 
    else {
        perror("fork");
    }

    return 1; // handled
}

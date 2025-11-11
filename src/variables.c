#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"

static VarNode *var_list = NULL;



void set_variable(const char *name, const char *value) {
    VarNode *curr = var_list;
    while (curr) {
        if (strcmp(curr->name, name) == 0) {
            strncpy(curr->value, value, sizeof(curr->value) - 1);
            return;
        }
        curr = curr->next;
    }
    VarNode *newnode = malloc(sizeof(VarNode));
    strncpy(newnode->name, name, sizeof(newnode->name) - 1);
    strncpy(newnode->value, value, sizeof(newnode->value) - 1);
    newnode->next = var_list;
    var_list = newnode;
}

char *get_variable(const char *name) {
    VarNode *curr = var_list;
    while (curr) {
        if (strcmp(curr->name, name) == 0)
            return curr->value;
        curr = curr->next;
    }
    return NULL;
}

void unset_variable(const char *name) {
    VarNode *curr = var_list, *prev = NULL;
    while (curr) {
        if (strcmp(curr->name, name) == 0) {
            if (prev) prev->next = curr->next;
            else var_list = curr->next;
            free(curr);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

void print_all_variables() {
    VarNode *curr = var_list;
    while (curr) {
        printf("%s=%s\n", curr->name, curr->value);
        curr = curr->next;
    }
}

// expand $VAR inside tokens
void expand_variables(char **args) {
    for (int i = 0; args[i] != NULL; i++) {
        if (args[i][0] == '$') {
            char *val = get_variable(args[i] + 1); // skip $
            if (val) args[i] = val;
        }
    }
}

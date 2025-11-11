#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"

static char *history[HISTORY_SIZE];
static int hist_count = 0;

void add_history(const char *cmd) {
    if (cmd == NULL || strlen(cmd) == 0)
        return;

    if (hist_count < HISTORY_SIZE) {
        history[hist_count++] = strdup(cmd);
    } else {
        free(history[0]);
        for (int i = 1; i < HISTORY_SIZE; i++)
            history[i - 1] = history[i];
        history[HISTORY_SIZE - 1] = strdup(cmd);
    }
}

void show_history() {
    for (int i = 0; i < hist_count; i++)
        printf("%d %s\n", i + 1, history[i]);
}

char *get_history_cmd(int index) {
    if (index <= 0 || index > hist_count) {
        fprintf(stderr, "Invalid history index: %d\n", index);
        return NULL;
    }
    return strdup(history[index - 1]);
}

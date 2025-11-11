#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include "shell.h"

typedef struct {
    pid_t pid;
    char command[256];
} Job;

static Job jobs[MAX_JOBS];
static int job_count = 0;

/* Add background job */
void add_job(pid_t pid, const char *cmd) {
    if (job_count < MAX_JOBS) {
        jobs[job_count].pid = pid;
        strncpy(jobs[job_count].command, cmd, sizeof(jobs[job_count].command) - 1);
        job_count++;
    }
}

/* Remove finished jobs */
void reap_jobs() {
    int status;
    pid_t pid;
    for (int i = 0; i < job_count; ) {
        pid = waitpid(jobs[i].pid, &status, WNOHANG);
        if (pid > 0) { // process finished
            printf("[Job Done] PID %d: %s\n", jobs[i].pid, jobs[i].command);
            for (int j = i; j < job_count - 1; j++)
                jobs[j] = jobs[j + 1];
            job_count--;
        } else {
            i++;
        }
    }
}

/* Print active background jobs */
void list_jobs() {
    printf("Active background jobs:\n");
    for (int i = 0; i < job_count; i++)
        printf("[%d] PID=%d %s\n", i + 1, jobs[i].pid, jobs[i].command);
    if (job_count == 0)
        printf("No active jobs.\n");
}

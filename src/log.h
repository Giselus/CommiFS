#include <errno.h>
#include <fuse.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "context.h"

#define LOG_FILE    

FILE *log_open()
{
    FILE *logfile;
    
    logfile = fopen("comiFS.log", "w");
    if (logfile == NULL) {
        perror("logfile");
        exit(EXIT_FAILURE);
    }
    
    setvbuf(logfile, NULL, _IOLBF, 0);
    return logfile;
}

void log_syscall(char *syscall, const char *path)
{
    fprintf(COMI_CONTEXT->logfile, "Path: %s    Syscall: %s\n", path, syscall);   
}

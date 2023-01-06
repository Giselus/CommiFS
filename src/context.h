// #include <stdio.h>

struct comi_state {
    // FILE *logfile;
    char *rootdir;
};

#define COMI_CONTEXT ((struct comi_state *) fuse_get_context()->private_data)

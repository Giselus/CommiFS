#include <stdio.h>
#include <stdlib.h>
// it is very important that this define is before fuse.h is included
#define FUSE_USE_VERSION 26
#include <fuse.h>

struct fuse_operations commi_oper = {
  .getattr = NULL,
  .readlink = NULL,
  .getdir = NULL,
  .mknod = NULL,
  .mkdir = NULL,
  .unlink = NULL,
  .rmdir = NULL,
  .symlink = NULL,
  .rename = NULL,
  .link = NULL,
  .chmod = NULL,
  .chown = NULL,
  .truncate = NULL,
  .utime = NULL,
  .open = NULL,
  .read = NULL,
  .write = NULL,
  .statfs = NULL,
  .flush = NULL,
  .release = NULL,
  .fsync = NULL,
  .opendir = NULL,
  .readdir = NULL,
  .releasedir = NULL,
  .fsyncdir = NULL,
  .init = NULL,
  .destroy = NULL,
  .access = NULL,
  .ftruncate = NULL,
  .fgetattr = NULL,
};


int main(int argc, char *argv[]) {
  fuse_main( argc, argv, &commi_oper, NULL);
}
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
// it is very important that this define is before fuse.h is included
#define FUSE_USE_VERSION 26
#include <fuse.h>

int comi_getattr(const char *path, struct stat *statbuf)
{
    return lstat(path, statbuf);
}

int comi_mkdir(const char *path, mode_t mode)
{ 
    return mkdir(path, mode);
}

int comi_chmod(const char *path, mode_t mode)
{
    return chmod(path, mode);
}

int comi_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    return pread(fi->fh, buf, size, offset);
}

int comi_write(const char *path, const char *buf, size_t size, off_t offset,
	     struct fuse_file_info *fi)
{     
    return pwrite(fi->fh, buf, size, offset);
}

int comi_opendir(const char *path, struct fuse_file_info *fi)
{
    DIR *dp;
    int retstat = 0;

    dp = opendir(path);
    if (dp == NULL) {
      //TODO
    }
    
    fi->fh = (intptr_t) dp;
    
    return retstat;
}

int comi_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset,
	       struct fuse_file_info *fi)
{
    DIR *dp;
    struct dirent *de;
    dp = (DIR *) (uintptr_t) fi->fh;
    de = readdir(dp);
    if (de == 0) {
      //TODO
      return -1;
    }
    return 0;
}

struct fuse_operations commi_oper = {
  .getattr = comi_getattr,
  .readlink = NULL,
  .getdir = NULL,
  .mknod = NULL,
  .mkdir = comi_mkdir,
  .unlink = NULL,
  .rmdir = NULL,
  .symlink = NULL,
  .rename = NULL,
  .link = NULL,
  .chmod = comi_chmod,
  .chown = NULL,
  .truncate = NULL,
  .utime = NULL,
  .open = NULL,
  .read = comi_read,
  .write = comi_write,
  .statfs = NULL,
  .flush = NULL,
  .release = NULL,
  .fsync = NULL,
  .opendir = comi_opendir,
  .readdir = comi_readdir,
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

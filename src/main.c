#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/types.h>
// it is very important that this define is before fuse.h is included
#define FUSE_USE_VERSION 26
#include <fuse.h>

static void comi_fullpath(char fpath[PATH_MAX], const char *path)
{
	strcpy(fpath, "/home/lukasz/Test/ProjektZespołowy/commifs/src/comiFolder");
	strncat(fpath, path, PATH_MAX);
}

static int comi_getattr(const char *path, struct stat *stbuf)
{
	int res;
	char fpath[PATH_MAX];
	comi_fullpath(fpath, path);
	res = lstat(fpath, stbuf);
	if (res == -1)
		return -errno;

	return 0;
}

static int comi_access(const char *path, int mask)
{
	int res;
	char fpath[PATH_MAX];
	comi_fullpath(fpath, path);
	res = access(fpath, mask);
	if (res == -1)
		return -errno;

	return 0;
}

static int comi_readlink(const char *path, char *buf, size_t size)
{
	int res;
	char fpath[PATH_MAX];
	comi_fullpath(fpath, path);
	res = readlink(fpath, buf, size - 1);
	if (res == -1)
		return -errno;

	buf[res] = '\0';
	return 0;
}


static int comi_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
		       off_t offset, struct fuse_file_info *fi)
{
	DIR *dp;
	struct dirent *de;

	(void) offset;
	(void) fi;
	char fpath[PATH_MAX];
	comi_fullpath(fpath, path);
	printf("Readdir %s\n", path);
	dp = opendir(fpath);
	if (dp == NULL)
		return -errno;

	while ((de = readdir(dp)) != NULL) {
		struct stat st;
		memset(&st, 0, sizeof(st));
		st.st_ino = de->d_ino;
		st.st_mode = de->d_type << 12;
		if (filler(buf, de->d_name, &st, 0))
			break;
	}

	closedir(dp);
	return 0;
}

static int comi_mknod(const char *path, mode_t mode, dev_t rdev)
{
	int res;
	char fpath[PATH_MAX];
	comi_fullpath(fpath, path);
	/* On Linux this could just be 'mknod(path, mode, rdev)' but this
	   is more portable */
	if (S_ISREG(mode)) {
		res = open(fpath, O_CREAT | O_EXCL | O_WRONLY, mode);
		if (res >= 0)
			res = close(res);
	} else if (S_ISFIFO(mode))
		res = mkfifo(fpath, mode);
	else
		res = mknod(fpath, mode, rdev);
	if (res == -1)
		return -errno;

	return 0;
}

static int comi_mkdir(const char *path, mode_t mode)
{
	int res;
	char fpath[PATH_MAX];
	comi_fullpath(fpath, path);
	printf("Mkdir %s\n", path);
	res = mkdir(fpath, mode);
	if (res == -1)
		return -errno;

	return 0;
}

static int comi_unlink(const char *path)
{
	int res;
	char fpath[PATH_MAX];
	comi_fullpath(fpath, path);
	res = unlink(fpath);
	if (res == -1)
		return -errno;

	return 0;
}

static int comi_rmdir(const char *path)
{
	int res;
	char fpath[PATH_MAX];
	comi_fullpath(fpath, path);
	printf("Rmdir %s\n", path);
	res = rmdir(fpath);
	if (res == -1)
		return -errno;

	return 0;
}

static int comi_symlink(const char *from, const char *to)
{
	int res;
	char ffrom[PATH_MAX];
	comi_fullpath(ffrom, from);
	char fto[PATH_MAX];
	comi_fullpath(fto, to);
	res = symlink(ffrom, fto);
	if (res == -1)
		return -errno;

	return 0;
}

static int comi_rename(const char *from, const char *to)
{
	int res;
	char ffrom[PATH_MAX];
	comi_fullpath(ffrom, from);
	char fto[PATH_MAX];
	comi_fullpath(fto, to);
	res = rename(ffrom, fto);
	if (res == -1)
		return -errno;

	return 0;
}

static int comi_link(const char *from, const char *to)
{
	int res;
	char ffrom[PATH_MAX];
	comi_fullpath(ffrom, from);
	char fto[PATH_MAX];
	comi_fullpath(fto, to);
	res = link(ffrom, fto);
	if (res == -1)
		return -errno;

	return 0;
}

static int comi_chmod(const char *path, mode_t mode)
{
	int res;
	char fpath[PATH_MAX];
	comi_fullpath(fpath, path);
	printf("Chmod +x %s", path);
	res = chmod(fpath, mode);
	if (res == -1)
		return -errno;

	return 0;
}

static int comi_chown(const char *path, uid_t uid, gid_t gid)
{
	int res;
	char fpath[PATH_MAX];
	comi_fullpath(fpath, path);
	res = lchown(fpath, uid, gid);
	if (res == -1)
		return -errno;

	return 0;
}

static int comi_truncate(const char *path, off_t size)
{
	int res;
	char fpath[PATH_MAX];
	comi_fullpath(fpath, path);
	res = truncate(fpath, size);
	if (res == -1)
		return -errno;

	return 0;
}

#ifdef HAVE_UTIMENSAT
static int comi_utimens(const char *path, const struct timespec ts[2])
{
	int res;
	char fpath[PATH_MAX];
	comi_fullpath(fpath, path);
	/* don't use utime/utimes since they follow symlinks */
	res = utimensat(0, fpath, ts, AT_SYMLINK_NOFOLLOW);
	if (res == -1)
		return -errno;

	return 0;
}
#endif

#define hashLength 16

static void comi_divide(char fpath[PATH_MAX], const char *path){
	char realPath[hashLength * 4];
	for(int i = 0; i < hashLength; i++){
		realPath[i*2] = path[i];
		realPath[i*2+1] = '/';
	}
	realPath[hashLength*2] = 0;

	char realPathWithPrefix[hashLength * 5];
	
	strncat(realPath, path, hashLength * 2);
	strcpy(realPathWithPrefix, "/rootOfAllFiles/");
	strncat(realPathWithPrefix, realPath, PATH_MAX);
	comi_fullpath(fpath, realPathWithPrefix);
}


static int get_real_path(char *fullRealPath, const char *path){
	printf("\n\nREAL PATH: %s\n", path);
	char fpath[PATH_MAX];
	comi_fullpath(fpath, path);
	printf("Comi fullpath: %s\n", fpath);
	int fd = open(fpath, O_RDONLY);
	if(fd == -1)
		return fd;

	char buf[hashLength * 2]; //hash_size
	int res = pread(fd, buf, hashLength, 0); //hash_size
	buf[hashLength] = 0;
	close(fd);
	if(res == -1) {
		return res;
	}
	
	comi_divide(fullRealPath, buf);
	printf("FullRealPAth: %s\n", fullRealPath);
	return 0;
}

static int proxy_open(const char *path, struct fuse_file_info *fi) {
	char fpath[PATH_MAX];
	int res = get_real_path(fpath, path);
	if(res == -1)
		return res;
	if(fi == NULL) {
		printf("Open with no file info %s\n", fpath);
		return open(fpath, O_RDONLY | O_WRONLY);
	}
	return open(fpath, fi->flags);
}

static int comi_open(const char *path, struct fuse_file_info *fi)
{
	printf("Open %s\n", path);
	int fd = proxy_open(path, fi);
	if (fd == -1) {
		return -1;
	}
	fi->fh = fd;
	return 0;
}

static int comi_read(const char *path, char *buf, size_t size, off_t offset,
		    struct fuse_file_info *fi)
{
	int fd;
	int res;
	printf("Read %s %d\n", path, (fi == NULL));
	if(fi == NULL)
		fd = proxy_open(path, NULL);
	else
		fd = fi->fh;
	
	if (fd == -1)
		return -errno;

	res = pread(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	if(fi == NULL)
		close(fd);
		
	return res;
}

static int hash_file(char *path, char *buf) 
{
	char cmd[PATH_MAX] = "shasum -a 256 ";   
	strncat(cmd, path, PATH_MAX);

	FILE *fp;

	if ((fp = popen(cmd, "r")) == NULL) {
			printf("Error opening pipe!\n");
			return -1;
	}

	fgets(buf, hashLength + 1, fp);
	buf[hashLength] = 0;

	if (pclose(fp)) {
			printf("Command not found or exited with error status\n");
			return -1;
	}

	return 0;
}

static int cp_file(char *src_path, char *dest_path) 
{
	printf("CP_FILE src: %s  dest: %s\n", src_path, dest_path);
	char cmd[2*PATH_MAX] = "cp ";   
	strncat(cmd, src_path, PATH_MAX);
	strncat(cmd, " ", 1);
	strncat(cmd, dest_path, PATH_MAX);

    FILE *fp;

    if ((fp = popen(cmd, "r")) == NULL) {
        printf("Error opening pipe!\n");
        return -1;
    }

    if (pclose(fp)) {
        printf("Command not found or exited with error status\n");
        return -1;
    }

    return 0;
}

static int mv_file(char *src_path, char *dest_path) 
{
	printf("MV_FILE src: %s  dest: %s\n", src_path, dest_path);
	char cmd[2*PATH_MAX] = "mv ";   
	strncat(cmd, src_path, PATH_MAX);
	strncat(cmd, " ", 1);
	strncat(cmd, dest_path, PATH_MAX);

    FILE *fp;

    if ((fp = popen(cmd, "r")) == NULL) {
        printf("Error opening pipe!\n");
        return -1;
    }

    if (pclose(fp)) {
        printf("Command not found or exited with error status\n");
        return -1;
    }

    return 0;
}

static int comi_write(const char *path, const char *buf, size_t size,
		     off_t offset, struct fuse_file_info *fi)
{
	if(path == NULL || path[0] == 0) {
		printf("If you want to modify a file path cannot be empty\n");
		return -1;
	}
	
	int fd;
	int res;
	(void) fi;
	char fpath[PATH_MAX] ;
	res = get_real_path(fpath, path);
	if(res == -1)
		return res;
	
	char tmp_path[PATH_MAX];
	comi_fullpath(tmp_path, "/rootOfAllFiles/tmp");
	cp_file(fpath, tmp_path);
	
	printf("TEMP path %s\n", tmp_path);

	fd = open(tmp_path, O_CREAT|O_WRONLY|O_TRUNC);
	res = pwrite(fd, buf, size, offset);
	if (res == -1)
		res = -errno;
	close(fd);

	char hash[hashLength + 1];
	hash_file(tmp_path, hash);

	char dst_path[PATH_MAX];
	comi_divide(dst_path, hash);

	struct stat st = {0};

	char hashPrefix[PATH_MAX];
	comi_fullpath(hashPrefix, "/rootOfAllFiles");
	
	for (int i = 0; i < hashLength; i++) {
		strncat(hashPrefix, "/", 1);
		strncat(hashPrefix, hash + i, 1);
		printf("Prefix %s\n", hashPrefix);
		if (stat(hashPrefix, &st) == -1) {
			mkdir(hashPrefix, 0700);
		}
	}

	mv_file(tmp_path, dst_path);
	
	char real_path[PATH_MAX];
	comi_fullpath(real_path, path);
	fd = open(real_path, O_WRONLY|O_TRUNC);
	pwrite(fd,hash,hashLength,0);
	close(fd);
	return res;
}

static int comi_statfs(const char *path, struct statvfs *stbuf)
{
	int res;
	char fpath[PATH_MAX];
	comi_fullpath(fpath, path);
	res = statvfs(fpath, stbuf);
	if (res == -1)
		return -errno;

	return 0;
}

static int comi_release(const char *path, struct fuse_file_info *fi)
{
	(void) path;
	close(fi->fh);
	return 0;
}

static int comi_fsync(const char *path, int isdatasync,
		     struct fuse_file_info *fi)
{
	(void) path;
	(void) isdatasync;
	(void) fi;
	return 0;
}

#ifdef HAVE_POSIX_FALLOCATE
static int comi_fallocate(const char *path, int mode,
			off_t offset, off_t length, struct fuse_file_info *fi)
{
	int fd;
	int res;

	(void) fi;

	if (mode)
		return -EOPNOTSUPP;
	char fpath[PATH_MAX];
	comi_fullpath(fpath, path);
	fd = open(fpath, O_WRONLY);
	if (fd == -1)
		return -errno;

	res = -posix_fallocate(fd, offset, length);

	close(fd);
	return res;
}
#endif


#ifdef HAVE_SETXATTR
/* xattr operations are optional and can safely be left unimplemented */
static int comi_setxattr(const char *path, const char *name, const char *value,
			size_t size, int flags)
{
	char fpath[PATH_MAX];
	comi_fullpath(fpath, path);
	int res = lsetxattr(fpath, name, value, size, flags);
	if (res == -1)
		return -errno;
	return 0;
}

static int comi_getxattr(const char *path, const char *name, char *value,
			size_t size)
{
	char fpath[PATH_MAX];
	comi_fullpath(fpath, path);
	int res = lgetxattr(fpath, name, value, size);
	if (res == -1)
		return -errno;
	return res;
}

static int comi_listxattr(const char *path, char *list, size_t size)
{
	char fpath[PATH_MAX];
	comi_fullpath(fpath, path);
	int res = llistxattr(fpath, list, size);
	if (res == -1)
		return -errno;
	return res;
}

static int comi_removexattr(const char *path, const char *name)
{
	char fpath[PATH_MAX];
	comi_fullpath(fpath, path);
	int res = lremovexattr(fpath, name);
	if (res == -1)
		return -errno;
	return 0;
}
#endif

static struct fuse_operations comi_oper = {
	.getattr	= comi_getattr,
	.access		= comi_access,
	.readlink	= comi_readlink,
	.readdir	= comi_readdir,
	.mknod		= comi_mknod,
	.mkdir		= comi_mkdir,
	.symlink	= comi_symlink,
	.unlink		= comi_unlink,
	.rmdir		= comi_rmdir,
	.rename		= comi_rename,
	.link		= comi_link,
	.chmod		= comi_chmod,
	.chown		= comi_chown,
	.truncate	= comi_truncate,
#ifdef HAVE_UTIMENSAT
	.utimens	= comi_utimens,
#endif
	.open		= comi_open,
	.read		= comi_read,
	.write		= comi_write,
	.statfs		= comi_statfs,
	.release	= comi_release,
	.fsync		= comi_fsync,
#ifdef HAVE_POSIX_FALLOCATE
	.fallocate	= comi_fallocate,
#endif
#ifdef HAVE_SETXATTR
	.setxattr	= comi_setxattr,
	.getxattr	= comi_getxattr,
	.listxattr	= comi_listxattr,
	.removexattr	= comi_removexattr,
#endif
};


int main(int argc, char *argv[]) {

	// printf("%s\n", argv[argc-1]);
	// printf("%s\n", realpath(argv[argc-1], NULL));

	argv[argc-2] = argv[argc-1];
	argv[argc-1] = NULL;
	argc--;

  fuse_main( argc, argv, &comi_oper, NULL);
}

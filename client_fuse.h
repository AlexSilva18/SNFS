#ifndef CLIENT_FUSE_H
#define CLIENT_FUSE_H

#define FUSE_USE_VERSION 30

#include <fuse.h>
#include <fuse_common.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>

struct dir_data {
   DIR *dp;
   struct dirent *entry;
   off_t offest;
};

static int client_getattr(const char*, struct stat*);
static int client_readdir(const char*, void*, fuse_fill_dir_t, off_t, struct fuse_file_info*);
static int client_read(const char*, char*, size_t, off_t, struct fuse_file_info*);
static int client_create(const char*, mode_t, struct fuse_file_info*);
static int client_open(const char*, struct fuse_file_info*);
static int client_write(const char*, const char*, size_t, off_t, struct fuse_file_info*);
static int client_flush(const char*, struct fuse_file_info*);
static int client_mkdir(const char*, mode_t);
/* static int client_truncate(const char*, off_t); */
/* static int client_opendir(const char*, struct fuse_file_info*); */
static int client_releasedir(const char*, struct fuse_file_info*);

#endif

#include "client_fuse.h"

/* static struct file_info {
  const char *filename;
  const char *data;
  }finfo;*/

static const char *hello_str = "Hello Worlds!\n";
static const char *hello_path = "/tmp/hello";

/** gets attributes from file
 * @param const char* path, path of the file which the system asked for its attributes
 * @param struct stat *st, holds the attributes of the file
 * @return 0 on success, -1 on failure
 */
static int client_getattr( const char *path, struct stat *st){
  printf("[getattr] path == %s\n", path);
  (void) path;
  if (lstat(path, st) == -1)
    return -errno;
  return 0;
  
  /* int res = 0; */
  
  /* st->st_uid = getuid(); // user ID of the process invoking the operation */
  /* st->st_gid = getgid(); // group ID of the process invoking the operation */
  /* st->st_atime = time( NULL ); // The last access of the file/directory is right now */
  /* st->st_mtime = time( NULL ); // The last modification of the file/directory is right now */
  
  /* memset(st, 0, sizeof(struct stat));; */
  
  /* if (strcmp(path, "/") == 0 ){ */
  /*   st->st_mode = S_IFDIR | 0755; // sets file system, file type and permission bits */
  /*   st->st_nlink = 2;  */
  /* } */
  /* //else if (strcmp(path+1, finfo.filename) == 0){ */
  /* else if (strcmp(path, hello_path) == 0){ */
  /*   st->st_mode = S_IFREG | 0444; */
  /*   st->st_nlink = 1; */
  /*   //st->st_size = strlen(finfo.data); */
  /*   st->st_size = strlen(hello_str); */
  /*   return 0; */
  /* } */
  /* else */
  /*   res = -ENOENT; */

  /* return res; */
}

/** shows the files and directories that reside in a specific directory (ls)
 * @param const char*
 * @param void*
 * @param fuse_fill_dir_t
 * @param off_t
 * @param struct fuse_file_info*
 * @return 
 */ 
static int client_readdir( const char *path, void *buffer, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi ){
  printf("[readdir] path == %s\n", path);
  
  (void) offset;
  (void) fi;
  DIR *dir = opendir(path);
  struct dirent *read;
  if (dir == NULL){
    closedir(dir);
    return -errno;
  }
  while ((read = readdir(dir)) != NULL){
    if (strcmp( path, "/" ) == 0 ) {
      //printf("step1");
      if (strcmp(read->d_name, ".") == 0){
	filler(buffer, ".", NULL, 0); // Current Directory
	continue;
      }
      else if (strcmp(read->d_name, "..") == 0){
	filler(buffer, "..", NULL, 0); // Parent Directory
	continue;
      }
    }
    struct stat st;
    memset(&st, 0, sizeof(st));
    st.st_ino = read->d_ino;
    st.st_mode = read->d_type << 12;
    if(filler(buffer, read->d_name, &st, 0))
      break;
  }
  // If the user is trying to show the files/directories of the root directory show the following
  /* if ( strcmp( path, "/" ) == 0 ) { */
    
    /* filler(buffer, ".", NULL, 0); // Current Directory */
    /* filler(buffer, "..", NULL, 0); // Parent Directory */
    /* filler(buffer, hello_path + 1, NULL, 0); */
  /*   /\* filler( buffer, "file1", NULL, 0 ); *\/ */
  /*   /\* filler( buffer, "file2", NULL, 0 ); *\/ */
  /* } */
  /* else */
  /*   return -ENOENT; */
  closedir(dir);
  return 0;
}

/** reads chunk of data from file
 * @param const char*
 * @param char*
 * @param size_t
 * @param off_t
 * @param struct fuse_file_info*
 * @return
 */
static int client_read( const char *path, char *buffer, size_t size, off_t offset, struct fuse_file_info *fi ){
  printf("[read] path == %s\n", path);
  printf("\tdata: %s\n", buffer);
  
  size_t len;
  (void) fi;

  //if (strcmp(path + 1, finfo.filename) != 0 )
  if (strcmp(path + 1, hello_path) != 0 )
    return -ENOENT;

  len = strlen(hello_str);
  if (offset < len){
    if (offset + size > len)
      size = len - offset;
    //memcpy(buffer, finfo.data + offset, size);
    memcpy(buffer, hello_str + offset, size);
  }
  else
    size = 0;
  
  return size;
}

static int client_open(const char *path, struct fuse_file_info *fi){
  printf("[open] path == %s\n", path);
  if (strcmp(path + 1, hello_path) != 0)
    return -ENOENT;
  
  if ((fi->flags & 3) != O_RDONLY)
    return -EACCES;

  return 0;
}


static int client_create(const char *path, mode_t mode, struct fuse_file_info *fi){
  printf("[create] path == %s\n", path);
  
  /* int ret; */
  /* if (S_ISREG(mode)){ */
  /*   ret = open(path, O_CREATE | O_EXCL | O_WRONLY, mode); */
  /*   if (ret >= 0) */
  /*     ret = close(ret); */
  /* } */
  /* else if (S_ISFIFO(mode)) */
  /*   ret = mkfifo(path, mode); */
  /* else */
  /*   ret = mknod(path, mode,  */
  
  if((fi->fh = open(path, fi->flags, mode)) == -1){
    printf("File Create Failed!\n");
    return -ENOSYS;
  }    
  
  return 0;
}

static int client_write(const char *path, const char *buffer, size_t size, off_t offset, struct fuse_file_info *fi){
  printf("[write] path == %s\n", path);
  printf("\tdata: %s\n", buffer);

  int fd;
  (void)fi;
  if(fi == NULL)
    fd = open(path, O_WRONLY);
  else
    fd = fi->fh;
  
  if (fd == -1)
    return -errno;
  
  int bytes_written = pwrite(fd, buffer, size, offset);
  if(bytes_written == -1){
    printf("Write Failed!\n");
    return ENOSYS;
  }
  if (fi == NULL)
    close(fd);

  return bytes_written;
}

static int client_flush(const char *path, struct fuse_file_info *fi){
  printf("[flush] path == %s\n", path);
  return 0;
}
static int client_mkdir(const char *path, mode_t mode){
  printf("[mkdir] path == %s\n", path);
  if(mkdir(path, mode) == -1){
    printf("MKDIR Failed!\n");
    return -ENOENT;
  }
  
  return 0;
}
/* static int client_truncate(const char*, off_t){ */
/*   return 0; */
/* } */
/* static int client_opendir(const char*, struct fuse_file_info*){ */
/*   return 0; */
/* } */
static int client_releasedir(const char *path, struct fuse_file_info *fi){
  struct dir_data *dir = (struct dir_data*) (uintptr_t) fi->fh;
  (void) path;
  closedir(dir->dp);
  return 0;
}


static struct fuse_operations fops = {
  .getattr = client_getattr,
  .readdir = client_readdir,
  .read = client_read,
  .create = client_create,
  .open = client_open,
  .write = client_write,
  .flush = client_flush,
  .mkdir = client_mkdir,
  /* .truncate = client_truncate, */
  /* .opendir = client_opendir, */
  .releasedir = client_releasedir,
};


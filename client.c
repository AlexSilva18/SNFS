#include "client_fuse.c"

int main( int argc, char *argv[] ){
  //int ret;
  //struct fuse_args args = FUSE_ARGS_INIT(argc, argv);
  
  /* finfo.filename = strdup("/tmp/hello"); */
  /* finfo.data = strdup("Heeeyy\n"); */
  
  return fuse_main(argc, argv, &fops, NULL);
}

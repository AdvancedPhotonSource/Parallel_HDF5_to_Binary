#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "framebuffer.h"

void usage(char* prg)
{
  printf("Usage: %s file-to-read", prg);
  exit(1);
}

int main(int argc, char* argv[])
{
  if (argc < 1) {
    fprintf(stderr, "%s\n", "Missing file name.");
    usage(argv[0]);
  }

  int bufsize = 1 * 1024 * 1024;

  if (argc > 2) {
    bufsize = atoi(argv[2]);
  }

  int fd = open(argv[1], O_CREAT | O_RDWR, S_IRWXU);
  if (fd < 0) return 1;

  unsigned char *buffer = new unsigned char[bufsize];
  

}
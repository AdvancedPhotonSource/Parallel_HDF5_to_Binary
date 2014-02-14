#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "bufferpool.h"
#include "queue.h"
#include "reader.h"
#include "writer.h"
#include "converter.h"

void usage(char* prg)
{
  printf("Usage: %s h5file immfile dataset_name buffer_count frames frames_per_buffer\n", prg);
  exit(1);
}

int main(int argc, char* argv[])
{
  if (argc < 7) {
    usage(argv[0]);
  }

  std::string h5file = argv[1];
  std::string immfile = argv[2];
  std::string dataset = argv[3];
  unsigned int buffer_count = atoi(argv[4]);
  unsigned int frames = atoi(argv[5]);
  unsigned int frames_per_buffer = atoi(argv[6]);

  BufferPool *pool = new BufferPool(buffer_count, 1024, 1024, frames_per_buffer);
  Queue<FrameBuffer*> *readconvert = new Queue<FrameBuffer*>();
  Queue<FrameBuffer*> *convertwrite = new Queue<FrameBuffer*>();

  Reader *reader = new Reader(h5file, dataset, 1024, 1024, frames, frames_per_buffer, pool, readconvert);
  Converter *converter = new Converter(readconvert, convertwrite);
  Writer *writer = new Writer(immfile, convertwrite, pool);

  writer->start();
  converter->start();
  reader->start();

  while(1) usleep(10 * 1000);
}
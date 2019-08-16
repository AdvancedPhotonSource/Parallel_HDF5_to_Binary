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

#include <boost/thread.hpp>

void usage(char* prg)
{
  printf("Usage: %s h5filename immfilebase dataset_name nframes sizex sizey is_compmode threshold file_wr_permission maxframes stframe\n", prg);
  printf("Example: %s /home/mydir/series_29_data_000000.h5 /home/mydir/series_29 data 1000 1030 1065 1 0 ab 10000 2000\n", prg);
  printf("The above will append to imm file named series_29_1-10000.imm. it will stick 1000 images at end of the file in compressed mode\n");
  printf("also, the images will be 1030x1065 pixels, and compression threshold is 0. The 1st image added to the file will be number 2000 in imm header.\n");
  printf("data set called data in the file named  series_29_data_000000.h5\n");
  exit(1);
}

int main(int argc, char* argv[])
{

	//usleep(30*1000000);
  
  
  
  if (argc < 12) {
    usage(argv[0]);
  }

  std::string h5file = argv[1];
  std::string immfile = argv[2];
  std::string dataset = argv[3];
  unsigned int frames = atoi(argv[4]);
  unsigned int sizex = atoi(argv[5]);
  unsigned int sizey = atoi(argv[6]);

  unsigned int is_comp = atoi(argv[7]);
  unsigned int threshold = atoi(argv[8]);
  char fileperm[32];
  
  strcpy(fileperm,argv[9]);
  unsigned int maxframes = atoi(argv[10]);
  unsigned int stframe = atoi(argv[11]);
  
  

/*
  std::string h5file = "DectrisFileReader/series_25_data_000000.h5";
  std::string immfile = "test";
  std::string dataset = "data";
  unsigned int buffer_count = 10;
  unsigned int frames = 1;
  unsigned int frames_per_buffer = 1;
*/

  unsigned int buffer_count=20;
  unsigned int frames_per_buffer=1;
  
  
  //raw images, 1kx1k and no header
  BufferPool *poolread = new BufferPool(buffer_count, sizex, sizey, 0);
  // imm data, 1kx1k and 1k header
  BufferPool *poolwrite = new BufferPool(buffer_count, sizex, sizey, 1024);


  Queue<FrameBuffer*> *readconvert = new Queue<FrameBuffer*>();
  Queue<FrameBuffer*> *convertwrite = new Queue<FrameBuffer*>();

  Reader *reader = new Reader(h5file, dataset, sizex, sizey, frames, poolread, readconvert,stframe);
  
  
  Converter *converter = new Converter(readconvert, convertwrite, poolread,poolwrite,frames,is_comp,threshold);
  
  
  Writer *writer = new Writer(immfile, convertwrite, poolwrite, frames,fileperm,maxframes);

  writer->start();
  converter->start();
  reader->start();


  reader->join();
  converter->join();
  writer->join();

  printf("Done\n");
}

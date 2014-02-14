#include "writer.h"
#include "queue.h"
#include "bufferpool.h"
#include "framebuffer.h"

#include <iostream>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h> 
#include <stdio.h>

Writer::Writer(std::string filepath,
               Queue<FrameBuffer *> *srcQueue,
               BufferPool *bufferPool)
: m_filepath(filepath),
  m_srcQueue(srcQueue),
  m_bufferPool(bufferPool)
{
  setup();
}

Writer::~Writer()
{

}

void Writer::setup()
{
  m_fd = open(m_filepath.c_str(), O_CREAT | O_RDWR | O_TRUNC,  S_IRWXU);
  if (m_fd < 0)
  {
    fprintf(stderr, "Failed to create output file - %s ", strerror(errno));
    throw "Error in setup"; 
  }
}

void Writer::run()
{
  if (m_fd < 0) return;

  while (isRunning()) {
    FrameBuffer *buffer = m_srcQueue->pop();

    int n = 0;
    int istart = buffer->getStart() * sizeof(unsigned int);
    int vstart = buffer->getStart() * sizeof(unsigned short);
    int isize = buffer->getSize() * sizeof(unsigned int);
    int vsize = buffer->getSize() * sizeof(unsigned short);

    do {
     n += write(m_fd,
           (char *) buffer->getIndexBuffer() + istart + n, 
           isize - n);   
    } while (n < isize);

    n = 0;
    do {
      n += write(m_fd, 
                 (char *) buffer->getValueBuffer() + vstart + n,
                 vsize - n);
    } while (n < vsize);

    // Final step, return the buffer for reuse. 
    m_bufferPool->put(buffer);

  }
}
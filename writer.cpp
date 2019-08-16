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
               BufferPool *bufferPool,
               unsigned int frames,
	       char *fileperm,
	       unsigned int maxframes)
: m_filepath(filepath),
  m_srcQueue(srcQueue),
  m_bufferPool(bufferPool),
  m_frames(frames),
  m_maxframes(maxframes)

{
  strcpy(m_fileperm,fileperm);

  m_imm = new imm_file(m_fileperm);
  
	setup();

}

Writer::~Writer()
{
  delete(m_imm);
}

void Writer::setup()
{

   m_imm->open_w(
	(char*)m_filepath.c_str(),
	1,
	m_maxframes);

  if (m_imm->is_open==false)
  {
    fprintf(stderr, "Failed to create output file - %s \n", strerror(errno));
    throw "Error in setup"; 
  }
  else
  {
	  fprintf(stderr, "OPened output file -\n");

  }
}

void Writer::run()
{
  if (!m_imm->is_open) return;

  unsigned int frameswrote = 0;
  unsigned int numbytes;

  while (frameswrote < m_frames) {

    FrameBuffer *buffer = m_srcQueue->pop();

//buffer->saveToFile("immbuffer_write");
	buffer->getAttr(&numbytes,0);
	
	m_imm->appendImage(buffer->getBuffer(), numbytes);

    // Final step, return the buffer for reuse. 
    // printf("(Writer) Frmae = %d\n", frameswrote);

    // for (int i = 0 ; i < 10 ; i++)
    // {
    //   printf("%d ", buffer->getValueBuffer()[i]);
    // }

    // printf("\n");

    m_bufferPool->put(buffer);

    frameswrote += 1;
  }

  m_imm->close();
}

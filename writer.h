#ifndef WRITER_H
#define WRITER_H

#include "thread.h"
#include "imm_file.h"

template <typename T>
class Queue;

class BufferPool;
class FrameBuffer;

class Writer : public Thread
{

public:

  Writer(std::string filepath, 
        Queue<FrameBuffer *> *srcQueue,
        BufferPool *bufferPool,
        unsigned int frames,
	char* fileperm,
	unsigned int maxframes);

  ~Writer();

protected:

  void run();

private:

  std::string m_filepath;

  Queue<FrameBuffer *> *m_srcQueue;

  BufferPool *m_bufferPool;

  int m_fd;

  imm_file *m_imm;
  
  char m_fileperm[16];
  
  unsigned int m_maxframes;
  unsigned int m_frames;

  void setup();

};
#endif

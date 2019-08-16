#ifndef CONVERTER_H
#define CONVERTER_H

#include "thread.h"
#include "imm_file.h"
#include "bufferpool.h"

template <typename T>
class Queue;
class FrameBuffer;

class Converter : public Thread
{
public: 
//readconvert, convertwrite, poolread,poolwrite,frames
  Converter(Queue<FrameBuffer *> *srcQueue, 
            Queue<FrameBuffer *> *destQueu,
	    BufferPool *readpool,
	    BufferPool *writepool,
            unsigned int frames,
	    unsigned int is_comp,
	    unsigned int threshold);

  ~Converter();

protected:

  void run();

private:

  Queue<FrameBuffer *> *m_srcQueue;
  Queue<FrameBuffer *> *m_destQueue;
  BufferPool *m_readpool;
  BufferPool *m_writepool;
  unsigned int m_frames;
  
  unsigned int m_threshold;
  unsigned int m_is_comp;
  
  imm_file *my_imm;

};

#endif
#ifndef BUFFER_POOL_H
#define BUFFER_POOL_H

#include <vector>
#include "queue.h"
#include "thread.h"

class FrameBuffer;

class BufferPool
{
public:  
  BufferPool(unsigned int size, 
             unsigned int dimX, 
             unsigned int dimY, 
             unsigned int framesPerBuffer);

  ~BufferPool();

  FrameBuffer* take();

  void put(FrameBuffer* buffer);

private:

  unsigned int m_size;
  unsigned int m_countFree;
  unsigned int m_countInUse;
  unsigned int m_dimX;
  unsigned int m_dimY;
  unsigned int m_framesPerBuffer;

  std::queue<FrameBuffer *> m_bufferQueue;

  boost::mutex m_mutex;
  boost::condition_variable m_condition;

  void alloc();
};

#endif
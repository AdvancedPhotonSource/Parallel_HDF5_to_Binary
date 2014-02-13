#include "bufferpool.h"
#include "frameBuffer.h"

BufferPool::BufferPool(int size)
: m_size(size),
  m_dimX(dimX),
  m_dimY(dimY),
  m_framesPerBuffer(framesPerBuffer)

{
  alloc()
}

BufferPool::~BufferPool()
{

}

void BufferPool::alloc()
{
  
  for (int i = 0 ; i < m_size ; i++)
  {
    FrameBuffer* fbuffer = new FrameBuffer(m_dimX, m_dimY, m_framesPerBuffer);
    freeBuffers.push_back(fbuffer);  
  }
   m_countFree =  m_size;
   m_countInUse = 0;

}
FrameBuffer* BufferPool::take()
{

  boost::unique_lock<boost::mutex> lock(m_mutex);
  while (m_bufferQueue.empty())
  {
    m_condition.wait(lock);
  }

  FrameBuffer *frameBuffer = m_bufferQueue.front();
  m_bufferQueue.pop();

  return frameBuffer;

}

void BufferPool::put(FrameBuffer * buffer)
{
  
  boost::unique_lock<boost::mutex> lock(m_mutex);
  m_bufferQueue.push(buffer);
 
  lock.unlock();
  m_condition.notify_one();

}
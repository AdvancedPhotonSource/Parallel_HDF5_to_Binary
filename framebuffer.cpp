#include "framebuffer.h"

FrameBuffer::FrameBuffer(unsigned int dimX, 
                         unsigned int dimY, 
                         unsigned int framesInBuffer)
 : m_dimX(dimX), 
   m_dimY(dimY), 
   m_framesInBuffer(framesInBuffer)
{
  alloc();
}

FrameBuffer::~FrameBuffer()
{
}

void FrameBuffer::alloc()
{
  m_value = new unsigned short[m_dimX * m_dimY * m_framesInBuffer];
  m_index = new unsigned int[m_dimX * m_dimY * m_framesInBuffer];
  m_start = 0;
  m_size = m_dimX * m_dimY * m_framesInBuffer;
}

unsigned short * FrameBuffer::getValueBuffer()
{
  return m_value;
}

unsigned int * FrameBuffer::getIndexBuffer()
{
  return m_index;
}

unsigned int FrameBuffer::getFramesPerBuffer()
{
  return m_framesInBuffer;
}

unsigned int FrameBuffer::getStart()
{
    return m_start;
}

void FrameBuffer::setStart(unsigned int s)
{
    m_start = s;
}

unsigned int FrameBuffer::getSize()
{
    return m_size;
}

void FrameBuffer::setSize(unsigned int s)
{
    m_size = s;
}
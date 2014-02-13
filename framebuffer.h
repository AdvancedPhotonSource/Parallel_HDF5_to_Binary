#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

class FrameBuffer
{
public:
  FrameBuffer(unsigned int dimX, unsigned int dimY, unsigned int framesInBuffer);

  ~FrameBuffer();

  unsigned short* getValueBuffer();

  unsigned int* getIndexBuffer();

  unsigned int getStart();
  void setStart(unsigned int s);

  unsigned int getSize();
  void setSize(unsigned int s);
  
private:
  unsigned short *m_value;
  unsigned int m_dimX;
  unsigned int m_dimY;
  unsigned int m_framesInBuffer;
  unsigned int *m_index;
  unsigned int m_start;
  unsigned int m_size;

  void alloc();
};

#endif
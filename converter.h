#ifndef CONVERTER_H
#define CONVERTER_H

#include "thread.h"

template <typename T>
class Queue;
class FrameBuffer;

class Converter : public Thread
{
public: 

  Converter(Queue<FrameBuffer *> *srcQueue, 
            Queue<FrameBuffer *> *destQueu,
            unsigned int frames);

  ~Converter();

protected:

  void run();
  void stop();

private:

  Queue<FrameBuffer *> *m_srcQueue;
  Queue<FrameBuffer *> *m_destQueue;

  unsigned int m_frames;

};

#endif

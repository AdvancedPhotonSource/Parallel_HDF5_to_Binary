#include "converter.h"
#include "queue.h"
#include "framebuffer.h"

Converter::Converter(Queue<FrameBuffer *> *srcQueue,
                     Queue<FrameBuffer *> *destQueue,
                     unsigned int frames)
: m_srcQueue(srcQueue),
  m_destQueue(destQueue),
  m_frames(frames)
{

}

Converter::~Converter()
{
  printf("Converter finished\n");
}

void Converter::run()
{
  
  unsigned framesconverted = 0;

  while (framesconverted < m_frames)
  {
    FrameBuffer *buffer = m_srcQueue->pop();

    unsigned short *valueBuffer = buffer->getValueBuffer();
    unsigned int *indexBuffer = buffer->getIndexBuffer();

    unsigned int indexSparse = 0;

    for (unsigned int i = buffer->getStart() ; i < buffer->getSize() ; i++)
    {
      if (valueBuffer[i] != 0) 
      {
        indexBuffer[indexSparse] = i;
        valueBuffer[indexSparse] = valueBuffer[i];
        indexSparse++;
      }
    }

    // printf("(Converter) Frame = %d\n", framesconverted);
    
    // for ( int i = 0 ; i < 10; i++)
    // {
    //   printf("%d ", valueBuffer[i]);
    // }

    // printf("\n");


    buffer->setStart(0);
    buffer->setSize(indexSparse);

    m_destQueue->push(buffer);

    framesconverted += buffer->getFramesPerBuffer();
  }

}
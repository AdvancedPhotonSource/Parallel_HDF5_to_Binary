#include "converter.h"
#include "queue.h"
#include "framebuffer.h"

Converter::Converter(Queue<FrameBuffer *> *srcQueue,
                     Queue<FrameBuffer *> *destQueue)
: m_srcQueue(srcQueue),
  m_destQueue(destQueue)
{

}

Converter::~Converter()
{
  printf("Converter finished\n");
}

void Converter::run()
{
  
  while (isRunning())
  {
    
    m_srcQueue->pop();

    printf("Converter go the buffer\n");

    // unsigned short *valueBuffer = buffer->getValueBuffer();
    // unsigned int *indexBuffer = buffer->getIndexBuffer();

    // unsigned int indexSparse = 0;

    // for (unsigned int i = buffer->getStart() ; i < buffer->getSize() ; i++)
    // {
    //   if (valueBuffer[i] != 0) 
    //   {
    //     indexBuffer[indexSparse] = i;
    //     valueBuffer[indexSparse] = valueBuffer[i];
    //     indexSparse++;
    //   }
    // }

    // buffer->setStart(0);
    // buffer->setSize(indexSparse);

    // m_destQueue->push(buffer);
  }

}
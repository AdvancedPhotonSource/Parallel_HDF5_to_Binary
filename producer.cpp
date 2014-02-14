#include "producer.h"
#include "queue.h"

Producer::Producer(Queue<int> *queue)
: m_queue(queue)
{

}

Producer::~Producer()
{

  printf("Thread dealloc\n");
}

void Producer::run()
{
  for (int i = 0 ; i < 100 * 100 ; i++)
  {
    printf("Producing %d\n", i);
    m_queue->push(i);
  }

  printf("Thread is done!\n");
}
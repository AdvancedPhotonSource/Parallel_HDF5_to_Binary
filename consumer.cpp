#include "consumer.h"
#include "queue.h"

Consumer::Consumer(Queue<int> *queue)
: m_queue(queue)
{


}

Consumer::~Consumer()
{
  printf("Thread consumer done\n");
}

void Consumer::run()
{
  int v = -1;

  for (int i=0; i<10; i++) 
  { 
    v = m_queue->pop();
    printf("consuming \n");
  }
}
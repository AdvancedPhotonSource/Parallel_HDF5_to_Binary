#ifndef PRODUCER_H
#define PRODUCER_H

#include "thread.h"

template <typename T>
class Queue;

class Producer : public Thread
{

public:
  Producer(Queue<int> *m_queue);

  ~Producer();

  void run();

private:
  
  Queue<int>* m_queue;

};

#endif

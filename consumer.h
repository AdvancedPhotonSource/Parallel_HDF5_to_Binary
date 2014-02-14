#ifndef CONSUMER_H
#define CONSUMER_H

#include "thread.h"

template <typename T>
class Queue;

class Consumer : public Thread
{

public:
  Consumer(Queue<int> *queue);

  ~Consumer();

  void run();

private:

  Queue<int> *m_queue;

};

#endif

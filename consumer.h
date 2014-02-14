#ifndef CONSUMER_H
#define CONSUMER_H

template <typename T>
class Queue;

class Consumer
{

public:
  Consumer(Queue<int> *queue);

  ~Consumer();

  void run();

private:

  Queue<int> *m_queue;

};

#endif

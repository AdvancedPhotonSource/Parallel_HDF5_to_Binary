#ifndef QUEUE_H
#define QUEUE_H

#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <queue>

template <class T>
class Queue {
  
public:
  T pop();
  void push(const T& item);

private:

  std::queue<T> m_queue;
  boost::mutex m_mutex;
  boost::condition_variable m_condition;
  
};

template <class T>
T Queue<T>::pop()
{
  boost::unique_lock<boost::mutex> lock(m_mutex);
  while (m_queue.empty())
  {
    m_condition.wait(lock);
  }
  T val = m_queue.front();
  m_queue.pop();

  return val;
}

template <class T>
void Queue<T>::push(const T& item)
{
  boost::unique_lock<boost::mutex> lock(m_mutex);
  m_queue.push(item);
  lock.unlock();

  m_condition.notify_one();
}

#endif
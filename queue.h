#ifndef QUEUE_H
#define QUEUE_H

#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <queue>

template <typename T>
class Queue {
  
public:
  T pop();
  
  void push(const T& item);
  
  int size();

private:

  std::queue<T> m_queue;
  boost::mutex m_mutex;
  boost::condition_variable m_condition;
  
};

template <typename T>
T Queue<T>::pop()
{
  boost::mutex::scoped_lock lock(m_mutex);
  
  while (m_queue.size() == 0)
  {
    printf("Locking\n");
    m_condition.wait(lock);
    printf("Woken %d\n", m_queue.size());  
  }

  printf("Woken 2\n");

  T val = m_queue.front();
  m_queue.pop();

  //printf("Popd %d\n", m_queue.size());

  return val;
}

template <typename T>
void Queue<T>::push(const T& item)
{
  
  boost::mutex::scoped_lock lock(m_mutex);
  m_queue.push(item);
  lock.unlock();
  m_condition.notify_one();

  //printf("Pushed %d\n", m_queue.size());  
}

template <typename T>
int Queue<T>::size()
{
  int size = 0;
  {
    boost::unique_lock<boost::mutex> lock(m_mutex);
    size = m_queue.size();
  }
  
  return size;
}

#endif
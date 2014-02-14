#ifndef THREAD_H
#define THREAD_H

#include <boost/thread.hpp>

class Thread
{
public:

  Thread();
  
  virtual ~Thread();

  void interrupt();

  void start();

  void stop();

  void join();
  
  bool isRunning();
  
protected:

  virtual void run() = 0;

  void interuptionPoint();

private:

  boost::shared_ptr<boost::thread> m_thread;

  bool m_running;
};

#endif
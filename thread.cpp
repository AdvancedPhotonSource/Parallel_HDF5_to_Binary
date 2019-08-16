#include "thread.h"
#include <stdio.h>

Thread::Thread()
: m_running(false)
{
  
}

Thread::~Thread()
{
}

void Thread::interrupt()
{
  m_thread->interrupt();
}

bool Thread::isRunning()
{
  return m_running;
}

void Thread::interuptionPoint()
{
  boost::this_thread::interruption_point();
}

void Thread::start()
{

  m_thread = boost::shared_ptr<boost::thread>(
    new boost::thread(boost::bind(&Thread::run, this)));

  m_running = true;
}

void Thread::stop()
{
  if (m_running == false) return;
  
  m_thread->interrupt();
  if(m_thread->joinable())
  {
    //wait 5 seconds for join
    boost::posix_time::time_duration timeout = boost::posix_time::milliseconds(5000);
      
    // Wait for thread to finish
    m_thread->timed_join(timeout);
  }
   m_running = false; 
}

void Thread::join()
{
  m_thread->join();
}

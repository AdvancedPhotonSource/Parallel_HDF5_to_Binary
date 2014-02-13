#include "thread.h"

Thread::Thread()
: m_running(false)
: m_thread(shared_ptr<thread>(new thread()))
{
  
}

Thread::~Thread()
{
  
}

void Thread::interrupt()
{
  m_thread->interrupt();
}

void Thread::isRunning()
{
  return m_isRunning;
}

void Thread::interuptionPoint()
{
  boost::this_thread::interuption_point();
}

void Thread::start()
{
  m_thread = shared_ptr<boost::thread>(new thread(bind(&Thread::run, this)));
  m_running = true;
}

void Thread::stop()
{
  if (m_running == false) return;
  m_thread->interrupt();
}
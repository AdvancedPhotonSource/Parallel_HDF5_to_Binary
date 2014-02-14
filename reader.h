#ifndef READER_H
#define READER_H

#include <hdf5.h>
#include "thread.h"

template <typename T>
class Queue;

class BufferPool;
class FrameBuffer;

class Reader : public Thread
{
public:

  Reader(std::string filepath,
         std::string datasetname,
         unsigned int dimX, 
         unsigned int dimY,
         unsigned int frames,
         unsigned int framesPerBuffer,
         BufferPool *pool,
         Queue<FrameBuffer *> *destQueue);

  ~Reader();

protected:

  void run();

private:

  std::string m_filepath;
  std::string m_dataset;

  unsigned int m_dimX;
  unsigned int m_dimY;
  unsigned int m_frames;
  unsigned int m_framesPerBuffer;

  BufferPool *m_pool;
  Queue<FrameBuffer *> *m_destQueue;

  hsize_t m_dims[3];
  hsize_t m_chunk[3];
  hsize_t m_count[3];

  hid_t m_datasetID;
  hid_t m_spaceID;
  hid_t m_memspaceID;

  void setup();

};

#endif
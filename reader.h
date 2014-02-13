#ifndef READER_H
#define READER_H

class BufferPool;
class Queue;

class Reader : public Thread
{
public:

  Reader(std::string filepath,
         std::string datasetname,
         unsigned int dimX, 
         unsigned int dimY,
         unsigned int frames,
         unsigend int framesPerBuffer,
         BufferPool *pool
         Queue* destQueue);

  ~Reader();

protected:

  void run();

private:

  BufferPool *m_pool;

  Queue *m_destQueue;

  unsigned int m_dimX;
  unsigned int m_dimY;
  unsigned int m_frames;
  unsigned int m_framesPerBuffer;

  // File descriptor
  int m_fd;

  hsize_t m_dims[3]
  hsize_t m_chunk[3];
  hsize_t m_count[3];

  hid_t m_datasetID;
  hid_t m_spaceID;
  hid_t m_memspaceID;

  std::string m_path;
  std::string m_dataset;

  void setup();

};

#endif
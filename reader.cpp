#include "reader.h"
#include "bufferpool.h"
#include "queue.h"
#include "framebuffer.h"

// #include "DNexusReadHelper.h"


Reader::Reader(std::string filepath,
               std::string dataset,
               unsigned int dimX,
               unsigned int dimY,
               unsigned int frames,
               BufferPool *pool,
               Queue<FrameBuffer *> *destQueue,
	       unsigned int stframe)
: m_filepath(filepath),
  m_dataset(dataset),
  m_dimX(dimX),
  m_dimY(dimY),
  m_frames(frames),
  m_pool(pool),
  m_destQueue(destQueue),
  m_stframe(stframe)
{
  setup();
}

Reader::~Reader()
{

}

void Reader::setup()
{
  
  m_dims[0] = m_frames;
  m_dims[1] = m_dimY;
  m_dims[2] = m_dimX;

  // Chunking size
  m_chunk[0] = 1;
  m_chunk[1] = m_dims[1];
  m_chunk[2] = m_dims[2];

  // Size of hyperslab. 
  m_count[0] = 1;
  m_count[1] = m_dims[1];
  m_count[2] = m_dims[2];

  hid_t fileID = H5Fopen(m_filepath.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
  hid_t gid = H5Gopen2(fileID, "/xpcs", H5P_DEFAULT);
  m_datasetID = H5Dopen1(gid, m_dataset.c_str());
  m_spaceID = H5Dget_space(m_datasetID);
  m_memspaceID = H5Screate_simple(3, m_count, NULL);

}

void Reader::run()
{


  unsigned int coreco_ts;
  double elapsed_ts;
  unsigned int systick_ts;
  unsigned int comp_threshold;
  int bytes_per_pix;
  int filenumber;
  unsigned int camtype;
  double acq_period;
  int num_comp_pixels;
  unsigned int IMM_bytes;
  unsigned int sizex, sizey;
  

  hsize_t offset[3];
  offset[0] = offset[1] = offset[2] = 0;

  hid_t HDF5_datatype;
  HDF5_datatype = H5T_NATIVE_UINT16;

  for (offset[0] = 0 ; offset[0] < m_frames; offset[0] += m_count[0])
  {
    // Might block if there is no buffer in the pool. 
    FrameBuffer *buffer = m_pool->take();
    unsigned short *valuebuffer = buffer->getBufferImg();
    
    herr_t errstatus = H5Sselect_hyperslab(m_spaceID, H5S_SELECT_SET, offset, NULL, m_count, NULL);
    hid_t status = H5Dread(m_datasetID, HDF5_datatype, m_memspaceID, m_spaceID, H5P_DEFAULT, valuebuffer);

    // printf("(Reader) Frame = %d\n", offset[0]);
    // for ( int i = 0 ; i < 10; i++)
    // {
    //   printf("%d ", valuebuffer[i]);
    // }

    // printf("\n");
    
   
    buffer->setDims(m_dims[2], m_dims[1]);
    
    //buffer->saveToFile("readbuffer");
    
    coreco_ts=m_stframe;
    elapsed_ts=0.0;
    systick_ts=0;
    comp_threshold=0;
    camtype=0;
    acq_period=0.0;
    
    buffer->setAttr(coreco_ts,0);
	buffer->setAttr(elapsed_ts,1);
	buffer->setAttr(systick_ts,2);
	
	buffer->setAttr(comp_threshold,3);

	buffer->setAttr(camtype,4);
	buffer->setAttr(acq_period,5);
	buffer->setAttr(m_stframe,6);
	
	
    m_stframe++;
    
    m_destQueue->push(buffer);
  }
}

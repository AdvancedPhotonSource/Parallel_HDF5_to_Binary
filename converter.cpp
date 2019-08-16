#include "converter.h"
#include "queue.h"
#include "framebuffer.h"

Converter::Converter(Queue<FrameBuffer *> *srcQueue,
                     Queue<FrameBuffer *> *destQueue,
		     BufferPool *readpool,
	    	     BufferPool *writepool,
                     unsigned int frames,
		     unsigned int is_comp,
		     unsigned int threshold)
: m_srcQueue(srcQueue),
  m_destQueue(destQueue),
  m_readpool(readpool),
  m_writepool(writepool),
  m_frames(frames),
  m_is_comp(is_comp),
  m_threshold(threshold)
{
	my_imm = new imm_file("wb");
	
}

Converter::~Converter()
{
  printf("Converter finished\n");
  
  delete(my_imm);
  
}

void Converter::run()
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
   int IMM_bytes;
  unsigned int sizex, sizey;
  
  unsigned char *imm_data;
  
  unsigned int framenum;
  
  bytes_per_pix=2;
  
  unsigned framesconverted = 0;

  while (framesconverted < m_frames)
  {
    FrameBuffer *readbuffer = m_srcQueue->pop();
    FrameBuffer *immbuffer = m_writepool->take();

	// attributes to put into imm header. 
	readbuffer->getAttr(&coreco_ts,0);
	readbuffer->getAttr(&elapsed_ts,1);
	readbuffer->getAttr(&systick_ts,2);
	
	//readbuffer->getAttr(&comp_threshold,3);

	readbuffer->getAttr(&camtype,4);
	readbuffer->getAttr(&acq_period,5);
	readbuffer->getAttr(&framenum,6);
	
	readbuffer->getDims(&sizex, &sizey);
	
	
	
	
	//replace all -1's or 65535's in image with 0's
	readbuffer->substVal(0,65535);
	
	imm_data = (unsigned char*)(immbuffer->getBuffer());
	IMM_bytes= immbuffer->getMemSizeBytes();
	
	
	//framenum is 1 based. set buff num to 0 based.
	my_imm->setBufferNumber(framenum-1);
		filenumber = framenum-1;

	
        if (m_is_comp==1)
	{
	    my_imm->rawToCompIMM(
		    coreco_ts,
		    elapsed_ts,
		    systick_ts,
		    m_threshold,
		    sizex,
		    sizey,
		    bytes_per_pix,
		    (void*)(readbuffer->getBufferImg()),
		    filenumber,
		    camtype,
		    acq_period,
		    &num_comp_pixels,
		    imm_data,
		    &IMM_bytes
		    );
		
   //readbuffer->saveToFile("readbuff_conv");
       }
       else
       {

	my_imm->rawToIMM(
		coreco_ts,
		elapsed_ts,
		systick_ts,
		m_threshold,
		sizex,
		sizey,
		bytes_per_pix,
		(void*)(readbuffer->getBufferImg()),
		framenum,
		camtype,
		acq_period,
		imm_data,
		&IMM_bytes
		);

   //immbuffer->saveToFile("immbuffer_conv");
     }
     
	immbuffer->setAttr((unsigned int)IMM_bytes,0);

    m_destQueue->push(immbuffer);
    m_readpool->put(readbuffer);
    framesconverted += 1;
  }

}

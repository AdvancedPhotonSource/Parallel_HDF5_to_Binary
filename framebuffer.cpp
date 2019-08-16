#include "framebuffer.h"
#include "string.h"
#include <stdio.h>
#include <stdlib.h>


FrameBuffer::FrameBuffer(unsigned int dimX, 
                         unsigned int dimY,
			 unsigned int headersize)
 : m_dimX(dimX), 
   m_dimY(dimY), 
   m_headersize(headersize)
{
  alloc();
}



// save buffer mem to a file. This is for debugging only...
void FrameBuffer::saveToFile(char *fnamebase)
{

  char fname[256];
  //to make unique filenames... add rand number at end.
  sprintf(fname,"%s_%04d.bin",fnamebase,rand()%10000);
	
  FILE *fp = fopen(fname,"wb");
  fwrite ( m_value, 1,m_sizebytes , fp );
   fclose(fp);
  
}
  
  
  



FrameBuffer::~FrameBuffer()
{
  delete(m_value);
}

void FrameBuffer::alloc()
{
// make memory buffer- x*y size in shorts, and add headersize in bytes. /2 is because short is 2 bytes
// we also add on size for compressed IMM, if we try to compress a non compressible file. 2*m_dimX + 2*m_dimY
// is an int for each pixel, which is pixel locations. imm is header if 1k, pix values in shorts, and pix
//locations in ints. we make room for max size of compressed imm where the compressed is actually bigger
// than the raw image. 
  m_value = new unsigned short[m_dimX * m_dimY  + m_headersize/2 + 2*m_dimX *m_dimY];
 
  m_sizebytes=m_dimX * m_dimY*sizeof(short) + m_headersize + m_dimX *m_dimY*sizeof(int);
  m_size = m_dimX * m_dimY;
}

//get size of buffer in bytes
unsigned int FrameBuffer::getMemSizeBytes()
{
	return(m_sizebytes);
}

unsigned short * FrameBuffer::getBuffer()
{
  return m_value;
}

// ret the start of image data area, after any header space
unsigned short * FrameBuffer::getBufferImg()
{
  return(m_value+m_headersize/2);
}



void FrameBuffer::substVal(unsigned short newval, unsigned short oldval)
{

  for (int k = 0; k<getSize();k++)
    if (m_value[k]==oldval)
      m_value[k]=newval;
}



unsigned int FrameBuffer::getSize()
{
    return m_size;
}


 void FrameBuffer::setDims(
  	unsigned int x,
	unsigned int y)
{
	m_dimX=x;
	m_dimY=y;
	m_size = m_dimX * m_dimY;
	
}
	
	



void FrameBuffer::getDims(
     unsigned int *x,
     unsigned int *y)
{
	*x=m_dimX;
	*y=m_dimY;
}

void FrameBuffer::setAttr(
	double val,
	int index)
{
	if (index<max_attributes)
		d_attr[index]=val;
		
}

void FrameBuffer::setAttr(
	unsigned int val,
	int index)
{
	if (index<max_attributes)
		i_attr[index]=val;
		
}


void FrameBuffer::setAttr(
	char *val,
	int index)
{
	if (index<max_attributes)
		strcpy(&str_attrib[index][0],val);
		
}

void FrameBuffer::getAttr(double *val,int index)
{
	if (index<max_attributes)
		*val=d_attr[index];
		

}

void FrameBuffer::getAttr(unsigned int *val,int index)
{
	if (index<max_attributes)
		*val=i_attr[index];
		

}




void FrameBuffer::getAttr(char *val,int index)  
{
	if (index<max_attributes)
		strcpy(val,&str_attrib[index][0]);
		
}

  

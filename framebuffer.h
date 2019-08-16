#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

class FrameBuffer
{
public:
  FrameBuffer(unsigned int dimX, unsigned int dimY, unsigned int headersize);

  ~FrameBuffer();

   //ret start of buffer mem
  unsigned short* getBuffer();
  //ret start of buffer mem, plus size of header. start of img memory
  unsigned short * getBufferImg();


  // get dims x*y in num of pixels
  unsigned int getSize();
  
  //get size of buffer in bytes
  unsigned int getMemSizeBytes();
  
  void getDims(
  	unsigned int *x,
	unsigned int *y);
  
  void setDims(
  	unsigned int x,
	unsigned int y);


  void setAttr(double val,int index);
  void setAttr(unsigned int val,int index);
  void setAttr(char *val,int index);
  
  void getAttr(double *val,int index);
  void getAttr(unsigned int *val,int index);
  void getAttr(char *val,int index);
  
  // save buffer mem to a file.
  void saveToFile(char *fnamebase);
  
  //subst a supplied for the vals found in image.example, change all -1's or 65535's in the image to a 0.
  void substVal(unsigned short newval, unsigned short oldval);
  
  
private:

   enum {max_attributes = 16};
   enum {attr_strlen = 64};
   
  double d_attr[max_attributes];
  unsigned int i_attr[max_attributes];
  char str_attrib[max_attributes][attr_strlen];  
  
  
  unsigned short *m_value;
  unsigned int m_dimX;
  unsigned int m_dimY;
  unsigned int m_sizebytes;
  unsigned int m_size;
  unsigned int m_headersize;

  void alloc();
};

#endif
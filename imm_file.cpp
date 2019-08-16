
#include "imm_file.h"
#include "time.h"
#include "math.h"
#include "stdio.h"
#include "string.h"


/************************************************************************************************
 *
 *
 *
 *************************************************************************************************/
 

imm_file::imm_file()
{
	imm_file("wb");
	

}

/************************************************************************************************
 *
 *
 *
 *************************************************************************************************/
 

imm_file::imm_file(char *fileperm)
{

	strcpy(m_fileperm, fileperm);
	
	last_nbytes =0;
	last_data=0;
	is_open=0;
	myfile=0;
	scratch_memory=new unsigned char[scratch_mem_bytes];
	scratch_memory2=new unsigned char[scratch_mem_bytes];

	buf_num=0;

}

/************************************************************************************************
 *
 *
 *
 *************************************************************************************************/
 
imm_file::~imm_file()
{
	delete scratch_memory;
	delete scratch_memory2;
}


/************************************************************************************************
 *
 *
 *
 *************************************************************************************************/
 
void imm_file::open_w(
		char* name,
		int start_num,
		int end_number)
{

	char fullname[512];

	if (start_num!= -1)
		sprintf(fullname,"%s_%05i-%05i.imm",name,start_num,end_number);
	else
		sprintf(fullname,"%s",name);


	myfile = fopen ( fullname, m_fileperm );

	is_open=0;

	if (myfile!=NULL)
		is_open=1;
		
	
	//is_open=true;

}

/************************************************************************************************
 *
 *
 *
 *************************************************************************************************/
 
void imm_file::open_w(
		char* name,
		char* path,
		int start_num,
		int end_number)
{

	char fullname[512];

	if (start_num!= -1)
		sprintf(fullname,"%s_%05i-%05i.imm",name,start_num,end_number);
	else
		sprintf(fullname,"%s",name);

	myfile = fopen ( fullname, m_fileperm);

	is_open=0;

	if (myfile!=NULL)
		is_open=1;
		

}


/************************************************************************************************
 *
 *
 *
 *************************************************************************************************/
 

void imm_file::open_r(
		char* name,
		int start_num,
		int end_number)
{

	char fullname[512];

	sprintf(fullname,"%s_%05i-%05i.imm",name,start_num,end_number);

	myfile = fopen ( fullname, "rb" );

	is_open=0;

	if (myfile!=NULL)
		is_open=1;
		

}


/************************************************************************************************
 *
 *
 *
 *************************************************************************************************/
 
void imm_file::open_r(
		char* name,
		char* path,
		int start_num,
		int end_number)
{

	char fullname[512];

	sprintf(fullname,"%s_%05i-%05i.imm",name,start_num,end_number);

	myfile = fopen ( fullname, "rb" );

	is_open=0;

	if (myfile!=NULL)
		is_open=1;
		

}


/************************************************************************************************
 *
 *
 *
 *************************************************************************************************/
 
void imm_file::close(void)
{
	if (myfile!=0)
	{
		fclose(myfile);
		is_open=0;
		myfile=0;
	}
}


 
/************************************************************************************************
 * Given we already have an open imm file, we pass pointer to image data, and pointer to imm header.
 *  data is written to disk.
 *
 *************************************************************************************************/
 
void imm_file::appendImage(
		imm_header *head,
		void *img_ptr)
{
	int num_bytes;
	
	 fwrite ( head, imm_header::header_size, 1, myfile );
	

	num_bytes = head->rows * head->bytes * head->cols;

	 fwrite ( img_ptr, 1, num_bytes, myfile );


}



/************************************************************************************************
 * Given we already have an open imm file, we pass pointer to image data. assume header is start if imgdata.
 *  data is written to disk.
 *
 *************************************************************************************************/
 
void imm_file::appendImage(
		void *img_ptr,
		unsigned int num_bytes)
{
	


	 fwrite ( img_ptr, 1, num_bytes, myfile );


}


/************************************************************************************************
 *
 * Create a fresh IMM header.
 *
 *************************************************************************************************/
 
imm_header* imm_file::newHeader(void)
{
	char *mem;

	mem= new char[imm_header::header_size];
	fillHeaderZZFF((imm_header*)mem);

	return((imm_header*)mem );

}

/************************************************************************************************
 *
 * For putting ZZZ's in the IMM header. 
 *
 *************************************************************************************************/
 

	// fill in 00's amd ZZ's intop header
void imm_file::fillHeaderZZFF(imm_header *h)
{
	int k;

	for(k=0;k<imm_header::z_len;k++)
	{
		h->ZZZZ[k] = 0;
	}

	for(k=0;k<imm_header::f_len;k++)
	{
		h->FFFF[k] = 255;
	}

}


/************************************************************************************************
 *
 * Read 1st header in the IMM file. 
 *
 *************************************************************************************************/
 
void imm_file::readTopHeader(imm_header* head)
{
	rewind(myfile);
	
	fread ( head, imm_header::header_size, 1, myfile );
}


/************************************************************************************************
 *
 * Read in image from IMM file. This will calc. the file location by image size, and skip to 
 * image we want, then read. This will not work for sparse IMM files. Sorry sonny. For that you
 * myst rewind file, then read each header one by one until you come to image. 
 *************************************************************************************************/
 
void imm_file::readImage(
		int index,
		imm_header *head,
		void *img_data)
{
	int num_bytes, skip_bytes;

	readTopHeader(head);

	//index from 0 to ...
	skip_bytes = index * (imm_header::header_size +
			head->rows * head->bytes * head->cols);

	num_bytes = head->rows * head->bytes * head->cols;
	
	fseek ( myfile, skip_bytes, SEEK_SET );
	
	fread ( head, imm_header::header_size, 1, myfile );
	
	fread ( img_data, 1, num_bytes, myfile );



}

/******************************************************************************
 * convert raw image into IMM. give raw image and num. butes. 
 * give num bytes per pixel, called raw_precision, give x and y image size.
 * give data for the imm header, like threshold, timestamp, and index. give memory
 * buffer and its size where to put imm imgage. imm data is retined and its size.]
 * this is NOT sparce but raw imm images.
 *****************************************************************************/


void imm_file::rawToIMM(
	unsigned char* raw_image,
	int raw_bytes,// tottal bytes
	int raw_precision,//num bytes per pixel
	int raw_x_pixels,
	int raw_y_pixels,
	int threshold,
	int raw_timestamp,
	int img_index,
	unsigned char* IMM_image,
	int *IMM_bytes)
{

	int k;
	int num_pixels;
	int pix_rec_bytes;
	int num_raw_pixels;
	unsigned short* raw_image_us;
	unsigned char* raw_image_uc;

    uint32_t *IMM_data_int32_t;
	imm_header *IMM_header;
	unsigned short* IMM_data_short;
	unsigned char *IMMptr;



	// get memory for new compressed image
	//*IMM_image = this->scratch_memory;

	// point header to top of new memory
	IMM_header=(imm_header*)IMM_image;

	IMMptr = IMM_image;
	for (k=0;k<imm_header::header_size;k++)
		IMMptr[k] = 0;


	fillHeaderZZFF(IMM_header);

	//
	// Put some magic numbers in the header
	//
	IMM_header->cols=raw_x_pixels;
	IMM_header->rows=raw_y_pixels;

			// bytes per pixel
	IMM_header->bytes = raw_precision;
		// magic numbers
		IMM_header->immversion=imm_header::immver;
		IMM_header->mode = 2;
		IMM_header->compression=0;


		IMM_header->col_beg=0;
		IMM_header->col_end=raw_x_pixels-1;
		IMM_header->corecotick=raw_timestamp;

	 IMM_data_short = (unsigned short*)(IMM_image+imm_header::header_size);


	//
	// Fill the pixel location section
	//


	raw_image_us = (unsigned short*)raw_image;
	raw_image_uc = raw_image;
    //raw_image_ul = (uint32_t*)raw_image;

	num_raw_pixels = raw_bytes/raw_precision;
	num_pixels=num_raw_pixels;

	memcpy((void*)IMM_data_short,(void*)raw_image_us,raw_bytes);




	IMM_header->dlen = num_pixels;
	// calculate the size of the new compressde image in bytes
	*IMM_bytes = imm_header::header_size;
	*IMM_bytes = *IMM_bytes + raw_bytes;

}



/******************************************************************************
 *
 *Gen sparse imm images from raw image. give threshold for throwing away data. 
 * all pixels below threshold are discaredd. IMM_bytes should have max mem size in bytes when called
 * will return IMM bytes w/ num bytes in teh imm file.
 *****************************************************************************/


void imm_file::rawToCompIMM(
	unsigned char* raw_image,
	int raw_bytes,// tottal bytes
	int raw_precision,//num bytes per pixel
	int raw_x_pixels,
	int raw_y_pixels,
	int threshold,
	int raw_timestamp,
	int img_index,
	unsigned char* IMM_image,
	int *IMM_bytes)
{

	int k;
	int num_pixels;
	int pix_rec_bytes;
	int num_raw_pixels;
	unsigned short* raw_image_us;
	unsigned char* raw_image_uc;

	unsigned short* pix_val_s;
	unsigned int *IMM_data_int;
	imm_header *IMM_header;
	unsigned short* IMM_data_short;
	unsigned char *IMMptr;
	int location;



	
	
	// get memory for new compressed image
	//*IMM_image = this->scratch_memory;

	// point header to top of new memory
	IMM_header=(imm_header*)IMM_image;

	IMMptr = IMM_image;
	for (k=0;k<imm_header::header_size;k++)
		IMMptr[k] = 0;

	fillHeaderZZFF(IMM_header);


	//
	// Put some magic numbers in the header
	//
	IMM_header->cols=raw_x_pixels;
	IMM_header->rows=raw_y_pixels;

			// bytes per pixel
	IMM_header->bytes = raw_precision;
		// magic numbers
		IMM_header->immversion=imm_header::immver;
		IMM_header->mode = 2;
		IMM_header->compression=6;


		IMM_header->col_beg=0;
		IMM_header->col_end=raw_x_pixels-1;
		IMM_header->corecotick=raw_timestamp;

	// IMM_data_short = (unsigned short*)(*IMM_image+imm_header::header_size);
	 IMM_data_int = (unsigned int*)(IMM_image+imm_header::header_size);


	//
	// Fill the pixel location section
	//


	raw_image_us = (unsigned short*)raw_image;
	raw_image_uc = raw_image;
    //raw_image_ul = (uint32_t*)raw_image;

	num_raw_pixels = raw_bytes/raw_precision;

	num_pixels=0;
	location = 0;

	pix_val_s=(unsigned short*)this->scratch_memory2;


	if (raw_precision==2)
	{
		for (k=0;k<num_raw_pixels; k++)
		{
			if (*raw_image_us > threshold)
			{
				*IMM_data_int = location;
				*pix_val_s=*raw_image_us;
				IMM_data_int++;
				pix_val_s++;
				num_pixels++;
			}
			raw_image_us++;
			location ++;

		}
	}
	else//for char data
	{
		for (k=0;k<num_raw_pixels; k++)
		{
			if (*raw_image_uc > threshold)
			{
				*IMM_data_int = location;
				*pix_val_s=(unsigned short)*raw_image_uc;
				IMM_data_int++;
				pix_val_s++;
				num_pixels++;
			}
			raw_image_uc++;

			location ++;
		}
	}



	
	memcpy((void*)IMM_data_int,(void*)this->scratch_memory2,num_pixels*sizeof(short));




	IMM_header->dlen = num_pixels;


	// calculate the size of the new compressde image in bytes
	*IMM_bytes = imm_header::header_size;
    *IMM_bytes = *IMM_bytes + sizeof(uint32_t)*num_pixels;
	*IMM_bytes = *IMM_bytes + sizeof(unsigned short)*num_pixels;


}







/******************************************************************************
 *
 *
 *****************************************************************************/

void imm_file::saveFileIMMRaw(
	unsigned int time_stamp,
	int comp_threshold,
	int sizex,
	int sizey,
	int bytes_per_pix,
	void *imgdata,
	int filenumber,
	int camtype,
	double acq_period
	)
{
	imm_header *header;

	unsigned char* IMM_image =  this->scratch_memory;
	int IMM_bytes;

	 time_t rawtime;
  struct tm * timeinfo;
  char today[32];

  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
   sprintf(today,"%s",asctime(timeinfo));



	rawToIMM(
		(unsigned char*)imgdata,
		bytes_per_pix*sizex*sizey,
		2,
		sizex,
		sizey,
		comp_threshold,
		0,//timestamp
		0,//imgindex
		IMM_image,
		&IMM_bytes);

	header=(imm_header*)IMM_image;
		header->threshhold=comp_threshold;
		header->elapsed=time_stamp;
		header->corecotick=time_stamp;
		header->systick=time_stamp;
		header->col_beg=0;
		header->col_beg=0;
		header->col_end=sizex;
		header->row_beg=0;
		header->row_end=sizey;
		header->number=filenumber;
		strcpy((char*)header->date,today);

		header->rows=sizey;
		header->cols=sizex;
		header->cameratype=camtype;// dalsa
		header->preset=acq_period;
		header->row_bin=1;
		header->col_bin=1;
		header->roi_number=1;

		header->buffer_number = buf_num;





		fwrite ( IMM_image, 1, IMM_bytes, myfile );


	last_nbytes=IMM_bytes;
	last_data = IMM_image;


}








/******************************************************************************
 *
 *
 *****************************************************************************/

void imm_file::rawToIMM(
	unsigned int coreco_ts,
	double elapsed_ts,
	unsigned int systick_ts,
	int comp_threshold,
	int sizex,
	int sizey,
	int bytes_per_pix,
	void *imgdata,
	int filenumber,
	int camtype,
	double acq_period,
	unsigned char* IMM_image,
	int *IMM_bytes
	)
{
	imm_header *header;


	 time_t rawtime;
  struct tm * timeinfo;
  char today[32];

  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
   sprintf(today,"%s",asctime(timeinfo));



	rawToIMM(
		(unsigned char*)imgdata,
		bytes_per_pix*sizex*sizey,
		2,
		sizex,
		sizey,
		comp_threshold,
		0,//timestamp
		0,//imgindex
		IMM_image,
		IMM_bytes);

	header=(imm_header*)(IMM_image);
		header->threshhold=comp_threshold;
		header->elapsed=elapsed_ts;
		header->corecotick=coreco_ts;
		header->systick=systick_ts;
		header->col_beg=0;
		header->col_beg=0;
		header->col_end=sizex;
		header->row_beg=0;
		header->row_end=sizey;
		header->number=filenumber;
		strcpy((char*)header->date,today);

		header->rows=sizey;
		header->cols=sizex;
		header->cameratype=camtype;// dalsa
		header->preset=acq_period;
		header->row_bin=1;
		header->col_bin=1;
		header->roi_number=1;

		header->buffer_number = buf_num;



		//fwrite ( IMM_image, 1, IMM_bytes, myfile );


		last_nbytes=*IMM_bytes;
	last_data = IMM_image;




}










/******************************************************************************
 *
 *
 *****************************************************************************/

void imm_file::saveFileIMMRaw(
	unsigned int coreco_ts,
	double elapsed_ts,
	unsigned int systick_ts,
	int comp_threshold,
	int sizex,
	int sizey,
	int bytes_per_pix,
	void *imgdata,
	int filenumber,
	int camtype,
	double acq_period
	)
{
	imm_header *header;

	unsigned char* IMM_image  =  this->scratch_memory;
	int IMM_bytes;

	 time_t rawtime;
  struct tm * timeinfo;
  char today[32];

  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
   sprintf(today,"%s",asctime(timeinfo));



	rawToIMM(
		(unsigned char*)imgdata,
		bytes_per_pix*sizex*sizey,
		2,
		sizex,
		sizey,
		comp_threshold,
		0,//timestamp
		0,//imgindex
		IMM_image,
		&IMM_bytes);

	header=(imm_header*)IMM_image;
		header->threshhold=comp_threshold;
		header->elapsed=elapsed_ts;
		header->corecotick=coreco_ts;
		header->systick=systick_ts;
		header->col_beg=0;
		header->col_beg=0;
		header->col_end=sizex;
		header->row_beg=0;
		header->row_end=sizey;
		header->number=filenumber;
		strcpy((char*)header->date,today);

		header->rows=sizey;
		header->cols=sizex;
		header->cameratype=camtype;// dalsa
		header->preset=acq_period;
		header->row_bin=1;
		header->col_bin=1;
		header->roi_number=1;

		header->buffer_number = buf_num;



		fwrite ( IMM_image, 1, IMM_bytes, myfile );


		last_nbytes=IMM_bytes;
	last_data = IMM_image;




}



/******************************************************************************
 *
 *
 *****************************************************************************/

void imm_file::saveFileIMMComp(
	unsigned int time_stamp,
	int comp_threshold,
	int sizex,
	int sizey,
	int bytes_per_pix,
	void *imgdata,
	int filenumber,
	int camtype,
	double acq_period,
	int *num_comp_pixels
	)
{
	imm_header *header;

	unsigned char* IMM_image =  this->scratch_memory;
	int IMM_bytes;

	 time_t rawtime;
  struct tm * timeinfo;
  char today[32];

  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
   sprintf(today,"%s",asctime(timeinfo));



	rawToCompIMM(
		(unsigned char*)imgdata,
		bytes_per_pix*sizex*sizey,
		2,
		sizex,
		sizey,
		comp_threshold,
		0,//timestamp
		0,//imgindex
		IMM_image,
		&IMM_bytes);

	header=(imm_header*)IMM_image;
		header->threshhold=comp_threshold;
		header->elapsed=time_stamp;
		header->corecotick=time_stamp;
		header->systick=time_stamp;

		header->col_beg=0;
		header->col_beg=0;
		header->col_end=sizex;
		header->row_beg=0;
		header->row_end=sizey;
		header->number=filenumber;
		strcpy((char*)header->date,today);

		header->rows=sizey;
		header->cols=sizex;
		header->cameratype=camtype;// dalsa
		header->preset=acq_period;
		header->row_bin=1;
		header->col_bin=1;
		header->roi_number=1;

		header->buffer_number = buf_num;

		*num_comp_pixels=header->dlen;



		fwrite ( IMM_image, 1, IMM_bytes, myfile );

		last_nbytes=IMM_bytes;
	last_data = IMM_image;




}



/******************************************************************************
 *
 *
 *****************************************************************************/

void imm_file::rawToCompIMM(
	unsigned int coreco_ts,
	double elapsed_ts,
	unsigned int systick_ts,
	int comp_threshold,
	int sizex,
	int sizey,
	int bytes_per_pix,
	void *imgdata,
	int filenumber,
	int camtype,
	double acq_period,
	int *num_comp_pixels,
	unsigned char* IMM_image,
	int *IMM_bytes
	
	)
{
	imm_header *header;

	
	

	 time_t rawtime;
  struct tm * timeinfo;
  char today[32];

  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
   sprintf(today,"%s",asctime(timeinfo));



	rawToCompIMM(
		(unsigned char*)imgdata,
		bytes_per_pix*sizex*sizey,
		2,
		sizex,
		sizey,
		comp_threshold,
		0,//timestamp
		0,//imgindex
		IMM_image,
		IMM_bytes);

	header=(imm_header*)(IMM_image);
		header->threshhold=comp_threshold;
		header->elapsed=elapsed_ts;
		header->corecotick=coreco_ts;
		header->systick=systick_ts;

		header->col_beg=0;
		header->col_beg=0;
		header->col_end=sizex;
		header->row_beg=0;
		header->row_end=sizey;
		header->number=filenumber;
		strcpy((char*)header->date,today);

		header->rows=sizey;
		header->cols=sizex;
		header->cameratype=camtype;// dalsa
		header->preset=acq_period;
		header->row_bin=1;
		header->col_bin=1;
		header->roi_number=1;

		header->buffer_number = buf_num;

		*num_comp_pixels=header->dlen;



		//fwrite ( IMM_image, 1, IMM_bytes, myfile );


		last_nbytes=*IMM_bytes;
			last_data = IMM_image;



}





/******************************************************************************
 *
 *
 *****************************************************************************/

void imm_file::saveFileIMMComp(
	unsigned int coreco_ts,
	double elapsed_ts,
	unsigned int systick_ts,
	int comp_threshold,
	int sizex,
	int sizey,
	int bytes_per_pix,
	void *imgdata,
	int filenumber,
	int camtype,
	double acq_period,
	int *num_comp_pixels
	)
{
	imm_header *header;

	unsigned char* IMM_image =  this->scratch_memory;
	int IMM_bytes;

	 time_t rawtime;
  struct tm * timeinfo;
  char today[32];

  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
   sprintf(today,"%s",asctime(timeinfo));



	rawToCompIMM(
		(unsigned char*)imgdata,
		bytes_per_pix*sizex*sizey,
		2,
		sizex,
		sizey,
		comp_threshold,
		0,//timestamp
		0,//imgindex
		IMM_image,
		&IMM_bytes);

	header=(imm_header*)IMM_image;
		header->threshhold=comp_threshold;
		header->elapsed=elapsed_ts;
		header->corecotick=coreco_ts;
		header->systick=systick_ts;

		header->col_beg=0;
		header->col_beg=0;
		header->col_end=sizex;
		header->row_beg=0;
		header->row_end=sizey;
		header->number=filenumber;
		strcpy((char*)header->date,today);

		header->rows=sizey;
		header->cols=sizex;
		header->cameratype=camtype;// dalsa
		header->preset=acq_period;
		header->row_bin=1;
		header->col_bin=1;
		header->roi_number=1;

		header->buffer_number = buf_num;

		*num_comp_pixels=header->dlen;



		fwrite ( IMM_image, 1, IMM_bytes, myfile );


		last_nbytes=IMM_bytes;
			last_data = IMM_image;



}



/************************************************************************************************
 *
 *
 *
 *************************************************************************************************/
 
void imm_file::setBufferNumber(int b)
{
	buf_num = b;
}




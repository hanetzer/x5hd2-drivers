/******************************************************************************

  Copyright (C), 2001-2020, Hisilicon. Co., Ltd.

******************************************************************************
File Name	    : jdatasrc.h
Version		    : Initial Draft
Author		    : 
Created		    : 2012/05/07
Description	    : read stream
Function List 	: 

			  		  
History       	:
Date				Author        		Modification
2011/11/23		            		    Created file      	
******************************************************************************/

#ifndef __JDATASRC_H__
#define __JDATASRC_H__


/*********************************add include here******************************/


#include "jinclude.h"


/*****************************************************************************/


#ifdef __cplusplus
      #if __cplusplus
   
extern "C" 
{

      #endif
#endif /* __cplusplus */

    /***************************** Macro Definition ******************************/

    #define INPUT_BUF_SIZE  4096	/* choose an efficiently fread'able size */

	#define STREAM_DO_NOT_NEED_MEMCPY /** 不分配2K码流buffer，不cpy码流 **/
	
    /*************************** Structure Definition ****************************/

	typedef struct
    {
    
		  char* img_buffer;         /** the user's input stream buffer **/
		  int buffer_size;          /** the stream buffer size **/
		  int pos;                  /** the stream read pos **/
          JOCTET EndMarkBuf[2];     /** the data that the end of jpeg file **/
		  
	}BUFF_JPG;


	/* Expanded data source object for stdio input */

	typedef struct 
    {
    
		  struct jpeg_source_mgr pub;	/* public fields */
		  union
		  {
		    BUFF_JPG stBufStream;		/* jpeg image buffer */
		    FILE * infile;		        /* source stream */
		  };

		  JOCTET * buffer;		        /* start of buffer */
		  boolean start_of_file;	    /* have we gotten any data yet? */
	  
	} my_source_mgr;

	typedef my_source_mgr * my_src_ptr;

    /***************************  The enum of Jpeg image format  ******************/


    /********************** Global Variable declaration **************************/

   

    /******************************* API declaration *****************************/


    #ifdef __cplusplus
    
        #if __cplusplus


      
}
      
        #endif
        
   #endif /* __cplusplus */

#endif /* __JDATASRC_H__*/

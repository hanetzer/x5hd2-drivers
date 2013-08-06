/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon. Co., Ltd.

******************************************************************************
File Name	    : hijpeg_type.h
Version		    : Initial Draft
Author		    : y00181162
Created		    : 2011/05/12
Description	    : this is used by user and kernel, and it only include hi_type.h
Function List 	: 
			    : 

History       	:
Date				Author        		Modification
2011/05/11		    y00181162		    Created file      	
******************************************************************************/

#ifndef __JMEMMGR_H__
#define __JMEMMGR_H__



/*********************************add include here******************************/

/*****************************************************************************/


#ifdef __cplusplus
#if __cplusplus
   extern "C" 
{
#endif
#endif /* __cplusplus */



    /***************************** Macro Definition ******************************/

   #ifndef ALIGN_TYPE		/* so can override from jconfig.h */
   #define ALIGN_TYPE  double
   #endif
    /*************************** Structure Definition ****************************/

   typedef union large_pool_struct FAR * large_pool_ptr;

    typedef union large_pool_struct {
      struct {
        large_pool_ptr next;	/* next in list of pools */
        size_t bytes_used;		/* how many bytes already used within pool */
        size_t bytes_left;		/* bytes still available in this pool */

        size_t PhysAddr;
            
      } hdr;
      ALIGN_TYPE dummy;		/* included in union to ensure alignment */
    } large_pool_hdr;


    /********************** Global Variable declaration **************************/



    /******************************* API declaration *****************************/



    /****************************************************************************/

#ifdef __cplusplus
#if __cplusplus   
}
      
#endif
#endif /* __cplusplus */

#endif /* __JMEMMGR_H__ */

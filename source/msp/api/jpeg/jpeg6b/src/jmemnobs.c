/*
 * jmemnobs.c
 *
 * Copyright (C) 1992-1996, Thomas G. Lane.
 * This file is part of the Independent JPEG Group's software.
 * For conditions of distribution and use, see the accompanying README file.
 *
 * This file provides a really simple implementation of the system-
 * dependent portion of the JPEG memory manager.  This implementation
 * assumes that no backing-store files are needed: all required space
 * can be obtained from malloc().
 * This is very portable in the sense that it'll compile on almost anything,
 * but you'd better have lots of main memory (or virtual memory) if you want
 * to process big images.
 * Note that the max_memory_to_use option is ignored by this implementation.
 */

#define JPEG_INTERNALS
#include "jinclude.h"
#include "jpeglib.h"
#include "jmemsys.h"		/* import the system-dependent declarations */

#include "hijpeg_decode_hw.h"
#include "jmemmgr.h"
#include "hi_common.h"


#include <sys/mman.h>
#include <sys/ioctl.h>
#include <semaphore.h>
#include <memory.h>
#include <assert.h>
#include <string.h>

#include <sys/mman.h>

#include <sys/stat.h>
#include <fcntl.h>


#include <unistd.h>

#ifndef HAVE_STDLIB_H		/* <stdlib.h> should declare malloc(),free() */
extern void * malloc JPP((size_t size));
extern void free JPP((void *ptr));
#endif


#define MEM_ALIGN2MUL(x,a)             ( ((x) + ((a) - 1)) & (~((a) - 1)) )

/*
 * Memory allocation and freeing are controlled by the regular library
 * routines malloc() and free().
 */

GLOBAL(void *)
jpeg_get_small (j_common_ptr cinfo, size_t sizeofobject)
{ 

    return (void *) malloc(sizeofobject);
     
}

GLOBAL(void)
jpeg_free_small (j_common_ptr cinfo, void * object, size_t sizeofobject)
{

    free(object);
}


/*
 * "Large" objects are treated the same as "small" ones.
 * NB: although we include FAR keywords in the routine declarations,
 * this file won't actually work in 80x86 small/medium model; at least,
 * you probably won't be able to process useful-size images in only 64KB.
 */

GLOBAL(void FAR *)
jpeg_get_large (j_common_ptr cinfo, size_t sizeofobject)
{


	    #if 0
	    	return (void FAR *) malloc(sizeofobject);
		#else
		    size_t tSize;
		    void FAR *  pVirtAddr;
		    size_t PhysAddr = 0;
			
		    large_pool_ptr hdr_ptr;
		    
		    tSize = MEM_ALIGN2MUL(sizeofobject, 64);

		    JPEG_MESSAGE_S  *pstMessagePrivate;
		    pstMessagePrivate = (JPEG_MESSAGE_S *)(cinfo->client_data);
		    if(FALSE == JPEG_Api_CheckAddStructMessage(cinfo,pstMessagePrivate))
		    {
		        return NULL;
		    }

			#if 1
			/**
			 ** TC AND XBMC平台的MMZ支持多进程，自己封装必须改实现，所有使用公共模块的
			 **/
	        PhysAddr = (size_t)HI_MMZ_New((HI_U32)tSize, 64, (HI_CHAR*)"jpeg", "JPEG");
			if(PhysAddr == 0)
			{
			    PhysAddr = (size_t)HI_MMZ_New((HI_U32)tSize, 64, (HI_CHAR*)"graphics", "JPEG");
				if(PhysAddr == 0)
				{
                   PhysAddr = (size_t)HI_MMZ_New((HI_U32)tSize, 64, NULL, "JPEG");
				}
			}
	        #else
		    PhysAddr= (size_t)JPEG_MMZ_NewPhyAddr(pstMessagePrivate->mmz, tSize, 64, (HI_U8*)"jpeg", "JPEG");
		    if(PhysAddr == 0){
		        PhysAddr= (size_t)JPEG_MMZ_NewPhyAddr(pstMessagePrivate->mmz,tSize, 64, (HI_U8*)"graphics", "JPEG");
		        if(PhysAddr == 0){
		             PhysAddr= (size_t)JPEG_MMZ_NewPhyAddr(pstMessagePrivate->mmz,tSize, 64, NULL, "JPEG");
		        }

		    }
			#endif
		    if(0 == PhysAddr){  
		          #ifdef JPEG6B_DEBUG
		          HI_JPEG_TRACE("failed to mmz buffer");
		          #endif
		          pstMessagePrivate->ErrMsg = TRUE;
		          return NULL;
		    }

		   	 #if 1
	         pVirtAddr = (void FAR *)HI_MMZ_Map((HI_U32)PhysAddr,TRUE);
	         #else   
		     pVirtAddr = (void FAR *)JPEG_MMB_MapToVirAddr(pstMessagePrivate->mmz,(void*)PhysAddr,TRUE);
             #endif
			 if (pVirtAddr == NULL){
		            #ifdef JPEG6B_DEBUG
		            HI_JPEG_TRACE("failed map Phycial address to virtual address!");
		            #endif
		            pstMessagePrivate->ErrMsg = TRUE;
		            return NULL;
		     }
			 
		     
		     hdr_ptr = (large_pool_ptr)pVirtAddr;
		     hdr_ptr->hdr.PhysAddr = PhysAddr;

		     return pVirtAddr;
		 
		 #endif
	
    
}

GLOBAL(void)
jpeg_free_large (j_common_ptr cinfo, void FAR * object, size_t sizeofobject)
{

	    #if 0
	        free(object);
		#else
		    JPEG_MESSAGE_S  *pstMessagePrivate;
		    pstMessagePrivate = (JPEG_MESSAGE_S *)(cinfo->client_data);

		    size_t PhysAddr = 0;
		    large_pool_ptr hdr_ptr;
		    hdr_ptr = (large_pool_ptr)object;
		    PhysAddr = hdr_ptr->hdr.PhysAddr;

			#if 1
			if (PhysAddr!=0)
			{
			    HI_MMZ_Unmap((HI_U32)PhysAddr);
			    HI_MMZ_Delete((HI_U32)PhysAddr);
			}
		    #else	
		    if (PhysAddr!=0){
				JPEG_MMB_UnmapToPhyAddr(pstMessagePrivate->mmz,(void*)PhysAddr);
				JPEG_MMB_RealseMMZDev(pstMessagePrivate->mmz, (void*)PhysAddr);
		    }
			#endif
		
		#endif
	    
}


/*
 * This routine computes the total memory space available for allocation.
 * Here we always say, "we got all you want bud!"
 */

GLOBAL(long)
jpeg_mem_available (j_common_ptr cinfo, long min_bytes_needed,
		    long max_bytes_needed, long already_allocated)
{
    JPEG_MESSAGE_S  *pstMessagePrivate;
    pstMessagePrivate = (JPEG_MESSAGE_S *)(cinfo->client_data);
    if(FALSE == JPEG_Api_CheckAddStructMessage(cinfo,pstMessagePrivate))
    {
        return -1;
    }
    return max_bytes_needed;
}


/*
 * Backing store (temporary file) management.
 * Since jpeg_mem_available always promised the moon,
 * this should never be called and we can just error out.
 */

GLOBAL(void)
jpeg_open_backing_store (j_common_ptr cinfo, backing_store_ptr info,
			 long total_bytes_needed)
{
    JPEG_MESSAGE_S  *pstMessagePrivate;
    pstMessagePrivate = (JPEG_MESSAGE_S *)(cinfo->client_data);
    if(FALSE == JPEG_Api_CheckAddStructMessage(cinfo,pstMessagePrivate))
    {
        return;
    }
    ERREXIT(cinfo, JERR_NO_BACKING_STORE);
}


/*
 * These routines take care of any system-dependent initialization and
 * cleanup required.  Here, there isn't any.
 */

GLOBAL(long)
jpeg_mem_init (j_common_ptr cinfo)
{

  return 0;			/* just set max_memory_to_use to 0 */
}

GLOBAL(void)
jpeg_mem_term (j_common_ptr cinfo)
{
      /* no work */
      JPEG_MESSAGE_S  *pstMessagePrivate;
      pstMessagePrivate = (JPEG_MESSAGE_S *)(cinfo->client_data);

      if (cinfo->is_decompressor)
      {
          /** revise at 2011-11-4
           ** only decompress use the hard device, at this release device
           ** this fuction should realse at jpeg_finish_decompress and destroy_decompress.
           ** because it maybe call create decompressor is one time, and finish
           ** will maybe many times but not call destory decompressor fuction,so
           ** the device is not closed but has opened, so the signal has not realsed
           ** this function can call many times, because at this function has check the device
           **/
          JPEG_Decode_ReleaseDev(cinfo);
		  
      }
      if (pstMessagePrivate->mmz>0){
            close(pstMessagePrivate->mmz);
            pstMessagePrivate->mmz = 0;    
      }
      JPEG_Api_DinitMessageStruct(cinfo);
      
}

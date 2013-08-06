/*
 * Copyright (C) 2007-2008 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define JPEG_INTERNALS
#include "jinclude.h"
#include "jpeglib.h"
#include "jmemsys.h"		/* import the system-dependent declarations */

#include "hijpeg_decode_hw.h"
#include "jmemmgr.h"
#ifndef HAVE_STDLIB_H		/* <stdlib.h> should declare malloc(),free() */
extern void * malloc JPP((size_t size));
extern void free JPP((void *ptr));
#endif

#ifndef SEEK_SET		/* pre-ANSI systems may not define this; */
#define SEEK_SET  0		/* if not, assume 0 is correct */
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


		#ifdef ANDROID_JPEG6B
		/**
		 ** revise at 2011-11-4
		 ** because if you use mmz malloc the mem, the capability of decode
		 ** will reduce
		 **/
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

        PhysAddr= (size_t)JPEG_MMZ_NewPhyAddr(pstMessagePrivate->mmz, tSize, 64, (HI_U8*)"jpeg", "JPEG");
        if(PhysAddr == 0){
            PhysAddr= (size_t)JPEG_MMZ_NewPhyAddr(pstMessagePrivate->mmz,tSize, 64, (HI_U8*)"graphics", "JPEG");
            if(PhysAddr == 0){
                 PhysAddr= (size_t)JPEG_MMZ_NewPhyAddr(pstMessagePrivate->mmz,tSize, 64, NULL, "JPEG");
            }

        }
        if(0 == PhysAddr){  
              #ifdef JPEG6B_DEBUG
              HI_JPEG_TRACE("=============failed to mmz buffer==============\n");
              HI_JPEG_TRACE("=============failed to mmz buffer  %p, %p===============\n", pstMessagePrivate, cinfo->client_data);
              #endif
              pstMessagePrivate->ErrMsg = TRUE;
              return NULL;
        }
       
         pVirtAddr = (void FAR *)JPEG_MMB_MapToVirAddr(pstMessagePrivate->mmz,(void*)PhysAddr,TRUE);
         if (pVirtAddr == NULL){
                #ifdef JPEG6B_DEBUG
                HI_JPEG_TRACE("=========failed map Phycial address to virtual address!=======\n");
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

        #ifdef ANDROID_JPEG6B

        free(object);
		
		#else
        JPEG_MESSAGE_S  *pstMessagePrivate;
        pstMessagePrivate = (JPEG_MESSAGE_S *)(cinfo->client_data);

        size_t PhysAddr = 0;
        large_pool_ptr hdr_ptr;
        hdr_ptr = (large_pool_ptr)object;
        PhysAddr = hdr_ptr->hdr.PhysAddr;
                
        if (PhysAddr!=0){
    		JPEG_MMB_UnmapToPhyAddr(pstMessagePrivate->mmz,(void*)PhysAddr);
    		JPEG_MMB_RealseMMZDev(pstMessagePrivate->mmz, (void*)PhysAddr);
        }
		
        #endif
		
}


/*
 * This routine computes the total memory space available for allocation.
 * It's impossible to do this in a portable way; our current solution is
 * to make the user tell us (with a default value set at compile time).
 * If you can actually get the available space, it's a good idea to subtract
 * a slop factor of 5% or so.
 */

#ifndef DEFAULT_MAX_MEM		/* so can override from makefile */
#define DEFAULT_MAX_MEM		10000000L /* default: ten megabyte */
#endif

GLOBAL(long)
jpeg_mem_available (j_common_ptr cinfo, long min_bytes_needed,
		    long max_bytes_needed, long already_allocated)
{

        /**
         ** if you use the file to save the data that the mem is
         ** insufficiency, the speed will down
         **/
        #ifdef ANDROID_JPEG6B
	    JPEG_MESSAGE_S  *pstMessagePrivate;
	    pstMessagePrivate = (JPEG_MESSAGE_S *)(cinfo->client_data);
	    if(FALSE == JPEG_Api_CheckAddStructMessage(cinfo,pstMessagePrivate))
	    {
	        return -1;
	    }
	    return max_bytes_needed;
		#else
	    return cinfo->mem->max_memory_to_use - already_allocated;
		#endif
    
}


/*
 * Backing store (temporary file) management.
 * Backing store objects are only used when the value returned by
 * jpeg_mem_available is less than the total space needed.  You can dispense
 * with these routines if you have plenty of virtual memory; see jmemnobs.c.
 */


METHODDEF(void)
read_backing_store (j_common_ptr cinfo, backing_store_ptr info,
		    void FAR * buffer_address,
		    long file_offset, long byte_count)
{
  if (fseek(info->temp_file, file_offset, SEEK_SET))
    ERREXIT(cinfo, JERR_TFILE_SEEK);
  if (JFREAD(info->temp_file, buffer_address, byte_count)
      != (size_t) byte_count)
    ERREXIT(cinfo, JERR_TFILE_READ);
}


METHODDEF(void)
write_backing_store (j_common_ptr cinfo, backing_store_ptr info,
		     void FAR * buffer_address,
		     long file_offset, long byte_count)
{
  if (fseek(info->temp_file, file_offset, SEEK_SET))
    ERREXIT(cinfo, JERR_TFILE_SEEK);
  if (JFWRITE(info->temp_file, buffer_address, byte_count)
      != (size_t) byte_count)
    ERREXIT(cinfo, JERR_TFILE_WRITE);
}


METHODDEF(void)
close_backing_store (j_common_ptr cinfo, backing_store_ptr info)
{
  fclose(info->temp_file);
  /* Since this implementation uses tmpfile() to create the file,
   * no explicit file deletion is needed.
   */
}

static FILE* getTempFileFromPath(const char * path) {
    FILE * fd = fopen(path, "w+");
    unlink(path);
    return fd;
}

static FILE* getTempFile() {
    char path[1024];
    snprintf(path, 1023, "/sdcard/.%d.tmp", getpid());
    FILE * fd = getTempFileFromPath(path);
    if (fd == NULL) {
        // anywhere else we can create a temp file?
		//	    snprintf(path, 1023, "/data/data/.%d.tmp", getpid());
		//      fd = getTempFileFromPath(path);
    }
    return fd;
}

/*
 * Initial opening of a backing-store object.
 *
 * This version uses tmpfile(), which constructs a suitable file name
 * behind the scenes.  We don't have to use info->temp_name[] at all;
 * indeed, we can't even find out the actual name of the temp file.
 */

GLOBAL(void)
jpeg_open_backing_store (j_common_ptr cinfo, backing_store_ptr info,
			 long total_bytes_needed)
{
  if ((info->temp_file = getTempFile()) == NULL)
    ERREXITS(cinfo, JERR_TFILE_CREATE, "");
  info->read_backing_store = read_backing_store;
  info->write_backing_store = write_backing_store;
  info->close_backing_store = close_backing_store;
}


/*
 * These routines take care of any system-dependent initialization and
 * cleanup required.
 */

GLOBAL(long)
jpeg_mem_init (j_common_ptr cinfo)
{

        /**
         ** if you use the file to save the data that the mem is
         ** insufficiency, the speed will down
         **/
        #ifdef ANDROID_JPEG6B
        return 0;	
		#else
        return DEFAULT_MAX_MEM;	/* default for max_memory_to_use */
		#endif
		
}

GLOBAL(void)
jpeg_mem_term (j_common_ptr cinfo)
{


      JPEG_MESSAGE_S  *pstMessagePrivate;
      pstMessagePrivate = (JPEG_MESSAGE_S *)(cinfo->client_data);
      /* no work */
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
      /**
       ** decompress and compress also use the alloc memmory
       **/
      if (pstMessagePrivate->mmz>0){
         close(pstMessagePrivate->mmz);
         pstMessagePrivate->mmz = 0;    
      }
  
      JPEG_Api_DinitMessageStruct(cinfo);


}

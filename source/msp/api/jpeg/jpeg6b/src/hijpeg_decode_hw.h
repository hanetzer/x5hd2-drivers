/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon. Co., Ltd.

******************************************************************************
File Name	    : hijpeg_decode_hw.h
Version		    : Initial Draft
Author		    : 
Created		    : 2011/05/12
Description	    : JPEG6B application interface
Function List 	: HI_JPEG_SetMemMode
			    : HI_JPEG_GetMemMode
			    : HI_JPEG_SetInflexion
			    : HI_JPEG_GetInflexion

			  		  
History       	:
Date				Author        		Modification
2011/05/11		            		    Created file      	
******************************************************************************/

#ifndef __HI_JPEG_DECODE_HW_H__
#define __HI_JPEG_DECODE_HW_H__





/*********************************add include here******************************/



#include "jpeglib.h"
#include <semaphore.h>
 
/*****************************************************************************/


#ifdef __cplusplus
      #if __cplusplus
   
extern "C" 
{

      #endif
#endif /* __cplusplus */

    /***************************** Macro Definition ******************************/

  
    #define MMZ_DEV                "/dev/mmz_userdev"
    #define JPG_DEV                "/dev/jpeg"

    #define FALSE                  0
    #define TRUE                   1

    #define SUCCESS                (0)
    #define FAILURE                (~0)


    /**************** define register parameter ****************************/
    #define X5_JPG_REG_LENGTH      0x6C0        /** the length of register            **/
    #define X5_JPGREG_START        0x0          /** JPEG register that decoding start **/
    #define X5_JPGREG_RESUME       0x4          /** JPEG continue stream register     **/
    #define X5_JPGREG_RESET        0x8          /** JPEG soft reset register          **/
    #define X5_JPGREG_STRIDE       0xC          /** VHB stride register               **/
    #define X5_JPGREG_PICSIZE      0x10         /** picture width register            **/
    #define X5_JPGREG_PICTYPE      0x14         /** picture type register             **/
    #define X5_JPGREG_STADDR       0x20         /** stream buffer start register      **/
    #define X5_JPGREG_ENDADDR      0x24         /** stream buffer end register        **/
    #define X5_JPGREG_STADD        0x28         /** stream saved start register       **/
    #define X5_JPGREG_ENDADD       0x2C         /** stream save end register          **/
    #define X5_JPGREG_YSTADDR      0x30         /** luminance address register        **/
    #define X5_JPGREG_UVSTADDR     0x34         /** chrominance address register      **/
    #define X5_JPGREG_SCALE        0x40         /** scale register                    **/
    #define X5_JPGREG_INT          0x100        /** halt status register              **/
    #define X5_JPGREG_INTMASK      0x104        /** halt shield register              **/
    #define X5_JPGREG_DEBUG        0x108        /** debug register                    **/
    #define X5_JPGREG_QUANT        0x200        /** dqt set register                  **/
    #define X5_JPGREG_HDCTABLE     0x300        /** Huffman  set register             **/
    #define X5_JPGREG_HACMINTABLE  0x340        /** Huffman AC mincode memory register**/
    #define X5_JPGREG_HACBASETABLE 0x360        /** Huffman AC base memory register   **/
    #define X5_JPGREG_HACSYMTABLE  0x400        /** Huffman AC symbol memory register **/


    /*allocate phy buffer*/
    #define IOC_MMB_ALLOC		           _IOWR('m', 10,  mmb_info)
    
    #define IOC_MMB_ATTR		           _IOR ('m', 11,  mmb_info)
    #define IOC_MMB_FREE		           _IOW ('m', 12,  mmb_info)
    #define IOC_MMB_ALLOC_V2	           _IOWR('m', 13,  mmb_info)
    #define IOC_MMB_USER_REMAP	           _IOWR('m', 20,  mmb_info)
    #define IOC_MMB_USER_REMAP_CACHED      _IOWR('m', 21,  mmb_info)
    #define IOC_MMB_USER_UNMAP	           _IOWR('m', 22,  mmb_info)
    #define IOC_MMB_ADD_REF		           _IO  ('r', 30)	/* ioctl(file, cmd, arg), arg is mmb_addr */
    #define IOC_MMB_DEC_REF		           _IO  ('r', 31)	/* ioctl(file, cmd, arg), arg is mmb_addr */
    #define IOC_MMB_FLUSH_DCACHE	       _IO  ('c', 40)
    #define IOC_MMB_TEST_CACHE	           _IOW ('t', 11,  mmb_info)
  


    /*************************** Structure Definition ****************************/

    typedef struct
    {
    	unsigned long phys_addr;   /** phys-memory address **/
    	unsigned long align;	   /** if you need your phys-memory have special align size **/
    	unsigned long size;		   /** length of memory you need, in bytes **/
    	unsigned int order;
    	void *mapped;			   /** userspace mapped ptr **/

    	union
    	{
    	
        		struct
        		{
        			unsigned long prot  :8;	 /** PROT_READ or PROT_WRITE **/
        			unsigned long flags :12; /** MAP_SHARED or MAP_PRIVATE **/
        		};
        		unsigned long w32_stuf;
        		
    	};
    	
    	char mmb_name[16];
    	char mmz_name[32];
    	unsigned long gfp;		   /* reserved, do set to 0 */
    	
    } mmb_info;


   
    /********************** Global Variable declaration **************************/



    /******************************* API declaration *****************************/
    /*****************************************************************************
    * func                     : JPEG_MMZ_NewPhyAddr
    * param[in] :mmz           :
    * param[in] :phyAddr       : 
    * retval    :
    * others:	:nothing
    *****************************************************************************/
    HI_VOID *JPEG_MMZ_NewPhyAddr(int mmz, int size, int align, char *mmz_name, char *mmb_name);

   /*****************************************************************************
    * func                     : JPEG_MMB_MapToVirAddr
    * param[in] :mmz           :
    * param[in] :phyAddr       : 
    * retval    :
    * others:	:nothing
    *****************************************************************************/
    HI_VOID *JPEG_MMB_MapToVirAddr(int mmz, void *phyAddr, int cached);


    /*****************************************************************************
    * func                     : JPEG_MMB_UnmapToPhyAddr
    * param[in] :mmz           :
    * param[in] :phyAddr       : 
    * retval    :
    * others:	:nothing
    *****************************************************************************/
    HI_S32 JPEG_MMB_UnmapToPhyAddr(int mmz, void *phyAddr);

   /*****************************************************************************
    * func                     : JPEG_MMB_RealseMMZDev
    * param[in] :mmz           :
    * param[in] :phyAddr       : 
    * retval    :
    * others:	:nothing
    *****************************************************************************/
    HI_S32 JPEG_MMB_RealseMMZDev(int mmz, void *phyAddr);


    /*****************************************************************************
    * func                     : before call hard decode, we should call this ensure
                                 to cache synchrotron
    * param[in] :mmz
    * param[in] :
    * retval    :
    * others:	:nothing
    *****************************************************************************/
    HI_S32 JPEG_MMB_Flush(int mmz);
	/*****************************************************************************
    * func                     : JPEG_Decode_IsHWSupport
    * description              : check whether hard support 
    * param[in] :cinfo         :
    * retval    :TRUE          : success
    * retval    :FALSE         : failure
    * others:	:nothing
    *****************************************************************************/
    HI_BOOL JPEG_Decode_IsHWSupport(j_decompress_ptr cinfo);
    
    /*****************************************************************************
    * func                     : JPEG_Decode_OpenDev
    * param[in] :cinfo         :
    * retval    :TRUE          : success
    * retval    :FALSE         : failure
    * others:	:nothing
    *****************************************************************************/
    HI_BOOL JPEG_Decode_OpenDev(j_common_ptr cinfo);

    /*****************************************************************************
    * func                     : JPEG_Decode_ReleaseDev
    * param[in] :cinfo         :
    * retval    :TRUE          : success
    * retval    :FALSE         : failure
    * others:	:nothing
    *****************************************************************************/
    HI_BOOL JPEG_Decode_ReleaseDev(j_common_ptr cinfo);
    
    /*****************************************************************************
    * func                     : JPEG_Decode_SetParameter
    * param[in] :cinfo         :
    * retval    :TRUE          : success
    * retval    :FALSE         : failure
    * others:	:nothing
    *****************************************************************************/
    HI_BOOL JPEG_Decode_SetParameter(j_decompress_ptr cinfo);

    /*****************************************************************************
    * func                     : JPEG_Decode_SendStream
    * param[in] :cinfo         :
    * retval    :TRUE          : success
    * retval    :FALSE         : failure
    * others:	:nothing
    *****************************************************************************/

    HI_BOOL JPEG_Decode_SendStream(j_decompress_ptr cinfo);

    /*****************************************************************************
    * func                     : JPEG_Decode_TDEColorConvert
    * param[in] :cinfo         :
    * retval    :TRUE          : success
    * retval    :FALSE         : failure
    * others:	:nothing
    *****************************************************************************/

    HI_S32 JPEG_Decode_TDEColorConvert(j_decompress_ptr cinfo, JSAMPARRAY scanlines, JDIMENSION max_lines);

    /*****************************************************************************
    * func                     : JPEG_Decode_OutputToVirBuf
    * param[in] :cinfo         :
    * retval    :TRUE          : success
    * retval    :FALSE         : failure
    * others:	:nothing
    *****************************************************************************/
    HI_BOOL JPEG_Decode_OutputToVirBuf(j_decompress_ptr cinfo, JSAMPARRAY scanlines,HI_S32 s32OutputWidth, \
                                    HI_S32 s32OutputComponent,HI_S32 s32OutputLines);
	
    /****************************************************************************/
    #ifdef __cplusplus
    
        #if __cplusplus


      
}
      
        #endif
        
   #endif /* __cplusplus */

#endif /* __HI_JPEG_DECODE_HW_H__*/

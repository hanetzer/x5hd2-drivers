/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon. Co., Ltd.

******************************************************************************
File Name	    : hi_jpeg_api.c
Version		    : Initial Draft
Author		    : y00181162
Created		    : 2011/05/12
Description	    : JPEG6B application interface
Function List 	: HI_JPEG_SetMemMode
			    : HI_JPEG_GetMemMode
			    : HI_JPEG_SetInflexion
			    : HI_JPEG_GetInflexion

			  		  
History       	:
Date				Author        		Modification
2011/05/11		    y00181162		    Created file      	
******************************************************************************/

/*********************************add include here******************************/
#include "jinclude.h"
#include "jerror.h"
#include "jpegint.h"
#include "jdatasrc.h"

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

#include "hijpeg_decode_hw.h"
#include "hijpeg_api.h"

#include "hi_tde_api.h"
#include "hi_tde_errcode.h"
#include "hi_tde_type.h"
#include "hi_jpg_ioctl.h"
#include "jpg_driver.h"

#include "hi_common.h"

/***************************** Macro Definition ******************************/

/**
 ** this size from the make menuconfig
 **/
 
#ifndef  CFG_HI_JPEG6B_STREAMBUFFER_SIZE
/**
 **1M is the best well of performance
 **/
#define MMZ_STREAM_BUFFER (1024*1024)     /** the stream buffer size，固定要分配的码流大小 **/
#define JPG_BUF_BLOCK_SIZE (1024*1024)    /** 实际给硬件使用的码流大小 **/
#else
#define MMZ_STREAM_BUFFER   CFG_HI_JPEG6B_STREAMBUFFER_SIZE     /** the stream buffer size **/
#define JPG_BUF_BLOCK_SIZE  CFG_HI_JPEG6B_STREAMBUFFER_SIZE
#endif
#define JPEG_SURFACE_ALIGN 128            /** register 128 bytes align **/
#define JPEG_MCU_ALIGN8  8
#define JPEG_MCU_ALIGN16 16

#define JPG_RESUME_VALUE 0x01
#define JPG_EOF_VALUE   0x02

#define CAI_JPEG_SWAP(a,b) do{ a=a+b; b=a-b; a=a-b; } while(0)


/******************** to see which include file we want to use***************/


/*************************** Structure Definition ****************************/


/********************** Global Variable declaration **************************/
static HI_U32 s_u32JpegInflexion   = (50*50);     /** the image inflexion **/


/******************************* API forward declarations *******************/
static long  HIJPEG_Round_Up (long a, long b);
static HI_BOOL  JPEG_Calc_Output_Dimensions (j_decompress_ptr cinfo,HI_U32 *pu32OutputWidth,HI_U32 *pu32OutputHeight);
static HI_BOOL  JPEG_Calc_Output_Dimensions (j_decompress_ptr cinfo,HI_U32 *pu32OutputWidth,HI_U32 *pu32OutputHeight);
static HI_BOOL JPEG_Dec_GetMemSize(j_decompress_ptr cinfo);
static HI_BOOL  JPEG_Alloc_Memmory (j_decompress_ptr cinfo);
static HI_S32 JPEG_Get_ScaleRatio(j_decompress_ptr cinfo);
static HI_S32 JPEG_GetIntStatus(j_decompress_ptr cinfo, JPG_INTTYPE_E *pIntType, HI_U32 u32TimeOut);
static void JPEG_Write_register(volatile char * reg_virAddr, int phyOff, int value);
static int JPEG_Read_register(volatile char *reg_virAddr, int phyOff);
static void JPEG_CopyDataToRegister(volatile char *reg_virAddr, void *memory, int phyOff, size_t bytes);
//static void JPEG_CopyDataFromRegister(volatile char *reg_virAddr, int phyOff, void *memory, size_t bytes);
static int JPEG_GetDqtTable(j_decompress_ptr cinfo);
/******************************* API realization *****************************/



#if 0
HI_VOID *JPEG_MMZ_NewPhyAddr(int mmz, int size, int align, char *mmz_name, char *mmb_name)
{

      mmb_info   mmi;
      
      memset(&mmi,0,sizeof(mmi));
      
      mmi.size = size;
      mmi.align =align;
      
      if (mmz_name != NULL)
      {
        strncpy(mmi.mmz_name, mmz_name, sizeof(mmi.mmz_name));
      }
      
      if (mmb_name != NULL)
        strncpy(mmi.mmb_name, mmb_name, sizeof(mmi.mmb_name));
      
      if (ioctl(mmz, IOC_MMB_ALLOC, &mmi) !=0)
    			return NULL;

      return (void *)mmi.phys_addr;

}

HI_VOID *JPEG_MMB_MapToVirAddr(int mmz, void *phyAddr, int cached)
{

	  HI_S32 s32Ret;
	  
      mmb_info   mmi;
      memset(&mmi,0,sizeof(mmi));
	  
	  if(cached != 0 && cached != 1)
	  {
	    return NULL;
	  }
      mmi.prot = PROT_READ | PROT_WRITE;
      mmi.flags = MAP_SHARED;
      mmi.phys_addr = (unsigned long)phyAddr;

	  if(cached)
	  {
	      s32Ret = ioctl(mmz,IOC_MMB_USER_REMAP_CACHED, &mmi);
          if (s32Ret!=0)
          {
    		return NULL;
          }
		  
	  }
	  else
	  {
	      s32Ret = ioctl(mmz,IOC_MMB_USER_REMAP, &mmi);
          if (s32Ret!=0)
          {
    		return NULL;
          }
	  }
	  
      return (void *)mmi.mapped;

	
}

/*****************************************************************************
* func                     : JPEG_MMB_UnmapToPhyAddr
* param[in] :mmz           :
* param[in] :phyAddr       : 
* retval    :
* others:	:nothing
*****************************************************************************/
HI_S32 JPEG_MMB_UnmapToPhyAddr(int mmz, void *phyAddr)
{

      mmb_info   mmi;
      memset(&mmi,0,sizeof(mmi));
      mmi.phys_addr = (unsigned long)phyAddr;
      return ioctl(mmz, IOC_MMB_USER_UNMAP, &mmi);
  
}

HI_S32 JPEG_MMB_RealseMMZDev(int mmz, void *phyAddr)
{

      mmb_info   mmi;
      memset(&mmi,0,sizeof(mmi));
      mmi.phys_addr = (unsigned long)phyAddr;
      return ioctl(mmz, IOC_MMB_FREE, &mmi);
  
}

/*****************************************************************************
* func                     : before call hard decode, we should call this ensure
                             to cache synchrotron
* param[in] :mmz
* param[in] :
* retval    :
* others:	:nothing
*****************************************************************************/
HI_S32 JPEG_MMB_Flush(int mmz)
{
	return ioctl(mmz, IOC_MMB_FLUSH_DCACHE, NULL);
}

#endif

/*****************************************************************************
* func      : calculate a/b
* param[in] :long a 
* param[in] :long b
* retval    :long
* others:	:nothing
*****************************************************************************/

static long  HIJPEG_Round_Up (long a, long b)
{

    return (a + b - 1L) / b; 

}

/*****************************************************************************
* func        : JPEG_Calc_Output_Dimensions
* description : calculate the image size, through this function we can get the 
                output_width and output_height of the image
* param[in]   : cinfo         
* param[in]   : *ps32MMZWidth 
* param[in]   : *ps32MMZHeight
* retval      : HI_FLASE if failure
* retval
* others:	  : nothing
*****************************************************************************/

static HI_BOOL  JPEG_Calc_Output_Dimensions (j_decompress_ptr cinfo,HI_U32 *pu32OutputWidth,HI_U32 *pu32OutputHeight)
{


	  HI_S32 s32Ratio = 0;
	   
	  s32Ratio = JPEG_Get_ScaleRatio(cinfo);

	  switch(s32Ratio)
	  {
	     case 0:
		 {
	            /********************** Provide 1/1 scaling ****************/
	            cinfo->output_width = cinfo->image_width;
	            cinfo->output_height = cinfo->image_height;
				break;
	     }
		 case 1:
	 	 {
	           /********************** Provide 1/2 scaling ******************/
	           cinfo->output_width = (JDIMENSION)
	                   HIJPEG_Round_Up((long) cinfo->image_width, 2L);

	           cinfo->output_height = (JDIMENSION)
	                   HIJPEG_Round_Up((long) cinfo->image_height, 2L);
			   break;
	 	 }
		 case 2:
		 {
			 	/****************** Provide 1/4 scaling *********************/
	            cinfo->output_width = (JDIMENSION)
	                    HIJPEG_Round_Up((long) cinfo->image_width, 4L);

	            cinfo->output_height = (JDIMENSION)
	                    HIJPEG_Round_Up((long) cinfo->image_height, 4L);
				break;
		 }
         case 3:
		 {
		 	
			 	/**************** Provide 1/8 scaling **********************/
	            cinfo->output_width = (JDIMENSION)
	                  HIJPEG_Round_Up((long) cinfo->image_width, 8L);
	            
	            cinfo->output_height = (JDIMENSION)
	                  HIJPEG_Round_Up((long) cinfo->image_height, 8L);  
				break;
			
			
         }
		 default:
		 	  #ifdef JPEG6B_DEBUG
		 	  HI_JPEG_TRACE("The ration is not support!\n");
			  #endif
		 	  break;

	  }

     *pu32OutputWidth  = cinfo->output_width;
     *pu32OutputHeight = cinfo->output_height;
      
      return HI_TRUE;
      

}


/*****************************************************************************
* func        : JPEG_Dec_GetMemSize
* description : to calculate the memmory size that we will allocated
* param[in]   : cinfo
* retval      : HI_TRUE     If SUCCESS
* retval      : HI_FALSE    If FAILURE
* others:	  : nothing
*****************************************************************************/
static HI_BOOL JPEG_Dec_GetMemSize(j_decompress_ptr cinfo)
{

    HI_U32 YHeightTmp = 0;
    HI_U32 CHeightTmp = 0;
	HI_U32 u32OutputHeight = 0;
	HI_U32 u32OutputWidth = 0;
	JPEG_IMAGEFORMAT_E enImageFmt; /** the image format **/

    JPEG_MESSAGE_S  *pstMessagePrivate;
    j_common_ptr pCinfo = (j_common_ptr)cinfo;
    pstMessagePrivate = (JPEG_MESSAGE_S *)(pCinfo->client_data);
    
	/** calculate the image size **/
	JPEG_Calc_Output_Dimensions (cinfo,&u32OutputWidth,&u32OutputHeight);

	/** s32YStride should 128 bytes align **/
    pstMessagePrivate->s32InYStride = (u32OutputWidth + JPEG_SURFACE_ALIGN - 1) 
                             & (~(JPEG_SURFACE_ALIGN - 1));

    /** get the image format **/
	enImageFmt = JPEG_Api_GetImagFormat(cinfo);
    /** the parameters of Y and C component will be different 
     ** about different image format
     **/
    switch (enImageFmt)
    {
    
        case JPEG_YUV_400:
        {
            /** height allign with MCU **/
            YHeightTmp = (u32OutputHeight + JPEG_MCU_ALIGN8 - 1)
                         & (~(JPEG_MCU_ALIGN8 - 1));

            /** the parameters of C component are zero **/
            CHeightTmp = 0;
            pstMessagePrivate->s32InCStride = 0;
            pstMessagePrivate->u32InImageWandH = (((cinfo->image_width+7)>>3) | (((cinfo->image_height+7)>>3)<<16));
            break;
        }
        case JPEG_YUV_420:
        {
            /** height allign with MCU **/
            YHeightTmp = (u32OutputHeight + JPEG_MCU_ALIGN16 - 1)
                         & (~(JPEG_MCU_ALIGN16 - 1));

			/** the height of C component is half of Y
			 ** and stride is the same as Y
			 **/
            CHeightTmp = YHeightTmp >> 1;
            pstMessagePrivate->s32InCStride = pstMessagePrivate->s32InYStride;
            pstMessagePrivate->u32InImageWandH = (((cinfo->image_width+15)>>4) | (((cinfo->image_height+15)>>4)<<16));
            break;
        }
        case JPEG_YUV_422_21:
        {
            /** height allign with MCU **/
			/**
			 **水平采样
			 **/
            YHeightTmp = (u32OutputHeight + JPEG_MCU_ALIGN8 - 1)
                         & (~(JPEG_MCU_ALIGN8 - 1));
            /** 
             ** CbCr's height is the same as Y height
             ** CbCr's width is half Y width
             ** CbCr's stride is the same as Y height
             **/
            CHeightTmp = YHeightTmp;            
            pstMessagePrivate->s32InCStride = pstMessagePrivate->s32InYStride;
			/**
			 **按照MCU对齐，也就是右移的操作，+的操作是对齐的，和MCU的单位有关系，左移刚好是高度的寄存器
			 **/
            pstMessagePrivate->u32InImageWandH = (((cinfo->image_width+15)>>4) | (((cinfo->image_height+7)>>3)<<16));
            break;
			
        }   
        case JPEG_YUV_422_12:
        {
			
            /** height allign with MCU **/
			/**
			 **垂直采样
			 **/			
            YHeightTmp = (u32OutputHeight + JPEG_MCU_ALIGN16 - 1)
                         & (~(JPEG_MCU_ALIGN16 - 1));

            /** 
             ** CbCr's height is half of Y height
             ** CbCr's width is the same as Y width
             ** CbCr's stride is twice of Y height
             **/
            CHeightTmp = YHeightTmp>>1;
            pstMessagePrivate->s32InCStride = pstMessagePrivate->s32InYStride<<1;
            pstMessagePrivate->u32InImageWandH = (((cinfo->image_width+7)>>3) | (((cinfo->image_height+15)>>4)<<16));
            break;
        }
        default:   /*JPEG_YUV_444:*/
        {
			
            /** height allign with MCU **/
            YHeightTmp = (u32OutputHeight + JPEG_MCU_ALIGN8 - 1)
                         & (~(JPEG_MCU_ALIGN8 - 1));

            /** the height of C component is the same as Y component, 
             ** and the stride is twice as alarge as Y component
             **/
            CHeightTmp = YHeightTmp;
            pstMessagePrivate->s32InCStride = pstMessagePrivate->s32InYStride << 1;
            pstMessagePrivate->u32InImageWandH = (((cinfo->image_width+7)>>3) | (((cinfo->image_height+7)>>3)<<16));
            break;
			
        }
    }
    /** calculate the ycc memmory size **/
    pstMessagePrivate->s32YMemSize = YHeightTmp * pstMessagePrivate->s32InYStride;
    pstMessagePrivate->s32CMemSize = CHeightTmp * pstMessagePrivate->s32InCStride;

   /** if output color space is RGB we should allocate another memory.
    ** and this memmory size can calculate followed **/
   if(   (JCS_RGB == cinfo->out_color_space) 
   	   ||(JCS_BGR == cinfo->out_color_space) )
   {

	  pstMessagePrivate->s32RGBMemSize = u32OutputHeight*(((u32OutputWidth*3)+15)&(~15));
        
   	}
    else if( (JCS_RGB_565 == cinfo->out_color_space)
		    ||(JCS_BGR_565 == cinfo->out_color_space))
    {

        pstMessagePrivate->s32RGBMemSize = u32OutputHeight*(((u32OutputWidth*2)+15)&(~15));

	}
	else if( (JCS_RGBA_8888 == cinfo->out_color_space)
		    ||(JCS_BGRA_8888 == cinfo->out_color_space) )
    {
        pstMessagePrivate->s32RGBMemSize = u32OutputHeight*(((u32OutputWidth*4)+15)&(~15));

	}
	/** if out color space is YUV and virtual output should TDE color convert
     ** and is the same as RGB output
	 **/
	else if(  (JCS_YCbCr == cinfo->out_color_space)
		    ||(JCS_CrCbY == cinfo->out_color_space))
	{

         if(HI_FALSE == pstMessagePrivate->IfHaveCallSetMemMode )
         {

            pstMessagePrivate->s32RGBMemSize = u32OutputHeight*(((u32OutputWidth*3)+15)&(~15));
           
         }

	}

	return HI_TRUE;    
	
}


/*****************************************************************************
* func        : JPEG_Alloc_Memory
* description : we and use this function to allocate memory that we need.
              : with different output image format and output bufffer mode
              : have different allcoate modes.
* param[in]   :cinfo
* retval      : HI_TRUE     If SUCCESS
* retval      : HI_FALSE    If FAILURE
* others:	  : nothing
*****************************************************************************/

static HI_BOOL  JPEG_Alloc_Memmory (j_decompress_ptr cinfo)
{

		  HI_S32 s32MemSize;   
		  
          JPEG_MESSAGE_S  *pstMessagePrivate;
          j_common_ptr pCinfo = (j_common_ptr)cinfo;
          pstMessagePrivate = (JPEG_MESSAGE_S *)(pCinfo->client_data);

          JPEG_Dec_GetMemSize(cinfo);
		  /** output color space is not YCbCr **/
          if(   (JCS_YCbCr != cinfo->out_color_space)
		  	  ||(JCS_CrCbY != cinfo->out_color_space))
          {

              /** if output buffer is virtual buffer we should allocate two 
               ** middle buffer, one is save YUV, and the other is save RGB
               ** that after TDE color convert. if User didn't call
               ** HI_JPEG_SetMemMode, default situation is JPEG_MEMMODE_VIRTUALBUF.
               **/ 
              if(HI_FALSE == pstMessagePrivate->IfHaveCallSetMemMode)
              {

                     /** the memory buffer size **/
                     s32MemSize = MMZ_STREAM_BUFFER+pstMessagePrivate->s32YMemSize+ \
                                     pstMessagePrivate->s32CMemSize+pstMessagePrivate->s32RGBMemSize;


              }
               /** should allocate one middle buffer to save YUV data **/
              else{
                  /** the memory buffer size **/
                  s32MemSize = MMZ_STREAM_BUFFER+pstMessagePrivate->s32YMemSize+  \
                                     pstMessagePrivate->s32CMemSize;

              }/** end else **/


          }

          /** output color space is  YCbCr **/
          else
          {
          
              /** should allocate one middle buffer save YUV data. if User
               ** didn't call HI_JPEG_SetMemMode, default situation is
               **JPEG_MEMMODE_VIRTUALBUF.
               **/
               if(HI_FALSE == pstMessagePrivate->IfHaveCallSetMemMode)
              {

                 s32MemSize = MMZ_STREAM_BUFFER+pstMessagePrivate->s32YMemSize+ \
                    pstMessagePrivate->s32CMemSize+pstMessagePrivate->s32RGBMemSize;

              }
              else
              { /** not need allocate middle buffer, only allocate stream buffer **/


				  s32MemSize = MMZ_STREAM_BUFFER+pstMessagePrivate->s32YMemSize+ \
                                 pstMessagePrivate->s32CMemSize;
                 
              }
              
          }

          /** attention: memory manage with three level **/ 
		  #if 1
		  pstMessagePrivate->mmz_PhyAddr= (char*)HI_MMZ_New((HI_U32)s32MemSize, 64, (HI_CHAR*)"jpeg", "JPEG");
		  if(NULL == pstMessagePrivate->mmz_PhyAddr)
		  {
		      pstMessagePrivate->mmz_PhyAddr= (char*)HI_MMZ_New((HI_U32)s32MemSize, 64, (HI_CHAR*)"graphics", "JPEG");
			  if(NULL == pstMessagePrivate->mmz_PhyAddr)
			  {
                 pstMessagePrivate->mmz_PhyAddr= (char*)HI_MMZ_New((HI_U32)s32MemSize, 64, NULL, "JPEG");
			  }
		  }
          #else
          pstMessagePrivate->mmz_PhyAddr= (char*)JPEG_MMZ_NewPhyAddr(pstMessagePrivate->mmz,s32MemSize, 64, (HI_U8*)"jpeg", "JPEG");
          if(pstMessagePrivate->mmz_PhyAddr == NULL)
          {

                pstMessagePrivate->mmz_PhyAddr= (char*)JPEG_MMZ_NewPhyAddr(pstMessagePrivate->mmz,s32MemSize, 64, (HI_U8*)"graphics", "JPEG");
               if(pstMessagePrivate->mmz_PhyAddr == NULL)
               {
                    pstMessagePrivate->mmz_PhyAddr= (char*)JPEG_MMZ_NewPhyAddr(pstMessagePrivate->mmz,s32MemSize, 64, NULL, "JPEG");
               }

          }
		  #endif
          if(NULL == pstMessagePrivate->mmz_PhyAddr)
          {
                #ifdef JPEG6B_DEBUG
        	    HI_JPEG_TRACE("failed to mmz buffer");
                #endif
                goto FAIL;
          }

		  #if 1
		  pstMessagePrivate->mmz_VirAddr = (char *)HI_MMZ_Map((HI_U32)pstMessagePrivate->mmz_PhyAddr,TRUE);
          #else
		  pstMessagePrivate->mmz_VirAddr = (char*)JPEG_MMB_MapToVirAddr(pstMessagePrivate->mmz,pstMessagePrivate->mmz_PhyAddr,TRUE);
          #endif
          if (pstMessagePrivate->mmz_VirAddr == NULL)
          {
                #ifdef JPEG6B_DEBUG
            	HI_JPEG_TRACE("failed map Phycial address to virtual address!");
                #endif
                goto FAIL;
                
          }
         return HI_TRUE;


	  FAIL:
              #ifdef JPEG6B_DEBUG
	          HI_JPEG_TRACE("allocate memmory failure!\n");
              #endif
	          JPEG_Decode_ReleaseDev(pCinfo);	
	          
	          return HI_FALSE;

}

#if 0
/*****************************************************************************
* func        : JPEG_GetScaleRation_FromStride
* description : calculat the scale ratio
                现在是根据图片大小来决定分配内存大小的，不是固定的
                所以应该不需要如下这个判断了
* param[in]   : cinfo
* retval      : s32Ratio, the scale ratio
* others:	  : nothing
*****************************************************************************/
static HI_VOID Calculate_Scale(HI_S32 s32SrcRation, HI_S32 *pS32DstRation)
{

        /**
         ** if s32Ration = 1; scale 1
         ** if s32Ration = 2; scale 1
         **/
         if(1 == s32SrcRation)
         {
              *pS32DstRation = 1;
         }
		 else if(2 == s32SrcRation)
		 {
              *pS32DstRation = 2;
		 }
		 else if(3 == s32SrcRation)
		 {
              *pS32DstRation = 4;
		 }
		 else
		 {
               *pS32DstRation = 8;
		 }
		 
}

static HI_S32 JPEG_GetScaleRation_FromStride(j_decompress_ptr cinfo, \
                                                               HI_S32 *pS32Ration)
{

	
        HI_S32 s32StrideY = 0,s32StrideCbCr = 0;
	    HI_S32 s32Ration = 1;
		HI_S32 s32TmpRation = 1;
        //HI_S32 s32Size = 0;
	   
	    /** 缩放系数按三种方式处理:
	     ** 1 根据输出内存的大小
	     ** 2 硬件的约束:硬件Y和C分量的stride不能超过4096
	     **/
	    #if 0
		  s32Size = ((cinfo->image_height+15)&(~15))*((cinfo->image_width+127)&(~127))*4;
		  if (s32Size>(16*MMZ_SIZE))
		  {
		       s32Ration=3;
		  }
		  else if (s32Size>(4*MMZ_SIZE))
		  {
		        s32Ration=2;
		  }
		  else if (s32Size>(MMZ_SIZE))
		  {
		        s32Ration=1;
		  }
		  else
		  {
		  	    s32Ration=0;
		  }
	    #endif
	  
        JPEG_IMAGEFORMAT_E enImageFmt; /** the image format **/

		enImageFmt = JPEG_Api_GetImagFormat(cinfo);
        	   
		s32StrideY = (cinfo->image_width + JPEG_SURFACE_ALIGN - 1) 
                             & (~(JPEG_SURFACE_ALIGN - 1));
		
		switch (enImageFmt)
		{
		    
		        case JPEG_YUV_400:
					  s32StrideCbCr = 0;
		              break;
		        case JPEG_YUV_420:
					  s32StrideCbCr = s32StrideY;
					  break;
		        case JPEG_YUV_422_21:
					  s32StrideCbCr = s32StrideY;
		              break;
		        case JPEG_YUV_422_12:
					  s32StrideCbCr = s32StrideY<<1;
		              break;    
		        default:   /*JPEG_YUV_444:*/
					  s32StrideCbCr = s32StrideY<<1;
		              break;
	   }

		Calculate_Scale(s32Ration,&s32TmpRation);
	    s32StrideY =  s32StrideY / s32TmpRation;
	    while (s32StrideY > 4096)
	    {
	         s32Ration++;
			 Calculate_Scale(s32Ration,&s32TmpRation);
	         s32StrideY = s32StrideY / s32TmpRation;
	    }
		if(s32Ration>3)
		{
             return HI_FAILURE;
		}

        Calculate_Scale(s32Ration,&s32TmpRation);
	    s32StrideCbCr =  s32StrideCbCr / s32TmpRation;
	    while (s32StrideCbCr > 4096)
	    {
	        s32Ration++;
			Calculate_Scale(s32Ration,&s32TmpRation);
	        s32StrideCbCr = s32StrideCbCr / s32TmpRation;
	    }

		if(s32Ration>3)
		{
             return HI_FAILURE;
		}

	    *pS32Ration = s32Ration;
			
	    return HI_SUCCESS;
	  

}
#endif

static HI_S32 JPEG_GetScaleRation_FromInput(j_decompress_ptr cinfo)
{

     HI_S32 s32Ratio = 0;
     /** first one, base on user provide scale ratio **/
      if (cinfo->scale_num * 8 <= cinfo->scale_denom){  
      /** Provide 1/8 scaling **/  
           s32Ratio = 3;
      }
      
      else if (cinfo->scale_num * 4 <= cinfo->scale_denom){
      /** Provide 1/4 scaling **/
           s32Ratio = 2;
      }
      
      else if (cinfo->scale_num * 2 <= cinfo->scale_denom){
      /** Provide 1/2 scaling **/
           s32Ratio = 1;
      }
      else {
      /** Provide 1/1 scaling **/
          s32Ratio = 0;      
      }

      return s32Ratio;

}

static HI_S32 JPEG_Get_ScaleRatio(j_decompress_ptr cinfo)
{

       HI_S32 s32Ratio = 0;

      JPEG_MESSAGE_S  *pstMessagePrivate;
      j_common_ptr pCinfo = (j_common_ptr)cinfo;
      pstMessagePrivate = (JPEG_MESSAGE_S *)(pCinfo->client_data);

      #if 0
      /**=======================================================================================
       (1) the scale ration according as the stride
       ========================================================================================**/
	  JPEG_GetScaleRation_FromStride(cinfo,&s32StrideRatio);
      #endif
	  
	  /**====================================================================
       (2) the scale ration according as the input scale_num and scale_denom
       ======================================================================**/
      s32Ratio = JPEG_GetScaleRation_FromInput(cinfo);
      
	 /**====================================================================
	    return the expection scale ration
       ======================================================================**/
	 #if 0
     if(s32StrideRatio>s32Ratio)
     {
        return s32StrideRatio;
     }
	 #endif
   
     return s32Ratio;

   
}

/*****************************************************************************
* func                     : get interrupt status
* param[in] :cinfo         : decompress object
* param[in] :s32ImageSize  : the size of image
* retval    :TRUE          : success
* retval    :FALSE         : failure
* others:	:nothing
*****************************************************************************/

static HI_S32 JPEG_GetIntStatus(j_decompress_ptr cinfo, JPG_INTTYPE_E *pIntType, HI_U32 u32TimeOut)
{

        HI_S32 s32RetVal;
        JPG_GETINTTYPE_S GetIntType;
        
        GetIntType.IntType = JPG_INTTYPE_NONE;
        GetIntType.TimeOut = u32TimeOut;

        JPEG_MESSAGE_S  *pstMessagePrivate;
        j_common_ptr pCinfo = (j_common_ptr)cinfo;
        pstMessagePrivate = (JPEG_MESSAGE_S *)(pCinfo->client_data);

        /** call ioctl to get interruptstatus, if failure return overtime or system err **/
		
        s32RetVal = ioctl(pstMessagePrivate->jpg, CMD_JPG_GETINTSTATUS, &GetIntType);
         
		
        if (HI_SUCCESS != s32RetVal)
        {/** 唤醒中断有三种形式，一个条件成立，二超时，三阻塞**/
            return s32RetVal;
        }
        *pIntType = GetIntType.IntType;

		
        return HI_SUCCESS;
    
}

/*****************************************************************************
* func                     : JPEG_Write_register
* param[in] :reg_virAddr   :
* param[in] :phyOff        : 
* param[in] :value         : 
* retval    :
* others:	:nothing
*****************************************************************************/
static void JPEG_Write_register(volatile char * reg_virAddr, int phyOff, int value)
{

      volatile int * addr;

      /** the phycial offset address can not larger than register length **/
      if (phyOff<X5_JPG_REG_LENGTH)
      {
            /** the two address are equal, so is also write data to this register **/
            addr = (volatile int*)(reg_virAddr+phyOff);
        	*addr = value;
      }
      
}

/*****************************************************************************
* func                     : JPEG_Read_register
* param[in] :reg_virAddr   :
* param[in] :phyOff        : 
* retval    :
* others:	:nothing
*****************************************************************************/

static int JPEG_Read_register(volatile char *reg_virAddr, int phyOff)
{

      return phyOff<X5_JPG_REG_LENGTH ? *(volatile int *)(reg_virAddr+phyOff) : FAILURE;

}

static void JPEG_CopyDataToRegister(volatile char *reg_virAddr, void *memory, int phyOff, size_t bytes)
{

      
      unsigned int cnt;
      
      for (cnt=0;cnt<bytes;cnt+=4)
      {
      
        *(volatile int *)(reg_virAddr+phyOff+cnt)=*(int *)((char*)memory+cnt);

      }

}

#if 0
static void JPEG_CopyDataFromRegister(volatile char *reg_virAddr, int phyOff, void *memory, size_t bytes)
{

      unsigned int cnt;
      
      for (cnt=0;cnt<bytes;cnt+=4)
      { 
      
        *(volatile int *)((char*)memory+cnt)=*(int *)(reg_virAddr+phyOff+cnt);

      }
      
}
#endif

/*****************************************************************************
* func        : JPEG_GetDqtTable
* param[in]   : cinfo
* retval      : HI_TRUE     If SUCCESS
* retval      : HI_FALSE    If FAILURE
* others:	  : nothing
*****************************************************************************/

static int JPEG_GetDqtTable(j_decompress_ptr cinfo)
{

      unsigned short *QCr, *QCb, *QY;
      int Q[DCTSIZE2];

      int i =0;

	  /** 量化表的处理 **/
	  unsigned int ci = 0;
	  /**四张量表，针对不同分量 **/
	  unsigned int quant_tbl_no[NUM_QUANT_TBLS] = {0};
      
      JPEG_MESSAGE_S  *pstMessagePrivate;
      j_common_ptr pCinfo = (j_common_ptr)cinfo;
      pstMessagePrivate = (JPEG_MESSAGE_S *)(pCinfo->client_data);

	  jpeg_component_info * compptr;

	  #if 0
	  /** not use this fixed table **/
      QY=cinfo->quant_tbl_ptrs[0]->quantval;
      QCb=cinfo->quant_tbl_ptrs[1]==NULL?QY:cinfo->quant_tbl_ptrs[1]->quantval;
      QCr=cinfo->quant_tbl_ptrs[2]==NULL?QCb:cinfo->quant_tbl_ptrs[2]->quantval;
	  #else
	  /**
	   ** 修改解码失真的问题，主要是配对量化表号
	   **/
	  for (ci = 0, compptr = cinfo->comp_info; ci < cinfo->num_components;
       ci++, compptr++)
	  {
	     /** 量化表号 **/
		 quant_tbl_no[ci] = compptr->quant_tbl_no;
      }
      QY=cinfo->quant_tbl_ptrs[quant_tbl_no[0]]->quantval;
      QCb=cinfo->quant_tbl_ptrs[quant_tbl_no[1]]==NULL?QY:cinfo->quant_tbl_ptrs[quant_tbl_no[1]]->quantval;
      QCr=cinfo->quant_tbl_ptrs[quant_tbl_no[2]]==NULL?QCb:cinfo->quant_tbl_ptrs[quant_tbl_no[2]]->quantval;
	  #endif
      /** translate to YCbCr format*/
      for(i=0;i<DCTSIZE2;i++)
      {
        
        Q[i]=QY[i]+(QCb[i]<<8)+(QCr[i]<<16);

      }

      /** write to register **/


      JPEG_CopyDataToRegister(pstMessagePrivate->reg_virAddr, (void *)Q, X5_JPGREG_QUANT, sizeof(Q));
      
      
      return HI_TRUE;
  
}


/*****************************************************************************
* func        : JPEG_Decode_huff
* param[in]   : huff_tbl
* param[in]   : bit[256]
* retval      : max_idx
* others:	  : nothing
*****************************************************************************/

static unsigned int JPEG_Decode_huff(JHUFF_TBL *huff_tbl, unsigned int bit[256])
{


      unsigned int idx=0, cnt=0, loc=0, value=0, max_idx=0;

      /** initial to zero **/
      for(loc=value=cnt=0;cnt<256;cnt++)
      {
      
              bit[cnt]=value;

       }

      /** Ln＝16，from one, zero is ignore */
      for(idx=1;idx<17;idx++)
      {

           if (huff_tbl->bits[idx]!=0)
           {
           
                  /** remember the temproary max index **/
                  
                  /** bits[]=iBits[] **/
                  for(cnt=huff_tbl->bits[max_idx=idx];cnt>0;cnt--)
                  {
                              bit[loc++]=value++;
                  }
                  
           }
           value<<=1;
          
      }
      
      return max_idx;
      
}

/*****************************************************************************
* func        : JPEG_Get_dht
* param[in]   : cinfo
* retval      : HI_TRUE     If SUCCESS
* retval      : HI_FALSE    If FAILURE
* others:	  : nothing
*****************************************************************************/

static HI_BOOL JPEG_Get_dht(j_decompress_ptr cinfo)
{


        #define LU 0         /** luminance **/
        
        #define CH 1         /** chrominance **/
        
        #define MAX_TAB 2

        unsigned int cnt,index,pre_index;

        /** 2 Huff tables(DC/AC): 0--luminance,1--chrominance **/
        
        JHUFF_TBL *huff_ptr[MAX_TAB];               /** MAX_TAB＝2 table **/
        
        unsigned int huffcode[MAX_TAB][256]={{0}};  /** dht table,256 is limit value **/

        unsigned int dc_hufflen[MAX_TAB][12]={{0}}; /** dht lenght，YU table，12 is limit value **/

        unsigned int dc_sym[MAX_TAB][12];          
        
        unsigned int temp;

        unsigned int index1;
        
        unsigned int hdc_tab[12];

        unsigned int max_idx[MAX_TAB]={0};
        unsigned int ac_max_sum_syms=0,sum_syms=0,syms;

        unsigned int min_tab[MAX_TAB][16]={{0}};
        unsigned int base_tab[MAX_TAB][16]={{0}};

        unsigned int hac_min_tab[8]={0}, hac_base_tab[8]={0};
        unsigned int hac_symbol_tab[256]={0};

        JPEG_MESSAGE_S  *pstMessagePrivate;
        j_common_ptr pCinfo = (j_common_ptr)cinfo;
        pstMessagePrivate = (JPEG_MESSAGE_S *)(pCinfo->client_data);

		  
        /******************  DC Begin ******************************/
        /** initial the DC table **/

       for(cnt=0;cnt<12;cnt++)
       {
	       hdc_tab[cnt]=0xffff;
       }

       /** luminance DC **/
       huff_ptr[LU] = cinfo->dc_huff_tbl_ptrs[0];
       
       /** chrominance DC **/
       huff_ptr[CH] = cinfo->dc_huff_tbl_ptrs[1]==NULL?cinfo->dc_huff_tbl_ptrs[0]:cinfo->dc_huff_tbl_ptrs[1];

       /** calculate the largest value of iHuffsize, here we can optimize
        ** the all zero of iBits[] that after Ln
        **/
       max_idx[LU]=JPEG_Decode_huff(huff_ptr[LU],huffcode[LU]);

       /** calculate the largest value of iHuffsize for chrominance**/
       max_idx[CH]=JPEG_Decode_huff(huff_ptr[CH],huffcode[CH]);
       

       

      /** Store huff length and huff value number **/
      for(cnt=0;cnt<MAX_TAB;cnt++)/** two, one is luminance, the other is chrominance*/
      {
      
            	temp=0;
            	sum_syms=0;
            	/** the largest value of iHuffsize**/
            	for(index=0;index<max_idx[cnt];index++)
            	{
            	   
            		syms=huff_ptr[cnt]->bits[index+1];

            		if(syms)
            		{
            		        /** calculate the number of node **/
                			sum_syms +=syms; 
                			while(syms--)
                			{
                			    /** calclulate the value of iHuffsize **/
                				dc_hufflen[cnt][temp]=index+1;

                				/** calculate iHuffVal[] **/
                				dc_sym[cnt][temp]=huff_ptr[cnt]->huffval[temp];
                				temp++;
                			}
            	    }
            		
        	    }
	
            	/************* sort ***************/
            	
            	/** from less to large about iHuffVal[] **/
            	for(index=0;index<sum_syms;index++)
            	{
            		for(index1=index+1;index1<sum_syms;index1++)
            			if(dc_sym[cnt][index]>dc_sym[cnt][index1])
            			{
            				CAI_JPEG_SWAP(dc_sym[cnt][index],dc_sym[cnt][index1]);
            				CAI_JPEG_SWAP(dc_hufflen[cnt][index],dc_hufflen[cnt][index1]);
            				CAI_JPEG_SWAP(huffcode[cnt][index],huffcode[cnt][index1]);
            			}

            	}
            	
              /** Generate DC table **/
              
              /** this table make up with iHuffVal[],iHuffSize,iHuffcode **/
             	for(index=0;index<sum_syms;index++)
            		*(hdc_tab+dc_sym[cnt][index])=((*(hdc_tab+dc_sym[cnt][index]))&(~(0xfff<<(12*cnt))) )|((dc_hufflen[cnt][index]&0xf)<<(8+12*cnt))|((huffcode[cnt][index]&0xff)<<(12*cnt));


       }

       /** Set HW DC table **/

       JPEG_CopyDataToRegister(pstMessagePrivate->reg_virAddr,hdc_tab,X5_JPGREG_HDCTABLE,48);
          


      /******* AC Begin ******************/
      /***** Generate hac_min_table, hac_base_table **/

      huff_ptr[LU] = cinfo->ac_huff_tbl_ptrs[0];
      huff_ptr[CH] = cinfo->ac_huff_tbl_ptrs[1]==NULL?cinfo->ac_huff_tbl_ptrs[0]:cinfo->ac_huff_tbl_ptrs[1];

      max_idx[LU]=JPEG_Decode_huff(huff_ptr[0],huffcode[LU]);
      max_idx[CH]=JPEG_Decode_huff(huff_ptr[1],huffcode[CH]);

      /** Luminance and Chrominance: LU=0,CH=1;**/
      for(cnt=0;cnt<MAX_TAB;cnt++)
       {
       
            	sum_syms=0;
              	for(index=0;index<16;index++)
              	{
                		syms=huff_ptr[cnt]->bits[index+1];
                		pre_index=index?index-1:0;
                		if(index<max_idx[cnt])
                		{
                			min_tab[cnt][index]=(min_tab[cnt][pre_index]+huff_ptr[cnt]->bits[index])<<1;
                			if(syms)
                				base_tab[cnt][index]=sum_syms-min_tab[cnt][index];

                			sum_syms += huff_ptr[cnt]->bits[index+1];

                		}
                		else
                		{
                			min_tab[cnt][index]=~0;
                  		}
            	}
              	
            	/** Create LU/CH symbol table **/
            	for(index=0;index<sum_syms;index++)
            		hac_symbol_tab[index]|=((unsigned int)(huff_ptr[cnt]->huffval[index])<<(8*cnt));

            	if(ac_max_sum_syms<sum_syms)
            		ac_max_sum_syms=sum_syms;

            	
      }

      /** Conbine Luminnance and Chrominance **/
      for(index=0;index<8;index++)
      {
      
        	hac_min_tab[index]=((min_tab[CH][2*index+1]&0xff)<<24)
        			|((min_tab[CH][2*index]&0xff)<<16)
        			|((min_tab[LU][2*index+1]&0xff)<<8)
        			|(min_tab[LU][2*index]&0xff);

        	hac_base_tab[index]=((base_tab[CH][2*index+1]&0xff)<<24)
        			|((base_tab[CH][2*index]&0xff)<<16)
        			|((base_tab[LU][2*index+1]&0xff)<<8)
        			|(base_tab[LU][2*index]&0xff);
        	
      }

      /** Write hac_min/base/symbol_table into  HW reisters **/

      JPEG_CopyDataToRegister(pstMessagePrivate->reg_virAddr,hac_min_tab,X5_JPGREG_HACMINTABLE,32);
      JPEG_CopyDataToRegister(pstMessagePrivate->reg_virAddr,hac_base_tab,X5_JPGREG_HACBASETABLE,32);
      JPEG_CopyDataToRegister(pstMessagePrivate->reg_virAddr,hac_symbol_tab,X5_JPGREG_HACSYMTABLE,0x2c0);  //0x2c0=ac_max_sum_syms*4

      return HI_TRUE;

  
}


/*****************************************************************************
* func        : JPEG_Get_sof
* param[in]   : cinfo
* retval      : HI_TRUE     If SUCCESS
* retval      : HI_FALSE    If FAILURE
* others:	  : nothing
*****************************************************************************/

static HI_BOOL JPEG_Get_sof(j_decompress_ptr cinfo)
{
 
      unsigned int ratio = 0;
      JPEG_IMAGEFORMAT_E enImageFmt = ~0;

      JPEG_MESSAGE_S  *pstMessagePrivate;
      j_common_ptr pCinfo = (j_common_ptr)cinfo;
      pstMessagePrivate = (JPEG_MESSAGE_S *)(pCinfo->client_data);

      /*********** sent image format to picture register ************/
      enImageFmt = JPEG_Api_GetImagFormat(cinfo);

      /** frame image and scene image **/
      if (enImageFmt == (unsigned int )(~0))
      {
          #ifdef JPEG6B_DEBUG
          HI_JPEG_TRACE("The image format that hardware is not support!\n");
          #endif
          
      }
      else
      {
          JPEG_Write_register(pstMessagePrivate->reg_virAddr, X5_JPGREG_PICTYPE,enImageFmt); 
      }

      /*********** sent scale ratio to picture register ************/
      ratio = JPEG_Get_ScaleRatio(cinfo);
      if (ratio == (unsigned int)-1)
      {
          return HI_FALSE;
      }
      JPEG_Write_register(pstMessagePrivate->reg_virAddr,X5_JPGREG_SCALE,(JPEG_Read_register(pstMessagePrivate->reg_virAddr,X5_JPGREG_SCALE)&0xfffffffc)|ratio);
 
  
      if (((pstMessagePrivate->u32InImageWandH&(~0x3ff03ff))!=0)) 
      {
            #ifdef JPEG6B_DEBUG
            HI_JPEG_TRACE("origin image width height error\n");
            #endif
            return HI_FAILURE;
        
      }

      /** write the image width and height **/
      JPEG_Write_register(pstMessagePrivate->reg_virAddr, X5_JPGREG_PICSIZE,pstMessagePrivate->u32InImageWandH);

       /** write the stride of y and c **/

	    /** if have call set mode function, users must to physcial output **/
	 if(HI_TRUE == pstMessagePrivate->IfHaveCallSetMemMode)
	 {
	  	      /** if out color space is JCS_YCbCr, output the users stride
	           ** immediacy
	  	       **/
	          if(  (JCS_YCbCr == cinfo->out_color_space)
			  	 ||(JCS_CrCbY == cinfo->out_color_space))
			  {
				     JPEG_Write_register(pstMessagePrivate->reg_virAddr, X5_JPGREG_STRIDE, \
                       (pstMessagePrivate->s32InCStride<<16)|pstMessagePrivate->s32InYStride);

			  }
			  else
			  {
                   JPEG_Write_register(pstMessagePrivate->reg_virAddr, X5_JPGREG_STRIDE, \
                       (pstMessagePrivate->s32InCStride<<16)|pstMessagePrivate->s32InYStride);
			  }
			  
	  }
	  else
	  {
		  /** whether physcial output with RGB or virtual RGB or YUV,
		   ** you should output your own stride.
		   **/
            JPEG_Write_register(pstMessagePrivate->reg_virAddr, X5_JPGREG_STRIDE, \
                (pstMessagePrivate->s32InCStride<<16)|pstMessagePrivate->s32InYStride);
	   }

      return HI_TRUE;

  
}

/*****************************************************************************
* func                     : Set the inflexion by imagesize to choose is hard decode
                             or software decode
* param[in] :cinfo         : decompress object
* param[in] :s32ImageSize  : the size of image
* retval    :TRUE          : success
* retval    :FALSE         : failure
* others:	:nothing
*****************************************************************************/

HI_BOOL JPEG_SetInflexion(HI_U32 u32JpegInflexionSize)
{

      s_u32JpegInflexion = u32JpegInflexionSize;
      return HI_TRUE;
        
}

#if 0
/*****************************************************************************
* func        : JPEG_IsSupportTheInputSize
* description : check the input image size whether support,
                the chip of 3716C has this limit。
* param[in]   : cinfo
* retval      : 
* others:	  : nothing
*****************************************************************************/
static HI_S32 JPEG_IsSupportTheInputSize(j_decompress_ptr cinfo)
{


	
        HI_S32 s32Width     = 0;
	    HI_S32 s32Height    = 0;
	    HI_S32 s32WidthMax  = 0;
	    HI_S32 s32HeightMax = 0;
        HI_S32 s32Ration    = 0;
       JPEG_IMAGEFORMAT_E enImageFmt; /** the image format **/

	   /**====================================================================================

           3716C对于图片输入的大小有底下这些限制，其它的芯片没有
           
	                   1/1 缩放输出(w,h)   1/2 缩放输出(w,h)  1/4 缩放输出(w,h)  1/8 缩放输出(w,h)

           yuv400         4096x4096       8192x8192             8192x8192         8192x8192

           yuv420         4096x4096       8192x8192             8192x8192         8192x8192

           yuv422(2x1)    4096x4096       4096x8192             8192x8192         8192x8192

           yuv422(1x2)    2048x4096       4096x8192             8192x8192         8192x8192

           yuv444         2048x4096       4096x8192             8192x8192         8192x8192

       =========================================================================================**/

	   
	   /** get the image format **/
		enImageFmt = JPEG_Api_GetImagFormat(cinfo);

		s32Width  = cinfo->image_width;
		s32Height = cinfo->image_height;

		s32Ration = JPEG_Get_ScaleRatio(cinfo);
		
		switch (enImageFmt)
		{
		    
		        case JPEG_YUV_400:
					  if(0 == s32Ration)
					  {
					  	 s32WidthMax  = 4096;
					     s32HeightMax = 4096;
					  }
					  else
					  {
					      s32WidthMax  = 8192;
					      s32HeightMax = 8192;
					  }
		              break;
		        case JPEG_YUV_420:
					  if(0 == s32Ration)
					  {
					  	 s32WidthMax  = 4096;
					     s32HeightMax = 4096;
					  }
					  else
					  {
					      s32WidthMax  = 8192;
					      s32HeightMax = 8192;
					  }
		              break;
		        case JPEG_YUV_422_21:
					  if(0 == s32Ration)
					  {
					  	 s32WidthMax  = 4096;
					     s32HeightMax = 4096;
					  }
					  else if(1 == s32Ration)
					  {
					      s32WidthMax   = 4096;
					      s32HeightMax  = 8192;
					  }
					  else
					  {
                          s32WidthMax      = 8192;
					      s32HeightMax     = 8192;
					  }
		              break;
		        case JPEG_YUV_422_12:
					  if(0 == s32Ration)
					  {
					  	 s32WidthMax  = 2048;
					     s32HeightMax = 4096;
					  }
					  else if(1 == s32Ration)
					  {
					      s32WidthMax  = 4096;
					      s32HeightMax = 8192;
					  }
					  else
					  {
                          s32WidthMax  = 8192;
					      s32HeightMax = 8192;
					  }
		              break;    
		        default:   /*JPEG_YUV_444:*/
		        {
				
					  if(0 == s32Ration)
					  {
					  	 s32WidthMax  = 2048;
					     s32HeightMax = 4096;
					  }
					  else if(1 == s32Ration)
					  {
					      s32WidthMax  = 4096;
					      s32HeightMax = 8192;
					  }
					  else
					  {
                          s32WidthMax  = 8192;
					      s32HeightMax = 8192;
					  }
		              break;
					
		        }
	   }
	   
	   if(s32Width > s32WidthMax || s32Height > s32HeightMax)
	   {
          return HI_FAILURE;
	   }
	  
	   return HI_SUCCESS;
	  

}
#endif

/*****************************************************************************
* func        : JPEG_Decode_IsHWSupport
* description : check whether hard support
* param[in]   : cinfo
* retval      : HI_TRUE     If SUCCESS
* retval      : HI_FALSE    If FAILURE
* others:	  : nothing
*****************************************************************************/

HI_BOOL JPEG_Decode_IsHWSupport(j_decompress_ptr cinfo)
{
 

        HI_S32  s32Ratio = 0;
        //HI_S32  s32RetVal = HI_FAILURE;
		HI_BOOL RetVal = HI_TRUE;
        
        HI_U32 u32InflexionSize = 0;
        HI_U32 u32ImageSize = 0;
       
        JPEG_MESSAGE_S  *pstMessagePrivate;
        j_common_ptr pCinfo = (j_common_ptr)cinfo;
        pstMessagePrivate = (JPEG_MESSAGE_S *)(pCinfo->client_data);

       /** at hard decode, we check this message only once just ok 
        ** because only one program can operation,and if the message
        ** is wrong, the hardware can not support, so the followed 
        ** can not operation
        **/
       if(FALSE == JPEG_Api_CheckAddStructMessage(pCinfo,pstMessagePrivate)){
          pstMessagePrivate->hardware_support = HI_FALSE;
          return HI_FALSE;
       }


       /** if call this, representative want to soft or hard decode **/
	   if(HI_TRUE == pstMessagePrivate->HaveCallForceDecodeType){
	       if(HI_TRUE == pstMessagePrivate->UseSoftDecode){
		   	   #ifdef JPEG6B_DEBUG
		   	   HI_JPEG_TRACE("force to use soft decode!\n"); 
			   #endif
		   	   pstMessagePrivate->hardware_support = HI_FALSE;
	           return HI_FALSE;
	       }
		   else{
		   	   #ifdef JPEG6B_DEBUG
		   	   HI_JPEG_TRACE("force to use hard decode!\n"); 
			   #endif
		   	   pstMessagePrivate->hardware_support = HI_TRUE;
		   	   return HI_TRUE;
		   }
	   }
	   /**************************************************************/
	   
        /**  set the image inflexion, use hardwire decode or soft decode **/
        u32ImageSize = (cinfo->image_width)*(cinfo->image_height);
        u32InflexionSize = s_u32JpegInflexion;
        if(u32ImageSize > u32InflexionSize){
             RetVal = HI_TRUE;
        }
        else{
			 pstMessagePrivate->hardware_support = HI_FALSE;
			 return HI_FALSE;
        }
        
         /** if the image_width and image_height larger than 8192 or less
          **than 1, hard cann't support.
          ** the the largest of Y stride is image_width.
          **/
        if ((cinfo->image_width<1) || (cinfo->image_width>=8192) || 
                   (cinfo->image_height<1) || (cinfo->image_height>=8192))
        {
             #ifdef JPEG6B_DEBUG
             HI_JPEG_TRACE("the width and height is not support for hard decode!"); 
			 #endif
			 pstMessagePrivate->hardware_support = HI_FALSE;
			 return HI_FALSE;
            
        }
		
        /** hard decode can  support scale ratio only 1 1/2 1/4 1/8 **/
	   #if defined (CHIP_TYPE_hi3716m) || defined (CHIP_TYPE_hi3716c) || defined (CHIP_TYPE_hi3716h)
       /**
        **通过芯片类新来控制 3712 ＝ X6V300
        **/
        s32RetVal = JPEG_IsSupportTheInputSize(cinfo);
	   if(HI_FAILURE == s32RetVal)
	   {
		     #ifdef JPEG6B_DEBUG
             HI_JPEG_TRACE("the stride that hard unsupport!"); 
			 #endif
		     pstMessagePrivate->hardware_support = HI_FALSE;
			 return HI_FALSE;
	   }
	   //#elif defined(CHIP_TYPE_hi3712)
	   #endif
	  
        s32Ratio = JPEG_Get_ScaleRatio(cinfo);
        if(s32Ratio > 3)
        {
             #ifdef JPEG6B_DEBUG
             HI_JPEG_TRACE("Hardware can not support the scale ratio = %d\n!",s32Ratio);
			 #endif
			 pstMessagePrivate->hardware_support = HI_FALSE;
			 return HI_FALSE;
        }
        /** check the color_space whethe the hardwire can decode **/
        switch(cinfo->out_color_space)
        {

              /**  hardwire decode can support followed three type **/
              #if 0
			  /****/
              case JCS_YCbCr:
			  #endif
			  case JCS_CrCbY:
              case JCS_RGB:
			  case JCS_BGR:
              case JCS_RGBA_8888:
			  case JCS_BGRA_8888:
              case JCS_RGB_565:
			  case JCS_BGR_565:
                   break;
                   
              default:
                 #ifdef JPEG6B_DEBUG
        	  	 HI_JPEG_TRACE("Hardware can not support this out_color_space:%d!", cinfo->out_color_space);
                 #endif
                 pstMessagePrivate->hardware_support = HI_FALSE;
				 return HI_FALSE;
          		 
         }


        /** progressive, arith code ,data_prcidion !=8, cann't use hard decode **/
        if ((cinfo->progressive_mode != FALSE) || (cinfo->arith_code != FALSE) ||
            (cinfo->data_precision != 8) || (cinfo->quant_tbl_ptrs[0]==NULL) )
        {
                #ifdef JPEG6B_DEBUG
                HI_JPEG_TRACE("cinfo->progressive_mode:%d, cinfo->arith_code:%d , cinfo->data_precision:%d, cinfo->quant_tbl_ptrs[0]:%x", 
        			                cinfo->progressive_mode,cinfo->arith_code, cinfo->data_precision, cinfo->quant_tbl_ptrs[0]);    
                #endif
                pstMessagePrivate->hardware_support = HI_FALSE;
				return HI_FALSE;
            
        }
        if ((cinfo->dc_huff_tbl_ptrs[0]==NULL) || (cinfo->dc_huff_tbl_ptrs[2]!=NULL) ||
      	   (cinfo->ac_huff_tbl_ptrs[0]==NULL) || (cinfo->ac_huff_tbl_ptrs[2]!=NULL) )
        {
                #ifdef JPEG6B_DEBUG
                HI_JPEG_TRACE("the width and height is not support!:cinfo->dc_huff_tbl_ptrs[0]:%d, cinfo->dc_huff_tbl_ptrs[2]:%d,cinfo->ac_huff_tbl_ptrs[0]:%d, cinfo->ac_huff_tbl_ptrs[2]:%d",
        			                cinfo->dc_huff_tbl_ptrs[0],cinfo->dc_huff_tbl_ptrs[2],cinfo->ac_huff_tbl_ptrs[0],cinfo->ac_huff_tbl_ptrs[2]);    
                #endif
				pstMessagePrivate->hardware_support = HI_FALSE;
				return HI_FALSE;
                
        }  
        pstMessagePrivate->hardware_support = RetVal;
	    #ifdef JPEG6B_DEBUG
		if(HI_TRUE == RetVal){
            HI_JPEG_TRACE("To use hard decode now\n");
		}
		else{
			HI_JPEG_TRACE("To use soft decode now\n");
		}
		#endif
        return RetVal;
        
}


/*****************************************************************************
* func        : JPEG_Decode_ReleaseDev
* description : release the device that opened
* param[in]   : cinfo
* retval      : HI_TRUE     If SUCCESS
* retval      : HI_FALSE    If FAILURE
* others:	  : nothing
*****************************************************************************/

HI_BOOL JPEG_Decode_ReleaseDev(j_common_ptr cinfo)
{

          
          JPEG_MESSAGE_S  *pstMessagePrivate;
          pstMessagePrivate = (JPEG_MESSAGE_S *)(cinfo->client_data);
                       
          if (pstMessagePrivate->reg_virAddr!=NULL){
                munmap((void*)pstMessagePrivate->reg_virAddr, X5_JPG_REG_LENGTH);
                pstMessagePrivate->reg_virAddr=NULL;
          }
          
          if (pstMessagePrivate->mmz_VirAddr!=NULL)
		  {
		        #if 1
                HI_MMZ_Unmap((HI_U32)pstMessagePrivate->mmz_PhyAddr);
                HI_MMZ_Delete((HI_U32)pstMessagePrivate->mmz_PhyAddr);
                #else
				JPEG_MMB_UnmapToPhyAddr(pstMessagePrivate->mmz,pstMessagePrivate->mmz_PhyAddr);
				JPEG_MMB_RealseMMZDev(pstMessagePrivate->mmz, (void*)pstMessagePrivate->mmz_PhyAddr);
                #endif
                pstMessagePrivate->mmz_VirAddr=NULL;
				pstMessagePrivate->mmz_PhyAddr=NULL; 
          }
          
          if (pstMessagePrivate->jpg>0){
                close(pstMessagePrivate->jpg);
                pstMessagePrivate->jpg = 0;      
          }
          if (pstMessagePrivate->openTDE==HI_TRUE) {
                HI_TDE2_Close();
                pstMessagePrivate->openTDE = HI_FALSE;   
          }

		 pstMessagePrivate->hardware_support = HI_FALSE;
		 pstMessagePrivate->HaveDoneSetPara = HI_FALSE;
         pstMessagePrivate->HaveDoneSentStream= HI_FALSE;
		  
          return HI_TRUE;    

}


/*****************************************************************************
* func        : JPEG_Decode_OpenDev
* description : open the device that hard decode need.
* param[in]   : cinfo
* retval      : HI_TRUE     If SUCCESS
* retval      : HI_FALSE    If FAILURE
* others:	  : nothing
*****************************************************************************/

HI_BOOL JPEG_Decode_OpenDev(j_common_ptr cinfo)
{


      /*********************initial the parameter ************************/
      HI_S32 s32RetVal = 0;
      JPEG_MESSAGE_S  *pstMessagePrivate;
      pstMessagePrivate = (JPEG_MESSAGE_S *)(cinfo->client_data);
	  if(HI_FALSE == pstMessagePrivate->hardware_support)
	  {
          #ifdef JPEG6B_DEBUG
          HI_JPEG_TRACE("hard cann't support!\n");
		  #endif
		  return HI_FALSE;
	  }
	  
      else
      {


               /*********************open the jpg device**************************/
               if((pstMessagePrivate->jpg)<=0){
                    pstMessagePrivate->jpg = open(JPG_DEV, O_RDWR | O_SYNC);
                    if (pstMessagePrivate->jpg < 0) {
                         #ifdef JPEG6B_DEBUG
                         HI_JPEG_TRACE("open jpeg device failed!\n");
                         #endif
                         goto FAIL;     
                    }
                    else{
                        s32RetVal = ioctl(pstMessagePrivate->jpg, CMD_JPG_GETDEVICE);
                        if (HI_SUCCESS!=s32RetVal){
                            #ifdef JPEG6B_DEBUG
                            HI_JPEG_TRACE("get jpeg device failed");
                            #endif
                            goto FAIL;
                        } 
                    }   
               }
              /*********************open the TDE device**************************/
              if (HI_TDE2_Open()!=HI_SUCCESS){
                   pstMessagePrivate->openTDE = HI_FALSE;
                   #ifdef JPEG6B_DEBUG
                   HI_JPEG_TRACE("failed to open TDE!");
                   #endif
                   goto FAIL;   
              }
              else{
              	  pstMessagePrivate->openTDE = HI_TRUE;	  
              }   
			  return HI_TRUE;

      
    } 
     /* if decode failure, jump here */
     FAIL:
         #ifdef JPEG6B_DEBUG
		 HI_JPEG_TRACE("process to soft decode!\n");
		 #endif
         JPEG_Decode_ReleaseDev(cinfo);				
         return HI_FALSE;


}





/*****************************************************************************
* func        : JPEG_Decode_SetParameter
* description : set the parameter before decode
* param[in]   : cinfo
* retval      : HI_TRUE     If SUCCESS
* retval      : HI_FALSE    If FAILURE
* others:	  : nothing
*****************************************************************************/

HI_BOOL JPEG_Decode_SetParameter(j_decompress_ptr cinfo)
{

          HI_BOOL RetVal = HI_FALSE;
          JPEG_MESSAGE_S  *pstMessagePrivate;
          j_common_ptr pCinfo = (j_common_ptr)cinfo;
          pstMessagePrivate = (JPEG_MESSAGE_S *)(pCinfo->client_data);
          
          /** the register virtual address **/
          pstMessagePrivate->reg_virAddr = (volatile char*  )mmap(NULL, X5_JPG_REG_LENGTH, PROT_READ | PROT_WRITE, MAP_SHARED, pstMessagePrivate->jpg, (off_t)0);

          /** write register with 0xffffffff **/
          JPEG_Write_register(pstMessagePrivate->reg_virAddr,X5_JPGREG_RESET,~0);

          /** allocate memmory, through this we can get
           ** 1.the scale ratio
           ** 2.the y and c stride
           ** 3.image output width and height
           **/
          RetVal = JPEG_Alloc_Memmory (cinfo);
          if(HI_FALSE == RetVal){
               #ifdef JPEG6B_DEBUG
		       HI_JPEG_TRACE("alloc memmory failure \n");
               #endif
               goto FAIL;
          }
		  if (JPG_BUF_BLOCK_SIZE > MMZ_STREAM_BUFFER)
		  {   
		      #ifdef JPEG6B_DEBUG
		      HI_JPEG_TRACE("the stream buffer is not enough!");
              #endif
		      goto FAIL;
		  }
          if (JPEG_GetDqtTable(cinfo) != HI_TRUE || 
          	  JPEG_Get_sof(cinfo) != HI_TRUE ||
          	  JPEG_Get_dht(cinfo) != HI_TRUE)
          {
                #ifdef JPEG6B_DEBUG
				HI_JPEG_TRACE("don't have DQT or Sof or DHT!\n");
                #endif
                goto FAIL;
              
          }

          /** turn on interrupt **/
          JPEG_Write_register(pstMessagePrivate->reg_virAddr,X5_JPGREG_INTMASK,0x0);

          /** allocate the physics address to hardwire before start decod each time **/
          /** stream buffer **/
          JPEG_Write_register(pstMessagePrivate->reg_virAddr,X5_JPGREG_STADDR, (int )pstMessagePrivate->mmz_PhyAddr);
          JPEG_Write_register(pstMessagePrivate->reg_virAddr,X5_JPGREG_ENDADDR, (int)(pstMessagePrivate->mmz_PhyAddr + MMZ_STREAM_BUFFER));

          /** if have call set mode function, users must to use physcial output **/
		  if(HI_TRUE == pstMessagePrivate->IfHaveCallSetMemMode){
		  	      /** if out color space is JCS_YCbCr, output the users buffer
                   ** immediacy
		  	       **/

				  if(  (JCS_YCbCr == cinfo->out_color_space)
				  	 ||(JCS_CrCbY == cinfo->out_color_space))
				  {
				  	   JPEG_Write_register(pstMessagePrivate->reg_virAddr,X5_JPGREG_YSTADDR, (int)(pstMessagePrivate->mmz_PhyAddr \
                                                                        +MMZ_STREAM_BUFFER));
                        JPEG_Write_register(pstMessagePrivate->reg_virAddr,X5_JPGREG_UVSTADDR,(int)(pstMessagePrivate->mmz_PhyAddr \
                                                       +MMZ_STREAM_BUFFER+pstMessagePrivate->s32YMemSize));
                  }
				  else
				  {
				  	
				  	   JPEG_Write_register(pstMessagePrivate->reg_virAddr,X5_JPGREG_YSTADDR, (int)(pstMessagePrivate->mmz_PhyAddr \
                                                                        +MMZ_STREAM_BUFFER));
                        JPEG_Write_register(pstMessagePrivate->reg_virAddr,X5_JPGREG_UVSTADDR,(int)(pstMessagePrivate->mmz_PhyAddr \
                                                       +MMZ_STREAM_BUFFER+pstMessagePrivate->s32YMemSize));
				  }
				  
		  }
		  else{
		  /** whether physcial output with RGB or virtual RGB or YUV,
		   ** you should output your own mmz buffer.
		   **/
               JPEG_Write_register(pstMessagePrivate->reg_virAddr,X5_JPGREG_YSTADDR, (int)(pstMessagePrivate->mmz_PhyAddr \
                                                                        +MMZ_STREAM_BUFFER));
               JPEG_Write_register(pstMessagePrivate->reg_virAddr,X5_JPGREG_UVSTADDR,(int)(pstMessagePrivate->mmz_PhyAddr \
                                                       +MMZ_STREAM_BUFFER+pstMessagePrivate->s32YMemSize));
           }
		  
         pstMessagePrivate->HaveDoneSetPara = HI_TRUE;
         return HI_TRUE;
		  
	    FAIL:
              #ifdef JPEG6B_DEBUG
              HI_JPEG_TRACE("Set parameter failure!\n");
			  #endif
              JPEG_Decode_ReleaseDev(pCinfo);	
              
              return HI_FALSE;

}

/*****************************************************************************
* func        : JPEG_Decode_SendStream
* description : send stream and begin to decode 
                硬件解码目前支持的是:
                输出的像素格式和输入源是一样的，是宏块的
                例如；源: yuv420
                  则输出: yuv420宏块

                假如要输出宏块格式的，就要调用setmod扩展接口，然后不进行
                TDE颜色空间转化，软解中也要做相应处理，参见非标的处理流程
                
* param[in]   : cinfo
* retval      : HI_TRUE     If SUCCESS
* retval      : HI_FALSE    If FAILURE
* others:	  : nothing
*****************************************************************************/

HI_BOOL JPEG_Decode_SendStream(j_decompress_ptr cinfo)
{


          JPG_INTTYPE_E eIntStatus = JPG_INTTYPE_NONE; /** interrupt state **/
          
          
          HI_BOOL ReachEOF = HI_FALSE;                 /** if reach at last stream data **/
          HI_BOOL IfStartDecompressor = HI_FALSE;      /** if start decode **/
          
          int NeedDecCnt = 0;                          /** need decode count **/
          int readpos=0;     
		  HI_S32 s32ByteNumbers;
		  
          int i=0; /** for loop**/

          JPEG_MESSAGE_S  *pstMessagePrivate;
          j_common_ptr pCinfo = (j_common_ptr)cinfo;
          pstMessagePrivate = (JPEG_MESSAGE_S *)(pCinfo->client_data);

          jinit_color_deconverter(cinfo);
          if(TRUE == pstMessagePrivate->ErrMsg)
          {
              #ifdef JPEG6B_DEBUG
		      HI_JPEG_TRACE("the color converter is not support\n");
              #endif
			  pstMessagePrivate->bCannotSupport = HI_TRUE;
              goto FAIL;   
          }

          /** hardwire decode **/
          do {
         	  do{	
			  	
        	      s32ByteNumbers = 0;
        	      /** read stream data **/
                  if (0 == cinfo->src->bytes_in_buffer){  
                      #ifdef ANDROID_JPEG6B
                      (*cinfo->src->fill_input_buffer)(cinfo);
                      #else
        		      if (! (*cinfo->src->fill_input_buffer)(cinfo)){
                           #ifdef JPEG6B_DEBUG
        		           HI_JPEG_TRACE("fill input buffer failure!\n");
                           #endif
                           goto FAIL;
                      }
                      #endif
                      #ifdef ANDROID_JPEG6B
        			  if (0==cinfo->src->bytes_in_buffer){
                          break;
        			  }	
                      #else

                      if((0xFF == (*cinfo->src->next_input_byte)) \
                               && (0xD9 == (*(cinfo->src->next_input_byte+1)))
                               && (2 == cinfo->src->bytes_in_buffer)){
                            break;
                        }
                      #endif
                      
        			  readpos = 0; /** when read stream, is zero **/
        		  }
				  
                  /** have enough space **/
        		  if (cinfo->src->bytes_in_buffer <= (JPG_BUF_BLOCK_SIZE - NeedDecCnt)){
        		     s32ByteNumbers = cinfo->src->bytes_in_buffer;
        		  }
        		  else{
        		      s32ByteNumbers = JPG_BUF_BLOCK_SIZE - NeedDecCnt;
        		  }
        		  /** stream buffer, read all stream data **/
        		  memcpy(pstMessagePrivate->mmz_VirAddr + NeedDecCnt, (char*)cinfo->src->next_input_byte+readpos, s32ByteNumbers);

                  NeedDecCnt += s32ByteNumbers;
        		  readpos = s32ByteNumbers;
                  cinfo->src->bytes_in_buffer -= s32ByteNumbers;
				  
        	  } while (NeedDecCnt < JPG_BUF_BLOCK_SIZE);


             #ifdef ANDROID_JPEG6B
             /** check whether is last stream data **/
			 ReachEOF = HI_TRUE;
             if (cinfo->src->bytes_in_buffer) {
                 ReachEOF = HI_FALSE;
             }
        	 else{/** is last stream data **/
        	      cinfo->src->fill_input_buffer(cinfo);
            	  readpos = 0;
    			  if (cinfo->src->bytes_in_buffer){
                      ReachEOF = HI_FALSE;
    			  }	
                
        	 }	

             #else
             ReachEOF = HI_FALSE;
             if((0xFF == (*cinfo->src->next_input_byte)) \
                   && (0xD9 == (*(cinfo->src->next_input_byte+1)))
                   && (2 == cinfo->src->bytes_in_buffer)){
                     ReachEOF = HI_TRUE;
             }
             #endif
             

             /* if you use the cached, when write to register, you should flush it*/
			 #if 1
             HI_MMZ_Flush(0);
             #else
             JPEG_MMB_Flush(pstMessagePrivate->mmz);
			 #endif
             /** the stream  start address **/
        	 JPEG_Write_register(pstMessagePrivate->reg_virAddr,X5_JPGREG_STADD, (int)pstMessagePrivate->mmz_PhyAddr);
             /** the stream  End address **/
             JPEG_Write_register(pstMessagePrivate->reg_virAddr, X5_JPGREG_ENDADD, (int)(pstMessagePrivate->mmz_PhyAddr + NeedDecCnt));

            /** want to start continue stream **/
			 NeedDecCnt = 0;

            /******************** stream register configure mode *************************/
            /**
             **          	     start decode register    whether last stream    start comtinue stream
             **
             ** sent over for one time:         1                        1                    0 
             ** sent first one        :         1                        0                    1
             ** sent middle	          :	   don't configure               0                    1    
             ** sent last one         :    don't configure               1                    0 
             **/

            /********************** sent first one *********************************/
            if (IfStartDecompressor == HI_FALSE){ 
                JPEG_Write_register(pstMessagePrivate->reg_virAddr, X5_JPGREG_RESUME, (ReachEOF ? JPG_EOF_VALUE : 0x0));
                /** start decode register **/
                JPEG_Write_register(pstMessagePrivate->reg_virAddr, X5_JPGREG_START, 1);
                IfStartDecompressor = HI_TRUE;  /** have start the decompressor **/
            }
        	else {/** after first time continue stream start **/
        	    JPEG_Write_register(pstMessagePrivate->reg_virAddr, X5_JPGREG_RESUME, (ReachEOF ? (JPG_EOF_VALUE|JPG_RESUME_VALUE) : JPG_RESUME_VALUE));
        	}

            /**** inspect interrupt bit ***/	
            eIntStatus = JPG_INTTYPE_ERROR;

            /**** get interrupt status ***/
            JPEG_GetIntStatus(cinfo, &eIntStatus, 1000000);

        	/** overtime or error **/
            if (eIntStatus == JPG_INTTYPE_ERROR) {
               #ifdef JPEG6B_DEBUG
               HI_JPEG_TRACE("dec hardware error:0x%x!\n", eIntStatus);    
               #endif
        	   goto FAIL;   
            }
        	else if (eIntStatus == JPG_INTTYPE_FINISH) {/** finish decode **/
                  break; 
            }
        	else if (eIntStatus == JPG_INTTYPE_CONTINUE) {/** should need another stream **/
        	     continue;  
        	}
        	else {/** 中断阻塞的时候返回的是JPG_INTTYPE_NONE值**/
               #ifdef JPEG6B_DEBUG
        	   HI_JPEG_TRACE("unkown status:0x%x!\n", eIntStatus);
               #endif
        	}
  
          } while (1);

          /** if hard decode failure, the followed parameter have not change;
           **
           ** used for getting cinfo->out_color_components,
           **cinfo->output_components
           **/
		  
          cinfo->output_scanline=0;

          /** hard decode success, so change the global state, others 
           ** did not change the global state.
           **/
		  
          cinfo->global_state = DSTATE_STOPPING;
          cinfo->inputctl->eoi_reached=TRUE;
          cinfo->rec_outbuf_height=1;
          cinfo->MCUs_per_row=JPEG_Read_register(pstMessagePrivate->reg_virAddr,X5_JPGREG_PICSIZE)&0xffff;
          cinfo->MCU_rows_in_scan=(JPEG_Read_register(pstMessagePrivate->reg_virAddr,X5_JPGREG_PICSIZE)>>16)&0xffff;
          cinfo->blocks_in_MCU=cinfo->num_components;
	  
          for(i=0;i<cinfo->num_components;i++){
               cinfo->MCU_membership[i]=i;
           }

         /**the send stream has done, so we can call TDE color convert **/
		 pstMessagePrivate->HaveDoneSentStream = HI_TRUE;

         pstMessagePrivate->bCannotSupport = HI_FALSE;
		 pstMessagePrivate->sJpegHtoSInfo.bHDECSuc = HI_TRUE;

         return HI_TRUE;

         /** if decode failure jump here **/
         FAIL:
         
              /** should read file again **/
              cinfo->src->bytes_in_buffer = 0;

              pstMessagePrivate->HaveDoneSentStream = HI_FALSE;
               
			  JPEG_Decode_ReleaseDev(pCinfo); 

			  pstMessagePrivate->sJpegHtoSInfo.bHDECSuc = HI_FALSE;

              return HI_FALSE;
   
}

/*****************************************************************************
* func                     : do outYUVtoVirBuffer or do outRGBToVirBuffer
* param[in] :cinfo         :
* param[in] :s32ImageSize  : 
* retval    :TRUE          : success
* retval    :FALSE         : failure
* others:	:nothing
*****************************************************************************/
HI_S32 JPEG_Decode_TDEColorConvert(j_decompress_ptr cinfo, JSAMPARRAY scanlines, JDIMENSION max_lines)
{

      HI_S32 s32Stride = 0;
      HI_S32 s32ColorComponent = 1;   /** output color component     **/
      TDE_HANDLE s32Handle;           /** TDE handle                 **/
      /** input **/
      TDE2_MB_S SrcSurface;           /** picture message            **/
      /** output**/
      TDE2_SURFACE_S DstSurface;      /** bit image message struct   **/
      TDE2_RECT_S SrcRect, DstRect;   /** TDE rectangle              **/
      TDE2_MBOPT_S stMbOpt;           /** Macro block operation      **/

	  HI_S32 i =0; /** loop count **/


      /**
       **硬件解码输出的五种semiplaner格式。
       **/
	  TDE2_COLOR_FMT_E enMbFmt[] = 
      {
      
          	 TDE2_MB_COLOR_FMT_JPG_YCbCr400MBP,   //0
          	 TDE2_MB_COLOR_FMT_BUTT,
          	 TDE2_MB_COLOR_FMT_BUTT,
          	 TDE2_MB_COLOR_FMT_JPG_YCbCr420MBP,   //6
			 TDE2_MB_COLOR_FMT_JPG_YCbCr422MBHP,  //水平采样
			 TDE2_MB_COLOR_FMT_JPG_YCbCr422MBVP,  //垂直采样
			 TDE2_MB_COLOR_FMT_JPG_YCbCr444MBP,   //7
          	 TDE2_MB_COLOR_FMT_BUTT,
      	 
      };

      /** start color convert **/
      JPEG_MESSAGE_S  *pstMessagePrivate;
      j_common_ptr pCinfo = (j_common_ptr)cinfo;
      pstMessagePrivate = (JPEG_MESSAGE_S *)(pCinfo->client_data);

	  if(HI_FALSE == pstMessagePrivate->HaveDoneTDE )
	  {  
			
	       /** if the output color space is YUV and is physical buffer output
	        **  need not  TDE color convert, otherwise need TDE convert.
	        **/
	        pstMessagePrivate->HaveDoneTDE = HI_TRUE;
			/** if output color space is RGB we should do followed **/
			
	        /** get picture type **/
	        SrcSurface.enMbFmt=enMbFmt[JPEG_Read_register(pstMessagePrivate->reg_virAddr,X5_JPGREG_PICTYPE)];
			/** get y phycial address **/
	        SrcSurface.u32YPhyAddr = (unsigned int)(pstMessagePrivate->mmz_PhyAddr+MMZ_STREAM_BUFFER);
	        /** get C phycial address **/
	        SrcSurface.u32CbCrPhyAddr= (unsigned int)(pstMessagePrivate->mmz_PhyAddr \
	                                       +MMZ_STREAM_BUFFER+pstMessagePrivate->s32YMemSize);
	        /** get y width **/
	        SrcSurface.u32YWidth=cinfo->output_width;
	        /** get y height **/
	        SrcSurface.u32YHeight=cinfo->output_height;
	        
	        /** get VHB stride include Y stride and C stride **/
	        s32Stride=JPEG_Read_register(pstMessagePrivate->reg_virAddr, X5_JPGREG_STRIDE);
	        /** get y stride, eliminate hight bit message **/
	        SrcSurface.u32YStride=s32Stride&0xffff;
	        /** get CbCr stride **/
	        SrcSurface.u32CbCrStride=(s32Stride>>16)&0xffff;
	    	memset(&DstSurface, 0, sizeof(DstSurface));

		    /** after calculate, you should bytes align**/
	        switch(cinfo->out_color_space)
	        {
	        
	              /**  TDE小端输出, 源YUV, TDE出来的是CrCbY , ARGB, TDE要求BGRA**/
	              case JCS_RGB:
	                  s32ColorComponent      = 3;
					  DstSurface.enColorFmt  = TDE2_COLOR_FMT_BGR888;  /** RGB888 **/
	        	      DstSurface.u32Stride   =  cinfo->output_width*3;   /** bit picture width **/
	                  break; 
					  
	              case JCS_BGR:
	                  s32ColorComponent       = 3;
					  DstSurface.enColorFmt   = TDE2_COLOR_FMT_RGB888;  /** BGR888 **/
	        	      DstSurface.u32Stride    = cinfo->output_width*3;   /** bit picture width **/
	                  break; 
					  
	              case JCS_RGBA_8888:
	                  s32ColorComponent       = 4;
                      DstSurface.enColorFmt   = TDE2_COLOR_FMT_ABGR8888;  /** RGBA8888 **/
	        	      DstSurface.u32Stride    = cinfo->output_width*4;
	                  break;
					  
	              case JCS_BGRA_8888:
	                  s32ColorComponent       = 4;
                      DstSurface.enColorFmt   = TDE2_COLOR_FMT_ARGB8888;  /** BGRA8888**/
	        	      DstSurface.u32Stride    = cinfo->output_width*4;
	                  break;  
					  
	              case JCS_RGB_565:
	                   s32ColorComponent     = 2;
	                   DstSurface.enColorFmt = TDE2_COLOR_FMT_BGR565; /** RGB565 **/
	                   DstSurface.u32Stride  = cinfo->output_width*2;
	        	       break;
					   
	              case JCS_BGR_565:
	                   s32ColorComponent      = 2;
	                   DstSurface.enColorFmt  = TDE2_COLOR_FMT_RGB565; /** BGR565 **/
	                   DstSurface.u32Stride   = cinfo->output_width*2;
	        	       break;
	              /** TDE can convert the semi planer of YUV(hard decode) to package of YUV **/
				  /**
				   ** wait for tde to added,no can not go here,because check is hard decode
				   ** support is return false, and go to run soft decode
				   **/
	        	  case JCS_YCbCr:
	        	      s32ColorComponent     = 3;
					  #if 0
					  DstSurface.enColorFmt = TDE2_COLOR_FMT_CrCbY888;  /** YCbCr **/
					  #else
					  DstSurface.enColorFmt  = TDE2_COLOR_FMT_YCbCr888;  /** CrCbY **/
					  #endif
	        	      DstSurface.u32Stride  = cinfo->output_width*3;   /** bit picture width **/
	                  break; 
	        	  case JCS_CrCbY:
	        	      s32ColorComponent      = 3;
	        	      DstSurface.enColorFmt  = TDE2_COLOR_FMT_YCbCr888;  /** CrCbY **/
	        	      DstSurface.u32Stride   = cinfo->output_width*3;   /** bit picture width **/
	                  break; 
	        	   default:
                       #ifdef JPEG6B_DEBUG
					   HI_JPEG_TRACE("the tde color convert format can not support!\n");
                       #endif
                       return -1;
	        	       
		    }



	           /** should second middle buffer, and should output to virtual address**/
	          if(HI_FALSE == pstMessagePrivate->IfHaveCallSetMemMode){

	                 DstSurface.u32PhyAddr = (unsigned int)(pstMessagePrivate->mmz_PhyAddr+MMZ_STREAM_BUFFER \
	                                          +pstMessagePrivate->s32YMemSize+pstMessagePrivate->s32CMemSize);

	                 DstSurface.u32Stride=(DstSurface.u32Stride+15)&(~15); /*bytes align*/

	          }
	          else{
					/** not need middle buffer, output physical buffer **/
	               DstSurface.u32PhyAddr = (unsigned int )pstMessagePrivate->stPub.u32OutPhyAddr[0];
	               DstSurface.u32Stride = pstMessagePrivate->stPub.s32OutStride[0];
	          }
	          
	        DstSurface.u32Height=cinfo->output_height;
	        DstSurface.u32Width=cinfo->output_width;
	        DstSurface.pu8ClutPhyAddr=NULL;
	        DstSurface.bYCbCrClut=HI_FALSE;
	        DstSurface.bAlphaMax255=HI_TRUE;
	        DstSurface.bAlphaExt1555=HI_TRUE;
	        DstSurface.u8Alpha0=0;
	        DstSurface.u8Alpha1=255;

	        /** TDE rectangle **/
	        SrcRect.s32Xpos=0;
	        SrcRect.s32Ypos=0;
	        SrcRect.u32Width=cinfo->output_width;
	        SrcRect.u32Height=cinfo->output_height;

	        /** TDE rectangle **/
	        DstRect.s32Xpos=0;
	        DstRect.s32Ypos=0;
	        DstRect.u32Width=cinfo->output_width;
	        DstRect.u32Height=cinfo->output_height;

	        /** Macro block operation **/
	        memset(&stMbOpt, 0 , sizeof(TDE2_MBOPT_S));
	        /** low quality resize **/
	        stMbOpt.enResize = TDE2_MBRESIZE_QUALITY_LOW;
	        

		   /**TDE转换**/
	       if (s32ColorComponent!=0)
	       {

	              /** begin to color convert **/
	              if ((s32Handle = HI_TDE2_BeginJob()) != HI_ERR_TDE_INVALID_HANDLE)
	              {
					    HI_TDE2_MbBlit(s32Handle, &SrcSurface, &SrcRect, &DstSurface, &DstRect, &stMbOpt);
						HI_TDE2_EndJob(s32Handle, HI_FALSE, HI_TRUE, 10000);  
						
	              }
	        }
   
	  	}
        /** followed three para are used at when call HIJPEG_Dec_OutputToVirBuf()**/

        switch(cinfo->out_color_space)
        {
        
              case JCS_RGB:
			  case JCS_BGR:
                  s32ColorComponent = 3;
                  break; 
                  
              case JCS_RGBA_8888:
			  case JCS_BGRA_8888:
                  s32ColorComponent = 4;
                  break;
                  
              case JCS_RGB_565:
			  case JCS_BGR_565:
                   s32ColorComponent = 2;
        	       break;

              /** TDE can convert the semi planer of YUV(hard decode) to package of YUV **/
        	  case JCS_YCbCr:
			  case JCS_CrCbY:
        	      s32ColorComponent = 3;
                  break; 
        	       
        	    default:
                   #ifdef JPEG6B_DEBUG
				   HI_JPEG_TRACE("the tde color convert format can not support!\n");
                   #endif
                   return -1;
        	       
	    }
			
       s32Stride=(s32ColorComponent*cinfo->output_width+15)&(~15);
	   if ((max_lines+(cinfo->output_scanline)) > (cinfo->output_height))
       {
        
             max_lines = (cinfo->output_height) - (cinfo->output_scanline);
          
       }
	   
	   /** is physical output, you should not output to virtual, so you can return now **/
       if(HI_TRUE == pstMessagePrivate->IfHaveCallSetMemMode )
       {

             /** need not output virtual, but the output_scanline should add **/
		   	 for(i=0; i<max_lines; i++)
		     {
	             (cinfo->output_scanline)++;
	         }
       }
       else{
	   	    /** if user didn't set memory mode, the stride should calculate by us **/
			JPEG_Decode_OutputToVirBuf(cinfo, scanlines,s32Stride,s32ColorComponent,max_lines);
       }

       return max_lines;
		
   
}

/*****************************************************************************
* func                     : JPEG_Decode_OutputToVirBuf
* retval    :TRUE          : success
* retval    :FALSE         : failure
* others:	:nothing
*****************************************************************************/
HI_BOOL JPEG_Decode_OutputToVirBuf(j_decompress_ptr cinfo, JSAMPARRAY scanlines,HI_S32 s32OutputStride, \
                                    HI_S32 s32OutputComponent,HI_S32 s32OutputLines)
{

        HI_S32 s32Lines;
        HI_S32 s32Stride;
        HI_S32 s32Component;
        HI_S32 s32BufSrcLength;


       JPEG_MESSAGE_S  *pstMessagePrivate;
       j_common_ptr pCinfo = (j_common_ptr)cinfo;
       pstMessagePrivate = (JPEG_MESSAGE_S *)(pCinfo->client_data);
	   
	    s32Lines = s32OutputLines;
	    s32Stride = s32OutputStride;
	    s32Component = s32OutputComponent;

        int i=0;
        HI_S32 s32MiddlePara = 0;
        s32MiddlePara =(HI_S32)(pstMessagePrivate->mmz_VirAddr+MMZ_STREAM_BUFFER \
	                    +pstMessagePrivate->s32YMemSize+pstMessagePrivate->s32CMemSize);
        s32BufSrcLength = s32Component*cinfo->output_width;
		/** currently s32Lines = 1**/
	    for(i=0; i<s32Lines; i++)
		{
	         memcpy(scanlines[i],(void*)(s32MiddlePara+((cinfo->output_scanline)++)*s32Stride),\
			 	                             s32BufSrcLength);
	         
	    }

        return HI_TRUE;
}

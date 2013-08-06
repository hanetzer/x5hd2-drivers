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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <assert.h>

#include "jinclude.h"
#include "jdatasrc.h"
#include "jmemsys.h"

#include "hijpeg_api.h"
#include "hijpeg_decode_hw.h"
#include "hi_jpg_ioctl.h"



/***************************** Macro Definition ******************************/



/******************** to see which include file we want to use***************/



/*************************** Structure Definition ****************************/



/********************** Global Variable declaration **************************/


static HI_S32 s_s32JpegDevice = 0;   /** only used at suspend**/
/******************************* API forward declarations *******************/

#if 0
static sem_t  s_IfUseSuspend;
sem_init(&s_IfUseSuspend, 0, 1);
(HI_VOID)sem_wait(&s_IfUseSuspend);
(HI_VOID)sem_post(&s_IfUseSuspend);
#endif

/******************************* API realization *****************************/


/**************************** realize hi_jpeg_api.h **************************/

/*****************************************************************************
* func                     : HI_JPEG_GetVersion
*                           get jpeg-6b version.
* param[in] :ppVersion      : version info
* param[in] :ppBuildTime    : build time
* retval    :TRUE          : success
* retval    :FALSE         : failure
* others:	:nothing
*****************************************************************************/
#define HIJPEG_ADP_VER_MAJOR 1
#define HIJPEG_ADP_VER_MINOR 0
#define HIJPEG_ADP_VER_Z 0
#define HIJPEG_ADP_VER_P 0
#define HIJPEG_ADP_VER_A "a01"

#define MAKE_VER_BIT(x) # x
#define MAKE_MACRO2STR(exp) MAKE_VER_BIT(exp)
#define MAKE_VERSION \
    MAKE_MACRO2STR(HIJPEG_ADP_VER_MAJOR) "." \
    MAKE_MACRO2STR(HIJPEG_ADP_VER_MINOR) "." \
    MAKE_MACRO2STR(HIJPEG_ADP_VER_Z) "." \
    MAKE_MACRO2STR(HIJPEG_ADP_VER_P) " " HIJPEG_ADP_VER_A

/*************************** Structure Definition ****************************/

/********************** Global Variable declaration **************************/
const HI_CHAR* g_pszJpegVersion   = "\nHiJPEG Ver:" MAKE_VERSION "\n";
const HI_CHAR* g_pszJpegBuildTime = "This version is building at " __DATE__ " " __TIME__;

/*****************************************************************************
* func                     : HI_JPEG_GetVersion
* description:
* param[in] :ppVersion
* param[in] :ppBuildTime
* retval    :HI_SUCCESS    : success
* retval    :HI_FAILURE    : failure
* others:	:nothing
*****************************************************************************/
HI_S32 HI_JPEG_GetVersion(HI_CHAR **ppVersion, HI_CHAR **ppBuildTime)
{

    if ((NULL == ppVersion) || (NULL == ppBuildTime))
    {
        return HI_FAILURE;
    }
    *ppVersion   = (HI_CHAR *)g_pszJpegVersion;
    *ppBuildTime = (HI_CHAR *)g_pszJpegBuildTime;
    return HI_SUCCESS;
}


/*****************************************************************************
* func                     : set the input memory mode
: description:  if you call this function, you must to use physcial memmory
                output, if you want to virtual output, you should not call
                this function.
* param[in] :cinfo         : decompress object
* param[in] :pstAppMessage : HI_JPEG_APP_MESSAGE_S struct
* param[in] :scanlines
* retval    :HI_SUCCESS    : success
* retval    :HI_FAILURE    : failure
* others:	:nothing
*****************************************************************************/

HI_S32 HI_JPEG_SetMemMode(j_decompress_ptr cinfo,HI_JPEG_APP_MESSAGE_S pstAppMessage)
{


	  HI_S32 i;
      if(NULL==cinfo){
        #ifdef JPEG6B_DEBUG
	  	HI_JPEG_TRACE("The cinfo is null pointer\n");
        #endif
	  	return HI_FAILURE;
      }
      
      JPEG_MESSAGE_S  *pstMessagePrivate;
      j_common_ptr pCinfo = (j_common_ptr)cinfo;
      pstMessagePrivate = (JPEG_MESSAGE_S *)(pCinfo->client_data);
      if(FALSE == JPEG_Api_CheckAddStructMessage(pCinfo,pstMessagePrivate))
      {
          return HI_FAILURE;
      }

	  if(    (0 == pstAppMessage.u32OutPhyAddr[0])
	  	  || (0 == pstAppMessage.s32OutStride[0]))
	  {
	        #ifdef JPEG6B_DEBUG
		  	HI_JPEG_TRACE("not have any phy mem\n");
	        #endif
            return HI_FAILURE;
	  }
      
      pstMessagePrivate->IfHaveCallSetMemMode = HI_TRUE;

	  for(i=0;i<NEED_ADDR_BLOCK_NUM;i++)
	 {
	      pstMessagePrivate->stPub.u32OutPhyAddr[i] = pstAppMessage.u32OutPhyAddr[i];
		  pstMessagePrivate->stPub.pu32OutVirAddr[i] = pstAppMessage.pu32OutVirAddr[i];
	      pstMessagePrivate->stPub.s32OutStride[i] = pstAppMessage.s32OutStride[i];
	 }

     return HI_SUCCESS;
}



/*****************************************************************************
* func                     : get the input memory mode
* param[in] :cinfo         : decompress object
* param[in] :memmode       : the memory mode
* retval    :HI_SUCCESS    : success
* retval    :HI_FAILURE    : failure
* others:	:nothing
*****************************************************************************/

HI_S32 HI_JPEG_GetMemMode(j_decompress_ptr cinfo,HI_JPEG_APP_MESSAGE_S *pstAppMessage)
{

      /** because it called after set mem mode, so it should not call again **/
     if(NULL==cinfo){
        #ifdef JPEG6B_DEBUG
	  	HI_JPEG_TRACE("The cinfo is null pointer\n");
        #endif
	  	return HI_FAILURE;
      }
     if(NULL == pstAppMessage){
         #ifdef JPEG6B_DEBUG
	  	 HI_JPEG_TRACE("The pstAppMessage pointer is NULL\n");
         #endif
		 return HI_FAILURE;
	  }
     
      JPEG_MESSAGE_S  *pstMessagePrivate;
      j_common_ptr pCinfo = (j_common_ptr)cinfo;
      pstMessagePrivate = (JPEG_MESSAGE_S *)(pCinfo->client_data);

	 HI_S32 i;
	 for(i=0;i<NEED_ADDR_BLOCK_NUM;i++)
	 {
	      pstAppMessage->u32OutPhyAddr[i] = pstMessagePrivate->stPub.u32OutPhyAddr[i];
	      pstAppMessage->pu32OutVirAddr[i] = pstMessagePrivate->stPub.pu32OutVirAddr[i];
	      pstAppMessage->s32OutStride[i] = pstMessagePrivate->stPub.s32OutStride[i];
	 }
  
      return HI_SUCCESS;
 

}


/*****************************************************************************
* func                 : if you want to suspend, you should call this function
                         first, if you have not call this function, you should
                         not suspending.
* param[in] :
* param[in] :
* retval    :HI_SUCCESS: success
* retval    :HI_FAILURE: failure
* others:	:nothing
*****************************************************************************/

HI_S32 HI_JPEG_BeginSuspend()
{

        HI_S32 s32RetVal = 0;
        /**if you want to suspend, you shoulde waite for decoding finish.**/
        s_s32JpegDevice = open(JPG_DEV, O_RDWR | O_SYNC);
        if (s_s32JpegDevice < 0) {
             #ifdef JPEG6B_DEBUG
             HI_JPEG_TRACE("open jpeg device failed!\n");
             #endif
        }
        else{
                           
            s32RetVal = ioctl(s_s32JpegDevice, CMD_JPG_SUSPEND);
            if (0 > s32RetVal){    
                #ifdef JPEG6B_DEBUG
                HI_JPEG_TRACE("suspend failed.");
                #endif
                return HI_FAILURE;
            } 
        }
        return HI_SUCCESS;        
}


/*****************************************************************************
* func                     : HI_JPEG_EndSuspend
* param[in] :
* param[in] :
* retval    :HI_SUCCESS    : success
* retval    :HI_FAILURE    : failure
* others:	:nothing
*****************************************************************************/

HI_S32 HI_JPEG_EndSuspend()
{

      HI_S32 s32RetVal = 1;
      /**if you want to suspend, you shoulde waite for decoding finish.**/
      if (s_s32JpegDevice < 0) {
             #ifdef JPEG6B_DEBUG
             HI_JPEG_TRACE("not have open jpeg!\n");
             #endif
      }
      else{        
          s32RetVal = ioctl(s_s32JpegDevice, CMD_JPG_RESUME);
          if (0 > s32RetVal){ 
              #ifdef JPEG6B_DEBUG 
              HI_JPEG_TRACE("resume failed.");
              #endif 
              return HI_FAILURE;
          }   
          close(s_s32JpegDevice); 
      }
      return HI_SUCCESS;
	  
}

/*****************************************************************************
* func                     : HI_JPEG_ForceDecodeType
*                          if users only use soft or hard decode, user can
                           call this function.
* param[in] :cinfo         : decompress object
* param[in] :SoftOrHard    : ture = use soft decode
                             false = use hard decode
* retval    :HI_SUCCESS    : success
* retval    :HI_FAILURE    : failure
* others:	:nothing
*****************************************************************************/

HI_S32 HI_JPEG_ForceDecodeType(j_decompress_ptr cinfo,HI_BOOL SoftOrHard)
{

      /** because it called after set mem mode, so it should not call again **/
	  if(NULL==cinfo){
        #ifdef JPEG6B_DEBUG
	  	HI_JPEG_TRACE("The cinfo is null pointer\n");
        #endif
	  	return HI_FAILURE;
      }
      
      JPEG_MESSAGE_S  *pstMessagePrivate;
      j_common_ptr pCinfo = (j_common_ptr)cinfo;
      pstMessagePrivate = (JPEG_MESSAGE_S *)(pCinfo->client_data);
	 if(HI_FALSE == JPEG_Api_CheckAddStructMessage(pCinfo,pstMessagePrivate))
     {  
        #ifdef JPEG6B_DEBUG
        HI_JPEG_TRACE("you may call this function before create_decompress: \
			           so you should call it after create_decompress!\n");
	    #endif
        return HI_FAILURE;
     }
	 pstMessagePrivate->HaveCallForceDecodeType = HI_TRUE;
	 if(HI_FALSE == SoftOrHard)
	 {
        pstMessagePrivate->UseSoftDecode = HI_FALSE;
	 }
	 else
	 {
	     pstMessagePrivate->UseSoftDecode = HI_TRUE;
	 }
	  
	  return HI_SUCCESS;
}

/*****************************************************************************
* func                     : Get the format of the image
* param[in] :cinfo         : decompress object
* retval    :TRUE          : success
* retval    :FALSE         : failure
* others:	:nothing
*****************************************************************************/

JPEG_IMAGEFORMAT_E JPEG_Api_GetImagFormat(j_decompress_ptr cinfo)
{

          JPEG_IMAGEFORMAT_E enImageFmt = ~0;
          
          if (cinfo->num_components==1)
          {
          
               if (cinfo->comp_info[0].h_samp_factor==cinfo->comp_info[0].v_samp_factor)
               {
                     enImageFmt = JPEG_YUV_400;
               }
               
          }
          else if ((cinfo->num_components==3) && (cinfo->comp_info[1].h_samp_factor==cinfo->comp_info[2].h_samp_factor) && (cinfo->comp_info[1].v_samp_factor==cinfo->comp_info[2].v_samp_factor))
          {
          
                if (cinfo->comp_info[0].h_samp_factor==((cinfo->comp_info[1].h_samp_factor)<<1))
                {

                     if (cinfo->comp_info[0].v_samp_factor==((cinfo->comp_info[1].v_samp_factor)<<1))
                     {

                         enImageFmt = JPEG_YUV_420;

                     }
                     else if (cinfo->comp_info[0].v_samp_factor==cinfo->comp_info[1].v_samp_factor)
                     {

                          enImageFmt = JPEG_YUV_422_21;

                     }
                     
                 }
                 else if (cinfo->comp_info[0].h_samp_factor==cinfo->comp_info[1].h_samp_factor)
                 {
               
                       if (cinfo->comp_info[0].v_samp_factor==((cinfo->comp_info[1].v_samp_factor)<<1))
                       {

                              enImageFmt = JPEG_YUV_422_12;;
                              
                       }
                       else if (cinfo->comp_info[0].v_samp_factor==cinfo->comp_info[1].v_samp_factor)
                       {


                               enImageFmt = JPEG_YUV_444;
                              
                       }
                  }
                  
          }

         return enImageFmt;



}


/*****************************************************************************
* func                     : initial the HI_JPEG_APP_MESSAGE_S struct at
                             create_decompress().
* param[in] :cinfo         : 
* param[in] :
* retval    :TRUE          : success
* retval    :FALSE         : failure
* others:	:nothing
*****************************************************************************/
HI_BOOL JPEG_Api_InitMessageStruct(j_common_ptr cinfo)
{


        JPEG_MESSAGE_S  *pAppMessagePrivate;
		
        //#ifndef ANDROID_JPEG6B
        #if 0
		/**
		 ** 使用6B中的内存管理会有点问题，问题描述如底下描述
		 ** 问题单号:HSCP201212109997
		 ** 3716C TC方案：长时间全屏播放在线视频会出现虚拟机自动断开现象
		 **/
        pAppMessagePrivate = (JPEG_MESSAGE_S*)
          (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_IMAGE,
    				  SIZEOF(JPEG_MESSAGE_S));
        #else
		/** revise at 2011-11-4
		 ** the problem at android,if used alloc_small malloc the mem of pAppMessagePrivate struct,
		 ** its mem will free before jpeg_mem_term, and you have allways use pAppMessagePrivate at 
		 ** jpeg_mem_term func, so the content of pAppMessagePrivate will be revised by other pthread.
		 **/
        pAppMessagePrivate = (JPEG_MESSAGE_S*)jpeg_get_small(cinfo, SIZEOF(JPEG_MESSAGE_S));
        #endif
	   
		if(pAppMessagePrivate!=NULL)
	    {
	        cinfo->client_data = (void *)pAppMessagePrivate;

	        pAppMessagePrivate->s32ClientData = CLIENT_DATA_MARK;
	        pAppMessagePrivate->IfHaveCallSetMemMode = HI_FALSE;
			pAppMessagePrivate->SetModeOutBuffer = NULL;


            int i=0;
			for(i=0;i<NEED_ADDR_BLOCK_NUM;i++)
		    {
		   
				pAppMessagePrivate->stPub.u32OutPhyAddr[i] = 0;
				pAppMessagePrivate->stPub.pu32OutVirAddr[i] = NULL;

				pAppMessagePrivate->stPub.s32OutStride[i] = 0;
				
		    }

			/** used at hard decode **/
			/** default hardwar support **/
			pAppMessagePrivate->hardware_support = HI_TRUE;
			pAppMessagePrivate->HaveDoneSetPara = HI_FALSE;
			pAppMessagePrivate->HaveDoneSentStream = HI_FALSE;
			pAppMessagePrivate->HaveDoneTDE = HI_FALSE;

            pAppMessagePrivate->u32InImageWandH = 0;
            pAppMessagePrivate->s32InYStride = 0;
            pAppMessagePrivate->s32InCStride = 0;
            pAppMessagePrivate->s32YMemSize = 0;
            pAppMessagePrivate->s32CMemSize = 0;
            pAppMessagePrivate->s32RGBMemSize = 0;
		    pAppMessagePrivate->mmz_PhyAddr = NULL;
			pAppMessagePrivate->mmz_VirAddr = NULL;
			pAppMessagePrivate->reg_virAddr = NULL;
			
            pAppMessagePrivate->mmz = 0;
			pAppMessagePrivate->jpg = 0;
			pAppMessagePrivate->openTDE = HI_FALSE;

			/** used at error_exit(), if error happen, the value is true **/
			pAppMessagePrivate->ErrMsg = FALSE;

			/** check whether has called start_decompress
			 ** if not called, when return soft decode,
			 ** should call it
			 **/
			pAppMessagePrivate->HaveCallStartDec = FALSE;

			/** this is global state before call start_decompress,
             ** if hard decode failure, we should retrun the state
			 **/
			pAppMessagePrivate->s32BeforeStartState = 0;

		    pAppMessagePrivate->UseSoftDecode = HI_FALSE;
			pAppMessagePrivate->HaveCallForceDecodeType = HI_FALSE;

			pAppMessagePrivate->s32ByteInBuffer = 0;

            /**================= add function var ===========================**/
			 /** 默认没有码流 **/
			pAppMessagePrivate->sJpegHtoSInfo.eReadStreamType = STREAM_TYPE_BUTT;
			pAppMessagePrivate->sJpegHtoSInfo.s32FilePos   = 0;
			pAppMessagePrivate->sJpegHtoSInfo.s32StreamPos = 0;
			pAppMessagePrivate->sJpegHtoSInfo.s32LeaveByte = 0;
			pAppMessagePrivate->bCannotSupport = HI_FALSE;
			#if 0
		    pAppMessagePrivate->sJpegHtoSInfo.pLeaveBuf = (HI_CHAR*)calloc(1,INPUT_BUF_SIZE);
			#else
            pAppMessagePrivate->sJpegHtoSInfo.pLeaveBuf = (char *)
	                  cinfo->mem->alloc_small ((j_common_ptr) cinfo, JPOOL_PERMANENT,
	                                           INPUT_BUF_SIZE * sizeof (JOCTET));
			#endif
			if(NULL==pAppMessagePrivate->sJpegHtoSInfo.pLeaveBuf)
		    {
				#ifdef JPEG6B_DEBUG
	            HI_JPEG_TRACE("malloc pstMessagePrivate->pLeaveBuf failure!\n");
	            #endif
				return HI_FALSE;
		    }
			/** 默认解成功 **/
			pAppMessagePrivate->sJpegHtoSInfo.bHDECSuc = HI_TRUE;
	   
	    }
		else
	    {   
	        #ifdef JPEG6B_DEBUG
            HI_JPEG_TRACE("JPEG_Api_InitMessageStruct failure!\n");
            #endif
			return HI_FALSE;
	    }
              
        /** when malloc mem failure at sorf decode, pthread will be killed, so
         ** we want to use MMZ malloc. we should open at creat decompress
         **/

		/**
		 **  O_RDWR|O_SYNC  "O_SYNC"打开的时候时间会变的很慢
		 ** 也就是说打开文件后，系统不会自动更新文件写的位置，对
		 ** 于每个“文件状态标志”和“当前文件偏移量” 不会更新。
         ** 对于多进程的时候要注意,可能需要打开不然会异常看情况,
		 **/
        if((pAppMessagePrivate->mmz)<=0){
		    pAppMessagePrivate->mmz = open(MMZ_DEV, O_RDWR); 
            if (pAppMessagePrivate->mmz <= 0){
				#ifdef JPEG6B_DEBUG
                HI_JPEG_TRACE("open mmz device failured!\n");
				#endif
                return HI_FALSE;
           }

        }
        
            #ifdef JPEG6B_DEBUG
            HI_JPEG_TRACE("======JPEG_Api_InitMessageStruct %p, %p===============\n",\
                                                       pAppMessagePrivate, cinfo->client_data);
            HI_JPEG_TRACE("s32ClientData = 0x%x\n",pAppMessagePrivate->s32ClientData);
            #endif
		return HI_TRUE;


}


/*****************************************************************************
* func                     : dinitial the HI_JPEG_APP_MESSAGE_S struct at
                             self_struct().
* param[in] :cinfo         : 
* param[in] :
* retval    :TRUE          : success
* retval    :FALSE         : failure
* others:	:nothing
*****************************************************************************/
HI_BOOL JPEG_Api_DinitMessageStruct(j_common_ptr cinfo)
{

        JPEG_MESSAGE_S  *pstMessagePrivate;
        pstMessagePrivate = (JPEG_MESSAGE_S *)(cinfo->client_data);
        pstMessagePrivate->s32ClientData = 0;

		#if 0
        if(NULL!=pstMessagePrivate->sJpegHtoSInfo.pLeaveBuf)
        {
		   free(pstMessagePrivate->sJpegHtoSInfo.pLeaveBuf);
		   pstMessagePrivate->sJpegHtoSInfo.pLeaveBuf = NULL;
        }
		#endif
		
        //#ifdef ANDROID_JPEG6B
        /**
         **这个要和初始化中对应
         **/
		/**
		 ** this is corresponding the malloc the mem of pstMessagePrivate struct
		 ** at JPEG_Api_InitMessageStruct function
		 **/
		if(pstMessagePrivate != NULL)
        {           
            jpeg_free_small(cinfo, pstMessagePrivate, SIZEOF(JPEG_MESSAGE_S));
            pstMessagePrivate = NULL;
        }
        //#endif
		
    	return HI_TRUE;
		

}

/*****************************************************************************
* func                     : JPEG_Api_CheckAddStructMessage
*           : check the added private struct message,check client data whether
*             is my data and cinfo whether is NULL and other message 
* param[in] :cinfo         : 
* retval    :TRUE          : success
* retval    :FALSE         : failure
* others:	:nothing
*****************************************************************************/
HI_BOOL JPEG_Api_CheckAddStructMessage(j_common_ptr cinfo,JPEG_MESSAGE_S  *pstMessagePrivate)
{

        if (cinfo==NULL){
           #ifdef JPEG6B_DEBUG
           HI_JPEG_TRACE("cinfo is NULL!\n");
           #endif
    	   return HI_FALSE;
    	}
    	else if(NULL==pstMessagePrivate){
           #ifdef JPEG6B_DEBUG
    	   HI_JPEG_TRACE("pstMessagePrivate is NULL!\n");
           #endif
    	   return HI_FALSE;
    	}
    	else if(CLIENT_DATA_MARK != pstMessagePrivate->s32ClientData){
           #ifdef JPEG6B_DEBUG
           HI_JPEG_TRACE("== JPEG_Api_CheckAddStructMessage  %p, %p ===============\n",\
                                                   pstMessagePrivate, cinfo->client_data);
    	   HI_JPEG_TRACE("s32ClientData = 0x%x  != 0xFFFFFFFF\n",pstMessagePrivate->s32ClientData);
           #endif
    	   return HI_FALSE; 	
    	}
        if(TRUE == pstMessagePrivate->ErrMsg){
	       return HI_FALSE;
	    }
    	return HI_TRUE;

}


/*****************************************************************************
* func        : hi_get_dec_state
* description : 获取硬件解码状态,这个是接口是在用户自己处理码流的时候使用,
                并且实在调用解码之后调用,从而决定是否需要重新调用一次解码,
                HI_FALSE才需要重新调用一次.
* param[in]   : 
* param[in]   :
* param[in]   :
* others:	  : nothing
*****************************************************************************/
HI_VOID hi_get_dec_state(HI_BOOL *bHDEC,j_decompress_ptr cinfo)
{

       JPEG_MESSAGE_S  *pstMessagePrivate;
	   //j_common_ptr pCinfo = (j_common_ptr)cinfo;
	   pstMessagePrivate = (JPEG_MESSAGE_S *)(cinfo->client_data);

	   *bHDEC =  pstMessagePrivate->sJpegHtoSInfo.bHDECSuc;

}
	
#ifdef USE_EXTERN_STREAM

/*****************************************************************************
* func        : hi_set_read_stream_type
* description : 设置读码流方式,假如不使用标准的,在调用jpeg_start_decompress之前
                调用该接口
* param[in]   : 
* param[in]   :
* param[in]   :
* others:	  : nothing
*****************************************************************************/
HI_VOID hi_set_read_stream_type(const HI_JPEG_HDEC_TO_SDEC_S stInfo,\
                                            j_decompress_ptr cinfo)
{

	   JPEG_MESSAGE_S  *pstMessagePrivate;
	   //j_common_ptr pCinfo = (j_common_ptr)cinfo;
	   pstMessagePrivate = (JPEG_MESSAGE_S *)(cinfo->client_data);

	   pstMessagePrivate->sJpegHtoSInfo.eReadStreamType = stInfo.eReadStreamType;
		
}
#endif

/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon. Co., Ltd.

******************************************************************************
File Name	    : hi_jpeg_api.h
Version		    : Initial Draft
Author		    : 
Created		    : 2011/05/12
Description	    : JPEG6B application interface
Function List 	: HI_JPEG_SetMemMode
			    : HI_JPEG_GetMemMode
			    : HI_JPEG_BeginSuspend
			    : HI_JPEG_EndSuspend

			  		  
History       	:
Date				Author        		Modification
2011/05/11		            		    Created file      	
******************************************************************************/

#ifndef __HI_JPEG_API_H__
#define __HI_JPEG_API_H__





/*********************************add include here******************************/



#include "jpeglib.h"
#include "hi_type.h"


/*****************************************************************************/


#ifdef __cplusplus
      #if __cplusplus
   
extern "C" 
{

      #endif
#endif /* __cplusplus */

    /***************************** Macro Definition ******************************/

    #define NEED_ADDR_BLOCK_NUM   2
    #define NEED_STRIDE_BLOCK_NUM   2

    /*************************** Structure Definition ****************************/

    /**
	 ** 这个保留，就是应用层自己封装读码流方式 
	 ** 两种办法，适配的时候让应用层使用标准的读码流接口
	 ** 第二种: 外部处理码流参考内部处理码流，如何备份码流信息及恢复
	 ** 因为应用层读码流的字节数不定，而内部只分配4096个字节，所以要注意
	 **/
	#if 1
	#define USE_EXTERN_STREAM
    #endif
	
    /***************************  The enum of Jpeg image format  ******************/

	typedef struct hiJPEG_APP_MESSAGE_S
	{

         /** u32OutPhyAddr[0]is RGB or YCbCr physical address
          **/
         HI_U32 u32OutPhyAddr[NEED_ADDR_BLOCK_NUM];
		 HI_U32 *pu32OutVirAddr[NEED_ADDR_BLOCK_NUM];/** extend use **/
		 
         
         /** s32OutStride[0]is RGB or Y stride, and s32OutStride[1] is c stride **/
         HI_S32 s32OutStride[NEED_STRIDE_BLOCK_NUM];

	}HI_JPEG_APP_MESSAGE_S;

    /** 码流类型 **/
    typedef enum hiJPEG_STREAM_TYPE_E
    {

        STREAM_TYPE_INTER_FILE    = 0,   /** 使用标准读文件接口 **/
        STREAM_TYPE_INTER_STREAM  = 1,   /** 使用标准读码流 **/
        #ifdef USE_EXTERN_STREAM
        STREAM_TYPE_EXTERN_FILE   = 2,   /** 使用外部读文件接口 **/
        STREAM_TYPE_EXTERN_STREAM = 3,   /** 使用外部读码流 **/
        #endif
        STREAM_TYPE_BUTT
        
    }HI_JPEG_STREAM_TYPE_E;

	/** 码流信息状态 **/
	typedef struct hiJPEG_HDEC_TO_SDEC_S
	{
	
     	 /** ====================硬解不成功回退到软解的过程=================== **/
		 /** 判断是读文件还是码流 **/
		 HI_JPEG_STREAM_TYPE_E eReadStreamType;
		 /** 当前读到的文件位置 **/
		 HI_S32 s32FilePos;
		 /** 当前码流的位置 **/
		 HI_S32 s32StreamPos;
		  /** 剩余的码流数  **/
		 HI_S32 s32LeaveByte;
		 /** 剩余码流 **/
		 HI_CHAR* pLeaveBuf;
         /** 硬解是否成功 **/
		 HI_BOOL bHDECSuc;

	}HI_JPEG_HDEC_TO_SDEC_S,*HI_JPEG_HDEC_TO_SDEC_S_PTR;
	
    /********************** Global Variable declaration **************************/



    /******************************* API declaration *****************************/

	/*****************************************************************************
	* func                     : HI_JPEG_GetVersion
	* description:
	* param[in] :ppVersion
	* param[in] :ppBuildTime
	* retval    :HI_SUCCESS    : success
	* retval    :HI_FAILURE    : failure
	* others:	:nothing
	*****************************************************************************/
	HI_S32 HI_JPEG_GetVersion(HI_CHAR **ppVersion, HI_CHAR **ppBuildTime);

    /*****************************************************************************
    * func                     : set the input memory mode
    * description: if users did not call this function, default use virtual address.
    *              and if users want use physical address mode, should call this function.
    * param[in] :cinfo         : decompress object
    * param[in] :pstAppMessage : HI_JPEG_APP_MESSAGE_S struct
    * param[in] :scanlines
    * retval    :HI_SUCCESS    : success
    * retval    :HI_FAILURE    : failure
    * others:	:nothing
    *****************************************************************************/

    HI_S32 HI_JPEG_SetMemMode(j_decompress_ptr cinfo,HI_JPEG_APP_MESSAGE_S pstAppMessage);





    /*****************************************************************************
    * func                     : get the input memory mode
    * param[in] :cinfo         : decompress object
    * param[in] :pstAppMessage : 
    * retval    :HI_SUCCESS    : success
    * retval    :HI_FAILURE    : failure
    * others:	:nothing
    *****************************************************************************/

    HI_S32 HI_JPEG_GetMemMode(j_decompress_ptr cinfo,HI_JPEG_APP_MESSAGE_S *pstAppMessage);

    /*****************************************************************************
    * func                     : HI_JPEG_BeginSuspend
    * param[in] : 
    * param[in] :
    * retval    :HI_SUCCESS    : success
    * retval    :HI_FAILURE    : failure
    * others:	:nothing
    *****************************************************************************/

    HI_S32 HI_JPEG_BeginSuspend();

    /*****************************************************************************
    * func                     : HI_JPEG_End_Suspend
    * param[in] :
    * retval    :HI_SUCCESS    : success
    * retval    :HI_FAILURE    : failure
    * others:	:nothing
    *****************************************************************************/

    HI_S32 HI_JPEG_EndSuspend();

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
    HI_S32 HI_JPEG_ForceDecodeType(j_decompress_ptr cinfo,HI_BOOL SoftOrHard);


	/*****************************************************************************
	* func        : hi_get_dec_state
	* description : 获取硬件解码状态,这个是接口是在用户自己处理码流的时候使用,
	                并且实在调用解码之后调用,从而决定是否需要重新调用一次解码.
	                HI_FALSE才需要重新调用一次.
	* param[in]   : 
	* param[in]   :
	* param[in]   :
	* others:	  : nothing
	*****************************************************************************/
	HI_VOID hi_get_dec_state(HI_BOOL *bHDEC,j_decompress_ptr cinfo);

	#ifdef USE_EXTERN_STREAM

   /*****************************************************************************
	* func        : hi_set_read_stream_type
	* description : 设置读码流方式,假如不使用标准的,在调用jpeg_start_decompress之前
	                调用该接口,要是没有调用该接口且外部读码流方式默认硬解失败就不解了
	* param[in]   : 
	* param[in]   :
	* param[in]   :
	* others:	  : nothing
	*****************************************************************************/
	HI_VOID hi_set_read_stream_type(const HI_JPEG_HDEC_TO_SDEC_S stInfo,\
	                                            j_decompress_ptr cinfo);

    #endif
    
    /****************************************************************************/
    #ifdef __cplusplus
    
        #if __cplusplus


      
}
      
        #endif
        
   #endif /* __cplusplus */

#endif /* __HI_JPEG_API_H__*/

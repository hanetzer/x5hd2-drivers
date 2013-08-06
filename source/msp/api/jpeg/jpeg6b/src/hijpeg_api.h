/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon. Co., Ltd.

******************************************************************************
File Name	    : hijpeg_api.h
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

#ifndef __HIJPEG_API_H__
#define __HIJPEG_API_H__





/*********************************add include here******************************/

#include "hi_jpeg_api.h"
#include "hijpeg_type.h"


/*****************************************************************************/


#ifdef __cplusplus
#if __cplusplus
   extern "C" 
{
#endif
#endif /* __cplusplus */


    /***************************** Macro Definition ******************************/

    #define CLIENT_DATA_MARK 0xFFFFFFFF

    #ifdef ANDROID_JPEG6B
    #define HI_JPEG_TRACE( fmt, args... )\
    do { \
            LOGE( "%s\n %s(): %d Line\n: ", __FILE__,  __FUNCTION__,  __LINE__ );\
               LOGE( fmt, ##args );\
    } while (0)
    #else
	#define HI_JPEG_TRACE( fmt, args... )\
    do { \
            printf( "%s\n %s(): %d Line\n: ", __FILE__,  __FUNCTION__,  __LINE__ );\
               printf( fmt, ##args );\
    } while (0)
    #endif
    /*************************** Structure Definition ****************************/


	typedef struct tagJPEG_MESSAGE_S
	{

         /** this is used at output **/
         HI_JPEG_APP_MESSAGE_S stPub;
		 
         HI_S32 s32ClientData;
         HI_BOOL IfHaveCallSetMemMode;        /** if not call this fun, default virtual
                                               ** buffer output and use scanlines buffer
                                               **/
         JSAMPARRAY SetModeOutBuffer;  /** if setmode and soft decode, the scanlines
                                           equal the SetModeOutBuffer that I malloc,
                                           at last copy to the u32OutVirBuffer that 
                                           users mmz
                                       **/
                                           
         /** followed para only used at hijpeg_decode_hw.c and initial at api.c**/
          HI_BOOL    hardware_support; 
	      HI_BOOL    HaveDoneSetPara;							   
          HI_BOOL    HaveDoneSentStream;
		  HI_BOOL    HaveDoneTDE;
		  /** used at allocate middle buffer **/
		  HI_U32 u32InImageWandH;
		  HI_S32 s32InYStride;
		  HI_S32 s32InCStride;
		  HI_S32 s32YMemSize;
		  HI_S32 s32CMemSize;
		  HI_S32 s32RGBMemSize;
		  char *mmz_PhyAddr;
          char *mmz_VirAddr;
          volatile char * reg_virAddr;
      
          /** device **/
          HI_S32    mmz;                      
          HI_S32    jpg;                      
          HI_BOOL   openTDE;                  

          /** used at error_exit()**/
          boolean ErrMsg;

        /** check whether has called start_decompress
		 ** if not called, when return soft decode,
		 ** should call it
		 **/
          boolean HaveCallStartDec;

         /** this is global state before call start_decompress,
           ** if hard decode failure,we should return the state
		   **/
          HI_S32 s32BeforeStartState;

		 /** if call forcesoft function, this para is TRUE, default is FALSE**/
		 HI_BOOL UseSoftDecode;
         HI_BOOL HaveCallForceDecodeType;

		 /** check whether read stream finish, because there are different between
		  ** androi and linux for the fill_input_buffer function.
		  **/
		 HI_S32 s32ByteInBuffer;

		 /** ====================硬解不成功回退到软解的过程=================== **/
	     /**如下文件用到,
	      ** hijpeg_decode_hw.c hi_jpeg_api.c jdatasrc.c jdapistd.c
	      **/
		 HI_JPEG_HDEC_TO_SDEC_S sJpegHtoSInfo;
		 /** 不支持解某种格式的图片 **/
		 HI_BOOL bCannotSupport;

	}JPEG_MESSAGE_S;
	

    /********************** Global Variable declaration **************************/



    /******************************* API declaration *****************************/


    /*****************************************************************************
    * func                     : Set the inflexion by imagesize to choose is hard decode
                                 or software decode
    * param[in] :s32ImageSize  : the size of image
    * retval    :TRUE          : success
    * retval    :FALSE         : failure
    * others:	:nothing
    *****************************************************************************/

    HI_BOOL JPEG_SetInflexion(HI_U32 u32JpegInflexionSize);

    /*****************************************************************************
    * func                     : Get the format of the image
    * param[in] :cinfo         : decompress object
    * retval    :TRUE          : success
    * retval    :FALSE         : failure
    * others:	:nothing
    *****************************************************************************/

    JPEG_IMAGEFORMAT_E JPEG_Api_GetImagFormat(j_decompress_ptr cinfo);


   /*****************************************************************************
    * func                     : initial the HI_JPEG_APP_MESSAGE_S struct at
                                 create_decompress()
    * param[in] :cinfo         : 
    * param[in] :
    * retval    :TRUE          : success
    * retval    :FALSE         : failure
    * others:	:nothing
    *****************************************************************************/
    HI_BOOL JPEG_Api_InitMessageStruct(j_common_ptr cinfo);
   
   /*****************************************************************************
    * func                     : dinitial the HI_JPEG_APP_MESSAGE_S struct at
                                 destroy_decompress()
    * param[in] :cinfo         : 
    * param[in] :
    * retval    :TRUE          : success
    * retval    :FALSE         : failure
    * others:	:nothing
    *****************************************************************************/
    HI_BOOL JPEG_Api_DinitMessageStruct(j_common_ptr cinfo);

   /*****************************************************************************
    * func                     : JPEG_Api_CheckAddStructMessage
    *           : check the added private struct message,check client data whether
    *             is my data and cinfo whether is NULL and other message 
    * param[in] :cinfo         : 
    * retval    :TRUE          : success
    * retval    :FALSE         : failure
    * others:	:nothing
    *****************************************************************************/
    HI_BOOL JPEG_Api_CheckAddStructMessage(j_common_ptr cinfo,JPEG_MESSAGE_S  *pstMessagePrivate);

    /****************************************************************************/



#ifdef __cplusplus
    
#if __cplusplus
   
}
#endif
#endif /* __cplusplus */

#endif /* __HIJPEG_API_H__*/

/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon. Co., Ltd.

******************************************************************************
File Name	    : hijpeg_type.h
Version		    : Initial Draft
Author		    : y00181162
Created		    : 2012/10/31
Description	    : this is used by user and kernel, and it only include hi_type.h
Function List 	: 
			    : 

History       	:
Date				Author        		Modification
2012/10/31		    y00181162		    Created file      	
******************************************************************************/

#ifndef __HIJPEG_TYPE_H__
#define __HIJPEG_TYPE_H__



/*********************************add include here******************************/

#include "hi_type.h"

/*****************************************************************************/


#ifdef __cplusplus
#if __cplusplus
   extern "C" 
{
#endif
#endif /* __cplusplus */



    /***************************** Macro Definition ******************************/


    /*************************** Structure Definition ****************************/

    /**
     **  The enum of Jpeg image format  
     **/

    /**
     ** according the hardware register 
     **/
    typedef enum tagJPEG_IMAGEFORMAT_E
    {

        JPEG_YUV_400      = 0,
        JPEG_YUV_BUTT1    = 1,
        JPEG_YUV_BUTT2    = 2,
        JPEG_YUV_420      = 3,
        JPEG_YUV_422_21   = 4,
        JPEG_YUV_422_12   = 5,
        JPEG_YUV_444      = 6,
        
        JPEG_YUV_BUTT

        
    }JPEG_IMAGEFORMAT_E;


    /**
     **  The enum of decode state  
     **/
     
    typedef enum tagJPEG_DECODESTATE_E
    {
    
        JPEG_STATE_NOSTART = 0x0,  /** didn't start decode **/
        JPEG_STATE_DECING,         /*  decoding            **/
        JPEG_STATE_FINISH,         /** finish decompress   **/
        JPEG_STATE_ERR,            /** decode error        **/
        JPEG_STATE_BUTT
        
    }JPEG_DECODESTATE_E;


    typedef enum tagJPEG_DECODETYPE_E
    {
    
        JPEG_DECODETYPE_HW = 0,    /** hard decode **/
        JPEG_DECODETYPE_SW = 1,    /** soft decode **/
        
        JPEG_DECODETYPE_BUTT
        
    }JPEG_DECODETYPE_E;
    

    /**
     ** The struct of proc message  
     **/
    typedef struct tagJPEG_PROC_INFO_S
    {
    
            HI_U32 u32ImageWidth;                  /** the open image width         **/
            HI_U32 u32ImageHeight;                 /** the open image height        **/
            HI_U32 u32OutputWidth;                 /** the output image width       **/
            HI_U32 u32OutputHeight;                /** the output image height      **/
            
            JPEG_IMAGEFORMAT_E eImageFormat;       /** the image format              **/

            HI_U32  u32OutputBufAddr;              /** output buffer address         **/
			HI_BOOL OutPutPhyBuf;                  /** the output buf information    **/
			HI_U32  u32OutputStreamBufAddr;        /** output stream buffer address  **/

            JPEG_DECODESTATE_E eDecodeState;       /** the decode state              **/

            JPEG_DECODETYPE_E eDecodeType;         /** the decode types              **/
            
            HI_S32 s32ImageScale;                  /** the image scale               **/
            
            HI_S32 s32OutPutComponents;            /** the output components         **/

 	        HI_U32 eOutputColorspace;              /** the output color space        **/

    }JPEG_PROC_INFO_S;


    /********************** Global Variable declaration **************************/



    /******************************* API declaration *****************************/



    /****************************************************************************/

#ifdef __cplusplus
#if __cplusplus   
}
      
#endif
#endif /* __cplusplus */

#endif /* __HIJPEG_TYPE_H__ */

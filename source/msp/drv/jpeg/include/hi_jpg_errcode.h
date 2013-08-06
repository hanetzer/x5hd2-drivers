/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon. Co., Ltd.

******************************************************************************
File Name	    : hi_jpegdrv_api.h
Version		    : Initial Draft
Author		    : 
Created		    : 2012/10/31
Description	    : 
Function List 	: 

			  		  
History       	:
Date				Author        		Modification
2012/10/31		    y00181162        		    Created file      	
******************************************************************************/



#ifndef __HI_JPG_ERRCODE_H__
#define __HI_JPG_ERRCODE_H__

#include "hi_type.h"



#ifdef __cplusplus
      #if __cplusplus
extern "C"{
      #endif
#endif /* __cplusplus */


    /***************************** Macro Definition ******************************/

    /*=============================================================================
      |----------------------------------------------------------------| 
      | 1 |   APP_ID   |   MOD_ID    | ERR_LEVEL |   ERR_ID            | 
      |----------------------------------------------------------------| 
      |<--><--7bits----><----8bits---><--3bits---><------13bits------->|
    =============================================================================*/
    #define HI_ERR_APPID  (0x80UL + 0x20UL)

    #define HI_DEF_ERR( mid, level, errid) \
        ((HI_S32)( ((HI_ERR_APPID)<<24) | ((mid) << 16 ) | ((level)<<13) | (errid) ))


    /**
     ** JPG error code define
     **/

    #define MID_JPG 0x25        /** JPG module ID **/



    #define HI_ERR_JPG_PTR_NULL\
    HI_DEF_ERR(MID_JPG, HI_LOG_LEVEL_ERROR, ERR_JPG_PTR_NULL)

    #define HI_ERR_JPG_DEV_OPENED\
    HI_DEF_ERR(MID_JPG, HI_LOG_LEVEL_ERROR, ERR_JPG_DEV_OPENED)

    #define HI_ERR_JPG_DEV_NOOPEN\
    HI_DEF_ERR(MID_JPG, HI_LOG_LEVEL_ERROR, ERR_JPG_DEV_NOOPEN)

    #define HI_ERR_JPG_INVALID_PARA\
    HI_DEF_ERR(MID_JPG, HI_LOG_LEVEL_ERROR, ERR_JPG_INVALID_PARA)

    #define HI_ERR_JPG_INVALID_FILE\
    HI_DEF_ERR(MID_JPG, HI_LOG_LEVEL_ERROR, ERR_JPG_INVALID_FILE)

    #define HI_ERR_JPG_NO_MEM\
    HI_DEF_ERR(MID_JPG, HI_LOG_LEVEL_ERROR, ERR_JPG_NO_MEM)

    #define HI_ERR_JPG_INVALID_SOURCE\
    HI_DEF_ERR(MID_JPG, HI_LOG_LEVEL_ERROR, ERR_JPG_INVALID_SOURCE)

    #define HI_ERR_JPG_TIME_OUT\
    HI_DEF_ERR(MID_JPG, HI_LOG_LEVEL_ERROR, ERR_JPG_TIME_OUT)

    #define HI_ERR_JPG_INVALID_HANDLE\
    HI_DEF_ERR(MID_JPG, HI_LOG_LEVEL_ERROR, ERR_JPG_INVALID_HANDLE)

    #define HI_ERR_JPG_EXIST_INSTANCE\
    HI_DEF_ERR(MID_JPG, HI_LOG_LEVEL_ERROR, ERR_JPG_EXIST_INSTANCE)

    #define HI_ERR_JPG_THUMB_NOEXIST\
    HI_DEF_ERR(MID_JPG, HI_LOG_LEVEL_ERROR, ERR_JPG_THUMB_NOEXIST)

    #define HI_ERR_JPG_NO_TASK\
    HI_DEF_ERR(MID_JPG, HI_LOG_LEVEL_ERROR, ERR_JPG_NO_TASK)

    #define HI_ERR_JPG_NOSUPPORT_FMT\
    HI_DEF_ERR(MID_JPG, HI_LOG_LEVEL_ERROR, ERR_JPG_NOSUPPORT_FMT)

    #define HI_ERR_JPG_DEC_BUSY\
    HI_DEF_ERR(MID_JPG, HI_LOG_LEVEL_ERROR, ERR_JPG_DEC_BUSY)

    #define HI_ERR_JPG_DEC_PARSING\
    HI_DEF_ERR(MID_JPG, HI_LOG_LEVEL_ERROR, ERR_JPG_DEC_PARSING)

    #define HI_ERR_JPG_DEC_DECODING\
    HI_DEF_ERR(MID_JPG, HI_LOG_LEVEL_ERROR, ERR_JPG_DEC_DECODING)    

    #define HI_ERR_JPG_WANT_STREAM\
    HI_DEF_ERR(MID_JPG, HI_LOG_LEVEL_ERROR, ERR_JPG_WANT_STREAM)   

    #define HI_ERR_JPG_DEC_FAIL\
    HI_DEF_ERR(MID_JPG, HI_LOG_LEVEL_ERROR, ERR_JPG_DEC_FAIL) 

    #define HI_ERR_JPG_PARSE_FAIL\
    HI_DEF_ERR(MID_JPG, HI_LOG_LEVEL_ERROR, ERR_JPG_PARSE_FAIL)  

    #define HI_ERR_JPG_DEC_RUNNING\
    HI_DEF_ERR(MID_JPG, HI_LOG_LEVEL_ERROR, ERR_JPG_DEC_RUNNING) 
    

    /***************************  The enum of Jpeg image format  ******************/

    /**
     ** JPG API error code
     **/
    enum hiJPG_ErrorCode_E
    {
        ERR_JPG_PTR_NULL       = 0x1,    /** NULL pointer                   **/
        ERR_JPG_DEV_OPENED     = 0x2,    /** device has opened              **/
        ERR_JPG_DEV_NOOPEN     = 0x3,    /** device no open                 **/
        ERR_JPG_INVALID_PARA   = 0x4,    /** inefficacy parameter           **/
        ERR_JPG_INVALID_FILE   = 0x5,    /** inefficacy file or file operation error **/
        ERR_JPG_NO_MEM         = 0x6,    /** mem lack                       **/
        ERR_JPG_INVALID_SOURCE = 0x7,    /** inefficacy input               **/
        ERR_JPG_TIME_OUT       = 0x8,    /** overtime                       **/
        ERR_JPG_INVALID_HANDLE = 0x9,    /** inefficacy handle              **/
        ERR_JPG_EXIST_INSTANCE = 0xA,    /** exist decode                   **/
        ERR_JPG_THUMB_NOEXIST  = 0xB,    /** thumed no exist                **/
        ERR_JPG_NO_TASK        = 0xC,    /** work have no begin             **/
        ERR_JPG_NOSUPPORT_FMT  = 0xD,    /** the format unsupport           **/
        ERR_JPG_DEC_BUSY       = 0xE,    /** device busy                    **/
        ERR_JPG_DEC_PARSING    = 0xF,    /** at parasing                    **/
        ERR_JPG_DEC_DECODING   = 0x10,   /** decoding                       **/
        ERR_JPG_WANT_STREAM    = 0x11,   /** short of stream                **/
        ERR_JPG_DEC_FAIL       = 0x12,   /** decoding failure               **/
        ERR_JPG_PARSE_FAIL     = 0x13,   /** parasing failure               **/
        ERR_JPG_DEC_RUNNING    = 0x14    /** request working,have no finish **/
    };

    /********************** Global Variable declaration **************************/

  
    /******************************* API declaration *****************************/


#ifdef __cplusplus
    #if __cplusplus
}
    #endif
#endif /* __cplusplus */

#endif /* __HI_JPG_ERRCODE_H__ */

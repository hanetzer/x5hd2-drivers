/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon. Co., Ltd.

******************************************************************************
File Name	    : hi_jpg_ioctl.h
Version		    : Initial Draft
Author		    : 
Created		    : 2012/10/31
Description	    : 
Function List 	: 
			  		  
History       	:
Date				Author        		Modification
2012/10/31		    y00181162		                	
******************************************************************************/


#ifndef __HI_JPG_OSR_H__
#define __HI_JPG_OSR_H__

#ifdef __cplusplus
     #if __cplusplus
extern "C"{
     #endif
#endif /* __cplusplus */


#include "hi_type.h"
#include "hijpeg_type.h"



#define VID_CMD_MAGIC     'j' 

#define CMD_JPG_GETDEVICE        _IO( VID_CMD_MAGIC, 0x0)
#define CMD_JPG_RELEASEDEVICE    _IO( VID_CMD_MAGIC, 0x1)
#define CMD_JPG_GETINTSTATUS     _IOWR( VID_CMD_MAGIC, 0x2, JPG_GETINTTYPE_S *)
#define CMD_JPG_SUSPEND          _IO( VID_CMD_MAGIC, 0x3)
#define CMD_JPG_RESUME           _IO( VID_CMD_MAGIC, 0x4)
#define CMD_JPG_READPROC         _IOWR( VID_CMD_MAGIC, 0x5, JPEG_PROC_INFO_S *)


#ifdef __cplusplus
     #if __cplusplus
}
     #endif
#endif /* __cplusplus */

#endif

/*
 * Copyright (c) (2011 - ...) digital media project platform development dept,
 * Hisilicon. All rights reserved.
 *
 * File: vdec.c
 *
 * Purpose: vdec omx adaptor layer
 *
 * Author: sunny. liucan 180788
 *
 * Date: 26, Dec, 2011
 *
 */

#ifndef __COMMON_H__
#define __COMMON_H__

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/spinlock.h>
#include <linux/slab.h>

//#include "hi_drv_vpss.h"
//#include "drv_vpss_ext.h"
#include "vfmw.h"
#include "hi_type.h"

#define OMX_VERSION	 		       	2013062000

#define OMX_ALWS                                 0xFFFFFFFF
#define OMX_FATAL			              0
#define OMX_ERR			                     1
#define OMX_WARN			              2
#define OMX_INFO			              3
#define OMX_TRACE			              4
#define OMX_INBUF			              5
#define OMX_OUTBUF	 		       	6
#define OMX_VPSS	 		       	7

extern HI_U32 OmxTraceParam;

#define OmxPrint(flag, format,arg...) \
	do { \
		if (OMX_ALWS == flag || (0 != (OmxTraceParam & (1 << flag)))) \
		    printk("<OMX VDEC> " format, ## arg); \
	} while (0)


/* OmxTraceParam 常用值

   1:      OMX_FATAL
   2:      OMX_ERR
   4:      OMX_WARN
   8:      OMX_INFO           (常用于查看创建通道配置项以及跟踪最后一帧情况)
   16:    OMX_TRACE
   32:    OMX_INBUF
   64:    OMX_OUTBUF
   128:   OMX_VPSS        (常用于跟踪图像不输出时VPSS 情况)

   3:      OMX_FATAL & OMX_ERR
   7:      OMX_FATAL & OMX_ERR & OMX_WARN
   11:    OMX_FATAL & OMX_ERR & OMX_INFO
   19:    OMX_FATAL & OMX_ERR & OMX_TRACE
   96:    OMX_INBUF & OMX_OUTBUF
   35:    OMX_FATAL & OMX_ERR & OMX_INBUF
   67:    OMX_FATAL & OMX_ERR & OMX_OUTBUF
   99:    OMX_FATAL & OMX_ERR & OMX_INBUF & OMX_OUTBUF
   
*/

#endif

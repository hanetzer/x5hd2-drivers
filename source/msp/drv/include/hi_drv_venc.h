/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : mpi_priv_venc.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2010/03/31
  Description   :
  History       :
  1.Date        : 2010/03/31
    Author      : j00131665
    Modification: Created file

******************************************************************************/

#ifndef __HI_DRV_VENC_H__
#define __HI_DRV_VENC_H__

#include "hi_unf_venc.h"
#include "hi_drv_file.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* End of #ifdef __cplusplus */

#define VENC_MAX_CHN_NUM 8

#define HI_FATAL_VENC(fmt...) HI_FATAL_PRINT(HI_ID_VENC, fmt)
#define HI_ERR_VENC(fmt...) HI_ERR_PRINT(HI_ID_VENC, fmt)
#define HI_WARN_VENC(fmt...) HI_WARN_PRINT(HI_ID_VENC, fmt)
#define HI_INFO_VENC(fmt...) HI_INFO_PRINT(HI_ID_VENC, fmt)
#define HI_DBG_VENC(fmt...) HI_DBG_PRINT(HI_ID_VENC, fmt)


/*********************************************************************/
/* for omxvenc struction                                             */
/*********************************************************************/
/* VENC msg response types */
#define VENC_MSG_RESP_BASE 		        0xA0000
//#define VENC_MSG_RESP_OPEN              (VENC_MSG_RESP_BASE + 0x1)
#define VENC_MSG_RESP_START_DONE        (VENC_MSG_RESP_BASE + 0x1)
#define VENC_MSG_RESP_STOP_DONE        	(VENC_MSG_RESP_BASE + 0x2)
#define VENC_MSG_RESP_PAUSE_DONE        (VENC_MSG_RESP_BASE + 0x3)
#define VENC_MSG_RESP_RESUME_DONE	    (VENC_MSG_RESP_BASE + 0x4)
#define VENC_MSG_RESP_FLUSH_INPUT_DONE  (VENC_MSG_RESP_BASE + 0x5)
#define VENC_MSG_RESP_FLUSH_OUTPUT_DONE (VENC_MSG_RESP_BASE + 0x6)
#define VENC_MSG_RESP_INPUT_DONE        (VENC_MSG_RESP_BASE + 0x7)          //改帧可以还
#define VENC_MSG_RESP_OUTPUT_DONE       (VENC_MSG_RESP_BASE + 0x8)          //已经填满?
#define VENC_MSG_RESP_MSG_STOP_DONE	    (VENC_MSG_RESP_BASE + 0x9)

typedef struct venc_chan_cfg_s {
    HI_U32 protocol;      /* VEDU_H264, VEDU_H263 or VEDU_MPEG4 */
    HI_U32 frame_width;    /* width	in pixel, 96 ~ 2048 */
    HI_U32 frame_height;   /* height in pixel, 96 ~ 2048 */
    HI_U32 CapLevel;
    HI_U32 rotation_angle; /* venc don't care */
    
    HI_U32 priority;
    HI_U32 streamBufSize;

    HI_U16 SlcSplitEn;    /* 0 or 1, slice split enable */
    //HI_U32 SplitSize;     /* 512 ~ max, bytes @ H264 & MP4, H263 don't care  */
    HI_U32 Gop;
    HI_U16 QuickEncode;

    HI_U32  TargetBitRate;
    HI_U32  TargetFrmRate;
    HI_U32  InputFrmRate;

    HI_U32  MinQP;
    HI_U32  MaxQP;  
}venc_chan_cfg;

enum venc_port_dir {
	PORT_DIR_INPUT,
	PORT_DIR_OUTPUT,
	PORT_DIR_BOTH = 0xFFFFFFFF
};

typedef struct venc_user_buf_s {
    
	void  *bufferaddr;     //虚拟地址
    HI_U32 bufferaddr_Phy;
	HI_U32 vir2phy_offset;
	HI_U32 buffer_size;    //buffer alloc size
	HI_U32 offset_YC;    //YC分量的偏移

	HI_U32 offset;         //
	HI_U32 data_len;      //filled len
    
	HI_U32 strideY;
    HI_U32 strideC;

    HI_S16  store_type;
	HI_S16  sample_type;
	HI_S16  package_sel;
	HI_U32 timestamp;    
	HI_S16 flags;

	enum venc_port_dir dir;

	void *ion_handle; /*used for ion*/
	HI_S32 pmem_fd;
	unsigned long mmaped_size;

	//union user_buf_extra_info info;
	HI_U32 client_data;
}venc_user_buf;

typedef struct venc_msginfo_s {
	HI_U32 status_code;          //记录操作的返回值(success/failure)
	HI_U32 msgcode;              //自定义的上行消息返回值，定义在此处
	venc_user_buf buf;     //
	HI_U32 msgdatasize;
}venc_msginfo;

/*********************************************************************/
/* for omxvenc struction  ->end                                      */
/*********************************************************************/

HI_S32 HI_DRV_VENC_Init(HI_VOID);
HI_S32 HI_DRV_VENC_DeInit(HI_VOID);
HI_S32 HI_DRV_VENC_GetDefaultAttr(HI_UNF_VENC_CHN_ATTR_S *pstAttr);
HI_S32 HI_DRV_VENC_Create(HI_HANDLE *phVencChn, HI_UNF_VENC_CHN_ATTR_S *pstAttr,HI_BOOL bOMXChn, struct file  *pfile);
HI_S32 HI_DRV_VENC_Destroy(HI_HANDLE hVenc);
HI_S32 HI_DRV_VENC_AttachInput(HI_HANDLE hVenc,HI_HANDLE hSrc);
HI_S32 HI_DRV_VENC_DetachInput(HI_HANDLE hVencChn);
HI_S32 HI_DRV_VENC_Start(HI_HANDLE hVenc);
HI_S32 HI_DRV_VENC_Stop(HI_HANDLE hVenc);
HI_S32 HI_DRV_VENC_AcquireStream(HI_HANDLE hVenc,HI_UNF_VENC_STREAM_S *pstStream, HI_U32 u32TimeoutMs);
HI_S32 HI_DRV_VENC_ReleaseStream(HI_HANDLE hVenc, HI_UNF_VENC_STREAM_S *pstStream);

HI_S32 HI_DRV_VENC_SetAttr(HI_HANDLE hVenc,HI_UNF_VENC_CHN_ATTR_S *pstAttr);
HI_S32 HI_DRV_VENC_GetAttr(HI_HANDLE hVenc, HI_UNF_VENC_CHN_ATTR_S *pstAttr);
HI_S32 HI_DRV_VENC_RequestIFrame(HI_HANDLE hVenc);
HI_S32 HI_DRV_VENC_QueueFrame(HI_HANDLE hVenc, HI_UNF_VIDEO_FRAME_INFO_S *pstFrameinfo);
HI_S32 HI_DRV_VENC_DequeueFrame(HI_HANDLE hVenc, HI_UNF_VIDEO_FRAME_INFO_S *pstFrameinfo);

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif

#endif //__HI_DRV_VENC_H__

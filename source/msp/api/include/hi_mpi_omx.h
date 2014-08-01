/*
 * Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
 * File Description: omx mpi interface
 */
 
 #ifndef __HI_MPI_OMX_H__
 #define __HI_MPI_OMX_H__
 
 #include "hi_unf_avplay.h"
 #include "hi_drv_video.h"
 #include "hi_drv_vpss.h"
 #include "hi_video_codec.h"

 /* for c++*/
 #ifdef __cplusplus
 #if __cplusplus
 extern "C" {
 #endif
 #endif /*__cplusplus*/
 
 /*callback for vo moudle to get or realse video frame buffer*/
 typedef int (*PFN_VDEC_Chan_VORlsFrame)(HI_HANDLE, HI_DRV_VIDEO_FRAME_S*);
 
 /*OMX Vdec INTERFACE Structure*/
 typedef struct tagOMXVDEC_INTERFACE_S {
  PFN_VDEC_Chan_VORlsFrame pfVORlsFrame;
 } OMXVDEC_INTERFACE_S;

 /* interface for vo module to get callback of video frame buffer*/
 HI_S32 HI_MPI_OMXVdec_GetRealseFrameInterface(OMXVDEC_INTERFACE_S *omxvdec_interface);
 
 #ifdef __cplusplus
 #if __cplusplus
 }
 #endif
 #endif /*__cplusplus*/
 
 #endif /*__HI_MPI_OMX_H__*/
 

/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_drv_pvr.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2009/12/21
  Description   :
  History       :
  1.Date        : 2009/12/21
    Author      : w58735
    Modification: Created file

******************************************************************************/
#ifndef __HI_DRV_PVR_H__
#define __HI_DRV_PVR_H__

#include "hi_unf_pvr.h"
#include "hi_mpi_pvr.h"

/* definition of max play channel */
#define PVR_PLAY_MAX_CHN_NUM            2

/* definition of max record channel */
#define PVR_REC_MAX_CHN_NUM             3
#define PVR_REC_START_NUM               PVR_PLAY_MAX_CHN_NUM

#define CMD_PVR_INIT_PLAY               _IOR(HI_ID_PVR, 0x01, HI_U32)
#define CMD_PVR_CREATE_PLAY_CHN         _IOR(HI_ID_PVR, 0x02, HI_U32)
#define CMD_PVR_DESTROY_PLAY_CHN        _IOW(HI_ID_PVR, 0x03, HI_U32)

#define CMD_PVR_INIT_REC                _IOR(HI_ID_PVR, 0x11, HI_U32)
#define CMD_PVR_CREATE_REC_CHN          _IOR(HI_ID_PVR, 0x12, HI_U32)
#define CMD_PVR_DESTROY_REC_CHN         _IOW(HI_ID_PVR, 0x13, HI_U32)

/* attributes of play channel                                               */
typedef struct hiPVR_PLAY_CHN_PROC_S
{
    HI_U32           u32DmxId;
    HI_HANDLE        hAvplay;
    HI_HANDLE        hTsBuffer;
#ifdef HI_CIPHER_SUPPORT
    HI_HANDLE        hCipher;                              /* cipher handle */
#endif
    HI_CHAR          szFileName[PVR_MAX_FILENAME_LEN];     /**name of stream file for playing,static attribution *//*CNcomment: 待播放码流的文件名，静态属性。        */

    HI_UNF_PVR_PLAY_STATE_E enState;                       /* play state */
    HI_UNF_PVR_PLAY_SPEED_E enSpeed;

    HI_U64          u64CurReadPos;      /* current data file read position */
    HI_U32          u32StartFrame;      /* the first available frame number in index while play cycle playing *//*CNcomment: 循环播放时有效的第一帧在Index中的帧号 */
    HI_U32          u32EndFrame;        /* the last available frame number in index while play cycle playing *//*CNcomment: 循环播放时有效的最后一帧在Index中的帧号 */
    HI_U32          u32LastFrame;       /* the frame number in the end of cycle playing *//*CNcomment: 循环尾部最后一帧的帧号 */
    HI_U32          u32ReadFrame;       /* read pointer of index file when playing,frame amount *//*CNcomment:播放时索引文件的读指针，帧个数 */

} PVR_PLAY_CHN_PROC_S;

/* attributes of record channel                                             */
typedef struct hiPVR_REC_CHN_PROC_S
{
    HI_UNF_PVR_REC_ATTR_S stUserCfg;
    HI_U32              u32RecOffset;
    HI_U64              u64CurFileSize;                     /* current position of record file */
    HI_UNF_PVR_REC_STATE_E     enState;                     /* record state */
    HI_UNF_PVR_REC_INDEX_TYPE_E enIndexType;                /**< index type,static attribution *//*CNcomment:索引类型，静态属性。 */
    HI_UNF_VCODEC_TYPE_E        enIndexVidType;             /**< code fomat of stream waitting for creating index,it should be configured when the index type is HI_UNF_PVR_REC_INDEX_TYPE_VIDEO,  static attribution*/
                                                            /*CNcomment:待建立索引的码流的视频编码协议,索引类型是HI_UNF_PVR_REC_INDEX_TYPE_VIDEO时才需要配置,静态属性  */
    HI_U32                      u32WriteFrame;              /*current written index offset *//*CNcomment:当前写的索引偏移*/
    PVR_INDEX_ENTRY_S           stLastRecFrm;
} PVR_REC_CHN_PROC_S;

#endif

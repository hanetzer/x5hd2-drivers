//******************************************************************************

//  Copyright (C), 2003-2008, Huawei Technologies Co., Ltd.
//
//******************************************************************************
// File Name       : optm_vi.h
// Version         : 1.0
// Author          : ChenWanjun/c57657
// Created         : 2009-4-14
// Last Modified   :
// Description     : The header file of viu_driver
// Function List   :
// History         :
// 1 Date          : 2009-4-14
//   Author        : ChenWanjun/c57657
//   Modification  : Created file
//------------------------------------------------------------------------------
// $Log: optm_vi.h,v $
// Revision 1.1  2009/04/16 00:34:31  c57657
// no message
//
//******************************************************************************

#ifndef __OPTM_VI_H__
#define __OPTM_VI_H__

#include "hi_unf_vi.h"
#include "drv_vi_ioctl.h"
#include "viu_reg.h"

typedef enum
{
    VIU_CC_INT = 0x0001,               /* Current image data capture finish int */
    VIU_BUFOVF_INT = 0x0002,           /* Internal FIFO overflow int */
    VIU_FIELDTHROW_INT = 0x0004,       /* Field data lose int */
    VIU_BUSERR_INT     = 0x0008,       /* AHB error int */
    VIU_PROCERR_INT    = 0x0010,       /* Protect bit error int(BT.656 mode) */
    VIU_REGUPDATE_INT  = 0x0020,       /* Work reg update int */
    VIU_FRAMEPULSE_INT = 0x0040,       /* Frame pulse int */
    VIU_NPTRANS_INT    = 0x0080,       /* Pal Ntsc transform int */
    VIU_CHDIVERR_INT   = 0x0100        /* Channel distribute error int */
} VIU_INT_STATUS_E;

typedef enum
{
    VIU_IMAGE_DONE_STATUS = 0x0001,    /* VIU recieve current field data */
    VIU_BUFOVF_STATUS = 0x0002,        /* VIU internal buffer overflow */
    VIU_FILED_THROW_STATUS = 0x0004,   /* VIU lost one field */
    VIU_BUS_ERR_STATUS  = 0x0008,      /* Bus error status */
    VIU_PROC_ERR_STATUS = 0x0010,      /* Protect bit error status */
    VIU_SNOOZE_STATUS  = 0x0020,       /* Current VIU snooze status */
    VIU_FILED2_STATUS  = 0x0040,       /* Current recieve field is even */
    VIU_VI_BUSY_STATUS = 0x0080,       /* VIU is busy */
    VIU_ACT_HEIGHT_STATUS = 0x0100     /* Detect effective pixel number of rows of the field */
} VIU_STATUS_E;

typedef struct
{
    HI_U32 u32PortScanMode;
    HI_U32 u32PortCapMode;
    HI_U32 u32PortMuxMode;
    HI_U32 u32PortVsync;
    HI_U32 u32PortVsyncNeg;
    HI_U32 u32PortHsync;
    HI_U32 u32PortHsyncNeg;
    HI_U32 u32PortEn;
} VI_GLOBAL_PORT_ATTR;

typedef struct
{
    HI_U32 u32DataWidth;
    HI_U32 u32StoreMode;
    HI_U32 u32CapSeq;
    HI_U32 u32CapSel;
    HI_U32 u32StoreMethod;
    HI_U32 u32ChromaResample;
    HI_U32 u32DownScaling;
    HI_U32 u32CorrectEn;
    HI_U32 u32OddLineSel;
    HI_U32 u32EvenLineSel;
    HI_U32 u32ChnId;
    HI_U32 u32ChnIdEn;
    HI_U32 u32ChromSwap;
    HI_U32 u32SeavFNeg;
    HI_U32 u32PrioCtrl;

    HI_U32 u32ChEn;
    HI_U32 u32Block0En;
    HI_U32 u32Block1En;
    HI_U32 u32Block2En;
    HI_U32 u32Block3En;

    /*HI_U32          u32Block0Mode       ;
    HI_U32          u32Block1Mode       ;
    HI_U32          u32Block2Mode       ;
    HI_U32          u32Block3Mode       ;*/
    HI_U32 u32Anc0En;
    HI_U32 u32Anc1En;
    HI_U32 u32DebugEn;
} VI_CH_ATTR;

/* line select mode */
typedef enum hi_VI_LINESEL_E
{
    VI_LINESEL_ODD = 0,     /* only select odd line */
    VI_LINESEL_EVEN,        /* only select even line */
    VI_LINESEL_BOTH,        /* select both odd and even line */
    VI_LINESEL_0347,        /* select 0 and 3rd line in each 4 lines */
    VI_LINESEL_1256,        /* select 1st and 2nd line in each 4 lines */
    VI_LINESEL_BUTT
} VI_LINESEL_E;

//=======Driver functions related to user-defined structures=========
HI_VOID                VIU_DRV_BoardInit(void);
HI_VOID                VIU_DRV_BoardDeinit(void);

//HI_VOID VIU_DRV_SetPortAttr(HI_U32 u32PortId, const VI_GLOBAL_PORT_ATTR *pstViAttr);
HI_VOID                VIU_DRV_SetPortAttr(HI_U32 u32PortId, HI_U32 u32PortScanMode, HI_U32 u32PortCapMode,
                                           HI_U32 u32PortMuxMode, HI_U32 u32PortVsync, HI_U32 u32PortVsyncNeg,
                                           HI_U32 u32PortHsync, HI_U32 u32PortHsyncNeg,
                                           HI_U32 u32PortEn);
HI_VOID                VIU_DRV_GetPortAttr(HI_U32 u32PortId, VI_GLOBAL_PORT_ATTR *pstViAttr);
HI_VOID                VIU_DRV_Start(HI_U32 u32PortId, HI_U32 ViChn);
HI_VOID                VIU_DRV_Stop(HI_U32 u32PortId, HI_U32 ViChn);

//HI_VOID VIU_DRV_SetCapWindow(HI_U32 u32PortId, HI_U32 ViChn, const VI_CAP_ATTR* pstViAttr);
HI_VOID                VIU_DRV_SetCapWindow(HI_U32 u32PortId, HI_U32 ViChn, HI_U32 u32CapStartX, HI_U32 u32CapStartY,
                                            HI_U32 u32CapWidth, HI_U32 u32CapHeight, HI_U32 ViChnFirEn,
                                            HI_U32 u32StoreMethod,
                                            HI_U32 u32Separated,
                                            HI_U32 u32EvenLineSel);
HI_VOID VIU_DRV_SetCapWindowRawData(HI_U32 u32PortId, HI_U32 ViChn, HI_U32 u32CapStartX, HI_U32 u32CapStartY,
                             HI_U32 u32CapWidth, HI_U32 u32CapHeight, HI_U32 ViChnFirEn,
                             HI_U32 u32Separated, HI_U32 u32EvenLineSel);

HI_VOID                VIU_DRV_SetChnAttr(HI_U32 u32PortId, HI_U32 ViChn, const VI_CH_ATTR* pstViAttr);
HI_U32                 VIU_DRV_GetIntStatus(HI_U32 u32PortId, HI_U32 ViChn);
HI_VOID                VIU_DRV_ClrInterruptStatus(HI_U32 u32PortId, HI_U32 ViChn, HI_U32 u32ClrInt);

//HI_VOID VIU_DRV_SetMemAddr(HI_U32 u32PortId, HI_U32 ViChn, VI_BASE_ADDR stViAddr);
HI_VOID                VIU_DRV_SetMemAddr(HI_U32 u32PortId, HI_U32 ViChn, HI_U32 u32YBaseAddr, HI_U32 u32UBaseAddr,
                                          HI_U32 u32VBaseAddr);
HI_VOID                VIU_DRV_SetMemStride(HI_U32 u32PortId, HI_U32 ViChn, HI_U32 u32YStride, HI_U32 u32UStride,
                                            HI_U32 u32VStride);

//HI_VOID VIU_DRV_GetLuma(HI_U32 u32PortId, HI_U32 ViChn, VI_LUMA_STATISTIC* pstViAttr);
// added for fpga using!!(y58808)---------------------------------------
HI_U32                 VIU_DRV_GetLumaAdder(HI_U32 u32PortId, HI_U32 ViChn);
HI_U32                 VIU_DRV_GetLumaDiffAdder(HI_U32 u32PortId, HI_U32 ViChn);
// end of added-------------------------------------------------------
HI_U32                 VIU_DRV_GetChnStatus(HI_U32 u32PortId, HI_U32 ViChn);

//HI_VOID VIU_DRV_SetBlockAttr(HI_U32 u32PortId, HI_U32 ViChn, HI_U32 u32BlockNo, const VI_CHN_BLOCK_ATTR* pstViAttr);
HI_VOID                VIU_DRV_SetBlockAttr(HI_U32 u32PortId, HI_U32 ViChn, HI_U32 u32BlockNo,
                                            HI_U32 u32BlockStartX, HI_U32 u32BlockStartY, HI_U32 u32BlockWidth,
                                            HI_U32 u32BlockHeight, HI_U32 u32BlockColorY, HI_U32 u32BlockColorU,
                                            HI_U32 u32BlockColorV, HI_U32 u32BlockMscWidth, HI_U32 u32BlockMscHeight);

//HI_VOID VIU_DRV_SetAncAttr(HI_U32 u32PortId, HI_U32 ViChn, HI_U32 u32AncNo, const VI_ANC_ATTR* pstViAttr);
HI_VOID                VIU_DRV_SetAncAttr(HI_U32 u32PortId, HI_U32 ViChn, HI_U32 u32AncNo, HI_U32 u32AncHos,
                                          HI_U32 u32AncVos, HI_U32 u32AncLoc, HI_U32 u32AncSize);

//HI_VOID VIU_DRV_SetSyncAttr(HI_U32 u32PortId, const VI_GLOBAL_SYNC_ATTR* pstViAttr);
HI_VOID                VIU_DRV_SetSyncAttr(HI_U32 u32PortId, HI_U32 u32Act1Height, HI_U32 u32Act1Voff,
                                           HI_U32 u32Act1Vbb, HI_U32 u32Act2Height, HI_U32 u32Act2Voff,
                                           HI_U32 u32Act2Vbb, HI_U32 u32ActWidth, HI_U32 u32ActHoff, HI_U32 u32ActHbb,
                                           HI_U32 u32VsynWidth, HI_U32 u32HsynWidthMsb, HI_U32 u32HsynWidthLsb);
HI_VOID                VIU_DRV_SetIntDlyCnt(HI_U32 u32Cnt);
HI_VOID                VIU_DRV_SetIntEn(HI_U32 u32PortId, HI_U32 ViChn, HI_U32 u32Value);
HI_U32                 VIU_DRV_GetRawIntStatus(HI_U32 u32PortId, HI_U32 ViChn);
HI_U32                 VIU_DRV_GetIntIndicator(void);
HI_U32                 VIU_DRV_GetRawIntIndicator(void);
HI_VOID                VIU_DRV_SetLumaStrh(HI_U32 u32PortId, HI_U32 ViChn, HI_U32 k, HI_U32 m0, HI_U32 m1);
HI_VOID                VIU_DRV_GetAncData(HI_U32 u32PortId, HI_U32 ViChn, HI_U32 u32AncNo, HI_U32 size, HI_U8* buf);
HI_VOID                VIU_DRV_SetPrioCtrl(HI_U32 u32OutstandingMax, HI_U32 u32PrioCtrl);
HI_VOID                VIU_DRV_SetFirLumaCoef(HI_U32 u32PortId, HI_U32 ViChn, HI_S32 i32Coef0, HI_S32 i32Coef1,
                                              HI_S32 i32Coef2, HI_S32 i32Coef3, HI_S32 i32Coef4, HI_S32 i32Coef5,
                                              HI_S32 i32Coef6,
                                              HI_S32 i32Coef7);
HI_VOID                VIU_DRV_SetFirChromaCoef(HI_U32 u32PortId, HI_U32 ViChn, HI_S32 i32Coef0, HI_S32 i32Coef1,
                                                HI_S32 i32Coef2,
                                                HI_S32 i32Coef3);

//=================================================

//=============Function of configuration for IU_CH_CFG, VIU_CH_CTRL==================
HI_VOID                VIU_DRV_SetFixCode(HI_U32 u32PortId, HI_U32 ViChn, const HI_U32 u32Value);
HI_VOID                VIU_DRV_SetYcChannel(HI_U32 u32PortId, HI_U32 ViChn, const HI_U32 u32Value);
HI_VOID                VIU_DRV_SetSeavFNeg(HI_U32 u32PortId, HI_U32 ViChn, const HI_U32 u32Value);
HI_VOID                VIU_DRV_SetChromaSwap(HI_U32 u32PortId, HI_U32 ViChn, const HI_U32 u32Value);
HI_VOID                VIU_DRV_SetChnIdEn(HI_U32 u32PortId, HI_U32 ViChn, const HI_U32 u32Value);
HI_VOID                VIU_DRV_SetChnId(HI_U32 u32PortId, HI_U32 ViChn, const HI_U32 u32Value);
HI_VOID                VIU_DRV_SetEvenLineSel(HI_U32 u32PortId, HI_U32 ViChn, const VI_LINESEL_E eLineSel);
HI_VOID                VIU_DRV_SetOddLineSel(HI_U32 u32PortId, HI_U32 ViChn, const VI_LINESEL_E eLineSel);
HI_VOID                VIU_DRV_SetCorrectEn(HI_U32 u32PortId, HI_U32 ViChn, const HI_U32 u32Value);
HI_VOID                VIU_DRV_SetDownScaling(HI_U32 u32PortId, HI_U32 ViChn, const HI_U32 u32Value);
HI_VOID                VIU_DRV_SetChromaResample(HI_U32 u32PortId, HI_U32 ViChn, const HI_U32 u32Value);
//HI_VOID                VIU_DRV_SetStoreMethod(HI_U32 u32PortId, HI_U32 ViChn,
//                                              const HI_UNF_VI_STOREMETHOD_E eStoreMothod);
HI_VOID VIU_DRV_SetStoreMethod(HI_U32 u32PortId, HI_U32 ViChn, const HI_UNF_VIDEO_FORMAT_E eStoreMothod);
HI_VOID                VIU_DRV_SetCapSel(HI_U32 u32PortId, HI_U32 ViChn, const HI_UNF_VI_CAPSEL_E eCapSel);
HI_VOID                VIU_DRV_SetCapSeq(HI_U32 u32PortId, HI_U32 ViChn, const VIU_CAPSEQ_E eCapSeq);
//HI_VOID                VIU_DRV_SetStoreMode(HI_U32 u32PortId, HI_U32 ViChn, const HI_UNF_VI_STORE_MODE_E eStoreMode);
HI_VOID                VIU_DRV_SetStoreMode(HI_U32 u32PortId, HI_U32 ViChn, const HI_UNF_VIDEO_FIELD_MODE_E eStoreMode);
//HI_UNF_VI_STORE_MODE_E VIU_DRV_GetStoreMode(HI_U32 u32PortId, HI_U32 ViChn);
HI_U32                 VIU_DRV_GetStoreMode(HI_U32 u32PortId, HI_U32 ViChn);

/*HI_VOID VIU_DRV_SetDataWidth(HI_U32 u32PortId, HI_U32 ViChn, const HI_UNF_VI_DATAWIDTH_E eDataWidth);*/

HI_VOID                VIU_DRV_SetDebugEn(HI_U32 u32PortId, HI_U32 ViChn, const HI_U32 u32Value);
HI_VOID                VIU_DRV_SetLumStrhEn(HI_U32 u32PortId, HI_U32 ViChn, const HI_U32 u32Value);
HI_VOID                VIU_DRV_SetAncEn(HI_U32 u32PortId, HI_U32 ViChn, HI_U32 u32AncNo, const HI_U32 u32Value);

//HI_VOID VIU_DRV_SetBlockMode(HI_U32 u32PortId, HI_U32 ViChn, HI_U32 u32BlockNo, const VIU_BLOCK_COVER_E eBlockFill);
HI_VOID                VIU_DRV_SetBlockEn(HI_U32 u32PortId, HI_U32 ViChn, HI_U32 u32BlockNo, const HI_U32 u32Value);
HI_VOID                VIU_DRV_SetChEn(HI_U32 u32PortId, HI_U32 ViChn, const HI_U32 u32Value);

// below added for fpga using!!(y58808)-------------------------------------------------
HI_U32                 VIU_DRV_GetMemYAddr(HI_U32 u32PortId, HI_U32 ViChn);

#include <linux/semaphore.h>
#include "viu_buf.h"

#define VIU_INTERRUPT_LINE (35 + 32)

#define VI_UID_MAX 4            /* userId:3 fixed for user mode access, 0 fixed for VI itself use */
#define VIU_PORT_MAX 2
#define VIU_CHANNEL_MAX 1
#define VI_MAX_CHN_NUM (VIU_PORT_MAX * VIU_CHANNEL_MAX)
#define VI_PHY_TO_KERN_OFFSET 0x40000000         //phyaddr to kernel addr

typedef enum hiVIDEO_CONTROL_MODE_E
{
    VIDEO_CONTROL_MODE_SLAVER = 0,
    VIDEO_CONTROL_MODE_MASTER,
    VIDEO_CONTROL_MODE_BUTT
} VIDEO_CONTROL_MODE_E;

typedef enum hiVIDEO_NORM_E
{
    VIDEO_ENCODING_MODE_PAL = 0,
    VIDEO_ENCODING_MODE_NTSC,
    VIDEO_ENCODING_MODE_AUTO,
    VIDEO_ENCODING_MODE_BUTT
} VIDEO_NORM_E;

typedef struct struVI_PUB_ATTR_S
{
    HI_U32 u32StartX;
    HI_U32 u32StartY;
    HI_U32 u32Width;
    HI_U32 u32Height;

    HI_BOOL bUseVbi;
} VI_PUB_ATTR_S;

typedef struct hiVI_CHN_BUF_ADDR_S
{
    HI_U32   au32ChnPhysAddr_Y[VI_MAX_CHN_NUM];  /* Y address of each vi channel */
    HI_U32   au32ChnPhysAddr_C[VI_MAX_CHN_NUM];
    HI_VOID *apChnVirtAddr_Y[VI_MAX_CHN_NUM];    /* Y virtual address of each vi channel */
    HI_VOID *apChnVirtAddr_C[VI_MAX_CHN_NUM];
} VI_CHN_BUF_ADDR_S;

/* Each buffer has phy addr and vir addr,
 * Venc need to know each VI channel's image phy addr,
 * OSD need both
 */
typedef struct hiVI_BUF_ADDR_S
{
    HI_U32   u32BufPhysAddr_Y;  /* Y addr of this buffer */
    HI_U32   u32BufPhysAddr_C;
    HI_VOID *pBufVirtAddr_Y;    /* Y vir addr of this buffer */
    HI_VOID *pBufVirtAddr_C;
} VI_BUF_ADDR_S;

typedef struct hiVI_CHN_ATTR_S
{
    HI_U32 u32CapSel;  /* capture top/bottom filed or both or a frame */
    HI_U32 u32StartX;
    HI_U32 u32StartY;
    HI_U32 u32Width;
    HI_U32 u32Height;
} VI_CHN_ATTR_S;

typedef struct hiVI_CH_ATTR_S
{
    VI_CHN_ATTR_S struApiAttr;
    HI_S32        chnId;
    HI_U32        u32CapX;
    HI_U32        u32CapY;
    HI_U32        u32CapWidth;
    HI_U32        u32CapHeight;
} VI_CH_ATTR_S;

/* Defines the configuration information of the input channel ID that is read from the VBI information when there are four D1 channels */
typedef struct hiVBI_CFG_S
{
    HI_U32 u32Position;     /* VBI data blank region. 0 indicates the front blank region (current setting). 1 indicates the back blank region. */
    HI_U32 u32Interval;     /* The distance from the start of the blank region to the VBI data lines (in lines) */
    HI_U32 u32Excursion;    /* The horizontal offset from the start of the line valid data to the start of the VBI data (in hours and a pixel is equal to two hours). */
    HI_U32 u32Start;        /* The bit of the VBI from which the channel ID starts. */
    HI_U32 u32Size;         /* The number of bits that a channel ID occupies. */
    HI_U32 u32Count;        /* Channel ID count. Because each field only has one input during the four-channel encoding process, this value is always 1. */
    HI_U32 Reserved;        /* Reserved. */
} VBI_CFG_S;

typedef struct hiVI_CHN_LUMSTRH_ATTR_S
{
    HI_U32 u32TargetK;
} VI_CHN_LUMSTRH_ATTR_S;

typedef struct hiVIU_DATA_S
{
    VI_PUB_ATTR_S struViuAttr;
    VI_CH_ATTR_S  struVchnAttr[VI_MAX_CHN_NUM];

    HI_U32 u32VchnMask;

    HI_U32        u32StrideY;
    HI_U32        u32StrideC;
    VI_BUF_ADDR_S struBufAddr[VIU_FB_MAX_NUM];

    /*VI_CHN_BUF_ADDR_S struChnBufAddr[VIU_FB_MAX_NUM]; liusanwei delete*/

    VBI_CFG_S struVIUVbiCfg; //Blanking zone
    HI_U32    u32VbiExcU32;  /* offset of channel id, 4bytes */
    HI_U32    u32VbiExcBit;  /* offset of channel id, bits outside of 4bytes*/
    HI_U32    u32VbiMask;          /* Mask used for access channel id*/

    HI_U32  u32TimeRef;
    HI_BOOL bRefTimeReset;

    HI_U32 u32LastBufIndex;
    HI_U32 u32ThisBufIndex;
    HI_U32 u32NextBufIndex;

    HI_BOOL bStoping;
    struct semaphore stopViuMutex;

    HI_BOOL bIsViuCfged;
    HI_U32  u32IsVicCfged;

    HI_U32               u32BufBack;
    HI_U32               u32RnInts;
    HI_U32               u32Discard;
    HI_U32               u32PTSMs[VIU_FB_MAX_NUM];
    HI_UNF_VI_BUF_MGMT_E enBufMode;      /** VI Frame buf management mode*/
} VIU_DATA_S;
#if 0
typedef struct hiVIDEO_FRAME_INFO_S
{
    HI_UNF_VI_BUF_S stVFrame;
    HI_U32          u32PoolId;
} VIDEO_FRAME_INFO_S;

typedef struct hiVI_BUF_BLK_S
{
    struct list_head list;
    HI_U32             u32Index;
    VIDEO_FRAME_INFO_S stVFrameInfo;
} VI_BUF_BLK_S;
#endif
typedef struct hiVI_STATISTIC_S
{
    HI_U32 AcquireTry[VI_UID_MAX];
    HI_U32 AcquireOK[VI_UID_MAX];
    HI_U32 ReleaseTry[VI_UID_MAX];
    HI_U32 ReleaseOK[VI_UID_MAX];
} VI_STATISTIC_S;

typedef struct  tagOPTM_VI_S
{
    /* Enale flag:TRUE, regist ISR; FALSE, logout ISR */
    HI_BOOL bOpened;

    HI_U32 u32x;
    HI_U32 u32y;
    HI_U32 u32Height;
    HI_U32 u32Width;

    HI_U32 u32StrideY;
    HI_U32 u32StrideC;

    HI_U32 u32RevFrmFrIntf;
    HI_U32 u32SndFrmToMd;
    HI_U32 u32RevFrmFrUsr;

    VI_STATISTIC_S stStatisticInfo;

    struct timeval stTimeStart;
} OPTM_VI_S;

typedef struct
{
    HI_BOOL          bValid;
	HI_UNF_VI_E      enViPort;
    HI_UNF_VI_ATTR_S stViCfg;
} VI_CFG_ATTR_S;

#define VIU_GET_HANDLE(s32Port, s32Chn) ((s32Port) * VIU_CHANNEL_MAX + (s32Chn))
#define VIU_GET_DEVID(handle) ((handle) / VIU_CHANNEL_MAX)
#define VIU_GET_CHANNEL(handle) ((handle) % VIU_CHANNEL_MAX)

HI_S32         Viu_OpenChn(HI_UNF_VI_E enVi);
HI_S32         Viu_CloseChn(HI_UNF_VI_E enVi, HI_BOOL bForce);
HI_S32         Viu_SetAttr(HI_U32 u32ChnID, HI_UNF_VI_ATTR_S  *stAttr);
HI_S32         Viu_GetAttr(HI_U32 u32ChnID, HI_UNF_VI_ATTR_S  *pstAttr);
HI_S32         Viu_EnableLumstrh(HI_S32 s32ViPort, HI_S32 s32ViChn);
HI_S32         Viu_DisableLumstrh(HI_S32 s32ViPort, HI_S32 s32ViChn);
HI_S32         Viu_EnableUserPic(HI_S32 s32ViPort, HI_S32 s32ViChn);
HI_S32         Viu_DisableUserPic(HI_S32 s32ViPort, HI_S32 s32ViChn);
//HI_S32         Viu_SetUserPic(HI_S32 s32ViPort, HI_S32 s32ViChn, VIDEO_FRAME_INFO_S *pstVFrame);
/*
HI_S32         Viu_UsrGetFrame(HI_U32 u32ChnID, HI_UNF_VI_BUF_S  *pstFrame);
HI_S32         Viu_UsrPutFrame(HI_U32 u32ChnID, HI_UNF_VI_BUF_S  *pstFrame);
*/
HI_S32         Viu_QueueFrame(HI_U32 u32ViPort, HI_UNF_VIDEO_FRAME_INFO_S *pFrameInfo);
HI_S32         Viu_DequeueFrame(HI_U32 u32ViPort, HI_UNF_VIDEO_FRAME_INFO_S *pFrameInfo);
HI_S32         Viu_UsrAcquireFrame(HI_U32 u32ChnID, HI_U32 u32Uid, HI_UNF_VIDEO_FRAME_INFO_S *pFrameInfo);
HI_S32         Viu_UsrReleaseFrame(HI_U32 u32ChnID, HI_U32 u32Uid, HI_UNF_VIDEO_FRAME_INFO_S *pFrameInfo);
irqreturn_t    Viu_InterruptHandler(int irq, void *dev_id);
HI_S32         Viu_AcquireFrame(HI_S32 handle, HI_UNF_VIDEO_FRAME_INFO_S  *pstFrame);

HI_S32         Viu_ReleaseFrame(HI_S32 handle, HI_UNF_VIDEO_FRAME_INFO_S  *pstFrame);

HI_S32         Viu_GetUsrID(HI_U32 u32ChnID, HI_MOD_ID_E enUserMode, HI_U32 *pu32Uid);

HI_S32         Viu_PutUsrID(HI_U32 u32ChnID, HI_U32 u32Uid);

HI_U32         do_GetTimeStamp(void);
HI_S32         Viu_SetExtBuf(HI_UNF_VI_E enViPort, HI_UNF_VI_BUFFER_ATTR_S *pstBufAttr);
HI_S32         Viu_SetBufAddr(HI_UNF_VI_E enViPort, HI_U32 u32VirAddr, HI_U32 u32BufIndex);
extern HI_VOID SYS_GetChipVersion(HI_CHIP_TYPE_E *penChipType, HI_CHIP_VERSION_E *penChipVersion);

#endif // __OPTM_VI_H__

#ifndef __VFMV_CTRL_H__
#define __VFMV_CTRL_H__
#include "vfmw.h"
#include "fsp.h"
#include "sysconfig.h"
#ifdef ENV_ARMLINUX_KERNEL
#include "sdec_imedia.h"
#endif
#include "syntax.h"

/*======================================================================*/
/*  常数                                                                */
/*======================================================================*/
#define VCTRL_OK                0
#define VCTRL_ERR              -1
#define VCTRL_ERR_VDM_BUSY     -2
#define VCTRL_ERR_NO_FSTORE    -3
#define VCTRL_ERR_NO_STREAM    -4
#define MAX_FRAME_SIZE 			2048*2048

#define VCTRL_FIND_PTS          0
#define VCTRL_PTS_ILLEAGLE     -1
#define VCTRL_SEEKING_PTS      -2

#define VCTRL_USER_OPTION     0
#define VCTRL_ADD_EXTRA        1
#define VCTRL_IGNOR_EXTRA     2

#define MAX_USRDEC_FRAME_NUM        (16)
/*======================================================================*/
/*  结构与枚举                                                          */
/*======================================================================*/
/* 帧存集合，用于调试: 查询有多少帧存，其内容分别为何 */
typedef struct hiVFMWFrameARRAY
{
    SINT32       s32FrameNum;
    SINT32       s32PixWidth;
    SINT32       s32PixHeight;
    SINT32       s32Stride;
    SINT32       s32ChromOfst;
    SINT32       s32PhyAddr[64];
} VFMW_FRAME_ARRAY_S;

/* 通道相关的统计信息 */
typedef struct hiVFMW_CHAN_STAT
{
    UINT32        u32FrBeginTime;   /* 计算帧率的开始时间 */
	UINT32        u32FrImgNum;      /* 帧数 */
	UINT32        u32FrFrameRate;   /* 帧率 */

	/* 丢帧统计 */
	UINT32        u32SkipFind;        /* 解到的skip帧数目 */
	UINT32        u32SkipDiscard;     /* 丢弃不解的skip帧数目 */
    UINT32        u32IsFieldFlag;     /* 当前插入队列中的帧是场图标志，用于PVR快进快退获取码流帧场属性 */
	
    VFMW_FRAME_ARRAY_S stFrameArray;
} VFMW_CHAN_STAT_S;

typedef struct hiVFMW_GLOBAL_STAT
{
    /* VDM占用率统计 */
    UINT32        u32VaBeginTime;     /* 统计VDM占用率的起始时间 */
    UINT32        u32VaLastStartTime; /* 上一次启动硬件的时间 */
	UINT32        u32VaVdmWorkTime;   /* VDM累计工作时间 */
	UINT32        u32VaVdmLoad;       /* VDM占用率 */

    /* DNR占用率统计 */
    UINT32        u32DaBeginTime;     /* 统计VDM占用率的起始时间 */
    UINT32        u32DaLastStartTime; /* 上一次启动硬件的时间 */
	UINT32        u32DaDnrWorkTime;   /* DNR累计工作时间 */
	UINT32        u32DaDnrLoad;       /* DNR占用率 */

	/* BTL占用率统计 */
    UINT32        u32BaBeginTime;     /* 统计BTL占用率的起始时间 */
    UINT32        u32BaLastStartTime; /* 上一次启动硬件的时间 */
	UINT32        u32BaBtlWorkTime;   /* BTL累计工作时间 */
	UINT32        u32BaBtlLoad;       /* BTL占用率 */

    /* VDM性能数据统计 */
    UINT32        u32PicNum;
    UINT32        u32AccVdmKiloCycle;
    UINT32        u32VaVdmKiloCycle;
    UINT32        u32StatTime;

} VFMW_GLOBAL_STAT_S;

#if 0
typedef struct
{
    SYNTAX_EXTRA_DATA_S *pstExtraData;	
    IMAGE_VO_QUEUE     ImageQue;
    SINT32        	   ChanID;
} USER_CTX_S;
#endif

typedef enum
{
    VDM_SUPPORT_VP6       = 1,  /* 1<<0 */
    VDM_SUPPORT_BPD       = 2,  /* 1<<1 */
    VDM_SUPPORT_VCMP      = 4,  /* 1<<2 */
    VDM_SUPPORT_GMC       = 8,   /* 1<<3 */    
    VDM_SUPPORT_DNR       = 16   /* 1<<4 */        
} VDM_CHARACTER_E;

/* 线程的状态 */
typedef enum hiTASKSTATE_E
{
    TASK_STATE_EXIT = 0,        /* 退出, 线程未创建或已销毁 */
    TASK_STATE_STOP,            /* 停止，线程已创建，但在空转，解码停止 */
    TASK_STATE_RUNNING,         /* 运行 */
    TASK_STATE_BUTT
} TASK_STATE_E;

/* 对线程发出的指令 */
typedef enum hiTASKCMD_E
{
    TASK_CMD_NONE = 0,        /* 无指令 */
    TASK_CMD_START,           /* 启动指令：启动处于停止状态的线程 */
    TASK_CMD_STOP,            /* 停止指令：停止处于运行状态的线程 */
    TASK_CMD_KILL,            /* 销毁指令：使处于运行或停止状态的线程自然退出 */
    TASK_CMD_BUTT
} TASK_CMD_E;

/* 通道的解码核心 */
typedef enum hiChanDecCore
{
    DEC_CORE_VDM,
	DEC_CORE_IMEDIA_H263,
	DEC_CORE_IMEDIA_SORENSON,
	DEC_CORE_IMEDIA_VP6,
    DEC_CORE_IMEDIA_VP6F,
    DEC_CORE_IMEDIA_VP6A,
	DEC_CORE_BUTT
} CHAN_DEC_CORE_E;

typedef struct hiDRV_MEM_S
{
    MEM_RECORD_S  stVdmHalMem[MAX_VDH_NUM];      /* VDM HAL内存 */
    MEM_RECORD_S  stScdCmnMem;      /* SCD 公共内存 */
//    MEM_RECORD_S  stFodCmnMem;      /* FOD 公共内存 */
    MEM_RECORD_S  stBTLReg;
    MEM_RECORD_S  stVdmReg[MAX_VDH_NUM];
    MEM_RECORD_S  stFodReg;
    MEM_RECORD_S  stScdReg[MAX_VDH_NUM];
    MEM_RECORD_S  stBpdReg;
    MEM_RECORD_S  stSystemReg;      /* 系统寄存器，比如复位FOD,VDM,SCD等 */
} DRV_MEM_S;

/* 解码器控制数据集 */
typedef struct hiVFMW_CTRL_DATA_S
{
    SINT32        s32IsVCTRLOpen;   /* 标志VCTRL是否被全局打开 */
    OSAL_IRQ_LOCK  DecLock;
    OSAL_TASK_MUTEX stDecMutex;
    OSAL_EVENT    eventVdmReady;

    TASK_STATE_E  eTaskState;       /* 线程状态 */
    TASK_CMD_E    eTaskCommand;     /* 线程指令 */
    OSAL_TASK     hThread;          /* 线程句柄 */

    SINT32        s32ThreadPos;    /* 线程位置 */

    SINT32        s32ThisChanIDPlus1;    /* 当前正在进行语法解码的通道号 */
    DRV_MEM_S     stDrvMem;
    SINT32        (*event_report)(SINT32 ChanID, SINT32 type, VOID* p_args );
} VFMW_CTRL_DATA_S;

/* 解码通道内存记录 */
typedef struct hiVFMW_CHAN_MEM_S
{
    MEM_RECORD_S   stChanMem;       /* 该通道的存储资源 */

    MEM_RECORD_S   stChanMem_vdh;       /* 该通道的帧存存储资源 */
    MEM_RECORD_S   stChanMem_scd;       /* 该通道的SCD存储资源 */
    MEM_RECORD_S   stChanMem_ctx;       /* 该通道的上下文存储资源 */ 
    
    SINT32         s32SelfAllocChanMem_vdh;  /* 标识通道 vdh 是否是自己分配的，1: 自己分配, 0: 外部分配 */
    SINT32         s32SelfAllocChanMem_scd;  /* 标识通道 scd 是否是自己分配的，1: 自己分配, 0: 外部分配 */
    SINT32         s32SelfAllocChanMem_ctx;  /* 标识通道 ctx 是否是自己分配的，1: 自己分配, 0: 外部分配 */
    STREAM_INTF_S  stStreamIntf;    /* 码流接口 */	
} VFMW_CHAN_MEM_S;

/* 解码通道 */
typedef struct hiVFMW_CHAN_S
{
    SINT32         s32BtlMemAddr;
	SINT32         s32BtlMemSize;
    SINT32         s32ChanID;       /* 通道ID */
    SINT32         s32IsOpen;       /* 0: 未打开，1: 打开 */
    SINT32         s32IsRun;        /* 0: 不被调度, 1: 运行，可被调度  */
    SINT32         s32Priority;     /* 优先级，0：最低优先级(从不调度) ~ 255(最高优先级，最优先调度) */
    SINT32         s32StopSyntax;  /* 停掉syntax解码，用于在stop通道的时候先停上游，让下游自由运行，把已生成
                                        的DecParam消耗干净，从而实现安全的通道stop和reset操作 */
    VDEC_CHAN_CAP_LEVEL_E eChanCapLevel;  /* 通道的能力级别 */
    UINT32         u32timeLastDecParamReady;
	
    VDEC_CHAN_CFG_S     stChanCfg;       /* 保存用户配置，便于查询接口实现 */
    STREAM_INTF_S  stStreamIntf;    /* 码流接口 */
    IMAGE_INTF_S   stImageIntf;     /* 图象接口 */
	FRAME_INTF_S   stFrameIntf;

    SINT32         s32SCDInstID;    /* 该通道对应的SCD实例ID */
    SINT32         s32VDMInstID;    /* 该通道对应的VDM实例ID */
    SINT32         s32FODInstID;    /* 该通道对应的FOD实例ID */

    SINT32         s32OneChanMem;  /*该通道内存由外部分配为一块，映射等操作时需要作为整体来处理*/
    MEM_RECORD_S   stChanMem;       /* 该通道的存储资源 */

//    SINT32         s32CreatWithMem;     /* 1:该通道内存资源由外部申请、外部释放 */
    MEM_RECORD_S   stChanMem_vdh;       /* 该通道的帧存存储资源 */
    MEM_RECORD_S   stChanMem_scd;       /* 该通道的SCD存储资源 */
    MEM_RECORD_S   stChanMem_ctx;       /* 该通道的上下文存储资源 */ 
    
//    SINT32         s32SelfAllocChanMem;  /* 标识通道存储资源是否是自己分配的，1: 自己分配, 0: 外部分配 */
    SINT32         s32SelfAllocChanMem_vdh;  /* 标识通道 vdh 是否是自己分配的，1: 自己分配, 0: 外部分配 */
    SINT32         s32SelfAllocChanMem_scd;  /* 标识通道 scd 是否是自己分配的，1: 自己分配, 0: 外部分配 */
    SINT32         s32SelfAllocChanMem_ctx;  /* 标识通道 ctx 是否是自己分配的，1: 自己分配, 0: 外部分配 */
    SINT32         s32VdmChanMemAddr;
    SINT32         s32VdmChanMemSize;    /* VDM通道所占据的存储空间大小 */
	SINT32         s32OffLineDnrMemAddr;
	SINT32         s32OffLineDNRMemSize; /* 离线DNR所占据的存储空间大小  */
    SINT32         s32ScdChanMemAddr;
    SINT32         s32SdecMemAddr;       /* 软解码所使用的存储空间，为DNR+VDM的空间 */
    SINT32         s32SdecMemSize;
    SINT32         s32ScdChanMemSize;    /* SCD通道所占据的存储空间大小 */

    SINT32         s32BpdChanMemAddr;
    SINT32         s32BpdChanMemSize;    /* BPD通道所占据的存储空间大小 */

    SINT32         s32Vp8SegIdChanMemAddr;
    SINT32         s32Vp8SegIdChanMemSize;    /* SegId通道所占据的存储空间大小 */	
	
	IMAGE          stRecentImg;     /* 最新IMAGE结构体 */

    SINT32         s32NoStreamFlag; /* 由于该通道没有足够码流导致未能生成解码参数decparam */
    SINT32         s32LastFrameIdPlus2;  /* 最后一帧输出时，用于记录最后一帧的image_id + 2 ，1D 转2D时，后面BTL/DNR会用到*/

    SINT32         s32CountNoDecparamFlag; /* 发现没有解码参数后开始计时标志 */    
    UINT32         u32NoDecparamStartTime;
	
    /* 用户态解码需要把VDM memory按帧映射上去. 创建时形成分割，以下信息记录帧存的分割和使用状况 */
    SINT32         s32UsrdecFrameUsed[MAX_USRDEC_FRAME_NUM];
    SINT32         s32UsrdecFramePhyAddr[MAX_USRDEC_FRAME_NUM];
    SINT32         s32UsrdecFrameSize;
    SINT32         s32UsrdecFrameNum;
    USRDEC_FRAME_DESC_S  stRecentUsrdecFrame;

    VID_STD_E      eVidStd;
    SYNTAX_CTX_S   stSynCtx;
    SYNTAX_EXTRA_DATA_S stSynExtraData;
	CHAN_DEC_CORE_E eDecCore;
    SM_INSTANCE_S   SmInstArray;
    FSP_INST_S      FspInst;

} VFMW_CHAN_S;

/* 线程的状态 */
typedef enum hiDSPSTATE_E
{
    DSP_STATE_NORMAL = 0,      /* DSP 还没有加载任何代码  */
    DSP_STATE_SCDLOWDLY,       /* DSP 已经加载了SCD低延的代码，说明已有一个通道处于低延迟模式 */
    DSP_STATE_AVS,             /* DSP 已经加载了AVS+的代码，说明已有一个通道正在跑AVS协议 */
    DSP_STATE_BUTT
} DSP_STATE_E;

/*======================================================================*/
/*  全局变量                                                            */
/*======================================================================*/
extern SINT32 (*g_event_report)(SINT32 InstID, SINT32 type, VOID* p_args );
extern VFMW_CHAN_STAT_S g_VfmwChanStat[MAX_CHAN_NUM];
extern VFMW_GLOBAL_STAT_S g_VfmwGlobalStat[MAX_VDH_NUM];
extern SINT32 g_VdmCharacter;
extern VDM_VERSION_E g_eVdmVersion;
extern SINT32  (*AcceleratorCharacter)(DECPARAM_INF_S *pDecParamInfo); 

/*======================================================================*/
/*  函数申明                                                            */
/*======================================================================*/
SINT32 VCTRL_OpenVfmw(SINT32 (*event_report)(SINT32 ChanID, SINT32 type, VOID* p_args ));
SINT32 VCTRL_StopVfmw(VOID);
SINT32 VCTRL_StartVfmw(VOID);
SINT32 VCTRL_CloseVfmw(VOID);
VOID VCTRL_Suspend(VOID);
VOID VCTRL_Resume(VOID);

#ifdef ENV_ARMLINUX_KERNEL
SINT32 VCTRL_RegisterSoftDecoder(iMediaSDEC_FUNC_S *pstSdecFunc);
VOID VCTRL_UnRegisterSoftDecoder(VOID);
#else
#define IRQ_HANDLED 0
#endif

SINT32 VCTRL_CreateChan(VDEC_CHAN_CAP_LEVEL_E eCapLevel, MEM_DESC_S *pChanMem);
SINT32 VCTRL_CreateChanWithOption(VDEC_CHAN_CAP_LEVEL_E eCapLevel, VDEC_CHAN_OPTION_S *pChanOption, SINT32 flag, SINT32 OneChanMemFlag);
SINT32 VCTRL_DestroyChan(SINT32 ChanID);
SINT32 VCTRL_DestroyChanWithOption(SINT32 ChanID);
//SINT32 VCTRL_DestroyChanWithMem(SINT32 ChanID);
SINT32 VCTRL_StartChan(SINT32 ChanID);
SINT32 VCTRL_StopChan(SINT32 ChanID);
SINT32 VCTRL_GetChanCfg(SINT32 ChanID, VDEC_CHAN_CFG_S *pstCfg);
SINT32 VCTRL_CmpConfigParameter(SINT32 ChanID, VDEC_CHAN_CFG_S *pstCfg);
SINT32 VCTRL_ConfigChan(SINT32 ChanID, VDEC_CHAN_CFG_S *pstCfg);
SINT32 VCTRL_ResetChanWithOption(SINT32 ChanID, VDEC_CHAN_RESET_OPTION_S *pOption);
SINT32 VCTRL_ReleaseStream(SINT32 ChanID);
SINT32 VCTRL_ResetChan(SINT32 ChanID);
SINT32 VCTRL_GetChanMemSize(VDEC_CHAN_CAP_LEVEL_E eCapLevel, SINT32 *VdmMemSize, SINT32 *ScdMemSize);
VOID VCTRL_GetChanState(SINT32 ChanID, VDEC_CHAN_STATE_S *pstChanState);
//VOID VCTRL_GetChanFrmState(SINT32 ChanID, VDEC_CHAN_FRMSTATUSINFO_S * pstChanFrmState);
SINT32 VCTRL_SetStreamInterface( SINT32 ChanID, STREAM_INTF_S *pstStreamIntf );
STREAM_INTF_S *VCTRL_GetStreamInterface(SINT32 ChanID);
SINT32 VCTRL_SetFrameInterface( SINT32 ChanID, FRAME_INTF_S *pstFrameIntf );
FRAME_INTF_S *VCTRL_GetFrameInterface( SINT32 ChanID);
#ifdef VFMW_BVT_SUPPORT
SINT32 VCTRL_SetFspFrameInterface(FSP_FRAME_INTF_S *pstFspFrameIntf);
#endif
VOID *VCTRL_GetSyntaxCtx(SINT32 ChanID);
IMAGE_INTF_S *VCTRL_GetImageInterface(SINT32 ChanID);
//SINT32 VCTRL_CreateChanWithMem(VDEC_CHAN_CAP_LEVEL_E eCapLevel, VDEC_CHAN_MEM_DETAIL_S *pChanMem);
//SINT32 VCTRL_GetChanDetailMemSize(VDEC_CHAN_CAP_LEVEL_E eCapLevel, SINT32 SupportBFrame, SINT32 MaxRefFrameNum, DETAIL_MEM_SIZE *pDetailMemSize);
SINT32 VCTRL_GetChanMemSizeWithOption(VDEC_CHAN_CAP_LEVEL_E eCapLevel, VDEC_CHAN_OPTION_S *pChanOption, DETAIL_MEM_SIZE *pDetailMemSize,SINT32 flag);
SINT32 VCTRL_GetStreamSize(SINT32 ChanID, SINT32 *pArgs);
SINT32 VCTRL_SetDiscardPicParam(SINT32 ChanID, VDEC_DISPIC_PARAM_S *pArgs);    //add by z00222166, 2012.11.20
//SINT32 VCTRL_OpenRawBuf( SINT32 ChanID, SINT32 RawBufSize );
//SINT32 VCTRL_CloseRawBuf( SINT32 ChanID);
//SINT32 VCTRL_MountRawBuf(SINT32 ChanID);
//SINT32 VCTRL_UnMountRawBuf(SINT32 ChanID);
//SINT32 VCTRL_ResetRawBuf( SINT32 ChanID);
//MEM_DESC_S VCTRL_MemAlloc(UINT8 * pu8MemName, UINT32 u32Len, UINT32 u32Align);
//SINT32 VCTRL_MemFree(MEM_DESC_S stMem);
UINT32 VCTRL_ArrangeMem(SINT32 ChanID, SINT32 MaxWidth, SINT32 MaxHeight, SINT32 MemAddr, SINT32 MemSize);
#ifdef  VFMW_USER_SUPPORT
SINT32  VCTRL_GetUsrdecFrame(SINT32 ChanID, MEM_DESC_S *pMem);
SINT32  VCTRL_PutUsrdecFrame(SINT32 ChanID, USRDEC_FRAME_DESC_S *pUsrdecFrame);
SINT32 USERDEC_Init(USER_CTX_S *pCtx, SYNTAX_EXTRA_DATA_S *pstExtraData);
SINT32 USERDEC_RecycleImage(USER_CTX_S *pCtx,UINT32 ImgID);
SINT32 VDH_PutUsrdecFrame(SINT32 ChanID, USRDEC_FRAME_DESC_S *pstUsrdecFrame);
#endif

SINT32 VCTRL_ChanDecparamInValidFlag(SINT32 ChanId);

VID_STD_E VCTRL_GetVidStd(SINT32 ChanId);
VOID *VCTRL_GetDecParam(SINT32 ChanId);
VOID VCTRL_VdmPostProc( SINT32 ChanId, SINT32 ErrRatio, UINT32 Mb0QpInCurrPic, LUMA_INFO_S *pLumaInfo);

SINT32 VCTRL_GetChanImage( SINT32 ChanID, IMAGE *pImage );
SINT32 VCTRL_ReleaseChanImage( SINT32 ChanID, IMAGE *pImage );

SINT32 VCTRL_RunProcess(VOID);
VOID VCTRL_InformVdmFree(SINT32 ChanID);
VOID VCTRL_ExtraWakeUpThread(VOID);
SINT32 VCTRL_FlushDecoder(SINT32 ChanID);
SINT32 VCTRL_GetImageBuffer( SINT32 ChanId );
SINT32 VCTRL_GetChanIDByCtx(VOID *pCtx);
SINT32 VCTRL_GetChanIDByMemAddr(SINT32 PhyAddr);
SINT32  VCTRL_IsChanDecable( SINT32 ChanID );
SINT32  VCTRL_IsChanSegEnough( SINT32 ChanID );
SINT32  VCTRL_IsChanActive( SINT32 ChanID );
VOID VCTRL_GetChanImgNum( SINT32 ChanID, SINT32 *pRefImgNum, SINT32 *pReadImgNum, SINT32 *pNewImgNum );
VDEC_CHAN_CAP_LEVEL_E  VCTRL_GetChanCapLevel(SINT32 ChanID);

SINT32 VCTRL_RegisterSoftDecoder(iMediaSDEC_FUNC_S *pstSdecFunc);
VOID VCTRL_UnRegisterSoftDecoder(VOID);

SINT32  VCTRL_ExtAccGetDecParam(DECPARAM_INF_S * pDecParamInfo);
VOID VCTRL_ExtAccPostProcess(SINT32 ChanID, SINT32 ErrorRatio, HI_U32 u32LumaPixSum);

SINT32 VCTRL_SetMoreGapEnable(SINT32 ChanID, SINT32 MoreGapEnable);

VOID VCTRL_MaskAllInt(VOID);
VOID VCTRL_EnableAllInt(VOID);
SINT32 VCTRL_SeekPts(SINT32 ChanID, UINT64 *pArgs);
#if defined(VFMW_SCD_LOWDLY_SUPPORT) || defined(VFMW_AVSPLUS_SUPPORT)
SINT32 VCTRL_LoadDspCode(SINT32 ChanID);
#endif
SINT32 VCTRL_ConfigFFFBSpeed(SINT32 ChanID, SINT32 *pArgs);
SINT32 VCTRL_ConfigPVRInfo(SINT32 ChanID, VFMW_CONTROLINFO_S *pArgs);

SINT32 VCTRL_GetChanMemInfo(SINT32 ChanId, MEM_RECORD_S *pstMemRec);
SINT32  VCTRL_SetDbgOption ( UINT32 opt, UINT8* p_args );

SINT32 VCTRL_GetChanWidth(SINT32 ChanID);
SINT32 VCTRL_GetChanHeight(SINT32 ChanID);
VOID VCTRL_SetVdecExtra(SINT32 new_extra_ref, SINT32 new_extra_disp);

SINT32 VCTRL_GetLastFrameIdPlus2(SINT32 ChanID);
VOID VCTRL_SetLastFrameIdPlus2(SINT32 ChanID, SINT32 Value);      

SINT32 VCTRL_OutputLastFrame(SINT32 ChanId);  
VOID VCTRL_SvdecLastFrame(SINT32 ChanId, UINT32 LastFrameID);

//输出s_pstVfmwChan供btl_drv.c使用
#if 0
IMAGE_VO_QUEUE getPstVoQueByChanId(SINT32 ChanID);
VDEC_CHAN_CFG_S getChanCfg(SINT32 ChanID);
#endif

#ifdef ENV_ARMLINUX_KERNEL
int   vfmw_proc_init(void);
void  vfmw_proc_exit(void);
#endif

#endif


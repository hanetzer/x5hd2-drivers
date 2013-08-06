#ifndef __VPSS_INSTANCE_H__
#define __VPSS_INSTANCE_H__

#include "hi_type.h"
#include"drv_vpss_ext.h"
#include "drv_mmz_ext.h"
#include "vpss_fb.h"
#include "linux/list.h"
#include "vpss_osal.h"
#include "vpss_alg.h"
#include "vpss_debug.h"

#define VPSS_PORT_MAX_NUMB 3   //控制单个实例最大输出PORT数目
#define VPSS_SOURCE_MAX_NUMB 6 //VPSS开辟的前级输入image信息缓存数目
#define VPSS_AUINFO_LENGTH 256  //vpss保留的算法信息字长

typedef struct semaphore  VPSS_INST_LOCK;
typedef struct hiVPSS_PORT_PRC_S{
    HI_S32 s32PortId;
    HI_BOOL bEnble;

    VPSS_FB_STATE_S stFbPrc;
    HI_DRV_VPSS_BUFLIST_CFG_S stBufListCfg;
    HI_DRV_PIX_FORMAT_E eFormat; /* Support ... */
    HI_S32  s32OutputWidth;
    HI_S32  s32OutputHeight;
    HI_DRV_COLOR_SPACE_E eDstCS;
    
    HI_DRV_ASPECT_RATIO_S stDispPixAR;
    HI_DRV_ASP_RAT_MODE_E eAspMode;
    HI_DRV_ASPECT_RATIO_S stCustmAR;
    
    HI_BOOL   bInterlaced;                /*送显 I/P*/
    HI_RECT_S stScreen;                   /*送显窗口大小 :0 0 即窗口大小等于输出宽高*/

    HI_U32 u32MaxFrameRate;  /* in 1/100 HZ  */
    HI_U32 u32OutCount;     /*输出帧计数*/

    /* 是否需要尽量保真，如果为TRUE，在输入输出分辨率一致的情况下走直通通道，保证指标能过 */
    HI_DRV_VPSS_PORT_PROCESS_S stProcCtrl;

    /* 与tunnel有关的配置 */
    HI_BOOL  bTunnelEnable;  /* 输出是否使能TUNNEL */
    HI_S32  s32SafeThr;    /* 安全水线，0~100，为输出帧已完成的百分比. 0表示随时可给后级，100表示完全完成才能给后级 */

    
}VPSS_PORT_PRC_S;
typedef struct hiVPSS_PORT_S{
    HI_S32 s32PortId;
    HI_BOOL bEnble;

    VPSS_FB_INFO_S stFrmInfo;
    
    HI_DRV_PIX_FORMAT_E eFormat; /* Support ... */
    HI_S32  s32OutputWidth;
    HI_S32  s32OutputHeight;
    HI_DRV_COLOR_SPACE_E eDstCS;
    
    HI_DRV_ASPECT_RATIO_S stDispPixAR;
    HI_DRV_ASP_RAT_MODE_E eAspMode;
    HI_DRV_ASPECT_RATIO_S stCustmAR;
    
    HI_BOOL   bInterlaced;                /*送显 I/P*/
    HI_RECT_S stScreen;                   /*送显窗口大小 :0 0 即窗口大小等于输出宽高*/

    HI_U32 u32MaxFrameRate;  /* in 1/100 HZ  */
    HI_U32 u32OutCount;     /*输出帧计数*/

    /* 是否需要尽量保真，如果为TRUE，在输入输出分辨率一致的情况下走直通通道，保证指标能过 */
    HI_DRV_VPSS_PORT_PROCESS_S stProcCtrl;

    /* 与tunnel有关的配置 */
    HI_BOOL  bTunnelEnable;  /* 输出是否使能TUNNEL */
    HI_S32  s32SafeThr;    /* 安全水线，0~100，为输出帧已完成的百分比. 0表示随时可给后级，100表示完全完成才能给后级 */

    
}VPSS_PORT_S;

typedef struct hiVPSS_IMAGE_NODE_S{
    HI_DRV_VIDEO_FRAME_S stSrcImage;
    LIST node;
}VPSS_IMAGE_NODE_S;

typedef struct hiVPSS_IMAGELIST_INFO_S{
    VPSS_OSAL_LOCK stEmptyListLock;
    VPSS_OSAL_LOCK stFulListLock;
    HI_U32 u32GetUsrTotal;
    HI_U32 u32GetUsrFailed;
    
    HI_U32 u32RelUsrTotal;
    HI_U32 u32RelUsrFailed;
    
    LIST stEmptyImageList;//VPSS_IMAGE_NODE_S
    LIST stFulImageList;//VPSS_IMAGE_NODE_S
    LIST* pstTarget_1; //指向UNDO-1
}VPSS_IMAGELIST_INFO_S;

typedef struct hiVPSS_IMAGELIST_STATE_S{
    HI_U32 u32TotalNumb;
    HI_U32 u32FulListNumb;
    HI_U32 u32EmptyListNumb;
    HI_U32 u32GetUsrTotal;
    HI_U32 u32GetUsrFailed;
    
    HI_U32 u32RelUsrTotal;
    HI_U32 u32RelUsrFailed;
    
    HI_U32 u32Target;
    HI_U32 u32FulList[VPSS_SOURCE_MAX_NUMB];
    HI_U32 u32EmptyList[VPSS_SOURCE_MAX_NUMB];
    HI_U32 u32List[VPSS_SOURCE_MAX_NUMB][2];
}VPSS_IMAGELIST_STATE_S;

typedef struct hiVPSS_INSTANCE_S{
    HI_S32  ID;
    HI_S32  s32Priority;    
    HI_U32  u32IsNewImage;
    VPSS_OSAL_LOCK stInstLock;

    /*
     *实例配置锁
     *用户设置实例配置先抢占锁，将用户配置写入stUsrCfg
     *线程启动前尝试抢锁，抢占成功后，刷新配置
     */
    HI_U32 u32IsNewCfg;
    HI_DRV_VPSS_CFG_S stUsrInstCfg;
    HI_DRV_VPSS_PORT_CFG_S stUsrPortCfg[DEF_HI_DRV_VPSS_PORT_MAX_NUMBER];
    VPSS_OSAL_SPIN stUsrSetSpin;

    VPSS_DBG_S stDbgCtrl;
    
    HI_U32 u32InRate;
    /*输入码流顶底场优先次序
     *初始值:0xffffffff
     *逐行:0xfffffffe
     *隔行:
     *
     *  来源1:码流自带
     *  来源2:算法检测
     *  优先相信算法检测值，改变时调整已入队image先后次序
     ***/
    HI_U32 u32RealTopFirst;
    HI_U32 u32NeedRstDei;
    HI_U32 u32Rwzb;
    
    HI_BOOL bAlwaysFlushSrc;

    VPSS_IMAGELIST_INFO_S stSrcImagesList;//存放获取的源图像指针，DEI会用到历史图像信息
        
    VPSS_PORT_S stPort[DEF_HI_DRV_VPSS_PORT_MAX_NUMBER];
        
    HI_U32 stAuInfo[VPSS_AUINFO_LENGTH];//VPSS_ALG_HISTORY_S
    
    HI_DRV_VPSS_PROCESS_S stProcCtrl;
    
    HI_HANDLE hDst;
    PFN_VPSS_CALLBACK pfUserCallBack;

    HI_DRV_VPSS_SOURCE_MODE_E eSrcImgMode;
    HI_DRV_VPSS_SOURCE_FUNC_S stSrcFuncs;
    
}VPSS_INSTANCE_S;


VPSS_PORT_S *VPSS_INST_GetPort(VPSS_INSTANCE_S * pstInstance, VPSS_HANDLE hPort);

HI_S32 VPSS_INST_GetUserImage(VPSS_INSTANCE_S *pstInstance,
                                VPSS_IMAGE_NODE_S *pstSrcImage);
/*
EMPTYLIST->FULLIST->UNDO->DONE->EMPTYLIST
*/
/*消费者: TASK*/

HI_DRV_VIDEO_FRAME_S *VPSS_INST_GetUndoImage(VPSS_INSTANCE_S *pstInstance);

HI_S32 VPSS_INST_RelDoneImage(VPSS_INSTANCE_S *pstInstance);

HI_S32 VPSS_INST_RelImageBuf(VPSS_INSTANCE_S *pstInstance,VPSS_IMAGE_NODE_S *pstDoneImageNode);

HI_BOOL VPSS_INST_CheckUndoImage(VPSS_INSTANCE_S *pstInstance);

//VPSS_INST_API_CompleteUndoImage
HI_S32 VPSS_INST_DelFulImage(VPSS_INSTANCE_S *pstInstance,
                            HI_DRV_VIDEO_FRAME_S *pstImage);
/*生产者:USRCALLBACK*/
VPSS_IMAGE_NODE_S* VPSS_INST_GetEmptyImage(VPSS_INSTANCE_S *pstInstance);

HI_S32 VPSS_INST_AddFulImage(VPSS_INSTANCE_S *pstInstance,
                                VPSS_IMAGE_NODE_S *pstUndoImage);


HI_U32 VPSS_INST_AddEmptyImage(VPSS_INSTANCE_S *pstInstance,
                                VPSS_IMAGE_NODE_S *pstUndoImage);
HI_S32 VPSS_INST_DelDoneImage(VPSS_INSTANCE_S *pstInstance,
                                HI_DRV_VIDEO_FRAME_S *pstImage);


HI_S32 VPSS_INST_Init(VPSS_INSTANCE_S *pstInstance,HI_DRV_VPSS_CFG_S *pstVpssCfg);
HI_S32 VPSS_INST_DelInit(VPSS_INSTANCE_S *pstInstance);
HI_S32 VPSS_INST_GetDefInstCfg(HI_DRV_VPSS_CFG_S *pstVpssCfg);
HI_S32 VPSS_INST_SetInstCfg(VPSS_INSTANCE_S *pstInstance,HI_DRV_VPSS_CFG_S *pstVpssCfg);
HI_U32 VPSS_INST_GetInstCfg(VPSS_INSTANCE_S *pstInstance,HI_DRV_VPSS_CFG_S *pstVpssCfg);
HI_S32 VPSS_INST_SetCallBack(VPSS_INSTANCE_S *pstInstance,HI_HANDLE hDst, PFN_VPSS_CALLBACK pfVpssCallback);


HI_S32 VPSS_INST_CreatePort(VPSS_INSTANCE_S *pstInstance,HI_DRV_VPSS_PORT_CFG_S *pstPortCfg,VPSS_HANDLE *phPort);
HI_S32 VPSS_INST_DestoryPort(VPSS_INSTANCE_S *pstInstance,VPSS_HANDLE hPort);
HI_U32 VPSS_INST_GetDefPortCfg(HI_DRV_VPSS_PORT_CFG_S *pstPortCfg);
HI_S32 VPSS_INST_GetPortCfg(VPSS_INSTANCE_S * pstInstance, VPSS_HANDLE hPort, HI_DRV_VPSS_PORT_CFG_S *pstPortCfg);
HI_S32 VPSS_INST_SetPortCfg(VPSS_INSTANCE_S * pstInstance, VPSS_HANDLE hPort, HI_DRV_VPSS_PORT_CFG_S *pstPortCfg);

HI_S32 VPSS_INST_RelPortFrame(VPSS_INSTANCE_S *pstInstance,VPSS_HANDLE hPort,HI_DRV_VIDEO_FRAME_S *pstFrame);
HI_S32 VPSS_INST_GetPortFrame(VPSS_INSTANCE_S *pstInstance,VPSS_HANDLE hPort,HI_DRV_VIDEO_FRAME_S *pstFrame);
HI_S32 VPSS_INST_EnablePort(VPSS_INSTANCE_S * pstInstance, VPSS_HANDLE hPort,HI_BOOL bEnPort);
HI_S32 VPSS_INST_CheckPortCfg(VPSS_INSTANCE_S * pstInstance, VPSS_HANDLE hPort,HI_DRV_VPSS_PORT_CFG_S *pstVpssPortCfg);

HI_S32 VPSS_INST_ReplyUserCommand(VPSS_INSTANCE_S * pstInstance,
                                    HI_DRV_VPSS_USER_COMMAND_E eCommand,
                                    HI_VOID *pArgs);



HI_BOOL VPSS_INST_CheckIsAvailable(VPSS_INSTANCE_S *pstInstance);

HI_S32 VPSS_INST_CompleteUndoImage(VPSS_INSTANCE_S *pstInstance);




HI_S32 VPSS_INST_GetFrmBuffer(VPSS_INSTANCE_S* pstInstance,VPSS_HANDLE hPort,
                    HI_DRV_VPSS_BUFLIST_CFG_S* pstBufCfg,VPSS_BUFFER_S *pstBuffer,
                    HI_U32 u32StoreH,HI_U32 u32StoreW);

HI_S32 VPSS_INST_RelFrmBuffer(VPSS_INSTANCE_S* pstInstance,VPSS_HANDLE  hPort,
                                HI_DRV_VPSS_BUFLIST_CFG_S   *pstBufCfg,
                                MMZ_BUFFER_S *pstMMZBuf);
                                
HI_S32 VPSS_INST_ReportNewFrm(VPSS_INSTANCE_S* pstInstance,
                                VPSS_HANDLE  hPort,HI_DRV_VIDEO_FRAME_S *pstFrm);

HI_S32 VPSS_INST_GetSrcListState(VPSS_INSTANCE_S* pstInstance,VPSS_IMAGELIST_STATE_S *pstListState);


HI_S32 VPSS_INST_GetPortListState(VPSS_INSTANCE_S* pstInstance,VPSS_HANDLE hPort,HI_DRV_VPSS_PORT_BUFLIST_STATE_S *pstListState);
HI_S32 VPSS_INST_GetDeiAddr(VPSS_INSTANCE_S* pstInstance,
                            HI_DRV_VID_FRAME_ADDR_S *pstFieldAddr,
                            HI_DRV_VPSS_DIE_MODE_E eDeiMode,
                            HI_DRV_BUF_ADDR_E eLReye);


HI_S32 VPSS_INST_CorrectImgListOrder(VPSS_INSTANCE_S* pstInstance,HI_U32 u32RealTopFirst);


HI_S32 VPSS_INST_GetVc1Info(VPSS_INSTANCE_S* pstInstance,VPSS_ALG_VC1INFO_S *pstVc1Info,HI_DRV_VPSS_DIE_MODE_E eDeiMode);

HI_S32 VPSS_INST_Reset(VPSS_INSTANCE_S *pstInstance);


HI_S32 VPSS_INST_CorrectProgInfo(VPSS_INSTANCE_S *pstInstance,HI_DRV_VIDEO_FRAME_S *pImage);


HI_S32 VPSS_INST_ChangeInRate(VPSS_INSTANCE_S *pstInstance,HI_U32 u32InRate);

HI_BOOL VPSS_INST_CheckIsDropped(VPSS_INSTANCE_S *pstInstance,HI_U32 u32OutRate,HI_U32 u32OutCount);


HI_BOOL VPSS_INST_CheckAllDone(VPSS_INSTANCE_S *pstInstance);


HI_S32 VPSS_INST_CheckNeedRstDei(VPSS_INSTANCE_S *pstInstance,HI_DRV_VIDEO_FRAME_S *pImage);


HI_S32 VPSS_INST_SyncUsrCfg(VPSS_INSTANCE_S *pstInstance);


HI_S32 VPSS_INST_GetPortPrc(VPSS_INSTANCE_S* pstInstance,VPSS_HANDLE hPort,VPSS_PORT_PRC_S *pstPortPrc);



HI_S32 VPSS_INST_StoreDeiData(VPSS_INSTANCE_S *pstInstance,ALG_FMD_RTL_STATPARA_S *pstDeiData);
HI_S32 VPSS_INST_GetDeiData(VPSS_INSTANCE_S *pstInstance,ALG_FMD_RTL_STATPARA_S *pstDeiData);
#endif
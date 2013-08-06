#ifndef __DRV_VPSS_EXT_H__
#define __DRV_VPSS_EXT_H__

#include "hi_type.h"
#include "hi_drv_vpss.h"



typedef HI_S32  (*FN_VPSS_GlobalInit)(HI_VOID);
typedef HI_S32  (*FN_VPSS_GlobalDeInit)(HI_VOID);
typedef HI_S32  (*FN_VPSS_GetDefaultCfg)(HI_DRV_VPSS_CFG_S *pstVpssCfg);
typedef HI_S32  (*FN_VPSS_CreateVpss)(HI_DRV_VPSS_CFG_S *pstVpssCfg,VPSS_HANDLE *phVPSS);
typedef HI_S32  (*FN_VPSS_DestroyVpss)(VPSS_HANDLE hVPSS);


typedef HI_S32  (*FN_VPSS_SetVpssCfg)(VPSS_HANDLE hVPSS, HI_DRV_VPSS_CFG_S *pstVpssCfg);
typedef HI_S32  (*FN_VPSS_GetVpssCfg)(VPSS_HANDLE hVPSS, HI_DRV_VPSS_CFG_S *pstVpssCfg);

typedef HI_S32  (*FN_VPSS_GetDefaultPortCfg)(HI_DRV_VPSS_PORT_CFG_S *pstVpssPortCfg);
typedef HI_S32  (*FN_VPSS_CreatePort)(VPSS_HANDLE hVPSS,HI_DRV_VPSS_PORT_CFG_S *pstVpssPortCfg,VPSS_HANDLE *phPort);
typedef HI_S32  (*FN_VPSS_DestroyPort)(VPSS_HANDLE hPort);

typedef HI_S32  (*FN_VPSS_GetPortCfg)(VPSS_HANDLE hPort, HI_DRV_VPSS_PORT_CFG_S *pstVpssPortCfg);
typedef HI_S32  (*FN_VPSS_SetPortCfg)(VPSS_HANDLE hPort, HI_DRV_VPSS_PORT_CFG_S *pstVpssPortCfg);

typedef HI_S32  (*FN_VPSS_EnablePort)(VPSS_HANDLE hPort, HI_BOOL bEnable);

typedef HI_S32  (*FN_VPSS_SendCommand)(VPSS_HANDLE hVPSS, HI_DRV_VPSS_USER_COMMAND_E eCommand, HI_VOID *pArgs);

typedef HI_S32  (*FN_VPSS_GetPortFrame)(VPSS_HANDLE hPort, HI_DRV_VIDEO_FRAME_S *pstVpssFrame);  

typedef HI_S32  (*FN_VPSS_RelPortFrame)(VPSS_HANDLE hPort, HI_DRV_VIDEO_FRAME_S *pstVpssFrame);

typedef HI_S32  (*FN_VPSS_GetPortBufListState)(VPSS_HANDLE hPort, HI_DRV_VPSS_PORT_BUFLIST_STATE_S *pstVpssBufListState);
typedef HI_S32  (*FN_VPSS_CheckPortBufListFul)(VPSS_HANDLE hPort);
typedef HI_S32  (*FN_VPSS_SetSourceMode)(VPSS_HANDLE hVPSS,
                          HI_DRV_VPSS_SOURCE_MODE_E eSrcMode,
                          HI_DRV_VPSS_SOURCE_FUNC_S* pstRegistSrcFunc);

typedef HI_S32  (*FN_VPSS_PutImage)(VPSS_HANDLE hVPSS,HI_DRV_VIDEO_FRAME_S *pstImage);
typedef HI_S32  (*FN_VPSS_GetImage)(VPSS_HANDLE hVPSS,HI_DRV_VIDEO_FRAME_S *pstImage);
typedef HI_S32  (*FN_VPSS_RegistHook)(VPSS_HANDLE hVPSS, HI_HANDLE hDst, PFN_VPSS_CALLBACK pfVpssCallback);


typedef struct
{
    FN_VPSS_GlobalInit      pfnVpssGlobalInit;
    FN_VPSS_GlobalDeInit    pfnVpssGlobalDeInit;
    
    FN_VPSS_GetDefaultCfg   pfnVpssGetDefaultCfg;
    FN_VPSS_CreateVpss      pfnVpssCreateVpss;
    FN_VPSS_DestroyVpss     pfnVpssDestroyVpss;
    FN_VPSS_SetVpssCfg      pfnVpssSetVpssCfg;
    FN_VPSS_GetVpssCfg      pfnVpssGetVpssCfg;

    FN_VPSS_GetDefaultPortCfg   pfnVpssGetDefaultPortCfg;
    FN_VPSS_CreatePort      pfnVpssCreatePort;
    FN_VPSS_DestroyPort     pfnVpssDestroyPort;
    FN_VPSS_GetPortCfg      pfnVpssGetPortCfg;
    FN_VPSS_SetPortCfg      pfnVpssSetPortCfg;
    FN_VPSS_EnablePort      pfnVpssEnablePort;

    FN_VPSS_SendCommand     pfnVpssSendCommand;

    FN_VPSS_GetPortFrame    pfnVpssGetPortFrame;     
    FN_VPSS_RelPortFrame    pfnVpssRelPortFrame;

    FN_VPSS_GetPortBufListState     pfnVpssGetPortBufListState;
    FN_VPSS_CheckPortBufListFul     pfnVpssCheckPortBufListFul;

    FN_VPSS_SetSourceMode   pfnVpssSetSourceMode;
    FN_VPSS_PutImage        pfnVpssPutImage;
    FN_VPSS_GetImage        pfnVpssGetImage;

    FN_VPSS_RegistHook      pfnVpssRegistHook;
    
} VPSS_EXPORT_FUNC_S;


HI_S32 VPSS_DRV_Init(HI_VOID);
HI_VOID VPSS_DRV_Exit(HI_VOID);

HI_S32 VPSS_DRV_ModInit(HI_VOID);
HI_VOID VPSS_DRV_ModExit(HI_VOID);
//================================================  接口函数 =================================================
/* 全局打开/关闭，处理VPSS公共资源和全局上下文 */

HI_S32 HI_DRV_VPSS_GlobalInit(HI_VOID);
HI_S32 HI_DRV_VPSS_GlobalDeInit(HI_VOID);

/* VPSS实例接口 */
/*
    CFG内属性有两种
    一种是实例运行过程中的可动态配置项
    一种是实例初始化时候静态配置
*/
HI_S32  HI_DRV_VPSS_GetDefaultCfg(HI_DRV_VPSS_CFG_S *pstVpssCfg);

HI_S32  HI_DRV_VPSS_CreateVpss(HI_DRV_VPSS_CFG_S *pstVpssCfg,VPSS_HANDLE *phVPSS);
HI_S32  HI_DRV_VPSS_DestroyVpss(VPSS_HANDLE hVPSS);

HI_S32  HI_DRV_VPSS_SetVpssCfg(VPSS_HANDLE hVPSS, HI_DRV_VPSS_CFG_S *pstVpssCfg);
HI_S32  HI_DRV_VPSS_GetVpssCfg(VPSS_HANDLE hVPSS, HI_DRV_VPSS_CFG_S *pstVpssCfg);

/* Port接口 */
HI_S32  HI_DRV_VPSS_GetDefaultPortCfg(HI_DRV_VPSS_PORT_CFG_S *pstVpssPortCfg);

HI_S32  HI_DRV_VPSS_CreatePort(VPSS_HANDLE hVPSS,HI_DRV_VPSS_PORT_CFG_S *pstVpssPortCfg,VPSS_HANDLE *phPort);
HI_S32  HI_DRV_VPSS_DestroyPort(VPSS_HANDLE hPort);

HI_S32  HI_DRV_VPSS_GetPortCfg(VPSS_HANDLE hPort, HI_DRV_VPSS_PORT_CFG_S *pstVpssPortCfg);
HI_S32  HI_DRV_VPSS_SetPortCfg(VPSS_HANDLE hPort, HI_DRV_VPSS_PORT_CFG_S *pstVpssPortCfg);

HI_S32  HI_DRV_VPSS_EnablePort(VPSS_HANDLE hPort, HI_BOOL bEnable);




HI_S32  HI_DRV_VPSS_SendCommand(VPSS_HANDLE hVPSS, HI_DRV_VPSS_USER_COMMAND_E eCommand, HI_VOID *pArgs);

/* 输出帧操作接口 *///无论何种buf管理机制，buf描述子都在port的队列上
HI_S32  HI_DRV_VPSS_GetPortFrame(VPSS_HANDLE hPort, HI_DRV_VIDEO_FRAME_S *pstVpssFrame);  
HI_S32  HI_DRV_VPSS_RelPortFrame(VPSS_HANDLE hPort, HI_DRV_VIDEO_FRAME_S *pstVpssFrame);
  
HI_S32  HI_DRV_VPSS_GetPortBufListState(VPSS_HANDLE hPort, HI_DRV_VPSS_PORT_BUFLIST_STATE_S *pstVpssBufListState);
HI_BOOL  HI_DRV_VPSS_CheckPortBufListFul(VPSS_HANDLE hPort);

HI_S32 HI_DRV_VPSS_SetSourceMode(VPSS_HANDLE hVPSS,
                          HI_DRV_VPSS_SOURCE_MODE_E eSrcMode,
                          HI_DRV_VPSS_SOURCE_FUNC_S* pstRegistSrcFunc);

/*推模式，用户调用这两个接口收发待处理IMAGE*/
HI_S32 HI_DRV_VPSS_PutImage(VPSS_HANDLE hVPSS,HI_DRV_VIDEO_FRAME_S *pstImage);
HI_S32 HI_DRV_VPSS_GetImage(VPSS_HANDLE hVPSS,HI_DRV_VIDEO_FRAME_S *pstImage);


HI_S32  HI_DRV_VPSS_RegistHook(VPSS_HANDLE hVPSS, HI_HANDLE hDst, PFN_VPSS_CALLBACK pfVpssCallback);

#endif


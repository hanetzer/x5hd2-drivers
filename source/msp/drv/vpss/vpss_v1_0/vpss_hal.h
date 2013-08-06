#ifndef __VPSS_HAL_H__
#define __VPSS_HAL_H__
#include"hi_type.h"

#include "drv_mmz_ext.h"

#include"vpss_alg.h"

#include"hi_drv_vpss.h"

#define VPSS_S40
//#define VPSS_CV200 

#ifdef VPSS_S40
#include"vpss_reg_s40.h"
#endif

#ifdef VPSS_CV200
#include"vpss_reg_cv200.h"
#endif

#define HAL_VERSION_1 0x101
#define HAL_VERSION_2 0x102

#define HAL_NODE_MAX 2
/*HAL 层的信息 */

/*
版本号
寄存器基地址
寄存器表单物理地址
寄存器表单虚拟地址
*/
typedef struct hiVPSS_HAL_CFG_S{
    HI_U32 u32LogicVersion;    
    HI_U32 u32BaseRegAddr;
    HI_U32 u32AppPhyAddr[HAL_NODE_MAX];
    HI_U32 u32AppVirtualAddr[HAL_NODE_MAX];
    MMZ_BUFFER_S stRegBuf[HAL_NODE_MAX];

    /*寄存器中各输出port的占用情况
     *[0]: HD
     *[1]: STR
     *[2]: SD
     * =1:已分配
     * =0:未分配
     */
    HI_U32 u32RegPortAlloc[DEF_HI_DRV_VPSS_PORT_MAX_NUMBER];
}VPSS_HAL_INFO_S;


/*

HAL Au的适配
芯片版本外部宏会传入

HAL层保留HAL通用接口

与硬件密切相关的寄存器操作放到REG
VPSS_HAL_Init
VPSS_HAL_DelInit
VPSS_HAL_GetCaps
VPSS_HAL_SetHalCfg
VPSS_HAL_SetIntMask
VPSS_HAL_ClearIntState
VPSS_HAL_GetIntState
HAL抽象的软件行为

HAL_Init
HAL_DelInit
HAL_GetLogicCapability


软件可配置项与硬件相关


VPSS从HAL要获取的能力集
1.最大通道数
2.各PORT能力集

*/
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    tunnel_strmDet          : 1  ; // [0] 
        unsigned int    tunnel_dei              : 1  ; // [1] 
        unsigned int    tunnel_dbdr             : 1  ; // [2]
        unsigned int    tunnel_accm             : 1  ; // [3]
        unsigned int    tunnel_reserve          : 4  ; // [4..7]
        
        unsigned int    port1_support           : 1  ; // [8]
        unsigned int    port1_zme               : 1  ; // [9] 
        unsigned int    port1_lba               : 1  ; // [10]
        unsigned int    port1_csc               : 1  ; // [11] 
        unsigned int    port1_crop              : 1  ; // [12] 
        unsigned int    port1_fidelity          : 1  ; // [13] 
        unsigned int    port1_reserve           : 2  ; // [14..15] 
        
        unsigned int    port2_support           : 1  ; // [16]
        unsigned int    port2_zme               : 1  ; // [17] 
        unsigned int    port2_lba               : 1  ; // [18]
        unsigned int    port2_csc               : 1  ; // [19]
        unsigned int    port2_crop              : 1  ; // [20]  
        unsigned int    port2_fidelity          : 1  ; // [21] 
        unsigned int    port2_reserve           : 2  ; // [22..23] 
        
        unsigned int    port3_support           : 1  ; // [24]
        unsigned int    port3_zme               : 1  ; // [25] 
        unsigned int    port3_lba               : 1  ; // [26]
        unsigned int    port3_csc               : 1  ; // [27]
        unsigned int    port3_crop              : 1  ; // [28]  
        unsigned int    port3_fidelity          : 1  ; // [29] 
        unsigned int    port3_reserve           : 2  ; // [30..31] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VPSS_HAL_CAPS;

/*
    HAL的对外接口
*/
typedef struct hiVPSS_HAL_CAP_S{
    U_VPSS_HAL_CAPS u32Caps;
    
    HI_S32 (*PFN_VPSS_HAL_SetHalCfg)(VPSS_ALG_CFG_S *pstAlgCfg,HI_U32 u32NodeID);
    
    HI_S32 (*PFN_VPSS_HAL_StartLogic)(HI_BOOL bTwoNode);
    
    HI_S32 (*PFN_VPSS_HAL_SetIntMask)(HI_U32 u32Data);
    
    HI_S32 (*PFN_VPSS_HAL_ClearIntState)(HI_U32 u32Data);

    HI_S32 (*PFN_VPSS_HAL_GetIntState)(HI_U32 *pData);
        
}VPSS_HAL_CAP_S;

/*通用接口*/
HI_S32 VPSS_HAL_Init(HI_U32 u32LogicVersion,VPSS_HAL_CAP_S *pstHalCaps);

HI_S32 VPSS_HAL_DelInit(HI_VOID);
HI_S32 VPSS_HAL_CloseClock(HI_VOID);   
HI_S32 VPSS_HAL_OpenClock(HI_VOID);     

/*需要根据逻辑版本适配的对外接口*/

HI_S32 VPSS_HAL_StartLogic(HI_BOOL bTwoNode);

HI_S32 VPSS_HAL_SetIntMask(HI_U32 u32Data);

HI_S32 VPSS_HAL_SetTimeOut(HI_U32 u32TimeOut);

HI_S32 VPSS_HAL_GetIntState(HI_U32 *pData);

HI_S32 VPSS_HAL_GetCycleCnt(HI_U32 *pCnt);

HI_S32 VPSS_HAL_ClearIntState(HI_U32 u32Data);

HI_S32 VPSS_HAL_SetHalCfg(VPSS_ALG_CFG_S *pstAlgCfg,HI_U32 u32NodeID);

/*内部实现*/
HI_S32 VPSS_HAL_GetCaps(HI_U32 u32DrvVersion,VPSS_HAL_CAP_S *pstHalCaps);

HI_S32 VPSS_HAL_SetSrcImg(VPSS_ALG_CFG_S *pstAlgCfg,HI_U32 u32NodeID);

HI_S32 VPSS_HAL_SetDstFrm(HI_U32 u32PortID,VPSS_REG_PORT_E ePort,
                            VPSS_ALG_CFG_S *pstAlgCfg,HI_U32 u32NodeID);

HI_S32 VPSS_HAL_SetDeiCfg(VPSS_ALG_CFG_S *pstAlgCfg,HI_U32 u32NodeID);

HI_S32 VPSS_HAL_SetUVCfg(VPSS_ALG_CFG_S *pstAlgCfg,HI_U32 u32NodeID);

HI_S32 VPSS_HAL_SetDnrCfg(VPSS_ALG_CFG_S *pstAlgCfg,HI_U32 u32NodeID);

HI_S32 VPSS_HAL_SetZmeCfg(HI_U32 u32PortID,VPSS_REG_PORT_E ePort,
                            VPSS_ALG_CFG_S *pstAlgCfg,HI_U32 u32NodeID); 

HI_S32 VPSS_HAL_SetRwzbCfg(VPSS_ALG_CFG_S *pstAlgCfg,HI_U32 u32NodeID);

HI_VOID VPSS_HAL_GetDetPixel(HI_U32 BlkNum, HI_U8* pstData);

HI_S32 VPSS_HAL_SetVC1Cfg(VPSS_ALG_CFG_S *pstAlgCfg,HI_U32 u32NodeID);

HI_S32 VPSS_HAL_GetDeiDate(ALG_FMD_RTL_STATPARA_S *pstFmdRtlStatPara);


HI_S32 VPSS_HAL_SetSharpCfg(HI_U32 u32PortID,VPSS_REG_PORT_E ePort,VPSS_ALG_CFG_S *pstAlgCfg,HI_U32 u32NodeID); 
HI_S32 VPSS_HAL_SetAspCfg(HI_U32 u32PortID,VPSS_REG_PORT_E ePort,
                            VPSS_ALG_CFG_S *pstAlgCfg,HI_U32 u32NodeID);
HI_S32 VPSS_HAL_SetSDFidelity(HI_BOOL bEnFidelity,HI_U32 u32NodeID);

#endif

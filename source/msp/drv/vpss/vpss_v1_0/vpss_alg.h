#ifndef __VPSS_ALG_H__
#define __VPSS_ALG_H__

#include"hi_type.h"

#include"hi_drv_video.h"
#include"drv_vpss_ext.h"
#include"vpss_alg_zme.h"
#include"vpss_alg_dei.h"
#include"vpss_alg_fmd.h"
#include"vpss_alg_rwzb.h"

#include"vpss_alg_ratio.h"
#include"vpss_alg_dnr.h"
#include"vpss_alg_sharp.h"
#include"vpss_reg_struct.h"
#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

#define ALG_VERSION_1 0x101
#define ALG_VERSION_2 0x102
#if 0 
typedef struct hiVPSS_ALG_EXT_S{
    HI_S32 (*PFN_VPSS_HAL_SetHalCfg)(VPSS_ALG_CFG_S *pstAlgCfg);
    
    HI_S32 (*PFN_VPSS_HAL_StartLogic)(HI_VOID);
    
    HI_S32 (*PFN_VPSS_HAL_SetIntMask)(HI_U32 u32Data);
    
    HI_S32 (*PFN_VPSS_HAL_ClearIntState)(HI_U32 u32Data);

    HI_S32 (*PFN_VPSS_HAL_GetIntState)(HI_U32 *pData);
        
}VPSS_ALG_EXT_S;
#endif
typedef struct hiVPSS_ALG_FRMCFG_S{
    HI_DRV_BUF_ADDR_E eLReye;
    HI_DRV_VID_FRAME_ADDR_S stBufAddr[HI_DRV_BUF_ADDR_MAX]; 
    HI_U32  u32Width;
    HI_U32  u32Height;
    HI_U32  u32ZmeWidth;
    HI_U32  u32ZmeHeight;
    HI_DRV_PIX_FORMAT_E  ePixFormat;
    HI_BOOL bProgressive;                    
	HI_DRV_FIELD_MODE_E  enFieldMode;
    HI_BOOL bTopFieldFirst;

    HI_DRV_ASPECT_RATIO_S stDispPixAR;

    HI_RECT_S stCropRect;
}VPSS_ALG_FRMCFG_S;

typedef struct hiVPSS_ALG_DEICFG_S{

    HI_BOOL bDei;
    ALG_DEI_DRV_PARA_S stDeiPara;
    /*
        0,1,2:PreFieldAddr
        3,4,5:AfterFieldAddr
     */
    HI_DRV_VID_FRAME_ADDR_S u32FieldAddr[6];
    
    ALG_DEI_RTL_OUTPARA_S stDeiOutPara;
    
    ALG_DEI_RTL_PARA_S *pstDeiDefaultPara;
}VPSS_ALG_DEICFG_S;

typedef struct hiVPSS_ALG_VC1INFO_S{
    HI_U8 u8PicStructure;     /**< 0: frame, 1: top, 2: bottom, 3: mbaff, 4: field pair */
    HI_U8 u8PicQPEnable;
    HI_U8 u8ChromaFormatIdc;  /**< 0: yuv400, 1: yuv420 */
    HI_U8 u8VC1Profile;
    
    HI_S32 s32QPY;
    HI_S32 s32QPU;
    HI_S32 s32QPV;
    HI_S32 s32RangedFrm;
    
    HI_U8 u8RangeMapYFlag;
    HI_U8 u8RangeMapY;
    HI_U8 u8RangeMapUVFlag;
    HI_U8 u8RangeMapUV;
    HI_U8 u8BtmRangeMapYFlag;
    HI_U8 u8BtmRangeMapY;
    HI_U8 u8BtmRangeMapUVFlag;
    HI_U8 u8BtmRangeMapUV;
}VPSS_ALG_VC1INFO_S;

typedef struct hiVPSS_ALG_VC1CFG_S{
    HI_U32 u32EnVc1;

    /*
     *0:PreFrame
     *1:CurFrame
     *2:NextFrame
     */
    VPSS_ALG_VC1INFO_S stVc1Info[3];
}VPSS_ALG_VC1CFG_S;

typedef struct hiVPSS_ALG_TUNNELCFG_S{
    VPSS_ALG_DEICFG_S stDeiCfg;
    ALG_DNR_RTL_PARA_S stDnrCfg;
    VPSS_ALG_RWZBCFG_S stRwzbCfg;
    VPSS_ALG_VC1CFG_S stVC1Cfg;
    HI_U32 u32EnUVCovert;
}VPSS_ALG_TUNNELCFG_S;


typedef struct hiVPSS_ALG_PortCFG_S{
    VPSS_REG_PORT_E eRegPort;
    
    HI_BOOL bFidelity;
    
    VPSS_ALG_FRMCFG_S stDstFrmInfo;

    ALG_VZME_RTL_PARA_S stZmeCfg;
    ALG_VTI_RTL_PARA_S stSharpCfg;
	
	
    ALG_RATIO_OUT_PARA_S stAspCfg;
    
}VPSS_ALG_PortCFG_S;

typedef struct hiVPSS_ALG_CFG_S{
    /*输入输出信息*/
    VPSS_ALG_FRMCFG_S stSrcImgInfo;

    VPSS_ALG_TUNNELCFG_S stAuTunnelCfg;
    
    VPSS_ALG_PortCFG_S stAuPortCfg[DEF_HI_DRV_VPSS_PORT_MAX_NUMBER];
    
    /*算法配置*/
}VPSS_ALG_CFG_S;

typedef struct hiVPSS_ALG_INFO_S{

    /*DET info*/
    DET_INFO_S stDetInfo;

    /*DEI 历史计算信息*/
    ALG_DEI_MEM_S  stDeiMem;

    /*DEI 历史逻辑统计信息*/
    ALG_FMD_RTL_STATPARA_S stFmdRtlStatPara; /*需要从逻辑读的统计信息*/
    HI_U32 reserve1;
}VPSS_ALG_INFO_S;

typedef struct hiVPSS_ALG_CTRL_S{
    /*
     算法公共资源管理
     1.算法保留信息
     2.算法开辟的资源
     3.算法能力集
     4.算法接口
     */
     /*zme 系数*/
    ALG_VZME_MEM_S stZmeMem; 
    
    /*dei 默认配置水线*/
    ALG_DEI_RTL_PARA_S stDeiRtlPara;
}VPSS_ALG_CTRL_S;

/*算法公共资源的初始化和销毁*/
HI_S32 VPSS_ALG_Init(VPSS_ALG_CTRL_S *pstAlgCtrl);
HI_S32 VPSS_ALG_DelInit(VPSS_ALG_CTRL_S *pstAlgCtrl);

/*单个实例算法处理信息的初始化和销毁*/
HI_S32 VPSS_ALG_InitAuInfo(HI_U32 u32InfoAddr);
HI_S32 VPSS_ALG_DeInitAuInfo(HI_U32 u32InfoAddr);

HI_S32 VPSS_ALG_SetFrameInfo(VPSS_ALG_FRMCFG_S* pstFrmCfg,HI_DRV_VIDEO_FRAME_S *pstFrm);


HI_S32 VPSS_ALG_GetRwzbCfg(HI_U32 u32InfoAddr,VPSS_ALG_RWZBCFG_S *pstDeiCfg,
                                VPSS_ALG_FRMCFG_S *pstImgCfg);

HI_S32 VPSS_ALG_GetDeiCfg(HI_DRV_VPSS_DIE_MODE_E eDeiMode,  /*DEI模式*/
                            HI_U32 u32AuInfoAddr,        /*DEI历史信息*/
                            VPSS_ALG_DEICFG_S *pstDeiCfg,    /*DEI配置*/
                            VPSS_ALG_CTRL_S   *pstAlgCtrl);  /*DEI默认配置*/

HI_S32 VPSS_ALG_GetZmeCfg(VPSS_ALG_FRMCFG_S *pstSrcImg,VPSS_ALG_FRMCFG_S *pstDstFrm,
                        ALG_VZME_RTL_PARA_S *pstZmeCfg,VPSS_ALG_CTRL_S   *pstAlgCtrl);   
                        
HI_S32 VPSS_ALG_GetAspCfg(ALG_RATIO_DRV_PARA_S *pstAspDrvPara,
                        HI_DRV_ASP_RAT_MODE_E eAspMode,HI_RECT_S *pstScreen,
                        ALG_RATIO_OUT_PARA_S *pstAspCfg);
HI_U32 VPSS_ALG_GetRwzbInfo(HI_U32 u32InfoAddr);

HI_U32 VPSS_ALG_StoreDeiData(HI_U32 u32InfoAddr,ALG_FMD_RTL_STATPARA_S *pstDeiData);

HI_U32 VPSS_ALG_GetDeiData(HI_U32 u32InfoAddr,ALG_FMD_RTL_STATPARA_S *pstDeiData);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif  /* __VO_EXT_H__ */

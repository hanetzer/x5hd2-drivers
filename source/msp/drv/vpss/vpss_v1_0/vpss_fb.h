#ifndef __VPSS_BUFFER_H__
#define __VPSS_BUFFER_H__


#include <linux/list.h>
#include "drv_mmz_ext.h"
#include"drv_vpss_ext.h"
#include "vpss_osal.h"
#include "vpss_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

typedef struct hiVPSS_BUFFER_S{
    MMZ_BUFFER_S stMMZBuf;
    HI_U32 u32Stride;
}VPSS_BUFFER_S;

typedef struct hiVPSS_FRAME_NODE_S{
    HI_DRV_VIDEO_FRAME_S stOutFrame;
    VPSS_BUFFER_S stBuffer;
    LIST node;
}VPSS_FB_NODE_S;

typedef struct hiVPSS_FB_INFO_S{
    HI_DRV_VPSS_BUFLIST_CFG_S  stBufListCfg;
    
    HI_U32 u32GetTotal;
    HI_U32 u32GetFail;
    
    HI_U32 u32RelTotal;
    HI_U32 u32RelFail;
    
    unsigned long ulStart; 
    HI_U32 u32GetHZ;
    HI_U32 u32GetLast;
    
    HI_U32 u32ListFul;
    //VPSS_OSAL_LOCK stFulBufLock;
    VPSS_OSAL_SPIN stFulBufSpin;
    //VPSS_OSAL_LOCK stEmptyBufLock;
    VPSS_OSAL_SPIN stEmptyBufSpin;
    LIST stEmptyFrmList;//VPSS_FB_NODE_S
    LIST stFulFrmList;//VPSS_FB_NODE_S
    LIST* pstTarget_1;
}VPSS_FB_INFO_S;

typedef struct hiVPSS_FB_STATE_S{
    HI_U32 u32TotalNumb;
    HI_U32 u32EmptyListNumb;
    HI_U32 u32FulListNumb;
    
    HI_U32 u32GetTotal;
    HI_U32 u32GetFail;
    HI_U32 u32RelTotal;
    HI_U32 u32RelFail;
    HI_U32 u32GetHZ;
    
    HI_U32 u32ListFul;
    HI_U32 u32Target_1;
    HI_U32 u32List[DEF_HI_DRV_VPSS_PORT_BUFFER_MAX_NUMBER][2];
    HI_U32 u32FulList[DEF_HI_DRV_VPSS_PORT_BUFFER_MAX_NUMBER];
    HI_U32 u32EmptyList[DEF_HI_DRV_VPSS_PORT_BUFFER_MAX_NUMBER];
}VPSS_FB_STATE_S;

HI_S32 VPSS_FB_Init(VPSS_FB_INFO_S *pstFrameList,HI_DRV_VPSS_BUFLIST_CFG_S *pstBufListCfg);
HI_S32 VPSS_FB_DelInit(VPSS_FB_INFO_S *pstFrameList);

//消费者INSTANCE
HI_S32 VPSS_FB_GetFulFrmBuf(VPSS_FB_INFO_S *pstFrameList,HI_DRV_VIDEO_FRAME_S *pstFrame,HI_CHAR* pchFile);
HI_S32 VPSS_FB_RelFulFrmBuf(VPSS_FB_INFO_S *pstFrameList,HI_DRV_VIDEO_FRAME_S *pstFrame);

//生产者TASK
VPSS_FB_NODE_S * VPSS_FB_GetEmptyFrmBuf(VPSS_FB_INFO_S *pstFrameList);
HI_S32 VPSS_FB_AddFulFrmBuf(VPSS_FB_INFO_S *pstFrameList,VPSS_FB_NODE_S *pstFBNode);
HI_S32 VPSS_FB_AddEmptyFrmBuf(VPSS_FB_INFO_S *pstFrameList,VPSS_FB_NODE_S *pstFBNode);

HI_BOOL VPSS_FB_CheckIsAvailable(VPSS_FB_INFO_S *pstFrameList);


HI_S32 VPSS_FB_GetState(VPSS_FB_INFO_S *pstFrameList,VPSS_FB_STATE_S *pstFbState);


HI_S32 VPSS_FB_Reset(VPSS_FB_INFO_S *pstFrameList);



HI_S32 VPSS_FB_WRITEYUV(VPSS_FB_NODE_S  *pstFbNode,HI_CHAR* pchFile);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif  /* __VO_EXT_H__ */
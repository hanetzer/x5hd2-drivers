

#ifndef __HIFB_COMM_H__
#define __HIFB_COMM_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#include "hifb.h"
#include "hi_debug.h"


#define MKSTR(exp) # exp
#define MKMARCOTOSTR(exp) MKSTR(exp)

#undef HIFB_SECURITY_VERSION

#if defined(CONFIG_SUPPORT_CA_RELEASE)
#define HIFB_SECURITY_VERSION
#define HIFB_DISENABLE_PROC
#endif


/* define debug level */
#define HIFB_FATAL(fmt...)  HI_TRACE(HI_LOG_LEVEL_FATAL, HI_ID_FB, fmt)
#define HIFB_ERROR(fmt...)  HI_TRACE(HI_LOG_LEVEL_ERROR, HI_ID_FB, fmt)
#define HIFB_WARNING(fmt...)  HI_TRACE(HI_LOG_LEVEL_WARNING, HI_ID_FB, fmt)
#define HIFB_INFO(fmt...)  HI_TRACE(HI_LOG_LEVEL_INFO, HI_ID_FB, fmt)


/* unit rect */
#define HIFB_UNITE_RECT(stDstRect, stSrcRect) do\
{\
    HIFB_RECT stRect;\
    stRect.x = (stDstRect.x < stSrcRect.x)? stDstRect.x : stSrcRect.x;\
    stRect.y = (stDstRect.y < stSrcRect.y)? stDstRect.y : stSrcRect.y;\
    stRect.w = ((stDstRect.x + stDstRect.w) > (stSrcRect.x + stSrcRect.w))? \
        (stDstRect.x + stDstRect.w - stRect.x) : (stSrcRect.x + stSrcRect.w - stRect.x);\
    stRect.h = ((stDstRect.y + stDstRect.h) > (stSrcRect.y + stSrcRect.h))? \
        (stDstRect.y + stDstRect.h - stRect.y) : (stSrcRect.y + stSrcRect.h - stRect.y);\
    memcpy(&stDstRect, &stRect, sizeof(HIFB_RECT));\
}while(0)


#define HIFB_MIN(m, n) (m) > (n) ? (n) : (m)



HI_VOID hifb_version(HI_VOID);

HI_S32 hifb_buf_initmem(HI_VOID);

HI_VOID hifb_buf_deinitmem(HI_VOID);
 
HI_VOID *hifb_buf_map(HI_U32 u32PhyAddr);

HI_S32 hifb_buf_ummap(HI_VOID *pViraddr);

HI_VOID hifb_buf_freemem(HI_U32 u32Phyaddr);

HI_U32 hifb_buf_allocmem(HI_CHAR *pName, HI_U32 u32LayerSize);


HI_BOOL HIFB_IsIntersectRect(const HIFB_RECT* pRect1, const HIFB_RECT* pRect2);

HI_VOID hifb_addrect(HIFB_RECT *pRectHead, HI_U32 TotalNum, HI_U32 *pValidNum, HIFB_RECT *pRect);

HI_BOOL hifb_iscontain(HIFB_RECT *pstParentRect, HIFB_RECT *pstChildRect);

HI_BOOL hifb_isoverlay(HIFB_RECT *pstSrcRect, HIFB_RECT *pstDstRect);

HI_U32 hifb_getbppbyfmt(HIFB_COLOR_FMT_E enColorFmt);

HI_S32 hifb_bitfieldcmp(struct fb_bitfield x, struct fb_bitfield y);


HI_S32 HIGO_Log_Init(HI_VOID);

HI_S32 HIGO_Log_Deinit(HI_VOID);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __HIFB_COMM_H__ */





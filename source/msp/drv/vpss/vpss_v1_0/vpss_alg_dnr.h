#ifndef __VPSS_ALG_DNR_H___
#define __VPSS_ALG_DNR_H___

#include "vpss_common.h"

typedef struct
{
    HI_S32 dralphascale;
    HI_S32 drbetascale;
    HI_S32 drthrflat3x3zone;
    HI_S32 drthrmaxsimilarpixdiff;
    
}ALG_DR_RTL_PARA_S;

typedef struct
{
    HI_S32 picestqp;
    HI_S32 dbuseweakflt;
    HI_S32 dbvertasprog;
    HI_S32 detailimgqpthr;
    HI_S32 dbthredge;
    HI_S32 dbalphascale;
    HI_S32 dbbetascale;
    HI_S32 dbthrlagesmooth;
    HI_S32 dbthrmaxdiffhor;
    HI_S32 dbthrmaxdiffvert;
    HI_S32 dbthrleastblkdiffhor;
    HI_S32 dbthrleastblkdiffvert;
}ALG_DB_RTL_PARA_S;

typedef struct
{
    HI_BOOL drEn;    
    HI_BOOL dbEn;
    HI_BOOL dbEnHort;
    HI_BOOL dbEnVert;
    HI_U32 u32YInfoAddr;
    HI_U32 u32CInfoAddr;
    HI_U32 u32YInfoStride;
    HI_U32 u32CInfoStride;
}ALG_DNR_CTRL_PARA_S;

typedef struct 
{
    HI_BOOL drEn;
    HI_BOOL dbEn;
    HI_BOOL dbEnHort;
    HI_BOOL dbEnVert;
    HI_U32 u32YInfoAddr;
    HI_U32 u32CInfoAddr;
    HI_U32 u32YInfoStride;
    HI_U32 u32CInfoStride;
    ALG_DR_RTL_PARA_S stDrThd;
    ALG_DB_RTL_PARA_S stDbThd;
}ALG_DNR_RTL_PARA_S;

HI_VOID ALG_DnrInit(ALG_DNR_CTRL_PARA_S *pstDrvPara,ALG_DNR_RTL_PARA_S * pstDnrRtlPara);

#endif

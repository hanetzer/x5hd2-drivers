#ifndef __DRV_PDM_H__
#define __DRV_PDM_H__

#include "hi_type.h"
#include "drv_pdm_ext.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct tagPDM_GLOBAL_S
{
    HI_DISP_PARAM_S     stDispParam;
    HI_GRC_PARAM_S      stGrcParam;
    HI_MCE_PARAM_S      stMceParam;
    HI_CHAR             ReleaseBufName[16][16];
    HI_U32              ReleaseBufNum;
}PDM_GLOBAL_S;

HI_S32 DRV_PDM_GetDispParam(HI_UNF_DISP_E enDisp, HI_DISP_PARAM_S *pstDispParam);
HI_S32 DRV_PDM_GetGrcParam(HI_GRC_PARAM_S *pGrcParam);
HI_S32 DRV_PDM_GetMceParam(HI_MCE_PARAM_S *pMceParam);
HI_S32 DRV_PDM_GetMceData(HI_U32 u32Size, HI_U32 *pAddr);
HI_S32 DRV_PDM_ReleaseReserveMem(const HI_CHAR *BufName);


#ifdef __cplusplus
}
#endif

#endif

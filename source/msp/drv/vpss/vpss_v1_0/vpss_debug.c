#include "vpss_debug.h"
#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

HI_S32 VPSS_DBG_DbgInit(VPSS_DBG_S *pstDbg)
{
    HI_U32 u32Count;

    for(u32Count = 0; u32Count < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER; u32Count++)
    {
        pstDbg->stPortDbg[u32Count].bWriteYUV = HI_FALSE;
    }

    return HI_SUCCESS;
    
}

HI_S32 VPSS_DBG_DbgDeInit(VPSS_DBG_S *pstDbg)
{
    return HI_SUCCESS;
}
HI_S32 VPSS_DBG_SendDbgCmd(VPSS_DBG_S *pstDbg,VPSS_DBG_CMD_S *pstCmd)
{
    VPSS_DBG_YUV_S  *pstYUV;
    HI_U32  u32Count;
    VPSS_DBG_PORT_S *pstPortDbg;
    VPSS_DBG_INST_S *pstInstDbg;
    
    switch(pstCmd->enDbgType)
    {
        case DBG_W_OUT_YUV:
            pstYUV = (VPSS_DBG_YUV_S  *)&(pstCmd->u32Reserve[0]);
            
            u32Count = pstYUV->hPort & 0xf;
            
            if (u32Count > DEF_HI_DRV_VPSS_PORT_MAX_NUMBER)
            {
                VPSS_FATAL("Debug Wrong hPort\n");
                return HI_FAILURE;
            }
            
            pstPortDbg = &(pstDbg->stPortDbg[u32Count]);

            pstPortDbg->bWriteYUV = HI_TRUE;
            
            memcpy(pstPortDbg->chFile,pstYUV->chFile,sizeof(HI_CHAR)*DEF_FILE_NAMELENGTH);
            break;
        #if 1
        case DBG_W_IN_YUV:
            pstInstDbg = &(pstDbg->stInstDbg);
            
            pstInstDbg->bWriteYUV = HI_TRUE;

            memcpy(pstInstDbg->chFile,&(pstCmd->u32Reserve[0]),sizeof(HI_CHAR)*DEF_FILE_NAMELENGTH);
        
            break;
        #endif
        default:
            VPSS_FATAL("Cmd isn't Supported.\n");
    }

    return HI_SUCCESS;
}

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */


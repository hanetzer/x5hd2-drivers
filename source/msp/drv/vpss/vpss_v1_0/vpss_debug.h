#ifndef __VPSS_DEBUG_H__
#define __VPSS_DEBUG_H__
#include "vpss_common.h"
#include "hi_drv_vpss.h"


typedef enum hiVPSS_DEBUG_E{
    DBG_W_OUT_YUV = 0,
    DBG_W_IN_YUV ,
    DBG_BUTT
}VPSS_DEBUG_E;

typedef struct hiVPSS_DBG_YUV_S{
    VPSS_HANDLE hPort;
    HI_CHAR chFile[DEF_FILE_NAMELENGTH];
}VPSS_DBG_YUV_S;


typedef struct hiVPSS_DBGCMD_S{
    VPSS_DEBUG_E enDbgType;
    HI_U32 u32Reserve[30];
}VPSS_DBG_CMD_S;

typedef struct hiVPSS_DBG_PORT_S{
    HI_BOOL bWriteYUV;
    HI_CHAR chFile[DEF_FILE_NAMELENGTH];
}VPSS_DBG_PORT_S;

typedef struct hiVPSS_DBG_INST_S{
    HI_BOOL bWriteYUV;
    HI_CHAR chFile[DEF_FILE_NAMELENGTH];
    HI_U32 u32Reserve;
}VPSS_DBG_INST_S;

typedef struct hiVPSS_DBG_S{
    VPSS_DBG_INST_S stInstDbg;
    
    VPSS_DBG_PORT_S stPortDbg[DEF_HI_DRV_VPSS_PORT_MAX_NUMBER];
    
}VPSS_DBG_S;


HI_S32 VPSS_DBG_DbgInit(VPSS_DBG_S *pstDbg);

HI_S32 VPSS_DBG_DbgDeInit(VPSS_DBG_S *pstDbg);

HI_S32 VPSS_DBG_SendDbgCmd(VPSS_DBG_S *pstDbg,VPSS_DBG_CMD_S *pstCmd);

#endif
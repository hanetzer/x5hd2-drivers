/*****************************************************************************
*              Copyright 2004 - 2050, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName: hi_aenc.c
* Description: Describe main functionality and purpose of this file.
*
* History:
* Version   Date         Author     DefectNum    Description
* 0.01      
*
*****************************************************************************/


#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* End of #ifdef __cplusplus */


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/poll.h>
#include <pthread.h>

#include "hi_type.h"
#include "hi_debug.h"
#include "hi_module.h"
#include "hi_mpi_mem.h"
#include "drv_struct_ext.h"
#include "hi_error_mpi.h"

#include "hi_mpi_ai.h"
#include "drv_ai_ioctl.h"


static HI_S32 g_s32AIFd = -1;
static const HI_CHAR g_acAIDevName[] = "/dev/" UMAP_DEVNAME_AI;


HI_S32 HI_MPI_AI_Init(HI_VOID)
{
    if (g_s32AIFd < 0)
    {
        g_s32AIFd = open(g_acAIDevName, O_RDWR, 0);
        if (g_s32AIFd < 0)    
        {
            //HI_FATAL_AI("OpenAIDevice err\n");
            g_s32AIFd = -1;
            return HI_ERR_AI_NOT_INIT;
        }
    }


    return HI_SUCCESS;
}

HI_S32   HI_MPI_AI_DeInit(HI_VOID)
{
    if(g_s32AIFd > 0)  
    {
        close(g_s32AIFd);
        g_s32AIFd = -1;
    }


    return HI_SUCCESS;
}

HI_S32   HI_MPI_AI_GetDefaultAttr(HI_UNF_AI_E enAiPort, HI_UNF_AI_ATTR_S *pstAttr)
{
	
    //todo
    return HI_SUCCESS;
}

HI_S32 HI_MPI_AI_SetAttr(HI_HANDLE hAI, HI_UNF_AI_ATTR_S *pstAttr)
{
	
    //todo
    return HI_SUCCESS;
}

HI_S32 HI_MPI_AI_GetAttr(HI_HANDLE hAI, HI_UNF_AI_ATTR_S *pstAttr)
{
	
    //todo
    return HI_SUCCESS;
}

HI_S32 HI_MPI_AI_Create(HI_UNF_AI_E enAiPort, HI_UNF_AI_ATTR_S *pstAttr, HI_HANDLE *phandle)
{
	
    //todo
    return HI_SUCCESS;
}

HI_S32 HI_MPI_AI_Destroy(HI_HANDLE hAI)
{
	
    //todo
    return HI_SUCCESS;
}

HI_S32 HI_MPI_AI_SetEnable(HI_HANDLE hAI, HI_BOOL bEnable)
{
	
    //todo
    return HI_SUCCESS;
}

HI_S32 HI_MPI_AI_AcquireFrame(HI_HANDLE hCast, HI_UNF_AO_FRAMEINFO_S *pstFrame)
{
	
    //todo
    return HI_SUCCESS;
}

HI_S32 HI_MPI_AI_ReleaseFrame(HI_HANDLE hCast, HI_UNF_AO_FRAMEINFO_S *pstFrame)
{
	
    //todo
    return HI_SUCCESS;
}


#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* End of #ifdef __cplusplus */

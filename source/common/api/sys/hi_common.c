/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_common.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2009/12/15
  Description   : Common apis for hisilicon system.
  History       :
  1.Date        : 2010/01/25
    Author      : jianglei
    Modification: Created file

*******************************************************************************/
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <pthread.h>
#include <unistd.h>

#include "hi_type.h"
#include "hi_common.h"
#include "hi_module.h"
#include "mpi_mmz.h"
#include "hi_mpi_mem.h"
#include "drv_struct_ext.h"
#include "mpi_module.h"
#include "mpi_log.h"
#include "drv_sys_ioctl.h"
#include "hi_mpi_stat.h"

//////////////////////////////////////////////////////////////////////////////////////
/// STATIC CONST Variable
//////////////////////////////////////////////////////////////////////////////////////
static const HI_U8 s_szMonth[12][4] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                       "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
static const HI_U8 s_szVersion[] = "SDK_VERSION:["\
                            MKMARCOTOSTR(SDK_VERSION)"] Build Time:["\
                            __DATE__", "__TIME__"]";

static pthread_mutex_t   s_SysMutex = PTHREAD_MUTEX_INITIALIZER;

/// STATIC Variable
static HI_S32 s_s32SysInitTimes = 0;
static HI_S32 s_s32SysFd = -1;

/// DEFINATION MACRO
#define SYS_OPEN_FILE                                       \
do{                                                         \
    if (-1 == s_s32SysFd)                                   \
    {                                                       \
        s_s32SysFd = open("/dev/"UMAP_DEVNAME_SYS, O_RDWR); \
        if (s_s32SysFd < 0)                                 \
        {                                                   \
            perror("open");                                 \
            HI_SYS_UNLOCK();                                \
            return errno;                                   \
        }                                                   \
    }                                                       \
}while(0)

#define HI_SYS_LOCK()     (void)pthread_mutex_lock(&s_SysMutex);
#define HI_SYS_UNLOCK()   (void)pthread_mutex_unlock(&s_SysMutex);


HI_S32 HI_SYS_Init(HI_VOID)
{
    HI_S32 s32Ret = HI_FAILURE;

    HI_SYS_LOCK();

    if (0 == s_s32SysInitTimes)
    {
        SYS_OPEN_FILE;

        s32Ret = HI_MPI_LogInit();
        if (HI_SUCCESS != s32Ret)
        {
            HI_FATAL_SYS("HI_MPI_LogInit failure, line:%d\n", __LINE__);
            goto LogErrExit;
        }

        s32Ret = HI_MODULE_Init();
        if (HI_SUCCESS != s32Ret)
        {
            HI_FATAL_SYS("HI_ModuleMGR_Init failure, line:%d\n", __LINE__);
            goto ModuleErrExit;
        }

        //MUST unlock, because HI_MPI_STAT_Init calling HI_SYS_GetVersion will cause deadlock
        HI_SYS_UNLOCK();

        s32Ret = HI_MPI_STAT_Init();
        if (HI_SUCCESS != s32Ret)
        {
            HI_FATAL_SYS("HI_MPI_STAT_Init failure, line:%d\n", __LINE__);
            goto StatErrExit;
        }

        HI_SYS_LOCK();

        s_s32SysInitTimes = 1;

        HI_INFO_SYS("HI_SYS_Init init OK\n");
    }

    HI_SYS_UNLOCK();

    return (HI_S32)HI_SUCCESS;

StatErrExit:
    HI_MPI_LogDeInit();

LogErrExit:
    (HI_VOID)HI_MODULE_DeInit();

ModuleErrExit:
    if (s_s32SysFd != -1)
    {
        close(s_s32SysFd);
        s_s32SysFd = -1;
    }

    s_s32SysInitTimes = 0;

    HI_SYS_UNLOCK();

    return (HI_S32)HI_FAILURE;
}

HI_S32 HI_SYS_DeInit(HI_VOID)
{
    HI_SYS_LOCK();

    if (0 != s_s32SysInitTimes)
    {
        (HI_VOID)HI_MPI_STAT_DeInit();

        (HI_VOID)HI_MODULE_DeInit();

        HI_MPI_LogDeInit();

        if (s_s32SysFd != -1)
        {
            close(s_s32SysFd);
            s_s32SysFd = -1;
        }

        s_s32SysInitTimes = 0;
    }

    HI_SYS_UNLOCK();

    return HI_SUCCESS;
}

HI_S32 HI_SYS_GetBuildTime(struct tm * pstTime)
{
    char szData[] = __DATE__;
    char szTime[] = __TIME__;
    char szTmp[5];
    int i;

    if (NULL == pstTime)
    {
        return HI_FAILURE;
    }

    /* month */
    memset(szTmp, 0, sizeof(szTmp));
    strncpy(szTmp, szData, 3);

    for(i=0; i<12; i++)
    {
            if(!strcmp((const char*)s_szMonth[i], szTmp))
            {
                pstTime->tm_mon = i + 1;
                break;
            }
    }

    /* day */
    memset(szTmp, 0, sizeof(szTmp));
    strncpy(szTmp, szData+4, 2);
    pstTime->tm_mday = atoi(szTmp);

    /* year */
    memset(szTmp, 0, sizeof(szTmp));
    strncpy(szTmp, szData+7, 4);
    pstTime->tm_year = atoi(szTmp);

    /* hour */
    memset(szTmp, 0, sizeof(szTmp));
    strncpy(szTmp, szTime, 2);
    pstTime->tm_hour = atoi(szTmp);

    /* minute */
    memset(szTmp, 0, sizeof(szTmp));
    strncpy(szTmp, szTime+3, 2);
    pstTime->tm_min = atoi(szTmp);


    /* second */
    memset(szTmp, 0, sizeof(szTmp));
    strncpy(szTmp, szTime+6, 2);
    pstTime->tm_sec = atoi(szTmp);

    return HI_SUCCESS;
}

HI_S32 HI_SYS_GetVersion(HI_SYS_VERSION_S *pstVersion)
{
    HI_S32 s32Ret = HI_FAILURE;

    if (NULL == pstVersion)
    {
        return HI_FAILURE;
    }

    HI_SYS_LOCK();

    if (s_s32SysFd < 0)
    {
        HI_SYS_UNLOCK();
        return HI_FAILURE;
    }

    s32Ret = ioctl(s_s32SysFd, SYS_GET_SYS_VERSION, pstVersion);
    if(s32Ret != 0)
    {
        HI_SYS_UNLOCK();

        HI_ERR_SYS("ioctl SYS_GET_SYS_VERSION error!\n");
        return HI_FAILURE;
    }

    sprintf(pstVersion->aVersion, "%s", s_szVersion);
    #if defined(CHIP_TYPE_hi3716h)
        pstVersion->enChipTypeSoft = HI_CHIP_TYPE_HI3716H;
    #elif defined(CHIP_TYPE_hi3716m) || defined(CHIP_TYPE_hi3716mv300_fpga)
        pstVersion->enChipTypeSoft = HI_CHIP_TYPE_HI3716M;
    #elif defined(CHIP_TYPE_hi3716c)
        pstVersion->enChipTypeSoft = HI_CHIP_TYPE_HI3716C;
    #elif defined(CHIP_TYPE_hi3720)
        pstVersion->enChipTypeSoft = HI_CHIP_TYPE_HI3720;
    #elif defined(CHIP_TYPE_hi3712)
        pstVersion->enChipTypeSoft = HI_CHIP_TYPE_HI3712;
    #elif defined(CHIP_TYPE_hi3716cv200)
        pstVersion->enChipTypeSoft = HI_CHIP_TYPE_HI3716C;
    #elif defined(CHIP_TYPE_hi3716cv200es)
        pstVersion->enChipTypeSoft = HI_CHIP_TYPE_HI3716CES;
    #else
        #error  YOU MUST DEFINE  CHIP_TYPE!
    #endif

    HI_SYS_UNLOCK();

    return HI_SUCCESS;
}

HI_S32 HI_SYS_GetChipAttr(HI_SYS_CHIP_ATTR_S *pstChipAttr)
{
    HI_S32              ret;
    HI_SYS_VERSION_S    SysVer;

    if (!pstChipAttr)
    {
        HI_ERR_SYS("null ptr!\n");
        return HI_FAILURE;
    }

    ret = HI_SYS_GetVersion(&SysVer);
    if (HI_SUCCESS != ret)
    {
        return HI_FAILURE;
    }

    if (   (HI_CHIP_TYPE_HI3712 == SysVer.enChipTypeHardWare)
        || (HI_CHIP_TYPE_HI3716CES == SysVer.enChipTypeHardWare)
        || ((HI_CHIP_TYPE_HI3716M == SysVer.enChipTypeHardWare) && (HI_CHIP_VERSION_V300 == SysVer.enChipVersion)) )
    {
        HI_U32 Val;

        ret = HI_SYS_ReadRegister(HI_DOLBY_REG, &Val);
        if (HI_SUCCESS == ret)
        {
            pstChipAttr->bDolbySupport = (Val & HI_DOLBY_BIT) ? HI_FALSE : HI_TRUE;
        }

        return ret;
    }

    return HI_FAILURE;
}

HI_S32 HI_SYS_SetConf(const HI_SYS_CONF_S *pstSysConf)
{
    HI_S32 s32Ret = HI_FAILURE;

    if ( NULL == pstSysConf )
    {
        HI_ERR_SYS("Set pstSysConf ptr!\n");

        return HI_FAILURE;
    }

    HI_SYS_LOCK();

    if (s_s32SysFd < 0)
    {
        HI_SYS_UNLOCK();
        return HI_FAILURE;
    }

    s32Ret = ioctl(s_s32SysFd, SYS_SET_CONFIG_CTRL, pstSysConf);

    HI_SYS_UNLOCK();

    return s32Ret;
}

HI_S32 HI_SYS_GetConf(HI_SYS_CONF_S *pstSysConf)
{
    HI_S32 s32Ret = HI_FAILURE;

    if ( NULL == pstSysConf )
    {
        HI_ERR_SYS("Get pstSysConf ptr!\n");

        return HI_FAILURE;
    }

    HI_SYS_LOCK();

    if (s_s32SysFd < 0)
    {
        HI_SYS_UNLOCK();
        return HI_FAILURE;
    }

    s32Ret = ioctl(s_s32SysFd, SYS_GET_CONFIG_CTRL, pstSysConf);

    HI_SYS_UNLOCK();

    return s32Ret;
}

HI_S32 HI_SYS_WriteRegister(HI_U32 u32RegAddr, HI_U32 u32Value)
{
    HI_U32 *pu32VirAddr;

    HI_SYS_LOCK();

    pu32VirAddr = (HI_U32*)HI_MMAP(u32RegAddr, 4);
    if (NULL == pu32VirAddr)
    {
        HI_ERR_SYS("HI_MMAP failed\n");

        HI_SYS_UNLOCK();

        return HI_FAILURE;
    }

    *pu32VirAddr = u32Value;

    HI_MUNMAP((void*)pu32VirAddr);

    HI_SYS_UNLOCK();

    return HI_SUCCESS;
}

HI_S32 HI_SYS_ReadRegister(HI_U32 u32RegAddr, HI_U32 *pu32Value)
{
    HI_U32 *pu32VirAddr;

    HI_SYS_LOCK();

    pu32VirAddr = (HI_U32*)HI_MMAP(u32RegAddr, 4);
    if (NULL == pu32VirAddr)
    {
        HI_ERR_SYS("HI_MMAP failed\n");

        HI_SYS_UNLOCK();

        return HI_FAILURE;
    }

    *pu32Value = *pu32VirAddr;

    HI_MUNMAP((void*)pu32VirAddr);

    HI_SYS_UNLOCK();

    return HI_SUCCESS;
}

HI_S32 HI_SYS_GetTimeStampMs(HI_U32 *pu32TimeMs)
{
    HI_S32 s32Ret = HI_FAILURE;

    if (HI_NULL == pu32TimeMs)
    {
        HI_ERR_SYS("null pointer error\n");
        return HI_FAILURE;
    }

    HI_SYS_LOCK();

    if (s_s32SysFd < 0)
    {
        HI_SYS_UNLOCK();
        return HI_FAILURE;
    }

    s32Ret = ioctl(s_s32SysFd, SYS_GET_TIMESTAMPMS, pu32TimeMs);

    HI_SYS_UNLOCK();

    return s32Ret;
}

HI_S32 HI_SYS_SetLogLevel(HI_MOD_ID_E enModId,  HI_LOG_LEVEL_E enLogLevel)
{
    return HI_MPI_LogLevelSet((HI_U32)enModId, enLogLevel);
}

HI_S32 HI_SYS_SetLogPath(const HI_CHAR* pszLogPath)
{
    return HI_MPI_LogPathSet(pszLogPath);
}

HI_S32 HI_SYS_SetStorePath(const HI_CHAR* pszPath)
{
    return HI_MPI_StorePathSet(pszPath);
}

HI_S32 HI_MMZ_Malloc(HI_MMZ_BUF_S *pstBuf)
{
    return HI_MPI_MMZ_Malloc(pstBuf);
}

HI_S32 HI_MMZ_Free(HI_MMZ_BUF_S *pstBuf)
{
    return HI_MPI_MMZ_Free(pstBuf);
}

HI_VOID *HI_MMZ_New(HI_U32 u32Size , HI_U32 u32Align, HI_CHAR *ps8MMZName, HI_CHAR *ps8MMBName)
{
    return HI_MPI_MMZ_New(u32Size, u32Align, ps8MMZName, ps8MMBName);
}

HI_S32 HI_MMZ_Delete(HI_U32 u32PhysAddr)
{
    return HI_MPI_MMZ_Delete(u32PhysAddr);
}

HI_VOID *HI_MMZ_Map(HI_U32 u32PhysAddr, HI_U32 u32Cached)
{
    return HI_MPI_MMZ_Map(u32PhysAddr, u32Cached);
}

HI_S32 HI_MMZ_Unmap(HI_U32 u32PhysAddr)
{
    return HI_MPI_MMZ_Unmap(u32PhysAddr);
}

HI_S32 HI_MMZ_Flush(HI_U32 u32PhysAddr)
{
    return HI_MPI_MMZ_Flush(u32PhysAddr);
}

HI_VOID *HI_MEM_Map(HI_U32 u32PhyAddr, HI_U32 u32Size)
{
    return HI_MMAP(u32PhyAddr, u32Size);
}

HI_S32 HI_MEM_Unmap(HI_VOID *pAddrMapped)
{
    return HI_MUNMAP(pAddrMapped);
}

HI_S32 HI_MMZ_GetPhyaddr(HI_VOID * pVir, HI_U32 *pu32Phyaddr, HI_U32 *pu32Size)
{
    return HI_MPI_MMZ_GetPhyAddr(pVir, pu32Phyaddr, pu32Size);
}


HI_VOID* HI_MEM_Malloc(HI_U32 u32ModuleID, HI_U32 u32Size)
{
    return HI_MALLOC(u32ModuleID, u32Size);
}

HI_VOID HI_MEM_Free(HI_U32 u32ModuleID, HI_VOID* pMemAddr)
{
    HI_FREE(u32ModuleID, pMemAddr);
}

HI_VOID* HI_MEM_Calloc(HI_U32 u32ModuleID, HI_U32 u32MemBlock, HI_U32 u32Size)
{
    return HI_CALLOC(u32ModuleID, u32MemBlock, u32Size);
}

HI_VOID* HI_MEM_Realloc(HI_U32 u32ModuleID, HI_VOID *pMemAddr, HI_U32 u32Size)
{
    return HI_REALLOC(u32ModuleID, pMemAddr, u32Size);
}

#ifdef MMZ_V2_SUPPORT
HI_VOID *HI_MMZ_New_Share(HI_U32 size , HI_U32 align, HI_CHAR *mmz_name, HI_CHAR *mmb_name)
{
    return HI_MPI_MMZ_New_Share(size, align, mmz_name, mmb_name);
}

HI_VOID *HI_MMZ_New_Shm_Com(HI_U32 size , HI_U32 align, HI_CHAR *mmz_name, HI_CHAR *mmb_name)
{
    return HI_MPI_MMZ_New_Shm_Com(size, align, mmz_name, mmb_name);
}

HI_S32 HI_MMZ_Get_Shm_Com(HI_U32 *phyaddr, HI_U32 *size)
{
    return HI_MPI_MMZ_Get_Shm_Com(phyaddr, size);
}
HI_S32 HI_MMZ_Force_Delete(HI_U32 phys_addr)
{
    return HI_MPI_MMZ_Force_Delete(phys_addr);
}

HI_S32 HI_MMZ_Flush_Dirty(HI_U32 phys_addr, HI_U32 virt_addr, HI_U32 size)
{
    return HI_MPI_MMZ_Flush_Dirty(phys_addr, virt_addr, size);
}

HI_S32 HI_MMZ_open(HI_VOID)
{
    return HI_MPI_MMZ_open();
}

HI_S32 HI_MMZ_close(HI_VOID)
{
    return HI_MPI_MMZ_close();
}
#endif


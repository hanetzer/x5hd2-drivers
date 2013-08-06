/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi3560.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2006/02/09
  Description   :
  History       :
  1.Date        : 2006/02/09
    Author      : Luo Chuanzao
    Modification: Created file

  2.Date         : 2006/2/9
    Author       : QuYaxin 46153
    Modification : Modified some macro for coherence
                   with mpi_struct.h

  3.Date         : 2010/1/25
    Author       : jianglei
    Modification : Modified for X5HD common module

******************************************************************************/

#include <linux/proc_fs.h>
#include <linux/seq_file.h>

#include <linux/delay.h>

#include "hi_type.h"
#include "drv_struct_ext.h"
#include "drv_dev_ext.h"
#include "drv_proc_ext.h"
#include "drv_sys_ext.h"
#include "drv_reg_ext.h"

#include "drv_sys_ioctl.h"

static HI_CHAR s_szSdkKoVersion[] = "SDK_VERSION:["\
    MKMARCOTOSTR(SDK_VERSION)"] Build Time:["\
    __DATE__", "__TIME__"]";

HI_CHAR *g_pszChipName[HI_CHIP_TYPE_BUTT+1] = {
"Hi3716M",
"Hi3716H",
"Hi3716C",
"Hi3716CES",

"Hi3720",

"HI3712" ,
"HI3715" ,

"UNKNOWN"
};

typedef struct hiHIxxxx_SOC_S
{
    struct semaphore stSem;
    HI_SYS_CONF_S stChipConf;
}HIxxxx_SOC_S;

static HIxxxx_SOC_S s_stSocData;

extern const char * get_sdkversion(void);

HI_S32 SYS_GetBootVersion(HI_CHAR *pVersion,HI_U32 u32VersionLen)
{
    const HI_U8* pu8BootVer = get_sdkversion();

    if (HI_NULL == pVersion || u32VersionLen == 0)
    {
        HI_WARN_SYS("SYS_GetBootVersion failure line:%d\n", __LINE__);
        return HI_FAILURE;
    }

    if (pu8BootVer != NULL)
    {
        if (u32VersionLen > strlen(pu8BootVer))
        {
            u32VersionLen = strlen(pu8BootVer);
        }

        memcpy(pVersion, pu8BootVer, u32VersionLen);
        pVersion[u32VersionLen] = '\0';

        return HI_SUCCESS;
    }

    return HI_FAILURE;
}


HI_S32 SysSetConfig(HI_SYS_CONF_S *pstConf)
{
    memcpy(&s_stSocData.stChipConf, pstConf, sizeof(*pstConf));
    return 0;
}


HI_S32 SysGetConfig(HI_SYS_CONF_S *pstConf)
{
    memcpy(pstConf, &s_stSocData.stChipConf, sizeof(*pstConf));
    return 0;
}

static HI_S32 SYS_Ioctl(struct inode *pInode,
         struct file  *pFile,
         HI_U32  cmd,
         HI_VOID *arg)
{
    HI_S32 ret = -ENOIOCTLCMD;
    HI_SYS_VERSION_S* chiptype;

    down(&s_stSocData.stSem);
    switch (cmd)
    {
        case SYS_SET_CONFIG_CTRL :
            ret = SysSetConfig((HI_SYS_CONF_S*)arg);
            break;

        case SYS_GET_CONFIG_CTRL :
            ret = SysGetConfig((HI_SYS_CONF_S*)arg);
            break;

        case SYS_GET_SYS_VERSION :
             chiptype = (HI_SYS_VERSION_S*)arg;
             HI_DRV_SYS_GetChipVersion(&chiptype->enChipTypeHardWare, &chiptype->enChipVersion);
             SYS_GetBootVersion(chiptype->BootVersion, sizeof(chiptype->BootVersion));
             ret = HI_SUCCESS;
             break;

        case SYS_GET_TIMESTAMPMS :
            ret = HI_DRV_SYS_GetTimeStampMs((HI_U32*)arg);

            break;

        default :
            HI_WARN_SYS("ioctl cmd %d nonexist!\n", cmd);
    }
    up(&s_stSocData.stSem);
    return ret;
}

static HI_S32 SysProcShow(struct seq_file *s, HI_VOID *pArg)
{
    HI_CHIP_TYPE_E      ChipType    = HI_CHIP_TYPE_BUTT;
    HI_CHIP_VERSION_E   ChipVersion = 0;
    HI_U32              Value;

    HI_DRV_SYS_GetChipVersion(&ChipType, &ChipVersion);

    seq_printf(s, "%s\n", s_szSdkKoVersion);

    if (ChipType <= HI_CHIP_TYPE_BUTT)
    {
        seq_printf(s, "CHIP_VERSION: %s(0x%x)_v%x\n", g_pszChipName[ChipType], ChipType, ChipVersion);
    }
    else
    {
        seq_printf(s, "CHIP_VERSION: %s(0x%x)_v%x\n", g_pszChipName[HI_CHIP_TYPE_BUTT], ChipType, ChipVersion);
    }

    if ((HI_CHIP_TYPE_HI3712 == ChipType) || (HI_CHIP_TYPE_HI3716CES == ChipType) || ((HI_CHIP_TYPE_HI3716M == ChipType) && (HI_CHIP_VERSION_V300 == ChipVersion)))
    {
        HI_REG_READ(IO_ADDRESS(HI_DOLBY_REG), Value);

        seq_printf(s, "DOLBY: %s\n", (Value & HI_DOLBY_BIT) ? "NO" : "YES");
    }

    return 0;
}

static long CMPI_SYS_Ioctl(struct file *file, HI_U32 cmd, unsigned long arg)
{
    long ret;
    ret=(long)HI_DRV_UserCopy(file->f_dentry->d_inode, file, cmd, arg, SYS_Ioctl);

    return ret;
}


static HI_S32 CMPI_SYS_Open(struct inode * vinode, struct file * vfile)
{
    return 0;
}

static HI_S32 CMPI_SYS_Close(struct inode * vinode, struct file * vfile)
{
    return 0;
}


static struct file_operations stFileOp =
{
     .owner       = THIS_MODULE,
     .open        = CMPI_SYS_Open,
     .unlocked_ioctl  = CMPI_SYS_Ioctl,
     .release     = CMPI_SYS_Close
};
static UMAP_DEVICE_S s_stDevice;


HI_S32 HI_DRV_SYS_Init(HI_VOID)
{
    sema_init(&s_stSocData.stSem, 1);
    sprintf(s_stDevice.devfs_name, UMAP_DEVNAME_SYS);
    s_stDevice.fops = &stFileOp;
    s_stDevice.minor = UMAP_MIN_MINOR_SYS;
    s_stDevice.owner  = THIS_MODULE;
    s_stDevice.drvops = NULL;
    if (HI_DRV_DEV_Register(&s_stDevice))
    {
        HI_ERR_SYS("Register system device failed!\n");
        goto OUT;
    }

    HI_DRV_PROC_AddModule(HI_MOD_SYS, SysProcShow, 0);

    return 0;
OUT:

    HI_WARN_SYS("load sys ...FAILED!\n");
    return HI_FAILURE;
}

HI_VOID HI_DRV_SYS_Exit(HI_VOID)
{
    HI_DRV_PROC_RemoveModule(HI_MOD_SYS);
    HI_DRV_DEV_UnRegister(&s_stDevice);
    return ;
}



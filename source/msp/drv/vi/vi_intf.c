/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : viu.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       :
  Description   :
  History       :
  1.Date        : 2010/03/17
    Author      : j00131665
    Modification: Created file
******************************************************************************/
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <asm/io.h>
#include <asm/uaccess.h>

#include "drv_vi_ext.h"
#include "hi_drv_vi.h"
#include "drv_proc_ext.h"
#include "drv_module_ext.h"
#include "drv_dev_ext.h"
#include "hi_unf_common.h"
#include "viu_buf.h"
#include "drv_sys_ext.h"
#include "drv_proc_ext.h"
#include "optm_vi.h"
#include "viu_utils.h"
#include "hi_error_mpi.h"
#include "drv_reg_ext.h"
#include "drv_mem_ext.h"
#include "hi_kernel_adapt.h"

/***************************** Register Address ******************************/

#define SYS_REGS_ADDR_BASE 0x101F5000

static HI_VOID __iomem * vi_clk_or_enable = HI_NULL;

#define SYS_REGS_ADDR vi_clk_or_enable  /*  0x101F5000*/
#define SYS_PERI_CRG3_ADDR (0x4C + SYS_REGS_ADDR)
#define SYS_PERI_CRG4_ADDR (0x50 + SYS_REGS_ADDR)

/***************************************************************************/

static UMAP_DEVICE_S g_ViRegisterData;
static atomic_t g_ViCount = ATOMIC_INIT(0); //VI main device open times
HI_DECLARE_MUTEX(g_ViMutex);

struct file *g_pViFileFlag[VI_MAX_CHN_NUM];
extern OPTM_VI_S g_stViOptmS[VI_MAX_CHN_NUM];
extern HI_U32 Cc_int[VI_MAX_CHN_NUM];
extern HI_U32 Reg_update_int[VI_MAX_CHN_NUM];
extern HI_U32 g_u32IntLost[VI_MAX_CHN_NUM];
extern VI_CFG_ATTR_S g_stViCurCfg[VI_MAX_CHN_NUM];
extern HI_U32 g_u32KernUserId[VI_MAX_CHN_NUM];
extern atomic_t g_ViUidUsed[VI_MAX_CHN_NUM][VI_UID_MAX];
extern HI_MOD_ID_E g_ViUserMod[VI_MAX_CHN_NUM][VI_UID_MAX];
extern VI_CFG_ATTR_S g_stViCurCfg[VI_MAX_CHN_NUM];

extern HI_S32  Viu_ResumeAttr(HI_U32 u32ViPort, const HI_UNF_VI_ATTR_S *pstAttr);
static HI_S32  VI_DRV_Open(struct inode *finode, struct file  *ffile);
static HI_S32  VI_DRV_Close(struct inode *finode, struct file  *ffile);
static long    VI_DRV_Ioctl(struct file *file, unsigned int cmd, unsigned long arg);
static HI_S32  VI_ProcRead(struct seq_file *p, HI_VOID *v);
static HI_S32  VI_ProcWrite(struct file * file, const char __user * buf, size_t count, loff_t *ppos);

HI_S32         Viu_Ioctl(struct inode *inode, struct file  *file, unsigned int cmd, HI_VOID *arg);
HI_S32         Viu_ProcShow(struct seq_file *s, HI_VOID *pData);

//HI_S32         VI_Create(HI_UNF_VI_ATTR_S *pstAttr, HI_UNF_VI_E *phVi);
HI_S32         VI_Create(HI_UNF_VI_ATTR_S *pstAttr, HI_UNF_VI_E hVi);
HI_S32         VI_Destroy(HI_UNF_VI_E hVi);
HI_S32         VI_DestroyForce(HI_UNF_VI_E enVi);
HI_S32         VI_Start(HI_UNF_VI_E enVi);
HI_S32         VI_Stop(HI_UNF_VI_E enVi);

static struct file_operations VI_FOPS =
{
    .owner			= THIS_MODULE,
    .open			= VI_DRV_Open,
    .unlocked_ioctl = VI_DRV_Ioctl,
    .release		= VI_DRV_Close,
};

static VI_EXPORT_FUNC_S s_ViExportFuncs =
{
    .pfnViAcquireFrame = Viu_AcquireFrame,
    .pfnViReleaseFrame = Viu_ReleaseFrame,
    .pfnViGetUsrID     = Viu_GetUsrID,
    .pfnViPutUsrID     = Viu_PutUsrID,
};

extern HI_U32 g_StartCap[VIU_PORT_MAX];

static HI_U32 g_ViOpenFlag = 0;
static HI_S32 VI_DRV_Open(struct inode *finode, struct file  *ffile)
{
    HI_S32 Ret;
    DRV_PROC_ITEM_S  *pProcItem;
    HI_CHAR ProcName[12];
    HI_U32 i;
    HI_CHIP_TYPE_E enChipType;

    Ret = down_interruptible(&g_ViMutex);
    if (Ret)
    {
        HI_ERR_VI("%s: down_interruptible err !\n", __FUNCTION__);
        return Ret;
    }

    if (1 == atomic_inc_return(&g_ViCount))
    {
        HI_DRV_SYS_GetChipVersion(&enChipType, HI_NULL);

        if ((HI_CHIP_TYPE_HI3716H == enChipType) || (HI_CHIP_TYPE_HI3716C == enChipType))
        {
            VIU_DRV_BoardInit();

            // Register interrupt process
            Ret = request_irq(VIU_INTERRUPT_LINE, Viu_InterruptHandler, IRQF_DISABLED, HI_MOD_VI, NULL);
            if (HI_SUCCESS != Ret)
            {
                HI_FATAL_VI("request vi interrupt failed.\n");
                HI_KFREE(HI_ID_VI, ffile->private_data);
                atomic_dec(&g_ViCount);
                up(&g_ViMutex);
                return HI_FAILURE;
            }
        }

        for (i = 0; i < VIU_PORT_MAX; i++)
        {
            g_StartCap[i] = 0;
        }

        sprintf(ProcName, "%s", HI_MOD_VI);

        //HI_DRV_PROC_RemoveModule(ProcName);
        pProcItem = HI_DRV_PROC_AddModule(ProcName, HI_NULL, HI_NULL);
        if (!pProcItem)
        {
            HI_FATAL_VI("add %s proc failed.\n", ProcName);
            HI_KFREE(HI_ID_VI, ffile->private_data);
            atomic_dec(&g_ViCount);
            up(&g_ViMutex);
            return HI_FAILURE;
        }

        pProcItem->read  = VI_ProcRead;
        pProcItem->write = VI_ProcWrite;
    }

    g_ViOpenFlag = 1;
    up(&g_ViMutex);

    return HI_SUCCESS;
}

static HI_S32 VI_DRV_Close(struct inode *finode, struct file  *ffile)
{
    HI_U32 i;
    HI_S32 Ret;
    HI_CHAR ProcName[12];
    HI_CHIP_TYPE_E enChipType;

    Ret = down_interruptible(&g_ViMutex);
    if (Ret)
    {
        HI_ERR_VI("%s: down_interruptible err !\n", __FUNCTION__);
        return Ret;
    }

    for (i = 0; i < VI_MAX_CHN_NUM; i++)
    {
        if ((g_pViFileFlag[i] == ffile) && (HI_TRUE == g_stViOptmS[i].bOpened))
        {
            HI_INFO_VI("Force Close VI %d.\n", i);
            VI_DestroyForce(i);
        }
    }

    if (atomic_dec_and_test(&g_ViCount))
    {
        HI_DRV_SYS_GetChipVersion(&enChipType, HI_NULL);
        if ((HI_CHIP_TYPE_HI3716H == enChipType) || (HI_CHIP_TYPE_HI3716C == enChipType))
        {
            free_irq(VIU_INTERRUPT_LINE, NULL);
        }

        sprintf(ProcName, "%s", HI_MOD_VI);
        HI_DRV_PROC_RemoveModule(ProcName);
        if ((HI_CHIP_TYPE_HI3716H == enChipType) || (HI_CHIP_TYPE_HI3716C == enChipType))
        {
            VIU_DRV_BoardDeinit();
        }
    }

    g_ViOpenFlag = 0;
    up(&g_ViMutex);
    return HI_SUCCESS;
}

/*****************************************************************************
 Prototype    : VI_DRV_Suspend
 Description  : VI
 Input        : None
 Output       : None
 Return Value : None
*****************************************************************************/
HI_VOID VI_DRV_Suspend(HI_VOID)
{
    HI_S32 Ret;
    HI_U32 i;
    HI_CHAR ProcName[12];
    HI_CHIP_TYPE_E enChipType;

    if (!g_ViOpenFlag)
    {
        return;
    }

    if (!atomic_dec_and_test(&g_ViCount))
    {
        HI_FATAL_VI("%s: VI_DRV_Resume OK !\n", __FUNCTION__);
        return;
    }

    Ret = down_interruptible(&g_ViMutex);
    if (Ret)
    {
        HI_ERR_VI("%s: down_interruptible err !\n", __FUNCTION__);
        return;
    }

    for (i = 0; i < VI_MAX_CHN_NUM; i++)
    {
        // save config

        // stop
        if (g_stViOptmS[i].bOpened)
        {
            HI_DRV_SYS_GetChipVersion(&enChipType, HI_NULL);
            if ((HI_CHIP_TYPE_HI3716H == enChipType) || (HI_CHIP_TYPE_HI3716C == enChipType))
            {
                VIU_DRV_Stop(i, 0);
                VIU_DRV_SetIntEn(i, 0, 0);
                VIU_DRV_SetChEn(i, 0, 0);
                VIU_DRV_ClrInterruptStatus(i, 0, 0x1FF);
            }
        }
    }

    HI_DRV_SYS_GetChipVersion(&enChipType, HI_NULL);
    if ((HI_CHIP_TYPE_HI3716H == enChipType) || (HI_CHIP_TYPE_HI3716C == enChipType))
    {
        free_irq(VIU_INTERRUPT_LINE, NULL);
    }

    sprintf(ProcName, "%s", HI_MOD_VI);
    HI_DRV_PROC_RemoveModule(ProcName);
    if ((HI_CHIP_TYPE_HI3716H == enChipType) || (HI_CHIP_TYPE_HI3716C == enChipType))
    {
        VIU_DRV_BoardDeinit();
    }

    up(&g_ViMutex);
    HI_ERR_VI("%s: VI_DRV_Suspend OK !\n", __FUNCTION__);
}

/*****************************************************************************
 Prototype    : VI_DRV_Resume
 Description  : VI
 Input        : None
 Output       : None
 Return Value : None
*****************************************************************************/
HI_VOID VI_DRV_Resume(HI_VOID)
{
    HI_U32 i;
    HI_S32 Ret;
    HI_CHAR ProcName[12];
    DRV_PROC_ITEM_S  *pProcItem;
    HI_CHIP_TYPE_E enChipType;

    if (!g_ViOpenFlag)
    {
        return;
    }

    if (1 != atomic_inc_return(&g_ViCount))
    {
        HI_FATAL_VI("VI_DRV_Resume error!\n");
        return;
    }

    Ret = down_interruptible(&g_ViMutex);
    if (Ret)
    {
        HI_ERR_VI("%s: down_interruptible err !\n", __FUNCTION__);
        return;
    }

    HI_DRV_SYS_GetChipVersion(&enChipType, HI_NULL);
    if ((HI_CHIP_TYPE_HI3716H == enChipType) || (HI_CHIP_TYPE_HI3716C == enChipType))
    {
        VIU_DRV_BoardInit();
    }

    sprintf(ProcName, "%s", HI_MOD_VI);
    pProcItem = HI_DRV_PROC_AddModule(ProcName, HI_NULL, HI_NULL);
    if (!pProcItem)
    {
        HI_FATAL_VI("add %s proc failed.\n", ProcName);
        up(&g_ViMutex);
        return;
    }

    pProcItem->read  = VI_ProcRead;
    pProcItem->write = VI_ProcWrite;
    if ((HI_CHIP_TYPE_HI3716H == enChipType) || (HI_CHIP_TYPE_HI3716C == enChipType))
    {
        Ret = request_irq(VIU_INTERRUPT_LINE, Viu_InterruptHandler, IRQF_DISABLED, HI_MOD_VI, NULL);
        if (HI_SUCCESS != Ret)
        {
            HI_FATAL_VI("request vi interrupt failed.\n");
            up(&g_ViMutex);
            return;
        }
    }

    for (i = 0; i < VI_MAX_CHN_NUM; i++)
    {
        if (g_stViCurCfg[i].bValid)
        {
            Viu_ResumeAttr(i, &g_stViCurCfg[i].stViCfg);
        }

        if (g_stViOptmS[i].bOpened)
        {
            if ((HI_CHIP_TYPE_HI3716H == enChipType) || (HI_CHIP_TYPE_HI3716C == enChipType))
            {
                VIU_DRV_SetIntEn(i, 0, 0xff);
                VIU_DRV_SetChEn(i, 0, 1);
                VIU_DRV_Start(i, 0);
            }
        }
    }

    HI_ERR_VI("%s: VI_DRV_Resume OK !\n", __FUNCTION__);
    up(&g_ViMutex);
}

static long VI_DRV_Ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    long Ret;

    Ret = (long)HI_DRV_UserCopy(file->f_dentry->d_inode, file, cmd, arg, Viu_Ioctl);
    return Ret;
}

HI_S32 Viu_Ioctl(struct inode *inode, struct file  *file, unsigned int cmd, HI_VOID *arg)
{
    HI_S32 Ret;

    Ret = down_interruptible(&g_ViMutex);
    if (Ret)
    {
        HI_ERR_VI("%s: down_interruptible err !\n", __FUNCTION__);
        return Ret;
    }

    switch (cmd)
    {
    case CMD_VI_OPEN:
    {
        VI_CREATE_INFO *pstCreateInfo = (VI_CREATE_INFO *)arg;
        Ret = VI_Create(&(pstCreateInfo->stViAttr), (pstCreateInfo->enViPort));
//        Ret = VI_Create(&(pstCreateInfo->stViAttr), &(pstCreateInfo->enViPort));
        if (HI_SUCCESS == Ret)
        {
            g_pViFileFlag[pstCreateInfo->enViPort] = file;
        }

        break;
    }
    case CMD_VI_CLOSE:
    {
        Ret = VI_Destroy(*(HI_UNF_VI_E*)arg);
        if (HI_SUCCESS == Ret)
        {
            g_pViFileFlag[*(HI_UNF_VI_E*)arg] = NULL;
        }

        break;
    }
    case CMD_VI_START:
    {
        Ret = VI_Start(*(HI_UNF_VI_E*)arg);

        break;
    }
    case CMD_VI_STOP:
    {
        Ret = VI_Stop(*(HI_UNF_VI_E*)arg);

        break;
    }
    case CMD_VI_SET_BUF:
    {
        VI_BUF_ATTR_S *pstAttr;
        pstAttr = (VI_BUF_ATTR_S*)arg;
        Ret = Viu_SetExtBuf(pstAttr->enVi, &pstAttr->stBufAttr);
        break;
    }
    case CMD_VI_SET_ATTR:
    {
        VI_ATTR_S *pstAttr;
        pstAttr = (VI_ATTR_S*)arg;
        Ret = Viu_SetAttr(pstAttr->enVi, &(pstAttr->stAttr));

        /*
        VI_ATTR_S *pstAttr;
        pstAttr = (VI_ATTR_S*)arg;
        Ret = Viu_SetBufAddr(pstAttr->enVi, pstAttr->stAttr.u32ViBufAddr, pstAttr->stAttr.u32BufNum);
         */break;
    }
    case CMD_VI_GET_ATTR:
    {
        VI_ATTR_S *pstAttr;
        pstAttr = (VI_ATTR_S*)arg;
        Ret = Viu_GetAttr(pstAttr->enVi, &(pstAttr->stAttr));
        break;
    }
#if 0
    case CMD_VI_GET_FRAME:
    {
        VI_FRAME_INFO_S *pViFrame;
        pViFrame = (VI_FRAME_INFO_S *)arg;
        Ret = Viu_UsrGetFrame(pViFrame->enVi, &(pViFrame->stViBuf));
        break;
    }
    case CMD_VI_PUT_FRAME:
    {
        VI_FRAME_INFO_S *pViFrame;
        pViFrame = (VI_FRAME_INFO_S *)arg;
        Ret = Viu_UsrPutFrame(pViFrame->enVi, &(pViFrame->stViBuf));
        break;
    }
#else
    case CMD_VI_DQ_FRAME:
    {
        VI_FRAME_INFO_S *pViFrame;
        pViFrame = (VI_FRAME_INFO_S *)arg;
        Ret = Viu_DequeueFrame(pViFrame->enVi, &(pViFrame->stViFrame));
        break;
    }
    case CMD_VI_Q_FRAME:
    {
        VI_FRAME_INFO_S *pViFrame;
        pViFrame = (VI_FRAME_INFO_S *)arg;
        Ret = Viu_QueueFrame(pViFrame->enVi, &(pViFrame->stViFrame));
        break;
    }
#endif
    case CMD_VI_ACQUIRE_FRAME:
    {
        VI_FRAME_INFO_S *pViFrame;
        pViFrame = (VI_FRAME_INFO_S *)arg;
        Ret = Viu_UsrAcquireFrame(pViFrame->enVi, pViFrame->u32Uid, &(pViFrame->stViFrame));
        break;
    }
    case CMD_VI_RELEASE_FRAME:
    {
        VI_FRAME_INFO_S *pViFrame;
        pViFrame = (VI_FRAME_INFO_S *)arg;
        Ret = Viu_UsrReleaseFrame(pViFrame->enVi, pViFrame->u32Uid, &(pViFrame->stViFrame));
        break;
    }
    case CMD_VI_GET_UID:
    {
        VI_UID_INFO_S *pstUIDInfo;
        pstUIDInfo = (VI_UID_INFO_S *)arg;
        Ret = Viu_GetUsrID(pstUIDInfo->enVi, HI_ID_USR_START, &(pstUIDInfo->u32UsrID));
        break;
    }
    case CMD_VI_PUT_UID:
    {
        VI_UID_INFO_S *pstUIDInfo;
        pstUIDInfo = (VI_UID_INFO_S *)arg;
        Ret = Viu_PutUsrID(pstUIDInfo->enVi, pstUIDInfo->u32UsrID);
        break;
    }
    default:
    {
        HI_ERR_VI("vi cmd value err,cmd=%x\n", cmd);
        Ret = HI_ERR_VI_NOT_SUPPORT;
        break;
    }
    }

    up(&g_ViMutex);
    return Ret;
}

static int  vi_pm_suspend(PM_BASEDEV_S *pdev, pm_message_t state)
{
#if 1
    HI_FATAL_VI("entering vi_pm_suspend\n");
    VI_DRV_Suspend();
#else
    int i;
    int ret;

    // 0  now all usr processes are in sleep status, any delay is serious error!
    ret = down_trylock(&g_ViMutex);
    if (ret)
    {
        HI_FATAL_VI("err0: lock !\n");
        return ret;
    }

    // 1.0
    if (atomic_read(&g_ViCount))
    {
        up(&g_ViMutex);
        HI_FATAL_VI("err1: not close all file \n");
        return HI_FAILURE;
    }

    // 1.2
    for (i = 0; i < VI_MAX_CHN_NUM; i++)
    {
        if (g_stViOptmS[i].bOpened)
        {
            up(&g_ViMutex);
            HI_FATAL_VI("err2: chan %d not close \n", i);
            return HI_FAILURE;
        }
    }

    HI_FATAL_VI("ok !\n");
    up(&g_ViMutex);
#endif
    return 0;
}

static int vi_pm_resume(PM_BASEDEV_S *pdev)
{
#if 1
    HI_FATAL_VI("entering VI_DRV_Resume\n");
    VI_DRV_Resume();
#else
 #if 0
    down_trylock(&g_ViMutex);
    up(&g_ViMutex);
 #endif
    HI_FATAL_VI("ok !\n");
#endif
    return 0;
}

static PM_BASEOPS_S vi_drvops =
{
    .probe        = NULL,
    .remove       = NULL,
    .shutdown     = NULL,
    .prepare      = NULL,
    .complete     = NULL,
    .suspend      = vi_pm_suspend,
    .suspend_late = NULL,
    .resume_early = NULL,
    .resume       = vi_pm_resume,
};

HI_S32 VI_DRV_ModInit(HI_VOID)
{
    HI_U32 i;
    HI_U32 Result;
    HI_S32 s32Ret = HI_FAILURE;
    HI_CHIP_TYPE_E enChipType;

    s32Ret = HI_DRV_MODULE_Register(HI_ID_VI, "HI_VI", (HI_VOID*)&s_ViExportFuncs);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_VI("HI_DRV_MODULE_Register failed, mode ID = 0x%08X\n", HI_ID_VI);
        return HI_FAILURE;
    }

    sprintf(g_ViRegisterData.devfs_name, "%s", UMAP_DEVNAME_VI);
    g_ViRegisterData.fops   = &VI_FOPS;
    g_ViRegisterData.minor  = UMAP_MIN_MINOR_VI;
    g_ViRegisterData.owner  = THIS_MODULE;
    g_ViRegisterData.drvops = &vi_drvops;
    if (HI_DRV_DEV_Register(&g_ViRegisterData) < 0)
    {
        HI_FATAL_VI("register VI failed.\n");
        return HI_FAILURE;
    }

    for (i = 0; i < VI_MAX_CHN_NUM; i++)
    {
        g_pViFileFlag[i] = NULL;
    }

    HI_DRV_SYS_GetChipVersion(&enChipType, HI_NULL);
    if ((HI_CHIP_TYPE_HI3716H == enChipType) || (HI_CHIP_TYPE_HI3716C == enChipType))
    {
        vi_clk_or_enable = ioremap_nocache(SYS_REGS_ADDR_BASE, 0x54);
        if (HI_NULL == vi_clk_or_enable)
        {
            HI_FATAL_VI("register VI failed,ioremap error.\n");
            HI_DRV_DEV_UnRegister(&g_ViRegisterData);
            return HI_FAILURE;
        }

        /*open clock 0x101F504C*/
        HI_REG_READ32(SYS_PERI_CRG3_ADDR, Result);
        Result = Result | 0x300;
        HI_REG_WRITE32(SYS_PERI_CRG3_ADDR, Result);

        /*0x101F5050*/
        HI_REG_READ32(SYS_PERI_CRG4_ADDR, Result);
        Result = Result | 0x100;
        HI_REG_WRITE32(SYS_PERI_CRG4_ADDR, Result);

        msleep(10);

        /*reset VI*/
        HI_REG_READ32(SYS_PERI_CRG3_ADDR, Result);
        Result = Result | 0x3;
        HI_REG_WRITE32(SYS_PERI_CRG3_ADDR, Result);
        msleep(10);
        HI_REG_READ32(SYS_PERI_CRG3_ADDR, Result);
        Result = Result & 0xFFFFFFFC;
        HI_REG_WRITE32(SYS_PERI_CRG3_ADDR, Result);
    }

#ifdef MODULE
#ifndef CONFIG_SUPPORT_CA_RELEASE
    printk("Load hi_vi.ko success.\t\t(%s)\n", VERSION_STRING);
#endif
#endif

    return 0;
}

HI_VOID VI_DRV_ModExit(HI_VOID)
{
    HI_CHIP_TYPE_E enChipType;

    //VO_VIUnRegister();

    HI_DRV_SYS_GetChipVersion(&enChipType, HI_NULL);

    if ((HI_CHIP_TYPE_HI3716H == enChipType) || (HI_CHIP_TYPE_HI3716C == enChipType))
    {
        iounmap(vi_clk_or_enable);
        vi_clk_or_enable = HI_NULL;
    }

    HI_DRV_DEV_UnRegister(&g_ViRegisterData);

    HI_DRV_MODULE_UnRegister(HI_ID_VI);

    return;
}

//HI_S32 VI_Create(HI_UNF_VI_ATTR_S *pstAttr, HI_UNF_VI_E *pViPort)
HI_S32 VI_Create(HI_UNF_VI_ATTR_S *pstAttr, HI_UNF_VI_E enViPort)
{
    HI_S32 Ret;
//    HI_U32 addr;
#if 0
    if (HI_NULL == pViPort)
    {
        HI_ERR_VI("NULL pointer phVi\n");
        return HI_ERR_VI_NULL_PTR;
    }
    if ((HI_UNF_VI_PORT0 > pstAttr->enViPort) || (pstAttr->enViPort > HI_UNF_VI_BUTT))
    {
        HI_ERR_VI("ViPort error,is %d\n", pstAttr->enViPort);
        return HI_ERR_VI_NO_CHN_LEFT;
    }
#endif
    if ((HI_UNF_VI_PORT0 > enViPort) || (enViPort > HI_UNF_VI_BUTT))
    {
        HI_ERR_VI("ViPort error,is %d\n", enViPort);
        return HI_ERR_VI_NO_CHN_LEFT;
    }

    HI_INFO_VI("ViPort is %d\n", enViPort);

//    if (g_stViOptmS[pstAttr->enViPort].bOpened)
	if (g_stViOptmS[enViPort].bOpened)
    {
        //*phVi=(HI_ID_VI<<16)+pstAttr->enViPort;
        HI_ERR_VI("VI port %d opened\n", enViPort);
        return HI_ERR_VI_DEV_OPENED;
    }

//    Ret = Viu_SetAttr((HI_U32)(pstAttr->enViPort), pstAttr);
    Ret = Viu_SetAttr((HI_U32)(enViPort), pstAttr);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }
	
    Ret = Viu_OpenChn(enViPort);
//    Ret = Viu_OpenChn(pstAttr->enViPort);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

//    *pViPort = pstAttr->enViPort;
//    addr = pstAttr->u32ViBufAddr;

    if ((pstAttr->enInputMode >= HI_UNF_VI_MODE_BT1120_480P) && (HI_UNF_VI_PORT0 == enViPort))
//    if ((pstAttr->enInputMode >= HI_UNF_VI_MODE_BT1120_480P) && (HI_UNF_VI_PORT0 == pstAttr->enViPort))
//    if ((pstAttr->enInputMode >= HI_UNF_VI_MODE_BT1120_480P)
//        && (HI_UNF_VI_CHN_YC_SEL_Y == pstAttr->enChnYC))
    {
//        pstAttr->enViPort = HI_UNF_VI_PORT1;
        enViPort = HI_UNF_VI_PORT1;

//        pstAttr->enChnYC = HI_UNF_VI_CHN_YC_SEL_C;

//        Ret = Viu_SetAttr((HI_U32)(pstAttr->enViPort), pstAttr);
        Ret = Viu_SetAttr((HI_U32)(enViPort), pstAttr);
        if (Ret != HI_SUCCESS)
        {
            return Ret;
        }

//        pstAttr->u32BufSize = addr;

//        Ret = Viu_OpenChn(pstAttr->enViPort);
        Ret = Viu_OpenChn(enViPort);
        if (Ret != HI_SUCCESS)
        {
            return Ret;
        }
    }

    return HI_SUCCESS;
}

HI_S32 VI_Destroy(HI_UNF_VI_E enVi)
{
    HI_S32 Ret;

    Ret = Viu_CloseChn(enVi, HI_FALSE);
    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }
	
	if ((g_stViCurCfg[enVi].stViCfg.enInputMode >= HI_UNF_VI_MODE_BT1120_480P) && 
		(HI_UNF_VI_PORT0 == g_stViCurCfg[enVi].enViPort))
//	if ((g_stViCurCfg[enVi].stViCfg.enInputMode >= HI_UNF_VI_MODE_BT1120_480P) && 
//		(HI_UNF_VI_PORT0 == g_stViCurCfg[enVi].stViCfg.enViPort))
//    if ((g_stViCurCfg[enVi].stViCfg.enInputMode >= HI_UNF_VI_MODE_BT1120_480P)
//        && (g_stViCurCfg[enVi].stViCfg.enChnYC == HI_UNF_VI_CHN_YC_SEL_Y))
    {
        Ret = Viu_CloseChn(HI_UNF_VI_PORT1, HI_TRUE);
        if (HI_SUCCESS != Ret)
        {
            return Ret;
        }
    }

    return Ret;
}

HI_S32 VI_DestroyForce(HI_UNF_VI_E enVi)
{
    HI_S32 Ret;

    Ret = Viu_CloseChn(enVi, HI_TRUE);
    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }

	if ((g_stViCurCfg[enVi].stViCfg.enInputMode >= HI_UNF_VI_MODE_BT1120_480P) && 
		(HI_UNF_VI_PORT0 == g_stViCurCfg[enVi].enViPort))
//	if ((g_stViCurCfg[enVi].stViCfg.enInputMode >= HI_UNF_VI_MODE_BT1120_480P) && 
//		(HI_UNF_VI_PORT0 == g_stViCurCfg[enVi].stViCfg.enViPort))
//    if ((g_stViCurCfg[enVi].stViCfg.enInputMode >= HI_UNF_VI_MODE_BT1120_480P)
//        && (g_stViCurCfg[enVi].stViCfg.enChnYC == HI_UNF_VI_CHN_YC_SEL_Y))
    {
        Ret = Viu_CloseChn(HI_UNF_VI_PORT1, HI_TRUE);
    }

    return Ret;
}

HI_S32 VI_Start(HI_UNF_VI_E enVi)
{
    if (HI_FALSE == g_stViCurCfg[enVi].stViCfg.bVirtual)
    {
        VIU_DRV_SetChEn(enVi, 0, 1);
    }
    return HI_SUCCESS;
}

HI_S32 VI_Stop(HI_UNF_VI_E enVi)
{
    if (HI_FALSE == g_stViCurCfg[enVi].stViCfg.bVirtual)
    {
        VIU_DRV_SetChEn(enVi, 0, 0);
    }
    return HI_SUCCESS;
}

HI_S32 Viu_ProcShow(struct seq_file *s, HI_VOID *pData)
{
    HI_INFO_VI("ViuProcShow!\n");
    return HI_SUCCESS;
}

static HI_S32 VI_ProcRead(struct seq_file *p, HI_VOID *v)
{
    HI_U32 i = 0;
    HI_U32 userId = 0;
    DRV_PROC_ITEM_S  *pProcItem;
    HI_U32 dTimeDiff;

    VI_STATISTIC_S *pStatInfo;

    struct timeval timeNow;

    pProcItem = p->private;

    for (i = 0; i < VI_MAX_CHN_NUM; i++)
    {
        if (g_stViOptmS[i].bOpened)
        {
            p += seq_printf(p, "\n ------ VI %d info ------\n", i);

            do_gettimeofday(&timeNow);
            if (timeNow.tv_sec > g_stViOptmS[i].stTimeStart.tv_sec)
            {
                dTimeDiff = timeNow.tv_sec - g_stViOptmS[i].stTimeStart.tv_sec;
            }
            else
            {
                dTimeDiff = timeNow.tv_sec + (0xffffffff - g_stViOptmS[i].stTimeStart.tv_sec);
            }

            dTimeDiff = dTimeDiff * 1000;
            dTimeDiff = dTimeDiff + (timeNow.tv_usec - g_stViOptmS[i].stTimeStart.tv_usec) / 1000;

            pStatInfo = &(g_stViOptmS[i].stStatisticInfo);
            p += seq_printf(p,
                            "Rect(X/Y/Width/Height) :%u/%u/%u/%u\n"
                            "Stride(Y/C)            :%u/%u\n"
                            "Int(Get/Lost)          :%u/%u\n",
                            g_stViOptmS[i].u32x, g_stViOptmS[i].u32y,
                            g_stViOptmS[i].u32Width, g_stViOptmS[i].u32Height

                            , g_stViOptmS[i].u32StrideY, g_stViOptmS[i].u32StrideC,
                            Reg_update_int[i], g_u32IntLost[i]);

            p += seq_printf(p,
                            "\nFrame Input(Cam/User->VI): \n"
                            "    Acquire(Try/OK):  %d/%d\n"
                            "    Release(Try/OK):  %d/%d\n\n",
                            pStatInfo->AcquireTry[g_u32KernUserId[i]], pStatInfo->AcquireOK[g_u32KernUserId[i]],
                            pStatInfo->ReleaseTry[g_u32KernUserId[i]], pStatInfo->ReleaseOK[g_u32KernUserId[i]]);

            for (userId = 1; userId < VI_UID_MAX; userId++)
            {
                if (0 != atomic_read(&(g_ViUidUsed[i][userId])))
                {
                    p += seq_printf(p,
                                    "Frame Output(VI -> %s[UserID:%u]): \n"
                                    "    Acquire(Try/OK):  %d/%d\n"
                                    "    Release(Try/OK):  %d/%d\n\n",
                                    (HI_ID_VO == g_ViUserMod[i][userId]) ? "VO" :
                                    ((HI_ID_VENC == g_ViUserMod[i][userId]) ? "VENC" :
                                     ((HI_ID_USR_START == g_ViUserMod[i][userId]) ? "USER" : "ERROR")),
                                    userId,
                                    pStatInfo->AcquireTry[userId], pStatInfo->AcquireOK[userId],
                                    pStatInfo->ReleaseTry[userId], pStatInfo->ReleaseOK[userId]);
                }
            }
        }
        else
        {
            p += seq_printf(p, "\n ------ VI %d NOT open ------\n", i);
        }
    }

    return HI_SUCCESS;
}

static HI_S32 VI_ProcWrite(struct file * file,
                           const char __user * buf, size_t count, loff_t *ppos)

{
    extern VIU_FB_ROOT_S g_struFbRoot[VI_MAX_CHN_NUM];
    HI_CHAR FileName[30];
    static HI_U32 u32FrameDumpCount = 0;
    HI_VOID *VirtualAddr = HI_NULL;
    struct file *fpSaveFile = HI_NULL;
    HI_S32 s32Ret = HI_FAILURE;
    HI_CHAR *p;
    HI_CHAR *org;
    HI_U32 u32Para1;
    HI_U32 u32ViPort;

    /* make sure input parameter is ok */
    if (count >= 1)
    {
        if (g_stViOptmS[HI_UNF_VI_PORT0].bOpened)
        {
            u32ViPort = 0;
        }
        else
        {
            u32ViPort = 1;
        }

        p = (char *)__get_free_page(GFP_KERNEL);

        if (copy_from_user(p, buf, count))
        {
            HI_ERR_VI("copy_from_user failed.\n");
            return HI_FAILURE;
        }

        org = p;
        u32Para1 = (HI_U32)simple_strtoul(p, &p, 10);

        sprintf(FileName, "/hidbg/vi_dump_%d.yuv", u32FrameDumpCount++);

        switch (u32Para1)
        {
        case 1:

            fpSaveFile = VIU_UtilsFopen(FileName, 1);
            if (HI_NULL == fpSaveFile)
            {
                HI_ERR_VI("Can not create %s file.\n", FileName);
                free_page((HI_U32)org);
                org = HI_NULL;
                p = HI_NULL;
                return HI_FAILURE;
            }

            VirtualAddr = ioremap(g_struFbRoot[u32ViPort].struFb[0].u32PhysAddr, g_struFbRoot[u32ViPort].u32BlkSize);
            if (HI_NULL == VirtualAddr)
            {
                HI_ERR_VI("VI_ProcWrite ioremap failed\n");
                VIU_UtilsFclose(fpSaveFile);
                free_page((HI_U32)org);
                org = HI_NULL;
                p = HI_NULL;
                return HI_FAILURE;
            }
			printk("debug, phyaddr = 0x%08x, size = %d\n", g_struFbRoot[u32ViPort].struFb[0].u32PhysAddr, g_struFbRoot[u32ViPort].u32BlkSize);

            s32Ret = VIU_UtilsFwrite(VirtualAddr, g_struFbRoot[u32ViPort].u32BlkSize, fpSaveFile);
            if (g_struFbRoot[u32ViPort].u32BlkSize != s32Ret)
            {
                HI_ERR_VI("VIU_UtilsFwrite failed.\n");
                iounmap(VirtualAddr);
                VIU_UtilsFclose(fpSaveFile);
                free_page((HI_U32)org);
                org = HI_NULL;
                p = HI_NULL;
                return HI_FAILURE;
            }

            iounmap(VirtualAddr);

            VIU_UtilsFclose(fpSaveFile);
            break;

        default:
            HI_ERR_VI("echo 1 > /proc/msp/vi\n");
            break;
        }

        free_page((HI_U32)org);
        org = HI_NULL;
        p = HI_NULL;
    }
    else
    {
        HI_ERR_VI("echo 1 > /proc/msp/vi\n");
    }

    return count;
}

module_init(VI_DRV_ModInit);
module_exit(VI_DRV_ModExit);

MODULE_AUTHOR("HISILICON");
MODULE_LICENSE("GPL");

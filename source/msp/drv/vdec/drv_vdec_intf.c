#include <linux/version.h>
#include <linux/proc_fs.h>
#include <linux/ioport.h>
#include <linux/string.h>
#include <linux/ioctl.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/vmalloc.h>
#include <linux/interrupt.h>
#include <linux/seq_file.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <mach/hardware.h>

#include "hi_unf_avplay.h"
#include "hi_error_mpi.h"

#include "vfmw.h"
#include "drv_vdec_ioctl.h"
#include "hi_drv_vdec.h"
#include "drv_vdec_private.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* End of #ifdef __cplusplus */

static HI_S32	VDEC_DRV_WriteProc(struct file * file,
                                   const char __user * buf, size_t count, loff_t *ppos);
static HI_S32	VDEC_DRV_ReadProc(struct seq_file *p, HI_VOID *v);
static long		VDEC_DRV_Ioctl(struct file *filp, unsigned int cmd, unsigned long arg);

static HI_CHAR *s_aszVdecType[HI_UNF_VCODEC_TYPE_BUTT + 1] =
{
    "MPEG2",
    "MPEG4",
    "AVS",
    "H263",
    "H264",
    "REAL8",
    "REAL9",
    "VC1",
    "VP6",
    "VP6F",
    "VP6A",
    "OTHER",
    "SORENSON",
    "DIVX3",
    "RAW",
    "JPEG",
    "VP8",
    "OTHER",
    "OTHER",
    "OTHER",
    "OTHER",
    "OTHER",
    "OTHER",
    "OTHER",
    "OTHER",
    "OTHER",
    "OTHER",
    "OTHER",
    "OTHER",
    "OTHER",
    "OTHER",
    "OTHER",
    "OTHER",
    "OTHER",
    "OTHER",
    "UNKNOWN",
};

static struct file_operations s_stDevFileOpts =
{
    .owner			= THIS_MODULE,
    .open			= VDEC_DRV_Open,
    .unlocked_ioctl = VDEC_DRV_Ioctl,
    .release		= VDEC_DRV_Release,
};

static PM_BASEOPS_S s_stDrvOps = {
    .probe        = NULL,
    .remove       = NULL,
    .shutdown     = NULL,
    .prepare      = NULL,
    .complete     = NULL,
    .suspend      = VDEC_DRV_Suspend,
    .suspend_late = NULL,
    .resume_early = NULL,
    .resume       = VDEC_DRV_Resume,
};

static VDEC_REGISTER_PARAM_S s_stProcParam = {
    .pfnReadProc  = VDEC_DRV_ReadProc,
    .pfnWriteProc = VDEC_DRV_WriteProc,
};

/* the attribute struct of video decoder device */
static UMAP_DEVICE_S s_stVdecUmapDev;

static long VDEC_DRV_Ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    int ret;

    ret = (long)HI_DRV_UserCopy(filp->f_dentry->d_inode, filp, cmd, arg, VDEC_Ioctl);
    return ret;
}

static __inline__ int  VDEC_DRV_RegisterDev(void)
{
    /*register aenc chn device*/
    snprintf(s_stVdecUmapDev.devfs_name, sizeof(s_stVdecUmapDev.devfs_name), UMAP_DEVNAME_VDEC);
    s_stVdecUmapDev.fops   = &s_stDevFileOpts;
    s_stVdecUmapDev.minor  = UMAP_MIN_MINOR_VDEC;
    s_stVdecUmapDev.owner  = THIS_MODULE;
    s_stVdecUmapDev.drvops = &s_stDrvOps;
    if (HI_DRV_DEV_Register(&s_stVdecUmapDev) < 0)
    {
        HI_FATAL_VDEC("FATAL: vdec register device failed\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static __inline__ void VDEC_DRV_UnregisterDev(void)
{
    /*unregister aenc chn device*/
    HI_DRV_DEV_UnRegister(&s_stVdecUmapDev);
}

static HI_S32 VDEC_DRV_WriteProc(struct file * file,
                                 const char __user * buf, size_t count, loff_t *ppos)
{
    return HI_DRV_PROC_ModuleWrite(file, buf, count, ppos, VDEC_DRV_DebugCtrl);
}

static HI_S32 VDEC_DRV_ReadProc(struct seq_file *p, HI_VOID *v)
{
    HI_S32 i;
    VDEC_CHANNEL_S *pstChan;
    VDEC_CHAN_STATINFO_S *pstStatInfo;
    DRV_PROC_ITEM_S *pstProcItem;
    BUFMNG_STATUS_S stBMStatus = {0};
    HI_CHAR aszDecMode[32];
    HI_CHAR aszDisplayNorm[32];
    HI_CHAR aszSampleType[32];
    HI_CHAR aszYUVType[32];
    HI_CHAR aszRatio[16];
    HI_CHAR aszFieldMode[16];
    HI_CHAR aszDecType[10];
    HI_CHAR aszCapLevel[10];
    HI_CHAR aszProtocolLevel[10];

    pstProcItem = p->private;

    if (0 == strcmp(pstProcItem->entry_name, "vdec_ctrl"))
    {
        return 0;
    }

    sscanf(pstProcItem->entry_name, "vdec%02d", &i);
    if (i >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        p += seq_printf(p, "Invalid VDEC ID:%d.\n", i);
        return 0;
    }

    pstChan = VDEC_DRV_GetChan(i);

    if (pstChan)
    {
        pstStatInfo = &(pstChan->stStatInfo);
        switch (pstChan->stCurCfg.enMode)
        {
        case HI_UNF_VCODEC_MODE_I:
            snprintf(aszDecMode, sizeof(aszDecMode), "I");
            break;
        case HI_UNF_VCODEC_MODE_IP:
            snprintf(aszDecMode, sizeof(aszDecMode), "IP");
            break;
        case HI_UNF_VCODEC_MODE_NORMAL:
            snprintf(aszDecMode, sizeof(aszDecMode), "NORMAL");
            break;
        default:
            snprintf(aszDecMode, sizeof(aszDecMode), "UNKNOWN(%d)", pstChan->stCurCfg.enMode);
            break;
        }

        switch (pstChan->enDisplayNorm)
        {
        case HI_UNF_ENC_FMT_PAL:
            snprintf(aszDisplayNorm, sizeof(aszDisplayNorm), "PAL");
            break;
        case HI_UNF_ENC_FMT_NTSC:
            snprintf(aszDisplayNorm, sizeof(aszDisplayNorm), "NTSC");
            break;
        default:
            snprintf(aszDisplayNorm, sizeof(aszDisplayNorm), "OTHER(%d)", pstChan->enDisplayNorm);
            break;
        }

        if (pstChan->stLastFrm.bProgressive)
        {
            snprintf(aszSampleType, sizeof(aszSampleType), "Progressive");
        }
        else
        {
            snprintf(aszSampleType, sizeof(aszSampleType), "Interlace");
        }

        snprintf(aszRatio, sizeof(aszRatio),  "%d:%d", pstChan->stLastFrm.u32AspectWidth, pstChan->stLastFrm.u32AspectHeight);

        switch (pstChan->stLastFrm.enFieldMode)
        {
        case HI_DRV_FIELD_ALL:
            snprintf(aszFieldMode, sizeof(aszFieldMode), "Frame");
            break;
        case HI_DRV_FIELD_TOP:
            snprintf(aszFieldMode, sizeof(aszFieldMode), "Top");
            break;
        case HI_DRV_FIELD_BOTTOM:
            snprintf(aszFieldMode, sizeof(aszFieldMode), "Bottom");
            break;
        default:
            snprintf(aszFieldMode, sizeof(aszFieldMode), "UNKNOWN");
            break;
        }

        switch (pstChan->stLastFrm.ePixFormat)
        {
        case HI_DRV_PIX_FMT_NV08:
            snprintf(aszYUVType, sizeof(aszYUVType), "SPYCbCr400");
            break;
        case HI_DRV_PIX_FMT_NV12_411:
            snprintf(aszYUVType, sizeof(aszYUVType), "SPYCbCr411");
            break;
        case HI_DRV_PIX_FMT_NV21:
            snprintf(aszYUVType, sizeof(aszYUVType), "SPYCbCr420");
            break;
        case HI_DRV_PIX_FMT_NV16:
            snprintf(aszYUVType, sizeof(aszYUVType), "SPYCbCr422_1X2");
            break;
        case HI_DRV_PIX_FMT_NV16_2X1:
            snprintf(aszYUVType, sizeof(aszYUVType), "SPYCbCr422_2X1");
            break;
        case HI_DRV_PIX_FMT_NV24:
            snprintf(aszYUVType, sizeof(aszYUVType), "SPYCbCr444");
            break;
        /*case HI_UNF_FORMAT_YUV_PACKAGE_UYVY:
            sprintf(aszYUVType, "Package_UYVY");
            break;
        case HI_UNF_FORMAT_YUV_PACKAGE_YUYV:
            sprintf(aszYUVType, "Package_YUYV");
            break;
        case HI_UNF_FORMAT_YUV_PACKAGE_YVYU:
            sprintf(aszYUVType, "Package_YVYU");
            break;*/
        case HI_DRV_PIX_FMT_YUV400:
            snprintf(aszYUVType, sizeof(aszYUVType), "PLNYCbCr400");
            break;
        case HI_DRV_PIX_FMT_YUV411:
            snprintf(aszYUVType, sizeof(aszYUVType), "PLNYCbCr411");
            break;
        case HI_DRV_PIX_FMT_YUV420p:
            snprintf(aszYUVType, sizeof(aszYUVType), "PLNYCbCr420");
            break;
        case HI_DRV_PIX_FMT_YUV422_1X2:
            snprintf(aszYUVType, sizeof(aszYUVType), "PLNYCbCr422_1X2");
            break;
        case HI_DRV_PIX_FMT_YUV422_2X1:
            snprintf(aszYUVType, sizeof(aszYUVType), "PLNYCbCr422_2X1");
            break;
        case HI_DRV_PIX_FMT_YUV_444:
            snprintf(aszYUVType, sizeof(aszYUVType), "PLNYCbCr444");
            break;
        case HI_DRV_PIX_FMT_YUV410p:
            snprintf(aszYUVType, sizeof(aszYUVType), "PLNYCbCr410");
            break;
        default:
            snprintf(aszYUVType, sizeof(aszYUVType), "UNKNOWN");
            break;
        }

        switch (pstChan->stUserCfgCap.enDecType)
        {
        case HI_UNF_VCODEC_DEC_TYPE_NORMAL:
            snprintf(aszDecType, sizeof(aszDecType), "NORMAL");
            break;
        case HI_UNF_VCODEC_DEC_TYPE_ISINGLE:
            snprintf(aszDecType, sizeof(aszDecType), "IFRAME");
            break;
        default:
            snprintf(aszDecType, sizeof(aszDecType), "UNKNOWN");
            break;
        }

        switch (pstChan->stUserCfgCap.enCapLevel)
        {
        case HI_UNF_VCODEC_CAP_LEVEL_QCIF:
            snprintf(aszCapLevel, sizeof(aszCapLevel), "QCIF");
            break;
        case HI_UNF_VCODEC_CAP_LEVEL_CIF:
            snprintf(aszCapLevel, sizeof(aszCapLevel), "CIF");
            break;
        case HI_UNF_VCODEC_CAP_LEVEL_D1:
            snprintf(aszCapLevel, sizeof(aszCapLevel), "D1");
            break;
        case HI_UNF_VCODEC_CAP_LEVEL_720P:
            snprintf(aszCapLevel, sizeof(aszCapLevel), "720P");
            break;
        case HI_UNF_VCODEC_CAP_LEVEL_FULLHD:
            snprintf(aszCapLevel, sizeof(aszCapLevel), "FULLHD");
            break;
        default:
            snprintf(aszCapLevel, sizeof(aszCapLevel), "UNKNOWN");
            break;
        }

        switch (pstChan->stUserCfgCap.enProtocolLevel)
        {
        case HI_UNF_VCODEC_PRTCL_LEVEL_MPEG:
            snprintf(aszProtocolLevel, sizeof(aszProtocolLevel), "NOT_H264");
            break;
        case HI_UNF_VCODEC_PRTCL_LEVEL_H264:
            snprintf(aszProtocolLevel, sizeof(aszProtocolLevel), "H264");
            break;
        default:
            snprintf(aszProtocolLevel, sizeof(aszProtocolLevel), "UNKNOWN");
            break;
        }

        p += seq_printf(p, "====== VDEC%d ======\n", i);
        p += seq_printf(p,
                        "Work State   : %s\n",
                        (VDEC_CHAN_STATE_RUN == pstChan->enCurState) ? "RUN" : "STOP");
        p += seq_printf(p,
                        "Codec ID     : %s(0x%x)\n"
                        "Mode         : %s\n"
                        "Priority     : %u\n"
                        "ErrCover     : %u\n"
                        "OrderOutput  : %u\n"
                        "CtrlOption   : 0x%x\n"
                        "Capbility    : %s/%s/%s\n"
                        "Stream:\n"
                        "         Source: %s(0x%x)\n"
                        "         Total/Current(Byte): 0x%x/0x%x\n"
                        "         BitRate(bps): %u\n"
                        "Picture:\n"
                        "         Resolution: %d*%d\n"
                        "         Stride(Y/C): %#x/%#x\n"
                        "         FrameRate(fps): %u.%u(%d)\n"
                        "         EncFormat: %s\n"
                        "         Aspect: %s\n"
                        "         FieldMode: %s\n"
                        "         Type: %s\n"
                        "         VideoFormat: %s\n"
                        "         TopFirst: %d\n"
                        "         ErrFrame: %u\n"
                        "         TypeStatistics(I/P): %u/%u\n\n"
                        ,

                        (pstChan->stCurCfg.enType
                         <= HI_UNF_VCODEC_TYPE_BUTT) ? (s_aszVdecType[pstChan->stCurCfg.enType]) : "UNKNOW",
                        pstChan->stCurCfg.enType,

                        aszDecMode,
                        pstChan->stCurCfg.u32Priority,
                        pstChan->stCurCfg.u32ErrCover,
                        pstChan->stCurCfg.bOrderOutput,
                        pstChan->stCurCfg.s32CtrlOptions,

                        aszDecType, aszCapLevel, aszProtocolLevel,

                        (HI_INVALID_HANDLE == pstChan->hDmxVidChn) ? "User" : "Demux",
                        (HI_INVALID_HANDLE == pstChan->hDmxVidChn) ? pstChan->hStrmBuf : pstChan->hDmxVidChn,

                        pstStatInfo->u32TotalVdecInByte,
                        pstStatInfo->u32TotalVdecHoldByte,
                        pstStatInfo->u32AvrgVdecInBps,

                        pstChan->stLastFrm.u32Width, pstChan->stLastFrm.u32Height,
                        pstChan->stLastFrm.stBufAddr[0].u32Stride_Y,
                        pstChan->stLastFrm.stBufAddr[0].u32Stride_C,
                        pstStatInfo->u32AvrgVdecFps, pstStatInfo->u32AvrgVdecFpsLittle,
                        pstChan->stLastFrm.u32FrameRate,
                        aszDisplayNorm, aszRatio, aszFieldMode,
                        aszSampleType, aszYUVType, pstChan->stLastFrm.bTopFieldFirst,

                        pstStatInfo->u32VdecErrFrame,
                        pstStatInfo->u32FrameType[0],
                        pstStatInfo->u32FrameType[1]
             );

        if (HI_INVALID_HANDLE == pstChan->hDmxVidChn)
        {
            BUFMNG_GetStatus(pstChan->hStrmBuf, &stBMStatus);
            p += seq_printf(p,
                            "Stream Input(AVPLAY->VDEC): \n"
                            "    Get(Try/OK)   : %d/%d\n"
                            "    Put(Try/OK)   : %d/%d\n",
                            stBMStatus.u32GetTry, stBMStatus.u32GetOK,
                            stBMStatus.u32PutTry, stBMStatus.u32PutOK);
        }

        p += seq_printf(p,
                        "Frame Output(VDEC->VPSS): \n"
                        "    Acquire(Try/OK): %d/%d\n"
                        "    Release(Try/OK): %d/%d\n\n",
                        pstStatInfo->u32UserAcqFrameTry, pstStatInfo->u32UserAcqFrameOK,
                        pstStatInfo->u32UserRlsFrameTry, pstStatInfo->u32UserRlsFrameOK);

        if (HI_INVALID_HANDLE == pstChan->hDmxVidChn)
        {
            p += seq_printf(p,
                            "Firmware: Stream Input(VDEC->Firmware):\n"
                            "    Acquire(Try/OK): %d/%d\n"
                            "    Release(Try/OK): %d/%d\n",
                            stBMStatus.u32RecvTry, stBMStatus.u32RecvOK,
                            stBMStatus.u32RlsTry, stBMStatus.u32RlsOK);
        }
        else
        {
            p += seq_printf(p,
                            "Firmware: Stream Input(VDEC->Firmware):\n"
                            "    Acquire(Try/OK): %d/%d\n"
                            "    Release(Try/OK): %d/%d\n",
                            pstStatInfo->u32VdecAcqBufTry, pstStatInfo->u32VdecAcqBufOK,
                            pstStatInfo->u32VdecRlsBufTry, pstStatInfo->u32VdecRlsBufOK);
        }

        p += seq_printf(p,
                        "Firmware: Frame Output(Firmware->VDEC):\n"
                        "    Acquire(Try/OK): %d/%d\n"
                        "    Release(Try/OK): %d/%d\n\n",
                        pstStatInfo->u32VdecRcvFrameTry, pstStatInfo->u32VdecRcvFrameOK,
                        pstStatInfo->u32VdecRlsFrameTry, pstStatInfo->u32VdecRlsFrameOK);
    }
    else
    {
        p += seq_printf(p, "vdec not init!\n" );
    }

    return 0;
}

HI_S32 VDEC_DRV_ModInit(HI_VOID)
{
    int ret;

#ifndef HI_MCE_SUPPORT
    ret = VDEC_DRV_Init();
    if (HI_SUCCESS != ret)
    {
        HI_FATAL_VDEC("Init drv fail!\n");
        return HI_FAILURE;
    }
#endif


    ret = VDEC_DRV_RegisterProc(&s_stProcParam);
    if (HI_SUCCESS != ret)
    {
        HI_FATAL_VDEC("Reg proc fail!\n");
        return HI_FAILURE;
    }

    ret = VDEC_DRV_RegisterDev();
    if (HI_SUCCESS != ret)
    {
        VDEC_DRV_UnregisterProc();
        HI_FATAL_VDEC("Reg dev fail!\n");
        return HI_FAILURE;
    }

#ifdef MODULE
#ifndef CONFIG_SUPPORT_CA_RELEASE
    printk("Load hi_vdec.ko success.\t(%s)\n", VERSION_STRING);
#endif
#endif

    return HI_SUCCESS;
}

HI_VOID VDEC_DRV_ModExit(HI_VOID)
{
    VDEC_DRV_UnregisterDev();
    VDEC_DRV_UnregisterProc();

#ifndef HI_MCE_SUPPORT
    VDEC_DRV_Exit();
#endif

    return;
}

#ifdef MODULE
module_init(VDEC_DRV_ModInit);
module_exit(VDEC_DRV_ModExit);
#endif

MODULE_AUTHOR("HISILICON");
MODULE_LICENSE("GPL");

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* End of #ifdef __cplusplus */

#include <linux/kernel.h>
#include <mach/hardware.h>
#include <asm/io.h>

#include "hi_module.h"
#include "drv_module_ext.h"
#include "drv_dev_ext.h"
#include "drv_proc_ext.h"

#include "hi_drv_mce.h"
#include "drv_mce_ext.h"
#include "drv_media_mem.h"
#include "drv_pdm_ext.h"
#include "drv_disp_ext.h"
#include "drv_hifb_ext.h"

#define MCE_NAME                "HI_MCE"
#define MMZ_INFOZONE_SIZE       (8*1024)


DECLARE_MUTEX(g_MceMutex);

#define DRV_MCE_Lock()      \
    do{         \
        if(down_interruptible(&g_MceMutex))   \
        {       \
            HI_ERR_MCE("ERR: mce intf lock error!\n");  \
        }       \
      }while(0)

#define DRV_MCE_UnLock()      \
    do{         \
        up(&g_MceMutex);    \
      }while(0)


typedef struct tagMCE_REGISTER_PARAM_S{
    DRV_PROC_READ_FN        rdproc;
    DRV_PROC_WRITE_FN       wtproc;
}MCE_REGISTER_PARAM_S;

MCE_S   g_Mce = 
{
    .hAvplay = HI_INVALID_HANDLE,
    .hWindow = HI_INVALID_HANDLE,
    .bPlayStop = HI_TRUE,
    .bMceExit = HI_TRUE,
    .BeginTime = 0,
    .EndTime = 0,
    .stStopParam.enCtrlMode = HI_UNF_MCE_PLAYCTRL_BUTT,
    .TsplayEnd = HI_FALSE
};

static HI_S32 MCE_ProcRead(struct seq_file *p, HI_VOID *v)
{
    HI_U32      u32BgColor;
    MCE_S       *pMce = &g_Mce;
    HI_UNF_MCE_PLAY_PARAM_S *pPlayParam = HI_NULL;
        
    u32BgColor = (pMce->stDispParam.stBgColor.u8Red << 16) + (pMce->stDispParam.stBgColor.u8Green << 8)
        + pMce->stDispParam.stBgColor.u8Blue;
    
    p += seq_printf(p,"---------------------Hisilicon MCE Out Info-----------------------\n");
    p += seq_printf(p,"----------------------------BaseParam-----------------------------\n");
    p += seq_printf(p,"enFormat:        %10d,   u32Brightness:      %10d\n"
                      "u32Contrast:     %10d,   u32Saturation:      %10d\n"
                      "u32HuePlus:      %10d,   bGammaEnable:       %10d\n"
                      "BgColor:         %10x,   enPixelFormat:      %10d\n"
                      "u32DisplayWidth: %10d,   u32DisplayHeight:   %10d\n"
                      "u32ScreenXpos:   %10d,   u32ScreenYpos:      %10d\n"
                      "u32ScreenWidth:  %10d,   u32ScreenHeight:    %10d\n",
                      pMce->stDispParam.enFormat,pMce->stDispParam.u32Brightness,
                      pMce->stDispParam.u32Contrast,pMce->stDispParam.u32Saturation,
                      pMce->stDispParam.u32HuePlus,pMce->stDispParam.bGammaEnable,
                      u32BgColor,                  pMce->stGrcParam.enPixelFormat,
                      pMce->stGrcParam.u32DisplayWidth,pMce->stGrcParam.u32DisplayHeight,
                      pMce->stGrcParam.u32ScreenXpos,pMce->stGrcParam.u32ScreenYpos,
                      pMce->stGrcParam.u32ScreenWidth,pMce->stGrcParam.u32ScreenHeight
    );
    
#ifdef HI_MCE_SUPPORT    
    p += seq_printf(p,"---------------------------PlayParam------------------------------\n");
    p += seq_printf(p,"enPlayType:      %10d,   bPlayEnable:        %10d\n",
                       pMce->stMceParam.stPlayParam.enPlayType, pMce->stMceParam.stPlayParam.bPlayEnable
                   );

    
    pPlayParam = &pMce->stMceParam.stPlayParam;
    if (HI_UNF_MCE_TYPE_PLAY_DVB == pPlayParam->enPlayType)
    {
        p += seq_printf(p,"u32VideoPid:     %10d,   u32AudioPid:        %10d\n"
                          "enVideoType:     %10d,   enAudioType:        %10d\n"
                          "u32Volume:       %10d,   enTrackMode:        %10d\n"
                          "enSigType:       %10d                            \n",
                          pPlayParam->unParam.stDvbParam.u32VideoPid, pPlayParam->unParam.stDvbParam.u32AudioPid,
                          pPlayParam->unParam.stDvbParam.enVideoType, pPlayParam->unParam.stDvbParam.enAudioType,
                          pPlayParam->unParam.stDvbParam.u32Volume, pPlayParam->unParam.stDvbParam.enTrackMode,
                          pPlayParam->unParam.stDvbParam.stConnectPara.enSigType
                        );

        if (HI_UNF_TUNER_SIG_TYPE_SAT == pPlayParam->unParam.stDvbParam.stConnectPara.enSigType)
        {
            p += seq_printf(p,"u32Freq:         %10d,   u32SymbolRate:      %10d\n"
                              "enPolar:         %10d                            \n",
                              pPlayParam->unParam.stDvbParam.stConnectPara.unConnectPara.stSat.u32Freq,pPlayParam->unParam.stDvbParam.stConnectPara.unConnectPara.stSat.u32SymbolRate,
                              pPlayParam->unParam.stDvbParam.stConnectPara.unConnectPara.stSat.enPolar
                           );
        }
        else if (HI_UNF_TUNER_SIG_TYPE_CAB == pPlayParam->unParam.stDvbParam.stConnectPara.enSigType)
        {
            p += seq_printf(p,"u32Freq:         %10d,   u32SymbolRate:      %10d\n"
                              "enModType:       %10d                            \n",
                              pPlayParam->unParam.stDvbParam.stConnectPara.unConnectPara.stCab.u32Freq,pPlayParam->unParam.stDvbParam.stConnectPara.unConnectPara.stCab.u32SymbolRate,
                              pPlayParam->unParam.stDvbParam.stConnectPara.unConnectPara.stCab.enModType
                           );
        }
    }
    else if (HI_UNF_MCE_TYPE_PLAY_TSFILE == pPlayParam->enPlayType)
    {
        p += seq_printf(p,"u32VideoPid:     %10d,   u32AudioPid:        %10d\n"
                          "enVideoType:     %10d,   enAudioType:        %10d\n"
                          "u32Volume:       %10d,   enTrackMode:        %10d\n"
                          "u32ContentLen:   %10d                            \n",
                          pPlayParam->unParam.stTsParam.u32VideoPid, pPlayParam->unParam.stTsParam.u32AudioPid,
                          pPlayParam->unParam.stTsParam.enVideoType, pPlayParam->unParam.stTsParam.enAudioType,
                          pPlayParam->unParam.stTsParam.u32Volume, pPlayParam->unParam.stTsParam.enTrackMode,
                          pPlayParam->unParam.stTsParam.u32ContentLen
                        );
    
    }
                   
    p += seq_printf(p,"---------------------------PlayStatus-----------------------------\n");
    p += seq_printf(p,"hAvplay:         %10d,   hWindow:            %10d\n"
                      "BeginTime:       %10d,   EndTime:            %10d\n",
                      pMce->hAvplay,    pMce->hWindow,
                      pMce->BeginTime,  pMce->EndTime
                   );
#endif

    return HI_SUCCESS;
}

static HI_S32 MCE_ProcWrite(struct file * file,
    const char __user * buf, size_t count, loff_t *ppos)
{
    return HI_SUCCESS;
}

HI_S32 MCE_DRV_Open(struct inode *finode, struct file  *ffile)
{
    return HI_SUCCESS;
}

HI_S32 MCE_DRV_Close(struct inode *finode, struct file  *ffile)
{
    return HI_SUCCESS;
}

HI_S32 MCE_Ioctl(struct inode *inode, struct file *file, unsigned int cmd, HI_VOID *arg)
{
    HI_S32              Ret = HI_SUCCESS;
    PDM_EXPORT_FUNC_S   *pstPdmFuncs = HI_NULL;
    HIFB_EXPORT_FUNC_S  *pstFbFuncs = HI_NULL;

    DRV_MCE_Lock();

    switch (cmd)
    {
        case HI_MCE_STOP_FASTPLAY_CMD:
        {
#ifdef HI_MCE_SUPPORT         
            Ret = HI_DRV_MCE_Stop((HI_UNF_MCE_STOPPARM_S *)arg);
#endif            
            break;
        }
        case HI_MCE_EXIT_FASTPLAY_CMD:
        {
#ifdef HI_MCE_SUPPORT 
            Ret = HI_DRV_MCE_Exit((HI_UNF_MCE_EXITPARAM_S *)arg);
#endif 
            break;
        }
        case HI_MCE_CLEAR_LOGO_CMD:
        {
		    HI_DRV_MODULE_GetFunction(HI_ID_FB, (HI_VOID **)&pstFbFuncs);
            HI_DRV_MODULE_GetFunction(HI_ID_PDM, (HI_VOID **)&pstPdmFuncs);

            if (HI_NULL != pstFbFuncs)
            {
                (HI_VOID)pstFbFuncs->pfnHifbSetLogoLayerEnable(HI_FALSE);
            }

            if (HI_NULL != pstPdmFuncs)
            {
                /*release the reserve mem for logo*/
                pstPdmFuncs->pfnPDM_ReleaseReserveMem("Optm_GfxWbc2");
                pstPdmFuncs->pfnPDM_ReleaseReserveMem("Display_Buffer"); 
            }
            
            break;
        }
        default:
            DRV_MCE_UnLock();
            return -ENOIOCTLCMD;        
    }

    DRV_MCE_UnLock();
    
    return Ret;
}

static long MCE_DRV_Ioctl(struct file *ffile, unsigned int cmd, unsigned long arg)
{
    HI_S32 Ret;

    Ret = HI_DRV_UserCopy(ffile->f_dentry->d_inode, ffile, cmd, arg, MCE_Ioctl);

    return Ret;
}

HI_S32 MCE_Suspend(PM_BASEDEV_S *pdev, pm_message_t state)
{
    return HI_SUCCESS;
}

HI_S32 MCE_Resume(PM_BASEDEV_S *pdev)
{
    return HI_SUCCESS;
}

static MCE_REGISTER_PARAM_S g_MceProcPara = {
    .rdproc = MCE_ProcRead,
    .wtproc = MCE_ProcWrite,
};

static UMAP_DEVICE_S g_MceRegisterData;


static struct file_operations g_MceFops =
{
    .owner          =    THIS_MODULE,
    .open           =     MCE_DRV_Open,
    .unlocked_ioctl =    MCE_DRV_Ioctl,
    .release        =   MCE_DRV_Close,
};

static PM_BASEOPS_S g_MceDrvOps = {
    .probe        = NULL,
    .remove       = NULL,
    .shutdown     = NULL,
    .prepare      = NULL,
    .complete     = NULL,
    .suspend      = MCE_Suspend,
    .suspend_late = NULL,
    .resume_early = NULL,
    .resume       = MCE_Resume,
};

HI_S32 MCE_DRV_ModInit(HI_VOID)
{
    HI_S32              Ret;
    HI_CHAR             ProcName[16];
    DRV_PROC_ITEM_S     *pProcItem = HI_NULL;

    Ret = HI_DRV_MODULE_Register(HI_ID_FASTPLAY, MCE_NAME, HI_NULL);

    sprintf(ProcName, "%s", HI_MOD_MCE);

    pProcItem = HI_DRV_PROC_AddModule(ProcName, HI_NULL, HI_NULL);
    if(HI_NULL != pProcItem)
    {
        pProcItem->read = g_MceProcPara.rdproc;
        pProcItem->write = g_MceProcPara.wtproc;
    }
        
    sprintf(g_MceRegisterData.devfs_name, UMAP_DEVNAME_MCE);
    g_MceRegisterData.fops = &g_MceFops;
    g_MceRegisterData.minor = UMAP_MIN_MINOR_MCE;
    g_MceRegisterData.owner  = THIS_MODULE;
    g_MceRegisterData.drvops = &g_MceDrvOps;
    if (HI_DRV_DEV_Register(&g_MceRegisterData) < 0)
    {
        HI_FATAL_MCE("register MCE failed.\n");
        return HI_FAILURE;
    }

    return  0;
}

HI_VOID MCE_DRV_ModExit(HI_VOID)
{
    HI_CHAR             ProcName[16];
    
    HI_DRV_DEV_UnRegister(&g_MceRegisterData);

    sprintf(ProcName, "%s", HI_MOD_MCE);
    HI_DRV_PROC_RemoveModule(ProcName);
    
    HI_DRV_MODULE_UnRegister(HI_ID_FASTPLAY);
}

#ifdef MODULE
module_init(MCE_DRV_ModInit);
module_exit(MCE_DRV_ModExit);
#endif

MODULE_AUTHOR("HISILICON");
MODULE_LICENSE("GPL");


/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : drv_disp.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2012/09/20
  Description   :
  History       :
  1.Date        : 
  Author        : 
  Modification  : Created file

*******************************************************************************/
#include <linux/vmalloc.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/fcntl.h>
#include <linux/mm.h>
#include <linux/miscdevice.h>
#include <linux/proc_fs.h>
#include <linux/fs.h>
#include <linux/slab.h>
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 36))
#include <linux/smp_lock.h>
#endif
#include <linux/seq_file.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <asm/system.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/string.h>

#include "drv_dev_ext.h"
#include "drv_proc_ext.h"
#include "hi_kernel_adapt.h"

#include "drv_module_ext.h"

#include "hi_drv_disp.h"
#include "drv_disp.h"
#include "drv_disp_ioctl.h"
#include "drv_display.h"
#include "drv_disp_debug.h"
#include "drv_disp_ext.h"
#include "drv_disp_osal.h"


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

DRV_DISP_STATE_S         g_DispModState;
DRV_DISP_GLOBAL_STATE_S  g_DispUserCountState;
DRV_DISP_GLOBAL_STATE_S  g_DispKernelCountState;
DRV_DISP_GLOBAL_STATE_S  g_DispAllCountState;
HI_BOOL g_DispSuspend = HI_FALSE;
HI_S32 g_s32DispAttachCount = 0;

static atomic_t        g_DispCount = ATOMIC_INIT(0);
HI_DECLARE_MUTEX(g_DispMutex);

HI_S32 DRV_DISP_ProcessCmd(unsigned int cmd, HI_VOID *arg, DRV_DISP_STATE_S *pDispState, HI_BOOL bUser);
HI_S32 DISP_ExtOpen(HI_DRV_DISPLAY_E enDisp, DRV_DISP_STATE_S *pDispState, HI_BOOL bUser);
HI_S32 DISP_ExtClose(HI_DRV_DISPLAY_E enDisp, DRV_DISP_STATE_S *pDispState, HI_BOOL bUser);

/* ================================================ */
#define DEF_DRV_DISP_PROC_FUNCTION_START_FROM_HERE
HI_U8 *g_pDispLayerString[HI_DRV_DISP_LAYER_BUTT] = {
    "NONE",
    "VIDEO_0",
    "VIDEO_1",
    "VIDEO_2",
    "GFX_0",
    "GFX_1",
    "GFX_2",
};

//TODO
HI_U8 *g_pVDPDispFmtString[HI_DRV_DISP_FMT_BUTT] = {
    "1080P60", 
    "1080P50", 
    "1080P30", 
    "1080P25",
    "1080P24",        
    "1080i60",        
    "1080i50",        
    "720P60",         
    "720P50",         

    "576P50",         
    "480P60",         

    "PAL",
    "PAL_B",
    "PAL_B1",
    "PAL_D",
    "PAL_D1",
    "PAL_G",
    "PAL_H",
    "PAL_K",
    "PAL_I",
    "PAL_M",
    "PAL_N",
    "PAL_Nc",
    "PAL_60",

    "NTSC",
    "NTSC_J",
    "NTSC_443",

    "SECAM_SIN",
    "SECAM_COS",
    "SECAM_L",
    "SECAM_B",
    "SECAM_G",
    "SECAM_D",
    "SECAM_K",
    "SECAM_H",

    "1440x576i",
    "1440x480i",

    "1080P24_FP",
    "720P60_FP",
    "720P50_FP",

    "640x480", 
    "800x600", 
    "1024x768", 
    "1280x720",
    "1280x800",
    "1280x1024",
    "1360x768",
    "1366x768",
    "1400x1050",
    "1440x900",        
    "1440x900_RB",

    "1600x900_RB",
    "1600x1200",
    "1680x1050",
    "1920x1080",
    "1920x1200",
    "2048x1152",
    "2560x1600",
    "3840x2160",
    "CustomerTiming"
};

HI_U8 *g_pVDPDispModeString[DISP_STEREO_BUTT] = {
    "2D", 
    "FPK", 
    "SBS_HALF",
    "TAB",
    "FILED_ALTE",
    "LINE_ALTE",
    "SBS_FULL", 
    "L_DEPTH",
    "L_DEPTH_G_DEPTH",
};


HI_U8 *g_pVDPColorSpaceString[HI_DRV_CS_BUTT] = {
    "Unknown", 
    "Default", 
    "BT601_YUV_LIMITED", 
    "BT601_YUV_FULL",
    "BT601_RGB_LIMITED", 
    "BT601_RGB_FULL",
    "NTSC1953",
    "BT470_M",
    "BT470_BG",
    "BT709_YUV_LIMITED", 
    "BT709_YUV_FULL",
    "BT709_RGB_LIMITED", 
    "BT709_RGB_FULL",
    "REC709",
    "SMPT170M",
    "SMPT240M", 
    "BT878",
    "XVYCC",
    "JPEG",
};


HI_U8 *g_pVDPCInterfaceString[HI_DRV_DISP_INTF_ID_MAX] = {
    "YPbPr0", 
    "S_VIDEO0", 
    "CVBS0",
    "VGA0", 
    "HDMI0",
    "HDMI1",
    "HDMI2",
    "BT656_0",
    "BT656_1", 
    "BT656_2",
    "BT1120_0", 
    "BT1120_1",
    "BT1120_2",
    "LCD_0",
    "LCD_1", 
    "LCD_2",
};


HI_U8 *g_pDispMacrovisionString[4] = {"TYPE0", "TYPE1", "TYPE2", "TYPE3"};


#define DISPLAY_INVALID_ID 0xFFFFFFFFul
static HI_U32 s_DispProcId[HI_DRV_DISPLAY_BUTT]={DISPLAY_INVALID_ID};

static HI_S32 DISP_ProcRead(struct seq_file *p, HI_VOID *v);
static HI_S32 DISP_ProcWrite(struct file * file,
    const char __user * buf, size_t count, loff_t *ppos);


static HI_S32 DRV_DISP_ProcInit(HI_VOID)
{ 
    HI_U32 u;

    for(u=0; u<(HI_U32)HI_DRV_DISPLAY_BUTT; u++)
    {
        s_DispProcId[u] = DISPLAY_INVALID_ID;
    }
    
    return HI_SUCCESS;
}

static HI_VOID DRV_DISP_ProcDeInit(HI_VOID)
{ 
    HI_CHAR ProcName[12];
    HI_U32 u;
    
    for(u=0; u<(HI_U32)HI_DRV_DISPLAY_BUTT; u++)
    {
        if (s_DispProcId[u] != DISPLAY_INVALID_ID)
        {
            sprintf(ProcName, "%s%d", HI_MOD_DISP, u);
            HI_DRV_PROC_RemoveModule(ProcName);
        }
    }

    return;
}

static HI_S32 DRV_DISP_ProcAdd(HI_DRV_DISPLAY_E enDisp)
{
    DRV_PROC_ITEM_S  *pProcItem;
    HI_CHAR           ProcName[12];

    /* register HD-display PROC*/
    sprintf(ProcName, "%s%d", HI_MOD_DISP, enDisp);
    pProcItem = HI_DRV_PROC_AddModule(ProcName, HI_NULL, HI_NULL);
    if (!pProcItem)
    {
        HI_FATAL_DISP("add %s proc failed.\n", ProcName);
        DISP_Close(enDisp);

        return HI_ERR_DISP_CREATE_ERR;
    }
    
    pProcItem->read  = DISP_ProcRead;
    pProcItem->write = DISP_ProcWrite;
    pProcItem->data  = (HI_VOID *)enDisp;

    s_DispProcId[enDisp] = enDisp;

    return HI_SUCCESS;
}

static HI_S32 DRV_DISP_ProcDel(HI_DRV_DISPLAY_E enDisp)
{
    HI_CHAR ProcName[12];

    /* register HD-display PROC*/
    sprintf(ProcName, "%s%d", HI_MOD_DISP, enDisp);
    HI_DRV_PROC_RemoveModule(ProcName);

    s_DispProcId[enDisp] = DISPLAY_INVALID_ID;
    return HI_SUCCESS;
}


HI_U8 disp_attr[256];
HI_U8 *Disp_GetAttr(HI_VOID)
{
    memset(disp_attr, 0x0, 256);
#ifdef  HI_DISP_TTX_SUPPORT
    strcat(disp_attr, "ttx ");
#endif

#ifdef HI_DISP_ATTACH_OSD_SUPPORT 
    strcat(disp_attr, "attach_osd ");
#endif

#ifdef  HI_DISP_MODE_TC
    strcat(disp_attr, "tc ");
#endif

#ifdef HI_VO_WRAP_SUPPORT
    strcat(disp_attr, "vo_wrap ");
#endif

#ifdef HI_DISP_CGMS_SUPPOR
    strcat(disp_attr, "cgms ");
#endif

#ifdef HI_DISP_LCD_SUPPORT
    strcat(disp_attr, "lcd ");
#endif

#ifdef HI_VO_MOSAIC_SUPPORT
    strcat(disp_attr, "mosaic ");
#endif

#ifdef HI_VO_SINGLE_VIDEO_SUPPORT
    strcat(disp_attr, "signle_vid ");
#endif

#ifdef HI_VO_STILLFRAME_SUPPORT
    strcat(disp_attr, "vo_still ");
#endif

#ifdef HI_VO_DUMPFRAME_SUPPORT
    strcat(disp_attr, "vo_dump ");
#endif

#ifdef HI_VO_MOSAIC_SUPPORT
    strcat(disp_attr, "mosaic ");
#endif

#ifdef HI_VO_SHARPNESS_SUPPORT
    strcat(disp_attr, "sharp ");
#endif

#ifdef HI_VO_HD_VIDEO_DO_DEI
    strcat(disp_attr, "vo_not_dei ");
#endif
    strcat(disp_attr, "\r\n");
    return disp_attr;
}


static DISP_PROC_INFO_S  s_DispAttr;
static HI_S32 DISP_ProcRead(struct seq_file *p, HI_VOID *v)
{
    DRV_PROC_ITEM_S  *pProcItem;
    HI_DRV_DISPLAY_E enDisp;
    HI_S32 Ret, i;
        
    pProcItem = p->private;
    enDisp = (HI_DRV_DISPLAY_E)pProcItem->data;

    /* Disp PROC*/
    Ret = DISP_GetProcInto(enDisp, &s_DispAttr);
    if (Ret)
    {
        p += seq_printf(p,"---------Get Hisilicon DISP %d Out Info Failed!---------\n", enDisp);
        return HI_SUCCESS;
    }
    p += seq_printf(p,"---------Hisilicon DISP %d State---------\n", enDisp);


    p += seq_printf(p,
        "GlobalCnt            :%d\n"
        "Attach/Suspend       :%d /%d\n"
        "Open/hCast           :%d/ 0x%x\n"
        "OpenCnt[U/K/A]       :%d /%d / %d\n",
        atomic_read(&g_DispCount),
        g_s32DispAttachCount,
        g_DispSuspend,
        g_DispModState.bDispOpen[enDisp],
        g_DispModState.hCastHandle[enDisp],
        g_DispUserCountState.DispOpenNum[enDisp],
        g_DispKernelCountState.DispOpenNum[enDisp],
        g_DispAllCountState.DispOpenNum[enDisp]
        );

    p += seq_printf(p,"---------Hisilicon DISP %d Out Info---------\n", enDisp);

    p += seq_printf(p,
        "Formt/Mode           :%s/ %s\n"
        "RightEyeFirst        :%d\n"
        "Enable/M/S           :%d / %d / %d\n"
        "StartTime            :0x%x\n"
        "Underflow            :%d\n",
        g_pVDPDispFmtString[(HI_S32)s_DispAttr.eFmt],
        g_pVDPDispModeString[(HI_S32)s_DispAttr.eDispMode],
        s_DispAttr.bRightEyeFirst,
        s_DispAttr.bEnable, s_DispAttr.bMaster, s_DispAttr.bSlave,
        s_DispAttr.u32StartTime,
        s_DispAttr.u32Underflow
        );

    /* customer timing */
    if (s_DispAttr.eFmt == HI_DRV_DISP_FMT_CUSTOM)
    {
        p += seq_printf(p,
            "------ TIMING -----\n"
            "V BB/ACT/FB          :%d/ %d/ %d\n"
            "H BB/ACT/FB          :%d/ %d/ %d\n"
            "VPW / HPW            :%d/ %d\n",
            s_DispAttr.stTiming.u32VBB,
            s_DispAttr.stTiming.u32VACT,
            s_DispAttr.stTiming.u32VFB,
            s_DispAttr.stTiming.u32HBB,
            s_DispAttr.stTiming.u32HACT,
            s_DispAttr.stTiming.u32HFB,
            s_DispAttr.stTiming.u32VPW,
            s_DispAttr.stTiming.u32HPW
            );
    }

    p += seq_printf(p,
        "AR/Custom            :[%d vs %d] /[%d vs %d]\n"
        "AdjRetc(x/y/w/h)     :%d/%d/%d/%d\n"
        "CS/Custom(I/O)       :%s->%s / %s->%s\n"
        "Bright               :%d\n"
        "Contrast             :%d\n"
        "Saturation           :%d\n"
        "Hue                  :%d\n"
        "Background (R/G/B)   :0x%x/0x%x/0x%x\n",
        s_DispAttr.u32AR_w,
        s_DispAttr.u32AR_h,
        s_DispAttr.u32CustomAR_w,
        s_DispAttr.u32CustomAR_h,
        s_DispAttr.stAdjRect.s32X,
        s_DispAttr.stAdjRect.s32Y,
        s_DispAttr.stAdjRect.s32Width,
        s_DispAttr.stAdjRect.s32Height,
        g_pVDPColorSpaceString[(HI_S32)s_DispAttr.eMixColorSpace],
        g_pVDPColorSpaceString[(HI_S32)s_DispAttr.eDispColorSpace],
        g_pVDPColorSpaceString[(HI_S32)s_DispAttr.stColorSetting.enInCS],
        g_pVDPColorSpaceString[(HI_S32)s_DispAttr.stColorSetting.enOutCS],
        s_DispAttr.stColorSetting.u32Bright,
        s_DispAttr.stColorSetting.u32Contrst,
        s_DispAttr.stColorSetting.u32Satur,
        s_DispAttr.stColorSetting.u32Hue,
        s_DispAttr.stBgColor.u8Red,
        s_DispAttr.stBgColor.u8Green,
        s_DispAttr.stBgColor.u8Blue
        );  

    // interface
    p += seq_printf(p,"---------interface Info---------\n");
    for(i=0; i<s_DispAttr.u32IntfNumber;i++)
    {
        p += seq_printf(p,
            "INTF(Y_G/PB_B/PR_R)  :%s(%d/%d/%d)\n",
            g_pVDPCInterfaceString[(HI_S32)s_DispAttr.stIntf[i].eID],
            s_DispAttr.stIntf[i].u8VDAC_Y_G,
            s_DispAttr.stIntf[i].u8VDAC_Pb_B,
            s_DispAttr.stIntf[i].u8VDAC_Pr_R
            );
    }

    p += seq_printf(p,"---------miracst Info---------\n");

    //mirrorcast
    p += seq_printf(p,
        "hCast                :0x%x\n",
        s_DispAttr.hCast
        );

    return HI_SUCCESS;
}

HI_S32 DISP_ProcParsePara(HI_CHAR *pProcPara,HI_CHAR **ppItem,HI_CHAR **ppValue)
{
    HI_CHAR *pChar = HI_NULL;
    HI_CHAR *pItem,*pValue;

    pChar = strchr(pProcPara,'=');
    if (HI_NULL == pChar)
    {
        return HI_FAILURE; /* Not Found '=' */
    }

    pItem = pProcPara;
    pValue = pChar + 1;
    *pChar = '\0';

    /* remove blank bytes from item tail */
    pChar = pItem;
    while(*pChar != ' ' && *pChar != '\0')
    {
        pChar++;
    }
    *pChar = '\0';
    
    /* remove blank bytes from value head */
    while(*pValue == ' ')
    {
        pValue++;
    }

    *ppItem = pItem;
    *ppValue = pValue;
    return HI_SUCCESS;
}

HI_VOID DISP_ProcPrintHelp(HI_VOID)
{
    printk("Please input these commands:\n"
           "echo enfromat = 0(1080P60)|1(1080P50)|2(1080p30)|3(1080p25)|4(1080p24)|\n"
                "5(1080i60)|6(1080i50)|7(720p60)|8(720p50)|9(576p50)|10(480p60)\n "
                "11(pal)|12(pal_n)|13(pal_nc)|14(ntsc)|15(ntsc_j)|16(ntsc_pal_m)\n "
                "17(secam_sin)|18(secam_cos) > /proc/msp/dispX\n"
           "echo bright = 0~100 > /proc/msp/dispxxx\n"
           "echo contrast = 0~100 > /proc/msp/dispxxx\n"
           "echo saturation = 0~100 > /proc/msp/dispxxx\n");
}

static HI_S32 DISP_ProcWrite(struct file * file,
    const char __user * buf, size_t count, loff_t *ppos)
{
    struct seq_file   *p = file->private_data;
    DRV_PROC_ITEM_S  *pProcItem = p->private;
    HI_DRV_DISPLAY_E enDisp;
        
    pProcItem = p->private;
    enDisp = (HI_DRV_DISPLAY_E)pProcItem->data;

#if 0
    HI_S32      bright, contrast, saturation;
    HI_CHAR           ProcPara[64];
    HI_CHAR           *pItem,*pValue;
    HI_S32            Ret;
    HI_DRV_DISPLAY_E  enintf;
    HI_DRV_DISP_FMT_E enEncFmt;
    HI_S32 Ret;

    if (copy_from_user(ProcPara, buf, count))
    {
        return -EFAULT;
    }
    if (NULL != g_pDebug2VouFanc)
    {
        stVouDebugModule.eModule = HI_VOU_DEBUG_MODULE_DISP;
        stVouDebugModule.ID =(pProcItem->entry_name[4] - '0');
        g_pDebug2VouFanc->pfnVOU_DebugProcWrite(stVouDebugModule,ProcPara,count);
        return count;
    }
    else
    {
        printk("new debug disp is null!!");
    }
    Ret = DISP_ProcParsePara(ProcPara,&pItem,&pValue);
    if (HI_SUCCESS != Ret)
    {
        DISP_ProcPrintHelp();
        return -EFAULT;
    }

    enintf = (pProcItem->entry_name[4] - '0');

    if (!strcmp(pItem,"enfromat"))
    {
        enEncFmt = simple_strtol(pValue, NULL, 10);
        DISP_SetFormat(enintf, enEncFmt);
    }
    else if (!strcmp(pItem,"bright"))
    {
        bright = simple_strtol(pValue, NULL, 10);   
        DISP_SetBright(enintf, bright);
    }
    else if (!strcmp(pItem,"contrast"))
    {
        contrast = simple_strtol(pValue, NULL, 10);
        DISP_SetContrast(enintf, contrast);

    }
    else if (!strcmp(pItem,"saturation"))
    {
        saturation = simple_strtol(pValue, NULL, 10);
        DISP_SetSaturation(enintf, saturation);
    }
#endif

    return count;
}



/***************************************************************/
#define DEF_DRV_DISP_FILE_FUNCTION_START_FROM_HERE

HI_S32 DISP_CheckPara(HI_DRV_DISPLAY_E enDisp, DRV_DISP_STATE_S *pDispState)
{
    if ((enDisp < HI_DRV_DISPLAY_BUTT) && pDispState->bDispOpen[enDisp])
    {
        return HI_SUCCESS;
    }

    return HI_ERR_DISP_INVALID_PARA;
}

HI_S32 DISP_FileOpen(struct inode *finode, struct file  *ffile)
{
    DRV_DISP_STATE_S *pDispState = HI_NULL;
    HI_DRV_DISPLAY_E u;
    HI_S32 Ret;

    Ret = down_interruptible(&g_DispMutex);

    pDispState = HI_KMALLOC(HI_ID_DISP, sizeof(DRV_DISP_STATE_S), GFP_KERNEL);
    if (!pDispState)
    {
        HI_FATAL_DISP("malloc pDispState failed.\n");
        up(&g_DispMutex);
        return -1;
    }

    if (1 == atomic_inc_return(&g_DispCount))
    {
        /* for configuration such as start clock, pins re-use, etc  */
        Ret = DISP_Init();
        if (Ret != HI_SUCCESS)
        {
            HI_KFREE(HI_ID_DISP, pDispState);
            HI_FATAL_DISP("call DISP_Init failed.\n");
            atomic_dec(&g_DispCount);
            up(&g_DispMutex);
            return -1;
        }
    }

    for(u=0; u<HI_DRV_DISPLAY_BUTT; u++)
    {
        pDispState->bDispOpen[u] = HI_FALSE;
        pDispState->hCastHandle[u] = HI_NULL;
    }

    ffile->private_data = pDispState;

    up(&g_DispMutex);
    return 0;
}


HI_S32 DISP_FileClose(struct inode *finode, struct file  *ffile)
{
    DRV_DISP_STATE_S *pDispState;
    HI_DRV_DISPLAY_E u;
    HI_S32 Ret;

    Ret = down_interruptible(&g_DispMutex);

    pDispState = ffile->private_data;


    for(u=0; u<HI_DRV_DISPLAY_BUTT; u++)
    {
        if (pDispState->bDispOpen[u])
        {
            //DEBUG_PRINTK("DISP_FileClose close hd gain \n");
            Ret = DISP_ExtClose(u, pDispState, HI_TRUE);
            if (Ret != HI_SUCCESS)
            {
                HI_ERR_DISP("Display %d close failed!\n", u);
            }
        }

        if (pDispState->hCastHandle[u])
        {
            //DEBUG_PRINTK("DISP_FileClose close hd gain \n");
            Ret = DISP_DestroyCast(pDispState->hCastHandle[u]);
            if (Ret != HI_SUCCESS)
            {
                HI_ERR_DISP("Display %d close failed!\n", u);
            }
        }
    }

    if (atomic_dec_and_test(&g_DispCount))
    {
        //DRV_DISP_ProcDeInit();

        /* for close of clock */
        DISP_DeInit();

        //DEBUG_PRINTK("DISP_FileClose DISP_DeInit\n");

        // add for multiple process
        g_s32DispAttachCount = 0;
    }

    HI_KFREE(HI_ID_DISP, ffile->private_data);

    up(&g_DispMutex);
    return 0;
}


HI_S32 DRV_DISP_Ioctl(struct inode *inode, struct file *file, unsigned int cmd, HI_VOID *arg)
{
    DRV_DISP_STATE_S *pDispState;
    HI_S32 Ret;

    Ret = down_interruptible(&g_DispMutex);

    pDispState = file->private_data;

    Ret = DRV_DISP_ProcessCmd(cmd, arg, pDispState, HI_TRUE);

    up(&g_DispMutex);
    return Ret;
}


/***************************************************************/
#define DEF_DRV_DISP_DRV_FUNCTION_START_FROM_HERE

HI_S32 DRV_DISP_Process(HI_U32 cmd, HI_VOID *arg)
{
    DRV_DISP_STATE_S    *pDispState;
    HI_S32          Ret;

    Ret = down_interruptible(&g_DispMutex);

    pDispState = &g_DispModState;

    Ret = DRV_DISP_ProcessCmd(cmd, arg, pDispState, HI_FALSE);

    up(&g_DispMutex);

    return Ret;
}

HI_U32 DISP_ResetCountStatus(void)
{
    HI_DRV_DISPLAY_E u;
    
    for(u=0; u<HI_DRV_DISPLAY_BUTT; u++)
    {
        g_DispAllCountState.DispOpenNum[u] = 0;
        g_DispUserCountState.DispOpenNum[u] = 0;
        g_DispKernelCountState.DispOpenNum[u] = 0;
        g_DispModState.bDispOpen[u] = HI_FALSE;
        g_DispModState.hCastHandle[u] = HI_NULL;
    }

    g_DispSuspend = HI_FALSE;
    g_s32DispAttachCount = 0;
    
    return HI_SUCCESS;
}


HI_U32 DISP_Get_CountStatus(void)
{
    HI_DRV_DISPLAY_E u;
    for(u=0; u<HI_DRV_DISPLAY_BUTT; u++)
    {
        if (g_DispAllCountState.DispOpenNum[u] > 0)
        {
            return HI_TRUE;
        }
    }

    return HI_FALSE;
}

HI_S32 DISP_ExtOpen(HI_DRV_DISPLAY_E enDisp, DRV_DISP_STATE_S *pDispState, HI_BOOL bUser)
{ 
    HI_S32            Ret;

    /* create DISP for the first time */
    if (!pDispState->bDispOpen[enDisp])
    {
        /* call basic interface for the first time creating DISP globally*/
        if (!g_DispAllCountState.DispOpenNum[enDisp])
        {
            Ret = DISP_Open(enDisp);
            if (Ret != HI_SUCCESS)
            {
                HI_FATAL_DISP("call DISP_Open failed.\n"); 
                return Ret;
            }
        }

        pDispState->bDispOpen[enDisp] = HI_TRUE;

        g_DispAllCountState.DispOpenNum[enDisp]++;

        if (HI_TRUE == bUser)
        {
            g_DispUserCountState.DispOpenNum[enDisp]++;
        }
        else
        {
            g_DispKernelCountState.DispOpenNum[enDisp]++;
        }
    }

    return HI_SUCCESS;
}

HI_S32 DISP_ExtClose(HI_DRV_DISPLAY_E enDisp, DRV_DISP_STATE_S *pDispState, HI_BOOL bUser)
{
    HI_S32       Ret = HI_SUCCESS;

    /* destroy DISP */
    if (pDispState->bDispOpen[enDisp])
    {
        if (HI_TRUE == bUser)
        {
            if (g_DispUserCountState.DispOpenNum[enDisp] == 0)
            {
                HI_WARN_DISP("Already Close User display%d =0\n", enDisp);
                return 0;
            }

            g_DispUserCountState.DispOpenNum[enDisp]--;  /* User count --   */
        }
        else
        {
            if (g_DispKernelCountState.DispOpenNum[enDisp] == 0)
            {
            HI_WARN_DISP("Already Close kernel display%d =0\n", enDisp);
            return 0;
            }
            g_DispKernelCountState.DispOpenNum[enDisp]--;

        }
        
        g_DispAllCountState.DispOpenNum[enDisp]--;  /* Global count -- */

        if (!g_DispAllCountState.DispOpenNum[enDisp])
        {
            Ret = DISP_Close(enDisp);
            if (Ret != HI_SUCCESS)
            {
                HI_FATAL_DISP("call DISP_Close failed.\n");
            }

            g_s32DispAttachCount = 0;
        }

        pDispState->bDispOpen[enDisp] = HI_FALSE;
    }

    return Ret;
}


HI_S32 DISP_ExtAttach(HI_DRV_DISPLAY_E enMaster, HI_DRV_DISPLAY_E enSlave)
{
    HI_S32 nRet;
    
    if ( (enMaster != HI_DRV_DISPLAY_1) || (enSlave != HI_DRV_DISPLAY_0))
    {
        HI_FATAL_DISP("Attach parameters invalid.\n");
        return HI_ERR_DISP_INVALID_OPT;
    }

    if (!g_s32DispAttachCount)
    {
        nRet = DISP_Attach(enMaster, enSlave);
    }
    else
    {
        nRet = HI_SUCCESS;
    }

    if (HI_SUCCESS == nRet)
    {
        g_s32DispAttachCount++;
    }

    return nRet;
}

HI_S32 DISP_ExtDetach(HI_DRV_DISPLAY_E enMaster, HI_DRV_DISPLAY_E enSlave)
{
    HI_S32 nRet = HI_FAILURE;
    
    if ( (enMaster != HI_DRV_DISPLAY_1) || (enSlave != HI_DRV_DISPLAY_0))
    {
        HI_FATAL_DISP("Attach parameters invalid.\n");
        return HI_ERR_DISP_INVALID_OPT;
    }

    if (g_s32DispAttachCount <= 0)
    {
        return HI_SUCCESS;
    }

    if (1 == g_s32DispAttachCount)
    {
        nRet = DISP_Detach(enMaster, enSlave);
    }
    else
    {
        nRet = HI_SUCCESS;
    }

    if (HI_SUCCESS == nRet)
    {
        g_s32DispAttachCount--;
    }

    return nRet;
}


/* DRV_DISP_XXX */
HI_S32 DRV_DISP_Attach(HI_DRV_DISPLAY_E enMaster, HI_DRV_DISPLAY_E enSlave)
{
    HI_S32 Ret;
    DISP_ATTACH_S  enDispAttach;
    
    enDispAttach.enMaster = enMaster;
    enDispAttach.enSlave  = enSlave;
    Ret = DRV_DISP_Process(CMD_DISP_ATTACH, &enDispAttach);
    return Ret;
}

HI_S32 DRV_DISP_Detach(HI_DRV_DISPLAY_E enMaster, HI_DRV_DISPLAY_E enSlave)
{
    HI_S32 Ret;
    DISP_ATTACH_S  enDispAttach;
    
    enDispAttach.enMaster = enMaster;
    enDispAttach.enSlave  = enSlave;
    Ret = DRV_DISP_Process(CMD_DISP_DETACH, &enDispAttach);
    return Ret;
}

HI_S32 DRV_DISP_SetFormat(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_FMT_E enFormat)
{    
    HI_S32 Ret;
    DISP_FORMAT_S  enDispFormat;

    enDispFormat.enDisp = enDisp;
    enDispFormat.enFormat = enFormat;
    Ret = DRV_DISP_Process(CMD_DISP_SET_FORMAT,  &enDispFormat);
    return Ret;
}

HI_S32 DRV_DISP_GetFormat(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_FMT_E *penFormat)
{    
    HI_S32 Ret;
    DISP_FORMAT_S  enDispFormat;

    if (!penFormat)
    {
        return HI_FAILURE;
    }

    enDispFormat.enDisp = enDisp;
    Ret = DRV_DISP_Process(CMD_DISP_GET_FORMAT,  &enDispFormat);
    if (!Ret)
    {
        *penFormat = enDispFormat.enFormat;
    }
    return Ret;
}

HI_S32 DRV_DISP_SetTiming(HI_DRV_DISPLAY_E enDisp,  HI_DRV_DISP_TIMING_S *pstTiming)
{    
    HI_S32 Ret;
    DISP_TIMING_S  DispTiming;
    DispTiming.enDisp = enDisp;
    memcpy(&DispTiming.stTimingPara, pstTiming, sizeof(HI_DRV_DISP_TIMING_S));
    Ret = DRV_DISP_Process(CMD_DISP_SET_TIMING, &DispTiming);
    return Ret;
}

HI_S32 DRV_DISP_AddIntf(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_INTF_S *pstIntf)
{
    HI_S32          Ret;
    DISP_SET_INTF_S DispIntf;

    DispIntf.enDisp = enDisp;

    memcpy(&DispIntf.stIntf, pstIntf, sizeof(HI_DRV_DISP_INTF_S));

    Ret = DRV_DISP_Process(CMD_DISP_ADD_INTF, &DispIntf);
    return Ret;
}

HI_S32 DRV_DISP_AttachVDAC(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_INTF_ID_E eVdacId, HI_DRV_DISP_VDAC_SIGNAL_E eSignal)
{
    return HI_FAILURE;
}

HI_S32 DRV_DISP_Open(HI_DRV_DISPLAY_E enDisp)
{
    HI_S32 Ret;
    
    Ret = DRV_DISP_Process(CMD_DISP_OPEN, &enDisp);
    return Ret;
}

HI_S32 DRV_DISP_Close(HI_DRV_DISPLAY_E enDisp)
{
    HI_S32 Ret;
    
    Ret = DRV_DISP_Process(CMD_DISP_CLOSE, &enDisp);
    return Ret;
}

HI_S32 DRV_DISP_SetBgColor(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_COLOR_S *pstBgColor)
{
    HI_S32 Ret;
    DISP_BGC_S  enDispBgc;

    enDispBgc.stBgColor = *pstBgColor;
    enDispBgc.enDisp = enDisp;
    Ret = DRV_DISP_Process(CMD_DISP_SET_BGC, &enDispBgc);
    return Ret;
}


HI_S32 DRV_DISP_SetColor(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_COLOR_SETTING_S *pstCS)
{
    HI_S32 Ret;
    DISP_COLOR_S stDispColor;

    stDispColor.enDisp = enDisp;
    stDispColor.stColor= *pstCS;
    
    Ret = DRV_DISP_Process(CMD_DISP_SET_COLOR, &stDispColor);
    return Ret;
}

HI_S32 DRV_DISP_GetColor(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_COLOR_SETTING_S *pstCS)
{
    HI_S32 Ret;
    DISP_COLOR_S stDispColor;

    stDispColor.enDisp = enDisp;
    Ret = DRV_DISP_Process(CMD_DISP_GET_COLOR, &stDispColor);
    if(!Ret)
    {
        *pstCS = stDispColor.stColor;
    }
    return Ret;
}


HI_S32 DRV_DISP_SetScreen(HI_DRV_DISPLAY_E enDisp, HI_RECT_S *pstRect)
{
    HI_S32 Ret;
    DISP_SCREEN_S stDispScreen;

    stDispScreen.enDisp = enDisp;
    stDispScreen.stRect = *pstRect;
    Ret = DRV_DISP_Process(CMD_DISP_SET_SCREEN, &stDispScreen);
    return Ret;
}

HI_S32 DRV_DISP_SetAspectRatio(HI_DRV_DISPLAY_E enDisp, HI_U32 u32Ratio_h, HI_U32 u32Ratio_v)
{
    HI_S32 Ret;
    DISP_ASPECT_RATIO_S stDispRatio;

    stDispRatio.enDisp = enDisp;
    stDispRatio.u32ARHori = u32Ratio_h;
    stDispRatio.u32ARVert = u32Ratio_v;
    Ret = DRV_DISP_Process(CMD_DISP_SET_DEV_RATIO, &stDispRatio);
    return Ret;
}

HI_S32 DRV_DISP_SetLayerZorder(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_LAYER_E enLayer, HI_DRV_DISP_ZORDER_E enZFlag)
{
    HI_S32 Ret;
    DISP_ZORDER_S stDispZorder;

    stDispZorder.enDisp = enDisp;
    stDispZorder.Layer  = enLayer;
    stDispZorder.ZFlag  = enZFlag;
    Ret = DRV_DISP_Process(CMD_DISP_SET_ZORDER, &stDispZorder);
    return Ret;
}

HI_S32 DRV_DISP_GetInitFlag(HI_BOOL *pbInited)
{
    HI_S32 Ret;

    Ret = down_interruptible(&g_DispMutex);

    Ret = DISP_GetInitFlag(pbInited);

    up(&g_DispMutex);

    return Ret;
}

HI_S32 DRV_DISP_GetVersion(HI_DRV_DISP_VERSION_S *pstVersion)
{
    HI_S32 Ret;

    Ret = down_interruptible(&g_DispMutex);

    Ret = DISP_GetVersion(pstVersion);

    up(&g_DispMutex);

    return Ret;
}

HI_BOOL DRV_DISP_IsOpened(HI_DRV_DISPLAY_E enDisp)
{
    HI_S32 Ret;

    Ret = down_interruptible(&g_DispMutex);

    Ret = DISP_IsOpened(enDisp);

    up(&g_DispMutex);

    return Ret;
}

HI_S32 DRV_DISP_GetSlave(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISPLAY_E *penSlave)
{
    HI_S32 Ret;

    Ret = down_interruptible(&g_DispMutex);

    Ret = DISP_GetSlave(enDisp, penSlave);

    up(&g_DispMutex);

    return Ret;
}

HI_S32 DRV_DISP_GetMaster(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISPLAY_E *penMaster)
{
    HI_S32 Ret;

    Ret = down_interruptible(&g_DispMutex);

    Ret = DISP_GetMaster(enDisp, penMaster);

    up(&g_DispMutex);

    return Ret;
}

HI_S32 DRV_DISP_GetDisplayInfo(HI_DRV_DISPLAY_E enDisp, HI_DISP_DISPLAY_INFO_S *pstInfo)
{
    HI_S32 Ret;

    Ret = down_interruptible(&g_DispMutex);

    Ret = DISP_GetDisplayInfo(enDisp, pstInfo);

    up(&g_DispMutex);

    return Ret;
}

HI_S32 DRV_DISP_RegCallback(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_CALLBACK_TYPE_E eType,
                            HI_DRV_DISP_CALLBACK_S *pstCallback)
{    
    HI_S32 Ret;

    Ret = down_interruptible(&g_DispMutex);
    Ret = DISP_RegCallback(enDisp, eType, pstCallback);
    up(&g_DispMutex);
    return Ret;
}

HI_S32 DRV_DISP_UnRegCallback(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_CALLBACK_TYPE_E eType,
                              HI_DRV_DISP_CALLBACK_S *pstCallback)
{    
    HI_S32 Ret;

    Ret = down_interruptible(&g_DispMutex);
    Ret = DISP_UnRegCallback(enDisp, eType, pstCallback);
    up(&g_DispMutex);
    return Ret;
}

HI_S32 DRV_DISP_Suspend(PM_BASEDEV_S *pdev, pm_message_t state)
{
    HI_S32 Ret;

    Ret = down_trylock(&g_DispMutex);
    if (Ret)
    {
        HI_FATAL_DISP("down g_DispMutex failed.\n");
        return -1;
    }

    /* no process opened the equipment, return directly */
    if (!atomic_read(&g_DispCount))
    {
        up(&g_DispMutex);
        return 0;
    }

    DISP_Suspend();

    msleep(50);

    g_DispSuspend = HI_TRUE;

    HI_FATAL_DISP("DISP suspend OK.\n");

    up(&g_DispMutex);

    return 0;
}

HI_S32 DRV_DISP_Resume(PM_BASEDEV_S *pdev)
{
    HI_S32  Ret;

    Ret = down_trylock(&g_DispMutex);
    if (Ret)
    {
    HI_FATAL_DISP("down g_DispMutex failed.\n");
    return -1;
    }

    /* no process opened the equipment, return directly */
    if (!atomic_read(&g_DispCount))
    {
        up(&g_DispMutex);
        return 0;
    }

    DISP_Resume();
    
    g_DispSuspend = HI_FALSE;

    HI_FATAL_DISP("DISP resume OK.\n");

    up(&g_DispMutex);


    return 0;
}

HI_S32 DRV_DISP_ProcessCmd(unsigned int cmd, HI_VOID *arg, DRV_DISP_STATE_S *pDispState, HI_BOOL bUser)
{
    HI_S32          Ret = HI_FAILURE;

    switch (cmd)
    {
        case CMD_DISP_ATTACH:
            {
                DISP_ATTACH_S  *pDispAttach;

                pDispAttach = (DISP_ATTACH_S *)arg;

                Ret = DISP_ExtAttach(pDispAttach->enMaster, pDispAttach->enSlave);

                break;
            }

        case CMD_DISP_DETACH:
            {
                DISP_ATTACH_S  *pDispAttach;

                pDispAttach = (DISP_ATTACH_S *)arg;

                Ret = DISP_ExtDetach(pDispAttach->enMaster, pDispAttach->enSlave);

                break;
            }

        case CMD_DISP_OPEN:
            {
                Ret = DISP_ExtOpen(*((HI_DRV_DISPLAY_E *)arg), pDispState, bUser);         

                break;
            }

        case CMD_DISP_CLOSE:
            {
                Ret = DISP_CheckPara(*((HI_DRV_DISPLAY_E *)arg), pDispState);
                if (HI_SUCCESS == Ret)
                {
                    Ret = DISP_ExtClose(*((HI_DRV_DISPLAY_E *)arg), pDispState, bUser);
                }
                else
                {
                    Ret = HI_SUCCESS;
                }

                break;
            }
#if 0
        case CMD_DISP_AttachOsd:
            {
                DISP_OSD_S  *pDispOsd;

                pDispOsd = (DISP_OSD_S *)arg;

                Ret = DISP_AttachOsd(pDispOsd->enDisp, pDispOsd->enLayer);

                break;
            }

        case CMD_DISP_DetachOsd:
            {
                DISP_OSD_S  *pDispOsd;

                pDispOsd = (DISP_OSD_S *)arg;

                Ret = DISP_DetachOsd(pDispOsd->enDisp, pDispOsd->enLayer);

                break;
            }
#endif
        case CMD_DISP_SET_ENABLE:
            {
                DISP_ENABLE_S  *pDispEnable;

                pDispEnable = (DISP_ENABLE_S *)arg;

                Ret = DISP_CheckPara(pDispEnable->enDisp, pDispState);
                if (HI_SUCCESS == Ret)
                {
                    Ret = DISP_SetEnable(pDispEnable->enDisp, pDispEnable->bEnable);
                }

                break;
            }

        case CMD_DISP_GET_ENABLE:
            {
                DISP_ENABLE_S  *pDispEnable;

                pDispEnable = (DISP_ENABLE_S *)arg;

                Ret = DISP_GetEnable(pDispEnable->enDisp, &pDispEnable->bEnable);

                break;
            }

        case CMD_DISP_ADD_INTF:
            {
                DISP_SET_INTF_S  *pDispIntf;

                pDispIntf = (DISP_SET_INTF_S *)arg;

                Ret = DISP_AddIntf(pDispIntf->enDisp, &pDispIntf->stIntf);

                break;
            }
        case CMD_DISP_DEL_INTF:
            {
                DISP_SET_INTF_S  *pDispIntf;

                pDispIntf = (DISP_SET_INTF_S *)arg;
                Ret = DISP_DelIntf(pDispIntf->enDisp, &pDispIntf->stIntf);

                break;

            }

#if 0
        case CMD_DISP_GET_INTF:
            {
                DISP_GET_INTF_S  *pDispIntf;

                pDispIntf = (DISP_GET_INTF_S *)arg;

                Ret = DISP_GetIntf(pDispIntf->enDisp, &(pDispIntf->u32IntfNum), (pDispIntf->stIntf));

                break;
            }
#endif
        case CMD_DISP_SET_FORMAT:
            {
                DISP_FORMAT_S  *pDispFormat;

                pDispFormat = (DISP_FORMAT_S *)arg;

                Ret = DISP_SetFormat(pDispFormat->enDisp, pDispFormat->enStereo, pDispFormat->enFormat);

                break;
            }

        case CMD_DISP_GET_FORMAT:
            {
                DISP_FORMAT_S  *pDispFormat;

                pDispFormat = (DISP_FORMAT_S *)arg;

                Ret = DISP_GetFormat(pDispFormat->enDisp, &pDispFormat->enFormat);

                break;
            }
        
        case CMD_DISP_SET_R_E_FIRST:
            {
                DISP_R_EYE_FIRST_S  *pREFirst;

                pREFirst = (DISP_R_EYE_FIRST_S *)arg;

                Ret = DISP_SetRightEyeFirst(pREFirst->enDisp, pREFirst->bREFirst);

                break;
            }

        case CMD_DISP_SET_TIMING:
            {
                DISP_TIMING_S  *pDispTiming;

                pDispTiming = (DISP_TIMING_S *)arg;
                Ret = DISP_SetCustomTiming(pDispTiming->enDisp, &pDispTiming->stTimingPara);
                break;
            }
        case CMD_DISP_GET_TIMING:
            {
                DISP_TIMING_S  *pDispTiming;
                pDispTiming = (DISP_TIMING_S *)arg;
                Ret = DISP_GetCustomTiming(pDispTiming->enDisp, &pDispTiming->stTimingPara);
                break;
            }

        case CMD_DISP_SET_ZORDER:
            {
                DISP_ZORDER_S   *pDispZorder;

                pDispZorder = (DISP_ZORDER_S *)arg;

                Ret = DISP_SetLayerZorder(pDispZorder->enDisp, pDispZorder->Layer, pDispZorder->ZFlag);

                break;
            }

        case CMD_DISP_GET_ORDER:
            {
                DISP_ORDER_S    *pDispOrder;

                pDispOrder = (DISP_ORDER_S *)arg;

                Ret = DISP_GetLayerZorder(pDispOrder->enDisp, pDispOrder->Layer, &pDispOrder->Order);

                break;
            }
        case CMD_DISP_SET_DEV_RATIO:
            {
                DISP_ASPECT_RATIO_S *pDispAspectRatio;

                pDispAspectRatio = (DISP_ASPECT_RATIO_S *)arg;

                Ret = DISP_SetAspectRatio(pDispAspectRatio->enDisp, pDispAspectRatio->u32ARHori, pDispAspectRatio->u32ARVert);

                break;
            }
        case CMD_DISP_GET_DEV_RATIO:
            {
                DISP_ASPECT_RATIO_S *pDispAspectRatio;

                pDispAspectRatio = (DISP_ASPECT_RATIO_S *)arg;

                Ret = DISP_GetAspectRatio(pDispAspectRatio->enDisp, &pDispAspectRatio->u32ARHori, &pDispAspectRatio->u32ARVert);
                break;
            }
        case CMD_DISP_SET_BGC:
            {
                DISP_BGC_S  *pDispBgc;

                pDispBgc = (DISP_BGC_S *)arg;

                Ret = DISP_SetBGColor(pDispBgc->enDisp, &pDispBgc->stBgColor);

                break;
            }

        case CMD_DISP_GET_BGC:
            {
                DISP_BGC_S  *pDispBgc;

                pDispBgc = (DISP_BGC_S *)arg;

                Ret = DISP_GetBGColor(pDispBgc->enDisp, &pDispBgc->stBgColor);

                break;
            }

        case CMD_DISP_SET_BRIGHT:
            {
                DISP_CSC_S  *pDispCsc;
                HI_DRV_DISP_COLOR_SETTING_S stCS;

                pDispCsc = (DISP_CSC_S *)arg;

                Ret = DISP_GetColor(pDispCsc->enDisp, &stCS);
                if (!Ret)
                {
                    stCS.u32Bright = pDispCsc->CscValue;
                    Ret = DISP_SetColor(pDispCsc->enDisp, &stCS);
                }

                break;
            }

        case CMD_DISP_GET_BRIGHT:
            {
                DISP_CSC_S  *pDispCsc;
                HI_DRV_DISP_COLOR_SETTING_S stCS;

                pDispCsc = (DISP_CSC_S *)arg;

                Ret = DISP_GetColor(pDispCsc->enDisp, &stCS);
                pDispCsc->CscValue = stCS.u32Bright;

                break;
            }

        case CMD_DISP_SET_CONTRAST:
            {
                DISP_CSC_S  *pDispCsc;
                HI_DRV_DISP_COLOR_SETTING_S stCS;

                pDispCsc = (DISP_CSC_S *)arg;

                Ret = DISP_GetColor(pDispCsc->enDisp, &stCS);
                if (!Ret)
                {
                    stCS.u32Contrst = pDispCsc->CscValue;
                    Ret = DISP_SetColor(pDispCsc->enDisp, &stCS);
                }
                break;
            }

        case CMD_DISP_GET_CONTRAST:
            {
                DISP_CSC_S  *pDispCsc;
                HI_DRV_DISP_COLOR_SETTING_S stCS;

                pDispCsc = (DISP_CSC_S *)arg;

                Ret = DISP_GetColor(pDispCsc->enDisp, &stCS);
                pDispCsc->CscValue = stCS.u32Contrst;
                break;
            }

        case CMD_DISP_SET_SATURATION:
            {
                DISP_CSC_S  *pDispCsc;
                HI_DRV_DISP_COLOR_SETTING_S stCS;

                pDispCsc = (DISP_CSC_S *)arg;

                Ret = DISP_GetColor(pDispCsc->enDisp, &stCS);
                if (!Ret)
                {
                    stCS.u32Satur = pDispCsc->CscValue;
                    Ret = DISP_SetColor(pDispCsc->enDisp, &stCS);
                }
                break;
            }

        case CMD_DISP_GET_SATURATION:
            {
                DISP_CSC_S  *pDispCsc;
                HI_DRV_DISP_COLOR_SETTING_S stCS;

                pDispCsc = (DISP_CSC_S *)arg;

                Ret = DISP_GetColor(pDispCsc->enDisp, &stCS);
                pDispCsc->CscValue = stCS.u32Satur;
                break;
            }

        case CMD_DISP_SET_HUE:
            {
                DISP_CSC_S  *pDispCsc;
                HI_DRV_DISP_COLOR_SETTING_S stCS;

                pDispCsc = (DISP_CSC_S *)arg;

                Ret = DISP_GetColor(pDispCsc->enDisp, &stCS);
                if (!Ret)
                {
                    stCS.u32Hue = pDispCsc->CscValue;
                    Ret = DISP_SetColor(pDispCsc->enDisp, &stCS);
                }

                break;
            }

        case CMD_DISP_GET_HUE:
            {
                DISP_CSC_S  *pDispCsc;
                HI_DRV_DISP_COLOR_SETTING_S stCS;

                pDispCsc = (DISP_CSC_S *)arg;

                Ret = DISP_GetColor(pDispCsc->enDisp, &stCS);
                pDispCsc->CscValue = stCS.u32Hue;

                break;
            }

        case CMD_DISP_SET_ALG:
            {
                DISP_ALG_S    *pDispAlg;

                pDispAlg = (DISP_ALG_S *)arg;

                //Ret = DISP_SetAccEnable(pDispAlg->enDisp, pDispAlg->stAlg.bAccEnable);

                break;
            }
        case CMD_DISP_GET_ALG:
            {
                DISP_ALG_S    *pDispAlg;

                pDispAlg = (DISP_ALG_S *)arg;

                //Ret = DISP_GetAccEnable(pDispAlg->enDisp, &pDispAlg->stAlg.bAccEnable);

                break;
            }
#if 0
        case CMD_DISP_SEND_TTX:
            {
                DISP_TTX_S  *pDispTtx;

                pDispTtx = (DISP_TTX_S *)arg;

                Ret = DISP_SendTtxData(pDispTtx->enDisp, &pDispTtx->TtxData);

                break;
            }
#endif

        case CMD_DISP_CREATE_VBI_CHANNEL:
            {
                DISP_VBI_CREATE_CHANNEL_S  *pstDispVbiCrtChanl;

                pstDispVbiCrtChanl = (DISP_VBI_CREATE_CHANNEL_S *)arg;

                Ret = DISP_CreateVBIChannel(pstDispVbiCrtChanl->enDisp,  &pstDispVbiCrtChanl->stCfg, &pstDispVbiCrtChanl->hVbi);


                break;
            }

        case CMD_DISP_DESTROY_VBI_CHANNEL:
            {
                HI_HANDLE  *pnVbiHandle;

                pnVbiHandle = (HI_HANDLE *)arg;

                Ret = DISP_DestroyVBIChannel(*pnVbiHandle);

                break;
            }

        case CMD_DISP_SEND_VBI:
            {
                DISP_VBI_S  *pDispVbi;

                pDispVbi = (DISP_VBI_S *)arg;

                Ret = DISP_SendVbiData(pDispVbi->hVbi, &pDispVbi->stVbiData);

                break;
            }

        case CMD_DISP_SET_WSS:
            {
                DISP_WSS_S  *pDispWss;

                pDispWss = (DISP_WSS_S *)arg;

                Ret = DISP_SetWss(pDispWss->enDisp, &pDispWss->WssData);

                break;
            }

        case CMD_DISP_SET_MCRVSN:
            {
                DISP_MCRVSN_S  *pDispMcrvsn;

                pDispMcrvsn = (DISP_MCRVSN_S *)arg;

                if (pDispMcrvsn->pPriv)
                {
                    Ret = DISP_SetMacrovisionCustomer(pDispMcrvsn->enDisp, pDispMcrvsn->pPriv);
                }

                Ret = DISP_SetMacrovision(pDispMcrvsn->enDisp, pDispMcrvsn->eMcrvsn);

                break;
            }

        case CMD_DISP_GET_MCRVSN:
            {
                DISP_MCRVSN_S  *pDispMcrvsn;

                pDispMcrvsn = (DISP_MCRVSN_S *)arg;

                Ret = DISP_GetMacrovision(pDispMcrvsn->enDisp, &pDispMcrvsn->eMcrvsn);

                break;
            }
#if 0
        case CMD_DISP_GET_HDMI_INTF:
            {
                DISP_HDMIINF_S  *pDispHdmiIntf;

                pDispHdmiIntf = (DISP_HDMIINF_S *)arg;

                Ret = DISP_GetHdmiIntf(pDispHdmiIntf->enDisp, &pDispHdmiIntf->HDMIInf);

                break;
            }

        case CMD_DISP_SET_HDMI_INTF:
            {
                DISP_HDMIINF_S  *pDispHdmiIntf;

                pDispHdmiIntf = (DISP_HDMIINF_S *)arg;

                Ret = DISP_SetHdmiIntf(pDispHdmiIntf->enDisp, &pDispHdmiIntf->HDMIInf);

                break;
            }
#endif
            /* 2011-05-18 HuangMinghu
             * CGMS
             */
        case CMD_DISP_SET_CGMS:
            {
                //DEBUG_PRINTK("#DRV_DISP_Ioctl@disp_intf_k.c\n");

                DISP_CGMS_S  *pDispCgms;

                pDispCgms = (DISP_CGMS_S *)arg;

                Ret = DISP_SetCGMS_A(pDispCgms->enDisp, &pDispCgms->stCgmsCfg);

                break;
            }
        case CMD_DISP_SET_DISP_SCREEN:
            {
                //DEBUG_PRINTK("#DRV_DISP_Ioctl@disp_intf_k.c\n");

                DISP_OUTRECT_S  *pDispOutRect;

                pDispOutRect = (DISP_OUTRECT_S *)arg;

                Ret = DISP_SetScreen(pDispOutRect->enDisp, &pDispOutRect->stOutRectCfg);

                break;
            }  
        case CMD_DISP_GET_DISP_SCREEN:
            {
                //DEBUG_PRINTK("#DRV_DISP_Ioctl@disp_intf_k.c\n");

                DISP_OUTRECT_S  *pDispOutRect;

                pDispOutRect = (DISP_OUTRECT_S *)arg;
                /*To do*/
                Ret = DISP_SetScreen(pDispOutRect->enDisp, &pDispOutRect->stOutRectCfg);
                break;
            }

         case CMD_DISP_CREATE_CAST:
            {
                DISP_CAST_CREATE_S *pstC = (DISP_CAST_CREATE_S *)arg;

                /*To do*/
                //printk(">>>>>>>>>>>>>>>> CMD_DISP_CREATE_CAST >>>>>>>>>>>>. \n");
                Ret = DISP_CreateCast(pstC->enDisp, &pstC->stCfg, &pstC->hCast);
                if (!Ret)
                {
                    pDispState->hCastHandle[pstC->enDisp] = pstC->hCast;
                }
                break;
            }
        case CMD_DISP_DESTROY_CAST:
            {
                DISP_CAST_DESTROY_S *pstC = (DISP_CAST_DESTROY_S *)arg;
                HI_S32 i;

                for (i=0; i<HI_DRV_DISPLAY_BUTT; i++)
                {
                    if (pDispState->hCastHandle[i] == pstC->hCast)
                    {
                        Ret = DISP_DestroyCast(pstC->hCast);
                        pDispState->hCastHandle[i] = HI_NULL;
                    }
                }

                break;
            }
        case CMD_DISP_SET_CAST_ENABLE:
            {
                DISP_CAST_ENABLE_S *pstC = (DISP_CAST_ENABLE_S *)arg;

                /*To do*/
                Ret = DISP_SetCastEnable(pstC->hCast, pstC->bEnable);
                break;
            }

        case CMD_DISP_GET_CAST_ENABLE:
            {
                DISP_CAST_ENABLE_S *pstC = (DISP_CAST_ENABLE_S *)arg;

                /*To do*/
                Ret = DISP_GetCastEnable(pstC->hCast, &pstC->bEnable);
                break;
            }

        case CMD_DISP_ACQUIRE_CAST_FRAME:
            {
                DISP_CAST_FRAME_S *pstC = (DISP_CAST_FRAME_S *)arg;

                /*To do*/
                Ret = DISP_AcquireCastFrame(pstC->hCast, &pstC->stFrame);
                break;
            }

        case CMD_DISP_RELEASE_CAST_FRAME:
            {
                DISP_CAST_FRAME_S *pstC = (DISP_CAST_FRAME_S *)arg;

                /*To do*/
                Ret = DISP_ReleaseCastFrame(pstC->hCast, &pstC->stFrame);
                break;
            }
        case CMD_DISP_SUSPEND:
            {
                /*To do*/
                Ret = DISP_Suspend();
                break;
            }
        case CMD_DISP_RESUME:
            {
                /*To do*/
                Ret = DISP_Resume();
                break;
            }

        default:
            // ????
            //up(&g_DispMutex);
            return -ENOIOCTLCMD;
    }

    return Ret;
}


HI_S32 DRV_DISP_Init2(HI_VOID)
{
    atomic_set(&g_DispCount, 1);

    DRV_DISP_ProcInit();

    DRV_DISP_ProcAdd(HI_DRV_DISPLAY_0);
    DRV_DISP_ProcAdd(HI_DRV_DISPLAY_1);

    DISP_Init();

    DRV_DISP_Open(HI_DRV_DISPLAY_0);
    DRV_DISP_Open(HI_DRV_DISPLAY_1);
    
    
    return HI_SUCCESS;
}

HI_S32 DRV_DISP_DeInit2(HI_VOID)
{
    DRV_DISP_ProcDel(HI_DRV_DISPLAY_1);
    DRV_DISP_ProcDel(HI_DRV_DISPLAY_0);

    DRV_DISP_ProcDeInit();

    DRV_DISP_Close(HI_DRV_DISPLAY_0);
    DRV_DISP_Close(HI_DRV_DISPLAY_1);

    /* closing clock */
    DISP_DeInit();

    atomic_set(&g_DispCount, 0);

    return HI_SUCCESS;
}

//may be delete
HI_S32 DRV_DISP_Init(HI_VOID)
{
    HI_S32          Ret;

    Ret = down_interruptible(&g_DispMutex);

    if (1 == atomic_inc_return(&g_DispCount))
    {
        /* for configuration such as start clock, re-use pins, etc */
        Ret = DISP_Init();
        if (Ret != HI_SUCCESS)
        {
            HI_FATAL_DISP("call DISP_Init failed.\n");
            atomic_dec(&g_DispCount);
            up(&g_DispMutex);
            return -1;
        }
    }

    up(&g_DispMutex);
    return Ret;
}

//HI_S32 HI_DRV_DISP_ModDeinit(HI_VOID)
HI_S32 DRV_DISP_DeInit(HI_VOID)
{
    HI_S32 Ret;
    HI_DRV_DISPLAY_E u;

    Ret = down_interruptible(&g_DispMutex);

    //HI_INFO_DISP("come to close HD/SD g_DispModState.bDisp1Open:%d, g_DispModState.bDisp0Open:%d\n", 
    //        g_DispModState.bDisp1Open, g_DispModState.bDisp0Open);

    for(u=0; u<HI_DRV_DISPLAY_BUTT; u++)
    {
        if (g_DispModState.bDispOpen[u])
        {
            HI_INFO_DISP("DISP_MOD_ExtClose HD0\n");
            Ret = DISP_ExtClose(u, &g_DispModState, HI_FALSE);
            if (Ret != HI_SUCCESS)
            {
                HI_FATAL_DISP("DISP_MOD_ExtClose Display %d failed!\n", u);
            }
        }
    }

    HI_INFO_DISP("HI_DRV_DISP_Deinit:atomic g_DispCount:%d\n", atomic_read(&g_DispCount));

    if (atomic_dec_and_test(&g_DispCount))
    {
        HI_INFO_DISP("close clock\n");

        /* closing clock */
        DISP_DeInit();
    }

    up(&g_DispMutex);
    return 0;
}


static DISP_EXPORT_FUNC_S s_stDispExportFuncs = {
    .DRV_DISP_Init             = DRV_DISP_Init            ,
    .DRV_DISP_DeInit           = DRV_DISP_DeInit          ,
    .DRV_DISP_Attach           = DRV_DISP_Attach          ,
    .DRV_DISP_Detach           = DRV_DISP_Detach          ,
    .DRV_DISP_SetFormat        = DRV_DISP_SetFormat       ,
    .DRV_DISP_GetFormat        = DRV_DISP_GetFormat       ,
    .DRV_DISP_SetTiming        = DRV_DISP_SetTiming       ,
    //.DRV_DISP_GetTiming        = DRV_DISP_GetTiming       ,
    .DRV_DISP_AddIntf          = DRV_DISP_AddIntf         ,
    //.DRV_DISP_DelIntf          = DRV_DISP_DelIntf         ,

    .DRV_DISP_Open             = DRV_DISP_Open            ,
    .DRV_DISP_Close            = DRV_DISP_Close           ,
    //.DRV_DISP_SetEnable        = DRV_DISP_SetEnable       ,
    //.DRV_DISP_GetEnable        = DRV_DISP_GetEnable       ,
    .DRV_DISP_SetBgColor       = DRV_DISP_SetBgColor      ,
    //.DRV_DISP_GetBgColor       = DRV_DISP_GetBgColor      ,
    .DRV_DISP_SetColor         = DRV_DISP_SetColor        ,
    .DRV_DISP_GetColor         = DRV_DISP_GetColor        ,
    .DRV_DISP_SetScreen        = DRV_DISP_SetScreen       ,
    //.DRV_DISP_GetScreen        = DRV_DISP_GetScreen       ,
    .DRV_DISP_SetAspectRatio   = DRV_DISP_SetAspectRatio  ,
    //.DRV_DISP_GetAspectRatio   = DRV_DISP_GetAspectRatio  ,
    .DRV_DISP_SetLayerZorder   = DRV_DISP_SetLayerZorder  ,
    //.DRV_DISP_GetLayerZorder   = DRV_DISP_GetLayerZorder  ,

    .DRV_DISP_GetInitFlag      = DRV_DISP_GetInitFlag     ,
    .DRV_DISP_GetVersion       = DRV_DISP_GetVersion      ,
    .DRV_DISP_IsOpened         = DRV_DISP_IsOpened        ,
    .DRV_DISP_GetSlave         = DRV_DISP_GetSlave        ,
    .DRV_DISP_GetMaster        = DRV_DISP_GetMaster       ,
    .DRV_DISP_GetDisplayInfo   = DRV_DISP_GetDisplayInfo  ,

    .FN_DISP_Ioctl         = DRV_DISP_Process,
    .FN_DISP_RegCallback   = DRV_DISP_RegCallback,
    .FN_DISP_UnRegCallback = DRV_DISP_UnRegCallback,
};

HI_S32 DRV_DISP_Register(HI_VOID)
{   
    HI_S32 Ret;

    // add for multiple process
    DISP_ResetCountStatus();

    //DRV_DISP_ProcInit();

    Ret = HI_DRV_MODULE_Register((HI_U32)HI_ID_DISP, "HI_DISP", (HI_VOID *)(&s_stDispExportFuncs));     
    if (HI_SUCCESS != Ret)
    {
        HI_FATAL_DISP("HI_DRV_MODULE_Register DISP failed\n");
        return Ret;
    }

    return  0;
}

HI_VOID DRV_DISP_UnRegister(HI_VOID)
{

    HI_DRV_MODULE_UnRegister((HI_U32)HI_ID_DISP);

    //DRV_DISP_ProcDeInit();

    return;
}


/* ======================================================================= */
#if 0
HI_S32 DRV_DISP_SetBrightness(HI_DRV_DISPLAY_E enDisp, HI_U32 u32Brightness)
{
    HI_S32 Ret;
    DISP_CSC_S  enDispCsc;

    enDispCsc.CscValue = u32Brightness;
    enDispCsc.enDisp = enDisp;
    Ret = DRV_DISP_Process(CMD_DISP_SET_BRIGHT,  &enDispCsc);
    return Ret;
}

HI_S32 DRV_DISP_SetContrast(HI_DRV_DISPLAY_E enDisp, HI_U32 u32Contrast)
{
    HI_S32 Ret;
    DISP_CSC_S  enDispCsc;

    enDispCsc.CscValue = u32Contrast;
    enDispCsc.enDisp = enDisp;
    Ret = DRV_DISP_Process(CMD_DISP_SET_CONTRAST,  &enDispCsc);
    return Ret;
}

HI_S32 DRV_DISP_SetSaturation(HI_DRV_DISPLAY_E enDisp, HI_U32 u32Saturation)
{
    HI_S32 Ret;
    DISP_CSC_S  enDispCsc;

    enDispCsc.CscValue = u32Saturation;
    enDispCsc.enDisp = enDisp;
    Ret = DRV_DISP_Process(CMD_DISP_SET_SATURATION,  &enDispCsc);
    return Ret;
}

HI_S32 DRV_DISP_SetHuePlus(HI_DRV_DISPLAY_E enDisp, HI_U32 u32HuePlus)
{    
    HI_S32 Ret;
    DISP_CSC_S  enDispCsc;

    enDispCsc.CscValue = u32HuePlus;
    enDispCsc.enDisp = enDisp;
    Ret = DRV_DISP_Process(CMD_DISP_SET_HUE,  &enDispCsc);
    return Ret;
}


HI_S32 DRV_DISP_SetMacrovision(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_MACROVISION_E enMode, HI_VOID *pData)
{    
    HI_S32 Ret;
    DISP_MCRVSN_S  enDispMcrvsn;

    enDispMcrvsn.enDisp = enDisp;
    enDispMcrvsn.eMcrvsn = enMode;
    Ret = DRV_DISP_Process(CMD_DISP_SET_MCRVSN,  &enDispMcrvsn);
    return Ret;
}
#endif


#if 0
//#ifndef MODULE

/* for intf */
EXPORT_SYMBOL(DRV_DISP_Open);
EXPORT_SYMBOL(DRV_DISP_Close);
EXPORT_SYMBOL(DRV_DISP_Attach);
EXPORT_SYMBOL(DRV_DISP_Detach);
EXPORT_SYMBOL(DRV_DISP_AddIntf);
EXPORT_SYMBOL(DRV_DISP_SetFormat);
EXPORT_SYMBOL(DRV_DISP_SetTiming);
//EXPORT_SYMBOL(DRV_DISP_SetHuePlus);
//EXPORT_SYMBOL(DRV_DISP_SetSaturation);
//EXPORT_SYMBOL(DRV_DISP_SetContrast);
//EXPORT_SYMBOL(DRV_DISP_SetBrightness);

EXPORT_SYMBOL(DRV_DISP_SetBgColor);
//EXPORT_SYMBOL(DRV_DISP_SetMacrovision);

EXPORT_SYMBOL(DRV_DISP_Suspend);
EXPORT_SYMBOL(DRV_DISP_Resume);

EXPORT_SYMBOL(DRV_DISP_Register);
EXPORT_SYMBOL(DRV_DISP_UnRegister);

EXPORT_SYMBOL(DRV_DISP_Ioctl);

EXPORT_SYMBOL(DRV_DISP_Init);
EXPORT_SYMBOL(DRV_DISP_DeInit);

EXPORT_SYMBOL(DRV_DISP_ProcRegister);
EXPORT_SYMBOL(DRV_DISP_ProcUnRegister);

#endif




#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

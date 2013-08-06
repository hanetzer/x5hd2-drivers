/******************************************************************************

  Copyright (C), 2010-2020, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
 File Name     :  drv_hdmi.c
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2010/6/20
Description   :
History       :
Date          : 2010/11/22
Author        : l00168554
Modification  :
 *******************************************************************************/
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/vmalloc.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/timer.h>
#include <linux/delay.h>
#include <asm/uaccess.h>
#include <asm/io.h>

#include <linux/device.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <asm/delay.h>
#include <linux/poll.h>
#include <mach/hardware.h>
#include <linux/interrupt.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>
#include <linux/string.h>

#include "drv_hdmi.h"



//#include "mpi_priv_hdmi.h"
#include "hi_drv_hdmi.h"

#include "hi_unf_hdmi.h"

#include "si_audio.h"
#include "si_hdmitx.h"
#include "si_infofrm.h"
#include "si_txapidefs.h"
#include "si_defstx.h"
#include "si_hdcpdefs.h"
#include "si_eedefs.h"
#include "si_de.h"
#include "si_edid.h"
#include "si_txvidp.h"
#include "si_timer.h"
#include "si_eeprom.h"
#include "si_cec.h"
#include "si_phy.h"

#include "hi_unf_disp.h"
#include "si_mpi_hdmi.h"
#include "drv_disp_ext.h"
#include "hi_kernel_adapt.h"
#include "drv_cipher_ext.h"
#include "drv_sys_ext.h"

#if defined (HDCP_SUPPORT)
extern CIPHER_RegisterFunctionlist_S *g_stCIPHERExportFunctionLists;
#endif

extern DISP_EXPORT_FUNC_S *disp_func_ops;

#ifdef ANDROID_SUPPORT
#include <linux/switch.h>
extern struct switch_dev hdmi_tx_sdev;
#endif

#define HDCP_KEY_CHECK_OK 0xa
typedef enum hiHDMI_VIDEO_TIMING_E
{
    VIDEO_TIMING_UNKNOWN,
    VIDEO_TIMING_640X480P_59940,        /* 1: 640x480p  @ 59.94Hz  No Repetition */
    VIDEO_TIMING_640X480P_60000,        /* 1: 640x480p  @ 60Hz     No Repetition */
#if defined (DVI_SUPPORT)
    VIDEO_TIMING_720X480P_59940,        /* 2: 720x7480p @ 59.94Hz  No Repetition */
#endif
    VIDEO_TIMING_720X480P_60000,        /* 2: 720x480p  @ 60Hz     No Repetition */
#if defined (DVI_SUPPORT)
    VIDEO_TIMING_1280X720P_59940,       /* 4: 1280x720p @ 59.94Hz  No Repetition */
#endif
    VIDEO_TIMING_1280X720P_60000,       /* 4: 1280x720p @ 60Hz     No Repetition */
    VIDEO_TIMING_1920X1080I_60000,      /* 5: 1920x1080i@ 59.94Hz  No Repetition */
#if defined (DVI_SUPPORT)
    VIDEO_TIMING_1920X1080I_59940,      /* 5: 1920x1080i@ 60Hz     No Repetition */
    VIDEO_TIMING_720X480I_59940,        /* 6: 720x480i  @ 59.94Hz  pixel sent 2 times */
#endif
    VIDEO_TIMING_720X480I_60000,        /* 6: 720x480i  @ 60Hz     pixel sent 2 times */
#if defined (DVI_SUPPORT)
    VIDEO_TIMING_720X240P_59940,        /* 8: 720x240p  @ 59.94Hz  pixel sent 2 times */
    VIDEO_TIMING_720X240P_60000,        /* 8: 720x240p  @ 60Hz     pixel sent 2 times */
    VIDEO_TIMING_2880X480I_59940,       /* 10:2880x480i @ 59.94Hz  pixel sent 1 to 10 times */
    VIDEO_TIMING_2880X480I_60000,       /* 10:2880x480i @ 60Hz     pixel sent 1 to 10 times */
    VIDEO_TIMING_2880X240P_59940,       /* 12:2880x240p @ 59.94Hz  pixel sent 1 to 10 timesn */
    VIDEO_TIMING_2880X240P_60000,       /* 12:2880x240p @ 60Hz     pixel sent 1 to 10 times */
    VIDEO_TIMING_1440X480P_59940,       /* 14:1440x480p @ 59.94Hz  pixel sent 1 to 2 times */
    VIDEO_TIMING_1440X480P_60000,       /* 14:1440x480p @ 60Hz     pixel sent 1 to 2 times */
    VIDEO_TIMING_1920X1080P_59940,      /* 16:1920x1080p@ 59.94Hz  No Repetition */
#endif
    VIDEO_TIMING_1920X1080P_60000,      /* 16:1920x1080p@ 60Hz     No Repetition */
    VIDEO_TIMING_720X576P_50000,        /* 17:720x576p  @ 50Hz     No Repetition */
    VIDEO_TIMING_1280X720P_50000,       /* 19:1280x720p @ 50Hz     No Repetition */
    VIDEO_TIMING_1920X1080I_50000,      /* 20:1920x1080i@ 50Hz     No Repetition */
    VIDEO_TIMING_720X576I_50000,        /* 21:720x576i  @ 50Hz     pixel sent 2 times */
#if defined (DVI_SUPPORT)
    VIDEO_TIMING_720X288P_50000,        /* 23:720x288p @ 50Hz      pixel sent 2 times */
    VIDEO_TIMING_2880X576I_50000,       /* 25:2880x576i @ 50Hz     pixel sent 1 to 10 times */
    VIDEO_TIMING_2880X288P_50000,       /* 27:2880x288p @ 50Hz     pixel sent 1 to 10 times */
    VIDEO_TIMING_1440X576P_50000,       /* 29:1440x576p @ 50Hz     pixel sent 1 to 2 times */
#endif
    VIDEO_TIMING_1920X1080P_50000,      /* 31:1920x1080p @ 50Hz    No Repetition */
#if defined (DVI_SUPPORT)
    VIDEO_TIMING_1920X1080P_23980,      /* 32:1920x1080p @ 23.98Hz No Repetition */
#endif
    VIDEO_TIMING_1920X1080P_24000,      /* 32:1920x1080p @ 24Hz   No Repetition */
    VIDEO_TIMING_1920X1080P_25000,      /* 33:1920x1080p @ 25Hz    No Repetition */
#if defined (DVI_SUPPORT)
    VIDEO_TIMING_1920X1080P_29980,      /* 34:1920x1080p @ 29.98Hz No Repetition */
#endif
    VIDEO_TIMING_1920X1080P_30000,      /* 34:1920x1080p @ 30Hz    No Repetition */
#if defined (DVI_SUPPORT)
    VIDEO_TIMING_2880X480P_59940,       /* 35:2880x480p @ 59.94Hz  pixel sent 1, 2 or 4 times */
    VIDEO_TIMING_2880X480P_60000,       /* 35:2880x480p @ 60Hz     pixel sent 1, 2 or 4 times */
    VIDEO_TIMING_2880X576P_50000,       /* 37:2880x576p @ 50Hz     pixel sent 1, 2 or 4 times*/
#endif
    VIDEO_TIMING_MAX
}DRV_HDMI_VIDEO_TIMING_E;

enum
{
    HDMI_CALLBACK_NULL,
    HDMI_CALLBACK_USER,
    HDMI_CALLBACK_KERNEL
};

typedef struct
{
    HI_U32                      VidIdCode;
    DRV_HDMI_VIDEO_TIMING_E     Mode;
    HI_U32                      FrameRate;
    HI_U32                      Active_X;
    HI_U32                      Active_Y;
    HI_U32                      Active_W;
    HI_U32                      Active_H;
    VIDEO_SAMPLE_TYPE_E  ScanType;
    HI_UNF_HDMI_ASPECT_RATIO_E       AspectRatio;
    HI_U32                      PixelRepetition;
}hdmi_VideoIdentification_t;


typedef struct
{
    HI_BOOL     bOpenGreenChannel;        /* Geen Channel */
    //HI_BOOL     bReadEDIDOk;
    HI_U32      enDefaultMode;              /*init parameter*//*CNcomment: ��ʼ������ */
    struct task_struct  *kThreadTimer;    /*timer thread*//*CNcomment:��ʱ���߳� */
    struct task_struct  *kCECRouter;      /*CEC thread*//*CNcomment: CEC�߳� */
    HI_BOOL     bHdmiStarted;             /*HDMI Start Flag*//*CNcomment:HDMI �ӿ��Ƿ�������־ */
    HI_BOOL     bHdmiExit;                /*HDMI exit flag*//*CNcomment:HDMI ģ���˳���־ */
    HI_UNF_HDMI_VIDEO_MODE_E enVidInMode; /*reservation,please setting VIDEO_MODE_YCBCR422 mode*//*CNcomment:������������ΪVIDEO_MODE_YCBCR422 */
}HDMI_COMM_ATTR_S;

static hdmi_VideoIdentification_t  VideoCodes[] = 
{
    /* {Video Identification Code, Timing Mode, x, y, w, h, Frame Rate, Scan Type, Aspect Ratio, Pixel Repetition} */
    {0,  VIDEO_TIMING_UNKNOWN,              0, 0, 0,    0,    0, VIDEO_SAMPLE_TYPE_UNKNOWN,      HI_UNF_HDMI_ASPECT_RATIO_UNKNOWN, 0},
    {1,  VIDEO_TIMING_640X480P_59940,   59940, 0, 0,  640,  480, VIDEO_SAMPLE_TYPE_PROGRESSIVE,  HI_UNF_HDMI_ASPECT_RATIO_4TO3,  0},
    {1,  VIDEO_TIMING_640X480P_60000,   60000, 0, 0,  640,  480, VIDEO_SAMPLE_TYPE_PROGRESSIVE,  HI_UNF_HDMI_ASPECT_RATIO_4TO3,  0},
#if defined (DVI_SUPPORT)
    {2,  VIDEO_TIMING_720X480P_59940,   59940, 0, 0,  720,  480, VIDEO_SAMPLE_TYPE_PROGRESSIVE,  HI_UNF_HDMI_ASPECT_RATIO_4TO3,  0},
#endif
    {2,  VIDEO_TIMING_720X480P_60000,   60000, 0, 0,  720,  480, VIDEO_SAMPLE_TYPE_PROGRESSIVE,  HI_UNF_HDMI_ASPECT_RATIO_4TO3,  0},
#if defined (DVI_SUPPORT)
    {4,  VIDEO_TIMING_1280X720P_59940,  59940, 0, 0, 1280,  720, VIDEO_SAMPLE_TYPE_PROGRESSIVE,  HI_UNF_HDMI_ASPECT_RATIO_16TO9,  0},
#endif
    {4,  VIDEO_TIMING_1280X720P_60000,  60000, 0, 0, 1280,  720, VIDEO_SAMPLE_TYPE_PROGRESSIVE,  HI_UNF_HDMI_ASPECT_RATIO_16TO9, 0},
    {5,  VIDEO_TIMING_1920X1080I_60000, 60000, 0, 0, 1920, 1080, VIDEO_SAMPLE_TYPE_INTERLACE,    HI_UNF_HDMI_ASPECT_RATIO_16TO9, 0},
#if defined (DVI_SUPPORT)
    {5,  VIDEO_TIMING_1920X1080I_59940, 59940, 0, 0, 1920, 1080, VIDEO_SAMPLE_TYPE_INTERLACE,    HI_UNF_HDMI_ASPECT_RATIO_16TO9, 0},
    {6,  VIDEO_TIMING_720X480I_59940,   59940, 0, 0,  720,  480, VIDEO_SAMPLE_TYPE_INTERLACE,    HI_UNF_HDMI_ASPECT_RATIO_4TO3, 1},
#endif
    {6,  VIDEO_TIMING_720X480I_60000,   60000, 0, 0,  720,  480, VIDEO_SAMPLE_TYPE_INTERLACE,    HI_UNF_HDMI_ASPECT_RATIO_4TO3, 1},
#if defined (DVI_SUPPORT)
    {8,  VIDEO_TIMING_720X240P_59940,   59940, 0, 0,  720,  240, VIDEO_SAMPLE_TYPE_PROGRESSIVE,  HI_UNF_HDMI_ASPECT_RATIO_UNKNOWN/*?*/, 1},
    {8,  VIDEO_TIMING_720X240P_60000,   60000, 0, 0,  720,  240, VIDEO_SAMPLE_TYPE_PROGRESSIVE,  HI_UNF_HDMI_ASPECT_RATIO_UNKNOWN/*?*/,  1},
    {10, VIDEO_TIMING_2880X480I_59940,  59940, 0, 0, 2280,  480, VIDEO_SAMPLE_TYPE_INTERLACE,    HI_UNF_HDMI_ASPECT_RATIO_UNKNOWN/*?*/,  1/*1 to 10 times*/},
    {10, VIDEO_TIMING_2880X480I_60000,  60000, 0, 0, 2280,  480, VIDEO_SAMPLE_TYPE_INTERLACE,    HI_UNF_HDMI_ASPECT_RATIO_UNKNOWN/*?*/, 1/*1 to 10 times*/},
    {12, VIDEO_TIMING_2880X240P_59940,  59940, 0, 0, 2280,  240, VIDEO_SAMPLE_TYPE_PROGRESSIVE,  HI_UNF_HDMI_ASPECT_RATIO_UNKNOWN/*?*/, 1/*1 to 10 times*/},
    {12, VIDEO_TIMING_2880X240P_60000,  60000, 0, 0, 2280,  240, VIDEO_SAMPLE_TYPE_PROGRESSIVE,  HI_UNF_HDMI_ASPECT_RATIO_UNKNOWN/*?*/, 1/*1 to 10 times*/},
    {14, VIDEO_TIMING_1440X480P_59940,  59940, 0, 0, 1440,  480, VIDEO_SAMPLE_TYPE_PROGRESSIVE,  HI_UNF_HDMI_ASPECT_RATIO_UNKNOWN/*?*/, 1/*1 to 2 times*/},
    {14, VIDEO_TIMING_1440X480P_60000,  60000, 0, 0, 1440,  480, VIDEO_SAMPLE_TYPE_PROGRESSIVE,  HI_UNF_HDMI_ASPECT_RATIO_UNKNOWN/*?*/,  1/*1 to 2 times*/},
    {16, VIDEO_TIMING_1920X1080P_59940, 59940, 0, 0, 1920, 1080, VIDEO_SAMPLE_TYPE_PROGRESSIVE,  HI_UNF_HDMI_ASPECT_RATIO_16TO9, 0},
#endif
    {16, VIDEO_TIMING_1920X1080P_60000, 60000, 0, 0, 1920, 1080, VIDEO_SAMPLE_TYPE_PROGRESSIVE,  HI_UNF_HDMI_ASPECT_RATIO_16TO9, 0},
    {17, VIDEO_TIMING_720X576P_50000,   50000, 0, 0,  720,  576, VIDEO_SAMPLE_TYPE_PROGRESSIVE,  HI_UNF_HDMI_ASPECT_RATIO_4TO3, 0},
    {19, VIDEO_TIMING_1280X720P_50000,  50000, 0, 0, 1280,  720, VIDEO_SAMPLE_TYPE_PROGRESSIVE,  HI_UNF_HDMI_ASPECT_RATIO_16TO9, 0},
    {20, VIDEO_TIMING_1920X1080I_50000, 50000, 0, 0, 1920, 1080, VIDEO_SAMPLE_TYPE_INTERLACE,    HI_UNF_HDMI_ASPECT_RATIO_16TO9, 0},
    {21, VIDEO_TIMING_720X576I_50000,   50000, 0, 0,  720,  576, VIDEO_SAMPLE_TYPE_INTERLACE,    HI_UNF_HDMI_ASPECT_RATIO_4TO3,  1},
#if defined (DVI_SUPPORT)
    {23, VIDEO_TIMING_720X288P_50000,   50000, 0, 0,  720,  288, VIDEO_SAMPLE_TYPE_PROGRESSIVE,  HI_UNF_HDMI_ASPECT_RATIO_UNKNOWN/*?*/, 1},
    {25, VIDEO_TIMING_2880X576I_50000,  50000, 0, 0, 2880,  576, VIDEO_SAMPLE_TYPE_INTERLACE,    HI_UNF_HDMI_ASPECT_RATIO_UNKNOWN/*?*/, 1/*1 to 10 times*/},
    {27, VIDEO_TIMING_2880X288P_50000,  50000, 0, 0, 2880,  288, VIDEO_SAMPLE_TYPE_PROGRESSIVE,  HI_UNF_HDMI_ASPECT_RATIO_UNKNOWN/*?*/, 1/*1 to 10 times*/},
    {29, VIDEO_TIMING_1440X576P_50000,  50000, 0, 0, 1440,  576, VIDEO_SAMPLE_TYPE_PROGRESSIVE,  HI_UNF_HDMI_ASPECT_RATIO_UNKNOWN/*?*/, 1/*1 to 2 times*/},
#endif
    {31, VIDEO_TIMING_1920X1080P_50000, 50000, 0, 0, 1920, 1080, VIDEO_SAMPLE_TYPE_PROGRESSIVE,  HI_UNF_HDMI_ASPECT_RATIO_16TO9, 0},
#if defined (DVI_SUPPORT)
    {32, VIDEO_TIMING_1920X1080P_23980, 23980, 0, 0, 1920, 1080, VIDEO_SAMPLE_TYPE_PROGRESSIVE,  HI_UNF_HDMI_ASPECT_RATIO_16TO9, 0},
#endif
    {32, VIDEO_TIMING_1920X1080P_24000, 24000, 0, 0, 1920, 1080, VIDEO_SAMPLE_TYPE_PROGRESSIVE,  HI_UNF_HDMI_ASPECT_RATIO_16TO9, 0},
    {33, VIDEO_TIMING_1920X1080P_25000, 25000, 0, 0, 1920, 1080, VIDEO_SAMPLE_TYPE_PROGRESSIVE,  HI_UNF_HDMI_ASPECT_RATIO_16TO9, 0},
#if defined (DVI_SUPPORT)
    {34, VIDEO_TIMING_1920X1080P_29980, 29980, 0, 0, 1920, 1080, VIDEO_SAMPLE_TYPE_PROGRESSIVE,  HI_UNF_HDMI_ASPECT_RATIO_16TO9, 0},
#endif
    {34, VIDEO_TIMING_1920X1080P_30000, 30000, 0, 0, 1920, 1080, VIDEO_SAMPLE_TYPE_PROGRESSIVE,  HI_UNF_HDMI_ASPECT_RATIO_16TO9, 0},
#if defined (DVI_SUPPORT)
    {35, VIDEO_TIMING_2880X480P_59940,  59940, 0, 0, 2280,  480, VIDEO_SAMPLE_TYPE_PROGRESSIVE,  HI_UNF_HDMI_ASPECT_RATIO_UNKNOWN/*?*/, 0/*1, 2 or 4 times*/},
    {35, VIDEO_TIMING_2880X480P_60000,  60000, 0, 0, 2280,  480, VIDEO_SAMPLE_TYPE_PROGRESSIVE,  HI_UNF_HDMI_ASPECT_RATIO_UNKNOWN/*?*/, 0/*1, 2 or 4 times*/},
    {37, VIDEO_TIMING_2880X576P_50000,  50000, 0, 0, 2280,  576, VIDEO_SAMPLE_TYPE_PROGRESSIVE,  HI_UNF_HDMI_ASPECT_RATIO_UNKNOWN/*?*/, 0/*1, 2 or 4 times*/},
#endif
};

static HDMI_COMM_ATTR_S g_stHdmiCommParam;

static HDMI_CHN_ATTR_S  g_stHdmiChnParam[HI_UNF_HDMI_ID_BUTT];


#define HDMI_CHECK_NULL_PTR(ptr)  do{                         \
    if (NULL == (ptr))                                    \
    {                                                     \
        return HI_ERR_HDMI_NUL_PTR;                       \
    }                                                     \
}while(0) 

#define HDMI_CHECK_ID(l_enHdmi)  do{                          \
    if (((l_enHdmi) >= HI_UNF_HDMI_ID_BUTT)                   \
        || ((l_enHdmi) < HI_UNF_HDMI_ID_0))                   \
    {                                                     \
        HI_INFO_HDMI("enHdmi %d is invalid.\n", l_enHdmi); \
        return HI_ERR_HDMI_INVALID_PARA;                  \
    }                                                     \
}while(0)                                                        

#define HDMI_CheckChnOpen(l_HdmiID) do{                       \
    if (HI_TRUE != g_stHdmiChnParam[l_HdmiID].bOpen)      \
    {                                                     \
        HI_INFO_HDMI("enHdmi %d is NOT open.\n", l_HdmiID);\
        return HI_ERR_HDMI_DEV_NOT_OPEN;                  \
    }                                                     \
}while(0)

static HI_U32 hdmi_Mutex_Event_Count = 0;
HI_DECLARE_MUTEX(g_HDMIEventMutex);
#define HDMI_EVENT_LOCK()                                     \
    do{                                                           \
        hdmi_Mutex_Event_Count ++;                                \
        /*HI_INFO_HDMI("hdmi_Mutex_Event_Count:%d\n", hdmi_Mutex_Count);*/  \
        if (down_interruptible(&g_HDMIEventMutex))                \
        ;                                                         \
    }while(0)                                                     \

#define HDMI_EVENT_UNLOCK()                                   \
    do{                                                           \
        hdmi_Mutex_Event_Count --;                                \
        /*HI_INFO_HDMI("hdmi_Mutex_Event_Count:%d\n", hdmi_Mutex_Count); */ \
        up(&g_HDMIEventMutex);                                    \
    }while(0)                                                     \


HI_DECLARE_MUTEX(g_HDMIAttrMutex);
#define HDMI_ATTR_LOCK()                                     \
    do{                                                           \
        if (down_interruptible(&g_HDMIAttrMutex))                \
        ;                                                         \
    }while(0)                                                     \
    
#define HDMI_ATTR_UNLOCK()                                   \
    do{                                                           \
        up(&g_HDMIAttrMutex);                                     \
    }while(0)                                                     \


static HI_U32 hdmi_Create_AVI_Infoframe(HI_UNF_HDMI_AVI_INFOFRAME_VER2_S *punAVIInfoFrame, HI_U8 *pu8AviInfoFrame);
static HI_U32 hdmi_Create_Audio_Infoframe(HI_UNF_HDMI_AUD_INFOFRAME_VER1_S *punAUDInfoFrame, HI_U8 *pu8AudioInfoFrame);
static HI_U32 hdmi_SetAttr(HI_UNF_HDMI_ID_E enHdmi, HDMI_ATTR_S *pstHDMIAttr, HI_BOOL UpdateFlag);
static HI_U32 hdmi_SetInfoFrame(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_INFOFRAME_S *pstInfoFrame);
static HI_U32 hdmi_GetInfoFrame(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_INFOFRAME_TYPE_E enInfoFrameType, HI_UNF_HDMI_INFOFRAME_S *pstInfoFrame);
static HI_U32 hdmi_AdjustAVIInfoFrame(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_AVI_INFOFRAME_VER2_S *punAVIInfoFrame);
static HI_U32 hdmi_AdjustAUDInfoFrame(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_AUD_INFOFRAME_VER1_S *punAUDInfoFrame);
static HI_VOID hdmi_SetAndroidState(HI_S32 PlugState);


//static void hdmi_Disp2EncFmt(HI_UNF_ENC_FMT_E *dstFmt,HI_DRV_DISP_FMT_E *srcFmt);
static void hdmi_ProcEvent(HI_UNF_HDMI_EVENT_TYPE_E event,HI_U32 procID);

static HI_U32 g_HDMIWaitFlag[MAX_PROCESS_NUM];
static wait_queue_head_t g_astHDMIWait;

//One Proccess can only init/open one HDMI Instance!
static HI_U32 g_HDMIKernelInitNum = 0; //Kernel HDMI Init Num
static HI_U32 g_HDMIUserInitNum   = 0; //User HDMI Init Num
static HI_U32 g_HDMIOpenNum       = 0; //HDMI Open Num
static HI_U32 g_UserCallbackFlag  = HDMI_CALLBACK_NULL;
//static HI_U32 g_UCallbackAddr     = 0; //save callback pointer Address
static HI_U32 DRV_HDMI_CheckVOFormat(HI_DRV_DISP_FMT_E setFmt);

#if defined (HDCP_SUPPORT)  
extern void reset_hdcp_counter(void);
extern void increase_hdcp_counter(void);
extern unsigned int  get_hdcp_counter(void);
#endif

#if defined(BOARD_TYPE_S40V2_fpga)
#include "hdmi_fpga.h"
#endif

HDMI_CHN_ATTR_S  *DRV_Get_Glb_Param(void)
{
    return &g_stHdmiChnParam[0];    
}

HI_U32 unStableTimes = 0;
HI_S32 Hdmi_KThread_Timer(void* pParm)
{
    HI_U32 Ret;

    pParm = pParm;//ignore compire warnning

    while ( 1 )
    {
        if (kthread_should_stop())
            break;
        if(!siiIsTClockStable())
        {
            unStableTimes++;
            //printk("Warning:unstable Times 0x%d \n",unStableTimes);
        }
        
        if (HI_FALSE == g_stHdmiChnParam[0].bOpen)
        {
            msleep(50);
            continue;
        }
    
#ifndef __HDMI_INTERRUPT__
        Ret = SI_TimerHandler();
#endif
    #ifdef BOARD_TYPE_S40V2_fpga
        msleep(200);
    #endif
        msleep(90); 
    }

    return HI_SUCCESS;
}

#if defined (CEC_SUPPORT)
HI_S32 Hdmi_KThread_CEC(void* pParm)
{
    HI_U32 Ret = 0;
    pParm = pParm;

    while ( 1 )
    {
        if (kthread_should_stop())
            break;

        /* HDMI do not start, Just sleep */
        if (HI_FALSE == g_stHdmiChnParam[0].bStart)
        {
            msleep(100);
            continue;
        }

        if (HI_FALSE == g_stHdmiChnParam[0].bCECStart)
        {
            HI_U32 u32Status, index;
            HI_UNF_HDMI_SINK_CAPABILITY_S sinkCap;
            HI_UNF_HDMI_CEC_STATUS_S     *pstCECStatus;

            /* Only Check CEC 5 time */
            if (g_stHdmiChnParam[0].u8CECCheckCount < 3)
            {
                g_stHdmiChnParam[0].u8CECCheckCount ++;
            } 
            else 
            {
                /* NO CEC Response, Just sleep */
                msleep(100);
                continue;
            }

            /*We need to do AutoPing */
            SI_CEC_AudioPing(&u32Status);

            if(0x01 == (u32Status & 0x01))
            {
                //Build up CEC Status!
                Ret = (HI_U32)SI_GetHdmiSinkCaps(&sinkCap);
                if(sinkCap.bIsRealEDID != HI_TRUE)
                    continue;

                //Physical Address
                pstCECStatus = &(g_stHdmiChnParam[0].stCECStatus);
                if(sinkCap.u8PhyAddr_A == 0x00)
                {
                    continue; //Bad CEC Phaycail Address
                }

                pstCECStatus->u8PhysicalAddr[0] = sinkCap.u8PhyAddr_A;
                pstCECStatus->u8PhysicalAddr[1] = sinkCap.u8PhyAddr_B;
                pstCECStatus->u8PhysicalAddr[2] = sinkCap.u8PhyAddr_C;
                pstCECStatus->u8PhysicalAddr[3] = sinkCap.u8PhyAddr_D;

                //CEC Network
                for(index = 0; index < HI_UNF_CEC_LOGICALADD_BUTT; index ++)
                {
                    if((u32Status & (1 << index)) != 0) 
                        pstCECStatus->u8Network[index] = HI_TRUE;
                }

                //Logical Address
                if(pstCECStatus->u8Network[HI_UNF_CEC_LOGICALADD_TUNER_1] == HI_TRUE)            //bit3
                {
                    if(pstCECStatus->u8Network[HI_UNF_CEC_LOGICALADD_TUNER_2] == HI_TRUE)        //bit6
                    {
                        if(pstCECStatus->u8Network[HI_UNF_CEC_LOGICALADD_TUNER_3] == HI_TRUE)    //bit7
                        {
                            if(pstCECStatus->u8Network[HI_UNF_CEC_LOGICALADD_TUNER_4] == HI_TRUE)//bit10
                                pstCECStatus->u8LogicalAddr = 0x0f;//Brocast Address!
                            else
                                pstCECStatus->u8LogicalAddr = HI_UNF_CEC_LOGICALADD_TUNER_4;
                        }
                        else
                        {
                            pstCECStatus->u8LogicalAddr = HI_UNF_CEC_LOGICALADD_TUNER_3;
                        }
                    }
                    else
                    {
                        pstCECStatus->u8LogicalAddr = HI_UNF_CEC_LOGICALADD_TUNER_2;
                    }
                }
                else
                {
                    pstCECStatus->u8LogicalAddr = HI_UNF_CEC_LOGICALADD_TUNER_1;
                }

                pstCECStatus->bEnable =  HI_TRUE;
                HI_INFO_HDMI("CEC is build up *****\n");

                SI_CEC_Open();
                //Should send out Brocast messsage of <Report Physical Address> !
                {
                    HI_UNF_HDMI_CEC_CMD_S CECCmd = {0};

                    CECCmd.u8Opcode = CEC_OPCODE_REPORT_PHYSICAL_ADDRESS;
                    CECCmd.enSrcAdd = pstCECStatus->u8LogicalAddr;
                    CECCmd.enDstAdd = HI_UNF_CEC_LOGICALADD_BROADCAST;
                    CECCmd.unOperand.stRawData.u8Length  = 2;
                    CECCmd.unOperand.stRawData.u8Data[0] = ((pstCECStatus->u8PhysicalAddr[0] << 4) & 0xf0) | (pstCECStatus->u8PhysicalAddr[1] & 0x0f); // [Physical Address(A.B.C.D):A B]
                    //ԭ����u8Data[0] ,�һ����ǲ���u8Data[1],��Ҫ������ԭ��
                    CECCmd.unOperand.stRawData.u8Data[0] = ((pstCECStatus->u8PhysicalAddr[2] << 4) & 0xf0) | (pstCECStatus->u8PhysicalAddr[3] & 0x0f) ; // [Physical Address(A.B.C.D):C D]
                    SI_CEC_SendCommand(&CECCmd);
                }
                //Should send out Brocast message of <Vendor Device ID>!
                g_stHdmiChnParam[0].bCECStart = HI_TRUE;
                HI_INFO_HDMI("\n-------CEC Started-------\n");
            }

            msleep(100);
            continue;
        }
        else
        {
            //DEBUG_PRINTK("SI_CEC_Event_Handler\n");
            SI_CEC_Event_Handler();
            msleep(50);
        }

        msleep(100);
    }
    return HI_SUCCESS;
}

#endif

static hdmi_VideoIdentification_t * hdmi_GetVideoCode(HI_DRV_DISP_FMT_E enTimingMode)
{
    HI_U32 Index, VideoTimingMode;

    switch (enTimingMode)
    {
        case HI_DRV_DISP_FMT_1080P_60:
            VideoTimingMode = VIDEO_TIMING_1920X1080P_60000;
            break;
        case HI_DRV_DISP_FMT_1080P_50:
            VideoTimingMode = VIDEO_TIMING_1920X1080P_50000;
            break;
        case HI_DRV_DISP_FMT_1080P_30:
            VideoTimingMode = VIDEO_TIMING_1920X1080P_30000;
            break;
        case HI_DRV_DISP_FMT_1080P_25:
            VideoTimingMode = VIDEO_TIMING_1920X1080P_25000;
            break;
        case HI_DRV_DISP_FMT_1080P_24:
            VideoTimingMode = VIDEO_TIMING_1920X1080P_24000;
            break;
        case HI_DRV_DISP_FMT_1080i_60:
            VideoTimingMode = VIDEO_TIMING_1920X1080I_60000;
            break;
        case HI_DRV_DISP_FMT_1080i_50:
            VideoTimingMode = VIDEO_TIMING_1920X1080I_50000;
            break;
        case HI_DRV_DISP_FMT_720P_60:
            VideoTimingMode = VIDEO_TIMING_1280X720P_60000;
            break;
        case HI_DRV_DISP_FMT_720P_50:
            VideoTimingMode = VIDEO_TIMING_1280X720P_50000;
            break;
        case HI_DRV_DISP_FMT_576P_50:
            VideoTimingMode = VIDEO_TIMING_720X576P_50000;
            break;
        case HI_DRV_DISP_FMT_480P_60:
            VideoTimingMode = VIDEO_TIMING_720X480P_60000;
            break;
        case HI_DRV_DISP_FMT_PAL:
        case HI_DRV_DISP_FMT_PAL_B:
        case HI_DRV_DISP_FMT_PAL_B1:
        case HI_DRV_DISP_FMT_PAL_D:
        case HI_DRV_DISP_FMT_PAL_D1:
        case HI_DRV_DISP_FMT_PAL_G:
        case HI_DRV_DISP_FMT_PAL_H:
        case HI_DRV_DISP_FMT_PAL_K:
        case HI_DRV_DISP_FMT_PAL_I:
        case HI_DRV_DISP_FMT_PAL_M:
        case HI_DRV_DISP_FMT_PAL_N:
        case HI_DRV_DISP_FMT_PAL_Nc:
        case HI_DRV_DISP_FMT_PAL_60:

        case HI_DRV_DISP_FMT_SECAM_SIN:
        case HI_DRV_DISP_FMT_SECAM_COS:
        case HI_DRV_DISP_FMT_SECAM_L:
        case HI_DRV_DISP_FMT_SECAM_B:
        case HI_DRV_DISP_FMT_SECAM_G:
        case HI_DRV_DISP_FMT_SECAM_D:
        case HI_DRV_DISP_FMT_SECAM_K:
        case HI_DRV_DISP_FMT_SECAM_H:
            VideoTimingMode = VIDEO_TIMING_720X576I_50000;
            break;
        case HI_DRV_DISP_FMT_NTSC:
        case HI_DRV_DISP_FMT_NTSC_J:
        case HI_DRV_DISP_FMT_NTSC_443:
            VideoTimingMode = VIDEO_TIMING_720X480I_60000;
            break;
        case HI_DRV_DISP_FMT_861D_640X480_60:
            VideoTimingMode = VIDEO_TIMING_640X480P_60000;
            break;
        default:
            HI_ERR_HDMI("error video timing:%d\n", enTimingMode);
            VideoTimingMode = VIDEO_TIMING_640X480P_59940;
            break;
    }

    for (Index=0; Index < VIDEO_TIMING_MAX; Index ++)
    {
        if (VideoCodes[Index].Mode == VideoTimingMode)
            break; 
    }

    if (Index >= VIDEO_TIMING_MAX)
    {
        Index = 0; 
    }

    HI_INFO_HDMI("Get Video Code index:%d, Mode:0x%x, VidIdCode:0x%x\n", Index, VideoCodes[Index].Mode, VideoCodes[Index].VidIdCode);
    return(&(VideoCodes[Index]));
}

#if 0 /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
HI_UNF_ENC_FMT_E hdmi_Disp2EncFmt(HI_DRV_DISP_FMT_E SrcFmt)
{
    HI_UNF_ENC_FMT_E dstFmt = HI_UNF_ENC_FMT_BUTT;
    switch(SrcFmt)
    {
        case HI_DRV_DISP_FMT_1080P_60 :
            dstFmt = HI_UNF_ENC_FMT_1080P_60;
            break;
        case HI_DRV_DISP_FMT_1080P_50 :
            dstFmt = HI_UNF_ENC_FMT_1080P_50;
            break;
        case HI_DRV_DISP_FMT_1080P_30 :
            dstFmt = HI_UNF_ENC_FMT_1080P_30;
            break;
        case HI_DRV_DISP_FMT_1080P_25 :
            dstFmt = HI_UNF_ENC_FMT_1080P_25;
            break;
        case HI_DRV_DISP_FMT_1080P_24 :
        case HI_DRV_DISP_FMT_1080P_24_FP:
            dstFmt = HI_UNF_ENC_FMT_1080P_24;
            break;
        case HI_DRV_DISP_FMT_1080i_60 :
            dstFmt = HI_UNF_ENC_FMT_1080i_60;
            break;
        case HI_DRV_DISP_FMT_1080i_50 :
            dstFmt = HI_UNF_ENC_FMT_1080i_50;
            break;
        case HI_DRV_DISP_FMT_720P_60 :
        case HI_DRV_DISP_FMT_720P_50_FP:
            dstFmt = HI_UNF_ENC_FMT_720P_60;
            break;
        case HI_DRV_DISP_FMT_720P_50 :
        case HI_DRV_DISP_FMT_720P_50_FP:
            dstFmt = HI_UNF_ENC_FMT_720P_50;
            break;
        case HI_DRV_DISP_FMT_576P_50 :
            dstFmt = HI_UNF_ENC_FMT_576P_50;
            break;
        case HI_DRV_DISP_FMT_480P_60 :
            dstFmt = HI_UNF_ENC_FMT_480P_60;
            break;
        case HI_DRV_DISP_FMT_PAL:
        case HI_DRV_DISP_FMT_PAL_B:
        case HI_DRV_DISP_FMT_PAL_B1:
        case HI_DRV_DISP_FMT_PAL_D:
        case HI_DRV_DISP_FMT_PAL_D1:
        case HI_DRV_DISP_FMT_PAL_G:
        case HI_DRV_DISP_FMT_PAL_H:
        case HI_DRV_DISP_FMT_PAL_K:
        case HI_DRV_DISP_FMT_PAL_I:
        case HI_DRV_DISP_FMT_PAL_M:
        case HI_DRV_DISP_FMT_PAL_N:
        case HI_DRV_DISP_FMT_PAL_Nc:
        case HI_DRV_DISP_FMT_PAL_60:
        case HI_DRV_DISP_FMT_1440x576i_50:

        case HI_DRV_DISP_FMT_SECAM_SIN:
        case HI_DRV_DISP_FMT_SECAM_COS:
        case HI_DRV_DISP_FMT_SECAM_L:
        case HI_DRV_DISP_FMT_SECAM_B:
        case HI_DRV_DISP_FMT_SECAM_G:
        case HI_DRV_DISP_FMT_SECAM_D:
        case HI_DRV_DISP_FMT_SECAM_K:
        case HI_DRV_DISP_FMT_SECAM_H:
            dstFmt = HI_UNF_ENC_FMT_PAL;
            break;
        case HI_DRV_DISP_FMT_NTSC :        
        case HI_DRV_DISP_FMT_NTSC_J :
        case HI_DRV_DISP_FMT_NTSC_443 :
        case HI_DRV_DISP_FMT_1440x480i_60:
            dstFmt = HI_UNF_ENC_FMT_NTSC;
            break;
        case HI_DRV_DISP_FMT_861D_640X480_60 :
            dstFmt = HI_UNF_ENC_FMT_861D_640X480_60;
            break;
        case HI_DRV_DISP_FMT_VESA_800X600_60 :
            dstFmt = HI_UNF_ENC_FMT_VESA_800X600_60;
            break;
        case HI_DRV_DISP_FMT_VESA_1024X768_60 :
            dstFmt = HI_UNF_ENC_FMT_VESA_1024X768_60;
            break;
        case HI_DRV_DISP_FMT_VESA_1280X720_60 :
            dstFmt = HI_UNF_ENC_FMT_VESA_1280X720_60;
            break;
        case HI_DRV_DISP_FMT_VESA_1280X800_60 :
            dstFmt = HI_UNF_ENC_FMT_VESA_1280X800_60;
            break;
        case HI_DRV_DISP_FMT_VESA_1280X1024_60 :
            dstFmt = HI_UNF_ENC_FMT_VESA_1280X1024_60;
            break;
        case HI_DRV_DISP_FMT_VESA_1360X768_60 :
            dstFmt = HI_UNF_ENC_FMT_VESA_1360X768_60;
            break;
        case HI_DRV_DISP_FMT_VESA_1366X768_60 :
            dstFmt = HI_UNF_ENC_FMT_VESA_1366X768_60;
            break;
        case HI_DRV_DISP_FMT_VESA_1400X1050_60:
            dstFmt = HI_UNF_ENC_FMT_VESA_1400X1050_60;
            break;
        case HI_DRV_DISP_FMT_VESA_1440X900_60:
            dstFmt = HI_UNF_ENC_FMT_VESA_1440X900_60;
            break;
        case HI_DRV_DISP_FMT_VESA_1440X900_60_RB:
            dstFmt = HI_UNF_ENC_FMT_VESA_1440X900_60_RB;
            break;
        case HI_DRV_DISP_FMT_VESA_1600X900_60_RB:
            dstFmt = HI_UNF_ENC_FMT_VESA_1600X900_60_RB;
            break;
        case HI_DRV_DISP_FMT_VESA_1600X1200_60:
            dstFmt = HI_UNF_ENC_FMT_VESA_1600X1200_60;
            break;
        case HI_DRV_DISP_FMT_VESA_1680X1050_60:
            dstFmt = HI_UNF_ENC_FMT_VESA_1680X1050_60;
            break;
        case HI_DRV_DISP_FMT_VESA_1920X1080_60:
            dstFmt = HI_UNF_ENC_FMT_VESA_1920X1080_60;
            break;
        case HI_DRV_DISP_FMT_VESA_1920X1200_60:
            dstFmt = HI_UNF_ENC_FMT_VESA_1920X1200_60;
            break;
        case HI_DRV_DISP_FMT_VESA_2048X1152_60:
            dstFmt = HI_UNF_ENC_FMT_VESA_2048X1152_60;
            break;
        default:
            dstFmt = HI_UNF_ENC_FMT_BUTT;
            break;
    }
    return dstFmt;
}

#endif /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/

static HI_VOID hdmi_SetAndroidState(HI_S32 PlugState)
{
#ifdef ANDROID_SUPPORT
    switch_set_state(&hdmi_tx_sdev, PlugState);
#endif
}

HI_S32 DRV_HDMI_WriteRegister(HI_U32 u32RegAddr, HI_U32 u32Value)
{
    HI_U32 VirAddr;
    volatile HI_U32     *pu32VirAddr = HI_NULL;

    VirAddr = (HI_U32)IO_ADDRESS(u32RegAddr);
    HI_INFO_HDMI("WriteRegister u32RegAddr:0x%x, VirAddr:0x%x\n", u32RegAddr, VirAddr);

    pu32VirAddr = (volatile HI_U32 *)(VirAddr);
    pu32VirAddr[0] = (HI_U32)u32Value;

    return HI_SUCCESS;
}

HI_S32 DRV_HDMI_ReadRegister(HI_U32 u32RegAddr, HI_U32 *pu32Value)
{
    HI_U32 VirAddr;
    volatile HI_U32     *pu32VirAddr = HI_NULL;

    VirAddr = (HI_U32)IO_ADDRESS(u32RegAddr);
    pu32VirAddr = (volatile HI_U32 *)(VirAddr);
    *pu32Value = *pu32VirAddr;

    return HI_SUCCESS;
}

/*
 ** HDMI Hardware Reset:Set PERI_CRG2
 ** ��λ �� 1 �� 0   ��Ҫ��ȡ?
 */
HI_S32 SI_HdmiHardwareReset(int iEnable)
{
    //HI_U32    u32BaseAddr;
    //u32BaseAddr = SYS_PHY_BASE_ADDR;//f8a22000
    
    //HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x10c, 0x33f);
    //HDMI_WriteReg(IO_ADDRESS(u32BaseAddr), 0x10c, 0x3f);
    //volatile HI_U32 *pulArgs = (HI_U32*)IO_ADDRESS(HDMI_HARDWARE_RESET_ADDR);
    HI_U32    u32Ctrller;
    HI_U32    u32Phy;
    HI_U32 tmp = 0;
    HI_U32 tmp2 = 0;
    
    DRV_HDMI_ReadRegister(HDMI_HARDWARE_RESET_ADDR,&u32Ctrller);
    DRV_HDMI_ReadRegister(0xf8a22110,&u32Phy);
    if (iEnable == 0)
    {
        tmp = u32Ctrller;
        //tmp &= 0xfffffffe;
        tmp &= ~0x300;
        u32Ctrller = tmp;

        tmp2 = u32Phy;
        tmp2 &= ~0x10;
        u32Phy = tmp2;
    }

    //��λ
    else
    {
        tmp = u32Ctrller;
        tmp |= 0x300;
        u32Ctrller = tmp;

        tmp2 = u32Phy;
        tmp2 |= 0x10;
        u32Phy = tmp2;
    }

    DRV_HDMI_WriteRegister(HDMI_HARDWARE_RESET_ADDR,u32Ctrller);
    DRV_HDMI_WriteRegister(0xf8a22110,u32Phy);
    return HI_SUCCESS;    
}

/*
hotplug:0x10203030 0x2 or 0x10203060 0x3
CEC:0x1020303c 0x2 or 0x1020305c 0x3
scl&sda:0x10203044 0x1
*/
HI_VOID HDMI_PinConfig(HI_VOID)
{
#if 0
    HI_CHIP_TYPE_E enChip;
    HI_CHIP_VERSION_E enChipVersion;

    HI_DRV_SYS_GetChipVersion(&enChip, &enChipVersion);
    
    if(enChipVersion == HI_CHIP_VERSION_V300)
    {
        DRV_HDMI_WriteRegister(0x10200094,0x0);
        DRV_HDMI_WriteRegister(0x10200098,0x0);
    }
    
    if(HI_CHIP_TYPE_HI3712 == enChip)
    {
        DRV_HDMI_WriteRegister(0x10203014, 0x1);
        DRV_HDMI_WriteRegister(0x10203018, 0x1);
        DRV_HDMI_WriteRegister(0x10203020, 0x1);
    }
#endif 

    HI_U32    u32BaseAddr = 0xf8a21000;

    //himm 0xF8A21000 0x01 HDMI_TX_SCL
    //himm 0xF8A21004 0x01 HDMI_TX_SDA  //DDC 
    DRV_HDMI_WriteRegister(u32BaseAddr + 0x00,0x01);
    DRV_HDMI_WriteRegister(u32BaseAddr + 0x04,0x01);
    //himm 0xF8A21008 0x01 hpd
    //himm 0xF8A2100C 0x01 cec
    DRV_HDMI_WriteRegister(u32BaseAddr + 0x08,0x01);
    DRV_HDMI_WriteRegister(u32BaseAddr + 0x0c,0x01);

#if defined(BOARD_TYPE_S40V2_fpga)    
    //fpga����i2c���ʵ�hdmi tx phy���ܽ�Ĭ��ΪGPIO����HDMI I2C
    //0xf8a210f4 0x01 i2c�ܽ� 
    //0xf8a210f8 0x01 i2c�ܽ�
    DRV_HDMI_WriteRegister(u32BaseAddr + 0xf4, 0x01);
    DRV_HDMI_WriteRegister(u32BaseAddr + 0xf8, 0x01);
#endif

    //crg
    u32BaseAddr = 0xf8a22000;
    DRV_HDMI_WriteRegister( u32BaseAddr + 0x10c,0x3f);
    DRV_HDMI_WriteRegister( u32BaseAddr + 0x110,0x01);
    //HI_HDMI_PRINT("\n PINHdmiInit \n");

    return ;
}

//#define HDMI_PHY_BASE_ADDR 0x10170000L


static HI_U32 hdmi_Get_FmtVIC(HI_DRV_DISP_FMT_E enEncFmt)
{
    HI_U32 u32VIC = 0;
    switch(enEncFmt)
    {
        case HI_DRV_DISP_FMT_1080P_60:
            u32VIC = 0x10;
            break;
        case HI_DRV_DISP_FMT_1080P_50:
            u32VIC = 0x1f;
            break;
        case HI_DRV_DISP_FMT_1080P_30:
            u32VIC = 0x22;
            break;
        case HI_DRV_DISP_FMT_1080P_25:
            u32VIC = 0x21;
            break;
        case HI_DRV_DISP_FMT_1080P_24:
            u32VIC = 0x20;
            break;
        case HI_DRV_DISP_FMT_1080i_60:
            u32VIC = 0x05;
            break;
        case HI_DRV_DISP_FMT_1080i_50:
            u32VIC = 0x14;
            break;
        case HI_DRV_DISP_FMT_720P_60:
            u32VIC = 0x04;
            break;
        case HI_DRV_DISP_FMT_720P_50:
            u32VIC = 0x13;
            break;
        case HI_DRV_DISP_FMT_576P_50:
            u32VIC = 0x11;
            break;
        case HI_DRV_DISP_FMT_480P_60:
            u32VIC = 0x02;
            break;
        case HI_DRV_DISP_FMT_PAL:
        case HI_DRV_DISP_FMT_PAL_B:
        case HI_DRV_DISP_FMT_PAL_B1:
        case HI_DRV_DISP_FMT_PAL_D:
        case HI_DRV_DISP_FMT_PAL_D1:
        case HI_DRV_DISP_FMT_PAL_G:
        case HI_DRV_DISP_FMT_PAL_H:
        case HI_DRV_DISP_FMT_PAL_K:
        case HI_DRV_DISP_FMT_PAL_I:
        case HI_DRV_DISP_FMT_PAL_M:
        case HI_DRV_DISP_FMT_PAL_N:
        case HI_DRV_DISP_FMT_PAL_Nc:
        case HI_DRV_DISP_FMT_PAL_60:
        
        case HI_DRV_DISP_FMT_SECAM_SIN:
        case HI_DRV_DISP_FMT_SECAM_COS:
        case HI_DRV_DISP_FMT_SECAM_L:
        case HI_DRV_DISP_FMT_SECAM_B:
        case HI_DRV_DISP_FMT_SECAM_G:
        case HI_DRV_DISP_FMT_SECAM_D:
        case HI_DRV_DISP_FMT_SECAM_K:
        case HI_DRV_DISP_FMT_SECAM_H:

             u32VIC = 0x15;
            break;
        case HI_DRV_DISP_FMT_NTSC:
        case HI_DRV_DISP_FMT_NTSC_J:
        case HI_DRV_DISP_FMT_NTSC_443:
            u32VIC = 0x06;
            break;
        default:
            u32VIC = 0x00;
            break;
    }
    HI_INFO_HDMI("hdmi vic:0x%x\n",u32VIC);
    return u32VIC;
}

static HI_U32 g_Event_Count[MAX_PROCESS_NUM];
#define MIX_EVENT_COUNT  1

HI_U32 DRV_HDMI_Init(HI_U32 FromUserSpace)
{
    HI_U8 ucChipInf[3] = {0};
    HI_U8 BlankValue[3];
    HI_U32  u32Vic = 0;
    HI_BOOL bOpenAlready = HI_FALSE;          //Judge whether HDMI is setup already
//    DISP_HDMI_SETTING_S stDispCfg;
    //HI_BOOL bHDMIDriverInitFlag = HI_FALSE;   //Judge HDMI init form MCE or fastboot: TRUE-->MCE int
    HI_U32 u32Ret;
    
    /* Need to reset this  param */
    g_stHdmiCommParam.bOpenGreenChannel = HI_FALSE;
    //g_stHdmiCommParam.bReadEDIDOk = HI_FALSE;

    HI_INFO_HDMI("****Enter DRV_HDMI_Init*****\n");
    HDMI_PinConfig();

    #if 1
    /*if func ops and func ptr is null, then return ;*/
    if(!disp_func_ops || !disp_func_ops->DRV_DISP_GetFormat)
    {
        HI_INFO_HDMI("disp hdmi init err\n");
        return HI_FAILURE;
    }
    #endif
    BlankValue[0] = 128;
    BlankValue[1] = 16;
    BlankValue[2] = 128;
    SI_WriteBlockHDMITXP0(TX_VID_BLANK1_BLUE, 3, BlankValue);

    /*if FromUserSpace == HI_TRUE,Setup from User,or Setup from Kerne */
    if (HI_TRUE != FromUserSpace) 
    {
        if (g_HDMIKernelInitNum)
        {
            g_HDMIKernelInitNum ++;
            HI_WARN_HDMI("From Kernel:HDMI has been inited!\n");
            return HI_ERR_HDMI_INIT_ALREADY;
        }
    }
    else
    {
        if (g_HDMIUserInitNum)
        {
            g_HDMIUserInitNum ++;
            HI_WARN_HDMI("FromUser:HDMI has been inited!\n");
            return HI_ERR_HDMI_CALLBACK_ALREADY;
        }
    }
    
    g_UserCallbackFlag = HDMI_CALLBACK_NULL;
 
    /*reset hdcp count*/
#if defined (HDCP_SUPPORT)
    reset_hdcp_counter();
#endif
      /* Judge whether it has opened already */
    bOpenAlready = SI_HDMI_Setup_INBoot(&u32Vic);
    HI_INFO_HDMI("bOpenAlready:%d\n", bOpenAlready);

    if (bOpenAlready == HI_TRUE)
    {
        HI_DRV_DISP_FMT_E enEncFmt;
        
        if(disp_func_ops && disp_func_ops->DRV_DISP_GetFormat)
        {
            disp_func_ops->DRV_DISP_GetFormat(HI_UNF_DISPLAY1, &enEncFmt);
        }
        
        if(hdmi_Get_FmtVIC(enEncFmt) == u32Vic)
        {
            bOpenAlready = HI_TRUE;
        }
#if defined (DVI_SUPPORT)
         else 
        {
            HI_U32 bHdmiMode;
            /*if fastboot is TV format,setting VGA format *//*CNcomment:fastboot ��tv�� ��Ҫ����Ϊvga*/
            bHdmiMode = SI_TX_IsHDMImode();//DRV_HDMI_ReadRegister((HI_U32)0x101704BC, &u32Value);
            if (bHdmiMode && (HI_DRV_DISP_FMT_861D_640X480_60 < enEncFmt))
            {
                bOpenAlready = HI_FALSE;
            }
            else if (!bHdmiMode /*if fastboot is VGA format, setting TV format*//*CNcomment:fastboot ��vga�� ��Ҫ����Ϊtv*/
                    &&(HI_DRV_DISP_FMT_861D_640X480_60 >= enEncFmt))            
            {
                bOpenAlready = HI_FALSE;
            }
            else if (!bHdmiMode /*if fastboot is VGA format, setting VGA format *//*CNcomment:fastboot ��vga�� ��Ҫ����Ϊvga*/
                    &&(HI_DRV_DISP_FMT_861D_640X480_60 < enEncFmt))
            {
                bOpenAlready = HI_TRUE;
            }
            else 
            {
                bOpenAlready = HI_FALSE;
            }
         }
#endif
        
    }
    
    if(bOpenAlready == HI_TRUE)
    {
        
        if ((g_HDMIUserInitNum > 0) || (g_HDMIKernelInitNum > 0))
        {
            if (HI_TRUE == FromUserSpace)
            {
                g_HDMIUserInitNum ++;
            }
            else
            {
                g_HDMIKernelInitNum ++;
            }
            HI_INFO_HDMI("Open Already in MCE!\n");
            /* open already flag */
            g_stHdmiCommParam.bOpenGreenChannel = HI_TRUE;
            HI_INFO_HDMI("****green channel change %d *****\n",g_stHdmiCommParam.bOpenGreenChannel);
            return HI_SUCCESS;   //Open Already in MCE!
        }
        else
        {
            //Open Already in FastBoot!
            HI_INFO_HDMI("HDMI is setup in fastboot!!\n");
        }
        
    }
    else /* Normal Setup */
    {
        HI_INFO_HDMI("come to SI_HW_ResetHDMITX\n");
        //printk("come to SI_HW_ResetHDMITX\n");
        SI_HW_ResetHDMITX();
        SI_SW_ResetHDMITX();
        if(HI_TRUE != SI_HPD_Status())
        {
            SI_PowerDownHdmiTx();
        }
    }
    
    if (HI_TRUE == FromUserSpace)
    {
        g_HDMIUserInitNum ++;
    }
    else
    {
        g_HDMIKernelInitNum ++;
    }

    
    g_stHdmiCommParam.bHdmiStarted   = HI_FALSE;
    g_stHdmiCommParam.bHdmiExit      = HI_FALSE;
            
    u32Ret = SI_OpenHdmiDevice();
#if defined (CEC_SUPPORT)
    /* Enable CEC_SETUP */
#if 0
    HI_INFO_HDMI("set CEC_SETUP\n");
    WriteByteHDMICEC(0X8E, 0x04);
#endif 
    SI_CEC_SetUp();
#endif
    SI_ReadChipInfo(ucChipInf);
    HI_INFO_HDMI("ReadChipInfo HDMI: Id 0x%02x.0x%02x. Rev %02x\n",
        (int)ucChipInf[0],(int)ucChipInf[1],(int)ucChipInf[2]);

    memset((void *)g_stHdmiChnParam, 0, sizeof(HDMI_CHN_ATTR_S) * HI_UNF_HDMI_ID_BUTT);

    //g_HDMIWaitFlag = HI_FALSE;
    memset(g_HDMIWaitFlag,0,sizeof(HI_U32) * MAX_PROCESS_NUM);
    memset(g_Event_Count,0,sizeof(HI_U32) * MAX_PROCESS_NUM);
    
    init_waitqueue_head(&g_astHDMIWait);
    /* create hdmi task */
    g_stHdmiCommParam.bHdmiExit = HI_FALSE;
    if (g_stHdmiCommParam.kThreadTimer == NULL)
    {
        HI_INFO_HDMI("create Timer task kThreadTimer:0x%x\n", g_stHdmiCommParam.kThreadTimer);
        g_stHdmiCommParam.kThreadTimer = kthread_create(Hdmi_KThread_Timer, NULL, "HDMI_kthread");
        if (g_stHdmiCommParam.kThreadTimer == NULL)
        {
            HI_ERR_HDMI("start hdmi kernel thread failed.\n");
        }
        else
        {
            wake_up_process(g_stHdmiCommParam.kThreadTimer);
        }
    }

#if defined (CEC_SUPPORT)
    if (g_stHdmiCommParam.kCECRouter == NULL)
    {    
        /* create CEC task */
        g_stHdmiCommParam.kCECRouter = kthread_create(Hdmi_KThread_CEC, NULL, "HDMI_kCEC");
        if (g_stHdmiCommParam.kCECRouter == 0)
        {
            HI_ERR_HDMI("Unable to start hdmi kernel thread.\n");
        }
        else
        {
            wake_up_process(g_stHdmiCommParam.kCECRouter);
        }
    } 
#endif
    
    /* Normal init HDMI */
    HI_INFO_HDMI("WriteDefaultConfigToEEPROM\n");
    SI_WriteDefaultConfigToEEPROM();

    HI_INFO_HDMI("Leave DRV_HDMI_Init\n");
#ifdef __HDMI_INTERRUPT__
    HDMI_IRQ_Setup();
#endif
    hdmi_SetAndroidState(STATE_PLUG_UNKNOWN);
    return HI_SUCCESS;
}

HI_U32 DRV_HDMI_DeInit(HI_U32 FromUserSpace)
{
    int ret = 0;
    HI_S32 i;

#ifdef __HDMI_INTERRUPT__
    HDMI_IRQ_Exit();
#endif

    HI_INFO_HDMI("Enter DRV_HDMI_DeInit g_HDMIUserInitNum:0x%x\n", g_HDMIUserInitNum);
    if ((g_HDMIKernelInitNum == 0) && (g_HDMIUserInitNum == 0))
    {
        HI_INFO_HDMI("HDMI has been deInited!\n");
        return HI_SUCCESS;
    }

#if 0 /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
    if( (FromUserSpace == HI_TRUE) && (g_HDMIUserInitNum > 0))
    {
        g_HDMIUserInitNum --;
    }
    if( (FromUserSpace == HI_FALSE) && (g_HDMIUserInitNum > 0))
    {
        g_HDMIKernelInitNum --;
    }
#endif /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
    
    if( (FromUserSpace == HI_TRUE) )
    {
        g_HDMIUserInitNum --;
    }
    if( (FromUserSpace == HI_FALSE) )
    {
        g_HDMIKernelInitNum --;
    }

    if(g_HDMIUserInitNum == 0)
    {
        g_UserCallbackFlag = HDMI_CALLBACK_KERNEL;
    }
    //We only do HDMI Deinit when UserLevel Count is 0.
    HI_INFO_HDMI("after g_HDMIUserInitNum:0x%x\n", g_HDMIUserInitNum);
#if 0 /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
    if(g_HDMIUserInitNum != 0 || g_HDMIKernelInitNum != 0)
    {
        return HI_SUCCESS;
    }
#endif /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
    if(DRV_HDMI_InitNum(HI_UNF_HDMI_ID_0) != 0)
    {
        HI_INFO_HDMI("\n ignore DeInit \n");
        return HI_SUCCESS;
    }
    
    g_stHdmiCommParam.bHdmiExit = HI_TRUE;
    HI_INFO_HDMI("stop hdmi task\n");

    if (g_stHdmiCommParam.kThreadTimer)
    {
        ret = kthread_stop(g_stHdmiCommParam.kThreadTimer);
        HI_INFO_HDMI("end HDMI Timer thread. ret = %d\n" , ret);
        g_stHdmiCommParam.kThreadTimer = NULL;
    }
#if defined (CEC_SUPPORT)
    if (g_stHdmiCommParam.kCECRouter)
    {
        ret = kthread_stop(g_stHdmiCommParam.kCECRouter);
        HI_INFO_HDMI("end HDMI CEC thread. ret = %d\n" , ret);
        g_stHdmiCommParam.kCECRouter = NULL;
    }
#endif
    for (i = 0; i < HI_UNF_HDMI_ID_BUTT; i++)
    {
        ret = DRV_HDMI_Close(i);
    }
    SI_HW_ResetHDMITX();
    SI_SW_ResetHDMITX();
    //Disable HDMI IP
    SI_DisableHdmiDevice();
    SI_CloseHdmiDevice();
    //It will power down the whole HDMI IP.
    SI_PoweDownHdmiDevice();

    //force to clear hdmi count
    g_HDMIKernelInitNum = 0;
    g_HDMIUserInitNum   = 0;
    
    HI_INFO_HDMI("Leave DRV_HDMI_DeInit\n");
    return HI_SUCCESS;
}

HI_VOID hdmi_OpenNotify(HI_U32 u32ProcID,HI_UNF_HDMI_EVENT_TYPE_E event)
{
    //HDMI_CHECK_ID(enHdmi);

    HI_INFO_HDMI("\nhdmi_OpenNotify-----u32ProcID %d,event 0x%x\n",u32ProcID,event);
    if (g_stHdmiChnParam[0].bOpen)
    {
        if ((event == HI_UNF_HDMI_EVENT_HOTPLUG))
        {  
            HI_INFO_HDMI("set Interrupts bit\n");
            // Enable Interrupts: VSync, Ri check, HotPlug
#if 0 /*-- ��װ��SI_EnableInterrupts --*/
            WriteByteHDMITXP0( HDMI_INT_ADDR, CLR_MASK);
            WriteByteHDMITXP0( HDMI_INT_MASK_ADDR, CLR_MASK);
#endif /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
            SI_EnableInterrupts();

#if defined (CEC_SUPPORT)
            /* Enable CEC_SETUP */
#if 0 /*--��װ��SI_CEC_SetUp--*/
            HI_INFO_HDMI("set CEC_SETUP\n");
            WriteByteHDMICEC(0X8E, 0x04);
#endif /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
            SI_CEC_SetUp();
#endif
            SI_HPD_SetHPDUserCallbackCount();

            hdmi_SetAndroidState(STATE_HOTPLUGIN);

        }
        else if (event == HI_UNF_HDMI_EVENT_NO_PLUG)
        {
            if (g_HDMIUserInitNum)
            {
                /* Close HDMI Output */
                SI_PowerDownHdmiTx();
                SI_DisableHdmiDevice();
                g_stHdmiChnParam[0].bStart = HI_FALSE;
#if defined (HDCP_SUPPORT)
                /*Set HDCP Off */
                SI_WriteByteEEPROM(EE_TX_HDCP, 0x00);
#endif
#if defined (CEC_SUPPORT)
                //Close CEC
                SI_CEC_Close();
                g_stHdmiChnParam[0].bCECStart       = HI_FALSE;
                g_stHdmiChnParam[0].u8CECCheckCount = 0;
                memset(&(g_stHdmiChnParam[0].stCECStatus), 0, sizeof(HI_UNF_HDMI_CEC_STATUS_S));           
#endif 
            }
            hdmi_SetAndroidState(STATE_HOTPLUGOUT);
        }
        else
        {
            //HI_INFO_HDMI("Unknow Event:0x%x\n", event);
            HI_ERR_HDMI("Unknow Event:0x%x\n", event);
        }
        HI_INFO_HDMI("line:%d,event:%d\n",__LINE__,event);

        hdmi_ProcEvent(event, u32ProcID);           
    }
}

HI_U32 DRV_HDMI_Open(HI_UNF_HDMI_ID_E enHdmi, HDMI_OPEN_S *pOpen, HI_U32 FromUserSpace,HI_U32 u32ProcID)
{

#if 1
    HI_U32 Ret;
    HI_BOOL bOpenLastTime = g_stHdmiChnParam[enHdmi].bOpen;
    
   
    HI_INFO_HDMI("Enter DRV_HDMI_Open\n");
    //printk("enHdmi %d, pOpen->enDefaultMode %d,FromUserSpace %d, u32ProcID %d \n",enHdmi,pOpen->enDefaultMode,FromUserSpace,u32ProcID);
    HDMI_CHECK_ID(enHdmi);

    
    
    g_HDMIOpenNum ++; //record open num

    /* Need to set this  param */
    g_stHdmiCommParam.enDefaultMode = pOpen->enDefaultMode;

    //set output mode
     HI_INFO_HDMI("enForceMode:%d\n", g_stHdmiCommParam.enDefaultMode);
    if (HI_UNF_HDMI_DEFAULT_ACTION_HDMI == g_stHdmiCommParam.enDefaultMode)
    {
        /* force to HDMI */
        Ret = SI_Set_Force_OutputMode(HI_FALSE, HI_TRUE);
    }
#if defined (DVI_SUPPORT)
    else if (HI_UNF_HDMI_DEFAULT_ACTION_DVI == g_stHdmiCommParam.enDefaultMode)
    {
        /* force to DVI */
        Ret = SI_Set_Force_OutputMode(HI_TRUE, HI_FALSE);
    }
#endif
    else if(HI_UNF_HDMI_DEFAULT_ACTION_NULL == g_stHdmiCommParam.enDefaultMode)
    {
        Ret = SI_Set_Force_OutputMode(HI_FALSE, HI_FALSE);
    }

    g_stHdmiChnParam[enHdmi].bOpen = HI_TRUE;//Enable HDMI thread

    //HI_ERR_HDMI("g_HDMIUserInitNum:%d, g_HDMIKernelInitNum:%d\n", g_HDMIUserInitNum, g_HDMIKernelInitNum);

    if (g_HDMIUserInitNum)
    {
        g_UserCallbackFlag = HDMI_CALLBACK_USER;
        if (g_HDMIKernelInitNum > 0)
        {
            /* It means we have initalized in Kernel and user */
            //We need to Reset a HotPlug Event to User
            if (HI_TRUE == SI_HPD_Status())
            {
                //DRV_HDMI_NotifyEvent(HI_UNF_HDMI_EVENT_HOTPLUG);
                hdmi_OpenNotify(u32ProcID,HI_UNF_HDMI_EVENT_HOTPLUG);
            }
            else
            {
                //DRV_HDMI_NotifyEvent(HI_UNF_HDMI_EVENT_NO_PLUG);
                hdmi_OpenNotify(u32ProcID,HI_UNF_HDMI_EVENT_NO_PLUG);
            }
        }
        else
        {   
            HI_INFO_HDMI("SI_HPD_Status():%d\n", SI_HPD_Status());
            if (HI_TRUE == SI_HPD_Status())
            {
                HI_U8 tempOutputState = SI_GetHDMIOutputStatus();
                HI_INFO_HDMI("tempOutputState:%d\n", tempOutputState);

                if((tempOutputState == CABLE_PLUGIN_HDMI_OUT) || (tempOutputState == CABLE_PLUGIN_DVI_OUT))
                {
                    //When OutputState is Out, Phy, HDMI should be open collectly!
                    DEBUG_PRINTK("Warring:Force to send out a HotPlug Event!\n");
                    //DRV_HDMI_NotifyEvent(HI_UNF_HDMI_EVENT_HOTPLUG);
                    hdmi_OpenNotify(u32ProcID,HI_UNF_HDMI_EVENT_HOTPLUG);
                }
            }
        }
    }
    else if (g_HDMIKernelInitNum)
    {
        g_UserCallbackFlag = HDMI_CALLBACK_KERNEL;
    }

    if (HI_FALSE == g_stHdmiCommParam.bOpenGreenChannel)
    {
        if (bOpenLastTime)
        {
            return HI_SUCCESS;
        }
    }
  
    //// ֻ��һ���� ////
    // Enable Interrupts: VSync, Ri check, HotPlug
    WriteByteHDMITXP0( HDMI_INT_ADDR, CLR_MASK);
    WriteByteHDMITXP0( HDMI_INT_MASK_ADDR, CLR_MASK);

    HI_INFO_HDMI("Leave DRV_HDMI_Open\n");
    return HI_SUCCESS;
#else
   HI_U32 Ret;
   
    HI_INFO_HDMI("Enter DRV_HDMI_Open\n");
    
    HDMI_CHECK_ID(enHdmi);
    g_HDMIOpenNum ++; //record open num

    if (HI_FALSE == g_stHdmiCommParam.bOpenGreenChannel)
    {
        if (g_stHdmiChnParam[enHdmi].bOpen)
        {
            return HI_SUCCESS;
        }      
    }
   
    /* Need to set this  param */
    g_stHdmiCommParam.enDefaultMode = pOpen->enDefaultMode;

    //set output mode
     HI_INFO_HDMI("enForceMode:%d\n", g_stHdmiCommParam.enDefaultMode);
    if (HI_UNF_HDMI_DEFAULT_ACTION_HDMI == g_stHdmiCommParam.enDefaultMode)
    {
        /* force to HDMI */
        Ret = SI_Set_Force_OutputMode(HI_FALSE, HI_TRUE);
    }
#if defined (DVI_SUPPORT)
    else if (HI_UNF_HDMI_DEFAULT_ACTION_DVI == g_stHdmiCommParam.enDefaultMode)
    {
        /* force to DVI */
        Ret = SI_Set_Force_OutputMode(HI_TRUE, HI_FALSE);
    }
#endif
    else if(HI_UNF_HDMI_DEFAULT_ACTION_NULL == g_stHdmiCommParam.enDefaultMode)
    {
        Ret = SI_Set_Force_OutputMode(HI_FALSE, HI_FALSE);
    }
    
  
    // Enable Interrupts: VSync, Ri check, HotPlug
    #if 0
    WriteByteHDMITXP0( HDMI_INT_ADDR, CLR_MASK);
    WriteByteHDMITXP0( HDMI_INT_MASK_ADDR, CLR_MASK);
    #endif
    SI_EnableInterrupts();

    g_stHdmiChnParam[enHdmi].bOpen = HI_TRUE;//Enable HDMI thread

    HI_INFO_HDMI("g_HDMIUserInitNum:%d, g_HDMIKernelInitNum:%d\n", g_HDMIUserInitNum, g_HDMIKernelInitNum);

    if (g_HDMIUserInitNum)
    {
        g_UserCallbackFlag = HDMI_CALLBACK_USER;
        if (g_HDMIKernelInitNum > 0)
        {
            /* It means we have initalized in Kernel and user */
            //We need to Reset a HotPlug Event to User
            if (HI_TRUE == SI_HPD_Status())
            {
                DRV_HDMI_NotifyEvent(HI_UNF_HDMI_EVENT_HOTPLUG);
            }
            else
            {
                DRV_HDMI_NotifyEvent(HI_UNF_HDMI_EVENT_NO_PLUG);
            }
        }
        else
        {
            HI_INFO_HDMI("SI_HPD_Status():%d\n", SI_HPD_Status());
            if (HI_TRUE == SI_HPD_Status())
            {
                HI_U8 tempOutputState = SI_GetHDMIOutputStatus();
                HI_INFO_HDMI("tempOutputState:%d\n", tempOutputState);

                if((tempOutputState == CABLE_PLUGIN_HDMI_OUT) || (tempOutputState == CABLE_PLUGIN_DVI_OUT))
                {
                    //When OutputState is Out, Phy, HDMI should be open collectly!
                    DEBUG_PRINTK("Warring:Force to send out a HotPlug Event!\n");
                    DRV_HDMI_NotifyEvent(HI_UNF_HDMI_EVENT_HOTPLUG);
                }
            }
        }
    }
    else if (g_HDMIKernelInitNum)
    {
        g_UserCallbackFlag = HDMI_CALLBACK_KERNEL;
    }

    HI_INFO_HDMI("Leave DRV_HDMI_Open\n");
    printk("Leave DRV_HDMI_Open\n");
    return HI_SUCCESS;


#endif 
}

HI_U32 DRV_HDMI_Close(HI_UNF_HDMI_ID_E enHdmi)
{
    HI_INFO_HDMI("Enter DRV_HDMI_Close g_HDMIOpenNum:%d\n", g_HDMIOpenNum);
    HDMI_CHECK_ID(enHdmi);

    //Stop HDMI IP
    if( g_HDMIOpenNum > 1)
    {
        g_HDMIOpenNum --;
        return HI_SUCCESS;
    }
    else if(g_HDMIOpenNum == 1)
    {
        g_HDMIOpenNum = 0;
    }
    else
    {
    //Just do following action.
    }

    if (HI_TRUE == g_stHdmiChnParam[enHdmi].bStart)
    {
        SI_SetHdmiVideo(HI_FALSE);
        SI_SetHdmiAudio(HI_FALSE);
        SI_PowerDownHdmiTx();
#if defined (HDCP_SUPPORT)
        /*Set HDCP Off */
        SI_WriteByteEEPROM(EE_TX_HDCP, 0x00);
        msleep(50);
#endif
        g_stHdmiChnParam[enHdmi].bStart = HI_FALSE;
    }
    //Close HDMI
    if (HI_TRUE == g_stHdmiChnParam[enHdmi].bOpen)
    {
        g_stHdmiChnParam[enHdmi].bOpen = HI_FALSE;
    }  
    //No callback
    g_UserCallbackFlag = HDMI_CALLBACK_NULL;
    //Disable HDMI IP
    SI_DisableHdmiDevice();
    SI_CloseHdmiDevice();

    HI_INFO_HDMI("Leave DRV_HDMI_Close\n");
    return HI_SUCCESS;
}

HI_U32 DRV_HDMI_GetSinkCapability(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_SINK_CAPABILITY_S *pstSinkAttr)
{
    HI_INFO_HDMI("Enter DRV_HDMI_GetSinkCapability\n");
    HDMI_CHECK_NULL_PTR(pstSinkAttr);    
    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);

    //if(g_stHdmiCommParam.bReadEDIDOk == HI_TRUE)
    //{
        
    //}
    //else
    //{
        if (0 != SI_GetHdmiSinkCaps(pstSinkAttr))
        {
            HI_INFO_HDMI("GetHdmiSinkCaps error.\n");
            return HI_FAILURE;
        }
    //}

    HI_INFO_HDMI("Leave DRV_HDMI_GetSinkCapability\n");
    return HI_SUCCESS;
}

static HI_U32 hdmi_IsChange_Attr(HI_UNF_HDMI_ID_E enHdmi, HDMI_ATTR_S *pstAttr1, HDMI_ATTR_S *pstAttr2,
    HI_BOOL *pVUpdate, HI_BOOL *pAUpdate)
{
    HI_U32 Ret = HI_SUCCESS;

    *pVUpdate = HI_FALSE;
    *pAUpdate = HI_TRUE;

    /* HDMI Has not start, we set thes value */
    if (HI_FALSE == g_stHdmiChnParam[enHdmi].bStart)
    {
        *pVUpdate = HI_TRUE;
        *pAUpdate = HI_TRUE;
        return HI_SUCCESS;
    }
    /* Same setting, return directly */
    if ((pstAttr1->stAttr.bEnableHdmi         == pstAttr2->stAttr.bEnableHdmi)
        && (pstAttr1->stAttr.bEnableVideo        == pstAttr2->stAttr.bEnableVideo)
        && (pstAttr1->stAttr.enVideoFmt          == pstAttr2->stAttr.enVideoFmt)
        && (pstAttr1->stAttr.enVidOutMode        == pstAttr2->stAttr.enVidOutMode)
        && (pstAttr1->stAttr.enDeepColorMode     == pstAttr2->stAttr.enDeepColorMode)
        && (pstAttr1->stAttr.bxvYCCMode          == pstAttr2->stAttr.bxvYCCMode)
        && (pstAttr1->stAttr.bEnableAudio        == pstAttr2->stAttr.bEnableAudio)
        && (pstAttr1->enSoundIntf         == pstAttr2->enSoundIntf)
        && (pstAttr1->stAttr.bIsMultiChannel     == pstAttr2->stAttr.bIsMultiChannel)
        && (pstAttr1->stAttr.enSampleRate        == pstAttr2->stAttr.enSampleRate)
        && (pstAttr1->stAttr.u8DownSampleParm    == pstAttr2->stAttr.u8DownSampleParm)
        && (pstAttr1->stAttr.enBitDepth          == pstAttr2->stAttr.enBitDepth)
        && (pstAttr1->stAttr.u8I2SCtlVbit        == pstAttr2->stAttr.u8I2SCtlVbit)
        && (pstAttr1->stAttr.bEnableAviInfoFrame == pstAttr2->stAttr.bEnableAviInfoFrame)
        && (pstAttr1->stAttr.bEnableAudInfoFrame == pstAttr2->stAttr.bEnableAudInfoFrame)
        && (pstAttr1->stAttr.bEnableSpdInfoFrame == pstAttr2->stAttr.bEnableSpdInfoFrame)
        && (pstAttr1->stAttr.bEnableMpegInfoFrame== pstAttr2->stAttr.bEnableMpegInfoFrame)
        && (pstAttr1->stAttr.bDebugFlag          == pstAttr2->stAttr.bDebugFlag)
        && (pstAttr1->stAttr.bHDCPEnable         == pstAttr2->stAttr.bHDCPEnable)
        && (pstAttr1->stAttr.b3DEnable           == pstAttr2->stAttr.b3DEnable)
        && (pstAttr1->stAttr.u83DParam           == pstAttr2->stAttr.u83DParam)
        )
        {
            *pVUpdate = HI_FALSE;
            *pAUpdate = HI_FALSE;
            HI_INFO_HDMI("Same as before, do not need to setting!\n");
            return HI_FAILURE;
        }

    if ((pstAttr1->stAttr.bEnableHdmi            != pstAttr2->stAttr.bEnableHdmi)
        || (pstAttr1->stAttr.bEnableVideo        != pstAttr2->stAttr.bEnableVideo)
        || (pstAttr1->stAttr.enVideoFmt          != pstAttr2->stAttr.enVideoFmt)
        || (pstAttr1->stAttr.enVidOutMode        != pstAttr2->stAttr.enVidOutMode)
        || (pstAttr1->stAttr.enDeepColorMode     != pstAttr2->stAttr.enDeepColorMode)
        || (pstAttr1->stAttr.bxvYCCMode          != pstAttr2->stAttr.bxvYCCMode)
        || (pstAttr1->stAttr.bEnableAviInfoFrame != pstAttr2->stAttr.bEnableAviInfoFrame)
        || (pstAttr1->stAttr.b3DEnable           != pstAttr2->stAttr.b3DEnable)
        || (pstAttr1->stAttr.u83DParam           != pstAttr2->stAttr.u83DParam)
        || (pstAttr1->stAttr.bDebugFlag          != pstAttr2->stAttr.bDebugFlag)
        || (pstAttr1->stAttr.bHDCPEnable         != pstAttr2->stAttr.bHDCPEnable)
       )
    {
        *pVUpdate = HI_TRUE;
    }

    if (*pVUpdate == HI_FALSE)
    {
        HI_INFO_HDMI("We do not need to update Video!\n");
    }

    return Ret;
}

HI_BOOL get_current_rgb_mode(HI_UNF_HDMI_ID_E enHdmi)
{
    return (HI_UNF_HDMI_VIDEO_MODE_RGB444 == g_stHdmiChnParam[enHdmi].stHDMIAttr.stAttr.enVidOutMode )?HI_TRUE:HI_FALSE;
}

static HI_U32 hdmi_SetAttr(HI_UNF_HDMI_ID_E enHdmi, HDMI_ATTR_S *pstHDMIAttr, HI_BOOL UpdateFlag)
{
    HI_U32 Ret = HI_SUCCESS;
    //HI_U32 Ret = HI_SUCCESS, u32Value = 0;
    HI_U8 ucData       = 0;
    HI_U8 bRxVideoMode = 0;
    HI_U8 bTxVideoMode = 0;
    HI_U8 bVideoMode;               /* Hdmi Video mode index define in vmtables.c */
    //HI_U8 RegVal;
    HI_UNF_HDMI_INFOFRAME_S stInfoFrame;
    HI_UNF_HDMI_ATTR_S *pstAttr;

    HI_BOOL VUpdate = HI_FALSE;
    HI_BOOL AUpdate = HI_FALSE;

    HI_INFO_HDMI("Enter hdmi_SetAttr\n");
    SI_timer_count();
    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);
    HDMI_CHECK_NULL_PTR(pstHDMIAttr);
    pstAttr = &pstHDMIAttr->stAttr;
    
    //printk("%s: 4.stAttr.enVideoFmt:%d\n",__FUNCTION__,pstHDMIAttr->stAttr.enVideoFmt);
    
    if(pstAttr->enVidOutMode == HI_UNF_HDMI_VIDEO_MODE_YCBCR422)
    {
        HI_ERR_HDMI("%s.%d : OutMod == YCBCR422.not support return \n",__FUNCTION__,__LINE__);
        return HI_ERR_HDMI_INVALID_PARA;      
    }

    pstAttr->enVideoFmt = DRV_HDMI_CheckVOFormat(pstAttr->enVideoFmt);

    if (pstAttr->bEnableVideo != HI_TRUE)
    {
        HI_ERR_HDMI("bEnableVideo Must be set to HI_TRUE!\n");
        pstAttr->bEnableVideo = HI_TRUE;
    }
    
    if (UpdateFlag == HI_TRUE)
    {
        VUpdate = HI_TRUE;
        AUpdate = HI_TRUE;
    }
    else
    {
        if (HI_SUCCESS != hdmi_IsChange_Attr(enHdmi, pstHDMIAttr, &(g_stHdmiChnParam[enHdmi].stHDMIAttr), &VUpdate, &AUpdate))
        {
            HI_INFO_HDMI("%s.%d : Attr Not change.return \n",__FUNCTION__,__LINE__);
            return HI_SUCCESS;
        }
    }

    /* Set HDMI InfoFrame Enable flag */
    SI_WriteByteEEPROM(EE_AVIINFO_ENABLE, pstAttr->bEnableAviInfoFrame);
    SI_WriteByteEEPROM(EE_AUDINFO_ENABLE, pstAttr->bEnableAudInfoFrame);
    SI_WriteByteEEPROM(EE_SPDINFO_ENABLE, pstAttr->bEnableSpdInfoFrame);

    /* We need to just whether we need to update video or audio parmaeter */
    if(pstHDMIAttr->enSoundIntf == HDMI_AUDIO_INTERFACE_BUTT)
    {
        pstHDMIAttr->enSoundIntf = g_stHdmiChnParam[enHdmi].stHDMIAttr.enSoundIntf;
    }
    memcpy(&(g_stHdmiChnParam[enHdmi].stHDMIAttr), pstHDMIAttr, sizeof(HDMI_ATTR_S));

    ucData = 0x00;    // DE(Data Enable generator) disable
    //WriteByteEEPROM(EE_TX_DE_ENABLED_ADDR, ucData); 
    SI_WriteByteEEPROM(EE_TX_DE_ENABLED_ADDR, ucData);

    /* Adjust Video Path Param */
    if ((VUpdate == HI_TRUE) && (pstAttr->bEnableVideo == HI_TRUE) && (HI_TRUE != g_stHdmiCommParam.bOpenGreenChannel))
    {
        HI_U8 u8VideoPath[4];

        //  SI_SetHdmiVideo(HI_FALSE);

        /* Write VIDEO MODE INDEX */
        /* Transfer_VideoTimingFromat_to_VModeIndex() is get g_s32VmodeOfUNFFormat[] */
        bVideoMode = Transfer_VideoTimingFromat_to_VModeTablesIndex(pstAttr->enVideoFmt);
        if (0 == bVideoMode)
        {
            HI_INFO_HDMI("The HDMI FMT (%d) is invalid.\n", pstAttr->enVideoFmt);
            SI_WriteByteEEPROM(EE_TX_VIDEO_MODE_INDEX_ADDR, 2); /* 720P */
        }
        else
        {
            HI_INFO_HDMI("The HDMI_VMode of HI_UNF_ENC_FMT(%d) is %d.\n", pstAttr->enVideoFmt, bVideoMode);
            SI_WriteByteEEPROM(EE_TX_VIDEO_MODE_INDEX_ADDR, bVideoMode);
        }

        SI_GetVideoPath(u8VideoPath); /* hdmi/txvidp.c, default setting DefaultTXVPath[4] in txvptbl.c  */
        /* inMode[] is defined in hdmi/txvptbl.c */
        HI_INFO_HDMI("default u8VideoPath :0x%02x,0x%02x,0x%02x,0x%02x\n", u8VideoPath[0], u8VideoPath[1], u8VideoPath[2], u8VideoPath[3]);

        if (pstAttr->enVideoFmt == HI_DRV_DISP_FMT_861D_640X480_60) 
        {
            //g_stHdmiCommParam.enVidInMode = HI_UNF_HDMI_VIDEO_MODE_YCBCR444;
            g_stHdmiCommParam.enVidInMode = HI_UNF_HDMI_VIDEO_MODE_RGB444;
        }
#if defined (DVI_SUPPORT)
        else if ((pstAttr->enVideoFmt > HI_DRV_DISP_FMT_861D_640X480_60) && (pstAttr->enVideoFmt <= HI_DRV_DISP_FMT_VESA_2048X1152_60))
        {
            HI_INFO_HDMI("LCD Format, force to RGB444 into hdmi ip\n");
            g_stHdmiCommParam.enVidInMode = HI_UNF_HDMI_VIDEO_MODE_RGB444;
        }
#endif
        else if ((pstAttr->enVideoFmt == HI_DRV_DISP_FMT_PAL)||
            (pstAttr->enVideoFmt == HI_DRV_DISP_FMT_PAL_B)||
            (pstAttr->enVideoFmt == HI_DRV_DISP_FMT_PAL_B1)||
            (pstAttr->enVideoFmt == HI_DRV_DISP_FMT_PAL_D)||
            (pstAttr->enVideoFmt == HI_DRV_DISP_FMT_PAL_D1)||
            (pstAttr->enVideoFmt == HI_DRV_DISP_FMT_PAL_G)||
            (pstAttr->enVideoFmt == HI_DRV_DISP_FMT_PAL_H)||
            (pstAttr->enVideoFmt == HI_DRV_DISP_FMT_PAL_K)||
            (pstAttr->enVideoFmt == HI_DRV_DISP_FMT_PAL_I)||
            (pstAttr->enVideoFmt == HI_DRV_DISP_FMT_PAL_M)||
            (pstAttr->enVideoFmt == HI_DRV_DISP_FMT_PAL_N)||
            (pstAttr->enVideoFmt == HI_DRV_DISP_FMT_PAL_Nc)||
            (pstAttr->enVideoFmt == HI_DRV_DISP_FMT_PAL_60)||        
            (pstAttr->enVideoFmt == HI_DRV_DISP_FMT_SECAM_SIN)||
            (pstAttr->enVideoFmt == HI_DRV_DISP_FMT_SECAM_COS)||
            (pstAttr->enVideoFmt == HI_DRV_DISP_FMT_SECAM_L)||
            (pstAttr->enVideoFmt == HI_DRV_DISP_FMT_SECAM_B)||
            (pstAttr->enVideoFmt == HI_DRV_DISP_FMT_SECAM_G)||
            (pstAttr->enVideoFmt == HI_DRV_DISP_FMT_SECAM_D)||
            (pstAttr->enVideoFmt == HI_DRV_DISP_FMT_SECAM_K)||
            (pstAttr->enVideoFmt == HI_DRV_DISP_FMT_SECAM_H))
        {
            HI_INFO_HDMI("SD TV Format, force to YCBCR444 into hdmi ip\n");
            //g_stHdmiCommParam.enVidInMode = HI_UNF_HDMI_VIDEO_MODE_YCBCR422;
            //DEBUG_PRINTK("SD TV Format, force to YCBCR444 into hdmi ip\n");
            g_stHdmiCommParam.enVidInMode = HI_UNF_HDMI_VIDEO_MODE_YCBCR444;
        }
        else if ((pstAttr->enVideoFmt == HI_DRV_DISP_FMT_NTSC)||
            (pstAttr->enVideoFmt == HI_DRV_DISP_FMT_NTSC_J)||
            (pstAttr->enVideoFmt == HI_DRV_DISP_FMT_NTSC_443))
        {
            HI_INFO_HDMI("SD TV Format, force to YCBCR444 into hdmi ip\n");
            //g_stHdmiCommParam.enVidInMode = HI_UNF_HDMI_VIDEO_MODE_YCBCR422;
            //DEBUG_PRINTK("SD TV Format, force to YCBCR444 into hdmi ip\n");
            g_stHdmiCommParam.enVidInMode = HI_UNF_HDMI_VIDEO_MODE_YCBCR444;
        }
        else
        {
            HI_INFO_HDMI("DTV Format, force to YCbCr444 into hdmi ip\n");
            g_stHdmiCommParam.enVidInMode = HI_UNF_HDMI_VIDEO_MODE_YCBCR444;
        }

        //DEBUG_PRINTK("enVidInMode:%d, pstAttr->enVidOutMode:%d\n", g_stHdmiCommParam.enVidInMode, pstAttr->enVidOutMode);
        if (HI_UNF_HDMI_VIDEO_MODE_RGB444 == g_stHdmiCommParam.enVidInMode)
        {
            bRxVideoMode = 0;  /* inRGB24[] */
            if (HI_UNF_HDMI_VIDEO_MODE_RGB444 == pstAttr->enVidOutMode)
            {
                HI_INFO_HDMI("HDMI Input RGB444, Output RGB444\n");
                bTxVideoMode = 0;
            }
            else
            {
                DEBUG_PRINTK("Error output mode when input RGB444\n");
                pstAttr->enVidOutMode = HI_UNF_HDMI_VIDEO_MODE_RGB444;
                bTxVideoMode = 0;
                Ret = HI_FAILURE;
            }
        }
        else if (HI_UNF_HDMI_VIDEO_MODE_YCBCR444 == g_stHdmiCommParam.enVidInMode)
        {
            HI_U32 reg;

            bRxVideoMode = 2; /* inYCbCr24[] */

            reg = ReadByteHDMITXP0(VID_ACEN_ADDR);//down sampler
            if (HI_UNF_HDMI_VIDEO_MODE_RGB444 == pstAttr->enVidOutMode)
            {
                HI_INFO_HDMI("HDMI Input YCBCR444, Output RGB444\n");
                bTxVideoMode = 0;
                reg &= ~BIT_VID_ACEN_DWN_SAMPLE;
            }
            else if(HI_UNF_HDMI_VIDEO_MODE_YCBCR444 == pstAttr->enVidOutMode)
            {
                HI_INFO_HDMI("HDMI Input YCBCR444, Output YCBCR444\n");
                bTxVideoMode = 1;
                reg &= ~BIT_VID_ACEN_DWN_SAMPLE;
            }
            else
            {
                DEBUG_PRINTK("Input YCBCR444, Output YCBCR422\n");
                bTxVideoMode = 1;
                reg |= BIT_VID_ACEN_DWN_SAMPLE;
                Ret = HI_FAILURE;
            }
            WriteByteHDMITXP0(VID_ACEN_ADDR, (HI_U8)reg);
        }
        else if (HI_UNF_HDMI_VIDEO_MODE_YCBCR422 == g_stHdmiCommParam.enVidInMode)
        {
            bRxVideoMode = 3; /* inYC24[] */
            if (HI_UNF_HDMI_VIDEO_MODE_RGB444 == pstAttr->enVidOutMode)
            {
                HI_INFO_HDMI("HDMI Input YCBCR422, Output RGB444\n");
                bTxVideoMode = 0;
            }
            else if (HI_UNF_HDMI_VIDEO_MODE_YCBCR444 == pstAttr->enVidOutMode)
            {
                HI_INFO_HDMI("HDMI Input YCBCR422, Output YCBCR444\n");
                bTxVideoMode = 1;
            }
            else
            {
                HI_INFO_HDMI("HDMI Input YCBCR422, Output YCBCR422\n");
                bTxVideoMode = 2;
            }
        }
        u8VideoPath[0] = bRxVideoMode;
        u8VideoPath[1] = bTxVideoMode;
        //u8VideoPath[2]; can use default value form getfunction.
        if (HI_UNF_HDMI_DEEP_COLOR_24BIT == pstAttr->enDeepColorMode)
        {
            u8VideoPath[3] = 0;
        }
        else if (HI_UNF_HDMI_DEEP_COLOR_30BIT == pstAttr->enDeepColorMode)
        {
            u8VideoPath[3] = 1;
        }
        else if (HI_UNF_HDMI_DEEP_COLOR_36BIT == pstAttr->enDeepColorMode)
        {
            u8VideoPath[3] = 2;
        }
        else if (HI_UNF_HDMI_DEEP_COLOR_OFF >= pstAttr->enDeepColorMode)
        {
            u8VideoPath[3] = 0xFF;
        }

        SI_UpdateTX_656(bVideoMode);
        SI_SetIClk( SI_ReadByteEEPROM(EE_TX_ICLK_ADDR) ); 


        HI_INFO_HDMI("setting video path u8VideoPath :0x%02x,0x%02x,0x%02x,0x%02x\n", u8VideoPath[0], u8VideoPath[1], u8VideoPath[2], u8VideoPath[3]);
        bVideoMode = SI_ReadByteEEPROM(EE_TX_VIDEO_MODE_INDEX_ADDR);
        HI_INFO_HDMI("bVideoMode:0x%x\n", bVideoMode);
        SI_SetVideoPath(bVideoMode, u8VideoPath);
        SI_BlockWriteEEPROM( 4, EE_TX_VIDEOPATH_ADDR , u8VideoPath);        

        HI_INFO_HDMI("out of siiSetVideoPath\n");

        /* Hsync/Vsync polarity:Video DE Control Register:VS_POL#, HS_POL# */
        if ((pstAttr->enVideoFmt == HI_DRV_DISP_FMT_576P_50)||
            (pstAttr->enVideoFmt == HI_DRV_DISP_FMT_480P_60)||
            (pstAttr->enVideoFmt == HI_DRV_DISP_FMT_PAL)||
            (pstAttr->enVideoFmt == HI_DRV_DISP_FMT_PAL_B)||
            (pstAttr->enVideoFmt == HI_DRV_DISP_FMT_PAL_B1)||
            (pstAttr->enVideoFmt == HI_DRV_DISP_FMT_PAL_D)||
            (pstAttr->enVideoFmt == HI_DRV_DISP_FMT_PAL_D1)||
            (pstAttr->enVideoFmt == HI_DRV_DISP_FMT_PAL_G)||
            (pstAttr->enVideoFmt == HI_DRV_DISP_FMT_PAL_H)||
            (pstAttr->enVideoFmt == HI_DRV_DISP_FMT_PAL_K)||
            (pstAttr->enVideoFmt == HI_DRV_DISP_FMT_PAL_I)||
            (pstAttr->enVideoFmt == HI_DRV_DISP_FMT_PAL_M)||
            (pstAttr->enVideoFmt == HI_DRV_DISP_FMT_PAL_N)||
            (pstAttr->enVideoFmt == HI_DRV_DISP_FMT_PAL_Nc)||
            (pstAttr->enVideoFmt == HI_DRV_DISP_FMT_PAL_60)||        
            (pstAttr->enVideoFmt == HI_DRV_DISP_FMT_SECAM_SIN)||
            (pstAttr->enVideoFmt == HI_DRV_DISP_FMT_SECAM_COS)||
            (pstAttr->enVideoFmt == HI_DRV_DISP_FMT_SECAM_L)||
            (pstAttr->enVideoFmt == HI_DRV_DISP_FMT_SECAM_B)||
            (pstAttr->enVideoFmt == HI_DRV_DISP_FMT_SECAM_G)||
            (pstAttr->enVideoFmt == HI_DRV_DISP_FMT_SECAM_D)||
            (pstAttr->enVideoFmt == HI_DRV_DISP_FMT_SECAM_K)||
            (pstAttr->enVideoFmt == HI_DRV_DISP_FMT_SECAM_H)||
            (pstAttr->enVideoFmt == HI_DRV_DISP_FMT_NTSC)||
            (pstAttr->enVideoFmt == HI_DRV_DISP_FMT_NTSC_J)||
            (pstAttr->enVideoFmt == HI_DRV_DISP_FMT_NTSC_443)||
            (pstAttr->enVideoFmt == HI_DRV_DISP_FMT_861D_640X480_60))
        {
        #if 0
            RegVal = ReadByteHDMITXP0(DE_CNTRL_ADDR);
            HI_INFO_HDMI("DE_CNTRL_ADDR:0x%x, Before RegVal:0x%x\n", DE_CNTRL_ADDR, RegVal);
            RegVal |= 0x30;
            WriteByteHDMITXP0(DE_CNTRL_ADDR,RegVal);
            HI_INFO_HDMI("Neagtiv Polarity DE_CNTRL_ADDR:0x%x, change RegVal:0x%x\n", DE_CNTRL_ADDR, RegVal);
        #endif
            SI_TX_InvertSyncPol(HI_TRUE);
        }
        else
        {
        #if 0
            RegVal = ReadByteHDMITXP0(DE_CNTRL_ADDR);
            HI_INFO_HDMI("DE_CNTRL_ADDR:0x%x, Before RegVal:0x%x\n", DE_CNTRL_ADDR, RegVal);
            RegVal &= (~0x30);
            WriteByteHDMITXP0(DE_CNTRL_ADDR,RegVal);
            HI_INFO_HDMI("Positive Polarity DE_CNTRL_ADDR:0x%x, change RegVal:0x%x\n", DE_CNTRL_ADDR, RegVal);
        #endif 
            SI_TX_InvertSyncPol(HI_FALSE);
        }

        /* DeepColor Atribute */  
        if ( ((SiI_DeepColor_30bit == pstAttr->enDeepColorMode) || (SiI_DeepColor_36bit == pstAttr->enDeepColorMode))
            && ((HI_DRV_DISP_FMT_1080P_60 == pstAttr->enVideoFmt) || (HI_DRV_DISP_FMT_1080P_50 == pstAttr->enVideoFmt))
           )
        {
            SI_TX_PHY_HighBandwidth(HI_TRUE);
        }
        else
        {
            SI_TX_PHY_HighBandwidth(HI_FALSE);
        }
    }
    //printk("%s: 5.stAttr.enVideoFmt:%d\n",__FUNCTION__,pstHDMIAttr->stAttr.enVideoFmt);
    /* Adjust Audio Path Param */
    if ((AUpdate == HI_TRUE) && (pstAttr->bEnableAudio == HI_TRUE))
    {
        HI_U8 u8AudioPath[4];
        HI_U32 audioSampleRate = 0;

        SI_SetHdmiAudio(HI_FALSE);

        audioSampleRate = pstAttr->enSampleRate;

        if (pstAttr->u8DownSampleParm != 0)
        {
            /* downsample audio freqency */
            if( 1 == pstAttr->u8DownSampleParm)
            {
                WriteByteHDMITXP1(SAMPLE_RATE_CONVERSION, 0x01);
            }
            else if( 2 == pstAttr->u8DownSampleParm)
            {
                WriteByteHDMITXP1(SAMPLE_RATE_CONVERSION, 0x03);
            }
        }
        else
        {
            /* Remove Downsample flag */
            WriteByteHDMITXP1(SAMPLE_RATE_CONVERSION, 0x00);
        }

        /* Use default setting before write Audio Path */
        SI_GetAudioPath(u8AudioPath);   /* hdmi/audio.c, default setting DefaultTXDVDAudio[4] in txvptbl.c */
        HI_INFO_HDMI("default audio path: 0x%02x,0x%02x,0x%02x,0x%02x\n", u8AudioPath[0], u8AudioPath[1], u8AudioPath[2], u8AudioPath[3]);
        memset(u8AudioPath, 0, 4);

        /* abAudioPath[0] set Audio format & Bit 7 */
        if (HDMI_AUDIO_INTERFACE_SPDIF == pstHDMIAttr->enSoundIntf)
        {
            u8AudioPath[0] = 0x0;
        }
        else if (HDMI_AUDIO_INTERFACE_HBR == pstHDMIAttr->enSoundIntf)
        {
            HI_INFO_HDMI("\nAudio input is HBR\n");
            u8AudioPath[0] = SiI_HBAudio;
        }
        else
        {
            u8AudioPath[0] = 0x1; //I2S format
        }
        /* Bit[7] of abAudioPath[0] */
        if (HI_TRUE == pstAttr->bIsMultiChannel)
        {
            u8AudioPath[0] |= 0x80;
        }
        else
        {
            u8AudioPath[0] &= ~0x80;
        }
        /* abAudioPath[1] set Sampling Freq Fs */
        /*
           CH_ST4          Fs Sampling Frequency
           3   2   1   0  <--- bit
           0   0   0   0   44.1 kHz
           1   0   0   0   88.2 kHz
           1   1   0   0   176.4 kHz
           0   0   1   0   48 kHz
           1   0   1   0   96 kHz
           1   1   1   0   192 kHz
           0   0   1   1   32 kHz
           0   0   0   1   not indicated
           */
        if(HI_UNF_SAMPLE_RATE_44K == audioSampleRate)
        {
            u8AudioPath[1] = 0x00;
        }
        else if (HI_UNF_SAMPLE_RATE_88K == audioSampleRate)
        {
            u8AudioPath[1] = 0x08;
        }
        else if (176400 == audioSampleRate)
        {
            u8AudioPath[1] = 0x0c;
        }
        else if (HI_UNF_SAMPLE_RATE_48K == audioSampleRate)
        {
            u8AudioPath[1] = 0x02;
        }
        else if (HI_UNF_SAMPLE_RATE_96K == audioSampleRate)
        {
            u8AudioPath[1] = 0x0a;
        }
        else if (HI_UNF_SAMPLE_RATE_192K == audioSampleRate)
        {
            u8AudioPath[1] = 0x0e;
        }
        else if (HI_UNF_SAMPLE_RATE_32K == audioSampleRate)
        {
            u8AudioPath[1] = 0x03;
        }
        else
        {
            u8AudioPath[1] = 0x00;
        }
        /* abAudioPath[2] set Sample length */
        /*
           IN_LENGTH 3:0 <--- sample length
           (0xff)0b1111 - 0b1110 = N/A
           (0x0d)0b1101 = 21 bit
           (0x0c)0b1100 = 17 bit
           (0x0b)0b1011 = 24 bit
           (0x0a)0b1010 = 20 bit
           (0x09)0b1001 = 23 bit
           (0x08)0b1000 = 19 bit
           (0x07)0b0111 - 0b0110 = N/A
           (0x05)0b0101 = 22 bit
           (0x04)0b0100 = 18 bit
           0b0011 = N/A
           (0x02)0b0010 = 16 bit
           0b0001 - 0b0000 = N/A
           */
        if (HI_UNF_BIT_DEPTH_16 == pstAttr->enBitDepth)
        {
            u8AudioPath[2] = 0x02;
        }
        else if (HI_UNF_BIT_DEPTH_18 == pstAttr->enBitDepth)
        {
            u8AudioPath[2] = 0x04;
        }
        else if (22 == pstAttr->enBitDepth)
        {
            u8AudioPath[2] = 0x05;
        }
        else if (19 == pstAttr->enBitDepth)
        {
            u8AudioPath[2] = 0x08;
        }
        else if (23 == pstAttr->enBitDepth)
        {
            u8AudioPath[2] = 0x09;
        }
        else if (20 == pstAttr->enBitDepth)
        {
            u8AudioPath[2] = 0x0a;
        }
        else if (HI_UNF_BIT_DEPTH_24 == pstAttr->enBitDepth)
        {
            u8AudioPath[2] = 0x0b;
        }
        else if (17 == pstAttr->enBitDepth)
        {
            u8AudioPath[2] = 0x0c;
        }
        else if (21 == pstAttr->enBitDepth)
        {
            u8AudioPath[2] = 0x0d;
        }
        else
        {
            u8AudioPath[2] = 0x0f;
        }
        /* abAudioPath[3] I2S control bits (for 0x7A:0x1D) */
        if(HI_TRUE == pstAttr->u8I2SCtlVbit)
        {
            u8AudioPath[3] |= 0x10; /* 1 = Compressed */
        }
        else
        {
            u8AudioPath[3] &= (HI_U8)(~0x10); /* 0 = Uncompressed */
        }
        u8AudioPath[3] |= 0x40; /* SCK sample edge Must use "Sample clock is rising" */
        HI_INFO_HDMI("set audio path: 0x%02x,0x%02x,0x%02x,0x%02x\n", u8AudioPath[0], u8AudioPath[1], u8AudioPath[2], u8AudioPath[3]);
        SI_SetAudioPath(u8AudioPath);  /* hdmi/audio.c */
    }


    if (pstAttr->b3DEnable == HI_TRUE)
        Ret |= SI_3D_Setting(pstAttr->u83DParam);
    else
        Ret |= SI_3D_Setting(0xff);

    SI_SetHdmiVideo(pstAttr->bEnableVideo);
    HI_INFO_HDMI("SET hdmi audio status Flag:%d\n", pstAttr->bEnableAudio);
    /* Set HDMI Audio Enable flag */
    SI_SetHdmiAudio(pstAttr->bEnableAudio);
    SI_timer_count();

    if((VUpdate == HI_TRUE) && (pstAttr->bEnableHdmi == HI_TRUE))
    {
        hdmi_GetInfoFrame(enHdmi, HI_INFOFRAME_TYPE_AVI, &stInfoFrame); 
        hdmi_SetInfoFrame(enHdmi, &stInfoFrame);
    }

    if ((AUpdate == HI_TRUE) 
        && (pstAttr->bEnableAudio == HI_TRUE)
        &&(pstAttr->bEnableHdmi == HI_TRUE))
    {
        hdmi_GetInfoFrame(enHdmi, HI_INFOFRAME_TYPE_AUDIO, &stInfoFrame);
        hdmi_SetInfoFrame(enHdmi, &stInfoFrame);
    }

    //Ret = DRV_HDMI_ReadRegister((HI_U32)0x101704BC, &u32Value);
    /* Enable HDMI Ouptut */
    if (HI_TRUE == pstAttr->bEnableHdmi)
    {
        if(!SI_TX_IsHDMImode())
        {
            HI_INFO_HDMI("-->start: SI_Start_HDMITX\n");
            SI_Start_HDMITX();
            SI_TX_SetHDMIMode(ON);    //for hdmi              
        }
    #if 0
    if (0x00000001 != (u32Value & 0x00000001)){
        HI_INFO_HDMI("-->start: SI_Start_HDMITX\n");
        SI_Start_HDMITX();
        SI_TX_SetHDMIMode(ON);    //for hdmi              
    }
    #endif
    }
#if defined (DVI_SUPPORT)
    else
    {
        //if (0x00000001 == (u32Value & 0x00000001)){
        if(SI_TX_IsHDMImode())
        {   
            DEBUG_PRINTK("-->start: SI_Init_DVITX\n");
            SI_Init_DVITX();
            SI_TX_SetHDMIMode(OFF);    //for dvi
        }
    }    
#endif
#ifdef HDCP_SUPPORT
    msleep(10);
    /* Set HDMI HDCP Enable flag */
    HI_INFO_HDMI("bHDCPEnable:0x%x, bDebugFlag:0x%x\n", pstAttr->bHDCPEnable, pstAttr->bDebugFlag);
    if(g_stHdmiChnParam[HI_UNF_HDMI_ID_0].bStart == HI_TRUE)
    {
        if(pstAttr->bHDCPEnable == HI_TRUE)
            SI_WriteByteEEPROM(EE_TX_HDCP, 0xFF);
        else
            SI_WriteByteEEPROM(EE_TX_HDCP, 0x00);
    }
#endif
    msleep(60);

    /* Set HDMI Video Enable flag */
    HI_INFO_HDMI("SET hdmi video status Flag:%d\n", pstAttr->bEnableVideo); 

    if( pstAttr->bEnableVideo == HI_TRUE)
        SI_SendCP_Packet(HI_FALSE);
    else
        SI_SendCP_Packet(HI_TRUE);

    HI_INFO_HDMI("Leave hdmi_SetAttr\n");
#if defined (HDCP_SUPPORT)
    SI_timer_count();

    SI_Set_DEBUG_Flag(pstAttr->bDebugFlag);
    pstAttr->bDebugFlag &= 0x01;
#endif
    //printk("%s: 6.stAttr.enVideoFmt:%d\n",__FUNCTION__,pstHDMIAttr->stAttr.enVideoFmt);

    return Ret;
}

HI_U32 DRV_HDMI_SetAttr(HI_UNF_HDMI_ID_E enHdmi, HDMI_ATTR_S *pstAttr)
{
    HI_U32 Ret = HI_SUCCESS;

    HI_INFO_HDMI("Enter DRV_HDMI_SetAttr\n");
    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);
    HDMI_CHECK_NULL_PTR(pstAttr);

    HDMI_ATTR_LOCK();
    Ret = hdmi_SetAttr(enHdmi, pstAttr, HI_FALSE);
    HDMI_ATTR_UNLOCK();
    
    HI_INFO_HDMI("Leave DRV_HDMI_SetAttr\n");
    return Ret;
}

HI_U32 DRV_HDMI_GetAttr(HI_UNF_HDMI_ID_E enHdmi, HDMI_ATTR_S *pstAttr)
{
    HI_INFO_HDMI("Enter DRV_HDMI_GetAttr\n");
    SI_timer_count();
    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);
    HDMI_CHECK_NULL_PTR(pstAttr);    

    HDMI_ATTR_LOCK();
    g_stHdmiChnParam[enHdmi].stHDMIAttr.stAttr.bEnableVideo = HI_TRUE;
    memcpy(pstAttr, &(g_stHdmiChnParam[enHdmi].stHDMIAttr), sizeof(HDMI_ATTR_S));
    HDMI_ATTR_UNLOCK();

    HI_INFO_HDMI("Leave DRV_HDMI_GetAttr\n");
    return HI_SUCCESS;
}

#if defined (CEC_SUPPORT)
HI_U32 DRV_HDMI_CECStatus(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_CEC_STATUS_S  *pStatus)
{
    memset(pStatus, 0, sizeof(HI_UNF_HDMI_CEC_STATUS_S));

    if(HI_TRUE != g_stHdmiChnParam[0].bStart)
    {
        return HI_FAILURE;
    }

    memcpy(pStatus, &(g_stHdmiChnParam[0].stCECStatus), sizeof(HI_UNF_HDMI_CEC_STATUS_S));

    return HI_SUCCESS;
}

HI_U32 DRV_HDMI_GetCECAddress(HI_U8 *pPhyAddr, HI_U8 *pLogicalAddr)
{
    //Only invoke in private mode
    if(HI_TRUE != g_stHdmiChnParam[0].bStart)
    {
        return HI_FAILURE;
    }
    memcpy(pPhyAddr, (g_stHdmiChnParam[0].stCECStatus.u8PhysicalAddr), 4);
    *pLogicalAddr = g_stHdmiChnParam[0].stCECStatus.u8LogicalAddr;

    return HI_SUCCESS;
}

HI_U32 DRV_HDMI_SetCECCommand(HI_UNF_HDMI_ID_E enHdmi, const HI_UNF_HDMI_CEC_CMD_S  *pCECCmd)
{
    HI_U32 Ret = HI_SUCCESS;

    HI_INFO_HDMI("Enter DRV_HDMI_SetCECCommand\n");
    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);
    HDMI_CHECK_NULL_PTR(pCECCmd);  

    if(g_stHdmiChnParam[enHdmi].bCECStart != HI_TRUE)
    {
        HI_ERR_HDMI("CEC do not start\n");
        return HI_ERR_HDMI_DEV_NOT_OPEN;
    }

    if(pCECCmd->enSrcAdd != g_stHdmiChnParam[enHdmi].stCECStatus.u8LogicalAddr)
    {
        HI_ERR_HDMI("Invalid enSrcAdd:0x%x\n", pCECCmd->enSrcAdd);
        return HI_ERR_HDMI_INVALID_PARA;
    }
    Ret = SI_CEC_SendCommand((HI_UNF_HDMI_CEC_CMD_S *)pCECCmd);

    HI_INFO_HDMI("Leave DRV_HDMI_SetCECCommand\n");
    return Ret;
}

extern unsigned int  get_cec_cmd(HI_UNF_HDMI_CEC_CMD_S *rx_cmd, unsigned int num, HI_U32 timeout);
HI_U32 DRV_HDMI_GetCECCommand(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_CEC_CMD_S  *pCECCmd, HI_U32 timeout)
{
    return get_cec_cmd(pCECCmd, 1, timeout);
}
#endif

static HI_U32 hdmi_Create_AVI_Infoframe(HI_UNF_HDMI_AVI_INFOFRAME_VER2_S *punAVIInfoFrame, HI_U8 *pu8AviInfoFrame)
{
    HI_U8  u8AviInfoFrameByte = 0;
    hdmi_VideoIdentification_t *pstVidCode;
    HI_S32 retval = HI_SUCCESS;
    HI_U32 VidIdCode;

    /* HDMI AVI Infoframe is use Version = 0x02 in HDMI1.3 */

    /* Fill Data Byte 1 */
    u8AviInfoFrameByte=0;
    /* Scan information bits 0-1:S0,S1 */
    /*
       S1 S0 Scan Information
       0   0    No Data
       0   1   overscanned
       1   0   underscanned
       1   1   Future
       */
    switch(punAVIInfoFrame->enScanInfo)
    {
        case HDMI_SCAN_INFO_NO_DATA :
            u8AviInfoFrameByte |= (HI_U8)HDMI_SCAN_INFO_NO_DATA;
            break;
        case HDMI_SCAN_INFO_OVERSCANNED :
            u8AviInfoFrameByte |= (HI_U8)HDMI_SCAN_INFO_OVERSCANNED;
            break;
        case HDMI_SCAN_INFO_UNDERSCANNED :
            u8AviInfoFrameByte |= (HI_U8)HDMI_SCAN_INFO_UNDERSCANNED;
            break;
        default :
            retval = HI_FAILURE;
            break;
    }
    /* Bar Information bits 2-3:B0,B1 */
    /*
       B1 B0  Bar Info
       0   0  not valid
       0   1  Vert. Bar Info valid
       1   0  Horiz.Bar Info Valid
       1   1  Vert. and Horiz. Bar Info valid
       */
    switch (punAVIInfoFrame->enBarInfo)
    {
        case HDMI_BAR_INFO_NOT_VALID :
            u8AviInfoFrameByte |= (HI_U8) 0x00;
            break;
        case HDMI_BAR_INFO_V :
            u8AviInfoFrameByte |= (HI_U8) 0x04;
            break;
        case HDMI_BAR_INFO_H :
            u8AviInfoFrameByte |= (HI_U8) 0x08;
            break;
        case HDMI_BAR_INFO_VH :
            u8AviInfoFrameByte |= (HI_U8) 0x0C;
            break;
        default :
            retval = HI_FAILURE;
            break;
    }
    /* Active information bit 4:A0 */
    /*
       A0 Active Format Information Present
       0        No Data
       1      Active Format(R0��R3) Information valid
       */
    if (punAVIInfoFrame->bActive_Infor_Present)
    {
        u8AviInfoFrameByte |= (HI_U8)0x10;  /* Active Format Information Valid */
    }
    else
    {
        u8AviInfoFrameByte &= ~(HI_U8)0x10;  /* Active Format Information Valid */
    }
    /* Output Type bits 5-6:Y0,Y1 */
    /*
       Y1 Y0  RGB orYCbCr
       0  0   RGB (default)
       0  1   YCbCr 4:2:2
       1  0   YCbCr 4:4:4
       1  1    Future
       */
    HI_INFO_HDMI("punAVIInfoFrame->enOutputType:%d\n", punAVIInfoFrame->enOutputType);
    switch (punAVIInfoFrame->enOutputType)
    {
        case HI_UNF_HDMI_VIDEO_MODE_RGB444 :
            u8AviInfoFrameByte |= (HI_U8)0x00;
            break;
        case HI_UNF_HDMI_VIDEO_MODE_YCBCR422 :
            u8AviInfoFrameByte |= (HI_U8)0x20;
            break;
        case HI_UNF_HDMI_VIDEO_MODE_YCBCR444 :
            u8AviInfoFrameByte |= (HI_U8)0x40;
            break;
        default :
            HI_INFO_HDMI("Error Output format *******\n");            
            retval = HI_FAILURE;
            break;
    }
    pu8AviInfoFrame[0]= (HI_U8)(u8AviInfoFrameByte&0x7F);

    /* Fill Data byte 2 */
    u8AviInfoFrameByte=0;
    /* Active Format aspect ratio bits 0-3:R0...R3 */
    /*
       R3 R2 R1 R0  Active Format Aspect Ratio
       1  0  0  0   Same as picture aspect ratio
       1  0  0  1   4:3 (Center)
       1  0  1  0   16:9 (Center)
       1  0  1  1   14:9 (Center)
       */

    HI_INFO_HDMI("Active Format aspect ratio  set to 0x1000:Same as picture aspect ratio\n");
    u8AviInfoFrameByte |= (HI_U8) 0x08;

    switch (punAVIInfoFrame->enAspectRatio)
    {
        case HI_UNF_HDMI_ASPECT_RATIO_4TO3 :
            u8AviInfoFrameByte |= (HI_U8) 0x10;
            break;
        case HI_UNF_HDMI_ASPECT_RATIO_16TO9 :
            u8AviInfoFrameByte |= (HI_U8) 0x20;
            break;
        default :
            u8AviInfoFrameByte |=  (HI_U8) 0x00;
            break;
    }

    /* Colorimetry bits 6-7 of data byte2:C0,C1 */
    /*
       C1 C0    Colorim
       0   0    No Data
       0   1    SMPTE 170M[1] ITU601 [5]
       1   0    ITU709 [6] 1 0 16:9
       1   1    Extended Colorimetry Information Valid (colorimetry indicated in bits EC0, EC1,
       EC2. See Table 11)
       */
    switch (punAVIInfoFrame->enColorimetry)
    {
        case HDMI_COLORIMETRY_ITU601 :
            u8AviInfoFrameByte |= (HI_U8)0x40;
            break;
        case HDMI_COLORIMETRY_ITU709 :
            u8AviInfoFrameByte |= (HI_U8)0x80;
            break;
        case HDMI_COLORIMETRY_XVYCC_601 :
        case HDMI_COLORIMETRY_XVYCC_709 :
        case HDMI_COLORIMETRY_EXTENDED :
            u8AviInfoFrameByte |= (HI_U8)0xC0;
            break;
        default :
            u8AviInfoFrameByte |= (HI_U8)0x00;
            break;
    }
    pu8AviInfoFrame[1] = (HI_U8)(u8AviInfoFrameByte&0XFF);

    /* Fill data Byte 3: Picture Scaling bits 0-1:SC0,SC1 */
    u8AviInfoFrameByte=0;
    /*
       SC1  SC0   Non-Uniform Picture Scaling
       0     0    No Known non-uniform Scaling
       0     1    Picture has been scaled horizontally
       1     0    Picture has been scaled vertically
       1     1    Picture has been scaled horizontally and vertically
       */
    switch (punAVIInfoFrame->enPictureScaling)
    {
        case HDMI_PICTURE_NON_UNIFORM_SCALING :
            u8AviInfoFrameByte |= (HI_U8)0x00;
            break;
        case HDMI_PICTURE_SCALING_H :
            u8AviInfoFrameByte |= (HI_U8)0x01;
            break;
        case HDMI_PICTURE_SCALING_V :
            u8AviInfoFrameByte |= (HI_U8)0x02;
            break;
        case HDMI_PICTURE_SCALING_HV :
            u8AviInfoFrameByte |= (HI_U8)0x03;
            break;
        default :
            retval = HI_FAILURE;
            break;
    }
    /* Fill data Byte 3: RGB quantization range bits 2-3:Q0,Q1 */
    /*
       Q1  Q0  RGB Quantization Range
       0   0   Default (depends on video format)
       0   1   Limited Range
       1   0   Full Range
       1   1   Reserved
       */
    switch (punAVIInfoFrame->enRGBQuantization)
    {
        case HDMI_RGB_QUANTIZATION_DEFAULT_RANGE :
            u8AviInfoFrameByte |= (HI_U8)0x00;
            break;
        case HDMI_RGB_QUANTIZATION_LIMITED_RANGE :
            u8AviInfoFrameByte |= (HI_U8)0x04;
            break;
        case HDMI_RGB_QUANTIZATION_FULL_RANGE :
            u8AviInfoFrameByte |= (HI_U8)0x08;
            break;
        default :
            retval = HI_FAILURE;
            break;
    }
    /* Fill data Byte 3: Extended colorimtery range bits 4-6:EC0,EC1,EC2 */
    /*
       EC2 EC1 EC0   Extended Colorimetry
       0   0   0      xvYCC601
       0   0   1      xvYCC709
       -   -   -      All other values reserved
       */
    /*
       xvYCC601 is based on the colorimetry defined in ITU-R BT.601. 
       xvYCC709 is based on the colorimetry defined in ITU-R BT.709.
       */
    switch (punAVIInfoFrame->enColorimetry)
    {
        case HDMI_COLORIMETRY_XVYCC_601 :
            u8AviInfoFrameByte |= (HI_U8)0x00;
            break;
        case HDMI_COLORIMETRY_XVYCC_709 :
            u8AviInfoFrameByte |= (HI_U8)0x10;
            break;
        default:
            break;
    }

    /* Fill data Byte 3: IT content bit 7:ITC 
       ITC  IT content
       0    No data
       1    IT content
       */
    if (punAVIInfoFrame->bIsITContent)
        u8AviInfoFrameByte |= 0x80;
    else
        u8AviInfoFrameByte &= ~0x80;

    pu8AviInfoFrame[2] = (HI_U8)(u8AviInfoFrameByte&0XFF);


    punAVIInfoFrame->enTimingMode = DRV_HDMI_CheckVOFormat(punAVIInfoFrame->enTimingMode);

    /* Fill Data byte 4: Video indentification data Code, Bit0~7:VIC0 ~ VIC6 */
    u8AviInfoFrameByte=0;
    pstVidCode = hdmi_GetVideoCode(punAVIInfoFrame->enTimingMode);

    VidIdCode = pstVidCode->VidIdCode;
    if (HI_UNF_HDMI_ASPECT_RATIO_16TO9 == punAVIInfoFrame->enAspectRatio)
    {
        if (punAVIInfoFrame->enTimingMode == HI_DRV_DISP_FMT_480P_60)
        {
            VidIdCode = 3;
            HI_INFO_HDMI("Sepcail setting:change pstVidCode(480p_60 16:9):%d-->%d\n", pstVidCode->VidIdCode, VidIdCode);
        }
        else if (punAVIInfoFrame->enTimingMode == HI_DRV_DISP_FMT_576P_50)
        {
            VidIdCode = 18;
            HI_INFO_HDMI("Sepcail setting:change pstVidCode(576p_50 16:9):%d-->%d\n", pstVidCode->VidIdCode, VidIdCode);
        }
        else if ((punAVIInfoFrame->enTimingMode == HI_DRV_DISP_FMT_PAL)||
            (punAVIInfoFrame->enTimingMode == HI_DRV_DISP_FMT_PAL_B)||
            (punAVIInfoFrame->enTimingMode == HI_DRV_DISP_FMT_PAL_B1)||
            (punAVIInfoFrame->enTimingMode == HI_DRV_DISP_FMT_PAL_D)||
            (punAVIInfoFrame->enTimingMode == HI_DRV_DISP_FMT_PAL_D1)||
            (punAVIInfoFrame->enTimingMode == HI_DRV_DISP_FMT_PAL_G)||
            (punAVIInfoFrame->enTimingMode == HI_DRV_DISP_FMT_PAL_H)||
            (punAVIInfoFrame->enTimingMode == HI_DRV_DISP_FMT_PAL_K)||
            (punAVIInfoFrame->enTimingMode == HI_DRV_DISP_FMT_PAL_I)||
            (punAVIInfoFrame->enTimingMode == HI_DRV_DISP_FMT_PAL_M)||
            (punAVIInfoFrame->enTimingMode == HI_DRV_DISP_FMT_PAL_N)||
            (punAVIInfoFrame->enTimingMode == HI_DRV_DISP_FMT_PAL_Nc)||
            (punAVIInfoFrame->enTimingMode == HI_DRV_DISP_FMT_PAL_60)||        
            (punAVIInfoFrame->enTimingMode == HI_DRV_DISP_FMT_SECAM_SIN)||
            (punAVIInfoFrame->enTimingMode == HI_DRV_DISP_FMT_SECAM_COS)||
            (punAVIInfoFrame->enTimingMode == HI_DRV_DISP_FMT_SECAM_L)||
            (punAVIInfoFrame->enTimingMode == HI_DRV_DISP_FMT_SECAM_B)||
            (punAVIInfoFrame->enTimingMode == HI_DRV_DISP_FMT_SECAM_G)||
            (punAVIInfoFrame->enTimingMode == HI_DRV_DISP_FMT_SECAM_D)||
            (punAVIInfoFrame->enTimingMode == HI_DRV_DISP_FMT_SECAM_K)||
            (punAVIInfoFrame->enTimingMode == HI_DRV_DISP_FMT_SECAM_H)) 
        {
            VidIdCode = 22;
            HI_INFO_HDMI("Sepcail setting:change pstVidCode(576i_50 16:9):%d-->%d\n", pstVidCode->VidIdCode, VidIdCode);
        }
        else if ((punAVIInfoFrame->enTimingMode == HI_DRV_DISP_FMT_NTSC)||
            (punAVIInfoFrame->enTimingMode == HI_DRV_DISP_FMT_NTSC_J)||
            (punAVIInfoFrame->enTimingMode == HI_DRV_DISP_FMT_NTSC_443))
        {
            VidIdCode = 7;
            HI_INFO_HDMI("Sepcail setting:change pstVidCode(480i_60 16:9):%d-->%d\n", pstVidCode->VidIdCode, VidIdCode);
        }
        else
        {
            HI_INFO_HDMI("do not need to change VIC\n");
        }
    }

    pu8AviInfoFrame[3] = (HI_U8)(VidIdCode & 0x7F);
    /* Fill Data byte 5: Pixel repetition, Bit0~3:PR0~PR3 */
    /*
       PR3 PR2 PR1 PR0 Pixel Repetition Factor
       0   0   0    0   No Repetition (i.e., pixel sent once)
       0   0   0    1   pixel sent 2 times (i.e., repeated once)
       0   0   1    0   pixel sent 3 times
       0   0   1    1   pixel sent 4 times
       0   1   0    0   pixel sent 5 times
       0   1   0    1   pixel sent 6 times
       0   1   1    0   pixel sent 7 times
       0   1   1    1   pixel sent 8 times
       1   0   0    0   pixel sent 9 times
       1   0   0    1   pixel sent 10 times
       0Ah-0Fh          Reserved
       */
    u8AviInfoFrameByte = (HI_U8)(punAVIInfoFrame->u32PixelRepetition& 0x0F);

    /* Fill Data byte 5: Content Type, Bit4~5:CN0~CN1 */
    /*
       ITC  CN1 CN0 Pixel Repetition Factor
       (1)   0    0   Graphics
       (1)   0    1   Photo
       (1)   1    0   Cinema
       (1)   1    1   Game
       */
    switch (punAVIInfoFrame->enContentType)
    {
        case HDMI_CONTNET_GRAPHIC:
            u8AviInfoFrameByte |= (HI_U8)0x00;
            break;
        case HDMI_CONTNET_PHOTO:
            u8AviInfoFrameByte |= (HI_U8)0x10;
            break;
        case HDMI_CONTNET_CINEMA:
            u8AviInfoFrameByte |= (HI_U8)0x20;
            break;
        case HDMI_CONTNET_GAME:
            u8AviInfoFrameByte |= (HI_U8)0x30;
            break;
        default:
            u8AviInfoFrameByte |= (HI_U8)0x00;
            break;
    }
    /* Fill Data byte 5: YCC Full Range, Bit6~7:YQ0~YQ1 */
    /*
       YQ1 YQ0 Pixel Repetition Factor
       0    0   Limitation Range
       0    1   Full Range
       */
    switch (punAVIInfoFrame->enYCCQuantization)
    {
        case HDMI_YCC_QUANTIZATION_LIMITED_RANGE:
            u8AviInfoFrameByte |= (HI_U8)0x00;
            break;
        case HDMI_YCC_QUANTIZATION_FULL_RANGE:
            u8AviInfoFrameByte |= (HI_U8)0x40;
            break;
        default:
            u8AviInfoFrameByte |= (HI_U8)0x00;
            break;
    }
    pu8AviInfoFrame[4]= (HI_U8)(u8AviInfoFrameByte&0XFF);

    if ( (0 == punAVIInfoFrame->u32LineNEndofTopBar) && (0 == punAVIInfoFrame->u32LineNStartofBotBar)
        && (0 == punAVIInfoFrame->u32PixelNEndofLeftBar) && (0 == punAVIInfoFrame->u32PixelNStartofRightBar) )
    {
        punAVIInfoFrame->u32LineNEndofTopBar      = pstVidCode->Active_X;
        punAVIInfoFrame->u32LineNStartofBotBar    = pstVidCode->Active_H;
        punAVIInfoFrame->u32PixelNEndofLeftBar    = pstVidCode->Active_Y;
        punAVIInfoFrame->u32PixelNStartofRightBar = pstVidCode->Active_W;
    }
    if(punAVIInfoFrame->enBarInfo == HDMI_BAR_INFO_NOT_VALID)
    {
        punAVIInfoFrame->u32LineNEndofTopBar      = 0;
        punAVIInfoFrame->u32LineNStartofBotBar    = 0;
        punAVIInfoFrame->u32PixelNEndofLeftBar    = 0;
        punAVIInfoFrame->u32PixelNStartofRightBar = 0;
    }
    /* Fill Data byte 6  */
    pu8AviInfoFrame[5] = (HI_U8)(punAVIInfoFrame->u32LineNEndofTopBar & 0XFF);

    /* Fill Data byte 7  */
    pu8AviInfoFrame[6] = (HI_U8)((punAVIInfoFrame->u32LineNEndofTopBar>>8) & 0XFF);

    /* Fill Data byte 8  */
    pu8AviInfoFrame[7] = (HI_U8)(punAVIInfoFrame->u32LineNStartofBotBar & 0XFF);

    /* Fill Data byte 9  */
    pu8AviInfoFrame[8] = (HI_U8)((punAVIInfoFrame->u32LineNStartofBotBar>>8) & 0XFF);

    /* Fill Data byte 10  */
    pu8AviInfoFrame[9] = (HI_U8)(punAVIInfoFrame->u32PixelNEndofLeftBar &0XFF);

    /* Fill Data byte 11  */
    pu8AviInfoFrame[10] = (HI_U8)((punAVIInfoFrame->u32PixelNEndofLeftBar>>8) &0XFF);

    /* Fill Data byte 12  */
    pu8AviInfoFrame[11] = (HI_U8)(punAVIInfoFrame->u32PixelNStartofRightBar &0XFF);

    /* Fill Data byte 13  */
    pu8AviInfoFrame[12] = (HI_U8)((punAVIInfoFrame->u32PixelNStartofRightBar>>8) &0XFF);

    return retval;
}

static HI_U32 hdmi_Create_Audio_Infoframe(HI_UNF_HDMI_AUD_INFOFRAME_VER1_S *punAUDInfoFrame, HI_U8 *pu8AudioInfoFrame)
{
    HI_U8 u8AudioInfoFrameByte=0;
    HI_S32 retval = HI_SUCCESS;
    u8AudioInfoFrameByte=0;

    switch (punAUDInfoFrame->u32ChannelCount)
    {
        case 2 :
            u8AudioInfoFrameByte |= 0x01;
            break;
        case 3 :
            u8AudioInfoFrameByte |= 0x02;
            break;
        case 4 :
            u8AudioInfoFrameByte |= 0x03;
            break;
        case 5 :
            u8AudioInfoFrameByte |= 0x04;
            break;
        case 6 :
            u8AudioInfoFrameByte |= 0x05;
            break;
        case 7 :
            u8AudioInfoFrameByte |= 0x06;
            break;
        case 8 :
            u8AudioInfoFrameByte |= 0x07;
            break;
        default :
            u8AudioInfoFrameByte |= 0x00;
            break;
    }

    switch (punAUDInfoFrame->enCodingType)
    {
        case HDMI_AUDIO_CODING_PCM :
            u8AudioInfoFrameByte |= 0x10;
            break;
        case HDMI_AUDIO_CODING_AC3 :
            u8AudioInfoFrameByte |= 0x20;
            break;
        case HDMI_AUDIO_CODING_MPEG1 :
            u8AudioInfoFrameByte |= 0x30;
            break;
        case HDMI_AUDIO_CODING_MP3 :
            u8AudioInfoFrameByte|= 0x40;
            break;
        case HDMI_AUDIO_CODING_MPEG2 :
            u8AudioInfoFrameByte |= 0x50;
            break;
        case HDMI_AUDIO_CODING_AAC :
            u8AudioInfoFrameByte |= 0x60;
            break;
        case HDMI_AUDIO_CODING_DTS :
            u8AudioInfoFrameByte |= 0x70;
            break;
        case HDMI_AUDIO_CODING_DDPLUS :
            u8AudioInfoFrameByte |= 0xA0;
            break;
        case HDMI_AUDIO_CODING_MLP :
            u8AudioInfoFrameByte |= 0xC0;
            break;
        case HDMI_AUDIO_CODING_WMA :
            u8AudioInfoFrameByte |= 0xE0;
            break;
        default :
            u8AudioInfoFrameByte |= 0x00;
            break;
    }
    pu8AudioInfoFrame[0] = (HI_U8)(u8AudioInfoFrameByte& 0xF7);

    u8AudioInfoFrameByte=0;
    /* Fill Sample Size (Data Byte 2) bits2: 0~1*/
    /*
       SS1 SS0    Sample Size
       0   0      Refer to Stream header
       0   1      16 bit
       1   0      20 bit
       1   1      24 bit
       */
    switch (punAUDInfoFrame->u32SampleSize)
    {
        case 16 :
            u8AudioInfoFrameByte |= 0x01;
            break;
        case 20 :
            u8AudioInfoFrameByte |= 0x02;
            break;
        case 24 :
            u8AudioInfoFrameByte |= 0x03;
            break;
        default :
            u8AudioInfoFrameByte |= 0x00;
            break;
    }

    /* Fill Sample Frequency (Data Byte 2)bits3: 2~4*/
    /*
       SF2 SF1 SF0 Sampling Frequency
       0   0   0   Refer to Stream Header
       0   0   1   32 kHz
       0   1   0   44.1 kHz (CD)
       0   1   1   48 kHz
       1   0   0   88.2 kHz
       1   0   1   96 kHz
       1   1   0   176.4 kHz
       1   1   1   192 kHz
       */
    switch (punAUDInfoFrame->u32SamplingFrequency)
    {
        case 32000 :
            u8AudioInfoFrameByte |= 0x04;
            break;
        case 44100 :
            u8AudioInfoFrameByte |= 0x08;
            break;
        case 48000 :
            u8AudioInfoFrameByte |= 0x0C;
            break;
        case 88200 :
            u8AudioInfoFrameByte |= 0x10;
            break;
        case 96000 :
            u8AudioInfoFrameByte |= 0x14;
            break;
        case 176400 :
            u8AudioInfoFrameByte |= 0x18;
            break;
        case 192000 :
            u8AudioInfoFrameByte |= 0x1C;
            break;
        default :
            u8AudioInfoFrameByte |= 0x00;
            break;
    }
    pu8AudioInfoFrame[1] = (HI_U8)(u8AudioInfoFrameByte& 0x1F);

    u8AudioInfoFrameByte=0;
    /* Fill the Bit rate coefficient for the compressed audio format (Data byte 3)*/
    switch (punAUDInfoFrame->enCodingType)
    {
        case HDMI_AUDIO_CODING_AC3 :
        case HDMI_AUDIO_CODING_DTS :
        case HDMI_AUDIO_CODING_MPEG1 :
        case HDMI_AUDIO_CODING_MPEG2 :
        case HDMI_AUDIO_CODING_MP3 :
        case HDMI_AUDIO_CODING_AAC :
            pu8AudioInfoFrame[2] = (HI_U8)0XFF;//? Data Byte 3 is reserved and shall be zero.
            break;
        case HDMI_AUDIO_CODING_PCM :
        default :
            pu8AudioInfoFrame[2] = 0X00;
            break;
    }

    /* Data Bytes 4 and 5 apply only to multi-channel (i.e., more than two channels) uncompressed audio. */
    /* Fill Channel allocation (Data Byte 4) */
    /*
       CA(binary)       CA(hex)  Channel Number
       7 6 5 4 3 2 1 0          8 7 6 5 4 3 2 1
       0 0 0 0 0 0 0 0  00      - - - - - - FR FL
       0 0 0 0 0 0 0 1  01      - - - - - LFE FR FL
       0 0 0 0 0 0 1 0  02      - - - - FC - FR FL
       0 0 0 0 0 0 1 1  03      - - - - FC LFE FR FL
       0 0 0 0 0 1 0 0  04      - - - RC - - FR FL
       0 0 0 0 0 1 0 1  05      - - - RC - LFE FR FL
       0 0 0 0 0 1 1 0  06      - - - RC FC - FR FL
       0 0 0 0 0 1 1 1  07      - - - RC FC LFE FR FL
       0 0 0 0 1 0 0 0  08      - - RR RL - - FR FL
       0 0 0 0 1 0 0 1  09      - - RR RL - LFE FR FL
       0 0 0 0 1 0 1 0  0A      - - RR RL FC - FR FL
       0 0 0 0 1 0 1 1  0B      - - RR RL FC LFE FR FL
       0 0 0 0 1 1 0 0  0C      - RC RR RL - - FR FL
       0 0 0 0 1 1 0 1  0D      - RC RR RL - LFE FR FL
       0 0 0 0 1 1 1 0  0E      - RC RR RL FC - FR FL
       0 0 0 0 1 1 1 1  0F      - RC RR RL FC LFE FR FL
       0 0 0 1 0 0 0 0  10      RRC RLC RR RL - - FR FL
       0 0 0 1 0 0 0 1  11      RRC RLC RR RL - LFE FR FL
       0 0 0 1 0 0 1 0  12      RRC RLC RR RL FC - FR FL
       0 0 0 1 0 0 1 1  13      RRC RLC RR RL FC LFE FR FL
       0 0 0 1 0 1 0 0  14      FRC FLC - - - - FR FL
       0 0 0 1 0 1 0 1  15      FRC FLC - - - LFE FR FL
       0 0 0 1 0 1 1 0  16      FRC FLC - - FC - FR FL
       0 0 0 1 0 1 1 1  17      FRC FLC - - FC LFE FR FL
       0 0 0 1 1 0 0 0  18      FRC FLC - RC - - FR FL
       0 0 0 1 1 0 0 1  19      FRC FLC - RC - LFE FR FL
       0 0 0 1 1 0 1 0  1A      FRC FLC - RC FC - FR FL
       0 0 0 1 1 0 1 1  1B      FRC FLC - RC FC LFE FR FL
       0 0 0 1 1 1 0 0  1C      FRC FLC RR RL - - FR FL
       0 0 0 1 1 1 0 1  1D      FRC FLC RR RL - LFE FR FL
       0 0 0 1 1 1 1 0  1E      FRC FLC RR RL FC - FR FL
       0 0 0 1 1 1 1 1  1F      FRC FLC RR RL FC LFE FR FL
       */
    pu8AudioInfoFrame[3] = (HI_U8)(punAUDInfoFrame->u32ChannelAlloc &0XFF);

    /* Fill Level Shift (Data Byte 5) bits4:3~7 */
    /*
       LSV3 LSV2 LSV1 LSV0 Level Shift Value
       0     0    0    0     0dB
       0     0    0    1     1dB
       0     0    1    0     2dB
       0     0    1    1     3dB
       0     1    0    0     4dB
       0     1    0    1     5dB
       0     1    1    0     6dB
       0     1    1    1     7dB
       1     0    0    0     8dB
       1     0    0    1     9dB
       1     0    1    0    10dB
       1     0    1    1    11dB
       1     1    0    0    12dB
       1     1    0    1    13dB
       1     1    1    0    14dB
       1     1    1    1    15dB    
       */
    switch (punAUDInfoFrame->u32LevelShift)
    {
        case 0 :
            u8AudioInfoFrameByte |= 0x00;
            break;
        case 1 :
            u8AudioInfoFrameByte |= 0x08;
            break;
        case 2 :
            u8AudioInfoFrameByte |= 0x10;
            break;
        case 3 :
            u8AudioInfoFrameByte |= 0x18;
            break;
        case 4 :
            u8AudioInfoFrameByte |= 0x20;
            break;
        case 5 :
            u8AudioInfoFrameByte |= 0x28;
            break;
        case 6 :
            u8AudioInfoFrameByte |= 0x30;
            break;
        case 7 :
            u8AudioInfoFrameByte |= 0x38;
            break;
        case 8 :
            u8AudioInfoFrameByte |= 0x40;
            break;
        case 9 :
            u8AudioInfoFrameByte |= 0x48;
            break;
        case 10 :
            u8AudioInfoFrameByte |= 0x50;
            break;
        case 11 :
            u8AudioInfoFrameByte |= 0x58;
            break;
        case 12 :
            u8AudioInfoFrameByte |= 0x60;
            break;
        case 13 :
            u8AudioInfoFrameByte |= 0x68;
            break;
        case 14 :
            u8AudioInfoFrameByte |= 0x70;
            break;
        case 15 :
            u8AudioInfoFrameByte |= 0x78;
            break;
        default :
            retval = HI_FAILURE;
            break;
    }
    /* Fill Down mix inhibit flag bit7 */
    if (punAUDInfoFrame->u32DownmixInhibit)
    {
        u8AudioInfoFrameByte |= 0x80;
    }
    else
    {
        u8AudioInfoFrameByte &= ~0x80;
    }
    pu8AudioInfoFrame[4] = (HI_U8)(u8AudioInfoFrameByte&0xF8);

    return retval;
}

static HI_U32 hdmi_SetInfoFrame(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_INFOFRAME_S *pstInfoFrame)
{
    HI_S32 siRet = HI_SUCCESS;
    

    HDMI_CHECK_NULL_PTR(pstInfoFrame);

    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);
    //printk("\n\n~~~~~~~~~~~~~info 0~~~~~~~~~~\n\n");
    switch(pstInfoFrame->enInfoFrameType)
    {
        case HI_INFOFRAME_TYPE_AVI:
        {
            /*The InfoFrame provided by HDMI is limited to 30 bytes plus a checksum byte.*/
            HI_U8 pu8AviInfoFrame[32];
             //printk("\n\n~~~~~~~~~~~~~info 2~~~~~~~~~~\n\n");
            memset(pu8AviInfoFrame, 0, 32);
            siRet = hdmi_Create_AVI_Infoframe((HI_UNF_HDMI_AVI_INFOFRAME_VER2_S *)&(pstInfoFrame->unInforUnit.stAVIInfoFrame), pu8AviInfoFrame);
            memcpy(&(g_stHdmiChnParam[enHdmi].stAVIInfoFrame), &(pstInfoFrame->unInforUnit.stAVIInfoFrame), sizeof(HI_UNF_HDMI_AVI_INFOFRAME_VER2_S));

            if (HI_TRUE == g_stHdmiCommParam.bOpenGreenChannel)
            {
                //printk("%s.%d \n",__FUNCTION__,__LINE__);
                return HI_SUCCESS;
            }
            //printk("\n\n~~~~~~~~~~~~~info 3~~~~~~~~~~\n\n");
            /* Set relative Register in HDMI IP */
            SI_DisableInfoFrame(AVI_TYPE);
            /* default AVI Infoframe: DefaultAVIInfoFrame[0xd] in eeprom.c*/
            SI_BlockWriteEEPROM( 13, EE_TXAVIINFO_ADDR, pu8AviInfoFrame);
            SI_SendAVIInfoFrame();
            SI_EnableInfoFrame(AVI_TYPE);
            siRet = HI_SUCCESS;
            break;
        }
        case HI_INFOFRAME_TYPE_SPD:
            break;
        case HI_INFOFRAME_TYPE_AUDIO:
        {
            HI_U8 pu8AudioInfoFrame[32];
            memset(pu8AudioInfoFrame, 0, 32);
            hdmi_Create_Audio_Infoframe((HI_UNF_HDMI_AUD_INFOFRAME_VER1_S *)&(pstInfoFrame->unInforUnit.stAUDInfoFrame), pu8AudioInfoFrame);
            memcpy(&(g_stHdmiChnParam[enHdmi].stAUDInfoFrame), &(pstInfoFrame->unInforUnit.stAUDInfoFrame), sizeof(HI_UNF_HDMI_AUD_INFOFRAME_VER1_S));
            //SI_SetHdmiAudio(HI_FALSE);
            SI_DisableInfoFrame(AUD_TYPE);
            SI_Set_AudioInfoFramePacket (pu8AudioInfoFrame, 0,  0);
            SI_EnableInfoFrame(AUD_TYPE);
            SI_SetHdmiAudio(HI_TRUE);

            break;
        }
        case HI_INFOFRAME_TYPE_MPEG:
            break;
        case HI_INFOFRAME_TYPE_VENDORSPEC:
            break;
        default:
            break;
    }    
    return siRet;
} 

static HI_U32 hdmi_GetInfoFrame(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_INFOFRAME_TYPE_E enInfoFrameType, HI_UNF_HDMI_INFOFRAME_S *pstInfoFrame)
{
    HI_S32 siRet = HI_SUCCESS;

    HDMI_CHECK_NULL_PTR(pstInfoFrame);

    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);

    memset(pstInfoFrame, 0, sizeof(HI_UNF_HDMI_INFOFRAME_S));
    switch(enInfoFrameType)
    {
        case HI_INFOFRAME_TYPE_AVI:
        {
            pstInfoFrame->enInfoFrameType = HI_INFOFRAME_TYPE_AVI;
            memcpy(&(pstInfoFrame->unInforUnit.stAVIInfoFrame), &(g_stHdmiChnParam[enHdmi].stAVIInfoFrame), sizeof(HI_UNF_HDMI_AVI_INFOFRAME_VER2_S));
            break;
        }
        case HI_INFOFRAME_TYPE_AUDIO:
        {
            pstInfoFrame->enInfoFrameType = HI_INFOFRAME_TYPE_AUDIO;
            memcpy(&(pstInfoFrame->unInforUnit.stAUDInfoFrame), &(g_stHdmiChnParam[enHdmi].stAUDInfoFrame), sizeof(HI_UNF_HDMI_AUD_INFOFRAME_VER1_S));
            break;
        }
        default:
            siRet = HI_FAILURE;
            break;
    }

    return siRet;
}

HI_U32 DRV_HDMI_SetInfoFrame(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_INFOFRAME_S *pstInfoFrame)
{
    HI_U32 Ret;
    HI_INFO_HDMI("Enter DRV_HDMI_SetInfoFrame\n");
    SI_timer_count();
    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);
    HDMI_CHECK_NULL_PTR(pstInfoFrame);    

    
    if(pstInfoFrame->unInforUnit.stAVIInfoFrame.enOutputType == HI_UNF_HDMI_VIDEO_MODE_YCBCR422)
    {
        //printk("%s.%d : SetInfoFrame YCBCR422 return \n",__FUNCTION__,__LINE__);
        return HI_ERR_HDMI_INVALID_PARA;
    }

    Ret = hdmi_SetInfoFrame(enHdmi, pstInfoFrame);
    SI_timer_count();
    HI_INFO_HDMI("Leave DRV_HDMI_SetInfoFrame\n");
    return HI_SUCCESS;
}

HI_U32 DRV_HDMI_GetInfoFrame(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_INFOFRAME_TYPE_E enInfoFrameType, HI_UNF_HDMI_INFOFRAME_S *pstInfoFrame)
{
    HI_U32 Ret;
    HI_INFO_HDMI("Enter DRV_HDMI_GetInfoFrame\n");
    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);
    HDMI_CHECK_NULL_PTR(pstInfoFrame);

    Ret = hdmi_GetInfoFrame(enHdmi, enInfoFrameType, pstInfoFrame);

    HI_INFO_HDMI("Leave DRV_HDMI_GetInfoFrame\n");
    return HI_SUCCESS;
}
#if defined (HDCP_SUPPORT)
static HI_U32 HDCP_FailCount = 0;
#endif

#ifdef DEBUG_NOTIFY_COUNT
HI_U32 hpd_changeCount = 0;
#endif
HI_U32 DRV_HDMI_ReadEvent(HI_UNF_HDMI_ID_E enHdmi,HI_U32 procID)
{
    HI_S32 s32Ret=-1;
    HI_U32 index, event = 0;
    HI_U32 u32EventNo = g_stHdmiChnParam[HI_UNF_HDMI_ID_0].eventList[procID].CurEventNo;

    if(procID >= MAX_PROCESS_NUM)
    {
        HI_ERR_HDMI("Invalid procID in ReadEvent\n");
        return HI_UNF_HDMI_EVENT_BUTT;
    }

    //We deal with HDMI Event only after HDMI opened!
    if (g_stHdmiChnParam[enHdmi].bOpen)
    {
        //DEBUG_PRINTK("msecs_to_jiffies(100):%d\n", msecs_to_jiffies(100));

        //s32Ret = wait_event_interruptible_timeout(g_astHDMIWait, g_HDMIWaitFlag, (HI_U32)msecs_to_jiffies(100));
        s32Ret = wait_event_interruptible_timeout(g_astHDMIWait, g_HDMIWaitFlag[procID], (HI_U32)msecs_to_jiffies(10));
        if (s32Ret <= 0)
        {
            return 0;
        }

        if(g_Event_Count[procID] > MIX_EVENT_COUNT)
        {
            g_Event_Count[procID]--;
        }
        else
        {
            g_HDMIWaitFlag[procID] = HI_FALSE;
        }

        HI_INFO_HDMI("Ha we get a event!!!!!!\n");
        SI_timer_count();

#ifdef DEBUG_EVENTLIST
        HI_WARN_HDMI("read start\n");
        HI_WARN_HDMI("\n procID %d CurEventNo %d \n",procID,g_stHdmiChnParam[HI_UNF_HDMI_ID_0].eventList[procID].CurEventNo);
        for(index = 0;index < PROC_EVENT_NUM;index++)
        {
            HI_WARN_HDMI("____eventlist %d: 0x%x_____\n",index, g_stHdmiChnParam[HI_UNF_HDMI_ID_0].eventList[procID].Event[index]);
        }
#endif
        
        HDMI_EVENT_LOCK();
        event = 0;
        for(index = 0; index < PROC_EVENT_NUM; index ++)
        {
            
            if ((g_stHdmiChnParam[HI_UNF_HDMI_ID_0].eventList[procID].Event[u32EventNo] >= HI_UNF_HDMI_EVENT_HOTPLUG)
                && (g_stHdmiChnParam[HI_UNF_HDMI_ID_0].eventList[procID].Event[u32EventNo] <=HI_UNF_HDMI_EVENT_RSEN_DISCONNECT))
            {
                event = g_stHdmiChnParam[HI_UNF_HDMI_ID_0].eventList[procID].Event[u32EventNo];
                g_stHdmiChnParam[HI_UNF_HDMI_ID_0].eventList[procID].Event[u32EventNo] = 0;
                if(event == HI_UNF_HDMI_EVENT_HOTPLUG)
                {
                    if (HI_TRUE == SI_Is_HPDKernelCallback_DetectHPD())
                    {
                    #ifdef DEBUG_NOTIFY_COUNT
                        hpd_changeCount++;
                        printk("____SI_Is_HPDKernelCallback_DetectHPD %d times____\n",hpd_changeCount);
                    #endif
                        event = 0;
                    }
                    break;
                }
#if defined (HDCP_SUPPORT)
                else if(event == HI_UNF_HDMI_EVENT_HDCP_FAIL)
                {
                    HDCP_FailCount ++;
                    if (HDCP_FailCount >= 50)
                    {
                        HDCP_FailCount = 0;
                        HI_ERR_HDMI("HDCP Authentication Fail times:50!\n");
                    }
                    break;
                }
#endif
                else
                {
                    HI_WARN_HDMI("event:%d\n",event);
                    break;
                }
            }
            u32EventNo = (u32EventNo + 1) % PROC_EVENT_NUM;
#ifdef DEBUG_EVENTLIST
            HI_WARN_HDMI("u32EventNo : %d \n",u32EventNo);
#endif
        }
        HDMI_EVENT_UNLOCK();
    }
    HI_INFO_HDMI("line:%d,event:%d\n",__LINE__,event);

#ifdef DEBUG_EVENTLIST
    HI_WARN_HDMI("read over\n");
    HI_WARN_HDMI("\n procID %d CurEventNo %d \n",procID,g_stHdmiChnParam[HI_UNF_HDMI_ID_0].eventList[procID].CurEventNo);
    for(index = 0;index < PROC_EVENT_NUM;index++)
    {
        HI_WARN_HDMI("____eventlist %d: 0x%x_____\n",index, g_stHdmiChnParam[HI_UNF_HDMI_ID_0].eventList[procID].Event[index]);
    }
#endif
    
    return event;
}

extern void hdmi_MCE_ProcHotPlug(HI_HANDLE hHdmi);

static void hdmi_ProcEvent(HI_UNF_HDMI_EVENT_TYPE_E event,HI_U32 procID)
{
    HI_INFO_HDMI("line:%d,event:%d,g_UserCallbackFlag:%d\n",__LINE__,event,g_UserCallbackFlag);
    
    if(g_UserCallbackFlag == HDMI_CALLBACK_USER) //app
    {
        HI_U32 u32CurEvent = g_stHdmiChnParam[HI_UNF_HDMI_ID_0].eventList[procID].CurEventNo;
#ifdef DEBUG_EVENTLIST
        int i;
#endif
        HDMI_EVENT_LOCK();
        g_Event_Count[procID]++;
        
        #if 1    
      
        g_stHdmiChnParam[HI_UNF_HDMI_ID_0].eventList[procID].Event[u32CurEvent] = event;
        g_stHdmiChnParam[HI_UNF_HDMI_ID_0].eventList[procID].CurEventNo = (u32CurEvent + 1) % PROC_EVENT_NUM;
        
#ifdef DEBUG_EVENTLIST
        HI_WARN_HDMI("\n procID %d CurEventNo %d \n",procID,g_stHdmiChnParam[HI_UNF_HDMI_ID_0].eventList[procID].CurEventNo);
        for(i = 0;i < PROC_EVENT_NUM;i++)
        {
            HI_WARN_HDMI("____eventlist %d: 0x%x_____\n",i, g_stHdmiChnParam[HI_UNF_HDMI_ID_0].eventList[procID].Event[i]);
        }
#endif
        #else
        //ԭ�з���������̶��߳��£�Event[0]λ�� ʱ��ʱ��ʧ��Ϣ
        if(event == HI_UNF_HDMI_EVENT_RSEN_CONNECT)
        {
            g_stHdmiChnParam[HI_UNF_HDMI_ID_0].eventList[procID].Event[1] = event;
        }
        else if(event == HI_UNF_HDMI_EVENT_RSEN_DISCONNECT)
        {
            g_stHdmiChnParam[HI_UNF_HDMI_ID_0].eventList[procID].Event[2] = event;
        }
        else
        {
            g_stHdmiChnParam[HI_UNF_HDMI_ID_0].eventList[procID].Event[0] = event;
        }
        HI_INFO_HDMI("line:%d,event:%d\n",__LINE__,event);

        #endif
        HDMI_EVENT_UNLOCK();
        g_HDMIWaitFlag[procID] = HI_TRUE;
        wake_up(&g_astHDMIWait);
        HI_INFO_HDMI("callback finish wake up event g_HDMIWaitFlag:0x%x\n", g_HDMIWaitFlag[procID]);
    }
    else if(g_UserCallbackFlag == HDMI_CALLBACK_KERNEL)//mce
    {
        switch ( event )
        {
            case HI_UNF_HDMI_EVENT_HOTPLUG:               
                hdmi_MCE_ProcHotPlug(HI_UNF_HDMI_ID_0);
                break;
#if defined (HDCP_SUPPORT)
            case HI_UNF_HDMI_EVENT_HDCP_SUCCESS:
                DEBUG_PRINTK("\n MCE HDMI event: HDCP_SUCCESS!\n");
                break;
#endif
            default:
                break;
        }
    }
    return;
}

#ifdef DEBUG_NOTIFY_COUNT
HI_U32 NotifyCount = 0;
#endif
void DRV_HDMI_NotifyEvent(HI_UNF_HDMI_EVENT_TYPE_E event)
{
    HI_U32 u32ProcIndex = 0;
    
#ifdef DEBUG_NOTIFY_COUNT
    if(event != HI_UNF_HDMI_EVENT_HDCP_USERSETTING)
    {
        NotifyCount++;
        HI_INFO_HDMI("\n **** Notify %d times : event 0x%x **** \n",NotifyCount,event);
    }
#endif
    
    HI_INFO_HDMI("HDMI EVENT TYPE:0x%x\n", event);
    if (g_stHdmiChnParam[0].bOpen)
    {
        if ((event == HI_UNF_HDMI_EVENT_HOTPLUG))
        {  
            #if 0
            HI_INFO_HDMI("set Interrupts bit\n");
            // Enable Interrupts: VSync, Ri check, HotPlug
            WriteByteHDMITXP0( HDMI_INT_ADDR, CLR_MASK);
            WriteByteHDMITXP0( HDMI_INT_MASK_ADDR, CLR_MASK);
            #endif 
            SI_EnableInterrupts();
#if defined (CEC_SUPPORT)
    #if 0 
            /* Enable CEC_SETUP */
            HI_INFO_HDMI("set CEC_SETUP\n");
            WriteByteHDMICEC(0X8E, 0x04);
    #endif
            SI_CEC_SetUp();
#endif
            SI_HPD_SetHPDUserCallbackCount();
            hdmi_SetAndroidState(STATE_HOTPLUGIN);
        }
#if defined (HDCP_SUPPORT)
        else if (event == HI_UNF_HDMI_EVENT_HDCP_USERSETTING)
        {
            //special doing!!
            //DRV_HDMI_Start(HI_UNF_HDMI_ID_0);            
            return;
        }
#endif
        else if (event == HI_UNF_HDMI_EVENT_NO_PLUG)
        {
            if (g_HDMIUserInitNum)
            {
                /* Close HDMI Output */
                SI_PowerDownHdmiTx();
                SI_DisableHdmiDevice();
                g_stHdmiChnParam[0].bStart = HI_FALSE;
#if defined (HDCP_SUPPORT)
                /*Set HDCP Off */
                SI_WriteByteEEPROM(EE_TX_HDCP, 0x00);
#endif
#if defined (CEC_SUPPORT)
                //Close CEC
                SI_CEC_Close();
                g_stHdmiChnParam[0].bCECStart       = HI_FALSE;
                g_stHdmiChnParam[0].u8CECCheckCount = 0;
                memset(&(g_stHdmiChnParam[0].stCECStatus), 0, sizeof(HI_UNF_HDMI_CEC_STATUS_S));           
#endif
            }
            hdmi_SetAndroidState(STATE_HOTPLUGOUT);
        }
        else if (event == HI_UNF_HDMI_EVENT_EDID_FAIL)
        {
        }
#if defined (HDCP_SUPPORT)
        else if (event == HI_UNF_HDMI_EVENT_HDCP_FAIL)
        {

        }
        else if (event == HI_UNF_HDMI_EVENT_HDCP_SUCCESS)
        {
            SI_timer_stop();
        }
#endif
        else if (event == HI_UNF_HDMI_EVENT_RSEN_CONNECT)
        {
            HI_INFO_HDMI("CONNECT Event:0x%x\n", event);
        }
        else if (event == HI_UNF_HDMI_EVENT_RSEN_DISCONNECT)
        {
            HI_INFO_HDMI("DISCONNECT Event:0x%x\n", event);
        }
        else
        {
            HI_INFO_HDMI("Unknow Event:0x%x\n", event);
        }
        HI_INFO_HDMI("line:%d,event:%d\n",__LINE__,event);
        for(u32ProcIndex = 0; u32ProcIndex < MAX_PROCESS_NUM; u32ProcIndex++)
        {           
            if(HI_TRUE == g_stHdmiChnParam[HI_UNF_HDMI_ID_0].eventList[u32ProcIndex].bUsed)
            {
                 HI_INFO_HDMI("proc id %d bUsed %d\n",u32ProcIndex,g_stHdmiChnParam[HI_UNF_HDMI_ID_0].eventList[u32ProcIndex].bUsed);
                hdmi_ProcEvent(event, u32ProcIndex);
            }            
        }

        SI_timer_count();
    }
}

HI_U32 DRV_HDMI_Start(HI_UNF_HDMI_ID_E enHdmi)
{
    HI_UNF_HDMI_ATTR_S *pstAttr;

    HI_INFO_HDMI("Enter DRV_HDMI_Start\n");
    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);

    SI_timer_count();

    pstAttr = &(g_stHdmiChnParam[enHdmi].stHDMIAttr.stAttr);

    if (HI_TRUE == g_stHdmiCommParam.bOpenGreenChannel)
    {
        HI_INFO_HDMI("HDMI release green channel\n");
        g_stHdmiCommParam.bOpenGreenChannel = HI_FALSE;
        SI_EnableHdmiDevice();  //Force to enable HDMI PHY
    }
    else
    {
        
        /* Enable HDMI Ouptut */
        if (HI_TRUE == pstAttr->bEnableHdmi)
        {
            HI_INFO_HDMI("-->start: SI_Start_HDMITX\n");
            SI_Start_HDMITX();
            SI_TX_SetHDMIMode(ON);    //for hdmi
        }
#if defined (DVI_SUPPORT)
        else
        {
            DEBUG_PRINTK("-->start: SI_Init_DVITX\n");
            SI_Init_DVITX();
            SI_TX_SetHDMIMode(OFF);    //for dvi
        }
#endif
        SI_timer_count();
        HI_INFO_HDMI("Before TX_SYS_CTRL1_ADDR:0x%x\n", ReadByteHDMITXP0(TX_SYS_CTRL1_ADDR));
        /* Now we wake up HDMI Output */
        SI_WakeUpHDMITX();

        /*leo in order to sync Audio  on hotplug*/
        SI_SetHdmiAudio(pstAttr->bEnableAudio);
        SI_SendCP_Packet(HI_FALSE);
        SI_EnableHdmiDevice();
        HI_INFO_HDMI("After TX_SYS_CTRL1_ADDR:0x%x\n", ReadByteHDMITXP0(TX_SYS_CTRL1_ADDR));
        
    }

    g_stHdmiChnParam[enHdmi].bStart = HI_TRUE;
    SI_timer_count();
#if defined (HDCP_SUPPORT)
    if (HI_TRUE == pstAttr->bHDCPEnable)
    {
        if (HI_TRUE == SI_Is_HPDKernelCallback_DetectHPD())
        {
            /* HPD again before we try to open Auth 1*/
            HI_ERR_HDMI("HPD Callback detect new HPD\n");
        }
        else if(HI_TRUE == SI_Is_HPDUserCallback_DetectHPD())
        {
            /* HPD again before we try to open Auth 2*/
            HI_ERR_HDMI("HPD Callback with Auth detect new HPD\n");
        }
        else
        {
            HI_INFO_HDMI("try to open HDCP Auth\n");
            SI_WriteByteEEPROM(EE_TX_HDCP, 0xFF);
        }
    }
    else
    {
        SI_WriteByteEEPROM(EE_TX_HDCP, 0x00);
    }
    SI_timer_count();

    SI_timer_count();
    if (HI_TRUE != pstAttr->bHDCPEnable)
    {
        SI_timer_stop();
    }
#endif
    HI_INFO_HDMI("Leave DRV_HDMI_Start\n");
    return HI_SUCCESS;
}

HI_U32 DRV_HDMI_Stop(HI_UNF_HDMI_ID_E enHdmi)
{
    HI_INFO_HDMI("Enter DRV_HDMI_Stop\n");
    HDMI_CHECK_ID(enHdmi);
    //ֻ��Init num����1ʱ�ſ��Թر�hdmi���
    HI_INFO_HDMI("Enter DRV_HDMI_Stop\n");
    if(DRV_HDMI_InitNum(enHdmi) != 1)
    {
        HI_INFO_HDMI("DRV_HDMI_InitNum != 1 return \n");
        return HI_SUCCESS;
    }
    
    if (HI_FALSE == g_stHdmiChnParam[enHdmi].bStart)
    {
        return HI_SUCCESS;
    }
    SI_SendCP_Packet(ON);
    msleep(40);
    SI_SetHdmiVideo(HI_FALSE);
    SI_SetHdmiAudio(HI_FALSE);
    SI_DisableHdmiDevice();

    msleep(10);
    SI_PowerDownHdmiTx();

    g_stHdmiChnParam[enHdmi].bStart = HI_FALSE;
#if defined (HDCP_SUPPORT)
    /*Set HDCP Off */
    SI_WriteByteEEPROM(EE_TX_HDCP, 0x00);
#endif

    HI_INFO_HDMI("Leave DRV_HDMI_Stop\n");
    return HI_SUCCESS;
}

HI_U32 DRV_HDMI_SetDeepColor(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_DEEP_COLOR_E enDeepColor)
{
    HI_UNF_HDMI_ATTR_S  attr;
    HDMI_ATTR_S         stHDMIAttr;
    HI_U32              SiDeepColor;
    HI_U32              RetError = HI_SUCCESS;

    HI_INFO_HDMI("Enter DRV_HDMI_SetDeepColor\n");
    if (HI_UNF_HDMI_DEEP_COLOR_24BIT == enDeepColor)
    {
        SiDeepColor = SiI_DeepColor_Off;
    }
    else if (HI_UNF_HDMI_DEEP_COLOR_30BIT == enDeepColor)
    {
        SiDeepColor = SiI_DeepColor_30bit;
    }
    else if (HI_UNF_HDMI_DEEP_COLOR_36BIT == enDeepColor)
    {
        SiDeepColor = SiI_DeepColor_36bit;
    }
    else
    {
        SiDeepColor = SiI_DeepColor_Off;
    }
    DRV_HDMI_SetAVMute(HI_UNF_HDMI_ID_0, HI_TRUE);

    SI_SetDeepColor(SiDeepColor);

    memcpy(&stHDMIAttr, &(g_stHdmiChnParam[HI_UNF_HDMI_ID_0].stHDMIAttr), sizeof(HDMI_ATTR_S));
    attr = stHDMIAttr.stAttr;

    /* DeepColor Atribute */  
    if ( ((SiI_DeepColor_30bit == attr.enDeepColorMode) || (SiI_DeepColor_36bit == attr.enDeepColorMode))
        && ((HI_DRV_DISP_FMT_1080P_60 == attr.enVideoFmt) || (HI_DRV_DISP_FMT_1080P_50 == attr.enVideoFmt))
       )
    {
        SI_TX_PHY_HighBandwidth(HI_TRUE);
    }
    else
    {
        SI_TX_PHY_HighBandwidth(HI_FALSE);
    }

    attr.enDeepColorMode = SiDeepColor;
    stHDMIAttr.stAttr = attr;
    RetError = hdmi_SetAttr(HI_UNF_HDMI_ID_0, &stHDMIAttr, HI_TRUE);
    DRV_HDMI_SetAVMute(HI_UNF_HDMI_ID_0, HI_FALSE);
    HI_INFO_HDMI("Leave DRV_HDMI_SetDeepColor\n");

    //DeepColor is setting by HDMI PHY, but it will effect
    //HDMI CONTROLLER Video frequency, it need to do software reset, before reset.
    if ((SiDeepColor == SiI_DeepColor_Off) || (SiDeepColor == SiI_DeepColor_24bit))
    { 
        SI_SW_ResetHDMITX();   //force to reset the whole CTRL+PHY!
    }

    return HI_SUCCESS;
}

HI_U32 DRV_HDMI_GetDeepColor(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_DEEP_COLOR_E *penDeepColor)
{
    return HI_SUCCESS;
}

HI_U32 DRV_HDMI_SetxvYCCMode(HI_UNF_HDMI_ID_E enHdmi, HI_BOOL bEnable)
{
    HI_U32 Ret;
    HI_U8 u8Data;
    HI_U8 InfCtrl2;

    InfCtrl2 = ReadByteHDMITXP1(INF_CTRL2);
    if (HI_TRUE == bEnable)
    {
        HI_INFO_HDMI("enable xvYCC\n");
        /* enable Gamut Metadata InfoFrame transmission */
        InfCtrl2 |= 0xc0;
        HI_INFO_HDMI("INF_CTRL2(0x%02x) is 0x%02x\n", INF_CTRL2, InfCtrl2);
        WriteByteHDMITXP1(INF_CTRL2, InfCtrl2);  //Packet Buffer Control #2 Register 0x7A   0x3F

        /* Gamut boundary descriptions (GBD) and other gamut-related metadata 
           are carried using the Gamut Metadata Packet.*/
        if ( g_stHdmiChnParam[enHdmi].stHDMIAttr.stAttr.enVideoFmt <= HI_DRV_DISP_FMT_720P_50)
        {
            Ret = SI_SendGamutMeta_Packet(HI_TRUE);
        }
        else
        {
            Ret = SI_SendGamutMeta_Packet(HI_FALSE);
        }

        u8Data = ReadByteHDMITXP0(RGB2XVYCC_CT);  //hdmi/hmitx.h  TX_SLV0:0x72
        u8Data = 0x07;
        HI_INFO_HDMI("RGB2XVYCC_CT (0x%02x) data is 0x%02x\n", RGB2XVYCC_CT, u8Data);
        WriteByteHDMITXP0(RGB2XVYCC_CT, u8Data);
    }
    else
    {
        HI_INFO_HDMI("Disable xvYCC\n");
        /* disable Gamut Metadata InfoFrame transmission */
        InfCtrl2 &= ~0xc0;
        HI_INFO_HDMI("INF_CTRL2(0x%02x) is 0x%02x\n", INF_CTRL2, InfCtrl2);
        WriteByteHDMITXP1(INF_CTRL2, InfCtrl2);  //Packet Buffer Control #2 Register 0x7A   0x3F

        u8Data = ReadByteHDMITXP0(RGB2XVYCC_CT);  //hdmi/hmitx.h  TX_SLV0:0x72
        u8Data = 0x00;
        HI_INFO_HDMI("RGB2XVYCC_CT (0x%02x) data is 0x%02x\n", RGB2XVYCC_CT, u8Data);
        WriteByteHDMITXP0(RGB2XVYCC_CT, u8Data);
    }
    HI_INFO_HDMI("end of HI_UNF_HDMI_SetxvYCCMode\n");

    return HI_SUCCESS;
}

HI_U32 DRV_HDMI_SetAVMute(HI_UNF_HDMI_ID_E enHdmi, HI_BOOL bAvMute)
{
    HI_INFO_HDMI("Enter DRV_HDMI_SetAVMute, bAvMute:%d\n", bAvMute);
    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);

    if (bAvMute)
    {
    //Disable HDMI Output!!
    SI_SendCP_Packet(HI_TRUE);
    msleep(50); //HDMI compatibility requirement for Suddenly Close.
    }
    else
    {
    SI_SendCP_Packet(HI_FALSE);
    msleep(50); //HDMI compatibility requirement for Suddenly Close.
    }

    HI_INFO_HDMI("Leave DRV_HDMI_SetAVMute\n");
    return HI_SUCCESS;
}

HI_U32 DRV_HDMI_SetFormat(HI_UNF_HDMI_ID_E enHdmi, HI_DRV_DISP_FMT_E enEncodingFormat)
{
    HI_U32                            Ret = HI_SUCCESS;
    HI_UNF_HDMI_COLORSPACE_E          enColorimetry;
    HI_UNF_HDMI_ASPECT_RATIO_E             enAspectRate;
    HI_U32                            u32PixelRepetition;
    HI_U32                            enRGBQuantization;
    HDMI_ATTR_S                       stHDMIAttr;
    HI_UNF_HDMI_INFOFRAME_S           stInfoFrame, stAUDInfoFrame;
    HI_UNF_HDMI_AVI_INFOFRAME_VER2_S *pstVIDInfoframe;
    HI_UNF_HDMI_SINK_CAPABILITY_S     sinkCap;
    HI_UNF_HDMI_VIDEO_MODE_E          enVidOutMode;
    HI_BOOL                           bHDMIMode = HI_TRUE;

    HI_INFO_HDMI("Enter DRV_HDMI_SetFormat enEncodingFormat:%d\n", enEncodingFormat);
    //printk("%s.%d : DRV_HDMI_SetFormat  enEncodingFormat:%d\n",__FUNCTION__,__LINE__,enEncodingFormat);
    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);

    //printk("DRV_HDMI_SetFormat : %d \n",enEncodingFormat);
    //printk("HI_DRV_DISP_FMT_1080P_24_FP : %d \n",HI_DRV_DISP_FMT_1080P_24_FP);

    
    if(HI_TRUE != SI_HPD_Status())
    {
        //when undetect hotplug,don't need to set fmt,we process it in hotplug callbackfunc 
        HI_INFO_HDMI("hot plug not detected!");
        return HI_SUCCESS;
    }
#if 0 /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
    //ƽ������ y00229039
    if (HI_TRUE == g_stHdmiCommParam.bOpenGreenChannel)
    {
        printk("DRV_HDMI_SetFormat \n");
        return Ret;
    }
#endif /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/

    DRV_HDMI_SetAVMute(HI_UNF_HDMI_ID_0, HI_TRUE);

    //Sef-check video format!!!
    enEncodingFormat = DRV_HDMI_CheckVOFormat(enEncodingFormat);


    memcpy(&stHDMIAttr, &(g_stHdmiChnParam[HI_UNF_HDMI_ID_0].stHDMIAttr), sizeof(HDMI_ATTR_S));

    if(enEncodingFormat == HI_DRV_DISP_FMT_1440x480i_60)
    {
        enEncodingFormat = HI_DRV_DISP_FMT_NTSC;
    }
    else if(enEncodingFormat == HI_DRV_DISP_FMT_1440x576i_50)
    {
        enEncodingFormat = HI_DRV_DISP_FMT_PAL;
    }
    else if(enEncodingFormat == HI_DRV_DISP_FMT_1080P_24_FP)
    {
        enEncodingFormat = HI_DRV_DISP_FMT_1080P_24;
        stHDMIAttr.stAttr.b3DEnable = HI_TRUE;
        stHDMIAttr.stAttr.u83DParam = HI_UNF_3D_FRAME_PACKETING;
    }
    else if(enEncodingFormat == HI_DRV_DISP_FMT_720P_60_FP)
    {
        enEncodingFormat = HI_DRV_DISP_FMT_720P_60;
        stHDMIAttr.stAttr.b3DEnable = HI_TRUE;
        stHDMIAttr.stAttr.u83DParam = HI_UNF_3D_FRAME_PACKETING;
    }
    else if(enEncodingFormat == HI_DRV_DISP_FMT_720P_50_FP)
    {
        enEncodingFormat = HI_DRV_DISP_FMT_720P_50;
        stHDMIAttr.stAttr.b3DEnable = HI_TRUE;
        stHDMIAttr.stAttr.u83DParam = HI_UNF_3D_FRAME_PACKETING;
    }

    //printk("%s: 1.enEncodingFormat:%d\n",__FUNCTION__,enEncodingFormat);
    /* Output all debug message */
    SI_GetHdmiSinkCaps(&sinkCap);
    if (HI_TRUE == sinkCap.bVideoFmtSupported[enEncodingFormat])
    {
        HI_INFO_HDMI("From EDID, sink can receive this format!!!\n");
    }
    else
    {
        HI_ERR_HDMI("Warring:From EDID, Sink CAN NOT receive this format*******\n");
        //return HI_FAILURE;
    }

    if(HI_TRUE == sinkCap.bSupportHdmi)
    {
        bHDMIMode = HI_TRUE;
    }
    else
    {
        bHDMIMode = HI_FALSE;
    }

    HI_INFO_HDMI("change DISP Timing to enEncodingFormat:%d\n", enEncodingFormat);


#if 0 /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
    if(stHDMIAttr.stAttr.enVideoFmt == enEncodingFormat)
    {
        //��ͬ��ʽ����Ҫ�������� y00229039
        printk("==> Setting the same fmt to HDMI <==\n");
        return Ret;
    }
#endif /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/

    enColorimetry      = HDMI_COLORIMETRY_ITU709;
    enAspectRate       = HI_UNF_HDMI_ASPECT_RATIO_16TO9;
    u32PixelRepetition = HI_FALSE;
    enRGBQuantization  = HDMI_RGB_QUANTIZATION_DEFAULT_RANGE;

    enVidOutMode = g_stHdmiChnParam[HI_UNF_HDMI_ID_0].stHDMIAttr.stAttr.enVidOutMode;
    if((enVidOutMode >= HI_UNF_HDMI_VIDEO_MODE_BUTT) ||(enVidOutMode < HI_UNF_HDMI_VIDEO_MODE_RGB444) )
    {
        HI_ERR_HDMI("no set color space!\n");

        if (HI_TRUE == sinkCap.bSupportYCbCr)
        {
            enVidOutMode = HI_UNF_HDMI_VIDEO_MODE_YCBCR444;
        }
        else 
        {
            enVidOutMode = HI_UNF_HDMI_VIDEO_MODE_RGB444;
        }        
    }

    /* New function to set AVI Infoframe */
    hdmi_GetInfoFrame(HI_UNF_HDMI_ID_0, HI_INFOFRAME_TYPE_AVI, &stInfoFrame); 

    pstVIDInfoframe = (HI_UNF_HDMI_AVI_INFOFRAME_VER2_S *)&(stInfoFrame.unInforUnit.stAVIInfoFrame);

    if(HI_DRV_DISP_FMT_1080P_60 == enEncodingFormat)
    {
        HI_INFO_HDMI("Set 1920X1080P_60000 enTimingMode:0x%x\n", pstVIDInfoframe->enTimingMode);
        pstVIDInfoframe->enTimingMode = HI_DRV_DISP_FMT_1080P_60;
    }
    else if(HI_DRV_DISP_FMT_1080P_50 == enEncodingFormat)
    {
        HI_INFO_HDMI("Set 1920X1080P_50000 enTimingMode:0x%x\n", pstVIDInfoframe->enTimingMode);
        pstVIDInfoframe->enTimingMode = HI_DRV_DISP_FMT_1080P_50;
    }
    else if(HI_DRV_DISP_FMT_1080P_30 == enEncodingFormat)
    {
        HI_INFO_HDMI("Set 1920X1080P_30000 enTimingMode:0x%x\n", pstVIDInfoframe->enTimingMode);
        pstVIDInfoframe->enTimingMode = HI_DRV_DISP_FMT_1080P_30;
    }
    else if(HI_DRV_DISP_FMT_1080P_25 == enEncodingFormat)
    {
        HI_INFO_HDMI("Set 1920X1080P_25000 enTimingMode:0x%x\n", pstVIDInfoframe->enTimingMode);
        pstVIDInfoframe->enTimingMode = HI_DRV_DISP_FMT_1080P_25;
    }
    else if(HI_DRV_DISP_FMT_1080P_24 == enEncodingFormat)
    {
        HI_INFO_HDMI("Set 1920X1080P_24000 enTimingMode:0x%x\n", pstVIDInfoframe->enTimingMode);
        pstVIDInfoframe->enTimingMode = HI_DRV_DISP_FMT_1080P_24;
    }
    else if(HI_DRV_DISP_FMT_1080i_60 == enEncodingFormat)
    {
        HI_INFO_HDMI("Set 1920X1080i_60000 enTimingMode:0x%x\n", pstVIDInfoframe->enTimingMode);
        pstVIDInfoframe->enTimingMode = HI_DRV_DISP_FMT_1080i_60;
    }
    else if(HI_DRV_DISP_FMT_1080i_50 == enEncodingFormat)
    {
        HI_INFO_HDMI("Set 1920X1080i_50000 enTimingMode:0x%x\n", pstVIDInfoframe->enTimingMode);
        pstVIDInfoframe->enTimingMode = HI_DRV_DISP_FMT_1080i_50;
    }
    else if(HI_DRV_DISP_FMT_720P_60 == enEncodingFormat)
    {
        HI_INFO_HDMI("Set 1280X720P_60000 enTimingMode:0x%x\n", pstVIDInfoframe->enTimingMode);
        pstVIDInfoframe->enTimingMode = HI_DRV_DISP_FMT_720P_60;
    }
    else if(HI_DRV_DISP_FMT_720P_50 == enEncodingFormat)
    {
        HI_INFO_HDMI("Set 1280X720P_50000 enTimingMode:0x%x\n", pstVIDInfoframe->enTimingMode);
        pstVIDInfoframe->enTimingMode = HI_DRV_DISP_FMT_720P_50;
    }
    else if(HI_DRV_DISP_FMT_576P_50 == enEncodingFormat)
    {
        HI_INFO_HDMI("Set 720X576P_50000 enTimingMode:0x%x\n", pstVIDInfoframe->enTimingMode);
        pstVIDInfoframe->enTimingMode = HI_DRV_DISP_FMT_576P_50;
        enColorimetry = HDMI_COLORIMETRY_ITU601;
        enAspectRate  = HI_UNF_HDMI_ASPECT_RATIO_4TO3;
    }
    else if(HI_DRV_DISP_FMT_480P_60 == enEncodingFormat)
    {
        HI_INFO_HDMI("Set 720X480P_60000 enTimingMode:0x%x\n", pstVIDInfoframe->enTimingMode);
        pstVIDInfoframe->enTimingMode = HI_DRV_DISP_FMT_480P_60;
        enColorimetry = HDMI_COLORIMETRY_ITU601;
        enAspectRate  = HI_UNF_HDMI_ASPECT_RATIO_4TO3;
    }
    else if((HI_DRV_DISP_FMT_PAL == enEncodingFormat)||
        (HI_DRV_DISP_FMT_PAL_B == enEncodingFormat)||
        (HI_DRV_DISP_FMT_PAL_B1 == enEncodingFormat)||
        (HI_DRV_DISP_FMT_PAL_D == enEncodingFormat)||
        (HI_DRV_DISP_FMT_PAL_D1 == enEncodingFormat)||
        (HI_DRV_DISP_FMT_PAL_G == enEncodingFormat)||
        (HI_DRV_DISP_FMT_PAL_H == enEncodingFormat)||
        (HI_DRV_DISP_FMT_PAL_K == enEncodingFormat)||
        (HI_DRV_DISP_FMT_PAL_I == enEncodingFormat)||
        (HI_DRV_DISP_FMT_PAL_M == enEncodingFormat)||
        (HI_DRV_DISP_FMT_PAL_N == enEncodingFormat)||
        (HI_DRV_DISP_FMT_PAL_Nc == enEncodingFormat)||
        (HI_DRV_DISP_FMT_PAL_60 == enEncodingFormat)||        
        (HI_DRV_DISP_FMT_SECAM_SIN == enEncodingFormat)||
        (HI_DRV_DISP_FMT_SECAM_COS == enEncodingFormat)||
        (HI_DRV_DISP_FMT_SECAM_L == enEncodingFormat)||
        (HI_DRV_DISP_FMT_SECAM_B == enEncodingFormat)||
        (HI_DRV_DISP_FMT_SECAM_G == enEncodingFormat)||
        (HI_DRV_DISP_FMT_SECAM_D == enEncodingFormat)||
        (HI_DRV_DISP_FMT_SECAM_K == enEncodingFormat)||
        (HI_DRV_DISP_FMT_SECAM_H == enEncodingFormat))
    {
        HI_INFO_HDMI("Set PAL 576I_50000 enTimingMode:0x%x\n", pstVIDInfoframe->enTimingMode);
        pstVIDInfoframe->enTimingMode = enEncodingFormat;
        enColorimetry = HDMI_COLORIMETRY_ITU601;
        enAspectRate  = HI_UNF_HDMI_ASPECT_RATIO_4TO3;
        u32PixelRepetition = HI_TRUE;
    }
    else if((HI_DRV_DISP_FMT_NTSC == enEncodingFormat)||
        (HI_DRV_DISP_FMT_NTSC_J == enEncodingFormat)||
        (HI_DRV_DISP_FMT_NTSC_443 == enEncodingFormat))
    {
        HI_INFO_HDMI("Set NTS 480I_60000 enTimingMode:0x%x\n", pstVIDInfoframe->enTimingMode);
        pstVIDInfoframe->enTimingMode = enEncodingFormat;
        enColorimetry = HDMI_COLORIMETRY_ITU601;
        enAspectRate  = HI_UNF_HDMI_ASPECT_RATIO_4TO3;
        u32PixelRepetition = HI_TRUE;
    }
    else if(HI_DRV_DISP_FMT_861D_640X480_60 == enEncodingFormat)
    {
        HI_INFO_HDMI("Set 640X480P_60000 enTimingMode:0x%x\n", pstVIDInfoframe->enTimingMode);
        pstVIDInfoframe->enTimingMode = HI_DRV_DISP_FMT_861D_640X480_60;
        enColorimetry      = HDMI_COLORIMETRY_ITU601;
        enAspectRate       = HI_UNF_HDMI_ASPECT_RATIO_4TO3;
        u32PixelRepetition = HI_FALSE;

        enRGBQuantization  = HDMI_RGB_QUANTIZATION_FULL_RANGE;
        enVidOutMode = HI_UNF_HDMI_VIDEO_MODE_RGB444;
    }
#if defined (DVI_SUPPORT)
    else if ((HI_DRV_DISP_FMT_VESA_800X600_60 <= enEncodingFormat) && (HI_DRV_DISP_FMT_VESA_2048X1152_60 >= enEncodingFormat))
    {
        HI_INFO_HDMI("DVI timing mode enTimingMode:0x%x\n", pstVIDInfoframe->enTimingMode);
        DEBUG_PRINTK("Force to DVI Mode\n");
        bHDMIMode = HI_FALSE;

        pstVIDInfoframe->enTimingMode = HI_DRV_DISP_FMT_861D_640X480_60;
        enColorimetry      = HDMI_COLORIMETRY_ITU601;
        enAspectRate       = HI_UNF_HDMI_ASPECT_RATIO_4TO3;
        u32PixelRepetition = HI_FALSE;

        enRGBQuantization  = HDMI_RGB_QUANTIZATION_FULL_RANGE;

        enVidOutMode = HI_UNF_HDMI_VIDEO_MODE_RGB444;        
    }
#endif
    HI_INFO_HDMI("enVidOutMode:%d\n", enVidOutMode);
    pstVIDInfoframe->enOutputType            = enVidOutMode;
    pstVIDInfoframe->bActive_Infor_Present   = HI_TRUE;
    pstVIDInfoframe->enBarInfo               = HDMI_BAR_INFO_NOT_VALID;
    pstVIDInfoframe->enScanInfo              = HDMI_SCAN_INFO_NO_DATA;//HDMI_SCAN_INFO_OVERSCANNED;
    pstVIDInfoframe->enColorimetry           = enColorimetry;
    pstVIDInfoframe->enAspectRatio           = enAspectRate;
    pstVIDInfoframe->enActiveAspectRatio     = enAspectRate;
    pstVIDInfoframe->enPictureScaling        = HDMI_PICTURE_NON_UNIFORM_SCALING;
    pstVIDInfoframe->enRGBQuantization       = enRGBQuantization;
    pstVIDInfoframe->bIsITContent            = HI_FALSE;
    pstVIDInfoframe->u32PixelRepetition      = u32PixelRepetition;

    pstVIDInfoframe->enYCCQuantization      = HDMI_YCC_QUANTIZATION_LIMITED_RANGE;

    pstVIDInfoframe->u32LineNEndofTopBar     = 0;  /* We can determine it in hi_unf_hdmi.c */
    pstVIDInfoframe->u32LineNStartofBotBar   = 0;  /* We can determine it in hi_unf_hdmi.c */
    pstVIDInfoframe->u32PixelNEndofLeftBar   = 0;  /* We can determine it in hi_unf_hdmi.c */
    pstVIDInfoframe->u32PixelNStartofRightBar= 0;  /* We can determine it in hi_unf_hdmi.c */

    if(enEncodingFormat < HI_DRV_DISP_FMT_861D_640X480_60)
    {
        SI_TX_SetHDMIMode(OFF);    //for hdmi
        SI_DisableHdmiDevice();
        SI_EnableHdmiDevice();
        SI_TX_SetHDMIMode(ON);    //for hdmi
    }
    msleep(100);

    if((HI_TRUE == sinkCap.bSupportHdmi) && (bHDMIMode == HI_TRUE)){
        HI_INFO_HDMI("***hdmi_SetInfoFrame for AVI Infoframe\n");
        hdmi_SetInfoFrame(HI_UNF_HDMI_ID_0, &stInfoFrame);
        hdmi_GetInfoFrame(HI_UNF_HDMI_ID_0, HI_INFOFRAME_TYPE_AUDIO, &stAUDInfoFrame);        
        HI_INFO_HDMI("***hdmi_SetInfoFrame for AUDIO Infoframe\n");
        hdmi_SetInfoFrame(HI_UNF_HDMI_ID_0, &stAUDInfoFrame);
    }

    HI_INFO_HDMI("hdmi_SetAttr in change TimingMode\n");
    stHDMIAttr.stAttr.enVideoFmt    = enEncodingFormat;
    stHDMIAttr.stAttr.enVidOutMode  = enVidOutMode;
    stHDMIAttr.stAttr.bEnableHdmi   = bHDMIMode;        //HDMI or DVI
    HI_INFO_HDMI("attr.bEnableHdmi:0x%x\n", stHDMIAttr.stAttr.bEnableHdmi);

    //printk("%s: 3.stAttr.enVideoFmt:%d\n",__FUNCTION__,stHDMIAttr.stAttr.enVideoFmt);
    Ret = hdmi_SetAttr(HI_UNF_HDMI_ID_0, &stHDMIAttr, HI_TRUE);
    SI_EnableHdmiDevice();
    DRV_HDMI_SetAVMute(HI_UNF_HDMI_ID_0, HI_FALSE);
    //printk("%s: 2.enEncodingFormat:%d\n",__FUNCTION__,enEncodingFormat);
    HI_INFO_HDMI("Leave DRV_HDMI_SetFormat\n");
    return Ret;
}

HI_U32 DRV_HDMI_PreFormat(HI_UNF_HDMI_ID_E enHdmi, HI_DRV_DISP_FMT_E enEncodingFormat)
{

#if defined (HDCP_SUPPORT)
    //Disable HDCP
    /* First to close HDCP Key */
    SI_WriteByteEEPROM(EE_TX_HDCP, 0x00);
    SI_SetEncryption(OFF);
#endif

    /* Let HDMI enter NO-HDCP Mode */
    DRV_HDMI_SetAVMute(HI_UNF_HDMI_ID_0, HI_TRUE);
    msleep(200);
    SI_DisableHdmiDevice();

    return 0;
}

static HI_U32 DRV_HDMI_CheckVOFormat(HI_DRV_DISP_FMT_E setFmt)
{
    #if 0 
    HI_S32                   Ret = HI_SUCCESS;
    HI_DRV_DISP_FMT_E         DispFormat;
    
    /*if func ops and func ptr is null, then return ;*/
    if(!disp_func_ops ||!disp_func_ops->pfnDispGetFormat)
    return 1;
    /*Check HDMI format/VO format */
    Ret = disp_func_ops->pfnDispGetFormat(HI_UNF_DISPLAY1, &DispFormat);
    if(DispFormat != setFmt)
    {
        HI_ERR_HDMI("disp:%d and hdmi %d set fmt error!",DispFormat,setFmt);
    }
    #endif
    return setFmt;
}

HI_U32 DRV_HDMI_Force_GetEDID(HDMI_EDID_S *pEDID)
{
    HI_U32 u32EdidLength;
    HI_U8 u32EdidBlock[512];
    HI_U32  ret;

    HI_INFO_HDMI("Enter DRV_HDMI_Extern_GetEDID\n");
    HDMI_CHECK_ID(pEDID->enHdmi);
    HDMI_CheckChnOpen(pEDID->enHdmi);

    ret = SI_Force_GetEDID(u32EdidBlock, &u32EdidLength);
    if(ret != HI_SUCCESS)                                //Rowe
    {
        HI_ERR_HDMI("Force Get EDID fail!:%x\n",ret);
        return ret;
    }

    if((u32EdidLength > 0) && (u32EdidLength < 512))
    {
        pEDID->u8EdidValid   = HI_TRUE;
        pEDID->u32Edidlength = u32EdidLength;
        memcpy(pEDID->u8Edid, u32EdidBlock, pEDID->u32Edidlength);
    }
    else
    {
        pEDID->u8EdidValid   = HI_FALSE;
        pEDID->u32Edidlength = 0;
        memset(pEDID->u8Edid, 0, 512);
    }

    HI_INFO_HDMI("Leave DRV_HDMI_Extern_GetEDID\n");
    return 0;
}

HI_U32 DRV_HDMI_GetPlayStatus(HI_UNF_HDMI_ID_E enHdmi, HI_U32 *pu32Stutus)
{
    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);

    *pu32Stutus = g_stHdmiChnParam[enHdmi].bStart;
    return 0;
}

HI_U32 DRV_HDMI_LoadKey(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_LOAD_KEY_S *pstLoadKey)
{
    HI_U32 u32Ret = HI_SUCCESS;
#if defined (HDCP_SUPPORT)
    CIPHER_FLASH_ENCRYPT_HDCPKEY_S stFlashEncrytedHdcpKey ;
    HI_CHIP_VERSION_E stpenChipVersion;
    HI_CHIP_TYPE_E stpenChipType;
    
    
    HDMI_CHECK_ID(enHdmi);

    HI_DRV_SYS_GetChipVersion(&stpenChipType, &stpenChipVersion);

    if(HI_CHIP_TYPE_HI3712 != stpenChipType)
    {
        HI_ERR_HDMI("chip no support read from flsah!\n");
        return HI_FAILURE;
    }
    
    if((HI_NULL != g_stCIPHERExportFunctionLists)
     ||(HI_NULL != g_stCIPHERExportFunctionLists->DRV_Cipher_LoadHdcpKey))
    {
        /*load hdcp key */
        memcpy(stFlashEncrytedHdcpKey.u8Key, pstLoadKey->pu8InputEncryptedKey, pstLoadKey->u32KeyLength);
        u32Ret = (g_stCIPHERExportFunctionLists->DRV_Cipher_LoadHdcpKey)(stFlashEncrytedHdcpKey);
        if( HI_SUCCESS != u32Ret)
        {
            HI_ERR_HDMI("Load hdcp key error!\n");
        }
        HI_INFO_HDMI("Load hdcp key successful!\n");
    }
#endif
    return u32Ret;
}

HI_S32 DRV_HDMI_GetProcID(HI_UNF_HDMI_ID_E enHdmi, HI_U32 *pu32ProcID)
{
    HI_S32 u32Ret = HI_FAILURE;
    HI_U32 u32ProcIndex = 0;
    HI_INFO_HDMI("\nDRV_HDMI_GetProcID\n");

    //������
    //û����ͨ��ID
    //HDMI_CHECK_ID(enHdmi);

    if(pu32ProcID == NULL)
    {
        return u32Ret;
    }

    for(u32ProcIndex = 0; u32ProcIndex < MAX_PROCESS_NUM; u32ProcIndex++)
    {
        if(HI_TRUE != g_stHdmiChnParam[HI_UNF_HDMI_ID_0].eventList[u32ProcIndex].bUsed)
        {
            *pu32ProcID = u32ProcIndex;
            g_stHdmiChnParam[HI_UNF_HDMI_ID_0].eventList[u32ProcIndex].bUsed = HI_TRUE;
            u32Ret = HI_SUCCESS;
            break;
        }            
    }
    return u32Ret;
}

HI_S32 DRV_HDMI_ReleaseProcID(HI_UNF_HDMI_ID_E enHdmi, HI_U32 u32ProcID)
{
    HI_S32 u32Ret = HI_FAILURE;
    //HI_U32 u32ProcIndex = 0;

    //
    HI_INFO_HDMI("\nDRV_HDMI_ReleaseProcID\n");
    
    //������
    //HDMI_CHECK_ID(enHdmi);

    if(u32ProcID >= MAX_PROCESS_NUM)
    {
        return u32Ret;
    }

    //g_stHdmiChnParam[HI_UNF_HDMI_ID_0].eventList[pu32ProcID].bUsed = FALSE;

    memset(&g_stHdmiChnParam[HI_UNF_HDMI_ID_0].eventList[u32ProcID], 0, sizeof(HDMI_PROC_EVENT_S));

    u32Ret = HI_SUCCESS;
        
    return u32Ret;
}

HI_S32 DRV_HDMI_AudioChange(HI_UNF_HDMI_ID_E enHdmi, HDMI_AUDIO_ATTR_S *pstHDMIAOAttr)
{
    HI_S32 Ret = HI_SUCCESS;
    //HDMI_AUDIO_ATTR_S      stHDMIAUDAttr;
    HI_UNF_HDMI_INFOFRAME_S InfoFrame;
    HI_UNF_HDMI_AUD_INFOFRAME_VER1_S *pstAUDInfoframe;
    HDMI_ATTR_S stHDMIAttr;
    HI_INFO_HDMI("=====>  DRV_HDMI_AudioChange <=====\n");
    //printk("%s.%d : DRV_HDMI_AudioChange \n",__FUNCTION__,__LINE__);
    //printk("%s.%d : enSoundIntf:%d,enSampleRate:%d,u32Channels:%d \n",__FUNCTION__,__LINE__,pstHDMIAOAttr->enSoundIntf,pstHDMIAOAttr->enSampleRate,pstHDMIAOAttr->u32Channels);
    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);

    Ret = DRV_HDMI_GetInfoFrame(enHdmi, HI_INFOFRAME_TYPE_AUDIO,&InfoFrame);
    if(Ret != HI_SUCCESS)
    {
        HI_ERR_HDMI("get HDMI infoframe failed\n");
        return HI_FAILURE;
    }
    pstAUDInfoframe = (HI_UNF_HDMI_AUD_INFOFRAME_VER1_S *)&(InfoFrame.unInforUnit.stAUDInfoFrame);

    //Ret =DRV_HDMI_GetAOAttr(enHdmi, &stHDMIAUDAttr);
    Ret = DRV_HDMI_GetAttr(enHdmi,&stHDMIAttr);
    if(Ret != HI_SUCCESS)
    {
        HI_ERR_HDMI("get HDMI attribute failed\n");
        return HI_FAILURE;
    }

    if(pstHDMIAOAttr->enSoundIntf == stHDMIAttr.enSoundIntf && 
       pstHDMIAOAttr->enSampleRate == stHDMIAttr.stAttr.enSampleRate &&
       pstHDMIAOAttr->u32Channels == pstAUDInfoframe->u32ChannelCount)
    {
        //printk("%s.%d : The same Audio Attr \n",__FUNCTION__,__LINE__);        
        return HI_SUCCESS;
    }
            
    if (HDMI_AUDIO_INTERFACE_I2S == pstHDMIAOAttr->enSoundIntf 
       || HDMI_AUDIO_INTERFACE_SPDIF== pstHDMIAOAttr->enSoundIntf
       || HDMI_AUDIO_INTERFACE_HBR== pstHDMIAOAttr->enSoundIntf)
    {
        stHDMIAttr.enSoundIntf = pstHDMIAOAttr->enSoundIntf;
    }
    else
    {
        HI_ERR_HDMI("Error input Audio interface(%d)\n",pstHDMIAOAttr->enSoundIntf);
        return HI_FAILURE;
    }

    HI_INFO_HDMI("\n\n\n audio Sample Rate : %d\n\n\n",pstHDMIAOAttr->enSampleRate);
    switch (pstHDMIAOAttr->enSampleRate)
    {
        case HI_UNF_SAMPLE_RATE_32K:
        case HI_UNF_SAMPLE_RATE_44K:
        case HI_UNF_SAMPLE_RATE_48K:
        case HI_UNF_SAMPLE_RATE_88K:
        case HI_UNF_SAMPLE_RATE_96K:
        case HI_UNF_SAMPLE_RATE_176K:
        case HI_UNF_SAMPLE_RATE_192K:
            stHDMIAttr.stAttr.enSampleRate = pstHDMIAOAttr->enSampleRate;
            break;
        default:
            HI_ERR_HDMI("Error input Audio Frequency(%d)\n",pstHDMIAOAttr->enSampleRate);
            return HI_FAILURE;
    }

    HI_INFO_HDMI("set HDMI Audio input chage interface(%d) samplerate(%d)\n", stHDMIAttr.enSoundIntf, stHDMIAttr.stAttr.enSampleRate);
    
    /* Set Audio infoframe */
    /* New function to set Audio Infoframe */
    /* HDMI requires the CT, SS and SF fields to be set to 0 ("Refer to Stream Header") 
       as these items are carried in the audio stream.*/
    memset(&InfoFrame, 0, sizeof(InfoFrame));
    
    InfoFrame.enInfoFrameType = HI_INFOFRAME_TYPE_AUDIO;
    pstAUDInfoframe->u32ChannelCount      = pstHDMIAOAttr->u32Channels;
    pstAUDInfoframe->enCodingType         = HDMI_AUDIO_CODING_REFER_STREAM_HEAD;
    //Refer to Stream Header
    pstAUDInfoframe->u32SampleSize        = HI_UNF_HDMI_DEFAULT_SETTING;
    //Refer to Stream Header
    pstAUDInfoframe->u32SamplingFrequency = HI_UNF_HDMI_DEFAULT_SETTING;
    
    switch(pstHDMIAOAttr->u32Channels)     //HDMI channel map
    {
        case 3:
            pstAUDInfoframe->u32ChannelAlloc = 0x01;
            break;
        case 6:
            pstAUDInfoframe->u32ChannelAlloc = 0x0b;
            break;                
        case 8:
            pstAUDInfoframe->u32ChannelAlloc = 0x13;
            break;
        default:
            pstAUDInfoframe->u32ChannelAlloc = 0x00;
            break;
    }
    pstAUDInfoframe->u32LevelShift        = 0;
    pstAUDInfoframe->u32DownmixInhibit    = HI_FALSE;
    HI_INFO_HDMI("***HI_UNF_HDMI_SetInfoFrame for AUDIO Infoframe\n");
    Ret = DRV_HDMI_SetInfoFrame(enHdmi, &InfoFrame);  
       
    if((pstHDMIAOAttr->u32Channels > 2) 
    &&(HDMI_AUDIO_INTERFACE_I2S == pstHDMIAOAttr->enSoundIntf))
    {
        stHDMIAttr.stAttr.bIsMultiChannel = HI_TRUE;
    }
    else
    {
        stHDMIAttr.stAttr.bIsMultiChannel = HI_FALSE;
    }
    Ret = DRV_HDMI_SetAttr(enHdmi, &stHDMIAttr);

    if(Ret != HI_SUCCESS)
    {
        HI_ERR_HDMI("Set HDMI Audio Attr failed\n");
    }
    
    return Ret;
}

HI_S32 DRV_HDMI_GetAOAttr(HI_UNF_HDMI_ID_E enHdmi, HDMI_AUDIO_ATTR_S *pstHDMIAOAttr)
{
    HI_S32  Ret = HI_SUCCESS;
    HDMI_ATTR_S stHDMIAttr;
    HI_UNF_HDMI_INFOFRAME_S     InfoFrame;
    HI_UNF_HDMI_AUD_INFOFRAME_VER1_S *pstAUDInfoframe;
      
    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);
    
    Ret = DRV_HDMI_GetInfoFrame(enHdmi, HI_INFOFRAME_TYPE_AUDIO,&InfoFrame);
    if(Ret != HI_SUCCESS)
    {
        HI_ERR_HDMI("get HDMI auido infoframe failed\n");
        return HI_FAILURE;
    }
    pstAUDInfoframe = (HI_UNF_HDMI_AUD_INFOFRAME_VER1_S *)&(InfoFrame.unInforUnit.stAUDInfoFrame);
    
    Ret = DRV_HDMI_GetAttr(enHdmi, &stHDMIAttr);
    if(Ret != HI_SUCCESS)
    {
        HI_ERR_HDMI("get HDMI AO attribute failed\n");
        return HI_FAILURE;
    }
    
    pstHDMIAOAttr->enSampleRate = stHDMIAttr.stAttr.enSampleRate;
    pstHDMIAOAttr->enSoundIntf = stHDMIAttr.enSoundIntf;
    pstHDMIAOAttr->u32Channels = pstAUDInfoframe->u32ChannelCount;
        
    return Ret;
}
    
HI_S32 DRV_HDMI_AdjustInfoFrame(HI_UNF_HDMI_ID_E enHdmi,HI_UNF_HDMI_INFOFRAME_S *pstInfoFrame)
{
    HI_U32 Ret = HI_SUCCESS;    

    HDMI_CHECK_NULL_PTR(pstInfoFrame);

    HDMI_CHECK_ID(enHdmi);
    HDMI_CheckChnOpen(enHdmi);
    
    switch(pstInfoFrame->enInfoFrameType)
    {
        case HI_INFOFRAME_TYPE_AVI:
        {
            HI_UNF_HDMI_AVI_INFOFRAME_VER2_S *pstAviInfoFrame;
            pstAviInfoFrame = (HI_UNF_HDMI_AVI_INFOFRAME_VER2_S *)&(pstInfoFrame->unInforUnit.stAVIInfoFrame);

            Ret = hdmi_AdjustAVIInfoFrame(enHdmi, pstAviInfoFrame);        
            break;
        }
        case HI_INFOFRAME_TYPE_SPD:
            break;
        case HI_INFOFRAME_TYPE_AUDIO:
        {
            HI_UNF_HDMI_AUD_INFOFRAME_VER1_S *pstAudInfoFrame;
            pstAudInfoFrame = (HI_UNF_HDMI_AUD_INFOFRAME_VER1_S *)&(pstInfoFrame->unInforUnit.stAUDInfoFrame);

            Ret = hdmi_AdjustAUDInfoFrame(enHdmi, pstAudInfoFrame);
            break;
        }
        case HI_INFOFRAME_TYPE_MPEG:
            break;
        case HI_INFOFRAME_TYPE_VENDORSPEC:
            break;
        default:
            break;
    }    
    return HI_SUCCESS;
}

static HI_U32 hdmi_AdjustAVIInfoFrame(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_AVI_INFOFRAME_VER2_S *pstAVIInfoFrame)
{
    HI_U32 Ret = HI_SUCCESS; 
    HDMI_ATTR_S stHDMIAttr;
    HI_UNF_HDMI_COLORSPACE_E enColorimetry;
    HI_UNF_HDMI_ASPECT_RATIO_E enAspectRate;
    HI_U32 u32PixelRepetition;
    HI_U32 enRGBQuantization;
    HI_UNF_HDMI_SINK_CAPABILITY_S sinkCap;
    HI_UNF_HDMI_VIDEO_MODE_E enVidOutMode;
    HI_BOOL bHDMIMode = HI_TRUE;
    HI_BOOL enEncodingFormat;
    
    DRV_HDMI_GetAttr(enHdmi, &stHDMIAttr);

    Ret = DRV_HDMI_GetSinkCapability(enHdmi, &sinkCap);
    {
        HI_INFO_HDMI("Get Sink Capability Failed! /n");
        return Ret;
    }

    if(HI_TRUE == sinkCap.bSupportHdmi)
    {
        bHDMIMode = HI_TRUE;
    }
    else
    {
        bHDMIMode = HI_FALSE;
    }

    HI_INFO_HDMI("change DISP Timing to enEncodingFormat:%d\n", enEncodingFormat);
    enEncodingFormat = stHDMIAttr.stAttr.enVideoFmt;

    enColorimetry      = HDMI_COLORIMETRY_ITU709;
    enAspectRate       = HI_UNF_HDMI_ASPECT_RATIO_16TO9;
    u32PixelRepetition = HI_FALSE;
    enRGBQuantization  = HDMI_RGB_QUANTIZATION_DEFAULT_RANGE;

    enVidOutMode = stHDMIAttr.stAttr.enVidOutMode;
    if((enVidOutMode >= HI_UNF_HDMI_VIDEO_MODE_BUTT) ||(enVidOutMode < HI_UNF_HDMI_VIDEO_MODE_RGB444) )
    {
        HI_ERR_HDMI("no set color space!\n");

        if (HI_TRUE == sinkCap.bSupportYCbCr)
        {
            enVidOutMode = HI_UNF_HDMI_VIDEO_MODE_YCBCR444;
        }
        else 
        {
            enVidOutMode = HI_UNF_HDMI_VIDEO_MODE_RGB444;
        }        
    }

    if(HI_DRV_DISP_FMT_1080P_60 == enEncodingFormat)
    {
        HI_INFO_HDMI("Set 1920X1080P_60000 enTimingMode:0x%x\n", pstAVIInfoFrame->enTimingMode);
        pstAVIInfoFrame->enTimingMode = HI_DRV_DISP_FMT_1080P_60;
    }
    else if(HI_DRV_DISP_FMT_1080P_50 == enEncodingFormat)
    {
        HI_INFO_HDMI("Set 1920X1080P_50000 enTimingMode:0x%x\n", pstAVIInfoFrame->enTimingMode);
        pstAVIInfoFrame->enTimingMode = HI_DRV_DISP_FMT_1080P_50;
    }
    else if(HI_DRV_DISP_FMT_1080P_30 == enEncodingFormat)
    {
        HI_INFO_HDMI("Set 1920X1080P_30000 enTimingMode:0x%x\n", pstAVIInfoFrame->enTimingMode);
        pstAVIInfoFrame->enTimingMode = HI_DRV_DISP_FMT_1080P_30;
    }
    else if(HI_DRV_DISP_FMT_1080P_25 == enEncodingFormat)
    {
        HI_INFO_HDMI("Set 1920X1080P_25000 enTimingMode:0x%x\n", pstAVIInfoFrame->enTimingMode);
        pstAVIInfoFrame->enTimingMode = HI_DRV_DISP_FMT_1080P_25;
    }
    else if(HI_UNF_ENC_FMT_1080P_24 == enEncodingFormat)
    {
        HI_INFO_HDMI("Set 1920X1080P_24000 enTimingMode:0x%x\n", pstAVIInfoFrame->enTimingMode);
        pstAVIInfoFrame->enTimingMode = HI_UNF_ENC_FMT_1080P_24;
    }
    else if(HI_DRV_DISP_FMT_1080i_60 == enEncodingFormat)
    {
        HI_INFO_HDMI("Set 1920X1080i_60000 enTimingMode:0x%x\n", pstAVIInfoFrame->enTimingMode);
        pstAVIInfoFrame->enTimingMode = HI_DRV_DISP_FMT_1080i_60;
    }
    else if(HI_DRV_DISP_FMT_1080i_50 == enEncodingFormat)
    {
        HI_INFO_HDMI("Set 1920X1080i_50000 enTimingMode:0x%x\n", pstAVIInfoFrame->enTimingMode);
        pstAVIInfoFrame->enTimingMode = HI_DRV_DISP_FMT_1080i_50;
    }
    else if(HI_DRV_DISP_FMT_720P_60 == enEncodingFormat)
    {
        HI_INFO_HDMI("Set 1280X720P_60000 enTimingMode:0x%x\n", pstAVIInfoFrame->enTimingMode);
        pstAVIInfoFrame->enTimingMode = HI_DRV_DISP_FMT_720P_60;
    }
    else if(HI_DRV_DISP_FMT_720P_50 == enEncodingFormat)
    {
        HI_INFO_HDMI("Set 1280X720P_50000 enTimingMode:0x%x\n", pstAVIInfoFrame->enTimingMode);
        pstAVIInfoFrame->enTimingMode = HI_DRV_DISP_FMT_720P_50;
    }
    else if(HI_DRV_DISP_FMT_576P_50 == enEncodingFormat)
    {
        HI_INFO_HDMI("Set 720X576P_50000 enTimingMode:0x%x\n", pstAVIInfoFrame->enTimingMode);
        pstAVIInfoFrame->enTimingMode = HI_DRV_DISP_FMT_576P_50;
        enColorimetry = HDMI_COLORIMETRY_ITU601;
        enAspectRate  = HI_UNF_HDMI_ASPECT_RATIO_4TO3;
    }
    else if(HI_DRV_DISP_FMT_480P_60 == enEncodingFormat)
    {
        HI_INFO_HDMI("Set 720X480P_60000 enTimingMode:0x%x\n", pstAVIInfoFrame->enTimingMode);
        pstAVIInfoFrame->enTimingMode = HI_DRV_DISP_FMT_480P_60;
        enColorimetry = HDMI_COLORIMETRY_ITU601;
        enAspectRate  = HI_UNF_HDMI_ASPECT_RATIO_4TO3;
    }
    else if((HI_DRV_DISP_FMT_PAL == enEncodingFormat)||
        (HI_DRV_DISP_FMT_PAL_B == enEncodingFormat)||
        (HI_DRV_DISP_FMT_PAL_B1 == enEncodingFormat)||
        (HI_DRV_DISP_FMT_PAL_D == enEncodingFormat)||
        (HI_DRV_DISP_FMT_PAL_D1 == enEncodingFormat)||
        (HI_DRV_DISP_FMT_PAL_G == enEncodingFormat)||
        (HI_DRV_DISP_FMT_PAL_H == enEncodingFormat)||
        (HI_DRV_DISP_FMT_PAL_K == enEncodingFormat)||
        (HI_DRV_DISP_FMT_PAL_I == enEncodingFormat)||
        (HI_DRV_DISP_FMT_PAL_M == enEncodingFormat)||
        (HI_DRV_DISP_FMT_PAL_N == enEncodingFormat)||
        (HI_DRV_DISP_FMT_PAL_Nc == enEncodingFormat)||
        (HI_DRV_DISP_FMT_PAL_60 == enEncodingFormat)||        
        (HI_DRV_DISP_FMT_SECAM_SIN == enEncodingFormat)||
        (HI_DRV_DISP_FMT_SECAM_COS == enEncodingFormat)||
        (HI_DRV_DISP_FMT_SECAM_L == enEncodingFormat)||
        (HI_DRV_DISP_FMT_SECAM_B == enEncodingFormat)||
        (HI_DRV_DISP_FMT_SECAM_G == enEncodingFormat)||
        (HI_DRV_DISP_FMT_SECAM_D == enEncodingFormat)||
        (HI_DRV_DISP_FMT_SECAM_K == enEncodingFormat)||
        (HI_DRV_DISP_FMT_SECAM_H == enEncodingFormat))
    {
        HI_INFO_HDMI("Set PAL 576I_50000 enTimingMode:0x%x\n", pstAVIInfoFrame->enTimingMode);
        pstAVIInfoFrame->enTimingMode = enEncodingFormat;
        enColorimetry = HDMI_COLORIMETRY_ITU601;
        enAspectRate  = HI_UNF_HDMI_ASPECT_RATIO_4TO3;
        u32PixelRepetition = HI_TRUE;
    }
    else if((HI_DRV_DISP_FMT_NTSC == enEncodingFormat)||
        (HI_DRV_DISP_FMT_NTSC_J == enEncodingFormat)||
        (HI_DRV_DISP_FMT_NTSC_443 == enEncodingFormat))
    {
        HI_INFO_HDMI("Set NTS 480I_60000 enTimingMode:0x%x\n", pstAVIInfoFrame->enTimingMode);
        pstAVIInfoFrame->enTimingMode = enEncodingFormat;
        enColorimetry = HDMI_COLORIMETRY_ITU601;
        enAspectRate  = HI_UNF_HDMI_ASPECT_RATIO_4TO3;
        u32PixelRepetition = HI_TRUE;
    }
    else if(HI_DRV_DISP_FMT_861D_640X480_60 == enEncodingFormat)
    {
        HI_INFO_HDMI("Set 640X480P_60000 enTimingMode:0x%x\n", pstAVIInfoFrame->enTimingMode);
        pstAVIInfoFrame->enTimingMode = HI_DRV_DISP_FMT_861D_640X480_60;
        enColorimetry      = HDMI_COLORIMETRY_ITU601;
        enAspectRate       = HI_UNF_HDMI_ASPECT_RATIO_4TO3;
        u32PixelRepetition = HI_FALSE;

        enRGBQuantization  = HDMI_RGB_QUANTIZATION_FULL_RANGE;
        enVidOutMode = HI_UNF_HDMI_VIDEO_MODE_RGB444;
    }
#if defined (DVI_SUPPORT)
    else if ((HI_DRV_DISP_FMT_VESA_800X600_60 <= enEncodingFormat) && (HI_DRV_DISP_FMT_VESA_2048X1152_60 >= enEncodingFormat))
    {
        HI_INFO_HDMI("DVI timing mode enTimingMode:0x%x\n", pstAVIInfoFrame->enTimingMode);
        DEBUG_PRINTK("Force to DVI Mode\n");
        bHDMIMode = HI_FALSE;

        pstAVIInfoFrame->enTimingMode = HI_DRV_DISP_FMT_861D_640X480_60;
        enColorimetry      = HDMI_COLORIMETRY_ITU601;
        enAspectRate       = HI_UNF_HDMI_ASPECT_RATIO_4TO3;
        u32PixelRepetition = HI_FALSE;

        enRGBQuantization  = HDMI_RGB_QUANTIZATION_FULL_RANGE;

        enVidOutMode = HI_UNF_HDMI_VIDEO_MODE_RGB444;        
    }
#endif
    HI_INFO_HDMI("enVidOutMode:%d\n", enVidOutMode);

    //xvYcc??
    pstAVIInfoFrame->enTimingMode = stHDMIAttr.stAttr.enVideoFmt; 
    pstAVIInfoFrame->enOutputType = enVidOutMode;   
    pstAVIInfoFrame->bActive_Infor_Present = HI_TRUE;  
    pstAVIInfoFrame->enBarInfo = HDMI_BAR_INFO_NOT_VALID; 
    pstAVIInfoFrame->enScanInfo = HDMI_SCAN_INFO_NO_DATA; 
    pstAVIInfoFrame->enColorimetry = enColorimetry; 
    pstAVIInfoFrame->enAspectRatio = enAspectRate;
    pstAVIInfoFrame->enActiveAspectRatio = enAspectRate;   
    pstAVIInfoFrame->enPictureScaling = HDMI_PICTURE_NON_UNIFORM_SCALING; 
    //pstAVIInfoFrame->enRGBQuantization = ;  
    pstAVIInfoFrame->bIsITContent = HI_FALSE;  
    pstAVIInfoFrame->u32PixelRepetition = u32PixelRepetition; 
    //pstAVIInfoFrame->enYCCQuantization = ; 
    pstAVIInfoFrame->u32LineNEndofTopBar = 0; 
    pstAVIInfoFrame->u32LineNStartofBotBar = 0; 
    pstAVIInfoFrame->u32PixelNEndofLeftBar = 0; 
    pstAVIInfoFrame->u32PixelNStartofRightBar = 0;  

    return HI_SUCCESS;
}

static HI_U32 hdmi_AdjustAUDInfoFrame(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_AUD_INFOFRAME_VER1_S *punAUDInfoFrame)
{
    return HI_SUCCESS;
}

HI_S32 DRV_HDMI_InitNum(HI_UNF_HDMI_ID_E enHdmi)
{
    return (g_HDMIKernelInitNum + g_HDMIUserInitNum);
}

HI_S32 DRV_HDMI_IsGreenChannel(HI_UNF_HDMI_ID_E enHdmi)
{
    return g_stHdmiCommParam.bOpenGreenChannel;
}

HI_S32 DRV_HDMI_ProcNum(HI_UNF_HDMI_ID_E enHdmi)
{
    HI_S32 u32ProcCount = 0;
    HI_S32 index;
    
    for(index = 0; index < MAX_PROCESS_NUM; index++)
    {
        if(HI_TRUE == g_stHdmiChnParam[HI_UNF_HDMI_ID_0].eventList[index].bUsed)
        {
            u32ProcCount++;
        }            
    }
    
    return u32ProcCount;
}

HI_S32 DRV_Get_Def_HDMIMode(HI_VOID)
{
    return g_stHdmiCommParam.enDefaultMode; 
}


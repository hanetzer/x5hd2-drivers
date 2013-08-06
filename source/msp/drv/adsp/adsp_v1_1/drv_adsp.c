/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : drv_adsp_intf_k.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/04/17
  Description   :
  History       :
  1.Date        : 2013/04/17
    Author      : zgjie
    Modification: Created file

******************************************************************************/

/******************************* Include Files *******************************/

/* Sys headers */
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

/* Unf headers */
#include "hi_error_mpi.h"

/* Drv headers */
#include "drv_adsp_private.h"
#include "drv_adsp_ext.h"
#include "hi_audsp_common.h"
#include "hi_audsp_aoe.h"
#ifdef HI_SND_DSP_HW_SUPPORT
/*kfile operation*/
#include "kfile_ops_func.h"
#include "dsp_elf_func.h"
#endif
#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

/***************************** Macro Definition ******************************/
#if defined (HI_SND_AOE_SWSIMULATE_SUPPORT)
extern HI_S32 AOE_SwEngineCreate(HI_VOID);
extern HI_S32 AOE_SwEngineDestory(HI_VOID);
#endif

#ifdef ENA_ADSP_IRQ_PROC
 #define ADSP_IRQ_NUM (38 + 32)           /*interrupt vdector*/
#endif

#define ADSP_NAME "HI_ADSP"

#define ADSP_COMM_USE_SRAM   //default config
//#define ADSP_COMM_USE_DDR

#ifdef HI_SND_DSP_HW_SUPPORT
#define ADSP_ELF_USE_ARRAY  //defaule config
//#define ADSP_ELF_USE_FILE   
#define ADSP_ELFNAME "/mnt/HWAOE"  

#define AOE_COMBUFFER_ADDR_ALIGN (32)
#define ELF_MAXLENGTH  (2*1024*1024)
#define WAITING_LOOP 500
#endif

/*************************** Structure Definition ****************************/

/* Global parameter */
typedef struct
{
    atomic_t                atmOpenCnt;     /* Open times */
    HI_BOOL                 bReady;         /* Init flag */
    ADSP_REGISTER_PARAM_S*  pstProcParam;   /* ADSP Proc functions */

    ADSP_EXPORT_FUNC_S stExtFunc;      /* ADSP extenal functions */
#ifdef ADSP_COMM_USE_DDR  
    HI_CHAR                szComMmzName[16];
    MMZ_BUFFER_S           stComMmz;
#endif
#ifdef HI_SND_DRV_SUSPEND_SUPPORT 
    ADSP_SETTINGS_S     stADSPSetting;
#endif
} ADSP_GLOBAL_PARAM_S;

/***************************** Global Definition *****************************/

/***************************** Static Definition *****************************/

static HI_S32 ADSP_LoadFirmware(ADSP_CODEID_E u32DspCodeId)
{
    HI_S32 sRet = HI_FAILURE;

    switch (u32DspCodeId)
    {
    case ADSP_CODE_AOE:
#if defined (HI_SND_AOE_SWSIMULATE_SUPPORT)
        sRet = AOE_SwEngineCreate();
#else
        sRet = HI_SUCCESS;
#endif

        break;
    default:
        HI_ERR_ADSP("dont support DspCode(%d)\n", u32DspCodeId);
        break;
    }

    return sRet;
}

static HI_S32 ADSP_UnLoadFirmware(ADSP_CODEID_E u32DspCodeId)
{
    HI_S32 sRet = HI_SUCCESS;

    switch (u32DspCodeId)
    {
    case ADSP_CODE_AOE:
#if defined (HI_SND_AOE_SWSIMULATE_SUPPORT)
        sRet = AOE_SwEngineDestory();
#else
        sRet = HI_SUCCESS;
#endif

        break;
    default:
        HI_WARN_ADSP("dont support DspCode(%d)\n", u32DspCodeId);
        break;
    }

    return sRet;
}

static HI_S32 ADSP_GetFirmwareInfo(ADSP_CODEID_E u32DspCodeId, ADSP_FIRMWARE_INFO_S* pstFrm)
{
    if ((HI_NULL == pstFrm))
    {
        HI_ERR_ADSP("Bad param!\n");
        return HI_FAILURE;
    }

    // todo

    return HI_SUCCESS;
}

static ADSP_GLOBAL_PARAM_S s_stAdspDrv =
{
    .atmOpenCnt                  = ATOMIC_INIT(0),
    .bReady                      = HI_FALSE,

    /*
        .stDefCfg =
        {
            .enType         = HI_UNF_VCODEC_TYPE_H264,
            .enMode         = HI_UNF_VCODEC_MODE_NORMAL,
            .u32ErrCover    = 100,
            .bOrderOutput   = 0,
            .u32Priority    = 15
        },
     */
    .pstProcParam                = HI_NULL,

    //.pDmxFunc = HI_NULL,
    //.pVfmwFunc = HI_NULL,
    //.pfnWatermark = HI_NULL,
    .stExtFunc                   =
    {
        .pfnADSP_LoadFirmware    = ADSP_LoadFirmware,
        .pfnADSP_UnLoadFirmware  = ADSP_UnLoadFirmware,
        .pfnADSP_GetFirmwareInfo = ADSP_GetFirmwareInfo,
    }
};

/*********************************** Code ************************************/
#ifdef HI_SND_DSP_HW_SUPPORT

#if defined ADSP_ELF_USE_ARRAY    
HI_UCHAR dsp_elf_array[]  = {
#include "firmware/hififw.dat"
};
#endif

static HI_UCHAR ELFBuf[ELF_MAXLENGTH]={0};
static HI_S32 Dsp_LoadElf(HI_VOID)
{
    
#ifdef  ADSP_ELF_USE_FILE  
    //1. read elf file
    struct file *fpELF;
    HI_U32 u32readsize = 0;
    fpELF = kfile_open(ADSP_ELFNAME, O_RDONLY, 0);
    if(HI_NULL == fpELF)
    {
        HI_FATAL_ADSP("ELF file %s open fail\n", ADSP_ELFNAME);
        return HI_FAILURE;
    }
    u32readsize = kfile_read( ELFBuf , ELF_MAXLENGTH , fpELF);
    if(u32readsize <= 0)
    {
        HI_FATAL_ADSP("ELF file  read fail\n");
        kfile_close(fpELF);
        return -EACCES;
    }
    kfile_close(fpELF);
    HI_INFO_ADSP("Read ELF file size 0x%x \n", u32readsize);
	
#elif defined ADSP_ELF_USE_ARRAY  
    memcpy(ELFBuf, dsp_elf_array, sizeof(dsp_elf_array));
    HI_INFO_ADSP("Read ELF array size 0x%x \n", sizeof(dsp_elf_array));
#endif

    //2. load elf file 
    //copy elf to reg and ddr 
    CopyELFSection(ELFBuf);
    //check elf copyed right
    if (CheckELFPaser(ELFBuf) == HI_FAILURE)
    {
        HI_FATAL_ADSP("ELF Load  fail\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static HI_S32 Dsp_ResetBoot(HI_U32 u32RunningFlagVirtAddr)
{
    HI_U32 u32RegDSPCrgVirAddr;
    HI_U32 u32RegDSPCtlVirAddr;
    volatile HI_U32 u32RunFlagCheck = 0;
    volatile HI_U32 u32WaitingLoop = 0;
    
    volatile S_DSP_SYS_CRG *pDSPSysCrg = HI_NULL;
    volatile S_DSP_CTL *pDSPCtlReg = HI_NULL;
    HI_U32 *pdsp0_running_flag_addr = HI_NULL;

    //remap reg 
    u32RegDSPCrgVirAddr = (HI_U32 )IO_ADDRESS(DSP_SYSCRG_REGBASE);
    pDSPSysCrg = (S_DSP_SYS_CRG *)u32RegDSPCrgVirAddr;

    u32RegDSPCtlVirAddr = (HI_U32 )IO_ADDRESS(DSP_CTL_REGBASE);
    pDSPCtlReg = (S_DSP_CTL *)u32RegDSPCtlVirAddr;

    //3. reset dsp
    pDSPCtlReg->DSP0_CTRL.bits.wdg2_en_dsp1 = 0;
    pDSPCtlReg->DSP0_CTRL.bits.wdg1_en_dsp0 = 0;
    pDSPCtlReg->DSP0_CTRL.bits.ocdhaltonreset_dsp0 = 0;
#if 1   //boot from bootreg
    pDSPCtlReg->DSP0_CTRL.bits.statvectorsel_dsp0 = 0;
#else //boot from ddr 
    pDSPCtlReg.DSP0_CTRL.bits.statvectorsel_dsp0 = 1;
#endif
    pDSPCtlReg->DSP0_CTRL.bits.runstall_dsp0 = 0;

    pDSPSysCrg->DSP_CRG.u32= 0x77;

    pDSPSysCrg->DSP_CRG.bits.dsp_clk_sel = 0;  //345.6M
    //pDSPSysCrg->DSP_CRG.bits.dsp_cken = 1;
    pDSPSysCrg->DSP_CRG.bits.dsp0_cken = 1;   //dsp0 clk enable
    pDSPSysCrg->DSP_CRG.bits.dsp1_cken = 0;   //dsp1 clk disable

    pDSPSysCrg->DSP_CRG.bits.dsp_srst_req = 1;   //dsp bus sreset
    pDSPSysCrg->DSP_CRG.bits.dsp0_srst_req = 1;   //dsp0 sreset
    pDSPSysCrg->DSP_CRG.bits.dsp1_srst_req = 1;   //dsp0 sreset

    pDSPSysCrg->DSP_CRG.bits.dsp_srst_req = 0;   
    pDSPSysCrg->DSP_CRG.bits.dsp0_srst_req = 0;

    //4. check dsp running
    HI_INFO_ADSP("u32RunningFlagVirtAddr=0x%x  \n", u32RunningFlagVirtAddr);
    pdsp0_running_flag_addr = (HI_U32 *)u32RunningFlagVirtAddr;
    
    if(HI_NULL == pdsp0_running_flag_addr)
    {
        HI_FATAL_ADSP("IoRemap Dsp0 Running Flag addr  fail\n");
        return HI_FAILURE;
    }
    
    while(0 == u32RunFlagCheck)
    {
        u32WaitingLoop++;
        if(*pdsp0_running_flag_addr != AOE_RUNNING_FLAG)
        {
            if(u32WaitingLoop == WAITING_LOOP)
            {
                HI_FATAL_ADSP("Dsp0 Start TimeOut \n");
                break;
            }
            else 
            {
                HI_INFO_ADSP("...\n");
           }
        }
        else
            u32RunFlagCheck = 1;
    }
    
    if(0 != u32RunFlagCheck)
    {
        HI_INFO_ADSP("Dsp0 running, And The Value 0x%x \n", *(volatile HI_U32)pdsp0_running_flag_addr);
    }
    else
    {
        HI_FATAL_ADSP("Dsp0 running Failed  \n");
        return HI_FAILURE;
    }
        return HI_SUCCESS;
}
#endif


HI_S32 ADSP_DRV_Open(struct inode *inode, struct file  *filp)
{
    if (atomic_inc_return(&s_stAdspDrv.atmOpenCnt) == 1)
    {}

    return HI_SUCCESS;
}

HI_S32 ADSP_DRV_Release(struct inode *inode, struct file  *filp)
{
    /* Not the last close, only close the channel match with the 'filp' */
    if (atomic_dec_return(&s_stAdspDrv.atmOpenCnt) != 0)
    {}
    /* Last close */
    else
    {}

    return HI_SUCCESS;
}

HI_S32 ADSP_DRV_RegisterProc(ADSP_REGISTER_PARAM_S *pstParam)
{
    // todo

    /* Check parameters */
    if (HI_NULL == pstParam)
    {
        return HI_FAILURE;
    }

    s_stAdspDrv.pstProcParam = pstParam;

    /* Create proc */

    return HI_SUCCESS;
}

HI_VOID ADSP_DRV_UnregisterProc(HI_VOID)
{
    // todo

    /* Unregister */

    /* Clear param */
    s_stAdspDrv.pstProcParam = HI_NULL;
    return;
}

HI_S32 ADSP_DRV_Suspend(PM_BASEDEV_S *pdev, pm_message_t state)
{
#if defined (HI_SND_DRV_SUSPEND_SUPPORT)
    HI_FATAL_ADSP("entering\n");
    s_stAdspDrv.stADSPSetting.u32ComPhyValue = *(HI_U32 *)IO_ADDRESS(ADSP_COM_PHY_REG_BASE);
    s_stAdspDrv.stADSPSetting.u32ComVirValue  = *(HI_U32 *)IO_ADDRESS(ADSP_COM_VIR_REG_BASE);
#endif
    // todo
    HI_FATAL_ADSP("ok\n");
    return HI_SUCCESS;
}

HI_S32 ADSP_DRV_Resume(PM_BASEDEV_S *pdev)
{
#if defined (HI_SND_DRV_SUSPEND_SUPPORT)
    HI_FATAL_ADSP("entering\n");
    *(HI_U32 *)IO_ADDRESS(ADSP_COM_PHY_REG_BASE) = s_stAdspDrv.stADSPSetting.u32ComPhyValue;
    *(HI_U32 *)IO_ADDRESS(ADSP_COM_VIR_REG_BASE) = s_stAdspDrv.stADSPSetting.u32ComVirValue;
#endif
    // todo
    HI_FATAL_ADSP("ok\n");
    return HI_SUCCESS;
}

HI_S32 ADSP_DRV_Init(HI_VOID)
{
    HI_S32 s32Ret;
    HI_U32 u32DSPComPhyCpuAddr;      //communicate phy cpu addr
    HI_U32 u32DSPComVirCpuAddr;      //communicate vir cpu addr

    // todo

    s32Ret = HI_DRV_MODULE_Register(HI_ID_ADSP, ADSP_NAME, (HI_VOID*)&s_stAdspDrv.stExtFunc);
    if (HI_SUCCESS != s32Ret)
    {
        HI_FATAL_ADSP("Reg module fail:%#x!\n", s32Ret);
        return s32Ret;
    }

    /*  prepare region for dual core communicate */
    u32DSPComPhyCpuAddr = (HI_U32)IO_ADDRESS(ADSP_COM_PHY_REG_BASE);
    u32DSPComVirCpuAddr = (HI_U32)IO_ADDRESS(ADSP_COM_VIR_REG_BASE);
    *(HI_U32 *)u32DSPComPhyCpuAddr = 0;
    *(HI_U32 *)u32DSPComVirCpuAddr = 0;
    
#ifdef ADSP_COMM_USE_DDR   //use ddr
    sprintf(s_stAdspDrv.szComMmzName, "AO_AOECom");
    if (HI_SUCCESS
        != HI_DRV_MMZ_AllocAndMap(s_stAdspDrv.szComMmzName, MMZ_OTHERS, AOE_REG_LENGTH, AOE_COMBUFFER_ADDR_ALIGN, &s_stAdspDrv.stComMmz))
    {
        HI_FATAL_ADSP("Unable to mmz %s \n", s_stAdspDrv.szComMmzName);
        return HI_FAILURE;
    }
    *(HI_U32 *)u32DSPComPhyCpuAddr = s_stAdspDrv.stComMmz.u32StartPhyAddr;
    *(HI_U32 *)u32DSPComVirCpuAddr = s_stAdspDrv.stComMmz.u32StartVirAddr;
    
    HI_INFO_ADSP("u32DSPComPhyCpuAddr: 0x%x!\n", s_stAdspDrv.stComMmz.u32StartPhyAddr);
    HI_INFO_ADSP("u32DSPComVirCpuAddr: 0x%x!\n", s_stAdspDrv.stComMmz.u32StartVirAddr);
    
#elif defined  ADSP_COMM_USE_SRAM             //use sram    ToDo
    *(HI_U32 *)u32DSPComPhyCpuAddr = 0xffff5000;
    *(HI_U32 *)u32DSPComVirCpuAddr = (volatile HI_U32 )ioremap_nocache(0xffff5000, sizeof(HI_U32));
#else
 #error YOU MUST DEFINE COM USE TYPE!
#endif

 #ifdef HI_SND_DSP_HW_SUPPORT
   
    s32Ret = Dsp_LoadElf();
    if (HI_SUCCESS != s32Ret)
    {
        HI_FATAL_ADSP("Dsp load Elf  fail: 0x%x!\n", s32Ret);
        return s32Ret;
    }

    s32Ret = Dsp_ResetBoot(*(HI_U32 *)u32DSPComVirCpuAddr );
    if (HI_SUCCESS != s32Ret)
    {
        HI_FATAL_ADSP("Dsp Boot  fail: 0x%x!\n", s32Ret);
        return s32Ret;
    }
#endif
    
#ifdef ENA_ADSP_IRQ_PROC
    /* register vdec ISR */
    if (0 != request_irq(ADSP_IRQ_NUM, ADSP_IntVdmProc, IRQF_DISABLED, "adsp", HI_NULL))
    {
        HI_FATAL_ADSP("FATAL: request_irq for VDI VDM err!\n");
        return HI_FAILURE;
    }
#endif


    return HI_SUCCESS;
}

HI_VOID ADSP_DRV_Exit(HI_VOID)
{
    // todo
#ifdef ENA_ADSP_IRQ_PROC
    free_irq(ADSP_IRQ_NUM, HI_NULL);
#endif
    HI_DRV_MODULE_UnRegister(HI_ID_ADSP);

#ifdef ADSP_COMM_USE_DDR   //use ddr
    HI_DRV_MMZ_UnmapAndRelease(&s_stAdspDrv.stComMmz);
#elif defined  ADSP_COMM_USE_SRAM             //use sram
#else
 #error YOU MUST DEFINE COM USE TYPE!
#endif

    return;
}

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* End of #ifdef __cplusplus */

/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name       		: 	aud_adac.c
  Version        		: 	Initial Draft
  Author         		: 	Hisilicon multimedia software group
  Created       		: 	2010/02/28
  Last Modified		    :
  Description  		    :  Hifi audio dac interface
  Function List 		:
  History       		:
  1.Date        		: 	2010/02/28
    Author      		: 	z40717
    Modification   	    :	Created file

******************************************************************************/
#include <linux/version.h>
#include <linux/proc_fs.h>
#include <linux/ioport.h>
#include <linux/string.h>
#include <linux/ioctl.h>
#include <linux/fs.h>
#include <linux/delay.h>

#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/seq_file.h>

#include "drv_mmz_ext.h"
#include "drv_stat_ext.h"
#include "drv_sys_ext.h"
#include "drv_proc_ext.h"

#include "hal_tianlai_adac.h"
#include "hi_drv_ao.h"

#define  DBG_ADAC_DISABLE_TIMER


//#define  ADAC_SW_SIMULAUTE
#if defined (ADAC_SW_SIMULAUTE)
static MMZ_BUFFER_S g_stADACSimulateCrgMmz;
static MMZ_BUFFER_S g_stADACSimulatePeriMmz;

static HI_VOID IOAddressMap(HI_VOID)
{
    if (HI_SUCCESS
        != HI_DRV_MMZ_AllocAndMap("adac_syscrg_reg", MMZ_OTHERS, 0x10000, 0x10000, &g_stADACSimulateCrgMmz))
    {
        HI_ERR_AO("Unable to mmz %s \n", "adac_syscrg_reg");
        return;
    }
    memset((HI_VOID*)g_stADACSimulateCrgMmz.u32StartVirAddr,0,0x10000);
    if (HI_SUCCESS
        != HI_DRV_MMZ_AllocAndMap("aiao_sysperi_reg", MMZ_OTHERS, 0x10000, 0x10000, &g_stADACSimulatePeriMmz))
    {
        HI_ERR_AO("Unable to mmz %s \n", "aiao_sysperi_reg");
        return;
    }
    memset((HI_VOID*)g_stADACSimulatePeriMmz.u32StartVirAddr,0,0x10000);

    HI_ERR_AO("adac_syscrg_reg(0x%x) aiao_sysperi_reg(0x%x)\n", g_stADACSimulateCrgMmz.u32StartPhyAddr,
                  g_stADACSimulatePeriMmz.u32StartPhyAddr);
}

static HI_VOID IOaddressUnmap(HI_VOID)
{
    HI_DRV_MMZ_UnmapAndRelease(&g_stADACSimulateCrgMmz);
    HI_DRV_MMZ_UnmapAndRelease(&g_stADACSimulatePeriMmz);
}

#endif

#if defined (ADAC_SW_SIMULAUTE)
#define HI_TIANLAI_CRG_BASE  (g_stADACSimulateCrgMmz.u32StartVirAddr  + 0x2114)
#define HI_TIANLAI_PERI_BASE (g_stADACSimulatePeriMmz.u32StartVirAddr + 0x01b8)
#else
#define HI_TIANLAI_CRG_BASE  IO_ADDRESS(0xf8a22000 + 0x0114)
//#define HI_TIANLAI_PERI_BASE IO_ADDRESS(0xf8a20000 + 0x01b8) /* 95%NL FPGA  */
#define HI_TIANLAI_PERI_BASE IO_ADDRESS(0xf8a20000 + 0x0110)   /* 100%NL FPGA and ASIC */
#endif

#define HI_TIANLAI_CRG_OFFSET  0x0
#define HI_TIANLAI_REG0_OFFSET 0x0
#define HI_TIANLAI_REG1_OFFSET 0x4

#define ADAC_CRG_WRITE_REG(offset, value)  (*(volatile HI_U32*)((HI_U32)(HI_TIANLAI_CRG_BASE) + (offset)) = (value))
#define ADAC_CRG_READ_REG(offset)          (*(volatile HI_U32*)((HI_U32)(HI_TIANLAI_CRG_BASE) + (offset)))

#define ADAC_PERI_WRITE_REG(offset, value) (*(volatile HI_U32*)((HI_U32)(HI_TIANLAI_PERI_BASE) + (offset)) = (value))
#define ADAC_PERI_READ_REG(offset)         (*(volatile HI_U32*)((HI_U32)(HI_TIANLAI_PERI_BASE) + (offset)))

/*----------------------------audio codec-----------------------------------*/

/*
0~0x7f
0:  +6dB
6:   0dB
7f: -121dB
 */
 static HI_VOID Digfi_DacSetVolume(HI_U32 left, HI_U32 right)
{
    SC_PERI_TIANLAI_ADAC0 Adac0;

    Adac0.u32 = ADAC_PERI_READ_REG(HI_TIANLAI_REG0_OFFSET);
    Adac0.bits.dacr_vol = right;
    Adac0.bits.dacl_vol = left;
    ADAC_PERI_WRITE_REG(HI_TIANLAI_REG0_OFFSET, Adac0.u32);
}

#if 0
static HI_VOID Digfi_DacGetVolume(HI_U32 *left, HI_U32 *right)
{
    SC_PERI_TIANLAI_ADAC0 Adac0;

    Adac0.u32  = ADAC_PERI_READ_REG(HI_TIANLAI_REG0_OFFSET);
    *left = Adac0.bits.dacl_vol;
    *right = Adac0.bits.dacr_vol;
}

static HI_S32 Digfi_DacGetMute(HI_VOID)
{
    SC_PERI_TIANLAI_ADAC1 Adac1;

    Adac1.u32 = ADAC_PERI_READ_REG( HI_TIANLAI_REG1_OFFSET);
    return Adac1.bits.smutel;
}
#endif

HI_VOID Digfi_DacSetMute(HI_VOID)
{
    SC_PERI_TIANLAI_ADAC1 Adac1;

    // soft mute digi
    Adac1.u32 = ADAC_PERI_READ_REG( HI_TIANLAI_REG1_OFFSET);
    Adac1.bits.smuter = 1;
    Adac1.bits.smutel = 1;
    ADAC_PERI_WRITE_REG( HI_TIANLAI_REG1_OFFSET, Adac1.u32);
}


HI_VOID Digfi_DacSetUnmute(HI_VOID)
{
    SC_PERI_TIANLAI_ADAC1 Adac1;

    // soft mute digi
    Adac1.u32 = ADAC_PERI_READ_REG( HI_TIANLAI_REG1_OFFSET);
    Adac1.bits.smuter = 0;
    Adac1.bits.smutel = 0;
    ADAC_PERI_WRITE_REG( HI_TIANLAI_REG1_OFFSET, Adac1.u32);
}

HI_VOID Digfi_DacSetSampleRate(HI_UNF_SAMPLE_RATE_E SR)
{
    SC_PERI_TIANLAI_ADAC1 Adac1;

    Adac1.u32  = ADAC_PERI_READ_REG(HI_TIANLAI_REG1_OFFSET);
    switch (SR)
    {
    case HI_UNF_SAMPLE_RATE_176K:
    case HI_UNF_SAMPLE_RATE_192K:
        Adac1.bits.sample_sel = 4;
        break;

    case HI_UNF_SAMPLE_RATE_88K:
    case HI_UNF_SAMPLE_RATE_96K:
        Adac1.bits.sample_sel = 3;
        break;

    case HI_UNF_SAMPLE_RATE_32K:
    case HI_UNF_SAMPLE_RATE_44K:
    case HI_UNF_SAMPLE_RATE_48K:
        Adac1.bits.sample_sel = 2;
        break;

    case HI_UNF_SAMPLE_RATE_16K:
    case HI_UNF_SAMPLE_RATE_22K:
    case HI_UNF_SAMPLE_RATE_24K:
        Adac1.bits.sample_sel = 1;
        break;

    case HI_UNF_SAMPLE_RATE_8K:
    case HI_UNF_SAMPLE_RATE_11K:
    case HI_UNF_SAMPLE_RATE_12K:
        Adac1.bits.sample_sel = 0;
        break;

    default:
        Adac1.bits.sample_sel = 2;
        break;
    }
    ADAC_PERI_WRITE_REG( HI_TIANLAI_REG1_OFFSET, Adac1.u32);

    return;
}

static HI_VOID Digfi_DacPoweup(HI_VOID)
{
    SC_PERI_TIANLAI_ADAC0 Adac0;
    SC_PERI_TIANLAI_ADAC1 Adac1; 

    /* open soft mute */
    Adac1.u32 = ADAC_PERI_READ_REG( HI_TIANLAI_REG1_OFFSET);
    Adac1.bits.smuter = 1;
    Adac1.bits.smutel = 1;
    ADAC_PERI_WRITE_REG( HI_TIANLAI_REG1_OFFSET, Adac1.u32);

    /* step 1: open popfree */
    Adac0.u32 = ADAC_PERI_READ_REG(HI_TIANLAI_REG0_OFFSET);
    Adac0.bits.popfreel = 1;
    Adac0.bits.popfreer = 1;
    ADAC_PERI_WRITE_REG(HI_TIANLAI_REG0_OFFSET, Adac0.u32);

    /*step 2: pd_vref power up	*/
    Adac0.u32 = ADAC_PERI_READ_REG(HI_TIANLAI_REG0_OFFSET);
    Adac0.bits.pd_vref = 0;
    ADAC_PERI_WRITE_REG(HI_TIANLAI_REG0_OFFSET, Adac0.u32);//0xf0

    /*step 3: open DAC */
    Adac0.u32 = ADAC_PERI_READ_REG(HI_TIANLAI_REG0_OFFSET);
    Adac0.bits.pd_dacr = 0;
    Adac0.bits.pd_dacl = 0;
    ADAC_PERI_WRITE_REG(HI_TIANLAI_REG0_OFFSET, Adac0.u32);//0x30

    /*step 4: close profree */
    Adac0.u32 = ADAC_PERI_READ_REG(HI_TIANLAI_REG0_OFFSET);
    Adac0.bits.popfreel = 0;
    Adac0.bits.popfreer = 0;
    ADAC_PERI_WRITE_REG(HI_TIANLAI_REG0_OFFSET, Adac0.u32);//0x00

    /*step 5: disable mute */
    Adac0.u32 = ADAC_PERI_READ_REG(HI_TIANLAI_REG0_OFFSET);
    Adac0.bits.mute_dacl = 0;
    Adac0.bits.mute_dacr = 0;
    ADAC_PERI_WRITE_REG(HI_TIANLAI_REG0_OFFSET, Adac0.u32);

    /* soft unmute */
    Adac1.u32 = ADAC_PERI_READ_REG( HI_TIANLAI_REG1_OFFSET);
    Adac1.bits.smutel = 0;      
    Adac1.bits.smuter = 0;
    Adac1.bits.sunmutel = 1;
    Adac1.bits.sunmuter = 1;
    ADAC_PERI_WRITE_REG( HI_TIANLAI_REG1_OFFSET, Adac1.u32);
    return;
}

static HI_VOID Digfi_DacPowedown(HI_VOID)
{
    SC_PERI_TIANLAI_ADAC0 Adac0;
    SC_PERI_TIANLAI_ADAC1 Adac1;

    // soft mute digi
    Adac1.u32 = ADAC_PERI_READ_REG( HI_TIANLAI_REG1_OFFSET);
    Adac1.bits.smutel = 1;
    Adac1.bits.smuter = 1;
    ADAC_PERI_WRITE_REG( HI_TIANLAI_REG1_OFFSET, Adac1.u32);
    //udelay(1000); // request??

    /*step 1: enable mute */
    Adac0.u32 = ADAC_PERI_READ_REG(HI_TIANLAI_REG0_OFFSET);
    Adac0.bits.mute_dacl = 1;
    Adac0.bits.mute_dacr = 1;
    ADAC_PERI_WRITE_REG(HI_TIANLAI_REG0_OFFSET, Adac0.u32);

    /*step 2: open popfree */
    Adac0.u32 = ADAC_PERI_READ_REG(HI_TIANLAI_REG0_OFFSET);
    Adac0.bits.popfreel = 1;
    Adac0.bits.popfreer = 1;
    ADAC_PERI_WRITE_REG(HI_TIANLAI_REG0_OFFSET, Adac0.u32);

    /*step 3: close DAC */
    Adac0.u32 = ADAC_PERI_READ_REG(HI_TIANLAI_REG0_OFFSET);
    Adac0.bits.pd_dacl = 1;
    Adac0.bits.pd_dacr = 1;
    ADAC_PERI_WRITE_REG(HI_TIANLAI_REG0_OFFSET, Adac0.u32);

     /*step 4: pd_vref power down	*/
    Adac0.u32 = ADAC_PERI_READ_REG(HI_TIANLAI_REG0_OFFSET);
    Adac0.bits.pd_vref = 0;
    ADAC_PERI_WRITE_REG(HI_TIANLAI_REG0_OFFSET, Adac0.u32);

    return;
}


static HI_VOID Digfi_DacInit(HI_UNF_SAMPLE_RATE_E SR)
{
    Digfi_DacPoweup();
    Digfi_DacSetSampleRate(SR);
    Digfi_DacSetVolume(0x06, 0x06);   /* 0dB */
}


/* same as old ADAC */
static HI_VOID Digfi_ADACEnable(HI_VOID)
{
    U_S40_TIANLAI_ADAC_CRG AdacCfg;
    SC_PERI_TIANLAI_ADAC1 Adac1;

    AdacCfg.u32 = ADAC_CRG_READ_REG(HI_TIANLAI_CRG_OFFSET);
    AdacCfg.bits.adac_cken = 1;  /* ADAC  clk en */
    ADAC_CRG_WRITE_REG(HI_TIANLAI_CRG_OFFSET, AdacCfg.u32);
    udelay(10);

    Adac1.u32 = ADAC_PERI_READ_REG( HI_TIANLAI_REG1_OFFSET);
    Adac1.bits.rst = 1;
    ADAC_PERI_WRITE_REG( HI_TIANLAI_REG1_OFFSET, Adac1.u32);
    Adac1.u32 = ADAC_PERI_READ_REG( HI_TIANLAI_REG1_OFFSET);
    Adac1.bits.rst = 0;
    ADAC_PERI_WRITE_REG( HI_TIANLAI_REG1_OFFSET, Adac1.u32);

    AdacCfg.u32 = ADAC_CRG_READ_REG(HI_TIANLAI_CRG_OFFSET);
    AdacCfg.bits.adac_srst_req = 1;  /* ADAC  soft reset */
    ADAC_CRG_WRITE_REG(HI_TIANLAI_CRG_OFFSET, AdacCfg.u32);
    AdacCfg.u32 = ADAC_CRG_READ_REG(HI_TIANLAI_CRG_OFFSET);
    AdacCfg.bits.adac_srst_req = 0; /* ADAC  undo soft reset */
    ADAC_CRG_WRITE_REG(HI_TIANLAI_CRG_OFFSET, AdacCfg.u32);
    return;
}

/* same as old ADAC */
static HI_VOID Digfi_ADACDisable(HI_VOID)
{
    U_S40_TIANLAI_ADAC_CRG AdacCfg;


 
    SC_PERI_TIANLAI_ADAC1 Adac1;
    Adac1.u32 = ADAC_PERI_READ_REG( HI_TIANLAI_REG1_OFFSET);
    Adac1.bits.rst = 1;
    ADAC_PERI_WRITE_REG( HI_TIANLAI_REG1_OFFSET, Adac1.u32);

    AdacCfg.u32 = ADAC_CRG_READ_REG(HI_TIANLAI_CRG_OFFSET);
    AdacCfg.bits.adac_srst_req = 1;  /* ADAC Datapath soft reset */
    ADAC_CRG_WRITE_REG(HI_TIANLAI_CRG_OFFSET, AdacCfg.u32);
    return;
}

/*
The start-up sequence consists on several steps in a pre-determined order as follows:
1. select the master clock mode (256 or 384 x Fs)
2. start the master clock
3. set pdz to high
4. select the sampling rate
5. reset the signal path (rstdpz to low and back to high after 100ns)
6. start the individual codec blocks
 */

HI_VOID ADAC_TIANLAI_Init(HI_UNF_SAMPLE_RATE_E enSR)
{
#if defined (ADAC_SW_SIMULAUTE)
        IOAddressMap();
#endif
    
    Digfi_ADACEnable();
    msleep(1);   //discharge
    Digfi_DacInit(enSR);
}

HI_VOID ADAC_TIANLAI_DeInit(HI_VOID)
{
    Digfi_DacPowedown();
    Digfi_ADACDisable();
#if defined (ADAC_SW_SIMULAUTE)
    IOaddressUnmap();
#endif

    return;
}


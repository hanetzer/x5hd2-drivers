/*******************************************************************************
 *              Copyright 2004 - 2050, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: hal_aiao.c
 * Description: aiao interface of module.
 *
 * History:
 * Version   Date         Author     DefectNum    Description
 * main\1    2012-09-22   z40717     NULL         init.
 ********************************************************************************/

#include <asm/setup.h>
#include <asm/io.h>
#include <mach/hardware.h>
#include <mach/platform.h>
#include <linux/delay.h>

#include "hi_type.h"
#include "hi_module.h"
#include "drv_struct_ext.h"
#include "drv_dev_ext.h"
#include "drv_proc_ext.h"
#include "drv_stat_ext.h"

#include "drv_module_ext.h"
#include "drv_mmz_ext.h"

#include "hi_drv_ao.h"
#include "hi_drv_ai.h"
#include "hal_aiao.h"
#include "hal_aiao_priv.h"
#include "hal_aiao_func.h"
#ifdef ALSA_DEBUG_TIME
#include <linux/time.h>
#endif
#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* End of #ifdef __cplusplus */

typedef struct
{
    HI_HANDLE     hPort[AIAO_INT_BUTT];
    AIAO_IsrFunc *fRegIsr[AIAO_INT_BUTT];
} AIAO_GLOBAL_SOURCE_S;

/* private state */
static AIAO_GLOBAL_SOURCE_S g_AIAORm;

/*
 *  AIAO interrupt handle function
 */
//TO DO
static volatile HI_U32 *pdsp0toa9_int_addr;
volatile HI_U32 aiao_isr_num = 0;
EXPORT_SYMBOL(aiao_isr_num);


static irqreturn_t AIAOIsr(int irq, void * dev_id)
{
    HI_U32 Id;
    HI_U32 TopIntStatus;
    AIAO_PORT_ID_E enPortId;
    HI_HANDLE hPort;

    TopIntStatus = iHAL_AIAO_GetTopIntStatus();
    for (Id = 0; Id < (HI_U32)AIAO_INT_BUTT; Id++)
    {
        enPortId = ID2PORT(Id);         //to verify .......+++
        if (Port2IntStatus(enPortId, TopIntStatus))
        {
            HI_U32 PortIntRawStatus = iHAL_AIAO_P_GetIntStatusRaw(enPortId);
	        //TO DO
            if(AIAO_PORT_TX2 == enPortId)
            {
               ACCESS_ONCE(aiao_isr_num)++;
               *pdsp0toa9_int_addr = 1;
            }

            hPort = g_AIAORm.hPort[Id];
            if (hPort)
            {
                //enPortId = ID2PORT(Id);
                if (g_AIAORm.fRegIsr[Id])
                {
                    g_AIAORm.fRegIsr[Id](ID2PORT(Id), PortIntRawStatus);
                }
            }

            iHAL_AIAO_P_ClrInt(enPortId, PortIntRawStatus);
        }
    }

    return IRQ_HANDLED;
}

HI_S32    HAL_AIAO_HwReset(HI_VOID)
{
    return AIAO_HW_Reset();
}

HI_S32    HAL_AIAO_RequestIsr(HI_VOID)
{
    if (request_irq(AIAO_IRQ_NUM, AIAOIsr, IRQF_DISABLED, "aiao", NULL) != 0)
    {
        HI_FATAL_AIAO("request_irq failed irq num =%d!\n", AIAO_IRQ_NUM);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_VOID    HAL_AIAO_FreeIsr(HI_VOID)
{
    /* free irq */
    free_irq(AIAO_IRQ_NUM, NULL);
}


/* global function */
HI_S32                  HAL_AIAO_Init(HI_VOID)
{
    HI_U32 Id;

    //TO DO
    pdsp0toa9_int_addr = (volatile HI_U32 *)ioremap_nocache((HI_U32)(0xf8a200c8), sizeof(HI_U32));

    if(HI_SUCCESS!=iHAL_AIAO_Init())
    {
        HI_FATAL_AIAO("iHAL_AIAO_Init failed \n");
        return HI_FAILURE;
    }
    /* init aiao state */
    for (Id = 0; Id < (HI_U32)AIAO_INT_BUTT; Id++)
    {
        g_AIAORm.hPort[Id] = HI_NULL;
    }

    if (HI_SUCCESS!=HAL_AIAO_RequestIsr())
    {
        HI_FATAL_AIAO("request_irq failed\n");
        return HI_FAILURE;
    }

    iHAL_AIAO_SetTopInt(0xffffffff);  /* enable all top interrupt */

    return HI_SUCCESS;
}

HI_VOID                 HAL_AIAO_DeInit(HI_VOID)
{
    HI_U32 Id;
    //TO DO
    iounmap(pdsp0toa9_int_addr);

    iHAL_AIAO_SetTopInt(0);  /* disable all top interrupt */
    HAL_AIAO_FreeIsr();
    
    /* close port */
    for (Id = 0; Id < (HI_U32)AIAO_INT_BUTT; Id++)
    {
        if (g_AIAORm.hPort[Id])
        {
            iHAL_AIAO_P_Close(g_AIAORm.hPort[Id]);
        }

        g_AIAORm.hPort[Id]   = HI_NULL;
        g_AIAORm.fRegIsr[Id] = HI_NULL;
    }

    iHAL_AIAO_DeInit();
}

HI_VOID                 HAL_AIAO_GetHwCapability(HI_U32 *pu32Capability)
{
    iHAL_AIAO_GetHwCapability(pu32Capability);
}

HI_VOID                 HAL_AIAO_GetHwVersion(HI_U32 *pu32Version)
{
    iHAL_AIAO_GetHwVersion(pu32Version);
}

HI_VOID                 HAL_AIAO_DBG_RWReg(AIAO_Dbg_Reg_S *pstReg)
{
    iHAL_AIAO_DBG_RWReg(pstReg);
}

HI_VOID                 HAL_AIAO_SetTopInt(HI_U32 u32Multibit)
{
    iHAL_AIAO_SetTopInt(u32Multibit);
}

HI_U32                  HAL_AIAO_GetTopIntRawStatus(HI_VOID)
{
    return iHAL_AIAO_GetTopIntRawStatus();
}

HI_U32                  HAL_AIAO_GetTopIntStatus(HI_VOID)
{
    return iHAL_AIAO_GetTopIntStatus();
}

/* global port function */
HI_S32                  HAL_AIAO_P_Open(AIAO_PORT_ID_E enPortID, const AIAO_PORT_USER_CFG_S *pstConfig)
{
    HI_S32 Ret = HI_SUCCESS;
    HI_U32 Id = PORT2ID(enPortID);
    AIAO_IsrFunc *pisr;

    if (HI_NULL == g_AIAORm.hPort[Id])
    {
        HI_HANDLE hPort;
        Ret = iHAL_AIAO_P_Open(ID2PORT(Id), pstConfig, &hPort, &pisr);
        if (HI_SUCCESS == Ret)
        {
            g_AIAORm.hPort[Id]   = hPort;
            g_AIAORm.fRegIsr[Id] = pisr;
        }
    }

    return Ret;
}

HI_VOID                 HAL_AIAO_P_Close(AIAO_PORT_ID_E enPortID)
{
    HI_U32 Id = PORT2ID(enPortID);

    if (g_AIAORm.hPort[Id])
    {
        iHAL_AIAO_P_Close(g_AIAORm.hPort[Id]);
        g_AIAORm.hPort[Id] = HI_NULL;
    }
}

HI_S32 HAL_AIAO_P_SetAttr(AIAO_PORT_ID_E enPortID, AIAO_PORT_ATTR_S *pstAttr)
{
    HI_U32 Id  = PORT2ID(enPortID);
    HI_S32 Ret = HI_FAILURE;

    if (g_AIAORm.hPort[Id])
    {
        Ret = iHAL_AIAO_P_SetAttr(g_AIAORm.hPort[Id], pstAttr);
    }

    return Ret;
}

HI_S32 HAL_AIAO_P_GetAttr(AIAO_PORT_ID_E enPortID, AIAO_PORT_ATTR_S *pstAttr)
{
    HI_S32 Ret = HI_FAILURE;
    HI_U32 Id = PORT2ID(enPortID);

    if (g_AIAORm.hPort[Id])
    {
        Ret = iHAL_AIAO_P_GetAttr(g_AIAORm.hPort[Id], pstAttr);
    }

    return Ret;
}

HI_S32                  HAL_AIAO_P_Start(AIAO_PORT_ID_E enPortID)
{
    HI_S32 Ret = HI_FAILURE;
    HI_U32 Id = PORT2ID(enPortID);

    if (g_AIAORm.hPort[Id])
    {
        Ret = iHAL_AIAO_P_Start(g_AIAORm.hPort[Id]);
    }

    return Ret;
}

HI_S32                  HAL_AIAO_P_Stop(AIAO_PORT_ID_E enPortID, AIAO_PORT_STOPMODE_E enStopMode)
{
    HI_S32 Ret = HI_FAILURE;
    HI_U32 Id = PORT2ID(enPortID);

    if (g_AIAORm.hPort[Id])
    {
        Ret = iHAL_AIAO_P_Stop(g_AIAORm.hPort[Id], enStopMode);
    }

    return Ret;
}

HI_S32                  HAL_AIAO_P_Mute(AIAO_PORT_ID_E enPortID, HI_BOOL bMute)
{
    HI_S32 Ret = HI_FAILURE;
    HI_U32 Id = PORT2ID(enPortID);

    if (g_AIAORm.hPort[Id])
    {
        Ret = iHAL_AIAO_P_Mute(g_AIAORm.hPort[Id], bMute);
    }

    return Ret;
}

HI_S32                  HAL_AIAO_P_SetSampleRate(AIAO_PORT_ID_E enPortID, HI_UNF_SAMPLE_RATE_E enSampleRate)
{
    //to do
    return HI_SUCCESS;
}


HI_S32                  HAL_AIAO_P_SetVolume(AIAO_PORT_ID_E enPortID, HI_U32 u32VolumedB)
{
    HI_S32 Ret = HI_FAILURE;
    HI_U32 Id = PORT2ID(enPortID);

    if (g_AIAORm.hPort[Id])
    {
        Ret = iHAL_AIAO_P_SetVolume(g_AIAORm.hPort[Id], u32VolumedB);
    }

    return Ret;
}

HI_S32                  HAL_AIAO_P_SetTrackMode(AIAO_PORT_ID_E enPortID, AIAO_TRACK_MODE_E enTrackMode)
{
    HI_S32 Ret = HI_FAILURE;
    HI_U32 Id = PORT2ID(enPortID);

    if (g_AIAORm.hPort[Id])
    {
        Ret = iHAL_AIAO_P_SetTrackMode(g_AIAORm.hPort[Id], enTrackMode);
    }

    return Ret;
}

HI_S32 AIAO_HAL_P_SetBypass(AIAO_PORT_ID_E enPortID, HI_BOOL bByBass)
{
    HI_S32 Ret = HI_FAILURE;
    HI_U32 Id = PORT2ID(enPortID);

    if (g_AIAORm.hPort[Id])
    {
        Ret = iAIAO_HAL_P_SetBypass(g_AIAORm.hPort[Id], bByBass); 
    }

    return Ret;
}

HI_S32                  HAL_AIAO_P_GetUserCongfig(AIAO_PORT_ID_E enPortID, AIAO_PORT_USER_CFG_S *pstUserConfig)
{
    HI_S32 Ret = HI_FAILURE;
    HI_U32 Id = PORT2ID(enPortID);

    if (g_AIAORm.hPort[Id])
    {
        Ret = iHAL_AIAO_P_GetUserCongfig(g_AIAORm.hPort[Id], pstUserConfig);
    }

    return Ret;
}

HI_S32                  HAL_AIAO_P_GetStatus(AIAO_PORT_ID_E enPortID, AIAO_PORT_STAUTS_S *pstProcInfo)
{
    HI_S32 Ret = HI_FAILURE;
    HI_U32 Id = PORT2ID(enPortID);

    if (g_AIAORm.hPort[Id])
    {
        Ret = iHAL_AIAO_P_GetStatus(g_AIAORm.hPort[Id], pstProcInfo);
    }

    return Ret;
}

HI_S32                  HAL_AIAO_P_SelectSpdifSource(AIAO_PORT_ID_E enPortID, AIAO_SPDIFPORT_SOURCE_E eSrcChnId)
{
    HI_S32 Ret = HI_FAILURE;
    HI_U32 Id = PORT2ID(enPortID);

    if (g_AIAORm.hPort[Id])
    {
        Ret = iHAL_AIAO_P_SelectSpdifSource(g_AIAORm.hPort[Id], eSrcChnId);
    }

    return Ret;
}

HI_S32                  HAL_AIAO_P_SetSpdifOutPort(AIAO_PORT_ID_E enPortID, HI_S32 bEn)
{
    HI_S32 Ret = HI_FAILURE;
    HI_U32 Id = PORT2ID(enPortID);

    if (g_AIAORm.hPort[Id])
    {
        Ret = iHAL_AIAO_P_SetSpdifOutPort(g_AIAORm.hPort[Id], bEn);
    }

    return Ret;
}

HI_S32                  HAL_AIAO_P_SetI2SSdSelect(AIAO_PORT_ID_E enPortID, AIAO_I2SDataSel_S  *pstSdSel)
{
    HI_S32 Ret = HI_FAILURE;
    HI_U32 Id = PORT2ID(enPortID);

    if (g_AIAORm.hPort[Id])
    {
        Ret = iHAL_AIAO_P_SetI2SSdSelect(g_AIAORm.hPort[Id], pstSdSel);
    }

    return Ret;
}

/* port buffer function */
HI_U32                  HAL_AIAO_P_ReadData(AIAO_PORT_ID_E enPortID, HI_U8 * pu32Dest, HI_U32 u32DestSize)
{
    HI_U32 ReadBytes = 0;
    HI_U32 Id = PORT2ID(enPortID);

    if (g_AIAORm.hPort[Id])
    {
        ReadBytes = iHAL_AIAO_P_ReadData(g_AIAORm.hPort[Id], pu32Dest, u32DestSize);
    }

    return ReadBytes;
}

HI_U32                  HAL_AIAO_P_WriteData(AIAO_PORT_ID_E enPortID, HI_U8 * pu32Src, HI_U32 u3SrcLen)
{
    HI_U32 WriteBytes = 0;
    HI_U32 Id = PORT2ID(enPortID);

    if (g_AIAORm.hPort[Id])
    {
        WriteBytes = iHAL_AIAO_P_WriteData(g_AIAORm.hPort[Id], pu32Src, u3SrcLen);
    }

    return WriteBytes;
}

HI_U32                  HAL_AIAO_P_PrepareData(AIAO_PORT_ID_E enPortID, HI_U8 * pu32Src, HI_U32 u3SrcLen)
{
    HI_U32 WriteBytes = 0;
    HI_U32 Id = PORT2ID(enPortID);

    if (g_AIAORm.hPort[Id])
    {
        WriteBytes = iHAL_AIAO_P_PrepareData(g_AIAORm.hPort[Id], pu32Src, u3SrcLen);
    }

    return WriteBytes;
}

HI_U32                  HAL_AIAO_P_QueryBufData(AIAO_PORT_ID_E enPortID)
{
    HI_U32 Bytes = 0;
    HI_U32 Id = PORT2ID(enPortID);

    if (g_AIAORm.hPort[Id])
    {
        Bytes = iHAL_AIAO_P_QueryBufData(g_AIAORm.hPort[Id]);
    }

    return Bytes;
}

HI_U32                  HAL_AIAO_P_QueryBufFree(AIAO_PORT_ID_E enPortID)
{
    HI_U32 Bytes = 0;
    HI_U32 Id = PORT2ID(enPortID);

    if (g_AIAORm.hPort[Id])
    {
        Bytes = iHAL_AIAO_P_QueryBufFree(g_AIAORm.hPort[Id]);
    }

    return Bytes;
}

HI_U32                  HAL_AIAO_P_UpdateRptr(AIAO_PORT_ID_E enPortID, HI_U8 * pu32Dest, HI_U32 u32DestSize)
{
    HI_U32 Bytes = 0;
    HI_U32 Id = PORT2ID(enPortID);

    if (g_AIAORm.hPort[Id])
    {
        Bytes = iHAL_AIAO_P_UpdateRptr(g_AIAORm.hPort[Id], pu32Dest, u32DestSize);
    }

    return Bytes;
}

HI_U32                  HAL_AIAO_P_UpdateWptr(AIAO_PORT_ID_E enPortID, HI_U8 * pu32Src, HI_U32 u3SrcLen)
{
    HI_U32 Bytes = 0;
    HI_U32 Id = PORT2ID(enPortID);

    if (g_AIAORm.hPort[Id])
    {
        Bytes = iHAL_AIAO_P_UpdateWptr(g_AIAORm.hPort[Id], pu32Src, u3SrcLen);
    }

    return Bytes;
}

HI_VOID                  HAL_AIAO_P_GetDelayMs(AIAO_PORT_ID_E enPortID, HI_U32 * pu32Delayms)
{
    HI_U32 Id = PORT2ID(enPortID);

    *pu32Delayms = 0;

    if (g_AIAORm.hPort[Id])
    {
        iHAL_AIAO_P_GetDelayMs(g_AIAORm.hPort[Id],pu32Delayms);
        return ;
    }

    return ;
}


HI_S32                  HAL_AIAO_P_GetRbfAttr(AIAO_PORT_ID_E enPortID, AIAO_RBUF_ATTR_S *pstRbfAttr)
{
    HI_S32 Ret = HI_FAILURE;
    HI_U32 Id = PORT2ID(enPortID);

    if (g_AIAORm.hPort[Id])
    {
        Ret = iHAL_AIAO_P_GetRbfAttr(g_AIAORm.hPort[Id], pstRbfAttr);
    }

    return Ret;
}

HI_VOID HAL_AIAO_P_ProcStatistics(AIAO_PORT_ID_E enPortID, HI_U32 u32IntStatus)
{
    HI_U32 Id = PORT2ID(enPortID);

    if (g_AIAORm.hPort[Id])
    {
        iHAL_AIAO_P_ProcStatistics(g_AIAORm.hPort[Id], u32IntStatus);
    }
}


//#define PERIOND_NUM 2  /* 4/16 */
#define PERIOND_NUM 4  /* 4/16 */

static AIAO_PORT_USER_CFG_S g_stAiaoTxI2SDefaultOpenAttr =
{
    .stIfAttr              =
    {
        .enCrgMode         = AIAO_CRG_MODE_MASTER,
        .enChNum           = AIAO_I2S_CHNUM_2,
        .enBitDepth        = AIAO_BIT_DEPTH_16,
        .enRiseEdge        = AIAO_MODE_EDGE_RISE,
        .enRate            = AIAO_SAMPLE_RATE_48K,
        .u32FCLK_DIV       =                     64,
        .u32BCLK_DIV       =                      4,
        .eCrgSource        = AIAO_TX_CRG0,
        .u32PcmDelayCycles =                      1,
        .enI2SMode         = AIAO_MODE_I2S,
        .enSource          = AIAO_TX0,
        .enSD0             = AIAO_I2S_SD0,
        .enSD1             = AIAO_I2S_SD1,
        .enSD2             = AIAO_I2S_SD2,
        .enSD3             = AIAO_I2S_SD3,
        .bMultislot        = HI_FALSE,
    },
    .stBufConfig           =
    {
        .u32PeriodBufSize  = AIAO_DF_PeriodBufSize,
        .u32PeriodNumber   = PERIOND_NUM,
    },
    .enTrackMode           = AIAO_TRACK_MODE_STEREO,
    .enFadeInRate          = AIAO_DF_FadeInRate,
    .enFadeOutRate         = AIAO_DF_FadeOutRate,
    .bMute                 = HI_FALSE,
    .bMuteFade             = HI_TRUE,
    .u32VolumedB           = 0x79,
    .bByBass               = HI_FALSE,
    .pIsrFunc              = HAL_AIAO_P_ProcStatistics,
};

static AIAO_PORT_USER_CFG_S g_stAiaoTxHdmiHbrSDefaultOpenAttr =
{
    .stIfAttr              =
    {
        .enCrgMode         = AIAO_CRG_MODE_MASTER,
        .enChNum           = AIAO_I2S_CHNUM_8,
        .enBitDepth        = AIAO_BIT_DEPTH_16,
        .enRiseEdge        = AIAO_MODE_EDGE_RISE,
        .enRate            = AIAO_SAMPLE_RATE_192K,
        .u32FCLK_DIV       =                     64,
        .u32BCLK_DIV       =                      4,
        .eCrgSource        = AIAO_TX_CRG0,
        .u32PcmDelayCycles =                      1,
        .enI2SMode         = AIAO_MODE_I2S,
        .enSource          = AIAO_TX0,
        .enSD0             = AIAO_I2S_SD0,
        .enSD1             = AIAO_I2S_SD1,
        .enSD2             = AIAO_I2S_SD2,
        .enSD3             = AIAO_I2S_SD3,
        .bMultislot        = HI_FALSE,
    },
    .stBufConfig           =
    {
        .u32PeriodBufSize  = AIAO_DF_PeriodBufSize*16,
        .u32PeriodNumber   = PERIOND_NUM,
    },
    .enTrackMode           = AIAO_TRACK_MODE_STEREO,
    .enFadeInRate          = AIAO_DF_FadeInRate,
    .enFadeOutRate         = AIAO_DF_FadeOutRate,
    .bMute                 = HI_FALSE,
    .bMuteFade             = HI_TRUE,
    .u32VolumedB           = 0x79,
    .bByBass               = HI_TRUE,
    .pIsrFunc              = HAL_AIAO_P_ProcStatistics,
};

static AIAO_PORT_USER_CFG_S g_stAiaoTxHdmiI2SSDefaultOpenAttr =
{
    .stIfAttr              =
    {
        .enCrgMode         = AIAO_CRG_MODE_MASTER,
        .enChNum           = AIAO_I2S_CHNUM_2,
        .enBitDepth        = AIAO_BIT_DEPTH_16,
        .enRiseEdge        = AIAO_MODE_EDGE_RISE,
        .enRate            = AIAO_SAMPLE_RATE_48K,
        .u32FCLK_DIV       =                     64,
        .u32BCLK_DIV       =                      4,
        .eCrgSource        = AIAO_TX_CRG0,
        .u32PcmDelayCycles =                      1,
        .enI2SMode         = AIAO_MODE_I2S,
        .enSource          = AIAO_TX0,
        .enSD0             = AIAO_I2S_SD0,
        .enSD1             = AIAO_I2S_SD1,
        .enSD2             = AIAO_I2S_SD2,
        .enSD3             = AIAO_I2S_SD3,
        .bMultislot        = HI_FALSE,
    },
    .stBufConfig           =
    {
        .u32PeriodBufSize  = AIAO_DF_PeriodBufSize,
        .u32PeriodNumber   = PERIOND_NUM,
    },
    .enTrackMode           = AIAO_TRACK_MODE_STEREO,
    .enFadeInRate          = AIAO_DF_FadeInRate,
    .enFadeOutRate         = AIAO_DF_FadeOutRate,
    .bMute                 = HI_FALSE,
    .bMuteFade             = HI_TRUE,
    .u32VolumedB           = 0x79,
    .bByBass               = HI_FALSE,
    .pIsrFunc              = HAL_AIAO_P_ProcStatistics,
};

static AIAO_PORT_USER_CFG_S g_stAiaoTxSpdDefaultOpenAttr =
{
    .stIfAttr             =
    {
        .enCrgMode        = AIAO_CRG_MODE_MASTER,
        .enChNum          = AIAO_I2S_CHNUM_2,
        .enBitDepth       = AIAO_BIT_DEPTH_16,
        .enRate           = AIAO_SAMPLE_RATE_48K,
        .u32FCLK_DIV      =                    128,
        .u32BCLK_DIV      =                      2,
    },
    .stBufConfig          =
    {
        .u32PeriodBufSize = AIAO_DF_PeriodBufSize,
        .u32PeriodNumber  = PERIOND_NUM,
    },
    .enTrackMode          = AIAO_TRACK_MODE_STEREO,
    .enFadeInRate         = AIAO_DF_FadeInRate,
    .enFadeOutRate        = AIAO_DF_FadeOutRate,
    .bMute                = HI_FALSE,
    .bMuteFade            = HI_TRUE,
    .u32VolumedB          = 0x79,
    .bByBass              = HI_FALSE,
    .pIsrFunc              = HAL_AIAO_P_ProcStatistics,
};
static AIAO_I2S_SOURCE_E g_enTxI2SSourceTab[8] =
{
    AIAO_TX0,
    AIAO_TX1,
    AIAO_TX2,
    AIAO_TX3,
    AIAO_TX4,
    AIAO_TX5,
    AIAO_TX6,
    AIAO_TX7,
};
static AIAO_CRG_SOURCE_E g_enTxCrgSourceTab[8] =
{
    AIAO_TX_CRG0,
    AIAO_TX_CRG1,
    AIAO_TX_CRG2,
    AIAO_TX_CRG3,
    AIAO_TX_CRG4,
    AIAO_TX_CRG5,
    AIAO_TX_CRG6,
    AIAO_TX_CRG7,
};



HI_VOID HAL_AIAO_P_GetTxI2SDfAttr(AIAO_PORT_ID_E enPortID, AIAO_PORT_USER_CFG_S *pAttr)
{
    memcpy(pAttr, &g_stAiaoTxI2SDefaultOpenAttr, sizeof(AIAO_PORT_USER_CFG_S));
    pAttr->stIfAttr.enSource   = g_enTxI2SSourceTab[PORT2CHID(enPortID)];
    pAttr->stIfAttr.eCrgSource = g_enTxCrgSourceTab[PORT2CHID(enPortID)];
}

HI_VOID HAL_AIAO_P_GetHdmiHbrDfAttr(AIAO_PORT_ID_E enPortID, AIAO_PORT_USER_CFG_S *pAttr)
{
    memcpy(pAttr, &g_stAiaoTxHdmiHbrSDefaultOpenAttr, sizeof(AIAO_PORT_USER_CFG_S));
    pAttr->stIfAttr.enSource   = g_enTxI2SSourceTab[PORT2CHID(enPortID)];
    pAttr->stIfAttr.eCrgSource = g_enTxCrgSourceTab[PORT2CHID(enPortID)];
}

HI_VOID HAL_AIAO_P_GetHdmiI2SDfAttr(AIAO_PORT_ID_E enPortID, AIAO_PORT_USER_CFG_S *pAttr)
{
    memcpy(pAttr, &g_stAiaoTxHdmiI2SSDefaultOpenAttr, sizeof(AIAO_PORT_USER_CFG_S));
    pAttr->stIfAttr.enSource   = g_enTxI2SSourceTab[PORT2CHID(enPortID)];
    pAttr->stIfAttr.eCrgSource = g_enTxCrgSourceTab[PORT2CHID(enPortID)];
}

HI_VOID HAL_AIAO_P_GetTxSpdDfAttr(AIAO_PORT_ID_E enPortID, AIAO_PORT_USER_CFG_S *pAttr)
{
    memcpy(pAttr, &g_stAiaoTxSpdDefaultOpenAttr, sizeof(AIAO_PORT_USER_CFG_S));
}

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

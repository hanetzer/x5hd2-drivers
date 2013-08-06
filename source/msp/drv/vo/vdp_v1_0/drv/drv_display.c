
/******************************************************************************
  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : drv_display.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2012/12/30
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/

#include "drv_display.h"
#include "drv_disp_com.h"
#include "drv_disp_priv.h"
#include "drv_disp_hal.h"
#include "drv_disp_da.h"

#ifdef __DISP_PLATFORM_BOOT__
#include "drv_hdmi_ext.h"
#endif

#ifdef HI_DISP_BUILD_FULL
#include "drv_disp_isr.h"
#include "drv_disp_ua.h"
#include "drv_disp_cast.h"
#include "drv_module_ext.h"
#include "drv_hdmi_ext.h"
#include "drv_sys_ext.h"
#endif

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

/******************************************************************************
    global object
******************************************************************************/
static volatile HI_S32 s_s32DisplayGlobalFlag = DISP_DEVICE_STATE_CLOSE;
static DISP_DEV_S s_stDisplayDevice;

/* OPTM ISR handle */
HI_U32 g_DispIrqHandle = 0;


/******************************************************************************
    local function and macro
******************************************************************************/
#define DEF_DRV_DISP_INTER_FUNCTION_AND_MACRO_START_HERE

#define DispCheckDeviceState()    \
{                                \
    if (DISP_DEVICE_STATE_OPEN != s_s32DisplayGlobalFlag)  \
    {                            \
        DISP_ERROR("DISP ERROR! VO is not inited in %s!\n", __FUNCTION__); \
        return HI_ERR_DISP_NO_INIT;  \
    }                             \
}

#define DispCheckNullPointer(ptr) \
{                                \
    if (!ptr)                    \
    {                            \
        DISP_ERROR("DISP ERROR! Input null pointer in %s!\n", __FUNCTION__); \
        return HI_ERR_DISP_NULL_PTR;  \
    }                             \
}

#define DispCheckID(id)    \
{                                \
    if ( (id > HI_DRV_DISPLAY_BUTT) || (id > HI_DRV_DISPLAY_BUTT))  \
    {                            \
        DISP_ERROR("DISP ERROR! Invalid display in %s!\n", __FUNCTION__); \
        return HI_ERR_DISP_INVALID_PARA;  \
    }                             \
}

#define DispGetPointerByID(id, ptr)    \
{                                      \
    if (id >= HI_DRV_DISPLAY_BUTT)     \
    {                                     \
        DISP_ERROR("DISP ERROR! Invalid display in %s!\n", __FUNCTION__); \
        return HI_ERR_DISP_INVALID_PARA;  \
    }                                     \
    ptr = &s_stDisplayDevice.stDisp[id - HI_DRV_DISPLAY_0]; \
}

#define DispGetPointerByIDNoReturn(id, ptr)    \
{                                      \
    if (id >= HI_DRV_DISPLAY_BUTT)     \
    {                                     \
        DISP_ERROR("DISP ERROR! Invalid display in %s!\n", __FUNCTION__); \
    }                                     \
    ptr = &s_stDisplayDevice.stDisp[id - HI_DRV_DISPLAY_0]; \
}


#define DispShouldBeOpened(id) \
{                              \
    if(!DISP_IsOpened(id))     \
    {                          \
        DISP_ERROR("DISP ERROR! Display is not opened!\n"); \
        return HI_ERR_DISP_NOT_OPEN;  \
    }                          \
}

HI_VOID DispSetHardwareState(HI_VOID)
{
    s_stDisplayDevice.bHwReseted = HI_TRUE;
    return;
}

HI_VOID DispClearHardwareState(HI_VOID)
{
    s_stDisplayDevice.bHwReseted = HI_FALSE;
    return;
}


HI_VOID DispResetHardware(HI_VOID)
{
    if (!s_stDisplayDevice.bHwReseted)
    {
        s_stDisplayDevice.stIntfOpt.PF_ResetHardware();
        s_stDisplayDevice.bHwReseted = HI_TRUE;

        DISP_WARN("========DispResetHardware=======\n");
    }
}


HI_S32 DispSearchCastHandle(HI_HANDLE hCast, HI_DRV_DISPLAY_E *penDisp)
{
    DISP_S *pstDisp;
    HI_DRV_DISPLAY_E id;

    for (id=HI_DRV_DISPLAY_0; id < HI_DRV_DISPLAY_BUTT; id++)
    {
        if (DISP_IsOpened(id))
        {
            DispGetPointerByID(id, pstDisp);
            if (pstDisp->hCast == hCast)
            {
                *penDisp = id;
                return HI_SUCCESS;
            }
        }
    }

    return HI_FAILURE;
}


HI_S32 DispCheckMaster(HI_DRV_DISPLAY_E enDisp, HI_BOOL bDetach)
{
    if (enDisp != HI_DRV_DISPLAY_0)
    {
        DISP_ERROR("DISP ERROR! Not support display %d as master!\n", (HI_S32)enDisp);
        return HI_ERR_DISP_INVALID_PARA;
    }

    if (bDetach)
    {
        // if detach, check  'eMaster is enDisp'
        if (s_stDisplayDevice.stAttchDisp.eMaster != enDisp)
        {
            DISP_ERROR("DISP ERROR! Display %d is not master!\n", (HI_S32)enDisp);
            return HI_ERR_DISP_INVALID_PARA;
        }
    }
    else
    {
        // if attach, check  'eMaster is exist'; only support one master diaplay.
        if (s_stDisplayDevice.stAttchDisp.eMaster != HI_DRV_DISPLAY_BUTT)
        {
            DISP_ERROR("DISP ERROR! Display master has existed!\n");
            return HI_ERR_DISP_INVALID_OPT;
        }
    }

    return HI_SUCCESS;
}

HI_S32 DispCheckSlace(HI_DRV_DISPLAY_E enDisp, HI_BOOL bDetach)
{
    if (enDisp != HI_DRV_DISPLAY_1)
    {
        DISP_ERROR("DISP ERROR! Not support display %d as slave!\n", (HI_S32)enDisp);
        return HI_ERR_DISP_INVALID_PARA;
    }

    if (bDetach)
    {
        // if detach, check  'eSlave is enDisp'
        if (s_stDisplayDevice.stAttchDisp.eSlave != enDisp)
        {
            DISP_ERROR("DISP ERROR! Display %d is not slave!\n", (HI_S32)enDisp);
            return HI_ERR_DISP_INVALID_PARA;
        }
    }
    else
    {
        // if attach, check  'eSlave is exist'; only support one slave diaplay.
        if (s_stDisplayDevice.stAttchDisp.eSlave != HI_DRV_DISPLAY_BUTT)
        {
            DISP_ERROR("DISP ERROR! Display slave has existed!\n");
            return HI_ERR_DISP_INVALID_OPT;
        }
    }
    
    return HI_SUCCESS;
}


HI_BOOL DispFmtIsStandDefinition(HI_DRV_DISP_FMT_E enEncFmt)
{
    if((enEncFmt >= HI_DRV_DISP_FMT_PAL) &&(enEncFmt <= HI_DRV_DISP_FMT_1440x480i_60) )
    {
        return HI_TRUE;
    }
    else
    {
        return HI_FALSE;
    }
}

HI_DRV_DISP_FMT_E DispTransferFormat(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_FMT_E enEncFmt)
{
    if (HI_DRV_DISPLAY_1 == enDisp)
    {
        if (   (HI_DRV_DISP_FMT_PAL <= enEncFmt)
            && (HI_DRV_DISP_FMT_PAL_Nc >= enEncFmt) )
        {
            return HI_DRV_DISP_FMT_1440x576i_50;
        }

        if (   (HI_DRV_DISP_FMT_SECAM_SIN <= enEncFmt)
            && (HI_DRV_DISP_FMT_SECAM_H >= enEncFmt) )
        {
            return HI_DRV_DISP_FMT_1440x576i_50;
        }

        if (   (HI_DRV_DISP_FMT_PAL_M <= enEncFmt)
            && (HI_DRV_DISP_FMT_NTSC_443 >= enEncFmt) )
        {
            return HI_DRV_DISP_FMT_1440x480i_60;
        }

        return enEncFmt;
    }
    else
    {
        return enEncFmt;
    }
}

HI_S32 DispSetMasterAndSlace(HI_DRV_DISPLAY_E enMaster, HI_DRV_DISPLAY_E enSlave)
{
    s_stDisplayDevice.stAttchDisp.eMaster = enMaster;
    s_stDisplayDevice.stAttchDisp.eSlave  = enSlave;
    
    return HI_SUCCESS;
}

HI_S32 DispCheckReadyForOpen(HI_DRV_DISPLAY_E enDisp)
{
    DISP_S *pstDisp;

    DispGetPointerByID(enDisp, pstDisp);

    // s1 check fmt
    if (HI_DRV_DISP_FMT_BUTT == pstDisp->stSetting.enFormat)
    {
        DISP_ERROR("DISP ERROR! Fmt is not set!\n");
        goto __DispCheckReadyForOpen_fmt_exit;
    }

    return HI_SUCCESS;
    
__DispCheckReadyForOpen_fmt_exit:
    
    return HI_FAILURE;
}

/*==========================================================================
    interface control
*/
#define DEF_DRV_DISP_INTERFACE_CONTROL_START_HERE

HI_S32 DispCheckIntfValid(HI_DRV_DISP_INTF_S *pstIntf)
{
    if (pstIntf->eID >= HI_DRV_DISP_INTF_ID_MAX)
    {
        return HI_FAILURE;
    }

    if (  (pstIntf->eID == HI_DRV_DISP_INTF_YPBPR0)
        &&(pstIntf->eID == HI_DRV_DISP_INTF_VGA0)
        )
    {
        if(  (pstIntf->u8VDAC_Y_G  >= HI_DISP_VDAC_MAX_NUMBER)
           ||(pstIntf->u8VDAC_Pb_B >= HI_DISP_VDAC_MAX_NUMBER)
           ||(pstIntf->u8VDAC_Pr_R >= HI_DISP_VDAC_MAX_NUMBER)
          ) 
        {
            return HI_FAILURE;
        }
    }

    if (pstIntf->eID == HI_DRV_DISP_INTF_SVIDEO0)
    {
        if(  (pstIntf->u8VDAC_Y_G  >= HI_DISP_VDAC_MAX_NUMBER)
           ||(pstIntf->u8VDAC_Pb_B >= HI_DISP_VDAC_MAX_NUMBER)
           ||(pstIntf->u8VDAC_Pr_R != HI_DISP_VDAC_INVALID_ID)
          ) 
        {
            return HI_FAILURE;
        }
    }

    if (pstIntf->eID == HI_DRV_DISP_INTF_CVBS0)
    {
        if(  (pstIntf->u8VDAC_Y_G  >= HI_DISP_VDAC_MAX_NUMBER)
           ||(pstIntf->u8VDAC_Pb_B != HI_DISP_VDAC_INVALID_ID)
           ||(pstIntf->u8VDAC_Pr_R != HI_DISP_VDAC_INVALID_ID)
          ) 
        {
            return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}

HI_BOOL DispCheckIntfExist(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_INTF_S *pstIntf)
{
    DISP_S *pstDisp;
    HI_DRV_DISP_INTF_S *pstIntf2;
    
    DispGetPointerByIDNoReturn(enDisp, pstDisp);

    pstIntf2 = &pstDisp->stSetting.stIntf[pstIntf->eID].stIf;

    if (  (HI_TRUE == pstDisp->stSetting.stIntf[pstIntf->eID].bOpen)
        &&(pstIntf->eID         == pstIntf2->eID)
        &&(pstIntf->u8VDAC_Y_G  == pstIntf2->u8VDAC_Y_G)
        &&(pstIntf->u8VDAC_Pb_B == pstIntf2->u8VDAC_Pb_B)
        &&(pstIntf->u8VDAC_Pr_R == pstIntf2->u8VDAC_Pr_R)
        )
    {
        return HI_TRUE;
    }
    else
    {
        return HI_FALSE;
    }
}

HI_BOOL DispCheckIntfExistByType(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_INTF_S *pstIntf)
{
    DISP_S *pstDisp;
    
    DispGetPointerByIDNoReturn(enDisp, pstDisp);

    if (HI_TRUE == pstDisp->stSetting.stIntf[pstIntf->eID].bOpen)
    {
        return HI_TRUE;
    }
    else
    {
        return HI_FALSE;
    }
}


HI_VOID DispCleanIntf(DISP_INTF_S *pstIntf)
{
    DISP_MEMSET(pstIntf, 0, sizeof(DISP_INTF_S));
    
    pstIntf->bOpen = HI_FALSE;
    pstIntf->bLinkVenc = HI_FALSE;
    pstIntf->eVencId = DISP_VENC_MAX;

/*
    for(i=0; i<DISP_VENC_SIGNAL_MAX_NUMBER; i++)
    {
        pstIntf->eSignal[i] = HI_DRV_DISP_VDAC_NONE;
    }
*/

    pstIntf->stIf.eID = HI_DRV_DISP_INTF_ID_MAX;
    pstIntf->stIf.u8VDAC_Y_G  = HI_DISP_VDAC_INVALID_ID;
    pstIntf->stIf.u8VDAC_Pb_B = HI_DISP_VDAC_INVALID_ID;
    pstIntf->stIf.u8VDAC_Pr_R = HI_DISP_VDAC_INVALID_ID;

    return;
}

HI_VOID DispCleanAllIntf(DISP_S *pstDisp)
{
    HI_S32 i;

    for(i=0; i<(HI_S32)HI_DRV_DISP_INTF_ID_MAX; i++)
    {
        DispCleanIntf(&pstDisp->stSetting.stIntf[i]);
    }

    return;
}

DISP_INTF_S *DispGetIntfPtr(DISP_S *pstDisp, HI_DRV_DISP_INTF_ID_E eID)
{
    return &pstDisp->stSetting.stIntf[eID];
}



HI_S32 DispAddIntf(DISP_S *pstDisp, HI_DRV_DISP_INTF_S *pstIntf)
{
    DISP_INTF_S *pstIt = &pstDisp->stSetting.stIntf[pstIntf->eID];
    DISP_INTF_OPERATION_S *pfOpt = DISP_HAL_GetOperationPtr();
    HI_DRV_DISP_INTF_S stBackup;
    HI_BOOL bBkFlag = HI_FALSE;
    HI_S32 nRet = HI_SUCCESS;

    /* if intf exist, release firstly */
    if(pstIt->bOpen)
    {
        bBkFlag = HI_TRUE;

        stBackup = pstIt->stIf;

        // s1 release vdac
        pfOpt->PF_ReleaseIntf2(pstDisp->enDisp, pstIt);
    }

    /* clean */
    DispCleanIntf(pstIt);

    DISP_PRINT("DispAddIntf  pstIntf->u8VDAC_Y_G = %d\n", pstIntf->u8VDAC_Y_G);    

    pstIt->stIf = *pstIntf;
    nRet = pfOpt->PF_AcquireIntf2(pstDisp->enDisp, pstIt);
    if (nRet)
    {
        DISP_ERROR("DISP %d acquire vdac %d failed\n", pstDisp->enDisp, pstIntf->u8VDAC_Y_G);
        goto __SET_BACKUP__;
    }

    pstIt->bOpen = HI_TRUE;

    return HI_SUCCESS;

__SET_BACKUP__:
    if (bBkFlag == HI_TRUE)
    {
        DispAddIntf(pstDisp, &stBackup);
    }
    
    return nRet;
}


/*==========================================================================
    video encoding
*/
HI_S32 DispProduceDisplayInfo(DISP_S *pstDisp, HI_DISP_DISPLAY_INFO_S *pstInfo)
{
    DISP_HAL_ENCFMT_PARAM_S stFmt;
    HI_DRV_DISP_FMT_E eFmt = pstDisp->stSetting.enFormat;
    HI_S32 nRet;

    if (!pstDisp)
    {
        DISP_ERROR("Found null pointer in %s\n", __FUNCTION__);
        return HI_FAILURE;
    }

    if (eFmt < HI_DRV_DISP_FMT_CUSTOM)
    {
        nRet = DISP_HAL_GetEncFmtPara(eFmt, &stFmt);

        pstInfo->bIsMaster = pstDisp->bIsMaster;
        pstInfo->bIsSlave  = pstDisp->bIsSlave;
        //printk("id=%d, bm=%d, bs=%d\n", pstDisp->enDisp, pstInfo->bIsMaster,  pstInfo->bIsSlave);
        pstInfo->enAttachedDisp = pstDisp->enAttachedDisp;


        pstInfo->eDispMode = pstDisp->stSetting.eDispMode;
        pstInfo->bRightEyeFirst = pstDisp->stSetting.bRightEyeFirst;
        pstInfo->bInterlace = stFmt.bInterlace;

        pstInfo->bUseAdjRect = pstDisp->stSetting.bAdjRect;
        pstInfo->stAdjRect   = pstDisp->stSetting.stUsingAdjRect;
        pstInfo->stOrgRect   = stFmt.stOrgRect;
        pstInfo->stRefRect   = stFmt.stRefRect;

        if (pstDisp->stSetting.stColor.enInCS != HI_DRV_CS_DEFAULT)
        {
            pstInfo->eColorSpace = pstDisp->stSetting.stColor.enInCS;
            //printk(">>>>>>>>>>>>>>>> 001 cs =%d\n",  pstInfo->eColorSpace);
        }
        else
        {
            pstInfo->eColorSpace = stFmt.enColorSpace;
            //printk(">>>>>>>>>>>>>>>> 002 cs =%d\n",  pstInfo->eColorSpace);
        }

        if (!pstDisp->stSetting.bCustomRatio)
        {
            pstInfo->stAR = stFmt.stAR;
        }
        else
        {
            pstInfo->stAR.u8ARh = pstDisp->stSetting.u32CustomRatioHeight;
            pstInfo->stAR.u8ARw = pstDisp->stSetting.u32CustomRatioWidth;
        }

        pstInfo->u32RefreshRate = stFmt.u32RefreshRate;
        
        pstInfo->u32Bright  = pstDisp->stSetting.stColor.u32Bright;
        pstInfo->u32Contrst = pstDisp->stSetting.stColor.u32Contrst;
        pstInfo->u32Hue     = pstDisp->stSetting.stColor.u32Hue;
        pstInfo->u32Satur   = pstDisp->stSetting.stColor.u32Satur;
        pstInfo->u32Kr      = pstDisp->stSetting.stColor.u32Kr;
        pstInfo->u32Kg      = pstDisp->stSetting.stColor.u32Kg;
        pstInfo->u32Kb      = pstDisp->stSetting.stColor.u32Kb;
    }
    else if (eFmt == HI_DRV_DISP_FMT_CUSTOM)
    {
        DISP_ERROR("Not support customer timing now\n");
        return HI_FAILURE;
    }
    else
    {
        DISP_ERROR("Invalid display encoding format now\n");
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

HI_VOID DispInitCSC(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_COLOR_SETTING_S *pstColor)
{
    // s1 CSC

    DISP_MEMSET(pstColor, 0, sizeof(HI_DRV_DISP_COLOR_SETTING_S));

    pstColor->enInCS  = HI_DRV_CS_DEFAULT;
    pstColor->enOutCS = HI_DRV_CS_DEFAULT;

    pstColor->u32Bright  = DISP_DEFAULT_BRIGHT;
    pstColor->u32Contrst = DISP_DEFAULT_CONTRAST;
    pstColor->u32Satur   = DISP_DEFAULT_SATURATION;
    pstColor->u32Hue     = DISP_DEFAULT_HUE;

    pstColor->u32Kr      = DISP_DEFAULT_KR;
    pstColor->u32Kg      = DISP_DEFAULT_KG;
    pstColor->u32Kb      = DISP_DEFAULT_KB;

    // s2 Gamma
    pstColor->bGammaEnable       = HI_FALSE;
    pstColor->bUseCustGammaTable = HI_FALSE; 

    pstColor->pReserve = HI_NULL;
    pstColor->u32Reserve = 0;
    return;
}


HI_VOID DispGetTestInitParam(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_SETTING_S *pstSetting)
{
    if (enDisp == HI_DRV_DISPLAY_0)
    {   
        pstSetting->bIsMaster = HI_TRUE;
        pstSetting->enAttachedDisp = HI_DRV_DISPLAY_1;

        pstSetting->enFormat = HI_DRV_DISP_FMT_1080i_50;
        pstSetting->stIntf[HI_DRV_DISP_INTF_YPBPR0].eID = HI_DRV_DISP_INTF_YPBPR0;
        pstSetting->stIntf[HI_DRV_DISP_INTF_YPBPR0].u8VDAC_Y_G  = 1;
        pstSetting->stIntf[HI_DRV_DISP_INTF_YPBPR0].u8VDAC_Pb_B = 2;
        pstSetting->stIntf[HI_DRV_DISP_INTF_YPBPR0].u8VDAC_Pr_R = 0;
    }
    if (enDisp == HI_DRV_DISPLAY_1)
    {
        pstSetting->bIsSlave = HI_TRUE;
        pstSetting->enAttachedDisp = HI_DRV_DISPLAY_0;
        
        pstSetting->enFormat = HI_DRV_DISP_FMT_PAL;
        pstSetting->stIntf[HI_DRV_DISP_INTF_CVBS0].eID = HI_DRV_DISP_INTF_CVBS0;
        pstSetting->stIntf[HI_DRV_DISP_INTF_CVBS0].u8VDAC_Y_G  = 3;
        pstSetting->stIntf[HI_DRV_DISP_INTF_CVBS0].u8VDAC_Pb_B = HI_DISP_VDAC_INVALID_ID;
        pstSetting->stIntf[HI_DRV_DISP_INTF_CVBS0].u8VDAC_Pr_R = HI_DISP_VDAC_INVALID_ID;
    }

    return;
}

extern HI_S32 DispGetInitParam(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_INIT_PARAM_S *pstSetting);

HI_S32 DispGetInitParamPriv(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_SETTING_S *pstSetting)
{
    HI_DRV_DISP_INTF_ID_E enIf;
    HI_DRV_DISP_INIT_PARAM_S stInitParam;
        
    DISP_MEMSET(pstSetting, 0, sizeof(HI_DRV_DISP_SETTING_S));
    
    if (enDisp > HI_DRV_DISPLAY_1)
    {
        return HI_FAILURE;
    }


    // s1 get init param form PDM 

    // s2 check para, if(ok){return;} else {get default param;}
    pstSetting->u32BootVersion = 0xfffffffful;
#ifdef __DISP_SELF_START__
    pstSetting->bSelfStart = HI_TRUE;
#else
    pstSetting->bSelfStart = HI_FALSE;
#endif

    pstSetting->bGetPDMParam = HI_FALSE;

    pstSetting->bIsMaster = HI_FALSE;
    pstSetting->bIsSlave  = HI_FALSE;
    pstSetting->enAttachedDisp = HI_DRV_DISPLAY_BUTT;

    /* output format */
    pstSetting->eDispMode = DISP_STEREO_NONE;
    
    //pstSetting->enFormat  = HI_DRV_DISP_FMT_1080i_50;
    //pstSetting->stTimgPara;

    if (HI_DRV_DISPLAY_1 == enDisp)
    {
        pstSetting->enFormat  = HI_DRV_DISP_FMT_1080i_50;
#ifdef __DISP_D0_FOLLOW_D1__
    pstSetting->bIsMaster = HI_TRUE;
    pstSetting->bIsSlave  = HI_FALSE;
    pstSetting->enAttachedDisp = HI_DRV_DISPLAY_0;
#endif
    }
    else if (HI_DRV_DISPLAY_0 == enDisp)
    {
        pstSetting->enFormat  = HI_DRV_DISP_FMT_PAL;
#ifdef __DISP_D0_FOLLOW_D1__
    pstSetting->bIsMaster = HI_FALSE;
    pstSetting->bIsSlave  = HI_TRUE;
    pstSetting->enAttachedDisp = HI_DRV_DISPLAY_1;
#endif
    }
    else
    {
        pstSetting->enFormat  = HI_DRV_DISP_FMT_PAL;
    }

    /* about color */
    DispInitCSC(enDisp, &pstSetting->stColor);

    /* background color */
    pstSetting->stBgColor.u8Red   = DISP_DEFAULT_COLOR_RED;
    pstSetting->stBgColor.u8Green = DISP_DEFAULT_COLOR_GREEN;
    pstSetting->stBgColor.u8Blue  = DISP_DEFAULT_COLOR_BLUE;

    /* interface setting */
    for(enIf=HI_DRV_DISP_INTF_YPBPR0; enIf<HI_DRV_DISP_INTF_ID_MAX; enIf++)
    {
        pstSetting->stIntf[enIf].eID = HI_DRV_DISP_INTF_ID_MAX;
        pstSetting->stIntf[enIf].u8VDAC_Y_G  = HI_DISP_VDAC_INVALID_ID;
        pstSetting->stIntf[enIf].u8VDAC_Pb_B = HI_DISP_VDAC_INVALID_ID;
        pstSetting->stIntf[enIf].u8VDAC_Pr_R = HI_DISP_VDAC_INVALID_ID;
    }

    pstSetting->u32LayerNumber = 0;
    //HI_DRV_DISP_LAYER_E enLayer[HI_DRV_DISP_LAYER_BUTT]; /* Z-order is from bottom to top */


    /* about display area */
    pstSetting->bAdjRect = HI_FALSE;

    pstSetting->bCustomRatio = HI_FALSE;
    pstSetting->u32CustomRatioWidth  = 0;
    pstSetting->u32CustomRatioHeight = 0;

    //pstSetting->u32Reseve;
    //pstSetting->pRevData;
    //DispGetTestInitParam(enDisp, pstSetting);
    //DispGetInitParam(enDisp, pstSetting);
    if( DispGetInitParam(enDisp, &stInitParam) == HI_SUCCESS)
    {
        //DISP_PRINT(">>>>>>>>>> DispGetInitParam  ok\n");

        pstSetting->bGetPDMParam = HI_TRUE;
        pstSetting->u32BootVersion = stInitParam.u32Version;

#ifdef __DISP_SELF_START__
        pstSetting->bSelfStart = stInitParam.bSelfStart;
#endif

#ifdef __DISP_D0_FOLLOW_D1__
        pstSetting->bIsMaster = stInitParam.bIsMaster;
        pstSetting->bIsSlave  = stInitParam.bIsSlave;
        pstSetting->enAttachedDisp = stInitParam.enAttachedDisp;
#endif
        pstSetting->enFormat = DispTransferFormat(enDisp, stInitParam.enFormat);
        
        pstSetting->stColor.u32Bright = stInitParam.u32Brightness;
        pstSetting->stColor.u32Contrst = stInitParam.u32Contrast;
        pstSetting->stColor.u32Satur = stInitParam.u32Saturation;
        pstSetting->stColor.u32Hue = stInitParam.u32HuePlus;
        pstSetting->stColor.bGammaEnable = stInitParam.bGammaEnable; 
        pstSetting->stAdjRect.s32X = stInitParam.u32ScreenXpos;
        pstSetting->stAdjRect.s32Y = stInitParam.u32ScreenYpos;
        pstSetting->stAdjRect.s32Width = stInitParam.u32ScreenWidth;
        pstSetting->stAdjRect.s32Height = stInitParam.u32ScreenHeight; 
        pstSetting->stBgColor = stInitParam.stBgColor;

        pstSetting->bCustomRatio = stInitParam.bCustomRatio;
        pstSetting->u32CustomRatioWidth  = stInitParam.u32CustomRatioWidth;
        pstSetting->u32CustomRatioHeight = stInitParam.u32CustomRatioHeight;

        for(enIf=HI_DRV_DISP_INTF_YPBPR0; enIf<HI_DRV_DISP_INTF_ID_MAX; enIf++)
        {
            if (stInitParam.stIntf[enIf].eID != HI_DRV_DISP_INTF_ID_MAX)
            {
                pstSetting->stIntf[enIf] = stInitParam.stIntf[enIf];
                DISP_PRINT(">>>>>>>>>> intf %d id=%d\n", enIf,
                            stInitParam.stIntf[enIf].eID);
            }
        }

        //stInitParam.stDispTiming;
    }

    return HI_SUCCESS;
}


HI_VOID DispParserInitParam(DISP_S *pstDisp, HI_DRV_DISP_SETTING_S *pstSetting)
{
    DISP_INTF_OPERATION_S *pstIntfOpt;
    DISP_HAL_ENCFMT_PARAM_S stFmtPara;
    DISP_SETTING_S *pstS = &pstDisp->stSetting;
    HI_S32 t = 0;
    
    pstIntfOpt = DISP_HAL_GetOperationPtr();
    //DISP_ASSERT(pstIntfOpt);
    pstS->u32Version = DISP_DRVIER_VERSION;
    pstS->u32BootVersion = pstSetting->u32BootVersion;
    pstS->bSelfStart = pstSetting->bSelfStart;
    pstS->bGetPDMParam = pstSetting->bGetPDMParam;

    pstS->eDispMode = pstSetting->eDispMode;
    pstS->bRightEyeFirst = HI_FALSE;
    pstS->enFormat  = pstSetting->enFormat;
    //pstS->bFmtChanged = HI_FALSE;  // TODO

    pstS->stCustomTimg = pstSetting->stCustomTimg;
    if (pstS->enFormat == HI_DRV_DISP_FMT_CUSTOM)
    {
        pstS->bCustomTimingIsSet = HI_TRUE;
    }
    else
    {
        pstS->bCustomTimingIsSet = HI_FALSE;
    }
    pstS->bCustomTimingChange = HI_FALSE;

    /* about color */
    pstS->stColor = pstSetting->stColor;

    /* background color */
    pstS->stBgColor = pstSetting->stBgColor;

    /* interface setting */
    DispCleanAllIntf(pstDisp);

    pstS->u32LayerNumber = 0;
    //HI_DRV_DISP_LAYER_E enLayer[HI_DRV_DISP_LAYER_BUTT]; /* Z-order is from bottom to top */

    /* about sink display screen */
    pstS->bAdjRect = pstSetting->bAdjRect;

    if (pstS->bAdjRect)
    {
        DISP_HAL_GetEncFmtPara(pstS->enFormat, &stFmtPara);

        pstS->stRefScreen = stFmtPara.stRefRect;
        pstS->stRefAdjRect = pstSetting->stAdjRect;
        pstS->stUsingAdjRect = pstS->stRefAdjRect;
    }

    pstS->bCustomRatio = pstSetting->bCustomRatio;
    if(pstS->bCustomRatio)
    {
        pstS->u32CustomRatioWidth = pstSetting->u32CustomRatioWidth;
        pstS->u32CustomRatioHeight = pstSetting->u32CustomRatioHeight;
    }

    pstS->u32Reseve = 0;
    pstS->pRevData  = HI_NULL;

    for(t=0; t<HI_DRV_DISP_INTF_ID_MAX; t++)
    {
        if (pstSetting->stIntf[t].eID < HI_DRV_DISP_INTF_ID_MAX)
        {
            //todo
            DispAddIntf(pstDisp, &pstSetting->stIntf[t]);
        }
    }

    /* for attach display */
    pstDisp->bIsMaster = pstSetting->bIsMaster;
    pstDisp->bIsSlave  = pstSetting->bIsSlave;
    pstDisp->enAttachedDisp = pstSetting->enAttachedDisp;

#if 0    
    DISP_PRINT("FOLLOW INFO: DISP %d, M=%d,S=%d,ATT=%d......\n", 
               pstDisp->enDisp,
               pstDisp->bIsMaster,
               pstDisp->bIsSlave,
               pstDisp->enAttachedDisp);
#endif

#ifdef HI_DISP_BUILD_FULL
    if (pstS->bGetPDMParam)
    {
        HI_BOOL bOutput;
        
        // todo
        pstIntfOpt->PF_GetChnEnable(pstDisp->enDisp, &bOutput);

        if (bOutput)
        {
            pstDisp->bEnable = HI_TRUE;
            DISP_PRINT("DISP %d is working......\n", pstDisp->enDisp);

            // VDP is working, not to reset.
            DispSetHardwareState();
        }
    }
#endif

    return;
}

HI_VOID DispInitDisplay(HI_DRV_DISPLAY_E enDisp)
{
    DISP_S *pstDisp;
    HI_DRV_DISP_SETTING_S stDefSetting;

    DispGetPointerByIDNoReturn(enDisp, pstDisp);

    DISP_MEMSET(pstDisp, 0, sizeof(DISP_S));

    // s1 set id
    pstDisp->enDisp = enDisp;

    // s2 get base parameters
    if (DispGetInitParamPriv(enDisp, &stDefSetting))
    {
        pstDisp->bSupport = HI_FALSE;
        DISP_PRINT("DispGetInitParam  failed\n");
        return;
    }

    pstDisp->bSupport = HI_TRUE;

    //component operation
    pstDisp->pstIntfOpt = &s_stDisplayDevice.stIntfOpt;

    DispParserInitParam(pstDisp, &stDefSetting);

    pstDisp->eState  = DISP_PRIV_STATE_DISABLE;

    pstDisp->bOpen = HI_FALSE;;
        
    //pstDisp->stDispInfo;
    //pstDisp->bDispInfoValid = HI_FALSE;
    DispProduceDisplayInfo(pstDisp, &pstDisp->stDispInfo);
    
    //mirrorcast
    pstDisp->hCast = HI_NULL;

    return;
}


HI_VOID DispDeInitDisplay(HI_DRV_DISPLAY_E enDisp)
{
    DISP_S *pstDisp;
    HI_S32 t;

    DispGetPointerByIDNoReturn(enDisp, pstDisp);

#ifdef HI_DISP_BUILD_FULL
    if (pstDisp->hCast)
    {
        DISP_CastSetEnable(pstDisp->hCast, HI_FALSE);

        DISP_CastDestroy(pstDisp->hCast);
    }
#endif

    for(t=0; t<HI_DRV_DISP_INTF_ID_MAX; t++)
    {
        if (pstDisp->stSetting.stIntf[t].bOpen)
        {
            DISP_INTF_OPERATION_S *pfOpt = DISP_HAL_GetOperationPtr();

            // s1 release vdac
            pfOpt->PF_ReleaseIntf2(pstDisp->enDisp, &pstDisp->stSetting.stIntf[t]);

            // s3 set intf
            DispCleanIntf(&pstDisp->stSetting.stIntf[t]);
        }
    }

    return;
}

HI_S32 DispSetFormat(HI_DRV_DISPLAY_E eDisp, HI_DRV_DISP_FMT_E eFmt)
{
    DISP_S *pstDisp;
    DISP_INTF_OPERATION_S *pfOpt = DISP_HAL_GetOperationPtr();
    HI_U32 u;
    HI_S32 nRet;
    
    DispGetPointerByID(eDisp, pstDisp);

    if (HI_DRV_DISP_FMT_CUSTOM != eFmt)
    {
        // s1 set channel
        nRet = pstDisp->pstIntfOpt->PF_SetChnFmt(pstDisp->enDisp, eFmt);

        // s2 set interface  if necessarily
        for (u=0; u<HI_DRV_DISP_INTF_ID_MAX; u++)
        {
            if (pstDisp->stSetting.stIntf[u].bOpen)
            {
                //DispSetIntfFmt(pstDisp, u, eFmt);
                    //pstDisp->pstIntfOpt->PF_ResetIntfFmt(enIntfId, enEncFmt);
                pfOpt->PF_ResetIntfFmt2(pstDisp->enDisp, &pstDisp->stSetting.stIntf[u], eFmt);
            }
        }

        // s3 produce
        DispProduceDisplayInfo(pstDisp, &pstDisp->stDispInfo);
        //printk("xxxxxxxxxxxxx eFmt = %d\n", eFmt);
    }
    else
    {
        if(!pstDisp->stSetting.bCustomTimingIsSet)
        {
            DISP_ERROR("User customer format without setting timming\n");
            return HI_ERR_DISP_INVALID_PARA;
        }

        DISP_ERROR("User customer format not support now!\n");
        return HI_ERR_DISP_INVALID_OPT;
    }

    return HI_SUCCESS;
}


HI_S32 DispSetColor(DISP_S *pstDisp)
{
#if 0
    HI_S32 nRet;
    DISP_HAL_COLOR_S stColor;
    HI_DRV_DISP_COLOR_SETTING_S *pstC = &pstDisp->stSetting.stColor;
    HI_U32 u;

    stColor.enInputCS  = pstC->enCustomMixCS;
    stColor.enOutputCS = pstC->enCustomOutCS;

    stColor.u32Bright  = pstC->u32Bright;
    stColor.u32Contrst = pstC->u32Contrst;
    stColor.u32Hue     = pstC->u32Hue;
    stColor.u32Satur   = pstC->u32Satur;
    stColor.u32Kr      = pstC->u32Kr;
    stColor.u32Kg      = pstC->u32Kg;
    stColor.u32Kb      = pstC->u32Kb;
    stColor.bGammaEnable = pstC->bGammaEnable;


    // s1 set channel
    nRet = pstDisp->pstIntfOpt->PF_SetChnColor(pstDisp->enDisp, &stColor);

    // s2 set interface if necessarily
    for (u=0; u<HI_DRV_DISP_INTF_ID_MAX; u++)
    {
        if (pstDisp->stSetting.stIntf[u].bOpen)
        {
            //nRet = pstDisp->pstIntfOpt->PF_SetIntfColor(pstDisp->stIntf[u].stIf.eID, pstColor);
        }
    }
#endif

    // s3 set bgc
/*
    DISP_PRINT("====DispSetColor R=%d, G=%d, B=%d\n", pstDisp->stSetting.stBgColor.u8Red,
                                                      pstDisp->stSetting.stBgColor.u8Green,
                                                      pstDisp->stSetting.stBgColor.u8Blue);
*/
    pstDisp->pstIntfOpt->PF_SetChnBgColor(pstDisp->enDisp, 
                                          pstDisp->stSetting.stColor.enOutCS,
                                          &pstDisp->stSetting.stBgColor);

    
    return HI_SUCCESS;
}

HI_S32 DispSetEnable(DISP_S *pstDisp, HI_BOOL bEnable)
{
    DISP_INTF_OPERATION_S *pfOpt = DISP_HAL_GetOperationPtr();
    HI_U32 u;
    HI_S32 nRet;
    
    // s1 set interface if necessarily
    for (u=0; u<HI_DRV_DISP_INTF_ID_MAX; u++)
    {
        if (pstDisp->stSetting.stIntf[u].bOpen)
        {
            //DispSetIntfLink(pstDisp, u);
            pfOpt->PF_SetIntfEnable2(pstDisp->enDisp, &pstDisp->stSetting.stIntf[u], bEnable);
        }
    }

    // s2 set channel
    nRet = pfOpt->PF_SetChnEnable(pstDisp->enDisp, bEnable);

    return HI_SUCCESS;
}


/******************************************************************************
    display function
*****************************************************************************/
#define DEF_DRV_DISP_API_FUNCTION_START_HERE

HI_S32 DISP_GetInitFlag(HI_BOOL *pbInited)
{
    DispCheckNullPointer(pbInited);

    *pbInited = (s_s32DisplayGlobalFlag == DISP_DEVICE_STATE_OPEN) ? HI_TRUE : HI_FALSE;
    
    return HI_SUCCESS;
}

HI_S32 DISP_GetVersion(HI_DRV_DISP_VERSION_S *pstVersion)
{
    DispCheckNullPointer(pstVersion);

    // check whether display is inited.
    DispCheckDeviceState();

    // return version
    DISP_HAL_GetVersion(pstVersion);

    return HI_SUCCESS;
}

HI_BOOL DISP_IsOpened(HI_DRV_DISPLAY_E enDisp)
{
    // s1 check input parameters
    DispCheckDeviceState();

    DispCheckID(enDisp);

    // s2 return display OPEN state
    return s_stDisplayDevice.stDisp[enDisp - HI_DRV_DISPLAY_0].bOpen;
}

HI_BOOL DISP_IsFollowed(HI_DRV_DISPLAY_E enDisp)
{
    // s1 check input parameters
    DispCheckDeviceState();

    DispCheckID(enDisp);

    // s2 return display OPEN state
    return s_stDisplayDevice.stDisp[enDisp - HI_DRV_DISPLAY_0].bIsMaster;
}

#ifdef __DISP_PLATFORM_BOOT__

#else
static HDMI_EXPORT_FUNC_S *s_pstHDMIFunc;
#endif

HI_S32 DISP_Init(HI_VOID)
{
    HI_DRV_DISPLAY_E eDisp;
    HI_DRV_DISP_VERSION_S stDispVersion;
//    HI_S32 nIntNum = -1;
    HI_S32 nRet;


//DISP_PRINT("DISP_Init 001\n");
    if (s_s32DisplayGlobalFlag != DISP_DEVICE_STATE_CLOSE)
    {
        DISP_INFO("DISPLAY has been inited");
        return HI_SUCCESS;
    }

#ifdef __DISP_PLATFORM_BOOT__

#else
    nRet = HI_DRV_MODULE_GetFunction(HI_ID_HDMI, (HI_VOID**)&s_pstHDMIFunc);
    if (nRet)
    {
        DISP_ERROR("DISP_get HDMI funt failed!");
        goto __ERR_EXIT__;
    }
#endif


//DISP_PRINT("DISP_Init 002\n");

    DISP_MEMSET(&s_stDisplayDevice, 0, sizeof(DISP_DEV_S));

    // s1 get interface operation
    nRet = DISP_HAL_Init(DISP_BASE_ADDRESS);
    if (nRet)
    {
        DISP_ERROR("DISP_HAL_Init failed!");
        goto __ERR_EXIT__;
    }

    nRet = DISP_HAL_GetOperation(&s_stDisplayDevice.stIntfOpt);
    nRet = DISP_HAL_GetVersion(&stDispVersion);

    // s1.1 init alg
    nRet = DISP_DA_Init(&stDispVersion);
    if (nRet)
    {
        DISP_ERROR("DISP_DA_Init failed!");
        goto __ERR_EXIT__;
    }


#ifdef HI_DISP_BUILD_FULL
    // s1.2 init alg
    nRet = DISP_UA_Init(&stDispVersion);
    if (nRet)
    {
        DISP_ERROR("DISP_UA_Init failed!");
        goto __ERR_EXIT__;
    }
#endif




    // s2 inited display
    for (eDisp=HI_DRV_DISPLAY_0; eDisp <HI_DRV_DISPLAY_2; eDisp++)
    {
        DispInitDisplay(eDisp);
    }

#ifdef HI_DISP_BUILD_FULL

    // s3 init irq
    nRet = DISP_ISR_Init(&s_stDisplayDevice.stIntfOpt);

    //printk("Reg disp irq %d\n", nIntNum);
    if (request_irq(DISP_INT_NUMBER, (irq_handler_t)DISP_ISR_Main, IRQF_SHARED, "DISP_IRQ", &g_DispIrqHandle) != 0)
    {
        DISP_ERROR("DISP registe IRQ failed!\n");
        nRet = HI_FAILURE;
        goto __ERR_EXIT__;
    }

#endif

//DISP_PRINT("DISP_Init 003\n");

    // todo:
    DispResetHardware();

    s_s32DisplayGlobalFlag = DISP_DEVICE_STATE_OPEN;

    for (eDisp=HI_DRV_DISPLAY_0; eDisp <HI_DRV_DISPLAY_2; eDisp++)
    {
#ifdef __DISP_PLATFORM_BOOT__
        //DISP_PRINT(" disp init set disp\n");
        DISP_Open(eDisp);
#else
        DISP_S *pstDisp;
        DispGetPointerByID(eDisp, pstDisp);

        //DISP_PRINT(" disp init set disp\n");

        if ( pstDisp->stSetting.bSelfStart || pstDisp->bEnable )
        {
            DISP_Open(eDisp);
            //DispSetEnable(pstDisp, HI_TRUE);
            //DISP_PRINT(" disp init set disp %d en\n", eDisp);
        }
#endif
    }


#ifdef __DISP_PLATFORM_BOOT__
    HI_DRV_DISP_FMT_E enEncFmt;
    DISP_GetFormat(HI_DRV_DISPLAY_1,&enEncFmt);

    HI_DRV_HDMI_Init();

    if(HI_SUCCESS != HI_DRV_HDMI_Open(enEncFmt))
    {
        DISP_PRINT("HI_UNF_HDMI_Open Err \n");
    }

    HI_DRV_HDMI_Start();

#else
    if (s_pstHDMIFunc->pfnHdmiInit && s_pstHDMIFunc->pfnHdmiOpen && 
        s_pstHDMIFunc->pfnHdmiPreFormat && s_pstHDMIFunc->pfnHdmiSetFormat)
    {
        //HI_DRV_DISP_FMT_E enEncFmt;
        DISP_PRINT(">>>>>>>>>>>>>>>>>>>>>>>>>>>> disp init hdmi\n");

        s_pstHDMIFunc->pfnHdmiInit();
        s_pstHDMIFunc->pfnHdmiOpen(HI_UNF_HDMI_ID_0);
#if 0 /*--初始化时调用SetFormat会闪一下黑屏--*/
        DISP_GetFormat(HI_DRV_DISPLAY_1,&enEncFmt);
        s_pstHDMIFunc->pfnHdmiPreFormat(HI_UNF_HDMI_ID_0,enEncFmt);
        s_pstHDMIFunc->pfnHdmiSetFormat(HI_UNF_HDMI_ID_0,enEncFmt);
#endif /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
    }
#endif

    
//DISP_PRINT("DISP_Init 004\n");


    return HI_SUCCESS;

__ERR_EXIT__:
#ifdef HI_DISP_BUILD_FULL
    DISP_UA_DeInit();
#endif

    DISP_HAL_DeInit();

    return nRet;
}

HI_S32 DISP_DeInit(HI_VOID)
{
    HI_DRV_DISPLAY_E eDisp;

DISP_PRINT("DISP_DeInit 001\n");

    if (DISP_DEVICE_STATE_CLOSE == s_s32DisplayGlobalFlag)
    {
        DISP_INFO("DISPLAY has NOT inited");
        return HI_SUCCESS;
    }

DISP_PRINT("DISP_DeInit 002\n");

    // s2 inited display
    for (eDisp=HI_DRV_DISPLAY_0; eDisp <HI_DRV_DISPLAY_2; eDisp++)
    {
        DISP_Close(eDisp);
        DispDeInitDisplay(eDisp);
    }

DISP_PRINT("DISP_DeInit 003\n");
    //DISP_MSLEEP(40);
    //s_stDisplayDevice.stIntfOpt.PF_ResetHardware();

#ifdef HI_DISP_BUILD_FULL
    free_irq(DISP_INT_NUMBER, &g_DispIrqHandle);

    DISP_ISR_DeInit();

DISP_PRINT("DISP_DeInit 004\n");


    DISP_UA_DeInit();
#endif

    DISP_DA_DeInit();


    DISP_HAL_DeInit();

    DISP_MEMSET(&s_stDisplayDevice, 0, sizeof(DISP_DEV_S));

DISP_PRINT("DISP_DeInit 004\n");


    s_s32DisplayGlobalFlag = DISP_DEVICE_STATE_CLOSE;

    return HI_SUCCESS;
}


HI_S32 DISP_Suspend(HI_VOID)
{
#ifndef __DISP_PLATFORM_BOOT__
    DISP_S *pstDisp;
    HI_DRV_DISPLAY_E enD;
    //DISP_INTF_OPERATION_S *pfOpt = DISP_HAL_GetOperationPtr();
    
    DispCheckDeviceState();

    if (DISP_DEVICE_STATE_OPEN == s_s32DisplayGlobalFlag)
    {
        for(enD=HI_DRV_DISPLAY_0; enD<HI_DRV_DISPLAY_BUTT; enD++)
        {
            DispGetPointerByIDNoReturn(enD, pstDisp);

            pstDisp->bStateBackup = pstDisp->bEnable;

            if (pstDisp->bEnable == HI_TRUE)
            {
                DISP_SetEnable(enD, HI_FALSE);
            }
        }

        DISP_ISR_Suspend();

        //TODO: CLOSE VDP
      
        DispClearHardwareState();
    
        s_s32DisplayGlobalFlag = DISP_DEVICE_STATE_SUSPEND;
    }
#endif
    return HI_SUCCESS;
}

HI_S32 DISP_Resume(HI_VOID)
{
#ifndef __DISP_PLATFORM_BOOT__
    DISP_S *pstDisp;
    HI_DRV_DISPLAY_E enD;

    if (DISP_DEVICE_STATE_SUSPEND == s_s32DisplayGlobalFlag)
    {
        // 
        DispResetHardware();

        DISP_ISR_Resume();

        s_s32DisplayGlobalFlag = DISP_DEVICE_STATE_OPEN;
        
        for(enD=HI_DRV_DISPLAY_0; enD<HI_DRV_DISPLAY_BUTT; enD++)
        {
            DispGetPointerByIDNoReturn(enD, pstDisp);

            if (pstDisp->bStateBackup == HI_TRUE)
            {
                DISP_SetEnable(enD, HI_TRUE);
            }
        }
    }
#endif
    return HI_SUCCESS;
}


HI_S32 DISP_Attach(HI_DRV_DISPLAY_E enMaster, HI_DRV_DISPLAY_E enSlave)
{
    DISP_S *pstM, *pstS;
    DISP_INTF_OPERATION_S *pfOpt = DISP_HAL_GetOperationPtr();
    
    // s1 check input parameters
    DispCheckDeviceState();

#if 0
    // s2 check whether display opened
    if(DISP_IsOpened(enMaster))
    {
        DISP_ERROR("Display has been opened!\n");
        return HI_ERR_DISP_INVALID_OPT;
    }

    if(DISP_IsOpened(enSlave))
    {
        DISP_ERROR("Display has been opened!\n");
        return HI_ERR_DISP_INVALID_OPT;
    }
#endif

    if( !pfOpt->PF_TestChnAttach(enMaster, enSlave) )
    {
        DISP_ERROR("Display NOT support %d attach to %d!\n", enSlave, enMaster);
        return HI_ERR_DISP_INVALID_OPT;
    }

    DispGetPointerByID(enMaster, pstM);
    DispGetPointerByID(enSlave, pstS);
    if (pstM->bIsMaster && (pstM->enAttachedDisp == enSlave))
    {
        DISP_INFO("Display has been ATTACHED!\n");
//DISP_PRINT("Func=%s, Line=%d\n", __FUNCTION__, __LINE__);
        return HI_SUCCESS;
    }

    if(pstM->bIsMaster || pstM->bIsSlave || pstS->bIsMaster || pstS->bIsSlave)
    {
        DISP_ERROR("Display has been opened!\n");
        return HI_ERR_DISP_INVALID_OPT;
    }

    pstM->bIsMaster = HI_TRUE;
    pstM->enAttachedDisp = enSlave;
    
    pstS->bIsSlave = HI_TRUE;
    pstS->enAttachedDisp = enMaster;

//DISP_PRINT("Func=%s, Line=%d\n", __FUNCTION__, __LINE__);

    return HI_SUCCESS;
}

HI_S32 DISP_Detach(HI_DRV_DISPLAY_E enMaster, HI_DRV_DISPLAY_E enSlave)
{
    DISP_S *pstM, *pstS;
    
    // s1 check input parameters
    DispCheckDeviceState();

#if 0
    // s2 check whether display opened
    if(DISP_IsOpened(enMaster))
    {
        DISP_ERROR("DISP ERROR! Display has been opened!\n");
        return HI_ERR_DISP_INVALID_OPT;
    }

    if(DISP_IsOpened(enSlave))
    {
        DISP_ERROR("DISP ERROR! Display has been opened!\n");
        return HI_ERR_DISP_INVALID_OPT;
    }
#endif

    // s3 set detach
    DispGetPointerByID(enMaster, pstM);
    DispGetPointerByID(enSlave, pstS);

    if (!pstM->bIsMaster || (pstM->enAttachedDisp != enSlave))
    {
        DISP_INFO("Display has NOT been ATTACHED!\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    pstM->bIsMaster = HI_FALSE;
    pstM->enAttachedDisp = HI_DRV_DISPLAY_BUTT;
    
    pstS->bIsSlave = HI_FALSE;
    pstS->enAttachedDisp = HI_DRV_DISPLAY_BUTT;

    return HI_SUCCESS;
}

HI_VOID debug_DISP_Callback(HI_HANDLE hDst, const HI_DRV_DISP_CALLBACK_INFO_S *pstInfo)
{
    //printk("event = %d, ", pstInfo->eEventType);
    return;
}


#ifdef HI_DISP_BUILD_FULL
#define  DISPLAY0_BUS_UNDERFLOW_INT 0x00000080UL
#define  DISPLAY1_BUS_UNDERFLOW_INT 0x00000008UL
HI_VOID DISP_CB_PreProcess(HI_HANDLE hHandle, const HI_DRV_DISP_CALLBACK_INFO_S *pstInfo)
{
    DISP_S *pstDisp = (DISP_S *)hHandle;
    DISP_INTF_OPERATION_S *pfHal = DISP_HAL_GetOperationPtr();
    HI_U32 uIntState;

    pfHal->PF_GetUnmaskedIntState(&uIntState);
    if ((pstDisp->enDisp == HI_DRV_DISPLAY_0) && (uIntState& DISPLAY0_BUS_UNDERFLOW_INT) )
    {
        pstDisp->u32Underflow++;
        pfHal->PF_CleanIntState(DISPLAY0_BUS_UNDERFLOW_INT);
    }
    if ((pstDisp->enDisp == HI_DRV_DISPLAY_1) && (uIntState& DISPLAY1_BUS_UNDERFLOW_INT) )
    {
        pstDisp->u32Underflow++;
        pfHal->PF_CleanIntState(DISPLAY1_BUS_UNDERFLOW_INT);
    }

    // display state change
    switch (pstDisp->eState)
    {
        case DISP_PRIV_STATE_DISABLE:
            if (pstDisp->bEnable)
            {
                DISP_ISR_SetEvent(pstDisp->enDisp, HI_DRV_DISP_C_OPEN);
                DISP_ISR_SetDispInfo(pstDisp->enDisp, &pstDisp->stDispInfo);
                pstDisp->eState = DISP_PRIV_STATE_WILL_ENABLE;
                DISP_PRINT("DISP_CB_PreProcess001 id=%d,en=%d\n", pstDisp->enDisp, pstDisp->bEnable);
            }
            break;
        case DISP_PRIV_STATE_WILL_ENABLE:
            DISP_ISR_SetEvent(pstDisp->enDisp, HI_DRV_DISP_C_VT_INT);
            DISP_ISR_SetDispInfo(pstDisp->enDisp, &pstDisp->stDispInfo);
            pstDisp->eState = DISP_PRIV_STATE_ENABLE;

            DISP_PRINT("DISP_CB_PreProcess002 id=%d,en=%d\n", pstDisp->enDisp, pstDisp->bEnable);

            break;
        case DISP_PRIV_STATE_ENABLE:
            if (!pstDisp->bEnable)
            {
                DISP_ISR_SetEvent(pstDisp->enDisp, HI_DRV_DISP_C_PREPARE_CLOSE);
                pstDisp->eState = DISP_PRIV_STATE_WILL_DISABLE;
                DISP_PRINT("DISP_CB_PreProcess003 id=%d,en=%d\n", pstDisp->enDisp, pstDisp->bEnable);
            }
            else
            {
                if (pstDisp->bDispSettingChange)
                {
                    DISP_PRINT("DISP_CB_PreProcess0031 id=%d,en=%d\n", pstDisp->enDisp, pstDisp->bEnable);
                    DispProduceDisplayInfo(pstDisp, &pstDisp->stDispInfo);
                    DISP_ISR_SetDispInfo(pstDisp->enDisp, &pstDisp->stDispInfo);
                    DISP_ISR_SetEvent(pstDisp->enDisp, HI_DRV_DISP_C_DISPLAY_SETTING_CHANGE);
                    pstDisp->bDispSettingChange = HI_FALSE;
                }
                else if (pstDisp->bDispAreaChange)
                {
                    DISP_PRINT("DISP_CB_PreProcess0032 id=%d,en=%d\n", pstDisp->enDisp, pstDisp->bEnable);
                    DispProduceDisplayInfo(pstDisp, &pstDisp->stDispInfo);
                    DISP_ISR_SetDispInfo(pstDisp->enDisp, &pstDisp->stDispInfo);
                    DISP_ISR_SetEvent(pstDisp->enDisp, HI_DRV_DISP_C_ADJUCT_SCREEN_AREA);
                    pstDisp->bDispAreaChange = HI_FALSE;
                }
                else
                {
                    DISP_ISR_SetEvent(pstDisp->enDisp, HI_DRV_DISP_C_VT_INT);
                    //DISP_ISR_SetDispInfo(pstDisp->enDisp, &pstDisp->stDispInfo);
                }
            }                

            break;
        case DISP_PRIV_STATE_WILL_DISABLE:
            DISP_ISR_SetEvent(pstDisp->enDisp, HI_DRV_DISP_C_EVET_NONE);
            pstDisp->eState = DISP_PRIV_STATE_DISABLE;

            DISP_PRINT("DISP_CB_PreProcess004 id=%d, en=%d\n",pstDisp->enDisp,  pstDisp->bEnable);

            break;
        default :
            break;

    }
//    printk("event = %d, ", pstDisp->eEventType);
    return;
}
#endif

HI_S32 DISP_Open(HI_DRV_DISPLAY_E enDisp)
{
    DISP_S *pstDisp;
    HI_S32 nRet = HI_SUCCESS;

    // s1 check input parameters
    DispCheckDeviceState();

    DispCheckID(enDisp);

    // s2 check whether display opened
    if(DISP_IsOpened(enDisp))
    {
        DISP_INFO("Display has been opened!\n");
        return HI_SUCCESS;
    }

//DISP_PRINT("Func=%s, Line=%d\n", __FUNCTION__, __LINE__);
    // s2.0 reset hardware
    //DispResetHardware();

    // s2.1 get display channel
    DispGetPointerByID(enDisp, pstDisp); 
    if (!pstDisp->pstIntfOpt->PF_TestChnSupport(enDisp))
    {
        DISP_ERROR("DISP ERROR! This version does not support display %d\n", (HI_S32)enDisp);
        return HI_ERR_DISP_INVALID_OPT;
    }
//DISP_PRINT("Func=%s, Line=%d\n", __FUNCTION__, __LINE__);

    // s3 check whether necessory attributes are configed
/*
    nRet = DispCheckReadyForOpen(enDisp);
    if (nRet)
    {
        return nRet;
    }
*/
//DISP_PRINT("Func=%s, Line=%d\n", __FUNCTION__, __LINE__);

#ifdef HI_DISP_BUILD_FULL

    // s3.2 add isr
    nRet = DISP_ISR_Add(enDisp);
    {
        HI_DRV_DISP_CALLBACK_S stCB;

        DISP_ISR_SetDispInfo(enDisp, &pstDisp->stDispInfo);

        stCB.hDst = (HI_HANDLE)pstDisp;
        stCB.pfDISP_Callback = DISP_CB_PreProcess;
        nRet = DISP_ISR_RegCallback(enDisp, HI_DRV_DISP_C_INTPOS_0_PERCENT,
                            &stCB);
        DISP_ASSERT(!nRet);

    }
    //DISP_ASSERT(!nRet);
//DISP_PRINT(" DISP_ISR_Add = 0x%x\n", nRet);
#endif
    // s5 Product ask for that display must be enabled at the same time.
    DISP_SetEnable(enDisp, HI_TRUE);

//DISP_PRINT("Func=%s, Line=%d\n", __FUNCTION__, __LINE__);


    // s4 set open state
    pstDisp->bOpen = HI_TRUE;

    //DISP_PRINT("mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm enDisp = %d\n", enDisp);

    if (pstDisp->bIsMaster)
    {
//        DISP_S *pstDispS;

        DISP_Open(pstDisp->enAttachedDisp);
#if 0
        DispGetPointerByID(pstDisp->enAttachedDisp, pstDispS); 

#ifdef HI_DISP_BUILD_FULL        
        // s3.2 add isr
        nRet = DISP_ISR_Add(pstDisp->enAttachedDisp, DISP_CB_PreProcess);
        //DISP_ASSERT(!nRet);
#endif
        DISP_SetEnable(pstDisp->enAttachedDisp, HI_TRUE);

    //printk("Func=%s, Line=%d\n", __FUNCTION__, __LINE__);

        // s4 set open state
        pstDispS->bOpen = HI_TRUE;
#endif
    }


#if 0
    if (0)
    {
        HI_DRV_DISP_CALLBACK_S stCB;

        stCB.hDst = 1;
        stCB.pfDISP_Callback = debug_DISP_Callback;

        nRet = DISP_ISR_RegCallback(enDisp, HI_DRV_DISP_C_INTPOS_0_PERCENT, &stCB);
        if (nRet)
        {
            DISP_WARN("XXXXXXXXXXXXXXX 001 \n");
        }
    }
#endif

    return nRet;
}



HI_VOID DispReleaseIntf(HI_DRV_DISPLAY_E enDisp)
{
    DISP_S *pstDisp;
    HI_DRV_DISP_INTF_ID_E i;
    
    DispGetPointerByIDNoReturn(enDisp, pstDisp);

    for(i=0; i<(HI_S32)HI_DRV_DISP_INTF_ID_MAX; i++)
    {
        if (pstDisp->stSetting.stIntf[i].bOpen)
        {
            DISP_INTF_OPERATION_S *pfOpt = DISP_HAL_GetOperationPtr();

            // s1 release vdac
            pfOpt->PF_ReleaseIntf2(pstDisp->enDisp, &pstDisp->stSetting.stIntf[i]);

            // s3 set intf
            DispCleanIntf(&pstDisp->stSetting.stIntf[i]);
        }
    }

    return;
}

HI_S32 DISP_Close(HI_DRV_DISPLAY_E enDisp)
{
    DISP_S *pstDisp;

    // s1 check input parameters
    DispCheckDeviceState();

    DispCheckID(enDisp);

    // s2 check whether display opened
    if(!DISP_IsOpened(enDisp))
    {
        DISP_INFO("Display is not opened!\n");
        return HI_SUCCESS;
    }

    // s4 set close state
    DispGetPointerByID(enDisp, pstDisp); 

    if (pstDisp->bIsMaster)
    {
        DISP_S *pstDispS;

        DISP_SetEnable(pstDisp->enAttachedDisp, HI_FALSE);

        DispGetPointerByID(pstDisp->enAttachedDisp, pstDispS); 

        pstDispS->bOpen = HI_FALSE;
    //printk("Func=%s, Line=%d\n", __FUNCTION__, __LINE__);

        // s5 release intf
        //DispReleaseIntf(pstDisp->enAttachedDisp);

#ifdef HI_DISP_BUILD_FULL
        // s4.2 delete isr
        DISP_ISR_Delete(pstDisp->enAttachedDisp);
#endif
    }

    // s3 Product ask for that display must be enabled at the same time.
//printk(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>> DISP_CLOSE 001\n");
    DISP_SetEnable(enDisp, HI_FALSE);

//printk(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>> DISP_CLOSE 002\n");
    
    pstDisp->bOpen = HI_FALSE;

    // s5 release intf
    //DispReleaseIntf(enDisp);

#ifdef HI_DISP_BUILD_FULL
    if (pstDisp->hCast)
    {
        DISP_CastDestroy(pstDisp->hCast);
        pstDisp->hCast = HI_NULL;
    }

    {
        HI_DRV_DISP_CALLBACK_S stCB;
        HI_S32 nRet;

        stCB.hDst = (HI_HANDLE)pstDisp;
        stCB.pfDISP_Callback = DISP_CB_PreProcess;
        nRet = DISP_ISR_UnRegCallback(enDisp, HI_DRV_DISP_C_INTPOS_0_PERCENT,
                                      &stCB);
        DISP_ASSERT(!nRet);

    }

    // s4.2 delete isr
    DISP_ISR_Delete(enDisp);
#endif

    return HI_SUCCESS;
}


HI_S32 DispGetVactTime(DISP_S *pstDisp)
{
    HI_S32 vtime;
    
    if (pstDisp->stDispInfo.u32RefreshRate)
    {
        vtime = (1000*100) / pstDisp->stDispInfo.u32RefreshRate;
    }
    else
    {
        vtime = 50;
    }

    if (vtime > 50)
    {
        vtime = 50;
    }
    else if (vtime < 20)
    {
        vtime = 20;
    }

    return vtime;
}


HI_S32 DISP_SetEnable(HI_DRV_DISPLAY_E enDisp, HI_BOOL bEnable)
{
    DISP_S *pstDisp;
    HI_S32 nRet, u;

    DispCheckDeviceState();

    // s1 check input parameters
    DispCheckID(enDisp);
    DispGetPointerByID(enDisp, pstDisp);

    // s2 check state
    if (bEnable == pstDisp->bEnable)
    {
        DISP_PRINT(" DISP Set enable return!\n");
        return HI_SUCCESS;
    }

    // s3 if Enable, set all config
    if (bEnable)
    {
        nRet = DispCheckReadyForOpen(enDisp);
        if (nRet)
        {
            return nRet;
        }

        // s1 set FMT
        nRet = DispSetFormat(enDisp, pstDisp->stSetting.enFormat);
        if (nRet)
        {
            DISP_ERROR("Set format failed\n");
            return nRet;
        }

        // s2 set CSC
        nRet = DispSetColor(pstDisp);
        if (nRet)
        {
            DISP_ERROR("Set color failed\n");
            return nRet;
        }

        // s3 set enable
        pstDisp->eState = DISP_PRIV_STATE_DISABLE;
        DispSetEnable(pstDisp, bEnable);
        
#ifndef __DISP_PLATFORM_BOOT__
        HI_DRV_SYS_GetTimeStampMs(&pstDisp->u32StartTime);
#endif

        //printk("disp%d =0x%x, ", pstDisp->enDisp,pstDisp->u32StartTime);
/*
        msleep(5);

        HI_DRV_SYS_GetTimeStampMs(&pstDisp->u32StartTime);
        printk("     T1=0x%x\n", pstDisp->u32StartTime);
*/
        pstDisp->bEnable = bEnable;
    }
    else
    {
        HI_S32 vtime;

        vtime = DispGetVactTime(pstDisp);
           
        // s1 set state and wait ISR Process
        pstDisp->bEnable = bEnable;
        
        DISP_MSLEEP(2 * vtime);

        u = 0;
        while(pstDisp->eState != DISP_PRIV_STATE_DISABLE)
        {
            DISP_MSLEEP(vtime);
            u++;
            if (u > DISP_SET_TIMEOUT_THRESHOLD)
            {
                DISP_WARN("Set enable timeout\n");
                break;
            }
        }

        // s2 set disable
        DispSetEnable(pstDisp, bEnable);

        // s3 wait vdp diable really
        DISP_MSLEEP(vtime);
    }

#if 0
    if (pstDisp->bIsMaster)
    {
        DISP_SetEnable(pstDisp->enAttachedDisp, bEnable);
    }
#endif

    return HI_SUCCESS;
}


HI_S32 DISP_GetEnable(HI_DRV_DISPLAY_E enDisp, HI_BOOL *pbEnable)
{
    DISP_S *pstDisp;

    DispCheckDeviceState();

    // s1 check input parameters
    DispCheckID(enDisp);
    DispCheckNullPointer(pbEnable);

    // s2 check whether display opened
    DispShouldBeOpened(enDisp);

    // s3 get ENABLE state and return
    DispGetPointerByID(enDisp, pstDisp);
    
    *pbEnable = pstDisp->bEnable;
    
    return HI_SUCCESS;
}

#ifndef __DISP_PLATFORM_BOOT__
//#define DISP_DEBUG_TEST_SET_FORMAT_TIME 1
#endif

HI_S32 DISP_SetFormat(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_STEREO_MODE_E enStereo, HI_DRV_DISP_FMT_E enEncFmt)
{
    DISP_S *pstDisp;
    HI_DRV_DISP_FMT_E enEncFmt2;
    HI_S32 nRet;
#ifdef DISP_DEBUG_TEST_SET_FORMAT_TIME
    struct timeval tv;
    HI_U32 t2, t1, t0;
#endif

    // s1 check input parameters
    DispCheckDeviceState();

    DispCheckID(enDisp);

    enEncFmt2 = DispTransferFormat(enDisp, enEncFmt);

    if(enEncFmt2 > HI_DRV_DISP_FMT_720P_50_FP)
    {
        DISP_ERROR("Display fmt is invalid\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    // s2 get pointer
    DispGetPointerByID(enDisp, pstDisp);     

    if ( (enEncFmt2 == pstDisp->stSetting.enFormat) && (enEncFmt2 < HI_DRV_DISP_FMT_CUSTOM) )
    {
        DISP_PRINT(" DISP Set same format and return!\n");
        return HI_SUCCESS;
    }

    if (!pstDisp->pstIntfOpt->PF_TestChnEncFmt(pstDisp->enDisp, enEncFmt2))
    {
        DISP_ERROR("Display %d does not support fmt %d\n", (HI_S32)enDisp, (HI_S32)enEncFmt2);
        return HI_ERR_DISP_INVALID_PARA;
    }

    // If hd work at follow-mode, set display0 sd format
    if (   (enDisp == HI_DRV_DISPLAY_1) && pstDisp->bIsMaster 
         && DispFmtIsStandDefinition(enEncFmt))
    {
        DISP_SetFormat(pstDisp->enAttachedDisp, HI_DRV_DISP_STEREO_NONE, enEncFmt);
    }

    pstDisp->stSetting.eDispMode = enStereo;
    pstDisp->stSetting.enFormat = enEncFmt2;

    // s4 if display is enabled, disable it and enable it,
    //    and new format will work.
    if (pstDisp->bEnable)
    {
#ifndef __DISP_PLATFORM_BOOT__
        if(enDisp == HI_DRV_DISPLAY_1)
        {   
            if (s_pstHDMIFunc->pfnHdmiPreFormat && s_pstHDMIFunc->pfnHdmiSetFormat)
            {
                s_pstHDMIFunc->pfnHdmiPreFormat(HI_UNF_HDMI_ID_0,enEncFmt);
            }
        }
#endif
    
#ifdef DISP_DEBUG_TEST_SET_FORMAT_TIME
    do_gettimeofday(&tv);
    t0 = (HI_U32)(tv.tv_sec * 1000 + (tv.tv_usec/1000));
#endif

    nRet = DISP_SetEnable(enDisp, HI_FALSE);

#ifdef DISP_DEBUG_TEST_SET_FORMAT_TIME
    do_gettimeofday(&tv);
    t1 = (HI_U32)(tv.tv_sec * 1000 + (tv.tv_usec/1000));
#endif

    if(enDisp == HI_DRV_DISPLAY_0)
    {
        // DTS2013060905670 : if time between setdisable and set enable is
        // less than 160ms, the screen on TV linked in CVBS flicker.
        // Increase time interval, flicker disappear.
        // set diable use time 60ms
        //DISP_MSLEEP(400);
        DISP_MSLEEP(500);
    }

    nRet = DISP_SetEnable(enDisp, HI_TRUE);

#ifdef DISP_DEBUG_TEST_SET_FORMAT_TIME
    do_gettimeofday(&tv);
    t2 = (HI_U32)(tv.tv_sec * 1000 + (tv.tv_usec/1000));
    DISP_FATAL("disable use time=%d, enable use time=%d ms\n", t1-t0, t2-t1);
#endif

#ifndef __DISP_PLATFORM_BOOT__
        if(enDisp == HI_DRV_DISPLAY_1)
        {   
            if (s_pstHDMIFunc->pfnHdmiPreFormat && s_pstHDMIFunc->pfnHdmiSetFormat)
            {
				//for avoid panasonic TH-L32CH3C change fmt error
                msleep(600);
                s_pstHDMIFunc->pfnHdmiSetFormat(HI_UNF_HDMI_ID_0,enEncFmt);  
            }
        }
#endif
    }


    return HI_SUCCESS;
}

HI_S32 DISP_GetFormat(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_FMT_E *penEncFmt)
{
    DISP_S *pstDisp;

    // s1 check input parameters
    DispCheckDeviceState();

    DispCheckID(enDisp);
    DispCheckNullPointer(penEncFmt);

    // s2 get pointer
    DispGetPointerByID(enDisp, pstDisp);     

    // s3 if display is not enabled, set format and return
    *penEncFmt = pstDisp->stSetting.enFormat;

    return HI_SUCCESS;
}

HI_S32 DISP_SetRightEyeFirst(HI_DRV_DISPLAY_E enDisp, HI_BOOL bEnable)
{
    DISP_S *pstDisp;

    DispCheckDeviceState();

    // s1 check input parameters
    DispCheckID(enDisp);

    // s3 check state
    DispGetPointerByID(enDisp, pstDisp);

    // s4 if display is enabled, disable it and enable it,
    //    and new format will work.
    pstDisp->bDispSettingChange = HI_FALSE;
    pstDisp->stSetting.bRightEyeFirst = bEnable;
    pstDisp->bDispSettingChange = HI_TRUE;

    return HI_SUCCESS;
}



//set aspect ratio
HI_S32 DISP_SetAspectRatio(HI_DRV_DISPLAY_E enDisp, HI_U32 u32Ratio_h, HI_U32 u32Ratio_v)
{
    DISP_S *pstDisp;

    DispCheckDeviceState();

    // s1 check input parameters
    DispCheckID(enDisp);

    // s3 check state
    DispGetPointerByID(enDisp, pstDisp);

    // s4 if display is enabled, disable it and enable it,
    //    and new format will work.
    pstDisp->bDispSettingChange = HI_FALSE;

    if (u32Ratio_h && u32Ratio_v)
    {
        pstDisp->stSetting.bCustomRatio = HI_TRUE;
        pstDisp->stSetting.u32CustomRatioWidth  = u32Ratio_h;
        pstDisp->stSetting.u32CustomRatioHeight = u32Ratio_v;
    }
    else
    {
        pstDisp->stSetting.bCustomRatio = HI_FALSE;
    }

    pstDisp->bDispSettingChange = HI_TRUE;

    return HI_SUCCESS;
}

HI_S32 DISP_GetAspectRatio(HI_DRV_DISPLAY_E enDisp, HI_U32 *pu32Ratio_h, HI_U32 *pu32Ratio_v)
{
    DISP_S *pstDisp;

    DispCheckDeviceState();

    // s1 check input parameters
    DispCheckID(enDisp);
    DispCheckNullPointer(pu32Ratio_h);
    DispCheckNullPointer(pu32Ratio_v);

    // s3 check state
    DispGetPointerByID(enDisp, pstDisp);

    // s4 if display is enabled, disable it and enable it,
    //    and new format will work.
    if (pstDisp->stSetting.bCustomRatio)
    {
        *pu32Ratio_h = pstDisp->stSetting.u32CustomRatioWidth;
        *pu32Ratio_v = pstDisp->stSetting.u32CustomRatioHeight;
    }
    else
    {
        *pu32Ratio_h = 0;
        *pu32Ratio_v = 0;
    }

    return HI_SUCCESS;
}

/* set Display output window  */
HI_S32 DISP_GetScreen(HI_DRV_DISPLAY_E enDisp, HI_RECT_S *pstRect)
{
    DISP_S *pstDisp;

    // s1 check input parameters
    DispCheckDeviceState();

    DispCheckID(enDisp);

    // s2 get pointer
    DispGetPointerByID(enDisp, pstDisp);     
    DispCheckNullPointer(pstRect);

    //todo check color setting

    *pstRect = pstDisp->stSetting.stRefAdjRect;

    return HI_SUCCESS;
}

HI_S32 DISP_SetScreen(HI_DRV_DISPLAY_E enDisp, HI_RECT_S *pstRect)
{
    DISP_S *pstDisp;

    // s1 check input parameters
    DispCheckDeviceState();

    DispCheckID(enDisp);

    // s2 get pointer
    DispGetPointerByID(enDisp, pstDisp);     
    DispCheckNullPointer(pstRect);

    //todo check color setting

    if (pstDisp->bIsMaster)
    {
        DISP_SetScreen(pstDisp->enAttachedDisp, pstRect);
    }
    else
    {
        pstDisp->stSetting.stRefAdjRect = *pstRect;

        if (pstDisp->bEnable)
        {
            pstDisp->bDispAreaChange = HI_TRUE;
        }
    }
    
    return HI_SUCCESS;
}

HI_S32 DISP_SetCustomTiming(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_TIMING_S *pstTiming)
{

    // if timing changed, set flag
    // pstDisp->stCfg.bTimingChange = HI_TURE;
    // pstDisp->stCfg.bTimingIsSet  = HI_TURE;    
    
    return HI_SUCCESS;
}
HI_S32 DISP_GetCustomTiming(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_TIMING_S *pstTiming)
{

    return HI_SUCCESS;
}

HI_S32 DISP_SetBGColor(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_COLOR_S *pstBGColor)
{
    DISP_S *pstDisp;

    // s1 check input parameters
    DispCheckDeviceState();

    DispCheckID(enDisp);

    // s2 get pointer
    DispGetPointerByID(enDisp, pstDisp);     
    DispCheckNullPointer(pstBGColor);

    //todo check color setting
/*
    DISP_PRINT("xxxxxxDISP_SetBGColor R=%d,G=%d,B=%d\n", 
                 pstBGColor->u8Red,
                 pstBGColor->u8Green, pstBGColor->u8Blue);
*/
    pstDisp->stSetting.stBgColor = *pstBGColor;

    if (pstDisp->bEnable)
    {
        DispSetColor(pstDisp);
    }

    if (pstDisp->bIsMaster)
    {
        //DISP_PRINT("DISP_SetColor, attech  disp ID = %d\n", pstDisp->enAttachedDisp);
        DISP_SetBGColor(pstDisp->enAttachedDisp, pstBGColor);
    }

    return HI_SUCCESS;
}
HI_S32 DISP_GetBGColor(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_COLOR_S *pstBGColor)
{
    DISP_S *pstDisp;

    // s1 check input parameters
    DispCheckDeviceState();

    DispCheckID(enDisp);

    // s2 get pointer
    DispGetPointerByID(enDisp, pstDisp);     
    DispCheckNullPointer(pstBGColor);

    //todo check color setting

    *pstBGColor = pstDisp->stSetting.stBgColor;

    return HI_SUCCESS;
}

HI_S32 DISP_SetLayerZorder(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_LAYER_E enLayer, HI_DRV_DISP_ZORDER_E enZFlag)
{

    return HI_SUCCESS;
}
HI_S32 DISP_GetLayerZorder(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_LAYER_E enLayer, HI_U32 *pu32Zorder)
{

    return HI_SUCCESS;
}


HI_S32 DISP_TestMacrovisionSupport(HI_DRV_DISPLAY_E enDisp, HI_BOOL *pbSupport)
{

    return HI_SUCCESS;
}

//snapshot
HI_S32 DISP_Snapshot(HI_DRV_DISPLAY_E enDisp, HI_DRV_VIDEO_FRAME_S * pstSnapShotFrame)
{

    return HI_SUCCESS;
}

#ifdef HI_DISP_BUILD_FULL

//miracast
HI_S32 DISP_CreateCast(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_CAST_CFG_S * pstCfg, HI_HANDLE *phCast)
{
    DISP_S *pstDisp;
    HI_S32 nRet;

    // s1 check input parameters
    DispCheckDeviceState();
//printk(">>>>>> DISP_CreateCast 001\n");
    DispCheckID(enDisp);
    DispCheckNullPointer(pstCfg);
    DispCheckNullPointer(phCast);

//printk(">>>>>> DISP_CreateCast 002\n");


    // s2 check whether display opened
    DispShouldBeOpened(enDisp);

    // s3 get pointer
    DispGetPointerByID(enDisp, pstDisp);

//printk(">>>>>> DISP_CreateCast 003\n");


    // s4 check whether support cast
    if (!pstDisp->pstIntfOpt->PF_TestChnSupportCast(pstDisp->enDisp))
    {
        DISP_ERROR("Disp %d not support cast!\n", (HI_S32)enDisp);
        return HI_ERR_DISP_INVALID_OPT;
    }

    //printk(">>>>>> DISP_CreateCast 004\n");

    // s5 create cast	
    nRet = DISP_CastCreate(pstDisp->enDisp, pstCfg, phCast);
    if (!nRet)
    {
        pstDisp->hCast = *phCast;
    }

DISP_WARN("DISP_CreateCast  pstDisp->hCast = 0x%x\n", (HI_U32)pstDisp->hCast);

    return nRet;
}

HI_S32 DISP_DestroyCast(HI_HANDLE hCast)
{
    HI_DRV_DISPLAY_E enDisp;
    DISP_S *pstDisp;
    HI_S32 nRet;

    // s1 check input parameters
    DispCheckDeviceState();
    DispCheckNullPointer(hCast);

    // s2 TODO: search display
    nRet = DispSearchCastHandle(hCast, &enDisp);
    if (nRet)
    {
        DISP_ERROR("DISP cast not exist!\n");
        return HI_ERR_DISP_NOT_EXIST;
    }

    // s3 check whether display opened
    DispShouldBeOpened(enDisp);

    // s4 get pointer
    DispGetPointerByID(enDisp, pstDisp);     

    // s5 destroy cast
    nRet = DISP_CastDestroy(hCast);

    pstDisp->hCast = HI_NULL;

    return nRet;
}

HI_S32 DISP_SetCastEnable(HI_HANDLE hCast, HI_BOOL bEnable)
{
    HI_DRV_DISPLAY_E enDisp;
    HI_S32 nRet;

    // s1 check input parameters
    DispCheckDeviceState();
    DispCheckNullPointer(hCast);

    DISP_WARN("DISP_SetCastEnable  hCast = 0x%x\n", (HI_U32)hCast);

    // s2 TODO: search display
    nRet = DispSearchCastHandle(hCast, &enDisp);
    if (nRet)
    {
        DISP_ERROR("DISP cast not exist!\n");
        return HI_ERR_DISP_NOT_EXIST;
    }

    // s3 check whether display opened
    DispShouldBeOpened(enDisp);

    nRet = DISP_CastSetEnable(hCast, bEnable);

    return nRet;
}


HI_S32 DISP_GetCastEnable(HI_HANDLE hCast, HI_BOOL *pbEnable)
{
    HI_DRV_DISPLAY_E enDisp;
    HI_S32 nRet;

    // s1 check input parameters
    DispCheckDeviceState();
    DispCheckNullPointer(hCast);
    DispCheckNullPointer(pbEnable);

    // s2 TODO: search display
    nRet = DispSearchCastHandle(hCast, &enDisp);
    if (nRet)
    {
        DISP_ERROR("DISP cast not exist!\n");
        return HI_ERR_DISP_NOT_EXIST;
    }

    // s3 check whether display opened
    DispShouldBeOpened(enDisp);

    nRet = DISP_CastGetEnable(hCast, pbEnable);

    return nRet;
}

HI_S32 DISP_AcquireCastFrame(HI_HANDLE hCast, HI_DRV_VIDEO_FRAME_S *pstCastFrame)
{
    HI_DRV_DISPLAY_E enDisp;
    HI_S32 nRet;

    // s1 check input parameters
    DispCheckDeviceState();
    DispCheckNullPointer(hCast);

    // s2 TODO: search display
    nRet = DispSearchCastHandle(hCast, &enDisp);
    if (nRet)
    {
        DISP_ERROR("DISP cast not exist!\n");
        return HI_ERR_DISP_NOT_EXIST;
    }

    // s3 check whether display opened
    DispShouldBeOpened(enDisp);

    nRet = DISP_CastAcquireFrame(hCast, pstCastFrame);

    return nRet;
}

HI_S32 DISP_ReleaseCastFrame(HI_HANDLE hCast, HI_DRV_VIDEO_FRAME_S *pstCastFrame)
{
    HI_DRV_DISPLAY_E enDisp;
    HI_S32 nRet;

    // s1 check input parameters
    DispCheckDeviceState();
    DispCheckNullPointer(hCast);
    DispCheckNullPointer(pstCastFrame);

    // s2 TODO: search display
    nRet = DispSearchCastHandle(hCast, &enDisp);
    if (nRet)
    {
        DISP_ERROR("DISP cast not exist!\n");
        return HI_ERR_DISP_NOT_EXIST;
    }

    // s3 check whether display opened
    DispShouldBeOpened(enDisp);

    nRet = DISP_CastReleaseFrame(hCast, pstCastFrame);
    return nRet;
}
#endif



HI_S32 DISP_SetColor(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_COLOR_SETTING_S *pstCS)
{
    DISP_S *pstDisp;

    // s1 check input parameters
    DispCheckDeviceState();

    DispCheckID(enDisp);

    // s2 get pointer
    DispGetPointerByID(enDisp, pstDisp);     
    DispCheckNullPointer(pstCS);

    //todo check color setting

    pstDisp->stSetting.stColor = *pstCS;

    //DISP_PRINT("DISP_SetColor, id=%d, en=%d\n", enDisp, pstDisp->bEnable);
    if (pstDisp->bEnable)
    {
        pstDisp->bDispSettingChange = HI_TRUE;
    }

    if (pstDisp->bIsMaster)
    {
        //DISP_PRINT("DISP_SetColor, attech  disp ID = %d\n", pstDisp->enAttachedDisp);
        DISP_SetColor(pstDisp->enAttachedDisp, pstCS);
    }

    return HI_SUCCESS;
}

HI_S32 DISP_GetColor(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_COLOR_SETTING_S *pstCS)
{
    DISP_S *pstDisp;

    // s1 check input parameters
    DispCheckDeviceState();

    DispCheckID(enDisp);

    // s2 get pointer
    DispGetPointerByID(enDisp, pstDisp);     
    DispCheckNullPointer(pstCS);

    *pstCS = pstDisp->stSetting.stColor;

    return HI_SUCCESS;
}



HI_S32 DISP_SetMacrovisionCustomer(HI_DRV_DISPLAY_E enDisp, HI_VOID *pData)
{

    return HI_SUCCESS;
}

HI_S32 DISP_SetMacrovision(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_MACROVISION_E enMode)
{

    return HI_SUCCESS;
}

HI_S32 DISP_GetMacrovision(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_MACROVISION_E *penMode)
{

    return HI_SUCCESS;
}

//cgms-a
HI_S32 DISP_SetCGMS_A(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_CGMSA_CFG_S *pstCfg)
{

    return HI_SUCCESS;
}


//vbi
HI_S32 DISP_CreateVBIChannel(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_VBI_CFG_S *pstCfg, HI_HANDLE *phVbi)
{

    return HI_SUCCESS;
}

HI_S32 DISP_DestroyVBIChannel(HI_HANDLE hVbi)
{

    return HI_SUCCESS;
}

HI_S32 DISP_SendVbiData(HI_HANDLE hVbi, HI_DRV_DISP_VBI_DATA_S *pstVbiData)
{

    return HI_SUCCESS;
}

HI_S32 DISP_SetWss(HI_HANDLE hVbi, HI_DRV_DISP_WSS_DATA_S *pstWssData)
{

    return HI_SUCCESS;
}


//may be deleted
HI_S32 DISP_SetHdmiIntf(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_HDMI_S *pstCfg)
{

    return HI_SUCCESS;
}

HI_S32 DISP_GetHdmiIntf(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_HDMI_S *pstCfg)
{

    return HI_SUCCESS;
}

HI_S32 DISP_SetSetting(HI_DRV_DISPLAY_E enDisp, DISP_SETTING_S *pstSetting)
{

    return HI_SUCCESS;
}

HI_S32 DISP_GetSetting(HI_DRV_DISPLAY_E enDisp, DISP_SETTING_S *pstSetting)
{

    return HI_SUCCESS;
}


HI_S32 DISP_AddIntf(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_INTF_S *pstIntf)
{
    DISP_S *pstDisp;
    HI_S32 nRet;
    DISP_INTF_OPERATION_S *pfOpt = DISP_HAL_GetOperationPtr();

    // s1 check input parameters
    DispCheckDeviceState();

    DispCheckID(enDisp);
    DispCheckNullPointer(pstIntf);

    nRet = DispCheckIntfValid(pstIntf);
    if(nRet)
    {
        DISP_ERROR("Invalid intf parameters in %s!\n", __FUNCTION__);
        return HI_ERR_DISP_INVALID_PARA;
    }  

    // s2 get pointer
    DispGetPointerByID(enDisp, pstDisp);

    // s3.2 check whether eIntf exists
    if (DispCheckIntfExist(enDisp, pstIntf))
    {
        return HI_SUCCESS;
    }

    // s3.1 check whether eIntf is supported 
    nRet = DispAddIntf(pstDisp, pstIntf);

    if ( pstDisp->bEnable && !nRet)
    {
//printk(">>>>> xb1\n");
        pfOpt->PF_ResetIntfFmt2(pstDisp->enDisp, 
                                DispGetIntfPtr(pstDisp, pstIntf->eID),  
                                pstDisp->stSetting.enFormat);

        pfOpt->PF_SetIntfEnable2(pstDisp->enDisp, 
                                 DispGetIntfPtr(pstDisp, pstIntf->eID), 
                                 HI_TRUE);
    }
  
    return nRet;
}

HI_S32 DISP_DelIntf(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_INTF_S *pstIntf)
{
    DISP_S *pstDisp;
    DISP_INTF_OPERATION_S *pfOpt = DISP_HAL_GetOperationPtr();
    DISP_INTF_S *pstIf;
    
    // s1 check input parameters
    DispCheckDeviceState();

    DispCheckID(enDisp);
    DispCheckNullPointer(pstIntf);

    // s2 get pointer
    DispGetPointerByID(enDisp, pstDisp);

    // s3 check if eIntf exists
    if (!DispCheckIntfExistByType(enDisp, pstIntf))
    {
        return HI_FAILURE;
    }

    pstIf = DispGetIntfPtr(pstDisp, pstIntf->eID);

    // s4 set intf disable
    pfOpt->PF_SetIntfEnable2(pstDisp->enDisp, pstIf, HI_FALSE);

    // s5 release intf
    pfOpt->PF_ReleaseIntf2(pstDisp->enDisp, pstIf);

    DispCleanIntf(pstIf);

    return HI_SUCCESS;
}


HI_S32 DISP_GetSlave(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISPLAY_E *penSlave)
{
    DISP_S *pstDisp;

    // s1 check input parameters
    DispCheckDeviceState();

    DispCheckID(enDisp);
    DispCheckNullPointer(penSlave);

    // s2 get pointer
    DispGetPointerByID(enDisp, pstDisp);

    // s3 check if eIntf exists
    if (!pstDisp->bIsMaster)
    {
        return HI_FAILURE;
    }

    *penSlave = pstDisp->enAttachedDisp;

    return HI_SUCCESS;
}

HI_S32 DISP_GetMaster(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISPLAY_E *penMaster)
{
   DISP_S *pstDisp;

    // s1 check input parameters
    DispCheckDeviceState();

    DispCheckID(enDisp);
    DispCheckNullPointer(penMaster);

    // s2 get pointer
    DispGetPointerByID(enDisp, pstDisp);

    // s3 check if eIntf exists
    if (!pstDisp->bIsSlave)
    {
        return HI_FAILURE;
    }

    *penMaster = pstDisp->enAttachedDisp;

    return HI_SUCCESS;
}


HI_S32 DISP_GetDisplayInfo(HI_DRV_DISPLAY_E enDisp, HI_DISP_DISPLAY_INFO_S *pstInfo)
{
    DISP_S *pstDisp;
    DISP_INTF_OPERATION_S * pfOpt = DISP_HAL_GetOperationPtr();
    HI_BOOL bBtm;
    HI_U32 vcnt;
    
    //DISP_HAL_ENCFMT_PARAM_S stFmt;
    //HI_S32 nRet;

    DispCheckDeviceState();

    // s1 check input parameters
    DispCheckID(enDisp);
    DispCheckNullPointer(pstInfo);

    // s2 check whether display opened
    DispShouldBeOpened(enDisp);

    // s3 get ENABLE state and return
    DispGetPointerByID(enDisp, pstDisp);

    if ( (pstDisp->stSetting.enFormat < HI_DRV_DISP_FMT_BUTT) && pfOpt )
    {
        *pstInfo = pstDisp->stDispInfo;

        pfOpt->FP_GetChnBottomFlag(enDisp, &bBtm, &vcnt);

        pstInfo->bIsBottomField = bBtm;
        pstInfo->u32Vline = vcnt;
    }
    else
    {
        DISP_ERROR("Display %d info not available!\n", enDisp);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}


#ifdef HI_DISP_BUILD_FULL
HI_S32 DISP_RegCallback(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_CALLBACK_TYPE_E eType,
                        HI_DRV_DISP_CALLBACK_S *pstCB)
{
//    DISP_S *pstDisp;
//    DISP_HAL_ENCFMT_PARAM_S stFmt;
    HI_S32 nRet;

    DispCheckDeviceState();

    // s1 check input parameters
    DispCheckID(enDisp);
    DispCheckNullPointer(pstCB);

    // s2 check whether display opened
    DispShouldBeOpened(enDisp);

    if (!pstCB->pfDISP_Callback)
    {
        DISP_ERROR("Callback function is null!\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    nRet = DISP_ISR_RegCallback(enDisp, eType,pstCB);

    return nRet;
}

HI_S32 DISP_UnRegCallback(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_CALLBACK_TYPE_E eType, 
                          HI_DRV_DISP_CALLBACK_S *pstCB)
{
//    DISP_S *pstDisp;
//    DISP_HAL_ENCFMT_PARAM_S stFmt;
    HI_S32 nRet;

    DispCheckDeviceState();

    // s1 check input parameters
    DispCheckID(enDisp);
    DispCheckNullPointer(pstCB);

    // s2 check whether display opened
    DispShouldBeOpened(enDisp);

    if (!pstCB->pfDISP_Callback)
    {
        DISP_ERROR("Callback function is null!\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    nRet = DISP_ISR_UnRegCallback(enDisp, eType, pstCB);

    return nRet;
}

HI_S32 DISP_GetProcInto(HI_DRV_DISPLAY_E enDisp, DISP_PROC_INFO_S *pstInfo)
{
    DISP_S *pstDisp;
    HI_S32 i;

    DispCheckDeviceState();

    // s1 check input parameters
    DispCheckID(enDisp);
    DispCheckNullPointer(pstInfo);

    // s2 check whether display opened
    DispShouldBeOpened(enDisp);

    // s3 get ENABLE state and return
    DispGetPointerByID(enDisp, pstDisp);

    //pstInfo->stSetting.;
    pstInfo->bEnable = pstDisp->bEnable;
    pstInfo->bMaster = pstDisp->bIsMaster;
    pstInfo->bSlave  = pstDisp->bIsSlave;
    pstInfo->u32Underflow = pstDisp->u32Underflow;
    pstInfo->u32StartTime = pstDisp->u32StartTime;

    pstInfo->eDispMode  = pstDisp->stSetting.eDispMode;
    pstInfo->bRightEyeFirst = pstDisp->stSetting.bRightEyeFirst;
    pstInfo->eFmt       = pstDisp->stSetting.enFormat;
    pstInfo->stTiming   = pstDisp->stSetting.stCustomTimg;

    pstInfo->bCustAspectRatio = pstDisp->stSetting.bCustomRatio;
    pstInfo->u32CustomAR_w = pstDisp->stSetting.u32CustomRatioWidth;
    pstInfo->u32CustomAR_h = pstDisp->stSetting.u32CustomRatioHeight;
    pstInfo->u32AR_w       = pstDisp->stDispInfo.stAR.u8ARw;
    pstInfo->u32AR_h       = pstDisp->stDispInfo.stAR.u8ARh;

    pstInfo->stAdjRect  = pstDisp->stDispInfo.stAdjRect;

    pstInfo->eMixColorSpace  = pstDisp->stDispInfo.eColorSpace;
    pstInfo->eDispColorSpace = pstDisp->stDispInfo.eColorSpace;
    pstInfo->stColorSetting  = pstDisp->stSetting.stColor;
    pstInfo->stBgColor = pstDisp->stSetting.stBgColor;

    // about color setting
    pstInfo->u32IntfNumber = 0;
    for(i=0; i<HI_DRV_DISP_INTF_ID_MAX; i++)
    {
        if (pstDisp->stSetting.stIntf[i].bOpen)
        {
            pstInfo->stIntf[pstInfo->u32IntfNumber] = pstDisp->stSetting.stIntf[i].stIf;
            pstInfo->u32IntfNumber++;
        }
    }


    // TODO: intf AND venc

    pstInfo->hCast = pstDisp->hCast;

    return HI_SUCCESS;
}
#endif


#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */



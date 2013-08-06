
/******************************************************************************
  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : drv_disp_x.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2012/12/30
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/

#include "drv_disp_isr.h"
#include "drv_disp_osal.h"


#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

static HI_S32 s_DispISRMngrInitFlag = -1;
static DISP_ISR_M_S s_DispISRMngr;

HI_S32 DISP_ISR_SwitchIntterrup(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_CALLBACK_TYPE_E eType, HI_BOOL bEnable)
{
    //printk("Open enDisp=%d int %d\n", enDisp, eType);
    switch(eType)
    {
        case HI_DRV_DISP_C_INTPOS_0_PERCENT:
            if (HI_DRV_DISPLAY_0 ==  enDisp)
            {
                s_DispISRMngr.pIntOpt->PF_SetIntEnable((HI_U32)DISP_INTERRUPT_D0_0_PERCENT, bEnable);
            }
            else if (HI_DRV_DISPLAY_1 ==  enDisp)
            {
                s_DispISRMngr.pIntOpt->PF_SetIntEnable((HI_U32)DISP_INTERRUPT_D1_0_PERCENT, bEnable);
            }
            break;
        case HI_DRV_DISP_C_INTPOS_90_PERCENT:
            if (HI_DRV_DISPLAY_0 ==  enDisp)
            {
                s_DispISRMngr.pIntOpt->PF_SetIntEnable((HI_U32)DISP_INTERRUPT_D0_90_PERCENT, bEnable);
            }
            else if (HI_DRV_DISPLAY_1 ==  enDisp)
            {
                s_DispISRMngr.pIntOpt->PF_SetIntEnable((HI_U32)DISP_INTERRUPT_D1_90_PERCENT, bEnable);
            }
            break;

        default :
            break;
    }

    return HI_SUCCESS;
}


HI_S32 DISP_ISR_Init(DISP_INTF_OPERATION_S *pstIntOptFunc)
{
    if (s_DispISRMngrInitFlag >= 0)
    {
        return HI_SUCCESS;
    }

    DISP_MEMSET(&s_DispISRMngr, 0, sizeof(DISP_ISR_M_S));

    s_DispISRMngr.pIntOpt = pstIntOptFunc;

    s_DispISRMngrInitFlag++;
        
    return HI_SUCCESS;
}


HI_S32 DISP_ISR_DeInit(HI_VOID)
{
    HI_DRV_DISPLAY_E enDisp;

    if (s_DispISRMngrInitFlag < 0)
    {
        return HI_SUCCESS;
    }

    for(enDisp=HI_DRV_DISPLAY_0; enDisp<HI_DRV_DISPLAY_BUTT; enDisp++)
    {
        DISP_ISR_Delete(enDisp);
    }

    DISP_MEMSET(&s_DispISRMngr, 0, sizeof(DISP_ISR_M_S));

    s_DispISRMngrInitFlag--;
    return HI_SUCCESS;
}

HI_S32 DISP_ISR_Suspend(HI_VOID)
{
    if (s_DispISRMngrInitFlag < 0)
    {
        return HI_FAILURE;
    }

    s_DispISRMngr.pIntOpt->PF_GetIntSetting(&s_DispISRMngr.u32IntMaskSave4Suspend);

    return HI_SUCCESS;
}

HI_S32 DISP_ISR_Resume(HI_VOID)
{
    if (s_DispISRMngrInitFlag < 0)
    {
        return HI_FAILURE;
    }

    s_DispISRMngr.pIntOpt->PF_SetIntEnable(s_DispISRMngr.u32IntMaskSave4Suspend, HI_TRUE);

    return HI_SUCCESS;
}

HI_S32 DISP_ISR_Add(HI_DRV_DISPLAY_E enDisp)
{
    DISP_ISR_CHN_S *pstChn;

    if (s_DispISRMngrInitFlag < 0)
    {
        return HI_FAILURE;
    }

    if (s_DispISRMngr.pstChn[enDisp])
    {
        DISP_ERROR("DISP %d existed!\n", enDisp);
        return HI_FAILURE;
    }

    pstChn = (DISP_ISR_CHN_S *)DISP_MALLOC(sizeof(DISP_ISR_CHN_S));
    if (!pstChn)
    {
        DISP_FATAL("Alloc DISP_ISR_CHN_S memory failed\n");
        return HI_FAILURE;
    }

    DISP_MEMSET(pstChn, 0, sizeof(DISP_ISR_CHN_S));

    pstChn->enDisp = enDisp;
    pstChn->stCBInfo.eEventType = HI_DRV_DISP_C_EVET_NONE;
    //pstChn->pfPreProcess = pfPreProcess;

    s_DispISRMngr.pstChn[enDisp] = pstChn;
    s_DispISRMngr.u32ChnNumber++;

    pstChn->bEnable = HI_TRUE;

    //DISP_ISR_SwitchIntterrup(enDisp, HI_DRV_DISP_C_INTPOS_0_PERCENT, HI_TRUE);

    return HI_SUCCESS;
}

HI_S32 DISP_ISR_Delete(HI_DRV_DISPLAY_E enDisp)
{
    DISP_ISR_CHN_S *pstChn;

    if (s_DispISRMngrInitFlag < 0)
    {
        return HI_FAILURE;
    }

    pstChn = s_DispISRMngr.pstChn[enDisp];
    if (!pstChn)
    {
        return HI_SUCCESS;
    }

    pstChn->bEnable = HI_FALSE;

    //DISP_ISR_SwitchIntterrup(enDisp, HI_DRV_DISP_C_INTPOS_0_PERCENT, HI_FALSE);
    //printk("Open int\n");


    s_DispISRMngr.pstChn[enDisp] = HI_NULL;
    s_DispISRMngr.u32ChnNumber--;

    DISP_FREE(pstChn);
    
    return HI_SUCCESS;
}

HI_S32 DISP_ISR_SetEnable(HI_DRV_DISPLAY_E enDisp, HI_BOOL bEnable)
{
    DISP_ISR_CHN_S *pstChn;

    if (s_DispISRMngrInitFlag < 0)
    {
        return HI_FAILURE;
    }

    pstChn = s_DispISRMngr.pstChn[enDisp];
    if (!pstChn)
    {
        return HI_SUCCESS;
    }

    pstChn->bEnable = bEnable;
  
    return HI_SUCCESS;
}

HI_S32 DISP_ISR_SearchNode(DISP_ISR_CHN_S *pstChn, HI_DRV_DISP_CALLBACK_TYPE_E eType,
                           HI_DRV_DISP_CALLBACK_S *pstCB)
{
    HI_U32 u, v;

    v = pstChn->stList[eType].u32NodeFlagNew;

    for (u=0; u<32 && v; u++)
    {
        if (    ( v & (1<<u) )
            && (pstChn->stList[eType].stNode[u].pfDISP_Callback == pstCB->pfDISP_Callback)
            && (pstChn->stList[eType].stNode[u].hDst== pstCB->hDst)
            )
        {
            return (HI_S32)u;
        }
    }

    return -1;
}

HI_S32 DISP_ISR_SearchNullNode(DISP_ISR_CHN_S *pstChn, HI_DRV_DISP_CALLBACK_TYPE_E eType)
{
    HI_S32 i, v;

    for (i=0; i<DEF_DISP_ISR_LIST_LENGTH; i++)
    {
        v = 1 << i;
        if (    !(pstChn->stList[eType].u32NodeFlag & v)
             && !(pstChn->stList[eType].u32NodeFlagNew & v)
            )
        {
            return i;
        }
    }

    return -1;
}




static HI_U32 s_DispIntTable[HI_DRV_DISPLAY_BUTT][HI_DRV_DISP_C_TYPE_BUTT] = 
{
//NONE, SHOW_MODE, INTPOS_0_PERCENT, INTPOS_90_PERCENT,       GFX_WBC, REG_UP
{0, DISP_INTERRUPT_D0_0_PERCENT, DISP_INTERRUPT_D0_90_PERCENT, 0, 0 },
{0, DISP_INTERRUPT_D1_0_PERCENT, DISP_INTERRUPT_D1_90_PERCENT, 0, 0 },    
{0, 0, 0, 0, 0 },
};

HI_S32 DISP_ISR_RegCallback(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_CALLBACK_TYPE_E eType,
                            HI_DRV_DISP_CALLBACK_S *pstCB)
{
    DISP_ISR_CHN_S *pstChn;
    HI_S32 index;

    if (s_DispISRMngrInitFlag < 0)
    {
        return HI_FAILURE;
    }

    pstChn = s_DispISRMngr.pstChn[enDisp];
    if (!pstChn)
    {
        DISP_ERROR("DISP %d is not add to ISR manager!\n", enDisp);
        return HI_FAILURE;
    }


    //printk("DISP_ISR_RegCallback  disp=%d, type=%d\n", enDisp, eType);
    index = DISP_ISR_SearchNullNode(pstChn, eType);
    if (index < 0)
    {
        DISP_ERROR("DISP %d  callback reach max number!\n", enDisp);
        return HI_FAILURE;
    }
    
    pstChn->stList[eType].stNode[index] = *pstCB;
    pstChn->stList[eType].u32NodeFlagNew |= (1 << index);

/*
    if ( (eType != HI_DRV_DISP_C_INTPOS_0_PERCENT)
        &&(pstChn->stList[eType].u32NodeFlagNew)
       )
*/
    if (pstChn->stList[eType].u32NodeFlagNew)
    {
        DISP_ISR_SwitchIntterrup(enDisp, eType, HI_TRUE);
        //printk("Open int\n");
    }

    s_DispISRMngr.bDispChange = HI_TRUE;
    
    pstChn->u32TotalNumber++;

    return HI_SUCCESS;
}

HI_S32 DISP_ISR_UnRegCallback(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_CALLBACK_TYPE_E eType,
                              HI_DRV_DISP_CALLBACK_S *pstCB)
{
    DISP_ISR_CHN_S *pstChn;
    HI_S32 index;

    if (s_DispISRMngrInitFlag < 0)
    {
        return HI_FAILURE;
    }

    pstChn = s_DispISRMngr.pstChn[enDisp];
    if (!pstChn)
    {
        DISP_ERROR("DISP %d is not add to ISR manager!\n", enDisp);
        return HI_FAILURE;
    }

    index = DISP_ISR_SearchNode(pstChn, eType, pstCB);
    if (index < 0)
    {
        DISP_ERROR("Callback is not exist!\n");
        return HI_FAILURE;
    }

    pstChn->stList[eType].u32NodeFlagNew &= ~(1 << index);

    if (!pstChn->stList[eType].u32NodeFlagNew)
    {
        //if (eType != HI_DRV_DISP_C_INTPOS_0_PERCENT)
        {
            DISP_ISR_SwitchIntterrup(enDisp, eType, HI_FALSE);
        }

        /* if run here before interrup happen, clear NodeFlag */
        pstChn->stList[eType].u32NodeFlag = pstChn->stList[eType].u32NodeFlagNew;

        pstChn->stList[eType].stNode[index].pfDISP_Callback = HI_NULL;
        pstChn->stList[eType].stNode[index].hDst = HI_NULL;
        //printk("close int\n");
    }



    return HI_SUCCESS;
}

HI_S32 DISP_ISR_SetEvent(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_CALLBACK_EVENT_E eEvent)
{
    DISP_ISR_CHN_S *pstChn;

    if (s_DispISRMngrInitFlag < 0)
    {
        return HI_FAILURE;
    }

    pstChn = s_DispISRMngr.pstChn[enDisp];
    if (!pstChn)
    {
        DISP_ERROR("DISP %d is not add to ISR manager!\n", enDisp);
        return HI_FAILURE;
    }

    pstChn->stCBInfo.eEventType = eEvent;
    
    return HI_SUCCESS;
}

HI_S32 DISP_ISR_SetDispInfo(HI_DRV_DISPLAY_E enDisp, HI_DISP_DISPLAY_INFO_S *pstDispInfo)
{
    DISP_ISR_CHN_S *pstChn;
    HI_BOOL bIsBottomFieldbk;
    HI_U32 u32Vlinebk;


    if (s_DispISRMngrInitFlag < 0)
    {
        return HI_FAILURE;
    }

    pstChn = s_DispISRMngr.pstChn[enDisp];
    if (!pstChn)
    {
        DISP_ERROR("DISP %d is not add to ISR manager!\n", enDisp);
        return HI_FAILURE;
    }

    //pstChn->bDispInfoUp    = HI_FALSE;
    //pstChn->stDispInfoNew  = *pstDispInfo;
    bIsBottomFieldbk = pstChn->stCBInfo.stDispInfo.bIsBottomField;
    u32Vlinebk = pstChn->stCBInfo.stDispInfo.u32Vline;

    pstChn->stCBInfo.stDispInfo = *pstDispInfo;

    pstChn->stCBInfo.stDispInfo.bIsBottomField = bIsBottomFieldbk;
    pstChn->stCBInfo.stDispInfo.u32Vline = u32Vlinebk;
    //pstChn->bDispInfoIsSet = HI_TRUE;
    //pstChn->bDispInfoUp    = HI_TRUE;

    //s_DispISRMngr.bDispChange = HI_TRUE;

    return HI_SUCCESS;
}

//#define DEF_DISP_ISR_Main_RETURN_VALUE HI_SUCCESS
#define DEF_DISP_ISR_Main_RETURN_VALUE IRQ_HANDLED



static HI_U32 s_DebugMyIntCount = 0;
#define DEF_DEBUG_DISP_INT_MAX_NUMBER 1000

HI_S32 DISP_ISR_Main(HI_S32 irq, HI_VOID *dev_id)
{
    HI_DRV_DISPLAY_E enDisp;
    HI_DRV_DISP_CALLBACK_TYPE_E eIntType;
    DISP_ISR_CHN_S *pstDisp;
    HI_U32 u32IntState = 0;
    HI_U32 n, v;
//    HI_U32 i;

    // s1 get interrupt state
    s_DispISRMngr.pIntOpt->PF_GetMaskedIntState(&u32IntState);

    // s2 clear interrupt state
    s_DispISRMngr.pIntOpt->PF_CleanIntState(u32IntState);


    s_DebugMyIntCount++;
    /*
    if (s_DebugMyIntCount < DEF_DEBUG_DISP_INT_MAX_NUMBER)
    {
    printk("<I>");
    }
    */

    //return DEF_DISP_ISR_Main_RETURN_VALUE;



    // s3 check and recode underload interrupt
    

    // s5 process interrupt one by one
    //printk("[0x%x]", u32IntState);


    // s5.0 if display is not open, return
    if (!s_DispISRMngr.u32ChnNumber)
    {
        return DEF_DISP_ISR_Main_RETURN_VALUE;
    }

    for(enDisp = HI_DRV_DISPLAY_0; enDisp < HI_DRV_DISPLAY_2; enDisp++)
    {
        if (s_DispISRMngr.pstChn[enDisp])
        {
            if (s_DispISRMngr.pstChn[enDisp]->bEnable != HI_TRUE)
            {
                continue;
            }
            
            for (eIntType=HI_DRV_DISP_C_INTPOS_0_PERCENT; eIntType<HI_DRV_DISP_C_TYPE_BUTT; eIntType++)
            {
                HI_BOOL bBtm;
                HI_U32 vcnt;

                if(!(s_DispIntTable[enDisp][eIntType] & u32IntState) )
                {
                    continue;
                }

                // there is new callback
                pstDisp = s_DispISRMngr.pstChn[enDisp];
                
                //if (pstDisp->stCBInfo.eEventType != pstDisp->eEvent)
                {
                    //printk("Change event to [%d, 0x%x]\n", pstDisp->eEvent, u32IntState);
                }
                

                if( s_DispIntTable[enDisp][HI_DRV_DISP_C_INTPOS_0_PERCENT] & u32IntState )
                {
                    //if (pstDisp->bDispInfoUp)
                    {
                        //pstDisp->stCBInfo.stDispInfo = pstDisp->stDispInfoNew;
                        //pstDisp->bDispInfoUp= HI_FALSE;
                        //pstDisp->bDispInfoIsSet = HI_TRUE;
                    }

                    // get top and bottom flag
                    s_DispISRMngr.pIntOpt->FP_GetChnBottomFlag(enDisp, &bBtm, &vcnt);

                    //printk("[%d, %d]", bBtm, vcnt);
                    pstDisp->stCBInfo.stDispInfo.bIsBottomField = bBtm;
                    pstDisp->stCBInfo.stDispInfo.u32Vline = vcnt;
                    //pstDisp->stCBInfo.eEventType = pstDisp->eEvent;
                }


                if (pstDisp->stList[eIntType].u32NodeFlagNew != pstDisp->stList[eIntType].u32NodeFlag)
                {
                    //printk("@@@@@@NEW=0x%x, OLD=0x%x\n", pstDisp->stList[eIntType].u32NodeFlagNew,
                    //    pstDisp->stList[eIntType].u32NodeFlag);

                    pstDisp->stList[eIntType].u32NodeFlag = pstDisp->stList[eIntType].u32NodeFlagNew;
                }
#if 0
                if (pstDisp->eEvent != HI_DRV_DISP_C_VT_INT)
                {
                    printk("isr eIntType=%d, 01=0x%x\n", eIntType,u32IntState);
                }
#endif

                v = pstDisp->stList[eIntType].u32NodeFlag;
                for(n=0; (n<32) && v; n++)
                {
                    if (v & (1 << n))
                    {
#if 0
                        if (pstDisp->eEvent != HI_DRV_DISP_C_VT_INT)
                        {
                            printk("##n=%d, id=%d, event=%d\n", n, enDisp,pstDisp->eEvent);
                        }
#endif
                        pstDisp->stList[eIntType].stNode[n].pfDISP_Callback(pstDisp->stList[eIntType].stNode[n].hDst, &pstDisp->stCBInfo);
                        v = v - (1<<n);

                        if( !irqs_disabled() )
                        {
                            DISP_PRINT("#######$$$$$$$$$$$............eIntType=%u, n=%d\n",eIntType, n);
                        }

                    }

                    if (!v)
                    {
                        break;
                    }
                }

                u32IntState = u32IntState & (~(HI_U32)s_DispIntTable[enDisp][eIntType]);
            }
        }
    }

    if (u32IntState)
    {
        DISP_ERROR("Unespexted interrup 0x%x happened!\n", u32IntState);
    }

    return DEF_DISP_ISR_Main_RETURN_VALUE;
}

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */













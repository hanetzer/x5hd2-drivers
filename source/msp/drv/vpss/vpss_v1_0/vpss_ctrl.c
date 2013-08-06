#include "vpss_ctrl.h"
#include "vpss_alg.h"
#include "vpss_common.h"
#include "drv_proc_ext.h"
#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

HI_U32 u32SuccessCount;


VPSS_CTRL_S stVpssCtrl;
static UMAP_DEVICE_S g_VpssRegisterData;

static struct file_operations s_VpssFileOps =
{
    .owner          = THIS_MODULE,
    .open           = NULL,
    .unlocked_ioctl = NULL,
    .release        = NULL,
};

static PM_BASEOPS_S  s_VpssBasicOps = {
    .probe        = NULL,
    .remove       = NULL,
    .shutdown     = NULL,
    .prepare      = NULL,
    .complete     = NULL,
    .suspend      = VPSS_CTRL_Suspend,
    .suspend_late = NULL,
    .resume_early = NULL,
    .resume       = VPSS_CTRL_Resume,
};

HI_S32 VPSS_CTRL_Suspend(PM_BASEDEV_S *pdev, pm_message_t state)
{
    HI_S32 s32Ret;
    /* >=2 means Thread is running*/
    if (stVpssCtrl.s32IsVPSSOpen >= 2)
    {
        s32Ret = VPSS_CTRL_DestoryThread();
        if (HI_FAILURE == s32Ret)
        {
            VPSS_FATAL("Can't Destory Thread\n");
        }
        VPSS_CTRL_CreateThread();
    }

    stVpssCtrl.bSuspend = HI_TRUE;
    return HI_SUCCESS;
}
HI_S32 VPSS_CTRL_Resume(PM_BASEDEV_S *pdev)
{
    if (stVpssCtrl.bSuspend)
    {
        wake_up_process(stVpssCtrl.hThread);
    }
    else
    {
        VPSS_FATAL("Vpss Resume Error\n");
    }
    return HI_SUCCESS;
}




HI_S32 VPSS_CTRL_GetVersion(HI_U32 u32Version,HI_U32 *pu32AuVersion,HI_U32 *pu32HalVersion)
{
    switch(u32Version)
    {
        case 0x101:
            *pu32AuVersion = ALG_VERSION_1;
            *pu32HalVersion = HAL_VERSION_1;
            break;
        case 0x102:
            *pu32AuVersion = ALG_VERSION_2;
            *pu32HalVersion = HAL_VERSION_2;
            break;
        default:
            VPSS_FATAL("VPSS driver verison is wrong\n");
            return HI_FAILURE;
    }
    return HI_SUCCESS;
}    
HI_S32 VPSS_CTRL_InitDev(HI_VOID)
{
    //HI_S32 s32Ret;
    
    stVpssCtrl.s32IsVPSSOpen = 0;

    #if 1
    sprintf(g_VpssRegisterData.devfs_name, UMAP_DEVNAME_VPSS);

    g_VpssRegisterData.fops   = &s_VpssFileOps;
    g_VpssRegisterData.minor  = UMAP_MIN_MINOR_VPSS;
    g_VpssRegisterData.owner  = THIS_MODULE;
    g_VpssRegisterData.drvops = &s_VpssBasicOps;
    
    if (HI_DRV_DEV_Register(&g_VpssRegisterData) < 0)
    {
        VPSS_FATAL("register VPSS failed.\n");
        return HI_FAILURE;
    }
    #endif

    return HI_SUCCESS;
}

HI_S32 VPSS_CTRL_DeInitDev(HI_VOID)
{
    //HI_S32 s32Ret;
    
    HI_DRV_DEV_UnRegister(&g_VpssRegisterData);
    
    return HI_SUCCESS;
}

HI_S32 VPSS_CTRL_Init(HI_U32 u32Version)
{
    HI_U32 u32AuVersion;
    HI_U32 u32HalVersion;
    HI_S32 s32Ret;
    
    /*hi_drv_vpss.h中定义vpss驱动版本号
      *由vpss驱动版本号，得到:
      *1.AuVersion 算法版本号
      *2.LogicVersion 适配逻辑版本号
      *
      *
      */
    s32Ret = VPSS_CTRL_GetVersion(u32Version,&u32AuVersion,&u32HalVersion);
    if (s32Ret != HI_SUCCESS)
    {
        return HI_FAILURE;
    }
    if(stVpssCtrl.s32IsVPSSOpen == 0)
    {
        VPSS_INST_CTRL_S * pstInstInfo;

        pstInstInfo = &(stVpssCtrl.stInstCtrlInfo);
    
        VPSS_CTRL_InitInstInfo(pstInstInfo);
        VPSS_ALG_Init(&(stVpssCtrl.stAlgCtrl));

        VPSS_HAL_Init(u32HalVersion, &(stVpssCtrl.stHalCaps));

        stVpssCtrl.bSuspend = HI_FALSE;

    }
    
    stVpssCtrl.s32IsVPSSOpen++;
    
    if(stVpssCtrl.s32IsVPSSOpen == 2)
    {
        wake_up_process(stVpssCtrl.hThread);
    }
    return HI_SUCCESS;
}

HI_S32 VPSS_CTRL_DelInit(HI_VOID)
{
    VPSS_HAL_CAP_S *pstHal;
    HI_U32 u32Count;
    HI_S32 s32Ret;
    pstHal = &(stVpssCtrl.stHalCaps);
    
    /*
    module载入后 IsVPSSOpen == 1
    */
    if (stVpssCtrl.s32IsVPSSOpen > 1)
    {
        stVpssCtrl.s32IsVPSSOpen--;

        if (stVpssCtrl.s32IsVPSSOpen == 1)
        {
            for(u32Count = 0; 
                u32Count < VPSS_INSTANCE_MAX_NUMB;
                u32Count ++)
            {
                if (stVpssCtrl.stInstCtrlInfo.pstInstPool[u32Count]
                    != HI_NULL)
                {
                    VPSS_FATAL("CTRL_DelInit Error");
                    return HI_FAILURE;
                }
                    
            }

            s32Ret = VPSS_CTRL_DestoryThread();
            if (HI_FAILURE == s32Ret)
            {
                VPSS_FATAL("Can't Destory Thread\n");
            }
            VPSS_CTRL_CreateThread();
            
        }
        
        return HI_SUCCESS;
    }
    else if(stVpssCtrl.s32IsVPSSOpen < 1)
    {
        VPSS_FATAL("\nVPSS hasn't initted \t\n");
        return HI_FAILURE;
    }
    else
    {
        pstHal->PFN_VPSS_HAL_ClearIntState(0xf);
        pstHal->PFN_VPSS_HAL_SetIntMask(0x0);

        VPSS_ALG_DelInit(&(stVpssCtrl.stAlgCtrl));
        VPSS_HAL_DelInit();
    }

    return HI_SUCCESS;
}
VPSS_HANDLE VPSS_CTRL_CreateInstance(HI_DRV_VPSS_CFG_S *pstVpssCfg)
{
    VPSS_INSTANCE_S* pstInstance;
    HI_S32 s32InstHandle;
    HI_S32 s32Ret;
    
    pstInstance = (VPSS_INSTANCE_S*)VPSS_VMALLOC(sizeof(VPSS_INSTANCE_S));
    
    VPSS_OSAL_InitLOCK(&(pstInstance->stInstLock),1);
    VPSS_OSAL_InitSpin(&(pstInstance->stUsrSetSpin),1);
    
    s32Ret = VPSS_INST_Init(pstInstance,pstVpssCfg);
    
    s32InstHandle = VPSS_CTRL_AddInstance(pstInstance);
    if (s32InstHandle != VPSS_INVALID_HANDLE)
    {
        s32Ret = VPSS_CTRL_CreateInstProc(s32InstHandle);
    }
    if (s32Ret != HI_SUCCESS)
    {
        VPSS_WARN("CreateInstance Error \n");
    }
    return s32InstHandle;
    
}

HI_S32 VPSS_CTRL_DestoryInstance(VPSS_HANDLE hVPSS)
{
    HI_S32 s32Ret;
    VPSS_INSTANCE_S* pstInstance;

    pstInstance = VPSS_CTRL_GetInstance(hVPSS);

    if (pstInstance == HI_NULL)
    {
        return HI_FAILURE;
    }
    
    /*
        删除INST节点时要先获取锁,保证删除实例没有被服务
    */

    
    stVpssCtrl.u32ThreadSleep = 1;
    s32Ret = VPSS_OSAL_TryLock(&(pstInstance->stInstLock)); 
    if (s32Ret != HI_SUCCESS || stVpssCtrl.s32ThreadPos != 5)
    {
        if (s32Ret == HI_SUCCESS)
        {
            VPSS_OSAL_UpLock(&(pstInstance->stInstLock));
        }
        
        while(VPSS_OSAL_TryLock(&(pstInstance->stInstLock) ) != HI_SUCCESS
                 || stVpssCtrl.s32ThreadPos != 5)
        /*
        while(stVpssCtrl.s32ThreadPos != 5 
                && VPSS_OSAL_DownLock(&(pstInstance->stInstLock) )!= HI_SUCCESS)
            */
        {

            if (s32Ret == HI_SUCCESS)
            {
                VPSS_OSAL_UpLock(&(pstInstance->stInstLock));
            }
            schedule_timeout(5*HZ);
        }

    }

    VPSS_CTRL_DestoryInstProc(hVPSS);
    VPSS_CTRL_DelInstance(hVPSS);
    
    VPSS_OSAL_UpLock(&(pstInstance->stInstLock));
    
    s32Ret = VPSS_INST_DelInit(pstInstance);
    
    VPSS_VFREE(pstInstance);

    pstInstance = HI_NULL;
    
    stVpssCtrl.u32ThreadSleep = 0;
    return HI_SUCCESS;

}

VPSS_INSTANCE_S *VPSS_CTRL_GetInstance(VPSS_HANDLE hVPSS)
{
    VPSS_INST_CTRL_S *pstInstCtrlInfo;
    
    
    pstInstCtrlInfo = &(stVpssCtrl.stInstCtrlInfo);
    
    if (hVPSS < 0 
        || hVPSS >= VPSS_INSTANCE_MAX_NUMB)
    {
        VPSS_FATAL("\nInvalid VPSS HANDLE------------->func %s line %d \r\n",
                    __func__, __LINE__);
        return HI_NULL;
    }

	//VPSS_OSAL_DownLock(&(pstInstCtrlInfo->stListLock));
    if(pstInstCtrlInfo->pstInstPool[hVPSS] != HI_NULL)
    {
		//VPSS_OSAL_UpLock(&(pstInstCtrlInfo->stListLock));
        return pstInstCtrlInfo->pstInstPool[hVPSS];
    }
    else
    {
		//VPSS_OSAL_UpLock(&(pstInstCtrlInfo->stListLock));
        VPSS_FATAL("\nVPSS isn't Created------------->func %s line %d \r\n",
                    __func__, __LINE__);
        return HI_NULL;
    }
    
}
HI_S32 VPSS_CTRL_CreateThread(HI_VOID)
{
    stVpssCtrl.u32ThreadKilled = 0;
    stVpssCtrl.u32ThreadSleep = 0;
    stVpssCtrl.hThread = 
        kthread_create(VPSS_CTRL_ThreadProc, (HI_VOID *)NULL, "VPSS THREAD");
    
    if (HI_NULL == stVpssCtrl.hThread)
    {
        VPSS_FATAL("\nCan not create thread!\n");
        return HI_FAILURE;
    }
    
    return HI_SUCCESS;
}

HI_S32 VPSS_CTRL_WakeUpThread(HI_VOID)
{
    if (stVpssCtrl.stTask.stState == TASK_STATE_IDLE)
    {
        VPSS_OSAL_GiveEvent(&(stVpssCtrl.stNewTask), 1, 0);
    }

    return HI_SUCCESS;
}
HI_S32 VPSS_CTRL_DestoryThread(HI_VOID)
{
    HI_S32 s32Ret;

    s32Ret = kthread_stop(stVpssCtrl.hThread);
    
    if (s32Ret != HI_SUCCESS)
    {
    
    }
    else
    {
        if(stVpssCtrl.s32ThreadPos != 6)
        {
            VPSS_FATAL("Destory Thread Error \n");
            return HI_FAILURE;
        }
    }
    return HI_SUCCESS;
}
HI_S32 VPSS_CTRL_RegistISR(HI_VOID)
{
    
    if (request_irq(VPSS_IRQ_NUM, (irq_handler_t)VPSS_CTRL_IntServe_Proc, 
                    IRQF_SHARED, "VPSS_IRQ", &(stVpssCtrl.hVpssIRQ)))
    {
        VPSS_FATAL("VPSS registe IRQ failed!\n");
    }
    

    return HI_SUCCESS;
}

HI_S32 VPSS_CTRL_UnRegistISR(HI_VOID)
{
    free_irq(VPSS_IRQ_NUM, &(stVpssCtrl.hVpssIRQ));

    return HI_SUCCESS;
}

HI_S32 VPSS_CTRL_InitInstInfo(VPSS_INST_CTRL_S *pstInstInfo)
{
    HI_U32 u32Count;

    VPSS_OSAL_InitLOCK(&(pstInstInfo->stListLock),1);
    
    for(u32Count = 0; u32Count < VPSS_INSTANCE_MAX_NUMB; u32Count ++)
    {
        pstInstInfo->pstInstPool[u32Count] = HI_NULL;
    }

    pstInstInfo->u32Target = 0;

    
    return HI_SUCCESS;
}

HI_U32 u32IRQCnt = 0;
HI_U32 u32THRCnt = 0;
HI_S32 VPSS_CTRL_ThreadProc(HI_VOID* pArg)
{   
    HI_U32 u32NowTime = 0;
    /*============================= 处理运行 ===========================*/
    stVpssCtrl.s32ThreadPos = 0;

    VPSS_OSAL_InitEvent(&(stVpssCtrl.stTaskComplete), EVENT_UNDO, EVENT_UNDO);
    VPSS_OSAL_InitEvent(&(stVpssCtrl.stNewTask), EVENT_UNDO, EVENT_UNDO);
    u32SuccessCount = 0;
    stVpssCtrl.stTask.u32LastTotal = 0;
    stVpssCtrl.stTask.u32Create = 0;
    stVpssCtrl.stTask.u32Fail = 0;
    stVpssCtrl.stTask.u32TimeOut= 0;
    while(!kthread_should_stop())
    {
        HI_S32 s32Ret;
        stVpssCtrl.stTask.stState = TASK_STATE_READY;

        if(stVpssCtrl.u32ThreadKilled == 1)
        {
            goto VpssThreadExit;
        }
        if(stVpssCtrl.u32ThreadSleep== 1)
        {
            goto VpssThreadIdle;
        }
        /*调用CTRL接口，创建一个TASK，返回0*/
        
        stVpssCtrl.stTask.u32Create++;
        s32Ret =  VPSS_CTRL_CreateTask(&(stVpssCtrl.stTask));

        /* 创建成功 运行 --> 等待 */
        if(s32Ret == HI_SUCCESS)
        {
            VPSS_INFO("\n...............CreateTask");
            stVpssCtrl.s32ThreadPos = 2;

            VPSS_OSAL_ResetEvent(&(stVpssCtrl.stTaskComplete), EVENT_UNDO, EVENT_UNDO);
            s32Ret = VPSS_CTRL_StartTask(&(stVpssCtrl.stTask));
            
            if (s32Ret == HI_SUCCESS)
            {
                VPSS_INFO("\n...............StartTask\n");
                /*
                    此处直接用内核等待事件，逻辑完成中断会唤醒线程
                    */
                stVpssCtrl.stTask.stState = TASK_STATE_WAIT;

                u32THRCnt++;
                s32Ret = VPSS_OSAL_WaitEvent(&(stVpssCtrl.stTaskComplete),HZ);
                u32THRCnt++;
            }
            else
            {
                VPSS_INFO("\n...............StartTask Faild");
                s32Ret = HI_FAILURE;
            }

            /*中断唤醒*/
            if (s32Ret == HI_SUCCESS)
            {
                stVpssCtrl.s32ThreadPos = 3;

                VPSS_CTRL_CompleteTask(&(stVpssCtrl.stTask));

                /*get logic dei date*/
                //VPSS_CTRL_StoreDeiData(stVpssCtrl.stTask.pstInstance);
                
                if(jiffies - u32NowTime >= HZ)
                {
                    u32NowTime = jiffies;
                    stVpssCtrl.stTask.u32SucRate = u32SuccessCount 
                                    - stVpssCtrl.stTask.u32LastTotal;
                    stVpssCtrl.stTask.u32LastTotal = u32SuccessCount;
                }
                
                u32SuccessCount++;
                VPSS_INFO("\n...............CompleteTask  %d",u32SuccessCount);
                
            }
            else/*超时*/
            {
                stVpssCtrl.s32ThreadPos = 4;
                stVpssCtrl.stTask.u32TimeOut++;
                /*1.逻辑复位*/
                VPSS_REG_ReSetCRG(); 
                VPSS_HAL_SetIntMask(0x0);
                VPSS_HAL_SetTimeOut(DEF_LOGIC_TIMEOUT);
                /*2.任务复位*/
                VPSS_CTRL_ClearTask(&(stVpssCtrl.stTask));
                VPSS_FATAL("\n...............ClearTask");
            }
            
        }
        else/*创建失败 运行 --> 空闲*/
        {
            stVpssCtrl.stTask.u32Fail++;
VpssThreadIdle:
            
            stVpssCtrl.s32ThreadPos = 5;

            stVpssCtrl.stTask.stState = TASK_STATE_IDLE;

            
            VPSS_OSAL_ResetEvent(&(stVpssCtrl.stNewTask), EVENT_UNDO, EVENT_UNDO);
            
            s32Ret = VPSS_OSAL_WaitEvent(&(stVpssCtrl.stNewTask),HZ/100);

            if(s32Ret == HI_SUCCESS)
            {
                VPSS_INFO("\n WakeUpThread SUCCESS \n");
            }
            
        }
        
    }

VpssThreadExit:
    stVpssCtrl.s32ThreadPos = 6;

    VPSS_INFO("\ns32ThreadPos = %d...",stVpssCtrl.s32ThreadPos);
    
    return HI_SUCCESS;
}


VPSS_INSTANCE_S *VPSS_CTRL_GetServiceInstance(HI_VOID)
{
    VPSS_INST_CTRL_S  *pstInstCtrlInfo;
    VPSS_INSTANCE_S *pstInstance;
    HI_S32 s32Ret;
    HI_U32 u32FirstCycle;
    HI_U32 u32Count;

    
    pstInstCtrlInfo = &(stVpssCtrl.stInstCtrlInfo);
    
    u32FirstCycle = 1;

    u32Count = pstInstCtrlInfo->u32Target;
    while(u32FirstCycle != 2)
    {
        VPSS_OSAL_DownLock(&(pstInstCtrlInfo->stListLock));
        pstInstance = pstInstCtrlInfo->pstInstPool[u32Count];
        VPSS_OSAL_UpLock(&(pstInstCtrlInfo->stListLock));
        
        if (pstInstance != HI_NULL)
        {
            s32Ret = VPSS_INST_SyncUsrCfg(pstInstance);
            if (s32Ret == HI_SUCCESS)
            {
                //printk("\n TASK SYNC OK\n");
            }
            else
            {
                //printk("\n--------------------------------TASK SYNC FAIL\n");
            }
        	s32Ret = VPSS_OSAL_TryLock(&(pstInstance->stInstLock));
        	
            if (s32Ret == HI_SUCCESS)
            {
                s32Ret = VPSS_INST_CheckIsAvailable(pstInstance);
                
                if (s32Ret)
                {
                    pstInstCtrlInfo->u32Target = u32Count+1;
                    return pstInstance;
                }
                else
                {
                    VPSS_OSAL_UpLock(&(pstInstance->stInstLock));
                }
            }
            else
            {
                VPSS_INFO("\nCan't Get Lock");
            }   
        }
        u32Count = (u32Count + 1)%VPSS_INSTANCE_MAX_NUMB;

        if(u32Count == pstInstCtrlInfo->u32Target)
        {
            u32FirstCycle = 2;
        }
    }
    
    return HI_NULL;
}


HI_S32 VPSS_CTRL_CreateTask(VPSS_TASK_S *pstTask)
{
    HI_U32 u32Count;
    VPSS_PORT_S* pstPort;
    HI_S32 s32Ret;
    HI_DRV_VPSS_BUFLIST_CFG_S  *pstBufListCfg;
    HI_U32 u32StoreH;
    HI_U32 u32StoreW;
    
    HI_DRV_VIDEO_FRAME_S *pstImg = HI_NULL;
    /*
        遍历实例队列，找出一个可服务实例，
    */
    pstTask->pstInstance = VPSS_CTRL_GetServiceInstance();
    
    if(HI_NULL == pstTask->pstInstance)
    {
        return HI_FAILURE;
    }
    
    VPSS_INFO("\n GetServiceInstance---%d OK",(pstTask->pstInstance)->ID);
    /*
    这是一段奇怪的代码，为了在保证图像宽高不变时，动态调整要获取的用户帧存大小
    */
    /***********************************/
    pstImg = VPSS_INST_GetUndoImage(pstTask->pstInstance);
    
    if (pstImg == HI_NULL)
    {
        VPSS_FATAL("\n CreateTask there is no src image \n");
        return HI_FAILURE;
    }
    /***********************************/
    
    for(u32Count = 0;u32Count < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER;u32Count++)
    {
        pstPort = &((pstTask->pstInstance)->stPort[u32Count]);
        pstBufListCfg = &(pstPort->stFrmInfo.stBufListCfg);
        
        /*某PORT是否输出BUFFER要考虑输出帧率*/
        if (pstPort->s32PortId != VPSS_INVALID_HANDLE &&
            pstPort->bEnble == HI_TRUE)
        {
            if(pstImg->eFrmType == HI_DRV_FT_NOT_STEREO)
            {
                pstTask->pstFrmNode[u32Count*2] = 
                    VPSS_FB_GetEmptyFrmBuf(&(pstPort->stFrmInfo));
                pstTask->pstFrmNode[u32Count*2+1] = HI_NULL;
            }
            else
            {
                pstTask->pstFrmNode[u32Count*2] = 
                    VPSS_FB_GetEmptyFrmBuf(&(pstPort->stFrmInfo));
                pstTask->pstFrmNode[u32Count*2+1] = 
                    VPSS_FB_GetEmptyFrmBuf(&(pstPort->stFrmInfo));
                if (pstTask->pstFrmNode[u32Count*2] == HI_NULL
                    || pstTask->pstFrmNode[u32Count*2+1] == HI_NULL)
                {
                    if(pstTask->pstFrmNode[u32Count*2] != HI_NULL)
                    {
                        VPSS_FB_AddEmptyFrmBuf(&(pstPort->stFrmInfo),pstTask->pstFrmNode[u32Count*2]);
                    }

                    if(pstTask->pstFrmNode[u32Count*2+1] != HI_NULL)
                    {
                        VPSS_FB_AddEmptyFrmBuf(&(pstPort->stFrmInfo),pstTask->pstFrmNode[u32Count*2+1]);
                    }
                    pstTask->pstFrmNode[u32Count*2] = HI_NULL;
                    pstTask->pstFrmNode[u32Count*2+1] = HI_NULL;
                }
            }
            #if 1
            if(pstBufListCfg->eBufType == HI_DRV_VPSS_BUF_USER_ALLOC_MANAGE)
            {
                /*************************/
                if(pstPort->s32OutputHeight == 0 && pstPort->s32OutputWidth == 0)
                {
                    if(pstImg->u32Height == 1088 && pstImg->u32Width == 1920)
                    {
                        u32StoreH = 1080;
                        u32StoreW = 1920;
                    }
                    else
                    {
                        u32StoreH = pstImg->u32Height;
                        u32StoreW = pstImg->u32Width;
                    }
                }
                else
                {
                    u32StoreH = pstPort->s32OutputHeight;
                    u32StoreW = pstPort->s32OutputWidth;
                }

                if(pstPort->eFormat == HI_DRV_PIX_FMT_NV12 
                       || pstPort->eFormat == HI_DRV_PIX_FMT_NV21)
                {
                    pstBufListCfg->u32BufStride = (u32StoreW + 0xf) & 0xfffffff0 ;
                    pstBufListCfg->u32BufSize = 
                                pstBufListCfg->u32BufStride * u32StoreH * 3 / 2;
                }
                else if(pstPort->eFormat == HI_DRV_PIX_FMT_NV16 
                        || pstPort->eFormat == HI_DRV_PIX_FMT_NV61)
                {
                    pstBufListCfg->u32BufStride =  (u32StoreW + 0xf) & 0xfffffff0 ;
                    pstBufListCfg->u32BufSize = 
                                pstBufListCfg->u32BufStride * u32StoreH * 2;
                }
                else
                {
                    VPSS_FATAL("Port %x OutFormat isn't supported ",pstPort->s32PortId);
                }
                /*************************/
                if(pstTask->pstFrmNode[u32Count*2] != HI_NULL)
                {
                    s32Ret = VPSS_INST_GetFrmBuffer(pstTask->pstInstance,pstPort->s32PortId,pstBufListCfg,
                            &(pstTask->pstFrmNode[u32Count*2]->stBuffer),u32StoreH,u32StoreW);
                    if (s32Ret != HI_SUCCESS)
                    {
                        VPSS_FB_AddEmptyFrmBuf(&(pstPort->stFrmInfo),pstTask->pstFrmNode[u32Count*2]);
                        if (pstTask->pstFrmNode[u32Count*2+1] != HI_NULL)
                        {
                            VPSS_FB_AddEmptyFrmBuf(&(pstPort->stFrmInfo),pstTask->pstFrmNode[u32Count*2+1]);
                        }
                        VPSS_OSAL_UpLock(&(pstTask->pstInstance->stInstLock));
                        return HI_FAILURE;
                    }
                }

                if(pstTask->pstFrmNode[u32Count*2+1] != HI_NULL)
                {
                    s32Ret = VPSS_INST_GetFrmBuffer(pstTask->pstInstance,pstPort->s32PortId,pstBufListCfg,
                            &(pstTask->pstFrmNode[u32Count*2+1]->stBuffer),u32StoreH,u32StoreW);
                    if(s32Ret != HI_SUCCESS)
                    {
                        VPSS_FB_AddEmptyFrmBuf(&(pstPort->stFrmInfo),pstTask->pstFrmNode[u32Count*2]); 
                        VPSS_FB_AddEmptyFrmBuf(&(pstPort->stFrmInfo),pstTask->pstFrmNode[u32Count*2+1]);
                        VPSS_OSAL_UpLock(&(pstTask->pstInstance->stInstLock));
                        return HI_FAILURE;
                    }
                }
            }
            else
            {

            }
            #endif
        }
        else
        {
            pstTask->pstFrmNode[u32Count*2] = HI_NULL;
            pstTask->pstFrmNode[u32Count*2+1] = HI_NULL;
        }
    }

    return HI_SUCCESS;

}
HI_S32 VPSS_CTRL_StartTask(VPSS_TASK_S *pstTask)
{
    HI_S32 s32Ret;
    VPSS_INSTANCE_S *pstInst;
    VPSS_HAL_CAP_S *pstHal;
    HI_DRV_VIDEO_FRAME_S *pstImg;
    VPSS_ALG_CFG_S *pstAlgCfg;
    VPSS_ALG_FRMCFG_S *pstImgCfg;
    HI_BOOL bUseTwoNode = HI_FALSE;
    HI_RECT_S stInRect;
    
    pstInst = pstTask->pstInstance;
    
    pstHal = &(stVpssCtrl.stHalCaps);
    
    /*1.释放上次已处理帧，获取本地待处理帧*/
  
    
    pstImg = VPSS_INST_GetUndoImage(pstInst);
    
    if (pstImg == HI_NULL)
    {
        VPSS_FATAL("\n ConfigOutFrame there is no src image \n");
        return HI_FAILURE;
    }
    
    
    /*2D 源输入*/
    pstAlgCfg = &(pstTask->stAlgCfg[0]);

    pstImgCfg = &(pstAlgCfg->stSrcImgInfo);
    pstImgCfg->eLReye = HI_DRV_BUF_ADDR_LEFT;

    if(pstInst->stProcCtrl.bUseCropRect == HI_FALSE)
    {
        stInRect.s32Height = pstInst->stProcCtrl.stInRect.s32Height;
        stInRect.s32Width  = pstInst->stProcCtrl.stInRect.s32Width;
        stInRect.s32X      = pstInst->stProcCtrl.stInRect.s32X;
        stInRect.s32Y      = pstInst->stProcCtrl.stInRect.s32Y;
    }
    else
    {
        
        stInRect.s32Height = pstImg->u32Height
                             - pstInst->stProcCtrl.stCropRect.u32TopOffset
                             - pstInst->stProcCtrl.stCropRect.u32BottomOffset;
        if (pstImg->u32Height == 1088 && pstImg->u32Width == 1920)
        {
            stInRect.s32Height = stInRect.s32Height - 8;
        }
        stInRect.s32Width  = pstImg->u32Width 
                             - pstInst->stProcCtrl.stCropRect.u32LeftOffset
                             - pstInst->stProcCtrl.stCropRect.u32RightOffset;
        stInRect.s32X      = pstInst->stProcCtrl.stCropRect.u32LeftOffset;
        stInRect.s32Y      = pstInst->stProcCtrl.stCropRect.u32TopOffset;
        
    }
    s32Ret = VPSS_ALG_SetImageInfo(pstImgCfg,pstImg,stInRect);
    if (s32Ret != HI_SUCCESS)
    {
        return HI_FAILURE;
    }
    /*2.1 根据算法检测，修正输入img的TopFirst信息*/
    pstImgCfg->bTopFieldFirst = pstInst->u32RealTopFirst;
    
	s32Ret = VPSS_CTRL_ConfigOutFrame(pstTask,pstAlgCfg,HI_DRV_BUF_ADDR_LEFT);
    if (s32Ret != HI_SUCCESS)
    {
        return HI_FAILURE;
    }
	
	s32Ret = pstHal->PFN_VPSS_HAL_SetHalCfg(&(pstTask->stAlgCfg[0]),0);
    if (s32Ret != HI_SUCCESS)
    {
        return HI_FAILURE;
    }
    
    if(pstImg->eFrmType != HI_DRV_FT_NOT_STEREO)
    {
        pstAlgCfg = &(pstTask->stAlgCfg[1]);

        pstImgCfg = &(pstAlgCfg->stSrcImgInfo);
        pstImgCfg->eLReye = HI_DRV_BUF_ADDR_RIGHT;

        stInRect.s32Height = pstInst->stProcCtrl.stInRect.s32Height;
        stInRect.s32Width  = pstInst->stProcCtrl.stInRect.s32Width;
        stInRect.s32X      = pstInst->stProcCtrl.stInRect.s32X;
        stInRect.s32Y      = pstInst->stProcCtrl.stInRect.s32X;
        
        VPSS_ALG_SetImageInfo(pstImgCfg,pstImg,stInRect);
        
        s32Ret = VPSS_CTRL_ConfigOutFrame(pstTask,pstAlgCfg,HI_DRV_BUF_ADDR_RIGHT);
       
    /*
        printk("\n SetImageInfo HI_DRV_BUF_ADDR_RIGHT"
                   "SrcH=%d \n"
                   "SrcW=%d \n"
                   "ImgH=%d \n"
                   "ImgW=%d \n",
                                pstImg->u32Height,pstImg->u32Width,
                                pstImgCfg->u32Height,pstImgCfg->u32Width
                                );
        */
        s32Ret = pstHal->PFN_VPSS_HAL_SetHalCfg(&(pstTask->stAlgCfg[1]),1);

        bUseTwoNode = HI_TRUE;
        
        if (s32Ret != HI_SUCCESS)
        {
            return HI_FAILURE;
        }
    }
    
    pstHal->PFN_VPSS_HAL_StartLogic(bUseTwoNode);
    

    return HI_SUCCESS;
    
}

HI_S32 VPSS_CTRL_CompleteTask(VPSS_TASK_S *pstTask)
{
    HI_U32 u32Count;
    VPSS_PORT_S *pstPort;
    VPSS_INSTANCE_S *pstInstance;
    VPSS_FB_NODE_S *pstLeftFbNode;
    VPSS_FB_NODE_S *pstRightFbNode;
    HI_BOOL bDropped;
    HI_DRV_VIDEO_FRAME_S stTmpFrame;
    HI_DRV_VIDEO_PRIVATE_S *pstPriv;
    
    pstInstance = pstTask->pstInstance;
   
    
    /*释放已处理帧*/
    if(pstInstance->eSrcImgMode == VPSS_SOURCE_MODE_VPSSACTIVE)
    {
        VPSS_INST_RelDoneImage(pstTask->pstInstance);
    }
     /*移动UNDOIMG指针，指向下一个要处理的IMG前一个*/
    VPSS_INST_CompleteUndoImage(pstInstance);
    
    /*将FRMNODE添加到输出PORT的FULFRMLIST*/
    for(u32Count = 0;u32Count < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER;u32Count++)
    {
        pstPort = &(pstInstance->stPort[u32Count]);
        if(pstPort->s32PortId == VPSS_INVALID_HANDLE)
        {
            continue;
        }
        pstLeftFbNode = pstTask->pstFrmNode[u32Count*2];
        pstRightFbNode = pstTask->pstFrmNode[u32Count*2 + 1];

        bDropped = HI_FALSE;
        if (pstLeftFbNode != HI_NULL || pstRightFbNode != HI_NULL)
        {
            pstPort->u32OutCount ++;
            bDropped = VPSS_INST_CheckIsDropped(pstInstance, 
                        pstPort->u32MaxFrameRate, 
                        pstPort->u32OutCount);
            if (pstLeftFbNode != HI_NULL)
            {
                memcpy(&stTmpFrame,&(pstLeftFbNode->stOutFrame),sizeof(HI_DRV_VIDEO_FRAME_S));
                pstPriv = (HI_DRV_VIDEO_PRIVATE_S*)&(stTmpFrame.u32Priv[0]);
                if (pstPriv->u32LastFlag == DEF_HI_DRV_VPSS_LAST_FRAME_FLAG)
                {
                    bDropped = HI_FALSE;
                }
            }
        }
        if (pstLeftFbNode != HI_NULL ) 
        {
            memcpy(&stTmpFrame,&(pstLeftFbNode->stOutFrame),sizeof(HI_DRV_VIDEO_FRAME_S));
            if(pstPort->stFrmInfo.stBufListCfg.eBufType == HI_DRV_VPSS_BUF_USER_ALLOC_MANAGE)
            {
                if(HI_FALSE == bDropped)
                {
                    VPSS_INST_ReportNewFrm(pstTask->pstInstance,
                            pstPort->s32PortId,
                            &(pstLeftFbNode->stOutFrame));
                }
                else
                {
                
                    VPSS_INST_RelFrmBuffer(pstTask->pstInstance,pstPort->s32PortId,
                        &(pstPort->stFrmInfo.stBufListCfg),
                        &(pstLeftFbNode->stBuffer.stMMZBuf));
                }
                VPSS_FB_AddEmptyFrmBuf(&(pstPort->stFrmInfo),pstTask->pstFrmNode[u32Count]);
            }
            else
            {
                if (HI_FALSE == bDropped)
                {
                    pstPriv = (HI_DRV_VIDEO_PRIVATE_S *)&(pstLeftFbNode->stOutFrame.u32Priv[0]);
                    pstPriv->u32FrmCnt = pstPort->u32OutCount;
                    VPSS_FB_AddFulFrmBuf(&(pstPort->stFrmInfo),pstLeftFbNode);
                }
                else
                {
                    VPSS_FB_AddEmptyFrmBuf(&(pstPort->stFrmInfo),pstLeftFbNode);
                }
            }
        }

        if(pstRightFbNode != HI_NULL) 
        {
            
            if(pstPort->stFrmInfo.stBufListCfg.eBufType == HI_DRV_VPSS_BUF_USER_ALLOC_MANAGE)
            {
                if(HI_FALSE == bDropped)
                {
                    memcpy(&(pstRightFbNode->stOutFrame.stBufAddr[0]),
                            &(stTmpFrame.stBufAddr[0]),
                            sizeof(HI_DRV_VID_FRAME_ADDR_S));
                    if(HI_FALSE == bDropped)
                    {
                        VPSS_INST_ReportNewFrm(pstTask->pstInstance,
                                pstPort->s32PortId,
                                &(pstRightFbNode->stOutFrame));
                    }
                    else
                    {
                        VPSS_INST_RelFrmBuffer(pstTask->pstInstance,pstPort->s32PortId,
                        &(pstPort->stFrmInfo.stBufListCfg),
                        &(pstRightFbNode->stBuffer.stMMZBuf));
                    }
                  
                }

                VPSS_FB_AddEmptyFrmBuf(&(pstPort->stFrmInfo),pstTask->pstFrmNode[u32Count]);
            }
            else
            {
                /*此处有问题，因为右眼数据可能已经释放，VDP中断随时可能来要数据，可能要到的不对*/
                memcpy(&(pstRightFbNode->stOutFrame.stBufAddr[0]),
                            &(stTmpFrame.stBufAddr[0]),
                            sizeof(HI_DRV_VID_FRAME_ADDR_S));
                if (HI_FALSE == bDropped)
                {
                    pstPriv = (HI_DRV_VIDEO_PRIVATE_S *)&(pstRightFbNode->stOutFrame.u32Priv[0]);
                    pstPriv->u32FrmCnt = pstPort->u32OutCount;
                    VPSS_FB_AddFulFrmBuf(&(pstPort->stFrmInfo),pstRightFbNode);
                }
                else
                {
                    VPSS_FB_AddEmptyFrmBuf(&(pstPort->stFrmInfo),pstRightFbNode);
                }
            }
        }
    }

	pstInstance->u32IsNewImage = HI_TRUE;
    /*打开INSTNODE占用锁*/
    VPSS_OSAL_UpLock(&(pstInstance->stInstLock));
    
    return HI_SUCCESS;
}

HI_S32 VPSS_CTRL_ClearTask(VPSS_TASK_S *pstTask)
{
    HI_U32 u32Count;
    VPSS_PORT_S *pstPort;
    VPSS_INSTANCE_S *pstInstance;
    VPSS_FB_NODE_S *pstFbNode;
    
    for(u32Count = 0;u32Count < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER;u32Count++)
    {
        pstPort = &((pstTask->pstInstance)->stPort[u32Count]);
        pstFbNode = pstTask->pstFrmNode[u32Count*2];
        if(pstFbNode != HI_NULL) 
        {
            if(pstPort->stFrmInfo.stBufListCfg.eBufType == HI_DRV_VPSS_BUF_USER_ALLOC_MANAGE)
            {
                VPSS_INST_RelFrmBuffer(pstTask->pstInstance,pstPort->s32PortId,
                        &(pstPort->stFrmInfo.stBufListCfg),
                        &(pstTask->pstFrmNode[u32Count]->stBuffer.stMMZBuf));
            }
            VPSS_FB_AddEmptyFrmBuf(&(pstPort->stFrmInfo),pstFbNode);
        }
        
        pstFbNode = pstTask->pstFrmNode[u32Count*2 + 1];
        if(pstFbNode != HI_NULL) 
        {
            
            if(pstPort->stFrmInfo.stBufListCfg.eBufType == HI_DRV_VPSS_BUF_USER_ALLOC_MANAGE)
            {
                VPSS_INST_RelFrmBuffer(pstTask->pstInstance,pstPort->s32PortId,
                        &(pstPort->stFrmInfo.stBufListCfg),
                        &(pstTask->pstFrmNode[u32Count]->stBuffer.stMMZBuf));
            }
            VPSS_FB_AddEmptyFrmBuf(&(pstPort->stFrmInfo),pstFbNode);
        }
    }

    /*打开INSTNODE占用锁*/
    pstInstance = pstTask->pstInstance;
	pstInstance->u32IsNewImage = HI_FALSE;
    VPSS_OSAL_UpLock(&(pstInstance->stInstLock));

    
    return HI_SUCCESS;
    
}



HI_S32 VPSS_CTRL_GetDstFrmCfg(VPSS_ALG_FRMCFG_S *pstFrmCfg,
                                VPSS_FB_NODE_S *pstFrmNode,
                                VPSS_PORT_S *pstPort,
                                HI_DRV_VIDEO_FRAME_S *pstImage,
                                HI_BOOL bRWZB)
{
    HI_DRV_VIDEO_FRAME_S *pstFrm;
    VPSS_BUFFER_S *pstBuff;
    HI_U32 u32Stride;
    HI_U32 u32PhyAddr;
	
    HI_U32 u32OutHeight;
    HI_U32 u32OutWidth;
    HI_DRV_VIDEO_PRIVATE_S *pstPriv;
    
    pstFrm = &(pstFrmNode->stOutFrame);
    pstBuff = &(pstFrmNode->stBuffer);

    /*1.完全复制输入的image信息*/
    memcpy(pstFrm,pstImage,sizeof(HI_DRV_VIDEO_FRAME_S));

    /*2.根据vpss的配置，改动相关项*/
    if(pstPort->s32OutputHeight == 0 && pstPort->s32OutputWidth == 0)
    {
        u32OutHeight =  pstImage->u32Height;
        u32OutWidth =  pstImage->u32Width;
    }
    else
    {
        if (!bRWZB)
        {
            u32OutHeight = pstPort->s32OutputHeight;
            u32OutWidth = pstPort->s32OutputWidth;
        }
        else
        {
            u32OutHeight =  pstImage->u32Height;
            u32OutWidth =  pstImage->u32Width;
        }
    }
    /*透传输入宽高，可能引入1920*1088的图*/
    if(u32OutHeight == 1088 && u32OutWidth == 1920)
    {
        u32OutHeight = 1080;
    }
    
    pstFrm->u32Height = u32OutHeight;
    pstFrm->u32Width = u32OutWidth;

    if (pstFrm->u32Width > pstFrmNode->stBuffer.u32Stride
        || pstFrm->u32Height*pstFrm->u32Width*3/2 > pstFrmNode->stBuffer.stMMZBuf.u32Size)
    {
        VPSS_FATAL("Write Buffer is too small %x.\n",pstFrmNode->stBuffer.stMMZBuf.u32Size);
        return HI_FAILURE;
    }
    
    pstFrm->ePixFormat = pstPort->eFormat;
    
    pstFrm->bProgressive = HI_TRUE;
    pstFrm->enFieldMode = HI_DRV_FIELD_ALL;
    pstPriv = (HI_DRV_VIDEO_PRIVATE_S *)&(pstFrm->u32Priv[0]);
    
    if (pstImage->bProgressive == HI_FALSE)
    {
        pstPriv->eOriginField = pstImage->enFieldMode;
    }
    else
    {
        pstPriv->eOriginField = HI_DRV_FIELD_ALL;
    }
    
    pstPriv->stOriginImageRect.s32X = 0 ;
    pstPriv->stOriginImageRect.s32Y = 0;
    pstPriv->stOriginImageRect.s32Width = pstImage->u32Width;
    pstPriv->stOriginImageRect.s32Height = pstImage->u32Height;
    
    u32Stride = pstBuff->u32Stride;
    u32PhyAddr = (pstBuff->stMMZBuf).u32StartPhyAddr;
    if(pstFrmCfg->eLReye == HI_DRV_BUF_ADDR_LEFT)
    {
        pstFrm->stBufAddr[0].u32Stride_Y  =  u32Stride;
        pstFrm->stBufAddr[0].u32Stride_C  =  u32Stride;

        pstFrm->stBufAddr[0].u32PhyAddr_Y =  u32PhyAddr;
        pstFrm->stBufAddr[0].u32PhyAddr_C =  u32PhyAddr + 
                                    u32Stride*pstFrm->u32Height;
    }
    else
    {
        
        pstFrm->stBufAddr[1].u32Stride_Y  =  u32Stride;
        pstFrm->stBufAddr[1].u32Stride_C  =  u32Stride;

        pstFrm->stBufAddr[1].u32PhyAddr_Y =  u32PhyAddr;
        pstFrm->stBufAddr[1].u32PhyAddr_C =  u32PhyAddr + 
                                    u32Stride*pstFrm->u32Height;
    }
    
    pstFrm->u32AspectHeight = pstPort->stDispPixAR.u8ARh;
    pstFrm->u32AspectWidth = pstPort->stDispPixAR.u8ARw;
    VPSS_ALG_SetFrameInfo(pstFrmCfg,pstFrm);

    return HI_SUCCESS;
}

HI_S32 VPSS_CTRL_GetRwzbData(VPSS_ALG_RWZBCFG_S *pstRwzbCfg)
{
    HI_U32 u32Count;


    for(u32Count = 0; u32Count < 6 ; u32Count ++)
    {
       VPSS_HAL_GetDetPixel(u32Count,&(pstRwzbCfg->u8Data[u32Count][0]));
       VPSS_INFO("\n dat%d0=%d dat%d1=%d dat%d2=%d dat%d3=%d dat%d4=%d dat%d5=%d dat%d6=%d dat%d7=%d",
       u32Count,pstRwzbCfg->u8Data[u32Count][0],
       u32Count,pstRwzbCfg->u8Data[u32Count][1],
       u32Count,pstRwzbCfg->u8Data[u32Count][2],
       u32Count,pstRwzbCfg->u8Data[u32Count][3],
       u32Count,pstRwzbCfg->u8Data[u32Count][4],
       u32Count,pstRwzbCfg->u8Data[u32Count][5],
       u32Count,pstRwzbCfg->u8Data[u32Count][6],
       u32Count,pstRwzbCfg->u8Data[u32Count][7]);
                
    }
    
    return HI_SUCCESS;
}

HI_S32 VPSS_CTRL_GetDeiData(VPSS_ALG_DEICFG_S *pstDeiCfg)
{
    ALG_FMD_RTL_STATPARA_S *pstFmdRtlStatPara;

    pstFmdRtlStatPara = &(pstDeiCfg->stDeiPara.stFmdRtlStatPara);

    VPSS_HAL_GetDeiDate(pstFmdRtlStatPara);
   #if 0 
    printk("\n FMD INFO\n" 
           "\n frmITDiff %x\n"                                          
           "\n frmUm.match_UM %x\n"                                 
           "\n frmUm.match_UM2 %x\n"                                
           "\n frmUm.nonmatch_UM %x\n"                              
           "\n frmUm.nonmatch_UM2 %x\n"                             
           "\n frmPcc.PCC_FWD %x\n"                                 
           "\n frmPcc.PCC_BWD %x\n"                                 
           "\n frmPcc.PCC_CRSS %x\n"                                
           "\n frmPcc.pixel_weave %x\n"                              
           "\n frmPcc.PCC_FWD_TKR %x\n"                              
           "\n frmPcc.PCC_BWD_TKR %x\n"                              
           "\n frmPcc.PCC_FWD %x\n"                                  
           "\n frmPcc.PCCBLK_FWD[0] %x\n"                           
           "\n frmPcc.PCCBLK_FWD[1] %x\n"                          
           "\n frmPcc.PCCBLK_FWD[2] %x\n"                          
           "\n frmPcc.PCCBLK_FWD[3] %x\n"                           
           "\n frmPcc.PCCBLK_FWD[4] %x\n"                           
           "\n frmPcc.PCCBLK_FWD[5] %x\n"                           
           "\n frmPcc.PCCBLK_FWD[6] %x\n"                           
           "\n frmPcc.PCCBLK_FWD[7] %x\n"                           
           "\n frmPcc.PCCBLK_FWD[8] %x\n"                           
           "\n frmPcc.PCCBLK_BWD[0] %x\n"                           
           "\n frmPcc.PCCBLK_BWD[1] %x\n"                           
           "\n frmPcc.PCCBLK_BWD[2] %x\n"                           
           "\n frmPcc.PCCBLK_BWD[3] %x\n"                           
           "\n frmPcc.PCCBLK_BWD[4] %x\n"                           
           "\n frmPcc.PCCBLK_BWD[5] %x\n"                              
           "\n frmPcc.PCCBLK_BWD[6] %x\n"                              
           "\n frmPcc.PCCBLK_BWD[7] %x\n"                              
           "\n frmPcc.PCCBLK_BWD[8] %x\n"                              
         "\n frmHstBin.HISTOGRAM_BIN_1 %x\n"                    
         "\n frmHstBin.HISTOGRAM_BIN_2 %x\n"                    
         "\n frmHstBin.HISTOGRAM_BIN_3 %x\n"                    
         "\n frmHstBin.HISTOGRAM_BIN_4 %x\n"                    
         "\n lasiStat.lasiCnt14 %x\n"                                
         "\n lasiStat.lasiCnt32 %x\n"                                
         "\n lasiStat.lasiCnt34 %x\n"                                
         "\n StillBlkInfo.StillBlkCnt %x\n"                     
         "\n StillBlkInfo.BlkSad[0] %x\n"                          
         "\n StillBlkInfo.BlkSad[1] %x\n"                           
         "\n StillBlkInfo.BlkSad[2] %x\n"                           
         "\n StillBlkInfo.BlkSad[3] %x\n"                           
         "\n StillBlkInfo.BlkSad[4] %x\n"                           
         "\n StillBlkInfo.BlkSad[5] %x\n"                           
         "\n StillBlkInfo.BlkSad[6] %x\n"                           
         "\n StillBlkInfo.BlkSad[7] %x\n"                           
         "\n StillBlkInfo.BlkSad[8] %x\n"                           
         "\n StillBlkInfo.BlkSad[9] %x\n"                           
         "\n StillBlkInfo.BlkSad[10] %x\n"                         
            "\n StillBlkInfo.BlkSad[11] %x\n"                   
            "\n StillBlkInfo.BlkSad[12] %x\n"                   
            "\n StillBlkInfo.BlkSad[13] %x\n"                   
            "\n StillBlkInfo.BlkSad[14] %x\n"                   
            "\n StillBlkInfo.BlkSad[15] %x\n"                   
            "\n SceneChangeInfo.iCHD %x\n",                     
            pstFmdRtlStatPara->frmITDiff,                         
            pstFmdRtlStatPara->frmUm.match_UM,                    
            pstFmdRtlStatPara->frmUm.match_UM2,                   
            pstFmdRtlStatPara->frmUm.nonmatch_UM,                 
            pstFmdRtlStatPara->frmUm.nonmatch_UM2,                
            pstFmdRtlStatPara->frmPcc.PCC_FWD,                    
            pstFmdRtlStatPara->frmPcc.PCC_BWD,                    
            pstFmdRtlStatPara->frmPcc.PCC_CRSS,                   
            pstFmdRtlStatPara->frmPcc.pixel_weave,                
            pstFmdRtlStatPara->frmPcc.PCC_FWD_TKR,                
            pstFmdRtlStatPara->frmPcc.PCC_BWD_TKR,                
            pstFmdRtlStatPara->frmPcc.PCC_FWD,                    
            pstFmdRtlStatPara->frmPcc.PCCBLK_FWD[0],              
            pstFmdRtlStatPara->frmPcc.PCCBLK_FWD[1],              
            pstFmdRtlStatPara->frmPcc.PCCBLK_FWD[2],              
            pstFmdRtlStatPara->frmPcc.PCCBLK_FWD[3],              
            pstFmdRtlStatPara->frmPcc.PCCBLK_FWD[4],              
            pstFmdRtlStatPara->frmPcc.PCCBLK_FWD[5],              
            pstFmdRtlStatPara->frmPcc.PCCBLK_FWD[6],              
            pstFmdRtlStatPara->frmPcc.PCCBLK_FWD[7],              
            pstFmdRtlStatPara->frmPcc.PCCBLK_FWD[8],              
            pstFmdRtlStatPara->frmPcc.PCCBLK_BWD[0],              
            pstFmdRtlStatPara->frmPcc.PCCBLK_BWD[1],              
            pstFmdRtlStatPara->frmPcc.PCCBLK_BWD[2],              
            pstFmdRtlStatPara->frmPcc.PCCBLK_BWD[3],              
            pstFmdRtlStatPara->frmPcc.PCCBLK_BWD[4],              
            pstFmdRtlStatPara->frmPcc.PCCBLK_BWD[5],              
            pstFmdRtlStatPara->frmPcc.PCCBLK_BWD[6],              
            pstFmdRtlStatPara->frmPcc.PCCBLK_BWD[7],              
            pstFmdRtlStatPara->frmPcc.PCCBLK_BWD[8],              
            pstFmdRtlStatPara->frmHstBin.HISTOGRAM_BIN_1     ,    
            pstFmdRtlStatPara->frmHstBin.HISTOGRAM_BIN_2     ,    
            pstFmdRtlStatPara->frmHstBin.HISTOGRAM_BIN_3     ,    
            pstFmdRtlStatPara->frmHstBin.HISTOGRAM_BIN_4     ,    
            pstFmdRtlStatPara->lasiStat.lasiCnt14,                
            pstFmdRtlStatPara->lasiStat.lasiCnt32,                
            pstFmdRtlStatPara->lasiStat.lasiCnt34,                
            pstFmdRtlStatPara->StillBlkInfo.StillBlkCnt,          
            pstFmdRtlStatPara->StillBlkInfo.BlkSad[0],            
            pstFmdRtlStatPara->StillBlkInfo.BlkSad[1],            
            pstFmdRtlStatPara->StillBlkInfo.BlkSad[2],            
            pstFmdRtlStatPara->StillBlkInfo.BlkSad[3],            
            pstFmdRtlStatPara->StillBlkInfo.BlkSad[4],            
            pstFmdRtlStatPara->StillBlkInfo.BlkSad[5],            
            pstFmdRtlStatPara->StillBlkInfo.BlkSad[6],            
            pstFmdRtlStatPara->StillBlkInfo.BlkSad[7],            
            pstFmdRtlStatPara->StillBlkInfo.BlkSad[8],            
            pstFmdRtlStatPara->StillBlkInfo.BlkSad[9],            
            pstFmdRtlStatPara->StillBlkInfo.BlkSad[10],           
            pstFmdRtlStatPara->StillBlkInfo.BlkSad[11],           
            pstFmdRtlStatPara->StillBlkInfo.BlkSad[12],           
            pstFmdRtlStatPara->StillBlkInfo.BlkSad[13],           
            pstFmdRtlStatPara->StillBlkInfo.BlkSad[14],           
            pstFmdRtlStatPara->StillBlkInfo.BlkSad[15],           
            pstFmdRtlStatPara->SceneChangeInfo.iCHD);
    printk("hist1=%d,hist2=%d,hist3=%d,hist4=%d\n",pstFmdRtlStatPara->frmHstBin.HISTOGRAM_BIN_1,pstFmdRtlStatPara->frmHstBin.HISTOGRAM_BIN_2,pstFmdRtlStatPara->frmHstBin.HISTOGRAM_BIN_3,pstFmdRtlStatPara->frmHstBin.HISTOGRAM_BIN_4);
    printk("FF=%d, F=%d,B= %d,C=%d,BT=%d,FT=%d\n", pstFmdRtlStatPara->frmPcc.PCC_FFWD, pstFmdRtlStatPara->frmPcc.PCC_FWD, pstFmdRtlStatPara->frmPcc.PCC_BWD,pstFmdRtlStatPara->frmPcc.PCC_CRSS,pstFmdRtlStatPara->frmPcc.PCC_BWD_TKR,pstFmdRtlStatPara->frmPcc.PCC_FWD_TKR);
    printk("um=%d, um2=%d, nm=%d,nm2=%d,It=%d\n", pstFmdRtlStatPara->frmUm.match_UM, pstFmdRtlStatPara->frmUm.match_UM2, pstFmdRtlStatPara->frmUm.nonmatch_UM,pstFmdRtlStatPara->frmUm.nonmatch_UM2,pstFmdRtlStatPara->frmITDiff);
    printk("ls14=%d,ls32=%d,ls34=%d\n",pstFmdRtlStatPara->lasiStat.lasiCnt14,pstFmdRtlStatPara->lasiStat.lasiCnt32,pstFmdRtlStatPara->lasiStat.lasiCnt34);

       #endif

    return HI_SUCCESS;
}

HI_S32 VPSS_CTRL_ConfigOutFrame(VPSS_TASK_S *pstTask,VPSS_ALG_CFG_S *pstAlgCfg
                                        ,HI_DRV_BUF_ADDR_E eLReye)
{   
    HI_U32 u32Count;
    VPSS_INSTANCE_S *pstInst;
    VPSS_FB_NODE_S *pstFrmNode;
    VPSS_ALG_FRMCFG_S *pstFrmCfg;
    VPSS_ALG_FRMCFG_S *pstImgCfg;
    ALG_VZME_RTL_PARA_S *pstZmeRtlPara;
    HI_DRV_VIDEO_FRAME_S *pstImg;
    VPSS_ALG_PortCFG_S *pstAuPortCfg;
    HI_VDEC_PRIV_FRAMEINFO_S *pstVdecPriv;
    HI_DRV_VIDEO_PRIVATE_S *pstFrmPriv;
	HI_S32 s32Ret;
    /*
     两步处理:
     1.配置算法
     2.配置输出图像信息
     */

    pstImgCfg = &(pstAlgCfg->stSrcImgInfo);
    pstInst = pstTask->pstInstance;
    
    pstImg = VPSS_INST_GetUndoImage(pstInst);
    
    if (pstImg == HI_NULL)
    {
        VPSS_FATAL("\n ConfigOutFrame there is no src image \n");
        return HI_FAILURE;
    }
    
    /*1.配置算法*/
    /*获取dei的配置*/
    if(pstImgCfg->bProgressive == HI_TRUE)
    {
        pstAlgCfg->stAuTunnelCfg.stDeiCfg.bDei = HI_FALSE;
    }
    else
    {
        HI_DRV_VPSS_DIE_MODE_E eDeiMode;
        VPSS_ALG_DEICFG_S *pstDeiCfg;
            
        ALG_DEI_DRV_PARA_S *pstDeiPara;
        pstDeiCfg = &(pstAlgCfg->stAuTunnelCfg.stDeiCfg);
        pstDeiPara = &(pstDeiCfg->stDeiPara);
        eDeiMode = pstInst->stProcCtrl.eDEI;
        
        /*
          获取逻辑统计的直方图信息
          */
        VPSS_CTRL_GetDeiData(pstDeiCfg);
        
        /*检测DEI是否需要重置*/
        if(pstInst->u32NeedRstDei == HI_TRUE)
        {
            pstDeiPara->bDeiRst = HI_TRUE;
            pstInst->u32NeedRstDei = HI_FALSE;
            
        }
        else
        {
            pstDeiPara->bDeiRst = HI_FALSE;
        }
        pstDeiPara->s32FrmHeight = pstImgCfg->u32Height;
        pstDeiPara->s32FrmWidth = pstImgCfg->u32Width;
        pstDeiPara->s32Drop = 0;
        pstDeiPara->BtMode = !pstImgCfg->bTopFieldFirst;

        pstDeiPara->stVdecInfo.IsProgressiveFrm = 0;
        pstDeiPara->stVdecInfo.IsProgressiveSeq = 0;
        pstDeiPara->stVdecInfo.RealFrmRate = 2500;
        pstDeiPara->bOfflineMode = 1;
        if(pstImgCfg->enFieldMode == HI_DRV_FIELD_TOP)
        {
           pstDeiPara->RefFld = 0;
        }
        else if(pstImgCfg->enFieldMode == HI_DRV_FIELD_BOTTOM)
        {
            pstDeiPara->RefFld = 1;
        }
        else
        {
            VPSS_FATAL("\nDEI ERROR\n");
        }

        if(pstInst->u32IsNewImage == HI_FALSE)
        {
          pstDeiPara->bPreInfo = HI_TRUE;
        }
        else
        {
          pstDeiPara->bPreInfo = HI_FALSE;
        }
        
        VPSS_ALG_GetDeiCfg(eDeiMode,(HI_U32)&(pstInst->stAuInfo[0]),pstDeiCfg,
                                 &(stVpssCtrl.stAlgCtrl));
        /*
        printk("\nCurID %d CurBT %d DirMch %d\n",
                    pstImg->u32FrameIndex,pstImg->enFieldMode,
                    pstDeiCfg->stDeiOutPara.stFmdRtlOutPara.DirMch);
            */
        VPSS_INST_GetDeiAddr(pstInst,&(pstDeiCfg->u32FieldAddr[0]),
                                eDeiMode,pstImgCfg->eLReye);
        /*
        printk("\n FieldAddr[0].u32PhyAddr_Y %x FieldAddr[0].u32PhyAddr_C %x \n"
               "\n FieldAddr[3].u32PhyAddr_Y %x FieldAddr[3].u32PhyAddr_C %x \n"
               "\n FieldAddr[4].u32PhyAddr_Y %x FieldAddr[4].u32PhyAddr_C %x \n"
               "\n FieldAddr[5].u32PhyAddr_Y %x FieldAddr[5].u32PhyAddr_C %x \n",
                        pstDeiCfg->u32FieldAddr[0].u32PhyAddr_Y,
                        pstDeiCfg->u32FieldAddr[0].u32PhyAddr_C,
                        pstDeiCfg->u32FieldAddr[3].u32PhyAddr_Y,
                        pstDeiCfg->u32FieldAddr[3].u32PhyAddr_C,
                        pstDeiCfg->u32FieldAddr[4].u32PhyAddr_Y,
                        pstDeiCfg->u32FieldAddr[4].u32PhyAddr_C,
                        pstDeiCfg->u32FieldAddr[5].u32PhyAddr_Y,
                        pstDeiCfg->u32FieldAddr[5].u32PhyAddr_C);     
        */
     //   printk("\nIndex=%d TB=%d bTFirst=%d\n",pstImg->u32FrameIndex,pstImgCfg->enFieldMode,pstImgCfg->bTopFieldFirst);
    
        /*调用场序调整函数*/
        #if 1
        if(pstInst->u32RealTopFirst == HI_FALSE)
        {
            if(pstImg->enFieldMode == HI_DRV_FIELD_TOP)
            {
                VPSS_INST_CorrectImgListOrder(pstInst,!pstDeiCfg->stDeiOutPara.stFmdRtlOutPara.s32FieldOrder);
            }
        }
        else if(pstInst->u32RealTopFirst == HI_TRUE)
        {
            if(pstImg->enFieldMode == HI_DRV_FIELD_BOTTOM)
            {
                VPSS_INST_CorrectImgListOrder(pstInst,!pstDeiCfg->stDeiOutPara.stFmdRtlOutPara.s32FieldOrder);
            }
        }
        else
        {

        }
        #endif
        //VPSS_INST_CorrectImgListOrder(pstInst,!pstDeiCfg->stDeiOutPara.stFmdRtlOutPara.s32FieldOrder);
        #if 0
        /*for test*/
        if(pstImg->u32FrameIndex % 100 == 0)
        {
            if(pstInst->u32RealTopFirst == HI_FALSE)
            {
                if(pstImg->enFieldMode == HI_DRV_FIELD_TOP)
                {
                    VPSS_INST_CorrectImgListOrder(pstInst,HI_TRUE);
                }
            }
            else
            {
                if(pstImg->enFieldMode == HI_DRV_FIELD_BOTTOM)
                {
                    VPSS_INST_CorrectImgListOrder(pstInst,HI_FALSE);
                }
            }
        }
        /*********/
        #endif
    }

    #if 1
    /*获取DB/DR配置*/
    if(pstImgCfg->u32Height <= 576 && pstImgCfg->u32Width <= 720)
    {
        ALG_DNR_CTRL_PARA_S stDnrPara;
        HI_DRV_VIDEO_PRIVATE_S *pstFrmPriv;
        HI_VDEC_PRIV_FRAMEINFO_S *pstVdecPriv;
        pstFrmPriv = (HI_DRV_VIDEO_PRIVATE_S *)&(pstImg->u32Priv[0]);
        pstVdecPriv = (HI_VDEC_PRIV_FRAMEINFO_S *)&(pstFrmPriv->u32Reserve[0]);
        
        if (pstInst->stProcCtrl.bUseCropRect == HI_TRUE 
            || ((pstInst->stProcCtrl.bUseCropRect == HI_FALSE)
                && (pstInst->stProcCtrl.stInRect.s32Height != 0 
                    || pstInst->stProcCtrl.stInRect.s32Width != 0
                    || pstInst->stProcCtrl.stInRect.s32X != 0
                    || pstInst->stProcCtrl.stInRect.s32Y != 0) )
            || pstVdecPriv->stBTLInfo.u32DNROpen == HI_FALSE)
        {
            stDnrPara.dbEnHort = 0;
            stDnrPara.dbEnVert = 0;
            stDnrPara.drEn = 0;
            stDnrPara.dbEn = 0;
        }
        else
        {
            stDnrPara.dbEnHort = 1;
            stDnrPara.dbEnVert = 1;
            stDnrPara.drEn = 1;
            stDnrPara.dbEn = 1;
            
            stDnrPara.u32YInfoAddr = pstVdecPriv->stBTLInfo.u32DNRInfoAddr;
            stDnrPara.u32CInfoAddr = stDnrPara.u32YInfoAddr
                                    + pstImg->u32Height/8*pstVdecPriv->stBTLInfo.u32DNRInfoStride;
            stDnrPara.u32YInfoStride = pstVdecPriv->stBTLInfo.u32DNRInfoStride;
            stDnrPara.u32CInfoStride = pstVdecPriv->stBTLInfo.u32DNRInfoStride;
        }
        ALG_DnrInit(&stDnrPara,&(pstAlgCfg->stAuTunnelCfg.stDnrCfg));
    }
    else
    #endif 
    #if 1
    {
        ALG_DNR_CTRL_PARA_S stDnrPara;
        stDnrPara.dbEnHort = 0;
        stDnrPara.dbEnVert = 0;
        stDnrPara.drEn = 0;
        stDnrPara.dbEn = 0;
        ALG_DnrInit(&stDnrPara,&(pstAlgCfg->stAuTunnelCfg.stDnrCfg));
    }
    #endif 

    #if 1
    /*获取VC1配置*/
    pstFrmPriv = (HI_DRV_VIDEO_PRIVATE_S *)&(pstImg->u32Priv[0]);
    pstVdecPriv = (HI_VDEC_PRIV_FRAMEINFO_S *)&(pstFrmPriv->u32Reserve[0]);
    
    if(pstVdecPriv->u32BeVC1 == HI_TRUE)
    {
        VPSS_ALG_VC1CFG_S *pstVc1Cfg;
        pstVc1Cfg = &(pstAlgCfg->stAuTunnelCfg.stVC1Cfg);
        pstVc1Cfg->u32EnVc1 = HI_TRUE;
        if(pstAlgCfg->stAuTunnelCfg.stDeiCfg.bDei == HI_FALSE)
        {
            VPSS_INST_GetVc1Info(pstInst,&(pstVc1Cfg->stVc1Info[0]),HI_DRV_VPSS_DIE_DISABLE);
        }
        else
        {
            VPSS_INST_GetVc1Info(pstInst,&(pstVc1Cfg->stVc1Info[0]),pstInst->stProcCtrl.eDEI);
        }
    }
    else
    {
        VPSS_ALG_VC1CFG_S *pstVc1Cfg;
        pstVc1Cfg = &(pstAlgCfg->stAuTunnelCfg.stVC1Cfg);
        pstVc1Cfg->u32EnVc1 = HI_FALSE;
    }
    #endif 

    #if 1
    /*获取rwzb的配置*/
    if(pstImgCfg->enFieldMode == HI_DRV_FIELD_BOTTOM
       || pstImgCfg->bProgressive == HI_TRUE)
    {
        VPSS_CTRL_GetRwzbData(&(pstAlgCfg->stAuTunnelCfg.stRwzbCfg));
    }

    VPSS_ALG_GetRwzbCfg((HI_U32)&(pstInst->stAuInfo[0]),&(pstAlgCfg->stAuTunnelCfg.stRwzbCfg),
                                pstImgCfg);
    pstInst->u32Rwzb =VPSS_ALG_GetRwzbInfo((HI_U32)&(pstInst->stAuInfo[0]));
    #endif
    
    for(u32Count = 0; u32Count < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER; u32Count++)
    {
        if(eLReye == HI_DRV_BUF_ADDR_LEFT)
        {
            pstFrmNode = pstTask->pstFrmNode[u32Count*2];
        }
        else
        {
            pstFrmNode = pstTask->pstFrmNode[u32Count*2+1];
        }
        
        pstAuPortCfg = &(pstAlgCfg->stAuPortCfg[u32Count]);
        pstFrmCfg = &(pstAuPortCfg->stDstFrmInfo);
        if(pstFrmNode != HI_NULL)
        {   
        
            VPSS_PORT_S *pstPort;
            HI_BOOL bRWZB;

            
            pstFrmCfg->eLReye = eLReye;
            
            pstPort = &(pstInst->stPort[u32Count]);

            /*
                    下发寄存器的逻辑PORT，获取算法时先归0，
                    在HAL配置时，根据逻辑各通道能力，获取相应PORT
               */
            pstAuPortCfg->eRegPort = VPSS_REG_BUTT;
            bRWZB = (pstInst->u32Rwzb > 0)?HI_TRUE:HI_FALSE;

            pstAuPortCfg->bFidelity = VPSS_CTRL_CheckRWZB(pstImg,pstPort,bRWZB);
            
            s32Ret = VPSS_CTRL_GetDstFrmCfg(pstFrmCfg,pstFrmNode,
                                    pstPort,pstImg,pstAuPortCfg->bFidelity);

            /*根据算法检测，修正输出frame的TopFirst信息*/
            pstFrmNode->stOutFrame.bTopFieldFirst = pstInst->u32RealTopFirst;
            
            /*获取UV反转配置*/
            VPSS_CTRL_GetUVCfg(pstFrmCfg,
                                &(pstAlgCfg->stAuTunnelCfg.u32EnUVCovert),
                                pstImg);
			#if 0
			if (pstPort->s32PortId == 0x0)
			{
                pstPort->eAspMode = HI_DRV_ASP_RAT_MODE_BUTT;
			}
			else
			{
                
			}
			#endif
			#if 1
            /*获取port上的LBOX配置*/
            if(pstPort->eAspMode == HI_DRV_ASP_RAT_MODE_LETTERBOX)
            {   
                HI_RECT_S stScreen;
                HI_RECT_S stOutWnd;
                
                ALG_RATIO_DRV_PARA_S stAspDrvPara;
                
                stScreen.s32Height = pstPort->stScreen.s32Height;
                stScreen.s32Width = pstPort->stScreen.s32Width;
                stScreen.s32X = pstPort->stScreen.s32X;
                stScreen.s32Y = pstPort->stScreen.s32Y;

                stOutWnd.s32Height = pstPort->s32OutputHeight;
                stOutWnd.s32Width = pstPort->s32OutputWidth;
                stOutWnd.s32X = 0;
                stOutWnd.s32Y = 0;
                
                stAspDrvPara.AspectHeight = pstImgCfg->stDispPixAR.u8ARh;
                stAspDrvPara.AspectWidth  = pstImgCfg->stDispPixAR.u8ARw;
                    
                #if 0
                if (pstPort->s32PortId == 0x0)
                {
                    stAspDrvPara.AspectHeight = 9;
                    stAspDrvPara.AspectWidth  = 16;
                }
                else
                {
                    stAspDrvPara.AspectHeight = 3;
                    stAspDrvPara.AspectWidth  = 4;
                }
                #endif
                #if 0
                if (pstPort->s32PortId == 0x0)
                {
                     stAspDrvPara.DeviceHeight = 9;
                     stAspDrvPara.DeviceWidth  = 16;
                }
                else
                {
                     stAspDrvPara.DeviceHeight = 9;
                    stAspDrvPara.DeviceWidth  = 16;
                }
                #endif
                #if 1
                stAspDrvPara.DeviceHeight = pstPort->stDispPixAR.u8ARh;
                stAspDrvPara.DeviceWidth  = pstPort->stDispPixAR.u8ARw;
                
                #endif
                
                stAspDrvPara.eAspMode = pstPort->eAspMode;
                
                stAspDrvPara.stInWnd.s32X = 0;
                stAspDrvPara.stInWnd.s32Y = 0;

                if (pstFrmNode->stOutFrame.eFrmType == HI_DRV_FT_NOT_STEREO
                    || pstFrmNode->stOutFrame.eFrmType == HI_DRV_FT_FPK)
                {
                    stAspDrvPara.stInWnd.s32Height = pstImgCfg->u32Height;
                    stAspDrvPara.stInWnd.s32Width = pstImgCfg->u32Width;
                }
                else if (pstFrmNode->stOutFrame.eFrmType == HI_DRV_FT_SBS)
                {
                    stAspDrvPara.stInWnd.s32Height = pstImgCfg->u32Height;
                    stAspDrvPara.stInWnd.s32Width = pstImgCfg->u32Width * 2;
                }
                else
                {
                    stAspDrvPara.stInWnd.s32Height = pstImgCfg->u32Height * 2;
                    stAspDrvPara.stInWnd.s32Width = pstImgCfg->u32Width;
                }
                
                stAspDrvPara.stOutWnd.s32X = 0;
                stAspDrvPara.stOutWnd.s32Y = 0;
                stAspDrvPara.stOutWnd.s32Height = stOutWnd.s32Height;
                stAspDrvPara.stOutWnd.s32Width = stOutWnd.s32Width;
                
                stAspDrvPara.stScreen.s32X = stScreen.s32X;
                stAspDrvPara.stScreen.s32Y = stScreen.s32Y;
                stAspDrvPara.stScreen.s32Height = stScreen.s32Height;
                stAspDrvPara.stScreen.s32Width = stScreen.s32Width;
               

               
                if (pstPort->stCustmAR.u8ARh != 0 
                    && pstPort->stCustmAR.u8ARw != 0)
                {
                    stAspDrvPara.stUsrAsp.bUserDefAspectRatio = HI_TRUE;
                }
                else
                {
                    stAspDrvPara.stUsrAsp.bUserDefAspectRatio = HI_FALSE;
                }
                
                stAspDrvPara.stUsrAsp.u32UserAspectHeight = pstPort->stCustmAR.u8ARh;
                stAspDrvPara.stUsrAsp.u32UserAspectWidth = pstPort->stCustmAR.u8ARw;

                pstAuPortCfg->stAspCfg.bEnAsp = HI_TRUE;
                pstAuPortCfg->stAspCfg.u32BgColor = 0x108080;
                pstAuPortCfg->stAspCfg.u32BgAlpha = 0x7f;
                VPSS_ALG_GetAspCfg(&stAspDrvPara,
                            pstPort->eAspMode,&stScreen,
                            &(pstAuPortCfg->stAspCfg));
                
                /*经过ASP计算后，修正缩放输出宽高*/
                pstFrmCfg->u32ZmeHeight = pstAuPortCfg->stAspCfg.stOutWnd.s32Height;
                pstFrmCfg->u32ZmeWidth  = pstAuPortCfg->stAspCfg.stOutWnd.s32Width;
                
            }
            else
            {   
                pstAuPortCfg->stAspCfg.bEnAsp = HI_FALSE;

                /*未经过ASP计算，缩放输出宽高为输出frame宽高*/
                pstFrmCfg->u32ZmeHeight = pstFrmCfg->u32Height;
                pstFrmCfg->u32ZmeWidth  = pstFrmCfg->u32Width;
            }   
            /*获取port上的ZME配置*/
            #endif

            #if 0
            pstFrmCfg->u32ZmeHeight = pstFrmCfg->u32Height;
            pstFrmCfg->u32ZmeWidth  = pstFrmCfg->u32Width;
            #endif
            
            pstZmeRtlPara  = &(pstAuPortCfg->stZmeCfg);

            VPSS_ALG_GetZmeCfg(pstImgCfg,pstFrmCfg,
                                pstZmeRtlPara,&(stVpssCtrl.stAlgCtrl));

            #if 1
            /*获取port上的SHARP配置*/
            {
                ALG_VTI_DRV_PARA_S stSharpCfg;
                stSharpCfg.bDeiEnLum = HI_TRUE;
                stSharpCfg.bEnLTI = HI_FALSE;//HI_TRUE;
                stSharpCfg.bEnCTI = HI_FALSE;//HI_TRUE;
                stSharpCfg.RwzbFlag = HI_FALSE;
                
                stSharpCfg.bZmeFrmFmtIn = 1;
                stSharpCfg.bZmeFrmFmtOut = 1;
                
                stSharpCfg.u32ZmeWOut = pstFrmCfg->u32Width;
                stSharpCfg.u32ZmeHOut = pstFrmCfg->u32Height;
                stSharpCfg.u32ZmeWIn = pstImgCfg->u32Width;
                stSharpCfg.u32ZmeHIn = pstImgCfg->u32Height;
                stSharpCfg.s16LTICTIStrengthRatio = 15;

                ALG_VtiInit(&stSharpCfg,&(pstAuPortCfg->stSharpCfg));
                ALG_VtiSet(&stSharpCfg,&(pstAuPortCfg->stSharpCfg));
            }
            #endif
            /*获取port上的CSC配置*/
            /*
                ********************
               */
            /*获取port上的CROP LBOX配置*/
            /*
                ********************
               */
        }
        else
        {   
            memset(pstFrmCfg,0,sizeof(VPSS_ALG_FRMCFG_S));
        }
        
    }
    
    return HI_SUCCESS;
}

VPSS_HANDLE VPSS_CTRL_AddInstance(VPSS_INSTANCE_S *pstInstance)
{
    VPSS_INST_CTRL_S *pstInstCtrlInfo;
    HI_U32 u32Count;
    
    
    pstInstCtrlInfo = &(stVpssCtrl.stInstCtrlInfo);

    VPSS_OSAL_DownLock(&(pstInstCtrlInfo->stListLock));
    
    for(u32Count = 0; u32Count < VPSS_INSTANCE_MAX_NUMB; u32Count++)
    {
        if (pstInstCtrlInfo->pstInstPool[u32Count] == HI_NULL)
        {
            pstInstCtrlInfo->pstInstPool[u32Count] = pstInstance;
            
        
            break;
        }
    }
    VPSS_OSAL_UpLock(&(pstInstCtrlInfo->stListLock));
    if (u32Count == VPSS_INSTANCE_MAX_NUMB)
    {
        VPSS_FATAL("\nInstance Number is Max");

        return VPSS_INVALID_HANDLE;
    }
    else
    {   
        pstInstance->ID = u32Count;

        return pstInstance->ID;
    }  
}

HI_S32 VPSS_CTRL_DelInstance(VPSS_HANDLE hVPSS)
{
    VPSS_INST_CTRL_S *pstInstCtrlInfo;

    pstInstCtrlInfo = &(stVpssCtrl.stInstCtrlInfo);

    
    VPSS_OSAL_DownLock(&(pstInstCtrlInfo->stListLock));
    pstInstCtrlInfo->pstInstPool[hVPSS] = HI_NULL;
    VPSS_OSAL_UpLock(&(pstInstCtrlInfo->stListLock));
    
    return HI_SUCCESS;
}

HI_S32 VPSS_CTRL_IntServe_Proc(HI_VOID)
{
    /*
    1.关中断
    2.报事件
    3.开中断
    */
    HI_U32 u32State;
    //HI_U32 u32Cnt;
    VPSS_HAL_CAP_S *pstHal;
    
    pstHal = &(stVpssCtrl.stHalCaps);

    pstHal->PFN_VPSS_HAL_GetIntState(&u32State);

    
    u32IRQCnt++;
    #if 0
    if(u32THRCnt - u32IRQCnt != 0   )
    {
        printk("\nError  IRQCnt %d THRCnt %d\n",u32IRQCnt,u32THRCnt);
    }
    #endif
    
    if((u32State & 0x8))
    {
        /*TEST*/
        #if 0
        VPSS_HAL_GetCycleCnt(&u32Cnt);
        printk("\n Cnt Done %x \n",u32Cnt);
        #endif
        
        pstHal->PFN_VPSS_HAL_ClearIntState(0xf);
        pstHal->PFN_VPSS_HAL_SetIntMask(0x0);
        VPSS_OSAL_GiveEvent(&(stVpssCtrl.stTaskComplete),EVENT_DONE,EVENT_UNDO);
        
    }
    else if((u32State & 0x1))
    {
        /*TEST*/
        #if 0
        VPSS_HAL_GetCycleCnt(&u32Cnt);
        printk("\n Cnt First %x \n",u32Cnt);
        #endif
        
        pstHal->PFN_VPSS_HAL_ClearIntState(0xf);
    }
    else if((u32State & 0x6))
    {
        VPSS_FATAL("\n Logic Time Out\n");
        pstHal->PFN_VPSS_HAL_ClearIntState(0xf);
        pstHal->PFN_VPSS_HAL_SetIntMask(0x0);
        VPSS_OSAL_GiveEvent(&(stVpssCtrl.stTaskComplete),EVENT_UNDO,EVENT_DONE);
    }
    else
    {
        VPSS_FATAL("\nIRQ Error\n");
    }

    
    u32IRQCnt++;
    return IRQ_HANDLED;
}




HI_S32 VPSS_CTRL_ProcRead(struct seq_file *p, HI_VOID *v)
{
    VPSS_INSTANCE_S* pstInstance;
    DRV_PROC_ITEM_S *pProcItem;
    VPSS_IMAGELIST_STATE_S stImgListState;
    VPSS_PORT_PRC_S *pstPortPrc[DEF_HI_DRV_VPSS_PORT_MAX_NUMBER];
    VPSS_FB_STATE_S *pstFbPrc[DEF_HI_DRV_VPSS_PORT_MAX_NUMBER];
    VPSS_PORT_S *pstPort;
    HI_U32 u32Count;
    pProcItem = p->private;

             
    pstInstance = VPSS_CTRL_GetInstance((VPSS_HANDLE)pProcItem->data);

    if(!pstInstance)
    {
        VPSS_FATAL("Can't get instance %x proc!\n",(VPSS_HANDLE)pProcItem->data);
        return HI_FAILURE;
    }
    for(u32Count = 0; u32Count < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER; u32Count++)
    {
        pstPortPrc[u32Count] = VPSS_VMALLOC(sizeof(VPSS_PORT_PRC_S));
        if (pstPortPrc[u32Count] == HI_NULL)
        {
            VPSS_FATAL("Vmalloc Proc space Failed.\n");
            
            goto READFREE;
        }
        memset(pstPortPrc[u32Count],0,sizeof(VPSS_PORT_PRC_S));
        
    }
    
    #if 1
    for(u32Count = 0; u32Count < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER; u32Count++)
    {
        pstPort = &(pstInstance->stPort[u32Count]);
        VPSS_INST_GetPortPrc(pstInstance,pstPort->s32PortId,pstPortPrc[u32Count]);
    }    
    
    VPSS_INST_GetSrcListState(pstInstance, &stImgListState);    
    #endif
    #if 1
    p += seq_printf(p,
        "--------VPSS%04x---------------|"   "--------Port%08x Info-------|"	        "--------Port%08x Info-------|"         "--------Port%08x Info-------|\n"      
        "ID                  :0x%-8x|"       "eFormat                   :%-5d|"         "Format                    :%-5d|"      "eFormat                   :%-5d|\n"      
        "Priority            :%-10d|"        "s32OutputWidth            :%-5d|"         "s32OutputWidth            :%-5d|"      "s32OutputWidth            :%-5d|\n"      
        "bAlwaysFlushSrc     :%-10d|"        "s32OutputHeight           :%-5d|"         "s32OutputHeight           :%-5d|"      "s32OutputHeight           :%-5d|\n"      
        "hDst                :0x%-8x|"       "eDstCS                    :%-5d|"         "eDstCS                    :%-5d|"      "eDstCS                    :%-5d|\n"      
        "PortState           :%-8x:%-1d|"    "stDispPixAR(H/W)          :%-2d/%-2d|"    "stDispPixAR(H/W)          :%-2d/%-2d|" "stDispPixAR(H/W)          :%-2d/%-2d|\n"   
        "                     %-8x:%-1d|"    "eAspMode                  :%-5d|"         "eAspMode                  :%-5d|"      "eAspMode                  :%-5d|\n"      
        "                     %-8x:%-1d|"    "u32MaxFrameRate           :%-5d|"         "u32MaxFrameRate           :%-5d|"      "u32MaxFrameRate           :%-5d|\n"      
        "-------- AlgConfig-------------|"   "bTunnelEnable             :%-5d|"         "bTunnelEnable             :%-5d|"      "bTunnelEnable             :%-5d|\n" 
        "HFlip               :%-10d|"        "s32SafeThr                :%-5d|"         "s32SafeThr                :%-5d|"      "s32SafeThr                :%-5d|\n"      
        "VFlip               :%-10d|"        "eCSC                      :%-5d|"         "eCSC                      :%-5d|"      "eCSC                      :%-5d|\n"      
        "Stereo              :%-10d|"        "eFidelity                 :%-5d|"         "eFidelity                 :%-5d|"      "eFidelity                 :%-5d|\n"      
        "Rotation            :%-10d|"        "                                |"        "                                |"     "                                |\n"
        "DEI                 :%-10d|"        "                                |"        "                                |"     "                                |\n"
        "ACC                 :%-10d|"        "                                |"        "                                |"     "                                |\n"
        "ACM                 :%-10d|"        "                                |"        "                                |"     "                                |\n"
        "CC                  :%-10d|"        "                                |"        "                                |"     "                                |\n"
        "Sharpness           :%-10d|"        "                                |"        "                                |"     "                                |\n"       
        "DB                  :%-10d|"        "                                |"        "                                |"     "                                |\n"
        "DR                  :%-10d|"        "                                |"        "                                |"     "                                |\n"
        "--------Detect Info------------|"   "                                |"        "                                |"     "                                |\n"
        "RWZB                :%-10d|"        "                                |"        "                                |"     "                                |\n"
        "RealTopFirst        :%-10d|"        "                                |"        "                                |"     "                                |\n"
        "InRate              :%-10d|"        "                                |"        "                                |"     "                                |\n",
        /* attribute */
        pstInstance->ID,                        pstPortPrc[0]->s32PortId,			      pstPortPrc[1]->s32PortId,            		pstPortPrc[2]->s32PortId,                        
        pstInstance->ID,                        pstPortPrc[0]->eFormat,                   pstPortPrc[1]->eFormat,                   pstPortPrc[2]->eFormat,                          
        pstInstance->s32Priority,               pstPortPrc[0]->s32OutputWidth,            pstPortPrc[1]->s32OutputWidth,                    pstPortPrc[2]->s32OutputWidth,                   
        pstInstance->bAlwaysFlushSrc,           pstPortPrc[0]->s32OutputHeight,           pstPortPrc[1]->s32OutputHeight,                   pstPortPrc[2]->s32OutputHeight,                  
        pstInstance->hDst,                      pstPortPrc[0]->eDstCS,                    pstPortPrc[1]->eDstCS,                            pstPortPrc[2]->eDstCS,                           
        pstInstance->stPort[0].s32PortId,                                                                                                                                              
            pstInstance->stPort[0].bEnble,      pstPortPrc[0]->stDispPixAR.u8ARh,                                                                                                        
            					                    pstPortPrc[0]->stDispPixAR.u8ARw,       pstPortPrc[1]->stDispPixAR.u8ARh,                 
            										                                            pstPortPrc[1]->stDispPixAR.u8ARw, 		pstPortPrc[2]->stDispPixAR.u8ARh,        	   
                                                                                                                                       	  pstPortPrc[2]->stDispPixAR.u8ARw,
                                                                                                                                                                                       
        pstInstance->stPort[1].s32PortId,                                                                                                                                              
            pstInstance->stPort[1].bEnble,      pstPortPrc[0]->eAspMode,                  pstPortPrc[1]->eAspMode,                          pstPortPrc[2]->eAspMode,                         
        pstInstance->stPort[2].s32PortId,                                                                                                                                              
            pstInstance->stPort[2].bEnble,       pstPortPrc[0]->u32MaxFrameRate,           pstPortPrc[1]->u32MaxFrameRate,                   pstPortPrc[2]->u32MaxFrameRate,                  
        /*alg config*/                              pstPortPrc[0]->bTunnelEnable,            pstPortPrc[1]->bTunnelEnable,                      pstPortPrc[2]->bTunnelEnable,                                                                                                                         
        pstInstance->stProcCtrl.eHFlip,          pstPortPrc[0]->s32SafeThr,               pstPortPrc[1]->s32SafeThr,                        pstPortPrc[2]->s32SafeThr,                                                          
        pstInstance->stProcCtrl.eVFlip,          pstPortPrc[0]->stProcCtrl.eCSC,           pstPortPrc[1]->stProcCtrl.eCSC,                   pstPortPrc[2]->stProcCtrl.eCSC,                                 
        pstInstance->stProcCtrl.eStereo,        pstPortPrc[0]->stProcCtrl.eFidelity,      pstPortPrc[1]->stProcCtrl.eFidelity,              pstPortPrc[2]->stProcCtrl.eFidelity,             
        pstInstance->stProcCtrl.eRotation,      
        pstInstance->stProcCtrl.eDEI,
        pstInstance->stProcCtrl.eACC,
        pstInstance->stProcCtrl.eACM,
        pstInstance->stProcCtrl.eCC,
        pstInstance->stProcCtrl.eSharpness,
        pstInstance->stProcCtrl.eDB,
        pstInstance->stProcCtrl.eDR,
        pstInstance->u32Rwzb,
        pstInstance->u32RealTopFirst,
        pstInstance->u32InRate
        );
    #endif
    #if 1
    p += seq_printf(p,
    "-----Source ImageList Info-----|"  "--------OutFrameList Info-------|"     "--------OutFrameList Info-------|"     "--------OutFrameList Info-------|\n"  
    "SrcImgMode          :%-10d|"	    "eBufType             :%-10d|" 		    "eBufType             :%-10d|"     	    "eBufType             :%-10d|\n"    	
    "BufNumber           :%-10d|"       "u32BufNumber         :%-10d|"          "u32BufNumber         :%-10d|"          "u32BufNumber         :%-10d|\n"        
    "Get(Total)          :%-10d|"       "u32BufSize           :0x%-8x|"         "u32BufSize           :0x%-8x|"         "u32BufSize           :0x%-8x|\n"       
    "Rel(Total)          :%-10d|"       "u32BufStride         :0x%-8x|"         "u32BufStride         :0x%-8x|"         "u32BufStride         :0x%-8x|\n"       
    "Get(Fail)           :%-10d|"       "Get(Total)           :%-10d|"          "Get(Total)           :%-10d|"          "Get(Total)           :%-10d|\n"  
    "Rel(Fail)           :%-10d|"       "Rel(Total)           :%-10d|"      	"Rel(Total)           :%-10d|"          "Rel(Total)           :%-10d|\n"  
    "                               |"  "Get(Fail)            :%-10d|"   	    "Get(Fail)            :%-10d|"          "Get(Fail)            :%-10d|\n"  
    "                               |"  "Rel(Fail)            :%-10d|"   	    "Rel(Fail)            :%-10d|"          "Rel(Fail)            :%-10d|\n"
    "                               |"  "GetHZ                :%-10d|"    	    "GetHZ                :%-10d|"          "GetHZ                :%-10d|\n" , 
                 		           	                
                                                                                                                                                                    
                                                                                                                                                                    
    pstInstance->eSrcImgMode,           (HI_U32)pstPortPrc[0]->stBufListCfg.eBufType,        (HI_U32)pstPortPrc[1]->stBufListCfg.eBufType,      (HI_U32)pstPortPrc[2]->stBufListCfg.eBufType,    
    stImgListState.u32TotalNumb,        pstPortPrc[0]->stBufListCfg.u32BufNumber,    	pstPortPrc[1]->stBufListCfg.u32BufNumber,                 pstPortPrc[2]->stBufListCfg.u32BufNumber,
    stImgListState.u32GetUsrTotal,     pstPortPrc[0]->stBufListCfg.u32BufSize,      	pstPortPrc[1]->stBufListCfg.u32BufSize,                   pstPortPrc[2]->stBufListCfg.u32BufSize,  
    stImgListState.u32RelUsrTotal,   	pstPortPrc[0]->stBufListCfg.u32BufStride,    	pstPortPrc[1]->stBufListCfg.u32BufStride,                 pstPortPrc[2]->stBufListCfg.u32BufStride,
    stImgListState.u32GetUsrFailed,      pstPortPrc[0]->stFbPrc.u32GetTotal,  	        pstPortPrc[1]->stFbPrc.u32GetTotal,	                    pstPortPrc[2]->stFbPrc.u32GetTotal,     	
    stImgListState.u32RelUsrFailed,      pstPortPrc[0]->stFbPrc.u32RelTotal,             pstPortPrc[1]->stFbPrc.u32RelTotal,                       pstPortPrc[2]->stFbPrc.u32RelTotal,     
        				                pstPortPrc[0]->stFbPrc.u32GetFail,              pstPortPrc[1]->stFbPrc.u32GetFail,                        pstPortPrc[2]->stFbPrc.u32GetFail,         
        			                    pstPortPrc[0]->stFbPrc.u32RelFail,    	        pstPortPrc[1]->stFbPrc.u32RelFail,                        pstPortPrc[2]->stFbPrc.u32RelFail,         
                                        pstPortPrc[0]->stFbPrc.u32GetHZ,     	        pstPortPrc[1]->stFbPrc.u32GetHZ,                          pstPortPrc[2]->stFbPrc.u32GetHZ            
                                        					 	          				
    ); 
    #endif
    pstFbPrc[0] = &(pstPortPrc[0]->stFbPrc);
    pstFbPrc[1] = &(pstPortPrc[1]->stFbPrc);
    pstFbPrc[2] = &(pstPortPrc[2]->stFbPrc);
    p += seq_printf(p,
"List state:(idx)(state)        |"          "List state:(idx)(state)         |"              "List state:(idx)(state)         |"              "List state:(idx)(state)         |\n"
"0 empty 1 done 2 willdo        |"          "0 empty 1 Acqed 2 willAcq 3 NULL|"              "0 empty 1 Acqed 2 willAcq 3 NULL|"              "0 empty 1 Acqed 2 willAcq 3 NULL|\n"
"%010d(%01d)                  |"            "%010d(%01d)    %010d(%01d)  |"                  "%010d(%01d)    %010d(%01d)  |"                  "%010d(%01d)    %010d(%01d)  |\n"    
"%010d(%01d)                  |"            "%010d(%01d)    %010d(%01d)  |"                  "%010d(%01d)    %010d(%01d)  |"                  "%010d(%01d)    %010d(%01d)  |\n"    
"%010d(%01d)                  |"            "%010d(%01d)    %010d(%01d)  |"                  "%010d(%01d)    %010d(%01d)  |"                  "%010d(%01d)    %010d(%01d)  |\n"    
"%010d(%01d)                  |"            "%010d(%01d)    %010d(%01d)  |"                  "%010d(%01d)    %010d(%01d)  |"                  "%010d(%01d)    %010d(%01d)  |\n"    
"%010d(%01d)                  |"            "%010d(%01d)    %010d(%01d)  |"                  "%010d(%01d)    %010d(%01d)  |"                  "%010d(%01d)    %010d(%01d)  |\n"    
"%010d(%01d)                  |"            "%010d(%01d)    %010d(%01d)  |"                  "%010d(%01d)    %010d(%01d)  |"                  "%010d(%01d)    %010d(%01d)  |\n",   





stImgListState.u32List[0][0],stImgListState.u32List[0][1],      pstFbPrc[0]->u32List[0][0],pstFbPrc[0]->u32List[0][1],pstFbPrc[0]->u32List[6][0],pstFbPrc[1]->u32List[6][1],		pstFbPrc[1]->u32List[0][0],pstFbPrc[1]->u32List[0][1],pstFbPrc[1]->u32List[6][0],pstFbPrc[1]->u32List[6][1],  		pstFbPrc[2]->u32List[0][0],pstFbPrc[2]->u32List[0][1],pstFbPrc[2]->u32List[6][0],pstFbPrc[2]->u32List[6][1],  
stImgListState.u32List[1][0],stImgListState.u32List[1][1],      pstFbPrc[0]->u32List[1][0],pstFbPrc[0]->u32List[1][1],pstFbPrc[0]->u32List[7][0],pstFbPrc[1]->u32List[7][1],            pstFbPrc[1]->u32List[1][0],pstFbPrc[1]->u32List[1][1],pstFbPrc[1]->u32List[7][0],pstFbPrc[1]->u32List[7][1],            pstFbPrc[2]->u32List[1][0],pstFbPrc[2]->u32List[1][1],pstFbPrc[2]->u32List[7][0],pstFbPrc[2]->u32List[7][1],  
stImgListState.u32List[2][0],stImgListState.u32List[2][1],      pstFbPrc[0]->u32List[2][0],pstFbPrc[0]->u32List[2][1],pstFbPrc[0]->u32List[8][0],pstFbPrc[0]->u32List[8][1],            pstFbPrc[1]->u32List[2][0],pstFbPrc[1]->u32List[2][1],pstFbPrc[1]->u32List[8][0],pstFbPrc[1]->u32List[8][1],            pstFbPrc[2]->u32List[2][0],pstFbPrc[2]->u32List[2][1],pstFbPrc[2]->u32List[8][0],pstFbPrc[2]->u32List[8][1],  
stImgListState.u32List[3][0],stImgListState.u32List[3][1],      pstFbPrc[0]->u32List[3][0],pstFbPrc[0]->u32List[3][1],pstFbPrc[0]->u32List[9][0],pstFbPrc[0]->u32List[9][1],            pstFbPrc[1]->u32List[3][0],pstFbPrc[1]->u32List[3][1],pstFbPrc[1]->u32List[9][0],pstFbPrc[1]->u32List[9][1],            pstFbPrc[2]->u32List[3][0],pstFbPrc[2]->u32List[3][1],pstFbPrc[2]->u32List[9][0],pstFbPrc[2]->u32List[9][1],  
stImgListState.u32List[4][0],stImgListState.u32List[4][1],      pstFbPrc[0]->u32List[4][0],pstFbPrc[0]->u32List[4][1],pstFbPrc[0]->u32List[10][0],pstFbPrc[0]->u32List[10][1],          pstFbPrc[1]->u32List[4][0],pstFbPrc[1]->u32List[4][1],pstFbPrc[1]->u32List[10][0],pstFbPrc[1]->u32List[10][1],          pstFbPrc[2]->u32List[4][0],pstFbPrc[2]->u32List[4][1],pstFbPrc[2]->u32List[10][0],pstFbPrc[2]->u32List[10][1],
stImgListState.u32List[5][0],stImgListState.u32List[5][1],      pstFbPrc[0]->u32List[5][0],pstFbPrc[0]->u32List[5][1],pstFbPrc[0]->u32List[11][0],pstFbPrc[0]->u32List[11][1],          pstFbPrc[1]->u32List[5][0],pstFbPrc[1]->u32List[5][1],pstFbPrc[1]->u32List[11][0],pstFbPrc[1]->u32List[11][1],          pstFbPrc[2]->u32List[5][0],pstFbPrc[2]->u32List[5][1],pstFbPrc[2]->u32List[11][0],pstFbPrc[2]->u32List[11][1]
                                                                    
);

    
READFREE:

    for(u32Count = 0; u32Count < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER; u32Count++)
    {
        if (pstPortPrc[u32Count] != HI_NULL)
            VPSS_VFREE(pstPortPrc[u32Count]);
    }
    return HI_SUCCESS;
    
}

HI_S32 VPSS_CTRL_ProcWrite(struct file * file,
    const char __user * buf, size_t count, loff_t *ppos)
{
    struct seq_file   *s = file->private_data;
    DRV_PROC_ITEM_S  *pProcItem = s->private;
    VPSS_HANDLE hVpss;
    HI_CHAR  chCmd[60] = {0};
    HI_CHAR  chArg1[DEF_FILE_NAMELENGTH] = {0};
    HI_CHAR  chArg2[DEF_FILE_NAMELENGTH] = {0};
    HI_CHAR  chArg3[DEF_FILE_NAMELENGTH] = {0};
    HI_CHAR  chFile[DEF_FILE_NAMELENGTH] = {0};
    HI_U32   u32PortID;
    VPSS_INSTANCE_S *pstInstance;
    
    hVpss = (VPSS_HANDLE)pProcItem->data;
    pstInstance = VPSS_CTRL_GetInstance(hVpss);
    if (pstInstance == HI_NULL)
    {
        VPSS_FATAL("Can't Get Debug Instance.\n");           
    }
    
    if(count > 40)
    {
        printk("Error:Echo too long.\n");
        return (-1);
    }
    
    if(copy_from_user(chCmd,buf,count))
    {
        printk("copy from user failed\n");
        return (-1);
    }


    VPSS_OSAL_GetProcArg(chCmd, chArg1, 1);
    VPSS_OSAL_GetProcArg(chCmd, chArg2, 2);
    VPSS_OSAL_GetProcArg(chCmd, chArg3, 3);

    if (chArg1[0] == 'h' && chArg1[1] == 'e' && chArg1[2] == 'l' && chArg1[3] == 'p')
    {
        printk("----------VPSS PROC HELP---------------------\n"
               "echo arg1 arg2 arg3 > /proc/msp/vpss_instxxxx\n"
               "arg1 == 0 :Write One Out YUV\n"
               "usage     :arg2 == PortID arg3 == path\n"
               "eg        :echo 0 1 yuv_test > /proc/msp/vpss_inst0000\n"
               "arg1 == 1 :Write One In YUV\n"
               "usage     :arg2 == path \n"
               "eg        :echo 1 yuv_test > /proc/msp/vpss_inst0000\n");
                  
    }
    else
    {
        VPSS_DBG_CMD_S stDbgCmd;
        VPSS_DBG_YUV_S *pstYUV;
        switch(chArg1[0]-'0')
        {
            case 0:
                memcpy(chFile,chArg3,sizeof(HI_CHAR)*DEF_FILE_NAMELENGTH);
                VPSS_OSAL_StrToNumb(chArg2,&u32PortID);

                stDbgCmd.enDbgType = DBG_W_OUT_YUV;
                
                pstYUV = (VPSS_DBG_YUV_S *)(&(stDbgCmd.u32Reserve[0]));
                pstYUV->hPort = u32PortID;
                memcpy(pstYUV->chFile,chArg3,sizeof(HI_CHAR)*DEF_FILE_NAMELENGTH);

                VPSS_DBG_SendDbgCmd(&(pstInstance->stDbgCtrl), &stDbgCmd);
                break;
            #if 1
            case 1:
                stDbgCmd.enDbgType = DBG_W_IN_YUV;
                memcpy(&(stDbgCmd.u32Reserve[0]),chArg2,sizeof(HI_CHAR)*DEF_FILE_NAMELENGTH);

                VPSS_DBG_SendDbgCmd(&(pstInstance->stDbgCtrl), &stDbgCmd);
                break;
            #endif
            default:
                VPSS_FATAL("Cmd Can't Support\n");
        }

    }
    return count;
}

HI_S32 VPSS_CTRL_CreateInstProc(VPSS_HANDLE hVPSS)
{
    HI_CHAR           ProcName[20];
    DRV_PROC_ITEM_S  *pProcItem;

    sprintf(ProcName, "vpss_inst%04x", (HI_U32)(hVPSS));

    pProcItem = HI_DRV_PROC_AddModule(ProcName, HI_NULL, HI_NULL);

    if (!pProcItem)
    {
        VPSS_FATAL("Vpss add proc failed!\n");
        return HI_FAILURE;
    }

    pProcItem->data  = (HI_VOID *)hVPSS;
    pProcItem->read  = VPSS_CTRL_ProcRead;
    pProcItem->write = VPSS_CTRL_ProcWrite;

    return HI_SUCCESS;
}
HI_S32 VPSS_CTRL_DestoryInstProc(VPSS_HANDLE hVPSS)
{
    HI_CHAR           ProcName[20];
    sprintf(ProcName, "vpss_inst%04x", (HI_U32)(hVPSS));
    //printk("\nDestoryInstProc %d\n",hVPSS);
    HI_DRV_PROC_RemoveModule(ProcName);
    return HI_SUCCESS;
}


HI_S32 VPSS_CTRL_GetUVCfg(VPSS_ALG_FRMCFG_S *pstFrmCfg,
                                HI_U32 *pu32EnUV,
                                HI_DRV_VIDEO_FRAME_S *pstImage)
{
    if ((pstFrmCfg->ePixFormat == HI_DRV_PIX_FMT_NV12 
        || pstFrmCfg->ePixFormat == HI_DRV_PIX_FMT_NV16)
        && pstImage->ePixFormat == HI_DRV_PIX_FMT_NV21)
    {   
        *pu32EnUV = HI_TRUE;
    }
    else if((pstFrmCfg->ePixFormat == HI_DRV_PIX_FMT_NV21 
        || pstFrmCfg->ePixFormat == HI_DRV_PIX_FMT_NV61)
        && pstImage->ePixFormat == HI_DRV_PIX_FMT_NV12)
    {
        *pu32EnUV = HI_TRUE;
    }
    else
    {
        *pu32EnUV = HI_FALSE;
    }

    return HI_SUCCESS;
}

HI_BOOL VPSS_CTRL_CheckRWZB(HI_DRV_VIDEO_FRAME_S *pstSrcImg,VPSS_PORT_S *pstPort,HI_BOOL bRWZB)
{
    HI_U32 u32InH;
    HI_U32 u32InW;
    HI_U32 u32OutH;
    HI_U32 u32OutW;
    
    u32InH = pstSrcImg->u32Height;
    u32InW = pstSrcImg->u32Width;
    if (u32InH == 1088 && u32InW == 1920)
    {
        u32InH = 1080;
    }
    
    u32OutH = pstPort->s32OutputHeight;
    u32OutW = pstPort->s32OutputWidth;
    if (u32OutH == 0 && u32OutW == 0)
    {
        u32OutH = u32InH;
        u32OutW = u32InW;
    }

    
    if (pstPort->stProcCtrl.eFidelity != HI_DRV_VPSS_FIDELITY_DISABLE
        && bRWZB 
        && u32OutH == u32InH
        && u32OutW == u32InW)
    {
        return HI_TRUE;
    }
    else
    {
        return HI_FALSE;
    }
}


HI_S32 VPSS_CTRL_StoreDeiData(VPSS_INSTANCE_S *pstInstance)
{
    ALG_FMD_RTL_STATPARA_S stFmdRtlStatPara;

    VPSS_HAL_GetDeiDate(&stFmdRtlStatPara);

    VPSS_INST_StoreDeiData(pstInstance,&stFmdRtlStatPara);

    return HI_SUCCESS;
}
#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */


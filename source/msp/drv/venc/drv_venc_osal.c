#include <linux/interrupt.h>
#include <linux/kthread.h>
#include <linux/delay.h>

#include "drv_mem_ext.h"
#include "drv_struct_ext.h"
#include "drv_venc_osal.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif

static HI_VOID (*ptrVencCallBack)(HI_VOID);

static irqreturn_t VENC_DRV_OsalVencISR(HI_S32 Irq, HI_VOID* DevID)
{
    (*ptrVencCallBack)();
    return IRQ_HANDLED;
}

HI_S32 VENC_DRV_OsalIrqInit( HI_U32 Irq, HI_VOID (*ptrCallBack)(HI_VOID))
{
    HI_S32 ret;

    if (Irq == VEDU_IRQ_ID)
    {
        ptrVencCallBack = ptrCallBack;
        ret = request_irq(Irq, VENC_DRV_OsalVencISR, IRQF_DISABLED, HI_MOD_VENC, NULL);
    }
    else
    {
        ret = HI_FAILURE;
    }

    if (ret == 0)
    {
        return HI_SUCCESS;
    }
    else
    {
        return HI_FAILURE;
    }
}

HI_VOID VENC_DRV_OsalIrqFree( HI_U32 Irq )
{
    free_irq(Irq, NULL);
}

HI_S32 VENC_DRV_OsalLockCreate( HI_VOID**phLock )
{
    spinlock_t *pLock;

    pLock = (spinlock_t *)HI_KMALLOC(HI_ID_VENC, sizeof(spinlock_t), GFP_KERNEL);
    if (pLock == NULL)
    {
        return HI_FAILURE;
    }

    spin_lock_init( pLock );

    *phLock = pLock;

    return HI_SUCCESS;
}

HI_VOID VENC_DRV_OsalLockDestroy( HI_VOID* hLock )
{
    HI_KFREE(HI_ID_VENC, hLock);
}

HI_VOID VENC_DRV_OsalLock( HI_VOID* hLock, VEDU_LOCK_FLAG *pFlag )
{
    spin_lock_irqsave((spinlock_t *)hLock, *pFlag);
}

HI_VOID VENC_DRV_OsalUnlock( HI_VOID* hLock, VEDU_LOCK_FLAG *pFlag )
{
    spin_unlock_irqrestore((spinlock_t *)hLock, *pFlag);
}

HI_S32 VENC_DRV_OsalCreateTask(HI_VOID **phTask, HI_U8 TaskName[], HI_VOID *pTaskFunction )
{
    *phTask = (HI_VOID *)kthread_create(pTaskFunction, NULL, TaskName);
    if (NULL == (*phTask))
    {
        return HI_FAILURE;
    }

    wake_up_process((struct task_struct*) (*phTask));
    return HI_SUCCESS;
}

HI_S32 VENC_DRV_OsalDeleteTask(HI_VOID *hTask)
{
    //kthread_stop( (struct task_struct*) hTask );
    return HI_SUCCESS;
}

#if 1
/************************************************************************/
/* 初始化事件                                                           */
/************************************************************************/
HI_S32 VENC_DRV_OsalInitEvent( VEDU_OSAL_EVENT *pEvent, HI_S32 InitVal )
{
	pEvent->flag = InitVal;
	init_waitqueue_head( &(pEvent->queue_head) );	
	return HI_SUCCESS;
}

/************************************************************************/
/* 发出事件唤醒                                                             */
/************************************************************************/
HI_S32 VENC_DRV_OsalGiveEvent( VEDU_OSAL_EVENT *pEvent )
{
	pEvent->flag = 1;
	//wake_up_interruptible ( &(pEvent->queue_head) );
	wake_up( &(pEvent->queue_head) );
	return HI_SUCCESS;
}

/************************************************************************/
/* 等待事件                                                             */
/* 事件发生返回OSAL_OK，超时返回OSAL_ERR 若condition不满足就阻塞等待    */
/* 被唤醒返回 0 ，超时返回非-1                                          */
/************************************************************************/
HI_S32 VENC_DRV_OsalWaitEvent( VEDU_OSAL_EVENT *pEvent, HI_U32 msWaitTime )
{
	HI_S32 l_ret = 0;

    if ( msWaitTime != 0xffffffff)
    {
       //l_ret = wait_event_interruptible_timeout( pEvent->queue_head, (pEvent->flag != 0), ((msWaitTime*10+50)/(msWaitTime)) );
       l_ret = wait_event_interruptible_timeout( pEvent->queue_head, (pEvent->flag != 0), msecs_to_jiffies(msWaitTime)/*msWaitTime/10*/ );

       pEvent->flag = 0;//(pEvent->flag>0)? (pEvent->flag-1): 0;

	   return (l_ret != 0)? HI_SUCCESS: HI_FAILURE;
    }
	else
	{
	   l_ret = wait_event_interruptible( pEvent->queue_head, (pEvent->flag != 0));
	   //wait_event(pEvent->queue_head, (pEvent->flag != 0));
	   pEvent->flag = 0;
	   return (l_ret == 0)? HI_SUCCESS: HI_FAILURE;
	}
}
#endif




#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif

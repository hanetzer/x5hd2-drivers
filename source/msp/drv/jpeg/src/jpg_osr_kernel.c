/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon. Co., Ltd.

******************************************************************************
File Name	    : jpg_osr_kernel.c
Version		    : Initial Draft
Author		    : 
Created		    : 2012/10/31
Description	    : 
Function List 	: 
			  		  
History       	:
Date				Author        		Modification
2012/10/31		    y00181162		                	
******************************************************************************/

#include <linux/module.h>
#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/ioctl.h>
#include <linux/delay.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/spinlock.h>
#include <linux/mm.h>
#include <linux/stddef.h>
#include <linux/fcntl.h>
#include <linux/slab.h>
#include <asm/atomic.h>
#include <asm/bitops.h>
#include <asm/system.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/pgtable.h>

#include "hi_kernel_adapt.h"

#include "jpgdrv_common.h"
#include "hi_jpg_ioctl.h"
#include "jpg_hal.h"
#include "jpg_driver.h"
#include "hi_jpg_errcode.h"
#include "hijpeg_type.h"

#include "hi_jpegdrv_common_k.h"
#include "hi_jpegdrv_api_k.h"

#include "hi_jpeg_config.h"


#ifdef __cplusplus
    #if __cplusplus
    extern "C"{
    #endif  /* __cplusplus */
#endif  /* __cplusplus */



/*********************************************************************************/
/**
 **   KERNEL
 **/
 
static JPEG_OSRDEV_S *gs_pstruJpegOsrDev_k = HI_NULL;

static struct semaphore s_JpegMutex;

static HI_BOOL gs_DrvIni = HI_FALSE;

 /*****************************************************************************
* func            : Jpeg_Halt_Osr
* description     : halt dispose function
* param[in]       : irq        halt singnal
* param[in]       : devId    
* param[in]       : ptrReg          the register message before to halt
* retval          : HI_SUCCESS      if success
* retval          : INVAL   
* retval          : EBUSY
* retval          : HI_FAILURE      if failure
* others:	      : nothing
*****************************************************************************/

static HI_S32 Jpeg_Halt_Osr(HI_S32 irq, HI_VOID * devId, struct pt_regs * ptrReg)
{

        HI_U32 IntType = 0;
        
        /** 
         ** get and set the halt status
         **/
        JpgHalGetIntStatus(&IntType);
        JpgHalSetIntStatus(IntType);

        if (IntType & 0x1)
        {
            gs_pstruJpegOsrDev_k->IntType = JPG_INTTYPE_FINISH;
        }
        else if (IntType & 0x2)
        {
            gs_pstruJpegOsrDev_k->IntType = JPG_INTTYPE_ERROR;
            HIJPEG_TRACE("Hard Decode Error!\n");
        }
        else if (IntType & 0x4)
        {
            gs_pstruJpegOsrDev_k->IntType = JPG_INTTYPE_CONTINUE;
        }
        /** AI7D02761 wake up the waiting halt **/
        wake_up_interruptible(&gs_pstruJpegOsrDev_k->QWaitInt);
        
        return IRQ_HANDLED;
        
    
}

/*****************************************************************************
* func            : Jpg_request_irq
* description     : request halt
* param[in]       : pOsrDev
* retval          : HI_SUCCESS      if success
* retval          : INVAL   
* retval          : EBUSY
* others:	      : nothing
*****************************************************************************/
static HI_VOID Jpg_Request_irq(HI_VOID)
{

        HI_S32 Ret = -1;

	    int IRQ_NUM = 0;
		HI_CHIP_TYPE_E     enChipType = HI_CHIP_TYPE_BUTT;
	    HI_CHIP_VERSION_E  enChipVersion = HI_CHIP_VERSION_BUTT;

		HI_DRV_SYS_GetChipVersion(&enChipType, &enChipVersion);

		if (   (HI_CHIP_TYPE_HI3716M == enChipType)
				|| (HI_CHIP_TYPE_HI3716H == enChipType)
				|| (HI_CHIP_TYPE_HI3716C == enChipType))
		{
		    IRQ_NUM = 42 + 32;
		}
		else if (HI_CHIP_TYPE_HI3712 == enChipType)
		{
	         IRQ_NUM = 42 + 32;
		}
		else
		{
		     HIJPEG_TRACE("chip type is not support.\n");
			 return;
		}
		
        Ret = request_irq(IRQ_NUM, (irq_handler_t)Jpeg_Halt_Osr, IRQF_SHARED, "x5_jpeg", gs_pstruJpegOsrDev_k);
        if ( HI_SUCCESS != Ret )
        {   
            kfree((HI_VOID *)gs_pstruJpegOsrDev_k);
            gs_pstruJpegOsrDev_k = HI_NULL;
            HIJPEG_TRACE("jpeg retuest halt fail.\n");
        }
		
        return;
            
}

/*****************************************************************************
* func            : Jpg_request_irq
* description     : request halt
* param[in]       : pOsrDev
* retval          : HI_SUCCESS      if success
* retval          : INVAL   
* retval          : EBUSY
* others:	      : nothing
*****************************************************************************/
static HI_VOID Jpg_Free_irq(HI_VOID)
{

	    int IRQ_NUM = 0;
		HI_CHIP_TYPE_E     enChipType = HI_CHIP_TYPE_BUTT;
	    HI_CHIP_VERSION_E  enChipVersion = HI_CHIP_VERSION_BUTT;

		HI_DRV_SYS_GetChipVersion(&enChipType, &enChipVersion);

		if (   (HI_CHIP_TYPE_HI3716M == enChipType)
				|| (HI_CHIP_TYPE_HI3716H == enChipType)
				|| (HI_CHIP_TYPE_HI3716C == enChipType))
		{
		    IRQ_NUM = 42 + 32;
		}
		else if (HI_CHIP_TYPE_HI3712 == enChipType)
		{
	         IRQ_NUM = 42 + 32;
		}
		else
		{
		     HIJPEG_TRACE("chip type is not support.\n");
			 return;
		}
		
	    free_irq(IRQ_NUM, (HI_VOID *)gs_pstruJpegOsrDev_k);    
		
	    return;
		
}

/*****************************************************************************
* func            : JpgOsrInit
* description     : initial the osr
* param[in]       : none
* retval          : HI_SUCCESS      if success
* retval          : INVAL   
* retval          : EBUSY
* others:	      : nothing
*****************************************************************************/
static HI_S32 JpgOsrInit(JPEG_OSRDEV_S *pOsrDev)
{    


        /** 
         ** map the register address to s_u32JpgRegAddr
         **/
        JpgHalInit();
        
        /**
         **  trun the halt status
         **/
        JpgHalSetIntMask(0x0);


        /**
         ** request halt
         **/
         Jpg_Request_irq();

        /**
         **  use to initial waitqueue head and mutex
         **/
        pOsrDev->EngageFlag = HI_FALSE;
        pOsrDev->pFile = HI_NULL;
        pOsrDev->IntType = JPG_INTTYPE_NONE;

        /**
         ** initial the waiting halt waiting queue
         **/
        init_waitqueue_head(&pOsrDev->QWaitInt);

        /**
         ** initial device occupy operation singnal 
         **/
        //init_MUTEX(&pOsrDev->SemGetDev);
        HI_INIT_MUTEX(&pOsrDev->SemGetDev);

        /************ want to realize suspend,should init mutex first *****
    	 ** create mutex.
    	 **/
         sema_init(&s_JpegMutex,1); /** initial the mutex with 1 **/
        
        return HI_SUCCESS;

    
}

/*****************************************************************************
* func            : HI_Jpeg_IfDrvIni
* description     : if first start kernel, the HI_JpegDrv_Init has called by init_play.
                    but when you rmmod jpeg.ko and insmod jpeg.ko the HI_JpegDrv_Init
                    has not called by init_play, so you should call it by _ini();
* param[in]       :
* retval          :
* retval          :
* others:	      : nothing
*****************************************************************************/

HI_VOID HI_Jpeg_IfDrvIni(HI_BOOL *bDrvIni)
{

    *bDrvIni = gs_DrvIni;
    return;
     
}

/*****************************************************************************
* func            : HI_JpegDrv_Init()
* description     :
* param[in]       :
* output          : 
* retval          :
* retval          : 
* retval          : 
* retval          : 
* others:	      : 
*****************************************************************************/
HI_S32 HI_JpegDrv_Init(HI_VOID)
{


        HI_S32 Ret = HI_FAILURE;
        
        /**
         ** if operation, return failure -EBUSY
         **/
        if (HI_NULL != gs_pstruJpegOsrDev_k)
        { 
            return -EBUSY;
        }

        /**
         **malloc and initial the struct that drive needed to s_pstruJpgOsrDev,
         ** if malloc failure, return -NOMEM
         **/
        gs_pstruJpegOsrDev_k = (JPEG_OSRDEV_S *) kmalloc(sizeof(JPEG_OSRDEV_S), GFP_KERNEL);
        if ( HI_NULL == gs_pstruJpegOsrDev_k )
        {
            return -ENOMEM;
        }
        memset(gs_pstruJpegOsrDev_k, 0x0, sizeof(JPEG_OSRDEV_S));


       /** call JpgOsrInit to initial OSR modual, if failure should release the
        ** resource and return failure
        **/
        Ret = JpgOsrInit(gs_pstruJpegOsrDev_k);
        if (HI_SUCCESS != Ret)
        {
           kfree((HI_VOID *)gs_pstruJpegOsrDev_k);
           gs_pstruJpegOsrDev_k = HI_NULL;
           return HI_FAILURE;
        }
       
		gs_DrvIni = HI_TRUE;
        
        return HI_SUCCESS;


}


/*****************************************************************************
* func            : JpgOsrDeinit
* description     : exit initial the osr
* param[in]       : pOsrDev   osr device imformation
* retval          : none
* others:	      : nothing
*****************************************************************************/
static HI_VOID JpgOsrDeinit(JPEG_OSRDEV_S *pOsrDev)
{    

        /**
	     **  use to initial waitqueue head and mutex
	     **/
	    pOsrDev->EngageFlag = HI_FALSE;
	    pOsrDev->pFile = HI_NULL;
	    pOsrDev->IntType = JPG_INTTYPE_NONE;

	    /**
	     ** initial the waiting halt waiting queue 
	     **/
	    init_waitqueue_head(&pOsrDev->QWaitInt);

	    /**
	     ** initial device occupy operation singnal
	     **/
	    //init_MUTEX(&pOsrDev->SemGetDev);
           HI_INIT_MUTEX(&pOsrDev->SemGetDev);
	    /**
	     ** unmap the register address and set s_u32JpgRegAddr with zero
	     **/
	    JpgHalExit();
        
	    return;
    
}
/*****************************************************************************
* func            : HI_JpegDrv_Exit()
* description     :
* param[in]       :
* output          : 
* retval          :
* retval          : 
* retval          : 
* retval          : 
* others:	      : 
*****************************************************************************/
HI_S32 HI_JpegDrv_Exit(HI_VOID)
{

        
	    JPEG_OSRDEV_S *pDev = gs_pstruJpegOsrDev_k;

        /**
    	 ** free the halt
    	 **/
        Jpg_Free_irq();
                
	    /** 
	     ** exit initial Osr device
	     **/
	    JpgOsrDeinit(pDev);
	    
	    /** release the data struct that drive needed **/
	    kfree((HI_VOID *)pDev);

	    gs_pstruJpegOsrDev_k = HI_NULL;
        gs_DrvIni = HI_FALSE;

        return HI_SUCCESS;

        
}


 /*****************************************************************************
* func            : HI_Jpeg_GetIntStatus
* description     :
* param[in]       :
* retval          :
* retval          :
* others:	      : nothing
*****************************************************************************/
HI_S32 HI_Jpeg_GetIntStatus(HI_SIZE_T Arg)
{
     
        JPG_GETINTTYPE_S IntInfo;
        HI_S32 Ret = 0;
        HI_S32 loop = 0;
        HI_U32 FirstCount = 1;
        struct timeval sStart, sCurr;

	
	    int IRQ_NUM = 0;
		HI_CHIP_TYPE_E     enChipType = HI_CHIP_TYPE_BUTT;
	    HI_CHIP_VERSION_E  enChipVersion = HI_CHIP_VERSION_BUTT;
		
        /**
         ** checkt parameter
         **/
        if (0 == Arg)
        {   

            return HI_FAILURE;
        }

		HI_DRV_SYS_GetChipVersion(&enChipType, &enChipVersion);

		if (   (HI_CHIP_TYPE_HI3716M == enChipType)
				|| (HI_CHIP_TYPE_HI3716H == enChipType)
				|| (HI_CHIP_TYPE_HI3716C == enChipType))
		{
		    IRQ_NUM = 42 + 32;
		}
		else if (HI_CHIP_TYPE_HI3712 == enChipType)
		{
	         IRQ_NUM = 42 + 32;
		}
		else
		{
		     HIJPEG_TRACE("chip type is not support.\n");
			 return HI_FAILURE;
		}
		
        /**
         ** copy input parameter
         **/
       if(copy_from_user((HI_VOID *)&IntInfo, (HI_VOID *)Arg, sizeof(JPG_GETINTTYPE_S)))
	   {   

            HIJPEG_TRACE("=========  copy from user failure =============\n");
            return -EFAULT;  
       	}

        disable_irq(IRQ_NUM);
       /**
        ** get the halt type 
        **/
        if ((JPG_INTTYPE_NONE != gs_pstruJpegOsrDev_k->IntType)
             || (0 == IntInfo.TimeOut))
        {
            IntInfo.IntType = gs_pstruJpegOsrDev_k->IntType;
            gs_pstruJpegOsrDev_k->IntType = JPG_INTTYPE_NONE;
            enable_irq(IRQ_NUM);
            if(copy_to_user((HI_VOID *)Arg, (HI_VOID *)&IntInfo, sizeof(JPG_GETINTTYPE_S)))
		    { 
              return -EFAULT;  
       	    }
            
            return HI_SUCCESS;
        }
        enable_irq(IRQ_NUM);
		
        do
        {			
	            /**
	             ** if the value of overtime, to overtime waitiong
	             **/
	            Ret = wait_event_interruptible_timeout(gs_pstruJpegOsrDev_k->QWaitInt, 
	                 JPG_INTTYPE_NONE != gs_pstruJpegOsrDev_k->IntType, 
	                 IntInfo.TimeOut * HZ/1000);
            
	            loop = 0;

	            if(Ret > 0 || (JPG_INTTYPE_NONE != gs_pstruJpegOsrDev_k->IntType))
	            {
		                disable_irq(IRQ_NUM);
		                IntInfo.IntType = gs_pstruJpegOsrDev_k->IntType;
		                gs_pstruJpegOsrDev_k->IntType = JPG_INTTYPE_NONE;
		                enable_irq(IRQ_NUM);
					    break;
	            } 
	            else if( -ERESTARTSYS == Ret)
	            {
	                if(FirstCount)
	                {
	                    do_gettimeofday(&sStart);
	                    FirstCount = 0;
	                    loop = 1;
	                } 
	                else
	                {
	                    do_gettimeofday(&sCurr);
	                    /** avoid dead lock **/
	                    loop = (((sCurr.tv_sec - sStart.tv_sec)*1000000 + (sCurr.tv_usec - sStart.tv_usec)) <  IntInfo.TimeOut)?1:0;  
	                    /** check timeout **/
						if(!loop)
	                    {
	                    	 return HI_FAILURE;
	                    }
	                }
	            } 
	            else /** == 0(wait timeout) and others **/ 
	            {
	                return HI_FAILURE;
	            }
			
        }while(loop);
			
        /** 
         ** get halt status and return
         **/
        if(copy_to_user((HI_VOID *)Arg, (HI_VOID *)&IntInfo,sizeof(JPG_GETINTTYPE_S)))
	    {  

            HIJPEG_TRACE("=========  copy to user failure =============\n");
            return -EFAULT;  
       	}

        return HI_SUCCESS;

}
 
/*****************************************************************************
* func            :HI_Jpeg_GetDevice
* description     :
* param[in]       :
* retval          :
* retval          :
* others:	      : nothing
*****************************************************************************/
HI_S32 HI_Jpeg_GetDevice(HI_VOID)
{

    if(down_interruptible(&s_JpegMutex)){

        HIJPEG_TRACE("=========  down interruptible failure =============\n");
        return HI_FAILURE;
    }
    return HI_SUCCESS;

}

/*****************************************************************************
* func            :HI_Jpeg_ReleaseDevice
* description     :
* param[in]       :
* retval          :
* retval          :
* others:	      : nothing
*****************************************************************************/

HI_S32 HI_Jpeg_ReleaseDevice(HI_VOID)
{

    up(&s_JpegMutex);
    return HI_SUCCESS;
}
/*****************************************************************************
* func            :HI_Jpeg_GetJpegOsrDev
* description     :
* param[in]       :
* retval          :
* retval          :
* others:	      : nothing
*****************************************************************************/

HI_VOID HI_Jpeg_GetJpegOsrDev(JPEG_OSRDEV_S **pstruJpegOsrDev)
{

    *pstruJpegOsrDev = gs_pstruJpegOsrDev_k;
    return;
     
}

   
EXPORT_SYMBOL (HI_Jpeg_IfDrvIni);
EXPORT_SYMBOL(HI_JpegDrv_Init);
EXPORT_SYMBOL(HI_JpegDrv_Exit);
EXPORT_SYMBOL(HI_Jpeg_GetIntStatus);
EXPORT_SYMBOL(HI_Jpeg_GetDevice);
EXPORT_SYMBOL(HI_Jpeg_ReleaseDevice);
EXPORT_SYMBOL(HI_Jpeg_GetJpegOsrDev);


#ifdef __cplusplus
    #if __cplusplus
}
    #endif  /* __cplusplus */
#endif  /* __cplusplus */

/******************************************************************************

  Copyright (C), 2001-2014, Hisilicon. Co., Ltd.

******************************************************************************
File Name	    : jpg_osr.c
Version		    : Initial Draft
Author		    : 
Created		    : 2013/03/26
Description	    : 
Function List 	: 
			  		  
History       	:
Date				Author        		Modification
2013/03/26		   y00181162 		                	
******************************************************************************/


/*********************************add include here******************************/
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
#include "drv_dev_ext.h"

#include "hi_kernel_adapt.h"

#include "jpg_common.h"
#include "hi_jpg_ioctl.h"
#include "jpg_hal.h"
#include "jpg_driver.h"
#include "hi_jpg_errcode.h"
#include "hijpeg_type.h"
#include "hi_jpeg_config.h"
#include "drv_module_ext.h"


#ifndef HIJPEG_GAO_AN_VERSION
#include "hijpeg_proc.h"
#endif


/***************************** Macro Definition ******************************/

#define JPEGNAME "HI_JPEG"

/**
 ** when you load driver, it can display vertion
 **/
#define MKSTR(exp) # exp
#define MKMARCOTOSTR(exp) MKSTR(exp)

/*************************** Structure Definition ****************************/

/********************** Global Variable declaration **************************/

extern JPEG_PROC_INFO_S s_stJpeg6bProcInfo;

/**
 ** used at suspend 
 **/
static struct semaphore s_JpegMutex;
static volatile HI_U32 *s_pJpegCRG;
static volatile HI_U32 *s_pJpegRegBase;

/******************************* API forward declarations *******************/

static HI_S32 jpg_osr_open(struct inode *inode, struct file *file);
static HI_S32 jpg_osr_close( struct inode *inode, struct file *file);
static long jpg_osr_ioctl(struct file *file, HI_U32 Cmd, unsigned long Arg);
static int jpg_osr_mmap(struct file * filp, struct vm_area_struct *vma);


/******************************* API realization *****************************/

/**
 ** device file operation 
 **/
static struct file_operations jpg_fops = {
    .owner   = THIS_MODULE,
    .open    = jpg_osr_open,
    .release = jpg_osr_close,
    .unlocked_ioctl = jpg_osr_ioctl,
    .mmap    = jpg_osr_mmap,
};


/**
 ** jpeg device imformation
 **/
typedef struct hiJPG_OSRDEV_S
{

    struct semaphore   SemGetDev;  /** protect the device to occupy the operation singnal **/
    HI_BOOL            EngageFlag; /** whether be occupied, HI_TRUE if be occupied **/
    struct file        *pFile;
    JPG_INTTYPE_E      IntType;    /** lately happened halt type **/
    wait_queue_head_t  QWaitInt;   /** waite halt queue **/
    
}JPG_OSRDEV_S;
static JPG_OSRDEV_S *s_pstruJpgOsrDev = HI_NULL;


#ifndef USE_HIMEDIA_DEVICE

static struct miscdevice jpeg_dev =
{
    MISC_DYNAMIC_MINOR,
    "jpeg",
    &jpg_fops,
};

#else
static HI_S32  jpeg_suspend(PM_BASEDEV_S *pdev, pm_message_t state)
{

	HIJPEG_TRACE("jpeg suspend ok.");
    return HI_SUCCESS;
    
}

static HI_S32  jpeg_resume(PM_BASEDEV_S *pdev)
{   
    HIJPEG_TRACE("jpeg resume ok.");
	return HI_SUCCESS;
}
static PM_BASEOPS_S  jpeg_drvops = {
	.probe        = NULL,
	.remove       = NULL,
	.shutdown     = NULL,
	.prepare      = NULL,
	.complete     = NULL,
	.suspend      = jpeg_suspend,
	.suspend_late = NULL,
	.resume_early = NULL,
	.resume       = jpeg_resume,
};


static UMAP_DEVICE_S jpeg_dev = {
	.minor	= UMAP_MIN_MINOR_JPEG,
	.devfs_name	= "jpeg",
	.owner  = THIS_MODULE,
	.fops  = &jpg_fops,
	.drvops = &jpeg_drvops
};

#endif


/**
 ** jpeg device imformation
 **/
typedef struct hiJPG_DISPOSE_CLOSE_S
{

     HI_S32 s32SuspendClose;
     HI_S32 s32DecClose;
     HI_BOOL bOpenUp;
     HI_BOOL bSuspendUp;
     HI_BOOL bRealse;

}JPG_DISPOSE_CLOSE_S;



/*****************************************************************************
* func            :GRC_SYS_GetTimeStampMs
* description     :得到ms
* param[in]       :
* param[in]       :
*****************************************************************************/
static HI_S32 GRC_SYS_GetTimeStampMs(HI_U32 *pu32TimeMs)
{
	HI_U64 u64TimeNow;
    HI_U64 ns;

    if(HI_NULL == pu32TimeMs)
	{
		return HI_FAILURE;
	}

	u64TimeNow = sched_clock();

	*pu32TimeMs = (HI_U32)iter_div_u64_rem(u64TimeNow,1000000,&ns);
	
	return HI_SUCCESS;
	
}

/*****************************************************************************
* func            : jpg_do_cancel_reset(HI_VOID)
                    要加void否则有告警
* description     : cancel reset jpeg register
* param[in]       :
* retval          : 
* others:	      : nothing
*****************************************************************************/
static HI_VOID jpg_do_cancel_reset(HI_VOID)
{

		volatile HI_U32* pResetAddr = NULL;

		pResetAddr   = s_pJpegCRG;

		/** 不复位 **/
	    #if defined(HI_S40V200_VERSION)
		
             *pResetAddr &= S40V200_JPG_UNRESET_REG_VALUE;
		
		#elif defined(HI_3716CV200_VERSION)
		
             *pResetAddr &= HI3716CV200_JPG_UNRESET_REG_VALUE;
		
		#else

		#endif

		
}

/*****************************************************************************
* func            : jpg_do_cancel_reset
* description     : cancel reset jpeg register
* param[in]       :
* retval          : 
* others:	      : nothing
*****************************************************************************/
static HI_VOID jpg_do_reset(HI_VOID)
{

	    volatile HI_U32* pResetAddr = NULL;
	    volatile HI_U32* pBusyAddr = NULL;

		pResetAddr   = s_pJpegCRG;
	    pBusyAddr    = s_pJpegRegBase;


	    #if defined(HI_S40V200_VERSION)
		
			*pResetAddr |= S40V200_JPG_RESET_REG_VALUE;
		    while (*pBusyAddr & 0x2);
			{
		       *pResetAddr &= S40V200_JPG_UNRESET_REG_VALUE;
		    }
			
		#elif defined(HI_3716CV200_VERSION)
		
	        /**解码完成之后要复位, 复位让JPEG解码器恢复到初始化状态 **/
			*pResetAddr |= HI3716CV200_JPG_RESET_REG_VALUE;

		    /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		      [31:2]    保留
		      [1]       是否复位标志
		                0: 已经复位
		                1: 没有复位
		      [0]       JPEG解码启动寄存器
		                1:启动JPEG进行解码，解码启动后，此寄存器会自动清0。

		      ++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	  
		    while (*pBusyAddr & 0x2);
			{/** 假如已经复位，则要写零不复位，否则会一直处于复位状态 **/
				
		       *pResetAddr &= HI3716CV200_JPG_UNRESET_REG_VALUE;
		    }
			
		#else
		
		#endif
	
}

/*****************************************************************************
* func            : jpg_do_clock_off
* description     : close the jpeg clock
* param[in]       :
* retval          : 
* others:	      : nothing
*****************************************************************************/
static HI_VOID jpg_do_clock_off(HI_VOID)
{

	    volatile HI_U32* pResetAddr = NULL;
	    pResetAddr   = s_pJpegCRG;

		#if defined(HI_S40V200_VERSION)
		
		   *pResetAddr &= S40V200_JPG_CLOCK_OFF;
		
	    #elif defined(HI_3716CV200_VERSION)
		
           *pResetAddr &= HI3716CV200_JPG_CLOCK_OFF;
		
		#else

		#endif
}

/*****************************************************************************
* func            : jpg_do_clock_on
* description     : open the jpeg clock
* param[in]       :
* retval          : 
* others:	      : nothing
*****************************************************************************/
static HI_VOID jpg_do_clock_on(HI_VOID)
{

	    volatile HI_U32* pResetAddr = NULL;
	    pResetAddr   = s_pJpegCRG;

		#if defined(HI_S40V200_VERSION)
		
		   *pResetAddr |= S40V200_JPG_CLOCK_ON;
		
		#elif defined(HI_3716CV200_VERSION)
		
           *pResetAddr |= HI3716CV200_JPG_CLOCK_ON;
		
		#else

		#endif
}


 /*****************************************************************************
* func            : JpgOsrISR
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
static HI_S32 JpgOsrISR(HI_S32 irq, HI_VOID * devId, struct pt_regs * ptrReg)
{

        HI_U32 IntType = 0;
        
        /** 
         ** get and set the halt status
         **/
        JpgHalGetIntStatus(&IntType);
        JpgHalSetIntStatus(IntType);


        if (IntType & 0x1)
        {
            s_pstruJpgOsrDev->IntType = JPG_INTTYPE_FINISH;	  
        }
        else if (IntType & 0x2)
        {
            s_pstruJpgOsrDev->IntType = JPG_INTTYPE_ERROR;
        }
        else if (IntType & 0x4)
        {
            s_pstruJpgOsrDev->IntType = JPG_INTTYPE_CONTINUE;
        }

		
        /** AI7D02761 wake up the waiting halt **/
        wake_up_interruptible(&s_pstruJpgOsrDev->QWaitInt);
        
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

        #if defined(HI_S40V200_VERSION)
		
             IRQ_NUM = S40V200_JPGD0_IRQ_NUM;
		
		#elif defined(HI_3716CV200_VERSION)
		
             IRQ_NUM = HI3716CV200_JPGD0_IRQ_NUM;
		
		#else
		
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
				 return;
			}
		#endif

	    Ret = request_irq(IRQ_NUM, (irq_handler_t)JpgOsrISR, IRQF_SHARED, "x5_jpeg", s_pstruJpgOsrDev);
	    if ( HI_SUCCESS != Ret )
	    {   
			JPEG_KFREE(HI_ID_JPGDEC, (HI_VOID *)s_pstruJpgOsrDev);
	        s_pstruJpgOsrDev = HI_NULL;
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


        #if defined(HI_S40V200_VERSION)
		
             IRQ_NUM = S40V200_JPGD0_IRQ_NUM;
		
		#elif defined(HI_3716CV200_VERSION)
		
             IRQ_NUM = HI3716CV200_JPGD0_IRQ_NUM;
		
		#else
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
				 return;
			}
		#endif

		
        free_irq(IRQ_NUM, (HI_VOID *)s_pstruJpgOsrDev);
		
        return;
		
}




/*****************************************************************************
* func            : JpgOsrDeinit
* description     : exit initial the osr
* param[in]       : pOsrDev   osr device imformation
* retval          : none
* others:	      : nothing
*****************************************************************************/
static HI_VOID JpgOsrDeinit(JPG_OSRDEV_S *pOsrDev)
{    

        /**
	     **  use to initial waitqueue head and mutex
	     **/
	    pOsrDev->EngageFlag = HI_FALSE;
	    pOsrDev->pFile      = HI_NULL;
	    pOsrDev->IntType    = JPG_INTTYPE_NONE;

	    /**
	     ** initial the waiting halt waiting queue 
	     **/
	    init_waitqueue_head(&pOsrDev->QWaitInt);

	    /**
	     ** initial device occupy operation singnal
	     **/
	    HI_INIT_MUTEX(&pOsrDev->SemGetDev);

        
         /**
    	  **clean up the proc
    	  **/
        JPEG_Proc_Cleanup();
		
	    /**
	     ** unmap the register address and set s_u32JpgRegAddr with zero
	     **/
	    JpgHalExit();
        
	    return;
    
}


 /*****************************************************************************
* func            : JPEG_DRV_ModExit
* description     : exit initial the device
* param[in]       :
* retval          : none
* others:	      :
*****************************************************************************/
HI_VOID JPEG_DRV_ModExit(HI_VOID)
{


	    JPG_OSRDEV_S *pDev = s_pstruJpgOsrDev;

		HI_DRV_MODULE_UnRegister(HI_ID_JPGDEC);
		
        /**
	     ** uninstall the device 
	     **/
        #ifndef USE_HIMEDIA_DEVICE
        misc_deregister(&jpeg_dev);
		#else
        HI_DRV_DEV_UnRegister(&jpeg_dev);
        #endif
		
        /**
    	 ** free the halt
    	 **/
        Jpg_Free_irq();
                
	    /** 
	     ** exit initial Osr device
	     **/
	    JpgOsrDeinit(pDev);

	    /** release the data struct that drive needed **/
		JPEG_KFREE(HI_ID_JPGDEC, (HI_VOID *)pDev);

	    s_pstruJpgOsrDev = HI_NULL;
        

        iounmap(s_pJpegRegBase);
        iounmap(s_pJpegCRG);
		
        s_pJpegRegBase  = NULL;
        s_pJpegCRG      = NULL;

	    return;

    
}




/*****************************************************************************
* func            : jpeg6b_version
* description     :
* param[in]       : 
* retval          :
* others:	      : nothing
*****************************************************************************/
static HI_VOID jpeg6b_version(HI_VOID)
{

	/** 高安版本不能有打印 **/
    #ifndef HIJPEG_GAO_AN_VERSION

	    HI_CHAR JPEG6BVersion[160] ="SDK_VERSION:["MKMARCOTOSTR(jpeg6bv1.0)"] Build Time:["\
	        __DATE__", "__TIME__"]";
	    printk("Load hi_jpeg.ko success.\t\t(%s)\n", JPEG6BVersion);
	
    #endif
}

/*****************************************************************************
* func            : JpgOsrInit
* description     : initial the osr
* param[in]       : none
* retval          : HI_SUCCESS
* retval          : INVAL   
* retval          : EBUSY
* others:	      : nothing
*****************************************************************************/
static HI_S32 JpgOsrInit(JPG_OSRDEV_S *pOsrDev)
{    

        jpeg6b_version();

        /** 
         ** map the register address to s_u32JpgRegAddr
         **/
        JpgHalInit();


        /** 
         ** initial proc message
         **/
         JPEG_Proc_init();

         
    	/************ want to realize suspend,should init mutex first *****
    	 ** create mutex.
    	 **/
         sema_init(&s_JpegMutex,1); /** initial the mutex with 1 **/
    	/**************************************************/

        
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
        pOsrDev->EngageFlag  = HI_FALSE;
        pOsrDev->pFile       = HI_NULL;
        pOsrDev->IntType     = JPG_INTTYPE_NONE;

        /**
         ** initial the waiting halt waiting queue
         **/
        init_waitqueue_head(&pOsrDev->QWaitInt);

        /**
         ** initial device occupy operation singnal 
         **/
        HI_INIT_MUTEX(&pOsrDev->SemGetDev);

        return HI_SUCCESS;

    
}



/*****************************************************************************
* func            : JPEG_DRV_ModInit
* description     : exit initial the device
* param[in]       : none
* output          : none
* retval          : HI_SUCCESS
* retval          : -ENOMEM
* retval          : -EFAULT
* retval          : -EINVAL
* others:	      : nothing
*****************************************************************************/
HI_S32 JPEG_DRV_ModInit(HI_VOID)
{

	
        HI_S32 Ret = HI_FAILURE;

		HI_CHIP_TYPE_E     enChipType = HI_CHIP_TYPE_BUTT;
	    HI_CHIP_VERSION_E  enChipVersion = HI_CHIP_VERSION_BUTT;
		
        /**
         ** if operation, return failure -EBUSY
         **/
        if (HI_NULL != s_pstruJpgOsrDev)
        {   
            return -EBUSY;
        }

		HI_DRV_SYS_GetChipVersion(&enChipType, &enChipVersion);


		/*===================================================================================
                           获取JPEG寄存器以及时钟寄存器地址
		  ==================================================================================*/
        #if defined(HI_S40V200_VERSION)

		    s_pJpegRegBase = (volatile HI_U32*)ioremap_nocache(S40V200_JPGD0_REG_BASEADDR, JPG_REG_LENGTH);
			s_pJpegCRG     = (volatile HI_U32*)ioremap_nocache(S40V200_JPGD0_CRG_REG_PHYADDR, 4);

		#elif defined(HI_3716CV200_VERSION)

		    s_pJpegRegBase = (volatile HI_U32*)ioremap_nocache(HI3716CV200_JPGD0_REG_BASEADDR, JPG_REG_LENGTH);
			s_pJpegCRG     = (volatile HI_U32*)ioremap_nocache(HI3716CV200_JPGD0_CRG_REG_PHYADDR, 4);

		#else
			if (   (HI_CHIP_TYPE_HI3716M == enChipType)
					|| (HI_CHIP_TYPE_HI3716H == enChipType)
					|| (HI_CHIP_TYPE_HI3716C == enChipType))
			{
		        s_pJpegRegBase  = (volatile HI_U32*) ioremap_nocache(0x60100000, JPG_REG_LENGTH);
		        s_pJpegCRG      = (volatile HI_U32*) ioremap_nocache(JPG_CTL_REG_PHYADDR, 0x10);
			}
			else if (HI_CHIP_TYPE_HI3712 == enChipType)
			{
		        s_pJpegRegBase  = (volatile HI_U32*) ioremap_nocache(0x101a0000, JPG_REG_LENGTH);
		        s_pJpegCRG      = (volatile HI_U32*) ioremap_nocache(JPG_CTL_REG_PHYADDR, 0x10);
			}
			else
			{
				 return HI_FAILURE;
			}
		#endif


	    /*===================================================================================
                           操作寄存器之前要将时钟打开并且jpeg去复位
		  ==================================================================================*/
        jpg_do_clock_on();
		jpg_do_cancel_reset();

			
        /**
         **malloc and initial the struct that drive needed to s_pstruJpgOsrDev,
         ** if malloc failure, return -NOMEM
         **/
        s_pstruJpgOsrDev = (JPG_OSRDEV_S *)JPEG_KMALLOC(HI_ID_JPGDEC,sizeof(JPG_OSRDEV_S),GFP_KERNEL);
        if ( HI_NULL == s_pstruJpgOsrDev )
        {   
            return -ENOMEM;
        }
        memset(s_pstruJpgOsrDev, 0x0, sizeof(JPG_OSRDEV_S));


       /** call JpgOsrInit to initial OSR modual, if failure should release the
        ** resource and return failure
        **/
        Ret = JpgOsrInit(s_pstruJpgOsrDev);
        if (HI_SUCCESS != Ret)
        {
		   JPEG_KFREE(HI_ID_JPGDEC, (HI_VOID *)s_pstruJpgOsrDev);
           s_pstruJpgOsrDev = HI_NULL;
           return Ret;
        }
    
       /*===================================================================================
                          对jpeg设备进行注册，这样上层才有设备可以打开
		 ==================================================================================*/
        #ifndef USE_HIMEDIA_DEVICE
        Ret = misc_register(&jpeg_dev);
        #else
    	Ret = HI_DRV_DEV_Register(&jpeg_dev);
        #endif
        if (HI_SUCCESS != Ret)
        { 
			JPEG_KFREE(HI_ID_JPGDEC, (HI_VOID *)s_pstruJpgOsrDev);
            s_pstruJpgOsrDev = HI_NULL;
            return HI_FAILURE;
        }

		
		Ret = HI_DRV_MODULE_Register(HI_ID_JPGDEC, JPEGNAME, NULL); 
        if (HI_SUCCESS != Ret)
        {
            JPEG_DRV_ModExit();
	        return HI_FAILURE;
        }
    	   
        return HI_SUCCESS;

    
}


/*****************************************************************************
* func            : jpg_osr_open
* description     : turn on the jpeg device
* param[in]       : inode   kernel node
* param[in]       : flip    device file message
* output          : none
* retval          : HI_SUCCESS
* retval          : HI_FAILURE
* others:	      : nothing
*****************************************************************************/
static HI_S32 jpg_osr_open(struct inode *inode, struct file *file)
{   

	
    JPG_DISPOSE_CLOSE_S *sDisposeClose = NULL;
    sDisposeClose = (JPG_DISPOSE_CLOSE_S *)JPEG_KMALLOC(HI_ID_JPGDEC,               \
                                                        sizeof(JPG_DISPOSE_CLOSE_S),\
                                                        GFP_KERNEL);
	if ( HI_NULL == sDisposeClose )
    {    
        return -ENOMEM;
    }
	
    memset(sDisposeClose, 0x0, sizeof(JPG_DISPOSE_CLOSE_S));
    file->private_data             = sDisposeClose;
    sDisposeClose->s32DecClose     = HI_SUCCESS;
    sDisposeClose->s32SuspendClose = HI_FAILURE;
    sDisposeClose->bOpenUp         = HI_FALSE;
    sDisposeClose->bSuspendUp      = HI_FALSE;
    sDisposeClose->bRealse         = HI_FALSE;


	
	jpg_do_clock_on();
	jpg_do_reset();
	
    return HI_SUCCESS;
    
}

 /*****************************************************************************
* func            : jpg_osr_close
* description     : turn off the jpeg device
* param[in]       : inode   kernel node
* param[in]       : flip    device file message
* output          : none
* retval          : HI_SUCCESS
* retval          : HI_FAILURE
* others:	      : nothing
*****************************************************************************/

static HI_S32 jpg_osr_close( struct inode *inode, struct file *file )
{
         

        JPG_DISPOSE_CLOSE_S *sDisposeClose = NULL;
        sDisposeClose = file->private_data;
        if(NULL == sDisposeClose)
        {  
           return HI_FAILURE;
        }

        /**
         ** if suspend , do close device only
         **/
        if(HI_SUCCESS==sDisposeClose->s32SuspendClose)
		{
             if(HI_TRUE == sDisposeClose->bSuspendUp)
			 {
                up(&s_JpegMutex);
             }
			 JPEG_KFREE(HI_ID_JPGDEC, (HI_VOID *)sDisposeClose);
             return HI_SUCCESS;
        }
        if(HI_SUCCESS==sDisposeClose->s32DecClose)
		{
             if(HI_TRUE == sDisposeClose->bOpenUp)
			 {
                up(&s_JpegMutex);
             }
			 JPEG_KFREE(HI_ID_JPGDEC, (HI_VOID *)sDisposeClose);
             return HI_SUCCESS;
        }


        /**
         **  if call realse, should not call this
         **/
        if(HI_FALSE == sDisposeClose->bRealse)
        {
            /**
             ** set file private data to HI_NULL 
             **/
			JPEG_KFREE(HI_ID_JPGDEC, (HI_VOID *)sDisposeClose);
            
            /**
             **if device has not initial, return failure
             **/
            if (HI_NULL == s_pstruJpgOsrDev)
            {    
                up(&s_JpegMutex);
                return HI_FAILURE;
            }
            /**
             ** if the file occupy the device, set this device to not occupied,
             ** wake up waiting halt waiting queue
             **/
            if(down_interruptible(&s_pstruJpgOsrDev->SemGetDev));

            if ((HI_TRUE == s_pstruJpgOsrDev->EngageFlag) && (file == s_pstruJpgOsrDev->pFile))
            {
            
                s_pstruJpgOsrDev->EngageFlag = HI_FALSE;
                (HI_VOID)wake_up_interruptible(&s_pstruJpgOsrDev->QWaitInt);
                
            }
            /**
             ** to JPG reset operation, open the clock
             **/
            if(s_pstruJpgOsrDev->EngageFlag != HI_FALSE)
			{
				jpg_do_cancel_reset();
				jpg_do_clock_off();
                up(&s_pstruJpgOsrDev->SemGetDev);
                up(&s_JpegMutex);
        		return HI_FAILURE;
        	}
            if(s_pstruJpgOsrDev->IntType != JPG_INTTYPE_NONE)
			{
				jpg_do_cancel_reset();
				jpg_do_clock_off();
                up(&s_pstruJpgOsrDev->SemGetDev);
                up(&s_JpegMutex);
        		return HI_FAILURE;
            }


			jpg_do_cancel_reset();
			jpg_do_clock_off();
           
			
            up(&s_JpegMutex);
			
            up(&s_pstruJpgOsrDev->SemGetDev);
            
            return HI_SUCCESS;
            
            
        }

        /**
         ** set file private data to HI_NULL 
         **/
		JPEG_KFREE(HI_ID_JPGDEC, (HI_VOID *)sDisposeClose);
        
        return HI_SUCCESS;

        
}
	
 /*****************************************************************************
* func            : jpg_osr_mmap
* description     : map the register address
* param[in]       : vma     device virtual address imformation
* param[in]       : flip    device file message
* output          : none
* retval          : HI_SUCCESS
* retval          : -EINVAL
* retval          : -EAGAIN
* others:	      : nothing
*****************************************************************************/

static int jpg_osr_mmap(struct file * filp, struct vm_area_struct *vma )
{
      
 
        /** 上层map jpeg设备的时候调用 **/
        HI_U32 Phys;

        /**
         ** set map parameter 
         **/
        HI_CHIP_TYPE_E     enChipType = HI_CHIP_TYPE_BUTT;
	    HI_CHIP_VERSION_E  enChipVersion = HI_CHIP_VERSION_BUTT;

		HI_DRV_SYS_GetChipVersion(&enChipType, &enChipVersion);

        #if defined(HI_S40V200_VERSION)
		
		    Phys = (S40V200_JPGD0_REG_BASEADDR >> PAGE_SHIFT);
		
		#elif defined(HI_3716CV200_VERSION)
		
		    Phys = (HI3716CV200_JPGD0_REG_BASEADDR >> PAGE_SHIFT);
		
		#else
			if (   (HI_CHIP_TYPE_HI3716M == enChipType)
					|| (HI_CHIP_TYPE_HI3716H == enChipType)
					|| (HI_CHIP_TYPE_HI3716C == enChipType))
			{
				Phys = (0x60100000 >> PAGE_SHIFT);
			}
			else if (HI_CHIP_TYPE_HI3712 == enChipType)
			{
				 Phys = (0x101a0000 >> PAGE_SHIFT);
			}
			else
			{
				 return HI_FAILURE;
			}
		#endif
        
        vma->vm_flags |= VM_RESERVED | VM_LOCKED | VM_IO;

        /** cancel map **/
        vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);

        if (remap_pfn_range(vma, vma->vm_start, Phys, vma->vm_end - vma->vm_start, 
                            vma->vm_page_prot))
        {
            return -EAGAIN;
        }

        return HI_SUCCESS;

    
}
 
 /*****************************************************************************
* func            : jpg_osr_ioctl
* description     : jpeg device control interface
* param[in]       : inode  
* param[in]       : flip    device file message
* param[in]       : Cmd  
* param[in]       : Arg    
* output          : none
* retval          : HI_SUCCESS
* retval          : HI_ERR_JPG_DEC_BUSY
* retval          : -EINVAL
* retval          : -EAGAIN
* others:	      : nothing
*****************************************************************************/
static long jpg_osr_ioctl(struct file *file, HI_U32 Cmd, unsigned long Arg)
{


	    int IRQ_NUM           = 0;
        HI_U32 u32StartTimeMs = 0; /** ms **/
		HI_U32 u32EndTimeMs   = 0; /** ms **/
        
		HI_CHIP_TYPE_E     enChipType = HI_CHIP_TYPE_BUTT;
	    HI_CHIP_VERSION_E  enChipVersion = HI_CHIP_VERSION_BUTT;

		HI_DRV_SYS_GetChipVersion(&enChipType, &enChipVersion);



        #if defined(HI_S40V200_VERSION)
		
             IRQ_NUM = S40V200_JPGD0_IRQ_NUM;
		
		#elif defined(HI_3716CV200_VERSION)
		
		     IRQ_NUM = HI3716CV200_JPGD0_IRQ_NUM;
		
		#else
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
				 return HI_FAILURE;
			}
		#endif
			
		    
	    switch(Cmd)
	    {
	    
	        case CMD_JPG_GETDEVICE:
	        {


	            JPG_DISPOSE_CLOSE_S *sDisposeClose = NULL;
	            sDisposeClose = file->private_data;
	            
	        	/********if jpeg has not close, so jpeg is busy, you should suspend now **/
	        	if(down_interruptible(&s_JpegMutex)){ /** Mutex initial with 1, and after this func,the
	        	                             ** Mutex is zero, so has not mutex, next time should
	        	                             ** wait here, only the mutex is no zero, followed can
	        	                             ** operation
	        	                             **/
	        	      sDisposeClose->bOpenUp = HI_FALSE;
	                  return -ERESTARTSYS;
	            }
	        	/*************************************************************************/
	        			
	            /**
	             ** if has not initial device, return failure
	             **/
	            if (HI_NULL == s_pstruJpgOsrDev)
	            {   
	                return HI_FAILURE;
	            }

	            /**
	             ** locked the occupied device 
	             **/
	            if(down_interruptible(&s_pstruJpgOsrDev->SemGetDev));

	            s_pstruJpgOsrDev->EngageFlag = HI_TRUE;
	            s_pstruJpgOsrDev->IntType    = JPG_INTTYPE_NONE;
	            s_pstruJpgOsrDev->pFile      = file;
	            
	            sDisposeClose->s32DecClose   = HI_FAILURE;
	            sDisposeClose->bOpenUp       = HI_TRUE;
	            sDisposeClose->bRealse       = HI_FALSE;
	            /**
	             ** to JPG reset operation, open the clock
	             **/
	             jpg_do_clock_on();
				 jpg_do_reset();
				 
	             up(&s_pstruJpgOsrDev->SemGetDev);
	            
	             break;
	             
	        }
	        case CMD_JPG_RELEASEDEVICE:
	        {

	            JPG_DISPOSE_CLOSE_S *sDisposeClose = NULL;
	            sDisposeClose = file->private_data;
	            /**
	             **if device has not initial, return failure
	             **/
	            if (HI_NULL == s_pstruJpgOsrDev)
	            {
	                up(&s_JpegMutex);
	                return HI_FAILURE;
	            }
	            /**
	             ** if the file occupy the device, set this device to not occupied,
	             ** wake up waiting halt waiting queue
	             **/
	            if(down_interruptible(&s_pstruJpgOsrDev->SemGetDev));

	            if ((HI_TRUE == s_pstruJpgOsrDev->EngageFlag) && (file == s_pstruJpgOsrDev->pFile))
	            {
	            
	                s_pstruJpgOsrDev->EngageFlag = HI_FALSE;
	                (HI_VOID)wake_up_interruptible(&s_pstruJpgOsrDev->QWaitInt);
	                
	            }
	            
	            /**
	             ** to JPG reset operation, open the clock
	             **/
	            if(s_pstruJpgOsrDev->EngageFlag != HI_FALSE)
				{
	                up(&s_pstruJpgOsrDev->SemGetDev);
	                up(&s_JpegMutex);
	        		return HI_FAILURE;
	        	}
	            if(s_pstruJpgOsrDev->IntType != JPG_INTTYPE_NONE)
				{
	                up(&s_pstruJpgOsrDev->SemGetDev);
	                up(&s_JpegMutex);
	        		return HI_FAILURE;
	            }

				jpg_do_cancel_reset();
				jpg_do_clock_off();
				
	            up(&s_JpegMutex);
	            sDisposeClose->bRealse = HI_TRUE;

	            up(&s_pstruJpgOsrDev->SemGetDev);
	                     
	            break;
	            
	        }
	        case CMD_JPG_SUSPEND:
	        {    
	             JPG_DISPOSE_CLOSE_S *sDisposeClose = NULL;
	             sDisposeClose                      = file->private_data;
	             sDisposeClose->s32SuspendClose     = HI_SUCCESS;
	             if(down_interruptible(&s_JpegMutex))
				 {
	                  sDisposeClose->bSuspendUp = HI_FALSE;
	                  return -ERESTARTSYS;
	             }
	             sDisposeClose->bSuspendUp = HI_TRUE;
	             break;
	        }
	        case CMD_JPG_RESUME:
	        {    
	             /**
	              ** it maybe realse the mutex between suspend and resume, so should
	              ** realse the mutex at close
	              **/
	             JPG_DISPOSE_CLOSE_S *sDisposeClose = NULL;
	             sDisposeClose                      = file->private_data;
	             sDisposeClose->s32SuspendClose     = HI_SUCCESS;
	             sDisposeClose->bSuspendUp          = HI_FALSE;
	             up(&s_JpegMutex);
	             break;
	        }
	        case CMD_JPG_GETINTSTATUS:
	        {

				
	            JPG_GETINTTYPE_S IntInfo;
	            HI_S32 Ret = 0;
	            HI_S32 loop = 0;
	            HI_U32 FirstCount = 1;
	            /**
	             ** checkt parameter
	             **/
	            if (0 == Arg)
	            {
	                return HI_FAILURE;
	            }

	            /**
	             ** copy input parameter
	             **/
	           if(copy_from_user((HI_VOID *)&IntInfo, (HI_VOID *)Arg, sizeof(JPG_GETINTTYPE_S)))
			   {   
	                return -EFAULT;  
	           	}

	            disable_irq(IRQ_NUM);
	            
	           /**
	            ** get the halt type 
	            **/
	            if (    (JPG_INTTYPE_NONE != s_pstruJpgOsrDev->IntType)
	                 || (0 == IntInfo.TimeOut))
	            {

					
	                IntInfo.IntType = s_pstruJpgOsrDev->IntType;
	                s_pstruJpgOsrDev->IntType = JPG_INTTYPE_NONE;
	                enable_irq(IRQ_NUM);
	                
	                if(copy_to_user((HI_VOID *)Arg, (HI_VOID *)&IntInfo, sizeof(JPG_GETINTTYPE_S)))
	  		        { 
	                    return -EFAULT;  
	           	    }
	                break;
	            }
	            enable_irq(IRQ_NUM);

	            do
	            {			
	               /**
	                ** if the value of overtime, to overtime waitiong
	                **/
	                Ret = wait_event_interruptible_timeout(s_pstruJpgOsrDev->QWaitInt, 
	                              JPG_INTTYPE_NONE != s_pstruJpgOsrDev->IntType, 
	                              IntInfo.TimeOut * HZ/1000);
					 
	                loop = 0;

	                if(Ret > 0 || (JPG_INTTYPE_NONE != s_pstruJpgOsrDev->IntType))
	                {

	                    disable_irq(IRQ_NUM);
	                    IntInfo.IntType = s_pstruJpgOsrDev->IntType;
	                    s_pstruJpgOsrDev->IntType = JPG_INTTYPE_NONE;
	                    enable_irq(IRQ_NUM);
	                    break;
	                } 
	                else if( -ERESTARTSYS == Ret)
	                {

	                    if(FirstCount)
	                    {
                            GRC_SYS_GetTimeStampMs(&u32StartTimeMs);
	                        FirstCount = 0;
	                        loop = 1;
	                    } 
	                    else
	                    {

	                        GRC_SYS_GetTimeStampMs(&u32EndTimeMs);
	                        /** avoid dead lock **/
                            loop = ((u32EndTimeMs - u32StartTimeMs) <  IntInfo.TimeOut)?1:0; 
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
	                return -EFAULT;  
	           	}
	            
	            break;
	        }
	        case CMD_JPG_READPROC:
	        {   

	            HI_BOOL bIsProcOn = HI_FALSE;
				JPEG_Get_Proc_Status(&bIsProcOn);
	            if(HI_TRUE == bIsProcOn)
	            {
		            if (0 == Arg)
		            {
		                return HI_FAILURE;
		            }
		                        
		            if(copy_from_user((HI_VOID *)&s_stJpeg6bProcInfo, (HI_VOID *)Arg, sizeof(JPEG_PROC_INFO_S)))
				    {  
		                return -EFAULT;  
		           	}
	            }
				
	            break;
				
	        }   	
	        default:
	        {
	            return -EINVAL;
	        }
	        
	    }
		
	    return HI_SUCCESS;

    
}



/** 这两个函数要按此命名 **/
#ifdef MODULE
module_init(JPEG_DRV_ModInit);
module_exit(JPEG_DRV_ModExit);
#endif

MODULE_DESCRIPTION("driver for the all jpeg");
MODULE_AUTHOR("HISILICON");
MODULE_LICENSE("GPL");


#ifdef __cplusplus
    #if __cplusplus
}
    #endif  /* __cplusplus */
#endif  /* __cplusplus */

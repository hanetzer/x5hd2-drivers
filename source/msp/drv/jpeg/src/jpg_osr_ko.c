/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon. Co., Ltd.

******************************************************************************
File Name	    : jpg_osr_ko.c
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

#include "jpgdrv_common.h"
#include "hi_jpg_ioctl.h"
#include "jpg_hal.h"
#include "jpg_driver.h"
#include "hi_jpg_errcode.h"
#include "hijpeg_type.h"
#include "drv_module_ext.h"

#include "hi_jpegdrv_common_k.h"

/** ko **/
#include "drv_dev_ext.h"
#include "hi_jpeg_config.h"

#ifndef HIJPEG_GAO_AN_VERSION
#include "hijpeg_proc.h"
#endif


#ifdef __cplusplus
    #if __cplusplus
    extern "C"{
    #endif  /* __cplusplus */
#endif  /* __cplusplus */



/** used the msp_based export function **/
extern HI_S32  JPEG_DRV_Init (HI_VOID);
extern HI_VOID JPEG_DRV_Exit (HI_VOID);

extern HI_VOID HI_Jpeg_IfDrvIni(HI_BOOL *bDrvIni);
extern HI_S32 HI_Jpeg_GetIntStatus(HI_SIZE_T Arg);
extern HI_S32 HI_Jpeg_GetDevice(HI_VOID);
extern HI_S32 HI_Jpeg_ReleaseDevice(HI_VOID);
extern HI_VOID HI_Jpeg_GetJpegOsrDev(JPEG_OSRDEV_S **pstruJpegOsrDev);



/**
 ** when you load driver, it can display vertion
 **/
#define MKSTR(exp) # exp
#define MKMARCOTOSTR(exp) MKSTR(exp)


extern JPEG_PROC_INFO_S s_stJpeg6bProcInfo;

static HI_S32 jpg_osr_open(struct inode *inode, struct file *file);
static HI_S32 jpg_osr_close( struct inode *inode, struct file *file);
static HI_S32 jpg_osr_ioctl(struct inode *inode, struct file *file, 
                         HI_U32 Cmd, unsigned long Arg);
static int jpg_osr_mmap(struct file * filp, struct vm_area_struct *vma);


#ifdef HI_MCE_SUPPORT

static HI_BOOL g_bOPenFirstTime = HI_TRUE;
extern HI_S32 HI_ANI_PIC_Stop(HI_U32 uLayerID);

#endif

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


//#ifndef HI_MCE_SUPPORT
/** 不管是否使用开机动画 **/
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
 ** jpeg device imformation of open and close
 **/
typedef struct hiJPG_DISPOSE_CLOSE_S
{

     HI_S32 s32SuspendClose;
     HI_S32 s32DecClose;
     HI_BOOL bOpenUp;
     HI_BOOL bSuspendUp;
     HI_BOOL bRealse;

}JPG_DISPOSE_CLOSE_S;



/*********************************************************************************/
/**
 **   to be compiled into ko
 **/
 
static JPEG_OSRDEV_S *gs_pstruJpegOsrDev = HI_NULL;

#ifndef HIJPEG_GAO_AN_VERSION
/*****************************************************************************
* func            : jpg_osr_init
* description     : exit initial the device
* param[in]       : none
* output          : none
* retval          :HI_SUCCESS
* retval          : -ENOMEM
* retval          : -EFAULT
* retval          : -EINVAL
* others:	      : nothing
*****************************************************************************/
static HI_VOID jpeg6b_version(HI_VOID)
{
    HI_CHAR JPEG6BVersion[160] ="SDK_VERSION:["MKMARCOTOSTR(jpeg6bv1.0)"] Build Time:["\
        __DATE__", "__TIME__"]";
    HIJPEG_TRACE("Load jpeg6b.ko success.\t\t(%s)\n", JPEG6BVersion);

    return;
}
#endif

#define JPEGNAME "HI_JPEG"

HI_VOID JPEG_DRV_ModExit(HI_VOID);

 HI_S32 JPEG_DRV_ModInit(HI_VOID)
{
                      
       HI_S32 RetVal = 0;
       HI_BOOL bIfDrvIni = HI_FALSE;
       HI_Jpeg_IfDrvIni(&bIfDrvIni);
       #ifndef HI_MCE_SUPPORT 
       if(HI_FALSE == bIfDrvIni)
       {/** this is public between ko and kernel**/
        	RetVal = JPEG_DRV_Init();   
            if(HI_SUCCESS != RetVal){
                return HI_FAILURE;
            }
        }
       #endif

    	/**************************************************/
        HI_Jpeg_GetJpegOsrDev(&gs_pstruJpegOsrDev);

	    #ifndef HIJPEG_GAO_AN_VERSION
        jpeg6b_version();
		#endif
		
        /**
         ** if not logo initial, should register device
         **/
        //#ifndef HI_MCE_SUPPORT
        /** 不管是否使用开机动画 **/
        #ifndef USE_HIMEDIA_DEVICE
        RetVal = misc_register(&jpeg_dev);
        #else
    	RetVal = HI_DRV_DEV_Register(&jpeg_dev);
        #endif
        if (HI_SUCCESS != RetVal)
        { 
			JPEG_KFREE(HI_ID_JPGDEC, (HI_VOID *)gs_pstruJpegOsrDev);
            gs_pstruJpegOsrDev = HI_NULL;
            HIJPEG_TRACE("========= register failure =========\n");
            return HI_FAILURE;
            
        }


		#ifndef HIJPEG_GAO_AN_VERSION
        /** 
         ** initial proc message
         **/
         JPEG_Proc_init();
        #endif
		HI_S32 Ret;
		Ret = HI_DRV_MODULE_Register(HI_ID_JPGDEC, JPEGNAME, NULL); 
        if (HI_SUCCESS != Ret)
        {
	        HIJPEG_TRACE("HI_DRV_MODULE_Register JPEG failed\n");
            JPEG_DRV_ModExit();
	        return HI_FAILURE;
        }
		
        return HI_SUCCESS;
    
}

 /*****************************************************************************
* func            : jpg_osr_exit
* description     : exit initial the device
* param[in]       : none
* output          : none
* retval          : none
* others:	      : nothing
*****************************************************************************/

HI_VOID JPEG_DRV_ModExit(HI_VOID)
{

 
        #ifndef HI_MCE_SUPPORT
          JPEG_DRV_Exit();   
        #endif
		HI_DRV_MODULE_UnRegister(HI_ID_JPGDEC);
        /**
	     ** uninstall the device 
	     **/
	    //#ifndef HI_MCE_SUPPORT
        /** 不管是否使用开机动画 **/
        #ifndef USE_HIMEDIA_DEVICE
        misc_deregister(&jpeg_dev);
        #else
    	HI_DRV_DEV_UnRegister(&jpeg_dev);
        #endif

        #ifdef HI_MCE_SUPPORT
        if(g_bOPenFirstTime == HI_FALSE){
            g_bOPenFirstTime = HI_TRUE;
        }
        #endif

		#ifndef HIJPEG_GAO_AN_VERSION
         /**
    	  **clean up the proc
    	  **/
        JPEG_Proc_Cleanup();
        #endif
		
        return;
            
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
	sDisposeClose = (JPG_DISPOSE_CLOSE_S *) JPEG_KMALLOC(HI_ID_JPGDEC,               \
		                                                 sizeof(JPG_DISPOSE_CLOSE_S),\
		                                                 GFP_KERNEL);
    if ( HI_NULL == sDisposeClose )
    {
         return -ENOMEM;
    }
    memset(sDisposeClose, 0x0, sizeof(JPG_DISPOSE_CLOSE_S));
    file->private_data = sDisposeClose;
    sDisposeClose->s32DecClose = HI_SUCCESS;
    sDisposeClose->s32SuspendClose = HI_FAILURE;
    sDisposeClose->bOpenUp = HI_FALSE;
    sDisposeClose->bSuspendUp = HI_FALSE;
    sDisposeClose->bRealse    = HI_FALSE;

    #ifdef HI_MCE_SUPPORT
    if(g_bOPenFirstTime == HI_TRUE)
    {

        g_bOPenFirstTime = HI_FALSE;
	    HI_ANI_PIC_Stop(2);
    }
    #endif
    
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

        volatile HI_U32* pResetAddr = NULL;
        JPG_DISPOSE_CLOSE_S *sDisposeClose = NULL;
        sDisposeClose = file->private_data;
        if(NULL == sDisposeClose)
        {  

           HIJPEG_TRACE("== the address of sDisposeClose is NULL==\n");
           return HI_FAILURE;
        }

        /**
         ** if suspend , do close device only
         **/
        if(HI_SUCCESS==sDisposeClose->s32SuspendClose){
             if(HI_TRUE == sDisposeClose->bSuspendUp){
                HI_Jpeg_ReleaseDevice();
             }
			 JPEG_KFREE(HI_ID_JPGDEC, (HI_VOID *)sDisposeClose);
             return HI_SUCCESS;
        }
        if(HI_SUCCESS==sDisposeClose->s32DecClose){
             if(HI_TRUE == sDisposeClose->bOpenUp){
                HI_Jpeg_ReleaseDevice();
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
            if (HI_NULL == gs_pstruJpegOsrDev)
            {
                HI_Jpeg_ReleaseDevice();
                return HI_FAILURE;
            }
            /**
             ** if the file occupy the device, set this device to not occupied,
             ** wake up waiting halt waiting queue
             **/
            if(down_interruptible(&gs_pstruJpegOsrDev->SemGetDev));

            if ((HI_TRUE == gs_pstruJpegOsrDev->EngageFlag) && (file == gs_pstruJpegOsrDev->pFile))
            {
            
                gs_pstruJpegOsrDev->EngageFlag = HI_FALSE;
                (HI_VOID)wake_up_interruptible(&gs_pstruJpegOsrDev->QWaitInt);
                
            }
            /**
             ** to JPG reset operation, open the clock
             **/
            if(gs_pstruJpegOsrDev->EngageFlag != HI_FALSE){
                up(&gs_pstruJpegOsrDev->SemGetDev);
                HI_Jpeg_ReleaseDevice();
        		return HI_FAILURE;
        	}
            if(gs_pstruJpegOsrDev->IntType != JPG_INTTYPE_NONE){
                up(&gs_pstruJpegOsrDev->SemGetDev);
                HI_Jpeg_ReleaseDevice();
        		return HI_FAILURE;
            }
            
        	pResetAddr = (volatile HI_U32*) IO_ADDRESS(JPG_CTL_REG_PHYADDR);
            *pResetAddr &= (~JPG_RESET_REG_VALUE);
            *pResetAddr &=(~JPG_CLOCK_ON);
            
            HI_Jpeg_ReleaseDevice();
            
            up(&gs_pstruJpegOsrDev->SemGetDev);
            
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



        HI_U32 Phys;

        /**
         ** set map parameter 
         **/
        Phys = (X5_JPG_REG_PHYADDR >> PAGE_SHIFT);
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
static HI_S32 jpg_osr_ioctl(struct file *file,  HI_U32 Cmd, unsigned long Arg)
{
 
    HI_S32 RetVal = 0;
    switch(Cmd)
    {

        case CMD_JPG_GETDEVICE:
        {

        	volatile HI_U32* pResetAddr = NULL;
        	volatile HI_U32* pBusyAddr = NULL;

            JPG_DISPOSE_CLOSE_S *sDisposeClose = NULL;
            sDisposeClose = file->private_data;
            
        	/********if jpeg has not close, so jpeg is busy, you should suspend now **/
        	RetVal = HI_Jpeg_GetDevice();
        	if(HI_FAILURE == RetVal){ /** Mutex initial with 1, and after this func,the
        	                             ** Mutex is zero, so has not mutex, next time should
        	                             ** wait here, only the mutex is no zero, followed can
        	                             ** operation
        	                             **/
        	      sDisposeClose->bOpenUp = HI_FALSE;
        	      HIJPEG_TRACE("down interruptible failure!\n"); 
                  return HI_FAILURE;
            }
        	/*************************************************************************/
        			
            /**
             ** if has not initial device, return failure
             **/
            if (HI_NULL == gs_pstruJpegOsrDev)
            {   
                return HI_FAILURE;
            }

            /**
             ** locked the occupied device 
             **/
            if(down_interruptible(&gs_pstruJpegOsrDev->SemGetDev));

            gs_pstruJpegOsrDev->EngageFlag = HI_TRUE;
            gs_pstruJpegOsrDev->IntType = JPG_INTTYPE_NONE;
            gs_pstruJpegOsrDev->pFile = file;
            
            sDisposeClose->s32DecClose = HI_FAILURE;
            sDisposeClose->bOpenUp = HI_TRUE;
            sDisposeClose->bRealse = HI_FALSE;
            /**
             ** to JPG reset operation, open the clock
             **/
            pResetAddr = (volatile HI_U32*) IO_ADDRESS(JPG_CTL_REG_PHYADDR);
            /**
             ** the jpeg register based address
             **/
            pBusyAddr = (volatile HI_U32*) IO_ADDRESS(X5_JPG_REG_PHYADDR);
            *pResetAddr |= JPG_CLOCK_ON;
        	*pResetAddr |= JPG_RESET_REG_VALUE;
            while (*pBusyAddr & 0x2);
            *pResetAddr &= (~JPG_RESET_REG_VALUE);

             up(&gs_pstruJpegOsrDev->SemGetDev);
            
            break;
            
        }
        case CMD_JPG_RELEASEDEVICE:
        {

            volatile HI_U32* pResetAddr = NULL;
            JPG_DISPOSE_CLOSE_S *sDisposeClose = NULL;
            sDisposeClose = file->private_data;
            /**
             **if device has not initial, return failure
             **/
            if (HI_NULL == gs_pstruJpegOsrDev)
            {
                HI_Jpeg_ReleaseDevice();
                return HI_FAILURE;
            }
            /**
             ** if the file occupy the device, set this device to not occupied,
             ** wake up waiting halt waiting queue
             **/
            if(down_interruptible(&gs_pstruJpegOsrDev->SemGetDev));

            if ((HI_TRUE == gs_pstruJpegOsrDev->EngageFlag) && (file == gs_pstruJpegOsrDev->pFile))
            {
            
                gs_pstruJpegOsrDev->EngageFlag = HI_FALSE;
                (HI_VOID)wake_up_interruptible(&gs_pstruJpegOsrDev->QWaitInt);
                
            }
            
            /**
             ** to JPG reset operation, open the clock
             **/
            if(gs_pstruJpegOsrDev->EngageFlag != HI_FALSE){
                up(&gs_pstruJpegOsrDev->SemGetDev);
                HI_Jpeg_ReleaseDevice();
        		return HI_FAILURE;
        	}
            if(gs_pstruJpegOsrDev->IntType != JPG_INTTYPE_NONE){
                up(&gs_pstruJpegOsrDev->SemGetDev);
                HI_Jpeg_ReleaseDevice();
        		return HI_FAILURE;
            }
            
        	pResetAddr = (volatile HI_U32*) IO_ADDRESS(JPG_CTL_REG_PHYADDR);
            *pResetAddr &= (~JPG_RESET_REG_VALUE);
            *pResetAddr &=(~JPG_CLOCK_ON);
            
            HI_Jpeg_ReleaseDevice();
            sDisposeClose->bRealse = HI_TRUE;

            up(&gs_pstruJpegOsrDev->SemGetDev);
                     
            break;
            
        }
        case CMD_JPG_SUSPEND:
        {    
             JPG_DISPOSE_CLOSE_S *sDisposeClose = NULL;
             sDisposeClose = file->private_data;
             sDisposeClose->s32SuspendClose = HI_SUCCESS;
             RetVal = HI_Jpeg_GetDevice();
             if(HI_FAILURE == RetVal){
                  sDisposeClose->bSuspendUp = HI_FALSE;
                  return HI_FAILURE;
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
             sDisposeClose = file->private_data;
             sDisposeClose->s32SuspendClose = HI_SUCCESS;
             sDisposeClose->bSuspendUp = HI_FALSE;
             HI_Jpeg_ReleaseDevice();
             break;
        }
        case CMD_JPG_GETINTSTATUS:
        {
            
            RetVal = HI_Jpeg_GetIntStatus(Arg);
            if(HI_SUCCESS != RetVal){
                return HI_FAILURE;
            }
            break;
        }
        case CMD_JPG_READPROC:
        {   

			#ifndef HIJPEG_GAO_AN_VERSION
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

	                HIJPEG_TRACE("=========  copy from user failure =============\n");
	                return -EFAULT;  
	           	}
	            
            }
			#endif
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

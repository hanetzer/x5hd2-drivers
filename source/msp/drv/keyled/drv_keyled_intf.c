/*************************************************************************
 * Copyright 2004 - 2050, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: keyled_intf.c
 * Description:
 *
 * History:
 * Version   Date             Author       DefectNum    Description
 * main\1    2007-10-16    jianglei 40671    NULL       Create this file.
 ***************************************************************************/
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/kthread.h>
#include <asm/unistd.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/fcntl.h>
#include <linux/mm.h>
#include <linux/miscdevice.h>
#include <linux/proc_fs.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <asm/system.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/string.h>
#include <mach/hardware.h>
#include <asm/signal.h>
#include <linux/time.h>
#include <linux/unistd.h>
#include <linux/delay.h>
#include <linux/mm.h>
#include <linux/poll.h>

#include "hi_common.h"
#include "hi_type.h"
#include "drv_struct_ext.h"
#include "drv_dev_ext.h"
#include "drv_proc_ext.h"

#include "hi_unf_keyled.h"
#include "hi_error_mpi.h"
#include "hi_drv_keyled.h"
#include "drv_keyled_ioctl.h"
#include "drv_keyled.h"

#ifdef KEYLED_STANDARD
#include "drv_keyled_std.h"
#endif

#ifdef KEYLED_PT6961
#include "drv_keyled_pt6961.h"
#endif

#ifdef KEYLED_CT1642
#include "drv_keyled_ct1642.h"
#endif

#ifdef KEYLED_PT6964
#include "drv_keyled_pt6964.h"
#endif

#ifdef KEYLED_FD650
#include "drv_keyled_fd650.h"
#endif

#include "drv_module_ext.h"

/*-------------------------------------------------------------------
 * macro define section
 *-----------------------------------------------------------------*/

/*-------------------------------------------------------------------
 * date structure define section
 *-----------------------------------------------------------------*/

/*-------------------------------------------------------------------
 * variable define section
 *-----------------------------------------------------------------*/
static KeyLed_Status_E keyledflag = KeyOFF_LedOFF;
static KEYLED_OPT_S g_stKeyLedOpt;

static HI_BOOL s_bInitialized = HI_FALSE;

#ifdef  KEYLED_STANDARD
static HI_CHIP_TYPE_E enChipType;
static HI_CHIP_VERSION_E enChipVersion;
#endif

/*-------------------------------------------------------------------
 * array define section
 *-----------------------------------------------------------------*/

/*-------------------------------------------------------------------
 * function define section
 *-----------------------------------------------------------------*/

/* register tuner standard interface function */
#ifdef KEYLED_CT1642
static HI_VOID KEYLEDSelectCT1642(HI_VOID)
{
    HI_INFO_KEYLED("select ct1642 keyled\n");
    g_stKeyLedOpt.KEYLED_KEY_Open  = KEYLED_KEY_Open_CT1642;
    g_stKeyLedOpt.KEYLED_KEY_Close = KEYLED_KEY_Close_CT1642;
    g_stKeyLedOpt.KEYLED_KEY_Reset = KEYLED_KEY_Reset_CT1642;
    g_stKeyLedOpt.KEYLED_KEY_GetValue = KEYLED_KEY_GetValue_CT1642;
    g_stKeyLedOpt.KEYLED_KEY_SetBlockTime = KEYLED_KEY_SetBlockTime_CT1642;
    g_stKeyLedOpt.KEYLED_KEY_SetRepTime = KEYLED_KEY_SetRepTime_CT1642;
    g_stKeyLedOpt.KEYLED_KEY_IsRepKey = KEYLED_KEY_IsRepKey_CT1642;
    g_stKeyLedOpt.KEYLED_KEY_IsKeyUp  = KEYLED_KEY_IsKeyUp_CT1642;
    g_stKeyLedOpt.KEYLED_LED_Open  = KEYLED_LED_Open_CT1642;
    g_stKeyLedOpt.KEYLED_LED_Close = KEYLED_LED_Close_CT1642;
    g_stKeyLedOpt.KEYLED_LED_Display = KEYLED_LED_Display_CT1642;
    g_stKeyLedOpt.KEYLED_LED_DisplayTime  = KEYLED_LED_DisplayTime_CT1642;
    g_stKeyLedOpt.KEYLED_LED_SetFlashPin  = KEYLED_LED_SetFlashPin_CT1642;
    g_stKeyLedOpt.KEYLED_LED_SetFlashFreq = KEYLED_LED_SetFlashFreq_CT1642;
    g_stKeyLedOpt.KEYLED_LED_DisplayLED = HI_NULL;
    g_stKeyLedOpt.KEYLED_SetMode = HI_NULL;

    g_stKeyLedOpt.KEYLED_Suspend = KEYLED_Suspend_CT1642;
    g_stKeyLedOpt.KEYLED_Resume	 = KEYLED_Resume_CT1642;

	s_bInitialized = HI_TRUE;
	
    return;
}
#endif

static HI_S32 KEYLEDSelectType(HI_UNF_KEYLED_TYPE_E enKeyLedType)
{
    if (enKeyLedType == HI_UNF_KEYLED_TYPE_STD)
    {
#ifdef KEYLED_STANDARD
        HI_INFO_KEYLED("select std keyled\n");

        g_stKeyLedOpt.KEYLED_KEY_Open  = KEYLED_KEY_Open;
        g_stKeyLedOpt.KEYLED_KEY_Close = KEYLED_KEY_Close;
        g_stKeyLedOpt.KEYLED_KEY_Reset = KEYLED_KEY_Reset;
        g_stKeyLedOpt.KEYLED_KEY_GetValue = KEYLED_KEY_GetValue;
        g_stKeyLedOpt.KEYLED_KEY_SetBlockTime = KEYLED_KEY_SetBlockTime;
        g_stKeyLedOpt.KEYLED_KEY_SetRepTime = KEYLED_KEY_SetRepTime;
        g_stKeyLedOpt.KEYLED_KEY_IsRepKey = KEYLED_KEY_IsRepKey;
        g_stKeyLedOpt.KEYLED_KEY_IsKeyUp  = KEYLED_KEY_IsKeyUp;

        g_stKeyLedOpt.KEYLED_LED_Open  = KEYLED_LED_Open;
        g_stKeyLedOpt.KEYLED_LED_Close = KEYLED_LED_Close;
        g_stKeyLedOpt.KEYLED_LED_Display = KEYLED_LED_Display;
        g_stKeyLedOpt.KEYLED_LED_DisplayTime  = KEYLED_LED_DisplayTime;
        g_stKeyLedOpt.KEYLED_LED_SetFlashPin  = KEYLED_LED_SetFlashPin;
        g_stKeyLedOpt.KEYLED_LED_SetFlashFreq = KEYLED_LED_SetFlashFreq;
        g_stKeyLedOpt.KEYLED_LED_DisplayLED = KEYLED_LED_DisplayLED;
        g_stKeyLedOpt.KEYLED_SetMode = KEYLED_SetMode;

        g_stKeyLedOpt.KEYLED_Suspend = KEYLED_Suspend;
        g_stKeyLedOpt.KEYLED_Resume	 = KEYLED_Resume;

		s_bInitialized = HI_TRUE;
#endif
    }
    else if (enKeyLedType == HI_UNF_KEYLED_TYPE_CT1642)
    {
#ifdef KEYLED_CT1642
        KEYLEDSelectCT1642();
#endif

    }
    else if (enKeyLedType == HI_UNF_KEYLED_TYPE_PT6961)
    {
#ifdef KEYLED_PT6961
        HI_INFO_KEYLED("select pt6961 keyled\n");
        g_stKeyLedOpt.KEYLED_KEY_Open  = KEYLED_KEY_Open_PT6961;
        g_stKeyLedOpt.KEYLED_KEY_Close = KEYLED_KEY_Close_PT6961;
        g_stKeyLedOpt.KEYLED_KEY_Reset = KEYLED_KEY_Reset_PT6961;
        g_stKeyLedOpt.KEYLED_KEY_GetValue = KEYLED_KEY_GetValue_PT6961;
        g_stKeyLedOpt.KEYLED_KEY_SetBlockTime = KEYLED_KEY_SetBlockTime_PT6961;
        g_stKeyLedOpt.KEYLED_KEY_SetRepTime = KEYLED_KEY_SetRepTime_PT6961;
        g_stKeyLedOpt.KEYLED_KEY_IsRepKey = KEYLED_KEY_IsRepKey_PT6961;
        g_stKeyLedOpt.KEYLED_KEY_IsKeyUp  = KEYLED_KEY_IsKeyUp_PT6961;

        g_stKeyLedOpt.KEYLED_LED_Open  = KEYLED_LED_Open_PT6961;
        g_stKeyLedOpt.KEYLED_LED_Close = KEYLED_LED_Close_PT6961;
        g_stKeyLedOpt.KEYLED_LED_Display = KEYLED_LED_Display_PT6961;
        g_stKeyLedOpt.KEYLED_LED_DisplayTime  = KEYLED_LED_DisplayTime_PT6961;
        g_stKeyLedOpt.KEYLED_LED_SetFlashPin  = KEYLED_LED_SetFlashPin_PT6961;
        g_stKeyLedOpt.KEYLED_LED_SetFlashFreq = KEYLED_LED_SetFlashFreq_PT6961;
        g_stKeyLedOpt.KEYLED_LED_DisplayLED = HI_NULL;
        g_stKeyLedOpt.KEYLED_SetMode = HI_NULL;

        g_stKeyLedOpt.KEYLED_Suspend = KEYLED_Suspend_PT6961;
        g_stKeyLedOpt.KEYLED_Resume	 = KEYLED_Resume_PT6961;

		s_bInitialized = HI_TRUE;
#endif
    }
    else if (enKeyLedType == HI_UNF_KEYLED_TYPE_PT6964)
    {
#ifdef KEYLED_PT6964
        HI_INFO_KEYLED("select pt6964 keyled\n");

        g_stKeyLedOpt.KEYLED_KEY_Open  = KEYLED_KEY_Open_PT6964;
        g_stKeyLedOpt.KEYLED_KEY_Close = KEYLED_KEY_Close_PT6964;
        g_stKeyLedOpt.KEYLED_KEY_Reset = KEYLED_KEY_Reset_PT6964;
        g_stKeyLedOpt.KEYLED_KEY_GetValue = KEYLED_KEY_GetValue_PT6964;
        g_stKeyLedOpt.KEYLED_KEY_SetBlockTime = KEYLED_KEY_SetBlockTime_PT6964;
        g_stKeyLedOpt.KEYLED_KEY_SetRepTime = KEYLED_KEY_SetRepTime_PT6964;
        g_stKeyLedOpt.KEYLED_KEY_IsRepKey = KEYLED_KEY_IsRepKey_PT6964;
        g_stKeyLedOpt.KEYLED_KEY_IsKeyUp  = KEYLED_KEY_IsKeyUp_PT6964;

        g_stKeyLedOpt.KEYLED_LED_Open  = KEYLED_LED_Open_PT6964;
        g_stKeyLedOpt.KEYLED_LED_Close = KEYLED_LED_Close_PT6964;
        g_stKeyLedOpt.KEYLED_LED_Display = KEYLED_LED_Display_PT6964;
        g_stKeyLedOpt.KEYLED_LED_DisplayTime  = KEYLED_LED_DisplayTime_PT6964;
        g_stKeyLedOpt.KEYLED_LED_SetFlashPin  = KEYLED_LED_SetFlashPin_PT6964;
        g_stKeyLedOpt.KEYLED_LED_SetFlashFreq = KEYLED_LED_SetFlashFreq_PT6964;
        g_stKeyLedOpt.KEYLED_LED_DisplayLED = HI_NULL;
        g_stKeyLedOpt.KEYLED_SetMode = HI_NULL;

        g_stKeyLedOpt.KEYLED_Suspend = KEYLED_Suspend_PT6964;
        g_stKeyLedOpt.KEYLED_Resume	 = KEYLED_Resume_PT6964;

		s_bInitialized = HI_TRUE;
#endif
    }
	else if (enKeyLedType == HI_UNF_KEYLED_TYPE_FD650)
	{
#ifdef KEYLED_FD650
		HI_INFO_KEYLED("select fd650 keyled\n");

        g_stKeyLedOpt.KEYLED_KEY_Open  = KEYLED_KEY_Open_FD650;
        g_stKeyLedOpt.KEYLED_KEY_Close = KEYLED_KEY_Close_FD650;
        g_stKeyLedOpt.KEYLED_KEY_Reset = KEYLED_KEY_Reset_FD650;
        g_stKeyLedOpt.KEYLED_KEY_GetValue = KEYLED_KEY_GetValue_FD650;
        g_stKeyLedOpt.KEYLED_KEY_SetBlockTime = KEYLED_KEY_SetBlockTime_FD650;
        g_stKeyLedOpt.KEYLED_KEY_SetRepTime = KEYLED_KEY_SetRepTime_FD650;
        g_stKeyLedOpt.KEYLED_KEY_IsRepKey = KEYLED_KEY_IsRepKey_FD650;
        g_stKeyLedOpt.KEYLED_KEY_IsKeyUp  = KEYLED_KEY_IsKeyUp_FD650;

        g_stKeyLedOpt.KEYLED_LED_Open  = KEYLED_LED_Open_FD650;
        g_stKeyLedOpt.KEYLED_LED_Close = KEYLED_LED_Close_FD650;
        g_stKeyLedOpt.KEYLED_LED_Display = KEYLED_LED_Display_FD650;
        g_stKeyLedOpt.KEYLED_LED_DisplayTime  = KEYLED_LED_DisplayTime_FD650;
        g_stKeyLedOpt.KEYLED_LED_SetFlashPin  = KEYLED_LED_SetFlashPin_FD650;
        g_stKeyLedOpt.KEYLED_LED_SetFlashFreq = KEYLED_LED_SetFlashFreq_FD650;
		g_stKeyLedOpt.KEYLED_LED_SetLockIndicator = KEYLED_LED_SetLockIndicator_FD650;
        g_stKeyLedOpt.KEYLED_LED_DisplayLED = HI_NULL;
        g_stKeyLedOpt.KEYLED_SetMode = HI_NULL;

        g_stKeyLedOpt.KEYLED_Suspend = KEYLED_Suspend_FD650;
        g_stKeyLedOpt.KEYLED_Resume	 = KEYLED_Resume_FD650;

		s_bInitialized = HI_TRUE;
#endif
	}
    else
    {
        HI_ERR_KEYLED("This version doesn't support this type(%d) keyled \n", enKeyLedType);
    }

/* If set ct1642 in kernel, force the functions to ct1642 */
#ifdef HI_KEYLED_CT1642_KERNEL_SUPPORT    
	KEYLEDSelectCT1642();
#endif

	if (HI_FALSE == s_bInitialized)
	{
		return HI_FAILURE;		
	}

    return HI_SUCCESS;
}

static HI_S32 KEYLEDIoctl(struct inode *inode, struct file * file, HI_U32 cmd, HI_VOID * arg)
{
    HI_S32 ret;
    GET_KEY_VALUE_S *pstPara;

    if (HI_ID_KEYLED != _IOC_TYPE(cmd))
    {
        return -ENOTTY;
    }

    switch (cmd)
    {
    case HI_KEYLED_SELECT_CMD:
        return KEYLEDSelectType(*(HI_UNF_KEYLED_TYPE_E*)arg);

    case HI_KEYLED_KEY_OPEN_CMD:

        /* deal with it temporary , do something more if it is needed later*/
        /*CNcomment: only one, 临时处理，如果严谨的话需要细化*/
        if (keyledflag & KeyON)
        {
            return HI_SUCCESS;
        }
        else
        {
            ret = g_stKeyLedOpt.KEYLED_KEY_Open();
            if (ret == HI_SUCCESS)
            {
                keyledflag |= KeyON;
            }

#ifdef  KEYLED_STANDARD
            KEYLED_GetChipType(&enChipType, &enChipVersion);
#endif
            return ret;
        }

    case HI_KEYLED_KEY_CLOSE_CMD:

        /* deal with it temporary , do something more if it is needed later*/
        /*CNcomment: only one, 临时处理，如果严谨的话需要细化*/
        if (keyledflag & KeyON)
        {
            keyledflag &= (~KeyON);
            return g_stKeyLedOpt.KEYLED_KEY_Close();
        }
        else
        {
            return HI_SUCCESS;
        }

    case HI_KEYLED_KEY_RESET_CMD:
        if (keyledflag & KeyON)
        {
            return g_stKeyLedOpt.KEYLED_KEY_Reset();
        }
        else
        {
            HI_WARN_KEYLED("Key not open\n");
            return HI_FAILURE;
        }

    case HI_KEYLED_KEY_GET_VALUE_CMD:
        if (keyledflag & KeyON)
        {
            pstPara = (GET_KEY_VALUE_S*)arg;
            return g_stKeyLedOpt.KEYLED_KEY_GetValue(&pstPara->u32KeyStatus, &pstPara->u32KeyCode);
        }
        else
        {
            HI_WARN_KEYLED("Key not open\n");
            return HI_FAILURE;
        }

    case HI_KEYLED_SET_BLOCK_TIME_CMD:
        if (keyledflag & KeyON)
        {
            return g_stKeyLedOpt.KEYLED_KEY_SetBlockTime(*(HI_U32*)arg);
        }
        else
        {
            HI_WARN_KEYLED("Key not open\n");
            return HI_FAILURE;
        }

    case HI_KEYLED_SET_IS_KEYUP_CMD:
        if (keyledflag & KeyON)
        {
            return g_stKeyLedOpt.KEYLED_KEY_IsKeyUp(*(HI_U32*)arg);
        }
        else
        {
            HI_WARN_KEYLED("Key not open\n");
            return HI_FAILURE;
        }

    case HI_KEYLED_SET_IS_REPKEY_CMD:
        if (keyledflag & KeyON)
        {
            return g_stKeyLedOpt.KEYLED_KEY_IsRepKey(*(HI_U32*)arg);
        }
        else
        {
            HI_WARN_KEYLED("Key not open\n");
            return HI_FAILURE;
        }

    case HI_KEYLED_SET_REPKEY_TIME_CMD:
        if (keyledflag & KeyON)
        {
            return g_stKeyLedOpt.KEYLED_KEY_SetRepTime(*(HI_U32*)arg);
        }
        else
        {
            HI_WARN_KEYLED("Key not open\n");
            return HI_FAILURE;
        }

    case HI_KEYLED_LED_OPEN_CMD:

        /* deal with it temporary , do something more if it is needed later*/
        /*CNcomment: only one, 临时处理，如果严谨的话需要细化*/
        if (keyledflag & LedON)
        {
            return HI_SUCCESS;
        }
        else
        {
            ret = g_stKeyLedOpt.KEYLED_LED_Open();
            if (ret == HI_SUCCESS)
            {
                keyledflag |= LedON;
            }

#ifdef  KEYLED_STANDARD
            KEYLED_GetChipType(&enChipType, &enChipVersion);
#endif
            return ret;
        }

    case HI_KEYLED_LED_CLOSE_CMD:

        /* deal with it temporary , do something more if it is needed later*/
        /*CNcomment: only one, 临时处理，如果严谨的话需要细化*/
        if (keyledflag & LedON)
        {
            keyledflag &= (~LedON);
            return g_stKeyLedOpt.KEYLED_LED_Close();
        }
        else
        {
            return HI_SUCCESS;
        }

    case HI_KEYLED_DISPLAY_CODE_CMD:
        if (keyledflag & LedON)
        {
            return g_stKeyLedOpt.KEYLED_LED_Display(*(HI_U32*)arg);
        }
        else
        {
            HI_WARN_KEYLED("Led not open\n");
            return HI_FAILURE;
        }

    case HI_KEYLED_DISPLAY_TIME_CMD:
        if (keyledflag & LedON)
        {
            return g_stKeyLedOpt.KEYLED_LED_DisplayTime(*(HI_UNF_KEYLED_TIME_S*)arg);
        }
        else
        {
            HI_WARN_KEYLED("Led not open\n");
            return HI_FAILURE;
        }

    case HI_KEYLED_SET_FLASH_PIN_CMD:
        if (keyledflag & LedON)
        {
            return g_stKeyLedOpt.KEYLED_LED_SetFlashPin(*(HI_UNF_KEYLED_LIGHT_E*)arg);
        }
        else
        {
            HI_WARN_KEYLED("Led not open\n");
            return HI_FAILURE;
        }

    case HI_KEYLED_CONFIG_FLASH_FREQ_CMD:
        if (keyledflag & LedON)
        {
            return g_stKeyLedOpt.KEYLED_LED_SetFlashFreq(*(HI_UNF_KEYLED_LEVEL_E*)arg);
        }
        else
        {
            HI_WARN_KEYLED("Led not open\n");
            return HI_FAILURE;
        }

#ifdef  KEYLED_STANDARD
    case HI_KEYLED_DISPLAY_MULITLED_CMD:
        if (HI_NULL != g_stKeyLedOpt.KEYLED_LED_DisplayLED)
        {
            if (keyledflag & LedON)
            {
                if ((HI_CHIP_TYPE_HI3716M == enChipType) && (HI_CHIP_VERSION_V300 == enChipVersion))
                {
                    return g_stKeyLedOpt.KEYLED_LED_DisplayLED(*(HI_KEYLED_DISPLAY_S *)arg);
                }
                else
                {
                    return HI_SUCCESS;
                }
            }
            else
            {
                HI_ERR_KEYLED("Led not open\n");
                return HI_FAILURE;
            }
        }
        else
        {
            HI_ERR_KEYLED("this set only for standard front panel\n");
            return HI_FAILURE;
        }

    case HI_KEYLED_SET_MODE_CMD:
        if (HI_NULL != g_stKeyLedOpt.KEYLED_SetMode)
        {
            if (keyledflag & KeyON || keyledflag & LedON)
            {
                if ((HI_CHIP_TYPE_HI3716M == enChipType) && (HI_CHIP_VERSION_V300 == enChipVersion))
                {
                    return g_stKeyLedOpt.KEYLED_SetMode(*(HI_KEYLED_MODE_S *)arg);
                }
                else
                {
                    return HI_SUCCESS;
                }
            }
            else
            {
                HI_ERR_KEYLED("The keyled not open\n");
                return HI_FAILURE;
            }
        }
        else
        {
            HI_ERR_KEYLED("this set only for standard front panel\n");
            return HI_FAILURE;
        }
#endif
#if 0
    case HI_KEYLED_SETLOCK_CMD:
		if (keyledflag & LedON)
		{
			HI_ERR_KEYLED("The keyled not open\n");
            return HI_FAILURE;
		}
		
		if (g_stKeyLedOpt.KEYLED_LED_SetLockIndicator)
	        return g_stKeyLedOpt.KEYLED_LED_SetLockIndicator((HI_BOOL)*(HI_U32*)arg);
		else 
		{
			HI_ERR_KEYLED("this set only for fd650 front panel\n");
			return HI_FAILURE;
		}
#endif  
    default:
        return -EFAULT;
    }

    return HI_SUCCESS;
}

#if 0

/*entrance of function controled by the proc file system*/
/*CNcomment:通过proc文件系统进行控制的函数入口*/

HI_S32 KEYLEDDebugCtrl(HI_U32 u32Para1, HI_U32 u32Para2)
{
    HI_INFO_KEYLED("para1=0x%x para2=0x%x\n", u32Para1, u32Para2);

    return HI_SUCCESS;
}

static HI_S32 KEYLEDProcWrite(struct file * file,
                              const char __user * buf, size_t count, loff_t *ppos)
{
    return HI_DRV_PROC_ModuleWrite(file, buf, count, ppos, KEYLEDDebugCtrl);
}

static HI_S32 KEYLEDProcRead(struct seq_file *p, HI_VOID *v)
{
    p += seq_printf(p, "---------Hisilicon keyled Info---------\n");

    return HI_SUCCESS;
}

#endif

static long KEYLED_Ioctl(struct file * file, HI_U32 cmd, unsigned long arg)
{
    long ret;

    ret = HI_DRV_UserCopy(file->f_dentry->d_inode, file, cmd, arg, KEYLEDIoctl);

    return ret;
}

static HI_S32 KEYLED_Open(struct inode *inode, struct file *filp)
{
    return HI_SUCCESS;
}

static HI_S32 KEYLED_Release(struct inode *inode, struct file *filp)
{
    //g_stKeyLedOpt.KEYLED_KEY_Close();
    //g_stKeyLedOpt.KEYLED_LED_Close();

    //keyledflag = 0;

    return HI_SUCCESS;
}

static struct file_operations stKEYLEDOpts =
{
    open :KEYLED_Open,
    release:KEYLED_Release,
	unlocked_ioctl:KEYLED_Ioctl,
};

/********************************************************************/

static int keyled_pm_suspend (PM_BASEDEV_S *pdev, pm_message_t state)
{
    HI_S32 ret;

    if (g_stKeyLedOpt.KEYLED_Suspend)
    {
        ret = g_stKeyLedOpt.KEYLED_Suspend();
        if (ret)
        {
            HI_FATAL_KEYLED("keyled_pm_suspend err \n");
            return HI_FAILURE;
        }
        else
        {
            HI_FATAL_KEYLED("keyled_pm_suspend ok \n");
        }
    }

    return HI_SUCCESS;
}

static int keyled_pm_resume(PM_BASEDEV_S *pdev)
{
    HI_S32 ret;

    if (g_stKeyLedOpt.KEYLED_Resume)
    {
        ret = g_stKeyLedOpt.KEYLED_Resume();
        if (ret)
        {
            HI_FATAL_KEYLED("keyled_pm_resume err \n");
            return HI_FAILURE;
        }
        else
        {
            HI_FATAL_KEYLED("keyled_pm_resume ok \n");
        }
    }

    return HI_SUCCESS;
}

static PM_BASEOPS_S keyled_baseOps = {
    .probe	= NULL,
    .remove = NULL,
    .shutdown = NULL,
    .prepare  = NULL,
    .complete = NULL,
    .suspend  = keyled_pm_suspend,
    .suspend_late = NULL,
    .resume_early = NULL,
    .resume		  = keyled_pm_resume
};

/********************************************************************/

static UMAP_DEVICE_S g_stKeyLedDev;


HI_S32 KEYLED_DRV_ModInit(HI_VOID)
{
    HI_S32 ret;

#if  defined (KEYLED_STANDARD)     \
        || defined (KEYLED_CT1642)  \
        || defined (KEYLED_PT6961)  \
        || defined (KEYLED_PT6964)  \
        || defined (KEYLED_FD650)
#else
    HI_ERR_KEYLED("keyled init failed \n");
    return HI_FAILURE;
#endif

    ret = HI_DRV_MODULE_Register(HI_ID_KEYLED, "HI_KEYLED", HI_NULL);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_KEYLED("keyled init failed ret = 0x%x\n", ret);
        return ret;
    }

    keyledflag = 0;

    sprintf(g_stKeyLedDev.devfs_name, "%s", UMAP_DEVNAME_KEYLED);
    g_stKeyLedDev.fops	 = &stKEYLEDOpts;
    g_stKeyLedDev.minor	 = UMAP_MIN_MINOR_KEYLED;
    g_stKeyLedDev.owner	 = THIS_MODULE;
    g_stKeyLedDev.drvops = &keyled_baseOps;
    if (HI_DRV_DEV_Register(&g_stKeyLedDev) < 0)
    {
        HI_FATAL_KEYLED("Unable to register keyled dev\n");
        return HI_FAILURE;
    }

#if 0
    item = HI_DRV_PROC_AddModule("keyled", NULL, NULL);
    if (!item)
    {
        HI_FATAL_KEYLED("add proc module failed\n");
        return HI_FAILURE;
    }

    item->read	= KEYLEDProcRead;
    item->write = KEYLEDProcWrite;
#endif

#ifdef KEYLED_STANDARD
    HI_INFO_KEYLED("select std keyled\n");

    g_stKeyLedOpt.KEYLED_KEY_Open  = KEYLED_KEY_Open;
    g_stKeyLedOpt.KEYLED_KEY_Close = KEYLED_KEY_Close;
    g_stKeyLedOpt.KEYLED_KEY_Reset = KEYLED_KEY_Reset;
    g_stKeyLedOpt.KEYLED_KEY_GetValue = KEYLED_KEY_GetValue;
    g_stKeyLedOpt.KEYLED_KEY_SetBlockTime = KEYLED_KEY_SetBlockTime;
    g_stKeyLedOpt.KEYLED_KEY_SetRepTime = KEYLED_KEY_SetRepTime;
    g_stKeyLedOpt.KEYLED_KEY_IsRepKey = KEYLED_KEY_IsRepKey;
    g_stKeyLedOpt.KEYLED_KEY_IsKeyUp  = KEYLED_KEY_IsKeyUp;

    g_stKeyLedOpt.KEYLED_LED_Open  = KEYLED_LED_Open;
    g_stKeyLedOpt.KEYLED_LED_Close = KEYLED_LED_Close;
    g_stKeyLedOpt.KEYLED_LED_Display = KEYLED_LED_Display;
    g_stKeyLedOpt.KEYLED_LED_DisplayTime  = KEYLED_LED_DisplayTime;
    g_stKeyLedOpt.KEYLED_LED_SetFlashPin  = KEYLED_LED_SetFlashPin;
    g_stKeyLedOpt.KEYLED_LED_SetFlashFreq = KEYLED_LED_SetFlashFreq;
    g_stKeyLedOpt.KEYLED_LED_DisplayLED = KEYLED_LED_DisplayLED;
    g_stKeyLedOpt.KEYLED_SetMode = KEYLED_SetMode;

    g_stKeyLedOpt.KEYLED_Suspend = KEYLED_Suspend;
    g_stKeyLedOpt.KEYLED_Resume	 = KEYLED_Resume;
#endif

#ifdef KEYLED_CT1642
    HI_INFO_KEYLED("select ct1642 keyled\n");

    g_stKeyLedOpt.KEYLED_KEY_Open  = KEYLED_KEY_Open_CT1642;
    g_stKeyLedOpt.KEYLED_KEY_Close = KEYLED_KEY_Close_CT1642;
    g_stKeyLedOpt.KEYLED_KEY_Reset = KEYLED_KEY_Reset_CT1642;
    g_stKeyLedOpt.KEYLED_KEY_GetValue = KEYLED_KEY_GetValue_CT1642;
    g_stKeyLedOpt.KEYLED_KEY_SetBlockTime = KEYLED_KEY_SetBlockTime_CT1642;
    g_stKeyLedOpt.KEYLED_KEY_SetRepTime = KEYLED_KEY_SetRepTime_CT1642;
    g_stKeyLedOpt.KEYLED_KEY_IsRepKey = KEYLED_KEY_IsRepKey_CT1642;
    g_stKeyLedOpt.KEYLED_KEY_IsKeyUp  = KEYLED_KEY_IsKeyUp_CT1642;
    g_stKeyLedOpt.KEYLED_LED_Open  = KEYLED_LED_Open_CT1642;
    g_stKeyLedOpt.KEYLED_LED_Close = KEYLED_LED_Close_CT1642;
    g_stKeyLedOpt.KEYLED_LED_Display = KEYLED_LED_Display_CT1642;
    g_stKeyLedOpt.KEYLED_LED_DisplayTime  = KEYLED_LED_DisplayTime_CT1642;
    g_stKeyLedOpt.KEYLED_LED_SetFlashPin  = KEYLED_LED_SetFlashPin_CT1642;
    g_stKeyLedOpt.KEYLED_LED_SetFlashFreq = KEYLED_LED_SetFlashFreq_CT1642;
    g_stKeyLedOpt.KEYLED_LED_DisplayLED = HI_NULL;
    g_stKeyLedOpt.KEYLED_SetMode = HI_NULL;

    g_stKeyLedOpt.KEYLED_Suspend = KEYLED_Suspend_CT1642;
    g_stKeyLedOpt.KEYLED_Resume	 = KEYLED_Resume_CT1642;
#endif

#ifdef KEYLED_PT6961
    HI_INFO_KEYLED("select pt6961 keyled\n");

    g_stKeyLedOpt.KEYLED_KEY_Open  = KEYLED_KEY_Open_PT6961;
    g_stKeyLedOpt.KEYLED_KEY_Close = KEYLED_KEY_Close_PT6961;
    g_stKeyLedOpt.KEYLED_KEY_Reset = KEYLED_KEY_Reset_PT6961;
    g_stKeyLedOpt.KEYLED_KEY_GetValue = KEYLED_KEY_GetValue_PT6961;
    g_stKeyLedOpt.KEYLED_KEY_SetBlockTime = KEYLED_KEY_SetBlockTime_PT6961;
    g_stKeyLedOpt.KEYLED_KEY_SetRepTime = KEYLED_KEY_SetRepTime_PT6961;
    g_stKeyLedOpt.KEYLED_KEY_IsRepKey = KEYLED_KEY_IsRepKey_PT6961;
    g_stKeyLedOpt.KEYLED_KEY_IsKeyUp  = KEYLED_KEY_IsKeyUp_PT6961;

    g_stKeyLedOpt.KEYLED_LED_Open  = KEYLED_LED_Open_PT6961;
    g_stKeyLedOpt.KEYLED_LED_Close = KEYLED_LED_Close_PT6961;
    g_stKeyLedOpt.KEYLED_LED_Display = KEYLED_LED_Display_PT6961;
    g_stKeyLedOpt.KEYLED_LED_DisplayTime  = KEYLED_LED_DisplayTime_PT6961;
    g_stKeyLedOpt.KEYLED_LED_SetFlashPin  = KEYLED_LED_SetFlashPin_PT6961;
    g_stKeyLedOpt.KEYLED_LED_SetFlashFreq = KEYLED_LED_SetFlashFreq_PT6961;
    g_stKeyLedOpt.KEYLED_LED_DisplayLED = HI_NULL;
    g_stKeyLedOpt.KEYLED_SetMode = HI_NULL;

    g_stKeyLedOpt.KEYLED_Suspend = KEYLED_Suspend_PT6961;
    g_stKeyLedOpt.KEYLED_Resume	 = KEYLED_Resume_PT6961;
#endif

#ifdef KEYLED_PT6964
    HI_INFO_KEYLED("select pt6964 keyled\n");

    g_stKeyLedOpt.KEYLED_KEY_Open  = KEYLED_KEY_Open_PT6964;
    g_stKeyLedOpt.KEYLED_KEY_Close = KEYLED_KEY_Close_PT6964;
    g_stKeyLedOpt.KEYLED_KEY_Reset = KEYLED_KEY_Reset_PT6964;
    g_stKeyLedOpt.KEYLED_KEY_GetValue = KEYLED_KEY_GetValue_PT6964;
    g_stKeyLedOpt.KEYLED_KEY_SetBlockTime = KEYLED_KEY_SetBlockTime_PT6964;
    g_stKeyLedOpt.KEYLED_KEY_SetRepTime = KEYLED_KEY_SetRepTime_PT6964;
    g_stKeyLedOpt.KEYLED_KEY_IsRepKey = KEYLED_KEY_IsRepKey_PT6964;
    g_stKeyLedOpt.KEYLED_KEY_IsKeyUp  = KEYLED_KEY_IsKeyUp_PT6964;

    g_stKeyLedOpt.KEYLED_LED_Open  = KEYLED_LED_Open_PT6964;
    g_stKeyLedOpt.KEYLED_LED_Close = KEYLED_LED_Close_PT6964;
    g_stKeyLedOpt.KEYLED_LED_Display = KEYLED_LED_Display_PT6964;
    g_stKeyLedOpt.KEYLED_LED_DisplayTime  = KEYLED_LED_DisplayTime_PT6964;
    g_stKeyLedOpt.KEYLED_LED_SetFlashPin  = KEYLED_LED_SetFlashPin_PT6964;
    g_stKeyLedOpt.KEYLED_LED_SetFlashFreq = KEYLED_LED_SetFlashFreq_PT6964;
    g_stKeyLedOpt.KEYLED_LED_DisplayLED = HI_NULL;
    g_stKeyLedOpt.KEYLED_SetMode = HI_NULL;

    g_stKeyLedOpt.KEYLED_Suspend = KEYLED_Suspend_PT6964;
    g_stKeyLedOpt.KEYLED_Resume	 = KEYLED_Resume_PT6964;
#endif

#ifdef KEYLED_FD650
	HI_INFO_KEYLED("select fd650 keyled\n");

    g_stKeyLedOpt.KEYLED_KEY_Open  = KEYLED_KEY_Open_FD650;
    g_stKeyLedOpt.KEYLED_KEY_Close = KEYLED_KEY_Close_FD650;
    g_stKeyLedOpt.KEYLED_KEY_Reset = KEYLED_KEY_Reset_FD650;
    g_stKeyLedOpt.KEYLED_KEY_GetValue = KEYLED_KEY_GetValue_FD650;
    g_stKeyLedOpt.KEYLED_KEY_SetBlockTime = KEYLED_KEY_SetBlockTime_FD650;
    g_stKeyLedOpt.KEYLED_KEY_SetRepTime = KEYLED_KEY_SetRepTime_FD650;
    g_stKeyLedOpt.KEYLED_KEY_IsRepKey = KEYLED_KEY_IsRepKey_FD650;
    g_stKeyLedOpt.KEYLED_KEY_IsKeyUp  = KEYLED_KEY_IsKeyUp_FD650;

    g_stKeyLedOpt.KEYLED_LED_Open  = KEYLED_LED_Open_FD650;
    g_stKeyLedOpt.KEYLED_LED_Close = KEYLED_LED_Close_FD650;
    g_stKeyLedOpt.KEYLED_LED_Display = KEYLED_LED_Display_FD650;
    g_stKeyLedOpt.KEYLED_LED_DisplayTime  = KEYLED_LED_DisplayTime_FD650;
    g_stKeyLedOpt.KEYLED_LED_SetFlashPin  = KEYLED_LED_SetFlashPin_FD650;
    g_stKeyLedOpt.KEYLED_LED_SetFlashFreq = KEYLED_LED_SetFlashFreq_FD650;
    g_stKeyLedOpt.KEYLED_LED_DisplayLED = HI_NULL;
    g_stKeyLedOpt.KEYLED_SetMode = HI_NULL;

    g_stKeyLedOpt.KEYLED_Suspend = KEYLED_Suspend_FD650;
    g_stKeyLedOpt.KEYLED_Resume	 = KEYLED_Resume_FD650;
#endif

#ifdef MODULE
#ifndef CONFIG_SUPPORT_CA_RELEASE
    printk("Load hi_keyled.ko success.\t(%s)\n", VERSION_STRING);
#endif
#endif

    return HI_SUCCESS;
}

HI_VOID KEYLED_DRV_ModExit(HI_VOID)
{
    //g_stKeyLedOpt.KEYLED_KEY_Close();
    //g_stKeyLedOpt.KEYLED_LED_Close();

    //HI_DRV_PROC_RemoveModule("keyled");

    (HI_VOID)HI_DRV_MODULE_UnRegister(HI_ID_KEYLED);

    /*unregister demux device*/
    HI_DRV_DEV_UnRegister(&g_stKeyLedDev);
}

#ifdef MODULE
module_init(KEYLED_DRV_ModInit);
module_exit(KEYLED_DRV_ModExit);
#endif

MODULE_LICENSE("GPL");


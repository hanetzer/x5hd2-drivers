/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon. Co., Ltd.

******************************************************************************
File Name	: png_proc.c
Version		: Initial Draft
Author		: z00141204
Created		: 2010/10/14
Description	: png proc infomation
Function List 	: 
			  		  
History       	:
Date				Author        		Modification
2010/10/14		z00141204		Created file      	
******************************************************************************/

#include <linux/module.h>
#include <linux/fs.h>		/* everything... */
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/slab.h>

#include "drv_proc_ext.h"

#include "png_proc.h"
#include "png_define.h"

PNG_PROC_INFO_S s_stPngProcInfo = {0};

static HI_BOOL s_bPngProcOn = HI_FALSE;

    
static char *s_decstate[HI_PNG_STATE_BUTT + 1] = 
{
    "Nostart",
    "Decoding",
    "Finish",
    "Err",
    "Unknown"
};

HI_S32 PNG_Read_Proc(struct seq_file *p, HI_VOID *v)
{
    DRV_PROC_ITEM_S *item;
    PNG_PROC_INFO_S *procinfo;
    char fmtname[8];

    item = (DRV_PROC_ITEM_S *)(p->private);
    procinfo = (PNG_PROC_INFO_S *)(item->data);

    switch(procinfo->eColorFmt)
    {
        case HI_PNG_IMAGEFMT_AGRAY:
            strcpy(fmtname, "AGray");
            break;
        case HI_PNG_IMAGEFMT_ARGB:
            strcpy(fmtname, "ARGB");
            break;
        case HI_PNG_IMAGEFMT_CLUT:
            strcpy(fmtname, "Clut");
            break;
        case HI_PNG_IMAGEFMT_GRAY:
            strcpy(fmtname, "Gray");
            break;
        case HI_PNG_IMAGEFMT_RGB:
            strcpy(fmtname, "RGB");
            break;
        default:
            strcpy(fmtname, "Unknown");
            break;
    }
    
    seq_printf(p, "width\t\t:%u\n", procinfo->u32Width);
    seq_printf(p, "height\t\t:%u\n", procinfo->u32Height);
    seq_printf(p, "fmt\t\t:%s\n", fmtname);
    seq_printf(p, "bitdepth\t:%d\n", procinfo->u8BitDepth);
    seq_printf(p, "transform\t:0x%x\n", procinfo->u32Transform);
    seq_printf(p, "sync\t\t:%s\n", procinfo->bSync?("YES"):("NO"));
    seq_printf(p, "state\t\t:%s\n", s_decstate[procinfo->eState]);
    seq_printf(p, "filter buf addr\t:0x%x\n", procinfo->u32FlterPhyaddr);
    seq_printf(p, "filter buf size\t:0x%x\n", procinfo->u32Size);
    seq_printf(p, "stream buf addr\t:0x%x\n", procinfo->u32StreamBufPhyaddr);
    seq_printf(p, "dst addr\t:0x%x\n", procinfo->u32ImagePhyaddr);
    seq_printf(p, "dst stride\t:0x%x\n", procinfo->u32Stride);
    seq_printf(p, "transcolor\t:0x%x%x%x\n", procinfo->u16TrnsColorRed, procinfo->u16TrnsColorGreen, procinfo->u16TrnsColorBlue);
    seq_printf(p, "filler\t\t:0x%x\n", procinfo->u16Filler);

    return HI_SUCCESS;
}

HI_S32 PNG_Write_Proc(struct file * file,
    const char __user * pBuf, size_t count, loff_t *ppos) 
{
    HI_CHAR buf[128];
    
    if (count > sizeof(buf))
    {
        return 0;
    }

    memset(buf, 0, sizeof(buf));

    if (copy_from_user(buf, pBuf, count))
    {
        return 0;
    }
    if (strstr(buf, "proc on"))
    {
        s_bPngProcOn = HI_TRUE;
        //seq_printf(seq, "png proc on!\n");
    }
    else if (strstr(buf, "proc off"))
    {
        s_bPngProcOn = HI_FALSE;
        //seq_printf(seq, "png proc off!\n");
    }

    return count;
}


HI_VOID PNG_ProcInit(HI_VOID)
{
    DRV_PROC_EX_S  stProc;

    stProc.fnRead   = PNG_Read_Proc;
    stProc.fnWrite  = PNG_Write_Proc;
    stProc.fnIoctl = NULL;

    HI_DRV_PROC_AddModuleEx("png", &stProc, &s_stPngProcInfo);
    
    return ;
}

HI_VOID PNG_ProcCleanup(HI_VOID)
{
    HI_DRV_PROC_RemoveModule("png");
    
    return;
}

HI_BOOL PNG_IsProcOn(HI_VOID)
{
    return s_bPngProcOn;
}

HI_VOID PNG_GetProcStruct(PNG_PROC_INFO_S **ppstProcInfo)
{
    *ppstProcInfo = &s_stPngProcInfo;

    return;
}


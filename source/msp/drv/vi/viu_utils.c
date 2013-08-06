/***********************************************************************************
*              Copyright 2004 - 2050, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName   :  viu_utils.c
* Description:
*
***********************************************************************************/
#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /*cplusplus*/

#include <linux/module.h> 
#include <linux/init.h> 
#include <linux/fs.h> 
#include <linux/uaccess.h> 

#include "viu_utils.h"

struct file *VIU_UtilsFopen(const HI_S8 *filename, HI_S32 flags)
{
    struct file *filp;

    if (flags == 0)
    {
        flags = O_RDONLY;
    }
    else
    {
        flags = O_WRONLY | O_CREAT;
    }

    filp = filp_open(filename, flags | O_LARGEFILE, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    return (IS_ERR(filp)) ? HI_NULL : filp;
}

HI_VOID VIU_UtilsFclose(struct file *filp)
{
    if (filp)
    {
        filp_close(filp, HI_NULL);
    }
}

HI_S32 VIU_UtilsFread(HI_S8 *buf, HI_U32 len, struct file *filp)
{
    HI_S32 readlen;
    mm_segment_t oldfs;

    if (filp == HI_NULL)
    {
        return -ENOENT;
    }

    if (filp->f_op->read == HI_NULL)
    {
        return -ENOSYS;
    }

    if (((filp->f_flags & O_ACCMODE) & (O_RDONLY | O_RDWR)) != 0)
    {
        return -EACCES;
    }

    oldfs = get_fs();
    set_fs(KERNEL_DS);
    readlen = filp->f_op->read(filp, buf, len, &filp->f_pos);
    set_fs(oldfs);

    return readlen;
}

HI_S32 VIU_UtilsFwrite(HI_S8 *buf, HI_S32 len, struct file *filp)
{
    HI_S32 writelen;
    mm_segment_t oldfs;

    if (filp == HI_NULL)
    {
        return -ENOENT;
    }

    if (filp->f_op->write == HI_NULL)
    {
        return -ENOSYS;
    }

    if (((filp->f_flags & O_ACCMODE) & (O_WRONLY | O_RDWR)) == 0)
    {
        return -EACCES;
    }

    oldfs = get_fs();
    set_fs(KERNEL_DS);
    writelen = filp->f_op->write(filp, buf, len, &filp->f_pos);
    set_fs(oldfs);

    return writelen;
}

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /*cplusplus*/

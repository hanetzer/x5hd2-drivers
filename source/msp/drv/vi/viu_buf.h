/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : viu_buf.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 
  Description   : 
  History       :
  1.Date        : 2006/06/13
    Author      : c42025
    Modification: Created file

******************************************************************************/
#ifndef __VIU_BUF_H__
#define __VIU_BUF_H__


#include <linux/list.h>
//#include <asm/arch/media-mem.h>
#include "hi_common.h"
//#include "../../ecs/drv/mmz/common/media-mem.h"
#include "drv_mmz_ext.h"

#define VIU_FB_MAX_NUM  16 /* max number of frame buffer */
#define VIU_FB_MIN_NUM  4  /* min number of frame buffer */
#define VIU_FB_MAX_USER 4  /* max user number of a frame buffer */


#define VIU_FB_INVAL_INDEX ((HI_U32)-1)
#define VIU_FB_INVAL_INDEX_LAST ((HI_U32)-2)

#define VIU_FB_UID0     0x00    /* VIU  */
#define VIU_FB_UID1     0x01    /* VENC */
#define VIU_FB_UID2     0x02    /* DSU  */
#define VIU_FB_UID3     0x03    /* VO   */



/* buffer descriptor  */
typedef struct hiVIU_FB_S
{
	struct list_head list;
	HI_U32 u32Index;
	union{
	    HI_U8  u8UseCnt[VIU_FB_MAX_USER];
	    HI_U32 u32UseCnt;
	}unUseCnter;
	union{
	    HI_U8  u8UseTag[VIU_FB_MAX_USER];
	    HI_U32 u32UseTag;
	}unUseTag;
	HI_U32 u32PhysAddr;
	HI_U32 u32VirtAddr;
	HI_BOOL bLost;
}VIU_FB_S;

typedef struct hiVIU_FB_ROOT_S
{    
    MMZ_BUFFER_S MMZBuf;
    HI_U32 u32FbPhysAddr;
    HI_U32 u32FbVirtAddr;

    HI_U32 u32BufSize;
    HI_U32 u32BlkSize;
    
    HI_U32 u32BlkNum;
    HI_U32 u32FreeNum;
	HI_U32 u32CompTag;

    VIU_FB_S struFb[VIU_FB_MAX_NUM];
    struct list_head free_list;
    struct list_head busy_list;
    struct list_head full_list;
}VIU_FB_ROOT_S;

#define VIU_FB_P2I(pRoot,addr)  (\
    (addr - (pRoot)->u32FbPhysAddr) / (pRoot)->u32BlkSize)
#define VIU_FB_I2P(pRoot,i)  ((pRoot)->struFb[i].u32PhysAddr)

#define VIU_FB_V2I(pRoot,addr)  (\
    (addr - (pRoot)->u32FbVirtAddr) / (pRoot)->u32BlkSize)
#define VIU_FB_I2V(pRoot,i)  ((pRoot)->struFb[i].u32VirtAddr)


/* return a free buffer's physical address */
HI_U32 VIU_FbGet(HI_U32 u32Uid, VIU_FB_ROOT_S *pRoot);
/* You can do to a busy buffer more than one time,
 * until its counter was decreased to zero.
 * but we suggest you NOT to do so strongly!!! */
HI_VOID VIU_FbPut(HI_U32 u32Uid, VIU_FB_ROOT_S *pRoot, HI_U32 u32PhysAddr);
/* Only to a NOT-FREE buffer, you can do this */
HI_VOID VIU_FbAdd(HI_U32 u32Uid,VIU_FB_ROOT_S *pRoot, HI_U32 u32Index);
HI_VOID VIU_FbSub(HI_U32 u32Uid,VIU_FB_ROOT_S *pRoot,HI_U32 u32Index);
HI_S32 VIU_FbInit(HI_U32 u32Port, VIU_FB_ROOT_S *pRoot, HI_U32 u32BufSize, HI_U32 u32BlkSize);
HI_VOID VIU_FbCleanup(VIU_FB_ROOT_S *pRoot);
void Fb_print_list(VIU_FB_ROOT_S *pRoot);

HI_S32 VIU_FbMMapInit(VIU_FB_ROOT_S *pRoot, HI_U32 u32BufIndex, HI_U32 u32BlkSize, HI_U32 u32VirAddr);
HI_S32 VIU_FbPutMMap(VIU_FB_ROOT_S *pRoot,  VIU_FB_S *pFb);
HI_S32 VIU_FbGetMMap(VIU_FB_ROOT_S *pRoot,  VIU_FB_S *pFb);
HI_S32 VIU_FbAddTailMMap(VIU_FB_ROOT_S *pRoot, HI_U32 u32FrameIndex);

HI_S32 VIU_FbAddMMap(HI_U32 u32Uid,VIU_FB_ROOT_S *pRoot,  VIU_FB_S *pFb);
HI_VOID VIU_FbSubMMap(HI_U32 u32Uid,VIU_FB_ROOT_S *pRoot, HI_U32 u32Index);
HI_S32 VIU_FbAddUser(HI_U32 u32Uid, VIU_FB_ROOT_S *pRoot);
HI_S32 VIU_FbSubUser(HI_U32 u32Uid, VIU_FB_ROOT_S *pRoot);
HI_S32 VIU_FbReset(VIU_FB_ROOT_S *pRoot);

#endif//__VIU_BUF_H__

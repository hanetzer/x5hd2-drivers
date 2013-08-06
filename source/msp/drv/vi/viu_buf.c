/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : viu_buf.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       :
  Description   :
  History       :
  1.Date        : 2006/06/13
    Author      : c42025
    Modification: Created file
******************************************************************************/
#include <linux/kernel.h>
#include "viu_buf.h"
#include "hi_error_mpi.h"
#include "drv_vi_ioctl.h"
#include "hi_drv_vi.h"

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 36))
spinlock_t lock_viulist = SPIN_LOCK_UNLOCKED;
#else
spinlock_t lock_viulist = __SPIN_LOCK_UNLOCKED(lock_viulist);
#endif
unsigned long viu_lockflag;

/* return a free buffer's physical address */
HI_U32 VIU_FbGet(HI_U32 u32Uid, VIU_FB_ROOT_S *pRoot)
{
    struct list_head *list;
    VIU_FB_S *pFb;

    if (!pRoot || !pRoot->MMZBuf.u32Size || (u32Uid >= VIU_FB_MAX_USER))
    {
        return (HI_U32)NULL;
    }

    if (list_empty(&pRoot->free_list))
    {
        return (HI_U32)NULL;
    }

    list = pRoot->free_list.next;
    pFb = list_entry(list, VIU_FB_S, list);
    HI_ASSERT(pFb && 0 == pFb->unUseCnter.u32UseCnt);
    pRoot->u32FreeNum--;
    list_del(list);
    pFb->unUseCnter.u8UseCnt[u32Uid]++;
    return pFb->u32PhysAddr;
}

/* You can do to a busy buffer more than one time,
 * until its counter was decreased to zero.
 * but we suggest you NOT to do so strongly!!!
 */
HI_VOID VIU_FbPut(HI_U32 u32Uid, VIU_FB_ROOT_S *pRoot, HI_U32 u32PhysAddr)
{
    HI_U32 i;

    if (!pRoot || !pRoot->MMZBuf.u32Size || (u32Uid >= VIU_FB_MAX_USER))
    {
        return;
    }

    i = VIU_FB_P2I(pRoot, u32PhysAddr);
    HI_ASSERT(i < pRoot->u32BlkNum);
    if (!pRoot->struFb[i].unUseCnter.u8UseCnt[u32Uid])
    {
        return;
    }

    pRoot->struFb[i].unUseCnter.u8UseCnt[u32Uid]--;
    if (!pRoot->struFb[i].unUseCnter.u32UseCnt)
    {
        list_add_tail(&pRoot->struFb[i].list, &pRoot->free_list);
        pRoot->u32FreeNum++;
    }

    return;
}

/* Only to a NOT-FREE buffer, you can do this */
HI_VOID VIU_FbAdd(HI_U32 u32Uid, VIU_FB_ROOT_S *pRoot, HI_U32 u32Index)
{
    if (!pRoot || !pRoot->MMZBuf.u32Size
        || (u32Uid >= VIU_FB_MAX_USER)
        || (u32Index >= pRoot->u32BlkNum)
        || !pRoot->struFb[u32Index].unUseCnter.u32UseCnt)
    {
        return;
    }

    pRoot->struFb[u32Index].unUseCnter.u8UseCnt[u32Uid]++;
    return;
}

HI_S32 VIU_FbAddMMap(HI_U32 u32Uid, VIU_FB_ROOT_S *pRoot, VIU_FB_S *pFb)
{
    int i = 0;
    struct list_head *ptr_buf;
    struct list_head *ptr_buf_n;
    VIU_FB_S *pTmpFb;

    if (list_empty(&pRoot->full_list))
    {
        return HI_FAILURE;
    }
    else
    {
        spin_lock_irqsave(&lock_viulist, viu_lockflag);
        list_for_each_safe (ptr_buf, ptr_buf_n, &pRoot->full_list)
        {
            pTmpFb = list_entry(ptr_buf, VIU_FB_S, list);
            if (!(pTmpFb->unUseCnter.u8UseCnt[u32Uid]) && !(pTmpFb->unUseTag.u8UseTag[u32Uid]))
            {
                pTmpFb->unUseCnter.u8UseCnt[u32Uid]++;
                pTmpFb->unUseTag.u8UseTag[u32Uid]++;

                //               printk("####### add fb id %d, userid %d ,u32UseCnt (%x),u32PhysAddr (%x)\n",pTmpFb->u32Index,u32Uid,pTmpFb->unUseCnter.u32UseCnt,pTmpFb->u32PhysAddr);
                memcpy(pFb, pTmpFb, sizeof(VIU_FB_S));
                spin_unlock_irqrestore(&lock_viulist, viu_lockflag);
                return 0;
            }

            i++;
        }

        //if(u32Uid != 2)
        //printk("UserID(%u) full depth %x.\n", u32Uid,i);
        spin_unlock_irqrestore(&lock_viulist, viu_lockflag);
        return HI_FAILURE;
    }
}

HI_VOID VIU_FbSub(HI_U32 u32Uid, VIU_FB_ROOT_S *pRoot, HI_U32 u32Index)
{
    if (!pRoot || !pRoot->MMZBuf.u32Size
        || (u32Uid >= VIU_FB_MAX_USER)
        || (u32Index >= pRoot->u32BlkNum)
        || !pRoot->struFb[u32Index].unUseCnter.u8UseCnt[u32Uid])
    {
        return;
    }

    pRoot->struFb[u32Index].unUseCnter.u8UseCnt[u32Uid]--;
    if (!pRoot->struFb[u32Index].unUseCnter.u32UseCnt)
    {
        list_add_tail(&pRoot->struFb[u32Index].list, &pRoot->free_list);
        pRoot->u32FreeNum++;
    }

    return;
}

HI_VOID VIU_FbSubMMap(HI_U32 u32Uid, VIU_FB_ROOT_S *pRoot, HI_U32 u32Index)
{
    struct list_head *ptr_buf;
    struct list_head *ptr_buf_n;
    VIU_FB_S *pFb;

    if (list_empty(&pRoot->full_list))
    {
        return;
    }
    else
    {
        spin_lock_irqsave(&lock_viulist, viu_lockflag);
        list_for_each_safe (ptr_buf, ptr_buf_n, &pRoot->full_list)
        {
            pFb = list_entry(ptr_buf, VIU_FB_S, list);

            //if(pFb->unUseCnter.u8UseCnt[u32Uid])
            if (u32Index == pFb->u32Index)
            {
                pFb->unUseCnter.u8UseCnt[u32Uid]--;

                //                printk("!!!!!!! sub fb id %d, userid %d,u32UseCnt (%x),Tag(%x =? %x)\n",pFb->u32Index,u32Uid,pFb->unUseCnter.u32UseCnt,pFb->unUseTag.u32UseTag,pRoot->u32CompTag);
                if ((!pFb->unUseCnter.u32UseCnt) && (pFb->unUseTag.u32UseTag == pRoot->u32CompTag))
                {
                    pFb->unUseTag.u32UseTag = 0;
                    list_move_tail(&pFb->list, &pRoot->free_list);
                    if (!list_empty(&pRoot->full_list))
                    {
                        pFb = list_entry(pRoot->free_list.next, VIU_FB_S, list);

                        //                        printk("list_empty sub fb id %d, userid %d,u32UseCnt (%x)\n",pFb->u32Index,u32Uid,pFb->unUseCnter.u32UseCnt);
                    }

                    pRoot->u32FreeNum++;
                }

                break;
            }
        }
        if (ptr_buf == &pRoot->full_list)
        {
            //printk("**** VIU_FbSub list_empty sub fb id %d, userid %d,u32UseCnt (%x)\n",pFb->u32Index,u32Uid,pFb->unUseCnter.u32UseCnt);
        }

        spin_unlock_irqrestore(&lock_viulist, viu_lockflag);
        return;
    }
}

HI_S32 VIU_FbInit(HI_U32 u32Port, VIU_FB_ROOT_S *pRoot, HI_U32 u32BufSize, HI_U32 u32BlkSize)
{
    HI_U32 i;
    HI_U8 name[25];

    if (!pRoot)
    {
        return HI_ERR_VI_NULL_PTR;
    }

    pRoot->u32BufSize = u32BufSize;
    pRoot->u32BlkSize = u32BlkSize;
    pRoot->u32BlkNum = u32BufSize / u32BlkSize;

    if (pRoot->u32BlkNum < VIU_FB_MIN_NUM)
    {
        pRoot->u32BlkNum = VIU_FB_MIN_NUM;
    }
    else if (pRoot->u32BlkNum > VIU_FB_MAX_NUM)
    {
        pRoot->u32BlkNum = VIU_FB_MAX_NUM;
    }

    pRoot->u32FreeNum = pRoot->u32BlkNum;

    /* Was the root already initialized? */
    if (pRoot->MMZBuf.u32Size)
    {
        HI_ERR_VI("VI has already been initialized! %s\n", __FUNCTION__);
        return HI_FAILURE;
    }

    sprintf(name, "VI_PoolBuf_%d", u32Port);

    if (HI_SUCCESS != HI_DRV_MMZ_AllocAndMap(name, MMZ_OTHERS, u32BufSize, 0, &(pRoot->MMZBuf)))
    {
        HI_ERR_VI("VI HI_DRV_MMZ_AllocAndMap failed! %s\n", __FUNCTION__);
        return HI_FAILURE;
    }

    pRoot->u32FbPhysAddr = pRoot->MMZBuf.u32StartPhyAddr;
    pRoot->u32FbVirtAddr = pRoot->MMZBuf.u32StartVirAddr;

    memset((HI_CHAR*)pRoot->u32FbVirtAddr, 0x00, u32BufSize);

    INIT_LIST_HEAD(&pRoot->free_list);
    INIT_LIST_HEAD(&pRoot->busy_list);
    INIT_LIST_HEAD(&pRoot->full_list);
    for (i = 0; i < pRoot->u32BlkNum; i++)
    {
        pRoot->struFb[i].u32Index = i;
        pRoot->struFb[i].unUseCnter.u32UseCnt = 0;
        pRoot->struFb[i].unUseTag.u32UseTag = 0;
        pRoot->struFb[i].u32PhysAddr = pRoot->u32FbPhysAddr + i * u32BlkSize;
        pRoot->struFb[i].u32VirtAddr = pRoot->u32FbVirtAddr + i * u32BlkSize;

        list_add_tail(&pRoot->struFb[i].list, &pRoot->free_list);
    }

    return HI_SUCCESS;
}

HI_VOID VIU_FbCleanup(VIU_FB_ROOT_S *pRoot)
{
    if (!pRoot || !pRoot->MMZBuf.u32Size)
    {
        return;
    }

    HI_DRV_MMZ_UnmapAndRelease(&(pRoot->MMZBuf));
    pRoot->MMZBuf.u32Size = 0;
    return;
}

/* You can do to a busy buffer more than one time,
 * until its counter was decreased to zero.
 * but we suggest you NOT to do so strongly!!!
 */
HI_VOID VIU_FbPutToCapDoneList(HI_U32 u32Uid, VIU_FB_ROOT_S *pRoot, HI_U32 u32PhysAddr)
{
    HI_U32 i;

    if (!pRoot || !pRoot->MMZBuf.u32Size || (u32Uid >= VIU_FB_MAX_USER))
    {
        return;
    }

    i = VIU_FB_P2I(pRoot, u32PhysAddr);
    HI_ASSERT(i < pRoot->u32BlkNum);
    if (!pRoot->struFb[i].unUseCnter.u8UseCnt[u32Uid])
    {
        return;
    }

    pRoot->struFb[i].unUseCnter.u8UseCnt[u32Uid]--;
    if (!pRoot->struFb[i].unUseCnter.u32UseCnt)
    {
        list_add_tail(&pRoot->struFb[i].list, &pRoot->free_list);
        pRoot->u32FreeNum++;
    }

    return;
}

void Fb_print_list(VIU_FB_ROOT_S *pRoot)
{
    struct list_head *ptr_buf;
    struct list_head *ptr_buf_n;
    VIU_FB_S *pFb;

    HI_INFO_VI("full list :");
    spin_lock_irqsave(&lock_viulist, viu_lockflag);

    list_for_each_safe (ptr_buf, ptr_buf_n, &pRoot->full_list)
    {
        pFb = list_entry(ptr_buf, VIU_FB_S, list);
        HI_INFO_VI(" %d(%x) ", pFb->u32Index, pFb->unUseCnter.u32UseCnt);
    }
    spin_unlock_irqrestore(&lock_viulist, viu_lockflag);
    HI_INFO_VI("\n");

    HI_INFO_VI("free list :");
    list_for_each_safe (ptr_buf, ptr_buf_n, &pRoot->free_list)
    {
        pFb = list_entry(ptr_buf, VIU_FB_S, list);
        HI_INFO_VI(" %d(%x) ", pFb->u32Index, pFb->unUseCnter.u32UseCnt);
    }
    HI_INFO_VI("\n");

    HI_INFO_VI("busy list :");
    list_for_each_safe (ptr_buf, ptr_buf_n, &pRoot->busy_list)
    {
        pFb = list_entry(ptr_buf, VIU_FB_S, list);
        HI_INFO_VI(" %d ", pFb->u32Index);
    }
    HI_INFO_VI("\n");
}

static HI_S32 usrvirtophyaddr(char __user *buf, HI_U32 *u32Phyaddr)
{
    HI_S32 res, nr_pages;
    struct page *pages;
    HI_U32 align_buf, physp;
    struct mm_struct *mm = current->mm;

    nr_pages  = 1;
    align_buf = PAGE_ALIGN((HI_U32)buf);
    down_read(&mm->mmap_sem);
    res = get_user_pages(current, mm, align_buf, 1, 1, 0, &pages, NULL);
    up_read(&mm->mmap_sem);
    if (res == nr_pages)
    {
        physp = __pa(page_address(&pages[0]) /* + (buf & ~PAGE_MASK)*/);
        *u32Phyaddr = physp;
    }
    else
    {
        HI_WARN_VI("get_user_pages failed, result: %d\n", res);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 VIU_FbMMapInit(VIU_FB_ROOT_S *pRoot, HI_U32 u32BufIndex, HI_U32 u32BlkSize, HI_U32 u32VirAddr)
{
    HI_U32 u32PhyAddr = 0;
    struct list_head *ptr_buf;
    struct list_head *ptr_buf_n;
    VIU_FB_S *pFb;

    if (!pRoot)
    {
        return HI_ERR_VI_NULL_PTR;
    }

    if (u32BufIndex < 0)
    {
        return HI_ERR_VI_INVALID_PARA;
    }
    else if (u32BufIndex > VIU_FB_MAX_NUM - 1)
    {
        u32BufIndex = VIU_FB_MAX_NUM - 1;
    }

    usrvirtophyaddr((HI_U8 *)u32VirAddr, &u32PhyAddr);
    if (0 == u32BufIndex)
    {
        INIT_LIST_HEAD(&pRoot->free_list);
        INIT_LIST_HEAD(&pRoot->busy_list);
        INIT_LIST_HEAD(&pRoot->full_list);

        pRoot->u32FbVirtAddr = u32VirAddr;
        pRoot->u32FbPhysAddr = u32PhyAddr;
    }

    pRoot->struFb[u32BufIndex].u32VirtAddr = u32VirAddr;
    pRoot->struFb[u32BufIndex].u32PhysAddr = u32PhyAddr;

    pRoot->u32BlkNum  = u32BufIndex + 1;
    pRoot->u32BlkSize = u32BlkSize;
    pRoot->u32BufSize = (u32BufIndex + 1) * u32BlkSize;
    pRoot->u32FreeNum = 0;

    pRoot->struFb[u32BufIndex].u32Index = u32BufIndex;
    pRoot->struFb[u32BufIndex].unUseCnter.u32UseCnt = 0;
    pRoot->struFb[u32BufIndex].unUseTag.u32UseTag = 0;

    if (!list_empty(&pRoot->full_list))
    {
        spin_lock_irqsave(&lock_viulist, viu_lockflag);
        list_for_each_safe (ptr_buf, ptr_buf_n, &pRoot->full_list)
        {
            pFb = list_entry(ptr_buf, VIU_FB_S, list);
            list_del(&pFb->list);
        }
        spin_unlock_irqrestore(&lock_viulist, viu_lockflag);
    }

    return HI_SUCCESS;
}

HI_S32 VIU_FbAddTailMMap(VIU_FB_ROOT_S *pRoot, HI_U32 u32FrameIndex)
{
    if (!pRoot)
    {
        return HI_FAILURE;
    }

    spin_lock_irqsave(&lock_viulist, viu_lockflag);
    list_add_tail(&pRoot->struFb[u32FrameIndex].list, &pRoot->full_list);
    spin_unlock_irqrestore(&lock_viulist, viu_lockflag);

    return HI_SUCCESS;
}

/* when capture picture,put it in full_list
 */
HI_S32 VIU_FbPutMMap(VIU_FB_ROOT_S *pRoot, VIU_FB_S *pFb)
{
    if (!pRoot)
    {
        return HI_FAILURE;
    }

    list_add_tail(&pFb->list, &pRoot->full_list);
    return HI_FAILURE;
}

/* when capture picture,put it in full_list
 */
HI_S32 VIU_FbGetMMap(VIU_FB_ROOT_S *pRoot, VIU_FB_S *pFb)
{
    //    struct list_head *ptr_buf;
    //    struct list_head *ptr_buf_n;
    VIU_FB_S *pTmpFb;

    if (!pRoot)
    {
        return HI_FAILURE;
    }

    if (list_empty(&pRoot->free_list))
    {
        return HI_FAILURE;
    }
    else
    {
        pTmpFb = list_entry(pRoot->free_list.next, VIU_FB_S, list);

        //printk("VIU_FbGetMMap index %d\n",pTmpFb->u32Index);
        list_del(&pTmpFb->list);
        memcpy(pFb, pTmpFb, sizeof(VIU_FB_S));
        return HI_SUCCESS;
    }
}

HI_S32 VIU_FbAddUser(HI_U32 u32Uid, VIU_FB_ROOT_S *pRoot)
{
    HI_U32 addTag = 0;

    addTag = 1 << (u32Uid * 8);
    pRoot->u32CompTag |= addTag;

    //printk("VIU_FbAddUser tag %d\n",pRoot->u32CompTag);
    return HI_SUCCESS;
}

HI_S32 VIU_FbSubUser(HI_U32 u32Uid, VIU_FB_ROOT_S *pRoot)
{
    HI_U32 subTag = 0;

    subTag = 1 << (u32Uid * 8);
    pRoot->u32CompTag &= ~subTag;
	
    return HI_SUCCESS;
}

HI_S32 VIU_FbReset(VIU_FB_ROOT_S *pRoot)
{
    struct list_head *ptr_buf;
    struct list_head *ptr_buf_n;
    VIU_FB_S *pFb = HI_NULL;

    if (!pRoot)
    {
        return HI_FAILURE;
    }

    if (!list_empty(&pRoot->full_list))
    {
        spin_lock_irqsave(&lock_viulist, viu_lockflag);
        list_for_each_safe (ptr_buf, ptr_buf_n, &pRoot->full_list)
        {
            pFb = list_entry(ptr_buf, VIU_FB_S, list);
            pFb->unUseTag.u32UseTag = 0;
            list_move_tail(&pFb->list, &pRoot->free_list);
        }
        spin_unlock_irqrestore(&lock_viulist, viu_lockflag);
    }
    
    return HI_SUCCESS;
}


/**
\file
\brief mmz ops
\copyright Shenzhen Hisilicon Co., Ltd.
\date 2008-2018
\version draft
\author QuYaxin 46153
\date 2008-12-25
*/
/* add include here */
#include "drv_mmz.h"
#include "drv_mmz_ext.h"
#include "drv_mem_ioctl.h"

#define ERR_MEM(fmt...) printk(fmt)

HI_S32 HI_DRV_MMZ_AllocAndMap(const char *bufname, char *zone_name, HI_U32 size, int align, MMZ_BUFFER_S *psMBuf)
{
    mmb_addr_t phyaddr;
    char *mmz_name =  zone_name;

    phyaddr = new_mmb(bufname, size, align, mmz_name);
    if (phyaddr == MMB_ADDR_INVALID)
    {
        //mmz_name = MMZ_OTHERS;
        /*alloc once again from others buffer*/
        /*CNcomment:再从others 缓冲区分配一次*/
        phyaddr = new_mmb(bufname, size, align, NULL);
        if (phyaddr == MMB_ADDR_INVALID)
        {
            psMBuf->u32Size =  0;
            psMBuf->u32StartPhyAddr = (HI_U32) MMB_ADDR_INVALID;
            psMBuf->u32StartVirAddr = (HI_U32) NULL;
            ERR_MEM("Alloc %s failed! \n", bufname);
            return HI_FAILURE;
        }
    }
    psMBuf->u32Size = size;

    psMBuf->u32StartVirAddr = (HI_U32)remap_mmb (phyaddr);
    if (HI_NULL == psMBuf->u32StartVirAddr)
    {
        delete_mmb(phyaddr);
        psMBuf->u32Size =  0;
        psMBuf->u32StartPhyAddr = (HI_U32) MMB_ADDR_INVALID;
        psMBuf->u32StartVirAddr = (HI_U32) NULL;
        ERR_MEM("Remap %s failed! \n", bufname);
        return HI_FAILURE;
    }
    psMBuf->u32StartPhyAddr = (HI_U32)phyaddr;

    return HI_SUCCESS;
}

HI_VOID HI_DRV_MMZ_UnmapAndRelease(MMZ_BUFFER_S *psMBuf)
{
    mmb_addr_t phyaddr = psMBuf->u32StartPhyAddr;
    if (MMB_ADDR_INVALID != phyaddr )
    {
        unmap_mmb((HI_VOID*)psMBuf->u32StartVirAddr);
        delete_mmb(phyaddr);
    }
}

HI_S32 HI_DRV_MMZ_Alloc(const char *bufname, char *zone_name, HI_U32 size, int align, MMZ_BUFFER_S *psMBuf)
{
    mmb_addr_t phyaddr;
    char *mmz_name =  zone_name;

    phyaddr = new_mmb(bufname, size, align, mmz_name);
    if (phyaddr == MMB_ADDR_INVALID)
    {
        //mmz_name = MMZ_OTHERS;
        /*alloc once again from others buffer*/
        /*CNcomment:再从others 缓冲区分配一次*/
        phyaddr = new_mmb(bufname, size, align, NULL);
        if (phyaddr == MMB_ADDR_INVALID)
        {
            psMBuf->u32Size =  0;
            psMBuf->u32StartPhyAddr = (HI_U32) MMB_ADDR_INVALID;
            psMBuf->u32StartVirAddr = (HI_U32) NULL;
            ERR_MEM("Alloc %s failed! \n", bufname);
            return HI_FAILURE;
        }
    }
    psMBuf->u32Size = size;

    psMBuf->u32StartPhyAddr = (HI_U32)phyaddr;

    return HI_SUCCESS;
}

HI_S32 HI_DRV_MMZ_MapCache(MMZ_BUFFER_S *psMBuf)
{
    mmb_addr_t phyaddr = psMBuf->u32StartPhyAddr;

    psMBuf->u32StartVirAddr = (HI_U32)remap_mmb_cached(phyaddr);
    if (HI_NULL == psMBuf->u32StartVirAddr)
    {
        psMBuf->u32StartVirAddr = (HI_U32) NULL;
        ERR_MEM("Remap buf failed! \n");
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

HI_S32 HI_DRV_MMZ_Map(MMZ_BUFFER_S *psMBuf)
{
    mmb_addr_t phyaddr = psMBuf->u32StartPhyAddr;

    psMBuf->u32StartVirAddr = (HI_U32)remap_mmb (phyaddr);
    if (HI_NULL == psMBuf->u32StartVirAddr)
    {
        psMBuf->u32StartVirAddr = (HI_U32) NULL;
        ERR_MEM("Remap buf failed! \n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_VOID HI_DRV_MMZ_Unmap(MMZ_BUFFER_S *psMBuf)
{
    unmap_mmb((HI_VOID*)psMBuf->u32StartVirAddr);
}

HI_VOID HI_DRV_MMZ_Release(MMZ_BUFFER_S *psMBuf)
{
    mmb_addr_t phyaddr = psMBuf->u32StartPhyAddr;

    if (MMB_ADDR_INVALID != phyaddr)
    {
        delete_mmb(phyaddr);
    }
}

EXPORT_SYMBOL(HI_DRV_MMZ_AllocAndMap);
EXPORT_SYMBOL(HI_DRV_MMZ_UnmapAndRelease);
EXPORT_SYMBOL(HI_DRV_MMZ_Alloc);
EXPORT_SYMBOL(HI_DRV_MMZ_MapCache);
EXPORT_SYMBOL(HI_DRV_MMZ_Map);
EXPORT_SYMBOL(HI_DRV_MMZ_Unmap);
EXPORT_SYMBOL(HI_DRV_MMZ_Release);


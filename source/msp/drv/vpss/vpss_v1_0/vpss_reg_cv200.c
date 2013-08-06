#include "vpss_reg_cv200.h"
#include "vpss_common.h"
#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

/*寄存器读写操作*/
HI_S32 VPSS_REG_RegWrite(volatile HI_U32 *a, HI_U32 b)
{
    *a = b;      
    return HI_SUCCESS;
}
HI_U32 VPSS_REG_RegRead(volatile HI_U32* a)
{
   return (*(a));
}


/*寄存器复位*/
HI_S32 VPSS_REG_ReSetCRG(HI_VOID)
{
    HI_U32 *g_pSysHdClkRegVirAddr;
    HI_U32 u32Count;
    
    g_pSysHdClkRegVirAddr = (HI_U32 *)IO_ADDRESS(VPSS_CRG_ADDR);

    *g_pSysHdClkRegVirAddr = 0x11;

    for(u32Count = 0;u32Count < 100;u32Count++)

    *g_pSysHdClkRegVirAddr = 0x1;

    return HI_SUCCESS;
}

/*寄存器物理地址映射*/
HI_S32 VPSS_REG_BaseRegInit(VPSS_REG_S **ppstPhyReg)
{
    *ppstPhyReg = (VPSS_REG_S * )IO_ADDRESS(VPSS_BASE_ADDR);

    return HI_SUCCESS;
}


/*载入配置结点地址映射*/
HI_S32 VPSS_REG_AppRegInit(VPSS_REG_S **ppstAppReg,HI_U32 u32VirAddr)
{
    *ppstAppReg = (VPSS_REG_S * )u32VirAddr;

    return HI_SUCCESS;
}

/*配置结点重置*/
HI_S32 VPSS_REG_ResetAppReg(HI_U32 u32AppAddr)
{
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S *)u32AppAddr;
    
    memset((void*)pstReg, 0, sizeof(VPSS_REG_S)); 

    VPSS_REG_RegWrite(&(pstReg->VPSS_TIMEOUT), 0xf);
    
    // TODO:VPSS_MISCELLANEOUS.bit.ck_gt_en_calc待确认
     
    VPSS_REG_RegWrite(&(pstReg->VPSS_MISCELLANEOUS.u32), 0x1003244);
    VPSS_REG_RegWrite(&(pstReg->VPSS_PNEXT), 0x0);
    VPSS_REG_RegWrite(&(pstReg->VPSS_INTMASK.u32), 0xf);

    return HI_SUCCESS;
}


/*中断寄存器相关操作*/
/********************************/
HI_S32 VPSS_REG_SetIntMask(HI_U32 u32AppAddr,HI_U32 u32Mask)
{
    
    U_VPSS_INTMASK VPSS_INTMASK;
    
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S *)u32AppAddr;
    

    VPSS_INTMASK.u32 = u32Mask;
    
    VPSS_REG_RegWrite(&(pstReg->VPSS_INTMASK.u32), VPSS_INTMASK.u32); 

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_GetIntMask(HI_U32 u32AppAddr,HI_U32 *pu32Mask)
{
    VPSS_REG_S *pstReg;
    
    U_VPSS_INTMASK VPSS_INTMASK;
    
    pstReg = (VPSS_REG_S *)u32AppAddr;

    // TODO:新增解压错误、TUNNEL中断
    VPSS_INTMASK.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_INTMASK.u32));


    *pu32Mask = VPSS_INTMASK.u32;

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_GetIntState(HI_U32 u32AppAddr,HI_U32 *pu32Int)
{
    U_VPSS_INTSTATE VPSS_INTSTATE;
    
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S *)u32AppAddr;
    
    // TODO:新增解压错误、TUNNEL中断
    VPSS_INTSTATE.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_INTSTATE.u32));

    *pu32Int = VPSS_INTSTATE.u32;

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_GetRawIntState(HI_U32 u32AppAddr,HI_U32 *pu32RawInt)
{
    U_VPSS_RAWINT VPSS_RAWINT;
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S *)u32AppAddr;
    
    VPSS_RAWINT.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_RAWINT.u32));

    *pu32RawInt = VPSS_RAWINT.u32;

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_ClearIntState(HI_U32 u32AppAddr,HI_U32 u32Data)
{
    U_VPSS_INTCLR VPSS_INTCLR;
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S *)u32AppAddr;
    
    VPSS_INTCLR.u32 = u32Data;

    VPSS_REG_RegWrite(&(pstReg->VPSS_INTCLR.u32), VPSS_INTCLR.u32);

    return HI_SUCCESS;
}
/********************************/


/*逻辑超时设置*/
HI_S32 VPSS_REG_SetTimeOut(HI_U32 u32AppAddr,HI_U32 u32Data)
{ 
    HI_U32 VPSS_TIMEOUT;
    VPSS_REG_S *pstReg;

    
    pstReg = (VPSS_REG_S *)u32AppAddr;
    VPSS_TIMEOUT = u32Data;
    
    VPSS_REG_RegWrite(&(pstReg->VPSS_TIMEOUT), VPSS_TIMEOUT);

    return HI_SUCCESS;
}

/*配置结点地址载入，启动逻辑*/
HI_S32 VPSS_REG_StartLogic(HI_U32 u32AppAddr,HI_U32 u32PhyAddr)
{
    U_VPSS_START VPSS_START;
    VPSS_REG_S *pstReg;

    
    pstReg = (VPSS_REG_S *)u32PhyAddr;
    VPSS_REG_RegWrite(&(pstReg->VPSS_PNEXT), u32AppAddr);
    
    VPSS_START.u32 = 0x1;
    VPSS_REG_RegWrite(&(pstReg->VPSS_START.u32), VPSS_START.u32);
    return HI_SUCCESS;
}

/*输出PORT使能*/
HI_S32 VPSS_REG_EnPort(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_BOOL bEnable)
{
    U_VPSS_CTRL VPSS_CTRL;
    VPSS_REG_S *pstReg;

    
    pstReg = (VPSS_REG_S *)u32AppAddr;
    VPSS_CTRL.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_CTRL.u32));
    
    switch(ePort)
    {
        case VPSS_REG_HD:
            VPSS_CTRL.bits.vhd_en = bEnable;
            break;
        case VPSS_REG_STR:
            VPSS_CTRL.bits.str_en = bEnable;
            break;
        case VPSS_REG_SD:
            VPSS_CTRL.bits.vsd_en = bEnable;
            break;
        default:
            VPSS_FATAL("\n ePort Error");
    }

    VPSS_REG_RegWrite(&(pstReg->VPSS_CTRL.u32), VPSS_CTRL.u32);

    return HI_SUCCESS;
}
/*输入Image相关操作*/
/********************************/
HI_S32 VPSS_REG_SetImgSize(HI_U32 u32AppAddr,HI_U32 u32Height,HI_U32 u32Width,HI_BOOL bProgressive)
{
    U_VPSS_IMGSIZE VPSS_IMGSIZE;
    VPSS_REG_S *pstReg;

    
    pstReg = (VPSS_REG_S *)u32AppAddr;
    VPSS_IMGSIZE.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_IMGSIZE.u32));

    // TODO:确认CV200 的输出IMG 高度是否还需要区分帧/场
    if(bProgressive)
    {
        VPSS_IMGSIZE.bits.imgheight = u32Height -1;
    }
    else
    {
        VPSS_IMGSIZE.bits.imgheight = (u32Height/2 - 1);
    }
    
    VPSS_IMGSIZE.bits.imgwidth = u32Width - 1;

    VPSS_REG_RegWrite(&(pstReg->VPSS_IMGSIZE.u32), VPSS_IMGSIZE.u32);
    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetImgAddr(HI_U32 u32AppAddr,REG_FIELDPOS_E ePos,HI_U32 u32Yaddr,HI_U32 u32Caddr)
{

    // TODO:CV200读入地址不再按场区分，需要确认DEI时的配置
    HI_U32 VPSS_CURYADDR;
    HI_U32 VPSS_CURCBADDR;
    
    HI_U32 VPSS_LASTYADDR;
    HI_U32 VPSS_LASTCBADDR;

    HI_U32 VPSS_NEXTYADDR;
    HI_U32 VPSS_NEXTCBADDR;
    
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S *)u32AppAddr;

    switch(ePos)
    {   
        case LAST_FIELD:
            VPSS_LASTYADDR = VPSS_REG_RegRead(&(pstReg->VPSS_LASTYADDR));
            VPSS_LASTCBADDR = VPSS_REG_RegRead(&(pstReg->VPSS_LASTCBADDR));

            VPSS_LASTYADDR = u32Yaddr;
            VPSS_LASTCBADDR = u32Caddr;

            VPSS_REG_RegWrite(&(pstReg->VPSS_LASTYADDR), VPSS_LASTYADDR);
            VPSS_REG_RegWrite(&(pstReg->VPSS_LASTCBADDR), VPSS_LASTCBADDR);
            break;
        case CUR_FIELD:
            VPSS_CURYADDR = VPSS_REG_RegRead(&(pstReg->VPSS_CURYADDR));
            VPSS_CURCBADDR = VPSS_REG_RegRead(&(pstReg->VPSS_CURCBADDR));

            VPSS_CURYADDR = u32Yaddr;
            VPSS_CURCBADDR = u32Caddr;

            VPSS_REG_RegWrite(&(pstReg->VPSS_CURYADDR), VPSS_CURYADDR);
            VPSS_REG_RegWrite(&(pstReg->VPSS_CURCBADDR), VPSS_CURCBADDR);
            break;
        case NEXT1_FIELD:
            VPSS_NEXTYADDR = VPSS_REG_RegRead(&(pstReg->VPSS_NEXTYADDR));
            VPSS_NEXTCBADDR = VPSS_REG_RegRead(&(pstReg->VPSS_NEXTCBADDR));

            VPSS_NEXTYADDR = u32Yaddr;
            VPSS_NEXTCBADDR = u32Caddr;

            VPSS_REG_RegWrite(&(pstReg->VPSS_NEXTYADDR), VPSS_NEXTYADDR);
            VPSS_REG_RegWrite(&(pstReg->VPSS_NEXTCBADDR), VPSS_NEXTCBADDR);
            break;
        default:
            VPSS_FATAL("FIELD ERROR\n");
    }
    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetImgStride(HI_U32 u32AppAddr,REG_FIELDPOS_E ePos,HI_U32 u32YStride,HI_U32 u32CStride)
{
    U_VPSS_CURSTRIDE VPSS_CURSTRIDE;
    U_VPSS_LASTSTRIDE VPSS_LASTSTRIDE;
    U_VPSS_NEXTSTRIDE VPSS_NEXTSTRIDE;
    
    VPSS_REG_S *pstReg;

    
    pstReg = (VPSS_REG_S *)u32AppAddr;
    switch(ePos)
    {   
        case LAST_FIELD:
            VPSS_LASTSTRIDE.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_LASTSTRIDE.u32));
            
            VPSS_LASTSTRIDE.bits.lasty_stride = u32YStride;
            VPSS_LASTSTRIDE.bits.lastc_stride = u32CStride;

            VPSS_REG_RegWrite(&(pstReg->VPSS_LASTSTRIDE.u32), VPSS_LASTSTRIDE.u32);
            break;
        case CUR_FIELD:
            VPSS_CURSTRIDE.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_CURSTRIDE.u32));
            
            VPSS_CURSTRIDE.bits.curry_stride = u32YStride;
            VPSS_CURSTRIDE.bits.currc_stride = u32CStride;

            VPSS_REG_RegWrite(&(pstReg->VPSS_CURSTRIDE.u32), VPSS_CURSTRIDE.u32);
            break;
        case NEXT1_FIELD:
            VPSS_NEXTSTRIDE.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_NEXTSTRIDE.u32));
            
            VPSS_NEXTSTRIDE.bits.nexty_stride = u32YStride;
            VPSS_NEXTSTRIDE.bits.nextc_stride = u32CStride;

            VPSS_REG_RegWrite(&(pstReg->VPSS_NEXTSTRIDE.u32), VPSS_NEXTSTRIDE.u32);
            break;
        default:
            VPSS_FATAL("REG ERROR\n");
    }
    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetImgFormat(HI_U32 u32AppAddr,HI_DRV_PIX_FORMAT_E eFormat)
{
    U_VPSS_CTRL2 VPSS_CTRL2;
    VPSS_REG_S *pstReg;

    
    pstReg = (VPSS_REG_S *)u32AppAddr;
    VPSS_CTRL2.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_CTRL2.u32));

    // TODO:后续确认支持全格式的枚举名
    switch(eFormat)
    {
        case HI_DRV_PIX_FMT_NV12:
            VPSS_CTRL2.bits.in_format = 0x0;
            break;
        case HI_DRV_PIX_FMT_NV16:
            VPSS_CTRL2.bits.in_format = 0x2;
            break;
        default:
            VPSS_FATAL("REG ERROR\n");
    }
    
    VPSS_REG_RegWrite(&(pstReg->VPSS_CTRL2.u32), VPSS_CTRL2.u32);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetImgReadMod(HI_U32 u32AppAddr,HI_BOOL bField)
{
    U_VPSS_CTRL VPSS_CTRL;
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S *)u32AppAddr;
    VPSS_CTRL.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_CTRL.u32));
    VPSS_CTRL.bits.bfield = bField;
    VPSS_REG_RegWrite(&(pstReg->VPSS_CTRL.u32), VPSS_CTRL.u32);
    return HI_SUCCESS;
}
/********************************/

/*输出Frame相关操作*/
/********************************/
HI_S32 VPSS_REG_SetFrmSize(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32Height,HI_U32 u32Width)
{
    U_VPSS_VHDSIZE VPSS_VHDSIZE;
    U_VPSS_VSDSIZE VPSS_VSDSIZE;
    U_VPSS_STRSIZE VPSS_STRSIZE;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S *)u32AppAddr;
    switch(ePort)
    {
        case VPSS_REG_HD:
            VPSS_VHDSIZE.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_VHDSIZE.u32));
            VPSS_VHDSIZE.bits.vhd_height = u32Height-1;
            VPSS_VHDSIZE.bits.vhd_width = u32Width-1;
            VPSS_REG_RegWrite(&(pstReg->VPSS_VHDSIZE.u32), VPSS_VHDSIZE.u32);
    
            break;
        case VPSS_REG_SD ://SD
            VPSS_VSDSIZE.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_VSDSIZE.u32));
            VPSS_VSDSIZE.bits.vsd_height = u32Height-1;
            VPSS_VSDSIZE.bits.vsd_width = u32Width-1;
            VPSS_REG_RegWrite(&(pstReg->VPSS_VSDSIZE.u32), VPSS_VSDSIZE.u32);
            break;
        case VPSS_REG_STR://STR
            VPSS_STRSIZE.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_STRSIZE.u32));
            VPSS_STRSIZE.bits.str_height = u32Height-1;
            VPSS_STRSIZE.bits.str_width = u32Width-1;
            VPSS_REG_RegWrite(&(pstReg->VPSS_STRSIZE.u32), VPSS_STRSIZE.u32);
            break;
        default:
            VPSS_FATAL("REG ERROR\n");
            break;
    }
    return HI_SUCCESS;
}

// TODO:函数原型增加传入压缩信息头地址
HI_S32 VPSS_REG_SetFrmAddr(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32Yaddr,HI_U32 u32Caddr)
{
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S *)u32AppAddr;
    switch(ePort)
    {
        case VPSS_REG_HD:
            VPSS_REG_RegWrite(&(pstReg->VPSS_VHDYADDR), u32Yaddr);
            VPSS_REG_RegWrite(&(pstReg->VPSS_VHDCADDR), u32Caddr);
            break;
        case VPSS_REG_STR://STR
            VPSS_REG_RegWrite(&(pstReg->VPSS_STRYADDR), u32Yaddr);
            VPSS_REG_RegWrite(&(pstReg->VPSS_STRCADDR), u32Caddr);
            break;
        case VPSS_REG_SD://SD
            VPSS_REG_RegWrite(&(pstReg->VPSS_VSDYADDR), u32Yaddr);
            VPSS_REG_RegWrite(&(pstReg->VPSS_VSDCADDR), u32Caddr);
            break;
        default:
            VPSS_FATAL("REG ERROR\n");
            break;
    }
    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetFrmStride(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32YStride,HI_U32 u32CStride)
{
    U_VPSS_VHDSTRIDE VPSS_VHDSTRIDE;
    U_VPSS_VSDSTRIDE VPSS_VSDSTRIDE;
    U_VPSS_STRSTRIDE VPSS_STRSTRIDE;
    
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S *)u32AppAddr;
    switch(ePort)
    {
        case VPSS_REG_HD:
            VPSS_VHDSTRIDE.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_VHDSTRIDE.u32));
            VPSS_VHDSTRIDE.bits.vhdy_stride = u32YStride;
            VPSS_VHDSTRIDE.bits.vhdc_stride = u32CStride;
            VPSS_REG_RegWrite(&(pstReg->VPSS_VHDSTRIDE.u32), VPSS_VHDSTRIDE.u32);
    
            break;
        case VPSS_REG_STR://SD
            VPSS_STRSTRIDE.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_STRSTRIDE.u32));
            VPSS_STRSTRIDE.bits.stry_stride = u32YStride;
            VPSS_STRSTRIDE.bits.strc_stride = u32CStride;
            VPSS_REG_RegWrite(&(pstReg->VPSS_STRSTRIDE.u32), VPSS_STRSTRIDE.u32);
            break;
        case VPSS_REG_SD://STR
            VPSS_VSDSTRIDE.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_VSDSTRIDE.u32));
            VPSS_VSDSTRIDE.bits.vsdy_stride = u32YStride;
            VPSS_VSDSTRIDE.bits.vsdc_stride = u32CStride;
            VPSS_REG_RegWrite(&(pstReg->VPSS_VSDSTRIDE.u32), VPSS_VSDSTRIDE.u32);
            break;
        default:
            VPSS_FATAL("REG ERROR\n");
            break;
    }
    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetFrmFormat(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_DRV_PIX_FORMAT_E eFormat)
{
    U_VPSS_CTRL2 VPSS_CTRL2;
    HI_U32 u32Format;
    VPSS_REG_S *pstReg;

    
    pstReg = (VPSS_REG_S *)u32AppAddr;


    // TODO: 增加各PORT输出格式检查
    switch(eFormat)
    {
        case HI_DRV_PIX_FMT_NV12:
            u32Format = 0x0;
            break;
        case HI_DRV_PIX_FMT_NV16:
            u32Format = 0x1;
            break;
        default:
            VPSS_FATAL("REG ERROR\n");
            return HI_FAILURE;
    }
    
    VPSS_CTRL2.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_CTRL2.u32));
    switch(ePort)
    {
        case VPSS_REG_HD:
            VPSS_CTRL2.bits.vhd_format = u32Format;
    
            break;
        case VPSS_REG_STR://SD
            VPSS_CTRL2.bits.str_format = u32Format;
            break;
        case VPSS_REG_SD://STR
            VPSS_CTRL2.bits.vsd_format = u32Format;
            break;
        default:
            VPSS_FATAL("REG ERROR\n");
            break;
    }
     
    VPSS_REG_RegWrite(&(pstReg->VPSS_CTRL2.u32), VPSS_CTRL2.u32);
    return HI_SUCCESS;
}
/********************************/

/*ZME相关操作*/
/********************************/
HI_S32 VPSS_REG_SetZmeEnable(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,REG_ZME_MODE_E eMode,HI_BOOL bEnable)
{
    U_VPSS_VHD_VSP VPSS_VHD_VSP;
    U_VPSS_VSD_VSP VPSS_VSD_VSP;
    U_VPSS_STR_VSP VPSS_STR_VSP;

    U_VPSS_VHD_HSP VPSS_VHD_HSP;
    U_VPSS_VSD_HSP VPSS_VSD_HSP;
    U_VPSS_STR_HSP VPSS_STR_HSP;
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S *)u32AppAddr;

    switch(ePort)
    {
        case VPSS_REG_HD:
            if((eMode ==  REG_ZME_MODE_HORL )||(eMode == REG_ZME_MODE_HOR)||(eMode == REG_ZME_MODE_ALL))
            {
                VPSS_VHD_HSP.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_VHD_HSP.u32));
                VPSS_VHD_HSP.bits.hlmsc_en = bEnable;
                VPSS_REG_RegWrite(&(pstReg->VPSS_VHD_HSP.u32), VPSS_VHD_HSP.u32); 
            }
            if((eMode == REG_ZME_MODE_HORC)||(eMode == REG_ZME_MODE_HOR)||(eMode == REG_ZME_MODE_ALL))
            {
                VPSS_VHD_HSP.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_VHD_HSP.u32));
                VPSS_VHD_HSP.bits.hchmsc_en = bEnable;
                VPSS_REG_RegWrite(&(pstReg->VPSS_VHD_HSP.u32), VPSS_VHD_HSP.u32); 
            }
            if((eMode == REG_ZME_MODE_VERL)||(eMode == REG_ZME_MODE_VER)||(eMode == REG_ZME_MODE_ALL))
            {
                VPSS_VHD_VSP.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_VHD_VSP.u32));
                VPSS_VHD_VSP.bits.vlmsc_en = bEnable;
                VPSS_REG_RegWrite(&(pstReg->VPSS_VHD_VSP.u32), VPSS_VHD_VSP.u32); 
            }
            if((eMode == REG_ZME_MODE_VERC)||(eMode == REG_ZME_MODE_VER)||(eMode == REG_ZME_MODE_ALL))
            {
                VPSS_VHD_VSP.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_VHD_VSP.u32));
                VPSS_VHD_VSP.bits.vchmsc_en = bEnable;
                VPSS_REG_RegWrite(&(pstReg->VPSS_VHD_VSP.u32), VPSS_VHD_VSP.u32); 
            }
            break;
        case VPSS_REG_SD://SD
            if((eMode == REG_ZME_MODE_HORL )||(eMode == REG_ZME_MODE_HOR)||(eMode == REG_ZME_MODE_ALL))
            {
                VPSS_VSD_HSP.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_VSD_HSP.u32));
                VPSS_VSD_HSP.bits.hlmsc_en = bEnable;
                VPSS_REG_RegWrite(&(pstReg->VPSS_VSD_HSP.u32), VPSS_VSD_HSP.u32); 
            }
            if((eMode == REG_ZME_MODE_HORC)||(eMode == REG_ZME_MODE_HOR)||(eMode == REG_ZME_MODE_ALL))
            {
                VPSS_VSD_HSP.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_VSD_HSP.u32));
                VPSS_VSD_HSP.bits.hchmsc_en = bEnable;
                VPSS_REG_RegWrite(&(pstReg->VPSS_VSD_HSP.u32), VPSS_VSD_HSP.u32); 
            }
            if((eMode == REG_ZME_MODE_VERL)||(eMode == REG_ZME_MODE_VER)||(eMode == REG_ZME_MODE_ALL))
            {
                VPSS_VSD_VSP.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_VSD_VSP.u32));
                VPSS_VSD_VSP.bits.vlmsc_en = bEnable;
                VPSS_REG_RegWrite(&(pstReg->VPSS_VSD_VSP.u32), VPSS_VSD_VSP.u32); 
            }
            if((eMode == REG_ZME_MODE_VERC)||(eMode == REG_ZME_MODE_VER)||(eMode == REG_ZME_MODE_ALL))
            {
                VPSS_VSD_VSP.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_VSD_VSP.u32));
                VPSS_VSD_VSP.bits.vchmsc_en = bEnable;
                VPSS_REG_RegWrite(&(pstReg->VPSS_VSD_VSP.u32), VPSS_VSD_VSP.u32); 
            }
            break;
        case VPSS_REG_STR://STR
            if((eMode == REG_ZME_MODE_HORL )||(eMode == REG_ZME_MODE_HOR)||(eMode == REG_ZME_MODE_ALL))
            {
                VPSS_STR_HSP.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_STR_HSP.u32));
                VPSS_STR_HSP.bits.hlmsc_en = bEnable;
                VPSS_REG_RegWrite(&(pstReg->VPSS_STR_HSP.u32), VPSS_STR_HSP.u32); 
            }
            if((eMode == REG_ZME_MODE_HORC)||(eMode == REG_ZME_MODE_HOR)||(eMode == REG_ZME_MODE_ALL))
            {
                VPSS_STR_HSP.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_STR_HSP.u32));
                VPSS_STR_HSP.bits.hchmsc_en = bEnable;
                VPSS_REG_RegWrite(&(pstReg->VPSS_STR_HSP.u32), VPSS_STR_HSP.u32); 
            }
            if((eMode == REG_ZME_MODE_VERL)||(eMode == REG_ZME_MODE_VER)||(eMode == REG_ZME_MODE_ALL))
            {
                VPSS_STR_VSP.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_STR_VSP.u32));
                VPSS_STR_VSP.bits.vlmsc_en = bEnable;
                VPSS_REG_RegWrite(&(pstReg->VPSS_STR_VSP.u32), VPSS_STR_VSP.u32); 
            }
            if((eMode == REG_ZME_MODE_VERC)||(eMode == REG_ZME_MODE_VER)||(eMode == REG_ZME_MODE_ALL))
            {
                VPSS_STR_VSP.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_STR_VSP.u32));
                VPSS_STR_VSP.bits.vchmsc_en = bEnable;
                VPSS_REG_RegWrite(&(pstReg->VPSS_STR_VSP.u32), VPSS_STR_VSP.u32); 
            }
            break;
        default:
            VPSS_FATAL("REG ERROR\n");
            break;
    }
    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetZmeInSize(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32Height,HI_U32 u32Width)
{
    VPSS_REG_S *pstReg;
    U_VPSS_VSD_ZMEIRESO VPSS_VSD_ZMEIRESO;
    U_VPSS_VHD_ZMEIRESO VPSS_VHD_ZMEIRESO;
    
    pstReg = (VPSS_REG_S *)u32AppAddr;
    switch(ePort)
    {
        case VPSS_REG_HD:
            VPSS_VHD_ZMEIRESO.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_VHD_ZMEIRESO.u32));
            VPSS_VHD_ZMEIRESO.bits.ih = u32Height - 1;
            VPSS_VHD_ZMEIRESO.bits.iw = u32Width - 1;
            VPSS_REG_RegWrite(&(pstReg->VPSS_VHD_ZMEIRESO.u32), VPSS_VHD_ZMEIRESO.u32);
            break;
        case VPSS_REG_SD://SD
            VPSS_VSD_ZMEIRESO.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_VSD_ZMEIRESO.u32));
            VPSS_VSD_ZMEIRESO.bits.ih = u32Height - 1;
            VPSS_VSD_ZMEIRESO.bits.iw = u32Width - 1;
            VPSS_REG_RegWrite(&(pstReg->VPSS_VSD_ZMEIRESO.u32), VPSS_VSD_ZMEIRESO.u32);
            break;
        default:
            VPSS_FATAL("REG ERROR\n");
            break;
    }
    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetZmeOutSize(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_U32 u32Height,HI_U32 u32Width)
{
    U_VPSS_VSD_ZMEORESO VPSS_VSD_ZMEORESO;
    U_VPSS_VHD_ZMEORESO VPSS_VHD_ZMEORESO;
    U_VPSS_STR_ZMEORESO VPSS_STR_ZMEORESO;
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S *)u32AppAddr;
    switch(ePort)
    {
        case VPSS_REG_HD:
            VPSS_VHD_ZMEORESO.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_VHD_ZMEORESO.u32));
            VPSS_VHD_ZMEORESO.bits.oh = u32Height - 1;
            VPSS_VHD_ZMEORESO.bits.ow = u32Width - 1;
            VPSS_REG_RegWrite(&(pstReg->VPSS_VHD_ZMEORESO.u32), VPSS_VHD_ZMEORESO.u32);
            break;
        case VPSS_REG_SD://SD
            VPSS_VSD_ZMEORESO.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_VSD_ZMEORESO.u32));
            VPSS_VSD_ZMEORESO.bits.oh = u32Height - 1;
            VPSS_VSD_ZMEORESO.bits.ow = u32Width - 1;
            VPSS_REG_RegWrite(&(pstReg->VPSS_VSD_ZMEORESO.u32), VPSS_VSD_ZMEORESO.u32);
            break;
        case VPSS_REG_STR://STR
            VPSS_STR_ZMEORESO.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_STR_ZMEORESO.u32));
            VPSS_STR_ZMEORESO.bits.oh = u32Height - 1;
            VPSS_STR_ZMEORESO.bits.ow = u32Width - 1;
            VPSS_REG_RegWrite(&(pstReg->VPSS_STR_ZMEORESO.u32), VPSS_STR_ZMEORESO.u32);
            break;
        default:
            VPSS_FATAL("REG ERROR\n");
            break;
    }
    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetZmeFirEnable(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,
                                    REG_ZME_MODE_E eMode,HI_BOOL bEnable)
{
    U_VPSS_VHD_VSP VPSS_VHD_VSP;
    U_VPSS_VSD_VSP VPSS_VSD_VSP;
    U_VPSS_STR_VSP VPSS_STR_VSP;

    U_VPSS_VHD_HSP VPSS_VHD_HSP;
    U_VPSS_VSD_HSP VPSS_VSD_HSP;
    U_VPSS_STR_HSP VPSS_STR_HSP;
    
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S *)u32AppAddr;

    switch(ePort)
    {
        case VPSS_REG_HD:
            if((eMode ==  REG_ZME_MODE_HORL )||(eMode == REG_ZME_MODE_HOR)||(eMode == REG_ZME_MODE_ALL))
            {
                VPSS_VHD_HSP.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_VHD_HSP.u32));
                VPSS_VHD_HSP.bits.hlfir_en = bEnable;
                VPSS_REG_RegWrite(&(pstReg->VPSS_VHD_HSP.u32), VPSS_VHD_HSP.u32); 
            }
            if((eMode == REG_ZME_MODE_HORC)||(eMode == REG_ZME_MODE_HOR)||(eMode == REG_ZME_MODE_ALL))
            {
                VPSS_VHD_HSP.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_VHD_HSP.u32));
                VPSS_VHD_HSP.bits.hchfir_en = bEnable;
                VPSS_REG_RegWrite(&(pstReg->VPSS_VHD_HSP.u32), VPSS_VHD_HSP.u32); 
            }
            if((eMode == REG_ZME_MODE_VERL)||(eMode == REG_ZME_MODE_VER)||(eMode == REG_ZME_MODE_ALL))
            {
                VPSS_VHD_VSP.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_VHD_VSP.u32));
                VPSS_VHD_VSP.bits.vlfir_en = bEnable;
                VPSS_REG_RegWrite(&(pstReg->VPSS_VHD_VSP.u32), VPSS_VHD_VSP.u32); 
            }
            if((eMode == REG_ZME_MODE_VERC)||(eMode == REG_ZME_MODE_VER)||(eMode == REG_ZME_MODE_ALL))
            {
                VPSS_VHD_VSP.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_VHD_VSP.u32));
                VPSS_VHD_VSP.bits.vchfir_en = bEnable;
                VPSS_REG_RegWrite(&(pstReg->VPSS_VHD_VSP.u32), VPSS_VHD_VSP.u32); 
            }
            break;
        case VPSS_REG_SD://SD
            if((eMode == REG_ZME_MODE_HORL )||(eMode == REG_ZME_MODE_HOR)||(eMode == REG_ZME_MODE_ALL))
            {
                VPSS_VSD_HSP.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_VSD_HSP.u32));
                VPSS_VSD_HSP.bits.hlfir_en = bEnable;
                VPSS_REG_RegWrite(&(pstReg->VPSS_VSD_HSP.u32), VPSS_VSD_HSP.u32); 
            }
            if((eMode == REG_ZME_MODE_HORC)||(eMode == REG_ZME_MODE_HOR)||(eMode == REG_ZME_MODE_ALL))
            {
                VPSS_VSD_HSP.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_VSD_HSP.u32));
                VPSS_VSD_HSP.bits.hchfir_en = bEnable;
                VPSS_REG_RegWrite(&(pstReg->VPSS_VSD_HSP.u32), VPSS_VSD_HSP.u32); 
            }
            if((eMode == REG_ZME_MODE_VERL)||(eMode == REG_ZME_MODE_VER)||(eMode == REG_ZME_MODE_ALL))
            {
                VPSS_VSD_VSP.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_VSD_VSP.u32));
                VPSS_VSD_VSP.bits.vlfir_en = bEnable;
                VPSS_REG_RegWrite(&(pstReg->VPSS_VSD_VSP.u32), VPSS_VSD_VSP.u32); 
            }
            if((eMode == REG_ZME_MODE_VERC)||(eMode == REG_ZME_MODE_VER)||(eMode == REG_ZME_MODE_ALL))
            {
                VPSS_VSD_VSP.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_VSD_VSP.u32));
                VPSS_VSD_VSP.bits.vchfir_en = bEnable;
                VPSS_REG_RegWrite(&(pstReg->VPSS_VSD_VSP.u32), VPSS_VSD_VSP.u32); 
            }
            break;
        case VPSS_REG_STR://STR
            if((eMode == REG_ZME_MODE_HORL )||(eMode == REG_ZME_MODE_HOR)||(eMode == REG_ZME_MODE_ALL))
            {
                VPSS_STR_HSP.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_STR_HSP.u32));
                VPSS_STR_HSP.bits.hlfir_en = bEnable;
                VPSS_REG_RegWrite(&(pstReg->VPSS_STR_HSP.u32), VPSS_STR_HSP.u32); 
            }
            if((eMode == REG_ZME_MODE_HORC)||(eMode == REG_ZME_MODE_HOR)||(eMode == REG_ZME_MODE_ALL))
            {
                VPSS_STR_HSP.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_STR_HSP.u32));
                VPSS_STR_HSP.bits.hchfir_en = bEnable;
                VPSS_REG_RegWrite(&(pstReg->VPSS_STR_HSP.u32), VPSS_STR_HSP.u32); 
            }
            if((eMode == REG_ZME_MODE_VERL)||(eMode == REG_ZME_MODE_VER)||(eMode == REG_ZME_MODE_ALL))
            {
                VPSS_STR_VSP.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_STR_VSP.u32));
                VPSS_STR_VSP.bits.vlfir_en = bEnable;
                VPSS_REG_RegWrite(&(pstReg->VPSS_STR_VSP.u32), VPSS_STR_VSP.u32); 
            }
            if((eMode == REG_ZME_MODE_VERC)||(eMode == REG_ZME_MODE_VER)||(eMode == REG_ZME_MODE_ALL))
            {
                VPSS_STR_VSP.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_STR_VSP.u32));
                VPSS_STR_VSP.bits.vchfir_en = bEnable;
                VPSS_REG_RegWrite(&(pstReg->VPSS_STR_VSP.u32), VPSS_STR_VSP.u32); 
            }
            break;
        default:
            VPSS_FATAL("REG ERROR\n");
            break;
    }

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetZmeMidEnable(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,
                                    REG_ZME_MODE_E eMode,HI_BOOL bEnable)
{
    U_VPSS_VHD_VSP VPSS_VHD_VSP;
    U_VPSS_VSD_VSP VPSS_VSD_VSP;
    U_VPSS_STR_VSP VPSS_STR_VSP;

    U_VPSS_VHD_HSP VPSS_VHD_HSP;
    U_VPSS_VSD_HSP VPSS_VSD_HSP;
    U_VPSS_STR_HSP VPSS_STR_HSP;
    
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S *)u32AppAddr;

    switch(ePort)
    {
        case VPSS_REG_HD:
            if((eMode == REG_ZME_MODE_HORL)||(eMode == REG_ZME_MODE_HOR)||(eMode == REG_ZME_MODE_ALL))
            {
                VPSS_VHD_HSP.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_VHD_HSP.u32));
                VPSS_VHD_HSP.bits.hlmid_en = bEnable;
                VPSS_REG_RegWrite(&(pstReg->VPSS_VHD_HSP.u32), VPSS_VHD_HSP.u32); 
            }
            if((eMode ==  REG_ZME_MODE_HORC)||(eMode == REG_ZME_MODE_HOR)||(eMode == REG_ZME_MODE_ALL))
            {
                VPSS_VHD_HSP.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_VHD_HSP.u32));
                VPSS_VHD_HSP.bits.hchmid_en = bEnable;
                VPSS_REG_RegWrite(&(pstReg->VPSS_VHD_HSP.u32), VPSS_VHD_HSP.u32); 
            }
            if((eMode == REG_ZME_MODE_VERL)||(eMode == REG_ZME_MODE_VER)||(eMode == REG_ZME_MODE_ALL))
            {
                VPSS_VHD_VSP.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_VHD_VSP.u32));
                VPSS_VHD_VSP.bits.vlmid_en = bEnable;
                VPSS_REG_RegWrite(&(pstReg->VPSS_VHD_VSP.u32), VPSS_VHD_VSP.u32); 
            }
            if((eMode == REG_ZME_MODE_VERC)||(eMode == REG_ZME_MODE_VER)||(eMode == REG_ZME_MODE_ALL))
            {
                VPSS_VHD_VSP.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_VHD_VSP.u32));
                VPSS_VHD_VSP.bits.vchmid_en = bEnable;
                VPSS_REG_RegWrite(&(pstReg->VPSS_VHD_VSP.u32), VPSS_VHD_VSP.u32); 
            }
            break;
        case VPSS_REG_SD://SD
            if((eMode == REG_ZME_MODE_HORL)||(eMode == REG_ZME_MODE_HOR)||(eMode == REG_ZME_MODE_ALL))
            {
                VPSS_VSD_HSP.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_VSD_HSP.u32));
                VPSS_VSD_HSP.bits.hlmid_en = bEnable;
                VPSS_REG_RegWrite(&(pstReg->VPSS_VSD_HSP.u32), VPSS_VSD_HSP.u32); 
            }
            if((eMode == REG_ZME_MODE_HORC)||(eMode == REG_ZME_MODE_HOR)||(eMode == REG_ZME_MODE_ALL))
            {
                VPSS_VSD_HSP.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_VSD_HSP.u32));
                VPSS_VSD_HSP.bits.hchmid_en = bEnable;
                VPSS_REG_RegWrite(&(pstReg->VPSS_VSD_HSP.u32), VPSS_VSD_HSP.u32); 
            }
            if((eMode == REG_ZME_MODE_VERL)||(eMode == REG_ZME_MODE_VER)||(eMode == REG_ZME_MODE_ALL))
            {
                VPSS_VSD_VSP.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_VSD_VSP.u32));
                VPSS_VSD_VSP.bits.vlmid_en = bEnable;
                VPSS_REG_RegWrite(&(pstReg->VPSS_VSD_VSP.u32), VPSS_VSD_VSP.u32); 
            }
            if((eMode == REG_ZME_MODE_VERC)||(eMode == REG_ZME_MODE_VER)||(eMode == REG_ZME_MODE_ALL))
            {
                VPSS_VSD_VSP.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_VSD_VSP.u32));
                VPSS_VSD_VSP.bits.vchmid_en = bEnable;
                VPSS_REG_RegWrite(&(pstReg->VPSS_VSD_VSP.u32), VPSS_VSD_VSP.u32); 
            }
            break;
        case VPSS_REG_STR://STR
            if((eMode == REG_ZME_MODE_HORC)||(eMode == REG_ZME_MODE_HOR)||(eMode == REG_ZME_MODE_ALL))
            {
                VPSS_STR_HSP.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_STR_HSP.u32));
                VPSS_STR_HSP.bits.hchmid_en = bEnable;
                VPSS_REG_RegWrite(&(pstReg->VPSS_STR_HSP.u32), VPSS_STR_HSP.u32); 
            }
            if((eMode == REG_ZME_MODE_HORL)||(eMode == REG_ZME_MODE_HOR)||(eMode == REG_ZME_MODE_ALL))
            {
                VPSS_STR_HSP.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_STR_HSP.u32));
                VPSS_STR_HSP.bits.hlmid_en = bEnable;
                VPSS_REG_RegWrite(&(pstReg->VPSS_STR_HSP.u32), VPSS_STR_HSP.u32); 
            }
            if((eMode == REG_ZME_MODE_VERL)||(eMode == REG_ZME_MODE_VER)||(eMode == REG_ZME_MODE_ALL))
            {
                VPSS_STR_VSP.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_STR_VSP.u32));
                VPSS_STR_VSP.bits.vlmid_en = bEnable;
                VPSS_REG_RegWrite(&(pstReg->VPSS_STR_VSP.u32), VPSS_STR_VSP.u32); 
            }
            if((eMode == REG_ZME_MODE_VERC)||(eMode == REG_ZME_MODE_VER)||(eMode == REG_ZME_MODE_ALL))
            {
                VPSS_STR_VSP.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_STR_VSP.u32));
                VPSS_STR_VSP.bits.vchmid_en = bEnable;
                VPSS_REG_RegWrite(&(pstReg->VPSS_STR_VSP.u32), VPSS_STR_VSP.u32); 
            }
            break;
        default:
            VPSS_FATAL("REG ERROR\n");
            break;
    }

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetZmePhase(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,
                                    REG_ZME_MODE_E eMode,HI_S32 s32Phase)
{
    U_VPSS_VHD_HLOFFSET VPSS_VHD_HLOFFSET;
    U_VPSS_VHD_HCOFFSET VPSS_VHD_HCOFFSET;
    U_VPSS_VHD_VOFFSET VPSS_VHD_VOFFSET;
    
    U_VPSS_VSD_HLOFFSET VPSS_VSD_HLOFFSET;
    U_VPSS_VSD_HCOFFSET VPSS_VSD_HCOFFSET;
    U_VPSS_VSD_VOFFSET VPSS_VSD_VOFFSET;
    
    U_VPSS_STR_HLOFFSET VPSS_STR_HLOFFSET;
    U_VPSS_STR_HCOFFSET VPSS_STR_HCOFFSET;
    U_VPSS_STR_VOFFSET VPSS_STR_VOFFSET;
    
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S *)u32AppAddr;

    switch(ePort)
    {
        case VPSS_REG_HD:
            if (eMode == REG_ZME_MODE_HORC)
            {
                VPSS_VHD_HCOFFSET.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_VHD_HCOFFSET.u32));
                VPSS_VHD_HCOFFSET.bits.hor_coffset = s32Phase;
                VPSS_REG_RegWrite(&(pstReg->VPSS_VHD_HCOFFSET.u32), VPSS_VHD_HCOFFSET.u32); 
            }
            else if(eMode == REG_ZME_MODE_HORL)
            {
                VPSS_VHD_HLOFFSET.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_VHD_HLOFFSET.u32));
                VPSS_VHD_HLOFFSET.bits.hor_loffset = s32Phase;
                VPSS_REG_RegWrite(&(pstReg->VPSS_VHD_HLOFFSET.u32), VPSS_VHD_HLOFFSET.u32); 
            }
            else if(eMode == REG_ZME_MODE_VERC)
            {
                VPSS_VHD_VOFFSET.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_VHD_VOFFSET.u32));
                VPSS_VHD_VOFFSET.bits.vchroma_offset = s32Phase;
                VPSS_REG_RegWrite(&(pstReg->VPSS_VHD_VOFFSET.u32), VPSS_VHD_VOFFSET.u32); 
            }
            else if(eMode == REG_ZME_MODE_VERL)
            {
                VPSS_VHD_VOFFSET.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_VHD_VOFFSET.u32));
                VPSS_VHD_VOFFSET.bits.vluma_offset = s32Phase;
                VPSS_REG_RegWrite(&(pstReg->VPSS_VHD_VOFFSET.u32), VPSS_VHD_VOFFSET.u32); 
            }
            else
            {
                VPSS_FATAL("REG ERROR\n");
            }
            break;
        case VPSS_REG_SD://SD
            if (eMode == REG_ZME_MODE_HORC)
            {
                VPSS_VSD_HCOFFSET.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_VSD_HCOFFSET.u32));
                VPSS_VSD_HCOFFSET.bits.hor_coffset = s32Phase;
                VPSS_REG_RegWrite(&(pstReg->VPSS_VSD_HCOFFSET.u32), VPSS_VSD_HCOFFSET.u32); 
            }
            else if(eMode == REG_ZME_MODE_HORL)
            {
                VPSS_VSD_HLOFFSET.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_VSD_HLOFFSET.u32));
                VPSS_VSD_HLOFFSET.bits.hor_loffset = s32Phase;
                VPSS_REG_RegWrite(&(pstReg->VPSS_VSD_HLOFFSET.u32), VPSS_VSD_HLOFFSET.u32); 
            }
            else if(eMode == REG_ZME_MODE_VERC)
            {
                VPSS_VSD_VOFFSET.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_VSD_VOFFSET.u32));
                VPSS_VSD_VOFFSET.bits.vchroma_offset = s32Phase;
                VPSS_REG_RegWrite(&(pstReg->VPSS_VSD_VOFFSET.u32), VPSS_VSD_VOFFSET.u32); 
            }
            else if(eMode == REG_ZME_MODE_VERL)
            {
                VPSS_VSD_VOFFSET.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_VSD_VOFFSET.u32));
                VPSS_VSD_VOFFSET.bits.vluma_offset = s32Phase;
                VPSS_REG_RegWrite(&(pstReg->VPSS_VSD_VOFFSET.u32), VPSS_VSD_VOFFSET.u32); 
            }
            else
            {
                VPSS_FATAL("REG ERROR\n");
            }
            break;
        case VPSS_REG_STR://STR
           if (eMode == REG_ZME_MODE_HORC)
            {
                VPSS_STR_HCOFFSET.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_STR_HCOFFSET.u32));
                VPSS_STR_HCOFFSET.bits.hor_coffset = s32Phase;
                VPSS_REG_RegWrite(&(pstReg->VPSS_STR_HCOFFSET.u32), VPSS_STR_HCOFFSET.u32); 
            }
            else if(eMode == REG_ZME_MODE_HORL)
            {
                VPSS_STR_HLOFFSET.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_STR_HLOFFSET.u32));
                VPSS_STR_HLOFFSET.bits.hor_loffset = s32Phase;
                VPSS_REG_RegWrite(&(pstReg->VPSS_STR_HLOFFSET.u32), VPSS_STR_HLOFFSET.u32); 
            }
            else if(eMode == REG_ZME_MODE_VERC)
            {
                VPSS_STR_VOFFSET.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_STR_VOFFSET.u32));
                VPSS_STR_VOFFSET.bits.vchroma_offset = s32Phase;
                VPSS_REG_RegWrite(&(pstReg->VPSS_STR_VOFFSET.u32), VPSS_STR_VOFFSET.u32); 
            }
            else if(eMode == REG_ZME_MODE_VERL)
            {
                VPSS_STR_VOFFSET.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_STR_VOFFSET.u32));
                VPSS_STR_VOFFSET.bits.vluma_offset = s32Phase;
                VPSS_REG_RegWrite(&(pstReg->VPSS_STR_VOFFSET.u32), VPSS_STR_VOFFSET.u32); 
            }
            else
            {
                VPSS_FATAL("REG ERROR\n");
            }
            break;
        default:
            VPSS_FATAL("REG ERROR\n");
            break;
    }

    return HI_SUCCESS;
}                                    
HI_S32 VPSS_REG_SetZmeRatio(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,
                                    REG_ZME_MODE_E eMode,HI_U32 u32Ratio)
{
    U_VPSS_VHD_HSP VPSS_VHD_HSP;
    U_VPSS_VHD_VSR VPSS_VHD_VSR;

    U_VPSS_VSD_HSP VPSS_VSD_HSP;
    U_VPSS_VSD_VSR VPSS_VSD_VSR;

    U_VPSS_STR_HSP VPSS_STR_HSP;
    U_VPSS_STR_VSP VPSS_STR_VSP;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S *)u32AppAddr;

    switch(ePort)
    {
        case VPSS_REG_HD:
            if (eMode == REG_ZME_MODE_HOR)
            {
                VPSS_VHD_HSP.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_VHD_HSP.u32));
                VPSS_VHD_HSP.bits.hratio = u32Ratio;
                VPSS_REG_RegWrite(&(pstReg->VPSS_VHD_HSP.u32), VPSS_VHD_HSP.u32); 
            }
            else
            {
                VPSS_VHD_VSR.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_VHD_VSR.u32));
                VPSS_VHD_VSR.bits.vratio = u32Ratio;
                VPSS_REG_RegWrite(&(pstReg->VPSS_VHD_VSR.u32), VPSS_VHD_VSR.u32); 
            }
            break;
        case VPSS_REG_SD://SD
            if (eMode == REG_ZME_MODE_HOR)
            {
                VPSS_VSD_HSP.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_VSD_HSP.u32));
                VPSS_VSD_HSP.bits.hratio = u32Ratio;
                VPSS_REG_RegWrite(&(pstReg->VPSS_VSD_HSP.u32), VPSS_VSD_HSP.u32); 
            }
            else
            {
                VPSS_VSD_VSR.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_VSD_VSR.u32));
                VPSS_VSD_VSR.bits.vratio = u32Ratio;
                VPSS_REG_RegWrite(&(pstReg->VPSS_VSD_VSR.u32), VPSS_VSD_VSR.u32); 
            }
            break;
        case VPSS_REG_STR://STR
           if (eMode == REG_ZME_MODE_HOR)
            {
                VPSS_STR_HSP.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_STR_HSP.u32));
                VPSS_STR_HSP.bits.hratio = u32Ratio;
                VPSS_REG_RegWrite(&(pstReg->VPSS_STR_HSP.u32), VPSS_STR_HSP.u32); 
            }
            else
            {
                VPSS_STR_VSP.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_STR_VSP.u32));
                VPSS_STR_VSP.bits.vratio = u32Ratio;
                VPSS_REG_RegWrite(&(pstReg->VPSS_STR_VSP.u32), VPSS_STR_VSP.u32); 
            }
            break;
        default:
            VPSS_FATAL("REG ERROR\n");
            break;
    }

    return HI_SUCCESS;
}                                    
HI_S32 VPSS_REG_SetZmeHfirOrder(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_BOOL bVfirst)
{
    U_VPSS_VSD_HSP VPSS_VSD_HSP;

    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S *)u32AppAddr;
    
    switch(ePort)
    {
        case VPSS_REG_SD://SD
            VPSS_VSD_HSP.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_VSD_HSP.u32));
            VPSS_VSD_HSP.bits.hfir_order = bVfirst;
            VPSS_REG_RegWrite(&(pstReg->VPSS_VSD_HSP.u32), VPSS_VSD_HSP.u32); 
            break;
        default:
            VPSS_FATAL("REG ERROR\n");
            break;
    }

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetZmeInFmt(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_DRV_PIX_FORMAT_E eFormat)
{
    U_VPSS_VHD_VSP VPSS_VHD_VSP;

    U_VPSS_VSD_VSP VPSS_VSD_VSP;

    HI_U32 u32Format;
    
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S *)u32AppAddr;

    switch(eFormat)
    {
        case HI_DRV_PIX_FMT_NV12:
            u32Format = 0x1;
            break;
        case HI_DRV_PIX_FMT_NV16:
            u32Format = 0x0;
            break;
        default:
            VPSS_FATAL("REG ERROR\n");
            return HI_FAILURE;
    }
    
    switch(ePort)
    {
        case VPSS_REG_HD:
            VPSS_VHD_VSP.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_VHD_VSP.u32));
            VPSS_VHD_VSP.bits.zme_in_fmt = u32Format;
            VPSS_REG_RegWrite(&(pstReg->VPSS_VHD_VSP.u32), VPSS_VHD_VSP.u32); 
            
            break;
        case VPSS_REG_SD://SD
            VPSS_VSD_VSP.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_VSD_VSP.u32));
            VPSS_VSD_VSP.bits.zme_in_fmt = u32Format;
            VPSS_REG_RegWrite(&(pstReg->VPSS_VSD_VSP.u32), VPSS_VSD_VSP.u32); 
            break;
        default:
            VPSS_FATAL("REG ERROR\n");
            break;
    }

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetZmeOutFmt(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,HI_DRV_PIX_FORMAT_E eFormat)
{
    U_VPSS_VHD_VSP VPSS_VHD_VSP;

    U_VPSS_VSD_VSP VPSS_VSD_VSP;

    U_VPSS_STR_VSP VPSS_STR_VSP;

    HI_U32 u32Format;
    
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S *)u32AppAddr;

    switch(eFormat)
    {
        case HI_DRV_PIX_FMT_NV12:
            u32Format = 0x1;
            break;
        case HI_DRV_PIX_FMT_NV16:
            u32Format = 0x0;
            break;
        default:
            VPSS_FATAL("REG ERROR\n");
            return HI_FAILURE;
    }
    
    switch(ePort)
    {
        case VPSS_REG_HD:
            VPSS_VHD_VSP.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_VHD_VSP.u32));
            VPSS_VHD_VSP.bits.zme_out_fmt = u32Format;
            VPSS_REG_RegWrite(&(pstReg->VPSS_VHD_VSP.u32), VPSS_VHD_VSP.u32); 
            
            break;
        case VPSS_REG_SD://SD
            VPSS_VSD_VSP.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_VSD_VSP.u32));
            VPSS_VSD_VSP.bits.zme_out_fmt = u32Format;
            VPSS_REG_RegWrite(&(pstReg->VPSS_VSD_VSP.u32), VPSS_VSD_VSP.u32); 
            break;
        case VPSS_REG_STR://STR
            VPSS_STR_VSP.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_STR_VSP.u32));
            VPSS_STR_VSP.bits.zme_out_fmt = u32Format;
            VPSS_REG_RegWrite(&(pstReg->VPSS_STR_VSP.u32), VPSS_STR_VSP.u32); 
            break;
        default:
            VPSS_FATAL("REG ERROR\n");
            break;
    }
    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetZmeCoefAddr(HI_U32 u32AppAddr,VPSS_REG_PORT_E ePort,
                                    REG_ZME_MODE_E eMode,HI_U32 u32Addr)
{

    HI_U32 VPSS_VHD_ZME_LHADDR;
    HI_U32 VPSS_VHD_ZME_LVADDR;
    HI_U32 VPSS_VHD_ZME_CHADDR;
    HI_U32 VPSS_VHD_ZME_CVADDR;
    
    HI_U32 VPSS_VSD_ZME_LHADDR;
    HI_U32 VPSS_VSD_ZME_LVADDR;
    HI_U32 VPSS_VSD_ZME_CHADDR;
    HI_U32 VPSS_VSD_ZME_CVADDR;
    
    HI_U32 VPSS_STR_ZME_LHADDR;
    HI_U32 VPSS_STR_ZME_LVADDR;
    HI_U32 VPSS_STR_ZME_CHADDR;
    HI_U32 VPSS_STR_ZME_CVADDR;
    
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S *)u32AppAddr;

    switch(ePort)
    {
        case VPSS_REG_HD:
            if (eMode == REG_ZME_MODE_HORC)
            {
                VPSS_VHD_ZME_CHADDR = VPSS_REG_RegRead(&(pstReg->VPSS_VHD_ZME_CHADDR));
                VPSS_VHD_ZME_CHADDR = u32Addr;
                VPSS_REG_RegWrite(&(pstReg->VPSS_VHD_ZME_CHADDR), VPSS_VHD_ZME_CHADDR); 
            }
            else if(eMode == REG_ZME_MODE_HORL)
            {
                VPSS_VHD_ZME_LHADDR = VPSS_REG_RegRead(&(pstReg->VPSS_VHD_ZME_LHADDR));
                VPSS_VHD_ZME_LHADDR = u32Addr;
                VPSS_REG_RegWrite(&(pstReg->VPSS_VHD_ZME_LHADDR), VPSS_VHD_ZME_LHADDR);  
            }
            else if(eMode == REG_ZME_MODE_VERC)
            {
                VPSS_VHD_ZME_CVADDR = VPSS_REG_RegRead(&(pstReg->VPSS_VHD_ZME_CVADDR));
                VPSS_VHD_ZME_CVADDR = u32Addr;
                VPSS_REG_RegWrite(&(pstReg->VPSS_VHD_ZME_CVADDR), VPSS_VHD_ZME_CVADDR);  
            }
            else if(eMode == REG_ZME_MODE_VERL)
            {
                VPSS_VHD_ZME_LVADDR = VPSS_REG_RegRead(&(pstReg->VPSS_VHD_ZME_LVADDR));
                VPSS_VHD_ZME_LVADDR = u32Addr;
                VPSS_REG_RegWrite(&(pstReg->VPSS_VHD_ZME_LVADDR), VPSS_VHD_ZME_LVADDR);
            }
            else
            {
                VPSS_FATAL("REG ERROR\n");
            }
            break;
        case VPSS_REG_SD://SD
            if (eMode == REG_ZME_MODE_HORC)
            {
                VPSS_VSD_ZME_CHADDR = VPSS_REG_RegRead(&(pstReg->VPSS_VSD_ZME_CHADDR));
                VPSS_VSD_ZME_CHADDR = u32Addr;
                VPSS_REG_RegWrite(&(pstReg->VPSS_VSD_ZME_CHADDR), VPSS_VSD_ZME_CHADDR); 
            }
            else if(eMode == REG_ZME_MODE_HORL)
            {
                VPSS_VSD_ZME_LHADDR = VPSS_REG_RegRead(&(pstReg->VPSS_VSD_ZME_LHADDR));
                VPSS_VSD_ZME_LHADDR = u32Addr;
                VPSS_REG_RegWrite(&(pstReg->VPSS_VSD_ZME_LHADDR), VPSS_VSD_ZME_LHADDR);  
            }
            else if(eMode == REG_ZME_MODE_VERC)
            {
                VPSS_VSD_ZME_CVADDR = VPSS_REG_RegRead(&(pstReg->VPSS_VSD_ZME_CVADDR));
                VPSS_VSD_ZME_CVADDR = u32Addr;
                VPSS_REG_RegWrite(&(pstReg->VPSS_VSD_ZME_CVADDR), VPSS_VSD_ZME_CVADDR);  
            }
            else if(eMode == REG_ZME_MODE_VERL)
            {
                VPSS_VSD_ZME_LVADDR = VPSS_REG_RegRead(&(pstReg->VPSS_VSD_ZME_LVADDR));
                VPSS_VSD_ZME_LVADDR = u32Addr;
                VPSS_REG_RegWrite(&(pstReg->VPSS_VSD_ZME_LVADDR), VPSS_VSD_ZME_LVADDR);
            }
            else
            {
                VPSS_FATAL("REG ERROR\n");
            }
            break;
        case VPSS_REG_STR://STR
            if (eMode == REG_ZME_MODE_HORC)
            {
                VPSS_STR_ZME_CHADDR = VPSS_REG_RegRead(&(pstReg->VPSS_STR_ZME_CHADDR));
                VPSS_STR_ZME_CHADDR = u32Addr;
                VPSS_REG_RegWrite(&(pstReg->VPSS_STR_ZME_CHADDR), VPSS_STR_ZME_CHADDR); 
            }
            else if(eMode == REG_ZME_MODE_HORL)
            {
                VPSS_STR_ZME_LHADDR = VPSS_REG_RegRead(&(pstReg->VPSS_STR_ZME_LHADDR));
                VPSS_STR_ZME_LHADDR = u32Addr;
                VPSS_REG_RegWrite(&(pstReg->VPSS_STR_ZME_LHADDR), VPSS_STR_ZME_LHADDR);  
            }
            else if(eMode == REG_ZME_MODE_VERC)
            {
                VPSS_STR_ZME_CVADDR = VPSS_REG_RegRead(&(pstReg->VPSS_STR_ZME_CVADDR));
                VPSS_STR_ZME_CVADDR = u32Addr;
                VPSS_REG_RegWrite(&(pstReg->VPSS_STR_ZME_CVADDR), VPSS_STR_ZME_CVADDR);  
            }
            else if(eMode == REG_ZME_MODE_VERL)
            {
                VPSS_STR_ZME_LVADDR = VPSS_REG_RegRead(&(pstReg->VPSS_STR_ZME_LVADDR));
                VPSS_STR_ZME_LVADDR = u32Addr;
                VPSS_REG_RegWrite(&(pstReg->VPSS_STR_ZME_LVADDR), VPSS_STR_ZME_LVADDR);
            }
            else
            {
                VPSS_FATAL("REG ERROR\n");
            }
            break;
        default:
            VPSS_FATAL("REG ERROR\n");
            break;
    }

    return HI_SUCCESS;
}                                    
                                    
/********************************/

/*入网指标DET相关操作*/
/********************************/
HI_VOID VPSS_REG_SetDetEn(HI_U32 u32AppAddr,HI_BOOL bEnable)
{
    U_VPSS_CTRL VPSS_CTRL;
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)u32AppAddr;
    
    VPSS_CTRL.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_CTRL.u32));
    VPSS_CTRL.bits.str_det_en = bEnable;
    VPSS_REG_RegWrite(&(pstReg->VPSS_CTRL.u32), VPSS_CTRL.u32);
    return;
}
HI_VOID VPSS_REG_SetDetMode(HI_U32 u32AppAddr,HI_U32 u32Mode)
{
    U_STR_DET_VIDCTRL STR_DET_VIDCTRL;
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)u32AppAddr;
    STR_DET_VIDCTRL.u32 = VPSS_REG_RegRead(&(pstReg->STR_DET_VIDCTRL.u32));
    STR_DET_VIDCTRL.bits.vid_mode = u32Mode;
    VPSS_REG_RegWrite(&(pstReg->STR_DET_VIDCTRL.u32), STR_DET_VIDCTRL.u32);

    return;
}
HI_VOID VPSS_REG_SetDetBlk(HI_U32 u32AppAddr,HI_U32 blk_id, HI_U32 *pu32Addr)
{
    U_STR_DET_VIDBLKPOS0_1 STR_DET_VIDBLKPOS0_1;
    U_STR_DET_VIDBLKPOS2_3 STR_DET_VIDBLKPOS2_3;
    U_STR_DET_VIDBLKPOS4_5 STR_DET_VIDBLKPOS4_5;
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)u32AppAddr;
    switch (blk_id)
    {
        case 0:
        {
            STR_DET_VIDBLKPOS0_1.u32 = VPSS_REG_RegRead(&(pstReg->STR_DET_VIDBLKPOS0_1.u32));
            
            STR_DET_VIDBLKPOS0_1.bits.blk0_xlpos = pu32Addr[0];
            STR_DET_VIDBLKPOS0_1.bits.blk0_ylpos = pu32Addr[1];

            VPSS_REG_RegWrite(&(pstReg->STR_DET_VIDBLKPOS0_1.u32), STR_DET_VIDBLKPOS0_1.u32);
            break;
        }
        case 1:
        {
            STR_DET_VIDBLKPOS0_1.u32 = VPSS_REG_RegRead(&(pstReg->STR_DET_VIDBLKPOS0_1.u32));
            
            STR_DET_VIDBLKPOS0_1.bits.blk1_xlpos = pu32Addr[0];
            STR_DET_VIDBLKPOS0_1.bits.blk1_ylpos = pu32Addr[1];

            VPSS_REG_RegWrite(&(pstReg->STR_DET_VIDBLKPOS0_1.u32), STR_DET_VIDBLKPOS0_1.u32);
            
            break;
        }
        case 2:
        {
            STR_DET_VIDBLKPOS2_3.u32 = VPSS_REG_RegRead(&(pstReg->STR_DET_VIDBLKPOS2_3.u32));
            
            STR_DET_VIDBLKPOS2_3.bits.blk2_xlpos = pu32Addr[0];
            STR_DET_VIDBLKPOS2_3.bits.blk2_ylpos = pu32Addr[1];

            VPSS_REG_RegWrite(&(pstReg->STR_DET_VIDBLKPOS2_3.u32), STR_DET_VIDBLKPOS2_3.u32);
            break;
        }
        case 3:
        {
            STR_DET_VIDBLKPOS2_3.u32 = VPSS_REG_RegRead(&(pstReg->STR_DET_VIDBLKPOS2_3.u32));
            
            STR_DET_VIDBLKPOS2_3.bits.blk3_xlpos = pu32Addr[0];
            STR_DET_VIDBLKPOS2_3.bits.blk3_ylpos = pu32Addr[1];

            VPSS_REG_RegWrite(&(pstReg->STR_DET_VIDBLKPOS2_3.u32), STR_DET_VIDBLKPOS2_3.u32);
            
            break;
        }
        case 4:
        {
            STR_DET_VIDBLKPOS4_5.u32 = VPSS_REG_RegRead(&(pstReg->STR_DET_VIDBLKPOS4_5.u32));
            
            STR_DET_VIDBLKPOS4_5.bits.blk4_xlpos = pu32Addr[0];
            STR_DET_VIDBLKPOS4_5.bits.blk4_ylpos = pu32Addr[1];

            VPSS_REG_RegWrite(&(pstReg->STR_DET_VIDBLKPOS4_5.u32), STR_DET_VIDBLKPOS4_5.u32);
            break;
        }
        case 5:
        {
            STR_DET_VIDBLKPOS4_5.u32 = VPSS_REG_RegRead(&(pstReg->STR_DET_VIDBLKPOS4_5.u32));
            
            STR_DET_VIDBLKPOS4_5.bits.blk5_xlpos = pu32Addr[0];
            STR_DET_VIDBLKPOS4_5.bits.blk5_ylpos = pu32Addr[1];

            VPSS_REG_RegWrite(&(pstReg->STR_DET_VIDBLKPOS4_5.u32), STR_DET_VIDBLKPOS4_5.u32);
            
            break;
        }

        default:
        {
            VPSS_FATAL("Error! Wrong Vou_SetViDetBlk() ID Select\n");
            return ;
        }
    }

    return ;

}
HI_VOID VPSS_REG_GetDetPixel(HI_U32 u32AppAddr,HI_U32 BlkNum, HI_U8* pstData)
{

    HI_U32  pixdata;
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)u32AppAddr;
    
    if (BlkNum > 5)
    {
        VPSS_FATAL("Error! Vou_GetDetPixel() Selected Wrong BLKNUM!\n");
        return ;
    }

    pixdata = VPSS_REG_RegRead((&(pstReg->STR_DET_VIDBLK0TOL0.u32) + BlkNum * 2 ));

    pstData[0] = (pixdata & 0xff);
    pstData[1] = (pixdata & 0xff00) >> 8;
    pstData[2] = (pixdata & 0xff0000) >> 16;
    pstData[3] = (pixdata & 0xff000000) >> 24;
    pixdata = VPSS_REG_RegRead((&(pstReg->STR_DET_VIDBLK0TOL0.u32)) + (BlkNum * 2  + 1) );

    pstData[4] = (pixdata & 0xff);
    pstData[5] = (pixdata & 0xff00) >> 8;
    pstData[6] = (pixdata & 0xff0000) >> 16;
    pstData[7] = (pixdata & 0xff000000) >> 24;

    return ;


}
/********************************/

/*DEI相关操作*/
/*************************************************************************************************/

/*DEI*/
HI_S32 VPSS_REG_EnDei(HI_U32 u32AppAddr,HI_BOOL bEnDei)
{
    U_VPSS_CTRL VPSS_CTRL;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;
    
    VPSS_CTRL.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_CTRL.u32));
    VPSS_CTRL.bits.dei_en = bEnDei;
    VPSS_REG_RegWrite(&(pstReg->VPSS_CTRL.u32), VPSS_CTRL.u32);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetDeiTopFirst(HI_U32 u32AppAddr,HI_BOOL bTopFirst)
{
    U_VPSS_CTRL VPSS_CTRL;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;
    
    VPSS_CTRL.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_CTRL.u32));
    VPSS_CTRL.bits.bfield_first = !bTopFirst;
    VPSS_REG_RegWrite(&(pstReg->VPSS_CTRL.u32), VPSS_CTRL.u32);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetDeiFieldMode(HI_U32 u32AppAddr,HI_BOOL bBottom)
{
    U_VPSS_CTRL VPSS_CTRL;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;
    
    VPSS_CTRL.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_CTRL.u32));
    VPSS_CTRL.bits.bfield_mode = bBottom;
    VPSS_REG_RegWrite(&(pstReg->VPSS_CTRL.u32), VPSS_CTRL.u32);

    return HI_SUCCESS;
    
}
HI_S32 VPSS_REG_SetDeiAddr(HI_U32 u32AppAddr,REG_FIELDPOS_E eField,HI_U32 u32YAddr,HI_U32 u32CAddr)
{

    // TODO: 确认DEI时，前后几场地址怎么配
    HI_U32 VPSS_LASTYADDR;
    HI_U32 VPSS_LASTCBADDR;
    
    HI_U32 VPSS_NEXTYADDR;
    HI_U32 VPSS_NEXTCBADDR;
    
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;

    switch(eField)
    {   
        case LAST_FIELD:
            VPSS_LASTYADDR = u32YAddr;
            VPSS_LASTCBADDR = u32CAddr;
            VPSS_REG_RegWrite(&(pstReg->VPSS_LASTYADDR), VPSS_LASTYADDR); 
            VPSS_REG_RegWrite(&(pstReg->VPSS_LASTCBADDR), VPSS_LASTCBADDR); 
            break;
        case NEXT1_FIELD:
            VPSS_NEXTYADDR = u32YAddr;
            VPSS_NEXTCBADDR = u32CAddr;
            VPSS_REG_RegWrite(&(pstReg->VPSS_NEXTYADDR), VPSS_NEXTYADDR); 
            VPSS_REG_RegWrite(&(pstReg->VPSS_NEXTCBADDR), VPSS_NEXTCBADDR); 
            break;
        default:
            VPSS_FATAL("REG ERROR\n");

    }

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetDeiStride(HI_U32 u32AppAddr,REG_FIELDPOS_E eField,HI_U32 u32YStride,HI_U32 u32CStride)
{
    U_VPSS_LASTSTRIDE   VPSS_LASTSTRIDE;
    U_VPSS_NEXTSTRIDE VPSS_NEXTSTRIDE;
    
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;

    switch(eField)
    {   
        case LAST_FIELD:
            VPSS_LASTSTRIDE.bits.lasty_stride= u32YStride;
            VPSS_LASTSTRIDE.bits.lastc_stride= u32CStride;
            VPSS_REG_RegWrite(&(pstReg->VPSS_LASTSTRIDE.u32), VPSS_LASTSTRIDE.u32); 
            break;
        case NEXT1_FIELD:
            VPSS_NEXTSTRIDE.bits.nexty_stride= u32YStride;
            VPSS_NEXTSTRIDE.bits.nextc_stride= u32CStride;

            VPSS_REG_RegWrite(&(pstReg->VPSS_NEXTSTRIDE.u32), VPSS_NEXTSTRIDE.u32); 
            break;
        default:
            VPSS_FATAL("REG ERROR\n");

    }

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetModeEn(HI_U32 u32AppAddr,REG_DIE_MODE_E eDieMode,HI_BOOL bEnMode)
{
    // TODO: CV200逻辑去掉了色度和亮度的开关，只保留了总开关
    #if 0
    U_VPSS_DIECTRL VPSS_DIECTRL;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;

    VPSS_DIECTRL.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_DIECTRL.u32));
    if (eDieMode == REG_DIE_MODE_CHROME || eDieMode == REG_DIE_MODE_ALL)
    {
        VPSS_DIECTRL.bits.die_chroma_en = bEnMode;
    }
    if (eDieMode == REG_DIE_MODE_LUMA || eDieMode == REG_DIE_MODE_ALL)
    {
        VPSS_DIECTRL.bits.die_luma_en = bEnMode;
    }
    VPSS_REG_RegWrite(&(pstReg->VPSS_DIECTRL.u32), VPSS_DIECTRL.u32);
    #endif
    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetOutSel(HI_U32 u32AppAddr,REG_DIE_MODE_E eDieMode,HI_BOOL bEnMode)
{
    U_VPSS_DIECTRL VPSS_DIECTRL;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;

    VPSS_DIECTRL.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_DIECTRL.u32));
    if (eDieMode == REG_DIE_MODE_CHROME || eDieMode == REG_DIE_MODE_ALL)
    {
        VPSS_DIECTRL.bits.die_out_sel_c = bEnMode;
    }
    if (eDieMode == REG_DIE_MODE_LUMA || eDieMode == REG_DIE_MODE_ALL)
    {
        VPSS_DIECTRL.bits.die_out_sel_l = bEnMode;
    }
    VPSS_REG_RegWrite(&(pstReg->VPSS_DIECTRL.u32), VPSS_DIECTRL.u32);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetMode(HI_U32 u32AppAddr,REG_DIE_MODE_E eDieMode,HI_U32  u32Mode)
{
    U_VPSS_DIECTRL VPSS_DIECTRL;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;

    VPSS_DIECTRL.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_DIECTRL.u32));
    if (eDieMode == REG_DIE_MODE_CHROME || eDieMode == REG_DIE_MODE_ALL)
    {
        VPSS_DIECTRL.bits.die_chmmode = u32Mode;
    }
    if (eDieMode == REG_DIE_MODE_LUMA || eDieMode == REG_DIE_MODE_ALL)
    {
        VPSS_DIECTRL.bits.die_lmmode = u32Mode;
    }
    VPSS_REG_RegWrite(&(pstReg->VPSS_DIECTRL.u32), VPSS_DIECTRL.u32);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetStInfo(HI_U32 u32AppAddr,HI_BOOL bStop)
{
    U_VPSS_DIECTRL VPSS_DIECTRL;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;

    VPSS_DIECTRL.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_DIECTRL.u32));

    VPSS_DIECTRL.bits.stinfo_stop = bStop;
    VPSS_REG_RegWrite(&(pstReg->VPSS_DIECTRL.u32), VPSS_DIECTRL.u32);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetMfMax(HI_U32 u32AppAddr,REG_DIE_MODE_E eDieMode,HI_BOOL bMax)
{
    U_VPSS_DIELMA2 VPSS_DIELMA2;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;

    VPSS_DIELMA2.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_DIELMA2.u32));
    if (eDieMode == REG_DIE_MODE_CHROME || eDieMode == REG_DIE_MODE_ALL)
    {
        VPSS_DIELMA2.bits.chroma_mf_max = bMax;
    }
    if (eDieMode == REG_DIE_MODE_LUMA || eDieMode == REG_DIE_MODE_ALL)
    {
        VPSS_DIELMA2.bits.luma_mf_max = bMax;
    }
    VPSS_REG_RegWrite(&(pstReg->VPSS_DIELMA2.u32), VPSS_DIELMA2.u32);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetLuSceSdfMax(HI_U32 u32AppAddr,HI_BOOL bMax)
{
    U_VPSS_DIELMA2 VPSS_DIELMA2;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;

    VPSS_DIELMA2.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_DIELMA2.u32));
    VPSS_DIELMA2.bits.luma_scesdf_max = bMax;
    VPSS_REG_RegWrite(&(pstReg->VPSS_DIELMA2.u32), VPSS_DIELMA2.u32);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetSadThd(HI_U32 u32AppAddr,HI_U32 u32Thd)
{
    U_VPSS_DIELMA2 VPSS_DIELMA2;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;

    VPSS_DIELMA2.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_DIELMA2.u32));
    VPSS_DIELMA2.bits.die_sad_thd = u32Thd;
    VPSS_REG_RegWrite(&(pstReg->VPSS_DIELMA2.u32), VPSS_DIELMA2.u32);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetMinIntern(HI_U32 u32AppAddr,HI_U32 u32MinIntern)
{
    U_VPSS_DIEINTEN VPSS_DIEINTEN;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;
    VPSS_DIEINTEN.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_DIEINTEN.u32));
    VPSS_DIEINTEN.bits.ver_min_inten = u32MinIntern;
    VPSS_REG_RegWrite(&(pstReg->VPSS_DIEINTEN.u32), VPSS_DIEINTEN.u32);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetInternVer(HI_U32 u32AppAddr,HI_U32 u32InternVer)
{
    U_VPSS_DIEINTEN VPSS_DIEINTEN;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;
    VPSS_DIEINTEN.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_DIEINTEN.u32));
    VPSS_DIEINTEN.bits.dir_inten_ver = u32InternVer;
    VPSS_REG_RegWrite(&(pstReg->VPSS_DIEINTEN.u32), VPSS_DIEINTEN.u32);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetRangeScale(HI_U32 u32AppAddr,HI_U32 u32Scale)
{
    U_VPSS_DIESCALE VPSS_DIESCALE;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;
    VPSS_DIESCALE.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_DIEINTEN.u32));
    VPSS_DIESCALE.bits.range_scale = u32Scale;
    VPSS_REG_RegWrite(&(pstReg->VPSS_DIESCALE.u32), VPSS_DIESCALE.u32);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetCK1(HI_U32 u32AppAddr,HI_U32 u32Gain,HI_U32 u32Range,HI_U32 u32Max)
{
    U_VPSS_DIECHECK1 VPSS_DIECHECK1;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;
    VPSS_DIECHECK1.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_DIECHECK1.u32));
    VPSS_DIECHECK1.bits.ck_gain = u32Gain;
    VPSS_DIECHECK1.bits.ck_range_gain = u32Range;
    VPSS_DIECHECK1.bits.ck_max_range = u32Max;
    VPSS_REG_RegWrite(&(pstReg->VPSS_DIECHECK1.u32), VPSS_DIECHECK1.u32);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetCK2(HI_U32 u32AppAddr,HI_U32 u32Gain,HI_U32 u32Range,HI_U32 u32Max)
{
    U_VPSS_DIECHECK2 VPSS_DIECHECK2;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;
    VPSS_DIECHECK2.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_DIECHECK2.u32));
    VPSS_DIECHECK2.bits.ck_gain = u32Gain;
    VPSS_DIECHECK2.bits.ck_range_gain =  u32Range;
    VPSS_DIECHECK2.bits.ck_max_range = u32Max;
    VPSS_REG_RegWrite(&(pstReg->VPSS_DIECHECK2.u32), VPSS_DIECHECK2.u32);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetDIR(HI_U32 u32AppAddr,HI_S32 *ps32MultDir)
{
    U_VPSS_DIEDIR0_3 VPSS_DIEDIR0_3;
    U_VPSS_DIEDIR4_7 VPSS_DIEDIR4_7;
    U_VPSS_DIEDIR8_11 VPSS_DIEDIR8_11;
    U_VPSS_DIEDIR12_14 VPSS_DIEDIR12_14;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;
    VPSS_DIEDIR0_3.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_DIEDIR0_3.u32));
    VPSS_DIEDIR0_3.bits.dir0_mult = ps32MultDir[0];
    VPSS_DIEDIR0_3.bits.dir1_mult = ps32MultDir[1];
    VPSS_DIEDIR0_3.bits.dir2_mult = ps32MultDir[2];
    VPSS_DIEDIR0_3.bits.dir3_mult = ps32MultDir[3];
    VPSS_REG_RegWrite(&(pstReg->VPSS_DIEDIR0_3.u32), VPSS_DIEDIR0_3.u32);
    
    VPSS_DIEDIR4_7.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_DIEDIR4_7.u32));
    VPSS_DIEDIR4_7.bits.dir4_mult = ps32MultDir[4];
    VPSS_DIEDIR4_7.bits.dir5_mult = ps32MultDir[5];
    VPSS_DIEDIR4_7.bits.dir6_mult = ps32MultDir[6];
    VPSS_DIEDIR4_7.bits.dir7_mult = ps32MultDir[7];
    VPSS_REG_RegWrite(&(pstReg->VPSS_DIEDIR4_7.u32), VPSS_DIEDIR4_7.u32);
    
    VPSS_DIEDIR8_11.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_DIEDIR8_11.u32));
    VPSS_DIEDIR8_11.bits.dir8_mult = ps32MultDir[8];
    VPSS_DIEDIR8_11.bits.dir9_mult = ps32MultDir[9];
    VPSS_DIEDIR8_11.bits.dir10_mult = ps32MultDir[10];
    VPSS_DIEDIR8_11.bits.dir11_mult = ps32MultDir[11];
    VPSS_REG_RegWrite(&(pstReg->VPSS_DIEDIR8_11.u32), VPSS_DIEDIR8_11.u32);
    
    VPSS_DIEDIR12_14.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_DIEDIR12_14.u32));
    VPSS_DIEDIR12_14.bits.dir12_mult = ps32MultDir[12];
    VPSS_DIEDIR12_14.bits.dir13_mult = ps32MultDir[13];
    VPSS_DIEDIR12_14.bits.dir14_mult = ps32MultDir[14];
    VPSS_REG_RegWrite(&(pstReg->VPSS_DIEDIR12_14.u32), VPSS_DIEDIR12_14.u32);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetCcEn(HI_U32 u32AppAddr,HI_BOOL bEnCc)
{
    U_VPSS_CCRSCLR0 VPSS_CCRSCLR0;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;

    VPSS_CCRSCLR0.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_CCRSCLR0.u32));
    VPSS_CCRSCLR0.bits.chroma_ccr_en = bEnCc;
    VPSS_REG_RegWrite(&(pstReg->VPSS_CCRSCLR0.u32), VPSS_CCRSCLR0.u32);

    return HI_SUCCESS;

}
HI_S32 VPSS_REG_SetCcOffset(HI_U32 u32AppAddr,HI_S32 s32Offset)
{
    U_VPSS_CCRSCLR0 VPSS_CCRSCLR0;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;

    VPSS_CCRSCLR0.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_CCRSCLR0.u32));
    VPSS_CCRSCLR0.bits.chroma_ma_offset = s32Offset;
    VPSS_REG_RegWrite(&(pstReg->VPSS_CCRSCLR0.u32), VPSS_CCRSCLR0.u32);

    return HI_SUCCESS;

}
HI_S32 VPSS_REG_SetCcDetMax(HI_U32 u32AppAddr,HI_S32 s32Max)
{
    U_VPSS_CCRSCLR0 VPSS_CCRSCLR0;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;

    VPSS_CCRSCLR0.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_CCRSCLR0.u32));
    VPSS_CCRSCLR0.bits.no_ccr_detect_max = s32Max;
    VPSS_REG_RegWrite(&(pstReg->VPSS_CCRSCLR0.u32), VPSS_CCRSCLR0.u32);

    return HI_SUCCESS;

}
HI_S32 VPSS_REG_SetCcDetThd(HI_U32 u32AppAddr,HI_S32 s32Thd)
{
    U_VPSS_CCRSCLR0 VPSS_CCRSCLR0;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;

    VPSS_CCRSCLR0.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_CCRSCLR0.u32));
    VPSS_CCRSCLR0.bits.no_ccr_detect_thd = s32Thd;
    VPSS_REG_RegWrite(&(pstReg->VPSS_CCRSCLR0.u32), VPSS_CCRSCLR0.u32);

    return HI_SUCCESS;

}
HI_S32 VPSS_REG_SetSimiMax(HI_U32 u32AppAddr,HI_S32 s32SimiMax)
{
    U_VPSS_CCRSCLR0 VPSS_CCRSCLR0;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;

    VPSS_CCRSCLR0.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_CCRSCLR0.u32));
    VPSS_CCRSCLR0.bits.no_ccr_detect_thd = s32SimiMax;
    VPSS_REG_RegWrite(&(pstReg->VPSS_CCRSCLR0.u32), VPSS_CCRSCLR0.u32);

    return HI_SUCCESS;

}
HI_S32 VPSS_REG_SetSimiThd(HI_U32 u32AppAddr,HI_S32 s32SimiThd)
{
    U_VPSS_CCRSCLR1 VPSS_CCRSCLR1;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;

    VPSS_CCRSCLR1.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_CCRSCLR1.u32));
    VPSS_CCRSCLR1.bits.similar_thd = s32SimiThd;
    VPSS_REG_RegWrite(&(pstReg->VPSS_CCRSCLR1.u32), VPSS_CCRSCLR1.u32);

    return HI_SUCCESS;

}
HI_S32 VPSS_REG_SetDetBlend(HI_U32 u32AppAddr,HI_S32 s32DetBlend)
{
    U_VPSS_CCRSCLR1 VPSS_CCRSCLR1;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;

    VPSS_CCRSCLR1.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_CCRSCLR1.u32));
    VPSS_CCRSCLR1.bits.no_ccr_detect_blend = s32DetBlend;
    VPSS_REG_RegWrite(&(pstReg->VPSS_CCRSCLR1.u32), VPSS_CCRSCLR1.u32);

    return HI_SUCCESS;

}
HI_S32 VPSS_REG_SetMaxXChroma(HI_U32 u32AppAddr,HI_S32 s32Max)
{
    U_VPSS_CCRSCLR1 VPSS_CCRSCLR1;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;

    VPSS_CCRSCLR1.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_CCRSCLR1.u32));
    VPSS_CCRSCLR1.bits.max_xchroma = s32Max;
    VPSS_REG_RegWrite(&(pstReg->VPSS_CCRSCLR1.u32), VPSS_CCRSCLR1.u32);

    return HI_SUCCESS;

}
HI_S32 VPSS_REG_SetIntpSclRat(HI_U32 u32AppAddr,HI_S32 *ps32Rat)
{
    U_VPSS_DIEINTPSCL0 VPSS_DIEINTPSCL0;
    U_VPSS_DIEINTPSCL1 VPSS_DIEINTPSCL1;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;
    VPSS_DIEINTPSCL0.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_DIEINTPSCL0.u32));
    VPSS_DIEINTPSCL0.bits.intp_scale_ratio_1 = ps32Rat[0];
    VPSS_DIEINTPSCL0.bits.intp_scale_ratio_2 = ps32Rat[1];
    VPSS_DIEINTPSCL0.bits.intp_scale_ratio_3 = ps32Rat[2];
    VPSS_DIEINTPSCL0.bits.intp_scale_ratio_4 = ps32Rat[3];
    VPSS_DIEINTPSCL0.bits.intp_scale_ratio_5 = ps32Rat[4];
    VPSS_DIEINTPSCL0.bits.intp_scale_ratio_6 = ps32Rat[5];
    VPSS_DIEINTPSCL0.bits.intp_scale_ratio_7 = ps32Rat[6];
    VPSS_DIEINTPSCL0.bits.intp_scale_ratio_8 = ps32Rat[7];
    VPSS_REG_RegWrite(&(pstReg->VPSS_DIEINTPSCL0.u32), VPSS_DIEINTPSCL0.u32);

    VPSS_DIEINTPSCL1.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_DIEINTPSCL1.u32));
    VPSS_DIEINTPSCL1.bits.intp_scale_ratio_9  = ps32Rat[8];
    VPSS_DIEINTPSCL1.bits.intp_scale_ratio_10 = ps32Rat[9];
    VPSS_DIEINTPSCL1.bits.intp_scale_ratio_11 = ps32Rat[10];
    VPSS_DIEINTPSCL1.bits.intp_scale_ratio_12 = ps32Rat[11];
    VPSS_DIEINTPSCL1.bits.intp_scale_ratio_13 = ps32Rat[12];
    VPSS_DIEINTPSCL1.bits.intp_scale_ratio_14 = ps32Rat[13];
    VPSS_DIEINTPSCL1.bits.intp_scale_ratio_15 = ps32Rat[14];
    VPSS_REG_RegWrite(&(pstReg->VPSS_DIEINTPSCL1.u32), VPSS_DIEINTPSCL1.u32);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetStrenThd(HI_U32 u32AppAddr,HI_S32 s32Thd)
{
    U_VPSS_DIEDIRTHD VPSS_DIEDIRTHD;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;
    VPSS_DIEDIRTHD.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_DIEDIRTHD.u32));
    VPSS_DIEDIRTHD.bits.strength_thd = s32Thd;
    VPSS_REG_RegWrite(&(pstReg->VPSS_DIEDIRTHD.u32), VPSS_DIEDIRTHD.u32);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetDirThd(HI_U32 u32AppAddr,HI_S32 s32Thd)
{
    U_VPSS_DIEDIRTHD VPSS_DIEDIRTHD;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;
    VPSS_DIEDIRTHD.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_DIEDIRTHD.u32));
    VPSS_DIEDIRTHD.bits.dir_thd = s32Thd;
    VPSS_REG_RegWrite(&(pstReg->VPSS_DIEDIRTHD.u32), VPSS_DIEDIRTHD.u32);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetBcGain(HI_U32 u32AppAddr,HI_S32 s32BcGain)
{
    U_VPSS_DIEDIRTHD VPSS_DIEDIRTHD;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;
    VPSS_DIEDIRTHD.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_DIEDIRTHD.u32));
    VPSS_DIEDIRTHD.bits.bc_gain = s32BcGain;
    VPSS_REG_RegWrite(&(pstReg->VPSS_DIEDIRTHD.u32), VPSS_DIEDIRTHD.u32);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetJitterMode(HI_U32 u32AppAddr,HI_BOOL bJitMd)
{
    U_VPSS_DIEJITMTN VPSS_DIEJITMTN;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;
    VPSS_DIEJITMTN.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_DIEJITMTN.u32));
    VPSS_DIEJITMTN.bits.jitter_mode = bJitMd;
    VPSS_REG_RegWrite(&(pstReg->VPSS_DIEJITMTN.u32), VPSS_DIEJITMTN.u32);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetJitterFactor(HI_U32 u32AppAddr,HI_S32 s32Factor)
{
    U_VPSS_DIEJITMTN VPSS_DIEJITMTN;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;
    VPSS_DIEJITMTN.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_DIEJITMTN.u32));
    VPSS_DIEJITMTN.bits.jitter_factor = s32Factor;
    VPSS_REG_RegWrite(&(pstReg->VPSS_DIEJITMTN.u32), VPSS_DIEJITMTN.u32);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetJitterCoring(HI_U32 u32AppAddr,HI_S32 s32Coring)
{
    U_VPSS_DIEJITMTN VPSS_DIEJITMTN;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;
    VPSS_DIEJITMTN.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_DIEJITMTN.u32));
    VPSS_DIEJITMTN.bits.jitter_coring = s32Coring;
    VPSS_REG_RegWrite(&(pstReg->VPSS_DIEJITMTN.u32), VPSS_DIEJITMTN.u32);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetJitterGain(HI_U32 u32AppAddr,HI_S32 s32Gain)
{
    U_VPSS_DIEJITMTN VPSS_DIEJITMTN;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;
    VPSS_DIEJITMTN.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_DIEJITMTN.u32));
    VPSS_DIEJITMTN.bits.jitter_gain = s32Gain;
    VPSS_REG_RegWrite(&(pstReg->VPSS_DIEJITMTN.u32), VPSS_DIEJITMTN.u32);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetJitterFilter(HI_U32 u32AppAddr,HI_S32 *ps32Filter)
{
    U_VPSS_DIEJITMTN VPSS_DIEJITMTN;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;
    VPSS_DIEJITMTN.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_DIEJITMTN.u32));
    VPSS_DIEJITMTN.bits.jitter_filter_2 = ps32Filter[2];
    VPSS_DIEJITMTN.bits.jitter_filter_1 = ps32Filter[1];
    VPSS_DIEJITMTN.bits.jitter_filter_0 = ps32Filter[0];
    VPSS_REG_RegWrite(&(pstReg->VPSS_DIEJITMTN.u32), VPSS_DIEJITMTN.u32);

    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetMotionSlope(HI_U32 u32AppAddr,HI_S32 s32Slope)
{
    U_VPSS_DIEFLDMTN VPSS_DIEFLDMTN;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;
    VPSS_DIEFLDMTN.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_DIEFLDMTN.u32));
    VPSS_DIEFLDMTN.bits.fld_motion_curve_slope = s32Slope;
    VPSS_REG_RegWrite(&(pstReg->VPSS_DIEFLDMTN.u32), VPSS_DIEFLDMTN.u32);

    return HI_SUCCESS;
        
}
HI_S32 VPSS_REG_SetMotionCoring(HI_U32 u32AppAddr,HI_S32 s32Coring)
{
    U_VPSS_DIEFLDMTN VPSS_DIEFLDMTN;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;
    VPSS_DIEFLDMTN.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_DIEFLDMTN.u32));
    VPSS_DIEFLDMTN.bits.fld_motion_coring = s32Coring;
    VPSS_REG_RegWrite(&(pstReg->VPSS_DIEFLDMTN.u32), VPSS_DIEFLDMTN.u32);

    return HI_SUCCESS;
        
}
HI_S32 VPSS_REG_SetMotionGain(HI_U32 u32AppAddr,HI_S32 s32Gain)
{
    U_VPSS_DIEFLDMTN VPSS_DIEFLDMTN;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;
    VPSS_DIEFLDMTN.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_DIEFLDMTN.u32));
    VPSS_DIEFLDMTN.bits.fld_motion_gain = s32Gain;
    VPSS_REG_RegWrite(&(pstReg->VPSS_DIEFLDMTN.u32), VPSS_DIEFLDMTN.u32);

    return HI_SUCCESS;
        
}
HI_S32 VPSS_REG_SetMotionHThd(HI_U32 u32AppAddr,HI_S32 s32HThd)
{
    U_VPSS_DIEFLDMTN VPSS_DIEFLDMTN;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;
    VPSS_DIEFLDMTN.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_DIEFLDMTN.u32));
    VPSS_DIEFLDMTN.bits.fld_motion_thd_high = s32HThd;
    VPSS_REG_RegWrite(&(pstReg->VPSS_DIEFLDMTN.u32), VPSS_DIEFLDMTN.u32);

    return HI_SUCCESS;
        
}
HI_S32 VPSS_REG_SetMotionLThd(HI_U32 u32AppAddr,HI_S32 s32LThd)
{
    U_VPSS_DIEFLDMTN VPSS_DIEFLDMTN;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;
    VPSS_DIEFLDMTN.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_DIEFLDMTN.u32));
    VPSS_DIEFLDMTN.bits.fld_motion_thd_low = s32LThd;
    VPSS_REG_RegWrite(&(pstReg->VPSS_DIEFLDMTN.u32), VPSS_DIEFLDMTN.u32);

    return HI_SUCCESS;
        
}
HI_S32 VPSS_REG_SetLumAvgThd(HI_U32 u32AppAddr,HI_S32 *ps32Thd)
{
    U_VPSS_DIEMTNCRVTHD VPSS_DIEMTNCRVTHD;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;
    VPSS_DIEMTNCRVTHD.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_DIEMTNCRVTHD.u32));
    VPSS_DIEMTNCRVTHD.bits.lum_avg_thd_3 = ps32Thd[3];
    VPSS_DIEMTNCRVTHD.bits.lum_avg_thd_2 = ps32Thd[2];
    VPSS_DIEMTNCRVTHD.bits.lum_avg_thd_1 = ps32Thd[1];
    VPSS_DIEMTNCRVTHD.bits.lum_avg_thd_0 = ps32Thd[0];
    VPSS_REG_RegWrite(&(pstReg->VPSS_DIEMTNCRVTHD.u32), VPSS_DIEMTNCRVTHD.u32);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetCurSlope(HI_U32 u32AppAddr,HI_S32 *ps32Slope)
{
    U_VPSS_DIEMTNCRVSLP VPSS_DIEMTNCRVSLP;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;
    VPSS_DIEMTNCRVSLP.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_DIEMTNCRVSLP.u32));
    VPSS_DIEMTNCRVSLP.bits.motion_curve_slope_3 = ps32Slope[3];
    VPSS_DIEMTNCRVSLP.bits.motion_curve_slope_2 = ps32Slope[2];
    VPSS_DIEMTNCRVSLP.bits.motion_curve_slope_1 = ps32Slope[1];
    VPSS_DIEMTNCRVSLP.bits.motion_curve_slope_0 = ps32Slope[0];
    VPSS_REG_RegWrite(&(pstReg->VPSS_DIEMTNCRVSLP.u32), VPSS_DIEMTNCRVSLP.u32);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetMotionEn(HI_U32 u32AppAddr,HI_BOOL bEnMotion)
{
    U_VPSS_DIEMTNCRVRAT0 VPSS_DIEMTNCRVRAT0;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;
    VPSS_DIEMTNCRVRAT0.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_DIEMTNCRVRAT0.u32));
    VPSS_DIEMTNCRVRAT0.bits.motion_ratio_en = bEnMotion;
    VPSS_REG_RegWrite(&(pstReg->VPSS_DIEMTNCRVRAT0.u32), VPSS_DIEMTNCRVRAT0.u32);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetMotionStart(HI_U32 u32AppAddr,HI_S32 s32Start)
{
    U_VPSS_DIEMTNCRVRAT0 VPSS_DIEMTNCRVRAT0;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;
    VPSS_DIEMTNCRVRAT0.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_DIEMTNCRVRAT0.u32));
    VPSS_DIEMTNCRVRAT0.bits.start_motion_ratio = s32Start;
    VPSS_REG_RegWrite(&(pstReg->VPSS_DIEMTNCRVRAT0.u32), VPSS_DIEMTNCRVRAT0.u32);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetMotionRatio(HI_U32 u32AppAddr,HI_S32 *ps32Ration)
{
    U_VPSS_DIEMTNCRVRAT0 VPSS_DIEMTNCRVRAT0;
    U_VPSS_DIEMTNCRVRAT1 VPSS_DIEMTNCRVRAT1;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;
    VPSS_DIEMTNCRVRAT0.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_DIEMTNCRVRAT0.u32));
    VPSS_DIEMTNCRVRAT0.bits.motion_curve_ratio_1 = ps32Ration[1];
    VPSS_DIEMTNCRVRAT0.bits.motion_curve_ratio_0 = ps32Ration[0];
    VPSS_REG_RegWrite(&(pstReg->VPSS_DIEMTNCRVRAT0.u32), VPSS_DIEMTNCRVRAT0.u32);

    VPSS_DIEMTNCRVRAT1.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_DIEMTNCRVRAT1.u32));
    VPSS_DIEMTNCRVRAT1.bits.motion_curve_ratio_3 = ps32Ration[3]; 
    VPSS_DIEMTNCRVRAT1.bits.motion_curve_ratio_2 = ps32Ration[2];
    VPSS_REG_RegWrite(&(pstReg->VPSS_DIEMTNCRVRAT1.u32), VPSS_DIEMTNCRVRAT1.u32);

    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetMotionMaxRatio(HI_U32 u32AppAddr,HI_S32 s32Max)
{
    U_VPSS_DIEMTNCRVRAT1 VPSS_DIEMTNCRVRAT1;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;
    VPSS_DIEMTNCRVRAT1.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_DIEMTNCRVRAT1.u32));
    VPSS_DIEMTNCRVRAT1.bits.max_motion_ratio = s32Max;
    VPSS_REG_RegWrite(&(pstReg->VPSS_DIEMTNCRVRAT1.u32), VPSS_DIEMTNCRVRAT1.u32);

    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetMotionMinRatio(HI_U32 u32AppAddr,HI_S32 s32Min)
{
    U_VPSS_DIEMTNCRVRAT1 VPSS_DIEMTNCRVRAT1;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;
    VPSS_DIEMTNCRVRAT1.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_DIEMTNCRVRAT1.u32));
    VPSS_DIEMTNCRVRAT1.bits.min_motion_ratio = s32Min;
    VPSS_REG_RegWrite(&(pstReg->VPSS_DIEMTNCRVRAT1.u32), VPSS_DIEMTNCRVRAT1.u32);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetMotionDiffThd(HI_U32 u32AppAddr,HI_S32 *ps32Thd)
{
    U_VPSS_DIEMTNDIFFTHD0 VPSS_DIEMTNDIFFTHD0;
    U_VPSS_DIEMTNDIFFTHD1 VPSS_DIEMTNDIFFTHD1;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;
    VPSS_DIEMTNDIFFTHD0.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_DIEMTNDIFFTHD0.u32));
    VPSS_DIEMTNDIFFTHD0.bits.motion_diff_thd_0 = ps32Thd[0];
    VPSS_DIEMTNDIFFTHD0.bits.motion_diff_thd_1 = ps32Thd[1];
    VPSS_DIEMTNDIFFTHD0.bits.motion_diff_thd_2 = ps32Thd[2];
    VPSS_DIEMTNDIFFTHD0.bits.motion_diff_thd_3 = ps32Thd[3];
    VPSS_REG_RegWrite(&(pstReg->VPSS_DIEMTNDIFFTHD0.u32), VPSS_DIEMTNDIFFTHD0.u32);

    VPSS_DIEMTNDIFFTHD1.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_DIEMTNDIFFTHD1.u32));
    VPSS_DIEMTNDIFFTHD1.bits.motion_diff_thd_4 = ps32Thd[4];
    VPSS_DIEMTNDIFFTHD1.bits.motion_diff_thd_5 = ps32Thd[5];
    VPSS_DIEMTNDIFFTHD1.bits.motion_diff_thd_6 = ps32Thd[6];
    VPSS_DIEMTNDIFFTHD1.bits.motion_diff_thd_7 = ps32Thd[7];
    VPSS_REG_RegWrite(&(pstReg->VPSS_DIEMTNDIFFTHD1.u32), VPSS_DIEMTNDIFFTHD1.u32);

    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetMotionIIrSlope(HI_U32 u32AppAddr,HI_S32 *ps32Slope)
{
    U_VPSS_DIEMTNIIRSLP0 VPSS_DIEMTNIIRSLP0;
    U_VPSS_DIEMTNIIRSLP1 VPSS_DIEMTNIIRSLP1;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;
    VPSS_DIEMTNIIRSLP0.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_DIEMTNIIRSLP0.u32));
    VPSS_DIEMTNIIRSLP0.bits.motion_iir_curve_slope_0 = ps32Slope[0];
    VPSS_DIEMTNIIRSLP0.bits.motion_iir_curve_slope_1 = ps32Slope[1];
    VPSS_DIEMTNIIRSLP0.bits.motion_iir_curve_slope_2 = ps32Slope[2];
    VPSS_DIEMTNIIRSLP0.bits.motion_iir_curve_slope_3 = ps32Slope[3];
    VPSS_REG_RegWrite(&(pstReg->VPSS_DIEMTNIIRSLP0.u32), VPSS_DIEMTNIIRSLP0.u32);

    VPSS_DIEMTNIIRSLP1.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_DIEMTNIIRSLP1.u32));
    VPSS_DIEMTNIIRSLP1.bits.motion_iir_curve_slope_4 = ps32Slope[4];
    VPSS_DIEMTNIIRSLP1.bits.motion_iir_curve_slope_5 = ps32Slope[5];
    VPSS_DIEMTNIIRSLP1.bits.motion_iir_curve_slope_6 = ps32Slope[6];
    VPSS_DIEMTNIIRSLP1.bits.motion_iir_curve_slope_7 = ps32Slope[7];
    VPSS_REG_RegWrite(&(pstReg->VPSS_DIEMTNIIRSLP1.u32), VPSS_DIEMTNIIRSLP1.u32);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetMotionIIrEn(HI_U32 u32AppAddr,HI_BOOL bEnIIr)
{
    U_VPSS_DIEMTNIIRRAT0 VPSS_DIEMTNIIRRAT0;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;
    VPSS_DIEMTNIIRRAT0.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_DIEMTNIIRRAT0.u32));
    VPSS_DIEMTNIIRRAT0.bits.motion_iir_en = bEnIIr;
    VPSS_REG_RegWrite(&(pstReg->VPSS_DIEMTNIIRRAT0.u32), VPSS_DIEMTNIIRRAT0.u32);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetMotionIIrStart(HI_U32 u32AppAddr,HI_S32 s32Start)
{
    U_VPSS_DIEMTNIIRRAT0 VPSS_DIEMTNIIRRAT0;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;
    VPSS_DIEMTNIIRRAT0.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_DIEMTNIIRRAT0.u32));
    VPSS_DIEMTNIIRRAT0.bits.start_motion_iir_ratio = s32Start;
    VPSS_REG_RegWrite(&(pstReg->VPSS_DIEMTNIIRRAT0.u32), VPSS_DIEMTNIIRRAT0.u32);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetIIrMaxRatio(HI_U32 u32AppAddr,HI_S32 s32Max)
{
    U_VPSS_DIEMTNIIRRAT1 VPSS_DIEMTNIIRRAT1;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;
    VPSS_DIEMTNIIRRAT1.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_DIEMTNIIRRAT1.u32));
    VPSS_DIEMTNIIRRAT1.bits.max_motion_iir_ratio = s32Max;
    VPSS_REG_RegWrite(&(pstReg->VPSS_DIEMTNIIRRAT1.u32), VPSS_DIEMTNIIRRAT1.u32);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetIIrMinRatio(HI_U32 u32AppAddr,HI_S32 s32Min)
{
    U_VPSS_DIEMTNIIRRAT1 VPSS_DIEMTNIIRRAT1;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;
    VPSS_DIEMTNIIRRAT1.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_DIEMTNIIRRAT1.u32));
    VPSS_DIEMTNIIRRAT1.bits.min_motion_iir_ratio = s32Min;
    VPSS_REG_RegWrite(&(pstReg->VPSS_DIEMTNIIRRAT1.u32), VPSS_DIEMTNIIRRAT1.u32);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetMotionIIrRatio(HI_U32 u32AppAddr,HI_S32 *ps32Ratio)
{
    U_VPSS_DIEMTNIIRRAT0 VPSS_DIEMTNIIRRAT0;
    U_VPSS_DIEMTNIIRRAT1 VPSS_DIEMTNIIRRAT1;
    U_VPSS_DIEMTNIIRRAT2 VPSS_DIEMTNIIRRAT2;
    U_VPSS_DIEMTNIIRRAT3 VPSS_DIEMTNIIRRAT3;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;
    VPSS_DIEMTNIIRRAT0.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_DIEMTNIIRRAT0.u32));
    VPSS_DIEMTNIIRRAT0.bits.motion_iir_curve_ratio_0 = ps32Ratio[0];
    VPSS_DIEMTNIIRRAT0.bits.motion_iir_curve_ratio_1 = ps32Ratio[1];
    VPSS_REG_RegWrite(&(pstReg->VPSS_DIEMTNIIRRAT0.u32), VPSS_DIEMTNIIRRAT0.u32);

    VPSS_DIEMTNIIRRAT1.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_DIEMTNIIRRAT1.u32));
    VPSS_DIEMTNIIRRAT1.bits.motion_iir_curve_ratio_2 = ps32Ratio[2];
    VPSS_DIEMTNIIRRAT1.bits.motion_iir_curve_ratio_3 = ps32Ratio[3];
    VPSS_REG_RegWrite(&(pstReg->VPSS_DIEMTNIIRRAT1.u32), VPSS_DIEMTNIIRRAT1.u32);

    
    VPSS_DIEMTNIIRRAT2.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_DIEMTNIIRRAT2.u32));
    VPSS_DIEMTNIIRRAT2.bits.motion_iir_curve_ratio_4 = ps32Ratio[4];
    VPSS_DIEMTNIIRRAT2.bits.motion_iir_curve_ratio_5 = ps32Ratio[5];
    VPSS_REG_RegWrite(&(pstReg->VPSS_DIEMTNIIRRAT2.u32), VPSS_DIEMTNIIRRAT2.u32);
    
    VPSS_DIEMTNIIRRAT3.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_DIEMTNIIRRAT3.u32));
    VPSS_DIEMTNIIRRAT3.bits.motion_iir_curve_ratio_6 = ps32Ratio[6];
    VPSS_DIEMTNIIRRAT3.bits.motion_iir_curve_ratio_7 = ps32Ratio[7];
    VPSS_REG_RegWrite(&(pstReg->VPSS_DIEMTNIIRRAT3.u32), VPSS_DIEMTNIIRRAT3.u32);

    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetRecWrMode(HI_U32 u32AppAddr,HI_BOOL bRecMdWrMd)
{
    U_VPSS_DIERECMODE VPSS_DIERECMODE;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;
    VPSS_DIERECMODE.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_DIERECMODE.u32));
    VPSS_DIERECMODE.bits.rec_mode_write_mode = bRecMdWrMd;
    VPSS_REG_RegWrite(&(pstReg->VPSS_DIERECMODE.u32), VPSS_DIERECMODE.u32);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetRecEn(HI_U32 u32AppAddr,HI_BOOL bEnRec)
{
    U_VPSS_DIERECMODE VPSS_DIERECMODE;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;
    VPSS_DIERECMODE.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_DIERECMODE.u32));
    VPSS_DIERECMODE.bits.rec_mode_en = bEnRec;
    VPSS_REG_RegWrite(&(pstReg->VPSS_DIERECMODE.u32), VPSS_DIERECMODE.u32);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetRecMixMode(HI_U32 u32AppAddr,HI_BOOL bRecMdMixMd)
{
    U_VPSS_DIERECMODE VPSS_DIERECMODE;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;
    VPSS_DIERECMODE.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_DIERECMODE.u32));
    VPSS_DIERECMODE.bits.rec_mode_mix_mode = bRecMdMixMd;
    VPSS_REG_RegWrite(&(pstReg->VPSS_DIERECMODE.u32), VPSS_DIERECMODE.u32);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetRecScale(HI_U32 u32AppAddr,HI_S32 s32RecScale)
{
    U_VPSS_DIERECMODE VPSS_DIERECMODE;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;
    VPSS_DIERECMODE.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_DIERECMODE.u32));
    VPSS_DIERECMODE.bits.rec_mode_scale = s32RecScale;
    VPSS_REG_RegWrite(&(pstReg->VPSS_DIERECMODE.u32), VPSS_DIERECMODE.u32);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetRecThd(HI_U32 u32AppAddr,HI_S32 s32Thd)
{
    U_VPSS_DIERECMODE VPSS_DIERECMODE;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;
    VPSS_DIERECMODE.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_DIERECMODE.u32));
    VPSS_DIERECMODE.bits.rec_mode_motion_thd = s32Thd;
    VPSS_REG_RegWrite(&(pstReg->VPSS_DIERECMODE.u32), VPSS_DIERECMODE.u32);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetRecCoring(HI_U32 u32AppAddr,HI_S32 s32Coring)
{
    U_VPSS_DIERECMODE VPSS_DIERECMODE;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;
    VPSS_DIERECMODE.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_DIERECMODE.u32));
    VPSS_DIERECMODE.bits.rec_mode_fld_motion_coring = s32Coring;
    VPSS_REG_RegWrite(&(pstReg->VPSS_DIERECMODE.u32), VPSS_DIERECMODE.u32);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetRecGain(HI_U32 u32AppAddr,HI_S32 s32Gain)
{
    U_VPSS_DIERECMODE VPSS_DIERECMODE;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;
    VPSS_DIERECMODE.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_DIERECMODE.u32));
    VPSS_DIERECMODE.bits.rec_mode_fld_motion_gain = s32Gain;
    VPSS_REG_RegWrite(&(pstReg->VPSS_DIERECMODE.u32), VPSS_DIERECMODE.u32);

    return HI_SUCCESS;
}


HI_S32 VPSS_REG_SetRecFldStep(HI_U32 u32AppAddr,HI_S32 *ps32Step)
{
    U_VPSS_DIEHISMODE VPSS_DIEHISMODE;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;
    VPSS_DIEHISMODE.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_DIEHISMODE.u32));
    VPSS_DIEHISMODE.bits.rec_mode_fld_motion_step_0 = ps32Step[0];
    VPSS_DIEHISMODE.bits.rec_mode_fld_motion_step_1 = ps32Step[1];
    VPSS_REG_RegWrite(&(pstReg->VPSS_DIEHISMODE.u32), VPSS_DIEHISMODE.u32);

    return HI_SUCCESS;

}
HI_S32 VPSS_REG_SetRecFrmStep(HI_U32 u32AppAddr,HI_S32 *ps32Step)
{
    U_VPSS_DIEHISMODE VPSS_DIEHISMODE;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;
    VPSS_DIEHISMODE.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_DIEHISMODE.u32));
    VPSS_DIEHISMODE.bits.rec_mode_frm_motion_step_0 = ps32Step[0];
    VPSS_DIEHISMODE.bits.rec_mode_frm_motion_step_1 = ps32Step[1];
    VPSS_REG_RegWrite(&(pstReg->VPSS_DIEHISMODE.u32), VPSS_DIEHISMODE.u32);

    return HI_SUCCESS;

}
HI_S32 VPSS_REG_SetHisEn(HI_U32 u32AppAddr,HI_BOOL bEnHis)
{
    U_VPSS_DIEHISMODE VPSS_DIEHISMODE;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;
    VPSS_DIEHISMODE.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_DIEHISMODE.u32));
    VPSS_DIEHISMODE.bits.his_motion_en = bEnHis;
    VPSS_REG_RegWrite(&(pstReg->VPSS_DIEHISMODE.u32), VPSS_DIEHISMODE.u32);

    return HI_SUCCESS;

}
HI_S32 VPSS_REG_SetHisWrMode(HI_U32 u32AppAddr,HI_BOOL bHisMtnWrMd)
{
    U_VPSS_DIEHISMODE VPSS_DIEHISMODE;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;
    VPSS_DIEHISMODE.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_DIEHISMODE.u32));
    VPSS_DIEHISMODE.bits.his_motion_write_mode = bHisMtnWrMd;
    VPSS_REG_RegWrite(&(pstReg->VPSS_DIEHISMODE.u32), VPSS_DIEHISMODE.u32);

    return HI_SUCCESS;

}
HI_S32 VPSS_REG_SetHisUseMode(HI_U32 u32AppAddr,HI_BOOL bHisMtnUseMd)
{
    U_VPSS_DIEHISMODE VPSS_DIEHISMODE;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;
    VPSS_DIEHISMODE.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_DIEHISMODE.u32));
    VPSS_DIEHISMODE.bits.his_motion_using_mode = bHisMtnUseMd;
    VPSS_REG_RegWrite(&(pstReg->VPSS_DIEHISMODE.u32), VPSS_DIEHISMODE.u32);

    return HI_SUCCESS;

}
HI_S32 VPSS_REG_SetHisPreEn(HI_U32 u32AppAddr,HI_BOOL bEnPre)
{
    U_VPSS_DIEHISMODE VPSS_DIEHISMODE;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;
    VPSS_DIEHISMODE.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_DIEHISMODE.u32));
    VPSS_DIEHISMODE.bits.pre_info_en = bEnPre;
    VPSS_REG_RegWrite(&(pstReg->VPSS_DIEHISMODE.u32), VPSS_DIEHISMODE.u32);

    return HI_SUCCESS;

}
HI_S32 VPSS_REG_SetHisPpreEn(HI_U32 u32AppAddr,HI_BOOL bEnPpre)
{
    U_VPSS_DIEHISMODE VPSS_DIEHISMODE;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;
    VPSS_DIEHISMODE.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_DIEHISMODE.u32));
    VPSS_DIEHISMODE.bits.ppre_info_en = bEnPpre;
    VPSS_REG_RegWrite(&(pstReg->VPSS_DIEHISMODE.u32), VPSS_DIEHISMODE.u32);

    return HI_SUCCESS;

}

HI_S32 VPSS_REG_SetCombLimit(HI_U32 u32AppAddr,HI_S32  s32UpLimit,HI_S32  s32LowLimit)
{
    U_VPSS_DIECOMBCHK0 VPSS_DIECOMBCHK0;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;
    VPSS_DIECOMBCHK0.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_DIECOMBCHK0.u32));
    VPSS_DIECOMBCHK0.bits.comb_chk_lower_limit = s32LowLimit;
    VPSS_DIECOMBCHK0.bits.comb_chk_upper_limit = s32UpLimit;
    VPSS_REG_RegWrite(&(pstReg->VPSS_DIECOMBCHK0.u32), VPSS_DIECOMBCHK0.u32);

    return HI_SUCCESS;

}
HI_S32 VPSS_REG_SetCombThd(HI_U32 u32AppAddr,HI_S32  s32Hthd,HI_S32  s32Vthd)
{
    U_VPSS_DIECOMBCHK0 VPSS_DIECOMBCHK0;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;
    VPSS_DIECOMBCHK0.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_DIECOMBCHK0.u32));
    VPSS_DIECOMBCHK0.bits.comb_chk_min_hthd = s32Hthd;
    VPSS_DIECOMBCHK0.bits.comb_chk_min_vthd = s32Vthd;
    VPSS_REG_RegWrite(&(pstReg->VPSS_DIECOMBCHK0.u32), VPSS_DIECOMBCHK0.u32);

    return HI_SUCCESS;

}
HI_S32 VPSS_REG_SetCombEn(HI_U32 u32AppAddr,HI_BOOL bEnComb)
{
    U_VPSS_DIECOMBCHK1 VPSS_DIECOMBCHK1;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;
    VPSS_DIECOMBCHK1.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_DIECOMBCHK1.u32));
    VPSS_DIECOMBCHK1.bits.comb_chk_en = bEnComb;
    VPSS_REG_RegWrite(&(pstReg->VPSS_DIECOMBCHK1.u32), VPSS_DIECOMBCHK1.u32);

    return HI_SUCCESS;

}
HI_S32 VPSS_REG_SetCombMdThd(HI_U32 u32AppAddr,HI_S32 s32MdThd)
{
    U_VPSS_DIECOMBCHK1 VPSS_DIECOMBCHK1;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;
    VPSS_DIECOMBCHK1.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_DIECOMBCHK1.u32));
    VPSS_DIECOMBCHK1.bits.comb_chk_md_thd = s32MdThd;
    VPSS_REG_RegWrite(&(pstReg->VPSS_DIECOMBCHK1.u32), VPSS_DIECOMBCHK1.u32);

    return HI_SUCCESS;

}
HI_S32 VPSS_REG_SetCombEdgeThd(HI_U32 u32AppAddr,HI_S32 s32EdgeThd)
{
    U_VPSS_DIECOMBCHK1 VPSS_DIECOMBCHK1;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;
    VPSS_DIECOMBCHK1.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_DIECOMBCHK1.u32));
    VPSS_DIECOMBCHK1.bits.comb_chk_edge_thd = s32EdgeThd;
    VPSS_REG_RegWrite(&(pstReg->VPSS_DIECOMBCHK1.u32), VPSS_DIECOMBCHK1.u32);

    return HI_SUCCESS;

}

HI_S32 VPSS_REG_SetStWrAddr(HI_U32 u32AppAddr,HI_U32 u32Addr)
{
    HI_U32 VPSS_STWADDR;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;

    VPSS_STWADDR = VPSS_REG_RegRead(&(pstReg->VPSS_STWADDR));
    VPSS_STWADDR = u32Addr;
    VPSS_REG_RegWrite(&(pstReg->VPSS_STWADDR), VPSS_STWADDR);

    return HI_SUCCESS;
}
HI_S32 VPSS_REG_SetStRdAddr(HI_U32 u32AppAddr,HI_U32 u32Addr)
{
    HI_U32 VPSS_STRADDR;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;

    VPSS_STRADDR = VPSS_REG_RegRead(&(pstReg->VPSS_STRADDR));
    VPSS_STRADDR = u32Addr;
    VPSS_REG_RegWrite(&(pstReg->VPSS_STRADDR), VPSS_STRADDR);

    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetStStride(HI_U32 u32AppAddr,HI_U32 u32Stride)
{
    U_VPSS_STSTRIDE VPSS_STSTRIDE;
    VPSS_REG_S *pstReg;

    pstReg = (VPSS_REG_S*)u32AppAddr;
    VPSS_STSTRIDE.u32 = VPSS_REG_RegRead(&(pstReg->VPSS_STSTRIDE.u32));
    VPSS_STSTRIDE.bits.st_stride = u32Stride;
    VPSS_REG_RegWrite(&(pstReg->VPSS_STSTRIDE.u32), VPSS_STSTRIDE.u32);

    return HI_SUCCESS;
}

HI_S32 VPSS_REG_SetDeiParaAddr(HI_U32 u32AppAddr,HI_U32 u32ParaPhyAddr)
{
    HI_U32 VPSS_DEI_ADDR;
    VPSS_REG_S *pstReg;
    
    pstReg = (VPSS_REG_S*)u32AppAddr;
    
    VPSS_DEI_ADDR = u32ParaPhyAddr;

    VPSS_REG_RegWrite(&(pstReg->VPSS_DEI_ADDR), VPSS_DEI_ADDR); 

    return HI_SUCCESS;

}
/*************************************************************************************************/
#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif  /* __cplusplus */


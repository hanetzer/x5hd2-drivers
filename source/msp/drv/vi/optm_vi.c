//******************************************************************************

//  Copyright (C), 2003-2008, Huawei Technologies Co., Ltd.
//
//******************************************************************************
// File Name       : optm_vi.c
// Version         : 1.0
// Author          : ChenWanjun/c57657
// Created         : 2009-4-8
// Last Modified   :
// Description     : The source file of viu_driver
// Function List   :
// History         :
// 1 Date          : 2009-4-8
//   Author        : ChenWanjun/c57657
//   Modification  : Created file
//******************************************************************************

#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <asm/io.h>

#include "drv_reg_ext.h"
#include "drv_module_ext.h"
#include "drv_sys_ext.h"
#include "hi_error_mpi.h"
#include "optm_vi.h"
#include "hi_drv_vi.h"
#include "drv_venc_ext.h"
#include "drv_vdec_ext.h"

#ifndef assert
 #define assert(expr) \
    if (unlikely(!(expr))) {                                   \
        HI_ERR_VI(KERN_ERR "Assertion failed! %s,%s,%s,line=%d\n", \
                  # expr, __FILE__, __FUNCTION__, __LINE__);          \
    }
#endif

#define VIU_CHECK_NULL_PTR(ptr) \
    do {\
        if (NULL == ptr)\
        {\
            HI_ERR_VI("NULL point \r\n"); \
            return HI_ERR_VI_NULL_PTR; \
        } \
    } while (0)

#define VIU_CHECK_PORT(u32ViPort) \
    do {\
        if ((u32ViPort >= VIU_PORT_MAX * VIU_CHANNEL_MAX) || (u32ViPort < 0))\
        {\
            HI_ERR_VI("u32ViPort(%d) is invalid\r\n", u32ViPort); \
            return HI_ERR_VI_INVALID_PARA; \
        } \
    } while (0)

// Define the struct pointer of the module VIU
volatile S_VIU_REGS_TYPE *gopVIUAllReg = NULL;
VIU_FB_ROOT_S g_struFbRoot[VI_MAX_CHN_NUM];
VIU_DATA_S g_struViuData[VI_MAX_CHN_NUM];
OPTM_VI_S g_stViOptmS[VI_MAX_CHN_NUM];
HI_U32 g_u32KernUserId[VI_MAX_CHN_NUM] = {0, 0};
wait_queue_head_t g_astViWait[VI_MAX_CHN_NUM];/*wait for data*/
static HI_U32 g_u32ViuFrameSize[VI_MAX_CHN_NUM];

spinlock_t g_stViUsrIdLock;
#define VIU_UID_SPIN_LOCK spin_lock_irqsave(&g_stViUsrIdLock, flags)
#define VIU_UID_SPIN_UNLOCK spin_unlock_irqrestore(&g_stViUsrIdLock, flags)

HI_U32 Cc_int[VI_MAX_CHN_NUM] = {0, 0};
HI_U32 Buf_ovf_int[VI_MAX_CHN_NUM] = {0, 0};
HI_U32 Field_throw_int[VI_MAX_CHN_NUM] = {0, 0};
HI_U32 AHB_error_int[VI_MAX_CHN_NUM] = {0, 0};
HI_U32 Proc_err_int[VI_MAX_CHN_NUM] = {0, 0};
HI_U32 Reg_update_int[VI_MAX_CHN_NUM] = {0, 0};
HI_U32 Frame_pulse_int[VI_MAX_CHN_NUM] = {0, 0};
HI_U32 Ntsc_pal_trans_int[VI_MAX_CHN_NUM] = {0, 0};
HI_U32 Chdiv_err_int[VI_MAX_CHN_NUM] = {0, 0};
HI_U32 g_u32IntLost[VI_MAX_CHN_NUM] = {0, 0};
HI_BOOL g_abNewInt[VI_MAX_CHN_NUM][VI_UID_MAX] = {{HI_FALSE, HI_FALSE, HI_FALSE, HI_FALSE},
                                                  {HI_FALSE, HI_FALSE, HI_FALSE, HI_FALSE}};

atomic_t g_ViUidUsed[VI_MAX_CHN_NUM][VI_UID_MAX]
= { {ATOMIC_INIT(0), ATOMIC_INIT(0), ATOMIC_INIT(0), ATOMIC_INIT(0)},
    {ATOMIC_INIT(0), ATOMIC_INIT(0), ATOMIC_INIT(0), ATOMIC_INIT(0)} };

HI_MOD_ID_E g_ViUserMod[VI_MAX_CHN_NUM][VI_UID_MAX]
= {
    {HI_ID_VI, HI_ID_BUTT, HI_ID_BUTT, HI_ID_BUTT},
    {HI_ID_VI, HI_ID_BUTT, HI_ID_BUTT, HI_ID_BUTT}
};

static HI_UNF_VI_ATTR_S s_astViAttr[VI_MAX_CHN_NUM];
VI_CFG_ATTR_S g_stViCurCfg[VI_MAX_CHN_NUM];       // saved configuration

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 36))
static spinlock_t time_lock = SPIN_LOCK_UNLOCKED;
#else
static spinlock_t time_lock = __SPIN_LOCK_UNLOCKED(time_lock);
#endif

//typedef HI_S32 (*VE_IMAGE_ENCODING)(HI_VOID);

//VE_IMAGE_ENCODING gEncodeFunc = HI_NULL;

// #define RTL_VERIFY
static VENC_EXPORT_FUNC_S   *s_pVencFunc;

/******************************************************************************
  Function:     VIU_DRV_BoardInit
  Description:  this function init board
  Input:        no
  Output:       no
  Return:       no
******************************************************************************/
HI_VOID VIU_DRV_BoardInit(void)
{
    HI_U32 u32RegValue0, u32RegValue1;

    gopVIUAllReg = (S_VIU_REGS_TYPE *)IO_ADDRESS(VIU_BASE_ADDR);

    /* open VI0 and VI Bus clock */
    HI_REG_READ32(IO_ADDRESS(VIU_CRG_CONTROL), u32RegValue0);
    u32RegValue0 |= 0x300;
    HI_REG_WRITE32(IO_ADDRESS(VIU_CRG_CONTROL), u32RegValue0);

    /* open VI1 clock */
    HI_REG_READ32(IO_ADDRESS(VIU_CRG_CONTROL + 4), u32RegValue1);
    u32RegValue1 |= 0x100;
    HI_REG_WRITE32(IO_ADDRESS(VIU_CRG_CONTROL + 4), u32RegValue1);

    msleep(1);

    /* request  VI and VIU BRG reset */
    u32RegValue0 &= 0xfffffffc;
    HI_REG_WRITE32(IO_ADDRESS(VIU_CRG_CONTROL), u32RegValue0);
}

HI_VOID VIU_DRV_BoardDeinit(void)
{
    HI_U32 u32RegValue0, u32RegValue1;

    if (gopVIUAllReg)
    {
        gopVIUAllReg = NULL;
    }

    /* cancle request of VI and VIU BRG reset */
    HI_REG_READ32(IO_ADDRESS(VIU_CRG_CONTROL), u32RegValue0);
    u32RegValue0 |= 0x3;
    HI_REG_WRITE32(IO_ADDRESS(VIU_CRG_CONTROL), u32RegValue0);

    msleep(1);

    /* close VI0 and VI Bus clock */
    HI_REG_READ32(IO_ADDRESS(VIU_CRG_CONTROL + 4), u32RegValue1);
    u32RegValue1 &= 0xfffffeff;
    HI_REG_WRITE32(IO_ADDRESS(VIU_CRG_CONTROL + 4), u32RegValue1);

    /* close VI1 clock */
    u32RegValue0 &= 0xfffffcff;
    HI_REG_WRITE32(IO_ADDRESS(VIU_CRG_CONTROL), u32RegValue0);
}

/******************************************************************************
  Function:     VIU_DRV_SetPortAttr
  Description:  this function set port attribute
  Input:        u32PortId(0~4); u32PortScanMode; u32PortCapMode; u32PortMuxMode; u32PortVsync; u32PortVsyncNeg; u32PortHsync; u32PortHsyncNeg; u32PortEn
  Output:       no
  Return:       no
******************************************************************************/
HI_VOID VIU_DRV_SetPortAttr(HI_U32 u32PortId, HI_U32 u32PortScanMode, HI_U32 u32PortCapMode, HI_U32 u32PortMuxMode,
                            HI_U32 u32PortVsync, HI_U32 u32PortVsyncNeg, HI_U32 u32PortHsync, HI_U32 u32PortHsyncNeg,
                            HI_U32 u32PortEn)
{
    //PowerBenchAPI::vPrintMsg("VIU Driver is called here!\n");

    U_VI_PORT_CFG m_port_cfg;
    U_VI_PORT_CFG *pPortCfgReg = HI_NULL;

    assert(u32PortId >= 0 && u32PortId < VIU_PORT_MAX);

    switch (u32PortId)
    {
    case 0:
        pPortCfgReg = (U_VI_PORT_CFG *)(&gopVIUAllReg->VI0_PORT_CFG);
        break;
    case 1:
        pPortCfgReg = (U_VI_PORT_CFG *)(&gopVIUAllReg->VI1_PORT_CFG);
        break;
    default:
        break;
    }

    if (HI_NULL != pPortCfgReg)
    {
        m_port_cfg.u32 = pPortCfgReg->u32;

        m_port_cfg.bits.u32PortCapMode = u32PortCapMode;
        m_port_cfg.bits.u32PortEn = u32PortEn;
        m_port_cfg.bits.u32PortHsync = u32PortHsync;
        m_port_cfg.bits.u32PortHsyncNeg = u32PortHsyncNeg;
        m_port_cfg.bits.u32PortMuxMode  = u32PortMuxMode;
        m_port_cfg.bits.u32PortScanMode = u32PortScanMode;
        m_port_cfg.bits.u32PortVsync = u32PortVsync;
        m_port_cfg.bits.u32PortVsyncNeg = u32PortVsyncNeg;

        pPortCfgReg->u32 = m_port_cfg.u32;
    }
}

/******************************************************************************
  Function:     VIU_DRV_GetPortAttr
  Description:  this function get port attribute
  Input:        no
  Output:       no
  Return:       no
******************************************************************************/
HI_VOID VIU_DRV_GetPortAttr(HI_U32 u32PortId, VI_GLOBAL_PORT_ATTR *pstViAttr)
{
    U_VI_PORT_CFG m_port_cfg;
    U_VI_PORT_CFG *pPortCfgReg = HI_NULL;

    assert(u32PortId >= 0 && u32PortId < VIU_PORT_MAX);

    switch (u32PortId)
    {
    case 0:
        pPortCfgReg = (U_VI_PORT_CFG *)(&gopVIUAllReg->VI0_PORT_CFG);
        break;
    case 1:
        pPortCfgReg = (U_VI_PORT_CFG *)(&gopVIUAllReg->VI1_PORT_CFG);
        break;
    default:
        break;
    }

    if (HI_NULL != pPortCfgReg)
    {
        m_port_cfg.u32 = pPortCfgReg->u32;

        pstViAttr->u32PortCapMode  = m_port_cfg.bits.u32PortCapMode;
        pstViAttr->u32PortScanMode = m_port_cfg.bits.u32PortScanMode;
        pstViAttr->u32PortMuxMode = m_port_cfg.bits.u32PortMuxMode;
        pstViAttr->u32PortVsync = m_port_cfg.bits.u32PortVsync;
        pstViAttr->u32PortVsyncNeg = m_port_cfg.bits.u32PortVsyncNeg;
        pstViAttr->u32PortHsync = m_port_cfg.bits.u32PortHsync;
        pstViAttr->u32PortHsyncNeg = m_port_cfg.bits.u32PortHsyncNeg;
        pstViAttr->u32PortEn = m_port_cfg.bits.u32PortEn;
    }

    return;
}

/******************************************************************************
  Function:     VIU_DRV_Start
  Description:  this function set port attribute
  Input:        u32PortId(0~4); ViChn(0~4)
  Output:       no
  Return:       no
******************************************************************************/
HI_VOID VIU_DRV_Start(HI_U32 u32PortId, HI_U32 ViChn)
{
    U_VI_REG_NEWER cfg;
    U_VI_REG_NEWER *pCfg;

    assert(u32PortId >= 0 && u32PortId < VIU_PORT_MAX);
    assert(ViChn >= 0 && ViChn < VIU_CHANNEL_MAX);

    //HI_INFO_VI("start Port=%u, chn=%u.\n", u32PortId, ViChn);
    if (u32PortId == 0)
    {
        pCfg = (U_VI_REG_NEWER*)(&gopVIUAllReg->VI0_REG_NEWER);
    }
    else
    {
        pCfg = (U_VI_REG_NEWER*)(&gopVIUAllReg->VI1_REG_NEWER);
    }

    cfg.u32 = pCfg->u32;
    cfg.bits.u32RegNewer = 1;
    pCfg->u32 = cfg.u32;
}

/******************************************************************************
  Function:     VIU_DRV_Stop
  Description:  this function set port attribute
  Input:        no
  Output:       no
  Return:       no
******************************************************************************/
HI_VOID VIU_DRV_Stop(HI_U32 u32PortId, HI_U32 ViChn)
{
    U_VI_REG_NEWER cfg;
    U_VI_REG_NEWER *pCfg;

    assert(u32PortId >= 0 && u32PortId < VIU_PORT_MAX);
    assert(ViChn >= 0 && ViChn < VIU_CHANNEL_MAX);

    if (u32PortId == 0)
    {
        pCfg = (U_VI_REG_NEWER*)(&gopVIUAllReg->VI0_REG_NEWER);
    }
    else
    {
        pCfg = (U_VI_REG_NEWER*)(&gopVIUAllReg->VI1_REG_NEWER);
    }

    cfg.u32 = pCfg->u32;
    cfg.bits.u32RegNewer = 0;
    pCfg->u32 = cfg.u32;
}

HI_VOID VIU_DRV_SetCapWindowRawData(HI_U32 u32PortId, HI_U32 ViChn, HI_U32 u32CapStartX, HI_U32 u32CapStartY,
                             HI_U32 u32CapWidth, HI_U32 u32CapHeight, HI_U32 ViChnFirEn,
                             HI_U32 u32Separated, HI_U32 u32EvenLineSel)
{
    U_VI_CAP_START m_capstart;
    U_VI_CAP_START *pCapStart;

    U_VI_CAP_SIZE m_capsize;
    U_VI_CAP_SIZE  *pCapSize;

    U_VI_STORESIZE m_storesize_y;
    U_VI_STORESIZE *pStoreSizeY;

    U_VI_STORESIZE m_storesize_u;
    U_VI_STORESIZE *pStoreSizeU;

    U_VI_STORESIZE m_storesize_v;
    U_VI_STORESIZE *pStoreSizeV;
    HI_U32 u32DataWidth = 0;

    assert(u32PortId >= 0 && u32PortId < VIU_PORT_MAX);
    assert(ViChn >= 0 && ViChn < VIU_CHANNEL_MAX);

    if (u32PortId == 0)
    {
        pCapStart = (U_VI_CAP_START*)(&gopVIUAllReg->VI0_CAP_START);
        pCapSize = (U_VI_CAP_SIZE*)(&gopVIUAllReg->VI0_CAP_SIZE);
        pStoreSizeY = (U_VI_STORESIZE*)(&gopVIUAllReg->VI0_Y_STORESIZE);
        pStoreSizeU = (U_VI_STORESIZE*)(&gopVIUAllReg->VI0_U_STORESIZE);
        pStoreSizeV = (U_VI_STORESIZE*)(&gopVIUAllReg->VI0_V_STORESIZE);
    }
    else
    {
        pCapStart = (U_VI_CAP_START*)(&gopVIUAllReg->VI1_CAP_START);
        pCapSize = (U_VI_CAP_SIZE*)(&gopVIUAllReg->VI1_CAP_SIZE);
        pStoreSizeY = (U_VI_STORESIZE*)(&gopVIUAllReg->VI1_Y_STORESIZE);
        pStoreSizeU = (U_VI_STORESIZE*)(&gopVIUAllReg->VI1_U_STORESIZE);
        pStoreSizeV = (U_VI_STORESIZE*)(&gopVIUAllReg->VI1_V_STORESIZE);
    }

    m_capstart.u32 = pCapStart->u32;
    m_capsize.u32 = pCapSize->u32;
    m_storesize_y.u32 = pStoreSizeY->u32;
    m_storesize_u.u32 = pStoreSizeU->u32;
    m_storesize_v.u32 = pStoreSizeV->u32;

    m_capstart.bits.u32StartX = u32CapStartX;
    m_capstart.bits.u32StartY = u32CapStartY;

    m_capsize.bits.u32Width  = u32CapWidth;
    m_capsize.bits.u32Height = u32CapHeight;

    if (u32CapWidth % 2)
    {
        //width is odd
        u32CapWidth -= 1;
    }
    else
    {
        ;
    }     //width is even, keep it unchanged

    if (ViChnFirEn)
    {
        //down-scaling
        u32CapWidth /= 2;
    }
    else
    {
        ;
    }     //not fir

    //raw data
    if (u32DataWidth == 0) //8 bit
    {
        if ((u32CapWidth % 16))
        {
            //odd
            m_storesize_y.bits.u32Width = u32CapWidth / 16 + 1;
        }
        else
        {
            //even
            m_storesize_y.bits.u32Width = u32CapWidth / 16;
        }
    }
    else
    {
        HI_ERR_VI("Wrong data width(driver)!\n");
    }

    m_storesize_y.bits.u32Width -= 1;
    m_storesize_u.bits.u32Width -= 1;
    m_storesize_v.bits.u32Width -= 1;

    if ((((u32PortId == 1) && (ViChn == 0)) && (u32Separated == 1)) && ((u32EvenLineSel == 0) || (u32EvenLineSel == 1)))
    {
        //luma and chroma is separated and line discard
        m_storesize_y.bits.u32Height = m_capsize.bits.u32Height / 2;
    }
    else
    {
        m_storesize_y.bits.u32Height = m_capsize.bits.u32Height;
    }

    m_storesize_u.bits.u32Height = m_capsize.bits.u32Height;

    m_storesize_v.bits.u32Height = m_capsize.bits.u32Height;

    pCapStart->u32 = m_capstart.u32;
    pCapSize->u32 = m_capsize.u32;
    pStoreSizeY->u32 = m_storesize_y.u32;
    pStoreSizeU->u32 = m_storesize_u.u32;
    pStoreSizeV->u32 = m_storesize_v.u32;
}


/******************************************************************************
  Function:     VIU_DRV_SetCapWindow
  Description:  this function set port attribute
  Input:        u32PortId(0~4); ViChn(0~4); u32CapStartX; u32CapStartY; u32CapWidth; u32CapHeight; ViChnFirEn; u32StoreMethod; u32DataWidth
  Output:       no
  Return:       no
******************************************************************************/
HI_VOID VIU_DRV_SetCapWindow(HI_U32 u32PortId, HI_U32 ViChn, HI_U32 u32CapStartX, HI_U32 u32CapStartY,
                             HI_U32 u32CapWidth, HI_U32 u32CapHeight, HI_U32 ViChnFirEn,
                             HI_U32 u32StoreMethod, HI_U32 u32Separated,
                             HI_U32 u32EvenLineSel)
{
    U_VI_CAP_START m_capstart;
    U_VI_CAP_START *pCapStart;

    U_VI_CAP_SIZE m_capsize;
    U_VI_CAP_SIZE  *pCapSize;

    U_VI_STORESIZE m_storesize_y;
    U_VI_STORESIZE *pStoreSizeY;

    U_VI_STORESIZE m_storesize_u;
    U_VI_STORESIZE *pStoreSizeU;

    U_VI_STORESIZE m_storesize_v;
    U_VI_STORESIZE *pStoreSizeV;
    HI_U32 u32DataWidth = 0;

    assert(u32PortId >= 0 && u32PortId < VIU_PORT_MAX);
    assert(ViChn >= 0 && ViChn < VIU_CHANNEL_MAX);

    if (u32PortId == 0)
    {
        pCapStart = (U_VI_CAP_START*)(&gopVIUAllReg->VI0_CAP_START);
        pCapSize = (U_VI_CAP_SIZE*)(&gopVIUAllReg->VI0_CAP_SIZE);
        pStoreSizeY = (U_VI_STORESIZE*)(&gopVIUAllReg->VI0_Y_STORESIZE);
        pStoreSizeU = (U_VI_STORESIZE*)(&gopVIUAllReg->VI0_U_STORESIZE);
        pStoreSizeV = (U_VI_STORESIZE*)(&gopVIUAllReg->VI0_V_STORESIZE);
    }
    else
    {
        pCapStart = (U_VI_CAP_START*)(&gopVIUAllReg->VI1_CAP_START);
        pCapSize = (U_VI_CAP_SIZE*)(&gopVIUAllReg->VI1_CAP_SIZE);
        pStoreSizeY = (U_VI_STORESIZE*)(&gopVIUAllReg->VI1_Y_STORESIZE);
        pStoreSizeU = (U_VI_STORESIZE*)(&gopVIUAllReg->VI1_U_STORESIZE);
        pStoreSizeV = (U_VI_STORESIZE*)(&gopVIUAllReg->VI1_V_STORESIZE);
    }

    m_capstart.u32 = pCapStart->u32;
    m_capsize.u32 = pCapSize->u32;
    m_storesize_y.u32 = pStoreSizeY->u32;
    m_storesize_u.u32 = pStoreSizeU->u32;
    m_storesize_v.u32 = pStoreSizeV->u32;

    m_capstart.bits.u32StartX = u32CapStartX;
    m_capstart.bits.u32StartY = u32CapStartY;

    m_capsize.bits.u32Width  = u32CapWidth;
    m_capsize.bits.u32Height = u32CapHeight;

    if (u32CapWidth % 2)
    {
        //width is odd
        u32CapWidth -= 1;
    }
    else
    {
        ;
    }     //width is even, keep it unchanged

    if (ViChnFirEn)
    {
        //down-scaling
        u32CapWidth /= 2;
    }
    else
    {
        ;
    }     //not fir

//    if (u32StoreMethod == HI_UNF_VI_STORE_METHOD_PNYUV) //YCbCr
	if ((u32StoreMethod >= HI_UNF_FORMAT_YUV_PLANAR_400) &&
		(u32StoreMethod <= HI_UNF_FORMAT_YUV_PLANAR_410))
    {
        if (u32DataWidth == 0) //8 bit
        {
            if ((u32CapWidth % 16))
            {
                //odd
                m_storesize_y.bits.u32Width = u32CapWidth / 16 + 1;
            }
            else
            {
                //even
                m_storesize_y.bits.u32Width = u32CapWidth / 16;
            }

            if ((u32CapWidth % 32))
            {
                //odd
                m_storesize_u.bits.u32Width = u32CapWidth / 32 + 1;
            }
            else
            {
                //even
                m_storesize_u.bits.u32Width = u32CapWidth / 32;
            }

            if ((u32CapWidth % 32))
            {
                //odd
                m_storesize_v.bits.u32Width = u32CapWidth / 32 + 1;
            }
            else
            {
                //even
                m_storesize_v.bits.u32Width = u32CapWidth / 32;
            }
        }
        else
        {
            HI_ERR_VI("Wrong data width(driver)!\n");
        }
    }
	else if ((u32StoreMethod >= HI_UNF_FORMAT_YUV_SEMIPLANAR_422) &&
		(u32StoreMethod <= HI_UNF_FORMAT_YUV_SEMIPLANAR_444))
//    else if (u32StoreMethod == HI_UNF_VI_STORE_METHOD_SPNYC) //YC
    {
        if (u32DataWidth == 0) //8 bit
        {
            if ((u32CapWidth % 16))
            {
                //odd
                m_storesize_y.bits.u32Width = u32CapWidth / 16 + 1;
            }
            else
            {
                //even
                m_storesize_y.bits.u32Width = u32CapWidth / 16;
            }

            if ((u32CapWidth % 16))
            {
                //odd
                m_storesize_u.bits.u32Width = u32CapWidth / 16 + 1;
            }
            else
            {
                //even
                m_storesize_u.bits.u32Width = u32CapWidth / 16;
            }
        }
        else
        {
            HI_ERR_VI("Wrong data width(driver)!\n");
        }
    }
	
	else if ((u32StoreMethod >= HI_UNF_FORMAT_YUV_PACKAGE_UYVY) &&
		(u32StoreMethod <= HI_UNF_FORMAT_YUV_PACKAGE_YVYU))
//    else if (u32StoreMethod == HI_UNF_VI_STORE_METHOD_PKYUV) //package
    {
        if (u32DataWidth == 0) //8 bit
        {
            if ((u32CapWidth % 8))
            {
                //odd
                m_storesize_y.bits.u32Width = u32CapWidth / 8 + 1;
            }
            else
            {
                m_storesize_y.bits.u32Width = u32CapWidth / 8;
            }
        }
        else
        {
            HI_ERR_VI("Wrong data width(driver)!\n");
        }
    }
	#if 0
    else //raw data ---> MOVE to VIU_DRV_SetCapWindowRawData
    {
        if (u32DataWidth == 0) //8 bit
        {
            if ((u32CapWidth % 16))
            {
                //odd
                m_storesize_y.bits.u32Width = u32CapWidth / 16 + 1;
            }
            else
            {
                //even
                m_storesize_y.bits.u32Width = u32CapWidth / 16;
            }
        }
        else
        {
            HI_ERR_VI("Wrong data width(driver)!\n");
        }
    }
	#endif

    m_storesize_y.bits.u32Width -= 1;
    m_storesize_u.bits.u32Width -= 1;
    m_storesize_v.bits.u32Width -= 1;

    if ((((u32PortId == 1) && (ViChn == 0)) && (u32Separated == 1)) && ((u32EvenLineSel == 0) || (u32EvenLineSel == 1)))
    {
        //luma and chroma is separated and line discard
        m_storesize_y.bits.u32Height = m_capsize.bits.u32Height / 2;
    }
    else
    {
        m_storesize_y.bits.u32Height = m_capsize.bits.u32Height;
    }

    m_storesize_u.bits.u32Height = m_capsize.bits.u32Height;

    m_storesize_v.bits.u32Height = m_capsize.bits.u32Height;

    pCapStart->u32 = m_capstart.u32;
    pCapSize->u32 = m_capsize.u32;
    pStoreSizeY->u32 = m_storesize_y.u32;
    pStoreSizeU->u32 = m_storesize_u.u32;
    pStoreSizeV->u32 = m_storesize_v.u32;
}

/******************************************************************************
Function:     VIU_DRV_GetCapWindow
Description:  this function get port attribute
Input:        u32PortId(0~1); ViChn(0);
Output:       no
Return:       no
******************************************************************************/
HI_S32 VIU_DRV_GetCapWindow(HI_U32 u32PortId, HI_U32 ViChn, HI_U32 *pu32CapStartX, HI_U32 *pu32CapStartY,
                            HI_U32 *pu32CapWidth,
                            HI_U32 *pu32CapHeight)
{
    U_VI_CAP_START* pChCfg[2] = {(U_VI_CAP_START*)&(gopVIUAllReg->VI0_CAP_START),
                                 (U_VI_CAP_START*)&(gopVIUAllReg->VI1_CAP_START)};
    U_VI_CAP_SIZE* pChCfgSize[2] = {(U_VI_CAP_SIZE*)&(gopVIUAllReg->VI0_CAP_SIZE),
                                    (U_VI_CAP_SIZE*)&(gopVIUAllReg->VI1_CAP_SIZE)};

    assert(u32PortId >= 0 && u32PortId < VIU_PORT_MAX);
    assert(ViChn >= 0 && ViChn < VIU_CHANNEL_MAX);

    *pu32CapStartX = pChCfg[u32PortId]->bits.u32StartX;
    *pu32CapStartY = pChCfg[u32PortId]->bits.u32StartY;

    *pu32CapHeight = pChCfgSize[u32PortId]->bits.u32Height;
    *pu32CapWidth = pChCfgSize[u32PortId]->bits.u32Width;
    return HI_SUCCESS;
}

/******************************************************************************
  Function:     VIU_DRV_SetChnAttr
  Description:  this function set port attribute
  Input:        no
  Output:       no
  Return:       no
******************************************************************************/
HI_VOID VIU_DRV_SetChnAttr(HI_U32 u32PortId, HI_U32 ViChn, const VI_CH_ATTR* pstViAttr)
{
    U_VI_CH_CFG cfg1;
    U_VI_CH_CFG  *pCfg1;
    U_VI_CH_CTRL cfg2;
    U_VI_CH_CTRL *pCfg2;

    assert(u32PortId >= 0 && u32PortId < VIU_PORT_MAX);
    assert(ViChn >= 0 && ViChn < VIU_CHANNEL_MAX);

    if (u32PortId == 0)
    {
        pCfg1 = (U_VI_CH_CFG*)(&gopVIUAllReg->VI0_CH_CFG);
        pCfg2 = (U_VI_CH_CTRL*)(&gopVIUAllReg->VI0_CH_CTRL);
    }
    else
    {
        pCfg1 = (U_VI_CH_CFG*)(&gopVIUAllReg->VI1_CH_CFG);
        pCfg2 = (U_VI_CH_CTRL*)(&gopVIUAllReg->VI1_CH_CTRL);
    }

    cfg1.u32 = pCfg1->u32;
    cfg2.u32 = pCfg2->u32;

    cfg1.bits.u32DataWidth = pstViAttr->u32DataWidth;
    cfg1.bits.u32StoreMode = pstViAttr->u32StoreMode;
    cfg1.bits.u32CapSeq = pstViAttr->u32CapSeq;
    cfg1.bits.u32CapSel = pstViAttr->u32CapSel;
    cfg1.bits.u32StoreMethod = pstViAttr->u32StoreMethod;
    cfg1.bits.u32ChromaResample = pstViAttr->u32ChromaResample;
    cfg1.bits.u32DownScaling = pstViAttr->u32DownScaling;
    cfg1.bits.u32CorrectEn   = pstViAttr->u32CorrectEn;
    cfg1.bits.u32OddLineSel  = pstViAttr->u32OddLineSel;
    cfg1.bits.u32EvenLineSel = pstViAttr->u32EvenLineSel;
    cfg1.bits.u32ChnId     = pstViAttr->u32ChnId;
    cfg1.bits.u32ChnIdEn   = pstViAttr->u32ChnIdEn;
    cfg1.bits.u32ChromSwap = pstViAttr->u32ChromSwap;
    cfg1.bits.u32SeavFNeg  = pstViAttr->u32SeavFNeg;

    //cfg1.bits.u32PrioCtrl    = pstViAttr->u32PrioCtrl;

    cfg2.bits.u32ChEn = pstViAttr->u32ChEn;
    cfg2.bits.u32Block0En = pstViAttr->u32Block0En;
    cfg2.bits.u32Block1En = pstViAttr->u32Block1En;
    cfg2.bits.u32Block2En = pstViAttr->u32Block2En;
    cfg2.bits.u32Block3En = pstViAttr->u32Block3En;

    /*cfg2.bits.u32Block0Mode  = pstViAttr->u32Block0Mode;
       cfg2.bits.u32Block1Mode  = pstViAttr->u32Block1Mode;
       cfg2.bits.u32Block2Mode  = pstViAttr->u32Block2Mode;
       cfg2.bits.u32Block3Mode  = pstViAttr->u32Block3Mode;*/
    cfg2.bits.u32Anc0En  = pstViAttr->u32Anc0En;
    cfg2.bits.u32Anc1En  = pstViAttr->u32Anc1En;
    cfg2.bits.u32DebugEn = pstViAttr->u32DebugEn;

    pCfg1->u32 = cfg1.u32;
    pCfg2->u32 = cfg2.u32;
}

/******************************************************************************
  Function:     VIU_DRV_GetIntStatus
  Description:  this function set port attribute
  Input:        no
  Output:       no
  Message:    Keep for fpga using!!!! (y58808)
******************************************************************************/
HI_U32 VIU_DRV_GetIntStatus(HI_U32 u32PortId, HI_U32 ViChn)
{
    U_VI_INT_STATUS* pIntStatusReg[2] = {(U_VI_INT_STATUS*)&(gopVIUAllReg->VI0_INT_STATUS),
                                         (U_VI_INT_STATUS*)&(gopVIUAllReg->VI1_INT_STATUS) };

    assert(u32PortId >= 0 && u32PortId < VIU_PORT_MAX);
    assert(ViChn >= 0 && ViChn < VIU_CHANNEL_MAX);

    return pIntStatusReg[u32PortId]->u32;
}

/******************************************************************************
  Function:     VIU_DRV_ClrInterruptStatus
  Description:  this function set port attribute
  Input:        no
  Output:       no
  Return:       no
******************************************************************************/
HI_VOID VIU_DRV_ClrInterruptStatus(HI_U32 u32PortId, HI_U32 ViChn, HI_U32 u32ClrInt)
{
    U_VI_INT_STATUS *pReg;

    assert(u32PortId >= 0 && u32PortId < VIU_PORT_MAX);
    assert(ViChn >= 0 && ViChn < VIU_CHANNEL_MAX);

    if (u32PortId == 0)
    {
        pReg = (U_VI_INT_STATUS*)(&gopVIUAllReg->VI0_INT_STATUS);
    }
    else
    {
        pReg = (U_VI_INT_STATUS*)(&gopVIUAllReg->VI1_INT_STATUS);
    }

    pReg->u32 = u32ClrInt;
}

/******************************************************************************
  Function:     VIU_DRV_SetMemAddr
  Description:  this function set port attribute
  Input:        u32PortId(0~4); ViChn(0~4); u32YBaseAddr; u32UBaseAddr; u32VBaseAddr
  Output:       no
  Return:       no
******************************************************************************/

//HI_VOID VIU_DRV_SetMemAddr(HI_U32 u32PortId, HI_U32 ViChn, VI_BASE_ADDR stViAddr)
HI_VOID VIU_DRV_SetMemAddr(HI_U32 u32PortId, HI_U32 ViChn, HI_U32 u32YBaseAddr, HI_U32 u32UBaseAddr,
                           HI_U32 u32VBaseAddr)
{
    U_VI_BASE_ADDR *pYAddrReg;
    U_VI_BASE_ADDR *pUAddrReg;
    U_VI_BASE_ADDR *pVAddrReg;

    assert(u32PortId >= 0 && u32PortId < VIU_PORT_MAX);
    assert(ViChn >= 0 && ViChn < VIU_CHANNEL_MAX);

    //HI_INFO_VI("Set Port%u Chn%u Addr, %#x,%#x.\n", u32PortId,ViChn,u32YBaseAddr, u32UBaseAddr);

    if (u32PortId == 0)
    {
        pYAddrReg = (U_VI_BASE_ADDR*)(&gopVIUAllReg->VI0_Y_BASE_ADDR);
        pUAddrReg = (U_VI_BASE_ADDR*)(&gopVIUAllReg->VI0_U_BASE_ADDR);
        pVAddrReg = (U_VI_BASE_ADDR*)(&gopVIUAllReg->VI0_V_BASE_ADDR);
    }
    else
    {
        pYAddrReg = (U_VI_BASE_ADDR*)(&gopVIUAllReg->VI1_Y_BASE_ADDR);
        pUAddrReg = (U_VI_BASE_ADDR*)(&gopVIUAllReg->VI1_U_BASE_ADDR);
        pVAddrReg = (U_VI_BASE_ADDR*)(&gopVIUAllReg->VI1_V_BASE_ADDR);
    }

    pYAddrReg->u32 = u32YBaseAddr;
    pUAddrReg->u32 = u32UBaseAddr;
    pVAddrReg->u32 = u32VBaseAddr;
}

/******************************************************************************
  Function:     VIU_DRV_GetMemYAddr
  Description:  this function set port attribute
  Input:        no
  Output:       no
  Message:    Keep for fpga using!!!! (y58808)
******************************************************************************/
HI_U32 VIU_DRV_GetMemYAddr(HI_U32 u32PortId, HI_U32 ViChn)
{
    U_VI_BASE_ADDR* pYAddrReg[2] = {(U_VI_BASE_ADDR*)&(gopVIUAllReg->VI0_Y_BASE_ADDR),
                                    (U_VI_BASE_ADDR*)&(gopVIUAllReg->VI1_Y_BASE_ADDR)};

    assert(u32PortId >= 0 && u32PortId < VIU_PORT_MAX);
    assert(ViChn >= 0 && ViChn < VIU_CHANNEL_MAX);

    return pYAddrReg[u32PortId]->u32;
}

/******************************************************************************
  Function:     VIU_DRV_SetMemStride
  Description:  this function set port attribute
  Input:        u32PortId(0~4); ViChn(0~4); u32YStride; u32UStride; u32VStride
  Output:       no
  Return:       no
******************************************************************************/
HI_VOID VIU_DRV_SetMemStride(HI_U32 u32PortId, HI_U32 ViChn, HI_U32 u32YStride, HI_U32 u32UStride, HI_U32 u32VStride)
{
    U_VI_LINE_OFFSET cfg;
    U_VI_LINE_OFFSET *pStrideReg;

    assert(u32PortId >= 0 && u32PortId < VIU_PORT_MAX);
    assert(ViChn >= 0 && ViChn < VIU_CHANNEL_MAX);

    if (u32PortId == 0)
    {
        pStrideReg = (U_VI_LINE_OFFSET*)(&gopVIUAllReg->VI0_LINE_OFFSET);
    }
    else
    {
        pStrideReg = (U_VI_LINE_OFFSET*)(&gopVIUAllReg->VI1_LINE_OFFSET);
    }

    cfg.u32 = pStrideReg->u32;

    if (u32YStride % 16 == 0)
    {
        cfg.bits.u32YLineOffset = u32YStride / 16;
    }
    else
    {
        cfg.bits.u32YLineOffset = u32YStride / 16 + 1;
    }

    if (u32UStride % 16 == 0)
    {
        cfg.bits.u32ULineOffset = u32UStride / 16;
    }
    else
    {
        cfg.bits.u32ULineOffset = u32UStride / 16 + 1;
    }

    if (u32VStride % 16 == 0)
    {
        cfg.bits.u32VLineOffset = u32VStride / 16;
    }
    else
    {
        cfg.bits.u32VLineOffset = u32VStride / 16 + 1;
    }

    pStrideReg->u32 = cfg.u32;
}

/******************************************************************************
  Function:     VIU_DRV_GetLumaAdder
  Description:  add for fpga using!!(y58808)
  Input:        no
  Output:       no
  Return:       no
******************************************************************************/
HI_U32 VIU_DRV_GetLumaAdder(HI_U32 u32PortId, HI_U32 ViChn)
{
    U_VI_LUM_ADDER* pChnLumAdderReg[2] = {(U_VI_LUM_ADDER*)&(gopVIUAllReg->VI0_LUM_ADDER),
                                          (U_VI_LUM_ADDER*)&(gopVIUAllReg->VI1_LUM_ADDER)};

    assert(u32PortId >= 0 && u32PortId < VIU_PORT_MAX);
    assert(ViChn >= 0 && ViChn < VIU_CHANNEL_MAX);

    return pChnLumAdderReg[u32PortId]->u32;
}

/******************************************************************************
  Function:     VIU_DRV_GetLumaDiffAdder
  Description:  add for fpga using!!(y58808)
  Input:        no
  Output:       no
  Return:       no
******************************************************************************/
HI_U32 VIU_DRV_GetLumaDiffAdder(HI_U32 u32PortId, HI_U32 ViChn)
{
    U_VI_LUM_DIFF_ADDER* pChnLumDiffAdderReg[2] = {(U_VI_LUM_DIFF_ADDER*)&(gopVIUAllReg->VI0_LUM_DIFF_ADDER),
                                                   (U_VI_LUM_DIFF_ADDER*)&(gopVIUAllReg->VI1_LUM_DIFF_ADDER)};

    assert(u32PortId >= 0 && u32PortId < VIU_PORT_MAX);
    assert(ViChn >= 0 && ViChn < VIU_CHANNEL_MAX);

    return pChnLumDiffAdderReg[u32PortId]->u32;
}

/******************************************************************************
  Function:     VIU_DRV_GetChnStatus
  Description:  this function get channel status,like buffer/bus/channel work.
  Input:        no
  Output:       no
  Message:    Keep for fpga using!!!!
******************************************************************************/
HI_U32  VIU_DRV_GetChnStatus(HI_U32 u32PortId, HI_U32 ViChn)
{
    U_VI_STATUS* pChnStatusReg[2] = {(U_VI_STATUS*)&(gopVIUAllReg->VI0_STATUS),
                                     (U_VI_STATUS*)&(gopVIUAllReg->VI1_STATUS)};

    assert(u32PortId >= 0 && u32PortId < VIU_PORT_MAX);
    assert(ViChn >= 0 && ViChn < VIU_CHANNEL_MAX);

    return pChnStatusReg[u32PortId]->u32;
}

/******************************************************************************
  Function:     VIU_DRV_SetBlockAttr
  Description:  this function set port attribute
  Input:        no
  Output:       no
  Return:       no
******************************************************************************/
HI_VOID VIU_DRV_SetBlockAttr(HI_U32 u32PortId, HI_U32 ViChn, HI_U32 u32BlockNo, \
                             HI_U32 u32BlockStartX, HI_U32 u32BlockStartY, HI_U32 u32BlockWidth, \
                             HI_U32 u32BlockHeight, HI_U32 u32BlockColorY, HI_U32 u32BlockColorU, \
                             HI_U32 u32BlockColorV, HI_U32 u32BlockMscWidth, HI_U32 u32BlockMscHeight)
{
    U_VI_BLOCK_START BlockStartCfg;
    U_VI_BLOCK_START  *pBlockStartReg;

    U_VI_BLOCK_SIZE BlockSizeCfg;
    U_VI_BLOCK_SIZE  *pBlockSizeReg;

    U_VI_BLOCK_COLOR BlockColorCfg;
    U_VI_BLOCK_COLOR  *pBlockColorReg;

    //U_VI_BLOCK_MSC_SIZE  BlockMscSizeCfg;
    //U_VI_BLOCK_MSC_SIZE *pBlockMscSizeReg;

    assert(u32PortId >= 0 && u32PortId < VIU_PORT_MAX);
    assert(ViChn >= 0 && ViChn < VIU_CHANNEL_MAX);
    assert(u32BlockNo >= 0 && u32BlockNo <= 3);

    if (u32PortId == 0)
    {
        switch (u32BlockNo)
        {
        case 0:
            pBlockStartReg = (U_VI_BLOCK_START*)(&gopVIUAllReg->VI0_BLOCK0_START);
            pBlockSizeReg  = (U_VI_BLOCK_SIZE*)(&gopVIUAllReg->VI0_BLOCK0_SIZE);
            pBlockColorReg = (U_VI_BLOCK_COLOR*)(&gopVIUAllReg->VI0_BLOCK0_COLOR);

            // pBlockMscSizeReg = (U_VI_BLOCK_MSC_SIZE*)(&gopVIUAllReg->VI0_BLOCK0_MSC_SIZE);
            break;
        case 1:
            pBlockStartReg = (U_VI_BLOCK_START*)(&gopVIUAllReg->VI0_BLOCK1_START);
            pBlockSizeReg  = (U_VI_BLOCK_SIZE*)(&gopVIUAllReg->VI0_BLOCK1_SIZE);
            pBlockColorReg = (U_VI_BLOCK_COLOR*)(&gopVIUAllReg->VI0_BLOCK1_COLOR);

            //pBlockMscSizeReg = (U_VI_BLOCK_MSC_SIZE*)(&gopVIUAllReg->VI0_BLOCK1_MSC_SIZE);
            break;
        case 2:
            pBlockStartReg = (U_VI_BLOCK_START*)(&gopVIUAllReg->VI0_BLOCK2_START);
            pBlockSizeReg  = (U_VI_BLOCK_SIZE*)(&gopVIUAllReg->VI0_BLOCK2_SIZE);
            pBlockColorReg = (U_VI_BLOCK_COLOR*)(&gopVIUAllReg->VI0_BLOCK2_COLOR);

            //pBlockMscSizeReg = (U_VI_BLOCK_MSC_SIZE*)(&gopVIUAllReg->VI0_BLOCK2_MSC_SIZE);
            break;
        case 3:
            pBlockStartReg = (U_VI_BLOCK_START*)(&gopVIUAllReg->VI0_BLOCK3_START);
            pBlockSizeReg  = (U_VI_BLOCK_SIZE*)(&gopVIUAllReg->VI0_BLOCK3_SIZE);
            pBlockColorReg = (U_VI_BLOCK_COLOR*)(&gopVIUAllReg->VI0_BLOCK3_COLOR);

            //pBlockMscSizeReg = (U_VI_BLOCK_MSC_SIZE*)(&gopVIUAllReg->VI0_BLOCK3_MSC_SIZE);
            break;
        default:
            pBlockStartReg = (U_VI_BLOCK_START*)(&gopVIUAllReg->VI0_BLOCK0_START);
            pBlockSizeReg  = (U_VI_BLOCK_SIZE*)(&gopVIUAllReg->VI0_BLOCK0_SIZE);
            pBlockColorReg = (U_VI_BLOCK_COLOR*)(&gopVIUAllReg->VI0_BLOCK0_COLOR);

            //pBlockMscSizeReg = (U_VI_BLOCK_MSC_SIZE*)(&gopVIUAllReg->VI0_BLOCK0_MSC_SIZE);
            break;
        }
    }
    else
    {
        switch (u32BlockNo)
        {
        case 0:
            pBlockStartReg = (U_VI_BLOCK_START*)(&gopVIUAllReg->VI1_BLOCK0_START);
            pBlockSizeReg  = (U_VI_BLOCK_SIZE*)(&gopVIUAllReg->VI1_BLOCK0_SIZE);
            pBlockColorReg = (U_VI_BLOCK_COLOR*)(&gopVIUAllReg->VI1_BLOCK0_COLOR);

            //pBlockMscSizeReg = (U_VI_BLOCK_MSC_SIZE*)(&gopVIUAllReg->VI1_BLOCK0_MSC_SIZE);
            break;
        case 1:
            pBlockStartReg = (U_VI_BLOCK_START*)(&gopVIUAllReg->VI1_BLOCK1_START);
            pBlockSizeReg  = (U_VI_BLOCK_SIZE*)(&gopVIUAllReg->VI1_BLOCK1_SIZE);
            pBlockColorReg = (U_VI_BLOCK_COLOR*)(&gopVIUAllReg->VI1_BLOCK1_COLOR);

            //pBlockMscSizeReg = (U_VI_BLOCK_MSC_SIZE*)(&gopVIUAllReg->VI1_BLOCK1_MSC_SIZE);
            break;
        case 2:
            pBlockStartReg = (U_VI_BLOCK_START*)(&gopVIUAllReg->VI1_BLOCK2_START);
            pBlockSizeReg  = (U_VI_BLOCK_SIZE*)(&gopVIUAllReg->VI1_BLOCK2_SIZE);
            pBlockColorReg = (U_VI_BLOCK_COLOR*)(&gopVIUAllReg->VI1_BLOCK2_COLOR);

            //pBlockMscSizeReg = (U_VI_BLOCK_MSC_SIZE*)(&gopVIUAllReg->VI1_BLOCK2_MSC_SIZE);
            break;
        case 3:
            pBlockStartReg = (U_VI_BLOCK_START*)(&gopVIUAllReg->VI1_BLOCK3_START);
            pBlockSizeReg  = (U_VI_BLOCK_SIZE*)(&gopVIUAllReg->VI1_BLOCK3_SIZE);
            pBlockColorReg = (U_VI_BLOCK_COLOR*)(&gopVIUAllReg->VI1_BLOCK3_COLOR);

            //pBlockMscSizeReg = (U_VI_BLOCK_MSC_SIZE*)(&gopVIUAllReg->VI1_BLOCK3_MSC_SIZE);
            break;
        default:
            pBlockStartReg = (U_VI_BLOCK_START*)(&gopVIUAllReg->VI1_BLOCK0_START);
            pBlockSizeReg  = (U_VI_BLOCK_SIZE*)(&gopVIUAllReg->VI1_BLOCK0_SIZE);
            pBlockColorReg = (U_VI_BLOCK_COLOR*)(&gopVIUAllReg->VI1_BLOCK0_COLOR);

            //pBlockMscSizeReg = (U_VI_BLOCK_MSC_SIZE*)(&gopVIUAllReg->VI1_BLOCK0_MSC_SIZE);
            break;
        }
    }

    BlockStartCfg.u32 = pBlockStartReg->u32;
    BlockSizeCfg.u32  = pBlockSizeReg->u32;
    BlockColorCfg.u32 = pBlockColorReg->u32;

    //BlockMscSizeCfg.u32 = pBlockMscSizeReg->u32;

    BlockStartCfg.bits.u32BlockStartX = u32BlockStartX;
    BlockStartCfg.bits.u32BlockStartY = u32BlockStartY;

    BlockSizeCfg.bits.u32BlockWidth  = u32BlockWidth;
    BlockSizeCfg.bits.u32BlockHeight = u32BlockHeight;

    BlockColorCfg.bits.u32BlockY = u32BlockColorY;
    BlockColorCfg.bits.u32BlockU = u32BlockColorU;
    BlockColorCfg.bits.u32BlockV = u32BlockColorV;

    //BlockMscSizeCfg.bits.u32MscWidth  = u32BlockMscWidth;
    //BlockMscSizeCfg.bits.u32MscHeight = u32BlockMscHeight;

    pBlockStartReg->u32 = BlockStartCfg.u32;
    pBlockSizeReg->u32  = BlockSizeCfg.u32;
    pBlockColorReg->u32 = BlockColorCfg.u32;

    //pBlockMscSizeReg->u32 = BlockMscSizeCfg.u32;
}

/******************************************************************************
  Function:     VIU_DRV_SetAncAttr
  Description:  this function set port attribute
  Input:        no
  Output:       no
  Return:       no
******************************************************************************/

//HI_VOID VIU_DRV_SetAncAttr(HI_U32 u32PortId, HI_U32 ViChn, HI_U32 u32AncNo, const VI_ANC_ATTR* pstViAttr)
HI_VOID VIU_DRV_SetAncAttr(HI_U32 u32PortId, HI_U32 ViChn, HI_U32 u32AncNo, HI_U32 u32AncHos, \
                           HI_U32 u32AncVos, HI_U32 u32AncLoc, HI_U32 u32AncSize)
{
    U_VI_ANC_START AncStartCfg;
    U_VI_ANC_START *pAncStartReg;

    U_VI_ANC_SIZE AncSizeCfg;
    U_VI_ANC_SIZE  *pAncSizeReg;

    assert(u32PortId >= 0 && u32PortId < VIU_PORT_MAX);
    assert(ViChn >= 0 && ViChn < VIU_CHANNEL_MAX);
    assert(u32AncNo >= 0 && u32AncNo <= 1);

    if (u32PortId == 0)
    {
        switch (u32AncNo)
        {
        case 0:
            pAncStartReg = (U_VI_ANC_START*)(&gopVIUAllReg->VI0_ANC0_START);
            pAncSizeReg = (U_VI_ANC_SIZE*)(&gopVIUAllReg->VI0_ANC0_SIZE);
            break;
        case 1:
            pAncStartReg = (U_VI_ANC_START*)(&gopVIUAllReg->VI0_ANC1_START);
            pAncSizeReg = (U_VI_ANC_SIZE*)(&gopVIUAllReg->VI0_ANC1_SIZE);
            break;
        default:
            pAncStartReg = (U_VI_ANC_START*)(&gopVIUAllReg->VI0_ANC0_START);
            pAncSizeReg = (U_VI_ANC_SIZE*)(&gopVIUAllReg->VI0_ANC0_SIZE);
            break;
        }
    }
    else
    {
        switch (u32AncNo)
        {
        case 0:
            pAncStartReg = (U_VI_ANC_START*)(&gopVIUAllReg->VI1_ANC0_START);
            pAncSizeReg = (U_VI_ANC_SIZE*)(&gopVIUAllReg->VI1_ANC0_SIZE);
            break;
        case 1:
            pAncStartReg = (U_VI_ANC_START*)(&gopVIUAllReg->VI1_ANC1_START);
            pAncSizeReg = (U_VI_ANC_SIZE*)(&gopVIUAllReg->VI1_ANC1_SIZE);
            break;
        default:
            pAncStartReg = (U_VI_ANC_START*)(&gopVIUAllReg->VI1_ANC0_START);
            pAncSizeReg = (U_VI_ANC_SIZE*)(&gopVIUAllReg->VI1_ANC0_SIZE);
            break;
        }
    }

    AncStartCfg.u32 = pAncStartReg->u32;
    AncSizeCfg.u32 = pAncSizeReg->u32;

    AncStartCfg.bits.u32AncHos = u32AncHos;
    AncStartCfg.bits.u32AncVos = u32AncVos;
    AncStartCfg.bits.u32AncLoc = u32AncLoc;
    AncSizeCfg.bits.u32AncSize = u32AncSize;

    pAncStartReg->u32 = AncStartCfg.u32;
    pAncSizeReg->u32 = AncSizeCfg.u32;
}

/******************************************************************************
  Function:     VIU_DRV_SetSyncAttr
  Description:  this function set port attribute
  Input:        no
  Output:       no
  Return:       no
******************************************************************************/

//HI_VOID VIU_DRV_SetSyncAttr(HI_U32 u32PortId, const VI_GLOBAL_SYNC_ATTR* pstViAttr)
HI_VOID VIU_DRV_SetSyncAttr(HI_U32 u32PortId, HI_U32 u32Act1Height, HI_U32 u32Act1Voff, \
                            HI_U32 u32Act1Vbb, HI_U32 u32Act2Height, HI_U32 u32Act2Voff, \
                            HI_U32 u32Act2Vbb, HI_U32 u32ActWidth, HI_U32 u32ActHoff, HI_U32 u32ActHbb, \
                            HI_U32 u32VsynWidth, HI_U32 u32HsynWidthMsb, HI_U32 u32HsynWidthLsb)
{
    U_VI_VSYNC1 Vsync1Cfg;
    U_VI_VSYNC1 *pVsync1Reg;

    U_VI_VSYNC2 Vsync2Cfg;
    U_VI_VSYNC2 *pVsync2Reg;

    U_VI_HSYNC HsyncCfg;
    U_VI_HSYNC  *pHsyncReg;

    assert(u32PortId >= 0 && u32PortId < VIU_PORT_MAX);

    assert(u32PortId == 0 || u32PortId == 2);

    //    if (u32PortId == 0)
    {
        pVsync1Reg = (U_VI_VSYNC1*)(&gopVIUAllReg->VI_P0_VSYNC1);
        pVsync2Reg = (U_VI_VSYNC2*)(&gopVIUAllReg->VI_P0_VSYNC2);
        pHsyncReg = (U_VI_HSYNC*)(&gopVIUAllReg->VI_P0_HSYNC);
    }

    Vsync1Cfg.u32 = pVsync1Reg->u32;
    Vsync2Cfg.u32 = pVsync2Reg->u32;
    HsyncCfg.u32 = pHsyncReg->u32;

    Vsync1Cfg.bits.u32Act1Voff   = u32Act1Voff;
    Vsync1Cfg.bits.u32Act1Height = u32Act1Height;
    Vsync1Cfg.bits.u32Act1Vbb   = u32Act1Vbb;
    Vsync1Cfg.bits.u32VsynWidth = u32VsynWidth;

    Vsync2Cfg.bits.u32Act2Voff   = u32Act2Voff;
    Vsync2Cfg.bits.u32Act2Height = u32Act2Height;
    Vsync2Cfg.bits.u32Act2Vbb = u32Act2Vbb;
    Vsync2Cfg.bits.u32HsynWidthMsb = u32HsynWidthMsb;

    HsyncCfg.bits.u32ActHoff  = u32ActHoff;
    HsyncCfg.bits.u32ActWidth = u32ActWidth;
    HsyncCfg.bits.u32ActHbb = u32ActHbb;
    HsyncCfg.bits.u32HsynWidthLsb = u32HsynWidthLsb;

    pVsync1Reg->u32 = Vsync1Cfg.u32;
    pVsync2Reg->u32 = Vsync2Cfg.u32;
    pHsyncReg->u32 = HsyncCfg.u32;
}

/******************************************************************************
  Function:     VIU_DRV_SetIntDlyCnt
  Description:  this function set port attribute
  Input:        no
  Output:       no
  Return:       no
******************************************************************************/
HI_VOID VIU_DRV_SetIntDlyCnt(HI_U32 u32Cnt)
{
    U_VI_INT_DLY_CNT *pIntDlyCntReg;

    pIntDlyCntReg = (U_VI_INT_DLY_CNT*)(&gopVIUAllReg->VI_INT_DLY_CNT);

    pIntDlyCntReg->u32 = u32Cnt;
}

/******************************************************************************
  Function:     VIU_DRV_SetIntEn
  Description:  this function set int enable
  Input:        u32PortId(0~4); ViChn(0~4); u32Value(1-ccint ~oxffffffff)
  Output:       no
  Return:       no
******************************************************************************/
HI_VOID VIU_DRV_SetIntEn(HI_U32 u32PortId, HI_U32 ViChn, HI_U32 u32Value)
{
    U_VI_INT_EN *pIntEnReg = HI_NULL;

    HI_INFO_VI("SetIntEN,%u,%u, %#x.\n", u32PortId, ViChn, u32Value);

    assert(u32PortId >= 0 && u32PortId < VIU_PORT_MAX);
    assert(ViChn >= 0 && ViChn < VIU_CHANNEL_MAX);

    if (u32PortId == 0)
    {
        pIntEnReg = (U_VI_INT_EN*)(&gopVIUAllReg->VI0_INT_EN);
    }
    else if (u32PortId == 1)
    {
        pIntEnReg = (U_VI_INT_EN*)(&gopVIUAllReg->VI1_INT_EN);
    }

    if (HI_NULL != pIntEnReg)
    {
        pIntEnReg->u32 = u32Value;
    }
}

/******************************************************************************
  Function:     VIU_DRV_GetRawIntStatus
  Description:  this function set port attribute
  Input:        no
  Output:       no
  Return:       no
******************************************************************************/
HI_U32  VIU_DRV_GetRawIntStatus(HI_U32 u32PortId, HI_U32 ViChn)
{
    assert(u32PortId >= 0 && u32PortId < VIU_PORT_MAX);
    assert(ViChn >= 0 && ViChn < VIU_CHANNEL_MAX);

    return 1;
}

/******************************************************************************
  Function:     VIU_DRV_GetIntIndicator
  Description:  this function set port attribute
  Input:        no
  Output:       no
  Return:       no
******************************************************************************/
HI_U32  VIU_DRV_GetIntIndicator(void)
{
    U_VI_INT_INDICATOR *pIntIndicatorReg;

    pIntIndicatorReg = (U_VI_INT_INDICATOR*)(&gopVIUAllReg->VI_INT_INDICATOR);

    return pIntIndicatorReg->u32;
}

/******************************************************************************
  Function:     VIU_DRV_GetRawIntIndicator
  Description:  this function set port attribute
  Input:        no
  Output:       no
  Return:       no
******************************************************************************/
HI_U32  VIU_DRV_GetRawIntIndicator(void)
{
    return 1;
}

/******************************************************************************
  Function:     VIU_DRV_SetLumaStrh
  Description:  this function set port attribute
  Input:        no
  Output:       no
  Return:       no
******************************************************************************/
HI_VOID VIU_DRV_SetLumaStrh(HI_U32 u32PortId, HI_U32 ViChn, HI_U32 k, HI_U32 m0, HI_U32 m1)
{
    U_VI_LUM_STRH LumaStrhCfg;
    U_VI_LUM_STRH *pLumaStrhReg;

    assert(u32PortId >= 0 && u32PortId < VIU_PORT_MAX);
    assert(ViChn >= 0 && ViChn < VIU_CHANNEL_MAX);

    if (u32PortId == 0)
    {
        pLumaStrhReg = (U_VI_LUM_STRH*)(&gopVIUAllReg->VI0_LUM_STRH);
    }
    else
    {
        pLumaStrhReg = (U_VI_LUM_STRH*)(&gopVIUAllReg->VI1_LUM_STRH);
    }

    LumaStrhCfg.u32 = pLumaStrhReg->u32;
    LumaStrhCfg.bits.u32CoeffK  = k;
    LumaStrhCfg.bits.u32CoeffM0 = m0;
    LumaStrhCfg.bits.u32CoeffM1 = m1;

    pLumaStrhReg->u32 = LumaStrhCfg.u32;
}

/******************************************************************************
  Function:     VIU_DRV_GetAncData
  Description:  this function set port attribute
  Input:        no
  Output:       no
  Return:       no
******************************************************************************/
HI_VOID VIU_DRV_GetAncData(HI_U32 u32PortId, HI_U32 ViChn, HI_U32 u32AncNo, HI_U32 size, HI_U8* buf)
{
    //    U_VI_ANC_WORD *pAncWordReg;

    assert(u32PortId >= 0 && u32PortId < VIU_PORT_MAX);
    assert(ViChn >= 0 && ViChn < VIU_CHANNEL_MAX);
    assert(u32AncNo >= 0 && u32AncNo <= 1);
    assert(size % 4 == 0 && size >= 4 && size <= 32);

    return;
}

/******************************************************************************
  Function:     VIU_DRV_SetPrioCtrl
  Description:  this function set port attribute
  Input:        u32OutstandingMax(); u32PrioCtrl()
  Output:       no
  Return:       no
******************************************************************************/
HI_VOID VIU_DRV_SetPrioCtrl(HI_U32 u32OutstandingMax, HI_U32 u32PrioCtrl)
{
    U_VI_PRIO_CFG m_priocfg;
    U_VI_PRIO_CFG *pPrioCfg;

    pPrioCfg = (U_VI_PRIO_CFG*)(&gopVIUAllReg->VI_PRIO_CFG);
    m_priocfg.u32 = pPrioCfg->u32;
    m_priocfg.bits.u32Vi0PrioCtrl = u32PrioCtrl & 0x1;
    m_priocfg.bits.u32Vi1PrioCtrl = (u32PrioCtrl >> 1) & 0x1;
    m_priocfg.bits.u32Vi2PrioCtrl = (u32PrioCtrl >> 2) & 0x1;
    m_priocfg.bits.u32Vi3PrioCtrl = (u32PrioCtrl >> 3) & 0x1;
    m_priocfg.bits.u32Vi4PrioCtrl = (u32PrioCtrl >> 4) & 0x1;
    m_priocfg.bits.u32Vi5PrioCtrl = (u32PrioCtrl >> 5) & 0x1;
    m_priocfg.bits.u32Vi6PrioCtrl = (u32PrioCtrl >> 6) & 0x1;
    m_priocfg.bits.u32Vi7PrioCtrl = (u32PrioCtrl >> 7) & 0x1;

    //m_priocfg.bits.u32Vi8PrioCtrl = (u32PrioCtrl>>8) & 0x1;
    //m_priocfg.bits.u32Vi9PrioCtrl = (u32PrioCtrl>>9) & 0x1;
    //m_priocfg.bits.u32Vi10PrioCtrl = (u32PrioCtrl>>10) & 0x1;
    //m_priocfg.bits.u32Vi11PrioCtrl = (u32PrioCtrl>>11) & 0x1;
    //m_priocfg.bits.u32Vi12PrioCtrl = (u32PrioCtrl>>12) & 0x1;
    //m_priocfg.bits.u32Vi13PrioCtrl = (u32PrioCtrl>>13) & 0x1;
    //m_priocfg.bits.u32Vi14PrioCtrl = (u32PrioCtrl>>14) & 0x1;
    //m_priocfg.bits.u32Vi15PrioCtrl = (u32PrioCtrl>>15) & 0x1;
    m_priocfg.bits.u32OutstandingMax = u32OutstandingMax;
    pPrioCfg->u32 = m_priocfg.u32;
    return;
}

/******************************************************************************
Function:     VIU_DRV_SetFirLumaCoef
Description:  this function set port attribute
Input:        no
Output:       no
Return:       no
******************************************************************************/
HI_VOID VIU_DRV_SetFirLumaCoef(HI_U32 u32PortId, HI_U32 ViChn, HI_S32 i32Coef0, HI_S32 i32Coef1, \
                               HI_S32 i32Coef2, HI_S32 i32Coef3, HI_S32 i32Coef4, \
                               HI_S32 i32Coef5, HI_S32 i32Coef6, HI_S32 i32Coef7)
{
    U_VI_FIR_COEF m_luma_coef_reg0;
    U_VI_FIR_COEF m_luma_coef_reg1;
    U_VI_FIR_COEF m_luma_coef_reg2;
    U_VI_FIR_COEF m_luma_coef_reg3;

    U_VI_FIR_COEF *pLumaCoefReg0;
    U_VI_FIR_COEF *pLumaCoefReg1;
    U_VI_FIR_COEF *pLumaCoefReg2;
    U_VI_FIR_COEF *pLumaCoefReg3;

    unsigned int temp0, temp1, temp2, temp3;
    unsigned int temp4, temp5, temp6, temp7;

    assert(u32PortId >= 0 && u32PortId < VIU_PORT_MAX);
    assert(ViChn >= 0 && ViChn < VIU_CHANNEL_MAX);

    if (u32PortId == 0)
    {
        pLumaCoefReg0 = (U_VI_FIR_COEF *)(&gopVIUAllReg->VI0_LUMA_COEF0);
        pLumaCoefReg1 = (U_VI_FIR_COEF *)(&gopVIUAllReg->VI0_LUMA_COEF1);
        pLumaCoefReg2 = (U_VI_FIR_COEF *)(&gopVIUAllReg->VI0_LUMA_COEF2);
        pLumaCoefReg3 = (U_VI_FIR_COEF *)(&gopVIUAllReg->VI0_LUMA_COEF3);
    }
    else
    {
        pLumaCoefReg0 = (U_VI_FIR_COEF *)(&gopVIUAllReg->VI1_LUMA_COEF0);
        pLumaCoefReg1 = (U_VI_FIR_COEF *)(&gopVIUAllReg->VI1_LUMA_COEF1);
        pLumaCoefReg2 = (U_VI_FIR_COEF *)(&gopVIUAllReg->VI1_LUMA_COEF2);
        pLumaCoefReg3 = (U_VI_FIR_COEF *)(&gopVIUAllReg->VI1_LUMA_COEF3);
    }

    m_luma_coef_reg0.u32 = pLumaCoefReg0->u32;
    m_luma_coef_reg1.u32 = pLumaCoefReg1->u32;
    m_luma_coef_reg2.u32 = pLumaCoefReg2->u32;
    m_luma_coef_reg3.u32 = pLumaCoefReg3->u32;

#ifndef FIR_REVISE
    if (i32Coef0 < 0)
    {
        temp0 = 0x200 + ((~(-i32Coef0) + 1) & 0x1FF);
    }
    else
    {
        temp0 = i32Coef0;
    }

    if (i32Coef1 < 0)
    {
        temp1 = 0x200 + ((~(-i32Coef1) + 1) & 0x1FF);
    }
    else
    {
        temp1 = i32Coef1;
    }

    if (i32Coef2 < 0)
    {
        temp2 = 0x200 + ((~(-i32Coef2) + 1) & 0x1FF);
    }
    else
    {
        temp2 = i32Coef2;
    }

    if (i32Coef3 < 0)
    {
        temp3 = 0x200 + ((~(-i32Coef3) + 1) & 0x1FF);
    }
    else
    {
        temp3 = i32Coef3;
    }

    if (i32Coef4 < 0)
    {
        temp4 = 0x200 + ((~(-i32Coef4) + 1) & 0x1FF);
    }
    else
    {
        temp4 = i32Coef4;
    }

    if (i32Coef5 < 0)
    {
        temp5 = 0x200 + ((~(-i32Coef5) + 1) & 0x1FF);
    }
    else
    {
        temp5 = i32Coef5;
    }

    if (i32Coef6 < 0)
    {
        temp6 = 0x200 + ((~(-i32Coef6) + 1) & 0x1FF);
    }
    else
    {
        temp6 = i32Coef6;
    }

    if (i32Coef7 < 0)
    {
        temp7 = 0x200 + ((~(-i32Coef7) + 1) & 0x1FF);
    }
    else
    {
        temp7 = i32Coef7;
    }

#else
    if (i32Coef0 < 0)
    {
        temp0 = 0x200 + ((-i32Coef0) & 0x1FF);
    }
    else
    {
        temp0 = i32Coef0;
    }

    if (i32Coef1 < 0)
    {
        temp1 = 0x200 + ((-i32Coef1) & 0x1FF);
    }
    else
    {
        temp1 = i32Coef1;
    }

    if (i32Coef2 < 0)
    {
        temp2 = 0x200 + ((-i32Coef2) & 0x1FF);
    }
    else
    {
        temp2 = i32Coef2;
    }

    if (i32Coef3 < 0)
    {
        temp3 = 0x200 + ((-i32Coef3) & 0x1FF);
    }
    else
    {
        temp3 = i32Coef3;
    }

    if (i32Coef4 < 0)
    {
        temp4 = 0x200 + ((-i32Coef4) & 0x1FF);
    }
    else
    {
        temp4 = i32Coef4;
    }

    if (i32Coef5 < 0)
    {
        temp5 = 0x200 + ((-i32Coef5) & 0x1FF);
    }
    else
    {
        temp5 = i32Coef5;
    }

    if (i32Coef6 < 0)
    {
        temp6 = 0x200 + ((-i32Coef6) & 0x1FF);
    }
    else
    {
        temp6 = i32Coef6;
    }

    if (i32Coef7 < 0)
    {
        temp7 = 0x200 + ((-i32Coef7) & 0x1FF);
    }
    else
    {
        temp7 = i32Coef7;
    }
#endif


    m_luma_coef_reg0.bits.u32FirCoef0 = temp0;
    m_luma_coef_reg0.bits.u32FirCoef1 = temp1;
    m_luma_coef_reg1.bits.u32FirCoef0 = temp2;
    m_luma_coef_reg1.bits.u32FirCoef1 = temp3;
    m_luma_coef_reg2.bits.u32FirCoef0 = temp4;
    m_luma_coef_reg2.bits.u32FirCoef1 = temp5;
    m_luma_coef_reg3.bits.u32FirCoef0 = temp6;
    m_luma_coef_reg3.bits.u32FirCoef1 = temp7;

    pLumaCoefReg0->u32 = m_luma_coef_reg0.u32;
    pLumaCoefReg1->u32 = m_luma_coef_reg1.u32;
    pLumaCoefReg2->u32 = m_luma_coef_reg2.u32;
    pLumaCoefReg3->u32 = m_luma_coef_reg3.u32;
}

/******************************************************************************
Function:     VIU_DRV_SetFirChromaCoef
Description:  this function set port attribute
Input:        no
Output:       no
Return:       no
******************************************************************************/
HI_VOID VIU_DRV_SetFirChromaCoef(HI_U32 u32PortId, HI_U32 ViChn, HI_S32 i32Coef0, \
                                 HI_S32 i32Coef1, HI_S32 i32Coef2, HI_S32 i32Coef3)
{
    U_VI_FIR_COEF m_chroma_coef_reg0;
    U_VI_FIR_COEF m_chroma_coef_reg1;

    U_VI_FIR_COEF *pChromaCoefReg0;
    U_VI_FIR_COEF *pChromaCoefReg1;

    unsigned int temp0, temp1;
    unsigned int temp2, temp3;

    assert(u32PortId >= 0 && u32PortId < VIU_PORT_MAX);
    assert(ViChn >= 0 && ViChn < VIU_CHANNEL_MAX);

    if (u32PortId == 0)
    {
        pChromaCoefReg0 = (U_VI_FIR_COEF *)(&gopVIUAllReg->VI0_CHROMA_COEF0);
        pChromaCoefReg1 = (U_VI_FIR_COEF *)(&gopVIUAllReg->VI0_CHROMA_COEF1);
    }
    else
    {
        pChromaCoefReg0 = (U_VI_FIR_COEF *)(&gopVIUAllReg->VI1_CHROMA_COEF0);
        pChromaCoefReg1 = (U_VI_FIR_COEF *)(&gopVIUAllReg->VI1_CHROMA_COEF1);
    }

    m_chroma_coef_reg0.u32 = pChromaCoefReg0->u32;
    m_chroma_coef_reg1.u32 = pChromaCoefReg1->u32;

#ifndef FIR_REVISE
    if (i32Coef0 < 0)
    {
        temp0 = 0x200 + ((~(-i32Coef0) + 1) & 0x1FF);
    }
    else
    {
        temp0 = i32Coef0;
    }

    if (i32Coef1 < 0)
    {
        temp1 = 0x200 + ((~(-i32Coef1) + 1) & 0x1FF);
    }
    else
    {
        temp1 = i32Coef1;
    }

    if (i32Coef2 < 0)
    {
        temp2 = 0x200 + ((~(-i32Coef2) + 1) & 0x1FF);
    }
    else
    {
        temp2 = i32Coef2;
    }

    if (i32Coef3 < 0)
    {
        temp3 = 0x200 + ((~(-i32Coef3) + 1) & 0x1FF);
    }
    else
    {
        temp3 = i32Coef3;
    }

#else
    if (i32Coef0 < 0)
    {
        temp0 = 0x200 + ((-i32Coef0) & 0x1FF);
    }
    else
    {
        temp0 = i32Coef0;
    }

    if (i32Coef1 < 0)
    {
        temp1 = 0x200 + ((-i32Coef1) & 0x1FF);
    }
    else
    {
        temp1 = i32Coef1;
    }

    if (i32Coef2 < 0)
    {
        temp2 = 0x200 + ((-i32Coef2) & 0x1FF);
    }
    else
    {
        temp2 = i32Coef2;
    }

    if (i32Coef3 < 0)
    {
        temp3 = 0x200 + ((-i32Coef3) & 0x1FF);
    }
    else
    {
        temp3 = i32Coef3;
    }
#endif


    m_chroma_coef_reg0.bits.u32FirCoef0 = temp0;
    m_chroma_coef_reg0.bits.u32FirCoef1 = temp1;
    m_chroma_coef_reg1.bits.u32FirCoef0 = temp2;
    m_chroma_coef_reg1.bits.u32FirCoef1 = temp3;

    pChromaCoefReg0->u32 = m_chroma_coef_reg0.u32;
    pChromaCoefReg1->u32 = m_chroma_coef_reg1.u32;
}

/******************************************************************************
Function:     VIU_DRV_SetFixCode
Description:  this function set port attribute
Input:        u32Value  0:fix 1  1:fix 0
Output:       no
Return:       no
******************************************************************************/
HI_VOID VIU_DRV_SetFixCode(HI_U32 u32PortId, HI_U32 ViChn, const HI_U32 u32Value)
{
    U_VI_CH_CFG m_chcfg;
    U_VI_CH_CFG *pChCfg;

    assert(u32PortId >= 0 && u32PortId < VIU_PORT_MAX);
    assert(ViChn >= 0 && ViChn < VIU_CHANNEL_MAX);

    if (u32PortId == 0)
    {
        pChCfg = (U_VI_CH_CFG *)(&gopVIUAllReg->VI0_CH_CFG);
        m_chcfg.u32 = pChCfg->u32;
        m_chcfg.bits.u32FixCode = u32Value;
        pChCfg->u32 = m_chcfg.u32;
    }
    else
    {
        pChCfg = (U_VI_CH_CFG *)(&gopVIUAllReg->VI1_CH_CFG);
        m_chcfg.u32 = pChCfg->u32;
        m_chcfg.bits.u32FixCode = u32Value;
        pChCfg->u32 = m_chcfg.u32;
    }
}

/******************************************************************************
Function:     VIU_DRV_SetYcChannel
Description:  this function set port attribute
Input:        u32Value  0:luma channel  1:chroma channel
Output:       no
Return:       no
******************************************************************************/
HI_VOID VIU_DRV_SetYcChannel(HI_U32 u32PortId, HI_U32 ViChn, const HI_U32 u32Value)
{
    U_VI_CH_CFG m_chcfg;
    U_VI_CH_CFG *pChCfg;

    assert(u32PortId >= 0 && u32PortId < VIU_PORT_MAX);
    assert(ViChn >= 0 && ViChn < VIU_CHANNEL_MAX);

    if (u32PortId == 0)
    {
        pChCfg = (U_VI_CH_CFG *)(&gopVIUAllReg->VI0_CH_CFG);
    }
    else
    {
        pChCfg = (U_VI_CH_CFG *)(&gopVIUAllReg->VI1_CH_CFG);
    }

    m_chcfg.u32 = pChCfg->u32;
    m_chcfg.bits.u32YcChannel = u32Value;
    pChCfg->u32 = m_chcfg.u32;
}

/******************************************************************************
Function:     VIU_DRV_SetSeavFNeg
Description:  this function set port attribute
Input:        no
Output:       no
Return:       no
******************************************************************************/
HI_VOID VIU_DRV_SetSeavFNeg(HI_U32 u32PortId, HI_U32 ViChn, const HI_U32 u32Value)
{
    U_VI_CH_CFG m_chcfg;
    U_VI_CH_CFG *pChCfg;

    assert(u32PortId >= 0 && u32PortId < VIU_PORT_MAX);
    assert(ViChn >= 0 && ViChn < VIU_CHANNEL_MAX);

    if (u32PortId == 0)
    {
        pChCfg = (U_VI_CH_CFG *)(&gopVIUAllReg->VI0_CH_CFG);
        m_chcfg.u32 = pChCfg->u32;
        m_chcfg.bits.u32SeavFNeg = u32Value;
        pChCfg->u32 = m_chcfg.u32;
    }
    else if (u32PortId == 1)
    {
        pChCfg = (U_VI_CH_CFG *)(&gopVIUAllReg->VI1_CH_CFG);
        m_chcfg.u32 = pChCfg->u32;
        m_chcfg.bits.u32SeavFNeg = u32Value;
        pChCfg->u32 = m_chcfg.u32;
    }
}

/******************************************************************************
Function:     VIU_DRV_SetChromaSwap
Description:  this function set port attribute
Input:        no
Output:       no
Return:       no
******************************************************************************/
HI_VOID VIU_DRV_SetChromaSwap(HI_U32 u32PortId, HI_U32 ViChn, const HI_U32 u32Value)
{
    U_VI_CH_CFG m_chcfg;
    U_VI_CH_CFG *pChCfg;

    assert(u32PortId >= 0 && u32PortId < VIU_PORT_MAX);
    assert(ViChn >= 0 && ViChn < VIU_CHANNEL_MAX);

    if (u32PortId == 0)
    {
        pChCfg = (U_VI_CH_CFG *)(&gopVIUAllReg->VI0_CH_CFG);
        m_chcfg.u32 = pChCfg->u32;
        m_chcfg.bits.u32ChromSwap = u32Value;
        pChCfg->u32 = m_chcfg.u32;
    }
    else if (u32PortId == 1)
    {
        pChCfg = (U_VI_CH_CFG *)(&gopVIUAllReg->VI1_CH_CFG);
        m_chcfg.u32 = pChCfg->u32;
        m_chcfg.bits.u32ChromSwap = u32Value;
        pChCfg->u32 = m_chcfg.u32;
    }
}

/******************************************************************************
Function:     VIU_DRV_SetChnIdEn
Description:  this function set port attribute
Input:        no
Output:       no
Return:       no
******************************************************************************/
HI_VOID VIU_DRV_SetChnIdEn(HI_U32 u32PortId, HI_U32 ViChn, const HI_U32 u32Value)
{
    U_VI_CH_CFG m_chcfg;
    U_VI_CH_CFG *pChCfg;

    assert(u32PortId >= 0 && u32PortId < VIU_PORT_MAX);
    assert(ViChn >= 0 && ViChn < VIU_CHANNEL_MAX);

    if (u32PortId == 0)
    {
        pChCfg = (U_VI_CH_CFG *)(&gopVIUAllReg->VI0_CH_CFG);
        m_chcfg.u32 = pChCfg->u32;
        m_chcfg.bits.u32ChnIdEn = u32Value;
        pChCfg->u32 = m_chcfg.u32;
    }
    else if (u32PortId == 1)
    {
        pChCfg = (U_VI_CH_CFG *)(&gopVIUAllReg->VI1_CH_CFG);
        m_chcfg.u32 = pChCfg->u32;
        m_chcfg.bits.u32ChnIdEn = u32Value;
        pChCfg->u32 = m_chcfg.u32;
    }
}

/******************************************************************************
Function:     VIU_DRV_SetChnId
Description:  this function set port attribute
Input:        no
Output:       no
Return:       no
******************************************************************************/
HI_VOID VIU_DRV_SetChnId(HI_U32 u32PortId, HI_U32 ViChn, const HI_U32 u32Value)
{
    U_VI_CH_CFG m_chcfg;
    U_VI_CH_CFG *pChCfg;

    assert(u32PortId >= 0 && u32PortId < VIU_PORT_MAX);
    assert(ViChn >= 0 && ViChn < VIU_CHANNEL_MAX);

    if (u32PortId == 0)
    {
        pChCfg = (U_VI_CH_CFG *)(&gopVIUAllReg->VI0_CH_CFG);
        m_chcfg.u32 = pChCfg->u32;
        m_chcfg.bits.u32ChnId = u32Value;
        pChCfg->u32 = m_chcfg.u32;
    }
    else if (u32PortId == 1)
    {
        pChCfg = (U_VI_CH_CFG *)(&gopVIUAllReg->VI1_CH_CFG);
        m_chcfg.u32 = pChCfg->u32;
        m_chcfg.bits.u32ChnId = u32Value;
        pChCfg->u32 = m_chcfg.u32;
    }
}

/******************************************************************************
Function:     VIU_DRV_SetEvenLineSel
Description:  this function set port attribute
Input:        u32PortId(0~4); ViChn(0~4); eLineSel(0-HI_UNF_VI_LINESEL_ODD; 1-HI_UNF_VI_LINESEL_EVEN; 2-HI_UNF_VI_LINESEL_BOTH)
Output:       no
Return:       no
******************************************************************************/
HI_VOID VIU_DRV_SetEvenLineSel(HI_U32 u32PortId, HI_U32 ViChn, const VI_LINESEL_E eLineSel)
{
    U_VI_CH_CFG m_chcfg;
    U_VI_CH_CFG *pChCfg;

    assert(u32PortId >= 0 && u32PortId < VIU_PORT_MAX);
    assert(ViChn >= 0 && ViChn < VIU_CHANNEL_MAX);

    if (u32PortId == 0)
    {
        pChCfg = (U_VI_CH_CFG *)(&gopVIUAllReg->VI0_CH_CFG);
        m_chcfg.u32 = pChCfg->u32;
        m_chcfg.bits.u32EvenLineSel = eLineSel;
        pChCfg->u32 = m_chcfg.u32;
    }
    else if (u32PortId == 1)
    {
        pChCfg = (U_VI_CH_CFG *)(&gopVIUAllReg->VI1_CH_CFG);
        m_chcfg.u32 = pChCfg->u32;
        m_chcfg.bits.u32EvenLineSel = eLineSel;
        pChCfg->u32 = m_chcfg.u32;
    }
}

/******************************************************************************
Function:     VIU_DRV_SetOddLineSel
Description:  this function set port attribute
Input:        no
Output:       no
Return:       no
******************************************************************************/
HI_VOID VIU_DRV_SetOddLineSel(HI_U32 u32PortId, HI_U32 ViChn, const VI_LINESEL_E eLineSel)
{
    U_VI_CH_CFG m_chcfg;
    U_VI_CH_CFG *pChCfg;

    assert(u32PortId >= 0 && u32PortId < VIU_PORT_MAX);
    assert(ViChn >= 0 && ViChn < VIU_CHANNEL_MAX);

    if (u32PortId == 0)
    {
        pChCfg = (U_VI_CH_CFG *)(&gopVIUAllReg->VI0_CH_CFG);
        m_chcfg.u32 = pChCfg->u32;
        m_chcfg.bits.u32OddLineSel = eLineSel;
        pChCfg->u32 = m_chcfg.u32;
    }
    else if (u32PortId == 1)
    {
        pChCfg = (U_VI_CH_CFG *)(&gopVIUAllReg->VI1_CH_CFG);
        m_chcfg.u32 = pChCfg->u32;
        m_chcfg.bits.u32OddLineSel = eLineSel;
        pChCfg->u32 = m_chcfg.u32;
    }
}

/******************************************************************************
Function:     VIU_DRV_GetLineSel
Description:  this function get port attribute
Input:        u32PortId(0~1); ViChn(0);
Output:       no
Return:       no
******************************************************************************/
VI_LINESEL_E VIU_DRV_GetLineSel(HI_U32 u32PortId, HI_U32 ViChn)
{
    U_VI_CH_CFG* pChCfg[2] = {(U_VI_CH_CFG*)&(gopVIUAllReg->VI0_CH_CFG), (U_VI_CH_CFG*)&(gopVIUAllReg->VI1_CH_CFG)};

    assert(u32PortId >= 0 && u32PortId < VIU_PORT_MAX);
    assert(ViChn >= 0 && ViChn < VIU_CHANNEL_MAX);

    return (VI_LINESEL_E)pChCfg[u32PortId]->bits.u32OddLineSel;
}

/******************************************************************************
Function:     VIU_DRV_SetCorrectEn
Description:  this function set port attribute
Input:        no
Output:       no
Return:       no
******************************************************************************/
HI_VOID VIU_DRV_SetCorrectEn(HI_U32 u32PortId, HI_U32 ViChn, const HI_U32 u32Value)
{
    U_VI_CH_CFG m_chcfg;
    U_VI_CH_CFG *pChCfg;

    assert(u32PortId >= 0 && u32PortId < VIU_PORT_MAX);
    assert(ViChn >= 0 && ViChn < VIU_CHANNEL_MAX);

    if (u32PortId == 0)
    {
        pChCfg = (U_VI_CH_CFG *)(&gopVIUAllReg->VI0_CH_CFG);
        m_chcfg.u32 = pChCfg->u32;
        m_chcfg.bits.u32CorrectEn = u32Value;
        pChCfg->u32 = m_chcfg.u32;
    }
    else if (u32PortId == 1)
    {
        pChCfg = (U_VI_CH_CFG *)(&gopVIUAllReg->VI1_CH_CFG);
        m_chcfg.u32 = pChCfg->u32;
        m_chcfg.bits.u32CorrectEn = u32Value;
        pChCfg->u32 = m_chcfg.u32;
    }
}

/******************************************************************************
Function:     VIU_DRV_SetDownScaling
Description:  this function set port attribute
Input:        no
Output:       no
Return:       no
******************************************************************************/
HI_VOID VIU_DRV_SetDownScaling(HI_U32 u32PortId, HI_U32 ViChn, const HI_U32 u32Value)
{
    U_VI_CH_CFG m_chcfg;
    U_VI_CH_CFG *pChCfg;

    assert(u32PortId >= 0 && u32PortId < VIU_PORT_MAX);
    assert(ViChn >= 0 && ViChn < VIU_CHANNEL_MAX);

    if (u32PortId == 0)
    {
        pChCfg = (U_VI_CH_CFG *)(&gopVIUAllReg->VI0_CH_CFG);
        m_chcfg.u32 = pChCfg->u32;
        m_chcfg.bits.u32DownScaling = u32Value;
        pChCfg->u32 = m_chcfg.u32;
    }
    else
    {
        pChCfg = (U_VI_CH_CFG *)(&gopVIUAllReg->VI1_CH_CFG);
        m_chcfg.u32 = pChCfg->u32;
        m_chcfg.bits.u32DownScaling = u32Value;
        pChCfg->u32 = m_chcfg.u32;
    }
}

/******************************************************************************
Function:     VIU_DRV_SetChromaResample
Description:  this function set port attribute
Input:        no
Output:       no
Return:       no
******************************************************************************/
HI_VOID VIU_DRV_SetChromaResample(HI_U32 u32PortId, HI_U32 ViChn, const HI_U32 u32Value)
{
    U_VI_CH_CFG m_chcfg;
    U_VI_CH_CFG *pChCfg;

    assert(u32PortId >= 0 && u32PortId < VIU_PORT_MAX);
    assert(ViChn >= 0 && ViChn < VIU_CHANNEL_MAX);

    if (u32PortId == 0)
    {
        pChCfg = (U_VI_CH_CFG *)(&gopVIUAllReg->VI0_CH_CFG);
        m_chcfg.u32 = pChCfg->u32;
        m_chcfg.bits.u32ChromaResample = u32Value;
        pChCfg->u32 = m_chcfg.u32;
    }
    else if (u32PortId == 1)
    {
        pChCfg = (U_VI_CH_CFG *)(&gopVIUAllReg->VI1_CH_CFG);
        m_chcfg.u32 = pChCfg->u32;
        m_chcfg.bits.u32ChromaResample = u32Value;
        pChCfg->u32 = m_chcfg.u32;
    }
}

/******************************************************************************
Function:     VIU_DRV_SetStoreMethod
Description:  this function set port attribute
Input:        u32PortId(0~4); ViChn(0~4); eStoreMothod(0-HI_UNF_VI_STORE_METHOD_PNYUV; 1-HI_UNF_VI_STORE_METHOD_SPNYC; 2-HI_UNF_VI_STORE_METHOD_PKYUV)
Output:       no
Return:       no
******************************************************************************/
HI_VOID VIU_DRV_SetStoreMethod(HI_U32 u32PortId, HI_U32 ViChn, const HI_UNF_VIDEO_FORMAT_E eStoreMothod)
{
    U_VI_CH_CFG m_chcfg;
    U_VI_CH_CFG *pChCfg;

    assert(u32PortId >= 0 && u32PortId < VIU_PORT_MAX);
    assert(ViChn >= 0 && ViChn < VIU_CHANNEL_MAX);

    if (u32PortId == 0)
    {
        pChCfg = (U_VI_CH_CFG *)(&gopVIUAllReg->VI0_CH_CFG);
        m_chcfg.u32 = pChCfg->u32;
		if ((eStoreMothod >= HI_UNF_FORMAT_YUV_SEMIPLANAR_422) && (eStoreMothod <= HI_UNF_FORMAT_YUV_SEMIPLANAR_444))
		{
		    m_chcfg.bits.u32StoreMethod = 1;
		}
		else if ((eStoreMothod >= HI_UNF_FORMAT_YUV_PACKAGE_UYVY) && (eStoreMothod <= HI_UNF_FORMAT_YUV_PACKAGE_YVYU))
		{
			m_chcfg.bits.u32StoreMethod = 2;
		}
		else if ((eStoreMothod >= HI_UNF_FORMAT_YUV_PLANAR_400) && (eStoreMothod <= HI_UNF_FORMAT_YUV_PLANAR_410))
		{
			m_chcfg.bits.u32StoreMethod = 0;
		}
        pChCfg->u32 = m_chcfg.u32;
    }
    else if (u32PortId == 1)
    {
        pChCfg = (U_VI_CH_CFG *)(&gopVIUAllReg->VI1_CH_CFG);
        m_chcfg.u32 = pChCfg->u32;
		if ((eStoreMothod >= HI_UNF_FORMAT_YUV_SEMIPLANAR_422) && (eStoreMothod <= HI_UNF_FORMAT_YUV_SEMIPLANAR_444))
		{
		    m_chcfg.bits.u32StoreMethod = 1;
		}
		else if ((eStoreMothod >= HI_UNF_FORMAT_YUV_PACKAGE_UYVY) && (eStoreMothod <= HI_UNF_FORMAT_YUV_PACKAGE_YVYU))
		{
			m_chcfg.bits.u32StoreMethod = 2;
		}
		else if ((eStoreMothod >= HI_UNF_FORMAT_YUV_PLANAR_400) && (eStoreMothod <= HI_UNF_FORMAT_YUV_PLANAR_410))
		{
			m_chcfg.bits.u32StoreMethod = 0;
		}
        pChCfg->u32 = m_chcfg.u32;
    }
}

HI_VOID VIU_DRV_SetStoreMethodRawData(HI_U32 u32PortId, HI_U32 ViChn)
{
    U_VI_CH_CFG m_chcfg;
    U_VI_CH_CFG *pChCfg;

    assert(u32PortId >= 0 && u32PortId < VIU_PORT_MAX);
    assert(ViChn >= 0 && ViChn < VIU_CHANNEL_MAX);

    if (u32PortId == 0)
    {
        pChCfg = (U_VI_CH_CFG *)(&gopVIUAllReg->VI0_CH_CFG);
        m_chcfg.u32 = pChCfg->u32;
		m_chcfg.bits.u32StoreMethod = 3;
        pChCfg->u32 = m_chcfg.u32;
    }
    else if (u32PortId == 1)
    {
        pChCfg = (U_VI_CH_CFG *)(&gopVIUAllReg->VI1_CH_CFG);
        m_chcfg.u32 = pChCfg->u32;
		m_chcfg.bits.u32StoreMethod = 3;
        pChCfg->u32 = m_chcfg.u32;
    }
}

/******************************************************************************
Function:     VIU_DRV_SetCapSel
Description:  this function set port attribute
Input:        u32PortId(0~4); ViChn(0~4); eCapSel(0-HI_UNF_VI_CAPSEL_ODD; 1-HI_UNF_VI_CAPSEL_EVEN; 2-HI_UNF_VI_CAPSEL_BOTH)
Output:       no
Return:       no
******************************************************************************/
HI_VOID VIU_DRV_SetCapSel(HI_U32 u32PortId, HI_U32 ViChn, const HI_UNF_VI_CAPSEL_E eCapSel)
{
    U_VI_CH_CFG m_chcfg;
    U_VI_CH_CFG *pChCfg;

    assert(u32PortId >= 0 && u32PortId < VIU_PORT_MAX);
    assert(ViChn >= 0 && ViChn < VIU_CHANNEL_MAX);

    if (u32PortId == 0)
    {
        pChCfg = (U_VI_CH_CFG *)(&gopVIUAllReg->VI0_CH_CFG);
        m_chcfg.u32 = pChCfg->u32;
        m_chcfg.bits.u32CapSel = eCapSel;
        pChCfg->u32 = m_chcfg.u32;
    }
    else if (u32PortId == 1)
    {
        pChCfg = (U_VI_CH_CFG *)(&gopVIUAllReg->VI1_CH_CFG);
        m_chcfg.u32 = pChCfg->u32;
        m_chcfg.bits.u32CapSel = eCapSel;
        pChCfg->u32 = m_chcfg.u32;
    }
}

/******************************************************************************
Function:     VIU_DRV_GetCapSel
Description:  this function get port attribute
Input:        u32PortId(0~1); ViChn(0);
Output:       no
Return:       no
******************************************************************************/
HI_UNF_VI_CAPSEL_E VIU_DRV_GetCapSel(HI_U32 u32PortId, HI_U32 ViChn)
{
    U_VI_CH_CFG* pChCfg[2] = {(U_VI_CH_CFG*)&(gopVIUAllReg->VI0_CH_CFG), (U_VI_CH_CFG*)&(gopVIUAllReg->VI1_CH_CFG)};

    assert(u32PortId >= 0 && u32PortId < VIU_PORT_MAX);
    assert(ViChn >= 0 && ViChn < VIU_CHANNEL_MAX);

    return (HI_UNF_VI_CAPSEL_E)pChCfg[u32PortId]->bits.u32CapSel;
}

/******************************************************************************
Function:     VIU_DRV_SetCapSeq
Description:  this function set port attribute
Input:        no
Output:       no
Return:       no
******************************************************************************/
HI_VOID VIU_DRV_SetCapSeq(HI_U32 u32PortId, HI_U32 ViChn, const VIU_CAPSEQ_E eCapSeq)
{
    U_VI_CH_CFG m_chcfg;
    U_VI_CH_CFG *pChCfg;

    assert(u32PortId >= 0 && u32PortId < VIU_PORT_MAX);
    assert(ViChn >= 0 && ViChn < VIU_CHANNEL_MAX);

    if (u32PortId == 0)
    {
        pChCfg = (U_VI_CH_CFG *)(&gopVIUAllReg->VI0_CH_CFG);
        m_chcfg.u32 = pChCfg->u32;
        m_chcfg.bits.u32CapSeq = eCapSeq;
        pChCfg->u32 = m_chcfg.u32;
    }
    else if (u32PortId == 1)
    {
        pChCfg = (U_VI_CH_CFG *)(&gopVIUAllReg->VI1_CH_CFG);
        m_chcfg.u32 = pChCfg->u32;
        m_chcfg.bits.u32CapSeq = eCapSeq;
        pChCfg->u32 = m_chcfg.u32;
    }
}

/******************************************************************************
Function:     VIU_DRV_SetStoreMode
Description:  this function set port attribute
Input:        u32PortId(0~4); ViChn(0~4); eDataWidth(0-HI_UNF_VI_STORE_FIELD; 1-HI_UNF_VI_STORE_FRAME)
Output:       no
Return:       no
******************************************************************************/
HI_VOID VIU_DRV_SetStoreMode(HI_U32 u32PortId, HI_U32 ViChn, const HI_UNF_VIDEO_FIELD_MODE_E eStoreMode)
{
    U_VI_CH_CFG m_chcfg;
    U_VI_CH_CFG *pChCfg;

    assert(u32PortId >= 0 && u32PortId < VIU_PORT_MAX);
    assert(ViChn >= 0 && ViChn < VIU_CHANNEL_MAX);

    if (u32PortId == 0)
    {
        pChCfg = (U_VI_CH_CFG *)(&gopVIUAllReg->VI0_CH_CFG);
        m_chcfg.u32 = pChCfg->u32;
		if (HI_UNF_VIDEO_FIELD_ALL == eStoreMode)
		{
            m_chcfg.bits.u32StoreMode = 1;
		}
		else
		{
            m_chcfg.bits.u32StoreMode = 0;
		}
        pChCfg->u32 = m_chcfg.u32;
    }
    else if (u32PortId == 1)
    {
        pChCfg = (U_VI_CH_CFG *)(&gopVIUAllReg->VI1_CH_CFG);
        m_chcfg.u32 = pChCfg->u32;
		if (HI_UNF_VIDEO_FIELD_ALL == eStoreMode)
		{
            m_chcfg.bits.u32StoreMode = 1;
		}
		else
		{
            m_chcfg.bits.u32StoreMode = 0;
		}
        pChCfg->u32 = m_chcfg.u32;
    }
}

/******************************************************************************
Function:     VIU_DRV_GetStoreMode
Description:  this function set port attribute
Input:        u32PortId(0~4); ViChn(0~4); eDataWidth(0-HI_UNF_VI_STORE_FIELD; 1-HI_UNF_VI_STORE_FRAME)
Output:       no
Return:       no
******************************************************************************/
//HI_UNF_VI_STORE_MODE_E VIU_DRV_GetStoreMode(HI_U32 u32PortId, HI_U32 ViChn)
HI_U32 VIU_DRV_GetStoreMode(HI_U32 u32PortId, HI_U32 ViChn)
{
    U_VI_CH_CFG* pChCfg[2] = {(U_VI_CH_CFG*)&(gopVIUAllReg->VI0_CH_CFG), (U_VI_CH_CFG*)&(gopVIUAllReg->VI1_CH_CFG)};

    assert(u32PortId >= 0 && u32PortId < VIU_PORT_MAX);
    assert(ViChn >= 0 && ViChn < VIU_CHANNEL_MAX);

//    return (HI_UNF_VI_STORE_MODE_E)pChCfg[u32PortId]->bits.u32StoreMode;
    return (HI_U32)pChCfg[u32PortId]->bits.u32StoreMode;
}

#if 0

/******************************************************************************
Function:     VIU_DRV_SetDataWidth
Description:  this function set port attribute
Input:        u32PortId(0~4); ViChn(0~4); eDataWidth(0-HI_UNF_VI_DATA_WIDTH8; 2-HI_UNF_VI_DATA_WIDTH10)
Output:       no
Return:       no
******************************************************************************/
HI_VOID VIU_DRV_SetDataWidth(HI_U32 u32PortId, HI_U32 ViChn, const HI_UNF_VI_DATAWIDTH_E eDataWidth)
{
    U_VI_CH_CFG m_chcfg;
    U_VI_CH_CFG *pChCfg;

    assert(u32PortId >= 0 && u32PortId < VIU_PORT_MAX);
    assert(ViChn >= 0 && ViChn < VIU_CHANNEL_MAX);

    if (u32PortId == 0)
    {
        pChCfg = (U_VI_CH_CFG *)(&gopVIUAllReg->VI0_CH_CFG);
        m_chcfg.u32 = pChCfg->u32;
        m_chcfg.bits.u32DataWidth = eDataWidth;
        pChCfg->u32 = m_chcfg.u32;
    }
    else if (u32PortId == 1)
    {
        pChCfg = (U_VI_CH_CFG *)(&gopVIUAllReg->VI1_CH_CFG);
        m_chcfg.u32 = pChCfg->u32;
        m_chcfg.bits.u32DataWidth = eDataWidth;
        pChCfg->u32 = m_chcfg.u32;
    }
}

/******************************************************************************
Function:     VIU_DRV_GetDataWidth
Description:  this function get port attribute
Input:        u32PortId(0~1); ViChn(0); eDataWidth(0-HI_UNF_VI_STORE_FIELD; 1-HI_UNF_VI_STORE_FRAME)
Output:       no
Return:       no
******************************************************************************/
HI_UNF_VI_DATAWIDTH_E VIU_DRV_GetDataWidth(HI_U32 u32PortId, HI_U32 ViChn)
{
    U_VI_CH_CFG* pChCfg[2] = {(U_VI_CH_CFG*)&(gopVIUAllReg->VI0_CH_CFG), (U_VI_CH_CFG*)&(gopVIUAllReg->VI1_CH_CFG)};

    assert(u32PortId >= 0 && u32PortId < VIU_PORT_MAX);
    assert(ViChn >= 0 && ViChn < VIU_CHANNEL_MAX);

    return (HI_UNF_VI_DATAWIDTH_E)pChCfg[u32PortId]->bits.u32DataWidth;
}

#endif

/******************************************************************************
Function:     VIU_DRV_SetDebugEn
Description:  this function set port attribute
Input:        no
Output:       no
Return:       no
******************************************************************************/
HI_VOID VIU_DRV_SetDebugEn(HI_U32 u32PortId, HI_U32 ViChn, const HI_U32 u32Value)
{
    U_VI_CH_CTRL m_chctrl;
    U_VI_CH_CTRL *pChCtrl;

    assert(u32PortId >= 0 && u32PortId < VIU_PORT_MAX);
    assert(ViChn >= 0 && ViChn < VIU_CHANNEL_MAX);

    if (u32PortId == 0)
    {
        pChCtrl = (U_VI_CH_CTRL *)(&gopVIUAllReg->VI0_CH_CTRL);
        m_chctrl.u32 = pChCtrl->u32;
        m_chctrl.bits.u32DebugEn = u32Value;
        pChCtrl->u32 = m_chctrl.u32;
    }
    else if (u32PortId == 1)
    {
        pChCtrl = (U_VI_CH_CTRL *)(&gopVIUAllReg->VI1_CH_CTRL);
        m_chctrl.u32 = pChCtrl->u32;
        m_chctrl.bits.u32DebugEn = u32Value;
        pChCtrl->u32 = m_chctrl.u32;
    }
}

/******************************************************************************
Function:     VIU_DRV_SetLumStrhEn
Description:  this function set port attribute
Input:        no
Output:       no
Return:       no
******************************************************************************/
HI_VOID VIU_DRV_SetLumStrhEn(HI_U32 u32PortId, HI_U32 ViChn, const HI_U32 u32Value)
{
    U_VI_CH_CTRL m_chctrl;
    U_VI_CH_CTRL *pChCtrl;

    assert(u32PortId >= 0 && u32PortId < VIU_PORT_MAX);
    assert(ViChn >= 0 && ViChn < VIU_CHANNEL_MAX);

    if (u32PortId == 0)
    {
        pChCtrl = (U_VI_CH_CTRL *)(&gopVIUAllReg->VI0_CH_CTRL);
        m_chctrl.u32 = pChCtrl->u32;
        m_chctrl.bits.u32LumStrhEn = u32Value;
        pChCtrl->u32 = m_chctrl.u32;
    }
    else if (u32PortId == 1)
    {
        pChCtrl = (U_VI_CH_CTRL *)(&gopVIUAllReg->VI1_CH_CTRL);
        m_chctrl.u32 = pChCtrl->u32;
        m_chctrl.bits.u32LumStrhEn = u32Value;
        pChCtrl->u32 = m_chctrl.u32;
    }
}

/******************************************************************************
Function:     VIU_DRV_SetAncEn
Description:  this function set port attribute
Input:        no
Output:       no
Return:       no
******************************************************************************/
HI_VOID VIU_DRV_SetAncEn(HI_U32 u32PortId, HI_U32 ViChn, HI_U32 u32AncNo, const HI_U32 u32Value)
{
    U_VI_CH_CTRL m_chctrl;
    U_VI_CH_CTRL *pChCtrl;

    assert(u32PortId >= 0 && u32PortId < VIU_PORT_MAX);
    assert(ViChn >= 0 && ViChn < VIU_CHANNEL_MAX);
    assert(u32AncNo >= 0 && u32AncNo <= 1);

    if (u32PortId == 0)
    {
        switch (u32AncNo)
        {
        case 0:
            pChCtrl = (U_VI_CH_CTRL *)(&gopVIUAllReg->VI0_CH_CTRL);
            m_chctrl.u32 = pChCtrl->u32;
            m_chctrl.bits.u32Anc0En = u32Value;
            pChCtrl->u32 = m_chctrl.u32;

            //RTL_Verify_RegWrite(VIU_BASE_ADDR, (HI_U32 *)(&gopVIUAllReg->VI0_CH_CTRL));
            break;
        case 1:
            pChCtrl = (U_VI_CH_CTRL *)(&gopVIUAllReg->VI0_CH_CTRL);
            m_chctrl.u32 = pChCtrl->u32;
            m_chctrl.bits.u32Anc1En = u32Value;
            pChCtrl->u32 = m_chctrl.u32;

            //RTL_Verify_RegWrite(VIU_BASE_ADDR, (HI_U32 *)(&gopVIUAllReg->VI0_CH_CTRL));
            break;
        default:
            break;
        }
    }
    else if (u32PortId == 1)
    {
        switch (u32AncNo)
        {
        case 0:
            pChCtrl = (U_VI_CH_CTRL *)(&gopVIUAllReg->VI1_CH_CTRL);
            m_chctrl.u32 = pChCtrl->u32;
            m_chctrl.bits.u32Anc0En = u32Value;
            pChCtrl->u32 = m_chctrl.u32;

            //RTL_Verify_RegWrite(VIU_BASE_ADDR, (HI_U32 *)(&gopVIUAllReg->VI1_CH_CTRL));
            break;
        case 1:
            pChCtrl = (U_VI_CH_CTRL *)(&gopVIUAllReg->VI1_CH_CTRL);
            m_chctrl.u32 = pChCtrl->u32;
            m_chctrl.bits.u32Anc1En = u32Value;
            pChCtrl->u32 = m_chctrl.u32;

            //RTL_Verify_RegWrite(VIU_BASE_ADDR, (HI_U32 *)(&gopVIUAllReg->VI1_CH_CTRL));
            break;
        default:
            break;
        }
    }
}

#if 0

/******************************************************************************
Function:     VIU_DRV_SetBlockMode
Description:  this function set port attribute
Input:        no
Output:       no
Return:       no
******************************************************************************/
HI_VOID VIU_DRV_SetBlockMode(HI_U32 u32PortId, HI_U32 ViChn, HI_U32 u32BlockNo, const VIU_BLOCK_COVER_E eBlockFill)
{
    U_VI_CH_CTRL m_chctrl;
    U_VI_CH_CTRL *pChCtrl;

    assert(u32PortId >= 0 && u32PortId < VIU_PORT_MAX);
    assert(ViChn >= 0 && ViChn < VIU_CHANNEL_MAX);
    assert(u32BlockNo >= 0 && u32BlockNo <= 3);

    if (u32PortId == 0)
    {
        switch (u32BlockNo)
        {
        case 0:
            pChCtrl = (U_VI_CH_CTRL *)(&gopVIUAllReg->VI0_CH_CTRL);
            m_chctrl.u32 = pChCtrl->u32;
            m_chctrl.bits.u32Block0Mode = eBlockFill;
            pChCtrl->u32 = m_chctrl.u32;

            //RTL_Verify_RegWrite(VIU_BASE_ADDR, (HI_U32 *)(&gopVIUAllReg->VI0_CH_CTRL));
            break;
        case 1:
            pChCtrl = (U_VI_CH_CTRL *)(&gopVIUAllReg->VI0_CH_CTRL);
            m_chctrl.u32 = pChCtrl->u32;
            m_chctrl.bits.u32Block1Mode = eBlockFill;
            pChCtrl->u32 = m_chctrl.u32;

            //RTL_Verify_RegWrite(VIU_BASE_ADDR, (HI_U32 *)(&gopVIUAllReg->VI0_CH_CTRL));
            break;
        case 2:
            pChCtrl = (U_VI_CH_CTRL *)(&gopVIUAllReg->VI0_CH_CTRL);
            m_chctrl.u32 = pChCtrl->u32;
            m_chctrl.bits.u32Block2Mode = eBlockFill;
            pChCtrl->u32 = m_chctrl.u32;

            //RTL_Verify_RegWrite(VIU_BASE_ADDR, (HI_U32 *)(&gopVIUAllReg->VI0_CH_CTRL));
            break;
        case 3:
            pChCtrl = (U_VI_CH_CTRL *)(&gopVIUAllReg->VI0_CH_CTRL);
            m_chctrl.u32 = pChCtrl->u32;
            m_chctrl.bits.u32Block3Mode = eBlockFill;
            pChCtrl->u32 = m_chctrl.u32;

            //RTL_Verify_RegWrite(VIU_BASE_ADDR, (HI_U32 *)(&gopVIUAllReg->VI0_CH_CTRL));
            break;
        }
    }
    else if (u32PortId == 1)
    {
        switch (u32BlockNo)
        {
        case 0:
            pChCtrl = (U_VI_CH_CTRL *)(&gopVIUAllReg->VI1_CH_CTRL);
            m_chctrl.u32 = pChCtrl->u32;
            m_chctrl.bits.u32Block0Mode = eBlockFill;
            pChCtrl->u32 = m_chctrl.u32;
            break;
        case 1:
            pChCtrl = (U_VI_CH_CTRL *)(&gopVIUAllReg->VI1_CH_CTRL);
            m_chctrl.u32 = pChCtrl->u32;
            m_chctrl.bits.u32Block1Mode = eBlockFill;
            pChCtrl->u32 = m_chctrl.u32;
            break;
        case 2:
            pChCtrl = (U_VI_CH_CTRL *)(&gopVIUAllReg->VI1_CH_CTRL);
            m_chctrl.u32 = pChCtrl->u32;
            m_chctrl.bits.u32Block2Mode = eBlockFill;
            pChCtrl->u32 = m_chctrl.u32;
            break;
        case 3:
            pChCtrl = (U_VI_CH_CTRL *)(&gopVIUAllReg->VI1_CH_CTRL);
            m_chctrl.u32 = pChCtrl->u32;
            m_chctrl.bits.u32Block3Mode = eBlockFill;
            pChCtrl->u32 = m_chctrl.u32;
            break;
        }
    }
}

#endif

/******************************************************************************
Function:     VIU_DRV_SetBlockEn
Description:  this function set port attribute
Input:        no
Output:       no
Return:       no
******************************************************************************/
HI_VOID VIU_DRV_SetBlockEn(HI_U32 u32PortId, HI_U32 ViChn, HI_U32 u32BlockNo, const HI_U32 u32Value)
{
    U_VI_CH_CTRL m_chctrl;
    U_VI_CH_CTRL *pChCtrl;

    assert(u32PortId >= 0 && u32PortId < VIU_PORT_MAX);
    assert(ViChn >= 0 && ViChn < VIU_CHANNEL_MAX);
    assert(u32BlockNo >= 0 && u32BlockNo <= 3);

    if (u32PortId == 0)
    {
        switch (u32BlockNo)
        {
        case 0:
            pChCtrl = (U_VI_CH_CTRL *)(&gopVIUAllReg->VI0_CH_CTRL);
            m_chctrl.u32 = pChCtrl->u32;
            m_chctrl.bits.u32Block0En = u32Value;
            pChCtrl->u32 = m_chctrl.u32;
            break;
        case 1:
            pChCtrl = (U_VI_CH_CTRL *)(&gopVIUAllReg->VI0_CH_CTRL);
            m_chctrl.u32 = pChCtrl->u32;
            m_chctrl.bits.u32Block1En = u32Value;
            pChCtrl->u32 = m_chctrl.u32;
            break;
        case 2:
            pChCtrl = (U_VI_CH_CTRL *)(&gopVIUAllReg->VI0_CH_CTRL);
            m_chctrl.u32 = pChCtrl->u32;
            m_chctrl.bits.u32Block2En = u32Value;
            pChCtrl->u32 = m_chctrl.u32;
            break;
        case 3:
            pChCtrl = (U_VI_CH_CTRL *)(&gopVIUAllReg->VI0_CH_CTRL);
            m_chctrl.u32 = pChCtrl->u32;
            m_chctrl.bits.u32Block3En = u32Value;
            pChCtrl->u32 = m_chctrl.u32;
            break;
        default:
            break;
        }
    }
    else if (u32PortId == 1)
    {
        switch (u32BlockNo)
        {
        case 0:
            pChCtrl = (U_VI_CH_CTRL *)(&gopVIUAllReg->VI1_CH_CTRL);
            m_chctrl.u32 = pChCtrl->u32;
            m_chctrl.bits.u32Block0En = u32Value;
            pChCtrl->u32 = m_chctrl.u32;
            break;
        case 1:
            pChCtrl = (U_VI_CH_CTRL *)(&gopVIUAllReg->VI1_CH_CTRL);
            m_chctrl.u32 = pChCtrl->u32;
            m_chctrl.bits.u32Block1En = u32Value;
            pChCtrl->u32 = m_chctrl.u32;
            break;
        case 2:
            pChCtrl = (U_VI_CH_CTRL *)(&gopVIUAllReg->VI1_CH_CTRL);
            m_chctrl.u32 = pChCtrl->u32;
            m_chctrl.bits.u32Block2En = u32Value;
            pChCtrl->u32 = m_chctrl.u32;
            break;
        case 3:
            pChCtrl = (U_VI_CH_CTRL *)(&gopVIUAllReg->VI1_CH_CTRL);
            m_chctrl.u32 = pChCtrl->u32;
            m_chctrl.bits.u32Block3En = u32Value;
            pChCtrl->u32 = m_chctrl.u32;
            break;
        default:
            break;
        }
    }
}

/******************************************************************************
Function:     VIU_DRV_SetChEn
Description:  this function set port attribute
Input:        u32PortId(0~4); ViChn(0~4); u32Value(0-disable; 1-enable)
Output:       no
Return:       no
******************************************************************************/
HI_VOID VIU_DRV_SetChEn(HI_U32 u32PortId, HI_U32 ViChn, const HI_U32 u32Value)
{
    U_VI_CH_CTRL m_chctrl;
    U_VI_CH_CTRL *pChCtrl;

    assert(u32PortId >= 0 && u32PortId < VIU_PORT_MAX);
    assert(ViChn >= 0 && ViChn < VIU_CHANNEL_MAX);

    HI_INFO_VI("SetChnEN,%u,%u, %#x.\n", u32PortId, ViChn, u32Value);

    if (u32PortId == 0)
    {
        pChCtrl = (U_VI_CH_CTRL *)(&gopVIUAllReg->VI0_CH_CTRL);
        m_chctrl.u32 = pChCtrl->u32;
        m_chctrl.bits.u32ChEn = u32Value;
        pChCtrl->u32 = m_chctrl.u32;
    }
    else if (u32PortId == 1)
    {
        pChCtrl = (U_VI_CH_CTRL *)(&gopVIUAllReg->VI1_CH_CTRL);
        m_chctrl.u32 = pChCtrl->u32;
        m_chctrl.bits.u32ChEn = u32Value;
        pChCtrl->u32 = m_chctrl.u32;
    }
}

HI_S32 Viu_OpenChn(HI_UNF_VI_E enVi)
{
    unsigned long flags;
    HI_CHIP_TYPE_E enChipType;

    if ((HI_UNF_VI_PORT0 > enVi) || (enVi > HI_UNF_VI_PORT1))
    {
        HI_ERR_VI("%s, %d\n", __FUNCTION__, __LINE__);
        return HI_ERR_VI_INVALID_PARA;
    }

    HI_DRV_SYS_GetChipVersion(&enChipType, HI_NULL);
    if ((HI_CHIP_TYPE_HI3716C == enChipType) || (HI_CHIP_TYPE_HI3716H == enChipType))
    {
        if (!gopVIUAllReg)
        {
            HI_ERR_VI("Viu_OpenChn failed!vi attr not set.\n");
            return HI_FAILURE;
        }
    }

    VIU_UID_SPIN_LOCK;

    if (g_stViOptmS[enVi].bOpened)
    {
        VIU_UID_SPIN_UNLOCK;
        return HI_SUCCESS;
    }

    g_stViOptmS[enVi].bOpened = HI_TRUE;
    Reg_update_int[enVi] = 0;

    VIU_UID_SPIN_UNLOCK;

    g_stViOptmS[enVi].u32RevFrmFrIntf = 0;
    g_stViOptmS[enVi].u32RevFrmFrUsr = 0;
    g_stViOptmS[enVi].u32SndFrmToMd = 0;

    memset(&(g_stViOptmS[enVi].stStatisticInfo), 0, sizeof(VI_STATISTIC_S));

    do_gettimeofday(&g_stViOptmS[enVi].stTimeStart);

    g_u32IntLost[enVi] = 0;

    HI_INFO_VI("Open Vi%d  \n", enVi);
    if ((HI_CHIP_TYPE_HI3716C == enChipType) || (HI_CHIP_TYPE_HI3716H == enChipType))
    {
        VIU_DRV_SetIntEn(enVi, 0, 0xff);
//        VIU_DRV_SetChEn(enVi, 0, 1); -->>MOVE to start
        VIU_DRV_Start(enVi, 0);
    }

    HI_INFO_VI("<===Open Vi%d  \n", enVi);

    g_u32KernUserId[enVi] = 0;

    //u32BufAddr = VIU_FbGet(g_u32KernUserId[enVi], &g_struFbRoot[enVi]);
    //g_struViuData[enVi].u32NextBufIndex = VIU_FB_P2I(&g_struFbRoot[enVi], u32BufAddr);
    g_struViuData[enVi].u32NextBufIndex = 0;
    return HI_SUCCESS;
}

HI_S32 Viu_CloseChn(HI_UNF_VI_E enVi, HI_BOOL bForce)
{
    unsigned long flags;
    HI_S32 i = 0;
    HI_CHIP_TYPE_E enChipType;

    if ((HI_UNF_VI_PORT0 > enVi) || (enVi > HI_UNF_VI_PORT1))
    {
        HI_ERR_VI("enVI invalid: enVi=%d.\n", enVi);
        return HI_ERR_VI_INVALID_PARA;
    }

    if (!g_stViOptmS[enVi].bOpened)
    {
        return HI_SUCCESS;
    }

    //if sink still attaching...
    if (HI_FALSE == bForce)
    {
        for (i = 1; i < VI_UID_MAX; i++)
        {
            if (0 != atomic_read(&(g_ViUidUsed[enVi][i])))
            {
                HI_ERR_VI("Vi is Attached by someone(%d), detach it first. enVi=%d.\n", enVi, i);
                return HI_ERR_VI_BUSY;
            }
        }
    }
    else
    {
        for (i = 1; i < VI_UID_MAX; i++)
        {
            atomic_set(&(g_ViUidUsed[enVi][i]), 0);
        }
    }

    HI_DRV_SYS_GetChipVersion(&enChipType, HI_NULL);
    if ((HI_CHIP_TYPE_HI3716C == enChipType) || (HI_CHIP_TYPE_HI3716H == enChipType))
    {
        VIU_DRV_Stop(enVi, 0);
        VIU_DRV_SetIntEn(enVi, 0, 0);
//        VIU_DRV_SetChEn(enVi, 0, 0); -->MOVE to stop
        VIU_DRV_ClrInterruptStatus(enVi, 0, 0x1FF);
    }

    VIU_FbCleanup(&g_struFbRoot[enVi]);
//    if ((s_astViAttr[HI_UNF_VI_PORT1].enInputMode >= HI_UNF_VI_MODE_BT1120_480P)
//        && (s_astViAttr[HI_UNF_VI_PORT1].enChnYC == HI_UNF_VI_CHN_YC_SEL_C))
    if (s_astViAttr[HI_UNF_VI_PORT1].enInputMode >= HI_UNF_VI_MODE_BT1120_480P)
	{
        VIU_FbCleanup(&g_struFbRoot[HI_UNF_VI_PORT1]);
    }

    VIU_UID_SPIN_LOCK;
    g_stViOptmS[enVi].bOpened = HI_FALSE;
    g_stViCurCfg[enVi].bValid = HI_TRUE;
    g_u32IntLost[enVi]   = 0;
    Reg_update_int[enVi] = 0;
    VIU_UID_SPIN_UNLOCK;

    HI_INFO_VI("Viu_close Ok!\r\n");
    return HI_SUCCESS;
}

HI_S32 Viu_GetAttr(HI_U32 u32ViPort, HI_UNF_VI_ATTR_S  *pstAttr)
{
    if (HI_NULL == pstAttr)
    {
        return HI_ERR_VI_NULL_PTR;
    }

    if ((HI_UNF_VI_PORT0 > u32ViPort) || (u32ViPort > HI_UNF_VI_PORT1))
    {
        HI_ERR_VI("%s, %d\n", __FUNCTION__, __LINE__);
        return HI_ERR_VI_INVALID_PARA;
    }

    memcpy(pstAttr, &(g_stViCurCfg[u32ViPort].stViCfg), sizeof(HI_UNF_VI_ATTR_S));

    return HI_SUCCESS;
}

HI_S32 Viu_SetAttr(HI_U32 u32ViPort, HI_UNF_VI_ATTR_S * pstAttr)
{
    HI_U32 u32RegValue = 0;
    HI_BOOL bField = HI_FALSE;
    HI_U32 YStride = 0, CStride = 0;
    int k = 0;
    HI_U32 u32ScanMode = 0;
    HI_U32 u32CapMode = 0;
    HI_CHIP_TYPE_E enChipType;

    if (g_stViOptmS[u32ViPort].bOpened)
    {
        return HI_ERR_VI_CHN_INVALID_STAT;
    }

    if (HI_NULL == pstAttr)
    {
        return HI_ERR_VI_NULL_PTR;
    }

    if ((HI_UNF_VI_PORT0 > u32ViPort) || (u32ViPort > HI_UNF_VI_PORT1))
    {
        return HI_ERR_VI_INVALID_PARA;
    }

    HI_INFO_VI("SetVi%d Attr, VideoFormat:%d, Mode:%d, %d*%d\n", u32ViPort, pstAttr->enVideoFormat,
               pstAttr->enInputMode,
               pstAttr->stInputRect.s32Width, pstAttr->stInputRect.s32Height);

    if ((pstAttr->enCapSel > HI_UNF_VI_CAPSEL_BOTH)/* || (pstAttr->enChnYC > HI_UNF_VI_CHN_YC_SEL_C)*/
        || (pstAttr->enInputMode >= HI_UNF_VI_MODE_BUTT)/* || (pstAttr->enViPort >= HI_UNF_VI_BUTT)*/)
    {
        return HI_ERR_VI_INVALID_PARA;
    }

    if ((pstAttr->enCapSel < HI_UNF_VI_CAPSEL_ODD)/* || (pstAttr->enChnYC < HI_UNF_VI_CHN_YC_SEL_Y)*/
        || (pstAttr->enInputMode < HI_UNF_VI_MODE_BT656_576I)/* || (pstAttr->enViPort < HI_UNF_VI_PORT0)*/
        || (pstAttr->stInputRect.s32X < 0) || (pstAttr->stInputRect.s32Y < 0))
    {
        return HI_ERR_VI_INVALID_PARA;
    }
#if 0
    if ((pstAttr->enInputMode == HI_UNF_VI_MODE_DIGITAL_CAMERA)
        && ((pstAttr->stInputRect.s32Height > 1536) || (pstAttr->stInputRect.s32Width > 2048)))
    {
        return HI_ERR_VI_INVALID_PARA;
    }
#endif
    if ((pstAttr->enInputMode == HI_UNF_VI_MODE_BT656_576I)
        && ((pstAttr->stInputRect.s32Height > 1280) || (pstAttr->stInputRect.s32Width > 1920)))
    {
        return HI_ERR_VI_INVALID_PARA;
    }

    if (((pstAttr->enInputMode == HI_UNF_VI_MODE_BT601_576I) || (pstAttr->enInputMode == HI_UNF_VI_MODE_BT601_480I))
        && ((pstAttr->stInputRect.s32Height > 1280) || (pstAttr->stInputRect.s32Width > 1920)))
    {
        return HI_ERR_VI_INVALID_PARA;
    }
#if 0
    if ((pstAttr->enInputMode == HI_UNF_VI_MODE_USB_CAM)
        && ((pstAttr->stInputRect.s32Height > 1280) || (pstAttr->stInputRect.s32Width > 1920)))
    {
        return HI_ERR_VI_INVALID_PARA;
    }
#endif
    if ((pstAttr->enInputMode >= HI_UNF_VI_MODE_BT1120_480P)
        && ((pstAttr->stInputRect.s32Height > 1280) || (pstAttr->stInputRect.s32Width > 1920)))
    {
        return HI_ERR_VI_INVALID_PARA;
    }

    if ((pstAttr->u32BufNum < VIU_FB_MIN_NUM) || ((pstAttr->u32BufNum > VIU_FB_MAX_NUM)))
    {
        return HI_ERR_VI_INVALID_PARA;
    }

    init_waitqueue_head(&g_astViWait[u32ViPort]);
    HI_DRV_SYS_GetChipVersion(&enChipType, HI_NULL);

    /* VI1 selects clock from vi0_clk_in */
    if ((HI_CHIP_TYPE_HI3716C == enChipType) && (pstAttr->enInputMode >= HI_UNF_VI_MODE_BT1120_480P))
    {
        HI_REG_READ32(IO_ADDRESS(VIU_CRG_CONTROL + 4), u32RegValue);
        u32RegValue |= 0x10000;
        HI_REG_WRITE32(IO_ADDRESS(VIU_CRG_CONTROL + 4), u32RegValue);
    }

    if ((HI_CHIP_TYPE_HI3716C == enChipType) || (HI_CHIP_TYPE_HI3716H == enChipType))
    {
        VIU_DRV_Stop(u32ViPort, 0);
        VIU_DRV_SetIntEn(u32ViPort, 0, 0);
        VIU_DRV_ClrInterruptStatus(u32ViPort, 0, 0x1FF);
    }

    VIU_FbCleanup(&g_struFbRoot[u32ViPort]);
    memset(&g_struFbRoot[u32ViPort], 0x00, sizeof(g_struFbRoot[u32ViPort]));

    memcpy(&(s_astViAttr[u32ViPort]), pstAttr, sizeof(HI_UNF_VI_ATTR_S));

    switch (pstAttr->enInputMode)
    {
    case HI_UNF_VI_MODE_BT1120_480P:
        u32CapMode  = 0;
        u32ScanMode = 3; /* separate Y/C, progressive */
		if ((pstAttr->enInputMode >= HI_UNF_VI_MODE_BT1120_480P) && (1 == u32ViPort))
		{
			VIU_DRV_SetYcChannel(u32ViPort, 0, 1);
		}
		else
		{
			VIU_DRV_SetYcChannel(u32ViPort, 0, 0);
		}
//        VIU_DRV_SetYcChannel(u32ViPort, 0, pstAttr->enChnYC);
        bField = HI_TRUE;
        break;

    case HI_UNF_VI_MODE_BT1120_576P:
        u32CapMode  = 0;
        u32ScanMode = 3; /* separate Y/C, progressive */
		if ((pstAttr->enInputMode >= HI_UNF_VI_MODE_BT1120_480P) && (1 == u32ViPort))
		{
			VIU_DRV_SetYcChannel(u32ViPort, 0, 1);
		}
		else
		{
			VIU_DRV_SetYcChannel(u32ViPort, 0, 0);
		}
//        VIU_DRV_SetYcChannel(u32ViPort, 0, pstAttr->enChnYC);
        bField = HI_TRUE;
        break;

    case HI_UNF_VI_MODE_BT1120_720P_50:
        u32CapMode  = 0;
        u32ScanMode = 3; /* separate Y/C, progressive */
		if ((pstAttr->enInputMode >= HI_UNF_VI_MODE_BT1120_480P) && (1 == u32ViPort))
		{
			VIU_DRV_SetYcChannel(u32ViPort, 0, 1);
		}
		else
		{
			VIU_DRV_SetYcChannel(u32ViPort, 0, 0);
		}
//        VIU_DRV_SetYcChannel(u32ViPort, 0, pstAttr->enChnYC);
        bField = HI_TRUE;
        break;

    case HI_UNF_VI_MODE_BT1120_720P_60:
        u32CapMode  = 0;
        u32ScanMode = 3; /* separate Y/C, progressive */
		if ((pstAttr->enInputMode >= HI_UNF_VI_MODE_BT1120_480P) && (1 == u32ViPort))
		{
			VIU_DRV_SetYcChannel(u32ViPort, 0, 1);
		}
		else
		{
			VIU_DRV_SetYcChannel(u32ViPort, 0, 0);
		}
//        VIU_DRV_SetYcChannel(u32ViPort, 0, pstAttr->enChnYC);
        bField = HI_TRUE;
        break;

    case HI_UNF_VI_MODE_BT1120_1080I_50:
        u32CapMode  = 0;
        u32ScanMode = 2;/* separate Y/C, interlace */
		if ((pstAttr->enInputMode >= HI_UNF_VI_MODE_BT1120_480P) && (1 == u32ViPort))
		{
			VIU_DRV_SetYcChannel(u32ViPort, 0, 1);
		}
		else
		{
			VIU_DRV_SetYcChannel(u32ViPort, 0, 0);
		}
//        VIU_DRV_SetYcChannel(u32ViPort, 0, pstAttr->enChnYC);
        bField = HI_TRUE;
        break;

    case HI_UNF_VI_MODE_BT1120_1080I_60:
        u32CapMode  = 0;
        u32ScanMode = 2;/* separate Y/C, interlace */
		if ((pstAttr->enInputMode >= HI_UNF_VI_MODE_BT1120_480P) && (1 == u32ViPort))
		{
			VIU_DRV_SetYcChannel(u32ViPort, 0, 1);
		}
		else
		{
			VIU_DRV_SetYcChannel(u32ViPort, 0, 0);
		}
//        VIU_DRV_SetYcChannel(u32ViPort, 0, pstAttr->enChnYC);
        bField = HI_TRUE;
        break;

    case HI_UNF_VI_MODE_BT1120_1080P_25:
    case HI_UNF_VI_MODE_BT1120_1080P_30:
	case HI_UNF_VI_MODE_BT1120_1080P_50:
	case HI_UNF_VI_MODE_BT1120_1080P_60:
        u32CapMode  = 0;
        u32ScanMode = 3;/* separate Y/C, progressive */
		if ((pstAttr->enInputMode >= HI_UNF_VI_MODE_BT1120_480P) && (1 == u32ViPort))
		{
			VIU_DRV_SetYcChannel(u32ViPort, 0, 1);
		}
		else
		{
			VIU_DRV_SetYcChannel(u32ViPort, 0, 0);
		}
//        VIU_DRV_SetYcChannel(u32ViPort, 0, pstAttr->enChnYC);
        bField = HI_TRUE;
        break;

    case HI_UNF_VI_MODE_BT656_576I:
        u32CapMode  = 0;
        u32ScanMode = 0;
        bField = HI_FALSE;
        break;

    case HI_UNF_VI_MODE_BT656_480I:
        u32CapMode  = 0;
        u32ScanMode = 0;
        bField = HI_FALSE;
        break;
		
	case HI_UNF_VI_MODE_BT601_576I:
		u32CapMode	= 1;
		u32ScanMode = 0;
		bField = HI_FALSE;
		break;
		
	case HI_UNF_VI_MODE_BT601_480I:
		u32CapMode	= 1;
		u32ScanMode = 0;
		bField = HI_FALSE;
		break;
#if 0
    case HI_UNF_VI_MODE_BT601:
        u32CapMode  = 1;
        u32ScanMode = 0;
        bField = HI_FALSE;
        break;

    case HI_UNF_VI_MODE_USB_CAM:
        u32CapMode  = 1;
        u32ScanMode = 0;
        bField = HI_FALSE;
        HI_INFO_VI("is USB_CAM!\n");
        break;
    case HI_UNF_VI_MODE_DIGITAL_CAMERA:
        u32CapMode  = 2;
        u32ScanMode = 1;
        bField = HI_TRUE;
        break;
#endif
    default:
        return HI_ERR_VI_INVALID_PARA;
    }

    if (HI_FALSE == pstAttr->bVirtual)
//	if ((HI_UNF_VI_MODE_USB_CAM != pstAttr->enInputMode)
//        && (HI_UNF_VI_MODE_VIRTUAL != pstAttr->enInputMode))
    {
		if ((pstAttr->enVideoFormat >= HI_UNF_FORMAT_YUV_SEMIPLANAR_422)&&
			(pstAttr->enVideoFormat <= HI_UNF_FORMAT_YUV_SEMIPLANAR_444))
		{
            if (s_astViAttr[u32ViPort].enInputMode >= HI_UNF_VI_MODE_BT1120_480P)
            {
                if (HI_UNF_VI_PORT0 == u32ViPort)
                {
                    YStride = pstAttr->stInputRect.s32Width;
                    CStride = 0;
                }
                else
                {
                    YStride = 0;
                    CStride = pstAttr->stInputRect.s32Width;
                }
            }
            else
            {
                YStride = ((pstAttr->stInputRect.s32Width + 63) / 64) * 64;
                CStride = YStride;
            }
		}
		else if ((pstAttr->enVideoFormat >= HI_UNF_FORMAT_YUV_PACKAGE_UYVY) &&
			(pstAttr->enVideoFormat <= HI_UNF_FORMAT_YUV_PACKAGE_YVYU))
		{
            YStride = ((pstAttr->stInputRect.s32Width + 63) / 64) * 64 * 2;
            CStride = 0;
		}
		else if ((pstAttr->enVideoFormat >= HI_UNF_FORMAT_YUV_PLANAR_400) &&
			(pstAttr->enVideoFormat <= HI_UNF_FORMAT_YUV_PLANAR_410))
		{
            YStride = ((pstAttr->stInputRect.s32Width + 63) / 64) * 64;
            CStride = YStride;

        }
    }
    else
    {
		if ((pstAttr->enVideoFormat >= HI_UNF_FORMAT_YUV_SEMIPLANAR_422)&&
			(pstAttr->enVideoFormat <= HI_UNF_FORMAT_YUV_SEMIPLANAR_444))
		{
            if (s_astViAttr[u32ViPort].enInputMode >= HI_UNF_VI_MODE_BT1120_480P)
            {
                if (HI_UNF_VI_PORT0 == u32ViPort)
                {
                    YStride = pstAttr->stInputRect.s32Width;
                    CStride = 0;
                }
                else
                {
                    YStride = 0;
                    CStride = pstAttr->stInputRect.s32Width;
                }
            }
            else
            {
                YStride = ((pstAttr->stInputRect.s32Width + 127) / 128) * 128;
//                YStride = pstAttr->stInputRect.s32Width;
                CStride = YStride;
            }
		}
		else if ((pstAttr->enVideoFormat >= HI_UNF_FORMAT_YUV_PACKAGE_UYVY) &&
			(pstAttr->enVideoFormat <= HI_UNF_FORMAT_YUV_PACKAGE_YVYU))
		{
            YStride = pstAttr->stInputRect.s32Width * 2;
            CStride = 0;
		}
		else if ((pstAttr->enVideoFormat >= HI_UNF_FORMAT_YUV_PLANAR_400) &&
			(pstAttr->enVideoFormat <= HI_UNF_FORMAT_YUV_PLANAR_410))
		{
            YStride = pstAttr->stInputRect.s32Width;
            CStride = YStride;
        }
    }
#if 0
    if ((HI_UNF_VI_MODE_USB_CAM != pstAttr->enInputMode)
        && (HI_UNF_VI_MODE_VIRTUAL != pstAttr->enInputMode))
    {
        switch (pstAttr->enStoreMethod)
        {
            /* YUV4:2:0 */
        case HI_UNF_VI_STORE_METHOD_PNYUV:
        {
            YStride = ((pstAttr->stInputRect.s32Width + 63) / 64) * 64;
            CStride = YStride;
            break;
        }

            /*YUV4:2:0 or YUV422, due to line_sel */
        case HI_UNF_VI_STORE_METHOD_SPNYC:
        {
            if (s_astViAttr[u32ViPort].enInputMode >= HI_UNF_VI_MODE_BT1120_480P)
            {
                if (HI_UNF_VI_PORT0 == u32ViPort)
                {
                    YStride = pstAttr->stInputRect.s32Width;
                    CStride = 0;
                }
                else
                {
                    YStride = 0;
                    CStride = pstAttr->stInputRect.s32Width;
                }
            }
            else
            {
                YStride = ((pstAttr->stInputRect.s32Width + 63) / 64) * 64;
                CStride = YStride;
            }

            break;
        }

            /*YUV4:2:2*/
        case HI_UNF_VI_STORE_METHOD_PKYUV:
        {
            YStride = ((pstAttr->stInputRect.s32Width + 63) / 64) * 64 * 2;
            CStride = 0;
            break;
        }

        default:
        {
            break;
        }
        }
    }
    else
    {
        switch (pstAttr->enStoreMethod)
        {
        case HI_UNF_VI_STORE_METHOD_PNYUV:
        {
            YStride = pstAttr->stInputRect.s32Width;
            CStride = YStride;
            break;
        }

        case HI_UNF_VI_STORE_METHOD_SPNYC:
        {
            if (s_astViAttr[u32ViPort].enInputMode >= HI_UNF_VI_MODE_BT1120_480P)
            {
                if (HI_UNF_VI_PORT0 == u32ViPort)
                {
                    YStride = pstAttr->stInputRect.s32Width;
                    CStride = 0;
                }
                else
                {
                    YStride = 0;
                    CStride = pstAttr->stInputRect.s32Width;
                }
            }
            else
            {
                YStride = pstAttr->stInputRect.s32Width;
                CStride = YStride;
            }

            break;
        }

        case HI_UNF_VI_STORE_METHOD_PKYUV:
        {
            YStride = pstAttr->stInputRect.s32Width * 2;
            CStride = 0;
            break;
        }

        default:
        {
            break;
        }
        }
    }
#endif
    /*
    if(HI_UNF_VI_INVALID_PARA_U32 != pstAttr->u32YStride)
    {
        YStride = (pstAttr->u32YStride + 0x0f) & (~0xf);
    }

    if(HI_UNF_VI_INVALID_PARA_U32 != pstAttr->u32CStride)
    {
        CStride = (pstAttr->u32CStride + 0x0f) & (~0xf);
    }
     */

    /*one field only need half frame room*/

    /*
    if(HI_UNF_VI_STORE_FIELD == pstAttr->enStoreMode)
    {
        g_u32ViuFrameSize[u32ViPort]=(YStride * pstAttr->stInputRect.s32Height
     + CStride * pstAttr->stInputRect.s32Height);
    }
    else
    {
        g_u32ViuFrameSize[u32ViPort]=(YStride * pstAttr->stInputRect.s32Height
     + CStride * pstAttr->stInputRect.s32Height);
    }
     */

    //modified by y00106256
    g_u32ViuFrameSize[u32ViPort] = (YStride * pstAttr->stInputRect.s32Height
                                    + CStride * pstAttr->stInputRect.s32Height);

    HI_INFO_VI("Port%d: FieldMode:%u, Rect:%d*%d, Y/CStride:%u/%u, size:%u.\n",
               u32ViPort, pstAttr->enFieldMode,
               pstAttr->stInputRect.s32Width, pstAttr->stInputRect.s32Height,
               YStride, CStride, g_u32ViuFrameSize[u32ViPort]);

//    pstAttr->u32FrameSize = g_u32ViuFrameSize[u32ViPort];
//    pstAttr->u32BufSize = pstAttr->u32BufNum * pstAttr->u32FrameSize;
//    pstAttr->u32BufSize = pstAttr->u32BufNum * pstAttr->u32FrameSize * g_u32ViuFrameSize[u32ViPort];

    if (HI_UNF_VI_BUF_ALLOC == pstAttr->enBufMgmtMode)
    {
        if (HI_SUCCESS
            != VIU_FbInit(u32ViPort, &g_struFbRoot[u32ViPort], pstAttr->u32BufNum * g_u32ViuFrameSize[u32ViPort],
                          g_u32ViuFrameSize[u32ViPort]))
        {
            HI_ERR_VI("Buf init failed! [%u / %u]\n", pstAttr->u32BufNum * g_u32ViuFrameSize[u32ViPort],
                      g_u32ViuFrameSize[u32ViPort]);
            return HI_ERR_VI_CHN_INIT_BUF_ERR;
        }

//        pstAttr->u32ViBufAddr = g_struFbRoot[u32ViPort].u32FbPhysAddr;
    }
    else if (HI_UNF_VI_BUF_MMAP == pstAttr->enBufMgmtMode)
    {
    // MOVE to SetExtBuf
    #if 0
        if (pstAttr->u32ViBufAddr)
        {
            if (HI_SUCCESS
                != VIU_FbMMapInit(&g_struFbRoot[u32ViPort], pstAttr->u32BufNum, g_u32ViuFrameSize[u32ViPort],
                                  pstAttr->u32ViBufAddr))
            {
                HI_ERR_VI("Buf mmap failed! Map addr[%x]\n", pstAttr->u32ViBufAddr);
                return HI_ERR_VI_CHN_INIT_BUF_ERR;
            }
        }
        else
        {
            g_struFbRoot[u32ViPort].u32BlkNum = pstAttr->u32BufNum;
        }
	#endif
    }

//    pstAttr->u32YStride = YStride;
//    pstAttr->u32CStride = CStride;

    /*
    if(s_astViAttr[u32ViPort].enInputMode >= HI_UNF_VI_MODE_BT1120_480P
            && s_astViAttr[u32ViPort].enChnYC == HI_UNF_VI_CHN_YC_SEL_C)
    {
        VIU_FbCleanup(&g_struFbRoot[u32ViPort]);
    }*/

    memset(&g_struViuData[u32ViPort], 0x00, sizeof(g_struViuData[u32ViPort]));
    sema_init(&g_struViuData[u32ViPort].stopViuMutex, 0);

    g_struViuData[u32ViPort].enBufMode       = pstAttr->enBufMgmtMode;
    g_struViuData[u32ViPort].u32VchnMask     = 0;
    g_struViuData[u32ViPort].u32TimeRef      = 0;
    g_struViuData[u32ViPort].bRefTimeReset   = HI_TRUE;
    g_struViuData[u32ViPort].u32LastBufIndex = VIU_FB_INVAL_INDEX_LAST;
    g_struViuData[u32ViPort].u32ThisBufIndex = VIU_FB_INVAL_INDEX;
    g_struViuData[u32ViPort].u32NextBufIndex = 0;
    g_struViuData[u32ViPort].bStoping = HI_FALSE;
    g_struViuData[u32ViPort].bIsViuCfged   = HI_FALSE;
    g_struViuData[u32ViPort].u32IsVicCfged = 0;
    g_struViuData[u32ViPort].struViuAttr.bUseVbi = HI_FALSE;
    g_struViuData[u32ViPort].u32StrideY = YStride;
    g_struViuData[u32ViPort].u32StrideC = CStride;
    g_struViuData[u32ViPort].struVchnAttr[0].u32CapWidth  = pstAttr->stInputRect.s32Width;

	if (HI_UNF_VIDEO_FIELD_ALL == pstAttr->enFieldMode)
	{
		g_struViuData[u32ViPort].struVchnAttr[0].u32CapHeight = pstAttr->stInputRect.s32Height;
	}
	else
	{
		g_struViuData[u32ViPort].struVchnAttr[0].u32CapHeight = pstAttr->stInputRect.s32Height/2;
	}
#if 0	
    g_struViuData[u32ViPort].struVchnAttr[0].u32CapHeight =
        (pstAttr->enStoreMode ? pstAttr->stInputRect.s32Height : (pstAttr->stInputRect.s32Height));                                                     /* = pstAttr->enStoreMode + 1. field:*1/2, frame:*1 */
#endif
    g_struFbRoot[u32ViPort].struFb[g_struViuData[u32ViPort].u32NextBufIndex].bLost = HI_TRUE;

    for (k = 0; k < g_struFbRoot[u32ViPort].u32BlkNum; k++)
    {
        g_struViuData[u32ViPort].struBufAddr[k].u32BufPhysAddr_Y = VIU_FB_I2P(&g_struFbRoot[u32ViPort], k);
        g_struViuData[u32ViPort].struBufAddr[k].pBufVirtAddr_Y   = (HI_VOID*)VIU_FB_I2V(&g_struFbRoot[u32ViPort], k);

		if (HI_UNF_VIDEO_FIELD_ALL == pstAttr->enFieldMode)
		{
	        g_struViuData[u32ViPort].struBufAddr[k].u32BufPhysAddr_C =
	            g_struViuData[u32ViPort].struBufAddr[k].u32BufPhysAddr_Y + g_struViuData[u32ViPort].u32StrideY * pstAttr->stInputRect.s32Height;
	        g_struViuData[u32ViPort].struBufAddr[k].pBufVirtAddr_C = 
				g_struViuData[u32ViPort].struBufAddr[k].pBufVirtAddr_Y + g_struViuData[u32ViPort].u32StrideY * pstAttr->stInputRect.s32Height;
		}
		else
		{
	        g_struViuData[u32ViPort].struBufAddr[k].u32BufPhysAddr_C =
	            g_struViuData[u32ViPort].struBufAddr[k].u32BufPhysAddr_Y + g_struViuData[u32ViPort].u32StrideY * pstAttr->stInputRect.s32Height/2;
	        g_struViuData[u32ViPort].struBufAddr[k].pBufVirtAddr_C = 
				g_struViuData[u32ViPort].struBufAddr[k].pBufVirtAddr_Y + g_struViuData[u32ViPort].u32StrideY * pstAttr->stInputRect.s32Height/2;
		}
#if 0		
        g_struViuData[u32ViPort].struBufAddr[k].u32BufPhysAddr_C =
            g_struViuData[u32ViPort].struBufAddr[k].u32BufPhysAddr_Y
            + g_struViuData[u32ViPort].u32StrideY
            * (pstAttr->enStoreMode ? pstAttr->stInputRect.s32Height : (pstAttr->stInputRect.s32Height));
        g_struViuData[u32ViPort].struBufAddr[k].pBufVirtAddr_C = g_struViuData[u32ViPort].struBufAddr[k].pBufVirtAddr_Y
                                                                 + g_struViuData[u32ViPort].u32StrideY
                                                                 * (pstAttr->enStoreMode ? pstAttr->stInputRect.s32Height : (pstAttr->stInputRect.s32Height));
#endif
        if ((s_astViAttr[u32ViPort].enInputMode >= HI_UNF_VI_MODE_BT1120_480P) && (0 == u32ViPort))
//        if (((s_astViAttr[u32ViPort].enInputMode >= HI_UNF_VI_MODE_BT1120_480P)
//             && (s_astViAttr[u32ViPort].enChnYC == HI_UNF_VI_CHN_YC_SEL_Y)))
        {
            g_struViuData[u32ViPort].struBufAddr[k].u32BufPhysAddr_Y = VIU_FB_I2P(&g_struFbRoot[u32ViPort], k);
            g_struViuData[u32ViPort].struBufAddr[k].pBufVirtAddr_Y = (HI_VOID*)VIU_FB_I2V(&g_struFbRoot[u32ViPort], k);

            g_struViuData[u32ViPort].struBufAddr[k].u32BufPhysAddr_C = 0;
            g_struViuData[u32ViPort].struBufAddr[k].pBufVirtAddr_C = 0;
        }
		else if ((s_astViAttr[u32ViPort].enInputMode >= HI_UNF_VI_MODE_BT1120_480P) && (1 == u32ViPort))
//        else if (((s_astViAttr[u32ViPort].enInputMode >= HI_UNF_VI_MODE_BT1120_480P)
//                  && (s_astViAttr[u32ViPort].enChnYC == HI_UNF_VI_CHN_YC_SEL_C)))
        {
            g_struViuData[u32ViPort].struBufAddr[k].u32BufPhysAddr_C = VIU_FB_I2P(&g_struFbRoot[u32ViPort], k);
            g_struViuData[u32ViPort].struBufAddr[k].pBufVirtAddr_C = (HI_VOID*)VIU_FB_I2V(&g_struFbRoot[u32ViPort], k);

            g_struViuData[u32ViPort].struBufAddr[k].u32BufPhysAddr_Y = 0;
            g_struViuData[u32ViPort].struBufAddr[k].pBufVirtAddr_Y = 0;
        }

        //modified by y00106256
        //g_struViuData[u32ViPort].struBufAddr[k].u32BufPhysAddr_C = g_struViuData[u32ViPort].struBufAddr[k].u32BufPhysAddr_Y
        //      		+ g_struViuData[u32ViPort].u32StrideY * pstAttr->stInputRect.s32Height;
    }

    if ((HI_CHIP_TYPE_HI3716C == enChipType) || (HI_CHIP_TYPE_HI3716H == enChipType))
    {
        VIU_DRV_SetFixCode(u32ViPort, 0, 0);
        VIU_DRV_SetSeavFNeg(u32ViPort, 0, 0);

        /* bField hd, separate Y/C */
        if (bField)
        {
            /* even field, setting only effective to chroma */
            VIU_DRV_SetEvenLineSel(u32ViPort, 0, VI_LINESEL_BOTH);
            VIU_DRV_SetOddLineSel(u32ViPort, 0, VI_LINESEL_BOTH);
        }
        else
        {
            if (HI_UNF_VI_CAPSEL_BOTH == pstAttr->enCapSel)
            {
                VIU_DRV_SetEvenLineSel(u32ViPort, 0, VI_LINESEL_ODD);
                VIU_DRV_SetOddLineSel(u32ViPort, 0, VI_LINESEL_EVEN);
            }
            else
            {
                if (HI_UNF_VI_CAPSEL_ODD == pstAttr->enCapSel)
                {
                    VIU_DRV_SetOddLineSel(u32ViPort, 0, VI_LINESEL_EVEN);
                }
                else
                {
                    VIU_DRV_SetEvenLineSel(u32ViPort, 0, VI_LINESEL_ODD);
                }
            }
        }

        if (s_astViAttr[u32ViPort].enInputMode >= HI_UNF_VI_MODE_BT1120_480P)
        {
            /* set raw data for BT1120 mode */
            VIU_DRV_SetStoreMethodRawData(u32ViPort, 0);
			VIU_DRV_SetCapWindowRawData(u32ViPort, 0, pstAttr->stInputRect.s32X, pstAttr->stInputRect.s32Y,
								 pstAttr->stInputRect.s32Width, pstAttr->stInputRect.s32Height,
								 0,  0, 0);
        }
        else
        {
//            VIU_DRV_SetStoreMethod(u32ViPort, 0, pstAttr->enStoreMethod);
			VIU_DRV_SetStoreMethod(u32ViPort, 0, pstAttr->enVideoFormat);
            VIU_DRV_SetCapWindow(u32ViPort, 0, pstAttr->stInputRect.s32X, pstAttr->stInputRect.s32Y,
                             pstAttr->stInputRect.s32Width, pstAttr->stInputRect.s32Height,
                             0, pstAttr->enVideoFormat, 0, 0);
        }

        VIU_DRV_SetCapSel(u32ViPort, 0, pstAttr->enCapSel);

        //attention:if input is progressive, store mode must be field
//        VIU_DRV_SetStoreMode(u32ViPort, 0, pstAttr->enStoreMode);
        VIU_DRV_SetStoreMode(u32ViPort, 0, pstAttr->enFieldMode);

        /*VIU_DRV_SetDataWidth(u32ViPort,0, pstAttr->enDataWidth);*/
        /*VIU_DRV_SetCapWindow(u32ViPort, 0, pstAttr->stInputRect.s32X, pstAttr->stInputRect.s32Y,
                             pstAttr->stInputRect.s32Width, pstAttr->stInputRect.s32Height,
                             0, pstAttr->enStoreMethod, 0, 0);*/
        VIU_DRV_SetMemAddr(u32ViPort, 0,
                           g_struViuData[u32ViPort].struBufAddr[g_struViuData[u32ViPort].u32NextBufIndex].u32BufPhysAddr_Y,
                           g_struViuData[u32ViPort].struBufAddr[g_struViuData[u32ViPort].u32NextBufIndex].u32BufPhysAddr_C,
                           0);

        //If other kinds of cameras, it will change here, maybe
        VIU_DRV_SetPortAttr(u32ViPort, u32ScanMode, u32CapMode, 0, 0, 0, 0, 0, 1);/*liusanwei move from  line 2615 to here*/

        if ((s_astViAttr[u32ViPort].enInputMode >= HI_UNF_VI_MODE_BT1120_480P) && (0 == u32ViPort))
//        if ((s_astViAttr[u32ViPort].enInputMode >= HI_UNF_VI_MODE_BT1120_480P)
//            && (s_astViAttr[u32ViPort].enChnYC == HI_UNF_VI_CHN_YC_SEL_Y))
        {
            VIU_DRV_SetMemAddr(u32ViPort, 0,
                               g_struViuData[u32ViPort].struBufAddr[g_struViuData[u32ViPort].u32NextBufIndex].u32BufPhysAddr_Y,
                               0,
                               0);
        }
		else if ((s_astViAttr[u32ViPort].enInputMode >= HI_UNF_VI_MODE_BT1120_480P) && (1 == u32ViPort))
//        else if ((s_astViAttr[u32ViPort].enInputMode >= HI_UNF_VI_MODE_BT1120_480P)
//                 && (s_astViAttr[u32ViPort].enChnYC == HI_UNF_VI_CHN_YC_SEL_C))
        {
            VIU_DRV_SetMemAddr(u32ViPort, 0,
                               g_struViuData[u32ViPort].struBufAddr[g_struViuData[u32ViPort].u32NextBufIndex].u32BufPhysAddr_C,
                               0,
                               0);
        }

        if (s_astViAttr[u32ViPort].enInputMode >= HI_UNF_VI_MODE_BT1120_480P)
        {
            if (0 == u32ViPort)        
//            if (s_astViAttr[u32ViPort].enChnYC == HI_UNF_VI_CHN_YC_SEL_Y)
            {
                VIU_DRV_SetMemStride(u32ViPort, 0, YStride, 0, 0);
            }
            if (1 == u32ViPort)        
//            if (s_astViAttr[u32ViPort].enChnYC == HI_UNF_VI_CHN_YC_SEL_C)
            {
                VIU_DRV_SetMemStride(u32ViPort, 0, CStride, 0, 0);
            }
        }
        else
        {
            VIU_DRV_SetMemStride(u32ViPort, 0, YStride, CStride, 0);
        }

        VIU_DRV_SetPrioCtrl(4, 0);
        VIU_DRV_SetDownScaling(u32ViPort, 0, 0);
        VIU_DRV_SetCorrectEn(u32ViPort, 0, 0);
        VIU_DRV_SetChromaResample(u32ViPort, 0, 0);
    }

    g_stViOptmS[u32ViPort].u32x = pstAttr->stInputRect.s32X;
    g_stViOptmS[u32ViPort].u32y = pstAttr->stInputRect.s32Y;
    g_stViOptmS[u32ViPort].u32Width   = pstAttr->stInputRect.s32Width;
    g_stViOptmS[u32ViPort].u32Height  = pstAttr->stInputRect.s32Height;
    g_stViOptmS[u32ViPort].u32StrideY = YStride;
    g_stViOptmS[u32ViPort].u32StrideC = CStride;

    // save configuration
    g_stViCurCfg[u32ViPort].bValid  = HI_TRUE;
	g_stViCurCfg[u32ViPort].enViPort = (HI_UNF_VI_E)u32ViPort;
    g_stViCurCfg[u32ViPort].stViCfg = *pstAttr;

    HI_INFO_VI(" leave Viu_SetAttr!\n");

    return HI_SUCCESS;
}

HI_S32 Viu_ResumeAttr(HI_U32 u32ViPort, const HI_UNF_VI_ATTR_S *pstAttr)
{
    HI_U32 YStride = 2048;
    HI_U32 CStride = 2048;
    HI_U32 u32ScanMode = 0;
    HI_U32 u32CapMode = 0;
    HI_U32 u32RegValue;
    HI_BOOL bField = HI_FALSE;
    HI_CHIP_TYPE_E enChipType;

    if (HI_NULL == pstAttr)
    {
        return HI_ERR_VI_NULL_PTR;
    }

    if ((HI_UNF_VI_PORT0 > u32ViPort) || (u32ViPort > HI_UNF_VI_PORT1))
    {
        HI_ERR_VI("%s, %d\n", __FUNCTION__, __LINE__);
        return HI_ERR_VI_INVALID_PARA;
    }

    g_struViuData[u32ViPort].u32LastBufIndex = VIU_FB_INVAL_INDEX_LAST;
    g_struViuData[u32ViPort].u32ThisBufIndex = VIU_FB_INVAL_INDEX;
    g_struViuData[u32ViPort].u32NextBufIndex = 0;

    HI_DRV_SYS_GetChipVersion(&enChipType, HI_NULL);

    /* VI1 selects clock from vi0_clk_in */
    if ((HI_CHIP_TYPE_HI3716C == enChipType) && (pstAttr->enInputMode >= HI_UNF_VI_MODE_BT1120_480P))
    {
        HI_REG_READ32(IO_ADDRESS(VIU_CRG_CONTROL + 4), u32RegValue);
        u32RegValue |= 0x10000;
        HI_REG_WRITE32(IO_ADDRESS(VIU_CRG_CONTROL + 4), u32RegValue);
    }

    switch (pstAttr->enInputMode)
    {
    case HI_UNF_VI_MODE_BT1120_480P:
        u32CapMode  = 0;
        u32ScanMode = 3; /* separate Y/C, progressive */
		if ((pstAttr->enInputMode >= HI_UNF_VI_MODE_BT1120_480P) && (1 == u32ViPort))
		{
			VIU_DRV_SetYcChannel(u32ViPort, 0, 1);
		}
		else
		{
			VIU_DRV_SetYcChannel(u32ViPort, 0, 0);
		}
//        VIU_DRV_SetYcChannel(u32ViPort, 0, pstAttr->enChnYC);
        bField = HI_TRUE;
        break;

    case HI_UNF_VI_MODE_BT1120_576P:
        u32CapMode  = 0;
        u32ScanMode = 3; /* separate Y/C, progressive */
		if ((pstAttr->enInputMode >= HI_UNF_VI_MODE_BT1120_480P) && (1 == u32ViPort))
		{
			VIU_DRV_SetYcChannel(u32ViPort, 0, 1);
		}
		else
		{
			VIU_DRV_SetYcChannel(u32ViPort, 0, 0);
		}
//        VIU_DRV_SetYcChannel(u32ViPort, 0, pstAttr->enChnYC);
        bField = HI_TRUE;
        break;

    case HI_UNF_VI_MODE_BT1120_720P_50:
        u32CapMode  = 0;
        u32ScanMode = 3; /* separate Y/C, progressive */
		if ((pstAttr->enInputMode >= HI_UNF_VI_MODE_BT1120_480P) && (1 == u32ViPort))
		{
			VIU_DRV_SetYcChannel(u32ViPort, 0, 1);
		}
		else
		{
			VIU_DRV_SetYcChannel(u32ViPort, 0, 0);
		}
//        VIU_DRV_SetYcChannel(u32ViPort, 0, pstAttr->enChnYC);
        bField = HI_TRUE;
        break;

    case HI_UNF_VI_MODE_BT1120_720P_60:
        u32CapMode  = 0;
        u32ScanMode = 3; /* separate Y/C, progressive */
		if ((pstAttr->enInputMode >= HI_UNF_VI_MODE_BT1120_480P) && (1 == u32ViPort))
		{
			VIU_DRV_SetYcChannel(u32ViPort, 0, 1);
		}
		else
		{
			VIU_DRV_SetYcChannel(u32ViPort, 0, 0);
		}
//        VIU_DRV_SetYcChannel(u32ViPort, 0, pstAttr->enChnYC);
        bField = HI_TRUE;
        break;

    case HI_UNF_VI_MODE_BT1120_1080I_50:
        u32CapMode  = 0;
        u32ScanMode = 2;/* separate Y/C, interlace */
		if ((pstAttr->enInputMode >= HI_UNF_VI_MODE_BT1120_480P) && (1 == u32ViPort))
		{
			VIU_DRV_SetYcChannel(u32ViPort, 0, 1);
		}
		else
		{
			VIU_DRV_SetYcChannel(u32ViPort, 0, 0);
		}
//        VIU_DRV_SetYcChannel(u32ViPort, 0, pstAttr->enChnYC);
        bField = HI_TRUE;
        break;

    case HI_UNF_VI_MODE_BT1120_1080I_60:
        u32CapMode  = 0;
        u32ScanMode = 2;/* separate Y/C, interlace */
		if ((pstAttr->enInputMode >= HI_UNF_VI_MODE_BT1120_480P) && (1 == u32ViPort))
		{
			VIU_DRV_SetYcChannel(u32ViPort, 0, 1);
		}
		else
		{
			VIU_DRV_SetYcChannel(u32ViPort, 0, 0);
		}
//        VIU_DRV_SetYcChannel(u32ViPort, 0, pstAttr->enChnYC);
        bField = HI_TRUE;
        break;

    case HI_UNF_VI_MODE_BT1120_1080P_25:
    case HI_UNF_VI_MODE_BT1120_1080P_30:
	case HI_UNF_VI_MODE_BT1120_1080P_50:
	case HI_UNF_VI_MODE_BT1120_1080P_60:
        u32CapMode  = 0;
        u32ScanMode = 3;/* separate Y/C, progressive */
		if ((pstAttr->enInputMode >= HI_UNF_VI_MODE_BT1120_480P) && (1 == u32ViPort))
		{
			VIU_DRV_SetYcChannel(u32ViPort, 0, 1);
		}
		else
		{
			VIU_DRV_SetYcChannel(u32ViPort, 0, 0);
		}
//        VIU_DRV_SetYcChannel(u32ViPort, 0, pstAttr->enChnYC);
        bField = HI_TRUE;
        break;

    case HI_UNF_VI_MODE_BT656_576I:
        u32CapMode  = 0;
        u32ScanMode = 0;
        bField = HI_FALSE;
        break;

    case HI_UNF_VI_MODE_BT656_480I:
        u32CapMode  = 0;
        u32ScanMode = 0;
        bField = HI_FALSE;
        break;
		
	case HI_UNF_VI_MODE_BT601_576I:
		u32CapMode	= 1;
		u32ScanMode = 0;
		bField = HI_FALSE;
		break;
		
	case HI_UNF_VI_MODE_BT601_480I:
		u32CapMode	= 1;
		u32ScanMode = 0;
		bField = HI_FALSE;
		break;
#if 0
    case HI_UNF_VI_MODE_BT601:
        u32CapMode  = 1;
        u32ScanMode = 0;
        bField = HI_FALSE;
        break;
    case HI_UNF_VI_MODE_USB_CAM:
        u32CapMode  = 1;
        u32ScanMode = 0;
        bField = HI_FALSE;
        HI_INFO_VI("is USB_CAM!\n");
        break;
    case HI_UNF_VI_MODE_DIGITAL_CAMERA:
        u32CapMode  = 2;
        u32ScanMode = 1;
        bField = HI_TRUE;
        break;
#endif
    default:
        return HI_ERR_VI_INVALID_PARA;
    }

//    if (HI_UNF_VI_INVALID_PARA_U32 != pstAttr->u32YStride)
    {
//        YStride = pstAttr->u32YStride;
	    YStride = g_struViuData[u32ViPort].u32StrideY;
    }

//    if (HI_UNF_VI_INVALID_PARA_U32 != pstAttr->u32CStride)
    {
//        CStride = pstAttr->u32CStride;
	    CStride = g_struViuData[u32ViPort].u32StrideC;
    }

    if ((HI_CHIP_TYPE_HI3716C == enChipType) || (HI_CHIP_TYPE_HI3716H == enChipType))
    {
        VIU_DRV_SetFixCode(u32ViPort, 0, 0);
        VIU_DRV_SetSeavFNeg(u32ViPort, 0, 0);
        if (bField)
        {
            VIU_DRV_SetEvenLineSel(u32ViPort, 0, VI_LINESEL_BOTH);
            VIU_DRV_SetOddLineSel(u32ViPort, 0, VI_LINESEL_BOTH);
        }
        else
        {
            if (HI_UNF_VI_CAPSEL_BOTH == pstAttr->enCapSel)
            {
                VIU_DRV_SetEvenLineSel(u32ViPort, 0, VI_LINESEL_ODD);
                VIU_DRV_SetOddLineSel(u32ViPort, 0, VI_LINESEL_EVEN);
            }
            else
            {
                if (HI_UNF_VI_CAPSEL_ODD == pstAttr->enCapSel)
                {
                    VIU_DRV_SetOddLineSel(u32ViPort, 0, VI_LINESEL_EVEN);
                }
                else
                {
                    VIU_DRV_SetEvenLineSel(u32ViPort, 0, VI_LINESEL_ODD);
                }
            }
        }
		if (s_astViAttr[u32ViPort].enInputMode >= HI_UNF_VI_MODE_BT1120_480P)
		{
			/* set raw data for BT1120 mode */
			VIU_DRV_SetStoreMethodRawData(u32ViPort, 0);
			VIU_DRV_SetCapWindowRawData(u32ViPort, 0, pstAttr->stInputRect.s32X, pstAttr->stInputRect.s32Y,
								 pstAttr->stInputRect.s32Width, pstAttr->stInputRect.s32Height,
								 0,  0, 0);
		}
		else
		{
//			  VIU_DRV_SetStoreMethod(u32ViPort, 0, pstAttr->enStoreMethod);
			VIU_DRV_SetStoreMethod(u32ViPort, 0, pstAttr->enVideoFormat);
			VIU_DRV_SetCapWindow(u32ViPort, 0, pstAttr->stInputRect.s32X, pstAttr->stInputRect.s32Y,
							 pstAttr->stInputRect.s32Width, pstAttr->stInputRect.s32Height,
							 0, pstAttr->enVideoFormat, 0, 0);
		}

//        VIU_DRV_SetStoreMethod(u32ViPort, 0, pstAttr->enStoreMethod);

        VIU_DRV_SetCapSel(u32ViPort, 0, pstAttr->enCapSel);
        VIU_DRV_SetStoreMode(u32ViPort, 0, pstAttr->enFieldMode);
//        VIU_DRV_SetStoreMode(u32ViPort, 0, pstAttr->enStoreMode);

        /*VIU_DRV_SetDataWidth(u32ViPort,0, pstAttr->enDataWidth);*/
/*        VIU_DRV_SetCapWindow(u32ViPort, 0, pstAttr->stInputRect.s32X, pstAttr->stInputRect.s32Y,
                             pstAttr->stInputRect.s32Width, pstAttr->stInputRect.s32Height,
                             0, pstAttr->enStoreMethod, 0, 0);
*/
        /*VIU_DRV_SetCapWindow(u32ViPort, 0, pstAttr->stInputRect.s32X, pstAttr->stInputRect.s32Y,
                             pstAttr->stInputRect.s32Width, pstAttr->stInputRect.s32Height,
                             0, pstAttr->enVideoFormat, 0, 0);*/

        VIU_DRV_SetMemAddr(u32ViPort, 0,
                           g_struViuData[u32ViPort].struBufAddr[g_struViuData[u32ViPort].u32NextBufIndex].u32BufPhysAddr_Y,
                           g_struViuData[u32ViPort].struBufAddr[g_struViuData[u32ViPort].u32NextBufIndex].u32BufPhysAddr_C,
                           0);
        VIU_DRV_SetPortAttr(u32ViPort, u32ScanMode, u32CapMode, 0, 0, 0, 0, 0, 1);/*liusanwei move from  line 2615 to here*/

        if ((s_astViAttr[u32ViPort].enInputMode >= HI_UNF_VI_MODE_BT1120_480P) && (0 == u32ViPort))
//        if ((s_astViAttr[u32ViPort].enInputMode >= HI_UNF_VI_MODE_BT1120_480P)
//            && (s_astViAttr[u32ViPort].enChnYC == HI_UNF_VI_CHN_YC_SEL_Y))
        {
            VIU_DRV_SetMemAddr(u32ViPort, 0,
                               g_struViuData[u32ViPort].struBufAddr[g_struViuData[u32ViPort].u32NextBufIndex].u32BufPhysAddr_Y,
                               0,
                               0);
        }
		else if ((s_astViAttr[u32ViPort].enInputMode >= HI_UNF_VI_MODE_BT1120_480P) && (1 == u32ViPort))
//        else if ((s_astViAttr[u32ViPort].enInputMode >= HI_UNF_VI_MODE_BT1120_480P)
//                 && (s_astViAttr[u32ViPort].enChnYC == HI_UNF_VI_CHN_YC_SEL_C))
        {
            VIU_DRV_SetMemAddr(u32ViPort, 0,
                               g_struViuData[u32ViPort].struBufAddr[g_struViuData[u32ViPort].u32NextBufIndex].u32BufPhysAddr_C,
                               0,
                               0);
        }

        if (s_astViAttr[u32ViPort].enInputMode >= HI_UNF_VI_MODE_BT1120_480P)
        {
            if (0 == u32ViPort)
//            if (s_astViAttr[u32ViPort].enChnYC == HI_UNF_VI_CHN_YC_SEL_Y)
            {
                VIU_DRV_SetMemStride(u32ViPort, 0, YStride, 0, 0);
            }
			if (1 == u32ViPort)
//            if (s_astViAttr[u32ViPort].enChnYC == HI_UNF_VI_CHN_YC_SEL_C)
            {
                VIU_DRV_SetMemStride(u32ViPort, 0, CStride, 0, 0);
            }
        }
        else
        {
            VIU_DRV_SetMemStride(u32ViPort, 0, YStride, CStride, 0);
        }

        VIU_DRV_SetPrioCtrl(4, 0);
        VIU_DRV_SetDownScaling(u32ViPort, 0, 0);
        VIU_DRV_SetCorrectEn(u32ViPort, 0, 0);
        VIU_DRV_SetChromaResample(u32ViPort, 0, 0);
    }

    return HI_SUCCESS;
}

/*  */
HI_S32 Viu_EnableLumstrh(HI_S32 s32ViPort, HI_S32 s32ViChn)
{
    return HI_SUCCESS;
}

HI_S32 Viu_DisableLumstrh(HI_S32 s32ViPort, HI_S32 s32ViChn)
{
    return HI_SUCCESS;
}

HI_S32 Viu_EnableUserPic(HI_S32 s32ViPort, HI_S32 s32ViChn)
{
    return HI_SUCCESS;
}

HI_S32 Viu_DisableUserPic(HI_S32 s32ViPort, HI_S32 s32ViChn)
{
    return HI_SUCCESS;
}
#if 0
HI_S32 Viu_SetUserPic(HI_S32 s32ViPort, HI_S32 s32ViChn, VIDEO_FRAME_INFO_S *pstVFrame)
{
    return HI_SUCCESS;
}
#endif
HI_S32 Viu_SetExtBuf(HI_UNF_VI_E enViPort, HI_UNF_VI_BUFFER_ATTR_S *pstBufAttr)
{
	HI_U32 i;
	HI_U32 u32YStride, u32CStride;
	
	if (HI_NULL == pstBufAttr)
	{
		return HI_FAILURE;
	}
	
	/* bt656/bt1120 mode */
    if (HI_FALSE == g_stViCurCfg[enViPort].stViCfg.bVirtual)
    {
    	if (pstBufAttr->u32Stride % 64 == 0)
    	{
    		u32YStride = pstBufAttr->u32Stride;
			if ((s_astViAttr[enViPort].enVideoFormat >= HI_UNF_FORMAT_YUV_PACKAGE_UYVY) &&
				(s_astViAttr[enViPort].enVideoFormat <= HI_UNF_FORMAT_YUV_PACKAGE_YVYU))
			{
				u32CStride = 0;
			}
			else
			{
				u32CStride = u32YStride;
			}

			if (s_astViAttr[enViPort].enInputMode >= HI_UNF_VI_MODE_BT1120_480P)
			{
				if (HI_UNF_VI_PORT0 == enViPort) 	   
				{
					VIU_DRV_SetMemStride(enViPort, 0, u32YStride, 0, 0);
				}
				if (HI_UNF_VI_PORT1 == enViPort) 	   
				{
					VIU_DRV_SetMemStride(enViPort, 0, u32CStride, 0, 0);
				}
			}
			else
			{
				VIU_DRV_SetMemStride(enViPort, 0, u32YStride, u32CStride, 0);
			}
    	}
		else
		{
			return HI_ERR_VI_NOT_SUPPORT;
		}
    }

	for (i = 0; i < pstBufAttr->u32BufNum; i++)
	{
		if (HI_SUCCESS != VIU_FbMMapInit(&g_struFbRoot[enViPort], i, g_u32ViuFrameSize[enViPort], pstBufAttr->u32UsrVirAddr[i]))
		{
			HI_ERR_VI("Buf mmap failed! Map addr[%x]\n", pstBufAttr->u32UsrVirAddr[i]);
			return HI_ERR_VI_CHN_INIT_BUF_ERR;
		}
	}
	
	return HI_SUCCESS;
}
#if 0
HI_S32  Viu_SetBufAddr(HI_UNF_VI_E enViPort, HI_U32 u32VirAddr, HI_U32 u32BufIndex)
{
    if (u32BufIndex < 0)
    {
        HI_ERR_VI("Viu_SetBufAddr failed, u32BufIndex < 0\n");
        return HI_ERR_VI_INVALID_PARA;
    }

    if (u32BufIndex > VIU_FB_MAX_NUM - 1)
    {
        HI_WARN_VI("Viu_SetBufAddr, VIU buffer over size!\n");
    }

    /* update buffer number */
    g_struFbRoot[enViPort].u32BlkNum = u32BufIndex + 1;
    if (HI_SUCCESS != VIU_FbMMapInit(&g_struFbRoot[enViPort], u32BufIndex, g_u32ViuFrameSize[enViPort], u32VirAddr))
    {
        HI_ERR_VI("Buf mmap failed! Map addr[%x]\n", u32VirAddr);
        return HI_ERR_VI_CHN_INIT_BUF_ERR;
    }

    return HI_SUCCESS;
}
#endif
HI_U32 g_StartCap[VIU_PORT_MAX];
extern wait_queue_head_t gqueue;
extern volatile HI_U32 gwait;

//extern HI_U32 g_VpStaticCtrl;
//extern HI_S32 g_VpStatic[];
//extern HI_S32 g_Pts;

//**********************************************************
//** viu interrupt processing program
//**********************************************************
irqreturn_t Viu_InterruptHandler(int irq, void *dev_id)
{
    HI_U32 i = 0;
    HI_U32 j = 0;
    HI_U32 u32IntIndicator = 0;
    HI_U32 u32IntStatus = 0;
    HI_S32 u32ViPort = 0;
    HI_U32 u32BufAddr;

    u32IntIndicator = VIU_DRV_GetIntIndicator();
    for (i = 0; i < VIU_PORT_MAX; i++)
    {
        for (j = 0; j < VIU_CHANNEL_MAX; j++)
        {
            //                printk(KERN_ERR "VIU int raised, port:%d, chn:%d,int_stat:%d\r\n",i,j,int_status);
            if (1 & (u32IntIndicator >> (((i * VIU_CHANNEL_MAX) | j))))
            {
                u32ViPort = VIU_GET_HANDLE(i, j);

                u32IntStatus = VIU_DRV_GetIntStatus(i, j);

                // cc_int--------------------------------------------
                if ((u32IntStatus & 0x1) == 0x1)
                {
                    VIU_DRV_ClrInterruptStatus(i, j, 0x1);
                    Cc_int[ u32ViPort] += 1;

                    //                    printk(KERN_ERR "Cc_int[Port:%d,Chn:%d]=%d\r\n",i,j,Cc_int[ i*VIU_CHANNEL_MAX + j] );
                }

                // Buf_ovf_int----------------------------------------
                if ((u32IntStatus & 0x2) == 0x2)
                {
                    VIU_DRV_ClrInterruptStatus(i, j, 0x2);
                    Buf_ovf_int[ u32ViPort] += 1;
                }

                // Field_throw_int-------------------------------------
                if ((u32IntStatus & 0x4) == 0x4)
                {
                    VIU_DRV_ClrInterruptStatus(i, j, 0x4);
                    Field_throw_int[u32ViPort] += 1;
                }

                // AHB_error_int--------------------------------------
                if ((u32IntStatus & 0x8) == 0x8)
                {
                    VIU_DRV_ClrInterruptStatus(i, j, 0x8);
                    AHB_error_int[ u32ViPort] += 1;
                }

                // Proc_err_int---------------------------------------
                if ((u32IntStatus & 0x10) == 0x10)
                {
                    VIU_DRV_ClrInterruptStatus(i, j, 0x10);
                    Proc_err_int[ u32ViPort] += 1;
                }

                // Reg_update_int-------------------------------------
                if (((u32IntStatus & 0x20) == 0x20) && !g_StartCap[i])
                {
                    //Discard the image captured by the first reg_new_int
                    VIU_DRV_ClrInterruptStatus(i, j, 0x20);
                    Reg_update_int[u32ViPort] += 1;
                    g_StartCap[i] = 1;

                    /* capture next frame */
                    VIU_DRV_Start(i, j);

                    g_stViOptmS[i].u32RevFrmFrIntf++;
                }
                else if ((u32IntStatus & 0x20) == 0x20)
                {
                    VIU_DRV_ClrInterruptStatus(i, j, 0x20);
                    Reg_update_int[u32ViPort] += 1;

                    g_abNewInt[u32ViPort][0] = HI_TRUE;
                    g_abNewInt[u32ViPort][1] = HI_TRUE;
                    g_abNewInt[u32ViPort][2] = HI_TRUE;
                    g_abNewInt[u32ViPort][3] = HI_TRUE;

                    /* capture next frame's block */
                    u32BufAddr = HI_NULL;
                    u32BufAddr = VIU_FbGet(g_u32KernUserId[u32ViPort], &g_struFbRoot[u32ViPort]);
                    if (!u32BufAddr)
                    {
                        //No free block,Keep the last block for VIU
                        HI_WARN_VI("viu buf full, thisIndex=%d, NextIndex=%d\n",
                                   g_struViuData[u32ViPort].u32ThisBufIndex, g_struViuData[u32ViPort].u32NextBufIndex);
                        g_u32IntLost[u32ViPort] += 1;
                    }
                    else
                    {
                        /* give back the block used */
                        if ((VIU_FB_INVAL_INDEX != g_struViuData[u32ViPort].u32LastBufIndex)
                            && (VIU_FB_INVAL_INDEX_LAST != g_struViuData[u32ViPort].u32LastBufIndex))
                        {
                            if (HI_TRUE
                                == g_struFbRoot[u32ViPort].struFb[g_struViuData[u32ViPort].u32LastBufIndex].bLost)
                            {
                                g_u32IntLost[u32ViPort] += 1;
                            }

                            VIU_FbSub( g_u32KernUserId[u32ViPort], &g_struFbRoot[u32ViPort],
                                       g_struViuData[u32ViPort].u32LastBufIndex);
                        }

                        g_struViuData[u32ViPort].u32LastBufIndex = g_struViuData[u32ViPort].u32ThisBufIndex;
                        g_struViuData[u32ViPort].u32ThisBufIndex = g_struViuData[u32ViPort].u32NextBufIndex;
                        g_struViuData[u32ViPort].u32NextBufIndex = VIU_FB_P2I(&g_struFbRoot[u32ViPort], u32BufAddr);

                        /*
                                                if (HI_NULL != gEncodeFunc)
                                                {
                                                    gEncodeFunc( );
                                                }
                         */
                        if (s_pVencFunc && s_pVencFunc->pfnVencEncodeFrame)
                        {
                            (s_pVencFunc->pfnVencEncodeFrame)( );
                        }

                        //        			printk(KERN_ERR "isr:lastIndex=%d, thisIndex=%d, NextIndex=%d\n",g_struViuData[u32ViPort].u32LastBufIndex,g_struViuData[u32ViPort].u32ThisBufIndex,g_struViuData[u32ViPort].u32NextBufIndex);
                    }

                    /* Update Y/C storage reg */
                    if (s_astViAttr[u32ViPort].enInputMode >= HI_UNF_VI_MODE_BT1120_480P)
                    {
                        VIU_DRV_SetMemAddr(HI_UNF_VI_PORT0, 0,
                                           g_struViuData[0].struBufAddr[g_struViuData[0].u32NextBufIndex].u32BufPhysAddr_Y,
                                           0, 0);

                        VIU_DRV_SetMemAddr(HI_UNF_VI_PORT1, 0,
                                           g_struViuData[1].struBufAddr[g_struViuData[1].u32NextBufIndex].u32BufPhysAddr_C,
                                           0, 0);
                    }
                    else
                    {
                        VIU_DRV_SetMemAddr(i, j,
                                           g_struViuData[u32ViPort].struBufAddr[g_struViuData[u32ViPort].u32NextBufIndex].u32BufPhysAddr_Y,
                                           g_struViuData[u32ViPort].struBufAddr[g_struViuData[u32ViPort].u32NextBufIndex].u32BufPhysAddr_C,
                                           0);
                    }

                    g_struFbRoot[u32ViPort].struFb[g_struViuData[u32ViPort].u32NextBufIndex].bLost = HI_TRUE;
                    g_struViuData[u32ViPort].u32PTSMs[g_struViuData[u32ViPort].u32NextBufIndex] = do_GetTimeStamp();
                    g_struViuData[u32ViPort].u32TimeRef++;

                    /* capture next frame  */
                    VIU_DRV_Start(i, j);

                    g_stViOptmS[i].u32RevFrmFrIntf++;
                }

                // Frame_pulse_int-------------------------------------
                if ((u32IntStatus & 0x40) == 0x40)
                {
                    VIU_DRV_ClrInterruptStatus(i, j, 0x40);
                    Frame_pulse_int[ u32ViPort] += 1;
                }

                // Ntsc_pal_trans_int------------------------------------
                if ((u32IntStatus & 0x80) == 0x80)
                {
                    VIU_DRV_ClrInterruptStatus(i, j, 0x80);
                    Ntsc_pal_trans_int[ i * VIU_CHANNEL_MAX + j] += 1;
                }

                // Chdiv_err_int----------------------------------------
                if ((u32IntStatus & 0x100) == 0x100)
                {
                    VIU_DRV_ClrInterruptStatus(i, j, 0x100);
                    Chdiv_err_int[ u32ViPort] += 1;
                }
            }
        } /* for VIU_CHANNEL_MAX */
    } /* for  VIU_PORT_MAX */

    return IRQ_HANDLED;
}

#if 0
/*****************************************************************************
Prototype    : ViuGetFrame
Description  : get frameinfo from vi
Input        : u32ViPort indicates vi channel and vi device id
Output       : ptViFrm:video frame info
Return Value : HI_SUCCESS,HI_FAILURE
Calls        :
Called By    :
History        :
1.Date         : 2010/03/15
Author       : j00131665
Modification : Created function
*****************************************************************************/

HI_S32 Viu_UsrGetFrame(HI_U32 u32ViPort, HI_UNF_VI_BUF_S  *pstFrame)
{
    VIU_FB_S stFbattr;

    HI_U32 bufIndex   = VIU_FB_INVAL_INDEX;
    HI_U32 picPhyAddr = 0;

    VIU_CHECK_NULL_PTR(pstFrame);
    VIU_CHECK_PORT(u32ViPort);

    g_stViOptmS[u32ViPort].stStatisticInfo.AcquireTry[g_u32KernUserId[u32ViPort]]++;

    pstFrame->enBufMode = g_struViuData[u32ViPort].enBufMode;
    if (HI_UNF_VI_BUF_MMAP == pstFrame->enBufMode)
    {
        if (HI_SUCCESS != VIU_FbGetMMap(&g_struFbRoot[u32ViPort], &stFbattr))
        {
            return HI_ERR_VI_BUF_EMPTY;
        }

        pstFrame->u32PhyAddr[0] = stFbattr.u32PhysAddr;
        pstFrame->pVirAddr[0]   = (HI_U8*)stFbattr.u32VirtAddr;
        pstFrame->u32FrameIndex = stFbattr.u32Index;
        g_stViOptmS[u32ViPort].stStatisticInfo.AcquireOK[g_u32KernUserId[u32ViPort]]++;
    }
    else
    {
        //Get new buffer
        picPhyAddr = VIU_FbGet(g_u32KernUserId[u32ViPort], &g_struFbRoot[u32ViPort]);
        if (!picPhyAddr)
        {
            //No free block,Keep the last block for sink
            HI_WARN_VI("viu buf full, thisIndex=%d, NextIndex=%d\n",
                       g_struViuData[u32ViPort].u32ThisBufIndex,
                       g_struViuData[u32ViPort].u32NextBufIndex);
            g_u32IntLost[u32ViPort] += 1;
            return HI_ERR_VI_BUF_FULL;
        }
        else
        {
            bufIndex = VIU_FB_P2I(&g_struFbRoot[u32ViPort], picPhyAddr);
            {
                //			printk(KERN_ERR "use drv:lastIndex=%d, thisIndex=%d, NextIndex=%d\n",g_struViuData[u32ViPort].u32LastBufIndex,g_struViuData[u32ViPort].u32ThisBufIndex,g_struViuData[u32ViPort].u32NextBufIndex);
                g_struFbRoot[u32ViPort].struFb[bufIndex].bLost = HI_TRUE;
                g_struViuData[u32ViPort].u32PTSMs[bufIndex] = do_GetTimeStamp();

                //g_struViuData[u32ViPort].u32PTSMs[bufIndex]  = g_Pts++;
                g_struViuData[u32ViPort].u32TimeRef++;

                g_struViuData[u32ViPort].u32ThisBufIndex = bufIndex;
                pstFrame->u32PhyAddr[0] = g_struViuData[u32ViPort].struBufAddr[bufIndex].u32BufPhysAddr_Y;
                pstFrame->u32PhyAddr[1] = g_struViuData[u32ViPort].struBufAddr[bufIndex].u32BufPhysAddr_C;
                pstFrame->u32PhyAddr[2] = g_struViuData[u32ViPort].struBufAddr[bufIndex].u32BufPhysAddr_C;
                pstFrame->u32Stride[0] = g_struViuData[u32ViPort].u32StrideY;
                pstFrame->u32Stride[1] = g_struViuData[u32ViPort].u32StrideC;
                pstFrame->u32Stride[2] = g_struViuData[u32ViPort].u32StrideC;
                pstFrame->u32Height = g_struViuData[u32ViPort].struVchnAttr[0].u32CapHeight;
                pstFrame->u32Width = g_struViuData[u32ViPort].struVchnAttr[0].u32CapWidth;
                pstFrame->u32FrameIndex = bufIndex;
                pstFrame->u32PtsMs = g_struViuData[u32ViPort].u32PTSMs[bufIndex];
                g_stViOptmS[u32ViPort].stStatisticInfo.AcquireOK[g_u32KernUserId[u32ViPort]]++;
                return HI_SUCCESS;
            }
        }

        HI_INFO_VI("bufIndex=%u, this/next/last=%u/%u/%u\n",
                   bufIndex,
                   g_struViuData[u32ViPort].u32ThisBufIndex,
                   g_struViuData[u32ViPort].u32NextBufIndex,
                   g_struViuData[u32ViPort].u32LastBufIndex );
    }

    //HI_TRACE(HI_LOG_LEVEL_INFO, HI_DEBUG_ID_VSYNC,"VI_to_USER_PTS:%u.\n", g_struViuData[u32ViPort].u32PTSMs[bufIndex]);
    return HI_SUCCESS;
}

/*****************************************************************************
Prototype    : ViuPutFrame
Description  : release frameinfo to vi
Input        : u32ViPort indicates vi channel and vi device id
Output       : ptViFrm:video frame info
Return Value : HI_SUCCESS,HI_FAILURE
Calls        :
Called By    :
History        :
1.Date         : 2010/03/15
Author       : j00131665
Modification : Created function
*****************************************************************************/

HI_S32 Viu_UsrPutFrame(HI_U32 u32ViPort, HI_UNF_VI_BUF_S  *pstFrame)
{
    HI_U32 i = 0;

    VIU_CHECK_NULL_PTR(pstFrame);
    VIU_CHECK_PORT(u32ViPort);

    g_stViOptmS[u32ViPort].stStatisticInfo.ReleaseTry[g_u32KernUserId[u32ViPort]]++;
    g_stViOptmS[u32ViPort].u32RevFrmFrUsr++;

    if (HI_UNF_VI_BUF_MMAP == g_struViuData[u32ViPort].enBufMode)
    {
        pstFrame->u32FrameIndex = g_struViuData[u32ViPort].u32NextBufIndex++;
        if (g_struViuData[u32ViPort].u32NextBufIndex >= g_struFbRoot[u32ViPort].u32BlkNum)
        {
            g_struViuData[u32ViPort].u32NextBufIndex = 0;
        }

        // user frame index unmatches with vi frame index
        if ((HI_U32)pstFrame->pVirAddr[0] != g_struFbRoot[u32ViPort].struFb[pstFrame->u32FrameIndex].u32VirtAddr)
        {
            for (i = 0; i < g_struFbRoot[u32ViPort].u32BlkNum; i++)
            {
                if ((HI_U32)pstFrame->pVirAddr[0] == g_struFbRoot[u32ViPort].struFb[i].u32VirtAddr)
                {
                    g_struViuData[u32ViPort].u32NextBufIndex = i;
                    pstFrame->u32FrameIndex = g_struViuData[u32ViPort].u32NextBufIndex++;
                    break;
                }
            }
        }

        pstFrame->u32PtsMs = do_GetTimeStamp();
        g_struViuData[u32ViPort].u32PTSMs[pstFrame->u32FrameIndex] = pstFrame->u32PtsMs;
        g_struViuData[u32ViPort].struBufAddr[pstFrame->u32FrameIndex].u32BufPhysAddr_Y =
            g_struFbRoot[u32ViPort].struFb[pstFrame->u32FrameIndex].u32PhysAddr;
        g_struViuData[u32ViPort].struBufAddr[pstFrame->u32FrameIndex].pBufVirtAddr_Y   = HI_NULL;
        g_struViuData[u32ViPort].struBufAddr[pstFrame->u32FrameIndex].u32BufPhysAddr_C = 0;
        g_struViuData[u32ViPort].struBufAddr[pstFrame->u32FrameIndex].pBufVirtAddr_C = 0;
        g_struViuData[u32ViPort].u32TimeRef++; 
        g_stViOptmS[u32ViPort].stStatisticInfo.ReleaseOK[g_u32KernUserId[u32ViPort]]++;
        VIU_FbAddTailMMap(&g_struFbRoot[u32ViPort], pstFrame->u32FrameIndex);

        /*
                if (HI_NULL != gEncodeFunc)
                {
                    gEncodeFunc();
                }
         */
        if (s_pVencFunc && s_pVencFunc->pfnVencEncodeFrame)
        {
            (s_pVencFunc->pfnVencEncodeFrame)( );
        }
    }
    else
    {
        g_abNewInt[u32ViPort][0] = HI_TRUE;
        g_abNewInt[u32ViPort][1] = HI_TRUE;
        g_abNewInt[u32ViPort][2] = HI_TRUE;
        g_abNewInt[u32ViPort][3] = HI_TRUE;

        HI_TRACE(HI_LOG_LEVEL_INFO, HI_ID_VSYNC, "VIU_SER_put, PTS=%u.\n", pstFrame->u32PtsMs);
        g_stViOptmS[u32ViPort].stStatisticInfo.ReleaseOK[g_u32KernUserId[u32ViPort]]++;
        g_struViuData[u32ViPort].u32LastBufIndex = g_struViuData[u32ViPort].u32ThisBufIndex;
        VIU_FbSub( g_u32KernUserId[u32ViPort], &g_struFbRoot[u32ViPort], g_struViuData[u32ViPort].u32LastBufIndex);

        /*
                if (HI_NULL != gEncodeFunc)
                {
                    gEncodeFunc();
                }
         */
        if (s_pVencFunc && s_pVencFunc->pfnVencEncodeFrame)
        {
            (s_pVencFunc->pfnVencEncodeFrame)( );
        }

        /*if(!(pstFrame->u32PtsMs%100) || (99 == pstFrame->u32PtsMs%100) || (1 == pstFrame->u32PtsMs%100))
        {
            printk("VI make frame %u at time %u\n",pstFrame->u32PtsMs,CMPI_STAT_GetTick());

        }*/

        HI_INFO_VI("idx=%u, this/next/last=%u/%u/%u\n",
                   pstFrame->u32FrameIndex,

                   g_struViuData[u32ViPort].u32ThisBufIndex,
                   g_struViuData[u32ViPort].u32NextBufIndex,
                   g_struViuData[u32ViPort].u32LastBufIndex );
    }

    return HI_SUCCESS;
}
#else

HI_S32 Viu_DequeueFrame(HI_U32 u32ViPort, HI_UNF_VIDEO_FRAME_INFO_S *pFrameInfo)
{
    VIU_FB_S stFbattr;
    HI_U32 bufIndex   = VIU_FB_INVAL_INDEX;
    HI_U32 picPhyAddr = 0;

    VIU_CHECK_NULL_PTR(pFrameInfo);
    VIU_CHECK_PORT(u32ViPort);

    g_stViOptmS[u32ViPort].stStatisticInfo.AcquireTry[g_u32KernUserId[u32ViPort]]++;

	memset(pFrameInfo, 0, sizeof(HI_UNF_VIDEO_FRAME_INFO_S));
    if (HI_UNF_VI_BUF_MMAP == g_struViuData[u32ViPort].enBufMode)
    {
        if (HI_SUCCESS != VIU_FbGetMMap(&g_struFbRoot[u32ViPort], &stFbattr))
        {
            return HI_ERR_VI_BUF_EMPTY;
        }
		
		pFrameInfo->stVideoFrameAddr[0].u32YAddr = stFbattr.u32PhysAddr;
		pFrameInfo->u32FrameIndex = stFbattr.u32Index;
		pFrameInfo->u32Width  = g_struViuData[u32ViPort].struVchnAttr[0].u32CapWidth;
		pFrameInfo->u32Height = g_struViuData[u32ViPort].struVchnAttr[0].u32CapHeight;

        g_stViOptmS[u32ViPort].stStatisticInfo.AcquireOK[g_u32KernUserId[u32ViPort]]++;
    }
    else
    {
        //Get new buffer
        picPhyAddr = VIU_FbGet(g_u32KernUserId[u32ViPort], &g_struFbRoot[u32ViPort]);
        if (!picPhyAddr)
        {
            //No free block,Keep the last block for sink
            HI_WARN_VI("viu buf full, thisIndex=%d, NextIndex=%d\n",
                       g_struViuData[u32ViPort].u32ThisBufIndex,
                       g_struViuData[u32ViPort].u32NextBufIndex);
            g_u32IntLost[u32ViPort] += 1;
            return HI_ERR_VI_BUF_FULL;
        }
        else
        {
            bufIndex = VIU_FB_P2I(&g_struFbRoot[u32ViPort], picPhyAddr);
            {
                //			printk(KERN_ERR "use drv:lastIndex=%d, thisIndex=%d, NextIndex=%d\n",g_struViuData[u32ViPort].u32LastBufIndex,g_struViuData[u32ViPort].u32ThisBufIndex,g_struViuData[u32ViPort].u32NextBufIndex);
                g_struFbRoot[u32ViPort].struFb[bufIndex].bLost = HI_TRUE;
                g_struViuData[u32ViPort].u32PTSMs[bufIndex] = do_GetTimeStamp();

                //g_struViuData[u32ViPort].u32PTSMs[bufIndex]  = g_Pts++;
                g_struViuData[u32ViPort].u32TimeRef++;

                g_struViuData[u32ViPort].u32ThisBufIndex = bufIndex;
                pFrameInfo->stVideoFrameAddr[0].u32YAddr = g_struViuData[u32ViPort].struBufAddr[bufIndex].u32BufPhysAddr_Y;
                pFrameInfo->stVideoFrameAddr[0].u32CAddr = g_struViuData[u32ViPort].struBufAddr[bufIndex].u32BufPhysAddr_C;
				
                pFrameInfo->stVideoFrameAddr[0].u32YStride = g_struViuData[u32ViPort].u32StrideY;
                pFrameInfo->stVideoFrameAddr[0].u32CStride = g_struViuData[u32ViPort].u32StrideC;
                pFrameInfo->u32Height = g_struViuData[u32ViPort].struVchnAttr[0].u32CapHeight;
                pFrameInfo->u32Width = g_struViuData[u32ViPort].struVchnAttr[0].u32CapWidth;
                pFrameInfo->u32FrameIndex = bufIndex;
                pFrameInfo->u32Pts = g_struViuData[u32ViPort].u32PTSMs[bufIndex];
                g_stViOptmS[u32ViPort].stStatisticInfo.AcquireOK[g_u32KernUserId[u32ViPort]]++;
                return HI_SUCCESS;
            }
        }
    }

    return HI_SUCCESS;
}

HI_S32 Viu_QueueFrame(HI_U32 u32ViPort, HI_UNF_VIDEO_FRAME_INFO_S *pFrameInfo)
{
    VIU_CHECK_NULL_PTR(pFrameInfo);
    VIU_CHECK_PORT(u32ViPort);

    g_stViOptmS[u32ViPort].stStatisticInfo.ReleaseTry[g_u32KernUserId[u32ViPort]]++;
    g_stViOptmS[u32ViPort].u32RevFrmFrUsr++;

    if (HI_UNF_VI_BUF_MMAP == g_struViuData[u32ViPort].enBufMode)
    {
        pFrameInfo->u32FrameIndex = g_struViuData[u32ViPort].u32NextBufIndex++;
        if (g_struViuData[u32ViPort].u32NextBufIndex >= g_struFbRoot[u32ViPort].u32BlkNum)
        {
            g_struViuData[u32ViPort].u32NextBufIndex = 0;
        }
#if 0
        // user frame index unmatches with vi frame index
        if ((HI_U32)pstFrame->pVirAddr[0] != g_struFbRoot[u32ViPort].struFb[pstFrame->u32FrameIndex].u32VirtAddr)
        {
            for (i = 0; i < g_struFbRoot[u32ViPort].u32BlkNum; i++)
            {
                if ((HI_U32)pstFrame->pVirAddr[0] == g_struFbRoot[u32ViPort].struFb[i].u32VirtAddr)
                {
                    g_struViuData[u32ViPort].u32NextBufIndex = i;
                    pstFrame->u32FrameIndex = g_struViuData[u32ViPort].u32NextBufIndex++;
                    break;
                }
            }
        }
#endif
        pFrameInfo->u32Pts = do_GetTimeStamp();
        g_struViuData[u32ViPort].u32PTSMs[pFrameInfo->u32FrameIndex] = pFrameInfo->u32Pts;
        g_struViuData[u32ViPort].struBufAddr[pFrameInfo->u32FrameIndex].u32BufPhysAddr_Y =
            g_struFbRoot[u32ViPort].struFb[pFrameInfo->u32FrameIndex].u32PhysAddr;
        g_struViuData[u32ViPort].struBufAddr[pFrameInfo->u32FrameIndex].pBufVirtAddr_Y   = HI_NULL;
        g_struViuData[u32ViPort].struBufAddr[pFrameInfo->u32FrameIndex].u32BufPhysAddr_C = 0;
        g_struViuData[u32ViPort].struBufAddr[pFrameInfo->u32FrameIndex].pBufVirtAddr_C = 0;
        g_struViuData[u32ViPort].u32TimeRef++; 
        g_stViOptmS[u32ViPort].stStatisticInfo.ReleaseOK[g_u32KernUserId[u32ViPort]]++;
        VIU_FbAddTailMMap(&g_struFbRoot[u32ViPort], pFrameInfo->u32FrameIndex);

        if (s_pVencFunc && s_pVencFunc->pfnVencEncodeFrame)
        {
            (s_pVencFunc->pfnVencEncodeFrame)( );
        }
    }
    else
    {
        g_abNewInt[u32ViPort][0] = HI_TRUE;
        g_abNewInt[u32ViPort][1] = HI_TRUE;
        g_abNewInt[u32ViPort][2] = HI_TRUE;
        g_abNewInt[u32ViPort][3] = HI_TRUE;

        HI_TRACE(HI_LOG_LEVEL_INFO, HI_ID_VSYNC, "VIU_SER_put, PTS=%u.\n", pFrameInfo->u32Pts);
        g_stViOptmS[u32ViPort].stStatisticInfo.ReleaseOK[g_u32KernUserId[u32ViPort]]++;
        g_struViuData[u32ViPort].u32LastBufIndex = g_struViuData[u32ViPort].u32ThisBufIndex;
        VIU_FbSub( g_u32KernUserId[u32ViPort], &g_struFbRoot[u32ViPort], g_struViuData[u32ViPort].u32LastBufIndex);

        if (s_pVencFunc && s_pVencFunc->pfnVencEncodeFrame)
        {
            (s_pVencFunc->pfnVencEncodeFrame)( );
        }

        HI_INFO_VI("idx=%u, this/next/last=%u/%u/%u\n",
                   pFrameInfo->u32FrameIndex,

                   g_struViuData[u32ViPort].u32ThisBufIndex,
                   g_struViuData[u32ViPort].u32NextBufIndex,
                   g_struViuData[u32ViPort].u32LastBufIndex );
    }

    return HI_SUCCESS;
}

#endif
/*****************************************************************************
Prototype    : ViuGetFrame
Description  : get frameinfo from vi
Input        : u32ViPort indicates vi channel and vi device id
Output       : ptViFrm:video frame info
Return Value : HI_SUCCESS,HI_FAILURE
Calls        :
Called By    :
History        :
1.Date         : 2010/03/15
Author       : j00131665
Modification : Created function
*****************************************************************************/
HI_S32 Viu_UsrAcquireFrame(HI_U32 u32ChnID, HI_U32 u32Uid, HI_UNF_VIDEO_FRAME_INFO_S *pFrameInfo)
{
    HI_S32 ret;
    VIU_FB_S stFbattr;

    HI_UNF_VIDEO_FRAME_INFO_S stFrameInfoTmp;

    VIU_CHECK_NULL_PTR(pFrameInfo);
    VIU_CHECK_PORT(u32ChnID);

    if (u32Uid >= VI_UID_MAX)
    {
        HI_ERR_VI("UserID(%u) invalid.\n", u32Uid);
        return HI_ERR_VI_INVALID_PARA;
    }

	memset(&stFrameInfoTmp, 0, sizeof(HI_UNF_VIDEO_FRAME_INFO_S));
    g_stViOptmS[u32ChnID].stStatisticInfo.AcquireTry[u32Uid]++;
    if (HI_UNF_VI_BUF_MMAP == g_struViuData[u32ChnID].enBufMode)
    {
        ret = VIU_FbAddMMap( u32Uid, &g_struFbRoot[u32ChnID], &stFbattr);
        if (ret != 0)
        {
            return HI_ERR_VI_BUF_EMPTY;
        }

        g_struFbRoot[u32ChnID].struFb[stFbattr.u32Index].bLost = HI_FALSE;
        stFrameInfoTmp.stVideoFrameAddr[0].u32YAddr = stFbattr.u32PhysAddr;
        stFrameInfoTmp.u32Pts = g_struViuData[u32ChnID].u32PTSMs[stFbattr.u32Index];
        stFrameInfoTmp.u32FrameIndex = stFbattr.u32Index;
        stFrameInfoTmp.enFieldMode = HI_UNF_VIDEO_FIELD_ALL;
    }
    else
    {
        if ((VIU_FB_INVAL_INDEX == g_struViuData[u32ChnID].u32LastBufIndex)
            || (VIU_FB_INVAL_INDEX_LAST == g_struViuData[u32ChnID].u32LastBufIndex))
        {
            return HI_ERR_VI_BUF_EMPTY;
        }

        VIU_FbAdd(u32Uid, &g_struFbRoot[u32ChnID], g_struViuData[u32ChnID].u32LastBufIndex);
        g_struFbRoot[u32ChnID].struFb[g_struViuData[u32ChnID].u32LastBufIndex].bLost = HI_FALSE;
        stFrameInfoTmp.u32FrameIndex = g_struViuData[u32ChnID].u32LastBufIndex;

        stFrameInfoTmp.stVideoFrameAddr[0].u32YAddr =
            g_struViuData[u32ChnID].struBufAddr[g_struViuData[u32ChnID].u32LastBufIndex].u32BufPhysAddr_Y;
        stFrameInfoTmp.u32Pts = g_struViuData[u32ChnID].u32PTSMs[g_struViuData[u32ChnID].u32LastBufIndex];
        stFrameInfoTmp.enFieldMode = g_struViuData[u32ChnID].u32LastBufIndex % 2;
    }

	stFrameInfoTmp.enVideoFormat = s_astViAttr[u32ChnID].enVideoFormat;
#if 0
    if (s_astViAttr[u32ChnID].enStoreMethod == HI_UNF_VI_STORE_METHOD_SPNYC)
    {
        stFrameInfoTmp.enVideoFormat = HI_UNF_FORMAT_YUV_SEMIPLANAR_420;
    }
    else if (s_astViAttr[u32ChnID].enStoreMethod == HI_UNF_VI_STORE_METHOD_PKYUV)
    {
        stFrameInfoTmp.enVideoFormat = HI_UNF_FORMAT_YUV_PACKAGE_YUYV;
    }
    else
    {
        stFrameInfoTmp.enVideoFormat = HI_UNF_FORMAT_YUV_SEMIPLANAR_420;
    }
#endif
    stFrameInfoTmp.stVideoFrameAddr[0].u32YStride = g_struViuData[u32ChnID].u32StrideY;
    stFrameInfoTmp.stVideoFrameAddr[0].u32CStride = g_struViuData[u32ChnID].u32StrideC;
    stFrameInfoTmp.u32Height = g_struViuData[u32ChnID].struVchnAttr[0].u32CapHeight;
    stFrameInfoTmp.u32Width = g_struViuData[u32ChnID].struVchnAttr[0].u32CapWidth;

    memcpy(pFrameInfo, &stFrameInfoTmp, sizeof(HI_UNF_VIDEO_FRAME_INFO_S));

    g_stViOptmS[u32ChnID].u32SndFrmToMd++;

    g_stViOptmS[u32ChnID].stStatisticInfo.AcquireOK[u32Uid]++;

    return HI_SUCCESS;
}

/*****************************************************************************
Prototype    : ViuPutFrame
Description  : release frameinfo to vi
Input        : u32ViPort indicates vi channel and vi device id
Output       : ptViFrm:video frame info
Return Value : HI_SUCCESS,HI_FAILURE
Calls        :
Called By    :
History        :
1.Date         : 2010/03/15
Author       : j00131665
Modification : Created function
*****************************************************************************/
HI_S32 Viu_UsrReleaseFrame(HI_U32 u32ChnID, HI_U32 u32Uid, HI_UNF_VIDEO_FRAME_INFO_S *pFrameInfo)
{
    VIU_CHECK_NULL_PTR(pFrameInfo);
    VIU_CHECK_PORT(u32ChnID);

    if (u32Uid >= VI_UID_MAX)
    {
        HI_ERR_VI("UserID(%u) invalid.\n", u32Uid);
        return HI_ERR_VI_INVALID_PARA;
    }

    g_stViOptmS[u32ChnID].stStatisticInfo.ReleaseTry[u32Uid]++;

    if (HI_UNF_VI_BUF_MMAP == g_struViuData[u32ChnID].enBufMode)
    {
        VIU_FbSubMMap( u32Uid, &g_struFbRoot[u32ChnID], pFrameInfo->u32FrameIndex);
    }
    else
    {
        VIU_FbSub( u32Uid, &g_struFbRoot[u32ChnID], pFrameInfo->u32FrameIndex);
    }

    g_stViOptmS[u32ChnID].stStatisticInfo.ReleaseOK[u32Uid]++;

    return HI_SUCCESS;
}

/* VO/VEDU get frame from VI*/
HI_S32 Viu_AcquireFrame(HI_S32 handle, HI_UNF_VIDEO_FRAME_INFO_S  *pstFrame)
{
    HI_S32 ret;
    HI_U32 u32ChnID, u32Uid;
    VIU_FB_S stFbattr;
    HI_VDEC_PRIV_FRAMEINFO_S* pstPrivInfo = (HI_VDEC_PRIV_FRAMEINFO_S*)pstFrame->u32Private;

    u32ChnID = handle & 0xff;
    u32Uid = (handle & 0xff00) >> 8;

    VIU_CHECK_NULL_PTR(pstFrame);
    VIU_CHECK_PORT(u32ChnID);

    if (u32Uid >= VI_UID_MAX)
    {
        HI_ERR_VI("UserID(%u) invalid.\n", u32Uid);
        return HI_ERR_VI_INVALID_PARA;
    }

    g_stViOptmS[u32ChnID].stStatisticInfo.AcquireTry[u32Uid]++;

	memset(pstFrame, 0, sizeof(HI_UNF_VIDEO_FRAME_INFO_S));
	
    if (HI_UNF_VI_BUF_MMAP == g_struViuData[u32ChnID].enBufMode)
    {
        if (!g_struFbRoot[u32ChnID].u32FbPhysAddr)
        {
            return HI_ERR_VI_BUF_EMPTY;
        }

        ret = VIU_FbAddMMap( u32Uid, &g_struFbRoot[u32ChnID], &stFbattr);
        if (ret != 0)
        {
            return HI_ERR_VI_BUF_EMPTY;
        }

        g_struFbRoot[u32ChnID].struFb[stFbattr.u32Index].bLost = HI_FALSE;
        pstFrame->stVideoFrameAddr[0].u32YAddr = stFbattr.u32PhysAddr;
        pstFrame->stVideoFrameAddr[0].u32CAddr = 0;
//        pstFrame->u32LstYAddr = stFbattr.u32VirtAddr;
//        pstFrame->u32LstYCddr = 0;
        pstFrame->u32Pts = g_struViuData[u32ChnID].u32PTSMs[stFbattr.u32Index];
        pstFrame->u32FrameIndex = stFbattr.u32Index;
    }
    else
    {
        if ((!g_abNewInt[u32ChnID][u32Uid])
            || (VIU_FB_INVAL_INDEX == g_struViuData[u32ChnID].u32LastBufIndex)
            || (VIU_FB_INVAL_INDEX_LAST == g_struViuData[u32ChnID].u32LastBufIndex))
        {
            return HI_ERR_VI_BUF_EMPTY;
        }

        VIU_FbAdd( u32Uid, &g_struFbRoot[u32ChnID], g_struViuData[u32ChnID].u32LastBufIndex);

        g_struFbRoot[u32ChnID].struFb[g_struViuData[u32ChnID].u32LastBufIndex].bLost = HI_FALSE;
        pstFrame->stVideoFrameAddr[0].u32YAddr =
            g_struViuData[u32ChnID].struBufAddr[g_struViuData[u32ChnID].u32LastBufIndex].u32BufPhysAddr_Y;
        pstFrame->stVideoFrameAddr[0].u32CAddr =
            g_struViuData[u32ChnID].struBufAddr[g_struViuData[u32ChnID].u32LastBufIndex].u32BufPhysAddr_C;
//        pstFrame->u32LstYAddr =
//            (HI_U32)(g_struViuData[u32ChnID].struBufAddr[g_struViuData[u32ChnID].u32LastBufIndex].pBufVirtAddr_Y);
//        pstFrame->u32LstYCddr =
//            (HI_U32)(g_struViuData[u32ChnID].struBufAddr[g_struViuData[u32ChnID].u32LastBufIndex].pBufVirtAddr_C);
        pstFrame->u32Pts = g_struViuData[u32ChnID].u32PTSMs[g_struViuData[u32ChnID].u32LastBufIndex];
        pstFrame->u32FrameIndex = g_struViuData[u32ChnID].u32LastBufIndex;

        if (s_astViAttr[u32ChnID].enInputMode >= HI_UNF_VI_MODE_BT1120_480P)
        {
            pstFrame->stVideoFrameAddr[0].u32YAddr =
                g_struViuData[0].struBufAddr[g_struViuData[u32ChnID].u32LastBufIndex].u32BufPhysAddr_Y;
            pstFrame->stVideoFrameAddr[0].u32CAddr =
                g_struViuData[1].struBufAddr[g_struViuData[u32ChnID].u32LastBufIndex].u32BufPhysAddr_C;
//            pstFrame->u32LstYAddr =
//                (HI_U32)(g_struViuData[0].struBufAddr[g_struViuData[u32ChnID].u32LastBufIndex].pBufVirtAddr_Y);
//            pstFrame->u32LstYCddr =
//                (HI_U32)(g_struViuData[1].struBufAddr[g_struViuData[u32ChnID].u32LastBufIndex].pBufVirtAddr_C);
        }
    }

    if (s_astViAttr[u32ChnID].enInputMode >= HI_UNF_VI_MODE_BT1120_480P)
    {
        pstFrame->stVideoFrameAddr[0].u32YStride = g_struViuData[0].u32StrideY;
        pstFrame->stVideoFrameAddr[0].u32CStride = g_struViuData[1].u32StrideC;
    }
    else
    {
        pstFrame->stVideoFrameAddr[0].u32YStride = g_struViuData[u32ChnID].u32StrideY;
        pstFrame->stVideoFrameAddr[0].u32CStride = g_struViuData[u32ChnID].u32StrideC;
    }

    pstFrame->u32Width  = g_struViuData[u32ChnID].struVchnAttr[0].u32CapWidth;
    pstFrame->u32Height = g_struViuData[u32ChnID].struVchnAttr[0].u32CapHeight;
	pstFrame->enVideoFormat = s_astViAttr[u32ChnID].enVideoFormat;

#if 0
    if (s_astViAttr[u32ChnID].enInputMode >= HI_UNF_VI_MODE_BT1120_480P)
    {
        pstFrame->enVideoFormat = HI_UNF_FORMAT_YUV_SEMIPLANAR_422;
    }
    else if (s_astViAttr[u32ChnID].enStoreMethod == HI_UNF_VI_STORE_METHOD_SPNYC)
    {
        pstFrame->enVideoFormat = HI_UNF_FORMAT_YUV_SEMIPLANAR_420;
    }
    else if (s_astViAttr[u32ChnID].enStoreMethod == HI_UNF_VI_STORE_METHOD_PKYUV)
    {
        pstFrame->enVideoFormat = HI_UNF_FORMAT_YUV_PACKAGE_YUYV;
    }
    else
    {
        pstFrame->enVideoFormat = HI_UNF_FORMAT_YUV_SEMIPLANAR_420;
    }
#endif
//    pstFrame->u32CompressFlag = 0;
    pstFrame->bTopFieldFirst = HI_TRUE;
    pstFrame->enFieldMode = HI_UNF_VIDEO_FIELD_ALL;
	pstPrivInfo->u32SeqFrameCnt = g_struViuData[u32ChnID].u32TimeRef;
//    pstFrame->u32SeqFrameCnt = g_struViuData[u32ChnID].u32TimeRef; /* important!! VENC care this!!! */
	pstFrame->bProgressive = HI_TRUE;
//    pstFrame->enSampleType = HI_UNF_VIDEO_SAMPLE_TYPE_PROGRESSIVE;
    pstFrame->enFieldMode = HI_UNF_VIDEO_FIELD_ALL;
    pstFrame->bTopFieldFirst = HI_TRUE;
	pstFrame->u32AspectWidth = 4;
	pstFrame->u32AspectHeight = 3;
//    pstFrame->enAspectRatio = HI_UNF_ASPECT_RATIO_4TO3;
    pstFrame->u32Width  = g_struViuData[u32ChnID].struVchnAttr[0].u32CapWidth;
    pstFrame->u32Height = g_struViuData[u32ChnID].struVchnAttr[0].u32CapHeight;
    pstFrame->u32DisplayCenterX = g_struViuData[u32ChnID].struVchnAttr[0].u32CapWidth / 2;
    pstFrame->u32DisplayCenterY = g_struViuData[u32ChnID].struVchnAttr[0].u32CapHeight / 2;
    pstFrame->u32DisplayWidth  = g_struViuData[u32ChnID].struVchnAttr[0].u32CapWidth;
    pstFrame->u32DisplayHeight = g_struViuData[u32ChnID].struVchnAttr[0].u32CapHeight;
	pstPrivInfo->u8Repeat = 1;

//    pstFrame->u32Repeat = 1;
//    memset(pstFrame->u32VdecInfo, 0, sizeof(HI_U32) * 8);
#if 0
    if (s_astViAttr[u32ChnID].enStoreMethod == HI_UNF_VI_STORE_METHOD_PKYUV)
    {
//        pstFrame->u32CAddr   = pstFrame->u32YAddr;
        pstFrame->stVideoFrameAddr[0].u32CStride = pstFrame->stVideoFrameAddr[0].u32YStride;
    }
#endif	
	if ((s_astViAttr[u32ChnID].enVideoFormat >= HI_UNF_FORMAT_YUV_PACKAGE_UYVY) &&
		(s_astViAttr[u32ChnID].enVideoFormat <= HI_UNF_FORMAT_YUV_PACKAGE_YVYU))
    {
        pstFrame->stVideoFrameAddr[0].u32CAddr = pstFrame->stVideoFrameAddr[0].u32YAddr;
        pstFrame->stVideoFrameAddr[0].u32CStride = pstFrame->stVideoFrameAddr[0].u32YStride;
    }
	g_abNewInt[u32ChnID][u32Uid] = HI_FALSE;

    //	printk(KERN_ERR "Acquireframe: index=%d, u32ViPort=%d, Chn=%d, uid=%d, pts=%d\n", g_struViuData[u32ChnID].u32LastBufIndex,handle,u32ChnID,u32Uid,pstFrame->u32Pts);
    g_stViOptmS[u32ChnID].u32SndFrmToMd++;
    g_stViOptmS[u32ChnID].stStatisticInfo.AcquireOK[u32Uid]++;
    HI_TRACE(HI_LOG_LEVEL_INFO, HI_ID_VSYNC, "VI_to_VENC_PTS:%u.\n", pstFrame->u32Pts);

    HI_INFO_VI("idx=%u, this/next/last=%u/%u/%u\n",
               pstFrame->u32FrameIndex,
               g_struViuData[u32ChnID].u32ThisBufIndex,
               g_struViuData[u32ChnID].u32NextBufIndex,
               g_struViuData[u32ChnID].u32LastBufIndex );

    return HI_SUCCESS;
}

/* VO/VEDU release frame to VI*/
HI_S32 Viu_ReleaseFrame(HI_S32 handle, HI_UNF_VIDEO_FRAME_INFO_S  *pstFrame)
{
    HI_U32 u32ChnID, u32Uid;

    u32ChnID = handle & 0xff;
    u32Uid = (handle & 0xff00) >> 8;

    VIU_CHECK_NULL_PTR(pstFrame);
    VIU_CHECK_PORT(u32ChnID);

    if (u32Uid >= VI_UID_MAX)
    {
        HI_ERR_VI("UserID(%u) invalid.\n", u32Uid);
        return HI_ERR_VI_INVALID_PARA;
    }

    g_stViOptmS[u32ChnID].stStatisticInfo.ReleaseTry[u32Uid]++;

    //    printk(KERN_ERR "Release frame index=%d\n",pstFrame->u32FrameIndex);
    if (HI_UNF_VI_BUF_MMAP == g_struViuData[u32ChnID].enBufMode)
    {
        /*if(u32Uid == 2)
        {
            printk("VO release frame time %d\n",CMPI_STAT_GetTick());
        }*/
        VIU_FbSubMMap( u32Uid, &g_struFbRoot[u32ChnID], pstFrame->u32FrameIndex);
    }
    else
    {
        VIU_FbSub( u32Uid, &g_struFbRoot[u32ChnID], pstFrame->u32FrameIndex);
    }

    g_stViOptmS[u32ChnID].stStatisticInfo.ReleaseOK[u32Uid]++;

    HI_INFO_VI("idx=%u, this/next/last=%u/%u/%u\n",
               pstFrame->u32FrameIndex,

               g_struViuData[u32ChnID].u32ThisBufIndex,
               g_struViuData[u32ChnID].u32NextBufIndex,
               g_struViuData[u32ChnID].u32LastBufIndex );

    return HI_SUCCESS;
}

/*   U32_userID:

---16bit------8bit--------8bit---
 |  MOD_ID  | userID   | viChn   |
---------------------------------

 */
HI_S32 Viu_GetUsrID(HI_U32 u32ChnID, HI_MOD_ID_E enUserMode, HI_U32 *pu32Uid)
{
    HI_S32 i;

    u32ChnID = u32ChnID & 0xff;

    VIU_CHECK_PORT(u32ChnID);
    VIU_CHECK_NULL_PTR(pu32Uid);

    for (i = 1; i < VI_UID_MAX; i++) /* 0 reserved for VIU */
    {
        if (0 == atomic_read(&(g_ViUidUsed[u32ChnID][i])))
        {
            *pu32Uid = (HI_ID_VI << 16) + (i << 8) + u32ChnID;
            atomic_inc(&(g_ViUidUsed[u32ChnID][i]));

            g_ViUserMod[u32ChnID][i] = enUserMode;

            //            printk("getuid, chn=%d, uid=%x\n",u32ChnID,*pu32Uid);
            break;
        }
    }

    /*
        if (HI_ID_VENC == enUserMode)
        {
            gEncodeFunc = &Venc_EncodeFrame;
        }
     */
    if (HI_ID_VENC == enUserMode)
    {
        HI_DRV_MODULE_GetFunction(HI_ID_VENC, (HI_VOID**)&s_pVencFunc);
        if (HI_NULL == s_pVencFunc)
        {
            HI_ERR_VI("HI_DRV_MODULE_GetFunction failed, mode ID = 0x%08X\n", HI_ID_VI);
			return HI_FAILURE; 
        }
    }

    VIU_FbAddUser(i, &g_struFbRoot[u32ChnID]);

    /* Max user id lie on VI Buffer's structure*/
    if (VI_UID_MAX <= i)
    {
        HI_ERR_VI("can NOT get VI_UserID, already max UserID num.\n");
        return HI_ERR_VI_BUSY;
    }

    HI_INFO_VI("Get VI%d user(%#x).\n", u32ChnID, (HI_ID_VI << 16) + (i << 8) + u32ChnID);

    return HI_SUCCESS;
}

HI_S32 Viu_PutUsrID(HI_U32 u32ChnID, HI_U32 u32Uid)
{
    HI_U32 u32Usrid;

    u32ChnID = u32ChnID & 0xff;
    VIU_CHECK_PORT(u32ChnID);

    //    printk("putuid, chn=%d, uid=%x\n",u32ChnID,u32Uid);

    u32Usrid = (u32Uid & 0xff00) >> 8;
    if (!atomic_read(&(g_ViUidUsed[u32ChnID][u32Usrid])))
    {
        HI_WARN_VI("PutUserID ERR, chn=%d, uid=%d\n", u32ChnID, u32Usrid);
        return HI_SUCCESS;
    }

    atomic_dec(&(g_ViUidUsed[u32ChnID][u32Usrid]));

    /*
    if(!atomic_dec_and_test(&(g_ViUidUsed[u32ChnID][(u32Uid&0xff00)>>8])))
    {
        HI_WARN_VI("PutUserID ERR, chn=%d, uid=%d\n",u32ChnID,(u32Uid&0xff00)>>8);
        return HI_FAILURE;
    }
     */
    if (HI_ID_VENC == g_ViUserMod[u32ChnID][u32Usrid])
    {
        s_pVencFunc = HI_NULL;
    }

    VIU_FbSubUser(u32Usrid, &g_struFbRoot[u32ChnID]);

    g_ViUserMod[u32ChnID][u32Usrid] = HI_ID_BUTT;

    HI_INFO_VI("Put VI%d user(%#x).\n", u32ChnID, u32Uid);

    return HI_SUCCESS;
}

HI_U32 g_u32JifFirst = 0;

HI_U32 do_GetTimeStamp(void)
{
    static HI_U32 u32JifLast = 0;
    static HI_U32 u32Ret = 0;
    HI_U32 u32Add;
    HI_U32 u32JifNow;
    static HI_U32 u32UsecComp = 0;
    unsigned long flags;

    spin_lock_irqsave(&time_lock, flags);

    u32JifNow = jiffies;

    if (g_u32JifFirst == 0)
    {
        g_u32JifFirst = 1;
        u32JifLast = u32JifNow;
    }

    if (u32JifNow < u32JifLast)
    {
        u32Add = 0xFFFFFFFFU - u32JifLast + u32JifNow + 1;
    }
    else
    {
        u32Add = u32JifNow - u32JifLast;
    }

    if (u32Add == 0)
    {
        u32UsecComp += 10;
    }
    else
    {
        u32UsecComp = 0;
    }

    u32Add = u32Add * 10; /* jiffies=10ms */

    u32Ret = u32Ret + u32Add;

    u32JifLast = u32JifNow;

    spin_unlock_irqrestore(&time_lock, flags);

    return (HI_U32) (u32Ret + u32UsecComp);
}

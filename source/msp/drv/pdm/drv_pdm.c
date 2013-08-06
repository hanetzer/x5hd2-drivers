#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/vmalloc.h>
#include <asm/memory.h>
#include <linux/bootmem.h>
#include <linux/gfp.h>
#include <linux/mm.h>
#include <linux/memblock.h>

#include "hi_drv_pdm.h"
#include "hi_db.h"
#include "drv_pdm_ext.h"
#include "drv_pdm.h"


extern PDM_GLOBAL_S        g_PdmGlobal;

/*the function to get pdm tag data*/
extern int get_param_data(const char *name, char *buf, unsigned int buflen);

static HI_UNF_DISP_TIMING_S   g_stDispTiming = 
{
    .VFB = 27,
    .VBB = 23,
    .VACT = 768,
    .HFB = 210, 
    .HBB = 46,  
    .HACT = 1366,
    .VPW = 4,    
    .HPW = 24,   
    .IDV = 0,    
    .IHS = 0,    
    .IVS = 0,    
    .ClockReversal = HI_FALSE,
    .DataWidth = HI_UNF_DISP_INTF_DATA_WIDTH24,
    .ItfFormat = HI_UNF_DISP_INTF_DATA_FMT_RGB888,
    .DitherEnable = HI_FALSE,                    
    .ClkPara0 = 0x912ccccc,       
    .ClkPara1 = 0x006d8157,       
    //.InRectWidth = 1366,
    //.InRectHeight = 768,
};

HI_VOID PDM_GetDefDispParam(HI_UNF_DISP_E enDisp, HI_DISP_PARAM_S *pstDispParam)
{
    HI_S32  i;
    
    pstDispParam->u32HuePlus                  = 50;
    pstDispParam->u32Saturation               = 50;
    pstDispParam->u32Contrast                 = 50;
    pstDispParam->u32Brightness               = 50;

    pstDispParam->stBgColor.u8Red             = 0x00;
    pstDispParam->stBgColor.u8Green           = 0x00;
    pstDispParam->stBgColor.u8Blue            = 0xFF;

    for (i=0; i<HI_UNF_DISP_INTF_TYPE_BUTT; i++)
    {
        pstDispParam->stIntf[i].enIntfType = HI_UNF_DISP_INTF_TYPE_BUTT;
    }

    if (enDisp == HI_UNF_DISPLAY1)
    {
        pstDispParam->enFormat                    = HI_UNF_ENC_FMT_1080i_50;
#if 0        
        pstDispParam->stIntf[HI_UNF_DISP_INTF_TYPE_YPBPR].enIntfType = HI_UNF_DISP_INTF_TYPE_YPBPR;
    	pstDispParam->stIntf[HI_UNF_DISP_INTF_TYPE_YPBPR].unIntf.stYPbPr.u8DacY = 1;
    	pstDispParam->stIntf[HI_UNF_DISP_INTF_TYPE_YPBPR].unIntf.stYPbPr.u8DacPb = 0;
    	pstDispParam->stIntf[HI_UNF_DISP_INTF_TYPE_YPBPR].unIntf.stYPbPr.u8DacPr = 2;
#endif    	
	}

    if (enDisp == HI_UNF_DISPLAY0)
    {
        pstDispParam->enFormat                    = HI_UNF_ENC_FMT_PAL;
#if 0        
    	pstDispParam->stIntf[HI_UNF_DISP_INTF_TYPE_CVBS].enIntfType = HI_UNF_DISP_INTF_TYPE_CVBS;
    	pstDispParam->stIntf[HI_UNF_DISP_INTF_TYPE_CVBS].unIntf.stCVBS.u8Dac = 3;
#endif    	
	}

    pstDispParam->stAspectRatio.enDispAspectRatio = HI_UNF_DISP_ASPECT_RATIO_AUTO;

    pstDispParam->stDispTiming                = g_stDispTiming;

    return;
}

HI_VOID PDM_GetDefGrcParam(HI_GRC_PARAM_S *pstGrcParam)
{
    pstGrcParam->enPixelFormat               = HIGO_PF_8888;
    pstGrcParam->u32DisplayWidth             = 1280;
    pstGrcParam->u32DisplayHeight            = 720;
    pstGrcParam->u32ScreenXpos               = 0;
    pstGrcParam->u32ScreenYpos               = 0;
    pstGrcParam->u32ScreenWidth              = 1920;
    pstGrcParam->u32ScreenHeight             = 1080;  
    
    return;
}

/*******************************************
tag format is version=1.0.0.0  fb=0x85000000,0x10000  baseparam=0x86000000,0x2000 бнбн
*******************************************/
HI_S32 PDM_GetBufByName(const HI_CHAR *BufName, HI_U32 *pu32BasePhyAddr, HI_U32 *pu32Len)
{
    HI_CHAR                 PdmTag[512];
    HI_U32                  PdmLen;
    HI_CHAR                 TmpBuf[32];
    HI_CHAR                 *p, *q;
    HI_U32                  i;    

    for (i = 0; i < g_PdmGlobal.ReleaseBufNum; i++)
    {
        if (0 == strncmp(g_PdmGlobal.ReleaseBufName[i], BufName, strlen(BufName)))
        {
            HI_ERR_PDM("the reserve mem has released!\n");
            return HI_FAILURE;
        }
    }

    memset(PdmTag, 0x0, 512);

    PdmLen = get_param_data("pdm_tag", PdmTag, 512);
    if (PdmLen < 0 || PdmLen >= 512)
    {
        return HI_FAILURE;
    }

    p = strstr(PdmTag, BufName);
    if (0 == p)
    {
        return HI_FAILURE;
    }

    p = strstr(p, "=");
    if (0 == p)
    {
        return HI_FAILURE;
    }

    p += 1;

    q = strstr(p, ",");
    if (0 == q)
    {
        return HI_FAILURE;
    }

    memset(TmpBuf, 0x0, 32);
    memcpy(TmpBuf, p, q-p);
    *pu32BasePhyAddr = simple_strtoul(TmpBuf, NULL, 16);

    q++;

    p = strstr(q, " ");
    if (0 == p)
    {
        p = PdmTag + PdmLen;
    }
    
    memset(TmpBuf, 0x0, 32);
    memcpy(TmpBuf, q, p-q);

    *pu32Len = simple_strtoul(TmpBuf, NULL, 16);

    return HI_SUCCESS;
}

HI_VOID PDM_TransFomat(HI_UNF_ENC_FMT_E enSrcFmt, HI_UNF_ENC_FMT_E *penHdFmt, HI_UNF_ENC_FMT_E *penSdFmt)
{
    switch(enSrcFmt)
    {
        /* bellow are tv display formats */
        case HI_UNF_ENC_FMT_1080P_60:
        {
            *penHdFmt = HI_UNF_ENC_FMT_1080P_60;
            *penSdFmt = HI_UNF_ENC_FMT_NTSC;
            break;
        }
        case HI_UNF_ENC_FMT_1080P_50:
        {
            *penHdFmt = HI_UNF_ENC_FMT_1080P_50;
            *penSdFmt = HI_UNF_ENC_FMT_PAL; 
            break;
        }
        case HI_UNF_ENC_FMT_1080i_60:
        {
            *penHdFmt = HI_UNF_ENC_FMT_1080i_60;
            *penSdFmt = HI_UNF_ENC_FMT_NTSC;
            break;
        }
        case HI_UNF_ENC_FMT_1080i_50:
        {
            *penHdFmt = HI_UNF_ENC_FMT_1080i_50;
            *penSdFmt = HI_UNF_ENC_FMT_PAL; 
            break;
        }
        case HI_UNF_ENC_FMT_720P_60:
        {
            *penHdFmt = HI_UNF_ENC_FMT_720P_60;
            *penSdFmt = HI_UNF_ENC_FMT_NTSC;
            break;
        }
        case HI_UNF_ENC_FMT_720P_50:
        {
            *penHdFmt = HI_UNF_ENC_FMT_720P_50;
            *penSdFmt = HI_UNF_ENC_FMT_PAL; 
            break;
        } 
        case HI_UNF_ENC_FMT_576P_50:
        {
            *penHdFmt = HI_UNF_ENC_FMT_576P_50;
            *penSdFmt = HI_UNF_ENC_FMT_PAL; 
            break;        
        }
        case HI_UNF_ENC_FMT_480P_60:
        {
            *penHdFmt = HI_UNF_ENC_FMT_480P_60;
            *penSdFmt = HI_UNF_ENC_FMT_NTSC; 
            break;         
        }
        case HI_UNF_ENC_FMT_PAL:
        {
            *penHdFmt = HI_UNF_ENC_FMT_PAL;
            *penSdFmt = HI_UNF_ENC_FMT_PAL;
            break;
        }
        case HI_UNF_ENC_FMT_NTSC:
        {
            *penHdFmt = HI_UNF_ENC_FMT_NTSC;
            *penSdFmt = HI_UNF_ENC_FMT_NTSC; 
            break;
        } 
        
        /* bellow are vga display formats */
        case HI_UNF_ENC_FMT_861D_640X480_60:
        {
            *penHdFmt = HI_UNF_ENC_FMT_861D_640X480_60;
            *penSdFmt = HI_UNF_ENC_FMT_NTSC; 
            break;
        }
        case HI_UNF_ENC_FMT_VESA_800X600_60:
        {
            *penHdFmt = HI_UNF_ENC_FMT_VESA_800X600_60;
            *penSdFmt = HI_UNF_ENC_FMT_NTSC; 
            break;
        }
        case HI_UNF_ENC_FMT_VESA_1024X768_60:
        {
            *penHdFmt = HI_UNF_ENC_FMT_VESA_1024X768_60;
            *penSdFmt = HI_UNF_ENC_FMT_NTSC; 
            break;
        }
        case HI_UNF_ENC_FMT_VESA_1280X720_60:
        {
            *penHdFmt = HI_UNF_ENC_FMT_VESA_1280X720_60;
            *penSdFmt = HI_UNF_ENC_FMT_NTSC; 
            break;
        }
        case HI_UNF_ENC_FMT_VESA_1280X800_60:
        {
            *penHdFmt = HI_UNF_ENC_FMT_VESA_1280X800_60;
            *penSdFmt = HI_UNF_ENC_FMT_NTSC; 
            break;
        }
        case HI_UNF_ENC_FMT_VESA_1280X1024_60:
        {
            *penHdFmt = HI_UNF_ENC_FMT_VESA_1280X1024_60;
            *penSdFmt = HI_UNF_ENC_FMT_NTSC;  
            break;
        }
        case HI_UNF_ENC_FMT_VESA_1360X768_60:
        {
            *penHdFmt = HI_UNF_ENC_FMT_VESA_1360X768_60;
            *penSdFmt = HI_UNF_ENC_FMT_NTSC;
            break;
        }
        case HI_UNF_ENC_FMT_VESA_1366X768_60:
        {
            *penHdFmt = HI_UNF_ENC_FMT_VESA_1366X768_60;
            *penSdFmt = HI_UNF_ENC_FMT_NTSC;   
            break;
        }
        case HI_UNF_ENC_FMT_VESA_1400X1050_60:
        {
            *penHdFmt = HI_UNF_ENC_FMT_VESA_1400X1050_60;
            *penSdFmt = HI_UNF_ENC_FMT_NTSC;  
            break;
        }
        case HI_UNF_ENC_FMT_VESA_1440X900_60:
        {
            *penHdFmt = HI_UNF_ENC_FMT_VESA_1440X900_60;
            *penSdFmt = HI_UNF_ENC_FMT_NTSC;    
            break;
        }
        case HI_UNF_ENC_FMT_VESA_1440X900_60_RB:
        {
            *penHdFmt = HI_UNF_ENC_FMT_VESA_1440X900_60_RB;
            *penSdFmt = HI_UNF_ENC_FMT_NTSC;  
            break;
        }
        case HI_UNF_ENC_FMT_VESA_1600X900_60_RB:
        {
            *penHdFmt = HI_UNF_ENC_FMT_VESA_1600X900_60_RB;
            *penSdFmt = HI_UNF_ENC_FMT_NTSC; 
            break;
        }
        case HI_UNF_ENC_FMT_VESA_1600X1200_60:
        {
            *penHdFmt = HI_UNF_ENC_FMT_VESA_1600X1200_60;
            *penSdFmt = HI_UNF_ENC_FMT_NTSC;   
            break;
        }
        case HI_UNF_ENC_FMT_VESA_1680X1050_60:
        {
            *penHdFmt = HI_UNF_ENC_FMT_VESA_1680X1050_60;
            *penSdFmt = HI_UNF_ENC_FMT_NTSC;  
            break;
        } 
        
        case HI_UNF_ENC_FMT_VESA_1920X1080_60:
        {
            *penHdFmt = HI_UNF_ENC_FMT_VESA_1920X1080_60;
            *penSdFmt = HI_UNF_ENC_FMT_NTSC;   
            break;
        }
        case HI_UNF_ENC_FMT_VESA_1920X1200_60:
        {
            *penHdFmt = HI_UNF_ENC_FMT_VESA_1920X1200_60;
            *penSdFmt = HI_UNF_ENC_FMT_NTSC;            
            break;
        }
        case HI_UNF_ENC_FMT_VESA_2048X1152_60:
        {
            *penHdFmt = HI_UNF_ENC_FMT_VESA_2048X1152_60;
            *penSdFmt = HI_UNF_ENC_FMT_NTSC;            
            break;
        }
        default:
        {
            *penHdFmt = HI_UNF_ENC_FMT_1080i_50;
            *penSdFmt = HI_UNF_ENC_FMT_PAL;
            break;
        }
    }

    return;
}

HI_S32 DRV_PDM_GetDispParam(HI_UNF_DISP_E enDisp, HI_DISP_PARAM_S *pstDispParam)
{
    HI_S32                      Ret;
    HI_DB_S                     stBaseDB;
    HI_DB_TABLE_S               stTable;
    HI_DB_KEY_S                 stKey;
    HI_U32                      u32BasePhyAddr;
    HI_U32                      u32BaseVirAddr;
    HI_U32                      u32BaseLen;
    HI_UNF_ENC_FMT_E            enHdFmt, enSdFmt;

    Ret = PDM_GetBufByName(PDM_BASEPARAM_BUFNAME, &u32BasePhyAddr, &u32BaseLen);
    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }
    
    u32BaseVirAddr = (HI_U32)phys_to_virt(u32BasePhyAddr);

    PDM_GetDefDispParam(enDisp, pstDispParam);

    Ret = HI_DB_GetDBFromMem((HI_U8 *)u32BaseVirAddr, &stBaseDB);
    if(HI_SUCCESS != Ret)
    {
        HI_INFO_PDM("ERR: HI_DB_GetDBFromMem, use default baseparam!\n");
        return HI_SUCCESS;
    }
    
    Ret = HI_DB_GetTableByName(&stBaseDB, MCE_BASE_TABLENAME_HD0, &stTable);
    if(HI_SUCCESS != Ret)
    {
        HI_INFO_PDM("ERR: HI_DB_GetTableByName, use default baseparam!\n");
        return HI_SUCCESS;
    }
    
    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_FMT, &stKey);
    if(HI_SUCCESS == Ret)
    {
        PDM_TransFomat(*(HI_UNF_ENC_FMT_E *)(stKey.pValue), &enHdFmt, &enSdFmt);
        
        if (HI_UNF_DISPLAY0 == enDisp)
        {
            pstDispParam->enFormat = enSdFmt;
        }

        if (HI_UNF_DISPLAY1 == enDisp)
        {
            pstDispParam->enFormat = enHdFmt;
        }

    }
    
    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_INTF_YPBPR, &stKey);
    if(HI_SUCCESS == Ret && HI_UNF_DISPLAY1 == enDisp)
    {
        pstDispParam->stIntf[HI_UNF_DISP_INTF_TYPE_YPBPR] = *(HI_UNF_DISP_INTF_S *)(stKey.pValue);
    }
    
    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_INTF_CVBS, &stKey);
    if(HI_SUCCESS == Ret && HI_UNF_DISPLAY0 == enDisp)
    {
        pstDispParam->stIntf[HI_UNF_DISP_INTF_TYPE_CVBS] = *(HI_UNF_DISP_INTF_S *)(stKey.pValue);
    }

    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_INTF_RGB, &stKey);
    if(HI_SUCCESS == Ret && HI_UNF_DISPLAY1 == enDisp)
    {
        pstDispParam->stIntf[HI_UNF_DISP_INTF_TYPE_RGB] = *(HI_UNF_DISP_INTF_S *)(stKey.pValue);
    }

/*
    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_INTF_SVIDEO, &stKey);
    if(HI_SUCCESS == Ret)
    {
        pstDispParam->stIntf[HI_UNF_DISP_INTF_TYPE_SVIDEO] = *(HI_UNF_DISP_INTF_S *)(stKey.pValue);
    }
*/

    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_HULEP, &stKey);
    if(HI_SUCCESS == Ret)
    {
        pstDispParam->u32HuePlus = *(HI_U32 *)(stKey.pValue);
    }

    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_SATU, &stKey);
    if(HI_SUCCESS == Ret)
    {
        pstDispParam->u32Saturation = *(HI_U32 *)(stKey.pValue);
    }

    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_CONTR, &stKey);
    if(HI_SUCCESS == Ret)
    {
        pstDispParam->u32Contrast = *(HI_U32 *)(stKey.pValue);
    }

    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_BRIG, &stKey);
    if(HI_SUCCESS == Ret)
    {
        pstDispParam->u32Brightness = *(HI_U32 *)(stKey.pValue);
    }

    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_BGCOLOR, &stKey);
    if(HI_SUCCESS == Ret)
    {
        pstDispParam->stBgColor = *(HI_UNF_DISP_BG_COLOR_S *)(stKey.pValue);
    }

    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_TIMING, &stKey);
    if(HI_SUCCESS == Ret)
    {
        pstDispParam->stDispTiming = *(HI_UNF_DISP_TIMING_S *)(stKey.pValue);
    }

    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_GAMA, &stKey);
    if(HI_SUCCESS == Ret)
    {
        pstDispParam->bGammaEnable = *(HI_BOOL *)(stKey.pValue);
    }

    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_SCRX, &stKey);
    if(HI_SUCCESS == Ret)
    {
        pstDispParam->u32ScreenXpos = *(HI_U32 *)(stKey.pValue);
    }

    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_SCRY, &stKey);
    if(HI_SUCCESS == Ret)
    {
        pstDispParam->u32ScreenYpos = *(HI_U32 *)(stKey.pValue);
    }
    
    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_SCRW, &stKey);
    if(HI_SUCCESS == Ret)
    {
        pstDispParam->u32ScreenWidth = *(HI_U32 *)(stKey.pValue);
    }

    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_SCRH, &stKey);
    if(HI_SUCCESS == Ret)
    {
        pstDispParam->u32ScreenHeight = *(HI_U32 *)(stKey.pValue);
    }

    return HI_SUCCESS;
}

HI_S32 DRV_PDM_GetGrcParam(HI_GRC_PARAM_S *pstGrcParam)
{
    HI_S32                      Ret;
    HI_GRC_PARAM_S              stDefGrcParam = {0};
    HI_DB_S                     stBaseDB;
    HI_DB_TABLE_S               stTable;
    HI_DB_KEY_S                 stKey;
    HI_U32                      u32BasePhyAddr;
    HI_U32                      u32BaseVirAddr;
    HI_U32                      u32BaseLen;

    Ret = PDM_GetBufByName(PDM_BASEPARAM_BUFNAME, &u32BasePhyAddr, &u32BaseLen);
    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }
    
    u32BaseVirAddr = (HI_U32)phys_to_virt(u32BasePhyAddr);

    PDM_GetDefGrcParam(&stDefGrcParam);

    Ret = HI_DB_GetDBFromMem((HI_U8 *)u32BaseVirAddr, &stBaseDB);
    if(HI_SUCCESS != Ret)
    {
        HI_INFO_PDM("ERR: HI_DB_GetDBFromMem, use def param!\n");
        *pstGrcParam = stDefGrcParam;
        return HI_SUCCESS;
    }
    
    Ret = HI_DB_GetTableByName(&stBaseDB, MCE_BASE_TABLENAME_HD0, &stTable);
    if(HI_SUCCESS != Ret)
    {
        HI_INFO_PDM("ERR: HI_DB_GetTableByName, use def param!\n");
        *pstGrcParam = stDefGrcParam;
        return HI_SUCCESS;
    }

    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_PF, &stKey);
    if(HI_SUCCESS == Ret)
    {
        pstGrcParam->enPixelFormat = *(HIGO_PF_E *)(stKey.pValue);
    }
    else
    {
        pstGrcParam->enPixelFormat = stDefGrcParam.enPixelFormat;
    }

    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_DISPW, &stKey);
    if(HI_SUCCESS == Ret)
    {
        pstGrcParam->u32DisplayWidth = *(HI_U32 *)(stKey.pValue);
    }
    else
    {
        pstGrcParam->u32DisplayWidth = stDefGrcParam.u32DisplayWidth;
    }   

    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_DISPH, &stKey);
    if(HI_SUCCESS == Ret)
    {
        pstGrcParam->u32DisplayHeight = *(HI_U32 *)(stKey.pValue);
    }
    else
    {
        pstGrcParam->u32DisplayHeight = stDefGrcParam.u32DisplayHeight;
    }    
    
    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_SCRX, &stKey);
    if(HI_SUCCESS == Ret)
    {
        pstGrcParam->u32ScreenXpos = *(HI_U32 *)(stKey.pValue);
    }
    else
    {
        pstGrcParam->u32ScreenXpos = stDefGrcParam.u32ScreenXpos;
    }   

    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_SCRY, &stKey);
    if(HI_SUCCESS == Ret)
    {
        pstGrcParam->u32ScreenYpos = *(HI_U32 *)(stKey.pValue);
    }
    else
    {
        pstGrcParam->u32ScreenYpos = stDefGrcParam.u32ScreenYpos;
    }   

    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_SCRW, &stKey);
    if(HI_SUCCESS == Ret)
    {
        pstGrcParam->u32ScreenWidth = *(HI_U32 *)(stKey.pValue);
    }
    else
    {
        pstGrcParam->u32ScreenWidth = stDefGrcParam.u32ScreenWidth;
    }   

    Ret = HI_DB_GetKeyByName(&stTable, MCE_BASE_KEYNAME_SCRH, &stKey);
    if(HI_SUCCESS == Ret)
    {
        pstGrcParam->u32ScreenHeight = *(HI_U32 *)(stKey.pValue);
    }
    else
    {
        pstGrcParam->u32ScreenHeight = stDefGrcParam.u32ScreenHeight;
    } 
    
    return HI_SUCCESS;

}

HI_S32 DRV_PDM_GetMceParam(HI_MCE_PARAM_S *pstMceParam)
{
    HI_S32                      Ret;
    HI_DB_S                     stBaseDB;
    HI_DB_TABLE_S               stTable;
    HI_DB_KEY_S                 stKey;
    HI_U32                      u32MceParaPhyAddr;
    HI_U32                      u32MceParaVirAddr;
    HI_U32                      u32MceParaLen;

    Ret = PDM_GetBufByName(PDM_PLAYPARAM_BUFNAME, &u32MceParaPhyAddr, &u32MceParaLen);
    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }
    
    u32MceParaVirAddr = (HI_U32)phys_to_virt(u32MceParaPhyAddr);


    Ret = HI_DB_GetDBFromMem((HI_U8 *)u32MceParaVirAddr, &stBaseDB);
    if(HI_SUCCESS != Ret)
    {
        HI_INFO_PDM("ERR: HI_DB_GetDBFromMem!\n");
        return HI_FAILURE;
    }

    Ret = HI_DB_GetTableByName(&stBaseDB, MCE_PLAY_TABLENAME, &stTable);
    if(HI_SUCCESS != Ret)
    {
        HI_INFO_PDM("ERR: HI_DB_GetTableByName!\n");
        return HI_FAILURE;        
    }

    Ret = HI_DB_GetKeyByName(&stTable, MCE_PLAY_KEYNAME_FLAG, &stKey);
    if(HI_SUCCESS == Ret)
    {
        pstMceParam->u32CheckFlag = *(HI_U32 *)(stKey.pValue);
    }
    else
    {
        pstMceParam->u32CheckFlag = 0;
    }    

    Ret = HI_DB_GetKeyByName(&stTable, MCE_PLAY_KEYNAME_DATALEN, &stKey);
    if(HI_SUCCESS == Ret)
    {
        pstMceParam->u32PlayDataLen = *(HI_U32 *)(stKey.pValue);
    }
    else
    {
        pstMceParam->u32PlayDataLen = 0;
    }

    Ret = HI_DB_GetKeyByName(&stTable, MCE_PLAY_KEYNAME_PARAM, &stKey);
    if(HI_SUCCESS == Ret)
    {
        pstMceParam->stPlayParam = *(HI_UNF_MCE_PLAY_PARAM_S *)(stKey.pValue);
    }
    else
    {
        memset(&(pstMceParam->stPlayParam), 0x00, sizeof(HI_UNF_MCE_PLAY_PARAM_S));
    }

    return HI_SUCCESS;

}

HI_S32 DRV_PDM_GetMceData(HI_U32 u32Size, HI_U32 *pAddr)
{
    HI_S32                      Ret;
    HI_U32                      u32MceDataPhyAddr;
    HI_U32                      u32MceDataLen;

    Ret = PDM_GetBufByName(PDM_PLAYDATA_BUFNAME, &u32MceDataPhyAddr, &u32MceDataLen);
    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }
    
    *pAddr = (HI_U32)phys_to_virt(u32MceDataPhyAddr);

    return HI_SUCCESS;
}


/*release memory*/
static HI_S32 PDM_FreeMem(HI_U32 PhyAddr, HI_U32 Len)
{
    HI_U32      pfn_start;
    HI_U32      pfn_end;
    HI_U32      pages = 0;

    pfn_start = __phys_to_pfn(PhyAddr);
    pfn_end = __phys_to_pfn(PhyAddr + Len);

    for (; pfn_start < pfn_end; pfn_start++)
    {
		struct page *page = pfn_to_page(pfn_start);
		ClearPageReserved(page);
		init_page_count(page);
		__free_page(page);
		pages++;
    }

    return HI_SUCCESS;
}

HI_S32 DRV_PDM_ReleaseReserveMem(const HI_CHAR *BufName)
{
    HI_S32                      Ret;
    HI_U32                      PhyAddr;
    HI_U32                      Len;
    HI_U32                      i;    

    for (i = 0; i < g_PdmGlobal.ReleaseBufNum; i++)
    {
        if (0 == strncmp(g_PdmGlobal.ReleaseBufName[i], BufName, strlen(BufName)))
        {
            return HI_SUCCESS;
        }
    }
    
    Ret = PDM_GetBufByName(BufName, &PhyAddr, &Len);
    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }

    PDM_FreeMem(PhyAddr, Len);

    memset(g_PdmGlobal.ReleaseBufName[g_PdmGlobal.ReleaseBufNum], 0x0, strlen(g_PdmGlobal.ReleaseBufName[g_PdmGlobal.ReleaseBufNum]));
    strncpy(g_PdmGlobal.ReleaseBufName[g_PdmGlobal.ReleaseBufNum], BufName, strlen(BufName));

    g_PdmGlobal.ReleaseBufNum++;
    
    return HI_SUCCESS;
}


HI_S32 HI_DRV_PDM_GetDispParam(HI_UNF_DISP_E enDisp, HI_DISP_PARAM_S *pstDispParam)
{
    return DRV_PDM_GetDispParam(enDisp, pstDispParam);
}

HI_S32 HI_DRV_PDM_GetGrcParam(HI_GRC_PARAM_S *pGrcParam)
{
    return DRV_PDM_GetGrcParam(pGrcParam);
}

HI_S32 HI_DRV_PDM_GetMceParam(HI_MCE_PARAM_S *pMceParam)
{
    return DRV_PDM_GetMceParam(pMceParam);
}

HI_S32 HI_DRV_PDM_GetMceData(HI_U32 u32Size, HI_U32 *pAddr)
{
    return DRV_PDM_GetMceData(u32Size, pAddr);
}

HI_S32 HI_DRV_PDM_ReleaseReserveMem(const HI_CHAR *BufName)
{
    return DRV_PDM_ReleaseReserveMem(BufName);
}



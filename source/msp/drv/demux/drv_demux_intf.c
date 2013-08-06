/*******************************************************************************
 *              Copyright 2004 - 2050, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: drv_demux_intf.c
 * Description: demux interface of module.
 *
 * History:
 * Version   Date         Author     DefectNum    Description
 * main\1    2010-01-25   j40671     NULL         init.
 ********************************************************************************/

/***************************** included files ******************************/
#include "hi_type.h"
#include "drv_struct_ext.h"
#include "drv_dev_ext.h"
#include "drv_proc_ext.h"
#include "drv_stat_ext.h"
#include "hi_module.h"
#include "hi_kernel_adapt.h"

#include "demux_debug.h"
#include "hi_mpi_demux.h"
#include "hi_drv_demux.h"

#include "drv_demux.h"
#include "drv_demux_ioctl.h"
#include "drv_demux_ext.h"
#include "drv_demux_func.h"

#ifdef DMX_DESCRAMBLER_SUPPORT
#include "drv_descrambler.h"
#include "drv_descrambler_func.h"
#endif

/**************************** global variables ****************************/
static UMAP_DEVICE_S g_stDemuxDev;

#ifdef HI_DEMUX_PROC_SUPPORT
/****************************** internal function *****************************/
char* SAVE_ES_STR = "save es";
char* SAVE_ALLTS_STR = "save allts";
char* SAVE_IPTS_STR = "save ipts";
char* SAVE_DMXTS_STR = "save dmxts";
char* HELP_STR = "help";
char* START_STR = "start";
char* STOP_STR = "stop";

static HI_VOID DMXDebugShowHelp(HI_VOID)
{
    printk("echo %s %s > /proc/msp/demux_main  -- begin save es\n",SAVE_ES_STR,START_STR);
    printk("echo %s %s > /proc/msp/demux_main  -- stop save es\n",SAVE_ES_STR,STOP_STR);
    printk("echo %s %s x[portid] > /proc/msp/demux_main  -- begin save allts\n",SAVE_ALLTS_STR,START_STR);
    printk("echo %s %s > /proc/msp/demux_main  -- stop save allts\n",SAVE_ALLTS_STR,STOP_STR);
    printk("echo %s %s x[ram portid]> /proc/msp/demux_main  -- begin save ram port ts\n",SAVE_IPTS_STR,START_STR);
    printk("echo %s %s > /proc/msp/demux_main  -- stop save ram port ts\n",SAVE_IPTS_STR,STOP_STR);
    printk("echo %s %s x[dmxid] > /proc/msp/demux_main  -- begin save all channels ts\n",SAVE_DMXTS_STR,START_STR);
    printk("echo %s %s > /proc/msp/demux_main  -- stop save dmx ts\n",SAVE_DMXTS_STR,STOP_STR);
    printk("echo %s > /proc/msp/demux_main  -- show help info\n",HELP_STR);
}

#define STRSKIPBLANK(str)      \
        while (str[0] == ' ')\
        {\
        (str)++;\
        }\

static HI_S32 DMXProcWrite(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
    HI_U32 cmd = 0xff;
    HI_U32 param = 0;
    HI_U32 needparam = 0;
    DMX_DEBUG_CMD_CTRl cmdctrl;
    char *p1;
    char *p2;
    HI_S32 ret;

    p1 = (char *)__get_free_page(GFP_KERNEL);
    if (copy_from_user(p1, buf, count))
    {
        HI_ERR_DEMUX("copy from user failed\n");
        return (-1);
    }
    p2 = p1;
    STRSKIPBLANK(p1);
    if (strstr(p1,SAVE_ES_STR) && p1[0] == 's')
    {
        cmd = DMX_DEBUG_CMD_SAVE_ES;
        p1 += strlen(SAVE_ES_STR);       
    }
    else if (strstr(p1,SAVE_ALLTS_STR) && p1[0] == 's')
    {
        cmd = DMX_DEBUG_CMD_SAVE_ALLTS;
        p1 += strlen(SAVE_ALLTS_STR);
        needparam = 1;
    }
    else if (strstr(p1,SAVE_IPTS_STR) && p1[0] == 's')
    {
        cmd = DMX_DEBUG_CMD_SAVE_IPTS;
        p1 += strlen(SAVE_IPTS_STR);
        needparam = 1;
    }
    else if (strstr(p1,SAVE_DMXTS_STR) && p1[0] == 's')
    {
        cmd = DMX_DEBUG_CMD_SAVE_DMXTS;
        p1 += strlen(SAVE_DMXTS_STR);
        needparam = 1;
    }
    else if (strstr(p1,HELP_STR) && p1[0] == 'h')
    {
        DMXDebugShowHelp();
        return count;
    }
    else
    {
        printk("unknow command:%s\n",p1);
        DMXDebugShowHelp();
        return (-1);
    }
    STRSKIPBLANK(p1);
    if (strstr(p1,START_STR))
    {
        cmdctrl = DMX_DEBUG_CMD_START;
        p1 += strlen(START_STR);
    }
    else if (strstr(p1,STOP_STR))
    {
        cmdctrl = DMX_DEBUG_CMD_STOP;
        p1 += strlen(STOP_STR);
        needparam = 0;
    }
    else
    {
        printk("command is not correct:%s\n",p2);
        DMXDebugShowHelp();
        return (-1);
    }
    if (needparam)
    {
        STRSKIPBLANK(p1);       
        if (p1[0] == '\n')//do not have param
        {
            printk("command is not correct:%s\n",p2);
            DMXDebugShowHelp();
            return (-1);
        }
        param = (HI_U32)simple_strtoul(p1, &p1, 10);
    }

    ret = DMX_OsrDebugCtrl(cmd,cmdctrl,param);
    if (ret != HI_SUCCESS)
    {
        printk("command is not correct:%s\n",p2);
        DMXDebugShowHelp();
        return ret;
    }

    return count;
}

static HI_S32 DMXProcRead(struct seq_file *p, HI_VOID *v)
{
    HI_U32 DmxId;

    seq_printf(p, "DmxId\tPortId\n");

    for (DmxId = 0; DmxId < DMX_CNT; DmxId++)
    {
        HI_S32              ret;
        HI_CHAR             PortStr[8] = "--";
        DMX_PORT_MODE_E     PortMode;
        HI_U32              PortId;

        ret = HI_DRV_DMX_GetPortId(DmxId, &PortMode, &PortId);
        if (HI_SUCCESS == ret)
        {
            sprintf(PortStr, "%u", (DMX_PORT_MODE_TUNER == PortMode) ? PortId : HI_UNF_DMX_PORT_RAM_0 + PortId);
        }

        seq_printf(p, "  %u\t%s\n", DmxId, PortStr);
    }

    return HI_SUCCESS;
}

static HI_VOID PortTypeToString(HI_CHAR *str, HI_U32 Type)
{
    switch (Type)
    {
        case HI_UNF_DMX_PORT_TYPE_PARALLEL_BURST :
            strcpy(str, "BST");
            break;

        case HI_UNF_DMX_PORT_TYPE_PARALLEL_VALID :
            strcpy(str, "VLD");
            break;

        case HI_UNF_DMX_PORT_TYPE_PARALLEL_NOSYNC_204 :
            strcpy(str, "204");
            break;

        case HI_UNF_DMX_PORT_TYPE_PARALLEL_NOSYNC_188_204 :
            strcpy(str, "188/204");
            break;

        case HI_UNF_DMX_PORT_TYPE_SERIAL :
            strcpy(str, "SER");
            break;

        case HI_UNF_DMX_PORT_TYPE_SERIAL2BIT :
            strcpy(str, "SER_2BIT");
            break;

        case HI_UNF_DMX_PORT_TYPE_SERIAL_NOSYNC :
            strcpy(str, "SER_NOSYNC");
            break;

        case HI_UNF_DMX_PORT_TYPE_SERIAL2BIT_NOSYNC :
            strcpy(str, "SER_2BIT_NOSYNC");
            break;

        case HI_UNF_DMX_PORT_TYPE_USER_DEFINED :
            strcpy(str, "USR");
            break;

        case HI_UNF_DMX_PORT_TYPE_AUTO :
            strcpy(str, "AUTO");
            break;

        case HI_UNF_DMX_PORT_TYPE_PARALLEL_NOSYNC_188 :
        default :
            strcpy(str, "188");
    }
}

static HI_S32 DMXPortProcRead(struct seq_file *p, HI_VOID *v)
{
    HI_U32                  i;
    HI_U32                  TsPacks;
    HI_U32                  ErrTsPacks;
    HI_UNF_DMX_PORT_ATTR_S  PortAttr;

    seq_printf(p, " Id  AllTsCnt   ErrTsCnt  Lock/lost  Type\n");

    for (i = 0; i < DMX_TUNERPORT_CNT; i++)
    {
        HI_CHAR str[16] = "";

        HI_DRV_DMX_TunerPortGetAttr(i, &PortAttr);

        HI_DRV_DMX_TunerPortGetPacketNum(i, &TsPacks, &ErrTsPacks);

        PortTypeToString(str, PortAttr.enPortType);

        seq_printf(p, "%3u 0x%-8x   0x%-4x      %u/%u     %s\n",
            i, TsPacks, ErrTsPacks, PortAttr.u32SyncLockTh, PortAttr.u32SyncLostTh, str);
    }

    for (i = 0; i < DMX_RAMPORT_CNT; i++)
    {
        HI_CHAR str[16] = "";

        HI_DRV_DMX_RamPortGetAttr(i, &PortAttr);

        HI_DRV_DMX_RamPortGetPacketNum(i, &TsPacks);

        PortTypeToString(str, PortAttr.enPortType);

        seq_printf(p, "%3u 0x%-8x   0x0         %u/%u     %s\n",
            HI_UNF_DMX_PORT_RAM_0 + i, TsPacks, PortAttr.u32SyncLockTh, PortAttr.u32SyncLostTh, str);
    }

    return HI_SUCCESS;
}

static HI_S32 DMXTsBufProcRead(struct seq_file *p, HI_VOID *v)
{
    HI_U32  PortId;

    seq_printf(p, "Id  BufAddr    BufSize    BufUsed    Read       Write           Get(Try/Ok)      Put\n");

    for (PortId = 0; PortId < DMX_RAMPORT_CNT; PortId++)
    {
        DMX_Proc_RamPort_BufInfo_S BufInfo;

        if (HI_SUCCESS != DMX_OsiRamPortGetBufInfo(PortId, &BufInfo))
        {
            continue;
        }

        seq_printf(p, "%u 0x%-8x 0x%-8x 0x%-8x 0x%-8x 0x%-8x %10u/%-10u %u\n",
                HI_UNF_DMX_PORT_RAM_0 + PortId,
                BufInfo.PhyAddr,
                BufInfo.BufSize,
                BufInfo.UsedSize,
                BufInfo.Read,
                BufInfo.Write,
                BufInfo.GetCount,
                BufInfo.GetValidCount,
                BufInfo.PutCount
            );
    }

    return HI_SUCCESS;
}

static HI_S32 DMXChanProcRead(struct seq_file *p, HI_VOID *v)
{
    HI_U32  ChanId;

    seq_printf(p, "Id DmxId  PID\tType Mod Stat  KeyId    Acquire(Try/Ok)    Release\n");

    for (ChanId = 0; ChanId < DMX_CHANNEL_CNT; ChanId++)
    {
        DMX_ChanInfo_S *ChanInfo;
        HI_CHAR         ChanType[][8]   = {"SEC", "PES", "AUD", "VID", "PST", "ECM"};
        HI_CHAR         OutMode[][8]    = {"PLY", "REC", "P&R"};
        HI_CHAR         KeyStr[8]       = "--";

        ChanInfo = DMX_OsiGetChannelProc(ChanId);
        if (!ChanInfo)
        {
            continue;
        }

        if (ChanInfo->KeyId < DMX_KEY_CNT)
        {
            sprintf(KeyStr, "%-2u", ChanInfo->KeyId);
        }

        seq_printf(p, "%-2u   %u   0x%x\t%s  %s %s\t%s   %10u/%-10u %u\n",
                        ChanId,
                        ChanInfo->DmxId,
                        ChanInfo->ChanPid,
                        ChanType[ChanInfo->ChanType],
                        OutMode[ChanInfo->ChanOutMode - 1],
                        (ChanInfo->ChanStatus == HI_UNF_DMX_CHAN_CLOSE)  ? "CLOSE" : "OPEN",
                        KeyStr,
                        ChanInfo->u32TotolAcq,
                        ChanInfo->u32HitAcq,
                        ChanInfo->u32Release
             );
    }

    return HI_SUCCESS;
}

static HI_S32 DMXChanBufProcRead(struct seq_file *p, HI_VOID *v)
{
    HI_U32 i;

    seq_printf(p, "Id  Size  BlkCnt BlkSize Read Write Used Overflow\n");

    for (i = 0; i < DMX_CHANNEL_CNT; i++)
    {
        DMX_Proc_ChanBuf_S  BufInfo;
        HI_U32              UsedPercent = 0;
        HI_U32              Size;

        if (HI_SUCCESS != DMX_OsiGetChanBufProc(i, &BufInfo))
        {
            continue;
        }

        if (BufInfo.DescDepth)
        {
            if (BufInfo.DescRead <= BufInfo.DescWrite)
            {
                UsedPercent = (BufInfo.DescWrite - BufInfo.DescRead) * 100 / BufInfo.DescDepth;
            }
            else
            {
                UsedPercent = (BufInfo.DescDepth + BufInfo.DescWrite - BufInfo.DescRead) * 100 / BufInfo.DescDepth;
            }
        }

        Size = BufInfo.DescDepth * BufInfo.BlockSize / 1024;

        seq_printf(p, "%2u %5uK %-6u %-7u %-4u %-5u %3u%% %u\n",
            i, Size, BufInfo.DescDepth, BufInfo.BlockSize, BufInfo.DescRead, BufInfo.DescWrite, UsedPercent, BufInfo.Overflow);
    }

    return HI_SUCCESS;
}

static HI_S32 DMXFilterProcRead(struct seq_file *p, HI_VOID *v)
{
    HI_U32 FilterId;
    HI_U32 i;

    seq_printf(p, "Id ChanId Depth Param\n");

    for (FilterId = 0; FilterId < DMX_FILTER_CNT; FilterId++)
    {
        DMX_FilterInfo_S   *FilterInfo;
        HI_CHAR             str[16] = "--";

        FilterInfo = DMX_OsiGetFilterProc(FilterId);
        if (!FilterInfo)
        {
            continue;
        }

        if (FilterInfo->ChanId < DMX_CHANNEL_CNT)
        {
            sprintf(str, "%2u", FilterInfo->ChanId);
        }

        seq_printf(p, "%2u   %s   %3u   Match :", FilterId, str, FilterInfo->Depth);

        for (i = 0; i < FilterInfo->Depth; i++)
        {
            seq_printf(p, " %02x", FilterInfo->Match[i]);
        }

        seq_printf(p, "\n\t\tMask  :");
        for (i = 0; i < FilterInfo->Depth; i++)
        {
            seq_printf(p, " %02x", FilterInfo->Mask[i]);
        }

        seq_printf(p, "\n\t\tNegate:");
        for (i = 0; i < FilterInfo->Depth; i++)
        {
            seq_printf(p, " %02x", FilterInfo->Negate[i]);
        }

        seq_printf(p, "\n");
    }

    return 0;
}

static HI_S32 DMXPcrProcRead(struct seq_file *p, HI_VOID *v)
{
    HI_U32 PcrId;

    seq_printf(p, "Id DmxId PID    CurrPcr     CurrScr\n");

    for (PcrId = 0; PcrId < DMX_PCR_CHANNEL_CNT; PcrId++)
    {
        DMX_PCR_Info_S *PcrInfo;

        PcrInfo = DMX_OsiGetPcrChannelProc(PcrId);
        if (!PcrInfo)
        {
            continue;
        }

        seq_printf(p, "%-2u   %u   0x%-4x 0x%-9llx 0x%-9llx\n",
            PcrId, PcrInfo->DmxId, PcrInfo->PcrPid, PcrInfo->PcrValue, PcrInfo->ScrValue);
    }

    return HI_SUCCESS;
}

static HI_S32 DMXRecProcRead(struct seq_file *p, HI_VOID *v)
{
    HI_U32 RecId;

    seq_printf(p, "DmxId Type Descramed Status  Size  BlkCnt BlkSize Read Write Overflow\n");

    for (RecId = 0; RecId < DMX_CNT; RecId++)
    {
        DMX_Proc_Rec_BufInfo_S  RecInfo;
        HI_CHAR                *Type;
        HI_CHAR                *Status;
        HI_U32                  Size;

        if (HI_SUCCESS != DMX_OsiGetDmxRecProc(RecId, &RecInfo))
        {
            continue;
        }

        switch (RecInfo.RecType)
        {
            case HI_UNF_DMX_REC_TYPE_SELECT_PID :
                Type = "pid";
                break;

            case HI_UNF_DMX_REC_TYPE_ALL_PID :
                Type = "all";
                break;

            default :
                continue;
        }

        Size    = RecInfo.BlockCnt * RecInfo.BlockSize / 1024;
        Status  = RecInfo.RecStatus ? "start" : "stop ";

        seq_printf(p, "%3u   %s      %u     %s  %5uK %-6u %-7u %-4u %-5u %u\n",
            RecId, Type, RecInfo.Descramed, Status, Size, RecInfo.BlockCnt,
            RecInfo.BlockSize, RecInfo.BufRead, RecInfo.BufWrite, RecInfo.Overflow);
    }

    return HI_SUCCESS;
}

static HI_S32 DMXRecScdProcRead(struct seq_file *p, HI_VOID *v)
{
    HI_U32 RecId;

    seq_printf(p, "DmxId Type  Pid    Size BlkCnt BlkSize Read Write Overflow\n");

    for (RecId = 0; RecId < DMX_CNT; RecId++)
    {
        DMX_Proc_RecScd_BufInfo_S   ScdInfo;
        HI_CHAR                    *Type;
        HI_U32                      Size;

        if (HI_SUCCESS != DMX_OsiGetDmxRecScdProc(RecId, &ScdInfo))
        {
            continue;
        }

        switch (ScdInfo.IndexType)
        {
            case HI_UNF_DMX_REC_INDEX_TYPE_VIDEO :
                Type = "video";
                break;

            case HI_UNF_DMX_REC_INDEX_TYPE_AUDIO :
                Type = "audio";
                break;

            case HI_UNF_DMX_REC_INDEX_TYPE_NONE :
            default :
                Type = "none ";
        }

        Size = ScdInfo.BlockCnt * ScdInfo.BlockSize / 1024;

        seq_printf(p, "%3u   %s 0x%-4x %3uK %-6u %-7u %-4u %-5u %u\n",
            RecId, Type, ScdInfo.IndexPid, Size, ScdInfo.BlockCnt,
            ScdInfo.BlockSize, ScdInfo.BufRead, ScdInfo.BufWrite, ScdInfo.Overflow);
    }

    return HI_SUCCESS;
}
#endif

/**************************** external functions ******************************/

static HI_S32 DMXGlobalIoctl(struct file *file, HI_U32 cmd, HI_VOID *arg)
{
    HI_S32 ret = HI_FAILURE;

    switch (cmd)
    {
        case CMD_DEMUX_GET_POOLBUF_ADDR:
        {
            DMX_MMZ_BUF_S *Param = (DMX_MMZ_BUF_S*)arg;

            ret = HI_DRV_DMX_GetPoolBufAddr(Param);

            break;
        }

        case CMD_DEMUX_GET_CAPABILITY:
        {
            HI_UNF_DMX_CAPABILITY_S *Param = (HI_UNF_DMX_CAPABILITY_S*)arg;

            ret = HI_DRV_DMX_GetCapability(Param);

            break;
        }

        default:
        {
            HI_WARN_DEMUX("unknown cmd: 0x%x\n", cmd);
        }
    }

    return ret;
}

static HI_S32 DMXPortIoctl(struct file *file, HI_U32 cmd, HI_VOID *arg)
{
    HI_S32 ret = HI_FAILURE;

    switch (cmd)
    {
        case CMD_DEMUX_PORT_GET_ATTR:
        {
            DMX_Port_GetAttr_S *Param = (DMX_Port_GetAttr_S*)arg;

            if (DMX_PORT_MODE_RAM == Param->PortMode)
            {
                ret = HI_DRV_DMX_RamPortGetAttr(Param->PortId, &Param->PortAttr);
            }
            else
            {
                ret = HI_DRV_DMX_TunerPortGetAttr(Param->PortId, &Param->PortAttr);
            }

            break;
        }

        case CMD_DEMUX_PORT_SET_ATTR:
        {
            DMX_Port_SetAttr_S *Param = (DMX_Port_SetAttr_S*)arg;

            if (DMX_PORT_MODE_RAM == Param->PortMode)
            {
                ret = HI_DRV_DMX_RamPortSetAttr(Param->PortId, &Param->PortAttr);
            }
            else
            {
                ret = HI_DRV_DMX_TunerPortSetAttr(Param->PortId, &Param->PortAttr);
            }

            break;
        }

        case CMD_DEMUX_PORT_ATTACH:
        {
            DMX_Port_Attach_S *Param = (DMX_Port_Attach_S*)arg;

            if (DMX_PORT_MODE_RAM == Param->PortMode)
            {
                ret = HI_DRV_DMX_AttachRamPort(Param->DmxId, Param->PortId);
            }
            else
            {
                ret = HI_DRV_DMX_AttachTunerPort(Param->DmxId, Param->PortId);
            }

            break;
        }

        case CMD_DEMUX_PORT_DETACH:
        {
            ret = HI_DRV_DMX_DetachPort(*(HI_U32*)arg);

            break;
        }

        case CMD_DEMUX_PORT_GETID:
        {
            DMX_Port_GetId_S *Param = (DMX_Port_GetId_S*)arg;

            ret = HI_DRV_DMX_GetPortId(Param->DmxId, &Param->PortMode, &Param->PortId);

            break;
        }

        case CMD_DEMUX_PORT_GETPACKETNUM:
        {
            DMX_PortPacketNum_S *Param = (DMX_PortPacketNum_S*)arg;

            if (DMX_PORT_MODE_RAM == Param->PortMode)
            {
                Param->ErrTsPackCnt = 0;

                ret = HI_DRV_DMX_RamPortGetPacketNum(Param->PortId, &Param->TsPackCnt);
            }
            else
            {
                ret = HI_DRV_DMX_TunerPortGetPacketNum(Param->PortId, &Param->TsPackCnt, &Param->ErrTsPackCnt);
            }

            break;
        }

        default:
        {
            HI_WARN_DEMUX("unknown cmd: 0x%x\n", cmd);
        }
    }

    return ret;
}

static HI_S32 DMXTsBufferIoctl(struct file *file, HI_U32 cmd, HI_VOID *arg)
{
    HI_S32 ret = HI_FAILURE;

    switch (cmd)
    {
        case CMD_DEMUX_TS_BUFFER_INIT:
        {
            DMX_TsBufInit_S *Param = (DMX_TsBufInit_S*)arg;

            ret = HI_DRV_DMX_CreateTSBuffer(Param->PortId, Param->BufSize, &Param->TsBuf, (HI_U32)file);

            break;
        }

        case CMD_DEMUX_TS_BUFFER_DEINIT:
        {
            ret = HI_DRV_DMX_DestroyTSBuffer(*(HI_U32*)arg);
            break;
        }

        case CMD_DEMUX_TS_BUFFER_GET:
        {
            DMX_TsBufGet_S *Param = (DMX_TsBufGet_S*)arg;

            ret = HI_DRV_DMX_GetTSBuffer(Param->PortId, Param->ReqLen, &Param->Data, Param->TimeoutMs);

            break;
        }

        case CMD_DEMUX_TS_BUFFER_PUT:
        {
            DMX_TsBufPut_S *Param = (DMX_TsBufPut_S*)arg;

            ret = HI_DRV_DMX_PutTSBuffer(Param->PortId, Param->ValidDataLen, Param->StartPos);

            break;
        }

        case CMD_DEMUX_TS_BUFFER_RESET:
        {
            ret = HI_DRV_DMX_ResetTSBuffer(*(HI_U32*)arg);

            break;
        }

        case CMD_DEMUX_TS_BUFFER_GET_STATUS:
        {
            DMX_TsBufStaGet_S *Param = (DMX_TsBufStaGet_S*)arg;

            ret = HI_DRV_DMX_GetTSBufferStatus(Param->PortId, &Param->Status);

            break;
        }

        default:
        {
            HI_WARN_DEMUX("unknown cmd: 0x%x\n", cmd);
        }
    }

    return ret;
}

static HI_S32 DMXChanIoctl(struct file *file, HI_U32 cmd, HI_VOID *arg)
{
    HI_S32 ret = HI_FAILURE;

    switch (cmd)
    {
        case CMD_DEMUX_CHAN_NEW:
        {
            DMX_ChanNew_S *pPara = (DMX_ChanNew_S *)arg;
            ret = HI_DRV_DMX_CreateChannel(pPara->u32DemuxId, &pPara->stChAttr,
                                    &pPara->hChannel, &pPara->stChBuf, (HI_U32)file);
            break;
        }

        case CMD_DEMUX_CHAN_DEL:
        {
            ret = HI_DRV_DMX_DestroyChannel(*(HI_HANDLE*)arg);
            break;
        }

        case CMD_DEMUX_CHAN_OPEN:
        {
            ret = HI_DRV_DMX_OpenChannel(*(HI_HANDLE*)arg);
            break;
        }

        case CMD_DEMUX_CHAN_CLOSE:
        {
            ret = HI_DRV_DMX_CloseChannel(*(HI_HANDLE*)arg);
            break;
        }

        case CMD_DEMUX_CHAN_ATTR_GET:
        {
            DMX_GetChan_Attr_S *pPara = (DMX_GetChan_Attr_S *)arg;
            ret = HI_DRV_DMX_GetChannelAttr(pPara->hChannel, &pPara->stChAttr);
            break;
        }

        case CMD_DEMUX_CHAN_ATTR_SET:
        {
            DMX_SetChan_Attr_S *pPara = (DMX_SetChan_Attr_S *)arg;
            ret = HI_DRV_DMX_SetChannelAttr(pPara->hChannel, &pPara->stChAttr);
            break;
        }

        case CMD_DEMUX_GET_CHAN_STATUS:
        {
            DMX_ChanStatusGet_S *pPara = (DMX_ChanStatusGet_S *)arg;
            ret = HI_DRV_DMX_GetChannelStatus(pPara->hChannel, &pPara->stStatus);
            break;
        }

        case CMD_DEMUX_PID_SET:
        {
            DMX_ChanPIDSet_S *pPara = (DMX_ChanPIDSet_S *)arg;
            ret = HI_DRV_DMX_SetChannelPID(pPara->hChannel, pPara->u32Pid);
            break;
        }

        case CMD_DEMUX_PID_GET:
        {
            DMX_ChanPIDGet_S *pPara = (DMX_ChanPIDGet_S *)arg;
            ret = HI_DRV_DMX_GetChannelPID(pPara->hChannel, &pPara->u32Pid);
            break;
        }

        case CMD_DEMUX_CHANID_GET:
        {
            DMX_ChannelIdGet_S *pPara = (DMX_ChannelIdGet_S *)arg;
            ret = HI_DRV_DMX_GetChannelHandle(pPara->u32DmxId, pPara->u32Pid, &pPara->hChannel);
            break;
        }

        case CMD_DEMUX_FREECHAN_GET:
        {
            DMX_FreeChanGet_S *pPara = (DMX_FreeChanGet_S *)arg;
            ret = HI_DRV_DMX_GetFreeChannelCount(pPara->u32DmxId, &pPara->u32FreeCount);
            break;
        }

        case CMD_DEMUX_SCRAMBLEFLAG_GET:
        {
            DMX_ScrambledFlagGet_S *pPara = (DMX_ScrambledFlagGet_S *)arg;
            ret = HI_DRV_DMX_GetScrambledFlag(pPara->hChannel, &pPara->enScrambleFlag);
            break;
        }

        case CMD_DEMUX_CHAN_SET_EOS_FLAG:
        {
            ret = HI_DRV_DMX_SetChannelEosFlag(*(HI_HANDLE*)arg);
            break;
        }

#ifdef DMX_USE_ECM
        case CMD_DEMUX_GET_CHAN_SWFLAG:
        {
            DMX_ChanSwGet_S *pPara = (DMX_ChanSwGet_S *)arg;
            ret = DMX_OsrGetChannelSwFlag(pPara->hChannel, &pPara->u32SwFlag);
            break;
        }

        case CMD_DEMUX_GET_CHAN_SWBUF_ADDR:
        {
            DMX_ChanSwBufGet_S *pPara = (DMX_ChanSwBufGet_S *)arg;
            ret = DMX_OsrGetChannelSwBufAddr(pPara->hChannel, &(pPara->stChnBuf));
            break;
        }
#endif

        case CMD_DEMUX_GET_CHAN_TSCNT:
        {
            DMX_ChanChanTsCnt_S *pPara = (DMX_ChanChanTsCnt_S *)arg;
            ret = HI_DRV_DMX_GetChannelTsCount(pPara->hChannel, &(pPara->u32ChanTsCnt));
            break;
        }
        case CMD_DEMUX_CHAN_CC_REPEAT_SET:
        {  
            DMX_SetChan_CC_REPEAT_S *pPara = (DMX_SetChan_CC_REPEAT_S *)arg;
            ret = HI_DRV_DMX_SetChannelCCRepeat(pPara->stChCCRepeatSet.hChannel, &pPara->stChCCRepeatSet);
            break;
        }

        default:
        {
            HI_WARN_DEMUX("unknown cmd: 0x%x\n", cmd);
        }
    }

    return ret;
}

static HI_S32 DMXFiltIoctl(struct file *file, HI_U32 cmd, HI_VOID *arg)
{
    HI_S32 ret = HI_FAILURE;

    switch (cmd)
    {
        case CMD_DEMUX_FLT_NEW:
        {
            DMX_NewFilter_S *Param = (DMX_NewFilter_S*)arg;

            ret = HI_DRV_DMX_CreateFilter(Param->DmxId, &Param->FilterAttr, &Param->Filter, (HI_U32)file);

            break;
        }

        case CMD_DEMUX_FLT_DEL:
        {
            ret = HI_DRV_DMX_DestroyFilter(*(HI_HANDLE*)arg);

            break;
        }

        case CMD_DEMUX_FLT_SET:
        {
            DMX_FilterSet_S *Param = (DMX_FilterSet_S*)arg;

            ret = HI_DRV_DMX_SetFilterAttr(Param->Filter, &Param->FilterAttr);

            break;
        }

        case CMD_DEMUX_FLT_GET:
        {
            DMX_FilterGet_S *Param = (DMX_FilterGet_S*)arg;

            ret = HI_DRV_DMX_GetFilterAttr(Param->Filter, &Param->FilterAttr);

            break;
        }

        case CMD_DEMUX_FLT_ATTACH:
        {
            DMX_FilterAttach_S *Param = (DMX_FilterAttach_S*)arg;

            ret = HI_DRV_DMX_AttachFilter(Param->Filter, Param->Channel);

            break;
        }

        case CMD_DEMUX_FLT_DETACH:
        {
            DMX_FilterDetach_S *Param = (DMX_FilterDetach_S*)arg;

            ret = HI_DRV_DMX_DetachFilter(Param->Filter, Param->Channel);

            break;
        }

        case CMD_DEMUX_FREEFLT_GET:
        {
            DMX_FreeFilterGet_S *Param = (DMX_FreeFilterGet_S*)arg;

            ret = HI_DRV_DMX_GetFreeFilterCount(Param->DmxId, &Param->FreeCount);

            break;
        }

        case CMD_DEMUX_FLT_DELALL:
        {
            ret = HI_DRV_DMX_DestroyAllFilter(*(HI_HANDLE*)arg);

            break;
        }

        case CMD_DEMUX_FLT_CHANID_GET:
        {
            DMX_FilterChannelIDGet_S *Param = (DMX_FilterChannelIDGet_S*)arg;

            ret = HI_DRV_DMX_GetFilterChannelHandle(Param->Filter, &Param->Channel);

            break;
        }

        default:
        {
            HI_ERR_DEMUX("unknown cmd: 0x%x\n", cmd);
        }
    }

    return ret;
}

static HI_S32 DMXRecvIoctl(struct file *file, HI_U32 cmd, HI_VOID *arg)
{
    HI_S32 ret = HI_FAILURE;

    switch (cmd)
    {
        case CMD_DEMUX_GET_DATA_FLAG:
        {
            DMX_GetDataFlag_S *pPara = (DMX_GetDataFlag_S *)arg;
            ret = HI_DRV_DMX_GetDataHandle(pPara->u32Flag, pPara->u32TimeOutMs);
            break;
        }

        case CMD_DEMUX_SELECT_DATA_FLAG:
        {
            DMX_SelectDataFlag_S *pPara = (DMX_SelectDataFlag_S*)arg;
            ret = HI_DRV_DMX_SelectDataHandle(pPara->channel, pPara->channelnum, pPara->u32Flag, pPara->u32TimeOutMs);
            break;
        }

        case CMD_DEMUX_ACQUIRE_MSG:
        {
            DMX_AcqMsg_S *pPara = (DMX_AcqMsg_S *)arg;
            ret = HI_DRV_DMX_AcquireBuf(pPara->hChannel, pPara->u32AcquireNum,
                                    &pPara->u32AcquiredNum, pPara->pstBuf, pPara->u32TimeOutMs);
            break;
        }

        case CMD_DEMUX_RELEASE_MSG:
        {
            DMX_RelMsg_S *pPara = (DMX_RelMsg_S *)arg;
            ret = HI_DRV_DMX_ReleaseBuf(pPara->hChannel, pPara->u32ReleaseNum, pPara->pstBuf);
            break;
        }

        default:
        {
            HI_WARN_DEMUX("unknown cmd: 0x%x\n", cmd);
        }
    }

    return ret;
}

static HI_S32 DMXPcrIoctl(struct file *file, HI_U32 cmd, HI_VOID *arg)
{
    HI_S32 ret = HI_FAILURE;

    switch (cmd)
    {
        case CMD_DEMUX_PCR_NEW:
        {
            DMX_NewPcr_S *pPara = (DMX_NewPcr_S *)arg;
            ret = HI_DRV_DMX_CreatePcrChannel(pPara->u32DmxId, &pPara->u32PcrId, (HI_U32)file);
            break;
        }

        case CMD_DEMUX_PCR_DEL:
        {
            ret = HI_DRV_DMX_DestroyPcrChannel(*(HI_U32*)arg);
            break;
        }

        case CMD_DEMUX_PCRPID_SET:
        {
            DMX_PcrPidSet_S *pPara = (DMX_PcrPidSet_S *)arg;
            ret = HI_DRV_DMX_PcrPidSet(pPara->pu32PcrChId, pPara->u32Pid);
            break;
        }

        case CMD_DEMUX_PCRPID_GET:
        {
            DMX_PcrPidGet_S *pPara = (DMX_PcrPidGet_S *)arg;
            ret = HI_DRV_DMX_PcrPidGet(pPara->pu32PcrChId, &pPara->u32Pid);
            break;
        }

        /* not support PCR user mode get, presently */
        case CMD_DEMUX_CURPCR_GET:
        {
            DMX_PcrScrGet_S *pPara = (DMX_PcrScrGet_S *)arg;
            ret = HI_DRV_DMX_PcrScrGet(pPara->pu32PcrChId, &pPara->u64PcrValue, &pPara->u64ScrValue);
            break;
        }

        case CMD_DEMUX_PCRSYN_ATTACH:
        {
            DMX_PCRSYNC_S *pPara = (DMX_PCRSYNC_S *)arg;
            ret = HI_DRV_DMX_PcrSyncAttach(pPara->u32PcrChId, pPara->u32SyncHandle);
            break;
        }

        case CMD_DEMUX_PCRSYN_DETACH:
        {
            DMX_PCRSYNC_S *pPara = (DMX_PCRSYNC_S *)arg;
            ret = HI_DRV_DMX_PcrSyncDetach(pPara->u32PcrChId);
            break;
        }

        default:
        {
            HI_WARN_DEMUX("unknown cmd: 0x%x\n", cmd);
        }
    }

    return ret;
}

static HI_S32 DMXAvIoctl(struct file *file, HI_U32 cmd, HI_VOID *arg)
{
    HI_S32 ret = HI_FAILURE;

    switch (cmd)
    {
        case CMD_DEMUX_PES_BUFFER_GETSTAT:
        {
            DMX_PesBufStaGet_S *pPara = (DMX_PesBufStaGet_S *)arg;
            ret = HI_DRV_DMX_GetPESBufferStatus(pPara->hChannel, &pPara->stBufStat);
            break;
        }

        case CMD_DEMUX_ES_BUFFER_GET:
        {
            DMX_PesBufGet_S *pPara = (DMX_PesBufGet_S *)arg;
            DMX_Stream_S stEsBuf;

            ret = HI_DRV_DMX_AcquireEs(pPara->hChannel, &stEsBuf);
            if (HI_SUCCESS == ret)
            {
                pPara->stEsBuf.pu8Buf = (HI_U8*)stEsBuf.u32BufPhyAddr;
                pPara->stEsBuf.u32BufLen = stEsBuf.u32BufLen;
                pPara->stEsBuf.u32PtsMs = stEsBuf.u32PtsMs;
            }

            break;
        }

        case CMD_DEMUX_ES_BUFFER_PUT:
        {
            DMX_PesBufGet_S *pPara = (DMX_PesBufGet_S *)arg;
            DMX_Stream_S stEsBuf;

            stEsBuf.u32BufPhyAddr = (HI_U32)pPara->stEsBuf.pu8Buf;
            stEsBuf.u32BufLen = pPara->stEsBuf.u32BufLen;
            stEsBuf.u32PtsMs = pPara->stEsBuf.u32PtsMs;

            ret = HI_DRV_DMX_ReleaseEs(pPara->hChannel, &stEsBuf);
            break;
        }

        default:
        {
            HI_WARN_DEMUX("unknown cmd: 0x%x\n", cmd);
        }
    }

    return ret;
}

static HI_S32 DMXRecIoctl(struct file *file, HI_U32 cmd, HI_VOID *arg)
{
    HI_S32 ret = HI_FAILURE;

    switch (cmd)
    {
        case CMD_DEMUX_REC_CHAN_CREATE :
        {
            DMX_Rec_CreateChan_S *Param = (DMX_Rec_CreateChan_S*)arg;

            ret = HI_DRV_DMX_CreateRecChn(
                    &Param->RecAttr,
                    &Param->RecHandle,
                    &Param->RecBufPhyAddr,
                    &Param->RecBufSize,
                    (HI_U32)file
                );

            break;
        }

        case CMD_DEMUX_REC_CHAN_DESTROY :
        {
            ret = HI_DRV_DMX_DestroyRecChn(*(HI_HANDLE*)arg);

            break;
        }

        case CMD_DEMUX_REC_CHAN_ADD_PID :
        {
            DMX_Rec_AddPid_S *Param = (DMX_Rec_AddPid_S*)arg;

            ret = HI_DRV_DMX_AddRecPid(Param->RecHandle, Param->Pid, &Param->ChanHandle, (HI_U32)file);

            break;
        }

        case CMD_DEMUX_REC_CHAN_DEL_PID :
        {
            DMX_Rec_DelPid_S *Param = (DMX_Rec_DelPid_S*)arg;

            ret = HI_DRV_DMX_DelRecPid(Param->RecHandle, Param->ChanHandle);

            break;
        }

        case CMD_DEMUX_REC_CHAN_DEL_ALL_PID :
        {
            ret = HI_DRV_DMX_DelAllRecPid(*(HI_HANDLE*)arg);

            break;
        }

        case CMD_DEMUX_REC_CHAN_ADD_EXCLUDE_PID :
        {
            DMX_Rec_ExcludePid_S *Param = (DMX_Rec_ExcludePid_S*)arg;

            ret = HI_DRV_DMX_AddExcludeRecPid(Param->RecHandle, Param->Pid);

            break;
        }

        case CMD_DEMUX_REC_CHAN_DEL_EXCLUDE_PID :
        {
            DMX_Rec_ExcludePid_S *Param = (DMX_Rec_ExcludePid_S*)arg;

            ret = HI_DRV_DMX_DelExcludeRecPid(Param->RecHandle, Param->Pid);

            break;
        }

        case CMD_DEMUX_REC_CHAN_CANCEL_EXCLUDE :
        {
            ret = HI_DRV_DMX_DelAllExcludeRecPid(*(HI_HANDLE*)arg);

            break;
        }

        case CMD_DEMUX_REC_CHAN_START :
        {
            ret = HI_DRV_DMX_StartRecChn(*(HI_HANDLE*)arg);

            break;
        }

        case CMD_DEMUX_REC_CHAN_STOP :
        {
            ret = HI_DRV_DMX_StopRecChn(*(HI_HANDLE*)arg);

            break;
        }

        case CMD_DEMUX_REC_CHAN_ACQUIRE_DATA :
        {
            DMX_Rec_AcquireData_S *Param = (DMX_Rec_AcquireData_S*)arg;

            ret = HI_DRV_DMX_AcquireRecData(Param->RecHandle, &Param->RecData, Param->TimeoutMs);

            break;
        }

        case CMD_DEMUX_REC_CHAN_RELEASE_DATA :
        {
            DMX_Rec_ReleaseData_S *Param = (DMX_Rec_ReleaseData_S*)arg;

            ret = HI_DRV_DMX_ReleaseRecData(Param->RecHandle, &Param->RecData);

            break;
        }

        case CMD_DEMUX_REC_CHAN_ACQUIRE_INDEX :
        {
            DMX_Rec_AcquireIndex_S *Param = (DMX_Rec_AcquireIndex_S*)arg;

            ret = HI_DRV_DMX_AcquireRecIndex(Param->RecHandle, &Param->IndexData, Param->TimeoutMs);

            break;
        }

        case CMD_DEMUX_REC_CHAN_GET_BUF_STATUS :
        {
            DMX_Rec_BufStatus_S *Param = (DMX_Rec_BufStatus_S*)arg;

            ret = HI_DRV_DMX_GetRecBufferStatus(Param->RecHandle, &Param->BufStatus);

            break;
        }

        case CMD_DEMUX_REC_CHAN_ACQUIRE_SCD :
        {
            DMX_Rec_AcquireData_S *Param = (DMX_Rec_AcquireData_S*)arg;

            ret = HI_DRV_DMX_AcquireScdData(Param->RecHandle, &Param->RecData, Param->TimeoutMs);

            break;
        }

        case CMD_DEMUX_REC_CHAN_RELEASE_SCD :
        {
            DMX_Rec_ReleaseData_S *Param = (DMX_Rec_ReleaseData_S*)arg;

            ret = HI_DRV_DMX_ReleaseScdData(Param->RecHandle, &Param->RecData);

            break;
        }

        default:
        {
            HI_WARN_DEMUX("unknown cmd: 0x%x\n", cmd);
        }
    }

    return ret;
}

HI_S32 DMX_OsrIoctl(struct inode *inode, struct file *file, HI_U32 cmd, HI_VOID *arg)
{
    HI_S32  ret     = HI_FAILURE;
    HI_U32  CmdType = cmd & DEMUX_CMD_MASK;

    switch (CmdType)
    {
        case DEMUX_GLOBAL_CMD:
        {
            ret = DMXGlobalIoctl(file, cmd, arg);

            break;
        }

        case DEMUX_PORT_CMD:
        {
            ret = DMXPortIoctl(file, cmd, arg);

            break;
        }

        case DEMUX_TSBUFFER_CMD:
        {
            ret = DMXTsBufferIoctl(file, cmd, arg);

            break;
        }

        case DEMUX_CHAN_CMD:
        {
            ret = DMXChanIoctl(file, cmd, arg);

            break;
        }

        case DEMUX_FILT_CMD:
        {
            ret = DMXFiltIoctl(file, cmd, arg);

            break;
        }

    #ifdef DMX_DESCRAMBLER_SUPPORT
        case DEMUX_KSET_CMD:
        {
            ret = DMXKeyIoctl(file, cmd, arg);

            break;
        }
    #endif

        case DEMUX_RECV_CMD:
        {
            ret = DMXRecvIoctl(file, cmd, arg);

            break;
        }

        case DEMUX_PCR_CMD:
        {
            ret = DMXPcrIoctl(file, cmd, arg);

            break;
        }

        case DEMUX_AV_CMD:
        {
            ret = DMXAvIoctl(file, cmd, arg);

            break;
        }

        case DEMUX_REC_CMD:
        {
            ret = DMXRecIoctl(file, cmd, arg);

            break;
        }

        default:
        {
            HI_WARN_DEMUX("unknown cmd: 0x%x\n", cmd);
        }
    }

    return ret;
}

/*****************************************************************************
 Prototype    : DMX_DRV_Open
 Description  : open function in DEMUX module
 Input        : None
 Output       : None
 Return Value :
*****************************************************************************/
static HI_S32 DMX_DRV_Open(struct inode * inode, struct file * file)
{
    HI_DRV_DMX_Open();

    return 0;
}

/*****************************************************************************
 Prototype    : DMX_DRV_Release
 Description  : release function in DEMUX module
 Input        : None
 Output       : None
 Return Value :
*****************************************************************************/
static HI_S32 DMX_DRV_Release(struct inode * inode, struct file * file)
{
    HI_DRV_DMX_Close((HI_U32)file);

    return 0;
}

/*****************************************************************************
 Prototype    : DMX_DRV_Ioctl
 Description  : Ioctl function in DEMUX module
 Input        : None
 Output       : None
 Return Value :
*****************************************************************************/
static long DMX_DRV_Ioctl(struct file *file, HI_U32 cmd, unsigned long arg)
{
    return (long)HI_DRV_UserCopy(file->f_dentry->d_inode, file, cmd, arg, DMX_OsrIoctl);
}

static struct file_operations DMX_DRV_Fops =
{
    .owner          = THIS_MODULE,
    .open           = DMX_DRV_Open,
    .unlocked_ioctl = DMX_DRV_Ioctl,
    .release        = DMX_DRV_Release,
};

static PM_BASEOPS_S dmx_drvops =
{
    .probe        = NULL,
    .remove       = NULL,
    .shutdown     = NULL,
    .prepare      = NULL,
    .complete     = NULL,
    .suspend      = DMX_OsiSuspend,
    .suspend_late = NULL,
    .resume_early = NULL,
    .resume       = DMX_OsiResume,
};

/*****************************************************************************
 Prototype    : DMX_DRV_ModInit
 Description  : initialize function in DEMUX module
 Input        : None
 Output       : None
 Return Value :
*****************************************************************************/
HI_S32 DMX_DRV_ModInit(HI_VOID)
{
#ifdef HI_DEMUX_PROC_SUPPORT
    DRV_PROC_ITEM_S *item;
#endif

#ifndef HI_MCE_SUPPORT
    if (HI_SUCCESS != HI_DRV_DMX_Init())
    {
        goto out;
    }
#endif

    strcpy(g_stDemuxDev.devfs_name, UMAP_DEVNAME_DEMUX);
    g_stDemuxDev.fops   = &DMX_DRV_Fops;
    g_stDemuxDev.minor  = UMAP_MIN_MINOR_DEMUX;
    g_stDemuxDev.owner  = THIS_MODULE;
    g_stDemuxDev.drvops = &dmx_drvops;

    if (HI_DRV_DEV_Register(&g_stDemuxDev) < 0)
    {
        HI_FATAL_DEMUX("Unable to register demux dev\n");
        goto out;
    }

#ifdef HI_DEMUX_PROC_SUPPORT
    item = HI_DRV_PROC_AddModule("demux_main", NULL, NULL);
    if (!item)
    {
        HI_WARN_DEMUX("add proc demux_main failed\n");
    }
    else
    {
        item->read  = DMXProcRead;
        item->write = DMXProcWrite;
    }

    item = HI_DRV_PROC_AddModule("demux_port", DMXPortProcRead, NULL);
    if (!item)
    {
        HI_WARN_DEMUX("add proc demux_port failed\n");
    }

    item = HI_DRV_PROC_AddModule("demux_tsbuf", DMXTsBufProcRead, NULL);
    if (!item)
    {
        HI_WARN_DEMUX("add proc demux_tsbuf failed\n");
    }

    item = HI_DRV_PROC_AddModule("demux_chan", DMXChanProcRead, NULL);
    if (!item)
    {
        HI_WARN_DEMUX("add proc demux_chan failed\n");
    }

    item = HI_DRV_PROC_AddModule("demux_chanbuf", DMXChanBufProcRead, NULL);
    if (!item)
    {
        HI_WARN_DEMUX("add proc demux_chanbuf failed\n");
    }

    item = HI_DRV_PROC_AddModule("demux_filter", DMXFilterProcRead, NULL);
    if (!item)
    {
        HI_WARN_DEMUX("add proc demux_filter failed\n");
    }

#ifdef DMX_DESCRAMBLER_SUPPORT
    item = HI_DRV_PROC_AddModule("demux_key", DMXKeyProcRead, NULL);
    if (!item)
    {
        HI_WARN_DEMUX("add proc demux_key failed\n");
    }
#endif

    item = HI_DRV_PROC_AddModule("demux_pcr", DMXPcrProcRead, NULL);
    if (!item)
    {
        HI_WARN_DEMUX("add proc demux_pcr failed\n");
    }

    item = HI_DRV_PROC_AddModule("demux_rec", DMXRecProcRead, NULL);
    if (!item)
    {
        HI_WARN_DEMUX("add proc demux_rec failed\n");
    }

    item = HI_DRV_PROC_AddModule("demux_rec_index", DMXRecScdProcRead, NULL);
    if (!item)
    {
        HI_WARN_DEMUX("add proc demux_rec_index failed\n");
    }
#endif

#ifdef MODULE
#ifndef CONFIG_SUPPORT_CA_RELEASE
    printk("Load hi_demux.ko success.  \t(%s)\n", VERSION_STRING);
#endif
#endif

    return HI_SUCCESS;

out :
    DMX_DRV_ModExit();

    return HI_FAILURE;
}

/*****************************************************************************
 Prototype    : DMX_DRV_ModExit
 Description  : exit function in DEMUX module
 Input        : None
 Output       : None
 Return Value :
*****************************************************************************/
HI_VOID DMX_DRV_ModExit(HI_VOID)
{
#ifdef HI_DEMUX_PROC_SUPPORT
    HI_DRV_PROC_RemoveModule("demux_rec_index");
    HI_DRV_PROC_RemoveModule("demux_rec");
    HI_DRV_PROC_RemoveModule("demux_pcr");
#ifdef DMX_DESCRAMBLER_SUPPORT
    HI_DRV_PROC_RemoveModule("demux_key");
#endif
    HI_DRV_PROC_RemoveModule("demux_filter");
    HI_DRV_PROC_RemoveModule("demux_chanbuf");
    HI_DRV_PROC_RemoveModule("demux_chan");
    HI_DRV_PROC_RemoveModule("demux_tsbuf");
    HI_DRV_PROC_RemoveModule("demux_port");
    HI_DRV_PROC_RemoveModule("demux_main");
#endif

    HI_DRV_DEV_UnRegister(&g_stDemuxDev);

#ifndef HI_MCE_SUPPORT
    HI_DRV_DMX_DeInit();
#endif
}

#ifdef MODULE
module_init(DMX_DRV_ModInit);
module_exit(DMX_DRV_ModExit);
#endif

MODULE_AUTHOR("HISILICON");
MODULE_LICENSE("GPL");


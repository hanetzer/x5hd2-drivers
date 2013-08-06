//******************************************************************************
//  Copyright (C), 2007-2008, Hisilicon Technologies Co., Ltd.
//
//******************************************************************************
// File name     : viu_reg.h
// Version       : 2.0
// Author        : c57657
// Created       : 2009-4-8
// Last Modified : 
// Description   :  The C union definition file for the module VIU
// Function List : 
// History       : 
// 1 Date        : 
// Author        : c57657
// Modification  : Create file
//------------------------------------------------------------------------------
// $Log: viu_reg.h,v $
// Revision 1.1  2009/04/16 00:34:31  c57657
// no message
//
//******************************************************************************

#ifndef __VIU_REG_H__
#define __VIU_REG_H__

//#include "../tw2864/hi_type.h"
#include "hi_type.h"

// The base address of the module VIU
#define VIU_BASE_ADDR 0x60130000
#define VIU_BASE_ADDR_OFFSET 0x1000
#define VIU_CRG_CONTROL 0x101f504c

// Define the union U_VI_PORT_CFG
typedef union
{
    // Define the struct bits
    struct
    {
        HI_U32          u32PortEn             : 1   ; // [0]
        HI_U32          u32PortHsyncNeg       : 1   ; // [1]
        HI_U32          u32PortHsync          : 1   ; // [2]
        HI_U32          u32PortVsyncNeg       : 1   ; // [3]
        HI_U32          u32PortVsync          : 1   ; // [4]
        HI_U32          u32Reserved0          : 1   ; // [5]
        HI_U32          u32PortMuxMode        : 2   ; // [7..6]
        HI_U32          u32PortCapMode        : 2   ; // [9..8]        
        HI_U32          u32PortScanMode       : 2   ; // [11:10]
        HI_U32          u32Reserved1          : 20  ; // [31:12]
    } bits;

    // Define an unsigned member
    HI_U32          u32;

} U_VI_PORT_CFG;

// Define the union U_VI_CH_CFG
typedef union
{
    // Define the struct bits
    struct
    {
        HI_U32          u32DataWidth          : 2   ; // [1..0]
        HI_U32          u32StoreMode          : 1   ; // [2]
        HI_U32          u32Reserved0          : 1   ; // [3]
        HI_U32          u32CapSeq             : 2   ; // [5..4]
        HI_U32          u32CapSel             : 2   ; // [7..6]
        HI_U32          u32StoreMethod        : 2   ; // [9..8]
        HI_U32          u32ChromaResample     : 1   ; // [10]
        HI_U32          u32DownScaling        : 1   ; // [11]
        HI_U32          u32CorrectEn          : 1   ; // [12]
        HI_U32          u32OddLineSel         : 3   ; // [15..13]
        HI_U32          u32EvenLineSel        : 3   ; // [18..16]
        HI_U32          u32Reserved1          : 1   ; // [19..19]
        HI_U32          u32ChnId              : 2   ; // [21..20]
        HI_U32          u32ChnIdEn            : 1   ; // [22]
        HI_U32          u32ChromSwap          : 1   ; // [23]
        HI_U32          u32SeavFNeg           : 1   ; // [24]
        HI_U32          u32YcChannel          : 1   ; // [25]
        HI_U32          u32FixCode            : 1   ; // [26]
        HI_U32          u32Reserved2          : 5   ; // [31..27]
    } bits;

    // Define an unsigned member
    HI_U32          u32;

} U_VI_CH_CFG;

// Define the union U_VI_CH_CTRL
typedef union
{
    // Define the struct bits
    struct
    {
        HI_U32          u32ChEn               : 1   ; // [0]
        HI_U32          u32Reserved0          : 3   ; // [3..1]
        HI_U32          u32Block0En           : 1   ; // [4]
        HI_U32          u32Block1En           : 1   ; // [5]
        HI_U32          u32Block2En           : 1   ; // [6]
        HI_U32          u32Block3En           : 1   ; // [7]
        HI_U32          u32Reserved1           : 4 ; //[8..11]
        
/*     HI_U32          u32Block0Mode         : 1   ; // [8]
        HI_U32          u32Block1Mode         : 1   ; // [9]
        HI_U32          u32Block2Mode         : 1   ; // [10]
        HI_U32          u32Block3Mode         : 1   ; // [11]*/

        HI_U32          u32Anc0En             : 1   ; // [12]
        HI_U32          u32Anc1En             : 1   ; // [13]
        HI_U32          u32LumStrhEn          : 1   ; // [14]
        HI_U32          u32DebugEn            : 1   ; // [15]
        HI_U32          u32Reserved2          : 16  ; // [31..16]
    } bits;

    // Define an unsigned member
    HI_U32          u32;

} U_VI_CH_CTRL;

// Define the union U_VI_REG_NEWER
typedef union
{
    // Define the struct bits
    struct
    {
        HI_U32          u32RegNewer           : 1   ; // [0]
        HI_U32          u32Reserved           : 31  ; // [31..1]
    } bits;

    // Define an unsigned member
    HI_U32          u32;

} U_VI_REG_NEWER;

// Define the union U_VI_CAP_START
typedef union
{
    // Define the struct bits
    struct
    {
        HI_U32          u32StartX             : 12  ; // [11..0]
        HI_U32          u32StartY             : 12  ; // [23..12]
        HI_U32          u32Reserved           : 8   ; // [31..24]
    } bits;

    // Define an unsigned member
    HI_U32          u32;

} U_VI_CAP_START;

// Define the union U_VI_CAP_SIZE
typedef union
{
    // Define the struct bits
    struct
    {
        HI_U32          u32Width              : 12  ; // [11..0]
        HI_U32          u32Height             : 12  ; // [23..12]
        HI_U32          u32Reserved           : 8   ; // [31..24]
    } bits;

    // Define an unsigned member
    HI_U32          u32;

} U_VI_CAP_SIZE;

// Define the union U_VI_STORESIZE
typedef union
{
    // Define the struct bits
    struct
    {
        HI_U32          u32Width              : 12  ; // [11..0]
        HI_U32          u32Height             : 12  ; // [23..12]
        HI_U32          u32Reserved           : 8   ; // [31..24]
    } bits;

    // Define an unsigned member
    HI_U32          u32;

} U_VI_STORESIZE;

// Define the union U_VI_LINE_OFFSET
typedef union
{
    // Define the struct bits
    struct
    {
        HI_U32          u32VLineOffset        : 10  ; // [9..0]
        HI_U32          u32ULineOffset        : 10  ; // [19..10]
        HI_U32          u32YLineOffset        : 12  ; // [31..20]
    } bits;

    // Define an unsigned member
    HI_U32          u32;

} U_VI_LINE_OFFSET;

// Define the union U_VI_BASE_ADDR
typedef union
{
    // Define the struct bits
    struct
    {
        HI_U32          u32ViBaseAddr         : 32  ; // [31:0]
    } bits;

    // Define an unsigned member
    HI_U32          u32;
    
} U_VI_BASE_ADDR;

// Define the union VI_INT_DLY_CNT
typedef union
{
    // Define the struct bits
    struct
    {
        HI_U32          u32IntDlyCnt          : 32  ; // [31:0]
    } bits;

    // Define an unsigned member
    HI_U32          u32;
    
} U_VI_INT_DLY_CNT;

// Define the union U_VI_INT_EN
typedef union
{
    // Define the struct bits
    struct
    {
        HI_U32          u32CcIntEn            : 1   ; // [0]
        HI_U32          u32BufOvfIntEn        : 1   ; // [1]
        HI_U32          u32FieldThrowIntEn    : 1   ; // [2]
        HI_U32          u32ErrIntEn           : 1   ; // [3]
        HI_U32          u32ProcErrIntEn       : 1   ; // [4]
        HI_U32          u32RegUpdateIntEn     : 1   ; // [5]
        HI_U32          u32FramePulseIntEn    : 1   ; // [6]
        HI_U32          u32NtscPalTransIntEn  : 1   ; // [7]
        HI_U32          u32ChDivErrIntEn      : 1   ; // [8]
        HI_U32          u32Reserved           : 23  ; // [31..9]
    } bits;

    // Define an unsigned member
    HI_U32          u32;

} U_VI_INT_EN;

// Define the union U_VI_INT_STATUS
typedef union
{
    // Define the struct bits
    struct
    {
        HI_U32          u32CcInt              : 1   ; // [0]
        HI_U32          u32BufOvfInt          : 1   ; // [1]
        HI_U32          u32FieldThrowInt      : 1   ; // [2]
        HI_U32          u32ErrorInt           : 1   ; // [3]
        HI_U32          u32ProcErrInt         : 1   ; // [4]
        HI_U32          u32RegUpdateInt       : 1   ; // [5]
        HI_U32          u32FramePulseInt      : 1   ; // [6]
        HI_U32          u32NtscPalTransInt    : 1   ; // [7]
        HI_U32          u32ChDivErrInt        : 1   ; // [8]
        HI_U32          u32Reserved           : 23  ; // [31..9]
    } bits;

    // Define an unsigned member
    HI_U32          u32;

} U_VI_INT_STATUS;

// Define the union U_VI_RAW_INT
typedef union
{
    // Define the struct bits
    struct
    {
        HI_U32          u32CcRawInt           : 1   ; // [0]
        HI_U32          u32BufOvfRawInt       : 1   ; // [1]
        HI_U32          u32FieldThrowRawInt   : 1   ; // [2]
        HI_U32          u32ErrorRawInt        : 1   ; // [3]
        HI_U32          u32ProcErrRawInt      : 1   ; // [4]
        HI_U32          u32RegUpdateRawInt    : 1   ; // [5]
        HI_U32          u32FramePulseRawInt   : 1   ; // [6]
        HI_U32          u32NtscPalTransRawInt  : 1   ; // [7]
        HI_U32          u32ChDivErrRawInt     : 1   ; // [8]
        HI_U32          u32Reserved           : 23  ; // [31..9]
    } bits;

    // Define an unsigned member
    HI_U32          u32;

} U_VI_RAW_INT;

// Define the union U_VI_INT_INDICATOR
typedef union
{
    // Define the struct bits
    struct
    {
        HI_U32          u32Ch0IntIndicator    : 1   ; // [0]
        HI_U32          u32Ch1IntIndicator    : 1   ; // [1]
        HI_U32          u32Ch2IntIndicator    : 1   ; // [2]
        HI_U32          u32Ch3IntIndicator    : 1   ; // [3]
        HI_U32          u32Ch4IntIndicator    : 1   ; // [4]
        HI_U32          u32Ch5IntIndicator    : 1   ; // [5]
        HI_U32          u32Ch6IntIndicator    : 1   ; // [6]
        HI_U32          u32Ch7IntIndicator    : 1   ; // [7]
        HI_U32          u32Ch8IntIndicator    : 1   ; // [8]
        HI_U32          u32Ch9IntIndicator    : 1   ; // [9]
        HI_U32          u32Ch10IntIndicator   : 1   ; // [10]
        HI_U32          u32Ch11IntIndicator   : 1   ; // [11]
        HI_U32          u32Ch12IntIndicator   : 1   ; // [12]
        HI_U32          u32Ch13IntIndicator   : 1   ; // [13]
        HI_U32          u32Ch14IntIndicator   : 1   ; // [14]
        HI_U32          u32Ch15IntIndicator   : 1   ; // [15]
        HI_U32          u32Reserved           : 16  ; // [31..16]
    } bits;

    // Define an unsigned member
    HI_U32          u32;

} U_VI_INT_INDICATOR;

// Define the union U_VI_RAW_INT_INDICATOR
typedef union
{
    // Define the struct bits
    struct
    {
        HI_U32          u32Ch0RawIntIndicator  : 1   ; // [0]
        HI_U32          u32Ch1RawIntIndicator  : 1   ; // [1]
        HI_U32          u32Ch2RawIntIndicator  : 1   ; // [2]
        HI_U32          u32Ch3RawIntIndicator  : 1   ; // [3]
        HI_U32          u32Ch4RawIntIndicator  : 1   ; // [4]
        HI_U32          u32Ch5RawIntIndicator  : 1   ; // [5]
        HI_U32          u32Ch6RawIntIndicator  : 1   ; // [6]
        HI_U32          u32Ch7RawIntIndicator  : 1   ; // [7]
        HI_U32          u32Ch8RawIntIndicator  : 1   ; // [8]
        HI_U32          u32Ch9RawIntIndicator  : 1   ; // [9]
        HI_U32          u32Ch10RawIntIndicator  : 1   ; // [10]
        HI_U32          u32Ch11RawIntIndicator  : 1   ; // [11]
        HI_U32          u32Ch12RawIntIndicator  : 1   ; // [12]
        HI_U32          u32Ch13RawIntIndicator  : 1   ; // [13]
        HI_U32          u32Ch14RawIntIndicator  : 1   ; // [14]
        HI_U32          u32Ch15RawIntIndicator  : 1   ; // [15]
        HI_U32          u32Reserved           : 16  ; // [31..16]
    } bits;

    // Define an unsigned member
    HI_U32          u32;

} U_VI_RAW_INT_INDICATOR;

// Define the union U_VI_STATUS
typedef union
{
    // Define the struct bits
    struct
    {
        HI_U32          u32ImageDone          : 1   ; // [0]
        HI_U32          u32BufOvf             : 1   ; // [1]
        HI_U32          u32FrameLoss          : 1   ; // [2]
        HI_U32          u32BusErr             : 1   ; // [3]
        HI_U32          u32ProcErr            : 1   ; // [4]
        HI_U32          u32Snooze             : 1   ; // [5]
        HI_U32          u32Field2             : 1   ; // [6]
        HI_U32          u32ViBusy             : 1   ; // [7]
        HI_U32          u32ActHeight          : 12  ; // [19..8]
        HI_U32          u32Reserved           : 12  ; // [31..20]
    } bits;

    // Define an unsigned member
    HI_U32          u32;

} U_VI_STATUS;

// Define the union U_VI_LUM_ADDER
typedef union
{
    // Define the struct bits
    struct
    {
        HI_U32          u32ViLumAdder         : 32  ; // [31:0]
    } bits;

    // Define an unsigned member
    HI_U32          u32;
    
} U_VI_LUM_ADDER;

// Define the union U_VI_LUM_STRH
typedef union
{
    // Define the struct bits
    struct
    {
        HI_U32          u32CoeffK             : 8   ; // [7..0]
        HI_U32          u32CoeffM0            : 10  ; // [17..8]
        HI_U32          u32Reserved0          : 2   ; // [19..18]
        HI_U32          u32CoeffM1            : 10  ; // [29..20]
        HI_U32          u32Reserved1          : 2   ; // [31..30]
    } bits;

    // Define an unsigned member
    HI_U32          u32;

} U_VI_LUM_STRH;

// Define the union U_VI_LUM_DIFF_ADDER
typedef union
{
    // Define the struct bits
    struct
    {
        HI_U32          u32LumDiffAdder        : 32  ; // [31:0]
    } bits;

    // Define an unsigned member
    HI_U32          u32;
    
} U_VI_LUM_DIFF_ADDER;

// Define the union U_VI_BLOCK_START
typedef union
{
    // Define the struct bits
    struct
    {
        HI_U32          u32BlockStartX        : 12  ; // [11..0]
        HI_U32          u32BlockStartY        : 12  ; // [23..12]
        HI_U32          u32Reserved           : 8   ; // [31..24]
    } bits;

    // Define an unsigned member
    HI_U32          u32;

} U_VI_BLOCK_START;

// Define the union U_VI_BLOCK_SIZE
typedef union
{
    // Define the struct bits
    struct
    {
        HI_U32          u32BlockWidth         : 12  ; // [11..0]
        HI_U32          u32BlockHeight        : 12  ; // [23..12]
        HI_U32          u32Reserved           : 8   ; // [31..24]
    } bits;

    // Define an unsigned member
    HI_U32          u32;

} U_VI_BLOCK_SIZE;

// Define the union U_VI_BLOCK_COLOR
typedef union
{
    // Define the struct bits
    struct
    {
        HI_U32          u32BlockV             : 8   ; // [7..0]
        HI_U32          u32BlockU             : 8   ; // [15..8]
        HI_U32          u32BlockY             : 8   ; // [23..16]
        HI_U32          u32Reserved           : 8   ; // [31..24]
    } bits;

    // Define an unsigned member
    HI_U32          u32;

} U_VI_BLOCK_COLOR;

// Define the union U_VI_BLOCK_MSC_SIZE
typedef union
{
    // Define the struct bits
    struct
    {
        HI_U32          u32MscWidth           : 8   ; // [7..0]
        HI_U32          u32MscHeight          : 8   ; // [15..8]
        HI_U32          u32Reserved           : 16  ; // [31..16]
    } bits;

    // Define an unsigned member
    HI_U32          u32;

} U_VI_BLOCK_MSC_SIZE;

// Define the union U_VI_ANC_START
typedef union
{
    // Define the struct bits
    struct
    {
        HI_U32          u32AncHos             : 12  ; // [11..0]
        HI_U32          u32AncVos             : 12  ; // [23..12]
        HI_U32          u32AncLoc             : 2   ; // [25..24]
        HI_U32          u32Reserved           : 6   ; // [31..26]
    } bits;

    // Define an unsigned member
    HI_U32          u32;

} U_VI_ANC_START;

// Define the union U_VI_ANC_SIZE
typedef union
{
    // Define the struct bits
    struct
    {
        HI_U32          u32AncSize            : 12  ; // [11..0]
        HI_U32          u32Reserved           : 20  ; // [31..12]
    } bits;

    // Define an unsigned member
    HI_U32          u32;

} U_VI_ANC_SIZE;

// Define the union U_VI_ANC_WORD
typedef union
{
    // Define the struct bits
    struct
    {
        HI_U32          u32ViAncWord          : 32  ; // [31:0]
    } bits;

    // Define an unsigned member
    HI_U32          u32;
    
} U_VI_ANC_WORD;

// Define the union U_VI_VSYNC1
typedef union
{
    // Define the struct bits
    struct
    {
        HI_U32          u32Act1Height         : 12  ; // [11..0]
        HI_U32          u32Act1Voff           : 8   ; // [19..12]
        HI_U32          u32Act1Vbb            : 8   ; // [27..20]
        HI_U32          u32VsynWidth          : 4   ; // [31..28]
    } bits;

    // Define an unsigned member
    HI_U32          u32;

} U_VI_VSYNC1;

// Define the union U_VI_VSYNC2
typedef union
{
    // Define the struct bits
    struct
    {
        HI_U32          u32Act2Height         : 12  ; // [11..0]
        HI_U32          u32Act2Voff           : 8   ; // [19..12]
        HI_U32          u32Act2Vbb            : 8   ; // [27..20]
        HI_U32          u32HsynWidthMsb       : 4   ; // [31..28]
    } bits;

    // Define an unsigned member
    HI_U32          u32;

} U_VI_VSYNC2;

// Define the union U_VI_HSYNC
typedef union
{
    // Define the struct bits
    struct
    {
        HI_U32          u32ActWidth           : 12  ; // [11..0]
        HI_U32          u32ActHoff            : 8   ; // [19..12]
        HI_U32          u32ActHbb             : 8   ; // [27..20]
        HI_U32          u32HsynWidthLsb       : 4   ; // [31..28]
    } bits;

    // Define an unsigned member
    HI_U32          u32;

} U_VI_HSYNC;

// Define the union U_VI_PRIO_CFG
typedef union
{
    // Define the struct bits
    struct
    {
        HI_U32          u32Vi0PrioCtrl        : 1   ; // [0]
        HI_U32          u32Vi1PrioCtrl        : 1   ; // [1]
        HI_U32          u32Vi2PrioCtrl        : 1   ; // [2]
        HI_U32          u32Vi3PrioCtrl        : 1   ; // [3]
        HI_U32          u32Vi4PrioCtrl        : 1   ; // [4]
        HI_U32          u32Vi5PrioCtrl        : 1   ; // [5]
        HI_U32          u32Vi6PrioCtrl        : 1   ; // [6]
        HI_U32          u32Vi7PrioCtrl        : 1   ; // [7]
        HI_U32          u32Vi8PrioCtrl        : 1   ; // [8]
        HI_U32          u32Vi9PrioCtrl        : 1   ; // [9]
        HI_U32          u32Vi10PrioCtrl       : 1   ; // [10]
        HI_U32          u32Vi11PrioCtrl       : 1   ; // [11]
        HI_U32          u32Vi12PrioCtrl       : 1   ; // [12]
        HI_U32          u32Vi13PrioCtrl       : 1   ; // [13]
        HI_U32          u32Vi14PrioCtrl       : 1   ; // [14]
        HI_U32          u32Vi15PrioCtrl       : 1   ; // [15]
        HI_U32          u32OutstandingMax     : 4   ; // [19..16]
        HI_U32          u32Reserved           : 12  ; // [31..20]
    } bits;

    // Define an unsigned member
    HI_U32          u32;

} U_VI_PRIO_CFG;

// Define the union U_VI_FIR_COEF
typedef union
{
    // Define the struct bits
    struct
    {
        HI_U32          u32FirCoef0           : 10  ; // [9..0]
        HI_U32          u32Reserved0          : 6   ; // [15..10]
        HI_U32          u32FirCoef1           : 10  ; // [25..16]
        HI_U32          u32Reserved1          : 6   ; // [31..26]
    } bits;

    // Define an unsigned member
    HI_U32          u32;

} U_VI_FIR_COEF;

//==============================================================================
// Define the global struct
typedef struct
{
    volatile U_VI_PORT_CFG              VI0_PORT_CFG;             //0x60130000
    volatile U_VI_CH_CFG                VI0_CH_CFG;
    volatile U_VI_CH_CTRL               VI0_CH_CTRL;
    volatile U_VI_REG_NEWER             VI0_REG_NEWER;
    volatile U_VI_CAP_START             VI0_CAP_START;
    volatile U_VI_CAP_SIZE              VI0_CAP_SIZE;
    volatile U_VI_STORESIZE             VI0_Y_STORESIZE;
    volatile U_VI_STORESIZE             VI0_U_STORESIZE;
    volatile U_VI_STORESIZE             VI0_V_STORESIZE;
    volatile U_VI_LINE_OFFSET           VI0_LINE_OFFSET;
    volatile U_VI_BASE_ADDR             VI0_Y_BASE_ADDR;
    volatile U_VI_BASE_ADDR             VI0_U_BASE_ADDR;
    volatile U_VI_BASE_ADDR             VI0_V_BASE_ADDR;
    volatile U_VI_INT_DLY_CNT           VI_INT_DLY_CNT;
    volatile U_VI_INT_EN                VI0_INT_EN;
    volatile U_VI_INT_STATUS            VI0_INT_STATUS;
    volatile U_VI_RAW_INT               VI0_RAW_INT;
    volatile U_VI_INT_INDICATOR         VI_INT_INDICATOR;
    volatile U_VI_RAW_INT_INDICATOR     VI_RAW_INT_INDICATOR;
    volatile U_VI_STATUS                VI0_STATUS;
    volatile U_VI_LUM_ADDER             VI0_LUM_ADDER;
    volatile U_VI_LUM_STRH              VI0_LUM_STRH;
    volatile U_VI_LUM_DIFF_ADDER        VI0_LUM_DIFF_ADDER;
    volatile U_VI_BLOCK_START           VI0_BLOCK0_START;
    volatile U_VI_BLOCK_START           VI0_BLOCK1_START;
    volatile U_VI_BLOCK_START           VI0_BLOCK2_START;
    volatile U_VI_BLOCK_START           VI0_BLOCK3_START;
    volatile U_VI_BLOCK_SIZE            VI0_BLOCK0_SIZE;
    volatile U_VI_BLOCK_SIZE            VI0_BLOCK1_SIZE;
    volatile U_VI_BLOCK_SIZE            VI0_BLOCK2_SIZE;
    volatile U_VI_BLOCK_SIZE            VI0_BLOCK3_SIZE;
    volatile U_VI_BLOCK_COLOR           VI0_BLOCK0_COLOR;
    volatile U_VI_BLOCK_COLOR           VI0_BLOCK1_COLOR;
    volatile U_VI_BLOCK_COLOR           VI0_BLOCK2_COLOR;
    volatile U_VI_BLOCK_COLOR           VI0_BLOCK3_COLOR;
    volatile U_VI_BLOCK_MSC_SIZE        VI0_BLOCK0_MSC_SIZE;
    volatile U_VI_BLOCK_MSC_SIZE        VI0_BLOCK1_MSC_SIZE;
    volatile U_VI_BLOCK_MSC_SIZE        VI0_BLOCK2_MSC_SIZE;
    volatile U_VI_BLOCK_MSC_SIZE        VI0_BLOCK3_MSC_SIZE;
    volatile U_VI_ANC_START             VI0_ANC0_START;
    volatile U_VI_ANC_SIZE              VI0_ANC0_SIZE;
    volatile U_VI_ANC_START             VI0_ANC1_START;
    volatile U_VI_ANC_SIZE              VI0_ANC1_SIZE;
    volatile U_VI_ANC_WORD              VI0_ANC0_WORD[8];
    volatile U_VI_ANC_WORD              VI0_ANC1_WORD[8];
    volatile U_VI_VSYNC1                VI_P0_VSYNC1;
    volatile U_VI_VSYNC2                VI_P0_VSYNC2;
    volatile U_VI_HSYNC                 VI_P0_HSYNC;
    volatile U_VI_PRIO_CFG              VI_PRIO_CFG;            //0x900600f8
    volatile U_VI_FIR_COEF              VI0_LUMA_COEF0;         //0x900600fc
    volatile U_VI_FIR_COEF              VI0_LUMA_COEF1;
    volatile U_VI_FIR_COEF              VI0_LUMA_COEF2;
    volatile U_VI_FIR_COEF              VI0_LUMA_COEF3;
    volatile U_VI_FIR_COEF              VI0_CHROMA_COEF0;
    volatile U_VI_FIR_COEF              VI0_CHROMA_COEF1;
    
    volatile HI_U32                     u32Reservedx_0[955];    //0x90060114--0x90060ffc
    
    volatile U_VI_PORT_CFG              VI1_PORT_CFG;          //0x60131000
    volatile U_VI_CH_CFG                VI1_CH_CFG;
    volatile U_VI_CH_CTRL               VI1_CH_CTRL;
    volatile U_VI_REG_NEWER             VI1_REG_NEWER;
    volatile U_VI_CAP_START             VI1_CAP_START;
    volatile U_VI_CAP_SIZE              VI1_CAP_SIZE;
    volatile U_VI_STORESIZE             VI1_Y_STORESIZE;
    volatile U_VI_STORESIZE             VI1_U_STORESIZE;
    volatile U_VI_STORESIZE             VI1_V_STORESIZE;
    volatile U_VI_LINE_OFFSET           VI1_LINE_OFFSET;
    volatile U_VI_BASE_ADDR             VI1_Y_BASE_ADDR;
    volatile U_VI_BASE_ADDR             VI1_U_BASE_ADDR;
    volatile U_VI_BASE_ADDR             VI1_V_BASE_ADDR;
    volatile HI_U32                     u32Reserved1_1[1];
    volatile U_VI_INT_EN                VI1_INT_EN;
    volatile U_VI_INT_STATUS            VI1_INT_STATUS;
    volatile U_VI_RAW_INT               VI1_RAW_INT;
    volatile HI_U32                     u32Reserved1_2[1];
    volatile HI_U32                     u32Reserved1_3[1];
    volatile U_VI_STATUS                VI1_STATUS;
    volatile U_VI_LUM_ADDER             VI1_LUM_ADDER;
    volatile U_VI_LUM_STRH              VI1_LUM_STRH;
    volatile U_VI_LUM_DIFF_ADDER        VI1_LUM_DIFF_ADDER;
    volatile U_VI_BLOCK_START           VI1_BLOCK0_START;
    volatile U_VI_BLOCK_START           VI1_BLOCK1_START;
    volatile U_VI_BLOCK_START           VI1_BLOCK2_START;
    volatile U_VI_BLOCK_START           VI1_BLOCK3_START;
    volatile U_VI_BLOCK_SIZE            VI1_BLOCK0_SIZE;
    volatile U_VI_BLOCK_SIZE            VI1_BLOCK1_SIZE;
    volatile U_VI_BLOCK_SIZE            VI1_BLOCK2_SIZE;
    volatile U_VI_BLOCK_SIZE            VI1_BLOCK3_SIZE;
    volatile U_VI_BLOCK_COLOR           VI1_BLOCK0_COLOR;
    volatile U_VI_BLOCK_COLOR           VI1_BLOCK1_COLOR;
    volatile U_VI_BLOCK_COLOR           VI1_BLOCK2_COLOR;
    volatile U_VI_BLOCK_COLOR           VI1_BLOCK3_COLOR;
    volatile U_VI_BLOCK_MSC_SIZE        VI1_BLOCK0_MSC_SIZE;
    volatile U_VI_BLOCK_MSC_SIZE        VI1_BLOCK1_MSC_SIZE;
    volatile U_VI_BLOCK_MSC_SIZE        VI1_BLOCK2_MSC_SIZE;
    volatile U_VI_BLOCK_MSC_SIZE        VI1_BLOCK3_MSC_SIZE;
    volatile U_VI_ANC_START             VI1_ANC0_START;
    volatile U_VI_ANC_SIZE              VI1_ANC0_SIZE;
    volatile U_VI_ANC_START             VI1_ANC1_START;
    volatile U_VI_ANC_SIZE              VI1_ANC1_SIZE;
    volatile U_VI_ANC_WORD              VI1_ANC0_WORD[8];
    volatile U_VI_ANC_WORD              VI1_ANC1_WORD[8];
    volatile HI_U32                     u32Reserved1_4[4];      //0x900610ec
    volatile U_VI_FIR_COEF              VI1_LUMA_COEF0;         //0x900610fc
    volatile U_VI_FIR_COEF              VI1_LUMA_COEF1;
    volatile U_VI_FIR_COEF              VI1_LUMA_COEF2;
    volatile U_VI_FIR_COEF              VI1_LUMA_COEF3;
    volatile U_VI_FIR_COEF              VI1_CHROMA_COEF0;
    volatile U_VI_FIR_COEF              VI1_CHROMA_COEF1;

    volatile HI_U32                     u32Reservedx_1[955];    //0x90061114--0x90061ffc
        
    volatile U_VI_PORT_CFG              VI1_PORT_CFG_Xxx;          //0x60131000
    volatile U_VI_CH_CFG                VI2_CH_CFG;
    volatile U_VI_CH_CTRL               VI2_CH_CTRL;
    volatile U_VI_REG_NEWER             VI2_REG_NEWER;
    volatile U_VI_CAP_START             VI2_CAP_START;
    volatile U_VI_CAP_SIZE              VI2_CAP_SIZE;
    volatile U_VI_STORESIZE             VI2_Y_STORESIZE;
    volatile U_VI_STORESIZE             VI2_U_STORESIZE;
    volatile U_VI_STORESIZE             VI2_V_STORESIZE;
    volatile U_VI_LINE_OFFSET           VI2_LINE_OFFSET;
    volatile U_VI_BASE_ADDR             VI2_Y_BASE_ADDR;
    volatile U_VI_BASE_ADDR             VI2_U_BASE_ADDR;
    volatile U_VI_BASE_ADDR             VI2_V_BASE_ADDR;
    volatile HI_U32                     u32Reserved2_1[1];
    volatile U_VI_INT_EN                VI2_INT_EN;
    volatile U_VI_INT_STATUS            VI2_INT_STATUS;
    volatile U_VI_RAW_INT               VI2_RAW_INT;
    volatile HI_U32                     u32Reserved2_2[1];
    volatile HI_U32                     u32Reserved2_3[1];
    volatile U_VI_STATUS                VI2_STATUS;
    volatile U_VI_LUM_ADDER             VI2_LUM_ADDER;
    volatile U_VI_LUM_STRH              VI2_LUM_STRH;
    volatile U_VI_LUM_DIFF_ADDER        VI2_LUM_DIFF_ADDER;
    volatile U_VI_BLOCK_START           VI2_BLOCK0_START;
    volatile U_VI_BLOCK_START           VI2_BLOCK1_START;
    volatile U_VI_BLOCK_START           VI2_BLOCK2_START;
    volatile U_VI_BLOCK_START           VI2_BLOCK3_START;
    volatile U_VI_BLOCK_SIZE            VI2_BLOCK0_SIZE;
    volatile U_VI_BLOCK_SIZE            VI2_BLOCK1_SIZE;
    volatile U_VI_BLOCK_SIZE            VI2_BLOCK2_SIZE;
    volatile U_VI_BLOCK_SIZE            VI2_BLOCK3_SIZE;
    volatile U_VI_BLOCK_COLOR           VI2_BLOCK0_COLOR;
    volatile U_VI_BLOCK_COLOR           VI2_BLOCK1_COLOR;
    volatile U_VI_BLOCK_COLOR           VI2_BLOCK2_COLOR;
    volatile U_VI_BLOCK_COLOR           VI2_BLOCK3_COLOR;
    volatile U_VI_BLOCK_MSC_SIZE        VI2_BLOCK0_MSC_SIZE;
    volatile U_VI_BLOCK_MSC_SIZE        VI2_BLOCK1_MSC_SIZE;
    volatile U_VI_BLOCK_MSC_SIZE        VI2_BLOCK2_MSC_SIZE;
    volatile U_VI_BLOCK_MSC_SIZE        VI2_BLOCK3_MSC_SIZE;
    volatile U_VI_ANC_START             VI2_ANC0_START;
    volatile U_VI_ANC_SIZE              VI2_ANC0_SIZE;
    volatile U_VI_ANC_START             VI2_ANC1_START;
    volatile U_VI_ANC_SIZE              VI2_ANC1_SIZE;
    volatile U_VI_ANC_WORD              VI2_ANC0_WORD[8];
    volatile U_VI_ANC_WORD              VI2_ANC1_WORD[8];
    volatile HI_U32                     u32Reserved2_4[4];      //0x900620ec
    volatile U_VI_FIR_COEF              VI2_LUMA_COEF0;         //0x900620fc
    volatile U_VI_FIR_COEF              VI2_LUMA_COEF1;
    volatile U_VI_FIR_COEF              VI2_LUMA_COEF2;
    volatile U_VI_FIR_COEF              VI2_LUMA_COEF3;
    volatile U_VI_FIR_COEF              VI2_CHROMA_COEF0;
    volatile U_VI_FIR_COEF              VI2_CHROMA_COEF1;
    
    volatile HI_U32                     u32Reservedx_2[955];    //0x90062114--0x90062ffc
 } S_VIU_REGS_TYPE;

// Declare the struct pointor of the module VIU
extern volatile S_VIU_REGS_TYPE *gopVIUAllReg;

#endif // __VIU_REG_H__

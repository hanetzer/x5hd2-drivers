#ifndef __HI_ADSP_PRIVATE_H__
#define __HI_ADSP_PRIVATE_H__

#include "hi_type.h"
#include "hi_module.h"
#include "drv_sys_ext.h"
#include "drv_mmz_ext.h"
#include "drv_mem_ext.h"
#include "drv_proc_ext.h"
#include "drv_stat_ext.h"
#include "drv_module_ext.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

#define HI_FATAL_ADSP(fmt...) \
    HI_FATAL_PRINT(HI_ID_ADSP, fmt)

#define HI_ERR_ADSP(fmt...) \
    HI_ERR_PRINT(HI_ID_ADSP, fmt)

#define HI_WARN_ADSP(fmt...) \
    HI_WARN_PRINT(HI_ID_ADSP, fmt)

#define HI_INFO_ADSP(fmt...) \
    HI_INFO_PRINT(HI_ID_ADSP, fmt)

typedef struct tagADSP_REGISTER_PARAM_S
{
    DRV_PROC_READ_FN  pfnReadProc;
    DRV_PROC_WRITE_FN pfnWriteProc;
} ADSP_REGISTER_PARAM_S;

#if defined(HI_SND_DRV_SUSPEND_SUPPORT)
typedef struct
{
    /*  AOE  */
    HI_U32  u32ComPhyValue;
    HI_U32  u32ComVirValue;

    /* ADE */

} ADSP_SETTINGS_S;
#endif




// Define the union U_DSP0_CTRL
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    runstall_dsp0         : 1   ; // [0] 
        unsigned int    statvectorsel_dsp0    : 1   ; // [1] 
        unsigned int    ocdhaltonreset_dsp0   : 1   ; // [2] 
        unsigned int    Reserved_102          : 1   ; // [3] 
        unsigned int    wdg1_en_dsp0          : 1   ; // [4] 
        unsigned int    wdg2_en_dsp1          : 1   ; // [5] 
        unsigned int    Reserved_101          : 26  ; // [31..6] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DSP0_CTRL;
// Define the union U_DSP0_PRID
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    prid_dsp0             : 2   ; // [1..0] 
        unsigned int    Reserved_104          : 30  ; // [31..2] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DSP0_PRID;
// Define the union U_DSP0_STATUS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    pwaitmode_dsp0        : 1   ; // [0] 
        unsigned int    xocdmode_dsp0         : 1   ; // [1] 
        unsigned int    Reserved_106          : 30  ; // [31..2] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DSP0_STATUS;

// Define the union U_DSP1_CTRL
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    runstall_dsp1         : 1   ; // [0] 
        unsigned int    statvectorsel_dsp1    : 1   ; // [1] 
        unsigned int    ocdhaltonreset_dsp1   : 1   ; // [2] 
        unsigned int    Reserved_110          : 29  ; // [31..3] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DSP1_CTRL;

// Define the union U_DSP1_PRID
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    prid_dsp1             : 2   ; // [1..0] 
        unsigned int    Reserved_112          : 30  ; // [31..2] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DSP1_PRID;
// Define the union U_DSP1_STATUS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    pwaitmode_dsp1        : 1   ; // [0] 
        unsigned int    xocdmode_dsp1         : 1   ; // [1] 
        unsigned int    Reserved_114          : 30  ; // [31..2] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DSP1_STATUS;


// Define the global struct
typedef struct
{
    volatile U_DSP0_CTRL                DSP0_CTRL;
    volatile U_DSP0_PRID                DSP0_PRID;
    volatile U_DSP0_STATUS            DSP0_STATUS;

    volatile U_DSP1_CTRL                DSP1_CTRL;
    volatile U_DSP1_PRID                DSP1_PRID;
    volatile U_DSP1_STATUS            DSP1_STATUS;
} S_DSP_CTL;

// Define the union U_DSP_CRG
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    dsp_cken   		  : 1   ; // [0] 
        unsigned int    dsp0_cken    	  : 1   ; // [1] 
        unsigned int    dsp1_cken   	  : 1   ; // [2] 
        unsigned int    reserved_1        : 1   ; // [3] 
        unsigned int    dsp_srst_req      : 1   ; // [4] 
        unsigned int    dsp0_srst_req     : 1   ; // [5] 
        unsigned int    dsp1_srst_req     : 1   ; // [6] 
        unsigned int    reserved_2        : 1   ; // [7] 
        unsigned int    dsp_clk_sel       : 2   ; // [9] 

        unsigned int    Reserved_3        : 22  ; // [31..10] 
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_DSP_CRG;

// Define the global struct
typedef struct
{
    volatile U_DSP_CRG                DSP_CRG;
    
} S_DSP_SYS_CRG;


HI_S32	ADSP_DRV_Init(HI_VOID);
HI_VOID ADSP_DRV_Exit(HI_VOID);
HI_S32	ADSP_DRV_Open(struct inode *inode, struct file  *filp);
HI_S32	ADSP_DRV_Release(struct inode *inode, struct file  *filp);
HI_S32	ADSP_DRV_RegisterProc(ADSP_REGISTER_PARAM_S *pstParam);
HI_VOID ADSP_DRV_UnregisterProc(HI_VOID);
HI_S32	ADSP_DRV_Suspend(PM_BASEDEV_S *pdev, pm_message_t state);
HI_S32	ADSP_DRV_Resume(PM_BASEDEV_S *pdev);

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif /* __HI_ADSP_PRIVATE_H__ */


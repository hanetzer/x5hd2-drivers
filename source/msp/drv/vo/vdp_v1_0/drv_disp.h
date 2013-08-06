/******************************************************************************
 *  Copyright (C), 2007-2010, Hisilicon Technologies Co., Ltd.
 *
 *******************************************************************************
 * File name     : drv_disp.h
 * Version       : 1.0
 * Author        : xxx
 * Created       : 2012-09-19
 * Last Modified : 
 * Description   :  
 * Function List : 
 * History       : 
 * 1 Date        : 
 * Author        : xxx
 * Modification  : Create file
 *******************************************************************************/
#ifndef __DRV_DISP_H__
#define __DRV_DISP_H__

#include "drv_proc_ext.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */


typedef struct hiDISP_PROC_FN_S
{
    DRV_PROC_READ_FN  rdproc;
    DRV_PROC_WRITE_FN wtproc;
}DISP_PROC_FN_S;

typedef struct hiWIN_PROC_FN_S{
    DRV_PROC_READ_FN  rdProc;
    DRV_PROC_WRITE_FN wtProc;
    DRV_PROC_READ_FN  winRdProc;
    DRV_PROC_WRITE_FN winWtProc;
}WIN_PROC_FN_S;





#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /*  __DRV_DISP_H__  */


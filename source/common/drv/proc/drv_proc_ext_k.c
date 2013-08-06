#include <linux/kernel.h>
#include <linux/module.h>
#include "hi_type.h"
#include "drv_proc_ext.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* End of #ifdef __cplusplus */

static  DRV_PROC_INTFPARAM  *procIntfParam = NULL;

HI_S32 HI_DRV_PROC_RegisterParam(DRV_PROC_INTFPARAM *param)
{
	if (NULL == param)
	{
		//printk("CMPI_PROC_Register param err! \n");
		return HI_FAILURE;
	}
	if( (param->addModulefun == NULL) || 
		(param->rmvModulefun == NULL))
	{	
		//printk("CMPI_PROC_Register param err! \n");
		return HI_FAILURE;
	}
	procIntfParam = param;
    return HI_SUCCESS;
}

HI_VOID HI_DRV_PROC_UnRegisterParam(HI_VOID)
{
	procIntfParam = NULL;
	return;
}

DRV_PROC_ITEM_S *HI_DRV_PROC_AddModule(char *entry_name, DRV_PROC_SHOW show, void * data)
{
    DRV_PROC_EX_S stFnOpt = {0};

    stFnOpt.fnRead = show;
    
	if(procIntfParam){
		if(procIntfParam->addModulefun){
			return procIntfParam->addModulefun(entry_name, &stFnOpt, data);
		}
	}
	return NULL;
}
DRV_PROC_ITEM_S* HI_DRV_PROC_AddModuleEx(HI_CHAR *entry_name ,DRV_PROC_EX_S* pfnOpt, HI_VOID * data)
{
	if(procIntfParam){
		if(procIntfParam->addModulefun){
			return procIntfParam->addModulefun(entry_name, pfnOpt, data);
		}
	}
	return NULL;
}

HI_VOID HI_DRV_PROC_RemoveModule(char *entry_name)
{
	if(procIntfParam){
		if(procIntfParam->rmvModulefun){
			procIntfParam->rmvModulefun(entry_name);
		}
	}
	return;
}


HI_S32 HI_DRV_PROC_KInit(void)
{
	procIntfParam = NULL;
    return HI_SUCCESS;
}

HI_VOID HI_DRV_PROC_KExit(void)
{
	if(procIntfParam){
		HI_DRV_PROC_UnRegisterParam();
	}
    return ;
}

#ifndef MODULE
EXPORT_SYMBOL(HI_DRV_PROC_RegisterParam);
EXPORT_SYMBOL(HI_DRV_PROC_UnRegisterParam);
#endif
EXPORT_SYMBOL(HI_DRV_PROC_AddModule);
EXPORT_SYMBOL(HI_DRV_PROC_AddModuleEx);
EXPORT_SYMBOL(HI_DRV_PROC_RemoveModule);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */


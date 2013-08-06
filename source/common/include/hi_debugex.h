#include "hi_common.h"

#define HI_MODULE_ID_INIT(MODULE_NAME)	\
	static HI_U32 g_u32ModuleId = HI_INVALID_MODULE_ID;	\
	static HI_S32 __attribute__((constructor(200))) init_module_id() \
	{	 \
		return HI_MODULE_RegisterByName(MODULE_NAME, &g_u32ModuleId); \
	}

#define MODULE_ID (g_u32ModuleId)

#define HI_LOG_FATAL(pszFormat...)  HI_FATAL_PRINT(MODULE_ID,pszFormat)
#define HI_LOG_ERROR(pszFormat...)  HI_ERR_PRINT(MODULE_ID,pszFormat)
#define HI_LOG_WARN(pszFormat...)   HI_WARN_PRINT(MODULE_ID,pszFormat)
#define HI_LOG_DEBUG(pszFormat...)  HI_DBG_PRINT(MODULE_ID,pszFormat)
#define HI_LOG_INFO(pszFormat...)   HI_INFO_PRINT(MODULE_ID,pszFormat)

#define HI_MODULE_MALLOC(u32Size)   HI_MALLOC(MODULE_ID, u32Size)
#define HI_MODULE_FREE(pMemAddr)    HI_FREE(MODULE_ID, pMemAddr)

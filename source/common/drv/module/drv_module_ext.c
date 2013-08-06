#include <linux/module.h>
#include <linux/slab.h>
#include <linux/sched.h>

#include "hi_type.h"
#include "hi_debug.h"
#include "hi_module.h"

#include "drv_mem_ext.h"

#include "drv_proc_ext.h"
#include "drv_dev_ext.h"

#include "drv_module_ioctl.h"

#include "drv_mutils.h"

#include "drv_module_ext.h"
#include "drv_mmgr.h"
#include "drv_module_ext.h"
#include "drv_mem_ext.h"

#ifdef CMN_MMGR_SUPPORT
#include "drv_mem.h"
#endif


static UMAP_DEVICE_S    g_stModuleDev = {{0}};

static HI_U32           g_ModuleModInit = 0;
static MODULE_S*        g_pstKModuleHeader = NULL;

#define MODULE_PROC_SUPPORT

#define COLOR_START      "\33[32m"
#define COLOR_START_HEAD "\33[35m" /* pink text color*/

#define COLOR_START_RED "\33[31m"  /* red text color */
#define COLOR_END       "\33[0m"

#define KERNEL_MODE   "kernel"
#if 0
#define THIS_ERR_PRINT( fmt, arg...) printk(COLOR_START_RED);printk("<%s:%s>: ", KERNEL_MODE, __func__);printk(fmt, ##arg);printk(COLOR_END)
#define THIS_INFO_PRINT(fmt, arg...) printk(COLOR_START);    printk("<%s:%s>: ", KERNEL_MODE, __func__);printk(fmt, ##arg);printk(COLOR_END)
#else
#define THIS_ERR_PRINT( fmt, arg...)
#define THIS_INFO_PRINT(fmt, arg...)
#endif

#define MIN_LEN(x, y) ( (x) > (y) ? (y) : (x) )

#define KMODULE_BASE_ID (HI_DEBUG_ID_BUTT)

// TODO: check
#define MIN_USER_MODULE_NUMBER 200

HI_HANDLE              g_hKModuleMgr = 0;

DECLARE_MUTEX(g_KModuleMgrMutex);

#define kmodule_mgr_lock(RET)                                   \
do{                                                             \
    HI_S32 s32LockRet = down_interruptible(&g_KModuleMgrMutex); \
    if ( s32LockRet != 0 )                                      \
    {                                                           \
        return RET;                                             \
    }                                                           \
}while(0)
#define kmodule_mgr_unlock() up(&g_KModuleMgrMutex)

static HI_S32 ModuleMgr_Link_DelNode(MODULE_S* pNodeHeader, MODULE_S* pstNode);
/*
HI_U32 ModuleMgr_RegisterID(const HI_U8* pu8Name)
{
    KMD5_CTX md5c = {{0}};
    HI_U32 u32Result = 0;
    HI_U8 u8Md5Result[16] = {0};
    HI_U8 u8BufResult[16]={'\0'};
    HI_U8 u8Tmp[3]={'\0'};
    HI_U8 i = 0;

    THIS_INFO_PRINT("to md5 name is %s\n", pu8Name);

    KMD5Init( &md5c );
    KMD5Update( &md5c, (unsigned char*)pu8Name, strlen(pu8Name) );
    KMD5Final( u8Md5Result, &md5c );

    //Just only make the first four bytes available.
    for( i=0; i<4; i++ )
    {
        sprintf(u8Tmp,"%02X", u8Md5Result[i] );
        strcat(u8BufResult,u8Tmp);
    }

    THIS_INFO_PRINT("u8BufResult is %s\n", u8BufResult);

    u32Result = (HI_U32)simple_strtoll(u8BufResult, NULL, 16) + KMODULE_BASE_ID;

    u32Result &= 0x7FFFFFFF;

    THIS_INFO_PRINT("u32Result 2 is 0x%08x\n", u32Result);

    return u32Result;
}
//*/

static HI_S32 ModuleMgr_FindNodeByName(MODULE_S* pNodeHeader, MODULE_S** pstNode, const HI_U8* pu8ModuleName)
{
    MODULE_S* pItrNode = NULL;
    HI_S32 s32CmpResult = 0;

    pItrNode = pNodeHeader;

    if (NULL == pu8ModuleName)
    {
        THIS_ERR_PRINT("param invalid!\n");

        return HI_FAILURE;
    }

    while (NULL != pItrNode)
    {
        s32CmpResult = strcmp(pItrNode->stModuleInfo.u8ModuleName, pu8ModuleName);
        if ( s32CmpResult == 0)
        {
            //find out
            *pstNode = pItrNode;

            break;
        }

        pItrNode = pItrNode->pNextModule;
    }

    if (NULL != pItrNode)
    {
        THIS_INFO_PRINT("found out the module name:%s node\n", pu8ModuleName);

        return HI_SUCCESS;
    }

    THIS_ERR_PRINT("not found out the module name:%s node\n", pu8ModuleName);

    return HI_FAILURE;
}

static HI_S32 ModuleMgr_FindNodeByID(MODULE_S* pNodeHeader, MODULE_S** pstNode, HI_U32 u32ModuleID)
{
    MODULE_S* pItrNode = NULL;

    pItrNode = pNodeHeader;

    while (NULL != pItrNode)
    {
        if ( u32ModuleID == pItrNode->stModuleInfo.u32ModuleID)
        {
            //find out
            *pstNode = pItrNode;

            break;
        }

        pItrNode = pItrNode->pNextModule;
    }

    if (NULL != pItrNode)
    {
        THIS_INFO_PRINT("found out the module id:0x%08x node\n", u32ModuleID);

        return HI_SUCCESS;
    }

    THIS_ERR_PRINT("not found out the module id:0x%08x node\n", u32ModuleID);

    return HI_FAILURE;

}

static HI_U32 ModuleMgr_AllocModuleId(HI_VOID)
{
    HI_U32 u32ModuleId;
    MODULE_S* pstFindMoudle;

    for (u32ModuleId=MIN_USER_MODULE_NUMBER; u32ModuleId<MIN_USER_MODULE_NUMBER+HI_MAX_USER_MODULE_NUMBER; u32ModuleId++)
    {
        if (HI_SUCCESS != ModuleMgr_FindNodeByID(g_pstKModuleHeader, &pstFindMoudle, u32ModuleId))
        {
            return u32ModuleId;
        }
    }

    return HI_INVALID_MODULE_ID;
}

static HI_S32 ModuleMgr_Link_Init(HI_U32 u32Count)
{
    KMEM_UTILS_S stModulePool = {0};

    if (NULL != g_pstKModuleHeader)
    {
        THIS_ERR_PRINT("Init has been called.\n");

        return HI_SUCCESS;
    }

    g_pstKModuleHeader = (MODULE_S*)kmalloc(sizeof(MODULE_S), GFP_KERNEL);
    if (NULL == g_pstKModuleHeader)
    {
        THIS_ERR_PRINT("kmalloc size %d failure\n", sizeof(MODULE_S));
        return HI_FAILURE;
    }

    memset(g_pstKModuleHeader, 0, sizeof(MODULE_S));

    stModulePool.enType = KMEM_POOL_TYPE_MODULE;

    g_hKModuleMgr = KMem_Utils_Init(u32Count, stModulePool);

    return HI_SUCCESS;
}

static HI_S32 ModuleMgr_Link_DeInit(HI_VOID)
{
    ModuleMgr_Link_DelNode(g_pstKModuleHeader, NULL);

    KMem_Utils_DeInit(g_hKModuleMgr);

    if (NULL != g_pstKModuleHeader)
    {
        kfree(g_pstKModuleHeader);
    }

    return HI_SUCCESS;
}

static MODULE_S* ModuleMgr_Link_HasNode(MODULE_S* pNodeHeader, MODULE_S* pstNode)
{
    MODULE_S* pItrNode = pNodeHeader;

    if (pNodeHeader == NULL)
    {
        return NULL;
    }

    pItrNode = pNodeHeader->pNextModule;

    while (NULL != pItrNode)
    {
        if (pItrNode->stModuleInfo.u32ModuleID == pstNode->stModuleInfo.u32ModuleID)
        {
            break;
        }

        pItrNode = pItrNode->pNextModule;
    }

    if (pItrNode != NULL)
    {
        return pItrNode;
    }

    return NULL;
}

// Make one node and add it to the link tail
static HI_S32 ModuleMgr_Link_AddNode(MODULE_S* pNodeHeader, MODULE_S* pstNode)
{
    MODULE_S* pItrNode = NULL;
    MODULE_S* pstAddNode = NULL;

    if (NULL == pNodeHeader)
    {
        THIS_ERR_PRINT("add node failure, node header is NULL\n");

        return HI_FAILURE;
    }

    pItrNode = ModuleMgr_Link_HasNode(pNodeHeader, pstNode);
    if (pItrNode)
    {
        if (!pItrNode->stModuleInfo.pFnCallback)
        {
            pItrNode->stModuleInfo.pFnCallback = pstNode->stModuleInfo.pFnCallback;
        }
        //has added it, so return success, directly
        THIS_INFO_PRINT("has node %s in link\n" , pstNode->stModuleInfo.u8ModuleName);
        return HI_SUCCESS;
    }

    pstAddNode = (MODULE_S*)KMem_Utils_MALLOC(g_hKModuleMgr);
    if ( NULL == pstAddNode)
    {

        THIS_ERR_PRINT("add node failure, malloc node failure.\n");
        return HI_FAILURE;
    }

    memset(pstAddNode, 0, sizeof(MODULE_S));

    THIS_INFO_PRINT("add module name %s at address %p\n", pstNode->stModuleInfo.u8ModuleName, pstAddNode);

    memcpy(pstAddNode->stModuleInfo.u8ModuleName, pstNode->stModuleInfo.u8ModuleName, sizeof(pstAddNode->stModuleInfo.u8ModuleName));
    pstAddNode->stModuleInfo.u32ModuleID = pstNode->stModuleInfo.u32ModuleID;
    pstAddNode->stModuleInfo.pFnCallback = pstNode->stModuleInfo.pFnCallback;
    pstAddNode->pNextModule = NULL;

    pItrNode = pNodeHeader;
    pNodeHeader->u32ItemCnt++;

    while ( NULL != pItrNode->pNextModule )
    {
        pItrNode = pItrNode->pNextModule;
    }

    pItrNode->pNextModule = pstAddNode;

    return HI_SUCCESS;
}

// Delete the node from link
static HI_S32 ModuleMgr_Link_DelNode(MODULE_S* pNodeHeader, MODULE_S* pstNode)
{
    MODULE_S* pItrNode = NULL;
    MODULE_S* pDelNode = NULL;

    if (NULL == pNodeHeader || pNodeHeader->pNextModule == NULL)
    {
        THIS_ERR_PRINT("delete node failure, node header is NULL\n");

        return HI_FAILURE;
    }

    pItrNode = pNodeHeader;

    //if pstNode is NULL, delete all the node, exclusive the header node;
    if (NULL == pstNode)
    {
        pItrNode = pItrNode->pNextModule;

        while ( NULL != pItrNode )
        {
            pDelNode = pItrNode;
            pItrNode = pItrNode->pNextModule;

            THIS_INFO_PRINT("delete module node:%s\n", pDelNode->stModuleInfo.u8ModuleName);

            KMem_Utils_FREE(g_hKModuleMgr, pDelNode);
        }

        pNodeHeader->pNextModule = NULL;

        return HI_SUCCESS;
    }

    while ( NULL != pItrNode->pNextModule)
    {
        if (pItrNode->pNextModule->stModuleInfo.u32ModuleID == pstNode->stModuleInfo.u32ModuleID)
        {
            break;
        }
        pItrNode = pItrNode->pNextModule;
    }

    if (NULL != pItrNode && NULL != pItrNode->pNextModule)
    {
        //found out the next node to delete.
        pDelNode = pItrNode->pNextModule;
        pItrNode->pNextModule = pItrNode->pNextModule->pNextModule;

        THIS_INFO_PRINT("delete module node:%s\n", pDelNode->stModuleInfo.u8ModuleName);

        KMem_Utils_FREE(g_hKModuleMgr, pDelNode);

        pNodeHeader->u32ItemCnt--;

        return HI_SUCCESS;
    }

    return HI_FAILURE;
}

static HI_S32 CMPI_Module_Ioctl(struct inode *inode, struct file *file,HI_U32 cmd, HI_VOID *arg)
{
    HI_S32 s32Ret = HI_FAILURE;

    switch(cmd)
    {
        case CMD_ADD_MODULE_INFO:
        {
            MODULE_INFO_S *pModule = (MODULE_INFO_S*)arg;

            if (HI_INVALID_MODULE_ID != pModule->u32ModuleID)
            {
                s32Ret = HI_DRV_MODULE_Register(pModule->u32ModuleID, pModule->u8ModuleName, pModule->pFnCallback);
            }

            THIS_INFO_PRINT("add module:%s, id %#x!\n", pModule->u8ModuleName, pModule->u32ModuleID);
        }
        break;
        case CMD_GET_MODULE_INFO:
        {
            MODULE_INFO_S *pModule = (MODULE_INFO_S*)arg;
            MODULE_S      *pNode = NULL;

            if (NULL != pModule)
            {
                if (pModule->u32ModuleID != 0)
                {
                    s32Ret = ModuleMgr_FindNodeByID(g_pstKModuleHeader, &pNode, pModule->u32ModuleID);
                }
                else
                {
                    s32Ret = ModuleMgr_FindNodeByName(g_pstKModuleHeader, &pNode, pModule->u8ModuleName);
                }

                if (HI_SUCCESS == s32Ret)
                {
                    HI_U32 u32MinLen = 0;

                    pModule->u32ModuleID = pNode->stModuleInfo.u32ModuleID;
                    pModule->pFnCallback = pNode->stModuleInfo.pFnCallback;

                    u32MinLen = MIN_LEN( sizeof(pModule->u8ModuleName)-1, strlen(pNode->stModuleInfo.u8ModuleName));
                    memcpy(pModule->u8ModuleName, pNode->stModuleInfo.u8ModuleName, u32MinLen);

                    THIS_INFO_PRINT("get module:%s, id %#x!\n", pModule->u8ModuleName, pModule->u32ModuleID);
                }
            }
        }
        break;
        case CMD_DEL_MODULE_INFO:
        {
            MODULE_INFO_S *pModule = (MODULE_INFO_S*)arg;

            s32Ret = HI_DRV_MODULE_UnRegister(pModule->u32ModuleID);
            THIS_INFO_PRINT("del module:%s, id %#x!\n", pModule->u8ModuleName, pModule->u32ModuleID);
        }
        break;
        case CMD_ALLOC_MODULE_ID:
        {
            MODULE_ALLOC_S *pstModule = (MODULE_ALLOC_S*)arg;

            pstModule->s32Status = -1;
            s32Ret = HI_DRV_MODULE_AllocId(pstModule->u8ModuleName, &(pstModule->u32ModuleID), &(pstModule->s32Status));
        }
        break;
        default:
            THIS_ERR_PRINT("================cmd:%#x\n", cmd);
            s32Ret = HI_SUCCESS;
        break;
    }

    UNUSED(file);
    UNUSED(inode);

    return s32Ret;
}

static long MODULE_DRV_Ioctl(struct file *file,
                            HI_U32 cmd,
                            unsigned long arg)
{
    int ret;
    ret=HI_DRV_UserCopy(file->f_dentry->d_inode, file, cmd, arg, CMPI_Module_Ioctl);

    return ret;
}

static HI_S32 MODULE_DRV_Release(struct inode * inode, struct file * file)
{
    return 0;
}

static HI_S32 MODULE_DRV_Open(struct inode * inode, struct file * file)
{
    return 0;
}

static struct file_operations DRV_module_Fops=
{
    .owner          =THIS_MODULE,
    .open           =MODULE_DRV_Open,
    .unlocked_ioctl =MODULE_DRV_Ioctl,
    .release        =MODULE_DRV_Release,
};

#ifdef MODULE_PROC_SUPPORT
HI_S32 ModuleProcWrite( struct file * file,  const char __user * buf,
                     size_t count, loff_t *ppos)
{
    return 0;
}

#define SPLIT_LINE "--------------------------------------------------------\n"

extern HI_S32 LOG_GetLevel(HI_U32 enModId, HI_U8* u8Buf, HI_U32 u32Length);

#ifdef CMN_MMGR_SUPPORT
HI_U32 KModule_MemMgr_GetUsedSize(HI_U32 u32ModuleID);
#endif

HI_S32 ModuleProcRead(struct seq_file *s, HI_VOID *pArg)
{
    HI_U32 u32Count = 0;
    MODULE_S* pItr = NULL;
    HI_U8  u8LevelName[8] = {0};
    HI_U32 u32LogLevel = 0;

#ifdef CMN_MMGR_SUPPORT
    HI_U32 u32TotalSize = 0;
    HI_U32 u32Size = 0;
#endif

    if (0 == g_ModuleModInit)
    {
        seq_printf(s,"    Module module not init\n");
        return 0;
    }

    kmodule_mgr_lock(HI_FAILURE);

    seq_printf(s, COLOR_START_HEAD);
    seq_printf(s, SPLIT_LINE);
    seq_printf(s, "|  Module Name  |     ID      | Log Level | Heap Memory |\n");
    seq_printf(s, SPLIT_LINE);
    seq_printf(s, COLOR_END);

    if (NULL != g_pstKModuleHeader)
    {
        pItr = g_pstKModuleHeader->pNextModule;
        while (NULL != pItr)
        {
            u32LogLevel = LOG_GetLevel(pItr->stModuleInfo.u32ModuleID, u8LevelName, sizeof(u8LevelName));

            if (u32Count%2 == 0)
            {
                seq_printf(s, COLOR_START_HEAD"|"COLOR_END);
                seq_printf(s, " %-16.16s 0x%08x   %d: %-s ", pItr->stModuleInfo.u8ModuleName,  pItr->stModuleInfo.u32ModuleID, \
                                                             u32LogLevel, u8LevelName);
#ifdef CMN_MMGR_SUPPORT
                u32Size = KModule_MemMgr_GetUsedSize(pItr->stModuleInfo.u32ModuleID);
                u32TotalSize += u32Size;
                seq_printf(s, "     %-10d", u32Size);
#else
                seq_printf(s, "     %-10s", " ");
#endif
                seq_printf(s, COLOR_START_HEAD"|\n"COLOR_END);

                u32Count = 1;
            }
            else
            {
                seq_printf(s, COLOR_START_HEAD"|"COLOR_END COLOR_START_RED);
                seq_printf(s, " %-16.16s 0x%08x   %d: %-s ", pItr->stModuleInfo.u8ModuleName,  pItr->stModuleInfo.u32ModuleID, \
                                                             u32LogLevel, u8LevelName);
#ifdef CMN_MMGR_SUPPORT
                u32Size = KModule_MemMgr_GetUsedSize(pItr->stModuleInfo.u32ModuleID);
                u32TotalSize += u32Size;
                seq_printf(s, "     %-10d", u32Size);
#else
                seq_printf(s, "     %-10s", " ");
#endif
                seq_printf(s, COLOR_END COLOR_START_HEAD"|"COLOR_END"\n");

                u32Count = 0;
            }

            pItr = pItr->pNextModule;
        }
    }

    seq_printf(s, COLOR_START_HEAD);
    seq_printf(s, SPLIT_LINE);

#ifdef CMN_MMGR_SUPPORT
    seq_printf(s, "| %-43s %-10d|\n", "Total", u32TotalSize);
    seq_printf(s, SPLIT_LINE);
#endif

    seq_printf(s, COLOR_END);

    kmodule_mgr_unlock();

    return 0;
}
#endif

HI_S32 HI_DRV_MMNGR_Init(HI_U32 u32ModuleCount, HI_U32 u32ModuleMemCount)
{
    ModuleMgr_Link_Init(u32ModuleCount);

    return HI_SUCCESS;
}

HI_S32 MMNGR_DRV_ModInit(HI_U32 u32ModuleCount, HI_U32 u32ModuleMemCount)
{
    DRV_PROC_ITEM_S *item ;

    if (g_ModuleModInit)
    {
        return HI_SUCCESS;
    }

    HI_DRV_MODULE_Register(HI_ID_SYS,    "HI_SYS",      HI_NULL);
    HI_DRV_MODULE_Register(HI_ID_MODULE, "HI_MODULE",   HI_NULL);
    HI_DRV_MODULE_Register(HI_ID_LOG,    "HI_LOG",      HI_NULL);
    HI_DRV_MODULE_Register(HI_ID_PROC,   "HI_PROC",     HI_NULL);
    HI_DRV_MODULE_Register(HI_ID_STAT,   "HI_STAT",     HI_NULL);
    HI_DRV_MODULE_Register(HI_ID_MEM,    "HI_MEM",      HI_NULL);

#ifdef CMN_MMGR_SUPPORT
    KModule_MemMgr_Init(u32ModuleCount, u32ModuleMemCount);
#endif

    sprintf(g_stModuleDev.devfs_name, "%s", UMAP_DEVNAME_MODULE);
    g_stModuleDev.fops = &DRV_module_Fops;
    g_stModuleDev.minor = UMAP_MIN_MINOR_MODULE;
    g_stModuleDev.owner  = THIS_MODULE;
    g_stModuleDev.drvops = NULL;

    if(HI_DRV_DEV_Register(&g_stModuleDev) < 0)
    {
        THIS_ERR_PRINT("Unable to register dbg dev\n");
        return HI_FAILURE;
    }
    // 1
    item = HI_DRV_PROC_AddModule(HI_MOD_MODULE, NULL, NULL);
    if (! item)
    {
        HI_DRV_DEV_UnRegister(&g_stModuleDev);
        return HI_FAILURE;
    }

#ifdef MODULE_PROC_SUPPORT
    //proc read and write interface.
    item->read = ModuleProcRead;
    item->write = ModuleProcWrite;
#endif
    g_ModuleModInit = 1;

    return HI_SUCCESS;
}

HI_VOID HI_DRV_MMNGR_Exit(HI_VOID)
{
    ModuleMgr_Link_DeInit();

    return;
}

HI_VOID MMNGR_DRV_ModExit(HI_VOID)
{
    if(g_ModuleModInit == 0)
    {
        return;
    }

#ifdef CMN_MMGR_SUPPORT
    HI_DRV_MODULE_UnRegister(HI_ID_SYS);
    HI_DRV_MODULE_UnRegister(HI_ID_MEM);
    HI_DRV_MODULE_UnRegister(HI_ID_LOG);
    HI_DRV_MODULE_UnRegister(HI_ID_MODULE);
    HI_DRV_MODULE_UnRegister(HI_ID_PROC);
    HI_DRV_MODULE_UnRegister(HI_ID_STAT);
    HI_DRV_MODULE_UnRegister(HI_ID_EVENT);

    KModule_MemMgr_Exit();
#endif

    g_ModuleModInit = 0;

#ifdef MODULE_PROC_SUPPORT
    HI_DRV_PROC_RemoveModule(HI_MOD_MODULE);
    HI_DRV_DEV_UnRegister(&g_stModuleDev);
#endif

    return;
}


#ifdef CMN_MMGR_SUPPORT
HI_U8* HI_DRV_MODULE_GetNameByID(HI_U32 u32ModuleID)
{
    MODULE_S* pItr = NULL;
    HI_U32    u32Count = 0;
    HI_U32    u32Total = 0;

    kmodule_mgr_lock(NULL);

    if (NULL == g_pstKModuleHeader)
    {
        kmodule_mgr_unlock();

        return NULL;
    }

    pItr = g_pstKModuleHeader->pNextModule;
    u32Total = g_pstKModuleHeader->u32ItemCnt;

    while (NULL != pItr)
    {
        u32Count++;
        if (pItr->stModuleInfo.u32ModuleID == u32ModuleID)
        {
            break;
        }

        if (u32Count > u32Total)
        {
            pItr = NULL;
            break;
        }

        pItr = pItr->pNextModule;
    }

    if (NULL != pItr)
    {
        kmodule_mgr_unlock();

        return pItr->stModuleInfo.u8ModuleName;
    }

    kmodule_mgr_unlock();

    return NULL;
}

HI_U32 HI_DRV_MODULE_GetIDByName(HI_U8* pu8Name)
{
    MODULE_S* pItr = NULL;

    if(pu8Name == NULL)
    {
        return 0;
    }

    kmodule_mgr_lock(0);

    if (NULL == g_pstKModuleHeader)
    {
        kmodule_mgr_unlock();

        return 0;
    }

    pItr = g_pstKModuleHeader->pNextModule;

    while (NULL != pItr)
    {
        if (memcmp(pItr->stModuleInfo.u8ModuleName, pu8Name, strlen(pu8Name)) == 0)
        {
            break;
        }

        pItr = pItr->pNextModule;
    }

    if (NULL != pItr)
    {
        kmodule_mgr_unlock();

        return pItr->stModuleInfo.u32ModuleID;
    }

    kmodule_mgr_unlock();

    return 0;
}
#endif
extern HI_S32 LOGAddModule(HI_PCHAR szProcName, HI_MOD_ID_E u32It);
HI_S32 HI_DRV_MODULE_Register(HI_U32 u32ModuleID, const HI_U8* pu8ModuleName, HI_VOID* pFunc)
{
    MODULE_S stModule = {{0}};
    HI_U32   u32MinLen = 0;
    HI_S32   s32Ret = HI_FAILURE;

    if ( NULL == pu8ModuleName )
    {
        return HI_FAILURE;
    }

    //request ID by name
    //u32ModuleID = ModuleMgr_RegisterID(pu8ModuleName);

    //build module node;
    stModule.pNextModule = NULL;
    stModule.stModuleInfo.u32ModuleID = u32ModuleID;
    u32MinLen = MIN_LEN( sizeof(stModule.stModuleInfo.u8ModuleName)-1, strlen(pu8ModuleName));
    memcpy(stModule.stModuleInfo.u8ModuleName, pu8ModuleName, u32MinLen);

    stModule.stModuleInfo.pFnCallback = pFunc;

    // Add a node to into the link.
    kmodule_mgr_lock(HI_FAILURE);

    s32Ret = ModuleMgr_Link_AddNode(g_pstKModuleHeader, &stModule);
    kmodule_mgr_unlock();

    if (HI_SUCCESS == s32Ret)
    {
        //KModule_Log_Pool_AddNode(u32ModuleID);

#ifdef CMN_MMGR_SUPPORT
        KMODULE_MEM_POOL_AddModule(u32ModuleID);
#endif
        LOGAddModule((HI_PCHAR)pu8ModuleName, (HI_MOD_ID_E)u32ModuleID);
    }

    THIS_INFO_PRINT("add module:%s, id %#x!\n", stModule.stModuleInfo.u8ModuleName, stModule.stModuleInfo.u32ModuleID);

    return s32Ret;
}

HI_S32 HI_DRV_MODULE_AllocId(HI_U8* pu8ModuleName, HI_U32 *pu32ModuleID, HI_S32 *ps32Status)
{
    MODULE_S *pstFindModude = HI_NULL;
    HI_S32   s32Ret = HI_FAILURE;
    HI_CHAR  aszModuleName[MAX_MODULE_NAME*2];

    if ( NULL == pu8ModuleName )
    {
        return HI_FAILURE;
    }

    sprintf(aszModuleName, "%s_%d", pu8ModuleName, current->pid);
    aszModuleName[MAX_MODULE_NAME-1] = 0;

    kmodule_mgr_lock(HI_FAILURE);

    /* Find module */
    s32Ret = ModuleMgr_FindNodeByName(g_pstKModuleHeader, &pstFindModude, aszModuleName);
    if ((HI_SUCCESS == s32Ret) && (HI_NULL != pstFindModude))
    {
        kmodule_mgr_unlock();
        memcpy(pu8ModuleName, aszModuleName, MAX_MODULE_NAME-1);
        *pu32ModuleID = pstFindModude->stModuleInfo.u32ModuleID;
        *ps32Status = 1;
        return HI_SUCCESS;
    }

    /* Alloc module ID */
    *pu32ModuleID = ModuleMgr_AllocModuleId();

    kmodule_mgr_unlock();

    if (HI_INVALID_MODULE_ID == *pu32ModuleID)
    {
        return HI_FAILURE;
    }

    *ps32Status = 0;
    memcpy(pu8ModuleName, aszModuleName, MAX_MODULE_NAME-1);
    return HI_SUCCESS;
}

HI_S32 HI_DRV_MODULE_UnRegister(HI_U32 u32ModuleID)
{
    HI_S32 s32Ret = HI_FAILURE;
    MODULE_S stNode;

    kmodule_mgr_lock(HI_FAILURE);

    stNode.stModuleInfo.u32ModuleID = u32ModuleID;

    s32Ret = ModuleMgr_Link_DelNode(g_pstKModuleHeader, &stNode);
    if (HI_FAILURE == s32Ret)
    {
        kmodule_mgr_unlock();

        return HI_FAILURE;
    }

   // KModule_Log_Pool_DelNode(u32ModuleID);

#ifdef CMN_MMGR_SUPPORT
    KMODULE_MEM_POOL_DelModule(u32ModuleID);
#endif

    kmodule_mgr_unlock();

    return HI_SUCCESS;
}



HI_S32 HI_DRV_MODULE_GetFunction(HI_U32 u32ModuleID, HI_VOID** ppFunc)
{
    HI_S32 s32Ret = HI_FAILURE;
    MODULE_S* pMoudle = NULL;

    if (ppFunc == NULL)
    {
        THIS_ERR_PRINT("param invalid!\n");

        return HI_FAILURE;
    }

    kmodule_mgr_lock(HI_FAILURE);

    s32Ret = ModuleMgr_FindNodeByID(g_pstKModuleHeader, &pMoudle, u32ModuleID);

    kmodule_mgr_unlock();

    if (s32Ret != HI_SUCCESS)
    {
        THIS_ERR_PRINT("Not found module ID: 0x%08x\n", u32ModuleID);

        return HI_FAILURE;
    }

    *ppFunc = pMoudle->stModuleInfo.pFnCallback;

    return HI_SUCCESS;
}



EXPORT_SYMBOL(HI_DRV_MMNGR_Init);
EXPORT_SYMBOL(HI_DRV_MMNGR_Exit);

EXPORT_SYMBOL(HI_DRV_MODULE_Register);
EXPORT_SYMBOL(HI_DRV_MODULE_UnRegister);

#ifdef CMN_MMGR_SUPPORT
EXPORT_SYMBOL(HI_DRV_MODULE_GetNameByID);
EXPORT_SYMBOL(HI_DRV_MODULE_GetIDByName);
#endif

EXPORT_SYMBOL(HI_DRV_MODULE_GetFunction);


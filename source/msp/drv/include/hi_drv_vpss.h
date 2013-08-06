#ifndef __HI_DRV_VPSS_H__
#define __HI_DRV_VPSS_H__

#include "hi_type.h"
#include "hi_drv_video.h"


//================================================  接口常量 =================================================

/* 句柄定义，句柄实际上是一个整数，用于标识chan和port时数值的定义有所不同，但都以-1作为无效句柄。
      1. VPSS实例句柄，其值为实例的索引号(ID)
      2. 端口句柄，其值要表示两个内容: 所属通道索引，以及端口本身的索引。
              = 实例索引*256 + 端口索引 */
      
typedef HI_S32 VPSS_HANDLE;
#define VPSS_INVALID_HANDLE (-1)

/* 从端口句柄中提取通道索引、端口索引 */
#define PORTHANDLE_TO_VPSSID(hPort)    (hPort >> 8)
#define PORTHANDLE_TO_PORTID(hPort)    (hPort & 0xff)

#define DEF_HI_DRV_VPSS_PORT_BUFFER_MAX_NUMBER 16
#define DEF_HI_DRV_VPSS_PORT_MAX_NUMBER 3

/*前级传入的最后一帧标记*/
#define DEF_HI_DRV_VPSS_LAST_FRAME_FLAG 0xffee
/*前级传入的最后一帧错误标记*/
#define DEF_HI_DRV_VPSS_LAST_ERROR_FLAG 0xff00
//==============================================  接口数据结构 ===============================================
/* 通道的配置信息 */
typedef enum
{
    HI_DRV_VPSS_HFLIP_DISABLE = 0,
    HI_DRV_VPSS_HFLIP_ENABLE,
    HI_DRV_VPSS_HFLIP_BUTT
}HI_DRV_VPSS_HFLIP_E;

typedef enum
{
    HI_DRV_VPSS_VFLIP_DISABLE = 0,
    HI_DRV_VPSS_VFLIP_ENABLE,
    HI_DRV_VPSS_VFLIP_BUTT
}HI_DRV_VPSS_VFLIP_E;

//配置考虑用户使用
typedef enum
{
    HI_DRV_VPSS_ROTATION_DISABLE = 0,
    HI_DRV_VPSS_ROTATION_90,
    HI_DRV_VPSS_ROTATION_180,
    HI_DRV_VPSS_ROTATION_270,
    HI_DRV_VPSS_ROTATION_BUTT
}HI_DRV_VPSS_ROTATION_E;

typedef enum
{
    HI_DRV_VPSS_STEREO_DISABLE = 0,
    HI_DRV_VPSS_STEREO_SIDE_BY_SIDE,
    HI_DRV_VPSS_STEREO_TOP_AND_BOTTOM,
    HI_DRV_VPSS_STEREO_TIME_INTERLACED,
    HI_DRV_VPSS_STEREO_BUTT
}HI_DRV_VPSS_STEREO_E;

typedef enum
{
    HI_DRV_VPSS_DIE_DISABLE = 0,
    HI_DRV_VPSS_DIE_AUTO,
    HI_DRV_VPSS_DIE_2FIELD,
    HI_DRV_VPSS_DIE_3FIELD,
    HI_DRV_VPSS_DIE_4FIELD,
    HI_DRV_VPSS_DIE_5FIELD,
    HI_DRV_VPSS_DIE_6FIELD,
    HI_DRV_VPSS_DIE_7FIELD,
    HI_DRV_VPSS_DIE_BUTT
}HI_DRV_VPSS_DIE_MODE_E;

typedef enum
{
    HI_DRV_VPSS_ACC_DISABLE = 0,
    HI_DRV_VPSS_ACC_LOW,    
    HI_DRV_VPSS_ACC_MIDDLE,
    HI_DRV_VPSS_ACC_HIGH,
    HI_DRV_VPSS_ACC_BUTT
}HI_DRV_VPSS_ACC_E;

typedef enum
{
    HI_DRV_VPSS_ACM_DISABLE = 0,
    HI_DRV_VPSS_ACM_BLUE,    
    HI_DRV_VPSS_ACM_GREEN,
    HI_DRV_VPSS_ACM_BG,
    HI_DRV_VPSS_ACM_SKIN,
    HI_DRV_VPSS_ACM_BUTT  
}HI_DRV_VPSS_ACM_E;

typedef enum
{
    HI_DRV_VPSS_CC_DISABLE = 0,
    HI_DRV_VPSS_CC_ENABLE,
    HI_DRV_VPSS_CC_AUTO,     
    HI_DRV_VPSS_CC_BUTT
}HI_DRV_VPSS_CC_E;

typedef enum
{
    HI_DRV_VPSS_SHARPNESS_DISABLE = 0,
    HI_DRV_VPSS_SHARPNESS_ENABLE,
    HI_DRV_VPSS_SHARPNESS_AUTO,
    HI_DRV_VPSS_SHARPNESS_BUTT
}HI_DRV_VPSS_SHARPNESS_E;

typedef enum
{
    HI_DRV_VPSS_DB_DISABLE = 0,
    HI_DRV_VPSS_DB_ENABLE,
    HI_DRV_VPSS_DB_AUTO,     
    HI_DRV_VPSS_DB_BUTT
}HI_DRV_VPSS_DB_E;

typedef enum
{
    HI_DRV_VPSS_DR_DISABLE = 0,
    HI_DRV_VPSS_DR_ENABLE,
    HI_DRV_VPSS_DR_AUTO,     
    HI_DRV_VPSS_DR_BUTT
}HI_DRV_VPSS_DR_E;

typedef enum
{
    HI_DRV_VPSS_CSC_DISABLE = 0,
    HI_DRV_VPSS_CSC_ENABLE,   
    HI_DRV_VPSS_CSC_AUTO,     
    HI_DRV_VPSS_CSC_BUTT
}HI_DRV_VPSS_CSC_E;

typedef enum
{
    HI_DRV_VPSS_FIDELITY_DISABLE = 0,
    HI_DRV_VPSS_FIDELITY_ENABLE,  
    HI_DRV_VPSS_FIDELITY_AUTO,     
    HI_DRV_VPSS_FIDELITY_BUTT
}HI_DRV_VPSS_FIDELITY_E;


typedef struct
{
    HI_DRV_VPSS_HFLIP_E  eHFlip;
    HI_DRV_VPSS_VFLIP_E  eVFlip;
    HI_DRV_VPSS_STEREO_E eStereo;
    HI_DRV_VPSS_ROTATION_E  eRotation;
    HI_DRV_VPSS_DIE_MODE_E eDEI;
    HI_DRV_VPSS_ACC_E eACC;
    HI_DRV_VPSS_ACM_E eACM;
    HI_DRV_VPSS_CC_E eCC;
    HI_DRV_VPSS_SHARPNESS_E eSharpness;
    HI_DRV_VPSS_DB_E eDB;
    HI_DRV_VPSS_DR_E eDR;

    /*输入画面裁剪信息*/
    HI_RECT_S stInRect;  /* (0,0,0,0) means full imgae, not clip */
    HI_BOOL   bUseCropRect;
    HI_DRV_CROP_RECT_S stCropRect;
}HI_DRV_VPSS_PROCESS_S;

typedef struct
{
    HI_DRV_VPSS_CSC_E eCSC;
    /* 是否需要尽量保真，如果为TRUE，在输入输出分辨率一致的情况下走直通通道，保证指标能过 */
    HI_DRV_VPSS_FIDELITY_E eFidelity;
}HI_DRV_VPSS_PORT_PROCESS_S;


typedef struct 
{
    /*VPSS实例优先级 */
    HI_S32  s32Priority;  /* 0无效，1 ~ 31为正常优先级，数值越大优先级越高 */

    /*以下为可动态配置项*/
    
    /* 是否只处理最新一帧，在低延迟模式下置为TRUE，这样每次都会读空Src buf，只留最后一帧处理 */
    HI_BOOL bAlwaysFlushSrc;

    /* 公共(与port无关的)配置，如算法等 */
    HI_DRV_VPSS_PROCESS_S stProcCtrl;

}HI_DRV_VPSS_CFG_S;

/*
BUFFER管理有三种模式:
1.USER ALLOC AND MANAGE
    VPSS内部对帧存不做管理，每次写出帧存时向用户上报事件 VPSS_EVENT_GET_FRMBUFFER获取帧存。
                            图像处理完成时，向用户上报事件VPSS_EVENT_NEW_FRAME释放帧存
2.VPSS ALLOC AND MANAGE
    VPSS内部对帧存做管理
3.USER ALLOC AND VPSS MANAGE
    用户分配帧存后，一次性发给VPSS使用
*/
typedef enum hiDRV_VPSS_BUFFER_TYPE_E{
    HI_DRV_VPSS_BUF_VPSS_ALLOC_MANAGE = 0,
    HI_DRV_VPSS_BUF_USER_ALLOC_MANAGE,
    HI_DRV_VPSS_BUF_USER_ALLOC_VPSS_MANAGE,
    HI_DRV_VPSS_BUF_TYPE_BUTT
}HI_DRV_VPSS_BUFFER_TYPE_E;

typedef struct hiDRV_VPSS_BUFFER_CFG_S
{
    HI_DRV_VPSS_BUFFER_TYPE_E eBufType;
    
    HI_U32 u32BufNumber;     /* bBufferNumber must be <= DEF_HI_DRV_VPSS_PORT_BUFFER_MAX_NUMBER */
    HI_U32 u32BufSize;          /* every buffer size in Byte */
    HI_U32 u32BufStride;       /* only for 'bUserAllocBuffer = TRUE' */
    HI_U32 u32BufPhyAddr[DEF_HI_DRV_VPSS_PORT_BUFFER_MAX_NUMBER]; /*128bit aligned */
}HI_DRV_VPSS_BUFLIST_CFG_S;

/* port的配置信息 */
typedef struct
{
    /*以下为可动态配置项*/
    HI_DRV_COLOR_SPACE_E eDstCS;   /*与逻辑确认*/
    HI_DRV_VPSS_PORT_PROCESS_S stProcCtrl;

    /*display Info*/
    HI_DRV_ASPECT_RATIO_S stDispPixAR;    /**/
    HI_DRV_ASP_RAT_MODE_E eAspMode;
    HI_DRV_ASPECT_RATIO_S stCustmAR;
    
    HI_S32  s32OutputWidth;
    HI_S32  s32OutputHeight;
    
    HI_BOOL   bInterlaced;                /*送显 I/P*/
    HI_RECT_S stScreen;                   /*送显窗口大小 :0 0 即窗口大小等于输出宽高*/
        
    /*以下为不可动态配置项*/
    HI_DRV_PIX_FORMAT_E eFormat; /* Support ... */
    HI_DRV_VPSS_BUFLIST_CFG_S stBufListCfg;
    HI_U32 u32MaxFrameRate;  /* in 1/100 HZ  */
    /* 与tunnel有关的配置 */
    HI_BOOL  bTunnelEnable;  /* 输出是否使能TUNNEL */
    HI_S32  s32SafeThr;    /* 安全水线，0~100，为输出帧已完成的百分比. 0表示随时可给后级，100表示完全完成才能给后级 */

    
}HI_DRV_VPSS_PORT_CFG_S;


/* VPSS用户的控制命令*/
typedef enum
{
    HI_DRV_VPSS_USER_COMMAND_IMAGEREADY = 0,
    HI_DRV_VPSS_USER_COMMAND_RESET,
    HI_DRV_VPSS_USER_COMMAND_CHECKALLDONE,
    HI_DRV_VPSS_USER_COMMAND_BUTT
}HI_DRV_VPSS_USER_COMMAND_E;

typedef struct
{   
    HI_U32 u32TotalBufNumber;
    HI_U32 u32FulBufNumber;  
}HI_DRV_VPSS_PORT_BUFLIST_STATE_S;

typedef enum
{
    HI_DRV_VPSS_BUFFUL_PAUSE = 0,
    HI_DRV_VPSS_BUFFUL_KEEPWORKING,
    HI_DRV_VPSS_BUFFUL_BUTT
}HI_DRV_VPSS_BUFFUL_STRATAGY_E;


/*
源模式分类
*/
typedef enum
{
    VPSS_SOURCE_MODE_USERACTIVE = 0,//推模式
    VPSS_SOURCE_MODE_VPSSACTIVE,//拉模式
    VPSS_SOURCE_MODE_BUTT
}HI_DRV_VPSS_SOURCE_MODE_E;

typedef HI_S32 (*PFN_VPSS_SRC_FUNC)(VPSS_HANDLE hVPSS,HI_DRV_VIDEO_FRAME_S *pstImage);

typedef struct
{
    /*拉模式下不可为空由用户设置，交给VPSS调用*/
    PFN_VPSS_SRC_FUNC VPSS_GET_SRCIMAGE;
    PFN_VPSS_SRC_FUNC VPSS_REL_SRCIMAGE;
}HI_DRV_VPSS_SOURCE_FUNC_S;


typedef struct
{
    VPSS_HANDLE hPort; 
    HI_U32 u32StartVirAddr;
    HI_U32 u32StartPhyAddr;
    HI_U32 u32Size;
    HI_U32 u32Stride;
    HI_U32 u32FrmH;
    HI_U32 u32FrmW;
}HI_DRV_VPSS_FRMBUF_S;

typedef struct
{
    VPSS_HANDLE hPort; 
    HI_DRV_VIDEO_FRAME_S stFrame;
}HI_DRV_VPSS_FRMINFO_S;

/* VPSS的上报事件枚举*/
typedef enum
{   
    /*
        VPSS生成任务时，获取要写入BUF失败，上报该事件,传入结构体HI_DRV_VPSS_BUFFUL_STRATAGY_E指针

        用户可通过HI_DRV_VPSS_CheckPortBufListFul
                  HI_DRV_VPSS_GetPortBufListState
                  接口，获取实例内各PORT的BUFFER占用情况,
        通过修改HI_DRV_VPSS_BUFFUL_STRATAGY_E结构体，返回处理策略
    */
    VPSS_EVENT_BUFLIST_FULL,    //HI_DRV_VPSS_BUFFUL_STRATAGY_E
    VPSS_EVENT_GET_FRMBUFFER,   //HI_DRV_VPSS_FRMBUF_S
    VPSS_EVENT_REL_FRMBUFFER,   //HI_DRV_VPSS_FRMBUF_S
    VPSS_EVENT_NEW_FRAME,        //HI_DRV_VPSS_FRMINFO_S
    VPSS_EVENT_BUTT,
}HI_DRV_VPSS_EVENT_E;

//注册用户对VPSS处理异常的响应函数  
typedef HI_S32 (*PFN_VPSS_CALLBACK)(HI_HANDLE hDst, HI_DRV_VPSS_EVENT_E enEventID, HI_VOID *pstArgs);

#endif


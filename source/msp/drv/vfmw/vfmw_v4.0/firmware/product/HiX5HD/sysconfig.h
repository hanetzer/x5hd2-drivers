#ifndef __VFMW_SYSCONFIG_HEADER__
#define __VFMW_SYSCONFIG_HEADER__

//#define TWO_VDH

/* 最大可支持的通道数目 */
#ifdef CFG_MAX_CHAN_NUM
#define MAX_CHAN_NUM CFG_MAX_CHAN_NUM
#else
#define MAX_CHAN_NUM 32
#endif

/*可支持的VDH个数*/
#if defined(TWO_VDH)
#define MAX_VDH_NUM 2
#else
#define MAX_VDH_NUM 1
#endif

/* 系统控制寄存器地址和位域 */
#define VDM_RESET_REG_PHY_ADDR  0x101f5060   // VDM0
#define VDM_RESET_REG_CTRL_BIT  0

#define VDM_RESET_REG_PHY_ADDR_1  0x101f5060 //VDM1
#define VDM_RESET_REG_CTRL_BIT_1  0
	
#define DNR_RESET_REG_PHY_ADDR  0x101f5064   // DNR
#define DNR_RESET_REG_CTRL_BIT  0

#define SCD_RESET_REG_PHY_ADDR  0x101f505c   //SCD
#define SCD_RESET_REG_CTRL_BIT  1

#define BPD_RESET_REG_PHY_ADDR  0x101f504c   // BPD
#define BPD_RESET_REG_CTRL_BIT  0

#define FOD_RESET_REG_PHY_ADDR  0x101f505c  //FOD
#define FOD_RESET_REG_CTRL_BIT  0

/* 硬件加速IP接口寄存器分址 */
#define VDM_REG_PHY_ADDR       0x10130000    /* VDM0寄存器映射到ARM空间的地址*/
#define VDM_REG_PHY_ADDR_1     0x10130000    /* VDM1寄存器映射到ARM空间的地址*/
#define DNR_REG_PHY_ADDR       0x10450000    /* DNR 寄存器映射到ARM空间的地址*/
#define SCD_REG_PHY_ADDR       0x600f0000    /* SCD 寄存器映射到ARM空间的地址*/
#define BPD_REG_PHY_ADDR       0x10140000    /* BPD 寄存器映射到ARM空间的地址*/
#define FOD_REG_PHY_ADDR       0x10150000    /* FOD 寄存器映射到ARM空间的地址*/
#define SYSTEM_REG_PHY_ADDR    0x101f5000    /* 系统寄存器*/
#define SYSTEM_REG_RANGE       (64*1024)
    
    /* 中断号 */
#define VDM_INT_NUM            (38+32)
#define VDM_INT_NUM_1          (38+32)
#define SCD_INT_NUM            (41+32)  
#define BPD_INT_NUM            (39+32)  
#define DNR_INT_NUM            (61+32)

/* 测试单板上可被视频解码使用的memory预算 */
#define BOARD_MEM_BASE_ADDR    0x88000000
#define BOARD_MEM_TOTAL_SIZE   (80*1024*1024)

#endif

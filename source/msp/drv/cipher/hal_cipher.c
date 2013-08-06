/**
\file
\brief cipher hal interface
\copyright Shenzhen Hisilicon Co., Ltd.
\date 2008-2018
\version draft
\author QuYaxin 46153
\date 2009-11-3
*/

#include <linux/jiffies.h>
#include "hi_type.h"
#include "hi_debug.h"
#include "hi_common.h"
#include "hi_error_mpi.h"
#include "drv_cipher_ioctl.h"
#include "hal_cipher.h"
#include "drv_advca_ext.h"
#include "drv_mmz_ext.h"

/* Set the defualt timeout value for hash calculating (6000 ms)*/
#define HASH_MAX_DURATION (6000)

extern HI_VOID HI_DRV_SYS_GetChipVersion(HI_CHIP_TYPE_E *penChipType, HI_CHIP_VERSION_E *penChipVersion);

/***************************** Macro Definition ******************************/
/* cipher reg read and write macro define */
#ifndef CIPHER_READ_REG
#define CIPHER_READ_REG(reg, result) ((result) = *(volatile unsigned int *)(reg))
#endif

#ifndef CIPHER_WRITE_REG
#define CIPHER_WRITE_REG(reg, data) (*((volatile unsigned int *)(reg)) = (data))
#endif

/*process of bit*/
#define HAL_SET_BIT(src, bit)        ((src) |= (1<<bit))
#define HAL_CLEAR_BIT(src,bit)       ((src) &= ~(1<<bit))


/*************************** Structure Definition ****************************/
typedef enum
{
    HASH_READY,
    REC_READY,
    DMA_READY,
}HASH_WAIT_TYPE;

/******************************* API declaration *****************************/
inline HI_S32 HASH_WaitReady( HASH_WAIT_TYPE enType)
{
    CIPHER_SHA_STATUS_U unCipherSHAstatus;
    HI_SIZE_T ulStartTime = 0;
    HI_SIZE_T ulLastTime = 0;
    HI_SIZE_T ulDuraTime = 0;

    /* wait for hash_rdy */
    ulStartTime = jiffies;
    while(1)
    {
        unCipherSHAstatus.u32 = 0;
        CIPHER_READ_REG(CIPHER_HASH_REG_STATUS_ADDR, unCipherSHAstatus.u32);
        if(HASH_READY == enType)
        {
            if(1 == unCipherSHAstatus.bits.hash_rdy)
            {
                break;
            }
        }
        else if (REC_READY == enType)
        {
            if(1 == unCipherSHAstatus.bits.rec_rdy)
            {
                break;
            }
        }
        else if (DMA_READY == enType)
        {
            if(1 == unCipherSHAstatus.bits.dma_rdy)
            {
                break;
            }
        }
        else
        {
            HI_ERR_CIPHER("Error! Invalid wait type!\n");
            return HI_FAILURE;
        }

        ulLastTime = jiffies;
        ulDuraTime = jiffies_to_msecs(ulLastTime - ulStartTime);
        if (ulDuraTime >= HASH_MAX_DURATION )
        { 
            HI_ERR_CIPHER("Error! Hash time out!\n");
            return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_SetInBufNum(HI_U32 chnId, HI_U32 num)
{
    HI_U32 regAddr = 0;

    if (CIPHER_PKGx1_CHAN == chnId)
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    /* register0~15 bit is valid, others bits reserved */
    regAddr = CIPHER_REG_CHANn_IBUF_NUM(chnId);

    if (num > 0xffff)
    {
        HI_ERR_CIPHER("value err:%#x, set to 0xffff\n", num);
        num = 0xffff;
    }

    CIPHER_WRITE_REG(regAddr, num);
    
    HI_INFO_CIPHER(" cnt=%u\n", num);
    
    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_GetInBufNum(HI_U32 chnId, HI_U32 *pNum)
{
    HI_U32 regAddr  = 0;
    HI_U32 regValue = 0;

    if (CIPHER_PKGx1_CHAN == chnId || HI_NULL == pNum)
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    regAddr = CIPHER_REG_CHANn_IBUF_NUM(chnId);
    CIPHER_READ_REG(regAddr, regValue);
    
    *pNum = regValue;

    HI_INFO_CIPHER(" cnt=%u\n", regValue);
    
    return HI_SUCCESS;
}


HI_S32 HAL_Cipher_SetInBufCnt(HI_U32 chnId, HI_U32 num)
{
    HI_U32 regAddr = 0;

    if (CIPHER_PKGx1_CHAN == chnId)
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    regAddr = CIPHER_REG_CHANn_IBUF_CNT(chnId);

    if (num > 0xffff)
    {
        HI_ERR_CIPHER("value err:%x, set to 0xffff\n", num);
        num = 0xffff;
    }

    CIPHER_WRITE_REG(regAddr, num);
    
    HI_INFO_CIPHER(" HAL_Cipher_SetInBufCnt=%u\n", num);
    
    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_GetInBufCnt(HI_U32 chnId, HI_U32 *pNum)
{
    HI_U32 regAddr = 0;
    HI_U32 regValue = 0;

    if ( (CIPHER_PKGx1_CHAN == chnId) || (HI_NULL == pNum) )
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    regAddr = CIPHER_REG_CHANn_IBUF_CNT(chnId);
    CIPHER_READ_REG(regAddr, regValue);
    *pNum = regValue;
    
    HI_INFO_CIPHER(" cnt=%u\n", regValue);
    
    return HI_SUCCESS;
}


HI_S32 HAL_Cipher_SetInBufEmpty(HI_U32 chnId, HI_U32 num)
{
    HI_U32 regAddr = 0;

    if (CIPHER_PKGx1_CHAN == chnId)
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    regAddr = CIPHER_REG_CHANn_IEMPTY_CNT(chnId);

    if (num > 0xffff)
    {
        HI_ERR_CIPHER("value err:%x, set to 0xffff\n", num);
        num = 0xffff;
    }

    CIPHER_WRITE_REG(regAddr, num);
    
    HI_INFO_CIPHER(" cnt=%u\n", num);
    
    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_GetInBufEmpty(HI_U32 chnId, HI_U32 *pNum)
{
    HI_U32 regAddr = 0;
    HI_U32 regValue = 0;

    if ( (CIPHER_PKGx1_CHAN == chnId) || (HI_NULL == pNum) )
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    regAddr = CIPHER_REG_CHANn_IEMPTY_CNT(chnId);
    CIPHER_READ_REG(regAddr, regValue);
    
    *pNum = regValue;
    
    HI_INFO_CIPHER(" cnt=%u\n", regValue);
    
    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_SetOutBufNum(HI_U32 chnId, HI_U32 num)
{
    HI_U32 regAddr = 0;

    if (CIPHER_PKGx1_CHAN == chnId)
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    regAddr = CIPHER_REG_CHANn_OBUF_NUM(chnId);

    if (num > 0xffff)
    {
        HI_ERR_CIPHER("value err:%x, set to 0xffff\n", num);
        num = 0xffff;
    }

    CIPHER_WRITE_REG(regAddr, num);
    
    HI_INFO_CIPHER("chn=%d cnt=%u\n", chnId, num);
    
    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_GetOutBufNum(HI_U32 chnId, HI_U32 *pNum)
{
    HI_U32 regAddr = 0;
    HI_U32 regValue = 0;

    if ( (CIPHER_PKGx1_CHAN == chnId) || (HI_NULL == pNum) )
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    regAddr = CIPHER_REG_CHANn_OBUF_NUM(chnId);
    CIPHER_READ_REG(regAddr, regValue);

    *pNum = regValue;

    HI_INFO_CIPHER(" cnt=%u\n", regValue);
    
    return HI_SUCCESS;
}


HI_S32 HAL_Cipher_SetOutBufCnt(HI_U32 chnId, HI_U32 num)
{
    HI_U32 regAddr = 0;

    if (CIPHER_PKGx1_CHAN == chnId)
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    regAddr = CIPHER_REG_CHANn_OBUF_CNT(chnId);

    if (num > 0xffff)
    {
        HI_ERR_CIPHER("value err:%x, set to 0xffff\n", num);
        num = 0xffff;
    }

    CIPHER_WRITE_REG(regAddr, num);
    
    HI_INFO_CIPHER("SetOutBufCnt=%u, chnId=%u\n", num,chnId);
    
    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_GetOutBufCnt(HI_U32 chnId, HI_U32 *pNum)
{
    HI_U32 regAddr = 0;
    HI_U32 regValue = 0;

    if ( (CIPHER_PKGx1_CHAN == chnId) || (HI_NULL == pNum) )
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    regAddr = CIPHER_REG_CHANn_OBUF_CNT(chnId);
    CIPHER_READ_REG(regAddr, regValue);

    *pNum = regValue;

    HI_INFO_CIPHER(" HAL_Cipher_GetOutBufCnt=%u\n", regValue);
    
    return HI_SUCCESS;
}


HI_S32 HAL_Cipher_SetOutBufFull(HI_U32 chnId, HI_U32 num)
{
    HI_U32 regAddr = 0;

    if (CIPHER_PKGx1_CHAN == chnId)
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    regAddr = CIPHER_REG_CHANn_OFULL_CNT(chnId);

    if (num > 0xffff)
    {
        HI_ERR_CIPHER("value err:%x, set to 0xffff\n", num);
        num = 0xffff;
    }

    CIPHER_WRITE_REG(regAddr, num);
    
    HI_INFO_CIPHER(" cnt=%u\n", num);
    
    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_GetOutBufFull(HI_U32 chnId, HI_U32 *pNum)
{
    HI_U32 regAddr = 0;
    HI_U32 regValue = 0;

    if ( (CIPHER_PKGx1_CHAN == chnId) || (HI_NULL == pNum) )
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    regAddr = CIPHER_REG_CHANn_OFULL_CNT(chnId);
    CIPHER_READ_REG(regAddr, regValue);

    *pNum = regValue;

    HI_INFO_CIPHER(" cnt=%u\n", regValue);
    
    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_WaitIdle(HI_VOID)
{
    HI_S32 i = 0;
    HI_U32 u32RegAddr = 0;
    HI_U32 u32RegValue = 0;

    /* channel 0 configuration register [31-2]:reserved, [1]:ch0_busy, [0]:ch0_start 
         * [1]:channel 0 status signal, [0]:channel 0 encrypt/decrypt start signal
         */

    u32RegAddr = CIPHER_REG_CHAN0_CFG;
    for (i = 0; i < CIPHER_WAIT_IDEL_TIMES; i++)
    {
        CIPHER_READ_REG(u32RegAddr, u32RegValue);
        if (0x0 == ((u32RegValue >> 1) & 0x01))
        {
            return HI_SUCCESS;
        }
        else
        {
            //udelay(1);
        }
    }

    return HI_FAILURE;
}
/*
just only check for channel 0
 */
HI_BOOL HAL_Cipher_IsIdle(HI_U32 chn)
{
    HI_U32 u32RegValue = 0;

    HI_ASSERT(CIPHER_PKGx1_CHAN == chn);

    CIPHER_READ_REG(CIPHER_REG_CHAN0_CFG, u32RegValue);
    if (0x0 == ((u32RegValue >> 1) & 0x01))
    {
        return HI_TRUE;
    }

    return HI_FALSE;
}

HI_S32 HAL_Cipher_SetDataSinglePkg(HI_DRV_CIPHER_DATA_INFO_S * info)
{
    HI_U32 regAddr = 0;
    HI_U32 i = 0;

    regAddr = CIPHER_REG_CHAN0_CIPHER_DIN(0);
    
    /***/
    for (i = 0; i < (16/sizeof(HI_U32)); i++)
    {
        CIPHER_WRITE_REG(regAddr + (i * sizeof(HI_U32)), (*(info->u32DataPkg + i)) );
    }
    
    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_ReadDataSinglePkg(HI_U32 *pData)
{
    HI_U32 regAddr = 0;
    HI_U32 i = 0;

    regAddr = CIPHER_REG_CHAN0_CIPHER_DOUT(0);

    /***/
    for (i = 0; i < (16/sizeof(HI_U32)); i++)
    {
        CIPHER_READ_REG(regAddr + (i * sizeof(HI_U32)), (*(pData+ i)) );
    }
    
    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_StartSinglePkg(HI_U32 chnId)
{
    HI_U32 u32RegAddr = 0;
    HI_U32 u32RegValue = 0;

    HI_ASSERT(CIPHER_PKGx1_CHAN == chnId);

    u32RegAddr = CIPHER_REG_CHAN0_CFG;
    CIPHER_READ_REG(u32RegAddr, u32RegValue);
    
    u32RegValue |= 0x1;
    CIPHER_WRITE_REG(u32RegAddr, u32RegValue); /* start work */
    
    return HI_SUCCESS;
}


HI_S32 HAL_Cipher_SetBufAddr(HI_U32 chnId, CIPHER_BUF_TYPE_E bufType, HI_U32 addr)
{
    HI_U32 regAddr = 0;

    if (CIPHER_PKGx1_CHAN == chnId)
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if (CIPHER_BUF_TYPE_IN == bufType)
    {
        regAddr = CIPHER_REG_CHANn_SRC_LST_SADDR(chnId);
    }
    else if (CIPHER_BUF_TYPE_OUT == bufType)
    {
        regAddr = CIPHER_REG_CHANn_DEST_LST_SADDR(chnId);
    }
    else
    {
        HI_ERR_CIPHER("SetBufAddr type err:%x.\n", bufType);
        return HI_ERR_CIPHER_INVALID_PARA;
    }


    HI_INFO_CIPHER("Set chn%d '%s' BufAddr to:%x.\n",chnId,
        (CIPHER_BUF_TYPE_IN == bufType)?"In":"Out",  addr);

    CIPHER_WRITE_REG(regAddr, addr);

    return HI_SUCCESS;
}



HI_VOID HAL_Cipher_Reset(HI_VOID)
{

    //CIPHER_WRITE_REG(CIPHER_SOFT_RESET_ADDR, 1);
    return;
}

HI_S32 HAL_Cipher_GetOutIV(HI_U32 chnId, HI_UNF_CIPHER_CTRL_S* pCtrl)
{
    HI_U32 i = 0;
    HI_U32 regAddr = 0;

    if (CIPHER_PKGx1_CHAN != chnId)
    {
        regAddr = CIPHER_REG_CHAN0_CIPHER_IVIN(0);
    }
    else
    {
        regAddr = CIPHER_REG_CHAN_CIPHER_IVOUT(chnId);
    }


    /***/
    for (i = 0; i < (CI_IV_SIZE/sizeof(HI_U32)); i++)
    {
        CIPHER_READ_REG(regAddr + (i * sizeof(HI_U32)), pCtrl->u32IV[i]);
    }

    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_SetInIV(HI_U32 chnId, HI_UNF_CIPHER_CTRL_S* pCtrl)
{
    HI_U32 i = 0;
    HI_U32 regAddr = 0;

    if (CIPHER_PKGx1_CHAN != chnId)
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    regAddr = CIPHER_REG_CHAN0_CIPHER_IVIN(0);

    /***/
    for (i = 0; i < (CI_IV_SIZE/sizeof(HI_U32)); i++)
    {
        CIPHER_WRITE_REG(regAddr + (i * sizeof(HI_U32)), pCtrl->u32IV[i]);
    }

    return HI_SUCCESS;
}

extern ADVCA_EXPORT_FUNC_S  *s_pAdvcaFunc;

HI_S32 HAL_Cipher_SetKey(HI_U32 chnId, HI_UNF_CIPHER_CTRL_S* pCtrl)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 i = 0;
    HI_U32 regAddr = 0;

    regAddr = CIPHER_REG_CIPHER_KEY(chnId);

    /***/
    if (HI_FALSE == pCtrl->bKeyByCA)
    {
        for (i = 0; i < (CI_KEY_SIZE/sizeof(HI_U32)); i++)
        {
            CIPHER_WRITE_REG(regAddr + (i * sizeof(HI_U32)), pCtrl->u32Key[i]);
        }
    }
    else
    {
        if (HI_UNF_CIPHER_CA_TYPE_SP == pCtrl->enCaType)
        {
            HI_CHIP_VERSION_E enChipVersion;
            HI_DRV_SYS_GetChipVersion(HI_NULL, &enChipVersion);
            if (HI_CHIP_VERSION_V300 == enChipVersion)
            {
                if (s_pAdvcaFunc && s_pAdvcaFunc->pfnAdvcaDecryptSP)
                {
                    s32Ret = (s_pAdvcaFunc->pfnAdvcaDecryptSP)(chnId,0,(HI_U8*)pCtrl->u32Key);
                    if (HI_SUCCESS != s32Ret)
                    {
                        return s32Ret;
                    }
                }
            }
            else
            {
                HI_ERR_CIPHER("Can not use SP Key Ladder!\n");
            }
        }
        else
        {
            if (s_pAdvcaFunc && s_pAdvcaFunc->pfnAdvcaDecryptCipher)
            {
                s32Ret = (s_pAdvcaFunc->pfnAdvcaDecryptCipher)(chnId,pCtrl->u32Key); 
                if (HI_SUCCESS != s32Ret)
                {
                    return s32Ret;
                }
            }
        }
    }

    HI_INFO_CIPHER("SetKey: chn%u,Key:%#x, %#x, %#x, %#x.\n", chnId,
        pCtrl->u32Key[0], pCtrl->u32Key[1], pCtrl->u32Key[2], pCtrl->u32Key[3]);
    
    return HI_SUCCESS;
}

/*
=========channel n control register==========
[31:22]             weight [in 64bytes, just only for multi-packet channel encrypt or decrypt, otherwise reserved.]
[21:17]             reserved
[16:14]     RW    key_adder [current key sequence number]
[13]          RW    key_sel [key select control, 0-CPU keys, 1-keys from key Ladder]
[12:11]             reserved
[10:9]      RW      key_length[key length control
                                            (1).AES, 00-128 bits key, 01-192bits 10-256bits, 11-128bits
                                            (2).DES, 00-3 keys, 01-3keys, 10-3keys, 11-2keys]
[8]                     reserved
[7:6]       RW      width[bits width control
                                 (1).for DES/3DES, 00-64bits, 01-8bits, 10-1bit, 11-64bits
                                 (2).for AES, 00-128bits, 01-8bits, 10-1bit, 11-128bits]
[5:4]       RW      alg_sel[algorithm type, 00-DES, 01-3DES, 10-AES, 11-DES]
[3:1]       RW      mode[mode control, 
                                  (1).for AES, 000-ECB, 001-CBC, 010-CFB, 011-OFB, 100-CTR, others-ECB
                                  (2).for DES, 000-ECB, 001-CBC, 010-CFB, 011-OFB, others-ECB]
[0]         RW      decrypt[encrypt or decrypt control, 0 stands for encrypt, 1 stands for decrypt]
*/
HI_S32 HAL_Cipher_Config(HI_U32 chnId, HI_BOOL bDecrypt, HI_BOOL bIVChange, HI_UNF_CIPHER_CTRL_S* pCtrl)
{
    HI_U32 keyId = 0;
    HI_U32 regAddr = 0;
    HI_U32 regValue = 0;

    HI_BOOL bKeyByCA = pCtrl->bKeyByCA;

    if (CIPHER_PKGx1_CHAN == chnId)
    {
        /* channel 0, single packet encrypt or decrypt channel */
        regAddr = CIPHER_REG_CHAN0_CIPHER_CTRL;
    }
    else
    {
        regAddr = CIPHER_REG_CHANn_CIPHER_CTRL(chnId);
    }

    CIPHER_READ_REG(regAddr, regValue);

    if (HI_FALSE == bDecrypt)/* encrypt */
    {
        regValue &= ~(1 << 0);
    }
    else /* decrypt */
    {
        regValue |= 1;
    }

    /* set mode */
    regValue &= ~(0x07 << 1);/* clear bit1~bit3 */
    regValue |= ((pCtrl->enWorkMode & 0x7) << 1);

    /* set algorithm bits */
    regValue &= ~(0x03 << 4); /* clear algorithm bits*/
    regValue |= ((pCtrl->enAlg & 0x3) << 4);

    /* set bits width */
    regValue &= ~(0x03 << 6);
    regValue |= ((pCtrl->enBitWidth & 0x3) << 6);

    regValue &= ~(0x01 << 8);
    regValue |= ((bIVChange & 0x1) << 8);
    if (bIVChange) ///?
    {
        HAL_Cipher_SetInIV(chnId, pCtrl);
    }

    regValue &= ~(0x03 << 9);
    regValue |= ((pCtrl->enKeyLen & 0x3) << 9);

    regValue &= ~(0x01 << 13);
    regValue |= ((bKeyByCA & 0x1) << 13);

//    if (HI_FALSE == bKeyByCA) /* By User */
//    {
        keyId = chnId;/**/

        //HAL_Cipher_SetKey(chnId, pCtrl->u32Key);

        regValue &= ~(0x07 << 14);
        regValue |= ((keyId & 0x7) << 14);
//    }

    CIPHER_WRITE_REG(regAddr, regValue);
    
    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_SetAGEThreshold(HI_U32 chnId, CIPHER_INT_TYPE_E intType, HI_U32 value)
{
    HI_U32 regAddr = 0;

    if (CIPHER_PKGx1_CHAN == chnId)
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if (CIPHER_INT_TYPE_IN_BUF == intType)
    {
        regAddr = CIPHER_REG_CHANn_IAGE_CNT(chnId);
    }
    else if (CIPHER_INT_TYPE_OUT_BUF == intType)
    {
        regAddr = CIPHER_REG_CHANn_OAGE_CNT(chnId);
    }
    else
    {
        HI_ERR_CIPHER("SetAGEThreshold type err:%x.\n", intType);
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if (value > 0xffff)
    {
        HI_ERR_CIPHER("SetAGEThreshold value err:%x, set to 0xffff\n", value);
        value = 0xffff;
    }

    CIPHER_WRITE_REG(regAddr, value);

    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_SetIntThreshold(HI_U32 chnId, CIPHER_INT_TYPE_E intType, HI_U32 value)
{
    HI_U32 regAddr = 0;

    if (CIPHER_PKGx1_CHAN == chnId)
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if (CIPHER_INT_TYPE_IN_BUF == intType)
    {
        regAddr = CIPHER_REG_CHANn_INT_ICNTCFG(chnId);
    }
    else if (CIPHER_INT_TYPE_OUT_BUF == intType)
    {
        regAddr = CIPHER_REG_CHANn_INT_OCNTCFG(chnId);
    }
    else
    {
        HI_ERR_CIPHER("SetIntThreshold type err:%x.\n", intType);
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if (value > 0xffff)
    {
        HI_ERR_CIPHER("SetIntThreshold value err:%x, set to 0xffff\n", value);
        value = 0xffff;
    }

    CIPHER_WRITE_REG(regAddr, value);

    return HI_SUCCESS;
}

/*
interrupt enable
[31]-----cipher module unitary interrupt enable
[30:16]--reserved
[15] channel 7 output queue data interrupt enable
[14] channel 6 output queue data interrupt enable
[... ] channel ... output queue data interrupt enable
[9]   channel 1 output queue data interrupt enable
[8]   channel 0 data dispose finished interrupt enble
[7] channel 7 input queue data interrupt enable
[6] channel 6 input queue data interrupt enable
...
[1] channel 1 input queue data interrupt enable
[0] reserved
*/
HI_S32 HAL_Cipher_EnableInt(HI_U32 chnId, int intType)
{
    HI_U32 regAddr = 0;
    HI_U32 regValue = 0;

    regAddr = CIPHER_REG_INT_EN;
    CIPHER_READ_REG(regAddr, regValue);

    regValue |= (1 << 31); /* sum switch int_en */

    if (CIPHER_PKGx1_CHAN == chnId)
    {
        regValue |= (1 << 8);
    }
    else
    {
        if (CIPHER_INT_TYPE_OUT_BUF == (CIPHER_INT_TYPE_OUT_BUF & intType))
        {
            regValue |= (1 << (8 + chnId));
        }

        /* NOT else if */
        if (CIPHER_INT_TYPE_IN_BUF == (CIPHER_INT_TYPE_IN_BUF & intType))
        {
            regValue |= (1 << (0 + chnId));
        }
    }

    CIPHER_WRITE_REG(regAddr, regValue);

    HI_INFO_CIPHER("HAL_Cipher_EnableInt: Set INT_EN:%#x\n", regValue);

    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_DisableInt(HI_U32 chnId, int intType)
{
    HI_U32 regAddr = 0;
    HI_U32 regValue = 0;

    regAddr = CIPHER_REG_INT_EN;
    CIPHER_READ_REG(regAddr, regValue);

    if (CIPHER_PKGx1_CHAN == chnId)
    {
        regValue &= ~(1 << 8);
    }
    else
    {
        if (CIPHER_INT_TYPE_OUT_BUF == (CIPHER_INT_TYPE_OUT_BUF & intType))
        {
            regValue &= ~(1 << (8 + chnId));
        }

        /* NOT else if */
        if (CIPHER_INT_TYPE_IN_BUF == (CIPHER_INT_TYPE_IN_BUF & intType))
        {
            regValue &= ~(1 << (0 + chnId));
        }
    }

    if (0 == (regValue & 0x7fffffff))
    {
        regValue &= ~(1 << 31); /* regValue = 0; sum switch int_en */
    }

    CIPHER_WRITE_REG(regAddr, regValue);

    HI_INFO_CIPHER("HAL_Cipher_DisableInt: Set INT_EN:%#x\n", regValue);

    return HI_SUCCESS;
}

HI_VOID HAL_Cipher_DisableAllInt(HI_VOID)
{
    HI_U32 regAddr = 0;
    HI_U32 regValue = 0;

    regAddr = CIPHER_REG_INT_EN;
    regValue = 0;
    CIPHER_WRITE_REG(regAddr, regValue);
}
/*
interrupt status register
[31:16]--reserved
[15] channel 7 output queue data interrupt enable
[14] channel 6 output queue data interrupt enable
[... ] channel ... output queue data interrupt enable
[9]   channel 1 output queue data interrupt enable
[8]   channel 0 data dispose finished interrupt enble
[7] channel 7 input queue data interrupt enable
[6] channel 6 input queue data interrupt enable
...
[1] channel 1 input queue data interrupt enable
[0] reserved
*/

HI_VOID HAL_Cipher_GetIntState(HI_U32 *pState)
{
    HI_U32 regAddr = 0;
    HI_U32 regValue = 0;

    regAddr = CIPHER_REG_INT_STATUS;
    CIPHER_READ_REG(regAddr, regValue);

    if (pState)
    {
        *pState = regValue;
    }

   HI_INFO_CIPHER("HAL_Cipher_GetIntState=%#x\n", regValue);
}

HI_VOID HAL_Cipher_GetIntEnState(HI_U32 *pState)
{
    HI_U32 regAddr = 0;
    HI_U32 regValue = 0;

    regAddr = CIPHER_REG_INT_EN;
    
    CIPHER_READ_REG(regAddr, regValue);

    if (pState)
    {
        *pState = regValue;
    }

   HI_INFO_CIPHER("HAL_Cipher_GetIntEnState=%#x\n", regValue);
}

HI_VOID HAL_Cipher_GetRawIntState(HI_U32 *pState)
{
    HI_U32 regAddr = 0;
    HI_U32 regValue = 0;

    regAddr = CIPHER_REG_INT_RAW;
    
    CIPHER_READ_REG(regAddr, regValue);

    if (pState)
    {
        *pState = regValue;
    }

    HI_INFO_CIPHER("HAL_Cipher_GetRawIntState=%#x\n", regValue);
}

HI_VOID HAL_Cipher_ClrIntState(HI_U32 intStatus)
{
    HI_U32 regAddr;
    HI_U32 regValue;

    regAddr = CIPHER_REG_INT_RAW;
    regValue = intStatus;
    CIPHER_WRITE_REG(regAddr, regValue);
}

HI_VOID HAL_Cipher_SetHdcpModeEn(HI_DRV_CIPHER_HDCP_MODE_E enMode)
{
    HI_U32 u32RegAddr = 0;
    CIPHER_HDCP_MODE_CTRL_U stHDCPModeCtrl;

    memset((HI_VOID *)&stHDCPModeCtrl, 0, sizeof(stHDCPModeCtrl.u32));

    u32RegAddr = CIPHER_REG_HDCP_MODE_CTRL;
    CIPHER_READ_REG(u32RegAddr, stHDCPModeCtrl.u32);
    
    if ( CIPHER_HDCP_MODE_NO_HDCP_KEY == enMode)
    {
        stHDCPModeCtrl.bits.hdcp_mode_en = 0;
    }
    else
    {
        stHDCPModeCtrl.bits.hdcp_mode_en = 1;
    }

    CIPHER_WRITE_REG(u32RegAddr, stHDCPModeCtrl.u32);
    
    return;
}

HI_S32 HAL_Cipher_GetHdcpModeEn(HI_DRV_CIPHER_HDCP_MODE_E *penMode)
{
    HI_U32 u32RegAddr = 0;
    CIPHER_HDCP_MODE_CTRL_U stHDCPModeCtrl;

    if ( NULL == penMode )
    {
        HI_ERR_CIPHER("Invald param, null pointer!\n");
        return HI_FAILURE;
    }

    memset((HI_VOID *)&stHDCPModeCtrl, 0, sizeof(CIPHER_HDCP_MODE_CTRL_U));

    u32RegAddr = CIPHER_REG_HDCP_MODE_CTRL;
    CIPHER_READ_REG(u32RegAddr, stHDCPModeCtrl.u32);
    
    if ( 0 == stHDCPModeCtrl.bits.hdcp_mode_en)
    {
        *penMode = CIPHER_HDCP_MODE_NO_HDCP_KEY;
    }
    else
    {
        *penMode = CIPHER_HDCP_MODE_HDCP_KEY;
    }

    return HI_SUCCESS;
}

HI_VOID HAL_Cipher_SetHdcpKeyRamMode(HI_DRV_CIPHER_HDCP_KEY_RAM_MODE_E enMode)
{
    HI_U32 u32RegAddr = 0;
    CIPHER_HDCP_MODE_CTRL_U unHDCPModeCtrl;

    memset((HI_VOID *)&unHDCPModeCtrl, 0, sizeof(CIPHER_HDCP_MODE_CTRL_U));

    u32RegAddr = CIPHER_REG_HDCP_MODE_CTRL;
    CIPHER_READ_REG(u32RegAddr, unHDCPModeCtrl.u32);

    if ( CIPHER_HDCP_KEY_RAM_MODE_READ == enMode)
    {
        unHDCPModeCtrl.bits.tx_read = 0x1;      //hdmi read mode
    }
    else
    {
        unHDCPModeCtrl.bits.tx_read = 0x0;      //cpu write mode
    }    

    CIPHER_WRITE_REG(u32RegAddr, unHDCPModeCtrl.u32);
    
    return;
}

HI_S32 HAL_Cipher_GetHdcpKeyRamMode(HI_DRV_CIPHER_HDCP_KEY_RAM_MODE_E *penMode)
{
    HI_U32 u32RegAddr = 0;
    CIPHER_HDCP_MODE_CTRL_U unHDCPModeCtrl;

    if ( NULL == penMode )
    {
        return HI_FAILURE;
    }

    memset((HI_VOID *)&unHDCPModeCtrl, 0, sizeof(CIPHER_HDCP_MODE_CTRL_U));
    
    u32RegAddr = CIPHER_REG_HDCP_MODE_CTRL;
    CIPHER_READ_REG(u32RegAddr, unHDCPModeCtrl.u32);
    
    if ( 0 == unHDCPModeCtrl.bits.tx_read )
    {
        *penMode = CIPHER_HDCP_KEY_RAM_MODE_WRITE;      //cpu write mode
    }
    else
    {
        *penMode = CIPHER_HDCP_KEY_RAM_MODE_READ;       //hdmi read mode
    }    
    
    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_SetHdcpKeySelectMode(HI_DRV_CIPHER_HDCP_KEY_TYPE_E enHdcpKeySelectMode)
{
    HI_U32 u32RegAddr = 0;
    CIPHER_HDCP_MODE_CTRL_U unHDCPModeCtrl;

    memset((HI_VOID *)&unHDCPModeCtrl, 0, sizeof(CIPHER_HDCP_MODE_CTRL_U));

    u32RegAddr = CIPHER_REG_HDCP_MODE_CTRL;
    CIPHER_READ_REG(u32RegAddr, unHDCPModeCtrl.u32);       
    
    if ( CIPHER_HDCP_KEY_TYPE_OTP_ROOT_KEY == enHdcpKeySelectMode )
    {
        unHDCPModeCtrl.bits.hdcp_rootkey_sel = 0x00;
    }
    else if ( CIPHER_HDCP_KEY_TYPE_HISI_DEFINED == enHdcpKeySelectMode )
    {
        unHDCPModeCtrl.bits.hdcp_rootkey_sel = 0x01;
    }
    else if ( CIPHER_HDCP_KEY_TYPE_HDCP_HOST_ROOT_KEY == enHdcpKeySelectMode)
    {
        unHDCPModeCtrl.bits.hdcp_rootkey_sel = 0x2;
    }
    else
    {
        unHDCPModeCtrl.bits.hdcp_rootkey_sel = 0x3;
        CIPHER_WRITE_REG(u32RegAddr, unHDCPModeCtrl.u32);

        HI_ERR_CIPHER("Unexpected hdcp key type selected!\n");
        return HI_FAILURE;
    }
    
    CIPHER_WRITE_REG(u32RegAddr, unHDCPModeCtrl.u32);
    
    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_GetHdcpKeySelectMode(HI_DRV_CIPHER_HDCP_KEY_TYPE_E *penHdcpKeySelectMode)
{
    HI_U32 u32RegAddr = 0;
    CIPHER_HDCP_MODE_CTRL_U unHDCPModeCtrl;
    
    if ( NULL == penHdcpKeySelectMode )
    {
        HI_ERR_CIPHER("Invalid param, NULL pointer!\n");
        return HI_FAILURE;
    }

    memset((HI_VOID *)&unHDCPModeCtrl, 0, sizeof(CIPHER_HDCP_MODE_CTRL_U));
    
    u32RegAddr = CIPHER_REG_HDCP_MODE_CTRL;
    CIPHER_READ_REG(u32RegAddr, unHDCPModeCtrl.u32);

    if ( 0x00 == unHDCPModeCtrl.bits.hdcp_rootkey_sel )
    {
        *penHdcpKeySelectMode = CIPHER_HDCP_KEY_TYPE_OTP_ROOT_KEY;
    }
    else if ( 0x01 == unHDCPModeCtrl.bits.hdcp_rootkey_sel)
    {
        *penHdcpKeySelectMode = CIPHER_HDCP_KEY_TYPE_HISI_DEFINED;
    }
    else if (  0x02 == unHDCPModeCtrl.bits.hdcp_rootkey_sel )
    {
        *penHdcpKeySelectMode = CIPHER_HDCP_KEY_TYPE_HDCP_HOST_ROOT_KEY;
    }
    else
    {
        *penHdcpKeySelectMode = CIPHER_HDCP_KEY_TYPE_BUTT;
    }
    
    return HI_SUCCESS;
}

HI_VOID HAL_Cipher_ClearHdcpCtrlReg(HI_VOID)
{
    CIPHER_WRITE_REG(CIPHER_REG_HDCP_MODE_CTRL, 0);
    return;
}

HI_S32 HAL_Cipher_CalcHashInit(CIPHER_HASH_DATA_S *pCipherHashData)
{
    HI_S32 ret = HI_SUCCESS;
    CIPHER_SHA_CTRL_U unCipherSHACtrl;
    CIPHER_SHA_START_U unCipherSHAStart;
    HI_CHIP_TYPE_E enChipType = HI_CHIP_TYPE_BUTT;
    HI_CHIP_VERSION_E enChipVersion = HI_CHIP_VERSION_BUTT;
    HI_U32 u32WriteData = 0;
    HI_U32 i = 0;

    if( NULL == pCipherHashData )
    {
        HI_ERR_CIPHER("Error! Null pointer input!\n");
        (HI_VOID)HAL_Cipher_HashSoftReset();
        return HI_FAILURE;
    }

    /* set little-endian for cv200es */
    (HI_VOID)HI_DRV_SYS_GetChipVersion(&enChipType, &enChipVersion);
    if((HI_CHIP_TYPE_HI3716CES == enChipType) && (HI_CHIP_VERSION_V200 == enChipVersion))
    {
        CIPHER_WRITE_REG(CIPHER_SEC_MISC_CTR_ADDR, 0x2);
    }

    /* wait for hash_rdy */
    ret = HASH_WaitReady(HASH_READY);
    if(HI_SUCCESS != ret)
    {
        HI_ERR_CIPHER("Hash wait ready failed!\n");
        (HI_VOID)HAL_Cipher_HashSoftReset();
        return HI_FAILURE;
    }

    /* set hmac-sha key */
    if( ((HI_UNF_CIPHER_HASH_TYPE_HMAC_SHA1 == pCipherHashData->enShaType) || (HI_UNF_CIPHER_HASH_TYPE_HMAC_SHA256 == pCipherHashData->enShaType))
        && (HI_CIPHER_HMAC_KEY_FROM_CPU == pCipherHashData->enHMACKeyFrom) )
    {
        for( i = 0; i < CIPHER_HMAC_KEY_LEN; i = i + 4)
        {
            u32WriteData = (pCipherHashData->u8HMACKey[3+i] << 24) |
                           (pCipherHashData->u8HMACKey[2+i] << 16) |
                           (pCipherHashData->u8HMACKey[1+i] << 8)  |
                           (pCipherHashData->u8HMACKey[0+i]);
            CIPHER_WRITE_REG(CIPHER_HASH_REG_MCU_KEY0 + i, u32WriteData);
        }
    }

    /* write total len low and high */
    CIPHER_WRITE_REG(CIPHER_HASH_REG_TOTALLEN_LOW_ADDR, pCipherHashData->u32TotalDataLen + pCipherHashData->u32PaddingLen);
    CIPHER_WRITE_REG(CIPHER_HASH_REG_TOTALLEN_HIGH_ADDR, 0);

    /* config sha_ctrl : read by dma first, and by cpu in the hash final function */
    unCipherSHACtrl.u32 = 0;
    unCipherSHACtrl.bits.read_ctrl = 0;
    if( HI_UNF_CIPHER_HASH_TYPE_SHA1 == pCipherHashData->enShaType )
    {
        unCipherSHACtrl.bits.hardkey_hmac_flag = 0;
        unCipherSHACtrl.bits.sha_sel= 0x0;
    }
    else if( HI_UNF_CIPHER_HASH_TYPE_SHA256 == pCipherHashData->enShaType )
    {
        unCipherSHACtrl.bits.hardkey_hmac_flag = 0;
        unCipherSHACtrl.bits.sha_sel= 0x1;
    }
    else if( HI_UNF_CIPHER_HASH_TYPE_HMAC_SHA1 == pCipherHashData->enShaType )
    {
        unCipherSHACtrl.bits.hardkey_hmac_flag = 1;
        unCipherSHACtrl.bits.sha_sel= 0x0;
        unCipherSHACtrl.bits.hardkey_sel = pCipherHashData->enHMACKeyFrom;
    }
    else if( HI_UNF_CIPHER_HASH_TYPE_HMAC_SHA256 == pCipherHashData->enShaType )
    {
        unCipherSHACtrl.bits.hardkey_hmac_flag = 1;
        unCipherSHACtrl.bits.sha_sel= 0x1;
        unCipherSHACtrl.bits.hardkey_sel = pCipherHashData->enHMACKeyFrom;
    }
    else
    {
        HI_ERR_CIPHER("Invalid hash type input!\n");
        (HI_VOID)HAL_Cipher_HashSoftReset();
        return HI_FAILURE;
    }

    CIPHER_WRITE_REG(CIPHER_HASH_REG_CTRL_ADDR, unCipherSHACtrl.u32);    
    
    /* config sha_start */
    unCipherSHAStart.u32 = 0;
    unCipherSHAStart.bits.sha_start = 1;
    CIPHER_WRITE_REG(CIPHER_HASH_REG_START_ADDR, unCipherSHAStart.u32);

    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_CalcHashUpdate(CIPHER_HASH_DATA_S *pCipherHashData)
{
    HI_S32 s32Ret = HI_SUCCESS;
    CIPHER_SHA_STATUS_U unCipherSHAStatus;
    CIPHER_SHA_CTRL_U unCipherSHACtrl;
    MMZ_BUFFER_S stMMZBuffer = {0};
    HI_U32 u32WriteData = 0;
    HI_U32 u32WriteLength = 0;
    HI_U8 *pu8Ptr = NULL;

    if( (NULL == pCipherHashData) || ( NULL == pCipherHashData->pu8InputData) )
    {
        HI_ERR_CIPHER("Error, Null pointer input!\n");
        (HI_VOID)HAL_Cipher_HashSoftReset();
        return HI_FAILURE;
    }

    s32Ret = HI_DRV_MMZ_AllocAndMap("HASH", NULL, pCipherHashData->u32InputDataLen, 0, &stMMZBuffer);
    if( HI_SUCCESS != s32Ret )
    {
        HI_ERR_CIPHER("Error, mmz alloc and map failed!\n");
        (HI_VOID)HAL_Cipher_HashSoftReset();
        return HI_FAILURE;
    }

    unCipherSHAStatus.u32 = 0;
    u32WriteLength = pCipherHashData->u32InputDataLen - pCipherHashData->u32InputDataLen % 4;
    memcpy((HI_U8 *)stMMZBuffer.u32StartVirAddr, pCipherHashData->pu8InputData, u32WriteLength);

    /* wait for rec_ready */
    s32Ret= HASH_WaitReady(REC_READY);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_CIPHER("Hash wait ready failed!\n");
        s32Ret = HI_FAILURE;
        (HI_VOID)HAL_Cipher_HashSoftReset();
        goto __QUIT__;
    }

    /* small endian */
    if( 0 != u32WriteLength )
    {
        CIPHER_WRITE_REG(CIPHER_HASH_REG_DMA_START_ADDR, stMMZBuffer.u32StartPhyAddr);
        CIPHER_WRITE_REG(CIPHER_HASH_REG_DMA_LEN, u32WriteLength);
    }

    u32WriteLength = pCipherHashData->u32InputDataLen % 4;
    if( 0 == u32WriteLength )
    {
        s32Ret = HI_SUCCESS;
        goto __QUIT__;
    }

    pu8Ptr = pCipherHashData->pu8InputData + pCipherHashData->u32InputDataLen - u32WriteLength;

    /* the last round , if input data is not 4bytes aligned */  
    s32Ret  = HASH_WaitReady(REC_READY);
    s32Ret |= HASH_WaitReady(DMA_READY);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_CIPHER("Hash wait ready failed!\n");
        s32Ret = HI_FAILURE;
        (HI_VOID)HAL_Cipher_HashSoftReset();
        goto __QUIT__;
    }

    unCipherSHACtrl.u32 = 0;
    CIPHER_READ_REG(CIPHER_HASH_REG_CTRL_ADDR, unCipherSHACtrl.u32);
    unCipherSHACtrl.bits.read_ctrl = 1;
    CIPHER_WRITE_REG(CIPHER_HASH_REG_CTRL_ADDR, unCipherSHACtrl.u32); 
    
    /* small endian */
    if( 1 == u32WriteLength )
    {
        u32WriteData = ( pCipherHashData->u8Padding[2] << 24)
                     | ( pCipherHashData->u8Padding[1] << 16)
                     | ( pCipherHashData->u8Padding[0] << 8 )
                     |   pu8Ptr[0];
        CIPHER_WRITE_REG(CIPHER_HASH_REG_DATA_IN, u32WriteData);
    }
    else if( 2 == u32WriteLength )
    {
        u32WriteData = ( pCipherHashData->u8Padding[1] << 24)
                     | ( pCipherHashData->u8Padding[0] << 16)
                     | ( pu8Ptr[1] << 8 )
                     |   pu8Ptr[0];
        CIPHER_WRITE_REG(CIPHER_HASH_REG_DATA_IN, u32WriteData);
    }
    else if( 3 == u32WriteLength )
    {
        u32WriteData = ( pCipherHashData->u8Padding[0] << 24)
                     | ( pu8Ptr[2] << 16)
                     | ( pu8Ptr[1] << 8 )
                     |   pu8Ptr[0];
        CIPHER_WRITE_REG(CIPHER_HASH_REG_DATA_IN, u32WriteData);
    }
    else
    {
        /* the input data is 4bytes aligned */
    }

__QUIT__:
    HI_DRV_MMZ_UnmapAndRelease(&stMMZBuffer);

    /* the last step: make sure rec_ready */
    s32Ret= HASH_WaitReady(REC_READY);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_CIPHER("Hash wait ready failed!\n");
        (HI_VOID)HAL_Cipher_HashSoftReset();
        s32Ret = HI_FAILURE;
    }

    return s32Ret;
}

HI_S32 HAL_Cipher_CalcHashFinal(CIPHER_HASH_DATA_S *pCipherHashData)
{
    HI_S32 s32Ret = HI_SUCCESS;
    CIPHER_SHA_STATUS_U unCipherSHAStatus;
    CIPHER_SHA_CTRL_U unCipherSHACtrl;
    HI_U32 u32WriteData = 0;
	HI_U32 sha_out[8];
    HI_U32 i = 0;
    HI_U32 u32WritePaddingLength = 0;
    HI_U32 u32DataLengthNotAligned = 0;
    HI_U32 u32StartFromPaddingBuffer = 0;

    if( (NULL == pCipherHashData) || (NULL == pCipherHashData->pu8Output) )
    {
        HI_ERR_CIPHER("Error, Null pointer input!\n");
        (HI_VOID)HAL_Cipher_HashSoftReset();
        return HI_FAILURE;
    }

    /* write padding data */
    unCipherSHAStatus.u32 = 0;
    u32DataLengthNotAligned = pCipherHashData->u32TotalDataLen % 4;
    u32StartFromPaddingBuffer = (0 == u32DataLengthNotAligned)?(0):(4 - u32DataLengthNotAligned);
    u32WritePaddingLength = pCipherHashData->u32PaddingLen - u32StartFromPaddingBuffer;

    if( 0 != (u32WritePaddingLength % 4) )
    {
        HI_ERR_CIPHER("Error, Padding length not aligned: %d!\n", (u32WritePaddingLength % 4));
        (HI_VOID)HAL_Cipher_HashSoftReset();
        return HI_FAILURE;
    }

    for( i = u32StartFromPaddingBuffer; i < pCipherHashData->u32PaddingLen; i = i + 4)
    {
        /* wait for rec_ready */
        s32Ret  = HASH_WaitReady(REC_READY);
        s32Ret |= HASH_WaitReady(DMA_READY);
        if(HI_SUCCESS != s32Ret)
        {
            HI_ERR_CIPHER("Hash wait ready failed!\n");
            (HI_VOID)HAL_Cipher_HashSoftReset();
            return HI_FAILURE;
        }

        unCipherSHACtrl.u32 = 0;
        CIPHER_READ_REG(CIPHER_HASH_REG_CTRL_ADDR, unCipherSHACtrl.u32);
        unCipherSHACtrl.bits.read_ctrl = 1;
        CIPHER_WRITE_REG(CIPHER_HASH_REG_CTRL_ADDR, unCipherSHACtrl.u32); 

        /* small endian */
        u32WriteData = (pCipherHashData->u8Padding[3+i] << 24) | (pCipherHashData->u8Padding[2+i] << 16) |(pCipherHashData->u8Padding[1+i] << 8) | pCipherHashData->u8Padding[0+i];
        CIPHER_WRITE_REG(CIPHER_HASH_REG_DATA_IN, u32WriteData);
    }
 
    /* wait for hash_ready */
    s32Ret= HASH_WaitReady(HASH_READY);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_CIPHER("Hash wait ready failed!\n");
        (HI_VOID)HAL_Cipher_HashSoftReset();
        return HI_FAILURE;
    }

    /* read digest */
    unCipherSHAStatus.u32 = 0;
    CIPHER_READ_REG(CIPHER_HASH_REG_STATUS_ADDR, unCipherSHAStatus.u32);

    if( 0x00 == unCipherSHAStatus.bits.error_state )
    {
        memset(sha_out, 0x0, sizeof(sha_out));
        if( (HI_UNF_CIPHER_HASH_TYPE_SHA1 == pCipherHashData->enShaType)
         || (HI_UNF_CIPHER_HASH_TYPE_HMAC_SHA1 == pCipherHashData->enShaType))
        {
    		CIPHER_READ_REG(CIPHER_HASH_REG_SHA_OUT1, sha_out[0]);
    		CIPHER_READ_REG(CIPHER_HASH_REG_SHA_OUT2, sha_out[1]);
    		CIPHER_READ_REG(CIPHER_HASH_REG_SHA_OUT3, sha_out[2]);
    		CIPHER_READ_REG(CIPHER_HASH_REG_SHA_OUT4, sha_out[3]);
    		CIPHER_READ_REG(CIPHER_HASH_REG_SHA_OUT5, sha_out[4]);

    		for(i = 0; i < 5; i++)
    		{
    		    /* small endian */
    			pCipherHashData->pu8Output[i * 4 + 3] = sha_out[i] >> 24;
    			pCipherHashData->pu8Output[i * 4 + 2] = sha_out[i] >> 16;
    			pCipherHashData->pu8Output[i * 4 + 1] = sha_out[i] >> 8;
    			pCipherHashData->pu8Output[i * 4]     = sha_out[i];
    		}
        }
        else if( (HI_UNF_CIPHER_HASH_TYPE_SHA256 == pCipherHashData->enShaType )
              || (HI_UNF_CIPHER_HASH_TYPE_HMAC_SHA256 == pCipherHashData->enShaType))
        {
    		CIPHER_READ_REG(CIPHER_HASH_REG_SHA_OUT1, sha_out[0]);
    		CIPHER_READ_REG(CIPHER_HASH_REG_SHA_OUT2, sha_out[1]);
    		CIPHER_READ_REG(CIPHER_HASH_REG_SHA_OUT3, sha_out[2]);
    		CIPHER_READ_REG(CIPHER_HASH_REG_SHA_OUT4, sha_out[3]);
    		CIPHER_READ_REG(CIPHER_HASH_REG_SHA_OUT5, sha_out[4]);
    		CIPHER_READ_REG(CIPHER_HASH_REG_SHA_OUT6, sha_out[5]);
    		CIPHER_READ_REG(CIPHER_HASH_REG_SHA_OUT7, sha_out[6]);
    		CIPHER_READ_REG(CIPHER_HASH_REG_SHA_OUT8, sha_out[7]);

    		for(i = 0; i < 8; i++)
    		{
    		    /* small endian */
    			pCipherHashData->pu8Output[i * 4 + 3] = sha_out[i] >> 24;
    			pCipherHashData->pu8Output[i * 4 + 2] = sha_out[i] >> 16;
    			pCipherHashData->pu8Output[i * 4 + 1] = sha_out[i] >> 8;
    			pCipherHashData->pu8Output[i * 4]     = sha_out[i];
    		}
        }
        else
        {
            HI_ERR_CIPHER("Invalid hash type : %d!\n", pCipherHashData->enShaType);
            (HI_VOID)HAL_Cipher_HashSoftReset();
            return HI_FAILURE;
        }
    }
    else
    {
        HI_ERR_CIPHER("Error! SHA Status Reg: error_state = %d!\n", unCipherSHAStatus.bits.error_state);
        (HI_VOID)HAL_Cipher_HashSoftReset();
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_HashSoftReset(HI_VOID)
{
    CIPHER_SHA_RST_U unShaRst;

    unShaRst.u32 = 0;
	CIPHER_READ_REG(CIPHER_REG_SYS_CLK_SHA_ADDR, unShaRst.u32);
    unShaRst.bits.sha_cken = 1;
    unShaRst.bits.sha_srst_req = 1;
    CIPHER_WRITE_REG(CIPHER_REG_SYS_CLK_SHA_ADDR, unShaRst.u32);

    unShaRst.u32 = 0;
	CIPHER_READ_REG(CIPHER_REG_SYS_CLK_SHA_ADDR, unShaRst.u32);
    unShaRst.bits.sha_cken = 1;
    unShaRst.bits.sha_srst_req = 0;
    CIPHER_WRITE_REG(CIPHER_REG_SYS_CLK_SHA_ADDR, unShaRst.u32);

    return HI_SUCCESS;
}

#if 0
HI_VOID HAL_Cipher_Init(void)
{
    HI_U32 CipherCrgValue;

#if 0 //by g00182102 fro debug clock register
    HI_U32 uCipherValue = 0;
#endif

    CIPHER_READ_REG(CIPHER_REG_SYS_CLK_CA_ADDR,CipherCrgValue);
    
    /* open clock [ref Hi3716C/Hi3716H/Hi3716M user guide section PERI_CRG29]
        [31:10] reserved
        [9] CA EFUSE clock status. 0-closed 1-opened
        [8] CA clock status. 0-closed 1-opened
        [7:2] reserved
        [1] CA EFUSE soft reset request. 0 cancel reset, 1-reset request
        [0] CA CI soft reset. 0-cancel reset 1-reset request
        */

    /* reset clock, so open clock and set [1:0] to 3, that is 0x0103 */
    HAL_SET_BIT(CipherCrgValue, 0); /* set the bit 0, CI reset request */
    HAL_SET_BIT(CipherCrgValue, 1); /* set the bit 1, EFUSE reset request */
    HAL_SET_BIT(CipherCrgValue, 8); /* set the bit 8, CA clock opened */
    HAL_SET_BIT(CipherCrgValue, 9); /* set the bit 9, CA EFUSE clock opened */

    CIPHER_WRITE_REG(CIPHER_REG_SYS_CLK_CA_ADDR, CipherCrgValue);

    #if 0 //debug by g00182102 debug clock register
    CIPHER_READ_REG(CIPHER_REG_SYS_CLK_CA_ADDR,uCipherValue);
    HI_INFO_CIPHER("after reset clock register:0x%08x\n", uCipherValue);
    #endif
    
    /* clock select and cancel reset */
    HAL_CLEAR_BIT(CipherCrgValue, 0); /* clear bit 0, for cancel reset CI bit request */
    HAL_CLEAR_BIT(CipherCrgValue, 1); /* clear bit 1, for cancel reset KL bit request */

    HAL_SET_BIT(CipherCrgValue, 8); /* make sure clock opened */
	HAL_SET_BIT(CipherCrgValue, 9); /* make sure clock opened */
    
    CIPHER_WRITE_REG(CIPHER_REG_SYS_CLK_CA_ADDR, CipherCrgValue);

    #if 0 //debug by g00182102 debug clock register
    CIPHER_READ_REG(CIPHER_REG_SYS_CLK_CA_ADDR,uCipherValue);
    HI_INFO_CIPHER("after cancel reset clock register:0x%08x\n", uCipherValue);
    #endif

    return;
}
#endif

HI_VOID HAL_Cipher_Init(void)
{
    CIPHER_CA_SYS_CLK_U unCRG;

    unCRG.u32 = 0;

#ifdef CHIP_TYPE_hi3716cv200es
    unCRG.bits.ca_kl_srst_req = 1;
    unCRG.bits.ca_ci_srst_req = 1;
    unCRG.bits.otp_srst_req = 1;
    /* clock open */
    unCRG.bits.ca_ci_bus_cken = 1;
    unCRG.bits.ca_ci_cken = 1;

    /* ca clock select : 200M */
    unCRG.bits.ca_clk_sel = 0;
    CIPHER_WRITE_REG(CIPHER_REG_SYS_CLK_CA_ADDR, unCRG.u32);

    unCRG.bits.ca_kl_srst_req = 0;
    unCRG.bits.ca_ci_srst_req = 0;
    unCRG.bits.otp_srst_req = 0;

    /* make sure clock opened */
    unCRG.bits.ca_ci_bus_cken = 1;
    unCRG.bits.ca_ci_cken = 1;
    /* make sure ca clock select : 200M */
    unCRG.bits.ca_clk_sel = 0;
    CIPHER_WRITE_REG(CIPHER_REG_SYS_CLK_CA_ADDR, unCRG.u32);
#else
    unCRG.bits.ca_kl_srst_req = 1;
    unCRG.bits.ca_ci_srst_req = 1;
    unCRG.bits.otp_srst_req = 1;

    /* ca clock select : 200M */
    unCRG.bits.ca_ci_clk_sel = 0;
    CIPHER_WRITE_REG(CIPHER_REG_SYS_CLK_CA_ADDR, unCRG.u32);

    unCRG.bits.ca_kl_srst_req = 0;
    unCRG.bits.ca_ci_srst_req = 0;
    unCRG.bits.otp_srst_req = 0;

    /* make sure ca clock select : 200M */
    unCRG.bits.ca_ci_clk_sel = 0;
    CIPHER_WRITE_REG(CIPHER_REG_SYS_CLK_CA_ADDR, unCRG.u32);
#endif

    return;
}

HI_VOID HAL_Cipher_DeInit(void)
{
    /* use the same clock of CA */
    //CIPHER_WRITE_REG(CIPHER_REG_SYS_CLK_CA_ADDR, 0x0103); 
    //CIPHER_WRITE_REG(CIPHER_REG_SYS_CLK_CA_ADDR, 0x3); /* close cipher's clock */
    return;
}



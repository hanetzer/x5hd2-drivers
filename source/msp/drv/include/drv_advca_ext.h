#ifndef __ADVCA_EXT_H_
#define __ADVCA_EXT_H_

#include "hi_type.h"

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

typedef HI_S32  (*FN_ADVCA_DecryptCipher)(HI_U32 AddrID, HI_U32 *pDataIn);
typedef HI_S32  (*FN_ADVCA_DecryptSP)(HI_U32 AddrID,HI_U32 EvenOrOdd, HI_U8 *pData);
typedef HI_S32  (*FN_ADVCA_DecryptCws)(HI_U32 AddrID,HI_U32 EvenOrOdd, HI_U8 *pData);
typedef HI_S32  (*FN_ADVCA_DecryptCsa3s)(HI_U32 AddrID,HI_U32 EvenOrOdd, HI_U8 *pData);

typedef struct
{
    FN_ADVCA_DecryptCipher       pfnAdvcaDecryptCipher;
    FN_ADVCA_DecryptSP           pfnAdvcaDecryptSP;
    FN_ADVCA_DecryptCws          pfnAdvcaDecryptCws;
    FN_ADVCA_DecryptCsa3s        pfnAdvcaDecryptCsa3s;
} ADVCA_EXPORT_FUNC_S;


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif   /* __ADVCA_EXT_H_ */


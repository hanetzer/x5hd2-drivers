#include "hi_drv_hdmi.h"

typedef struct hiHDMI_DELAY_TIME_S{
    HI_U8           u8IDManufactureName[4];   /**<Manufacture name*//**<CNcomment:�豸���̱�ʶ */
    HI_U32          u32IDProductCode;         /**<Product code*//**<CNcomment:�豸ID */
    HI_U32          u32DelayTimes;
    HI_U8           u8ProductType[32];    /**<Product Type*//**<CNcomment:��Ʒ�ͺ� */
}HDMI_DELAY_TIME_S;

HI_S32 SetFormatDelay(HI_UNF_HDMI_SINK_CAPABILITY_S *sinkCap,HI_U32 *DelayTime);


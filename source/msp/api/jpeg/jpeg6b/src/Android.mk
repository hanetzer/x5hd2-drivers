LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_ARM_MODE := arm

LOCAL_SRC_FILES := jcapimin.c jcapistd.c jccoefct.c jccolor.c jcdctmgr.c jchuff.c \
        jcinit.c jcmainct.c jcmarker.c jcmaster.c jcparam.c \
        jcphuff.c jcprepct.c jcsample.c jctrans.c \
        jmem-android.c  hijpeg_decode_hw.c hi_jpeg_api.c \
        jcomapi.c jdapimin.c jdapistd.c \
        jdatadst.c jdatasrc.c jdcoefct.c jdcolor.c jddctmgr.c jdhuff.c \
        jdinput.c jdmainct.c jdmarker.c jdmaster.c jdmerge.c jdphuff.c \
        jdpostct.c  jdsample.c jdtrans.c jerror.c jfdctflt.c jfdctfst.c \
        jfdctint.c jidctflt.c jidctred.c jquant1.c \
        jquant2.c jutils.c jmemmgr.c

LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../device/hisi/Hi3716C/driver/sdk/msp/common/include/
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../device/hisi/Hi3716C/driver/sdk/msp_base/graphics/tde/include/
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../device/hisi/Hi3716C/driver/sdk/msp/graphics/jpeg/driver/include/

# temp fix until we understand why this broke cnn.com
#ANDROID_JPEG_NO_ASSEMBLER := true

ifeq ($(strip $(ANDROID_JPEG_NO_ASSEMBLER)),true)
LOCAL_SRC_FILES += jidctint.c jidctfst.c
else
LOCAL_SRC_FILES += jidctint.c jidctfst.S
endif

LOCAL_CFLAGS += -DAVOID_TABLES
LOCAL_CFLAGS += -DANDROID_JPEG6B
#判断芯片是否支持硬件解码
#LOCAL_CFLAGS += -DJPEG6B_DEBUG

LOCAL_CFLAGS += -g3 -fstrict-aliasing -fprefetch-loop-arrays

LOCAL_C_INCLUDES += \
	external/dvmMalloc

LOCAL_SHARED_LIBRARIES := libtde libcutils  libutils libdvmmalloc
LOCAL_MODULE:= libjpeg
ALL_DEFAULT_INSTALLED_MODULES += $(LOCAL_MODULE)

include $(BUILD_SHARED_LIBRARY)


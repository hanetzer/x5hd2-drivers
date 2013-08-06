#===============================================================================
# export variables
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
    ifneq ($(KERNELRELEASE),)
    KERNEL_DIR := $(srctree)

    SDK_DIR := $(KERNEL_DIR)/../../..
    else
    SDK_DIR := $(CURDIR)/../../../..
    endif

include $(SDK_DIR)/base.mak
endif

KBUILD_EXTRA_SYMBOLS += $(COMMON_DIR)/drv/Module.symvers

TARGET := jpeg


#===============================================================================
# local variables
#===============================================================================
EXTRA_CFLAGS += -I$(COMMON_UNF_INCLUDE)                 \
                -I$(COMMON_API_INCLUDE)                 \
                -I$(COMMON_DRV_INCLUDE)                 \
                -I$(MSP_UNF_INCLUDE)                    \
                -I$(MSP_API_INCLUDE)                    \
                -I$(MSP_DRV_INCLUDE)                    \
                -I$(MSP_DIR)/api/$(TARGET)/include      \
                -I$(MSP_DIR)/drv/$(TARGET)/include      \
                -I$(MSP_DIR)/drv/$(TARGET)/src     

EXTRA_CFLAGS += $(CFG_HI_KMOD_CFLAGS)
#ifeq ($(CFG_HI_CHIP_TYPE),hi3712)
#EXTRA_CFLAGS += -DJPGD_BASE_ADDR=0x101a0000
#else
#EXTRA_CFLAGS += -DJPGD_BASE_ADDR=0x60100000
#endif

MOD_NAME := hi_$(TARGET)


obj-$(CFG_MSP_BUILDTYPE) += $(MOD_NAME).o


$(MOD_NAME)-y   := src/jpg_hal.o \
             src/hijpeg_proc.o 

#ifeq ($(CFG_HI_MCE_SUPPORT),y)
#$(MOD_NAME)-y   +=  src/jpg_osr_ko.o 
#$(MOD_NAME)-y   +=  src/jpg_osr_kernel.o 
#else
$(MOD_NAME)-y   +=  src/jpg_osr.o 
#endif

#===============================================================================
#   rules
#===============================================================================
.PHONY: all clean modules_install

all:
	$(AT)make -C $(LINUX_DIR) M=$(CURDIR) modules

clean:
	$(AT)make -C $(LINUX_DIR) M=$(CURDIR) clean
	$(AT)rm -rf  *.o
	$(AT)rm -rf  .*.o.cmd
	$(AT)rm -rf .tmp_versions
	$(AT)rm -rf  Module.symvers
	$(AT)rm -rf  modules.order

modules_install:
	$(AT)make -C $(LINUX_DIR) M=$(CURDIR) modules_install


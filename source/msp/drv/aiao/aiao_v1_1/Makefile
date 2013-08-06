#======::=========================================================================
# export variables
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
    ifneq ($(KERNELRELEASE),)
    KERNEL_DIR := $(srctree)

    SDK_DIR := $(KERNEL_DIR)/../../..
    else
    SDK_DIR := $(CURDIR)/../../../../..
    endif

include $(SDK_DIR)/base.mak
endif

#===============================================================================
# local variables
#===============================================================================
ADSP_DRV_INCLUDE = $(MSP_DRV_INCLUDE)/adsp 

EXTRA_CFLAGS += $(CFG_HI_KMOD_CFLAGS)

EXTRA_CFLAGS += -I$(COMMON_UNF_INCLUDE)                 \
                -I$(COMMON_API_INCLUDE)                 \
                -I$(COMMON_DRV_INCLUDE)                 \
                -I$(MSP_UNF_INCLUDE)                    \
                -I$(MSP_API_INCLUDE)                    \
                -I$(MSP_DRV_INCLUDE)                    \
                -I$(ADSP_DRV_INCLUDE)                    \
		-I$(CURDIR)	

#COMMON include  
EXTRA_CFLAGS += -I$(SDK_DIR)/source/msp/drv/aiao/aiao_v1_1/common/include
#AO include 
EXTRA_CFLAGS += -I$(SDK_DIR)/source/msp/drv/aiao/aiao_v1_1/ao
#AI include 
ifeq ($(CFG_HI_SND_AI_SUPPORT),y)
EXTRA_CFLAGS += -I$(SDK_DIR)/source/msp/drv/aiao/aiao_v1_1/ai
EXTRA_CFLAGS += -DHI_SND_AI_SUPPORT
endif
#ALSA include 
ifeq ($(CFG_HI_SND_ALSA_AO_SUPPORT),y)
EXTRA_CFLAGS += -I$(SDK_DIR)/source/msp/drv/aiao/aiao_v1_1/alsa
EXTRA_CFLAGS += -DHI_ALSA_AO_SUPPORT
endif

#Simulate or DSP HW
#CFG_HI_SND_AOE_HW_SUPPORT = y	do not enable , include in base.mak

ifneq ($(CFG_HI_SND_AOE_HW_SUPPORT),y)
#simulate support
EXTRA_CFLAGS += -DHI_SND_AOE_SWSIMULATE_SUPPORT
endif

#===============================================================================
# HI_SND_DRV_TEMPLATE_OPT : stantard drvier template 
#===============================================================================
EXTRA_CFLAGS += -DHI_SND_DRV_TEMPLATE_OPT

#===============================================================================
# HI_SND_DRV_SUSPEND_SUPPORT : suspend & resume driver 
#===============================================================================
EXTRA_CFLAGS += -DHI_SND_DRV_SUSPEND_SUPPORT

#EXTRA_CFLAGS += -DHI_SND_FPGA

KBUILD_EXTRA_SYMBOLS += $(COMMON_DIR)/drv/Module.symvers

#EXTRA_CFLAGS += -DHIAO_DEBUG

MOD_NAME := hi_aiao

obj-$(CFG_MSP_BUILDTYPE) += $(MOD_NAME).o

$(MOD_NAME)-y := drv_aiao_module.o

#COMMON SUPPORT
$(MOD_NAME)-y += common/src/audio_util.o \
		 common/src/hal_aiao.o \
		 common/src/hal_aiao_func.o \
		 common/src/hal_aiao_priv.o 

#AO SUPPORT (default)
$(MOD_NAME)-y += ao/drv_ao.o \
		 ao/drv_ao_op.o \
		 ao/drv_ao_track.o \
		 ao/hal_aoe_func.o \
		 ao/hal_aoe.o \
		 ao/drv_ao_intf.o \
		 ao/hal_tianlai_adac.o 

#AI SUPPORT(optional)
ifeq ($(CFG_HI_SND_AI_SUPPORT),y)
$(MOD_NAME)-y += drv_ai_intf.o drv_ai_intf_k.o 
endif

#ALSA SUPPORT(optional)
ifeq ($(CFG_HI_SND_ALSA_AO_SUPPORT),y)
#ALSA drvier V1
#$(MOD_NAME)-y += alsa/hiaiao_codec.o \
		 alsa/htd_aiao.o \
		 alsa/htd_i2s.o \
		 alsa/htd_dma.o \
		 alsa/drv_aiao_alsa_proc.o \
		 alsa/drv_aiao_alsa_func.o 
#ALSA drvier V2
$(MOD_NAME)-y += alsa/hiaiao_card.o \
		 alsa/hiaiao_dma.o \
		 alsa/hiaiao_codec.o \
		 alsa/hiaiao_i2s.o \
		 alsa/alsa_aiao_proc_func.o \
		 alsa/drv_ao_func.o \
		 alsa/alsa_aiao_volume_func.o
endif

#CAST SUPPORT
ifeq ($(CFG_HI_SND_CAST_SUPPORT),y)
EXTRA_CFLAGS += -DHI_SND_CAST_SUPPORT
$(MOD_NAME)-y += ao/drv_ao_cast.o \
		 ao/hal_cast.o 
endif





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

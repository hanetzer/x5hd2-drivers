#===============================================================================
# export variable
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR ?= $(CURDIR)/../../..

include $(SDK_DIR)/base.mak
endif

HI_FLASH_DIR=flash

#===============================================================================
# local variable
#===============================================================================
CFLAGS  += $(CFG_HI_CFLAGS)

CFLAGS += -I$(MSP_DIR)/api/include
CFLAGS += -I$(COMMON_DIR)/include
CFLAGS += -I$(COMMON_DIR)/api/include
CFLAGS += -I$(COMMON_DIR)/drv/include
CFLAGS += -I$(COMMON_DIR)/api/stat
CFLAGS += -I$(COMMON_DIR)/api/sys
CFLAGS += -I$(COMMON_DIR)/api/log
CFLAGS += -I$(COMMON_DIR)/api/mem
CFLAGS += -I$(COMMON_DIR)/api/mmz
CFLAGS += -I$(COMMON_DIR)/api/module
CFLAGS += -I$(COMMON_DIR)/drv/module
CFLAGS += -I$(COMMON_DIR)/drv/mem

ifeq (y,$(CFG_HI_TEST_SUPPORT))
CFLAGS += -I$(COMMON_DIR)/drv/test
endif

LIBS := libhi_common
        
OBJS := ./sys/hi_common.o     \
        ./mmz/mpi_mmz.o       \
        ./mem/mpi_mem.o       \
        ./mem/mpi_mutils.o    \
        ./mem/mpi_memmap.o    \
        ./mem/mpi_mmgr.o      \
        ./module/mpi_module.o \
        ./log/mpi_log.o       \
        ./stat/mpi_stat.o

DEPENDS := $(patsubst %.o, %.d, $(OBJS))

ifeq (y,$(CFG_HI_TEST_SUPPORT))
CFLAGS += -DCMN_TEST_SUPPORTED
OBJS   += ./test/mpi_test.o
endif
PUB_HEADERS  := $(wildcard $(COMMON_DIR)/include/*.h)

ifeq ("$(origin V)", "command line")
ifeq ($V,0)
AT := @
endif
else
AT := @
endif

#===============================================================================
# rules
#===============================================================================
.PHONY: all clean install uninstall

all: $(DEPENDS) $(LIBS)
	make -C $(HI_FLASH_DIR) all

clean:
	rm -f $(LIBS).a
	rm -f $(LIBS).so
	find $(COMMON_DIR)/ -name "*.[dios]" -exec rm {} \;
	make -C $(HI_FLASH_DIR) clean
	
install:all
	@cp -f $(PUB_HEADERS) $(INCLUDE_DIR)/
	@cp -f $(LIBS).a  $(STATIC_LIB_DIR)
	@cp -f $(LIBS).so $(SHARED_LIB_DIR)
	@echo "Install common resources completed."
	make -C $(HI_FLASH_DIR) install 
	
uninstall:
	$(AT)cd $(INCLUDE_DIR) && rm -f $(notdir $(PUB_HEADERS)) ; cd - 1>/dev/null
	$(AT)rm -f $(STATIC_LIB_DIR)/$(LIBS).a
	$(AT)rm -f $(SHARED_LIB_DIR)/$(LIBS).so
	make -C $(HI_FLASH_DIR) uninstall 
	$(AT)echo "Uninstall common resources completed."

#################################################################################
## Building library
$(LIBS): $(OBJS)
	$(AT)$(AR) -rcs $@.a $^
	$(AT)$(CC) -shared -fPIC -o $@.so $^
	$(AT)echo "Build $@ has completed."

## Building api objects
## using: -save-temps for temporary files
#${OBJS}: %.o : %.c
#	$(AT)echo "Compiling $@..."
#	$(AT)$(CC) $(CFLAGS) -c $< -o $@

$(DEPENDS):%.d:%.c
	@echo "[$(CC)] $@..."
	@set -e; rm -f $@; \
	$(CC) -MM $(CFLAGS) $< > $@.$$$$; \
	sed 's,\($(notdir $*)\)\.o[ :]*,$(dir $@)\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$
	
-include $(DEPENDS)
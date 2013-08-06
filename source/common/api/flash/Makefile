################################################################################
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR     := $(shell pwd)/../../../..

include $(SDK_DIR)/base.mak
endif

HIFLASH_DIR := $(SDK_DIR)/source/common/api/flash

################################################################################
CROSS_COMPILE         := \
	$(if $(CROSS_COMPILE),$(CROSS_COMPILE),arm-hisiv200-linux-)

CC    ?= $(CROSS_COMPILE)gcc
AR    ?= $(CROSS_COMPILE)ar

################################################################################
source =  $(HIFLASH_DIR)/src/hi_flash.c
source +=  $(HIFLASH_DIR)/src/nand.c
source +=  $(HIFLASH_DIR)/src/spi_raw.c
source +=  $(HIFLASH_DIR)/src/nand_raw.c
source +=  $(HIFLASH_DIR)/src/emmc_raw.c

objs=$(source:.c=.o)

CFG_INC = -I$(HIFLASH_DIR)/include
CFG_INC += -I$(SDK_DIR)/source/common/include
ifndef CFG_HI_CFLAGS
CFG_HI_CFLAGS= -Wall -g -march=armv5te -ggdb
endif

SLIB := libhiflash.a
DLIB := libhiflash.so
	
target= $(SLIB) $(DLIB)

all:$(target)
	
$(SLIB): $(objs)
	$(AR) -rc -o $@ $^

$(DLIB): ${objs}
	$(CC) -shared -o $@ $^

%.o: %.c
	@echo cc: $^
	@$(CC) $(CFG_HI_CFLAGS) -fPIC -c $^ -o "$(HIFLASH_DIR)/src/$(*F).o" $(CFG_INC)

install: all
	@-cp -rvf $(HIFLASH_DIR)/include/hi_flash.h $(INCLUDE_DIR)
	@-cp -rvf $(HIFLASH_DIR)/libhiflash.a $(STATIC_LIB_DIR)
	@-cp -rvf $(HIFLASH_DIR)/libhiflash.so $(SHARED_LIB_DIR)
	@echo "make $@ over !"

uninstall:
	@-rm -rf $(INCLUDE_DIR)/hi_flash.h
	@-rm -rf $(STATIC_LIB_DIR)/libhiflash.a
	@-rm -rf $(SHARED_LIB_DIR)/libhiflash.so
	@echo "make $@ over !"
clean:
	@rm *.o -rvf
	@rm -rf $(HIFLASH_DIR)/src/*.o
	@rm -f $(target)
	@echo "make $@ over !"

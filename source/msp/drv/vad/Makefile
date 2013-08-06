
#===============================================================================
#   rules
#===============================================================================
.PHONY: all clean install uninstall

all:
	$(AT)make -C adv7401
	$(AT)make -C ov7725
	$(AT)make -C tvp5150
	$(AT)make -C tw2864

clean:
	$(AT)make -C adv7401 clean
	$(AT)make -C ov7725  clean
	$(AT)make -C tvp5150 clean
	$(AT)make -C tw2864  clean

install: all
	$(AT)make -C adv7401 install
	$(AT)make -C ov7725  install
	$(AT)make -C tvp5150 install
	$(AT)make -C tw2864  install

uninstall:
	$(AT)make -C adv7401 uninstall
	$(AT)make -C ov7725  uninstall
	$(AT)make -C tvp5150 uninstall
	$(AT)make -C tw2864  uninstall
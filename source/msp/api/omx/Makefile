#=======================================
#	export varibale
#=======================================


#=======================================
#	local varibale
#=======================================
objects:= omx_core omx_vdec omx_venc


objects_clean:=${addsuffix _clean,${objects}}
objects_install:=${addsuffix _install,${objects}}
objects_uninstall:=${addsuffix _uninstall,${objects}}

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#	  rules
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.PHONY: all clean install uninstall ${objects} ${objects_clean} ${objects_install} ${objects_uninstall}


all:    ${objects}

clean:  ${objects_clean}

install:${objects_install}

uninstall:${objects_uninstall}
      	
${objects}:
	make -C $@

${objects_clean}:
	make -C ${patsubst %_clean,%,$@} clean

${objects_install}:
	make -C ${patsubst %_install,%,$@} install

${objects_uninstall}:
	make -C ${patsubst %_uninstall,%,$@} uninstall

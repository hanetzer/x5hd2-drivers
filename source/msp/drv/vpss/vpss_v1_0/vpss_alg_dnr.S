	.arch armv7-a
	.fpu softvfp
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 2
	.eabi_attribute 30, 2
	.eabi_attribute 18, 4
	.file	"vpss_alg_dnr.c"
@ GNU C (Hisilicon_v200(gcc4.4-290+glibc-2.11+eabi+nptl)) version 4.4.1 (arm-hisiv200-linux-gnueabi)
@	compiled by GNU C version 4.1.1 20061011 (Red Hat 4.1.1-30), GMP version 4.3.1, MPFR version 2.4.2.
@ GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
@ options passed:  -nostdinc
@ -I/home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/arch/arm/include
@ -Iarch/arm/include/generated -Iinclude
@ -I/home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/include
@ -I/home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/drivers/msp/vpss
@ -Idrivers/msp/vpss
@ -I/home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/arch/arm/mach-s40/include
@ -I/home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/../../../source/common/include
@ -I/home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/../../../source/common/api/include
@ -I/home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/../../../source/common/drv/include
@ -I/home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/../../../source/msp/include
@ -I/home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/../../../source/msp/api/include
@ -I/home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/../../../source/msp/drv/include
@ -imultilib armv7a_soft -iprefix
@ /opt/hisi-linux/x86-arm/arm-hisiv200-linux/bin/../lib/gcc/arm-hisiv200-linux-gnueabi/4.4.1/
@ -isysroot /opt/hisi-linux/x86-arm/arm-hisiv200-linux/bin/../target
@ -D__KERNEL__ -D__LINUX_ARM_ARCH__=7 -Uarm -DCHIP_TYPE_hi3716cv200es
@ -DSDK_VERSION=HiSTBLinuxV100R002C00SPC011 -DCFG_HI_LOG_LEVEL=4
@ -DHI_KEYLED_SUPPORT -DHI_SCI_SUPPORT -DHI_VENC_SUPPORT -DHI_AENC_SUPPORT
@ -DKBUILD_STR(s)=#s -DKBUILD_BASENAME=KBUILD_STR(vpss_alg_dnr)
@ -DKBUILD_MODNAME=KBUILD_STR(hi_vpss) -isystem
@ /opt/hisi-linux/x86-arm/arm-hisiv200-linux/bin/../lib/gcc/arm-hisiv200-linux-gnueabi/4.4.1/include
@ -include
@ /home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/include/linux/kconfig.h
@ -MD drivers/msp/vpss/vpss_v1_0/.vpss_alg_dnr.o.d
@ /home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/drivers/msp/vpss/vpss_v1_0/vpss_alg_dnr.c
@ -mlittle-endian -marm -mapcs -mno-sched-prolog -mabi=aapcs-linux
@ -mno-thumb-interwork -march=armv7-a -msoft-float -auxbase-strip
@ drivers/msp/vpss/vpss_v1_0/vpss_alg_dnr.o -O2 -Wall -Wundef
@ -Wstrict-prototypes -Wno-trigraphs -Werror-implicit-function-declaration
@ -Wno-format-security -Wframe-larger-than=1024
@ -Wdeclaration-after-statement -Wno-pointer-sign -p -fno-strict-aliasing
@ -fno-common -fno-delete-null-pointer-checks -fno-dwarf2-cfi-asm
@ -funwind-tables -fno-stack-protector -fno-omit-frame-pointer
@ -fno-optimize-sibling-calls -fno-strict-overflow -fconserve-stack
@ -fverbose-asm
@ options enabled:  -falign-loops -fargument-alias -fauto-inc-dec
@ -fbranch-count-reg -fcaller-saves -fcprop-registers -fcrossjumping
@ -fcse-follow-jumps -fdefer-pop -fearly-inlining
@ -feliminate-unused-debug-types -fexpensive-optimizations
@ -fforward-propagate -ffunction-cse -fgcse -fgcse-lm
@ -fguess-branch-probability -fident -fif-conversion -fif-conversion2
@ -findirect-inlining -finline -finline-functions-called-once
@ -finline-small-functions -fipa-cp -fipa-pure-const -fipa-reference
@ -fira-share-save-slots -fira-share-spill-slots -fivopts
@ -fkeep-static-consts -fleading-underscore -fmath-errno -fmerge-constants
@ -fmerge-debug-strings -fmove-loop-invariants -foptimize-register-move
@ -fpeephole -fpeephole2 -fprofile -fpromote-loop-indices
@ -freg-struct-return -fregmove -frename-registers -freorder-blocks
@ -freorder-functions -frerun-cse-after-loop -fsched-interblock
@ -fsched-spec -fsched-stalled-insns-dep -fschedule-insns -fschedule-insns2
@ -fsection-anchors -fsigned-zeros -fsplit-ivs-in-unroller
@ -fsplit-wide-types -fthread-jumps -ftoplevel-reorder -ftrapping-math
@ -ftree-builtin-call-dce -ftree-ccp -ftree-ch -ftree-copy-prop
@ -ftree-copyrename -ftree-cselim -ftree-dce -ftree-dominator-opts
@ -ftree-dse -ftree-fre -ftree-loop-im -ftree-loop-ivcanon
@ -ftree-loop-optimize -ftree-parallelize-loops= -ftree-pre -ftree-reassoc
@ -ftree-scev-cprop -ftree-sink -ftree-sra -ftree-switch-conversion
@ -ftree-ter -ftree-vect-loop-version -ftree-vrp -funit-at-a-time
@ -funroll-loops -funwind-tables -fverbose-asm -fweb
@ -fzero-initialized-in-bss -mapcs-frame -mglibc -mlittle-endian

@ Compiler executable checksum: fa59e0bd179ef45b3a3c3e6f3383fdac

	.text
	.align	2
	.global	DnrThdParaInit
	.type	DnrThdParaInit, %function
DnrThdParaInit:
	.fnstart
.LFB580:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	.movsp ip
	mov	ip, sp	@,
	stmfd	sp!, {fp, ip, lr, pc}	@,
	.pad #4
	.save {fp, ip, lr}
	.setfp fp, ip, #-4
	sub	fp, ip, #4	@,,
	push	{lr}
	bl	__gnu_mcount_nc
	mov	r3, #8	@ tmp134,
	mov	r2, #4	@ tmp142,
	mov	r1, #0	@ tmp139,
	mov	ip, #16	@ tmp137,
	str	r3, [r0, #72]	@ tmp134, <variable>.stDbThd.dbbetascale
	str	ip, [r0, #44]	@ tmp137, <variable>.stDrThd.drthrmaxsimilarpixdiff
	mov	ip, #6	@ tmp143,
	str	r2, [r0, #84]	@ tmp142, <variable>.stDbThd.dbthrmaxdiffvert
	str	ip, [r0, #60]	@ tmp143, <variable>.stDbThd.detailimgqpthr
	mov	ip, #2	@ tmp146,
	str	r1, [r0, #56]	@ tmp139, <variable>.stDbThd.dbvertasprog
	str	ip, [r0, #88]	@ tmp146, <variable>.stDbThd.dbthrleastblkdiffhor
	mov	ip, #14	@ tmp148,
	str	r3, [r0, #32]	@ tmp134, <variable>.stDrThd.dralphascale
	str	ip, [r0, #80]	@ tmp148, <variable>.stDbThd.dbthrmaxdiffhor
	str	r3, [r0, #36]	@ tmp134, <variable>.stDrThd.drbetascale
	str	r3, [r0, #40]	@ tmp134, <variable>.stDrThd.drthrflat3x3zone
	str	r3, [r0, #48]	@ tmp134, <variable>.stDbThd.picestqp
	str	r1, [r0, #52]	@ tmp139, <variable>.stDbThd.dbuseweakflt
	str	r3, [r0, #64]	@ tmp134, <variable>.stDbThd.dbthredge
	str	r2, [r0, #76]	@ tmp142, <variable>.stDbThd.dbthrlagesmooth
	str	r2, [r0, #68]	@ tmp142, <variable>.stDbThd.dbalphascale
	str	r2, [r0, #92]	@ tmp142, <variable>.stDbThd.dbthrleastblkdiffvert
	ldmfd	sp, {fp, sp, pc}	@
	.fnend
	.size	DnrThdParaInit, .-DnrThdParaInit
	.align	2
	.global	ALG_DnrInit
	.type	ALG_DnrInit, %function
ALG_DnrInit:
	.fnstart
.LFB581:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	.movsp ip
	mov	ip, sp	@,
	stmfd	sp!, {fp, ip, lr, pc}	@,
	.pad #4
	.save {fp, ip, lr}
	.setfp fp, ip, #-4
	sub	fp, ip, #4	@,,
	push	{lr}
	bl	__gnu_mcount_nc
	ldr	r2, [r0, #0]	@ <variable>.drEn, <variable>.drEn
	mov	ip, r0	@ pstDrvPara, pstDrvPara
	mov	r0, r1	@, pstDnrRtlPara
	str	r2, [r1, #0]	@ <variable>.drEn, <variable>.drEn
	ldr	r3, [ip, #4]	@ <variable>.dbEn, <variable>.dbEn
	str	r3, [r1, #4]	@ <variable>.dbEn, <variable>.dbEn
	ldr	lr, [ip, #8]	@ <variable>.dbEnHort, <variable>.dbEnHort
	str	lr, [r1, #8]	@ <variable>.dbEnHort, <variable>.dbEnHort
	ldr	r2, [ip, #12]	@ <variable>.dbEnVert, <variable>.dbEnVert
	str	r2, [r1, #12]	@ <variable>.dbEnVert, <variable>.dbEnVert
	ldr	r3, [ip, #16]	@ <variable>.u32YInfoAddr, <variable>.u32YInfoAddr
	str	r3, [r1, #16]	@ <variable>.u32YInfoAddr, <variable>.u32YInfoAddr
	ldr	lr, [ip, #20]	@ <variable>.u32CInfoAddr, <variable>.u32CInfoAddr
	str	lr, [r1, #20]	@ <variable>.u32CInfoAddr, <variable>.u32CInfoAddr
	ldr	r2, [ip, #24]	@ <variable>.u32YInfoStride, <variable>.u32YInfoStride
	str	r2, [r1, #24]	@ <variable>.u32YInfoStride, <variable>.u32YInfoStride
	ldr	r3, [ip, #28]	@ <variable>.u32CInfoStride, <variable>.u32CInfoStride
	str	r3, [r1, #28]	@ <variable>.u32CInfoStride, <variable>.u32CInfoStride
	bl	DnrThdParaInit	@
	ldmfd	sp, {fp, sp, pc}	@
	.fnend
	.size	ALG_DnrInit, .-ALG_DnrInit
	.ident	"GCC: (Hisilicon_v200(gcc4.4-290+glibc-2.11+eabi+nptl)) 4.4.1"
	.section	.note.GNU-stack,"",%progbits

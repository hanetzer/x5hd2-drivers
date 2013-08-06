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
	.file	"aoe_main.c"
@ GNU C (Hisilicon_v200(gcc4.4-290+glibc-2.11+eabi+nptl)) version 4.4.1 (arm-hisiv200-linux-gnueabi)
@	compiled by GNU C version 4.1.1 20061011 (Red Hat 4.1.1-30), GMP version 4.3.1, MPFR version 2.4.2.
@ GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
@ options passed:  -nostdinc
@ -I/home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/arch/arm/include
@ -Iarch/arm/include/generated -Iinclude
@ -I/home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/include
@ -I/home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/drivers/msp/adsp/adsp_v1_1
@ -Idrivers/msp/adsp/adsp_v1_1
@ -I/home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/arch/arm/mach-s40/include
@ -I/home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/../../../source/common/include
@ -I/home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/../../../source/common/api/include
@ -I/home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/../../../source/common/drv/include
@ -I/home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/../../../source/msp/include
@ -I/home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/../../../source/msp/api/include
@ -I/home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/../../../source/msp/drv/include
@ -I/home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/../../../source/msp/drv/include/adsp/
@ -I/home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/../../../source/msp/drv/adsp/adsp_v1_1/hifi/common/include
@ -I/home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/../../../source/msp/drv/adsp/adsp_v1_1/hifi/aoe/include
@ -I/home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/../../../source/msp/drv/adsp/adsp_v1_1/hifi/aoe/src/include
@ -imultilib armv7a_soft -iprefix
@ /opt/hisi-linux/x86-arm/arm-hisiv200-linux/bin/../lib/gcc/arm-hisiv200-linux-gnueabi/4.4.1/
@ -isysroot /opt/hisi-linux/x86-arm/arm-hisiv200-linux/bin/../target
@ -D__KERNEL__ -D__LINUX_ARM_ARCH__=7 -Uarm -DHI_SND_DRV_SUSPEND_SUPPORT
@ -DCHIP_TYPE_hi3716cv200es -DSDK_VERSION=HiSTBLinuxV100R002C00SPC011
@ -DCFG_HI_LOG_LEVEL=4 -DHI_KEYLED_SUPPORT -DHI_SCI_SUPPORT
@ -DHI_VENC_SUPPORT -DHI_AENC_SUPPORT -DHI_SND_AOE_SWSIMULATE_SUPPORT
@ -DKBUILD_STR(s)=#s -DKBUILD_BASENAME=KBUILD_STR(aoe_main)
@ -DKBUILD_MODNAME=KBUILD_STR(hi_adsp) -isystem
@ /opt/hisi-linux/x86-arm/arm-hisiv200-linux/bin/../lib/gcc/arm-hisiv200-linux-gnueabi/4.4.1/include
@ -include
@ /home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/include/linux/kconfig.h
@ -MD drivers/msp/adsp/adsp_v1_1/./hifi/aoe/src/.aoe_main.o.d
@ /home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/drivers/msp/adsp/adsp_v1_1/./hifi/aoe/src/aoe_main.c
@ -mlittle-endian -marm -mapcs -mno-sched-prolog -mabi=aapcs-linux
@ -mno-thumb-interwork -march=armv7-a -msoft-float -auxbase-strip
@ drivers/msp/adsp/adsp_v1_1/./hifi/aoe/src/aoe_main.o -O2 -Wall -Wundef
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
	.global	AOE_FlushWKBuf
	.type	AOE_FlushWKBuf, %function
AOE_FlushWKBuf:
	.fnstart
.LFB770:
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
	movw	r3, #:lower16:.LANCHOR0	@ tmp133,
	movt	r3, #:upper16:.LANCHOR0	@ tmp133,
	mov	r2, #0	@ tmp134,
	str	r2, [r3, #0]	@ tmp134, g_stWkbuf.uWkBufIdx
	ldmfd	sp, {fp, sp, pc}	@
	.fnend
	.size	AOE_FlushWKBuf, .-AOE_FlushWKBuf
	.align	2
	.global	AOE_GetWKBuf
	.type	AOE_GetWKBuf, %function
AOE_GetWKBuf:
	.fnstart
.LFB771:
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
	movw	r3, #:lower16:.LANCHOR0	@ tmp149,
	movt	r3, #:upper16:.LANCHOR0	@ tmp149,
	ldr	r2, [r3, #0]	@ D.14662, g_stWkbuf.uWkBufIdx
	ldr	r1, [r3, #8]	@ g_stWkbuf.uWkBufBlockNumber, g_stWkbuf.uWkBufBlockNumber
	cmp	r2, r1	@ D.14662, g_stWkbuf.uWkBufBlockNumber
	ldr	r1, [r3, #12]	@ g_stWkbuf.uWkBufBase, g_stWkbuf.uWkBufBase
	addlt	ip, r2, #1	@ prephitmp.270, D.14662,
	ldrlt	r0, [r3, #4]	@ g_stWkbuf.uWkBufBlockSize, g_stWkbuf.uWkBufBlockSize
	movge	ip, #1	@ prephitmp.270,
	movge	r0, #0	@ prephitmp.268,
	str	ip, [r3, #0]	@ prephitmp.270, g_stWkbuf.uWkBufIdx
	mullt	r0, r0, r2	@ prephitmp.268, g_stWkbuf.uWkBufBlockSize, D.14662
	add	r0, r0, r1	@, prephitmp.268, g_stWkbuf.uWkBufBase
	ldmfd	sp, {fp, sp, pc}	@
	.fnend
	.size	AOE_GetWKBuf, .-AOE_GetWKBuf
	.align	2
	.global	AOE_ReleaseWKBuf
	.type	AOE_ReleaseWKBuf, %function
AOE_ReleaseWKBuf:
	.fnstart
.LFB772:
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
	movw	r3, #:lower16:.LANCHOR0	@ tmp134,
	movt	r3, #:upper16:.LANCHOR0	@ tmp134,
	ldr	r0, [r3, #0]	@ D.14674, g_stWkbuf.uWkBufIdx
	cmp	r0, #0	@ D.14674,
	ldreq	r0, [r3, #8]	@ g_stWkbuf.uWkBufBlockNumber, g_stWkbuf.uWkBufBlockNumber
	sub	r2, r0, #1	@ tmp140, g_stWkbuf.uWkBufBlockNumber,
	str	r2, [r3, #0]	@ tmp140, g_stWkbuf.uWkBufIdx
	ldmfd	sp, {fp, sp, pc}	@
	.fnend
	.size	AOE_ReleaseWKBuf, .-AOE_ReleaseWKBuf
	.align	2
	.global	AOE_GetWKBufSize
	.type	AOE_GetWKBufSize, %function
AOE_GetWKBufSize:
	.fnstart
.LFB773:
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
	movw	r3, #:lower16:.LANCHOR0	@ tmp135,
	movt	r3, #:upper16:.LANCHOR0	@ tmp135,
	ldr	r0, [r3, #4]	@, g_stWkbuf.uWkBufBlockSize
	ldmfd	sp, {fp, sp, pc}	@
	.fnend
	.size	AOE_GetWKBufSize, .-AOE_GetWKBufSize
	.align	2
	.global	AOE_ProcThread
	.type	AOE_ProcThread, %function
AOE_ProcThread:
	.fnstart
.LFB774:
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
	bl	AIP_Process	@
	bl	AOP_Process	@
	bl	ENGINE_Process	@
	ldmfd	sp, {fp, sp, pc}	@
	.fnend
	.size	AOE_ProcThread, .-AOE_ProcThread
	.align	2
	.global	AOE_DeInitWKBuf
	.type	AOE_DeInitWKBuf, %function
AOE_DeInitWKBuf:
	.fnstart
.LFB769:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	.movsp ip
	mov	ip, sp	@,
	stmfd	sp!, {r3, r4, fp, ip, lr, pc}	@,
	.pad #4
	.save {r3, r4, fp, ip, lr}
	.setfp fp, ip, #-4
	sub	fp, ip, #4	@,,
	push	{lr}
	bl	__gnu_mcount_nc
	movw	r4, #:lower16:.LANCHOR0	@ tmp133,
	movt	r4, #:upper16:.LANCHOR0	@ tmp133,
	mov	r0, #1	@,
	ldr	r1, [r4, #12]	@, g_stWkbuf.uWkBufBase
	bl	HI_V_FREE_Aligned	@
	mov	r0, r4	@, tmp133
	mov	r1, #16	@,
	bl	__memzero	@
	ldmfd	sp, {r3, r4, fp, sp, pc}	@
	.fnend
	.size	AOE_DeInitWKBuf, .-AOE_DeInitWKBuf
	.align	2
	.global	AOE_InitWKBuf
	.type	AOE_InitWKBuf, %function
AOE_InitWKBuf:
	.fnstart
.LFB768:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	.movsp ip
	mov	ip, sp	@,
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}	@,
	.pad #4
	.save {r4, r5, r6, r7, fp, ip, lr}
	.setfp fp, ip, #-4
	sub	fp, ip, #4	@,,
	.pad #8
	sub	sp, sp, #8	@,,
	push	{lr}
	bl	__gnu_mcount_nc
	movw	r4, #:lower16:.LANCHOR0	@ tmp137,
	movt	r4, #:upper16:.LANCHOR0	@ tmp137,
	mov	r5, r1	@ u32WKBufNum, u32WKBufNum
	mov	r6, r0	@ u32WKBufSize, u32WKBufSize
	mov	r1, #16	@,
	mov	r0, r4	@, tmp137
	mov	r7, r2	@ u32AlignToBytes, u32AlignToBytes
	bl	__memzero	@
	sub	r3, r5, #1	@ tmp138, u32WKBufNum,
	cmp	r3, #1	@ tmp138,
	bhi	.L24	@,
	mov	r2, r7	@, u32AlignToBytes
	mov	r0, #1	@,
	mul	r1, r6, r5	@, u32WKBufSize, u32WKBufNum
	bl	HI_V_MALLOC_Aligned	@
	cmp	r0, #0	@ ptr
	strne	r6, [r4, #4]	@ u32WKBufSize, g_stWkbuf.uWkBufBlockSize
	strne	r0, [r4, #12]	@ ptr, g_stWkbuf.uWkBufBase
	strne	r5, [r4, #8]	@ u32WKBufNum, g_stWkbuf.uWkBufBlockNumber
	beq	.L25	@,
.L22:
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L25:
	movw	r0, #:lower16:.LC1	@,
	movw	r1, #:lower16:.LC2	@,
	movw	r2, #:lower16:.LANCHOR1	@,
	movt	r1, #:upper16:.LC2	@,
	movt	r2, #:upper16:.LANCHOR1	@,
	mov	r3, #111	@,
	movt	r0, #:upper16:.LC1	@,
	movw	ip, #:lower16:.LC3	@ tmp147,
	movt	ip, #:upper16:.LC3	@ tmp147,
	str	ip, [sp, #0]	@ tmp147,
	bl	printk	@
	b	.L22	@
.L24:
	mov	r1, r5	@, u32WKBufNum
	movw	r0, #:lower16:.LC0	@,
	mov	r2, #2	@,
	movt	r0, #:upper16:.LC0	@,
	bl	printk	@
	b	.L22	@
	.fnend
	.size	AOE_InitWKBuf, .-AOE_InitWKBuf
	.align	2
	.global	AOE_DeInit
	.type	AOE_DeInit, %function
AOE_DeInit:
	.fnstart
.LFB767:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	.movsp ip
	mov	ip, sp	@,
	stmfd	sp!, {r3, r4, fp, ip, lr, pc}	@,
	.pad #4
	.save {r3, r4, fp, ip, lr}
	.setfp fp, ip, #-4
	sub	fp, ip, #4	@,,
	push	{lr}
	bl	__gnu_mcount_nc
	movw	r4, #:lower16:.LANCHOR0	@ tmp133,
	movt	r4, #:upper16:.LANCHOR0	@ tmp133,
	ldr	r3, [r4, #16]	@ g_stAoeInitFlag, g_stAoeInitFlag
	cmp	r3, #0	@ g_stAoeInitFlag,
	ldmeqfd	sp, {r3, r4, fp, sp, pc}	@
	bl	AIP_DeInit	@
	bl	AOP_DeInit	@
	bl	ENGINE_DeInit	@
	bl	AOE_DeInitWKBuf	@
	ldr	r0, [r4, #20]	@ u32RegMapAddr, u32RegMapAddr
	mov	r1, #0	@ tmp136,
	str	r1, [r4, #16]	@ tmp136, g_stAoeInitFlag
	cmp	r0, r1	@ u32RegMapAddr,
	ldmnefd	sp, {r3, r4, fp, sp, pc}	@
	bl	__arm_iounmap	@
	ldmfd	sp, {r3, r4, fp, sp, pc}	@
	.fnend
	.size	AOE_DeInit, .-AOE_DeInit
	.align	2
	.global	AOE_Init
	.type	AOE_Init, %function
AOE_Init:
	.fnstart
.LFB766:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	.movsp ip
	mov	ip, sp	@,
	stmfd	sp!, {r3, r4, fp, ip, lr, pc}	@,
	.pad #4
	.save {r3, r4, fp, ip, lr}
	.setfp fp, ip, #-4
	sub	fp, ip, #4	@,,
	push	{lr}
	bl	__gnu_mcount_nc
	movw	r4, #:lower16:.LANCHOR0	@ tmp135,
	movt	r4, #:upper16:.LANCHOR0	@ tmp135,
	ldr	r3, [r4, #16]	@ g_stAoeInitFlag, g_stAoeInitFlag
	cmp	r3, #0	@ g_stAoeInitFlag,
	ldmnefd	sp, {r3, r4, fp, sp, pc}	@
	movw	r3, #:lower16:-39714816	@ tmp137,
	movt	r3, #:upper16:-39714816	@ tmp137,
	mov	r1, #2048	@,
	ldr	lr, [r3, #1052]	@ u32RegMapAddr.223,
	mov	r0, lr	@, u32RegMapAddr.223
	str	lr, [r4, #20]	@ u32RegMapAddr.223, u32RegMapAddr
	bl	__memzero	@
	ldr	ip, [r4, #20]	@ u32RegMapAddr.224, u32RegMapAddr
	add	r0, ip, #256	@, u32RegMapAddr.224,
	str	ip, [r4, #24]	@ u32RegMapAddr.224, hwAOEComReg
	bl	AIP_Init	@
	ldr	r2, [r4, #20]	@ u32RegMapAddr, u32RegMapAddr
	add	r0, r2, #768	@, u32RegMapAddr,
	bl	AOP_Init	@
	ldr	r1, [r4, #20]	@ u32RegMapAddr, u32RegMapAddr
	add	r0, r1, #64	@, u32RegMapAddr,
	bl	ENGINE_Init	@
	mov	r0, #61440	@,
	mov	r1, #2	@,
	mov	r2, #32	@,
	bl	AOE_InitWKBuf	@
	mov	r0, #1	@ tmp149,
	str	r0, [r4, #16]	@ tmp149, g_stAoeInitFlag
	ldmfd	sp, {r3, r4, fp, sp, pc}	@
	.fnend
	.size	AOE_Init, .-AOE_Init
	.global	hwAOEComReg
	.section	.rodata
	.align	2
.LANCHOR1 = . + 0
	.type	__func__.14626, %object
	.size	__func__.14626, 14
__func__.14626:
	.ascii	"AOE_InitWKBuf\000"
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
.LC0:
	.ascii	"WKBufNum(%d) invalid,max(%d)\000"
	.space	3
.LC1:
	.ascii	"\012ASSERT failed at:\012  >File name: %s\012  >Fun"
	.ascii	"ction : %s\012  >Line No. : %d\012  >Condition: %s\012"
	.ascii	"\000"
.LC2:
	.ascii	"/home/release/HiSTBLinuxV100R002C00SPC011/source/ke"
	.ascii	"rnel/linux-3.4.y/drivers/msp/adsp/adsp_v1_1/./hifi/"
	.ascii	"aoe/src/aoe_main.c\000"
	.space	3
.LC3:
	.ascii	"HI_NULL!=ptr\000"
	.bss
	.align	2
.LANCHOR0 = . + 0
	.type	g_stWkbuf, %object
	.size	g_stWkbuf, 16
g_stWkbuf:
	.space	16
	.type	g_stAoeInitFlag, %object
	.size	g_stAoeInitFlag, 4
g_stAoeInitFlag:
	.space	4
	.type	u32RegMapAddr, %object
	.size	u32RegMapAddr, 4
u32RegMapAddr:
	.space	4
	.type	hwAOEComReg, %object
	.size	hwAOEComReg, 4
hwAOEComReg:
	.space	4
	.ident	"GCC: (Hisilicon_v200(gcc4.4-290+glibc-2.11+eabi+nptl)) 4.4.1"
	.section	.note.GNU-stack,"",%progbits

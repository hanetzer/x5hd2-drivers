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
	.file	"aoe_aop.c"
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
@ -DKBUILD_STR(s)=#s -DKBUILD_BASENAME=KBUILD_STR(aoe_aop)
@ -DKBUILD_MODNAME=KBUILD_STR(hi_adsp) -isystem
@ /opt/hisi-linux/x86-arm/arm-hisiv200-linux/bin/../lib/gcc/arm-hisiv200-linux-gnueabi/4.4.1/include
@ -include
@ /home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/include/linux/kconfig.h
@ -MD drivers/msp/adsp/adsp_v1_1/./hifi/aoe/src/aop/.aoe_aop.o.d
@ /home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/drivers/msp/adsp/adsp_v1_1/./hifi/aoe/src/aop/aoe_aop.c
@ -mlittle-endian -marm -mapcs -mno-sched-prolog -mabi=aapcs-linux
@ -mno-thumb-interwork -march=armv7-a -msoft-float -auxbase-strip
@ drivers/msp/adsp/adsp_v1_1/./hifi/aoe/src/aop/aoe_aop.o -O2 -Wall -Wundef
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
	.global	AOPAckChnCmd
	.type	AOPAckChnCmd, %function
AOPAckChnCmd:
	.fnstart
.LFB766:
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
	movw	r3, #:lower16:.LANCHOR0	@ tmp138,
	movt	r3, #:upper16:.LANCHOR0	@ tmp138,
	ldr	r3, [r3, r0, asl #2]	@ pAopReg, g_stAopRm.hwAopReg
	ldr	r2, [r3, #4]	@ D.14644,
	tst	r2, #8	@ D.14644,
	ldmnefd	sp, {fp, sp, pc}	@
	ldr	ip, [r3, #4]	@ tmp142,
	bfi	ip, r1, #4, #4	@ tmp142, Ack,,
	str	ip, [r3, #4]	@ tmp142,
	ldr	r1, [r3, #4]	@ tmp143,
	orr	r0, r1, #8	@ tmp145, tmp143,
	str	r0, [r3, #4]	@ tmp145,
	ldmfd	sp, {fp, sp, pc}	@
	.fnend
	.size	AOPAckChnCmd, .-AOPAckChnCmd
	.align	2
	.global	AOP_GetChnStatus
	.type	AOP_GetChnStatus, %function
AOP_GetChnStatus:
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
	movw	ip, #:lower16:.LANCHOR0	@ tmp136,
	add	r0, r0, #6	@ tmp137, Id,
	movt	ip, #:upper16:.LANCHOR0	@ tmp136,
	ldr	r3, [ip, r0, asl #2]	@ state, g_stAopRm.hAop
	cmp	r3, #0	@ state,
	streq	r3, [r1, #0]	@ state, <variable>.CurnStatus
	ldrne	r2, [r3, #64]	@ <variable>.CurnStatus, <variable>.CurnStatus
	strne	r2, [r1, #0]	@ <variable>.CurnStatus, <variable>.CurnStatus
	ldrne	r2, [r3, #20]	@ <variable>.stAttr.Priority, <variable>.stAttr.Priority
	strne	r2, [r1, #4]	@ <variable>.stAttr.Priority, <variable>.Priority
	ldrne	r2, [r3, #48]	@ <variable>.stAttr.stRbfAttr.uFrameSize, <variable>.stAttr.stRbfAttr.uFrameSize
	strne	r2, [r1, #12]	@ <variable>.stAttr.stRbfAttr.uFrameSize, <variable>.uFrameSize
	ldrne	r3, [r3, #0]	@ <variable>.stAttr.u32BitPerSample, <variable>.stAttr.u32BitPerSample
	strne	r3, [r1, #8]	@ <variable>.stAttr.u32BitPerSample, <variable>.u32BitPerSample
	ldmfd	sp, {fp, sp, pc}	@
	.fnend
	.size	AOP_GetChnStatus, .-AOP_GetChnStatus
	.align	2
	.global	AOP_GetChnAttr
	.type	AOP_GetChnAttr, %function
AOP_GetChnAttr:
	.fnstart
.LFB775:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	.movsp ip
	mov	ip, sp	@,
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}	@,
	.pad #4
	.save {r4, r5, fp, ip, lr}
	.setfp fp, ip, #-4
	sub	fp, ip, #4	@,,
	push	{lr}
	bl	__gnu_mcount_nc
	mov	r4, r1	@ pstAttr, pstAttr
	mov	r5, r0	@ Id, Id
	mov	r1, #60	@,
	mov	r0, r4	@, pstAttr
	add	r5, r5, #6	@ tmp137, Id,
	bl	__memzero	@
	movw	r3, #:lower16:.LANCHOR0	@ tmp136,
	movt	r3, #:upper16:.LANCHOR0	@ tmp136,
	ldr	ip, [r3, r5, asl #2]	@ state, g_stAopRm.hAop
	cmp	ip, #0	@ state,
	ldmeqfd	sp, {r4, r5, fp, sp, pc}	@
	ldmia	ip!, {r0, r1, r2, r3}	@ state,,,,
	mov	r5, r4	@ pstAttr, pstAttr
	mov	r4, ip	@ state, state
	stmia	r5!, {r0, r1, r2, r3}	@ pstAttr,,,,
	mov	ip, r5	@ pstAttr, pstAttr
	ldmia	r4!, {r0, r1, r2, r3}	@ state,,,,
	stmia	ip!, {r0, r1, r2, r3}	@ pstAttr,,,,
	ldmia	r4!, {r0, r1, r2, r3}	@ state,,,,
	stmia	ip!, {r0, r1, r2, r3}	@ pstAttr,,,,
	mov	lr, ip	@ pstAttr, pstAttr
	ldmia	r4, {r0, r1, r2}	@ state,,,
	stmia	ip, {r0, r1, r2}	@ pstAttr,,,
	ldmfd	sp, {r4, r5, fp, sp, pc}	@
	.fnend
	.size	AOP_GetChnAttr, .-AOP_GetChnAttr
	.align	2
	.global	AOPSwResetReg
	.type	AOPSwResetReg, %function
AOPSwResetReg:
	.fnstart
.LFB770:
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
	mov	r1, #32	@,
	mov	r4, r0	@ pAopReg, pAopReg
	bl	__memzero	@
	ldr	r0, [r4, #4]	@ tmp134,
	orr	r3, r0, #8	@ tmp136, tmp134,
	str	r3, [r4, #4]	@ tmp136,
	ldmfd	sp, {r3, r4, fp, sp, pc}	@
	.fnend
	.size	AOPSwResetReg, .-AOPSwResetReg
	.align	2
	.global	AOP_Init
	.type	AOP_Init, %function
AOP_Init:
	.fnstart
.LFB771:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	.movsp ip
	mov	ip, sp	@,
	stmfd	sp!, {r3, r4, r5, r6, fp, ip, lr, pc}	@,
	.pad #4
	.save {r3, r4, r5, r6, fp, ip, lr}
	.setfp fp, ip, #-4
	sub	fp, ip, #4	@,,
	push	{lr}
	bl	__gnu_mcount_nc
	movw	r4, #:lower16:.LANCHOR0	@ tmp140,
	movt	r4, #:upper16:.LANCHOR0	@ tmp140,
	mov	r6, r0	@ u32AopRegBase, u32AopRegBase
	mov	r5, #0	@ tmp142,
	str	r0, [r4, #0]	@ u32AopRegBase, g_stAopRm.hwAopReg
	bl	AOPSwResetReg	@
	add	r0, r6, #32	@ temp.358, u32AopRegBase,
	str	r5, [r4, #24]	@ tmp142, g_stAopRm.hAop
	str	r0, [r4, #4]	@ temp.358, g_stAopRm.hwAopReg
	bl	AOPSwResetReg	@
	add	r2, r6, #64	@ temp.360, u32AopRegBase,
	str	r5, [r4, #28]	@ tmp142, g_stAopRm.hAop
	str	r2, [r4, #8]	@ temp.360, g_stAopRm.hwAopReg
	mov	r0, r2	@, temp.360
	bl	AOPSwResetReg	@
	add	r1, r6, #96	@ temp.362, u32AopRegBase,
	str	r5, [r4, #32]	@ tmp142, g_stAopRm.hAop
	mov	r0, r1	@, temp.362
	str	r1, [r4, #12]	@ temp.362, g_stAopRm.hwAopReg
	bl	AOPSwResetReg	@
	add	r3, r6, #128	@ temp.364, u32AopRegBase,
	add	r6, r6, #160	@ D.14718, u32AopRegBase,
	str	r3, [r4, #16]	@ temp.364, g_stAopRm.hwAopReg
	mov	r0, r3	@, temp.364
	str	r5, [r4, #36]	@ tmp142, g_stAopRm.hAop
	bl	AOPSwResetReg	@
	mov	r0, r6	@, D.14718
	str	r5, [r4, #40]	@ tmp142, g_stAopRm.hAop
	str	r6, [r4, #20]	@ D.14718, g_stAopRm.hwAopReg
	bl	AOPSwResetReg	@
	str	r5, [r4, #44]	@ tmp142, g_stAopRm.hAop
	ldmfd	sp, {r3, r4, r5, r6, fp, sp, pc}	@
	.fnend
	.size	AOP_Init, .-AOP_Init
	.align	2
	.global	AOP_QueryRbfInfo
	.type	AOP_QueryRbfInfo, %function
AOP_QueryRbfInfo:
	.fnstart
.LFB777:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	.movsp ip
	mov	ip, sp	@,
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}	@,
	.pad #4
	.save {r4, r5, fp, ip, lr}
	.setfp fp, ip, #-4
	sub	fp, ip, #4	@,,
	push	{lr}
	bl	__gnu_mcount_nc
	mov	r4, r1	@ pstBufInfo, pstBufInfo
	mov	r5, r0	@ Id, Id
	mov	r1, #36	@,
	mov	r0, r4	@, pstBufInfo
	add	r5, r5, #6	@ tmp137, Id,
	bl	__memzero	@
	movw	r3, #:lower16:.LANCHOR0	@ tmp136,
	movt	r3, #:upper16:.LANCHOR0	@ tmp136,
	ldr	r3, [r3, r5, asl #2]	@ state, g_stAopRm.hAop
	cmp	r3, #0	@ state,
	ldmeqfd	sp, {r4, r5, fp, sp, pc}	@
	ldr	r2, [r3, #64]	@ <variable>.CurnStatus, <variable>.CurnStatus
	cmp	r2, #0	@ <variable>.CurnStatus,
	ldmeqfd	sp, {r4, r5, fp, sp, pc}	@
	ldr	r0, [r3, #60]	@, <variable>.hRingOut
	mov	r1, r4	@, pstBufInfo
	bl	RBUF_QueryInfo	@
	ldmfd	sp, {r4, r5, fp, sp, pc}	@
	.fnend
	.size	AOP_QueryRbfInfo, .-AOP_QueryRbfInfo
	.align	2
	.global	AOP_WriteRbfData
	.type	AOP_WriteRbfData, %function
AOP_WriteRbfData:
	.fnstart
.LFB776:
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
	movw	r3, #:lower16:.LANCHOR0	@ tmp139,
	add	r0, r0, #6	@ tmp140, Id,
	movt	r3, #:upper16:.LANCHOR0	@ tmp139,
	ldr	r3, [r3, r0, asl #2]	@ state, g_stAopRm.hAop
	cmp	r3, #0	@ state,
	beq	.L20	@,
	ldr	ip, [r3, #64]	@ <variable>.CurnStatus, <variable>.CurnStatus
	cmp	ip, #0	@ <variable>.CurnStatus,
	bne	.L23	@,
.L20:
	mov	r0, #0	@ uWriteSize,
	ldmfd	sp, {fp, sp, pc}	@
.L23:
	ldr	r0, [r3, #60]	@, <variable>.hRingOut
	bl	RBUF_WriteData	@
	ldmfd	sp, {fp, sp, pc}	@
	.fnend
	.size	AOP_WriteRbfData, .-AOP_WriteRbfData
	.align	2
	.global	AOP_DestroyChn
	.type	AOP_DestroyChn, %function
AOP_DestroyChn:
	.fnstart
.LFB768:
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
	subs	r4, r0, #0	@ inst, inst
	ldmeqfd	sp, {r3, r4, fp, sp, pc}	@
	ldr	r0, [r4, #60]	@, <variable>.hRingOut
	bl	RBUF_Destroy	@
	mov	r1, r4	@, inst
	mov	r0, #1	@,
	bl	HI_FREE	@
	ldmfd	sp, {r3, r4, fp, sp, pc}	@
	.fnend
	.size	AOP_DestroyChn, .-AOP_DestroyChn
	.align	2
	.global	AOP_DeInit
	.type	AOP_DeInit, %function
AOP_DeInit:
	.fnstart
.LFB772:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	.movsp ip
	mov	ip, sp	@,
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}	@,
	.pad #4
	.save {r4, r5, fp, ip, lr}
	.setfp fp, ip, #-4
	sub	fp, ip, #4	@,,
	push	{lr}
	bl	__gnu_mcount_nc
	movw	r4, #:lower16:.LANCHOR0	@ tmp133,
	movt	r4, #:upper16:.LANCHOR0	@ tmp133,
	mov	r5, #0	@ tmp136,
	ldr	r0, [r4, #24]	@, g_stAopRm.hAop
	bl	AOP_DestroyChn	@
	ldr	r0, [r4, #28]	@, g_stAopRm.hAop
	str	r5, [r4, #24]	@ tmp136, g_stAopRm.hAop
	bl	AOP_DestroyChn	@
	ldr	r0, [r4, #32]	@, g_stAopRm.hAop
	str	r5, [r4, #28]	@ tmp136, g_stAopRm.hAop
	bl	AOP_DestroyChn	@
	ldr	r0, [r4, #36]	@, g_stAopRm.hAop
	str	r5, [r4, #32]	@ tmp136, g_stAopRm.hAop
	bl	AOP_DestroyChn	@
	ldr	r0, [r4, #40]	@, g_stAopRm.hAop
	str	r5, [r4, #36]	@ tmp136, g_stAopRm.hAop
	bl	AOP_DestroyChn	@
	ldr	r0, [r4, #44]	@, g_stAopRm.hAop
	str	r5, [r4, #40]	@ tmp136, g_stAopRm.hAop
	bl	AOP_DestroyChn	@
	str	r5, [r4, #44]	@ tmp136, g_stAopRm.hAop
	ldmfd	sp, {r4, r5, fp, sp, pc}	@
	.fnend
	.size	AOP_DeInit, .-AOP_DeInit
	.align	2
	.global	AOPGetChnCmd
	.type	AOPGetChnCmd, %function
AOPGetChnCmd:
	.fnstart
.LFB765:
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
	movw	r3, #:lower16:.LANCHOR0	@ tmp141,
	movt	r3, #:upper16:.LANCHOR0	@ tmp141,
	mov	r4, r1	@ newcmd, newcmd
	ldr	r3, [r3, r0, asl #2]	@ pAopReg, g_stAopRm.hwAopReg
	ldr	r2, [r3, #4]	@ D.14629,
	tst	r2, #8	@ D.14629,
	bne	.L37	@,
	ldr	r0, [r3, #4]	@ tmp143,
	ands	r0, r0, #3	@ D.14633, tmp143,
	beq	.L31	@,
	cmp	r0, #1	@ D.14633,
	beq	.L31	@,
	ldr	r2, [r3, #4]	@ tmp145,
	mov	r1, #4	@ tmp146,
	movw	r0, #:lower16:.LC0	@,
	movt	r0, #:upper16:.LC0	@,
	bfi	r2, r1, #4, #4	@ tmp145, tmp146,,
	str	r2, [r3, #4]	@ tmp145,
	ldr	lr, [r3, #4]	@ tmp147,
	orr	ip, lr, #8	@ tmp153, tmp147,
	str	ip, [r3, #4]	@ tmp153,
	ldr	r1, [r3, #4]	@ tmp149,
	and	r1, r1, #3	@, tmp149,
	bl	printk	@
.L37:
	mov	r0, #2	@ eNewCmd,
.L31:
	str	r0, [r4, #0]	@ eNewCmd,* newcmd
	ldmfd	sp, {r3, r4, fp, sp, pc}	@
	.fnend
	.size	AOPGetChnCmd, .-AOPGetChnCmd
	.align	2
	.global	AOPGetChnCreateAttr
	.type	AOPGetChnCreateAttr, %function
AOPGetChnCreateAttr:
	.fnstart
.LFB764:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	.movsp ip
	mov	ip, sp	@,
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}	@,
	.pad #4
	.save {r4, r5, fp, ip, lr}
	.setfp fp, ip, #-4
	sub	fp, ip, #4	@,,
	.pad #8
	sub	sp, sp, #8	@,,
	push	{lr}
	bl	__gnu_mcount_nc
	movw	r3, #:lower16:.LANCHOR0	@ tmp159,
	movt	r3, #:upper16:.LANCHOR0	@ tmp159,
	mov	r4, r1	@ pstAttr, pstAttr
	ldr	r5, [r3, r0, asl #2]	@ pAopReg, g_stAopRm.hwAopReg
	mov	r0, r1	@, pstAttr
	ldr	r1, [r5, #0]	@ tmp160,* pAopReg
	and	r1, r1, #3	@, tmp160,
	bl	AOE_REG_ReadBitDepth	@
	cmp	r0, #0	@,
	beq	.L44	@,
.L39:
	mvn	r3, #0	@ D.14570,
.L42:
	mov	r0, r3	@, D.14570
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L44:
	ldr	r2, [r5, #0]	@ tmp162,* pAopReg
	add	r0, r4, #4	@, pstAttr,
	ubfx	r1, r2, #2, #2	@, tmp162,,
	bl	AOE_REG_ReadChannel	@
	cmp	r0, #0	@,
	bne	.L39	@,
	ldr	ip, [r5, #0]	@ tmp165,* pAopReg
	add	r0, r4, #8	@, pstAttr,
	ubfx	r1, ip, #4, #4	@, tmp165,,
	bl	AOE_REG_ReadSampelRate	@
	cmp	r0, #0	@,
	bne	.L39	@,
	ldr	lr, [r5, #0]	@ tmp168,* pAopReg
	add	r0, r4, #12	@, pstAttr,
	ubfx	r1, lr, #8, #2	@, tmp168,,
	bl	AOE_REG_ReadDataFormat	@
	cmp	r0, #0	@,
	bne	.L39	@,
	ldr	r1, [r5, #0]	@ tmp171,* pAopReg
	sub	r0, fp, #24	@,,
	ubfx	r1, r1, #20, #6	@, tmp171,,
	bl	AOE_REG_ReadBufLatencyMs	@
	cmp	r0, #0	@,
	bne	.L39	@,
	ldr	r3, [r5, #0]	@ tmp174,* pAopReg
	ldr	r1, [r4, #0]	@, <variable>.u32BitPerSample
	ubfx	lr, r3, #10, #1	@ D.14594, tmp174,,
	ldr	r0, [r4, #4]	@, <variable>.u32Channels
	str	lr, [r4, #20]	@ D.14594, <variable>.Priority
	bl	UTIL_CalcFrameSize	@
	mov	r1, #1	@ tmp176,
	str	r1, [r4, #24]	@ tmp176, <variable>.stRbfAttr.bExtMem
	ldr	ip, [r5, #20]	@ tmp177,
	ubfx	r2, ip, #24, #1	@ D.14599, tmp177,,
	str	r2, [r4, #44]	@ D.14599, <variable>.stRbfAttr.uBufFlag
	ldr	r3, [r5, #16]	@ D.14601, <variable>.AOP_BUF_ADDR
	str	r3, [r4, #28]	@ D.14601, <variable>.stRbfAttr.uBufAddr
	ldr	r1, [r5, #20]	@ tmp179,
	ubfx	ip, r1, #0, #24	@ D.14602, tmp179,,
	str	ip, [r4, #32]	@ D.14602, <variable>.stRbfAttr.uBufSize
	ldr	r2, [r5, #20]	@ D.14604,
	tst	r2, #16777216	@ D.14604,
	addeq	r2, r5, #28	@ D.14613, pAopReg,
	addeq	r5, r5, #24	@ D.14611, pAopReg,
	ldrne	r2, [r5, #24]	@ D.14608, <variable>.AOP_BUF_WPTR
	streq	r2, [r4, #40]	@ D.14613, <variable>.stRbfAttr.uBufRptr
	streq	r5, [r4, #36]	@ D.14611, <variable>.stRbfAttr.uBufWptr
	strne	r2, [r4, #36]	@ D.14608, <variable>.stRbfAttr.uBufWptr
	ldrne	r2, [r5, #28]	@ D.14609, <variable>.AOP_BUF_RPTR
	strne	r2, [r4, #40]	@ D.14609, <variable>.stRbfAttr.uBufRptr
	ldr	r3, [fp, #-24]	@ uLatencyMs, uLatencyMs
	ldr	r2, [r4, #8]	@, <variable>.u32SampleRate
	str	r3, [r4, #56]	@ uLatencyMs, <variable>.stRbfAttr.uLatencyMs
	mov	r1, r0	@, u32FrameSize
	str	r0, [r4, #48]	@ u32FrameSize, <variable>.stRbfAttr.uFrameSize
	mov	r0, r3	@, uLatencyMs
	bl	UTIL_LatencyMs2ByteSize	@
	mov	r3, #0	@ D.14570,
	str	r0, [r4, #52]	@, <variable>.stRbfAttr.uLatencySize
	b	.L42	@
	.fnend
	.size	AOPGetChnCreateAttr, .-AOPGetChnCreateAttr
	.align	2
	.global	AOPCreateChn
	.type	AOPCreateChn, %function
AOPCreateChn:
	.fnstart
.LFB767:
	@ args = 0, pretend = 0, frame = 64
	@ frame_needed = 1, uses_anonymous_args = 0
	.movsp ip
	mov	ip, sp	@,
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}	@,
	.pad #4
	.save {r4, r5, r6, r7, fp, ip, lr}
	.setfp fp, ip, #-4
	sub	fp, ip, #4	@,,
	.pad #64
	sub	sp, sp, #64	@,,
	push	{lr}
	bl	__gnu_mcount_nc
	sub	r5, fp, #92	@ tmp141,,
	mov	r4, #0	@ tmp140,
	str	r4, [fp, #-32]	@ tmp140, hRingOut
	mov	r1, r5	@, tmp141
	bl	AOPGetChnCreateAttr	@
	cmp	r0, r4	@,
	movne	ip, r4	@ D.14663, tmp140
	beq	.L52	@,
.L47:
	mov	r0, ip	@, D.14663
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L52:
	sub	r0, fp, #32	@,,
	add	r1, r5, #24	@, tmp141,
	bl	RBUF_Create	@
	ldr	r0, [fp, #-32]	@ hRingOut, hRingOut
	cmp	r0, #0	@ hRingOut,
	beq	.L50	@,
	mov	r0, #1	@,
	mov	r1, #68	@,
	bl	HI_MALLOC	@
	subs	r4, r0, #0	@ D.14666,
	ldreq	r0, [fp, #-32]	@ prephitmp.524, hRingOut
	beq	.L50	@,
	mov	r1, #68	@,
	mov	r6, r4	@ state, D.14666
	bl	__memzero	@
	ldmia	r5!, {r0, r1, r2, r3}	@ tmp147,,,,
	mov	ip, r4	@ D.14663, D.14666
	stmia	r6!, {r0, r1, r2, r3}	@ state,,,,
	mov	r7, r6	@ state, state
	ldmia	r5!, {r0, r1, r2, r3}	@ tmp151,,,,
	stmia	r7!, {r0, r1, r2, r3}	@ state,,,,
	ldmia	r5!, {r0, r1, r2, r3}	@ tmp153,,,,
	stmia	r7!, {r0, r1, r2, r3}	@ state,,,,
	ldmia	r5, {r0, r1, r2}	@ tmp153,,,
	stmia	r7, {r0, r1, r2}	@ state,,,
	ldr	r3, [fp, #-32]	@ hRingOut, hRingOut
	mov	r2, #1	@ tmp149,
	str	r2, [r4, #64]	@ tmp149, <variable>.CurnStatus
	str	r3, [r4, #60]	@ hRingOut, <variable>.hRingOut
	b	.L47	@
.L50:
	bl	RBUF_Destroy	@
	mov	ip, #0	@ D.14663,
	b	.L47	@
	.fnend
	.size	AOPCreateChn, .-AOPCreateChn
	.align	2
	.global	AOPProcChnlCmd
	.type	AOPProcChnlCmd, %function
AOPProcChnlCmd:
	.fnstart
.LFB769:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	.movsp ip
	mov	ip, sp	@,
	stmfd	sp!, {r4, r5, r6, fp, ip, lr, pc}	@,
	.pad #4
	.save {r4, r5, r6, fp, ip, lr}
	.setfp fp, ip, #-4
	sub	fp, ip, #4	@,,
	.pad #12
	sub	sp, sp, #12	@,,
	push	{lr}
	bl	__gnu_mcount_nc
	sub	r1, fp, #32	@,,
	mov	r4, r0	@ Id, Id
	bl	AOPGetChnCmd	@
	cmp	r0, #2	@,
	beq	.L60	@,
	ldr	r3, [fp, #-32]	@ newcmd, newcmd
	cmp	r3, #0	@ newcmd,
	beq	.L56	@,
	cmp	r3, #1	@ newcmd,
	beq	.L57	@,
	movw	r3, #:lower16:.LANCHOR0	@ tmp141,
	movt	r3, #:upper16:.LANCHOR0	@ tmp141,
	ldr	r3, [r3, r4, asl #2]	@ pAopReg, g_stAopRm.hwAopReg
.L58:
	ldr	ip, [r3, #4]	@ D.14829,
	ands	r2, ip, #8	@ tmp159, D.14829,
	bne	.L60	@,
	ldr	r0, [r3, #4]	@ tmp160,
	bfi	r0, r2, #4, #4	@ tmp160, tmp159,,
	str	r0, [r3, #4]	@ tmp160,
	ldr	r1, [r3, #4]	@ tmp161,
	orr	lr, r1, #8	@ tmp164, tmp161,
	str	lr, [r3, #4]	@ tmp164,
.L60:
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L56:
	mov	r0, r4	@, Id
	bl	AOPCreateChn	@
	movw	r2, #:lower16:.LANCHOR0	@ tmp142,
	add	r1, r4, #6	@ tmp143, Id,
	movt	r2, #:upper16:.LANCHOR0	@ tmp142,
	cmp	r0, #0	@ D.14693,
	str	r0, [r2, r1, asl #2]	@ D.14693, g_stAopRm.hAop
	bne	.L61	@,
	ldr	r3, [r2, r4, asl #2]	@ pAopReg, g_stAopRm.hwAopReg
	ldr	r0, [r3, #4]	@ D.14822,
	tst	r0, #8	@ D.14822,
	bne	.L58	@,
	ldr	r1, [r3, #4]	@ tmp147,
	mov	r0, #1	@ tmp148,
	bfi	r1, r0, #4, #4	@ tmp147, tmp148,,
	str	r1, [r3, #4]	@ tmp147,
	ldr	lr, [r3, #4]	@ tmp149,
	orr	ip, lr, #8	@ tmp163, tmp149,
	str	ip, [r3, #4]	@ tmp163,
.L61:
	ldr	r3, [r2, r4, asl #2]	@ pAopReg, g_stAopRm.hwAopReg
	b	.L58	@
.L57:
	add	r6, r4, #6	@ tmp153, Id,
	movw	r5, #:lower16:.LANCHOR0	@ tmp152,
	movt	r5, #:upper16:.LANCHOR0	@ tmp152,
	ldr	r0, [r5, r6, asl #2]	@, g_stAopRm.hAop
	bl	AOP_DestroyChn	@
	mov	r2, #0	@ tmp157,
	ldr	r3, [r5, r4, asl #2]	@ pAopReg, g_stAopRm.hwAopReg
	str	r2, [r5, r6, asl #2]	@ tmp157, g_stAopRm.hAop
	b	.L58	@
	.fnend
	.size	AOPProcChnlCmd, .-AOPProcChnlCmd
	.align	2
	.global	AOP_Process
	.type	AOP_Process, %function
AOP_Process:
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
	mov	r0, #0	@,
	bl	AOPProcChnlCmd	@
	mov	r0, #1	@,
	bl	AOPProcChnlCmd	@
	mov	r0, #2	@,
	bl	AOPProcChnlCmd	@
	mov	r0, #3	@,
	bl	AOPProcChnlCmd	@
	mov	r0, #4	@,
	bl	AOPProcChnlCmd	@
	mov	r0, #5	@,
	bl	AOPProcChnlCmd	@
	ldmfd	sp, {fp, sp, pc}	@
	.fnend
	.size	AOP_Process, .-AOP_Process
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
.LC0:
	.ascii	"invalid cmd(%d)\012\000"
	.bss
	.align	2
.LANCHOR0 = . + 0
	.type	g_stAopRm, %object
	.size	g_stAopRm, 48
g_stAopRm:
	.space	48
	.ident	"GCC: (Hisilicon_v200(gcc4.4-290+glibc-2.11+eabi+nptl)) 4.4.1"
	.section	.note.GNU-stack,"",%progbits

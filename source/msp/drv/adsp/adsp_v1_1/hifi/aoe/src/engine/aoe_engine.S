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
	.file	"aoe_engine.c"
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
@ -DKBUILD_STR(s)=#s -DKBUILD_BASENAME=KBUILD_STR(aoe_engine)
@ -DKBUILD_MODNAME=KBUILD_STR(hi_adsp) -isystem
@ /opt/hisi-linux/x86-arm/arm-hisiv200-linux/bin/../lib/gcc/arm-hisiv200-linux-gnueabi/4.4.1/include
@ -include
@ /home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/include/linux/kconfig.h
@ -MD drivers/msp/adsp/adsp_v1_1/./hifi/aoe/src/engine/.aoe_engine.o.d
@ /home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/drivers/msp/adsp/adsp_v1_1/./hifi/aoe/src/engine/aoe_engine.c
@ -mlittle-endian -marm -mapcs -mno-sched-prolog -mabi=aapcs-linux
@ -mno-thumb-interwork -march=armv7-a -msoft-float -auxbase-strip
@ drivers/msp/adsp/adsp_v1_1/./hifi/aoe/src/engine/aoe_engine.o -O2 -Wall
@ -Wundef -Wstrict-prototypes -Wno-trigraphs
@ -Werror-implicit-function-declaration -Wno-format-security
@ -Wframe-larger-than=1024 -Wdeclaration-after-statement -Wno-pointer-sign
@ -p -fno-strict-aliasing -fno-common -fno-delete-null-pointer-checks
@ -fno-dwarf2-cfi-asm -funwind-tables -fno-stack-protector
@ -fno-omit-frame-pointer -fno-optimize-sibling-calls -fno-strict-overflow
@ -fconserve-stack -fverbose-asm
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
	.global	ENGINEAckCmd
	.type	ENGINEAckCmd, %function
ENGINEAckCmd:
	.fnstart
.LFB765:
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
	ldr	r3, [r3, r0, asl #2]	@ pEneReg, g_stEneRm.hwEneReg
	ldr	r2, [r3, #4]	@ D.14641,
	tst	r2, #8	@ D.14641,
	ldmnefd	sp, {fp, sp, pc}	@
	ldr	ip, [r3, #4]	@ tmp142,
	bfi	ip, r1, #4, #4	@ tmp142, Ack,,
	str	ip, [r3, #4]	@ tmp142,
	ldr	r1, [r3, #4]	@ tmp143,
	orr	r0, r1, #8	@ tmp145, tmp143,
	str	r0, [r3, #4]	@ tmp145,
	ldmfd	sp, {fp, sp, pc}	@
	.fnend
	.size	ENGINEAckCmd, .-ENGINEAckCmd
	.align	2
	.global	ENGINEProcChnStopCmd
	.type	ENGINEProcChnStopCmd, %function
ENGINEProcChnStopCmd:
	.fnstart
.LFB775:
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
	add	r0, r0, #6	@ tmp136, Id,
	movt	r3, #:upper16:.LANCHOR0	@ tmp135,
	ldr	r3, [r3, r0, asl #2]	@ state, g_stEneRm.hEne
	cmp	r3, #0	@ state,
	ldmeqfd	sp, {fp, sp, pc}	@
	ldr	r2, [r3, #56]	@ <variable>.CurStatus, <variable>.CurStatus
	cmp	r2, #1	@ <variable>.CurStatus,
	moveq	r2, #0	@ tmp138,
	streq	r2, [r3, #56]	@ tmp138, <variable>.CurStatus
	ldmfd	sp, {fp, sp, pc}	@
	.fnend
	.size	ENGINEProcChnStopCmd, .-ENGINEProcChnStopCmd
	.align	2
	.global	ENGINEUpdateChnAipState
	.type	ENGINEUpdateChnAipState, %function
ENGINEUpdateChnAipState:
	.fnstart
.LFB778:
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
	str	r1, [r0, #16]	@ AipFlag, <variable>.AipMask
	ldmfd	sp, {fp, sp, pc}	@
	.fnend
	.size	ENGINEUpdateChnAipState, .-ENGINEUpdateChnAipState
	.align	2
	.global	ENGINEGetCunrStatus
	.type	ENGINEGetCunrStatus, %function
ENGINEGetCunrStatus:
	.fnstart
.LFB780:
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
	movw	r1, #:lower16:.LANCHOR0	@ tmp137,
	add	r0, r0, #6	@ tmp138, Id,
	movt	r1, #:upper16:.LANCHOR0	@ tmp137,
	ldr	r3, [r1, r0, asl #2]	@ state, g_stEneRm.hEne
	cmp	r3, #0	@ state,
	moveq	r0, #2	@ D.15025,
	ldrne	r0, [r3, #56]	@ D.15025, <variable>.CurStatus
	ldmfd	sp, {fp, sp, pc}	@
	.fnend
	.size	ENGINEGetCunrStatus, .-ENGINEGetCunrStatus
	.align	2
	.global	ENGINESwResetReg
	.type	ENGINESwResetReg, %function
ENGINESwResetReg:
	.fnstart
.LFB782:
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
	mov	r4, r0	@ pEneReg, pEneReg
	bl	__memzero	@
	ldr	r0, [r4, #4]	@ tmp134,
	orr	r3, r0, #8	@ tmp136, tmp134,
	str	r3, [r4, #4]	@ tmp136,
	ldmfd	sp, {r3, r4, fp, sp, pc}	@
	.fnend
	.size	ENGINESwResetReg, .-ENGINESwResetReg
	.align	2
	.global	ENGINE_Init
	.type	ENGINE_Init, %function
ENGINE_Init:
	.fnstart
.LFB784:
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
	mov	r6, r0	@ u32EngineRegBase, u32EngineRegBase
	mov	r5, #0	@ tmp142,
	str	r0, [r4, #0]	@ u32EngineRegBase, g_stEneRm.hwEneReg
	bl	ENGINESwResetReg	@
	add	r0, r6, #32	@ temp.385, u32EngineRegBase,
	str	r5, [r4, #24]	@ tmp142, g_stEneRm.hEne
	str	r0, [r4, #4]	@ temp.385, g_stEneRm.hwEneReg
	bl	ENGINESwResetReg	@
	add	r2, r6, #64	@ temp.387, u32EngineRegBase,
	str	r5, [r4, #28]	@ tmp142, g_stEneRm.hEne
	str	r2, [r4, #8]	@ temp.387, g_stEneRm.hwEneReg
	mov	r0, r2	@, temp.387
	bl	ENGINESwResetReg	@
	add	r1, r6, #96	@ temp.389, u32EngineRegBase,
	str	r5, [r4, #32]	@ tmp142, g_stEneRm.hEne
	mov	r0, r1	@, temp.389
	str	r1, [r4, #12]	@ temp.389, g_stEneRm.hwEneReg
	bl	ENGINESwResetReg	@
	add	r3, r6, #128	@ temp.391, u32EngineRegBase,
	add	r6, r6, #160	@ D.15077, u32EngineRegBase,
	str	r3, [r4, #16]	@ temp.391, g_stEneRm.hwEneReg
	mov	r0, r3	@, temp.391
	str	r5, [r4, #36]	@ tmp142, g_stEneRm.hEne
	bl	ENGINESwResetReg	@
	mov	r0, r6	@, D.15077
	str	r5, [r4, #40]	@ tmp142, g_stEneRm.hEne
	str	r6, [r4, #20]	@ D.15077, g_stEneRm.hwEneReg
	bl	ENGINESwResetReg	@
	str	r5, [r4, #44]	@ tmp142, g_stEneRm.hEne
	ldmfd	sp, {r3, r4, r5, r6, fp, sp, pc}	@
	.fnend
	.size	ENGINE_Init, .-ENGINE_Init
	.align	2
	.global	ENGINEUpdateChnAopState
	.type	ENGINEUpdateChnAopState, %function
ENGINEUpdateChnAopState:
	.fnstart
.LFB779:
	@ args = 0, pretend = 0, frame = 72
	@ frame_needed = 1, uses_anonymous_args = 0
	.movsp ip
	mov	ip, sp	@,
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, ip, lr, pc}	@,
	.pad #4
	.save {r4, r5, r6, r7, r8, r9, sl, fp, ip, lr}
	.setfp fp, ip, #-4
	sub	fp, ip, #4	@,,
	.pad #76
	sub	sp, sp, #76	@,,
	push	{lr}
	bl	__gnu_mcount_nc
	ldr	r2, [r0, #0]	@, <variable>.stAttr.u32BitPerSample
	tst	r1, #1	@ AopFlag,
	mov	r5, r1	@ AopFlag, AopFlag
	mov	r4, r0	@ state, state
	str	r2, [fp, #-108]	@, %sfp
	ldr	r3, [r0, #52]	@, <variable>.SamplePerFrame
	str	r3, [fp, #-104]	@, %sfp
	bne	.L68	@,
.L19:
	mov	r6, #0	@ AopBufFreeFlag,
	mov	r7, r6	@ AopNeedUnifyMask, AopBufFreeFlag
	mov	sl, r6	@ AopNoneUnifyMask, AopBufFreeFlag
	mov	r9, r6	@ AopHiPriorityFlag, AopBufFreeFlag
	mov	r8, r6	@ AopLoPriorityFlag, AopBufFreeFlag
.L23:
	tst	r5, #2	@ AopFlag,
	bne	.L69	@,
.L26:
	tst	r5, #4	@ AopFlag,
	bne	.L70	@,
.L33:
	tst	r5, #8	@ AopFlag,
	bne	.L71	@,
.L40:
	tst	r5, #16	@ AopFlag,
	bne	.L72	@,
.L47:
	tst	r5, #32	@ AopFlag,
	bne	.L73	@,
.L54:
	str	r6, [r4, #40]	@ AopBufFreeFlag, <variable>.AopBufFreeFlag
	str	r5, [r4, #20]	@ AopFlag, <variable>.AopMask
	str	r9, [r4, #24]	@ AopHiPriorityFlag, <variable>.AopHiPriorityMask
	str	r8, [r4, #28]	@ AopLoPriorityFlag, <variable>.AopLoPriorityMask
	str	sl, [r4, #32]	@ AopNoneUnifyMask, <variable>.AopNoneUnifyMask
	str	r7, [r4, #36]	@ AopNeedUnifyMask, <variable>.AopNeedUnifyMask
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, sl, fp, sp, pc}
.L68:
	mov	r0, #0	@,
	sub	r1, fp, #60	@,,
	bl	AOP_GetChnStatus	@
	ldr	r6, [fp, #-60]	@ status.CurnStatus, status.CurnStatus
	cmp	r6, #1	@ status.CurnStatus,
	bne	.L19	@,
	ldr	r7, [fp, #-48]	@ status.uFrameSize, status.uFrameSize
	mov	r0, #0	@,
	ldr	sl, [fp, #-104]	@, %sfp
	sub	r1, fp, #96	@,,
	ldr	r9, [fp, #-56]	@ status.Priority, status.Priority
	ldr	r3, [fp, #-52]	@ status.u32BitPerSample, status.u32BitPerSample
	mul	ip, r7, sl	@ blocksize.428, status.uFrameSize,
	cmp	r9, #1	@ status.Priority,
	ldr	lr, [fp, #-108]	@, %sfp
	movne	r9, r0	@, AopHiPriorityFlag,
	moveq	r8, #0	@, AopLoPriorityFlag
	movne	r8, #1	@, AopLoPriorityFlag
	cmp	r3, lr	@ status.u32BitPerSample,
	str	ip, [fp, #-112]	@,
	movne	sl, #0	@, AopNoneUnifyMask
	moveq	sl, #1	@, AopNoneUnifyMask
	moveq	r7, #0	@, AopNeedUnifyMask
	movne	r7, #1	@, AopNeedUnifyMask
	bl	AOP_QueryRbfInfo	@
	ldr	r0, [fp, #-68]	@ stRbfOut.u32TotalFreeSize, stRbfOut.u32TotalFreeSize
	ldr	r3, [fp, #-112]	@,
	cmp	r3, r0	@ blocksize.428, stRbfOut.u32TotalFreeSize
	bcs	.L22	@,
	ldr	r0, [fp, #-76]	@ stRbfOut.u32TotalDataSize, stRbfOut.u32TotalDataSize
	ldr	r2, [fp, #-64]	@ stRbfOut.u32LatencySize, stRbfOut.u32LatencySize
	add	r1, r3, r0	@ tmp156, blocksize.428, stRbfOut.u32TotalDataSize
	cmp	r1, r2	@ tmp156, stRbfOut.u32LatencySize
	bhi	.L22	@,
	cmp	r3, #0	@ blocksize.428,
	bne	.L23	@,
.L22:
	mov	r6, #0	@ AopBufFreeFlag,
	b	.L23	@
.L73:
	mov	r0, #5	@,
	sub	r1, fp, #60	@,,
	bl	AOP_GetChnStatus	@
	ldr	ip, [fp, #-60]	@ status.CurnStatus, status.CurnStatus
	cmp	ip, #1	@ status.CurnStatus,
	bne	.L54	@,
	ldr	lr, [fp, #-48]	@ status.uFrameSize, status.uFrameSize
	ldr	r2, [fp, #-104]	@, %sfp
	ldr	r1, [fp, #-56]	@ status.Priority, status.Priority
	ldr	r0, [fp, #-108]	@, %sfp
	mul	r3, lr, r2	@ blocksize, status.uFrameSize,
	ldr	ip, [fp, #-52]	@ status.u32BitPerSample, status.u32BitPerSample
	cmp	r1, #1	@ status.Priority,
	sub	r1, fp, #96	@,,
	orreq	r9, r9, #32	@ AopHiPriorityFlag, AopHiPriorityFlag,
	orrne	r8, r8, #32	@ AopLoPriorityFlag, AopLoPriorityFlag,
	cmp	ip, r0	@ status.u32BitPerSample,
	mov	r0, #5	@,
	orreq	sl, sl, #32	@ AopNoneUnifyMask, AopNoneUnifyMask,
	orrne	r7, r7, #32	@ AopNeedUnifyMask, AopNeedUnifyMask,
	str	r3, [fp, #-112]	@,
	bl	AOP_QueryRbfInfo	@
	ldr	r1, [fp, #-68]	@ stRbfOut.u32TotalFreeSize, stRbfOut.u32TotalFreeSize
	ldr	r3, [fp, #-112]	@,
	cmp	r3, r1	@ blocksize, stRbfOut.u32TotalFreeSize
	bcs	.L54	@,
	ldr	r0, [fp, #-76]	@ stRbfOut.u32TotalDataSize, stRbfOut.u32TotalDataSize
	ldr	r2, [fp, #-64]	@ stRbfOut.u32LatencySize, stRbfOut.u32LatencySize
	add	lr, r3, r0	@ tmp211, blocksize, stRbfOut.u32TotalDataSize
	cmp	lr, r2	@ tmp211, stRbfOut.u32LatencySize
	bhi	.L54	@,
	cmp	r3, #0	@ blocksize,
	orrne	r6, r6, #32	@ AopBufFreeFlag, AopBufFreeFlag,
	b	.L54	@
.L72:
	mov	r0, #4	@,
	sub	r1, fp, #60	@,,
	bl	AOP_GetChnStatus	@
	ldr	r3, [fp, #-60]	@ status.CurnStatus, status.CurnStatus
	cmp	r3, #1	@ status.CurnStatus,
	bne	.L47	@,
	ldr	lr, [fp, #-56]	@ status.Priority, status.Priority
	cmp	lr, #1	@ status.Priority,
	orrne	r8, r8, #16	@ AopLoPriorityFlag, AopLoPriorityFlag,
	beq	.L74	@,
.L53:
	ldr	r1, [fp, #-52]	@ status.u32BitPerSample, status.u32BitPerSample
	ldr	r2, [fp, #-108]	@, %sfp
	cmp	r1, r2	@ status.u32BitPerSample,
	orrne	r7, r7, #16	@ AopNeedUnifyMask, AopNeedUnifyMask,
	beq	.L75	@,
.L52:
	ldr	r3, [fp, #-48]	@ status.uFrameSize, status.uFrameSize
	mov	r0, #4	@,
	ldr	lr, [fp, #-104]	@, %sfp
	sub	r1, fp, #96	@,,
	mul	ip, r3, lr	@ blocksize.468, status.uFrameSize,
	str	ip, [fp, #-112]	@,
	bl	AOP_QueryRbfInfo	@
	ldr	r0, [fp, #-68]	@ stRbfOut.u32TotalFreeSize, stRbfOut.u32TotalFreeSize
	ldr	r3, [fp, #-112]	@,
	cmp	r3, r0	@ blocksize.468, stRbfOut.u32TotalFreeSize
	bcs	.L47	@,
	ldr	r0, [fp, #-76]	@ stRbfOut.u32TotalDataSize, stRbfOut.u32TotalDataSize
	ldr	r2, [fp, #-64]	@ stRbfOut.u32LatencySize, stRbfOut.u32LatencySize
	add	r1, r3, r0	@ tmp200, blocksize.468, stRbfOut.u32TotalDataSize
	cmp	r1, r2	@ tmp200, stRbfOut.u32LatencySize
	bhi	.L47	@,
	cmp	r3, #0	@ blocksize.468,
	orrne	r6, r6, #16	@ AopBufFreeFlag, AopBufFreeFlag,
	b	.L47	@
.L71:
	mov	r0, #3	@,
	sub	r1, fp, #60	@,,
	bl	AOP_GetChnStatus	@
	ldr	ip, [fp, #-60]	@ status.CurnStatus, status.CurnStatus
	cmp	ip, #1	@ status.CurnStatus,
	bne	.L40	@,
	ldr	lr, [fp, #-56]	@ status.Priority, status.Priority
	cmp	lr, #1	@ status.Priority,
	orrne	r8, r8, #8	@ AopLoPriorityFlag, AopLoPriorityFlag,
	beq	.L76	@,
.L46:
	ldr	r3, [fp, #-52]	@ status.u32BitPerSample, status.u32BitPerSample
	ldr	r1, [fp, #-108]	@, %sfp
	cmp	r3, r1	@ status.u32BitPerSample,
	orrne	r7, r7, #8	@ AopNeedUnifyMask, AopNeedUnifyMask,
	beq	.L77	@,
.L45:
	ldr	lr, [fp, #-48]	@ status.uFrameSize, status.uFrameSize
	mov	r0, #3	@,
	ldr	r2, [fp, #-104]	@, %sfp
	sub	r1, fp, #96	@,,
	mul	ip, lr, r2	@ blocksize.458, status.uFrameSize,
	str	ip, [fp, #-112]	@,
	bl	AOP_QueryRbfInfo	@
	ldr	r0, [fp, #-68]	@ stRbfOut.u32TotalFreeSize, stRbfOut.u32TotalFreeSize
	ldr	r3, [fp, #-112]	@,
	cmp	r3, r0	@ blocksize.458, stRbfOut.u32TotalFreeSize
	bcs	.L40	@,
	ldr	ip, [fp, #-76]	@ stRbfOut.u32TotalDataSize, stRbfOut.u32TotalDataSize
	ldr	r0, [fp, #-64]	@ stRbfOut.u32LatencySize, stRbfOut.u32LatencySize
	add	r1, r3, ip	@ tmp189, blocksize.458, stRbfOut.u32TotalDataSize
	cmp	r1, r0	@ tmp189, stRbfOut.u32LatencySize
	bhi	.L40	@,
	cmp	r3, #0	@ blocksize.458,
	orrne	r6, r6, #8	@ AopBufFreeFlag, AopBufFreeFlag,
	b	.L40	@
.L70:
	mov	r0, #2	@,
	sub	r1, fp, #60	@,,
	bl	AOP_GetChnStatus	@
	ldr	ip, [fp, #-60]	@ status.CurnStatus, status.CurnStatus
	cmp	ip, #1	@ status.CurnStatus,
	bne	.L33	@,
	ldr	lr, [fp, #-56]	@ status.Priority, status.Priority
	cmp	lr, #1	@ status.Priority,
	orrne	r8, r8, #4	@ AopLoPriorityFlag, AopLoPriorityFlag,
	beq	.L78	@,
.L39:
	ldr	r3, [fp, #-52]	@ status.u32BitPerSample, status.u32BitPerSample
	ldr	r0, [fp, #-108]	@, %sfp
	cmp	r3, r0	@ status.u32BitPerSample,
	orrne	r7, r7, #4	@ AopNeedUnifyMask, AopNeedUnifyMask,
	beq	.L79	@,
.L38:
	ldr	r3, [fp, #-104]	@, %sfp
	mov	r0, #2	@,
	ldr	lr, [fp, #-48]	@ status.uFrameSize, status.uFrameSize
	sub	r1, fp, #96	@,,
	mul	ip, lr, r3	@ blocksize.448, status.uFrameSize,
	str	ip, [fp, #-112]	@,
	bl	AOP_QueryRbfInfo	@
	ldr	r2, [fp, #-68]	@ stRbfOut.u32TotalFreeSize, stRbfOut.u32TotalFreeSize
	ldr	r3, [fp, #-112]	@,
	cmp	r3, r2	@ blocksize.448, stRbfOut.u32TotalFreeSize
	bcs	.L33	@,
	ldr	r2, [fp, #-76]	@ stRbfOut.u32TotalDataSize, stRbfOut.u32TotalDataSize
	ldr	r0, [fp, #-64]	@ stRbfOut.u32LatencySize, stRbfOut.u32LatencySize
	add	r1, r3, r2	@ tmp178, blocksize.448, stRbfOut.u32TotalDataSize
	cmp	r1, r0	@ tmp178, stRbfOut.u32LatencySize
	bhi	.L33	@,
	cmp	r3, #0	@ blocksize.448,
	orrne	r6, r6, #4	@ AopBufFreeFlag, AopBufFreeFlag,
	b	.L33	@
.L69:
	mov	r0, #1	@,
	sub	r1, fp, #60	@,,
	bl	AOP_GetChnStatus	@
	ldr	ip, [fp, #-60]	@ status.CurnStatus, status.CurnStatus
	cmp	ip, #1	@ status.CurnStatus,
	bne	.L26	@,
	ldr	lr, [fp, #-56]	@ status.Priority, status.Priority
	cmp	lr, #1	@ status.Priority,
	orrne	r8, r8, #2	@ AopLoPriorityFlag, AopLoPriorityFlag,
	beq	.L80	@,
.L32:
	ldr	r3, [fp, #-52]	@ status.u32BitPerSample, status.u32BitPerSample
	ldr	r1, [fp, #-108]	@, %sfp
	cmp	r3, r1	@ status.u32BitPerSample,
	orrne	r7, r7, #2	@ AopNeedUnifyMask, AopNeedUnifyMask,
	beq	.L81	@,
.L31:
	ldr	r3, [fp, #-104]	@, %sfp
	mov	r0, #1	@,
	ldr	lr, [fp, #-48]	@ status.uFrameSize, status.uFrameSize
	sub	r1, fp, #96	@,,
	mul	ip, lr, r3	@ blocksize.438, status.uFrameSize,
	str	ip, [fp, #-112]	@,
	bl	AOP_QueryRbfInfo	@
	ldr	r2, [fp, #-68]	@ stRbfOut.u32TotalFreeSize, stRbfOut.u32TotalFreeSize
	ldr	r3, [fp, #-112]	@,
	cmp	r3, r2	@ blocksize.438, stRbfOut.u32TotalFreeSize
	bcs	.L26	@,
	ldr	r1, [fp, #-76]	@ stRbfOut.u32TotalDataSize, stRbfOut.u32TotalDataSize
	ldr	r2, [fp, #-64]	@ stRbfOut.u32LatencySize, stRbfOut.u32LatencySize
	add	r0, r3, r1	@ tmp167, blocksize.438, stRbfOut.u32TotalDataSize
	cmp	r0, r2	@ tmp167, stRbfOut.u32LatencySize
	bhi	.L26	@,
	cmp	r3, #0	@ blocksize.438,
	orrne	r6, r6, #2	@ AopBufFreeFlag, AopBufFreeFlag,
	b	.L26	@
.L79:
	orr	sl, sl, #4	@ AopNoneUnifyMask, AopNoneUnifyMask,
	b	.L38	@
.L78:
	orr	r9, r9, #4	@ AopHiPriorityFlag, AopHiPriorityFlag,
	b	.L39	@
.L81:
	orr	sl, sl, #2	@ AopNoneUnifyMask, AopNoneUnifyMask,
	b	.L31	@
.L80:
	orr	r9, r9, #2	@ AopHiPriorityFlag, AopHiPriorityFlag,
	b	.L32	@
.L75:
	orr	sl, sl, #16	@ AopNoneUnifyMask, AopNoneUnifyMask,
	b	.L52	@
.L74:
	orr	r9, r9, #16	@ AopHiPriorityFlag, AopHiPriorityFlag,
	b	.L53	@
.L77:
	orr	sl, sl, #8	@ AopNoneUnifyMask, AopNoneUnifyMask,
	b	.L45	@
.L76:
	orr	r9, r9, #8	@ AopHiPriorityFlag, AopHiPriorityFlag,
	b	.L46	@
	.fnend
	.size	ENGINEUpdateChnAopState, .-ENGINEUpdateChnAopState
	.global	__aeabi_uidiv
	.align	2
	.global	ENGINEProcChnData
	.type	ENGINEProcChnData, %function
ENGINEProcChnData:
	.fnstart
.LFB774:
	@ args = 0, pretend = 0, frame = 112
	@ frame_needed = 1, uses_anonymous_args = 0
	.movsp ip
	mov	ip, sp	@,
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, ip, lr, pc}	@,
	.pad #4
	.save {r4, r5, r6, r7, r8, r9, sl, fp, ip, lr}
	.setfp fp, ip, #-4
	sub	fp, ip, #4	@,,
	.pad #116
	sub	sp, sp, #116	@,,
	push	{lr}
	bl	__gnu_mcount_nc
	ldr	r2, [r0, #40]	@ BufFreeFlag, <variable>.AopBufFreeFlag
	mov	r4, r0	@ state, state
	ldr	r3, [r0, #24]	@ D.15135, <variable>.AopHiPriorityMask
	cmp	r2, #0	@ BufFreeFlag,
	ldr	r1, [r0, #28]	@ D.15134, <variable>.AopLoPriorityMask
	beq	.L161	@,
	cmp	r3, #0	@ D.15135,
	moveq	r0, r3	@ Flag, D.15135
	bne	.L182	@,
.L85:
	cmp	r1, #0	@ D.15134,
	beq	.L94	@,
	tst	r1, #1	@ D.15134,
	beq	.L95	@,
	tst	r2, #1	@ BufFreeFlag,
	beq	.L96	@,
	orr	r0, r0, #2	@ Flag, Flag,
.L95:
	tst	r1, #2	@ D.15134,
	beq	.L97	@,
	tst	r2, #2	@ BufFreeFlag,
	beq	.L96	@,
	orr	r0, r0, #2	@ Flag, Flag,
.L97:
	tst	r1, #4	@ D.15134,
	beq	.L98	@,
	tst	r2, #4	@ BufFreeFlag,
	beq	.L96	@,
	orr	r0, r0, #2	@ Flag, Flag,
.L98:
	tst	r1, #8	@ D.15134,
	beq	.L99	@,
	tst	r2, #8	@ BufFreeFlag,
	beq	.L96	@,
	orr	r0, r0, #2	@ Flag, Flag,
.L99:
	tst	r1, #16	@ D.15134,
	beq	.L100	@,
	tst	r2, #16	@ BufFreeFlag,
	beq	.L96	@,
	orr	r0, r0, #2	@ Flag, Flag,
.L100:
	tst	r1, #32	@ D.15134,
	beq	.L94	@,
	tst	r2, #32	@ BufFreeFlag,
	orrne	r0, r0, #2	@ Flag, Flag,
	beq	.L96	@,
.L94:
	cmp	r3, #0	@ D.15135,
	beq	.L101	@,
	tst	r0, #1	@ Flag,
	beq	.L161	@,
.L102:
	sub	r7, fp, #100	@,,
	str	r7, [fp, #-152]	@, %sfp
	bl	AOE_FlushWKBuf	@
	mov	r1, #32	@,
	sub	r0, fp, #100	@,,
	ldr	r9, [r4, #4]	@ Ch, <variable>.stAttr.u32Channels
	ldr	sl, [r4, #0]	@ BitDepth, <variable>.stAttr.u32BitPerSample
	bl	__memzero	@
	bl	AOE_GetWKBuf	@
	mov	r7, r0	@ pDataBuf,
	bl	AOE_GetWKBufSize	@
	subs	r1, r0, #0	@ __n,
	beq	.L160	@,
	mov	r0, r7	@, pDataBuf
	bl	__memzero	@
.L160:
	ldr	r3, [r4, #16]	@ <variable>.AipMask, <variable>.AipMask
	tst	r3, #1	@ <variable>.AipMask,
	bne	.L183	@,
.L103:
	mov	r5, #0	@ u32UnderFlag,
	mov	r6, r5	@ MixCnt, u32UnderFlag
.L105:
	tst	r3, #2	@ <variable>.AipMask,
	bne	.L184	@,
.L108:
	tst	r3, #4	@ <variable>.AipMask,
	bne	.L185	@,
.L112:
	tst	r3, #8	@ <variable>.AipMask,
	bne	.L186	@,
.L116:
	tst	r3, #16	@ <variable>.AipMask,
	bne	.L187	@,
.L120:
	tst	r3, #32	@ <variable>.AipMask,
	bne	.L188	@,
.L124:
	tst	r3, #64	@ <variable>.AipMask,
	bne	.L189	@,
.L128:
	tst	r3, #128	@ <variable>.AipMask,
	bne	.L190	@,
.L132:
	cmp	r6, #0	@ MixCnt,
	ldreq	r3, [fp, #-88]	@ prephitmp.505, stFrame.u32SamplesPerFrame
	beq	.L136	@,
	tst	r5, #1	@ u32UnderFlag,
	bne	.L191	@,
.L137:
	tst	r5, #2	@ u32UnderFlag,
	bne	.L192	@,
.L138:
	tst	r5, #4	@ u32UnderFlag,
	bne	.L193	@,
.L139:
	tst	r5, #8	@ u32UnderFlag,
	bne	.L194	@,
.L140:
	tst	r5, #16	@ u32UnderFlag,
	bne	.L195	@,
.L141:
	tst	r5, #32	@ u32UnderFlag,
	bne	.L196	@,
.L142:
	tst	r5, #64	@ u32UnderFlag,
	bne	.L197	@,
.L143:
	tst	r5, #128	@ u32UnderFlag,
	bne	.L198	@,
.L144:
	ldr	lr, [r4, #44]	@ <variable>.BlockSize, <variable>.BlockSize
	ldr	r3, [r4, #52]	@ prephitmp.505, <variable>.SamplePerFrame
	ldmia	r4, {r0, ip}	@ phole ldm
	ldr	r1, [r4, #8]	@ <variable>.stAttr.u32SampleRate, <variable>.stAttr.u32SampleRate
	ldr	r2, [r4, #12]	@ <variable>.stAttr.u32DataFormat, <variable>.stAttr.u32DataFormat
	str	r7, [fp, #-100]	@ pDataBuf, stFrame.pDataBuf
	str	lr, [fp, #-92]	@ <variable>.BlockSize, stFrame.u32DataBytes
	str	ip, [fp, #-80]	@ <variable>.stAttr.u32Channels, stFrame.u32Channels
	str	r0, [fp, #-84]	@ <variable>.stAttr.u32BitPerSample, stFrame.u32BitPerSample
	str	r1, [fp, #-76]	@ <variable>.stAttr.u32SampleRate, stFrame.u32SampleRate
	str	r2, [fp, #-72]	@ <variable>.stAttr.u32DataFormat, stFrame.u32DataFormat
	str	r3, [fp, #-88]	@ prephitmp.505, stFrame.u32SamplesPerFrame
.L136:
	cmp	r3, #0	@ prephitmp.505,
	bne	.L199	@,
.L145:
	bl	AOE_FlushWKBuf	@
.L161:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, sl, fp, sp, pc}
.L182:
	ands	r0, r3, #1	@ tmp203, D.15135,
	beq	.L87	@,
	ands	r0, r2, #1	@ tmp204, BufFreeFlag,
	beq	.L90	@,
	mov	r0, #1	@ Flag,
.L87:
	tst	r3, #2	@ D.15135,
	beq	.L89	@,
	tst	r2, #2	@ BufFreeFlag,
	beq	.L90	@,
	mov	r0, #1	@ Flag,
.L89:
	tst	r3, #4	@ D.15135,
	beq	.L91	@,
	tst	r2, #4	@ BufFreeFlag,
	beq	.L90	@,
	mov	r0, #1	@ Flag,
.L91:
	tst	r3, #8	@ D.15135,
	beq	.L92	@,
	tst	r2, #8	@ BufFreeFlag,
	beq	.L90	@,
	mov	r0, #1	@ Flag,
.L92:
	tst	r3, #16	@ D.15135,
	beq	.L93	@,
	tst	r2, #16	@ BufFreeFlag,
	beq	.L90	@,
	mov	r0, #1	@ Flag,
.L93:
	tst	r3, #32	@ D.15135,
	beq	.L85	@,
	tst	r2, #32	@ BufFreeFlag,
	movne	r0, #1	@ Flag,
	bne	.L85	@,
.L90:
	bic	r0, r0, #1	@ Flag, Flag,
	b	.L85	@
.L101:
	cmp	r1, #0	@ D.15134,
	beq	.L161	@,
	tst	r0, #2	@ Flag,
	bne	.L102	@,
	b	.L161	@
.L199:
	ldr	r5, [r4, #24]	@ prephitmp.507, <variable>.AopHiPriorityMask
	sub	r8, fp, #68	@ tmp516,,
	ldr	r3, [r4, #28]	@ <variable>.AopLoPriorityMask, <variable>.AopLoPriorityMask
	sub	r7, fp, #136	@ tmp517,,
	sub	sl, fp, #52	@ tmp522,,
	mov	r6, #0	@ ivtmp.547,
	str	r5, [fp, #-52]	@ prephitmp.507, PriorityMaskTab
	str	r3, [fp, #-48]	@ <variable>.AopLoPriorityMask, PriorityMaskTab
.L158:
	tst	r5, #1	@ prephitmp.507,
	beq	.L146	@,
	ldr	r2, [r4, #40]	@ <variable>.AopBufFreeFlag, <variable>.AopBufFreeFlag
	tst	r2, #1	@ <variable>.AopBufFreeFlag,
	beq	.L146	@,
	ldr	r9, [r4, #32]	@ <variable>.AopNoneUnifyMask, <variable>.AopNoneUnifyMask
	ands	r9, r9, #1	@ tmp430, <variable>.AopNoneUnifyMask,
	beq	.L200	@,
	mov	r0, #0	@,
	ldr	r1, [fp, #-100]	@, stFrame.pDataBuf
	ldr	r2, [fp, #-92]	@, stFrame.u32DataBytes
	bl	AOP_WriteRbfData	@
.L146:
	tst	r5, #2	@ prephitmp.507,
	beq	.L148	@,
	ldr	r1, [r4, #40]	@ <variable>.AopBufFreeFlag, <variable>.AopBufFreeFlag
	tst	r1, #2	@ <variable>.AopBufFreeFlag,
	beq	.L148	@,
	ldr	r0, [r4, #32]	@ <variable>.AopNoneUnifyMask, <variable>.AopNoneUnifyMask
	tst	r0, #2	@ <variable>.AopNoneUnifyMask,
	beq	.L201	@,
	mov	r0, #1	@,
	ldr	r1, [fp, #-100]	@, stFrame.pDataBuf
	ldr	r2, [fp, #-92]	@, stFrame.u32DataBytes
	bl	AOP_WriteRbfData	@
.L148:
	tst	r5, #4	@ prephitmp.507,
	beq	.L150	@,
	ldr	r3, [r4, #40]	@ <variable>.AopBufFreeFlag, <variable>.AopBufFreeFlag
	tst	r3, #4	@ <variable>.AopBufFreeFlag,
	beq	.L150	@,
	ldr	r2, [r4, #32]	@ <variable>.AopNoneUnifyMask, <variable>.AopNoneUnifyMask
	tst	r2, #4	@ <variable>.AopNoneUnifyMask,
	beq	.L202	@,
	mov	r0, #2	@,
	ldr	r1, [fp, #-100]	@, stFrame.pDataBuf
	ldr	r2, [fp, #-92]	@, stFrame.u32DataBytes
	bl	AOP_WriteRbfData	@
.L150:
	tst	r5, #8	@ prephitmp.507,
	beq	.L152	@,
	ldr	r1, [r4, #40]	@ <variable>.AopBufFreeFlag, <variable>.AopBufFreeFlag
	tst	r1, #8	@ <variable>.AopBufFreeFlag,
	beq	.L152	@,
	ldr	r0, [r4, #32]	@ <variable>.AopNoneUnifyMask, <variable>.AopNoneUnifyMask
	tst	r0, #8	@ <variable>.AopNoneUnifyMask,
	beq	.L203	@,
	mov	r0, #3	@,
	ldr	r1, [fp, #-100]	@, stFrame.pDataBuf
	ldr	r2, [fp, #-92]	@, stFrame.u32DataBytes
	bl	AOP_WriteRbfData	@
.L152:
	tst	r5, #16	@ prephitmp.507,
	beq	.L154	@,
	ldr	r3, [r4, #40]	@ <variable>.AopBufFreeFlag, <variable>.AopBufFreeFlag
	tst	r3, #16	@ <variable>.AopBufFreeFlag,
	beq	.L154	@,
	ldr	r2, [r4, #32]	@ <variable>.AopNoneUnifyMask, <variable>.AopNoneUnifyMask
	tst	r2, #16	@ <variable>.AopNoneUnifyMask,
	beq	.L204	@,
	mov	r0, #4	@,
	ldr	r1, [fp, #-100]	@, stFrame.pDataBuf
	ldr	r2, [fp, #-92]	@, stFrame.u32DataBytes
	bl	AOP_WriteRbfData	@
.L154:
	tst	r5, #32	@ prephitmp.507,
	beq	.L156	@,
	ldr	r1, [r4, #40]	@ <variable>.AopBufFreeFlag, <variable>.AopBufFreeFlag
	tst	r1, #32	@ <variable>.AopBufFreeFlag,
	beq	.L156	@,
	ldr	r0, [r4, #32]	@ <variable>.AopNoneUnifyMask, <variable>.AopNoneUnifyMask
	tst	r0, #32	@ <variable>.AopNoneUnifyMask,
	beq	.L157	@,
	ldr	r1, [fp, #-100]	@, stFrame.pDataBuf
	mov	r0, #5	@,
	ldr	r2, [fp, #-92]	@, stFrame.u32DataBytes
	bl	AOP_WriteRbfData	@
.L156:
	add	r6, r6, #4	@ ivtmp.547, ivtmp.547,
	cmp	r6, #8	@ ivtmp.547,
	beq	.L145	@,
	ldr	r5, [sl, r6]	@ prephitmp.507, PriorityMaskTab
	b	.L158	@
.L183:
	mov	r0, #0	@,
	sub	r1, fp, #136	@,,
	bl	AIP_QueryFiFoInfo	@
	ldr	r3, [r4, #44]	@ <variable>.BlockSize, <variable>.BlockSize
	ldr	r2, [fp, #-116]	@ stRbfIn.u32TotalDataSize, stRbfIn.u32TotalDataSize
	ldr	r6, [fp, #-120]	@ TailBytes.568, stRbfIn.u32ContiguousDataSize
	cmp	r2, r3	@ stRbfIn.u32TotalDataSize, <variable>.BlockSize
	bhi	.L104	@,
	mov	r0, #0	@,
	sub	r1, fp, #52	@,,
	bl	AIP_GetChnStatus	@
	ldr	r5, [fp, #-52]	@ stChnStatus.CurnStatus, stChnStatus.CurnStatus
	cmp	r5, #1	@ stChnStatus.CurnStatus,
	ldrne	r3, [r4, #16]	@ <variable>.AipMask, <variable>.AipMask
	bne	.L103	@,
	mov	r6, #0	@ MixCnt,
	ldr	r3, [r4, #16]	@ <variable>.AipMask, <variable>.AipMask
	b	.L105	@
.L190:
	mov	r0, #7	@,
	sub	r1, fp, #136	@,,
	bl	AIP_QueryFiFoInfo	@
	ldr	r3, [r4, #44]	@ <variable>.BlockSize, <variable>.BlockSize
	ldr	r0, [fp, #-116]	@ stRbfIn.u32TotalDataSize, stRbfIn.u32TotalDataSize
	ldr	ip, [fp, #-120]	@ TailBytes, stRbfIn.u32ContiguousDataSize
	cmp	r0, r3	@ stRbfIn.u32TotalDataSize, <variable>.BlockSize
	bls	.L133	@,
	ldr	r2, [fp, #-136]	@ stRbfIn.u32BasePtr, stRbfIn.u32BasePtr
	mov	r0, ip	@, TailBytes
	ldr	lr, [fp, #-132]	@ stRbfIn.u32ReadPtr, stRbfIn.u32ReadPtr
	ldr	r1, [r4, #48]	@, <variable>.FrameSize
	add	r8, r2, lr	@ tmp395, stRbfIn.u32BasePtr, stRbfIn.u32ReadPtr
	str	ip, [fp, #-144]	@,
	bl	__aeabi_uidiv	@
	mov	r3, sl	@, BitDepth
	mov	r1, r8	@, tmp395
	mul	r2, r9, r0	@ tmp402, Ch,
	mov	r0, r7	@, pDataBuf
	bl	UTIL_MixerPro	@
	ldr	r1, [r4, #44]	@ prephitmp.515, <variable>.BlockSize
	ldr	ip, [fp, #-144]	@,
	cmp	ip, r1	@ TailBytes, prephitmp.515
	bcc	.L205	@,
.L134:
	mov	r0, #7	@,
	add	r6, r6, #1	@ MixCnt, MixCnt,
	bl	AIP_ReadFiFoComplete	@
	b	.L132	@
.L185:
	mov	r0, #2	@,
	sub	r1, fp, #136	@,,
	bl	AIP_QueryFiFoInfo	@
	ldr	r2, [r4, #44]	@ <variable>.BlockSize, <variable>.BlockSize
	ldr	r0, [fp, #-116]	@ stRbfIn.u32TotalDataSize, stRbfIn.u32TotalDataSize
	ldr	ip, [fp, #-120]	@ TailBytes.610, stRbfIn.u32ContiguousDataSize
	cmp	r0, r2	@ stRbfIn.u32TotalDataSize, <variable>.BlockSize
	bhi	.L113	@,
	mov	r0, #2	@,
	sub	r1, fp, #52	@,,
	bl	AIP_GetChnStatus	@
	ldr	r3, [fp, #-52]	@ stChnStatus.CurnStatus, stChnStatus.CurnStatus
	cmp	r3, #1	@ stChnStatus.CurnStatus,
	ldr	r3, [r4, #16]	@ <variable>.AipMask, <variable>.AipMask
	orreq	r5, r5, #4	@ u32UnderFlag, u32UnderFlag,
	b	.L112	@
.L184:
	mov	r0, #1	@,
	sub	r1, fp, #136	@,,
	bl	AIP_QueryFiFoInfo	@
	ldr	r1, [r4, #44]	@ <variable>.BlockSize, <variable>.BlockSize
	ldr	r0, [fp, #-116]	@ stRbfIn.u32TotalDataSize, stRbfIn.u32TotalDataSize
	ldr	ip, [fp, #-120]	@ TailBytes.589, stRbfIn.u32ContiguousDataSize
	cmp	r0, r1	@ stRbfIn.u32TotalDataSize, <variable>.BlockSize
	bhi	.L109	@,
	mov	r0, #1	@,
	sub	r1, fp, #52	@,,
	bl	AIP_GetChnStatus	@
	ldr	r2, [fp, #-52]	@ stChnStatus.CurnStatus, stChnStatus.CurnStatus
	ldr	r3, [r4, #16]	@ <variable>.AipMask, <variable>.AipMask
	cmp	r2, #1	@ stChnStatus.CurnStatus,
	orreq	r5, r5, #2	@ u32UnderFlag, u32UnderFlag,
	b	.L108	@
.L189:
	mov	r0, #6	@,
	sub	r1, fp, #136	@,,
	bl	AIP_QueryFiFoInfo	@
	ldr	r3, [r4, #44]	@ <variable>.BlockSize, <variable>.BlockSize
	ldr	r0, [fp, #-116]	@ stRbfIn.u32TotalDataSize, stRbfIn.u32TotalDataSize
	ldr	ip, [fp, #-120]	@ TailBytes.694, stRbfIn.u32ContiguousDataSize
	cmp	r0, r3	@ stRbfIn.u32TotalDataSize, <variable>.BlockSize
	bhi	.L129	@,
	mov	r0, #6	@,
	sub	r1, fp, #52	@,,
	bl	AIP_GetChnStatus	@
	ldr	r2, [fp, #-52]	@ stChnStatus.CurnStatus, stChnStatus.CurnStatus
	ldr	r3, [r4, #16]	@ <variable>.AipMask, <variable>.AipMask
	cmp	r2, #1	@ stChnStatus.CurnStatus,
	orreq	r5, r5, #64	@ u32UnderFlag, u32UnderFlag,
	b	.L128	@
.L187:
	mov	r0, #4	@,
	sub	r1, fp, #136	@,,
	bl	AIP_QueryFiFoInfo	@
	ldr	r3, [r4, #44]	@ <variable>.BlockSize, <variable>.BlockSize
	ldr	r0, [fp, #-116]	@ stRbfIn.u32TotalDataSize, stRbfIn.u32TotalDataSize
	ldr	ip, [fp, #-120]	@ TailBytes.652, stRbfIn.u32ContiguousDataSize
	cmp	r0, r3	@ stRbfIn.u32TotalDataSize, <variable>.BlockSize
	bhi	.L121	@,
	mov	r0, #4	@,
	sub	r1, fp, #52	@,,
	bl	AIP_GetChnStatus	@
	ldr	r2, [fp, #-52]	@ stChnStatus.CurnStatus, stChnStatus.CurnStatus
	ldr	r3, [r4, #16]	@ <variable>.AipMask, <variable>.AipMask
	cmp	r2, #1	@ stChnStatus.CurnStatus,
	orreq	r5, r5, #16	@ u32UnderFlag, u32UnderFlag,
	b	.L120	@
.L188:
	mov	r0, #5	@,
	sub	r1, fp, #136	@,,
	bl	AIP_QueryFiFoInfo	@
	ldr	r3, [r4, #44]	@ <variable>.BlockSize, <variable>.BlockSize
	ldr	r0, [fp, #-116]	@ stRbfIn.u32TotalDataSize, stRbfIn.u32TotalDataSize
	ldr	ip, [fp, #-120]	@ TailBytes.673, stRbfIn.u32ContiguousDataSize
	cmp	r0, r3	@ stRbfIn.u32TotalDataSize, <variable>.BlockSize
	bhi	.L125	@,
	mov	r0, #5	@,
	sub	r1, fp, #52	@,,
	bl	AIP_GetChnStatus	@
	ldr	r2, [fp, #-52]	@ stChnStatus.CurnStatus, stChnStatus.CurnStatus
	ldr	r3, [r4, #16]	@ <variable>.AipMask, <variable>.AipMask
	cmp	r2, #1	@ stChnStatus.CurnStatus,
	orreq	r5, r5, #32	@ u32UnderFlag, u32UnderFlag,
	b	.L124	@
.L186:
	mov	r0, #3	@,
	sub	r1, fp, #136	@,,
	bl	AIP_QueryFiFoInfo	@
	ldr	r3, [r4, #44]	@ <variable>.BlockSize, <variable>.BlockSize
	ldr	r0, [fp, #-116]	@ stRbfIn.u32TotalDataSize, stRbfIn.u32TotalDataSize
	ldr	ip, [fp, #-120]	@ TailBytes.631, stRbfIn.u32ContiguousDataSize
	cmp	r0, r3	@ stRbfIn.u32TotalDataSize, <variable>.BlockSize
	bhi	.L117	@,
	mov	r0, #3	@,
	sub	r1, fp, #52	@,,
	bl	AIP_GetChnStatus	@
	ldr	r2, [fp, #-52]	@ stChnStatus.CurnStatus, stChnStatus.CurnStatus
	ldr	r3, [r4, #16]	@ <variable>.AipMask, <variable>.AipMask
	cmp	r2, #1	@ stChnStatus.CurnStatus,
	orreq	r5, r5, #8	@ u32UnderFlag, u32UnderFlag,
	b	.L116	@
.L198:
	mov	r0, #7	@,
	bl	AIP_UnderFlowCalc	@
	b	.L144	@
.L194:
	mov	r0, #3	@,
	bl	AIP_UnderFlowCalc	@
	b	.L140	@
.L193:
	mov	r0, #2	@,
	bl	AIP_UnderFlowCalc	@
	b	.L139	@
.L192:
	mov	r0, #1	@,
	bl	AIP_UnderFlowCalc	@
	b	.L138	@
.L191:
	mov	r0, #0	@,
	bl	AIP_UnderFlowCalc	@
	b	.L137	@
.L196:
	mov	r0, #5	@,
	bl	AIP_UnderFlowCalc	@
	b	.L142	@
.L195:
	mov	r0, #4	@,
	bl	AIP_UnderFlowCalc	@
	b	.L141	@
.L197:
	mov	r0, #6	@,
	bl	AIP_UnderFlowCalc	@
	b	.L143	@
.L113:
	ldr	r2, [fp, #-132]	@ stRbfIn.u32ReadPtr, stRbfIn.u32ReadPtr
	mov	r0, ip	@, TailBytes.610
	ldr	lr, [fp, #-136]	@ stRbfIn.u32BasePtr, stRbfIn.u32BasePtr
	ldr	r1, [r4, #48]	@, <variable>.FrameSize
	add	r8, lr, r2	@ tmp282, stRbfIn.u32BasePtr, stRbfIn.u32ReadPtr
	str	ip, [fp, #-144]	@,
	bl	__aeabi_uidiv	@
	mov	r3, sl	@, BitDepth
	mov	r1, r8	@, tmp282
	mul	r2, r9, r0	@ tmp289, Ch,
	mov	r0, r7	@, pDataBuf
	bl	UTIL_MixerPro	@
	ldr	r1, [r4, #44]	@ prephitmp.621, <variable>.BlockSize
	ldr	ip, [fp, #-144]	@,
	cmp	ip, r1	@ TailBytes.610, prephitmp.621
	bcc	.L114	@,
.L115:
	mov	r0, #2	@,
	add	r6, r6, #1	@ MixCnt, MixCnt,
	bl	AIP_ReadFiFoComplete	@
	ldr	r3, [r4, #16]	@ <variable>.AipMask, <variable>.AipMask
	b	.L112	@
.L117:
	ldr	lr, [fp, #-132]	@ stRbfIn.u32ReadPtr, stRbfIn.u32ReadPtr
	mov	r0, ip	@, TailBytes.631
	ldr	r8, [fp, #-136]	@ stRbfIn.u32BasePtr, stRbfIn.u32BasePtr
	ldr	r1, [r4, #48]	@, <variable>.FrameSize
	add	r8, r8, lr	@ tmp305, stRbfIn.u32BasePtr, stRbfIn.u32ReadPtr
	str	ip, [fp, #-144]	@,
	bl	__aeabi_uidiv	@
	mov	r3, sl	@, BitDepth
	mov	r1, r8	@, tmp305
	mul	r2, r9, r0	@ tmp312, Ch,
	mov	r0, r7	@, pDataBuf
	bl	UTIL_MixerPro	@
	ldr	r1, [r4, #44]	@ prephitmp.642, <variable>.BlockSize
	ldr	ip, [fp, #-144]	@,
	cmp	ip, r1	@ TailBytes.631, prephitmp.642
	bcc	.L118	@,
.L119:
	mov	r0, #3	@,
	add	r6, r6, #1	@ MixCnt, MixCnt,
	bl	AIP_ReadFiFoComplete	@
	ldr	r3, [r4, #16]	@ <variable>.AipMask, <variable>.AipMask
	b	.L116	@
.L133:
	mov	r0, #7	@,
	sub	r1, fp, #52	@,,
	bl	AIP_GetChnStatus	@
	ldr	r3, [fp, #-52]	@ stChnStatus.CurnStatus, stChnStatus.CurnStatus
	cmp	r3, #1	@ stChnStatus.CurnStatus,
	orreq	r5, r5, #128	@ u32UnderFlag, u32UnderFlag,
	b	.L132	@
.L104:
	ldr	lr, [fp, #-132]	@ stRbfIn.u32ReadPtr, stRbfIn.u32ReadPtr
	mov	r0, r6	@, TailBytes.568
	ldr	ip, [fp, #-136]	@ stRbfIn.u32BasePtr, stRbfIn.u32BasePtr
	ldr	r1, [r4, #48]	@, <variable>.FrameSize
	add	r5, ip, lr	@ tmp236, stRbfIn.u32BasePtr, stRbfIn.u32ReadPtr
	bl	__aeabi_uidiv	@
	mov	r3, sl	@, BitDepth
	mov	r1, r5	@, tmp236
	mul	r2, r9, r0	@ tmp243, Ch,
	mov	r0, r7	@, pDataBuf
	bl	UTIL_MixerPro	@
	ldr	r1, [r4, #44]	@ prephitmp.579, <variable>.BlockSize
	cmp	r6, r1	@ TailBytes.568, prephitmp.579
	bcc	.L106	@,
.L107:
	mov	r0, #0	@,
	mov	r5, #0	@ u32UnderFlag,
	bl	AIP_ReadFiFoComplete	@
	mov	r6, #1	@ MixCnt,
	ldr	r3, [r4, #16]	@ <variable>.AipMask, <variable>.AipMask
	b	.L105	@
.L109:
	ldr	r3, [fp, #-132]	@ stRbfIn.u32ReadPtr, stRbfIn.u32ReadPtr
	mov	r0, ip	@, TailBytes.589
	ldr	r8, [fp, #-136]	@ stRbfIn.u32BasePtr, stRbfIn.u32BasePtr
	ldr	r1, [r4, #48]	@, <variable>.FrameSize
	add	r8, r8, r3	@ tmp259, stRbfIn.u32BasePtr, stRbfIn.u32ReadPtr
	str	ip, [fp, #-144]	@,
	bl	__aeabi_uidiv	@
	mov	r3, sl	@, BitDepth
	mov	r1, r8	@, tmp259
	mul	r2, r9, r0	@ tmp266, Ch,
	mov	r0, r7	@, pDataBuf
	bl	UTIL_MixerPro	@
	ldr	r1, [r4, #44]	@ prephitmp.600, <variable>.BlockSize
	ldr	ip, [fp, #-144]	@,
	cmp	ip, r1	@ TailBytes.589, prephitmp.600
	bcc	.L110	@,
.L111:
	mov	r0, #1	@,
	add	r6, r6, #1	@ MixCnt, MixCnt,
	bl	AIP_ReadFiFoComplete	@
	ldr	r3, [r4, #16]	@ <variable>.AipMask, <variable>.AipMask
	b	.L108	@
.L121:
	ldr	lr, [fp, #-132]	@ stRbfIn.u32ReadPtr, stRbfIn.u32ReadPtr
	mov	r0, ip	@, TailBytes.652
	ldr	r8, [fp, #-136]	@ stRbfIn.u32BasePtr, stRbfIn.u32BasePtr
	ldr	r1, [r4, #48]	@, <variable>.FrameSize
	add	r8, r8, lr	@ tmp328, stRbfIn.u32BasePtr, stRbfIn.u32ReadPtr
	str	ip, [fp, #-144]	@,
	bl	__aeabi_uidiv	@
	mov	r3, sl	@, BitDepth
	mov	r1, r8	@, tmp328
	mul	r2, r9, r0	@ tmp335, Ch,
	mov	r0, r7	@, pDataBuf
	bl	UTIL_MixerPro	@
	ldr	r1, [r4, #44]	@ prephitmp.663, <variable>.BlockSize
	ldr	ip, [fp, #-144]	@,
	cmp	ip, r1	@ TailBytes.652, prephitmp.663
	bcc	.L122	@,
.L123:
	mov	r0, #4	@,
	add	r6, r6, #1	@ MixCnt, MixCnt,
	bl	AIP_ReadFiFoComplete	@
	ldr	r3, [r4, #16]	@ <variable>.AipMask, <variable>.AipMask
	b	.L120	@
.L129:
	ldr	lr, [fp, #-132]	@ stRbfIn.u32ReadPtr, stRbfIn.u32ReadPtr
	mov	r0, ip	@, TailBytes.694
	ldr	r8, [fp, #-136]	@ stRbfIn.u32BasePtr, stRbfIn.u32BasePtr
	ldr	r1, [r4, #48]	@, <variable>.FrameSize
	add	r8, r8, lr	@ tmp374, stRbfIn.u32BasePtr, stRbfIn.u32ReadPtr
	str	ip, [fp, #-144]	@,
	bl	__aeabi_uidiv	@
	mov	r3, sl	@, BitDepth
	mov	r1, r8	@, tmp374
	mul	r2, r9, r0	@ tmp381, Ch,
	mov	r0, r7	@, pDataBuf
	bl	UTIL_MixerPro	@
	ldr	r1, [r4, #44]	@ prephitmp.705, <variable>.BlockSize
	ldr	ip, [fp, #-144]	@,
	cmp	ip, r1	@ TailBytes.694, prephitmp.705
	bcc	.L130	@,
.L131:
	mov	r0, #6	@,
	add	r6, r6, #1	@ MixCnt, MixCnt,
	bl	AIP_ReadFiFoComplete	@
	ldr	r3, [r4, #16]	@ <variable>.AipMask, <variable>.AipMask
	b	.L128	@
.L125:
	ldr	lr, [fp, #-132]	@ stRbfIn.u32ReadPtr, stRbfIn.u32ReadPtr
	mov	r0, ip	@, TailBytes.673
	ldr	r8, [fp, #-136]	@ stRbfIn.u32BasePtr, stRbfIn.u32BasePtr
	ldr	r1, [r4, #48]	@, <variable>.FrameSize
	add	r8, r8, lr	@ tmp351, stRbfIn.u32BasePtr, stRbfIn.u32ReadPtr
	str	ip, [fp, #-144]	@,
	bl	__aeabi_uidiv	@
	mov	r3, sl	@, BitDepth
	mov	r1, r8	@, tmp351
	mul	r2, r9, r0	@ tmp358, Ch,
	mov	r0, r7	@, pDataBuf
	bl	UTIL_MixerPro	@
	ldr	r1, [r4, #44]	@ prephitmp.684, <variable>.BlockSize
	ldr	ip, [fp, #-144]	@,
	cmp	ip, r1	@ TailBytes.673, prephitmp.684
	bcc	.L126	@,
.L127:
	mov	r0, #5	@,
	add	r6, r6, #1	@ MixCnt, MixCnt,
	bl	AIP_ReadFiFoComplete	@
	ldr	r3, [r4, #16]	@ <variable>.AipMask, <variable>.AipMask
	b	.L124	@
.L157:
	mov	r1, r8	@, tmp516
	mov	r0, #5	@,
	bl	AOP_GetChnStatus	@
	ldr	lr, [fp, #-152]	@ tmp506, %sfp
	mov	ip, r7	@ tmp528, tmp517
	ldmia	lr!, {r0, r1, r2, r3}	@ tmp506,,,,
	stmia	ip!, {r0, r1, r2, r3}	@ tmp528,,,,
	ldmia	lr, {r0, r1, r2, r3}	@ tmp506,,,,
	stmia	ip, {r0, r1, r2, r3}	@ tmp528,,,,
	bl	AOE_GetWKBuf	@
	mov	r2, r7	@, tmp517
	mov	r1, r0	@ D.15472,
	ldr	r0, [fp, #-60]	@, AopStatus.u32BitPerSample
	bl	UTIL_UnifyBitDepthPcmData	@
	ldr	r1, [fp, #-136]	@, stUnfFrame.pDataBuf
	ldr	r2, [fp, #-128]	@, stUnfFrame.u32DataBytes
	mov	r0, #5	@,
	bl	AOP_WriteRbfData	@
	bl	AOE_ReleaseWKBuf	@
	b	.L156	@
.L204:
	mov	r1, r8	@, tmp516
	mov	r0, #4	@,
	bl	AOP_GetChnStatus	@
	ldr	lr, [fp, #-152]	@ tmp490, %sfp
	mov	ip, r7	@ tmp527, tmp517
	ldmia	lr!, {r0, r1, r2, r3}	@ tmp490,,,,
	stmia	ip!, {r0, r1, r2, r3}	@ tmp527,,,,
	ldmia	lr, {r0, r1, r2, r3}	@ tmp490,,,,
	stmia	ip, {r0, r1, r2, r3}	@ tmp527,,,,
	bl	AOE_GetWKBuf	@
	mov	r2, r7	@, tmp517
	mov	r1, r0	@ temp.793,
	ldr	r0, [fp, #-60]	@, AopStatus.u32BitPerSample
	bl	UTIL_UnifyBitDepthPcmData	@
	ldr	r1, [fp, #-136]	@, stUnfFrame.pDataBuf
	ldr	r2, [fp, #-128]	@, stUnfFrame.u32DataBytes
	mov	r0, #4	@,
	bl	AOP_WriteRbfData	@
	bl	AOE_ReleaseWKBuf	@
	b	.L154	@
.L203:
	mov	r1, r8	@, tmp516
	mov	r0, #3	@,
	bl	AOP_GetChnStatus	@
	ldr	lr, [fp, #-152]	@ tmp476, %sfp
	mov	ip, r7	@ tmp526, tmp517
	ldmia	lr!, {r0, r1, r2, r3}	@ tmp476,,,,
	stmia	ip!, {r0, r1, r2, r3}	@ tmp526,,,,
	ldmia	lr, {r0, r1, r2, r3}	@ tmp476,,,,
	stmia	ip, {r0, r1, r2, r3}	@ tmp526,,,,
	bl	AOE_GetWKBuf	@
	mov	r2, r7	@, tmp517
	mov	r1, r0	@ temp.777,
	ldr	r0, [fp, #-60]	@, AopStatus.u32BitPerSample
	bl	UTIL_UnifyBitDepthPcmData	@
	ldr	r1, [fp, #-136]	@, stUnfFrame.pDataBuf
	ldr	r2, [fp, #-128]	@, stUnfFrame.u32DataBytes
	mov	r0, #3	@,
	bl	AOP_WriteRbfData	@
	bl	AOE_ReleaseWKBuf	@
	b	.L152	@
.L202:
	mov	r1, r8	@, tmp516
	mov	r0, #2	@,
	bl	AOP_GetChnStatus	@
	ldr	lr, [fp, #-152]	@ tmp462, %sfp
	mov	ip, r7	@ tmp525, tmp517
	ldmia	lr!, {r0, r1, r2, r3}	@ tmp462,,,,
	stmia	ip!, {r0, r1, r2, r3}	@ tmp525,,,,
	ldmia	lr, {r0, r1, r2, r3}	@ tmp462,,,,
	stmia	ip, {r0, r1, r2, r3}	@ tmp525,,,,
	bl	AOE_GetWKBuf	@
	mov	r2, r7	@, tmp517
	mov	r1, r0	@ temp.761,
	ldr	r0, [fp, #-60]	@, AopStatus.u32BitPerSample
	bl	UTIL_UnifyBitDepthPcmData	@
	ldr	r1, [fp, #-136]	@, stUnfFrame.pDataBuf
	ldr	r2, [fp, #-128]	@, stUnfFrame.u32DataBytes
	mov	r0, #2	@,
	bl	AOP_WriteRbfData	@
	bl	AOE_ReleaseWKBuf	@
	b	.L150	@
.L201:
	mov	r1, r8	@, tmp516
	mov	r0, #1	@,
	bl	AOP_GetChnStatus	@
	ldr	lr, [fp, #-152]	@ tmp448, %sfp
	mov	ip, r7	@ tmp524, tmp517
	ldmia	lr!, {r0, r1, r2, r3}	@ tmp448,,,,
	stmia	ip!, {r0, r1, r2, r3}	@ tmp524,,,,
	ldmia	lr, {r0, r1, r2, r3}	@ tmp448,,,,
	stmia	ip, {r0, r1, r2, r3}	@ tmp524,,,,
	bl	AOE_GetWKBuf	@
	mov	r2, r7	@, tmp517
	mov	r1, r0	@ temp.745,
	ldr	r0, [fp, #-60]	@, AopStatus.u32BitPerSample
	bl	UTIL_UnifyBitDepthPcmData	@
	ldr	r1, [fp, #-136]	@, stUnfFrame.pDataBuf
	ldr	r2, [fp, #-128]	@, stUnfFrame.u32DataBytes
	mov	r0, #1	@,
	bl	AOP_WriteRbfData	@
	bl	AOE_ReleaseWKBuf	@
	b	.L148	@
.L200:
	mov	r0, r9	@, tmp430
	mov	r1, r8	@, tmp516
	bl	AOP_GetChnStatus	@
	ldr	lr, [fp, #-152]	@ tmp434, %sfp
	mov	ip, r7	@ tmp523, tmp517
	ldmia	lr!, {r0, r1, r2, r3}	@ tmp434,,,,
	stmia	ip!, {r0, r1, r2, r3}	@ tmp523,,,,
	ldmia	lr, {r0, r1, r2, r3}	@ tmp434,,,,
	stmia	ip, {r0, r1, r2, r3}	@ tmp523,,,,
	bl	AOE_GetWKBuf	@
	mov	r2, r7	@, tmp517
	mov	r1, r0	@ temp.729,
	ldr	r0, [fp, #-60]	@, AopStatus.u32BitPerSample
	bl	UTIL_UnifyBitDepthPcmData	@
	ldr	r1, [fp, #-136]	@, stUnfFrame.pDataBuf
	ldr	r2, [fp, #-128]	@, stUnfFrame.u32DataBytes
	mov	r0, r9	@, tmp430
	bl	AOP_WriteRbfData	@
	bl	AOE_ReleaseWKBuf	@
	b	.L146	@
.L126:
	rsb	r0, ip, r1	@, TailBytes.673, prephitmp.684
	ldr	r1, [r4, #48]	@, <variable>.FrameSize
	str	ip, [fp, #-144]	@,
	bl	__aeabi_uidiv	@
	ldr	r1, [fp, #-144]	@,
	mov	r3, sl	@, BitDepth
	add	ip, r7, r1	@ tmp359, pDataBuf, TailBytes.673
	ldr	r1, [fp, #-136]	@, stRbfIn.u32BasePtr
	mul	r2, r9, r0	@ tmp366, Ch,
	mov	r0, ip	@, tmp359
	bl	UTIL_MixerPro	@
	ldr	r1, [r4, #44]	@ prephitmp.684, <variable>.BlockSize
	b	.L127	@
.L130:
	rsb	r0, ip, r1	@, TailBytes.694, prephitmp.705
	ldr	r1, [r4, #48]	@, <variable>.FrameSize
	str	ip, [fp, #-144]	@,
	bl	__aeabi_uidiv	@
	ldr	r1, [fp, #-144]	@,
	mov	r3, sl	@, BitDepth
	add	ip, r7, r1	@ tmp382, pDataBuf, TailBytes.694
	ldr	r1, [fp, #-136]	@, stRbfIn.u32BasePtr
	mul	r2, r9, r0	@ tmp389, Ch,
	mov	r0, ip	@, tmp382
	bl	UTIL_MixerPro	@
	ldr	r1, [r4, #44]	@ prephitmp.705, <variable>.BlockSize
	b	.L131	@
.L122:
	rsb	r0, ip, r1	@, TailBytes.652, prephitmp.663
	ldr	r1, [r4, #48]	@, <variable>.FrameSize
	str	ip, [fp, #-144]	@,
	bl	__aeabi_uidiv	@
	ldr	r1, [fp, #-144]	@,
	mov	r3, sl	@, BitDepth
	add	ip, r7, r1	@ tmp336, pDataBuf, TailBytes.652
	ldr	r1, [fp, #-136]	@, stRbfIn.u32BasePtr
	mul	r2, r9, r0	@ tmp343, Ch,
	mov	r0, ip	@, tmp336
	bl	UTIL_MixerPro	@
	ldr	r1, [r4, #44]	@ prephitmp.663, <variable>.BlockSize
	b	.L123	@
.L110:
	rsb	r0, ip, r1	@, TailBytes.589, prephitmp.600
	ldr	r1, [r4, #48]	@, <variable>.FrameSize
	str	ip, [fp, #-144]	@,
	bl	__aeabi_uidiv	@
	ldr	r1, [fp, #-144]	@,
	mov	r3, sl	@, BitDepth
	add	ip, r7, r1	@ tmp267, pDataBuf, TailBytes.589
	ldr	r1, [fp, #-136]	@, stRbfIn.u32BasePtr
	mul	r2, r9, r0	@ tmp274, Ch,
	mov	r0, ip	@, tmp267
	bl	UTIL_MixerPro	@
	ldr	r1, [r4, #44]	@ prephitmp.600, <variable>.BlockSize
	b	.L111	@
.L106:
	rsb	r0, r6, r1	@, TailBytes.568, prephitmp.579
	ldr	r1, [r4, #48]	@, <variable>.FrameSize
	bl	__aeabi_uidiv	@
	add	r6, r7, r6	@ tmp244, pDataBuf, TailBytes.568
	ldr	r1, [fp, #-136]	@, stRbfIn.u32BasePtr
	mov	r3, sl	@, BitDepth
	mul	r2, r9, r0	@ tmp251, Ch,
	mov	r0, r6	@, tmp244
	bl	UTIL_MixerPro	@
	ldr	r1, [r4, #44]	@ prephitmp.579, <variable>.BlockSize
	b	.L107	@
.L118:
	rsb	r0, ip, r1	@, TailBytes.631, prephitmp.642
	ldr	r1, [r4, #48]	@, <variable>.FrameSize
	str	ip, [fp, #-144]	@,
	bl	__aeabi_uidiv	@
	ldr	r1, [fp, #-144]	@,
	mov	r3, sl	@, BitDepth
	add	ip, r7, r1	@ tmp313, pDataBuf, TailBytes.631
	ldr	r1, [fp, #-136]	@, stRbfIn.u32BasePtr
	mul	r2, r9, r0	@ tmp320, Ch,
	mov	r0, ip	@, tmp313
	bl	UTIL_MixerPro	@
	ldr	r1, [r4, #44]	@ prephitmp.642, <variable>.BlockSize
	b	.L119	@
.L114:
	rsb	r0, ip, r1	@, TailBytes.610, prephitmp.621
	ldr	r1, [r4, #48]	@, <variable>.FrameSize
	str	ip, [fp, #-144]	@,
	bl	__aeabi_uidiv	@
	ldr	r1, [fp, #-144]	@,
	mov	r3, sl	@, BitDepth
	add	ip, r7, r1	@ tmp290, pDataBuf, TailBytes.610
	ldr	r1, [fp, #-136]	@, stRbfIn.u32BasePtr
	mul	r2, r9, r0	@ tmp297, Ch,
	mov	r0, ip	@, tmp290
	bl	UTIL_MixerPro	@
	ldr	r1, [r4, #44]	@ prephitmp.621, <variable>.BlockSize
	b	.L115	@
.L205:
	rsb	r0, ip, r1	@, TailBytes, prephitmp.515
	ldr	r1, [r4, #48]	@, <variable>.FrameSize
	bl	__aeabi_uidiv	@
	ldr	r1, [fp, #-144]	@,
	mov	r3, sl	@, BitDepth
	add	ip, r7, r1	@ tmp403, pDataBuf, TailBytes
	ldr	r1, [fp, #-136]	@, stRbfIn.u32BasePtr
	mul	r2, r9, r0	@ tmp410, Ch,
	mov	r0, ip	@, tmp403
	bl	UTIL_MixerPro	@
	ldr	r1, [r4, #44]	@ prephitmp.515, <variable>.BlockSize
	b	.L134	@
.L96:
	bic	r0, r0, #2	@ Flag, Flag,
	b	.L94	@
	.fnend
	.size	ENGINEProcChnData, .-ENGINEProcChnData
	.align	2
	.global	ENGINEDestroyChn
	.type	ENGINEDestroyChn, %function
ENGINEDestroyChn:
	.fnstart
.LFB769:
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
	subs	r1, r0, #0	@ inst, inst
	ldmeqfd	sp, {fp, sp, pc}	@
	mov	r0, #1	@,
	bl	HI_FREE	@
	ldmfd	sp, {fp, sp, pc}	@
	.fnend
	.size	ENGINEDestroyChn, .-ENGINEDestroyChn
	.align	2
	.global	ENGINE_DeInit
	.type	ENGINE_DeInit, %function
ENGINE_DeInit:
	.fnstart
.LFB785:
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
	ldr	r0, [r4, #24]	@, g_stEneRm.hEne
	bl	ENGINEDestroyChn	@
	ldr	r0, [r4, #28]	@, g_stEneRm.hEne
	str	r5, [r4, #24]	@ tmp136, g_stEneRm.hEne
	bl	ENGINEDestroyChn	@
	ldr	r0, [r4, #32]	@, g_stEneRm.hEne
	str	r5, [r4, #28]	@ tmp136, g_stEneRm.hEne
	bl	ENGINEDestroyChn	@
	ldr	r0, [r4, #36]	@, g_stEneRm.hEne
	str	r5, [r4, #32]	@ tmp136, g_stEneRm.hEne
	bl	ENGINEDestroyChn	@
	ldr	r0, [r4, #40]	@, g_stEneRm.hEne
	str	r5, [r4, #36]	@ tmp136, g_stEneRm.hEne
	bl	ENGINEDestroyChn	@
	ldr	r0, [r4, #44]	@, g_stEneRm.hEne
	str	r5, [r4, #40]	@ tmp136, g_stEneRm.hEne
	bl	ENGINEDestroyChn	@
	str	r5, [r4, #44]	@ tmp136, g_stEneRm.hEne
	ldmfd	sp, {r4, r5, fp, sp, pc}	@
	.fnend
	.size	ENGINE_DeInit, .-ENGINE_DeInit
	.align	2
	.global	ENGINECreateChn
	.type	ENGINECreateChn, %function
ENGINECreateChn:
	.fnstart
.LFB768:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	.movsp ip
	mov	ip, sp	@,
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}	@,
	.pad #4
	.save {r4, r5, fp, ip, lr}
	.setfp fp, ip, #-4
	sub	fp, ip, #4	@,,
	.pad #16
	sub	sp, sp, #16	@,,
	push	{lr}
	bl	__gnu_mcount_nc
	movw	r3, #:lower16:.LANCHOR0	@ tmp151,
	movt	r3, #:upper16:.LANCHOR0	@ tmp151,
	sub	r5, fp, #36	@ tmp152,,
	mov	r1, #16	@,
	ldr	r4, [r3, r0, asl #2]	@ pEneReg, g_stEneRm.hwEneReg
	mov	r0, r5	@, tmp152
	bl	__memzero	@
	mov	r0, r5	@, tmp152
	ldr	r1, [r4, #0]	@ tmp153,* pEneReg
	and	r1, r1, #3	@, tmp153,
	bl	AOE_REG_ReadBitDepth	@
	cmp	r0, #0	@,
	beq	.L220	@,
.L212:
	mov	r3, #0	@ D.14715,
.L215:
	mov	r0, r3	@, D.14715
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L220:
	ldr	r2, [r4, #0]	@ tmp156,* pEneReg
	add	r0, r5, #4	@, tmp152,
	ubfx	r1, r2, #2, #2	@, tmp156,,
	bl	AOE_REG_ReadChannel	@
	cmp	r0, #0	@,
	bne	.L212	@,
	ldr	ip, [r4, #0]	@ tmp160,* pEneReg
	add	r0, r5, #8	@, tmp152,
	ubfx	r1, ip, #4, #4	@, tmp160,,
	bl	AOE_REG_ReadSampelRate	@
	cmp	r0, #0	@,
	bne	.L212	@,
	ldr	lr, [r4, #0]	@ tmp164,* pEneReg
	add	r0, r5, #12	@, tmp152,
	ubfx	r1, lr, #8, #2	@, tmp164,,
	bl	AOE_REG_ReadDataFormat	@
	cmp	r0, #0	@,
	bne	.L212	@,
	mov	r0, #1	@,
	mov	r1, #60	@,
	bl	HI_MALLOC	@
	subs	r4, r0, #0	@ state,
	beq	.L212	@,
	mov	r1, #60	@,
	bl	__memzero	@
	ldmia	r5, {r0, r1, r2, r3}	@ tmp152,,,,
	mov	ip, r0	@ <variable>.stAttr.u32BitPerSample,
	stmia	r4, {r0, r1, r2, r3}	@ state,,,,
	mov	r0, r1	@,
	mov	r1, ip	@, <variable>.stAttr.u32BitPerSample
	bl	UTIL_CalcFrameSize	@
	ldr	r3, [r4, #8]	@ <variable>.stAttr.u32SampleRate, <variable>.stAttr.u32SampleRate
	movw	r2, #:lower16:274877907	@ tmp177,
	movt	r2, #:upper16:274877907	@ tmp177,
	add	r1, r3, r3, asl #2	@, tmp175, <variable>.stAttr.u32SampleRate, <variable>.stAttr.u32SampleRate,
	umull	r3, ip, r2, r1	@, tmp176, tmp177, tmp175
	ldr	r1, [r4, #0]	@, <variable>.stAttr.u32BitPerSample
	mov	r2, ip, lsr #6	@ samples, tmp176,
	tst	r2, #3	@ samples,
	addne	r2, r2, #3	@ tmp179, samples,
	movne	r2, r2, lsr #2	@ D.15141, tmp179,
	movne	r2, r2, asl #2	@ samples, D.15141,
	mul	r3, r0, r2	@ tmp180, framesize, samples
	ldr	r0, [r4, #4]	@, <variable>.stAttr.u32Channels
	str	r3, [r4, #44]	@ tmp180, <variable>.BlockSize
	bl	UTIL_CalcFrameSize	@
	mov	r1, r0	@, D.14728
	str	r0, [r4, #48]	@ D.14728, <variable>.FrameSize
	ldr	r0, [r4, #44]	@, <variable>.BlockSize
	bl	__aeabi_uidiv	@
	mov	r1, #1	@ tmp186,
	mov	r3, r4	@ D.14715, state
	str	r1, [r4, #56]	@ tmp186, <variable>.CurStatus
	str	r0, [r4, #52]	@, <variable>.SamplePerFrame
	b	.L215	@
	.fnend
	.size	ENGINECreateChn, .-ENGINECreateChn
	.align	2
	.global	ENGINEProcChnStartCmd
	.type	ENGINEProcChnStartCmd, %function
ENGINEProcChnStartCmd:
	.fnstart
.LFB776:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	.movsp ip
	mov	ip, sp	@,
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}	@,
	.pad #4
	.save {r4, r5, r6, r7, fp, ip, lr}
	.setfp fp, ip, #-4
	sub	fp, ip, #4	@,,
	push	{lr}
	bl	__gnu_mcount_nc
	movw	r4, #:lower16:.LANCHOR0	@ tmp170,
	add	r7, r0, #6	@ tmp145, Id,
	movt	r4, #:upper16:.LANCHOR0	@ tmp170,
	mov	r6, r0	@ Id, Id
	ldr	r5, [r4, r7, asl #2]	@ D.14950, g_stEneRm.hEne
	cmp	r5, #0	@ D.14950,
	beq	.L231	@,
	ldr	r3, [r5, #56]	@ <variable>.CurStatus, <variable>.CurStatus
	cmp	r3, #0	@ <variable>.CurStatus,
	beq	.L232	@,
.L226:
	mov	r3, #1	@ tmp164,
	str	r3, [r5, #56]	@ tmp164, <variable>.CurStatus
.L225:
	ldr	r3, [r4, r6, asl #2]	@ pEneReg, g_stEneRm.hwEneReg
	ldr	r2, [r3, #4]	@ D.15168,
	ands	r2, r2, #8	@ tmp166, D.15168,
	ldmnefd	sp, {r4, r5, r6, r7, fp, sp, pc}	@
	ldr	ip, [r3, #4]	@ tmp167,
	bfi	ip, r2, #4, #4	@ tmp167, tmp166,,
	str	ip, [r3, #4]	@ tmp167,
	ldr	r1, [r3, #4]	@ tmp168,
	orr	r0, r1, #8	@ tmp173, tmp168,
	str	r0, [r3, #4]	@ tmp173,
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}	@
.L232:
	mov	r0, r5	@, D.14950
	bl	ENGINEDestroyChn	@
	mov	r0, r6	@, Id
	bl	ENGINECreateChn	@
	cmp	r0, #0	@ D.14960,
	str	r0, [r4, r7, asl #2]	@ D.14960, g_stEneRm.hEne
	bne	.L226	@,
.L230:
	ldr	r3, [r4, r6, asl #2]	@ pEneReg, g_stEneRm.hwEneReg
	ldr	r2, [r3, #4]	@ D.15161,
	tst	r2, #8	@ D.15161,
	ldmnefd	sp, {r4, r5, r6, r7, fp, sp, pc}	@
	ldr	ip, [r3, #4]	@ tmp160,
	mov	lr, #3	@ tmp161,
	bfi	ip, lr, #4, #4	@ tmp160, tmp161,,
	str	ip, [r3, #4]	@ tmp160,
	ldr	r1, [r3, #4]	@ tmp162,
	orr	r0, r1, #8	@ tmp172, tmp162,
	str	r0, [r3, #4]	@ tmp172,
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}	@
.L231:
	bl	ENGINECreateChn	@
	cmp	r0, #0	@ D.14953,
	str	r0, [r4, r7, asl #2]	@ D.14953, g_stEneRm.hEne
	movne	r3, #1	@ tmp154,
	strne	r3, [r0, #56]	@ tmp154, <variable>.CurStatus
	bne	.L225	@,
	b	.L230	@
	.fnend
	.size	ENGINEProcChnStartCmd, .-ENGINEProcChnStartCmd
	.align	2
	.global	ENGINE_GetChnCmd
	.type	ENGINE_GetChnCmd, %function
ENGINE_GetChnCmd:
	.fnstart
.LFB764:
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
	ldr	r3, [r3, r0, asl #2]	@ pEneReg, g_stEneRm.hwEneReg
	ldr	r2, [r3, #4]	@ D.14626,
	tst	r2, #8	@ D.14626,
	bne	.L241	@,
	ldr	r0, [r3, #4]	@ tmp143,
	ands	r0, r0, #3	@ D.14630, tmp143,
	beq	.L235	@,
	cmp	r0, #1	@ D.14630,
	beq	.L235	@,
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
.L241:
	mov	r0, #2	@ eNewCmd,
.L235:
	str	r0, [r4, #0]	@ eNewCmd,* newcmd
	ldmfd	sp, {r3, r4, fp, sp, pc}	@
	.fnend
	.size	ENGINE_GetChnCmd, .-ENGINE_GetChnCmd
	.align	2
	.global	ENGINEProcChnCmd
	.type	ENGINEProcChnCmd, %function
ENGINEProcChnCmd:
	.fnstart
.LFB777:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	.movsp ip
	mov	ip, sp	@,
	stmfd	sp!, {r4, fp, ip, lr, pc}	@,
	.pad #4
	.save {r4, fp, ip, lr}
	.setfp fp, ip, #-4
	sub	fp, ip, #4	@,,
	.pad #12
	sub	sp, sp, #12	@,,
	push	{lr}
	bl	__gnu_mcount_nc
	sub	r1, fp, #24	@,,
	mov	r4, r0	@ Id, Id
	bl	ENGINE_GetChnCmd	@
	cmp	r0, #2	@,
	beq	.L246	@,
	ldr	r3, [fp, #-24]	@ newcmd, newcmd
	cmp	r3, #0	@ newcmd,
	bne	.L247	@,
	mov	r0, r4	@, Id
	bl	ENGINEProcChnStartCmd	@
.L246:
	sub	sp, fp, #16
	ldmfd	sp, {r4, fp, sp, pc}
.L247:
	cmp	r3, #1	@ newcmd,
	bne	.L246	@,
	add	r4, r4, #6	@ tmp139, Id,
	movw	r3, #:lower16:.LANCHOR0	@ tmp138,
	movt	r3, #:upper16:.LANCHOR0	@ tmp138,
	ldr	r3, [r3, r4, asl #2]	@ state, g_stEneRm.hEne
	cmp	r3, #0	@ state,
	beq	.L246	@,
	ldr	r2, [r3, #56]	@ <variable>.CurStatus, <variable>.CurStatus
	cmp	r2, #1	@ <variable>.CurStatus,
	moveq	r2, #0	@ tmp141,
	streq	r2, [r3, #56]	@ tmp141, <variable>.CurStatus
	b	.L246	@
	.fnend
	.size	ENGINEProcChnCmd, .-ENGINEProcChnCmd
	.align	2
	.global	ENGINEProcChn
	.type	ENGINEProcChn, %function
ENGINEProcChn:
	.fnstart
.LFB781:
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
	mov	r5, r0	@ Id, Id
	bl	ENGINEProcChnCmd	@
	movw	r3, #:lower16:.LANCHOR0	@ tmp141,
	add	r2, r5, #6	@ tmp142, Id,
	movt	r3, #:upper16:.LANCHOR0	@ tmp141,
	ldr	r4, [r3, r2, asl #2]	@ state, g_stEneRm.hEne
	cmp	r4, #0	@ state,
	beq	.L249	@,
	ldr	r0, [r4, #56]	@ <variable>.CurStatus, <variable>.CurStatus
	cmp	r0, #1	@ <variable>.CurStatus,
	beq	.L253	@,
.L249:
	mov	r0, #0	@,
	ldmfd	sp, {r4, r5, fp, sp, pc}	@
.L253:
	ldr	r5, [r3, r5, asl #2]	@ pEneReg, g_stEneRm.hwEneReg
	ldrh	r3, [r5, #12]	@ <variable>.ENGINE_ROU_DST.bits.aop_buf_ena, <variable>.ENGINE_ROU_DST.bits.aop_buf_ena
	cmp	r3, #0	@ <variable>.ENGINE_ROU_DST.bits.aop_buf_ena,
	bne	.L254	@,
.L250:
	ldrh	r1, [r5, #8]	@ <variable>.ENGINE_MIX_SRC.bits.aip_fifo_ena, <variable>.ENGINE_MIX_SRC.bits.aip_fifo_ena
	mov	r0, r4	@, state
	ldr	ip, [r4, #16]	@ <variable>.AipMask, <variable>.AipMask
	cmp	ip, r1	@ <variable>.AipMask, <variable>.ENGINE_MIX_SRC.bits.aip_fifo_ena
	ldrneh	r1, [r5, #8]	@ <variable>.ENGINE_MIX_SRC.bits.aip_fifo_ena, <variable>.ENGINE_MIX_SRC.bits.aip_fifo_ena
	strne	r1, [r4, #16]	@ <variable>.ENGINE_MIX_SRC.bits.aip_fifo_ena, <variable>.AipMask
	bl	ENGINEProcChnData	@
	mov	r0, #0	@,
	ldmfd	sp, {r4, r5, fp, sp, pc}	@
.L254:
	ldrh	r1, [r5, #12]	@ <variable>.ENGINE_ROU_DST.bits.aop_buf_ena, <variable>.ENGINE_ROU_DST.bits.aop_buf_ena
	mov	r0, r4	@, state
	bl	ENGINEUpdateChnAopState	@
	b	.L250	@
	.fnend
	.size	ENGINEProcChn, .-ENGINEProcChn
	.align	2
	.global	ENGINE_Process
	.type	ENGINE_Process, %function
ENGINE_Process:
	.fnstart
.LFB783:
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
	bl	ENGINEProcChn	@
	mov	r0, #1	@,
	bl	ENGINEProcChn	@
	mov	r0, #2	@,
	bl	ENGINEProcChn	@
	mov	r0, #3	@,
	bl	ENGINEProcChn	@
	mov	r0, #4	@,
	bl	ENGINEProcChn	@
	mov	r0, #5	@,
	bl	ENGINEProcChn	@
	ldmfd	sp, {fp, sp, pc}	@
	.fnend
	.size	ENGINE_Process, .-ENGINE_Process
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
.LC0:
	.ascii	"invalid cmd(%d)\012\000"
	.bss
	.align	2
.LANCHOR0 = . + 0
	.type	g_stEneRm, %object
	.size	g_stEneRm, 48
g_stEneRm:
	.space	48
	.ident	"GCC: (Hisilicon_v200(gcc4.4-290+glibc-2.11+eabi+nptl)) 4.4.1"
	.section	.note.GNU-stack,"",%progbits

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
	.file	"aoe_aip.c"
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
@ -DKBUILD_STR(s)=#s -DKBUILD_BASENAME=KBUILD_STR(aoe_aip)
@ -DKBUILD_MODNAME=KBUILD_STR(hi_adsp) -isystem
@ /opt/hisi-linux/x86-arm/arm-hisiv200-linux/bin/../lib/gcc/arm-hisiv200-linux-gnueabi/4.4.1/include
@ -include
@ /home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/include/linux/kconfig.h
@ -MD drivers/msp/adsp/adsp_v1_1/./hifi/aoe/src/aip/.aoe_aip.o.d
@ /home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/drivers/msp/adsp/adsp_v1_1/./hifi/aoe/src/aip/aoe_aip.c
@ -mlittle-endian -marm -mapcs -mno-sched-prolog -mabi=aapcs-linux
@ -mno-thumb-interwork -march=armv7-a -msoft-float -auxbase-strip
@ drivers/msp/adsp/adsp_v1_1/./hifi/aoe/src/aip/aoe_aip.o -O2 -Wall -Wundef
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
	.global	AIPGetCunrStatus
	.type	AIPGetCunrStatus, %function
AIPGetCunrStatus:
	.fnstart
.LFB764:
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
	add	r0, r0, #8	@ tmp138, Id,
	movt	r1, #:upper16:.LANCHOR0	@ tmp137,
	ldr	r3, [r1, r0, asl #2]	@ state, g_stAipRm.hAip
	cmp	r3, #0	@ state,
	moveq	r0, #3	@ D.14614,
	ldrne	r0, [r3, #204]	@ D.14614, <variable>.CurStatus
	ldmfd	sp, {fp, sp, pc}	@
	.fnend
	.size	AIPGetCunrStatus, .-AIPGetCunrStatus
	.align	2
	.global	AIPIsChnAttrChange
	.type	AIPIsChnAttrChange, %function
AIPIsChnAttrChange:
	.fnstart
.LFB779:
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
	movw	r3, #:lower16:.LANCHOR0	@ tmp162,
	add	r2, r0, #8	@ tmp164, Id,
	movt	r3, #:upper16:.LANCHOR0	@ tmp162,
	ldr	r2, [r3, r2, asl #2]	@ state, g_stAipRm.hAip
	ldr	r3, [r3, r0, asl #2]	@ pAipReg, g_stAipRm.hwAipReg
	ldr	r6, [r2, #120]	@ D.15065, <variable>.stBackRegs.AIP_BUFF_ATTR.u32
	ldr	ip, [r3, #0]	@ D.15066, <variable>.AIP_BUFF_ATTR.u32
	ldr	r7, [r2, #132]	@ D.15068, <variable>.stBackRegs.AIP_SRC_ATTR_EXT.u32
	ldr	r0, [r3, #12]	@ D.15069, <variable>.AIP_SRC_ATTR_EXT.u32
	subs	r4, r6, ip	@ tmp168, D.15065, D.15066
	movne	r4, #1	@ tmp168
	ldr	ip, [r2, #136]	@ D.15071, <variable>.stBackRegs.AIP_BUF_ADDR
	ldr	r5, [r3, #16]	@ D.15072, <variable>.AIP_BUF_ADDR
	cmp	r7, r0	@ D.15068, D.15069
	moveq	r1, r4	@, isChange, tmp168
	addne	r1, r4, #1	@,, isChange, tmp168
	ldr	r7, [r2, #140]	@ D.15074, <variable>.stBackRegs.AIP_BUF_SIZE.u32
	cmp	ip, r5	@ D.15071, D.15072
	moveq	r4, r1	@, isChange.342, isChange
	addne	r4, r1, #1	@,, isChange.342, isChange
	ldr	r6, [r3, #20]	@ D.15075, <variable>.AIP_BUF_SIZE.u32
	ldr	r0, [r2, #124]	@ tmp173,
	ldr	r1, [r3, #4]	@ tmp175,
	cmp	r7, r6	@ D.15074, D.15075
	moveq	ip, r4	@, isChange.343, isChange.342
	addne	ip, r4, #1	@,, isChange.343, isChange.342
	and	r5, r0, #3	@ D.15077, tmp173,
	ldr	r7, [r2, #124]	@ tmp179,
	and	r1, r1, #3	@ D.15078, tmp175,
	ldr	r4, [r3, #4]	@ tmp181,
	cmp	r5, r1	@ D.15077, D.15078
	moveq	r0, ip	@, isChange.344, isChange.343
	addne	r0, ip, #1	@,, isChange.344, isChange.343
	ldr	r6, [r2, #124]	@ tmp185,
	ldr	r1, [r3, #4]	@ tmp187,
	ubfx	r7, r7, #2, #2	@ D.15080, tmp179,,
	ldr	ip, [r2, #124]	@ tmp191,
	ubfx	r5, r4, #2, #2	@ D.15081, tmp181,,
	ldr	r4, [r3, #4]	@ tmp193,
	cmp	r7, r5	@ D.15080, D.15081
	moveq	r5, r0	@, isChange.345, isChange.344
	addne	r5, r0, #1	@,, isChange.345, isChange.344
	ubfx	r6, r6, #4, #4	@ D.15083, tmp185,,
	ldr	r0, [r2, #124]	@ tmp197,
	ubfx	r1, r1, #4, #4	@ D.15084, tmp187,,
	ldr	r2, [r3, #4]	@ tmp199,
	cmp	r6, r1	@ D.15083, D.15084
	moveq	r1, r5	@, isChange.346, isChange.345
	addne	r1, r5, #1	@,, isChange.346, isChange.345
	ubfx	ip, ip, #8, #2	@ D.15086, tmp191,,
	ubfx	r3, r4, #8, #2	@ D.15087, tmp193,,
	ubfx	r2, r2, #10, #6	@ D.15090, tmp199,,
	cmp	ip, r3	@ D.15086, D.15087
	moveq	r3, r1	@, isChange.347, isChange.346
	addne	r3, r1, #1	@,, isChange.347, isChange.346
	ubfx	r0, r0, #10, #6	@ D.15089, tmp197,,
	cmp	r0, r2	@ D.15089, D.15090
	moveq	r0, r3	@,, isChange.347
	addne	r0, r3, #1	@,,, isChange.347
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}	@
	.fnend
	.size	AIPIsChnAttrChange, .-AIPIsChnAttrChange
	.align	2
	.global	AIPAckChnCmd
	.type	AIPAckChnCmd, %function
AIPAckChnCmd:
	.fnstart
.LFB790:
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
	ldr	r3, [r3, r0, asl #2]	@ pAipReg, g_stAipRm.hwAipReg
	ldr	r2, [r3, #8]	@ D.15345,
	tst	r2, #8	@ D.15345,
	ldmnefd	sp, {fp, sp, pc}	@
	ldr	ip, [r3, #8]	@ tmp142,
	bfi	ip, r1, #4, #4	@ tmp142, Ack,,
	str	ip, [r3, #8]	@ tmp142,
	ldr	r1, [r3, #8]	@ tmp143,
	orr	r0, r1, #8	@ tmp145, tmp143,
	str	r0, [r3, #8]	@ tmp145,
	ldmfd	sp, {fp, sp, pc}	@
	.fnend
	.size	AIPAckChnCmd, .-AIPAckChnCmd
	.align	2
	.global	AIPUpdateChnState
	.type	AIPUpdateChnState, %function
AIPUpdateChnState:
	.fnstart
.LFB796:
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
	movw	r3, #:lower16:.LANCHOR0	@ tmp141,
	add	r2, r0, #8	@ tmp143, Id,
	movt	r3, #:upper16:.LANCHOR0	@ tmp141,
	ldr	r2, [r3, r2, asl #2]	@ state, g_stAipRm.hAip
	ldr	r3, [r3, r0, asl #2]	@ pAipReg, g_stAipRm.hwAipReg
	cmp	r2, #0	@ state,
	ldmeqfd	sp, {fp, sp, pc}	@
	mov	r1, #0	@ tmp144,
	str	r1, [r2, #208]	@ tmp144, <variable>.stGain.bLinearMode
	ldr	ip, [r3, #8]	@ tmp145,
	ubfx	r0, ip, #8, #7	@ D.15434, tmp145,,
	strh	r0, [r2, #212]	@ movhi	@ D.15434, <variable>.stGain.u16Gain
	ldr	r1, [r3, #8]	@ D.15436,
	tst	r1, #1610612736	@ D.15436,
	beq	.L15	@,
	ldr	r1, [r3, #8]	@ D.15443,
	and	ip, r1, #1610612736	@ tmp150, D.15443,
	cmp	ip, #536870912	@ tmp150,
	ldreq	r3, [r3, #8]	@ tmp151,
	ubfxeq	r3, r3, #24, #5	@ D.15447, tmp151,,
	rsbeq	r3, r3, #0	@ tmp153, D.15447
	streq	r3, [r2, #232]	@ tmp153, <variable>.s32UserSpeedAdjust
	ldmfd	sp, {fp, sp, pc}	@
.L15:
	ldr	r0, [r3, #8]	@ tmp148,
	ubfx	r3, r0, #24, #5	@ D.15440, tmp148,,
	str	r3, [r2, #232]	@ D.15440, <variable>.s32UserSpeedAdjust
	ldmfd	sp, {fp, sp, pc}	@
	.fnend
	.size	AIPUpdateChnState, .-AIPUpdateChnState
	.align	2
	.global	AIP_GetChnStatus
	.type	AIP_GetChnStatus, %function
AIP_GetChnStatus:
	.fnstart
.LFB802:
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
	movw	r2, #:lower16:.LANCHOR0	@ tmp136,
	add	r0, r0, #8	@ tmp137, Id,
	movt	r2, #:upper16:.LANCHOR0	@ tmp136,
	ldr	r3, [r2, r0, asl #2]	@ D.15517, g_stAipRm.hAip
	cmp	r3, #0	@ D.15517,
	ldrne	r3, [r3, #204]	@ <variable>.CurStatus, <variable>.CurStatus
	str	r3, [r1, #0]	@ <variable>.CurStatus, <variable>.CurnStatus
	ldmfd	sp, {fp, sp, pc}	@
	.fnend
	.size	AIP_GetChnStatus, .-AIP_GetChnStatus
	.align	2
	.global	AIP_UnderFlowCalc
	.type	AIP_UnderFlowCalc, %function
AIP_UnderFlowCalc:
	.fnstart
.LFB807:
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
	add	ip, r0, #8	@ tmp139, Id,
	movt	r1, #:upper16:.LANCHOR0	@ tmp137,
	ldr	r2, [r1, ip, asl #2]	@ tmp140, g_stAipRm.hAip
	ldr	r3, [r1, r0, asl #2]	@ pAipReg, g_stAipRm.hwAipReg
	cmp	r2, #0	@ tmp140,
	ldrneh	r2, [r3, #54]	@ <variable>.AIP_FIFO_SIZE.bits.aip_fifo_underflow_cnt, <variable>.AIP_FIFO_SIZE.bits.aip_fifo_underflow_cnt
	addne	r2, r2, #1	@ tmp143, <variable>.AIP_FIFO_SIZE.bits.aip_fifo_underflow_cnt,
	uxthne	r2, r2	@ D.15605, tmp143
	strneh	r2, [r3, #54]	@ movhi	@ D.15605, <variable>.AIP_FIFO_SIZE.bits.aip_fifo_underflow_cnt
	ldmfd	sp, {fp, sp, pc}	@
	.fnend
	.size	AIP_UnderFlowCalc, .-AIP_UnderFlowCalc
	.align	2
	.global	AIPFlushUnderFlowCalc
	.type	AIPFlushUnderFlowCalc, %function
AIPFlushUnderFlowCalc:
	.fnstart
.LFB808:
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
	movw	r1, #:lower16:.LANCHOR0	@ tmp135,
	movt	r1, #:upper16:.LANCHOR0	@ tmp135,
	mov	r2, #0	@ movhi	@,
	ldr	r3, [r1, r0, asl #2]	@ pAipReg, g_stAipRm.hwAipReg
	strh	r2, [r3, #54]	@ movhi	@, <variable>.AIP_FIFO_SIZE.bits.aip_fifo_underflow_cnt
	ldmfd	sp, {fp, sp, pc}	@
	.fnend
	.size	AIPFlushUnderFlowCalc, .-AIPFlushUnderFlowCalc
	.align	2
	.global	AIPFlushChnRufIn
	.type	AIPFlushChnRufIn, %function
AIPFlushChnRufIn:
	.fnstart
.LFB783:
	@ args = 0, pretend = 0, frame = 40
	@ frame_needed = 1, uses_anonymous_args = 0
	.movsp ip
	mov	ip, sp	@,
	stmfd	sp!, {r4, fp, ip, lr, pc}	@,
	.pad #4
	.save {r4, fp, ip, lr}
	.setfp fp, ip, #-4
	sub	fp, ip, #4	@,,
	.pad #44
	sub	sp, sp, #44	@,,
	push	{lr}
	bl	__gnu_mcount_nc
	mov	r4, r0	@ state, state
	ldr	r0, [r0, #184]	@ D.15216, <variable>.hRingIn
	cmp	r0, #0	@ D.15216,
	beq	.L28	@,
	sub	r1, fp, #56	@,,
	bl	RBUF_QueryInfo	@
	ldr	r0, [r4, #184]	@, <variable>.hRingIn
	ldr	r1, [fp, #-36]	@, stRbfIn.u32TotalDataSize
	bl	RBUF_ReadDataComplete	@
.L28:
	sub	sp, fp, #16
	ldmfd	sp, {r4, fp, sp, pc}
	.fnend
	.size	AIPFlushChnRufIn, .-AIPFlushChnRufIn
	.align	2
	.global	AIPCheckRbfOutFree
	.type	AIPCheckRbfOutFree, %function
AIPCheckRbfOutFree:
	.fnstart
.LFB769:
	@ args = 0, pretend = 0, frame = 40
	@ frame_needed = 1, uses_anonymous_args = 0
	.movsp ip
	mov	ip, sp	@,
	stmfd	sp!, {r4, fp, ip, lr, pc}	@,
	.pad #4
	.save {r4, fp, ip, lr}
	.setfp fp, ip, #-4
	sub	fp, ip, #4	@,,
	.pad #44
	sub	sp, sp, #44	@,,
	push	{lr}
	bl	__gnu_mcount_nc
	mov	r4, r1	@ blocksize, blocksize
	ldr	r0, [r0, #188]	@, <variable>.hRingOut
	sub	r1, fp, #56	@,,
	bl	RBUF_QueryInfo	@
	ldr	r3, [fp, #-28]	@ stRbfOut.u32TotalFreeSize, stRbfOut.u32TotalFreeSize
	cmp	r3, r4	@ stRbfOut.u32TotalFreeSize, blocksize
	bls	.L30	@,
	ldr	r1, [fp, #-36]	@ stRbfOut.u32TotalDataSize, stRbfOut.u32TotalDataSize
	ldr	r0, [fp, #-24]	@ stRbfOut.u32LatencySize, stRbfOut.u32LatencySize
	add	r2, r4, r1	@ tmp139, blocksize, stRbfOut.u32TotalDataSize
	cmp	r2, r0	@ tmp139, stRbfOut.u32LatencySize
	bhi	.L30	@,
.L31:
	mov	r0, r4	@, blocksize
	sub	sp, fp, #16
	ldmfd	sp, {r4, fp, sp, pc}
.L30:
	mov	r4, #0	@ blocksize,
	b	.L31	@
	.fnend
	.size	AIPCheckRbfOutFree, .-AIPCheckRbfOutFree
	.align	2
	.global	AIP_ReadFiFoComplete
	.type	AIP_ReadFiFoComplete, %function
AIP_ReadFiFoComplete:
	.fnstart
.LFB806:
	@ args = 0, pretend = 0, frame = 40
	@ frame_needed = 1, uses_anonymous_args = 0
	.movsp ip
	mov	ip, sp	@,
	stmfd	sp!, {r4, r5, r6, fp, ip, lr, pc}	@,
	.pad #4
	.save {r4, r5, r6, fp, ip, lr}
	.setfp fp, ip, #-4
	sub	fp, ip, #4	@,,
	.pad #44
	sub	sp, sp, #44	@,,
	push	{lr}
	bl	__gnu_mcount_nc
	movw	r5, #:lower16:.LANCHOR0	@ tmp140,
	add	r3, r0, #8	@ tmp141, Id,
	movt	r5, #:upper16:.LANCHOR0	@ tmp140,
	mov	r6, r0	@ Id, Id
	ldr	r4, [r5, r3, asl #2]	@ D.15583, g_stAipRm.hAip
	cmp	r4, #0	@ D.15583,
	beq	.L35	@,
	ldr	r0, [r4, #188]	@, <variable>.hRingOut
	bl	RBUF_ReadDataComplete	@
	ldr	r0, [r4, #188]	@, <variable>.hRingOut
	sub	r1, fp, #64	@,,
	ldr	r5, [r5, r6, asl #2]	@ pAipReg, g_stAipRm.hwAipReg
	bl	RBUF_QueryInfo	@
	ldr	r1, [r4, #1012]	@, <variable>.OutFrameSize
	ldr	r2, [r4, #72]	@, <variable>.stAttr.u32OutSampleRate
	ldr	r0, [fp, #-44]	@, stBufInfo.u32TotalDataSize
	bl	UTIL_LatencyMs	@
	ldr	r1, [r5, #4]	@ tmp151,
	bfi	r1, r0, #16, #6	@ tmp151,,,
	str	r1, [r5, #4]	@ tmp151,
.L35:
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
	.fnend
	.size	AIP_ReadFiFoComplete, .-AIP_ReadFiFoComplete
	.align	2
	.global	AIP_QueryFiFoInfo
	.type	AIP_QueryFiFoInfo, %function
AIP_QueryFiFoInfo:
	.fnstart
.LFB805:
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
	movw	r3, #:lower16:.LANCHOR0	@ tmp136,
	add	r2, r0, #8	@ tmp137, Id,
	movt	r3, #:upper16:.LANCHOR0	@ tmp136,
	mov	r0, r1	@ pstBufInfo, pstBufInfo
	ldr	r3, [r3, r2, asl #2]	@ D.15565, g_stAipRm.hAip
	cmp	r3, #0	@ D.15565,
	beq	.L40	@,
	ldr	r0, [r3, #188]	@, <variable>.hRingOut
	bl	RBUF_QueryInfo	@
	ldmfd	sp, {fp, sp, pc}	@
.L40:
	mov	r1, #36	@,
	bl	__memzero	@
	ldmfd	sp, {fp, sp, pc}	@
	.fnend
	.size	AIP_QueryFiFoInfo, .-AIP_QueryFiFoInfo
	.align	2
	.global	AIPSwResetReg
	.type	AIPSwResetReg, %function
AIPSwResetReg:
	.fnstart
.LFB798:
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
	mov	r1, #64	@,
	mov	r4, r0	@ pAipReg, pAipReg
	bl	__memzero	@
	ldr	ip, [r4, #8]	@ tmp134,
	mov	r2, #121	@ tmp137,
	orr	r1, ip, #8	@ tmp139, tmp134,
	str	r1, [r4, #8]	@ tmp139,
	ldr	r0, [r4, #8]	@ tmp136,
	bfi	r0, r2, #8, #7	@ tmp136, tmp137,,
	str	r0, [r4, #8]	@ tmp136,
	ldr	r3, [r4, #0]	@ tmp138,* pAipReg
	bfc	r3, #10, #1	@ tmp138,,
	str	r3, [r4, #0]	@ tmp138,* pAipReg
	ldmfd	sp, {r3, r4, fp, sp, pc}	@
	.fnend
	.size	AIPSwResetReg, .-AIPSwResetReg
	.align	2
	.global	AIP_Init
	.type	AIP_Init, %function
AIP_Init:
	.fnstart
.LFB799:
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
	movw	r4, #:lower16:.LANCHOR0	@ tmp142,
	movt	r4, #:upper16:.LANCHOR0	@ tmp142,
	mov	r6, r0	@ u32AipRegBase, u32AipRegBase
	mov	r5, #0	@ tmp144,
	str	r0, [r4, #0]	@ u32AipRegBase, g_stAipRm.hwAipReg
	bl	AIPSwResetReg	@
	add	r0, r6, #64	@ temp.608, u32AipRegBase,
	str	r5, [r4, #32]	@ tmp144, g_stAipRm.hAip
	str	r0, [r4, #4]	@ temp.608, g_stAipRm.hwAipReg
	bl	AIPSwResetReg	@
	add	r3, r6, #128	@ temp.610, u32AipRegBase,
	str	r5, [r4, #36]	@ tmp144, g_stAipRm.hAip
	str	r3, [r4, #8]	@ temp.610, g_stAipRm.hwAipReg
	mov	r0, r3	@, temp.610
	bl	AIPSwResetReg	@
	add	ip, r6, #192	@ temp.612, u32AipRegBase,
	str	r5, [r4, #40]	@ tmp144, g_stAipRm.hAip
	mov	r0, ip	@, temp.612
	str	ip, [r4, #12]	@ temp.612, g_stAipRm.hwAipReg
	bl	AIPSwResetReg	@
	add	r2, r6, #256	@ temp.614, u32AipRegBase,
	str	r5, [r4, #44]	@ tmp144, g_stAipRm.hAip
	mov	r0, r2	@, temp.614
	str	r2, [r4, #16]	@ temp.614, g_stAipRm.hwAipReg
	bl	AIPSwResetReg	@
	add	r1, r6, #320	@ temp.616, u32AipRegBase,
	str	r5, [r4, #48]	@ tmp144, g_stAipRm.hAip
	mov	r0, r1	@, temp.616
	str	r1, [r4, #20]	@ temp.616, g_stAipRm.hwAipReg
	bl	AIPSwResetReg	@
	add	r3, r6, #384	@ temp.618, u32AipRegBase,
	add	r6, r6, #448	@ D.15493, u32AipRegBase,
	str	r3, [r4, #24]	@ temp.618, g_stAipRm.hwAipReg
	mov	r0, r3	@, temp.618
	str	r5, [r4, #52]	@ tmp144, g_stAipRm.hAip
	bl	AIPSwResetReg	@
	mov	r0, r6	@, D.15493
	str	r5, [r4, #56]	@ tmp144, g_stAipRm.hAip
	str	r6, [r4, #28]	@ D.15493, g_stAipRm.hwAipReg
	bl	AIPSwResetReg	@
	str	r5, [r4, #60]	@ tmp144, g_stAipRm.hAip
	ldmfd	sp, {r3, r4, r5, r6, fp, sp, pc}	@
	.fnend
	.size	AIP_Init, .-AIP_Init
	.align	2
	.global	AIP_ReadFiFoData
	.type	AIP_ReadFiFoData, %function
AIP_ReadFiFoData:
	.fnstart
.LFB804:
	@ args = 0, pretend = 0, frame = 40
	@ frame_needed = 1, uses_anonymous_args = 0
	.movsp ip
	mov	ip, sp	@,
	stmfd	sp!, {r4, r5, r6, r7, fp, ip, lr, pc}	@,
	.pad #4
	.save {r4, r5, r6, r7, fp, ip, lr}
	.setfp fp, ip, #-4
	sub	fp, ip, #4	@,,
	.pad #40
	sub	sp, sp, #40	@,,
	push	{lr}
	bl	__gnu_mcount_nc
	movw	r4, #:lower16:.LANCHOR0	@ tmp143,
	add	r3, r0, #8	@ tmp144, Id,
	movt	r4, #:upper16:.LANCHOR0	@ tmp143,
	mov	r6, r0	@ Id, Id
	ldr	r5, [r4, r3, asl #2]	@ D.15542, g_stAipRm.hAip
	cmp	r5, #0	@ D.15542,
	moveq	r7, r5	@ ReadBytes, D.15542
	beq	.L47	@,
	ldr	r0, [r5, #188]	@, <variable>.hRingOut
	bl	RBUF_ReadData	@
	sub	r1, fp, #64	@,,
	ldr	r4, [r4, r6, asl #2]	@ pAipReg, g_stAipRm.hwAipReg
	mov	r7, r0	@ ReadBytes,
	ldr	r0, [r5, #188]	@, <variable>.hRingOut
	bl	RBUF_QueryInfo	@
	ldr	r1, [r5, #1012]	@, <variable>.OutFrameSize
	ldr	r2, [r5, #72]	@, <variable>.stAttr.u32OutSampleRate
	ldr	r0, [fp, #-44]	@, stBufInfo.u32TotalDataSize
	bl	UTIL_LatencyMs	@
	ldr	r1, [r4, #4]	@ tmp154,
	bfi	r1, r0, #16, #6	@ tmp154,,,
	str	r1, [r4, #4]	@ tmp154,
.L47:
	mov	r0, r7	@, ReadBytes
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
	.fnend
	.size	AIP_ReadFiFoData, .-AIP_ReadFiFoData
	.align	2
	.global	AIP_GetChnAttr
	.type	AIP_GetChnAttr, %function
AIP_GetChnAttr:
	.fnstart
.LFB803:
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
	movw	r3, #:lower16:.LANCHOR0	@ tmp136,
	add	r2, r0, #8	@ tmp137, Id,
	movt	r3, #:upper16:.LANCHOR0	@ tmp136,
	ldr	r3, [r3, r2, asl #2]	@ D.15527, g_stAipRm.hAip
	cmp	r3, #0	@ D.15527,
	ldmeqfd	sp, {fp, sp, pc}	@
	mov	r0, r1	@, pstAttr
	mov	r2, #120	@,
	mov	r1, r3	@, D.15527
	bl	memcpy	@
	ldmfd	sp, {fp, sp, pc}	@
	.fnend
	.size	AIP_GetChnAttr, .-AIP_GetChnAttr
	.align	2
	.global	AIPGetChnCmd
	.type	AIPGetChnCmd, %function
AIPGetChnCmd:
	.fnstart
.LFB789:
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
	movw	r3, #:lower16:.LANCHOR0	@ tmp140,
	movt	r3, #:upper16:.LANCHOR0	@ tmp140,
	ldr	r3, [r3, r0, asl #2]	@ pAipReg, g_stAipRm.hwAipReg
	ldr	r2, [r3, #8]	@ D.15330,
	tst	r2, #8	@ D.15330,
	movne	r0, #4	@ eNewCmd,
	bne	.L54	@,
	ldr	r0, [r3, #8]	@ tmp142,
	and	r0, r0, #3	@ D.15334, tmp142,
	cmp	r0, #2	@ D.15334,
	beq	.L54	@,
	cmp	r0, #3	@ D.15334,
	beq	.L54	@,
	cmp	r0, #1	@ D.15334,
	movne	r0, #0	@, eNewCmd,
.L54:
	str	r0, [r1, #0]	@ eNewCmd,* newcmd
	ldmfd	sp, {fp, sp, pc}	@
	.fnend
	.size	AIPGetChnCmd, .-AIPGetChnCmd
	.align	2
	.global	AIPSpeedAdjProc
	.type	AIPSpeedAdjProc, %function
AIPSpeedAdjProc:
	.fnstart
.LFB788:
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
	mov	r7, r0	@ state, state
	ldr	r0, [r0, #192]	@ D.15301, <variable>.hADJ
	mov	r4, r1	@ pstFrame, pstFrame
	cmp	r0, #0	@ D.15301,
	ldmeqfd	sp, {r4, r5, r6, r7, fp, sp, pc}	@
	bl	LSRC_CheckDoSRC	@
	cmp	r0, #0	@,
	ldmeqfd	sp, {r4, r5, r6, r7, fp, sp, pc}	@
	ldr	r6, [r4, #12]	@ uPcmSamplesPerFrame, <variable>.u32SamplesPerFrame
	bl	AOE_GetWKBuf	@
	ldr	r1, [r4, #0]	@, <variable>.pDataBuf
	mov	r3, r6	@, uPcmSamplesPerFrame
	mov	r5, r0	@ D.15308,
	ldr	r0, [r7, #192]	@, <variable>.hADJ
	mov	r2, r5	@, D.15308
	bl	LSRC_ProcessFrame	@
	ldr	r1, [r4, #16]	@, <variable>.u32BitPerSample
	str	r5, [r4, #0]	@ D.15308, <variable>.pDataBuf
	str	r0, [r4, #12]	@, <variable>.u32SamplesPerFrame
	ldr	r0, [r4, #20]	@, <variable>.u32Channels
	bl	UTIL_CalcFrameSize	@
	ldr	r1, [r4, #12]	@ <variable>.u32SamplesPerFrame, <variable>.u32SamplesPerFrame
	mul	r3, r1, r0	@ tmp147, <variable>.u32SamplesPerFrame,
	str	r3, [r4, #8]	@ tmp147, <variable>.u32DataBytes
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}	@
	.fnend
	.size	AIPSpeedAdjProc, .-AIPSpeedAdjProc
	.align	2
	.global	AIPCalcOutLatencyThdSize
	.type	AIPCalcOutLatencyThdSize, %function
AIPCalcOutLatencyThdSize:
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
	mov	r4, r0	@ state, state
	ldr	r1, [r4, #64]	@, <variable>.stAttr.u32OutBitPerSample
	ldr	r0, [r0, #68]	@, <variable>.stAttr.u32OutChannels
	bl	UTIL_CalcFrameSize	@
	ldr	r1, [r4, #72]	@ <variable>.stAttr.u32OutSampleRate, <variable>.stAttr.u32OutSampleRate
	ldr	ip, [r4, #80]	@ <variable>.stAttr.u32OutLatencyThdMs, <variable>.stAttr.u32OutLatencyThdMs
	movw	r3, #:lower16:274877907	@ tmp145,
	movt	r3, #:upper16:274877907	@ tmp145,
	mul	r2, r1, ip	@ tmp139, <variable>.stAttr.u32OutSampleRate, <variable>.stAttr.u32OutLatencyThdMs
	umull	r1, ip, r3, r2	@, tmp143, tmp145, tmp139
	mov	r3, ip, lsr #6	@ tmp142, tmp143,
	mul	r0, r3, r0	@, tmp142,
	ldmfd	sp, {r3, r4, fp, sp, pc}	@
	.fnend
	.size	AIPCalcOutLatencyThdSize, .-AIPCalcOutLatencyThdSize
	.align	2
	.global	AIPCalcProcBlockSize
	.type	AIPCalcProcBlockSize, %function
AIPCalcProcBlockSize:
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
	mov	r4, r0	@ state, state
	ldr	r1, [r4, #64]	@, <variable>.stAttr.u32OutBitPerSample
	ldr	r0, [r0, #68]	@, <variable>.stAttr.u32OutChannels
	bl	UTIL_CalcFrameSize	@
	ldr	r1, [r4, #72]	@ <variable>.stAttr.u32OutSampleRate, <variable>.stAttr.u32OutSampleRate
	movw	r2, #:lower16:274877907	@ tmp145,
	movt	r2, #:upper16:274877907	@ tmp145,
	add	r3, r1, r1, asl #2	@, tmp143, <variable>.stAttr.u32OutSampleRate, <variable>.stAttr.u32OutSampleRate,
	umull	r1, ip, r2, r3	@, tmp144, tmp145, tmp143
	mov	r3, ip, lsr #6	@ samples, tmp144,
	tst	r3, #3	@ samples,
	addne	r3, r3, #3	@ tmp147, samples,
	movne	r3, r3, lsr #2	@ D.14660, tmp147,
	movne	r3, r3, asl #2	@ samples, D.14660,
	mul	r0, r0, r3	@, framesize, samples
	ldmfd	sp, {r3, r4, fp, sp, pc}	@
	.fnend
	.size	AIPCalcProcBlockSize, .-AIPCalcProcBlockSize
	.align	2
	.global	AIPResamplePcmData
	.type	AIPResamplePcmData, %function
AIPResamplePcmData:
	.fnstart
.LFB787:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	.movsp ip
	mov	ip, sp	@,
	stmfd	sp!, {r4, r5, r6, fp, ip, lr, pc}	@,
	.pad #4
	.save {r4, r5, r6, fp, ip, lr}
	.setfp fp, ip, #-4
	sub	fp, ip, #4	@,,
	.pad #20
	sub	sp, sp, #20	@,,
	push	{lr}
	bl	__gnu_mcount_nc
	mov	r5, r0	@ state, state
	mov	r4, r1	@ pstFrame, pstFrame
	bl	AOE_GetWKBuf	@
	mov	r6, r0	@ D.15276,
	ldr	r0, [r5, #196]	@ D.15277, <variable>.hSRC
	ldr	r5, [r5, #72]	@ outRate, <variable>.stAttr.u32OutSampleRate
	cmp	r0, #0	@ D.15277,
	beq	.L70	@,
	ldr	ip, [r4, #24]	@ <variable>.u32SampleRate, <variable>.u32SampleRate
	mov	r2, r6	@, D.15276
	ldr	r3, [r4, #12]	@ <variable>.u32SamplesPerFrame, <variable>.u32SamplesPerFrame
	mov	lr, #1	@ tmp144,
	ldr	r1, [r4, #0]	@ <variable>.pDataBuf, <variable>.pDataBuf
	str	lr, [sp, #8]	@ tmp144,
	str	ip, [sp, #0]	@ <variable>.u32SampleRate,
	str	r5, [sp, #4]	@ outRate,
	bl	ResamplerProcessFrame	@
	ldr	r1, [r4, #16]	@, <variable>.u32BitPerSample
	str	r6, [r4, #0]	@ D.15276, <variable>.pDataBuf
	str	r5, [r4, #24]	@ outRate, <variable>.u32SampleRate
	str	r0, [r4, #12]	@, <variable>.u32SamplesPerFrame
	ldr	r0, [r4, #20]	@, <variable>.u32Channels
	bl	UTIL_CalcFrameSize	@
	ldr	r1, [r4, #12]	@ <variable>.u32SamplesPerFrame, <variable>.u32SamplesPerFrame
	mul	r3, r1, r0	@ tmp148, <variable>.u32SamplesPerFrame,
	str	r3, [r4, #8]	@ tmp148, <variable>.u32DataBytes
.L70:
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
	.fnend
	.size	AIPResamplePcmData, .-AIPResamplePcmData
	.align	2
	.global	AIPFadePcmData
	.type	AIPFadePcmData, %function
AIPFadePcmData:
	.fnstart
.LFB786:
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
	mov	r5, r0	@ state, state
	ldr	r0, [r0, #200]	@ D.15260, <variable>.hFADE
	mov	r4, r1	@ pstFrame, pstFrame
	cmp	r0, #0	@ D.15260,
	ldmeqfd	sp, {r4, r5, fp, sp, pc}	@
	ldr	r2, [r4, #16]	@, <variable>.u32BitPerSample
	ldr	r1, [r1, #24]	@, <variable>.u32SampleRate
	ldr	r3, [r4, #20]	@, <variable>.u32Channels
	bl	FADE_Flush	@
	ldr	r0, [r5, #200]	@, <variable>.hFADE
	ldr	r2, [r4, #12]	@, <variable>.u32SamplesPerFrame
	ldr	r1, [r4, #0]	@, <variable>.pDataBuf
	bl	FADE_Process	@
	ldmfd	sp, {r4, r5, fp, sp, pc}	@
	.fnend
	.size	AIPFadePcmData, .-AIPFadePcmData
	.global	__aeabi_uidiv
	.align	2
	.global	AIPFadeoutChnData
	.type	AIPFadeoutChnData, %function
AIPFadeoutChnData:
	.fnstart
.LFB775:
	@ args = 0, pretend = 0, frame = 40
	@ frame_needed = 1, uses_anonymous_args = 0
	.movsp ip
	mov	ip, sp	@,
	stmfd	sp!, {r4, r5, r6, fp, ip, lr, pc}	@,
	.pad #4
	.save {r4, r5, r6, fp, ip, lr}
	.setfp fp, ip, #-4
	sub	fp, ip, #4	@,,
	.pad #44
	sub	sp, sp, #44	@,,
	push	{lr}
	bl	__gnu_mcount_nc
	mov	r4, r0	@ state, state
	sub	r1, fp, #64	@,,
	ldr	r0, [r0, #188]	@, <variable>.hRingOut
	bl	RBUF_QueryInfo	@
	ldr	r6, [r4, #108]	@ D.14915, <variable>.stAttr.stRbfOutAttr.uFrameSize
	ldr	r0, [fp, #-48]	@, stRbfFifoInfo.u32ContiguousDataSize
	mov	r1, r6	@, D.14915
	bl	__aeabi_uidiv	@
	mov	r1, r6	@, D.14915
	mov	r5, r0	@ tmp141,
	ldr	r0, [fp, #-44]	@, stRbfFifoInfo.u32TotalDataSize
	bl	__aeabi_uidiv	@
	mov	r1, #2	@,
	mov	r2, #4	@,
	mov	r6, r0	@ tmp144,
	ldr	r0, [r4, #200]	@, <variable>.hFADE
	bl	FADE_Active	@
	ldr	r0, [r4, #200]	@, <variable>.hFADE
	ldr	r1, [r4, #72]	@, <variable>.stAttr.u32OutSampleRate
	ldr	r2, [r4, #64]	@, <variable>.stAttr.u32OutBitPerSample
	ldr	r3, [r4, #68]	@, <variable>.stAttr.u32OutChannels
	bl	FADE_Flush	@
	ldr	r3, [fp, #-64]	@ stRbfFifoInfo.u32BasePtr, stRbfFifoInfo.u32BasePtr
	ldr	r1, [fp, #-60]	@ stRbfFifoInfo.u32ReadPtr, stRbfFifoInfo.u32ReadPtr
	mov	r2, r5	@, tmp141
	ldr	r0, [r4, #200]	@, <variable>.hFADE
	add	r1, r1, r3	@, stRbfFifoInfo.u32ReadPtr, stRbfFifoInfo.u32BasePtr
	bl	FADE_Process	@
	rsb	r2, r5, r6	@, tmp141, tmp144
	ldr	r0, [r4, #200]	@, <variable>.hFADE
	ldr	r1, [fp, #-64]	@, stRbfFifoInfo.u32BasePtr
	bl	FADE_Process	@
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
	.fnend
	.size	AIPFadeoutChnData, .-AIPFadeoutChnData
	.align	2
	.global	AIPFlushChnFifo
	.type	AIPFlushChnFifo, %function
AIPFlushChnFifo:
	.fnstart
.LFB784:
	@ args = 0, pretend = 0, frame = 40
	@ frame_needed = 1, uses_anonymous_args = 0
	.movsp ip
	mov	ip, sp	@,
	stmfd	sp!, {r4, r5, r6, fp, ip, lr, pc}	@,
	.pad #4
	.save {r4, r5, r6, fp, ip, lr}
	.setfp fp, ip, #-4
	sub	fp, ip, #4	@,,
	.pad #44
	sub	sp, sp, #44	@,,
	push	{lr}
	bl	__gnu_mcount_nc
	mov	r4, r0	@ state, state
	bl	AOE_GetWKBuf	@
	ldr	r1, [r4, #1012]	@ <variable>.OutFrameSize, <variable>.OutFrameSize
	ldr	r3, [r4, #1008]	@ <variable>.OutBlockSize, <variable>.OutBlockSize
	adds	r1, r1, r3	@ __n, <variable>.OutFrameSize, <variable>.OutBlockSize
	mov	r6, r0	@ u8Data,
	bne	.L80	@,
.L77:
	sub	r5, fp, #64	@ tmp141,,
	ldr	r0, [r4, #188]	@, <variable>.hRingOut
	mov	r1, r5	@, tmp141
	bl	RBUF_QueryInfo	@
	ldr	r0, [r4, #188]	@ temp.858, <variable>.hRingOut
	cmp	r0, #0	@ temp.858,
	beq	.L79	@,
	mov	r1, r5	@, tmp141
	bl	RBUF_QueryInfo	@
	ldr	r0, [r4, #188]	@, <variable>.hRingOut
	ldr	r1, [fp, #-44]	@, stRbfFifoInfo.u32TotalDataSize
	bl	RBUF_ReadDataComplete	@
	ldr	r2, [r4, #1012]	@ <variable>.OutFrameSize, <variable>.OutFrameSize
	ldr	ip, [r4, #1008]	@ <variable>.OutBlockSize, <variable>.OutBlockSize
	mov	r1, r6	@, u8Data
	ldr	r0, [r4, #188]	@, <variable>.hRingOut
	add	r2, r2, ip	@, <variable>.OutFrameSize, <variable>.OutBlockSize
	bl	RBUF_WriteData	@
.L79:
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L80:
	bl	__memzero	@
	b	.L77	@
	.fnend
	.size	AIPFlushChnFifo, .-AIPFlushChnFifo
	.align	2
	.global	AIPDmaFrame2FiFo
	.type	AIPDmaFrame2FiFo, %function
AIPDmaFrame2FiFo:
	.fnstart
.LFB772:
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
	mov	r4, r0	@ state, state
	ldr	r6, [r4, #236]	@ <variable>.OutputLeftSamples, <variable>.OutputLeftSamples
	mov	r7, r1	@ pstFrame, pstFrame
	ldr	r0, [r0, #1012]	@ <variable>.OutFrameSize, <variable>.OutFrameSize
	ldr	r3, [r1, #8]	@ prephitmp.879, <variable>.u32DataBytes
	ldr	r5, [r4, #1008]	@ block_size, <variable>.OutBlockSize
	mul	r6, r6, r0	@ left_size, <variable>.OutputLeftSamples, <variable>.OutFrameSize
	add	r2, r6, r3	@ tmp141, left_size, prephitmp.879
	cmp	r5, r2	@ block_size, tmp141
	bhi	.L90	@,
.L82:
	rsb	r1, r5, r6	@ tmp147, block_size, left_size
	add	r3, r1, r3	@ tmp148, tmp147, prephitmp.879
	cmp	r3, #768	@ tmp148,
	bhi	.L91	@,
.L83:
	cmp	r6, #0	@ left_size,
	bne	.L92	@,
.L84:
	cmp	r5, #0	@ block_size,
	bne	.L93	@,
.L85:
	ldr	r2, [r7, #8]	@ <variable>.u32DataBytes, <variable>.u32DataBytes
	rsb	r6, r5, r2	@ left_size.892, block_size, <variable>.u32DataBytes
	cmp	r6, #768	@ left_size.892,
	bhi	.L94	@,
	cmp	r6, #0	@ left_size.892,
	bne	.L87	@,
.L88:
	mov	r0, r6	@, left_size.892
	ldr	r1, [r4, #1012]	@, <variable>.OutFrameSize
	bl	__aeabi_uidiv	@
	str	r0, [r4, #236]	@, <variable>.OutputLeftSamples
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L94:
	movw	r0, #:lower16:.LC0	@,
	movw	r1, #:lower16:.LC1	@,
	movw	r2, #:lower16:.LANCHOR1	@,
	movt	r0, #:upper16:.LC0	@,
	movt	r1, #:upper16:.LC1	@,
	movt	r2, #:upper16:.LANCHOR1	@,
	mov	r3, #444	@,
	movw	ip, #:lower16:.LC4	@ tmp162,
	movt	ip, #:upper16:.LC4	@ tmp162,
	str	ip, [sp, #0]	@ tmp162,
	bl	printk	@
.L87:
	ldr	r3, [r7, #0]	@ <variable>.pDataBuf, <variable>.pDataBuf
	add	r0, r4, #240	@, state,
	mov	r2, r6	@, left_size.892
	add	r1, r5, r3	@, block_size, <variable>.pDataBuf
	bl	memcpy	@
	b	.L88	@
.L93:
	ldr	r0, [r4, #188]	@, <variable>.hRingOut
	mov	r2, r5	@, block_size
	ldr	r1, [r7, #0]	@, <variable>.pDataBuf
	bl	RBUF_WriteData	@
	b	.L85	@
.L92:
	ldr	r0, [r4, #188]	@, <variable>.hRingOut
	add	r1, r4, #240	@, state,
	mov	r2, r6	@, left_size
	rsb	r5, r6, r5	@ block_size, left_size, block_size
	bl	RBUF_WriteData	@
	b	.L84	@
.L91:
	movw	r0, #:lower16:.LC0	@,
	movw	r1, #:lower16:.LC1	@,
	movw	r2, #:lower16:.LANCHOR1	@,
	movt	r1, #:upper16:.LC1	@,
	movt	r2, #:upper16:.LANCHOR1	@,
	movw	r3, #419	@,
	movt	r0, #:upper16:.LC0	@,
	movw	lr, #:lower16:.LC3	@ tmp153,
	movt	lr, #:upper16:.LC3	@ tmp153,
	str	lr, [sp, #0]	@ tmp153,
	bl	printk	@
	b	.L83	@
.L90:
	movw	r3, #418	@,
	movw	r0, #:lower16:.LC0	@,
	movw	r1, #:lower16:.LC1	@,
	movt	r0, #:upper16:.LC0	@,
	movt	r1, #:upper16:.LC1	@,
	movw	r2, #:lower16:.LANCHOR1	@,
	movt	r2, #:upper16:.LANCHOR1	@,
	movw	ip, #:lower16:.LC2	@ tmp146,
	movt	ip, #:upper16:.LC2	@ tmp146,
	str	ip, [sp, #0]	@ tmp146,
	bl	printk	@
	ldr	r3, [r7, #8]	@ prephitmp.879, <variable>.u32DataBytes
	b	.L82	@
	.fnend
	.size	AIPDmaFrame2FiFo, .-AIPDmaFrame2FiFo
	.align	2
	.global	AIPFlushChnCtx
	.type	AIPFlushChnCtx, %function
AIPFlushChnCtx:
	.fnstart
.LFB782:
	@ args = 0, pretend = 0, frame = 40
	@ frame_needed = 1, uses_anonymous_args = 0
	.movsp ip
	mov	ip, sp	@,
	stmfd	sp!, {r4, fp, ip, lr, pc}	@,
	.pad #4
	.save {r4, fp, ip, lr}
	.setfp fp, ip, #-4
	sub	fp, ip, #4	@,,
	.pad #44
	sub	sp, sp, #44	@,,
	push	{lr}
	bl	__gnu_mcount_nc
	mov	r4, r0	@ state, state
	ldr	r0, [r0, #196]	@ D.15197, <variable>.hSRC
	cmp	r0, #0	@ D.15197,
	bne	.L101	@,
.L96:
	ldr	r0, [r4, #192]	@ D.15200, <variable>.hADJ
	cmp	r0, #0	@ D.15200,
	bne	.L102	@,
.L97:
	ldr	r0, [r4, #200]	@ D.15203, <variable>.hFADE
	mov	r3, #0	@ tmp138,
	str	r3, [r4, #236]	@ tmp138, <variable>.OutputLeftSamples
	cmp	r0, r3	@ D.15203,
	bne	.L103	@,
.L98:
	ldr	r0, [r4, #1024]	@ <variable>.bAlsa, <variable>.bAlsa
	cmp	r0, #0	@ <variable>.bAlsa,
	beq	.L99	@,
	ldr	r0, [r4, #188]	@, <variable>.hRingOut
	sub	r1, fp, #56	@,,
	bl	RBUF_QueryInfo	@
	ldr	r0, [r4, #188]	@, <variable>.hRingOut
	ldr	r1, [fp, #-36]	@, stRbfFiFoInfo.u32TotalDataSize
	bl	RBUF_ReadDataComplete	@
.L99:
	ldr	r3, [r4, #1028]	@ <variable>.Id, <variable>.Id
	movw	ip, #:lower16:.LANCHOR0	@ tmp144,
	movt	ip, #:upper16:.LANCHOR0	@ tmp144,
	mov	r2, #0	@ movhi	@,
	ldr	r1, [ip, r3, asl #2]	@ pAipReg, g_stAipRm.hwAipReg
	strh	r2, [r1, #54]	@ movhi	@, <variable>.AIP_FIFO_SIZE.bits.aip_fifo_underflow_cnt
	sub	sp, fp, #16
	ldmfd	sp, {r4, fp, sp, pc}
.L103:
	mov	r1, #1	@,
	mov	r2, #8	@,
	bl	FADE_Active	@
	b	.L98	@
.L102:
	bl	LSRC_Flush	@
	b	.L97	@
.L101:
	bl	ResampleFlush	@
	b	.L96	@
	.fnend
	.size	AIPFlushChnCtx, .-AIPFlushChnCtx
	.align	2
	.global	AIPFlushChn
	.type	AIPFlushChn, %function
AIPFlushChn:
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
	tst	r1, #1	@ type,
	mov	r4, r1	@ type, type
	mov	r5, r0	@ state, state
	bne	.L109	@,
.L105:
	tst	r4, #2	@ type,
	bne	.L110	@,
	tst	r4, #4	@ type,
	ldmeqfd	sp, {r4, r5, fp, sp, pc}	@
.L111:
	mov	r0, r5	@, state
	bl	AIPFlushChnFifo	@
	ldmfd	sp, {r4, r5, fp, sp, pc}	@
.L110:
	mov	r0, r5	@, state
	bl	AIPFlushChnRufIn	@
	tst	r4, #4	@ type,
	ldmeqfd	sp, {r4, r5, fp, sp, pc}	@
	b	.L111	@
.L109:
	bl	AIPFlushChnCtx	@
	b	.L105	@
	.fnend
	.size	AIPFlushChn, .-AIPFlushChn
	.align	2
	.global	AIPPauseChnCmdProc
	.type	AIPPauseChnCmdProc, %function
AIPPauseChnCmdProc:
	.fnstart
.LFB793:
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
	movw	r4, #:lower16:.LANCHOR0	@ tmp146,
	add	r3, r0, #8	@ tmp138, Id,
	movt	r4, #:upper16:.LANCHOR0	@ tmp146,
	mov	r6, r0	@ Id, Id
	ldr	r5, [r4, r3, asl #2]	@ state, g_stAipRm.hAip
	cmp	r5, #0	@ state,
	ldmeqfd	sp, {r3, r4, r5, r6, fp, sp, pc}	@
	ldr	r1, [r5, #204]	@ <variable>.CurStatus, <variable>.CurStatus
	cmp	r1, #1	@ <variable>.CurStatus,
	beq	.L116	@,
	ldr	r3, [r4, r6, asl #2]	@ pAipReg, g_stAipRm.hwAipReg
	ldr	r2, [r3, #8]	@ D.15652,
	ands	r2, r2, #8	@ tmp142, D.15652,
	ldmnefd	sp, {r3, r4, r5, r6, fp, sp, pc}	@
.L117:
	ldr	r0, [r3, #8]	@ tmp143,
	bfi	r0, r2, #4, #4	@ tmp143, tmp142,,
	str	r0, [r3, #8]	@ tmp143,
	ldr	ip, [r3, #8]	@ tmp144,
	orr	r1, ip, #8	@ tmp147, tmp144,
	str	r1, [r3, #8]	@ tmp147,
	ldmfd	sp, {r3, r4, r5, r6, fp, sp, pc}	@
.L116:
	mov	r0, r5	@, state
	bl	AIPFlushChn	@
	mov	r0, #2	@ tmp140,
	str	r0, [r5, #204]	@ tmp140, <variable>.CurStatus
	ldr	r3, [r4, r6, asl #2]	@ pAipReg, g_stAipRm.hwAipReg
	ldr	r2, [r3, #8]	@ D.15652,
	ands	r2, r2, #8	@ tmp142, D.15652,
	beq	.L117	@,
	ldmfd	sp, {r3, r4, r5, r6, fp, sp, pc}	@
	.fnend
	.size	AIPPauseChnCmdProc, .-AIPPauseChnCmdProc
	.align	2
	.global	AIFlushChnCmdProc
	.type	AIFlushChnCmdProc, %function
AIFlushChnCmdProc:
	.fnstart
.LFB792:
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
	movw	r4, #:lower16:.LANCHOR0	@ tmp146,
	add	r3, r0, #8	@ tmp138, Id,
	movt	r4, #:upper16:.LANCHOR0	@ tmp146,
	mov	r5, r0	@ Id, Id
	ldr	r6, [r4, r3, asl #2]	@ state, g_stAipRm.hAip
	cmp	r6, #0	@ state,
	ldmeqfd	sp, {r4, r5, r6, r7, fp, sp, pc}	@
	ldr	r7, [r6, #204]	@ <variable>.CurStatus, <variable>.CurStatus
	cmp	r7, #1	@ <variable>.CurStatus,
	beq	.L122	@,
	ldr	r3, [r4, r5, asl #2]	@ pAipReg, g_stAipRm.hwAipReg
	ldr	r2, [r3, #8]	@ D.15659,
	ands	r2, r2, #8	@ tmp142, D.15659,
	ldmnefd	sp, {r4, r5, r6, r7, fp, sp, pc}	@
.L123:
	ldr	ip, [r3, #8]	@ tmp143,
	bfi	ip, r2, #4, #4	@ tmp143, tmp142,,
	str	ip, [r3, #8]	@ tmp143,
	ldr	r1, [r3, #8]	@ tmp144,
	orr	r0, r1, #8	@ tmp147, tmp144,
	str	r0, [r3, #8]	@ tmp147,
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}	@
.L122:
	mov	r0, r6	@, state
	mov	r1, #7	@,
	bl	AIPFlushChn	@
	mov	r2, #8	@,
	ldr	r0, [r6, #200]	@, <variable>.hFADE
	mov	r1, r7	@, <variable>.CurStatus
	bl	FADE_Active	@
	ldr	r3, [r4, r5, asl #2]	@ pAipReg, g_stAipRm.hwAipReg
	ldr	r2, [r3, #8]	@ D.15659,
	ands	r2, r2, #8	@ tmp142, D.15659,
	beq	.L123	@,
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}	@
	.fnend
	.size	AIFlushChnCmdProc, .-AIFlushChnCmdProc
	.align	2
	.global	AIPStopChnCmdProc
	.type	AIPStopChnCmdProc, %function
AIPStopChnCmdProc:
	.fnstart
.LFB791:
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
	movw	r4, #:lower16:.LANCHOR0	@ tmp147,
	add	r3, r0, #8	@ tmp138, Id,
	movt	r4, #:upper16:.LANCHOR0	@ tmp147,
	mov	r6, r0	@ Id, Id
	ldr	r5, [r4, r3, asl #2]	@ state, g_stAipRm.hAip
	cmp	r5, #0	@ state,
	ldmeqfd	sp, {r3, r4, r5, r6, fp, sp, pc}	@
	ldr	r1, [r5, #204]	@ <variable>.CurStatus, <variable>.CurStatus
	sub	r0, r1, #1	@ tmp139, <variable>.CurStatus,
	cmp	r0, #1	@ tmp139,
	bls	.L128	@,
	ldr	r3, [r4, r6, asl #2]	@ pAipReg, g_stAipRm.hwAipReg
	ldr	ip, [r3, #8]	@ D.15666,
	ands	r2, ip, #8	@ tmp143, D.15666,
	ldmnefd	sp, {r3, r4, r5, r6, fp, sp, pc}	@
.L129:
	ldr	ip, [r3, #8]	@ tmp144,
	bfi	ip, r2, #4, #4	@ tmp144, tmp143,,
	str	ip, [r3, #8]	@ tmp144,
	ldr	r1, [r3, #8]	@ tmp145,
	orr	r0, r1, #8	@ tmp148, tmp145,
	str	r0, [r3, #8]	@ tmp148,
	ldmfd	sp, {r3, r4, r5, r6, fp, sp, pc}	@
.L128:
	mov	r0, r5	@, state
	mov	r1, #7	@,
	bl	AIPFlushChn	@
	mov	r2, #0	@ tmp141,
	str	r2, [r5, #204]	@ tmp141, <variable>.CurStatus
	ldr	r3, [r4, r6, asl #2]	@ pAipReg, g_stAipRm.hwAipReg
	ldr	ip, [r3, #8]	@ D.15666,
	ands	r2, ip, #8	@ tmp143, D.15666,
	beq	.L129	@,
	ldmfd	sp, {r3, r4, r5, r6, fp, sp, pc}	@
	.fnend
	.size	AIPStopChnCmdProc, .-AIPStopChnCmdProc
	.align	2
	.global	AIPDestroyChn
	.type	AIPDestroyChn, %function
AIPDestroyChn:
	.fnstart
.LFB781:
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
	ldr	r0, [r4, #184]	@ D.15178, <variable>.hRingIn
	cmp	r0, #0	@ D.15178,
	bne	.L138	@,
	ldr	r0, [r4, #188]	@ D.15181, <variable>.hRingOut
	cmp	r0, #0	@ D.15181,
	bne	.L139	@,
.L133:
	ldr	r0, [r4, #200]	@ D.15184, <variable>.hFADE
	cmp	r0, #0	@ D.15184,
	bne	.L140	@,
.L134:
	ldr	r0, [r4, #192]	@ D.15187, <variable>.hADJ
	cmp	r0, #0	@ D.15187,
	bne	.L141	@,
.L135:
	ldr	r0, [r4, #196]	@ D.15190, <variable>.hSRC
	cmp	r0, #0	@ D.15190,
	bne	.L142	@,
.L136:
	mov	r1, r4	@, inst
	mov	r0, #1	@,
	bl	HI_FREE	@
	ldmfd	sp, {r3, r4, fp, sp, pc}	@
.L142:
	bl	ResamplerFree	@
	mov	r1, r4	@, inst
	mov	r0, #1	@,
	bl	HI_FREE	@
	ldmfd	sp, {r3, r4, fp, sp, pc}	@
.L138:
	bl	RBUF_Destroy	@
	ldr	r0, [r4, #188]	@ D.15181, <variable>.hRingOut
	cmp	r0, #0	@ D.15181,
	beq	.L133	@,
.L139:
	bl	RBUF_Destroy	@
	ldr	r0, [r4, #200]	@ D.15184, <variable>.hFADE
	cmp	r0, #0	@ D.15184,
	beq	.L134	@,
.L140:
	bl	FADE_Destroy	@
	ldr	r0, [r4, #192]	@ D.15187, <variable>.hADJ
	cmp	r0, #0	@ D.15187,
	beq	.L135	@,
.L141:
	bl	LSRC_Destroy	@
	ldr	r0, [r4, #196]	@ D.15190, <variable>.hSRC
	cmp	r0, #0	@ D.15190,
	beq	.L136	@,
	b	.L142	@
	.fnend
	.size	AIPDestroyChn, .-AIPDestroyChn
	.align	2
	.global	AIP_DeInit
	.type	AIP_DeInit, %function
AIP_DeInit:
	.fnstart
.LFB800:
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
	ldr	r0, [r4, #32]	@, g_stAipRm.hAip
	bl	AIPDestroyChn	@
	ldr	r0, [r4, #36]	@, g_stAipRm.hAip
	str	r5, [r4, #32]	@ tmp136, g_stAipRm.hAip
	bl	AIPDestroyChn	@
	ldr	r0, [r4, #40]	@, g_stAipRm.hAip
	str	r5, [r4, #36]	@ tmp136, g_stAipRm.hAip
	bl	AIPDestroyChn	@
	ldr	r0, [r4, #44]	@, g_stAipRm.hAip
	str	r5, [r4, #40]	@ tmp136, g_stAipRm.hAip
	bl	AIPDestroyChn	@
	ldr	r0, [r4, #48]	@, g_stAipRm.hAip
	str	r5, [r4, #44]	@ tmp136, g_stAipRm.hAip
	bl	AIPDestroyChn	@
	ldr	r0, [r4, #52]	@, g_stAipRm.hAip
	str	r5, [r4, #48]	@ tmp136, g_stAipRm.hAip
	bl	AIPDestroyChn	@
	ldr	r0, [r4, #56]	@, g_stAipRm.hAip
	str	r5, [r4, #52]	@ tmp136, g_stAipRm.hAip
	bl	AIPDestroyChn	@
	ldr	r0, [r4, #60]	@, g_stAipRm.hAip
	str	r5, [r4, #56]	@ tmp136, g_stAipRm.hAip
	bl	AIPDestroyChn	@
	str	r5, [r4, #60]	@ tmp136, g_stAipRm.hAip
	ldmfd	sp, {r4, r5, fp, sp, pc}	@
	.fnend
	.size	AIP_DeInit, .-AIP_DeInit
	.align	2
	.global	AIPGetRbfInAttr
	.type	AIPGetRbfInAttr, %function
AIPGetRbfInAttr:
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
	movw	r3, #:lower16:.LANCHOR0	@ tmp160,
	movt	r3, #:upper16:.LANCHOR0	@ tmp160,
	mov	r4, r1	@ pstChnAttr, pstChnAttr
	ldr	r5, [r3, r0, asl #2]	@ pAipReg, g_stAipRm.hwAipReg
	add	r0, r1, #8	@, pstChnAttr,
	ldr	r1, [r5, #0]	@ tmp161,* pAipReg
	and	r1, r1, #3	@, tmp161,
	bl	AOE_REG_ReadBitDepth	@
	cmp	r0, #0	@,
	beq	.L155	@,
.L146:
	mvn	r0, #0	@ D.14992,
	ldmfd	sp, {r4, r5, fp, sp, pc}	@
.L155:
	ldr	r2, [r5, #0]	@ tmp164,* pAipReg
	add	r0, r4, #12	@, pstChnAttr,
	ubfx	r1, r2, #2, #2	@, tmp164,,
	bl	AOE_REG_ReadChannel	@
	cmp	r0, #0	@,
	bne	.L146	@,
	ldr	ip, [r5, #0]	@ tmp167,* pAipReg
	add	r0, r4, #16	@, pstChnAttr,
	ubfx	r1, ip, #4, #4	@, tmp167,,
	bl	AOE_REG_ReadSampelRate	@
	cmp	r0, #0	@,
	beq	.L147	@,
	ldr	lr, [r5, #0]	@ D.15005,* pAipReg
	and	r0, lr, #240	@ tmp170, D.15005,
	cmp	r0, #240	@ tmp170,
	bne	.L146	@,
	ldr	r1, [r5, #12]	@ tmp171,
	movw	r3, #:lower16:188000	@ tmp174,
	movt	r3, #:upper16:188000	@ tmp174,
	ubfx	r1, r1, #0, #24	@ D.15009, tmp171,,
	str	r1, [r4, #16]	@ D.15009, <variable>.u32InSampleRate
	sub	r2, r1, #4000	@ tmp172, D.15009,
	cmp	r2, r3	@ tmp172, tmp174
	bhi	.L156	@,
	ldr	r1, [r4, #12]	@ D.15017, <variable>.u32InChannels
	cmp	r1, #2	@ D.15017,
	ldrhi	r2, [r4, #8]	@ prephitmp.1081, <variable>.u32InBitPerSample
	bhi	.L151	@,
	ldr	r2, [r4, #8]	@ prephitmp.1081, <variable>.u32InBitPerSample
	cmp	r2, #16	@ prephitmp.1081,
	beq	.L146	@,
.L151:
	movw	r0, #:lower16:.LC6	@,
	movt	r0, #:upper16:.LC6	@,
	bl	printk	@
	mvn	r0, #0	@ D.14992,
	ldmfd	sp, {r4, r5, fp, sp, pc}	@
.L147:
	ldr	r2, [r5, #0]	@ tmp177,* pAipReg
	add	r0, r4, #20	@, pstChnAttr,
	ubfx	r1, r2, #8, #2	@, tmp177,,
	bl	AOE_REG_ReadDataFormat	@
	cmp	r0, #0	@,
	bne	.L146	@,
	ldr	r1, [r4, #8]	@, <variable>.u32InBitPerSample
	ldr	r0, [r4, #12]	@, <variable>.u32InChannels
	bl	UTIL_CalcFrameSize	@
	mov	ip, r0	@ u32FrameSize,
	mov	r0, #1	@ tmp180,
	str	r0, [r4, #28]	@ tmp180, <variable>.stRbfInAttr.bExtMem
	ldr	r1, [r5, #16]	@ D.15027, <variable>.AIP_BUF_ADDR
	str	r1, [r4, #32]	@ D.15027, <variable>.stRbfInAttr.uBufAddr
	ldr	r2, [r5, #20]	@ tmp181,
	ubfx	r3, r2, #0, #24	@ D.15028, tmp181,,
	str	r3, [r4, #36]	@ D.15028, <variable>.stRbfInAttr.uBufSize
	ldr	r0, [r5, #20]	@ D.15030,
	mov	r3, #0	@ tmp185,
	ldr	r1, [r4, #36]	@ <variable>.stRbfInAttr.uBufSize, <variable>.stRbfInAttr.uBufSize
	tst	r0, #16777216	@ D.15030,
	addeq	r0, r5, #24	@ D.15037, pAipReg,
	streq	r0, [r4, #40]	@ D.15037, <variable>.stRbfInAttr.uBufWptr
	ldrne	r0, [r5, #24]	@ D.15034, <variable>.AIP_BUF_WPTR
	addeq	r0, r5, #28	@ D.15039, pAipReg,
	strne	r0, [r4, #40]	@ D.15034, <variable>.stRbfInAttr.uBufWptr
	ldrne	r0, [r5, #28]	@ D.15035, <variable>.AIP_BUF_RPTR
	str	r0, [r4, #44]	@ D.15039, <variable>.stRbfInAttr.uBufRptr
	mov	r0, r3	@ D.14992, tmp185
	ldr	r2, [r5, #20]	@ tmp183,
	str	ip, [r4, #52]	@ u32FrameSize, <variable>.stRbfInAttr.uFrameSize
	mov	ip, #8	@ tmp188,
	str	r1, [r4, #56]	@ <variable>.stRbfInAttr.uBufSize, <variable>.stRbfInAttr.uLatencySize
	ubfx	r2, r2, #24, #1	@ D.15041, tmp183,,
	mov	r1, #16	@ tmp187,
	str	r2, [r4, #48]	@ D.15041, <variable>.stRbfInAttr.uBufFlag
	stmia	r4, {r1, ip}	@ phole stm
	str	r3, [r4, #24]	@ tmp185, <variable>.u32InPeriondSize
	str	r3, [r4, #60]	@ tmp185, <variable>.stRbfInAttr.uLatencyMs
	ldmfd	sp, {r4, r5, fp, sp, pc}	@
.L156:
	movw	r0, #:lower16:.LC5	@,
	movt	r0, #:upper16:.LC5	@,
	bl	printk	@
	mvn	r0, #0	@ D.14992,
	ldmfd	sp, {r4, r5, fp, sp, pc}	@
	.fnend
	.size	AIPGetRbfInAttr, .-AIPGetRbfInAttr
	.align	2
	.global	AIPGetRbfFiFoAttr
	.type	AIPGetRbfFiFoAttr, %function
AIPGetRbfFiFoAttr:
	.fnstart
.LFB776:
	@ args = 0, pretend = 0, frame = 8
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
	movw	r3, #:lower16:.LANCHOR0	@ tmp152,
	movt	r3, #:upper16:.LANCHOR0	@ tmp152,
	mov	r4, r1	@ pstChnAttr, pstChnAttr
	ldr	r5, [r3, r0, asl #2]	@ pAipReg, g_stAipRm.hwAipReg
	add	r0, r1, #64	@, pstChnAttr,
	ldr	r1, [r5, #4]	@ tmp153,
	and	r1, r1, #3	@, tmp153,
	bl	AOE_REG_ReadBitDepth	@
	cmp	r0, #0	@,
	beq	.L161	@,
.L158:
	mvn	r0, #0	@ D.14944,
.L159:
	sub	sp, fp, #28
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}
.L161:
	ldr	r2, [r5, #4]	@ tmp156,
	add	r0, r4, #68	@, pstChnAttr,
	ubfx	r1, r2, #2, #2	@, tmp156,,
	bl	AOE_REG_ReadChannel	@
	cmp	r0, #0	@,
	bne	.L158	@,
	ldr	ip, [r5, #4]	@ tmp159,
	add	r0, r4, #72	@, pstChnAttr,
	ubfx	r1, ip, #4, #4	@, tmp159,,
	bl	AOE_REG_ReadSampelRate	@
	cmp	r0, #0	@,
	bne	.L158	@,
	ldr	lr, [r5, #4]	@ tmp162,
	add	r0, r4, #76	@, pstChnAttr,
	ubfx	r1, lr, #8, #2	@, tmp162,,
	bl	AOE_REG_ReadDataFormat	@
	cmp	r0, #0	@,
	bne	.L158	@,
	ldr	r6, [r5, #4]	@ tmp165,
	sub	r0, fp, #32	@,,
	ubfx	r1, r6, #10, #6	@, tmp165,,
	bl	AOE_REG_ReadBufLatencyMs	@
	subs	r6, r0, #0	@ D.14965,
	bne	.L158	@,
	ldr	r1, [r4, #64]	@, <variable>.u32OutBitPerSample
	ldr	r0, [r4, #68]	@, <variable>.u32OutChannels
	bl	UTIL_CalcFrameSize	@
	ldr	r2, [r4, #72]	@, <variable>.u32OutSampleRate
	mov	r1, r0	@, u32FrameSize
	mov	r7, r0	@ u32FrameSize,
	ldr	r0, [fp, #-32]	@, uLatencyMs
	bl	UTIL_LatencyMs2ByteSize	@
	ldr	r3, [fp, #-32]	@ uLatencyMs, uLatencyMs
	add	r1, r5, #60	@ D.14976, pAipReg,
	str	r7, [r4, #108]	@ u32FrameSize, <variable>.stRbfOutAttr.uFrameSize
	add	r5, r5, #56	@ D.14974, pAipReg,
	str	r1, [r4, #100]	@ D.14976, <variable>.stRbfOutAttr.uBufRptr
	str	r3, [r4, #80]	@ uLatencyMs, <variable>.u32OutLatencyThdMs
	str	r5, [r4, #96]	@ D.14974, <variable>.stRbfOutAttr.uBufWptr
	str	r6, [r4, #84]	@ D.14965, <variable>.stRbfOutAttr.bExtMem
	str	r6, [r4, #104]	@ D.14965, <variable>.stRbfOutAttr.uBufFlag
	str	r3, [r4, #116]	@ uLatencyMs, <variable>.stRbfOutAttr.uLatencyMs
	mov	r2, r0	@ uLatencySize,
	mov	r0, r6	@ D.14944, D.14965
	str	r2, [r4, #112]	@ uLatencySize, <variable>.stRbfOutAttr.uLatencySize
	str	r2, [r4, #92]	@ uLatencySize, <variable>.stRbfOutAttr.uBufSize
	b	.L159	@
	.fnend
	.size	AIPGetRbfFiFoAttr, .-AIPGetRbfFiFoAttr
	.align	2
	.global	AIPGetChnCreateAttr
	.type	AIPGetChnCreateAttr, %function
AIPGetChnCreateAttr:
	.fnstart
.LFB778:
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
	mov	r4, r1	@ pstChnAttr, pstChnAttr
	bl	AIPGetRbfInAttr	@
	cmp	r0, #0	@,
	bne	.L167	@,
	mov	r0, r5	@, Id
	mov	r1, r4	@, pstChnAttr
	bl	AIPGetRbfFiFoAttr	@
	cmp	r0, #0	@ Ret.1141
	ldmeqfd	sp, {r4, r5, fp, sp, pc}	@
	ldr	r1, .L168	@,
	movw	r2, #687	@,
	movw	r0, #:lower16:.LC7	@,
	movt	r0, #:upper16:.LC7	@,
	bl	printk	@
	mvn	r0, #0	@ D.15053,
	ldmfd	sp, {r4, r5, fp, sp, pc}	@
.L167:
	movw	r0, #:lower16:.LC7	@,
	ldr	r1, .L168	@,
	movt	r0, #:upper16:.LC7	@,
	movw	r2, #685	@,
	bl	printk	@
	mvn	r0, #0	@ D.15053,
	ldmfd	sp, {r4, r5, fp, sp, pc}	@
.L169:
	.align	2
.L168:
	.word	.LANCHOR1+20
	.fnend
	.size	AIPGetChnCreateAttr, .-AIPGetChnCreateAttr
	.align	2
	.global	AIPCreateChn
	.type	AIPCreateChn, %function
AIPCreateChn:
	.fnstart
.LFB780:
	@ args = 0, pretend = 0, frame = 176
	@ frame_needed = 1, uses_anonymous_args = 0
	.movsp ip
	mov	ip, sp	@,
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, ip, lr, pc}	@,
	.pad #4
	.save {r4, r5, r6, r7, r8, r9, sl, fp, ip, lr}
	.setfp fp, ip, #-4
	sub	fp, ip, #4	@,,
	.pad #188
	sub	sp, sp, #188	@,,
	push	{lr}
	bl	__gnu_mcount_nc
	sub	r5, fp, #216	@ tmp211,,
	movw	r4, #:lower16:.LANCHOR0	@ tmp210,
	movt	r4, #:upper16:.LANCHOR0	@ tmp210,
	mov	r1, r5	@, tmp211
	mov	r8, #0	@ tmp153,
	mov	r6, r0	@ Id, Id
	str	r8, [fp, #-48]	@ tmp153, hADJ
	str	r8, [fp, #-52]	@ tmp153, hFade
	str	r8, [fp, #-56]	@ tmp153, hRingIn
	str	r8, [fp, #-60]	@ tmp153, hRingOut
	ldr	sl, [r4, r0, asl #2]	@ pAipReg, g_stAipRm.hwAipReg
	bl	AIPGetChnCreateAttr	@
	cmp	r0, r8	@,
	bne	.L181	@,
	ldr	r7, [sl, #0]	@ D.15116,* pAipReg
	ands	r7, r7, #1024	@ tmp159, D.15116,
	bne	.L182	@,
	sub	r0, fp, #56	@,,
	add	r1, r5, #28	@, tmp211,
	bl	RBUF_Create	@
.L174:
	ldr	r3, [fp, #-56]	@ hRingIn, hRingIn
	cmp	r3, #0	@ hRingIn,
	beq	.L175	@,
	sub	r0, fp, #60	@,,
	add	r1, r5, #84	@, tmp211,
	bl	RBUF_Create	@
	ldr	r0, [fp, #-60]	@ hRingOut.262, hRingOut
	cmp	r0, #0	@ hRingOut.262,
	bne	.L183	@,
.L175:
	mov	r0, #0	@,
	bl	AIPDestroyChn	@
	mov	r0, #0	@ D.15115,
.L172:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, sl, fp, sp, pc}
.L182:
	sub	r0, fp, #56	@,,
	add	r1, r5, #28	@, tmp211,
	bl	RBUF_Create_EXT	@
	mov	r7, #1	@ bAlsa,
	b	.L174	@
.L183:
	sub	r1, fp, #96	@,,
	bl	RBUF_QueryInfo	@
	ldr	r1, [fp, #-96]	@ D.15127, stRbfOut.u32BasePtr
	str	r1, [sl, #48]	@ D.15127, <variable>.AIP_FIFO_ADDR
	ldrh	r0, [fp, #-84]	@ D.15129, stRbfOut.u32BufSize
	strh	r0, [sl, #52]	@ movhi	@ D.15129, <variable>.AIP_FIFO_SIZE.bits.aip_fifo_size
	ldr	sl, [fp, #-196]	@ stAttr.u32InDataFormat, stAttr.u32InDataFormat
	cmp	sl, #0	@ stAttr.u32InDataFormat,
	movne	sl, #0	@ hSRC,
	bne	.L177	@,
	ldr	r0, [fp, #-200]	@ prephitmp.1168, stAttr.u32InSampleRate
	ldr	r1, [fp, #-144]	@ D.15134, stAttr.u32OutSampleRate
	cmp	r0, r1	@ prephitmp.1168, D.15134
	beq	.L179	@,
	ldr	r2, [fp, #-148]	@, stAttr.u32OutChannels
	ldr	r3, [fp, #-152]	@, stAttr.u32OutBitPerSample
	str	sl, [sp, #0]	@ stAttr.u32InDataFormat,
	bl	ResamplerCreate	@
	subs	sl, r0, #0	@ hSRC,
	beq	.L175	@,
	ldr	r0, [fp, #-144]	@ prephitmp.1168, stAttr.u32OutSampleRate
.L179:
	ldr	ip, [fp, #-148]	@ stAttr.u32OutChannels, stAttr.u32OutChannels
	mov	r1, r0	@, prephitmp.1168
	mov	r2, r1	@,
	sub	r0, fp, #48	@,,
	ldr	r3, [fp, #-152]	@, stAttr.u32OutBitPerSample
	str	ip, [sp, #0]	@ stAttr.u32OutChannels,
	bl	LSRC_Create	@
	ldr	r2, [fp, #-48]	@ hADJ, hADJ
	cmp	r2, #0	@ hADJ,
	beq	.L175	@,
	ldr	r3, [fp, #-204]	@, stAttr.u32InChannels
	sub	r0, fp, #52	@,,
	ldr	r1, [fp, #-200]	@, stAttr.u32InSampleRate
	ldr	r2, [fp, #-208]	@, stAttr.u32InBitPerSample
	bl	FADE_Create	@
	ldr	r3, [fp, #-52]	@ hFade, hFade
	cmp	r3, #0	@ hFade,
	beq	.L175	@,
.L177:
	mov	r0, #1	@,
	movw	r1, #1032	@,
	bl	HI_MALLOC	@
	subs	r8, r0, #0	@ D.15153,
	beq	.L175	@,
	movw	r1, #1032	@,
	mov	r9, #1	@ tmp198,
	bl	__memzero	@
	mov	r1, r5	@, tmp211
	mov	r2, #120	@,
	mov	r0, r8	@, D.15153
	bl	memcpy	@
	str	r7, [r8, #1024]	@ bAlsa, <variable>.bAlsa
	str	r6, [r8, #1028]	@ Id, <variable>.Id
	add	r5, r8, #120	@ tmp191, D.15153,
	ldr	ip, [r4, r6, asl #2]	@ tmp190, g_stAipRm.hwAipReg
	mov	lr, r5	@ tmp212, tmp191
	ldmia	ip!, {r0, r1, r2, r3}	@ tmp192,,,,
	stmia	lr!, {r0, r1, r2, r3}	@ tmp212,,,,
	mov	r4, lr	@ tmp214, tmp212
	ldmia	ip!, {r0, r1, r2, r3}	@ tmp213,,,,
	stmia	r4!, {r0, r1, r2, r3}	@ tmp214,,,,
	mov	lr, r4	@ tmp216, tmp214
	ldmia	ip!, {r0, r1, r2, r3}	@ tmp215,,,,
	stmia	lr!, {r0, r1, r2, r3}	@ tmp216,,,,
	ldmia	ip, {r0, r1, r2, r3}	@ tmp215,,,,
	mov	ip, #100	@ movhi	@,
	stmia	lr, {r0, r1, r2, r3}	@ tmp216,,,,
	ldr	r1, [fp, #-56]	@ hRingIn, hRingIn
	mov	r2, #0	@ tmp197,
	mov	r0, r8	@, D.15153
	str	r1, [r8, #184]	@ hRingIn, <variable>.hRingIn
	mvn	r1, #32768	@ tmp202,
	ldr	lr, [fp, #-60]	@ hRingOut, hRingOut
	str	sl, [r8, #196]	@ hSRC, <variable>.hSRC
	str	lr, [r8, #188]	@ hRingOut, <variable>.hRingOut
	ldr	r3, [fp, #-48]	@ hADJ, hADJ
	str	r3, [r8, #192]	@ hADJ, <variable>.hADJ
	ldr	lr, [fp, #-52]	@ hFade, hFade
	str	r2, [r8, #228]	@ tmp197, <variable>.s32FadeState
	strh	ip, [r8, #212]	@ movhi	@, <variable>.stGain.u16Gain
	str	lr, [r8, #200]	@ hFade, <variable>.hFADE
	strh	r1, [r8, #216]	@ movhi	@ tmp202, <variable>.s16LinearGainFrac
	str	r9, [r8, #208]	@ tmp198, <variable>.stGain.bLinearMode
	bl	AIPCalcProcBlockSize	@
	str	r0, [r8, #1008]	@, <variable>.OutBlockSize
	mov	r0, r8	@, D.15153
	bl	AIPCalcOutLatencyThdSize	@
	ldr	r1, [r8, #64]	@, <variable>.stAttr.u32OutBitPerSample
	str	r0, [r8, #1020]	@, <variable>.OutLatencySize
	ldr	r0, [r8, #68]	@, <variable>.stAttr.u32OutChannels
	bl	UTIL_CalcFrameSize	@
	ldr	r1, [r8, #8]	@, <variable>.stAttr.u32InBitPerSample
	str	r0, [r8, #1012]	@, <variable>.OutFrameSize
	ldr	r0, [r8, #12]	@, <variable>.stAttr.u32InChannels
	bl	UTIL_CalcFrameSize	@
	str	r9, [r8, #204]	@ tmp198, <variable>.CurStatus
	mov	r1, r9	@, tmp198
	mov	r2, #8	@,
	str	r0, [r8, #1016]	@, <variable>.InFrameSize
	ldr	r0, [r8, #200]	@, <variable>.hFADE
	bl	FADE_Active	@
.L181:
	mov	r0, r8	@ D.15115, D.15153
	b	.L172	@
	.fnend
	.size	AIPCreateChn, .-AIPCreateChn
	.align	2
	.global	AIPStartChnCmdProc
	.type	AIPStartChnCmdProc, %function
AIPStartChnCmdProc:
	.fnstart
.LFB794:
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
	add	r6, r0, #8	@ tmp143, Id,
	movt	r4, #:upper16:.LANCHOR0	@ tmp170,
	mov	r5, r0	@ Id, Id
	ldr	r7, [r4, r6, asl #2]	@ prephitmp.1202, g_stAipRm.hAip
	cmp	r7, #0	@ prephitmp.1202,
	beq	.L192	@,
	ldr	r3, [r7, #204]	@ D.15396, <variable>.CurStatus
	cmp	r3, #0	@ D.15396,
	beq	.L194	@,
	cmp	r3, #2	@ D.15396,
	beq	.L193	@,
.L186:
	cmp	r7, #0	@ prephitmp.1202,
	bne	.L189	@,
	ldr	r3, [r4, r5, asl #2]	@ pAipReg, g_stAipRm.hwAipReg
	ldr	r2, [r3, #8]	@ D.15673,
	tst	r2, #8	@ D.15673,
	ldmnefd	sp, {r4, r5, r6, r7, fp, sp, pc}	@
	ldr	ip, [r3, #8]	@ tmp161,
	mov	r2, #1	@ tmp162,
	bfi	ip, r2, #4, #4	@ tmp161, tmp162,,
	str	ip, [r3, #8]	@ tmp161,
	ldr	r1, [r3, #8]	@ tmp163,
	orr	r0, r1, #8	@ tmp171, tmp163,
	str	r0, [r3, #8]	@ tmp171,
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}	@
.L189:
	ldr	r3, [r4, r5, asl #2]	@ pAipReg, g_stAipRm.hwAipReg
	ldr	r0, [r3, #8]	@ D.15680,
	ands	r2, r0, #8	@ tmp166, D.15680,
	ldmnefd	sp, {r4, r5, r6, r7, fp, sp, pc}	@
	ldr	r0, [r3, #8]	@ tmp167,
	bfi	r0, r2, #4, #4	@ tmp167, tmp166,,
	str	r0, [r3, #8]	@ tmp167,
	ldr	ip, [r3, #8]	@ tmp168,
	orr	r1, ip, #8	@ tmp172, tmp168,
	str	r1, [r3, #8]	@ tmp172,
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}	@
.L194:
	bl	AIPIsChnAttrChange	@
	cmp	r0, #0	@,
	beq	.L193	@,
	ldr	r0, [r4, r6, asl #2]	@, g_stAipRm.hAip
	bl	AIPDestroyChn	@
	mov	r0, r5	@, Id
.L192:
	bl	AIPCreateChn	@
	mov	r7, r0	@ prephitmp.1202,
	str	r0, [r4, r6, asl #2]	@ prephitmp.1202, g_stAipRm.hAip
	b	.L186	@
.L193:
	mov	r1, #1	@ tmp155,
	ldr	r0, [r7, #200]	@, <variable>.hFADE
	str	r1, [r7, #204]	@ tmp155, <variable>.CurStatus
	mov	r2, #8	@,
	bl	FADE_Active	@
	ldr	r7, [r4, r6, asl #2]	@ prephitmp.1202, g_stAipRm.hAip
	b	.L186	@
	.fnend
	.size	AIPStartChnCmdProc, .-AIPStartChnCmdProc
	.align	2
	.global	AIPProcChnCmd
	.type	AIPProcChnCmd, %function
AIPProcChnCmd:
	.fnstart
.LFB795:
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
	bl	AIPGetChnCmd	@
	cmp	r0, #4	@,
	beq	.L202	@,
	ldr	r3, [fp, #-24]	@ newcmd, newcmd
	cmp	r3, #3	@ newcmd,
	ldrls	pc, [pc, r3, asl #2]	@ newcmd
	b	.L202	@
.L201:
	.word	.L197
	.word	.L198
	.word	.L199
	.word	.L200
.L198:
	mov	r0, r4	@, Id
	bl	AIPStopChnCmdProc	@
.L202:
	sub	sp, fp, #16
	ldmfd	sp, {r4, fp, sp, pc}
.L200:
	mov	r0, r4	@, Id
	bl	AIFlushChnCmdProc	@
	b	.L202	@
.L197:
	mov	r0, r4	@, Id
	bl	AIPStartChnCmdProc	@
	b	.L202	@
.L199:
	mov	r0, r4	@, Id
	bl	AIPPauseChnCmdProc	@
	b	.L202	@
	.fnend
	.size	AIPProcChnCmd, .-AIPProcChnCmd
	.align	2
	.global	AIPCalcRbfInNeedBytes
	.type	AIPCalcRbfInNeedBytes, %function
AIPCalcRbfInNeedBytes:
	.fnstart
.LFB770:
	@ args = 0, pretend = 0, frame = 40
	@ frame_needed = 1, uses_anonymous_args = 0
	.movsp ip
	mov	ip, sp	@,
	stmfd	sp!, {r4, r5, r6, r7, r8, fp, ip, lr, pc}	@,
	.pad #4
	.save {r4, r5, r6, r7, r8, fp, ip, lr}
	.setfp fp, ip, #-4
	sub	fp, ip, #4	@,,
	.pad #52
	sub	sp, sp, #52	@,,
	push	{lr}
	bl	__gnu_mcount_nc
	ldr	r3, [r0, #20]	@ <variable>.stAttr.u32InDataFormat, <variable>.stAttr.u32InDataFormat
	ldr	r6, [r0, #68]	@ Channel, <variable>.stAttr.u32OutChannels
	mov	r4, r0	@ state, state
	cmp	r3, #0	@ <variable>.stAttr.u32InDataFormat,
	mov	r5, r2	@ pNeedInBytes, pNeedInBytes
	ldr	r8, [r0, #72]	@ SampleRate, <variable>.stAttr.u32OutSampleRate
	ldr	r7, [r0, #64]	@ BitWidth, <variable>.stAttr.u32OutBitPerSample
	movne	r6, r1	@ NeedInBytes, blocksize
	beq	.L212	@,
.L205:
	ldr	r0, [r4, #184]	@, <variable>.hRingIn
	sub	r1, fp, #72	@,,
	bl	RBUF_QueryInfo	@
	ldr	r1, [fp, #-52]	@ stRbfIn.u32TotalDataSize, stRbfIn.u32TotalDataSize
	cmp	r6, r1	@ NeedInBytes, stRbfIn.u32TotalDataSize
	strcc	r6, [r5, #0]	@ NeedInBytes,* pNeedInBytes
	movcs	r6, #0	@ NeedInBytes,
	mov	r0, r6	@, NeedInBytes
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
.L212:
	mov	r0, r1	@, blocksize
	ldr	r1, [r4, #1012]	@, <variable>.OutFrameSize
	bl	__aeabi_uidiv	@
	ldr	ip, [r4, #192]	@ <variable>.hADJ, <variable>.hADJ
	ldr	lr, [r4, #236]	@ <variable>.OutputLeftSamples, <variable>.OutputLeftSamples
	mov	r3, r7	@, BitWidth
	mov	r2, r8	@, tmp1
	mov	r1, r8	@, SampleRate
	str	r6, [sp, #0]	@ Channel,
	rsb	r6, lr, r0	@ NeedSampels.1260, <variable>.OutputLeftSamples,
	mov	r0, ip	@, <variable>.hADJ
	bl	LSRC_UpdateChange	@
	ldr	r0, [r4, #192]	@, <variable>.hADJ
	ldr	r1, [r4, #232]	@, <variable>.s32UserSpeedAdjust
	bl	LSRC_SetSpeedAdjust	@
	mov	r1, r6	@, NeedSampels.1260
	ldr	r0, [r4, #192]	@, <variable>.hADJ
	bl	LSRC_GetMinInputNum	@
	mov	r1, r0	@ D.14726,
	ldr	r0, [r4, #196]	@ D.14736, <variable>.hSRC
	cmp	r0, #0	@ D.14736,
	bne	.L213	@,
.L207:
	tst	r1, #3	@ NeedSampels,
	ldr	r0, [r4, #1016]	@ <variable>.InFrameSize, <variable>.InFrameSize
	addne	r1, r1, #3	@ tmp159, NeedSampels,
	bicne	r1, r1, #3	@ NeedSampels, tmp159,
	mul	r6, r0, r1	@ NeedInBytes, <variable>.InFrameSize, NeedSampels
	b	.L205	@
.L213:
	bl	ResamplerGetMinInputNum	@
	mov	r1, r0	@ NeedSampels,
	b	.L207	@
	.fnend
	.size	AIPCalcRbfInNeedBytes, .-AIPCalcRbfInNeedBytes
	.align	2
	.global	AIPDmaFrame
	.type	AIPDmaFrame, %function
AIPDmaFrame:
	.fnstart
.LFB771:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	.movsp ip
	mov	ip, sp	@,
	stmfd	sp!, {r4, r5, r6, fp, ip, lr, pc}	@,
	.pad #4
	.save {r4, r5, r6, fp, ip, lr}
	.setfp fp, ip, #-4
	sub	fp, ip, #4	@,,
	.pad #20
	sub	sp, sp, #20	@,,
	push	{lr}
	bl	__gnu_mcount_nc
	ldr	r6, [r0, #1008]	@ blocksize, <variable>.OutBlockSize
	mov	r5, r1	@ pstInFrame, pstInFrame
	mov	r3, #0	@ tmp143,
	mov	r4, r0	@ state, state
	str	r3, [fp, #-32]	@ tmp143, needsize
	mov	r1, r6	@, blocksize
	bl	AIPCheckRbfOutFree	@
	cmp	r0, #0	@,
	bne	.L219	@,
.L215:
	mov	r0, #0	@ D.14782,
.L217:
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L219:
	mov	r1, r6	@, blocksize
	mov	r0, r4	@, state
	sub	r2, fp, #32	@,,
	bl	AIPCalcRbfInNeedBytes	@
	cmp	r0, #0	@,
	beq	.L215	@,
	bl	AOE_GetWKBuf	@
	cmp	r0, #0	@ uWkAddr
	beq	.L215	@,
	str	r0, [r5, #0]	@ uWkAddr, <variable>.pDataBuf
	bl	AOE_GetWKBufSize	@
	ldr	r6, [fp, #-32]	@ needsize, needsize
	stmib	r5, {r0, r6}	@ phole stm
	mov	r0, r6	@, needsize
	ldr	r1, [r4, #1016]	@, <variable>.InFrameSize
	bl	__aeabi_uidiv	@
	ldr	r1, [r5, #0]	@, <variable>.pDataBuf
	mov	r2, r6	@, needsize
	str	r0, [r5, #12]	@, <variable>.u32SamplesPerFrame
	ldr	r0, [r4, #12]	@ <variable>.stAttr.u32InChannels, <variable>.stAttr.u32InChannels
	str	r0, [r5, #20]	@ <variable>.stAttr.u32InChannels, <variable>.u32Channels
	ldr	r3, [r4, #8]	@ <variable>.stAttr.u32InBitPerSample, <variable>.stAttr.u32InBitPerSample
	str	r3, [r5, #16]	@ <variable>.stAttr.u32InBitPerSample, <variable>.u32BitPerSample
	ldr	ip, [r4, #16]	@ <variable>.stAttr.u32InSampleRate, <variable>.stAttr.u32InSampleRate
	str	ip, [r5, #24]	@ <variable>.stAttr.u32InSampleRate, <variable>.u32SampleRate
	ldr	r0, [r4, #20]	@ <variable>.stAttr.u32InDataFormat, <variable>.stAttr.u32InDataFormat
	str	r0, [r5, #28]	@ <variable>.stAttr.u32InDataFormat, <variable>.u32DataFormat
	ldr	r0, [r4, #184]	@, <variable>.hRingIn
	bl	RBUF_ReadData	@
	ldr	r1, [fp, #-32]	@ needsize, needsize
	cmp	r0, r1	@ readsize, needsize
	beq	.L217	@,
	movw	r0, #:lower16:.LC0	@,
	movw	r1, #:lower16:.LC1	@,
	movt	r0, #:upper16:.LC0	@,
	movt	r1, #:upper16:.LC1	@,
	ldr	r2, .L220	@,
	movw	r3, #409	@,
	movw	lr, #:lower16:.LC8	@ tmp163,
	movt	lr, #:upper16:.LC8	@ tmp163,
	str	lr, [sp, #0]	@ tmp163,
	bl	printk	@
	ldr	r0, [fp, #-32]	@ readsize, needsize
	b	.L217	@
.L221:
	.align	2
.L220:
	.word	.LANCHOR1+40
	.fnend
	.size	AIPDmaFrame, .-AIPDmaFrame
	.align	2
	.global	AIPGainPcmData
	.type	AIPGainPcmData, %function
AIPGainPcmData:
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
	ldr	r3, [r0, #208]	@ bLinearMode, <variable>.stGain.bLinearMode
	mov	r2, r0	@ state, state
	cmp	r3, #1	@ bLinearMode,
	ldreqh	r2, [r0, #216]	@ Volume, <variable>.s16LinearGainFrac
	mov	r0, r1	@, pstFrame
	ldrneh	r2, [r2, #212]	@ Volume, <variable>.stGain.u16Gain
	mov	r1, r3	@, bLinearMode
	bl	UTIL_GainProcess	@
	ldmfd	sp, {fp, sp, pc}	@
	.fnend
	.size	AIPGainPcmData, .-AIPGainPcmData
	.align	2
	.global	AIPUnifyPcmData
	.type	AIPUnifyPcmData, %function
AIPUnifyPcmData:
	.fnstart
.LFB765:
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
	ldr	r6, [r0, #68]	@ dstCh, <variable>.stAttr.u32OutChannels
	ldr	r3, [r1, #20]	@ D.14622, <variable>.u32Channels
	mov	r4, r1	@ pstFrame, pstFrame
	ldr	r5, [r0, #64]	@ dstBitDepth, <variable>.stAttr.u32OutBitPerSample
	cmp	r3, r6	@ D.14622, dstCh
	beq	.L227	@,
	cmp	r6, r3	@ dstCh, D.14622
	bls	.L228	@,
	bl	AOE_GetWKBuf	@
	mov	r2, r4	@, pstFrame
	mov	r1, r0	@ D.14627,
	mov	r0, r6	@, dstCh
	bl	UTIL_UpmixPcmData	@
.L227:
	ldr	r3, [r4, #16]	@ <variable>.u32BitPerSample, <variable>.u32BitPerSample
	cmp	r3, r5	@ <variable>.u32BitPerSample, dstBitDepth
	ldmeqfd	sp, {r3, r4, r5, r6, fp, sp, pc}	@
.L231:
	bl	AOE_GetWKBuf	@
	mov	r2, r4	@, pstFrame
	mov	r1, r0	@ D.14633,
	mov	r0, r5	@, dstBitDepth
	bl	UTIL_UnifyBitDepthPcmData	@
	ldmfd	sp, {r3, r4, r5, r6, fp, sp, pc}	@
.L228:
	bl	AOE_GetWKBuf	@
	mov	r2, r4	@, pstFrame
	mov	r1, r0	@ D.14629,
	mov	r0, r6	@, dstCh
	bl	UTIL_DownmixPcmData	@
	ldr	r3, [r4, #16]	@ <variable>.u32BitPerSample, <variable>.u32BitPerSample
	cmp	r3, r5	@ <variable>.u32BitPerSample, dstBitDepth
	bne	.L231	@,
	ldmfd	sp, {r3, r4, r5, r6, fp, sp, pc}	@
	.fnend
	.size	AIPUnifyPcmData, .-AIPUnifyPcmData
	.align	2
	.global	AIPProcFrame
	.type	AIPProcFrame, %function
AIPProcFrame:
	.fnstart
.LFB773:
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
	ldr	r3, [r0, #20]	@ <variable>.stAttr.u32InDataFormat, <variable>.stAttr.u32InDataFormat
	mov	r4, r0	@ state, state
	mov	r5, r1	@ pstInFrame, pstInFrame
	cmp	r3, #0	@ <variable>.stAttr.u32InDataFormat,
	beq	.L236	@,
.L233:
	mov	r0, r4	@, state
	mov	r1, r5	@, pstInFrame
	bl	AIPDmaFrame2FiFo	@
	ldmfd	sp, {r4, r5, fp, sp, pc}	@
.L236:
	bl	AIPUnifyPcmData	@
	mov	r0, r4	@, state
	mov	r1, r5	@, pstInFrame
	bl	AIPGainPcmData	@
	mov	r0, r4	@, state
	mov	r1, r5	@, pstInFrame
	bl	AIPFadePcmData	@
	ldr	r0, [r4, #196]	@ <variable>.hSRC, <variable>.hSRC
	cmp	r0, #0	@ <variable>.hSRC,
	bne	.L237	@,
.L234:
	ldr	r1, [r4, #192]	@ <variable>.hADJ, <variable>.hADJ
	cmp	r1, #0	@ <variable>.hADJ,
	beq	.L233	@,
	mov	r0, r4	@, state
	mov	r1, r5	@, pstInFrame
	bl	AIPSpeedAdjProc	@
	mov	r0, r4	@, state
	mov	r1, r5	@, pstInFrame
	bl	AIPDmaFrame2FiFo	@
	ldmfd	sp, {r4, r5, fp, sp, pc}	@
.L237:
	mov	r0, r4	@, state
	mov	r1, r5	@, pstInFrame
	bl	AIPResamplePcmData	@
	b	.L234	@
	.fnend
	.size	AIPProcFrame, .-AIPProcFrame
	.align	2
	.global	AIPProcChnData
	.type	AIPProcChnData, %function
AIPProcChnData:
	.fnstart
.LFB774:
	@ args = 0, pretend = 0, frame = 32
	@ frame_needed = 1, uses_anonymous_args = 0
	.movsp ip
	mov	ip, sp	@,
	stmfd	sp!, {r4, r5, fp, ip, lr, pc}	@,
	.pad #4
	.save {r4, r5, fp, ip, lr}
	.setfp fp, ip, #-4
	sub	fp, ip, #4	@,,
	.pad #32
	sub	sp, sp, #32	@,,
	push	{lr}
	bl	__gnu_mcount_nc
	sub	r4, fp, #52	@ tmp135,,
	mov	r5, r0	@ state, state
	bl	AOE_FlushWKBuf	@
	mov	r0, r5	@, state
	mov	r1, r4	@, tmp135
	bl	AIPDmaFrame	@
	cmp	r0, #0	@,
	beq	.L239	@,
	mov	r0, r5	@, state
	mov	r1, r4	@, tmp135
	bl	AIPProcFrame	@
.L239:
	bl	AOE_FlushWKBuf	@
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, pc}
	.fnend
	.size	AIPProcChnData, .-AIPProcChnData
	.align	2
	.global	AIPProcChn
	.type	AIPProcChn, %function
AIPProcChn:
	.fnstart
.LFB797:
	@ args = 0, pretend = 0, frame = 40
	@ frame_needed = 1, uses_anonymous_args = 0
	.movsp ip
	mov	ip, sp	@,
	stmfd	sp!, {r4, r5, r6, fp, ip, lr, pc}	@,
	.pad #4
	.save {r4, r5, r6, fp, ip, lr}
	.setfp fp, ip, #-4
	sub	fp, ip, #4	@,,
	.pad #44
	sub	sp, sp, #44	@,,
	push	{lr}
	bl	__gnu_mcount_nc
	mov	r5, r0	@ Id, Id
	bl	AIPProcChnCmd	@
	movw	r3, #:lower16:.LANCHOR0	@ tmp172,
	add	r0, r5, #8	@ tmp173, Id,
	movt	r3, #:upper16:.LANCHOR0	@ tmp172,
	ldr	r2, [r3, r0, asl #2]	@ state, g_stAipRm.hAip
	ldr	r1, [r3, r5, asl #2]	@ pAipReg, g_stAipRm.hwAipReg
	cmp	r2, #0	@ state,
	beq	.L242	@,
	mov	lr, #0	@ tmp149,
	str	lr, [r2, #208]	@ tmp149, <variable>.stGain.bLinearMode
	ldr	r6, [r1, #8]	@ tmp150,
	ubfx	r4, r6, #8, #7	@ D.15694, tmp150,,
	strh	r4, [r2, #212]	@ movhi	@ D.15694, <variable>.stGain.u16Gain
	ldr	ip, [r1, #8]	@ D.15692,
	tst	ip, #1610612736	@ D.15692,
	beq	.L246	@,
	ldr	r6, [r1, #8]	@ D.15688,
	and	r4, r6, #1610612736	@ tmp155, D.15688,
	cmp	r4, #536870912	@ tmp155,
	ldreq	r1, [r1, #8]	@ tmp156,
	ubfxeq	r1, r1, #24, #5	@ D.15686, tmp156,,
	rsbeq	r1, r1, #0	@ tmp158, D.15686
	streq	r1, [r2, #232]	@ tmp158, <variable>.s32UserSpeedAdjust
.L244:
	ldr	r4, [r3, r0, asl #2]	@ state, g_stAipRm.hAip
	movw	r6, #:lower16:.LANCHOR0	@ tmp159,
	movt	r6, #:upper16:.LANCHOR0	@ tmp159,
	cmp	r4, #0	@ state,
	beq	.L242	@,
	ldr	r3, [r4, #204]	@ <variable>.CurStatus, <variable>.CurStatus
	cmp	r3, #1	@ <variable>.CurStatus,
	beq	.L247	@,
.L242:
	mov	r0, #0	@,
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L246:
	ldr	ip, [r1, #8]	@ tmp153,
	ubfx	r1, ip, #24, #5	@ D.15690, tmp153,,
	str	r1, [r2, #232]	@ D.15690, <variable>.s32UserSpeedAdjust
	b	.L244	@
.L247:
	mov	r0, r4	@, state
	bl	AIPProcChnData	@
	ldr	r0, [r4, #188]	@, <variable>.hRingOut
	sub	r1, fp, #64	@,,
	ldr	r5, [r6, r5, asl #2]	@ pAipReg, g_stAipRm.hwAipReg
	bl	RBUF_QueryInfo	@
	ldr	r2, [r4, #72]	@, <variable>.stAttr.u32OutSampleRate
	ldr	r0, [fp, #-44]	@, stBufInfo.u32TotalDataSize
	ldr	r1, [r4, #1012]	@, <variable>.OutFrameSize
	bl	UTIL_LatencyMs	@
	ldr	r2, [r5, #4]	@ tmp170,
	bfi	r2, r0, #16, #6	@ tmp170,,,
	str	r2, [r5, #4]	@ tmp170,
	b	.L242	@
	.fnend
	.size	AIPProcChn, .-AIPProcChn
	.align	2
	.global	AIP_Process
	.type	AIP_Process, %function
AIP_Process:
	.fnstart
.LFB801:
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
	bl	AIPProcChn	@
	mov	r0, #1	@,
	bl	AIPProcChn	@
	mov	r0, #2	@,
	bl	AIPProcChn	@
	mov	r0, #3	@,
	bl	AIPProcChn	@
	mov	r0, #4	@,
	bl	AIPProcChn	@
	mov	r0, #5	@,
	bl	AIPProcChn	@
	mov	r0, #6	@,
	bl	AIPProcChn	@
	mov	r0, #7	@,
	bl	AIPProcChn	@
	ldmfd	sp, {fp, sp, pc}	@
	.fnend
	.size	AIP_Process, .-AIP_Process
	.section	.rodata
	.align	2
.LANCHOR1 = . + 0
	.type	__func__.14820, %object
	.size	__func__.14820, 17
__func__.14820:
	.ascii	"AIPDmaFrame2FiFo\000"
	.space	3
	.type	__func__.15049, %object
	.size	__func__.15049, 20
__func__.15049:
	.ascii	"AIPGetChnCreateAttr\000"
	.type	__func__.14778, %object
	.size	__func__.14778, 12
__func__.14778:
	.ascii	"AIPDmaFrame\000"
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
.LC0:
	.ascii	"\012ASSERT failed at:\012  >File name: %s\012  >Fun"
	.ascii	"ction : %s\012  >Line No. : %d\012  >Condition: %s\012"
	.ascii	"\000"
.LC1:
	.ascii	"/home/release/HiSTBLinuxV100R002C00SPC011/source/ke"
	.ascii	"rnel/linux-3.4.y/drivers/msp/adsp/adsp_v1_1/./hifi/"
	.ascii	"aoe/src/aip/aoe_aip.c\000"
.LC2:
	.ascii	"(pstFrame->u32DataBytes + left_size) >= block_size\000"
	.space	1
.LC3:
	.ascii	"(pstFrame->u32DataBytes + left_size - block_size) <"
	.ascii	"= AIP_MAX_BYTES_LEFT\000"
.LC4:
	.ascii	"left_size <= AIP_MAX_BYTES_LEFT\000"
.LC5:
	.ascii	"invalid fs_ext(%d)\012\000"
.LC6:
	.ascii	"fs_ext invalid ch(%d) or bitdepth(%d)\012\000"
	.space	1
.LC7:
	.ascii	"\012<%s %d>: ASSERT Failure{HI_SUCCESS == Ret}\012\000"
	.space	3
.LC8:
	.ascii	"readsize == needsize\000"
	.bss
	.align	2
.LANCHOR0 = . + 0
	.type	g_stAipRm, %object
	.size	g_stAipRm, 64
g_stAipRm:
	.space	64
	.ident	"GCC: (Hisilicon_v200(gcc4.4-290+glibc-2.11+eabi+nptl)) 4.4.1"
	.section	.note.GNU-stack,"",%progbits

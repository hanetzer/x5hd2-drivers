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
	.file	"aoe_reg.c"
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
@ -DKBUILD_STR(s)=#s -DKBUILD_BASENAME=KBUILD_STR(aoe_reg)
@ -DKBUILD_MODNAME=KBUILD_STR(hi_adsp) -isystem
@ /opt/hisi-linux/x86-arm/arm-hisiv200-linux/bin/../lib/gcc/arm-hisiv200-linux-gnueabi/4.4.1/include
@ -include
@ /home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/include/linux/kconfig.h
@ -MD drivers/msp/adsp/adsp_v1_1/./hifi/aoe/src/.aoe_reg.o.d
@ /home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/drivers/msp/adsp/adsp_v1_1/./hifi/aoe/src/aoe_reg.c
@ -mlittle-endian -marm -mapcs -mno-sched-prolog -mabi=aapcs-linux
@ -mno-thumb-interwork -march=armv7-a -msoft-float -auxbase-strip
@ drivers/msp/adsp/adsp_v1_1/./hifi/aoe/src/aoe_reg.o -O2 -Wall -Wundef
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
	.global	AOE_REG_ReadDataFormat
	.type	AOE_REG_ReadDataFormat, %function
AOE_REG_ReadDataFormat:
	.fnstart
.LFB117:
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
	str	r1, [r0, #0]	@ format,* pu32DataFormat
	mov	r0, #0	@,
	ldmfd	sp, {fp, sp, pc}	@
	.fnend
	.size	AOE_REG_ReadDataFormat, .-AOE_REG_ReadDataFormat
	.align	2
	.global	AOE_REG_ReadBufLatencyMs
	.type	AOE_REG_ReadBufLatencyMs, %function
AOE_REG_ReadBufLatencyMs:
	.fnstart
.LFB118:
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
	str	r1, [r0, #0]	@ latency,* pu32LatencyMs
	mov	r0, #0	@,
	ldmfd	sp, {fp, sp, pc}	@
	.fnend
	.size	AOE_REG_ReadBufLatencyMs, .-AOE_REG_ReadBufLatencyMs
	.align	2
	.global	AOE_REG_ReadBufDelayMs
	.type	AOE_REG_ReadBufDelayMs, %function
AOE_REG_ReadBufDelayMs:
	.fnstart
.LFB119:
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
	str	r1, [r0, #0]	@ delay,* pu32DelayMs
	mov	r0, #0	@,
	ldmfd	sp, {fp, sp, pc}	@
	.fnend
	.size	AOE_REG_ReadBufDelayMs, .-AOE_REG_ReadBufDelayMs
	.align	2
	.global	AOE_REG_ReadSampelRate
	.type	AOE_REG_ReadSampelRate, %function
AOE_REG_ReadSampelRate:
	.fnstart
.LFB116:
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
	cmp	r1, #12	@ rate,
	mov	r3, r1	@ rate, rate
	mov	r2, r0	@ pu32SampelRate, pu32SampelRate
	bhi	.L8	@,
	movw	r1, #:lower16:.LANCHOR0	@ tmp137,
	movt	r1, #:upper16:.LANCHOR0	@ tmp137,
	mov	r0, #0	@ D.3749,
	ldr	r3, [r1, r3, asl #2]	@ tmp138, g_u32AoeSampleRateTab
	str	r3, [r2, #0]	@ tmp138,* pu32SampelRate
	ldmfd	sp, {fp, sp, pc}	@
.L8:
	movw	r0, #:lower16:.LC0	@,
	movt	r0, #:upper16:.LC0	@,
	bl	printk	@
	mvn	r0, #0	@ D.3749,
	ldmfd	sp, {fp, sp, pc}	@
	.fnend
	.size	AOE_REG_ReadSampelRate, .-AOE_REG_ReadSampelRate
	.align	2
	.global	AOE_REG_ReadChannel
	.type	AOE_REG_ReadChannel, %function
AOE_REG_ReadChannel:
	.fnstart
.LFB115:
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
	cmp	r1, #1	@ ch,
	mov	r3, r1	@ ch, ch
	moveq	r3, #2	@ tmp138,
	streq	r3, [r0, #0]	@ tmp138,* pu32Channels
	moveq	r0, #0	@ D.3740,
	ldmeqfd	sp, {fp, sp, pc}	@
	movcc	r3, #1	@ tmp137,
	strcc	r3, [r0, #0]	@ tmp137,* pu32Channels
	movcc	r0, #0	@ D.3740,
	ldmccfd	sp, {fp, sp, pc}	@
	cmp	r3, #3	@ ch,
	beq	.L20	@,
	mov	r2, #0	@ tmp140,
	str	r2, [r0, #0]	@ tmp140,* pu32Channels
	movw	r0, #:lower16:.LC1	@,
	movt	r0, #:upper16:.LC1	@,
	bl	printk	@
	mvn	r0, #0	@ D.3740,
	ldmfd	sp, {fp, sp, pc}	@
.L20:
	mov	r3, #8	@ tmp139,
	str	r3, [r0, #0]	@ tmp139,* pu32Channels
	mov	r0, #0	@ D.3740,
	ldmfd	sp, {fp, sp, pc}	@
	.fnend
	.size	AOE_REG_ReadChannel, .-AOE_REG_ReadChannel
	.align	2
	.global	AOE_REG_ReadBitDepth
	.type	AOE_REG_ReadBitDepth, %function
AOE_REG_ReadBitDepth:
	.fnstart
.LFB114:
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
	cmp	r1, #1	@ precision,
	mov	r3, r1	@ precision, precision
	moveq	r3, #16	@ tmp138,
	streq	r3, [r0, #0]	@ tmp138,* pu32BitPerSample
	moveq	r0, #0	@ D.3730,
	ldmeqfd	sp, {fp, sp, pc}	@
	movcc	r3, #8	@ tmp137,
	strcc	r3, [r0, #0]	@ tmp137,* pu32BitPerSample
	movcc	r0, #0	@ D.3730,
	ldmccfd	sp, {fp, sp, pc}	@
	cmp	r3, #2	@ precision,
	beq	.L29	@,
	mov	r2, #0	@ tmp140,
	str	r2, [r0, #0]	@ tmp140,* pu32BitPerSample
	movw	r0, #:lower16:.LC2	@,
	movt	r0, #:upper16:.LC2	@,
	bl	printk	@
	mvn	r0, #0	@ D.3730,
	ldmfd	sp, {fp, sp, pc}	@
.L29:
	mov	r3, #24	@ tmp139,
	str	r3, [r0, #0]	@ tmp139,* pu32BitPerSample
	mov	r0, #0	@ D.3730,
	ldmfd	sp, {fp, sp, pc}	@
	.fnend
	.size	AOE_REG_ReadBitDepth, .-AOE_REG_ReadBitDepth
	.section	.rodata
	.align	2
.LANCHOR0 = . + 0
	.type	g_u32AoeSampleRateTab, %object
	.size	g_u32AoeSampleRateTab, 52
g_u32AoeSampleRateTab:
	.word	8000
	.word	11025
	.word	12000
	.word	16000
	.word	22050
	.word	24000
	.word	32000
	.word	44100
	.word	48000
	.word	88200
	.word	96000
	.word	176400
	.word	192000
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
.LC0:
	.ascii	"\012invalid rate(%d)\012\000"
	.space	1
.LC1:
	.ascii	"invalid ch(%d)\012\000"
.LC2:
	.ascii	"invalid precision(%d)\012\000"
	.ident	"GCC: (Hisilicon_v200(gcc4.4-290+glibc-2.11+eabi+nptl)) 4.4.1"
	.section	.note.GNU-stack,"",%progbits

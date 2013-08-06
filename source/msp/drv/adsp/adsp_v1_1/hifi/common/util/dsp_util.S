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
	.file	"dsp_util.c"
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
@ -DKBUILD_STR(s)=#s -DKBUILD_BASENAME=KBUILD_STR(dsp_util)
@ -DKBUILD_MODNAME=KBUILD_STR(hi_adsp) -isystem
@ /opt/hisi-linux/x86-arm/arm-hisiv200-linux/bin/../lib/gcc/arm-hisiv200-linux-gnueabi/4.4.1/include
@ -include
@ /home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/include/linux/kconfig.h
@ -MD drivers/msp/adsp/adsp_v1_1/./hifi/common/util/.dsp_util.o.d
@ /home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/drivers/msp/adsp/adsp_v1_1/./hifi/common/util/dsp_util.c
@ -mlittle-endian -marm -mapcs -mno-sched-prolog -mabi=aapcs-linux
@ -mno-thumb-interwork -march=armv7-a -msoft-float -auxbase-strip
@ drivers/msp/adsp/adsp_v1_1/./hifi/common/util/dsp_util.o -O2 -Wall
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
	.global	UTIL_CalcFrameSize
	.type	UTIL_CalcFrameSize, %function
UTIL_CalcFrameSize:
	.fnstart
.LFB795:
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
	cmp	r1, #16	@ uBitDepth,
	moveq	r0, r0, asl #1	@ uFrameSize, uCh,
	ldmeqfd	sp, {fp, sp, pc}	@
	cmp	r1, #24	@ uBitDepth,
	moveq	r0, r0, asl #2	@ uFrameSize, uCh,
	movne	r0, #0	@ uFrameSize,
	ldmfd	sp, {fp, sp, pc}	@
	.fnend
	.size	UTIL_CalcFrameSize, .-UTIL_CalcFrameSize
	.align	2
	.global	UTIL_LatencyMs2ByteSize
	.type	UTIL_LatencyMs2ByteSize, %function
UTIL_LatencyMs2ByteSize:
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
	mul	ip, r0, r1	@ tmp138, u32LatencyMs, u32FrameSize
	movw	r0, #:lower16:274877907	@ tmp142,
	movt	r0, #:upper16:274877907	@ tmp142,
	mul	r1, r2, ip	@ tmp139, u32SampleRate, tmp138
	umull	r2, r3, r0, r1	@, tmp140, tmp142, tmp139
	mov	r0, r3, lsr #6	@, tmp140,
	ldmfd	sp, {fp, sp, pc}	@
	.fnend
	.size	UTIL_LatencyMs2ByteSize, .-UTIL_LatencyMs2ByteSize
	.global	__aeabi_uidiv
	.align	2
	.global	UTIL_LatencyMs
	.type	UTIL_LatencyMs, %function
UTIL_LatencyMs:
	.fnstart
.LFB797:
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
	mov	r3, #1000	@ tmp139,
	mul	r0, r3, r0	@, tmp139, u32DataBytes
	mul	r1, r1, r2	@, u32FrameSize, u32SampleRate
	bl	__aeabi_uidiv	@
	ldmfd	sp, {fp, sp, pc}	@
	.fnend
	.size	UTIL_LatencyMs, .-UTIL_LatencyMs
	.align	2
	.global	UTIL_UpmixPcmData
	.type	UTIL_UpmixPcmData, %function
UTIL_UpmixPcmData:
	.fnstart
.LFB800:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	.movsp ip
	mov	ip, sp	@,
	stmfd	sp!, {r3, r4, r5, r6, r7, r8, fp, ip, lr, pc}	@,
	.pad #4
	.save {r3, r4, r5, r6, r7, r8, fp, ip, lr}
	.setfp fp, ip, #-4
	sub	fp, ip, #4	@,,
	push	{lr}
	bl	__gnu_mcount_nc
	ldr	r3, [r2, #16]	@ <variable>.u32BitPerSample, <variable>.u32BitPerSample
	cmp	r3, #24	@ <variable>.u32BitPerSample,
	beq	.L74	@,
	cmp	r3, #16	@ <variable>.u32BitPerSample,
	ldmnefd	sp, {r3, r4, r5, r6, r7, r8, fp, sp, pc}	@
	ldr	ip, [r2, #12]	@ <variable>.u32SamplesPerFrame, <variable>.u32SamplesPerFrame
	ldr	r4, [r2, #0]	@ temp.342, <variable>.pDataBuf
	cmp	ip, #0	@ <variable>.u32SamplesPerFrame,
	movne	r5, r1	@ pu16Dst, WkBuf
	movne	r7, #0	@ i.346,
	beq	.L25	@,
.L26:
	cmp	r0, #0	@ dstCh,
	beq	.L30	@,
	ldr	r3, [r2, #20]	@ <variable>.u32Channels, <variable>.u32Channels
	add	ip, r5, #2	@ ivtmp.336, pu16Dst,
	sub	r6, r0, #1	@ tmp243, dstCh,
	cmp	r3, #0	@ <variable>.u32Channels,
	and	r6, r6, #1	@ tmp245, tmp243,
	movls	r3, #0	@ movhi	@,
	movls	r5, ip	@ pu16Dst, ivtmp.336
	strlsh	r3, [ip, #-2]	@ movhi	@,
	ldrhih	r5, [r4], #2	@,
	strhih	r5, [ip, #-2]	@ movhi	@,
	movhi	r5, ip	@ pu16Dst, ivtmp.336
.L65:
	mov	r3, #1	@ ch.345,
	cmp	r0, r3	@ dstCh, ch.345
	add	ip, ip, #2	@ ivtmp.336, ivtmp.336,
	bls	.L66	@,
	cmp	r6, #0	@ tmp245,
	beq	.L29	@,
	ldr	r5, [r2, #20]	@ <variable>.u32Channels, <variable>.u32Channels
	cmp	r5, r3	@ <variable>.u32Channels, ch.345
	movls	r6, #0	@ movhi	@,
	movls	r5, ip	@ pu16Dst, ivtmp.336
	strlsh	r6, [ip, #-2]	@ movhi	@,
	bls	.L69	@,
	b	.L58	@
.L29:
	ldr	r5, [r2, #20]	@ <variable>.u32Channels, <variable>.u32Channels
	cmp	r5, r3	@ <variable>.u32Channels, ch.345
	add	r3, r3, #1	@ tmp246, ch.345,
	ldrhih	r5, [r4], #2	@,
	movls	r6, #0	@ movhi	@,
	strlsh	r6, [ip, #-2]	@ movhi	@,
	strhih	r5, [ip, #-2]	@ movhi	@,
	add	ip, ip, #2	@ tmp247, ivtmp.336,
	ldr	r6, [r2, #20]	@ <variable>.u32Channels, <variable>.u32Channels
	mov	r5, ip	@ pu16Dst, tmp247
	cmp	r6, r3	@ <variable>.u32Channels, tmp246
	movls	r6, #0	@ movhi	@,
	strlsh	r6, [ip, #-2]	@ movhi	@,
	bls	.L69	@,
.L58:
	ldrh	r5, [r4], #2	@,
	strh	r5, [ip, #-2]	@ movhi	@,
	mov	r5, ip	@ pu16Dst, tmp247
.L69:
	add	r3, r3, #1	@ ch.345, tmp246,
	add	ip, ip, #2	@ ivtmp.336, tmp247,
	cmp	r0, r3	@ dstCh, ch.345
	bhi	.L29	@,
.L66:
	ldr	ip, [r2, #12]	@ <variable>.u32SamplesPerFrame, <variable>.u32SamplesPerFrame
.L30:
	add	r7, r7, #1	@ i.346, i.346,
	cmp	ip, r7	@ <variable>.u32SamplesPerFrame, i.346
	bhi	.L26	@,
.L73:
	ldr	r3, [r2, #16]	@ <variable>.u32BitPerSample, <variable>.u32BitPerSample
.L25:
	cmp	r3, #16	@ <variable>.u32BitPerSample,
	str	r0, [r2, #20]	@ dstCh, <variable>.u32Channels
	moveq	ip, ip, asl #1	@ tmp228, <variable>.u32SamplesPerFrame,
	str	r1, [r2, #0]	@ WkBuf, <variable>.pDataBuf
	muleq	r0, r0, ip	@ prephitmp.350, dstCh, tmp228
	beq	.L34	@,
	cmp	r3, #24	@ <variable>.u32BitPerSample,
	movne	r0, #0	@ prephitmp.350,
	moveq	ip, ip, asl #2	@ tmp230, <variable>.u32SamplesPerFrame,
	muleq	r0, r0, ip	@ prephitmp.350, dstCh, tmp230
.L34:
	str	r0, [r2, #8]	@ prephitmp.350, <variable>.u32DataBytes
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, fp, sp, pc}	@
.L74:
	ldr	ip, [r2, #12]	@ <variable>.u32SamplesPerFrame, <variable>.u32SamplesPerFrame
	ldr	r4, [r2, #0]	@ D.14424, <variable>.pDataBuf
	cmp	ip, #0	@ <variable>.u32SamplesPerFrame,
	movne	r7, #0	@ i,
	movne	r5, r1	@ pu32Dst, WkBuf
	movne	r6, r7	@ ch, i
	beq	.L25	@,
.L15:
	cmp	r0, #0	@ dstCh,
	beq	.L19	@,
	ldr	r3, [r2, #20]	@ <variable>.u32Channels, <variable>.u32Channels
	add	ip, r5, #4	@ ivtmp.328, pu32Dst,
	sub	r8, r0, #1	@ tmp238, dstCh,
	cmp	r3, #0	@ <variable>.u32Channels,
	and	r8, r8, #1	@ tmp240, tmp238,
	strls	r6, [ip, #-4]	@ ch,
	movls	r5, ip	@ pu32Dst, ivtmp.328
	ldrhi	r3, [r4], #4	@ tmp266,
	movhi	r5, ip	@ pu32Dst, ivtmp.328
	strhi	r3, [ip, #-4]	@ tmp266,
.L60:
	mov	r3, #1	@ ch,
	cmp	r0, r3	@ dstCh, ch
	add	ip, ip, #4	@ ivtmp.328, ivtmp.328,
	bls	.L61	@,
	cmp	r8, #0	@ tmp240,
	beq	.L18	@,
	ldr	r5, [r2, #20]	@ <variable>.u32Channels, <variable>.u32Channels
	cmp	r5, r3	@ <variable>.u32Channels, ch
	strls	r6, [ip, #-4]	@ ch,
	movls	r5, ip	@ pu32Dst, ivtmp.328
	bls	.L64	@,
	b	.L56	@
.L18:
	ldr	r5, [r2, #20]	@ <variable>.u32Channels, <variable>.u32Channels
	cmp	r5, r3	@ <variable>.u32Channels, ch
	strls	r6, [ip, #-4]	@ ch,
	add	r3, r3, #1	@ tmp241, ch,
	ldrhi	r5, [r4], #4	@ tmp212,
	strhi	r5, [ip, #-4]	@ tmp212,
	add	ip, ip, #4	@ tmp242, ivtmp.328,
	ldr	r8, [r2, #20]	@ <variable>.u32Channels, <variable>.u32Channels
	mov	r5, ip	@ pu32Dst, tmp242
	cmp	r8, r3	@ <variable>.u32Channels, tmp241
	strls	r6, [ip, #-4]	@ ch,
	bls	.L64	@,
.L56:
	ldr	r8, [r4], #4	@ tmp271,
	mov	r5, ip	@ pu32Dst, tmp242
	str	r8, [ip, #-4]	@ tmp271,
.L64:
	add	r3, r3, #1	@ ch, tmp241,
	add	ip, ip, #4	@ ivtmp.328, tmp242,
	cmp	r0, r3	@ dstCh, ch
	bhi	.L18	@,
.L61:
	ldr	ip, [r2, #12]	@ <variable>.u32SamplesPerFrame, <variable>.u32SamplesPerFrame
.L19:
	add	r7, r7, #1	@ i, i,
	cmp	ip, r7	@ <variable>.u32SamplesPerFrame, i
	bhi	.L15	@,
	b	.L73	@
	.fnend
	.size	UTIL_UpmixPcmData, .-UTIL_UpmixPcmData
	.align	2
	.global	UTIL_DownmixPcmData
	.type	UTIL_DownmixPcmData, %function
UTIL_DownmixPcmData:
	.fnstart
.LFB801:
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
	ldr	r3, [r2, #16]	@ <variable>.u32BitPerSample, <variable>.u32BitPerSample
	cmp	r3, #24	@ <variable>.u32BitPerSample,
	beq	.L107	@,
	cmp	r3, #16	@ <variable>.u32BitPerSample,
	ldmnefd	sp, {r4, r5, r6, r7, fp, sp, pc}	@
	ldr	ip, [r2, #12]	@ <variable>.u32SamplesPerFrame, <variable>.u32SamplesPerFrame
	ldr	r4, [r2, #0]	@ temp.396, <variable>.pDataBuf
	cmp	ip, #0	@ <variable>.u32SamplesPerFrame,
	movne	r5, r1	@ pu16Dst, WkBuf
	movne	r6, #0	@ i.399,
	beq	.L88	@,
.L89:
	cmp	r0, #0	@ dstCh,
	beq	.L93	@,
	add	r3, r4, #2	@ ivtmp.390, pu16Src,
	mov	ip, #0	@ ch.398,
	movls	r4, r3	@ pu16Src, ivtmp.390
	bhi	.L100	@,
	add	ip, ip, #1	@ ch.398, ch.398,
	add	r3, r3, #2	@ ivtmp.390, ivtmp.390,
	cmp	r0, ip	@ dstCh, ch.398
	bls	.L108	@,
.L100:
	ldrh	r4, [r3, #-2]	@,
	add	ip, ip, #1	@ ch.398, ch.398,
	cmp	r0, ip	@ dstCh, ch.398
	strh	r4, [r5], #2	@ movhi	@,
	mov	r4, r3	@ pu16Src, ivtmp.390
	add	r3, r3, #2	@ ivtmp.390, ivtmp.390,
	bhi	.L100	@,
.L108:
	ldr	ip, [r2, #12]	@ <variable>.u32SamplesPerFrame, <variable>.u32SamplesPerFrame
.L93:
	add	r6, r6, #1	@ i.399, i.399,
	cmp	ip, r6	@ <variable>.u32SamplesPerFrame, i.399
	bhi	.L89	@,
.L106:
	ldr	r3, [r2, #16]	@ <variable>.u32BitPerSample, <variable>.u32BitPerSample
.L88:
	cmp	r3, #16	@ <variable>.u32BitPerSample,
	str	r0, [r2, #20]	@ dstCh, <variable>.u32Channels
	moveq	ip, ip, asl #1	@ tmp164, <variable>.u32SamplesPerFrame,
	str	r1, [r2, #0]	@ WkBuf, <variable>.pDataBuf
	muleq	r0, r0, ip	@ prephitmp.403, dstCh, tmp164
	beq	.L97	@,
	cmp	r3, #24	@ <variable>.u32BitPerSample,
	movne	r0, #0	@ prephitmp.403,
	moveq	ip, ip, asl #2	@ tmp166, <variable>.u32SamplesPerFrame,
	muleq	r0, r0, ip	@ prephitmp.403, dstCh, tmp166
.L97:
	str	r0, [r2, #8]	@ prephitmp.403, <variable>.u32DataBytes
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}	@
.L107:
	ldr	ip, [r2, #12]	@ <variable>.u32SamplesPerFrame, <variable>.u32SamplesPerFrame
	ldr	r4, [r2, #0]	@ D.14468, <variable>.pDataBuf
	cmp	ip, #0	@ <variable>.u32SamplesPerFrame,
	movne	r5, r1	@ pu32Dst, WkBuf
	movne	r7, #0	@ i,
	beq	.L88	@,
.L78:
	cmp	r0, #0	@ dstCh,
	beq	.L82	@,
	add	r3, r4, #4	@ ivtmp.383, pu32Src,
	mov	ip, #0	@ ch,
	movls	r4, r3	@ pu32Src, ivtmp.383
	bhi	.L99	@,
	add	ip, ip, #1	@ ch, ch,
	add	r3, r3, #4	@ ivtmp.383, ivtmp.383,
	cmp	r0, ip	@ dstCh, ch
	bls	.L109	@,
.L99:
	ldr	r6, [r3, #-4]	@ tmp152,
	add	ip, ip, #1	@ ch, ch,
	cmp	r0, ip	@ dstCh, ch
	mov	r4, r3	@ pu32Src, ivtmp.383
	add	r3, r3, #4	@ ivtmp.383, ivtmp.383,
	str	r6, [r5], #4	@ tmp152,
	bhi	.L99	@,
.L109:
	ldr	ip, [r2, #12]	@ <variable>.u32SamplesPerFrame, <variable>.u32SamplesPerFrame
.L82:
	add	r7, r7, #1	@ i, i,
	cmp	ip, r7	@ <variable>.u32SamplesPerFrame, i
	bhi	.L78	@,
	b	.L106	@
	.fnend
	.size	UTIL_DownmixPcmData, .-UTIL_DownmixPcmData
	.align	2
	.global	UTIL_UnifyBitDepthPcmData
	.type	UTIL_UnifyBitDepthPcmData, %function
UTIL_UnifyBitDepthPcmData:
	.fnstart
.LFB802:
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
	ldr	r3, [r2, #20]	@ <variable>.u32Channels, <variable>.u32Channels
	ldr	ip, [r2, #12]	@ <variable>.u32SamplesPerFrame, <variable>.u32SamplesPerFrame
	cmp	r0, #16	@ dstBitDepth,
	mul	r3, ip, r3	@ Samples, <variable>.u32SamplesPerFrame, <variable>.u32Channels
	beq	.L130	@,
	cmp	r0, #24	@ dstBitDepth,
	ldmnefd	sp, {r4, r5, r6, r7, fp, sp, pc}	@
	ldr	ip, [r2, #16]	@ <variable>.u32BitPerSample, <variable>.u32BitPerSample
	cmp	ip, #16	@ <variable>.u32BitPerSample,
	ldmnefd	sp, {r4, r5, r6, r7, fp, sp, pc}	@
	cmp	r3, #0	@ Samples,
	ldr	r4, [r2, #0]	@ temp.458, <variable>.pDataBuf
	beq	.L115	@,
	ldrh	ip, [r4, #0]	@ Data,* temp.458
	cmp	r3, #1	@ Samples,
	sub	r5, r3, #1	@ tmp165, Samples,
	mov	r0, #1	@ i.460,
	and	r5, r5, r0	@ tmp167, tmp165,
	mov	ip, ip, asl #16	@ tmp157, Data,
	str	ip, [r1, #0]	@ tmp157,* WkBuf
	bls	.L115	@,
	cmp	r5, #0	@ tmp167,
	beq	.L116	@,
	mov	r0, #2	@ tmp193,
	cmp	r3, r0	@ Samples, i.460
	ldrh	ip, [r4, r0]	@ Data,
	mov	ip, ip, asl #16	@ tmp196, Data,
	str	ip, [r1, #4]	@ tmp196,
	bls	.L115	@,
.L116:
	mov	r5, r0, asl #1	@ tmp180, i.460,
	add	ip, r0, #1	@ tmp168, i.460,
	ldrh	r6, [r4, r5]	@ Data,
	mov	r5, ip, asl #1	@ tmp185, tmp168,
	mov	r6, r6, asl #16	@ tmp183, Data,
	str	r6, [r1, r0, asl #2]	@ tmp183,* WkBuf
	add	r0, ip, #1	@ i.460, tmp168,
	ldrh	r5, [r4, r5]	@ Data,
	cmp	r3, r0	@ Samples, i.460
	mov	r5, r5, asl #16	@ tmp188, Data,
	str	r5, [r1, ip, asl #2]	@ tmp188,* WkBuf
	bhi	.L116	@,
.L115:
	ldr	r3, [r2, #8]	@ <variable>.u32DataBytes, <variable>.u32DataBytes
	mov	r0, #24	@ tmp158,
	str	r1, [r2, #0]	@ WkBuf, <variable>.pDataBuf
	str	r0, [r2, #16]	@ tmp158, <variable>.u32BitPerSample
	mov	r1, r3, asl #1	@ tmp160, <variable>.u32DataBytes,
	str	r1, [r2, #8]	@ tmp160, <variable>.u32DataBytes
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}	@
.L130:
	ldr	r0, [r2, #16]	@ <variable>.u32BitPerSample, <variable>.u32BitPerSample
	cmp	r0, #24	@ <variable>.u32BitPerSample,
	ldmnefd	sp, {r4, r5, r6, r7, fp, sp, pc}	@
	cmp	r3, #0	@ Samples,
	ldr	r4, [r2, #0]	@ D.14511, <variable>.pDataBuf
	beq	.L113	@,
	sub	r0, r3, #1	@ tmp161, Samples,
	cmp	r3, #1	@ Samples,
	and	ip, r0, #1	@ tmp163, tmp161,
	ldrh	r0, [r4, #2]	@,
	strh	r0, [r1, #0]	@ movhi	@,* WkBuf
	mov	r0, #1	@ i,
	bls	.L113	@,
	cmp	ip, #0	@ tmp163,
	beq	.L114	@,
	ldrh	r5, [r4, #6]	@,
	mov	ip, #2	@ tmp191,
	cmp	r3, ip	@ Samples, i
	mov	r0, ip	@ i, tmp191
	strh	r5, [r1, ip]	@ movhi	@,
	bls	.L113	@,
.L114:
	add	r7, r4, r0, asl #2	@, tmp173, D.14511, i,
	add	ip, r0, #1	@ tmp164, i,
	mov	r6, r0, asl #1	@ tmp172, i,
	add	r0, ip, #1	@ i, tmp164,
	ldrh	r7, [r7, #2]	@,
	mov	r5, ip, asl #1	@ tmp177, tmp164,
	add	ip, r4, ip, asl #2	@, tmp178, D.14511, tmp164,
	cmp	r3, r0	@ Samples, i
	strh	r7, [r1, r6]	@ movhi	@,
	ldrh	ip, [ip, #2]	@,
	strh	ip, [r1, r5]	@ movhi	@,
	bhi	.L114	@,
.L113:
	ldr	r3, [r2, #8]	@ <variable>.u32DataBytes, <variable>.u32DataBytes
	mov	r0, #16	@ tmp151,
	str	r1, [r2, #0]	@ WkBuf, <variable>.pDataBuf
	str	r0, [r2, #16]	@ tmp151, <variable>.u32BitPerSample
	mov	r1, r3, lsr #1	@ tmp153, <variable>.u32DataBytes,
	str	r1, [r2, #8]	@ tmp153, <variable>.u32DataBytes
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}	@
	.fnend
	.size	UTIL_UnifyBitDepthPcmData, .-UTIL_UnifyBitDepthPcmData
	.align	2
	.global	UTIL_GainProcess
	.type	UTIL_GainProcess, %function
UTIL_GainProcess:
	.fnstart
.LFB807:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	.movsp ip
	mov	ip, sp	@,
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, ip, lr, pc}	@,
	.pad #4
	.save {r4, r5, r6, r7, r8, r9, sl, fp, ip, lr}
	.setfp fp, ip, #-4
	sub	fp, ip, #4	@,,
	.pad #12
	sub	sp, sp, #12	@,,
	push	{lr}
	bl	__gnu_mcount_nc
	ldr	r3, [r0, #20]	@ <variable>.u32Channels, <variable>.u32Channels
	ldr	ip, [r0, #12]	@ <variable>.u32SamplesPerFrame, <variable>.u32SamplesPerFrame
	cmp	r1, #1	@ bLinearMode,
	mul	ip, ip, r3	@ samples, <variable>.u32SamplesPerFrame, <variable>.u32Channels
	beq	.L236	@,
	ldr	r3, [r0, #16]	@ <variable>.u32BitPerSample, <variable>.u32BitPerSample
	cmp	r3, #16	@ <variable>.u32BitPerSample,
	beq	.L144	@,
	cmp	r3, #24	@ <variable>.u32BitPerSample,
	beq	.L237	@,
.L156:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, sl, fp, sp, pc}
.L237:
	and	r3, r2, #127	@ tmp216, Volume,
	movw	r1, #:lower16:.LANCHOR0	@ tmp217,
	rsb	r2, r3, #127	@ idx.563, tmp216,
	movt	r1, #:upper16:.LANCHOR0	@ tmp217,
	cmp	r2, #87	@ idx.563,
	movge	r2, #87	@ tmp218,
	cmp	ip, #0	@ samples,
	ldr	r3, [r0, #0]	@ temp.559, <variable>.pDataBuf
	mov	r2, r2, asl #1	@ tmp219, tmp218,
	ldrh	r0, [r2, r1]	@ realVol, g_u16VolCoef
	beq	.L156	@,
	cmp	r0, #0	@ realVol,
	sub	r6, ip, #1	@ tmp270, samples,
	and	r1, r6, #1	@ tmp271, tmp270,
	mov	r4, #16384	@ tmp245,
	movw	r6, #:lower16:2147483392	@ tmp247,
	mov	r5, #0	@,
	movt	r6, #:upper16:2147483392	@ tmp247,
	ldr	r2, [r3, #0]	@ data,* temp.559
	bne	.L238	@,
.L189:
	mov	r2, #0	@ prephitmp.486,
.L216:
	subs	ip, ip, #1	@ ivtmp.537, samples,
	str	r2, [r3, #0]	@ prephitmp.486,* temp.559
	mov	r2, #4	@ ivtmp.543,
	beq	.L156	@,
	cmp	r1, #0	@ tmp271,
	beq	.L155	@,
	cmp	r0, #0	@ realVol,
	ldr	r1, [r3, r2]	@ data,* ivtmp.543
	beq	.L192	@,
	mov	sl, r1, asr #8	@ D.14977, data,
	mov	r8, r4	@ tmp403, tmp245
	mov	r9, r5	@,
	smlal	r8, r9, sl, r0	@ tmp403, D.14977, realVol
	mov	r1, r8, lsr #15	@ tmp405, tmp403,
	orrs	r1, r1, r9, asl #17	@, tmp406, tmp405,,
	beq	.L192	@,
	eor	r8, r1, sl	@ tmp407, tmp406, D.14977
	mov	r8, r8, lsr #23	@ tmp408, tmp407,
	mov	r8, r8, asl #23	@ tmp408, tmp408,
	cmp	r8, #0	@ tmp408,
	moveq	r1, r1, asl #8	@ prephitmp.486, tmp406,
	beq	.L219	@,
	mov	r1, sl, lsr #23	@ tmp409, D.14977,
	mov	r1, r1, asl #23	@ tmp409, tmp409,
	cmp	r1, #0	@ tmp409,
	moveq	r1, r6	@ prephitmp.486, tmp247
	movne	r1, #-2147483648	@ prephitmp.486,
.L219:
	subs	ip, ip, #1	@ ivtmp.537, ivtmp.537,
	str	r1, [r3, r2]	@ prephitmp.486,* ivtmp.543
	add	r2, r2, #4	@ ivtmp.543, ivtmp.543,
	beq	.L156	@,
.L235:
	cmp	r0, #0	@ realVol,
	ldr	r8, [r3, r2]	@ data,* ivtmp.543
	beq	.L151	@,
.L239:
	mov	sl, r8, asr #8	@ D.14977, data,
	mov	r9, r5	@,
	mov	r8, r4	@ tmp222, tmp245
	smlal	r8, r9, sl, r0	@ tmp222, D.14977, realVol
	mov	r1, r8, lsr #15	@ tmp234, tmp222,
	orrs	r1, r1, r9, asl #17	@, tmp315, tmp234,,
	beq	.L151	@,
	eor	r9, r1, sl	@ tmp227, tmp315, D.14977
	mov	r9, r9, lsr #23	@ tmp228, tmp227,
	mov	r9, r9, asl #23	@ tmp228, tmp228,
	cmp	r9, #0	@ tmp228,
	moveq	r8, r1, asl #8	@ prephitmp.486, tmp315,
	beq	.L153	@,
	mov	r8, sl, lsr #23	@ tmp230, D.14977,
	mov	r8, r8, asl #23	@ tmp230, tmp230,
	cmp	r8, #0	@ tmp230,
	moveq	r8, r6	@ prephitmp.486, tmp247
	movne	r8, #-2147483648	@ prephitmp.486,
.L153:
	add	r1, r2, #4	@ tmp272, ivtmp.543,
	cmp	r0, #0	@ realVol,
	str	r8, [r3, r2]	@ prephitmp.486,* ivtmp.543
	sub	ip, ip, #1	@ tmp273, ivtmp.537,
	ldr	r2, [r3, r1]	@ data,
	beq	.L196	@,
.L240:
	mov	sl, r2, asr #8	@ D.14977, data,
	mov	r8, r4	@ tmp416, tmp245
	mov	r9, r5	@,
	smlal	r8, r9, sl, r0	@ tmp416, D.14977, realVol
	mov	r2, r8, lsr #15	@ tmp418, tmp416,
	orrs	r2, r2, r9, asl #17	@, tmp419, tmp418,,
	beq	.L196	@,
	eor	r8, r2, sl	@ tmp420, tmp419, D.14977
	mov	r8, r8, lsr #23	@ tmp421, tmp420,
	mov	r8, r8, asl #23	@ tmp421, tmp421,
	cmp	r8, #0	@ tmp421,
	moveq	r2, r2, asl #8	@ prephitmp.486, tmp419,
	beq	.L221	@,
	mov	r2, sl, lsr #23	@ tmp422, D.14977,
	mov	r2, r2, asl #23	@ tmp422, tmp422,
	cmp	r2, #0	@ tmp422,
	moveq	r2, r6	@ prephitmp.486, tmp247
	movne	r2, #-2147483648	@ prephitmp.486,
.L221:
	subs	ip, ip, #1	@ ivtmp.537, tmp273,
	str	r2, [r3, r1]	@ prephitmp.486,
	add	r2, r1, #4	@ ivtmp.543, tmp272,
	beq	.L156	@,
.L155:
	cmp	r0, #0	@ realVol,
	ldr	r8, [r3, r2]	@ data,* ivtmp.543
	bne	.L239	@,
.L151:
	add	r1, r2, #4	@ tmp272, ivtmp.543,
	cmp	r0, #0	@ realVol,
	mov	r8, #0	@ prephitmp.486,
	sub	ip, ip, #1	@ tmp273, ivtmp.537,
	str	r8, [r3, r2]	@ prephitmp.486,* ivtmp.543
	ldr	r2, [r3, r1]	@ data,
	bne	.L240	@,
.L196:
	mov	r2, #0	@ prephitmp.486,
	b	.L221	@
.L236:
	ldr	r3, [r0, #16]	@ <variable>.u32BitPerSample, <variable>.u32BitPerSample
	cmp	r3, #16	@ <variable>.u32BitPerSample,
	beq	.L134	@,
	cmp	r3, #24	@ <variable>.u32BitPerSample,
	bne	.L156	@,
	uxth	r2, r2	@ temp.556, Volume
	ldr	r3, [r0, #0]	@ temp.555, <variable>.pDataBuf
	cmp	r2, #0	@ temp.556,
	bne	.L241	@,
	cmp	ip, #0	@ samples,
	ble	.L156	@,
	cmp	ip, #1	@ samples,
	sub	r0, ip, #1	@ tmp257, samples,
	and	r0, r0, #1	@ tmp259, tmp257,
	str	r2, [r3, #0]	@ tmp239,* temp.555
	beq	.L156	@,
	cmp	r0, #0	@ tmp259,
	beq	.L142	@,
	mov	r1, #2	@ n,
	cmp	ip, r1	@ samples, n
	str	r2, [r3, #4]	@ tmp239,
	beq	.L156	@,
.L142:
	add	r0, r1, #1	@ tmp260, n,
	str	r2, [r3, r1, asl #2]	@ tmp239,* temp.555
	add	r1, r0, #1	@ n, tmp260,
	cmp	ip, r1	@ samples, n
	str	r2, [r3, r0, asl #2]	@ tmp239,* temp.555
	bne	.L142	@,
	b	.L156	@
.L144:
	and	r3, r2, #127	@ tmp190, Volume,
	movw	r1, #:lower16:.LANCHOR0	@ tmp191,
	rsb	r2, r3, #127	@ idx.562, tmp190,
	movt	r1, #:upper16:.LANCHOR0	@ tmp191,
	cmp	r2, #87	@ idx.562,
	movge	r2, #87	@ tmp192,
	cmp	ip, #0	@ samples,
	ldr	r3, [r0, #0]	@ temp.558, <variable>.pDataBuf
	mov	r2, r2, asl #1	@ tmp193, tmp192,
	ldrh	r0, [r2, r1]	@ realVol, g_u16VolCoef
	beq	.L156	@,
	cmp	r0, #0	@ realVol,
	sub	r5, ip, #1	@ tmp266, samples,
	and	r1, r5, #1	@ tmp267, tmp266,
	ldrh	r2, [r3, #0]	@ D.14954,* temp.558
	mov	r4, r0	@ realVol, realVol
	mov	r5, r4, asr #31	@ realVol
	bne	.L242	@,
.L179:
	mov	r8, #0	@ prephitmp.488,
.L209:
	subs	ip, ip, #1	@ ivtmp.537, samples,
	strh	r8, [r3, #0]	@ movhi	@ prephitmp.488,* temp.558
	mov	r2, #2	@ ivtmp.527,
	beq	.L156	@,
	cmp	r1, #0	@ tmp267,
	beq	.L232	@,
	cmp	r0, #0	@ realVol,
	ldrh	r1, [r3, r2]	@ D.14954,* ivtmp.527
	beq	.L182	@,
	sxth	sl, r1	@ D.14954, D.14954
	mov	r6, sl	@ D.14954, D.14954
	mov	r7, r6, asr #31	@ D.14954
	mul	r8, r4, r7	@ tmp354, realVol, D.14954
	mla	r9, r6, r5, r8	@ tmp355, D.14954, realVol, tmp354
	mov	r8, #16384	@,
	umull	r6, r7, r4, r6	@ tmp356, realVol, D.14954
	adds	r6, r6, r8	@ tmp359, tmp356,
	add	r7, r9, r7	@, tmp355,
	mov	r8, r6, lsr #15	@ tmp361, tmp359,
	mov	r9, #0	@,
	adc	r7, r7, r9	@,,
	orrs	r8, r8, r7, asl #17	@, tmp362, tmp361,,
	beq	.L182	@,
	and	r1, r1, #32768	@ tmp363, D.14954,
	bic	r6, r8, #32512	@ tmp365, tmp362,
	bic	r6, r6, #255	@ tmp365, tmp365,
	sxth	r1, r1	@ tmp364, tmp363
	cmp	r1, r6	@ tmp364, tmp365
	uxtheq	r8, r8	@ prephitmp.488, tmp362
	beq	.L212	@,
	cmp	sl, #0	@ D.14954,
	movw	r8, #32767	@ prephitmp.488,
	movlt	r8, #32768	@ prephitmp.488,
.L212:
	subs	ip, ip, #1	@ ivtmp.537, ivtmp.537,
	strh	r8, [r3, r2]	@ movhi	@ prephitmp.488,* ivtmp.527
	add	r2, r2, #2	@ ivtmp.527, ivtmp.527,
	beq	.L156	@,
.L232:
	strd	r4, [fp, #-52]	@ realVol,,
	b	.L150	@
.L243:
	ldr	r7, [fp, #-52]	@, %sfp
	sxth	r6, r1	@ D.14954, D.14954
	ldr	r9, [fp, #-48]	@, %sfp
	mov	r4, r6	@ D.14954, D.14954
	mov	r5, r4, asr #31	@ D.14954
	mul	r8, r7, r5	@ tmp199,, D.14954
	mla	r8, r4, r9, r8	@ tmp306, D.14954,, tmp199
	mov	r9, #0	@,
	umull	r4, r5, r7, r4	@ tmp200,, D.14954
	add	r5, r8, r5	@, tmp306,
	mov	r8, #16384	@,
	adds	r4, r4, r8	@ tmp201, tmp200,
	adc	r5, r5, r9	@,,
	mov	r8, r4, lsr #15	@ tmp232, tmp201,
	orrs	r8, r8, r5, asl #17	@, tmp308, tmp232,,
	beq	.L146	@,
	and	r1, r1, #32768	@ tmp209, D.14954,
	bic	r4, r8, #32512	@ tmp211, tmp308,
	bic	r4, r4, #255	@ tmp211, tmp211,
	sxth	r1, r1	@ tmp210, tmp209
	cmp	r1, r4	@ tmp210, tmp211
	uxtheq	r6, r8	@ prephitmp.488, tmp308
	beq	.L148	@,
	cmp	r6, #0	@ D.14954,
	movw	r9, #32767	@,
	movlt	r9, #32768	@,
	mov	r6, r9	@ prephitmp.488,
.L148:
	add	r1, r2, #2	@ tmp268, ivtmp.527,
	cmp	r0, #0	@ realVol,
	strh	r6, [r3, r2]	@ movhi	@ prephitmp.488,* ivtmp.527
	sub	ip, ip, #1	@ tmp269, ivtmp.537,
	ldrh	r2, [r3, r1]	@ D.14954,
	beq	.L186	@,
.L244:
	ldr	r9, [fp, #-52]	@, %sfp
	sxth	r6, r2	@ D.14954, D.14954
	ldr	r7, [fp, #-48]	@, %sfp
	mov	r4, r6	@ D.14954, D.14954
	mov	r5, r4, asr #31	@ D.14954
	mul	r8, r9, r5	@ tmp372,, D.14954
	mla	r8, r4, r7, r8	@ tmp373, D.14954,, tmp372
	umull	r4, r5, r9, r4	@ tmp374,, D.14954
	mov	r9, #0	@,
	add	r5, r8, r5	@, tmp373,
	mov	r8, #16384	@,
	adds	r4, r4, r8	@ tmp377, tmp374,
	adc	r5, r5, r9	@,,
	mov	r8, r4, lsr #15	@ tmp379, tmp377,
	orrs	r8, r8, r5, asl #17	@, tmp380, tmp379,,
	beq	.L186	@,
	and	r2, r2, #32768	@ tmp381, D.14954,
	bic	r4, r8, #32512	@ tmp383, tmp380,
	bic	r4, r4, #255	@ tmp383, tmp383,
	sxth	r2, r2	@ tmp382, tmp381
	cmp	r2, r4	@ tmp382, tmp383
	uxtheq	r6, r8	@ prephitmp.488, tmp380
	beq	.L214	@,
	cmp	r6, #0	@ D.14954,
	movw	r9, #32767	@,
	movlt	r9, #32768	@,
	mov	r6, r9	@ prephitmp.488,
.L214:
	subs	ip, ip, #1	@ ivtmp.537, tmp269,
	strh	r6, [r3, r1]	@ movhi	@ prephitmp.488,
	add	r2, r1, #2	@ ivtmp.527, tmp268,
	beq	.L156	@,
.L150:
	cmp	r0, #0	@ realVol,
	ldrh	r1, [r3, r2]	@ D.14954,* ivtmp.527
	bne	.L243	@,
.L146:
	add	r1, r2, #2	@ tmp268, ivtmp.527,
	cmp	r0, #0	@ realVol,
	mov	r6, #0	@ prephitmp.488,
	sub	ip, ip, #1	@ tmp269, ivtmp.537,
	strh	r6, [r3, r2]	@ movhi	@ prephitmp.488,* ivtmp.527
	ldrh	r2, [r3, r1]	@ D.14954,
	bne	.L244	@,
.L186:
	mov	r6, #0	@ prephitmp.488,
	b	.L214	@
.L242:
	sxth	r6, r2	@, D.14954
	str	r6, [fp, #-52]	@, %sfp
	mov	r7, r6, asr #31	@ D.14954
	mul	r8, r4, r7	@ tmp335, realVol, D.14954
	mla	r9, r6, r5, r8	@ tmp336, D.14954, realVol, tmp335
	mov	r8, #16384	@,
	umull	r6, r7, r4, r6	@ tmp337, realVol, D.14954
	adds	r6, r6, r8	@ tmp340, tmp337,
	add	r7, r9, r7	@, tmp336,
	mov	sl, r6, lsr #15	@ tmp342, tmp340,
	mov	r9, #0	@,
	adc	r7, r7, r9	@,,
	orrs	sl, sl, r7, asl #17	@, tmp343, tmp342,,
	beq	.L179	@,
	and	r2, r2, #32768	@ tmp344, D.14954,
	bic	r6, sl, #32512	@ tmp346, tmp343,
	bic	r6, r6, #255	@ tmp346, tmp346,
	sxth	r2, r2	@ tmp345, tmp344
	cmp	r2, r6	@ tmp345, tmp346
	uxtheq	r8, sl	@ prephitmp.488, tmp343
	beq	.L209	@,
	ldr	r9, [fp, #-52]	@, %sfp
	movw	r8, #32767	@ prephitmp.488,
	cmp	r9, #0	@,
	movlt	r8, #32768	@ prephitmp.488,
	b	.L209	@
.L238:
	mov	sl, r2, asr #8	@ D.14977, data,
	mov	r8, r4	@ tmp389, tmp245
	mov	r9, r5	@,
	smlal	r8, r9, sl, r0	@ tmp389, D.14977, realVol
	mov	r2, r8, lsr #15	@ tmp391, tmp389,
	orrs	r2, r2, r9, asl #17	@, tmp392, tmp391,,
	beq	.L189	@,
	eor	r8, r2, sl	@ tmp393, tmp392, D.14977
	mov	r8, r8, lsr #23	@ tmp394, tmp393,
	mov	r8, r8, asl #23	@ tmp394, tmp394,
	cmp	r8, #0	@ tmp394,
	moveq	r2, r2, asl #8	@ prephitmp.486, tmp392,
	beq	.L216	@,
	mov	r2, sl, lsr #23	@ tmp395, D.14977,
	mov	r2, r2, asl #23	@ tmp395, tmp395,
	cmp	r2, #0	@ tmp395,
	moveq	r2, r6	@ prephitmp.486, tmp247
	movne	r2, #-2147483648	@ prephitmp.486,
	b	.L216	@
.L134:
	uxth	r2, r2	@ D.14609, Volume
	ldr	r3, [r0, #0]	@ D.14607, <variable>.pDataBuf
	cmp	r2, #0	@ D.14609,
	bne	.L245	@,
	cmp	ip, #0	@ samples,
	ble	.L156	@,
	cmp	ip, #1	@ samples,
	sub	r0, ip, #1	@ tmp248, samples,
	and	r0, r0, #1	@ tmp250, tmp248,
	strh	r2, [r3, #0]	@ movhi	@ D.14609,* D.14607
	beq	.L156	@,
	cmp	r0, #0	@ tmp250,
	beq	.L138	@,
	mov	r0, #2	@ tmp321,
	cmp	ip, r0	@ samples, n
	mov	r1, r0	@ n, tmp321
	strh	r2, [r3, r0]	@ movhi	@ D.14609,
	beq	.L156	@,
.L138:
	add	r4, r1, #1	@ tmp251, n,
	mov	r0, r1, asl #1	@ tmp275, n,
	add	r1, r4, #1	@ n, tmp251,
	mov	r2, r4, asl #1	@ tmp277, tmp251,
	cmp	ip, r1	@ samples, n
	mov	r4, #0	@ movhi	@,
	strh	r4, [r3, r0]	@ movhi	@,
	strh	r4, [r3, r2]	@ movhi	@,
	bne	.L138	@,
	b	.L156	@
.L182:
	mov	r8, #0	@ prephitmp.488,
	b	.L212	@
.L192:
	subs	ip, ip, #1	@ ivtmp.537, ivtmp.537,
	mov	r1, #0	@ prephitmp.486,
	str	r1, [r3, r2]	@ prephitmp.486,* ivtmp.543
	add	r2, r2, #4	@ ivtmp.543, ivtmp.543,
	bne	.L235	@,
	b	.L156	@
.L245:
	cmp	ip, #0	@ samples,
	ble	.L156	@,
	sub	r0, ip, #1	@ tmp252, samples,
	sxth	r4, r2	@ pretmp.496, D.14609
	and	r0, r0, #1	@ tmp254, tmp252,
	ldrsh	r2, [r3, #0]	@ tmp182,* D.14607
#APP
@ 637 "/home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/../../../source/msp/drv/adsp/adsp_v1_1/hifi/common/include/hi_dsp_basicop.h" 1
	smulwb  r2, r2, r4	@ ret, tmp182, pretmp.496
@ 0 "" 2
@ 638 "/home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/../../../source/msp/drv/adsp/adsp_v1_1/hifi/common/include/hi_dsp_basicop.h" 1
	qadd    r2, r2, r2	@ sum, ret, ret
@ 0 "" 2
	cmp	ip, #1	@ samples,
	strh	r2, [r3, #0]	@ movhi	@ sum,* D.14607
	mov	r2, #2	@ ivtmp.514,
	beq	.L156	@,
	cmp	r0, #0	@ tmp254,
	beq	.L139	@,
	ldrsh	r0, [r3, #2]	@ tmp322,
#APP
@ 637 "/home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/../../../source/msp/drv/adsp/adsp_v1_1/hifi/common/include/hi_dsp_basicop.h" 1
	smulwb  r1, r0, r4	@ ret, tmp322, pretmp.496
@ 0 "" 2
@ 638 "/home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/../../../source/msp/drv/adsp/adsp_v1_1/hifi/common/include/hi_dsp_basicop.h" 1
	qadd    r0, r1, r1	@ sum, ret, ret
@ 0 "" 2
	cmp	ip, r2	@ samples, n.560
	mov	r1, r2	@ n.560, ivtmp.514
	strh	r0, [r3, #2]	@ movhi	@ sum,
	mov	r2, #4	@ ivtmp.514,
	beq	.L156	@,
.L139:
	ldrsh	r6, [r3, r2]	@ tmp280,* ivtmp.514
#APP
@ 637 "/home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/../../../source/msp/drv/adsp/adsp_v1_1/hifi/common/include/hi_dsp_basicop.h" 1
	smulwb  r0, r6, r4	@ ret, tmp280, pretmp.496
@ 0 "" 2
@ 638 "/home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/../../../source/msp/drv/adsp/adsp_v1_1/hifi/common/include/hi_dsp_basicop.h" 1
	qadd    r6, r0, r0	@ sum, ret, ret
@ 0 "" 2
	add	r0, r2, #2	@ tmp256, ivtmp.514,
	strh	r6, [r3, r2]	@ movhi	@ sum,* ivtmp.514
	add	r1, r1, #1	@ tmp255, n.560,
	ldrsh	r2, [r3, r0]	@ tmp285,
#APP
@ 637 "/home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/../../../source/msp/drv/adsp/adsp_v1_1/hifi/common/include/hi_dsp_basicop.h" 1
	smulwb  r2, r2, r4	@ ret, tmp285, pretmp.496
@ 0 "" 2
@ 638 "/home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/../../../source/msp/drv/adsp/adsp_v1_1/hifi/common/include/hi_dsp_basicop.h" 1
	qadd    r2, r2, r2	@ sum, ret, ret
@ 0 "" 2
	add	r1, r1, #1	@ n.560, tmp255,
	strh	r2, [r3, r0]	@ movhi	@ sum,
	cmp	ip, r1	@ samples, n.560
	add	r2, r0, #2	@ ivtmp.514, tmp256,
	bne	.L139	@,
	b	.L156	@
.L241:
	cmp	ip, #0	@ samples,
	ble	.L156	@,
	sub	r0, ip, #1	@ tmp261, samples,
	sxth	r4, r2	@ temp.556, temp.556
	and	r0, r0, #1	@ tmp263, tmp261,
	ldr	r2, [r3, #0]	@ tmp185,* temp.555
#APP
@ 637 "/home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/../../../source/msp/drv/adsp/adsp_v1_1/hifi/common/include/hi_dsp_basicop.h" 1
	smulwb  r2, r2, r4	@ ret, tmp185, temp.556
@ 0 "" 2
@ 638 "/home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/../../../source/msp/drv/adsp/adsp_v1_1/hifi/common/include/hi_dsp_basicop.h" 1
	qadd    r2, r2, r2	@ sum, ret, ret
@ 0 "" 2
	cmp	ip, #1	@ samples,
	str	r2, [r3, #0]	@ sum,* temp.555
	mov	r2, #4	@ ivtmp.522,
	beq	.L156	@,
	cmp	r0, #0	@ tmp263,
	beq	.L143	@,
	ldr	r0, [r3, #4]	@ tmp327,
#APP
@ 637 "/home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/../../../source/msp/drv/adsp/adsp_v1_1/hifi/common/include/hi_dsp_basicop.h" 1
	smulwb  r1, r0, r4	@ ret, tmp327, temp.556
@ 0 "" 2
@ 638 "/home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/../../../source/msp/drv/adsp/adsp_v1_1/hifi/common/include/hi_dsp_basicop.h" 1
	qadd    r2, r1, r1	@ sum, ret, ret
@ 0 "" 2
	mov	r1, #2	@ n.561,
	cmp	ip, r1	@ samples, n.561
	str	r2, [r3, #4]	@ sum,
	mov	r2, #8	@ ivtmp.522,
	beq	.L156	@,
.L143:
	ldr	r6, [r3, r2]	@ tmp296,* ivtmp.522
#APP
@ 637 "/home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/../../../source/msp/drv/adsp/adsp_v1_1/hifi/common/include/hi_dsp_basicop.h" 1
	smulwb  r0, r6, r4	@ ret, tmp296, temp.556
@ 0 "" 2
@ 638 "/home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/../../../source/msp/drv/adsp/adsp_v1_1/hifi/common/include/hi_dsp_basicop.h" 1
	qadd    r6, r0, r0	@ sum, ret, ret
@ 0 "" 2
	add	r0, r2, #4	@ tmp265, ivtmp.522,
	str	r6, [r3, r2]	@ sum,* ivtmp.522
	add	r1, r1, #1	@ tmp264, n.561,
	ldr	r2, [r3, r0]	@ tmp301,
#APP
@ 637 "/home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/../../../source/msp/drv/adsp/adsp_v1_1/hifi/common/include/hi_dsp_basicop.h" 1
	smulwb  r2, r2, r4	@ ret, tmp301, temp.556
@ 0 "" 2
@ 638 "/home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/../../../source/msp/drv/adsp/adsp_v1_1/hifi/common/include/hi_dsp_basicop.h" 1
	qadd    r2, r2, r2	@ sum, ret, ret
@ 0 "" 2
	add	r1, r1, #1	@ n.561, tmp264,
	str	r2, [r3, r0]	@ sum,
	cmp	ip, r1	@ samples, n.561
	add	r2, r0, #4	@ ivtmp.522, tmp265,
	bne	.L143	@,
	b	.L156	@
	.fnend
	.size	UTIL_GainProcess, .-UTIL_GainProcess
	.align	2
	.global	UTIL_MixerPro
	.type	UTIL_MixerPro, %function
UTIL_MixerPro:
	.fnstart
.LFB808:
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
	cmp	r2, #0	@ samples
	ldmeqfd	sp, {r4, r5, r6, r7, fp, sp, pc}	@
	cmp	r3, #16	@ uBitDepth,
	beq	.L270	@,
	cmp	r3, #24	@ uBitDepth,
	ldmnefd	sp, {r4, r5, r6, r7, fp, sp, pc}	@
	sub	r3, r2, #1	@ tmp156, samples,
	ldr	r4, [r0, #0]	@ tmp148,* pMixBuf
	ldr	ip, [r1, #0]	@ tmp149,* pInBuf
	and	r5, r3, #1	@ tmp158, tmp156,
#APP
@ 687 "/home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/../../../source/msp/drv/adsp/adsp_v1_1/hifi/common/include/hi_dsp_basicop.h" 1
	qadd r3, r4, ip	@ Lvar1, tmp148, tmp149
@ 0 "" 2
	cmp	r2, #1	@ samples,
	str	r3, [r0, #0]	@ Lvar1,* pMixBuf
	mov	r4, #1	@ i.615,
	mov	r3, #4	@ ivtmp.610,
	ldmlsfd	sp, {r4, r5, r6, r7, fp, sp, pc}	@
	cmp	r5, #0	@ tmp158,
	beq	.L251	@,
	ldr	r4, [r0, #4]	@ tmp193,
	ldr	ip, [r1, #4]	@ tmp194,
#APP
@ 687 "/home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/../../../source/msp/drv/adsp/adsp_v1_1/hifi/common/include/hi_dsp_basicop.h" 1
	qadd r3, r4, ip	@ Lvar1, tmp193, tmp194
@ 0 "" 2
	mov	r4, #2	@ i.615,
	cmp	r2, r4	@ samples, i.615
	str	r3, [r0, #4]	@ Lvar1,
	mov	r3, #8	@ ivtmp.610,
	ldmlsfd	sp, {r4, r5, r6, r7, fp, sp, pc}	@
.L251:
	ldr	ip, [r0, r3]	@ tmp177,* ivtmp.610
	ldr	r5, [r1, r3]	@ tmp178,* ivtmp.610
#APP
@ 687 "/home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/../../../source/msp/drv/adsp/adsp_v1_1/hifi/common/include/hi_dsp_basicop.h" 1
	qadd r5, ip, r5	@ Lvar1, tmp177, tmp178
@ 0 "" 2
	add	ip, r3, #4	@ tmp159, ivtmp.610,
	str	r5, [r0, r3]	@ Lvar1,* ivtmp.610
	add	r4, r4, #1	@ tmp160, i.615,
	ldr	r3, [r0, ip]	@ tmp182,
	ldr	r5, [r1, ip]	@ tmp183,
#APP
@ 687 "/home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/../../../source/msp/drv/adsp/adsp_v1_1/hifi/common/include/hi_dsp_basicop.h" 1
	qadd r3, r3, r5	@ Lvar1, tmp182, tmp183
@ 0 "" 2
	add	r4, r4, #1	@ i.615, tmp160,
	str	r3, [r0, ip]	@ Lvar1,
	cmp	r2, r4	@ samples, i.615
	add	r3, ip, #4	@ ivtmp.610, tmp159,
	bhi	.L251	@,
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}	@
.L270:
	ldrsh	r5, [r1, #0]	@ tmp144,* pInBuf
	movw	r7, #32767	@ tmp150,
	ldrsh	r3, [r0, #0]	@ tmp145,* pMixBuf
	sub	r4, r2, #1	@ tmp151, samples,
	and	r4, r4, #1	@ tmp153, tmp151,
	add	r3, r5, r3	@ x, tmp144, tmp145
	mov	r5, #1	@ i,
	mov	ip, r3, asr #31	@ sign, x,
	cmp	ip, r3, asr #15	@ sign, x,
	eorne	r3, ip, r7	@ x, sign, tmp150
	cmp	r2, r5	@ samples, i
	strh	r3, [r0, #0]	@ movhi	@ x,* pMixBuf
	mov	r3, #2	@ ivtmp.598,
	ldmlsfd	sp, {r4, r5, r6, r7, fp, sp, pc}	@
	cmp	r4, #0	@ tmp153,
	beq	.L250	@,
	ldrsh	r4, [r1, r3]	@ tmp186,* ivtmp.598
	add	r5, r5, #1	@ i, i,
	ldrsh	ip, [r0, r3]	@ tmp187,* ivtmp.598
	add	ip, r4, ip	@ x, tmp186, tmp187
	mov	r4, ip, asr #31	@ sign, x,
	cmp	r4, ip, asr #15	@ sign, x,
	eorne	ip, r4, r7	@ x, sign, tmp150
	cmp	r2, r5	@ samples, i
	strh	ip, [r0, r3]	@ movhi	@ x,* ivtmp.598
	add	r3, r3, #2	@ ivtmp.598, ivtmp.598,
	ldmlsfd	sp, {r4, r5, r6, r7, fp, sp, pc}	@
.L250:
	ldrsh	r6, [r1, r3]	@ tmp162,* ivtmp.598
	add	ip, r3, #2	@ tmp155, ivtmp.598,
	ldrsh	r4, [r0, r3]	@ tmp163,* ivtmp.598
	add	r5, r5, #1	@ tmp154, i,
	add	r5, r5, #1	@ i, tmp154,
	add	r4, r6, r4	@ x, tmp162, tmp163
	mov	r6, r4, asr #31	@ sign, x,
	cmp	r6, r4, asr #15	@ sign, x,
	eorne	r4, r6, r7	@ x, sign, tmp150
	strh	r4, [r0, r3]	@ movhi	@ x,* ivtmp.598
	ldrsh	r4, [r1, ip]	@ tmp170,
	ldrsh	r3, [r0, ip]	@ tmp171,
	add	r3, r4, r3	@ x, tmp170, tmp171
	mov	r4, r3, asr #31	@ sign, x,
	cmp	r4, r3, asr #15	@ sign, x,
	eorne	r3, r4, r7	@ x, sign, tmp150
	cmp	r2, r5	@ samples, i
	strh	r3, [r0, ip]	@ movhi	@ x,
	add	r3, ip, #2	@ ivtmp.598, tmp155,
	bhi	.L250	@,
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}	@
	.fnend
	.size	UTIL_MixerPro, .-UTIL_MixerPro
	.align	2
	.global	FADE_Active
	.type	FADE_Active, %function
FADE_Active:
	.fnstart
.LFB812:
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
	cmp	r0, #0	@ hFade
	ldmeqfd	sp, {fp, sp, pc}	@
	cmp	r2, #0	@ u32FadeMs,
	str	r2, [r0, #12]	@ u32FadeMs, <variable>.u32FadeMs
	mov	r3, #0	@ tmp137,
	str	r3, [r0, #20]	@ tmp137, <variable>.u32FadeCnt
	ldrne	ip, [r0, #8]	@ <variable>.u32InRate, <variable>.u32InRate
	movwne	r3, #:lower16:-819967583	@ tmp143,
	movtne	r3, #:upper16:-819967583	@ tmp143,
	streq	r2, [r0, #24]	@ u32FadeMs, <variable>.u32FadeStep
	str	r1, [r0, #28]	@ enUserFadeStauts, <variable>.enCurnFadeStauts
	mulne	r2, ip, r2	@ tmp139, <variable>.u32InRate, u32FadeMs
	umullne	ip, r3, r3, r2	@, tmp142, tmp143, tmp139
	movne	r3, r3, lsr #16	@ tmp141, tmp142,
	addne	r3, r3, #1	@ tmp144, tmp141,
	strne	r3, [r0, #24]	@ tmp144, <variable>.u32FadeStep
	cmp	r1, #1	@ enUserFadeStauts,
	moveq	r3, #87	@ tmp145,
	streq	r3, [r0, #16]	@ tmp145, <variable>.u32FadeCurnCoefIdx
	ldmeqfd	sp, {fp, sp, pc}	@
	cmp	r1, #2	@ enUserFadeStauts,
	moveq	r3, #6	@ tmp146,
	streq	r3, [r0, #16]	@ tmp146, <variable>.u32FadeCurnCoefIdx
	ldmfd	sp, {fp, sp, pc}	@
	.fnend
	.size	FADE_Active, .-FADE_Active
	.align	2
	.global	FADE_Flush
	.type	FADE_Flush, %function
FADE_Flush:
	.fnstart
.LFB810:
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
	movw	ip, #:lower16:191999	@ tmp142,
	sub	r4, r1, #1	@ tmp141, inRate,
	movt	ip, #:upper16:191999	@ tmp142,
	cmp	r4, ip	@ tmp141, tmp142
	mov	ip, r0	@ hFade, hFade
	ldmhifd	sp, {r3, r4, fp, sp, pc}	@
	cmp	r2, #24	@ BitPerSample,
	cmpne	r2, #16	@, BitPerSample,
	ldmnefd	sp, {r3, r4, fp, sp, pc}	@
	sub	r0, r3, #1	@ tmp150, Channels,
	cmp	r3, #8	@ Channels,
	cmpne	r0, #1	@, tmp150,
	ldmhifd	sp, {r3, r4, fp, sp, pc}	@
	cmp	ip, #0	@ hFade,
	ldmeqfd	sp, {r3, r4, fp, sp, pc}	@
	ldr	r0, [ip, #4]	@ <variable>.u32Channels, <variable>.u32Channels
	ldr	r4, [ip, #0]	@ D.14716, <variable>.u32BitPerSample
	cmp	r3, r0	@ Channels, <variable>.u32Channels
	movne	r4, #1	@ isChange,
	beq	.L283	@,
.L280:
	ldr	r0, [ip, #8]	@ <variable>.u32InRate, <variable>.u32InRate
	cmp	r0, r1	@ <variable>.u32InRate, inRate
	beq	.L284	@,
.L281:
	str	r2, [ip, #0]	@ BitPerSample, <variable>.u32BitPerSample
	mov	r0, ip	@, hFade
	str	r1, [ip, #8]	@ inRate, <variable>.u32InRate
	str	r3, [ip, #4]	@ Channels, <variable>.u32Channels
	ldr	r1, [ip, #28]	@, <variable>.enCurnFadeStauts
	ldr	r2, [ip, #12]	@, <variable>.u32FadeMs
	bl	FADE_Active	@
	ldmfd	sp, {r3, r4, fp, sp, pc}	@
.L283:
	subs	r4, r2, r4	@ isChange, BitPerSample, D.14716
	movne	r4, #1	@ isChange
	b	.L280	@
.L284:
	cmp	r4, #0	@ isChange,
	ldmeqfd	sp, {r3, r4, fp, sp, pc}	@
	b	.L281	@
	.fnend
	.size	FADE_Flush, .-FADE_Flush
	.align	2
	.global	FADE_Process
	.type	FADE_Process, %function
FADE_Process:
	.fnstart
.LFB813:
	@ args = 0, pretend = 0, frame = 48
	@ frame_needed = 1, uses_anonymous_args = 0
	.movsp ip
	mov	ip, sp	@,
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, ip, lr, pc}	@,
	.pad #4
	.save {r4, r5, r6, r7, r8, r9, sl, fp, ip, lr}
	.setfp fp, ip, #-4
	sub	fp, ip, #4	@,,
	.pad #52
	sub	sp, sp, #52	@,,
	push	{lr}
	bl	__gnu_mcount_nc
	cmp	r0, #0	@ hFade
	beq	.L305	@,
	ldr	r3, [r0, #28]	@ <variable>.enCurnFadeStauts, <variable>.enCurnFadeStauts
	cmp	r3, #0	@ <variable>.enCurnFadeStauts,
	beq	.L305	@,
	cmp	r2, #0	@ samples,
	ldr	r9, [r0, #4]	@ ivtmp.708, <variable>.u32Channels
	beq	.L305	@,
	movw	r4, #:lower16:.LANCHOR0	@,
	mov	r5, r9, asl #1	@, ivtmp.708,
	movt	r4, #:upper16:.LANCHOR0	@,
	mov	r6, r9, asl #2	@, ivtmp.708,
	str	r4, [fp, #-64]	@, %sfp
	movw	r4, #:lower16:2147483392	@,
	str	r5, [fp, #-80]	@, %sfp
	movt	r4, #:upper16:2147483392	@,
	str	r6, [fp, #-76]	@, %sfp
	str	r4, [fp, #-68]	@, %sfp
	b	.L304	@
.L355:
	cmp	ip, #24	@ <variable>.u32BitPerSample,
	beq	.L353	@,
.L287:
	ldr	r3, [r0, #20]	@ <variable>.u32FadeCnt, <variable>.u32FadeCnt
	sub	r2, r2, #1	@ samples, samples,
	ldr	ip, [r0, #24]	@ <variable>.u32FadeStep, <variable>.u32FadeStep
	add	r3, r3, #1	@ D.14784, <variable>.u32FadeCnt,
	str	r3, [r0, #20]	@ D.14784, <variable>.u32FadeCnt
	cmp	r3, ip	@ D.14784, <variable>.u32FadeStep
	beq	.L354	@,
.L302:
	cmp	r2, #0	@ samples,
	beq	.L305	@,
.L304:
	ldr	r7, [r0, #16]	@ <variable>.u32FadeCurnCoefIdx, <variable>.u32FadeCurnCoefIdx
	ldr	ip, [r0, #0]	@ <variable>.u32BitPerSample, <variable>.u32BitPerSample
	mov	r3, r7, asl #1	@ tmp162, <variable>.u32FadeCurnCoefIdx,
	ldr	r7, [fp, #-64]	@, %sfp
	cmp	ip, #16	@ <variable>.u32BitPerSample,
	ldrh	r3, [r3, r7]	@ Volume, g_u16VolCoef
	bne	.L355	@,
	cmp	r9, #0	@ ivtmp.708,
	beq	.L290	@,
	cmp	r3, #0	@ Volume,
	sub	r8, r9, #1	@ tmp229, ivtmp.708,
	and	r5, r8, #1	@, tmp229,
	ldrh	ip, [r1, #0]	@ D.15007,* pInBuf
	str	r5, [fp, #-72]	@, %sfp
	mov	r4, r3	@, Volume
	mov	r5, r4, asr #31	@
	strd	r4, [fp, #-60]	@,,
	beq	.L309	@,
	sxth	r8, ip	@ D.15007, D.15007
	ldr	r6, [fp, #-60]	@, %sfp
	ldr	r7, [fp, #-56]	@, %sfp
	mov	r4, r8	@, D.15007
	mov	r5, r4, asr #31	@
	strd	r4, [fp, #-52]	@,,
	ldr	sl, [fp, #-48]	@, %sfp
	mul	r5, r6, sl	@ tmp259,,
	mla	r5, r4, r7, r5	@ tmp260,,, tmp259
	mov	r7, r6	@,
	ldr	r6, [fp, #-52]	@, %sfp
	umull	r6, r7, r7, r6	@,,
	add	sl, r5, r7	@, tmp260,
	strd	r6, [fp, #-52]	@,,
	mov	r6, #16384	@,
	str	sl, [fp, #-48]	@, %sfp
	mov	r7, #0	@,
	ldrd	r4, [fp, #-52]	@,,
	adds	r4, r4, r6	@,,
	adc	r5, r5, r7	@,,
	strd	r4, [fp, #-52]	@,,
	ldr	r7, [fp, #-48]	@, %sfp
	mov	r5, r4, lsr #15	@ tmp266,,
	orrs	r5, r5, r7, asl #17	@, tmp267, tmp266,,
	beq	.L309	@,
	and	sl, ip, #32768	@ tmp268, D.15007,
	bic	r6, r5, #32512	@ tmp270, tmp267,
	bic	r6, r6, #255	@ tmp270, tmp270,
	sxth	ip, sl	@ tmp269, tmp268
	cmp	ip, r6	@ tmp269, tmp270
	uxtheq	r8, r5	@ prephitmp.683, tmp267
	beq	.L335	@,
	cmp	r8, #0	@ D.15007,
	movw	ip, #32767	@,
	movlt	ip, #32768	@,
	mov	r8, ip	@ prephitmp.683,
.L335:
	subs	sl, r9, #1	@ ivtmp.733, ivtmp.708,
	strh	r8, [r1, #0]	@ movhi	@ prephitmp.683,* pInBuf
	mov	ip, #2	@ ivtmp.698,
	beq	.L290	@,
	ldr	r4, [fp, #-72]	@, %sfp
	cmp	r4, #0	@,
	beq	.L350	@,
	ldrh	r5, [r1, ip]	@,* ivtmp.698
	cmp	r3, #0	@ Volume,
	str	r5, [fp, #-88]	@, %sfp
	beq	.L312	@,
	ldr	r4, [fp, #-88]	@, %sfp
	ldr	r8, [fp, #-56]	@, %sfp
	sxth	r7, r4	@,
	str	r7, [fp, #-72]	@, %sfp
	mov	r4, r7	@, tmp5
	mov	r5, r4, asr #31	@
	ldr	r7, [fp, #-60]	@, %sfp
	strd	r4, [fp, #-52]	@,,
	mul	r6, r7, r5	@ tmp278,,
	ldr	r5, [fp, #-52]	@, %sfp
	mla	r4, r5, r8, r6	@ tmp279,,, tmp278
	mov	r8, #0	@,
	umull	r6, r7, r7, r5	@,, tmp6
	add	r5, r4, r7	@, tmp279,
	mov	r4, #16384	@,
	strd	r6, [fp, #-52]	@,,
	str	r5, [fp, #-48]	@, %sfp
	ldrd	r6, [fp, #-52]	@,,
	adds	r4, r4, r6	@,,
	adc	r5, r8, r7	@,,
	mov	r6, r4, lsr #15	@ tmp285,,
	strd	r4, [fp, #-52]	@,,
	orrs	r8, r6, r5, asl #17	@, tmp286, tmp285,,
	beq	.L312	@,
	ldr	r7, [fp, #-88]	@, %sfp
	bic	r6, r8, #32512	@ tmp289, tmp286,
	bic	r6, r6, #255	@ tmp289, tmp289,
	and	r4, r7, #32768	@ tmp287,,
	sxth	r4, r4	@ tmp288, tmp287
	cmp	r4, r6	@ tmp288, tmp289
	uxtheq	r8, r8	@ prephitmp.683, tmp286
	beq	.L338	@,
	ldr	r4, [fp, #-72]	@, %sfp
	movw	r8, #32767	@ prephitmp.683,
	cmp	r4, #0	@,
	movlt	r8, #32768	@ prephitmp.683,
.L338:
	subs	sl, sl, #1	@ ivtmp.733, ivtmp.733,
	strh	r8, [r1, ip]	@ movhi	@ prephitmp.683,* ivtmp.698
	add	ip, ip, #2	@ ivtmp.698, ivtmp.698,
	beq	.L290	@,
.L350:
	str	r0, [fp, #-72]	@ hFade, %sfp
	mov	r0, sl	@ ivtmp.733, ivtmp.733
	str	r9, [fp, #-52]	@ ivtmp.708, %sfp
	str	r2, [fp, #-92]	@ samples, %sfp
	ldrd	r4, [fp, #-60]	@ pretmp.681,,
	b	.L295	@
.L357:
	sxth	r6, r2	@, D.15007
	str	r6, [fp, #-88]	@, %sfp
	mov	r7, r6, asr #31	@ D.15007
	mul	r8, r4, r7	@ tmp169, pretmp.681, D.15007
	mla	r7, r6, r5, r8	@ tmp239, D.15007, pretmp.681, tmp169
	umull	r8, r9, r4, r6	@, pretmp.681, D.15007
	add	r6, r7, r9	@, tmp239,
	strd	r8, [fp, #-60]	@,,
	mov	r8, #16384	@,
	str	r6, [fp, #-56]	@, %sfp
	mov	r9, #0	@,
	ldrd	r6, [fp, #-60]	@,,
	adds	r6, r6, r8	@,,
	adc	r7, r7, r9	@,,
	mov	sl, r6, lsr #15	@ tmp202,,
	strd	r6, [fp, #-60]	@,,
	orrs	sl, sl, r7, asl #17	@, tmp241, tmp202,,
	beq	.L291	@,
	and	r9, r2, #32768	@ tmp179, D.15007,
	bic	r6, sl, #32512	@ tmp181, tmp241,
	bic	r6, r6, #255	@ tmp181, tmp181,
	sxth	r2, r9	@ tmp180, tmp179
	cmp	r2, r6	@ tmp180, tmp181
	uxtheq	r8, sl	@ prephitmp.683, tmp241
	beq	.L293	@,
	ldr	sl, [fp, #-88]	@, %sfp
	movw	r8, #32767	@ prephitmp.683,
	cmp	sl, #0	@,
	movlt	r8, #32768	@ prephitmp.683,
.L293:
	add	r2, ip, #2	@ tmp232, ivtmp.698,
	cmp	r3, #0	@ Volume,
	strh	r8, [r1, ip]	@ movhi	@ prephitmp.683,* ivtmp.698
	sub	r0, r0, #1	@ tmp231, ivtmp.733,
	ldrh	ip, [r1, r2]	@ D.15007,
	beq	.L316	@,
.L358:
	sxth	r6, ip	@, D.15007
	str	r6, [fp, #-88]	@, %sfp
	mov	r7, r6, asr #31	@ D.15007
	mul	r8, r4, r7	@ tmp296, pretmp.681, D.15007
	mla	sl, r6, r5, r8	@ tmp297, D.15007, pretmp.681, tmp296
	umull	r8, r9, r4, r6	@, pretmp.681, D.15007
	add	r7, sl, r9	@, tmp297,
	mov	sl, #0	@,
	strd	r8, [fp, #-60]	@,,
	mov	r8, #16384	@,
	str	r7, [fp, #-56]	@, %sfp
	ldrd	r6, [fp, #-60]	@,,
	adds	r6, r6, r8	@,,
	adc	r7, r7, sl	@,,
	mov	r9, r6, lsr #15	@ tmp303,,
	strd	r6, [fp, #-60]	@,,
	orrs	sl, r9, r7, asl #17	@, tmp304, tmp303,,
	beq	.L316	@,
	and	r9, ip, #32768	@ tmp305, D.15007,
	bic	r6, sl, #32512	@ tmp307, tmp304,
	bic	r6, r6, #255	@ tmp307, tmp307,
	sxth	ip, r9	@ tmp306, tmp305
	cmp	ip, r6	@ tmp306, tmp307
	uxtheq	r8, sl	@ prephitmp.683, tmp304
	beq	.L340	@,
	ldr	sl, [fp, #-88]	@, %sfp
	movw	r8, #32767	@ prephitmp.683,
	cmp	sl, #0	@,
	movlt	r8, #32768	@ prephitmp.683,
.L340:
	subs	r0, r0, #1	@ ivtmp.733, tmp231,
	strh	r8, [r1, r2]	@ movhi	@ prephitmp.683,
	add	ip, r2, #2	@ ivtmp.698, tmp232,
	beq	.L356	@,
.L295:
	cmp	r3, #0	@ Volume,
	ldrh	r2, [r1, ip]	@ D.15007,* ivtmp.698
	bne	.L357	@,
.L291:
	add	r2, ip, #2	@ tmp232, ivtmp.698,
	cmp	r3, #0	@ Volume,
	mov	r8, #0	@ prephitmp.683,
	sub	r0, r0, #1	@ tmp231, ivtmp.733,
	strh	r8, [r1, ip]	@ movhi	@ prephitmp.683,* ivtmp.698
	ldrh	ip, [r1, r2]	@ D.15007,
	bne	.L358	@,
.L316:
	subs	r0, r0, #1	@ ivtmp.733, tmp231,
	mov	r8, #0	@ prephitmp.683,
	add	ip, r2, #2	@ ivtmp.698, tmp232,
	strh	r8, [r1, r2]	@ movhi	@ prephitmp.683,
	bne	.L295	@,
.L356:
	ldr	r0, [fp, #-72]	@ hFade, %sfp
	ldr	r2, [fp, #-92]	@ samples, %sfp
	ldr	r9, [fp, #-52]	@ ivtmp.708, %sfp
.L290:
	ldr	r3, [r0, #20]	@ <variable>.u32FadeCnt, <variable>.u32FadeCnt
	sub	r2, r2, #1	@ samples, samples,
	ldr	ip, [r0, #24]	@ <variable>.u32FadeStep, <variable>.u32FadeStep
	add	r3, r3, #1	@ D.14784, <variable>.u32FadeCnt,
	ldr	sl, [fp, #-80]	@, %sfp
	cmp	r3, ip	@ D.14784, <variable>.u32FadeStep
	str	r3, [r0, #20]	@ D.14784, <variable>.u32FadeCnt
	add	r1, r1, sl	@ pInBuf, pInBuf,
	bne	.L302	@,
.L354:
	ldr	r3, [r0, #28]	@ temp.730, <variable>.enCurnFadeStauts
	mov	r6, #0	@,
	str	r6, [r0, #20]	@, <variable>.u32FadeCnt
	cmp	r3, #1	@ temp.730,
	beq	.L359	@,
	cmp	r3, #2	@ temp.730,
	bne	.L302	@,
	ldr	r3, [r0, #16]	@ <variable>.u32FadeCurnCoefIdx, <variable>.u32FadeCurnCoefIdx
	add	ip, r3, #1	@ D.14796, <variable>.u32FadeCurnCoefIdx,
	str	ip, [r0, #16]	@ D.14796, <variable>.u32FadeCurnCoefIdx
	cmp	ip, #87	@ D.14796,
	moveq	r7, #0	@,
	streq	r7, [r0, #28]	@, <variable>.enCurnFadeStauts
	cmp	r2, #0	@ samples,
	bne	.L304	@,
.L305:
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, sl, fp, sp, pc}
.L353:
	cmp	r9, #0	@ ivtmp.708,
	beq	.L296	@,
	cmp	r3, #0	@ Volume,
	sub	ip, r9, #1	@ tmp233, ivtmp.708,
	and	r6, ip, #1	@ tmp234, tmp233,
	ldr	r8, [r1, #0]	@ data,* pInBuf
	beq	.L319	@,
	mov	r8, r8, asr #8	@ D.15025, data,
	mov	r4, #16384	@ tmp313,
	mov	r5, #0	@,
	smlal	r4, r5, r8, r3	@ tmp313, D.15025, Volume
	mov	ip, r4, lsr #15	@ tmp315, tmp313,
	orrs	ip, ip, r5, asl #17	@, tmp316, tmp315,,
	beq	.L319	@,
	eor	r4, ip, r8	@ tmp317, tmp316, D.15025
	mov	r5, r4, lsr #23	@ tmp318, tmp317,
	mov	r4, r5, asl #23	@ tmp318, tmp318,
	cmp	r4, #0	@ tmp318,
	moveq	ip, ip, asl #8	@ prephitmp.680, tmp316,
	beq	.L342	@,
	mov	sl, r8, lsr #23	@ tmp319, D.15025,
	mov	ip, sl, asl #23	@ tmp319, tmp319,
	ldr	sl, [fp, #-68]	@, %sfp
	cmp	ip, #0	@ tmp319,
	movne	sl, #-2147483648	@,
	mov	ip, sl	@ prephitmp.680,
.L342:
	subs	r5, r9, #1	@ ivtmp.734, ivtmp.708,
	str	ip, [r1, #0]	@ prephitmp.680,* pInBuf
	mov	ip, #4	@ ivtmp.714,
	beq	.L296	@,
	cmp	r6, #0	@ tmp234,
	beq	.L301	@,
	cmp	r3, #0	@ Volume,
	ldr	r8, [r1, ip]	@ data,* ivtmp.714
	beq	.L322	@,
	mov	r8, r8, asr #8	@ D.15025, data,
	mov	r6, #16384	@ tmp327,
	mov	r7, #0	@,
	smlal	r6, r7, r8, r3	@ tmp327, D.15025, Volume
	mov	r4, r6, lsr #15	@ tmp329, tmp327,
	orrs	r4, r4, r7, asl #17	@, tmp330, tmp329,,
	beq	.L322	@,
	eor	r6, r4, r8	@ tmp331, tmp330, D.15025
	mov	r6, r6, lsr #23	@ tmp332, tmp331,
	mov	r6, r6, asl #23	@ tmp332, tmp332,
	cmp	r6, #0	@ tmp332,
	moveq	r4, r4, asl #8	@ prephitmp.680, tmp330,
	beq	.L345	@,
	mov	r6, r8, lsr #23	@ tmp333, D.15025,
	mov	r4, r6, asl #23	@ tmp333, tmp333,
	ldr	r6, [fp, #-68]	@, %sfp
	cmp	r4, #0	@ tmp333,
	movne	r6, #-2147483648	@,
	mov	r4, r6	@ prephitmp.680,
.L345:
	subs	r5, r5, #1	@ ivtmp.734, ivtmp.734,
	str	r4, [r1, ip]	@ prephitmp.680,* ivtmp.714
	add	ip, ip, #4	@ ivtmp.714, ivtmp.714,
	bne	.L301	@,
.L296:
	ldr	r5, [fp, #-76]	@, %sfp
	add	r1, r1, r5	@ pInBuf, pInBuf,
	b	.L287	@
.L360:
	mov	r8, r8, asr #8	@ D.15025, data,
	mov	r6, #16384	@ tmp185,
	mov	r7, #0	@,
	smlal	r6, r7, r8, r3	@ tmp185, D.15025, Volume
	mov	r4, r6, lsr #15	@ tmp204, tmp185,
	orrs	r4, r4, r7, asl #17	@, tmp248, tmp204,,
	beq	.L297	@,
	eor	r6, r4, r8	@ tmp190, tmp248, D.15025
	mov	r6, r6, lsr #23	@ tmp191, tmp190,
	mov	r6, r6, asl #23	@ tmp191, tmp191,
	cmp	r6, #0	@ tmp191,
	moveq	r8, r4, asl #8	@ prephitmp.680, tmp248,
	beq	.L299	@,
	mov	r4, r8, lsr #23	@ tmp193, D.15025,
	mov	r8, r4, asl #23	@ tmp193, tmp193,
	ldr	r4, [fp, #-68]	@, %sfp
	cmp	r8, #0	@ tmp193,
	movne	r4, #-2147483648	@,
	mov	r8, r4	@ prephitmp.680,
.L299:
	add	r4, ip, #4	@ tmp235, ivtmp.714,
	cmp	r3, #0	@ Volume,
	str	r8, [r1, ip]	@ prephitmp.680,* ivtmp.714
	sub	r5, r5, #1	@ tmp236, ivtmp.734,
	ldr	r8, [r1, r4]	@ data,
	beq	.L326	@,
.L361:
	mov	r8, r8, asr #8	@ D.15025, data,
	mov	r6, #16384	@ tmp340,
	mov	r7, #0	@,
	smlal	r6, r7, r8, r3	@ tmp340, D.15025, Volume
	mov	ip, r6, lsr #15	@ tmp342, tmp340,
	orrs	ip, ip, r7, asl #17	@, tmp343, tmp342,,
	beq	.L326	@,
	eor	r6, ip, r8	@ tmp344, tmp343, D.15025
	mov	r6, r6, lsr #23	@ tmp345, tmp344,
	mov	r6, r6, asl #23	@ tmp345, tmp345,
	cmp	r6, #0	@ tmp345,
	moveq	ip, ip, asl #8	@ prephitmp.680, tmp343,
	beq	.L347	@,
	mov	r7, r8, lsr #23	@ tmp346, D.15025,
	mov	ip, r7, asl #23	@ tmp346, tmp346,
	ldr	r7, [fp, #-68]	@, %sfp
	cmp	ip, #0	@ tmp346,
	movne	r7, #-2147483648	@,
	mov	ip, r7	@ prephitmp.680,
.L347:
	subs	r5, r5, #1	@ ivtmp.734, tmp236,
	str	ip, [r1, r4]	@ prephitmp.680,
	add	ip, r4, #4	@ ivtmp.714, tmp235,
	beq	.L296	@,
.L301:
	cmp	r3, #0	@ Volume,
	ldr	r8, [r1, ip]	@ data,* ivtmp.714
	bne	.L360	@,
.L297:
	add	r4, ip, #4	@ tmp235, ivtmp.714,
	cmp	r3, #0	@ Volume,
	mov	r8, #0	@ prephitmp.680,
	sub	r5, r5, #1	@ tmp236, ivtmp.734,
	str	r8, [r1, ip]	@ prephitmp.680,* ivtmp.714
	ldr	r8, [r1, r4]	@ data,
	bne	.L361	@,
.L326:
	mov	ip, #0	@ prephitmp.680,
	b	.L347	@
.L309:
	mov	r8, #0	@ prephitmp.683,
	b	.L335	@
.L319:
	mov	ip, #0	@ prephitmp.680,
	b	.L342	@
.L322:
	subs	r5, r5, #1	@ ivtmp.734, ivtmp.734,
	mov	r4, #0	@ prephitmp.680,
	str	r4, [r1, ip]	@ prephitmp.680,* ivtmp.714
	add	ip, ip, #4	@ ivtmp.714, ivtmp.714,
	bne	.L301	@,
	b	.L296	@
.L312:
	mov	r8, #0	@ prephitmp.683,
	b	.L338	@
.L359:
	ldr	r3, [r0, #16]	@ <variable>.u32FadeCurnCoefIdx, <variable>.u32FadeCurnCoefIdx
	sub	ip, r3, #1	@ D.14790, <variable>.u32FadeCurnCoefIdx,
	str	ip, [r0, #16]	@ D.14790, <variable>.u32FadeCurnCoefIdx
	cmp	ip, #6	@ D.14790,
	streq	r6, [r0, #28]	@, <variable>.enCurnFadeStauts
	b	.L302	@
	.fnend
	.size	FADE_Process, .-FADE_Process
	.align	2
	.global	FADE_Destroy
	.type	FADE_Destroy, %function
FADE_Destroy:
	.fnstart
.LFB811:
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
	subs	r1, r0, #0	@ hFade, hFade
	ldmeqfd	sp, {fp, sp, pc}	@
	mov	r0, #1	@,
	bl	HI_FREE	@
	ldmfd	sp, {fp, sp, pc}	@
	.fnend
	.size	FADE_Destroy, .-FADE_Destroy
	.align	2
	.global	FADE_Create
	.type	FADE_Create, %function
FADE_Create:
	.fnstart
.LFB809:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	.movsp ip
	mov	ip, sp	@,
	stmfd	sp!, {r4, r5, r6, r7, r8, sl, fp, ip, lr, pc}	@,
	.pad #4
	.save {r4, r5, r6, r7, r8, sl, fp, ip, lr}
	.setfp fp, ip, #-4
	sub	fp, ip, #4	@,,
	push	{lr}
	bl	__gnu_mcount_nc
	mov	r5, r1	@ inRate, inRate
	movw	r1, #:lower16:191999	@ tmp142,
	sub	ip, r5, #1	@ tmp141, inRate,
	movt	r1, #:upper16:191999	@ tmp142,
	mov	r7, r3	@ Channels, Channels
	cmp	ip, r1	@ tmp141, tmp142
	mov	r3, #0	@ tmp140,
	mov	r4, r0	@ phFade, phFade
	mov	r6, r2	@ BitPerSample, BitPerSample
	str	r3, [r0, #0]	@ tmp140,* phFade
	ldmhifd	sp, {r4, r5, r6, r7, r8, sl, fp, sp, pc}	@
	cmp	r2, #24	@ BitPerSample,
	cmpne	r2, #16	@, BitPerSample,
	ldmnefd	sp, {r4, r5, r6, r7, r8, sl, fp, sp, pc}	@
	sub	sl, r7, #1	@ tmp150, Channels,
	cmp	r7, #8	@ Channels,
	cmpne	sl, #1	@, tmp150,
	movls	sl, #0	@, tmp157
	movhi	sl, #1	@, tmp157
	ldmhifd	sp, {r4, r5, r6, r7, r8, sl, fp, sp, pc}	@
	mov	r0, #1	@,
	mov	r1, #32	@,
	bl	HI_MALLOC	@
	subs	r8, r0, #0	@ D.14681,
	ldmeqfd	sp, {r4, r5, r6, r7, r8, sl, fp, sp, pc}	@
	mov	r1, #32	@,
	bl	__memzero	@
	stmia	r8, {r6, r7}	@ phole stm
	str	r5, [r8, #8]	@ inRate, <variable>.u32InRate
	str	sl, [r8, #28]	@ tmp157, <variable>.enCurnFadeStauts
	str	r8, [r4, #0]	@ D.14681,* phFade
	ldmfd	sp, {r4, r5, r6, r7, r8, sl, fp, sp, pc}	@
	.fnend
	.size	FADE_Create, .-FADE_Create
	.section	.rodata
	.align	2
.LANCHOR0 = . + 0
	.type	g_u16VolCoef, %object
	.size	g_u16VolCoef, 176
g_u16VolCoef:
	.short	-156
	.short	-7266
	.short	-13603
	.short	-19250
	.short	-24284
	.short	-28770
	.short	-32768
	.short	29204
	.short	26028
	.short	23197
	.short	20675
	.short	18426
	.short	16422
	.short	14636
	.short	13045
	.short	11626
	.short	10362
	.short	9235
	.short	8230
	.short	7335
	.short	6538
	.short	5827
	.short	5193
	.short	4628
	.short	4125
	.short	3676
	.short	3276
	.short	2920
	.short	2602
	.short	2319
	.short	2067
	.short	1842
	.short	1642
	.short	1463
	.short	1304
	.short	1162
	.short	1036
	.short	923
	.short	823
	.short	733
	.short	653
	.short	582
	.short	519
	.short	462
	.short	412
	.short	367
	.short	327
	.short	292
	.short	260
	.short	231
	.short	206
	.short	184
	.short	164
	.short	146
	.short	130
	.short	116
	.short	103
	.short	92
	.short	82
	.short	73
	.short	65
	.short	58
	.short	51
	.short	46
	.short	41
	.short	36
	.short	32
	.short	29
	.short	26
	.short	23
	.short	20
	.short	18
	.short	16
	.short	14
	.short	13
	.short	11
	.short	10
	.short	9
	.short	8
	.short	7
	.short	6
	.short	5
	.short	5
	.short	4
	.short	4
	.short	3
	.short	3
	.short	0
	.ident	"GCC: (Hisilicon_v200(gcc4.4-290+glibc-2.11+eabi+nptl)) 4.4.1"
	.section	.note.GNU-stack,"",%progbits

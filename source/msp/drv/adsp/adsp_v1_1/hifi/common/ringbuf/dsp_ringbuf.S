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
	.file	"dsp_ringbuf.c"
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
@ -DKBUILD_STR(s)=#s -DKBUILD_BASENAME=KBUILD_STR(dsp_ringbuf)
@ -DKBUILD_MODNAME=KBUILD_STR(hi_adsp) -isystem
@ /opt/hisi-linux/x86-arm/arm-hisiv200-linux/bin/../lib/gcc/arm-hisiv200-linux-gnueabi/4.4.1/include
@ -include
@ /home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/include/linux/kconfig.h
@ -MD drivers/msp/adsp/adsp_v1_1/./hifi/common/ringbuf/.dsp_ringbuf.o.d
@ /home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/drivers/msp/adsp/adsp_v1_1/./hifi/common/ringbuf/dsp_ringbuf.c
@ -mlittle-endian -marm -mapcs -mno-sched-prolog -mabi=aapcs-linux
@ -mno-thumb-interwork -march=armv7-a -msoft-float -auxbase-strip
@ drivers/msp/adsp/adsp_v1_1/./hifi/common/ringbuf/dsp_ringbuf.o -O2 -Wall
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
	.global	RBUF_ReadDataComplete
	.type	RBUF_ReadDataComplete, %function
RBUF_ReadDataComplete:
	.fnstart
.LFB18:
	@ args = 0, pretend = 0, frame = 16
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
	cmp	r0, #0	@ hRBUF
	beq	.L10	@,
	mov	r2, #0	@ tmp144,
	str	r2, [fp, #-36]	@ tmp144, u32RdLen
	str	r2, [fp, #-32]	@ tmp144, u32RdLen
	str	r2, [fp, #-40]	@ tmp144, u32RdPos
	ldr	r4, [r0, #40]	@ D.2173, <variable>.stCB.pu32Read
	ldr	ip, [r0, #36]	@ D.2172, <variable>.stCB.pu32Write
	ldr	r3, [r4, #0]	@ u32ReadPos,* D.2173
	ldr	r5, [ip, #0]	@ u32WritePos,* D.2172
	cmp	r3, r5	@ u32ReadPos, u32WritePos
	str	r3, [fp, #-44]	@ u32ReadPos, u32RdPos
	bhi	.L3	@,
	add	r2, r1, r3	@ tmp147, amountRead, u32ReadPos
	cmp	r5, r2	@ u32WritePos, tmp147
	strcs	r1, [fp, #-36]	@ amountRead, u32RdLen
	rsbcc	ip, r3, r5	@ amountRead.45, u32ReadPos, u32WritePos
	strcc	ip, [fp, #-36]	@ amountRead.45, u32RdLen
	movcs	ip, r1	@ amountRead.45, amountRead
.L5:
	cmp	ip, #0	@ amountRead.45,
	beq	.L7	@,
	ldr	r2, [fp, #-32]	@ pretmp.33, u32RdLen
	ldr	r3, [fp, #-44]	@ u32RdPos, u32RdPos
	cmp	r2, #0	@ pretmp.33,
	add	r3, ip, r3	@ u32ReadPos, amountRead.45, u32RdPos
	ldrne	ip, [fp, #-40]	@ u32RdPos, u32RdPos
	addne	r3, r2, ip	@ u32ReadPos, pretmp.33, u32RdPos
.L7:
	ldr	r2, [r0, #48]	@ <variable>.stCB.u32Lenght, <variable>.stCB.u32Lenght
	cmp	r2, r3	@ <variable>.stCB.u32Lenght, u32ReadPos
	moveq	r3, #0	@ u32ReadPos,
	str	r3, [r4, #0]	@ u32ReadPos,* D.2173
	ldr	r3, [r0, #52]	@ <variable>.stProc.uTotalByteRead, <variable>.stProc.uTotalByteRead
	add	r1, r3, r1	@ tmp154, <variable>.stProc.uTotalByteRead, amountRead
	str	r1, [r0, #52]	@ tmp154, <variable>.stProc.uTotalByteRead
.L10:
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L3:
	ldr	ip, [r0, #48]	@ D.2169, <variable>.stCB.u32Lenght
	add	r6, r1, r3	@ tmp148, amountRead, u32ReadPos
	cmp	r6, ip	@ tmp148, D.2169
	movls	ip, r1	@ amountRead.45, amountRead
	strls	r1, [fp, #-36]	@ amountRead, u32RdLen
	bls	.L5	@
.L6:
	rsb	ip, r3, ip	@ amountRead.45, u32ReadPos, D.2169
	str	r2, [fp, #-40]	@ tmp144, u32RdPos
	str	ip, [fp, #-36]	@ amountRead.45, u32RdLen
	rsb	r2, ip, r1	@ D.2166, amountRead.45, amountRead
	cmp	r5, r2	@ u32WritePos, D.2166
	strls	r5, [fp, #-32]	@, u32WritePos, u32RdLen
	strhi	r2, [fp, #-32]	@, D.2166, u32RdLen
	b	.L5	@
	.fnend
	.size	RBUF_ReadDataComplete, .-RBUF_ReadDataComplete
	.align	2
	.global	RBUF_WriteDataComplete
	.type	RBUF_WriteDataComplete, %function
RBUF_WriteDataComplete:
	.fnstart
.LFB19:
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
	cmp	r0, #0	@ hRBUF
	beq	.L19	@,
	mov	r2, #0	@ tmp143,
	str	r2, [fp, #-28]	@ tmp143, u32WtLen
	str	r2, [fp, #-24]	@ tmp143, u32WtLen
	str	r2, [fp, #-32]	@ tmp143, u32WtPos
	ldr	r3, [r0, #40]	@ D.2189, <variable>.stCB.pu32Read
	ldr	r4, [r0, #36]	@ D.2188, <variable>.stCB.pu32Write
	ldr	ip, [r3, #0]	@ u32ReadPos,* D.2189
	ldr	r3, [r4, #0]	@ u32WritePos,* D.2188
	cmp	ip, r3	@ u32ReadPos, u32WritePos
	strhi	r1, [fp, #-28]	@ amountWritten, u32WtLen
	str	r3, [fp, #-36]	@ u32WritePos, u32WtPos
	movhi	ip, r1	@ amountWritten.75, amountWritten
	bls	.L20	@,
.L16:
	cmp	ip, #0	@ amountWritten.75,
	beq	.L17	@,
	ldr	r2, [fp, #-24]	@ pretmp.63, u32WtLen
	ldr	r3, [fp, #-36]	@ u32WtPos, u32WtPos
	cmp	r2, #0	@ pretmp.63,
	add	r3, ip, r3	@ u32WritePos, amountWritten.75, u32WtPos
	ldrne	r3, [fp, #-32]	@ u32WtPos, u32WtPos
	addne	r3, r2, r3	@ u32WritePos, pretmp.63, u32WtPos
.L17:
	ldr	r2, [r0, #48]	@ <variable>.stCB.u32Lenght, <variable>.stCB.u32Lenght
	cmp	r2, r3	@ <variable>.stCB.u32Lenght, u32WritePos
	moveq	r3, #0	@ u32WritePos,
	str	r3, [r4, #0]	@ u32WritePos,* D.2188
	ldr	ip, [r0, #68]	@ <variable>.stProc.uTotalByteWrite, <variable>.stProc.uTotalByteWrite
	add	r1, ip, r1	@ tmp153, <variable>.stProc.uTotalByteWrite, amountWritten
	str	r1, [r0, #68]	@ tmp153, <variable>.stProc.uTotalByteWrite
.L19:
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L20:
	ldr	ip, [r0, #48]	@ D.2187, <variable>.stCB.u32Lenght
	add	r5, r1, r3	@ tmp146, amountWritten, u32WritePos
	cmp	ip, r5	@ D.2187, tmp146
	strcc	r2, [fp, #-32]	@ tmp143, u32WtPos
	rsbcc	ip, r3, ip	@ amountWritten.75, u32WritePos, D.2187
	strcs	r1, [fp, #-28]	@ amountWritten, u32WtLen
	rsbcc	r2, ip, r1	@ tmp147, amountWritten.75, amountWritten
	movcs	ip, r1	@ amountWritten.75, amountWritten
	strcc	ip, [fp, #-28]	@ amountWritten.75, u32WtLen
	strcc	r2, [fp, #-24]	@ tmp147, u32WtLen
	b	.L16	@
	.fnend
	.size	RBUF_WriteDataComplete, .-RBUF_WriteDataComplete
	.align	2
	.global	RBUF_QueryInfo
	.type	RBUF_QueryInfo, %function
RBUF_QueryInfo:
	.fnstart
.LFB20:
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
	subs	r3, r0, #0	@ hRBUF, hRBUF
	beq	.L31	@,
	ldr	ip, [r3, #28]	@ <variable>.stAttr.uLatencySize, <variable>.stAttr.uLatencySize
	ldr	r0, [r3, #24]	@ u32Gap, <variable>.stAttr.uFrameSize
	str	ip, [r1, #32]	@ <variable>.stAttr.uLatencySize, <variable>.u32LatencySize
	ldr	r2, [r3, #44]	@ <variable>.stCB.pu8Data, <variable>.stCB.pu8Data
	str	r2, [r1, #0]	@ <variable>.stCB.pu8Data, <variable>.u32BasePtr
	ldr	ip, [r3, #48]	@ <variable>.stCB.u32Lenght, <variable>.stCB.u32Lenght
	str	ip, [r1, #12]	@ <variable>.stCB.u32Lenght, <variable>.u32BufSize
	ldr	r2, [r3, #36]	@ D.2131, <variable>.stCB.pu32Write
	ldr	ip, [r2, #0]	@ D.2132,* D.2131
	str	ip, [r1, #8]	@ D.2132, <variable>.u32WritePtr
	ldr	r2, [r3, #40]	@ D.2133, <variable>.stCB.pu32Read
	ldr	ip, [r2, #0]	@ D.2134,* D.2133
	str	ip, [r1, #4]	@ D.2134, <variable>.u32ReadPtr
	ldr	r2, [r3, #40]	@ D.2200, <variable>.stCB.pu32Read
	ldr	ip, [r3, #36]	@ D.2199, <variable>.stCB.pu32Write
	ldr	r2, [r2, #0]	@ u32ReadPos,* D.2200
	ldr	ip, [ip, #0]	@ u32WritePos,* D.2199
	cmp	r2, ip	@ u32ReadPos, u32WritePos
	rsbls	r2, r2, ip	@ u32BusyLen, u32ReadPos, u32WritePos
	strls	r2, [r1, #20]	@ u32BusyLen, <variable>.u32TotalDataSize
	ldrhi	r4, [r3, #48]	@ D.2198, <variable>.stCB.u32Lenght
	strls	r2, [r1, #16]	@ u32BusyLen, <variable>.u32ContiguousDataSize
	addhi	ip, ip, r4	@ tmp158, u32WritePos, D.2198
	rsbhi	r4, r2, r4	@ tmp157, u32ReadPos, D.2198
	rsbhi	r2, r2, ip	@ tmp159, u32ReadPos, tmp158
	strhi	r4, [r1, #16]	@ tmp157, <variable>.u32ContiguousDataSize
	strhi	r2, [r1, #20]	@ tmp159, <variable>.u32TotalDataSize
	ldr	ip, [r3, #40]	@ D.2215, <variable>.stCB.pu32Read
	ldr	r2, [r3, #36]	@ D.2214, <variable>.stCB.pu32Write
	ldr	ip, [ip, #0]	@ u32ReadPos,* D.2215
	ldr	r2, [r2, #0]	@ u32WritePos,* D.2214
	cmp	ip, r2	@ u32ReadPos, u32WritePos
	rsbhi	r3, r2, ip	@ u32FreeLen, u32WritePos, u32ReadPos
	strhi	r3, [r1, #24]	@ u32FreeLen, <variable>.u32ContiguousFreeSize
	ldrls	r4, [r3, #48]	@ D.2213, <variable>.stCB.u32Lenght
	addls	r3, ip, r4	@ tmp160, u32ReadPos, D.2213
	rsbls	r4, r2, r4	@ tmp161, u32WritePos, D.2213
	rsbls	r3, r2, r3	@ u32FreeLen, u32WritePos, tmp160
	strls	r4, [r1, #24]	@ tmp161, <variable>.u32ContiguousFreeSize
	cmp	r0, r3	@ u32Gap, u32FreeLen
	str	r3, [r1, #28]	@ u32FreeLen, <variable>.u32TotalFreeSize
	movcs	r3, #0	@ tmp162,
	strcs	r3, [r1, #28]	@ tmp162, <variable>.u32TotalFreeSize
	strcs	r3, [r1, #24]	@ tmp162, <variable>.u32ContiguousFreeSize
	ldmcsfd	sp, {r3, r4, fp, sp, pc}	@
	ldr	ip, [r1, #28]	@ <variable>.u32TotalFreeSize, <variable>.u32TotalFreeSize
	rsb	r3, r0, r3	@ u32FreeLen.102, u32Gap, u32FreeLen
	ldr	r2, [r1, #24]	@ <variable>.u32ContiguousFreeSize, <variable>.u32ContiguousFreeSize
	str	r3, [r1, #28]	@ u32FreeLen.102, <variable>.u32TotalFreeSize
	rsb	r2, r2, ip	@ tmp164, <variable>.u32ContiguousFreeSize, <variable>.u32TotalFreeSize
	cmp	r0, r2	@ u32Gap, tmp164
	strhi	r3, [r1, #24]	@ u32FreeLen.102, <variable>.u32ContiguousFreeSize
	ldmfd	sp, {r3, r4, fp, sp, pc}	@
.L31:
	mov	r0, r1	@, pstBufInfo
	mov	r1, #36	@,
	bl	__memzero	@
	ldmfd	sp, {r3, r4, fp, sp, pc}	@
	.fnend
	.size	RBUF_QueryInfo, .-RBUF_QueryInfo
	.align	2
	.global	RBUF_WriteData
	.type	RBUF_WriteData, %function
RBUF_WriteData:
	.fnstart
.LFB17:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	.movsp ip
	mov	ip, sp	@,
	stmfd	sp!, {r4, r5, r6, r7, r8, sl, fp, ip, lr, pc}	@,
	.pad #4
	.save {r4, r5, r6, r7, r8, sl, fp, ip, lr}
	.setfp fp, ip, #-4
	sub	fp, ip, #4	@,,
	.pad #16
	sub	sp, sp, #16	@,,
	push	{lr}
	bl	__gnu_mcount_nc
	subs	r4, r0, #0	@ hRBUF, hRBUF
	mov	r7, r1	@ pu32Src, pu32Src
	beq	.L33	@,
	ldr	ip, [r4, #72]	@ <variable>.stProc.uTryWriteCnt, <variable>.stProc.uTryWriteCnt
	mov	sl, #0	@ tmp153,
	add	r8, ip, #1	@ tmp152, <variable>.stProc.uTryWriteCnt,
	str	r8, [r4, #72]	@ tmp152, <variable>.stProc.uTryWriteCnt
	str	sl, [fp, #-44]	@ tmp153, u32WtLen
	str	sl, [fp, #-40]	@ tmp153, u32WtLen
	str	sl, [fp, #-48]	@ tmp153, u32WtPos
	ldr	r0, [r4, #40]	@ D.2239, <variable>.stCB.pu32Read
	ldr	r1, [r4, #36]	@ D.2238, <variable>.stCB.pu32Write
	ldr	r3, [r0, #0]	@ u32ReadPos,* D.2239
	ldr	r8, [r1, #0]	@ u32WritePos,* D.2238
	cmp	r3, r8	@ u32ReadPos, u32WritePos
	strhi	r2, [fp, #-44]	@ u3SrcLen, u32WtLen
	str	r8, [fp, #-52]	@ u32WritePos, u32WtPos
	movhi	r5, r2	@ u3SrcLen.130, u3SrcLen
	bls	.L41	@,
.L36:
	cmp	r5, #0	@ u3SrcLen.130,
	beq	.L37	@,
	ldr	r8, [fp, #-52]	@ temp.135, u32WtPos
	mov	r1, r7	@, pu32Src
	ldr	r6, [r4, #44]	@ <variable>.stCB.pu8Data, <variable>.stCB.pu8Data
	mov	r2, r5	@, u3SrcLen.130
	add	r0, r8, r6	@, temp.135, <variable>.stCB.pu8Data
	add	r8, r5, r8	@ u32WritePos, u3SrcLen.130, temp.135
	bl	memcpy	@
	ldr	r6, [fp, #-40]	@ pretmp.118, u32WtLen
	add	r1, r7, r5	@ pu32Src.138, pu32Src, u3SrcLen.130
	cmp	r6, #0	@ pretmp.118,
	beq	.L37	@,
	ldr	r8, [fp, #-48]	@ D.2230, u32WtPos
	mov	r2, r6	@, pretmp.118
	ldr	lr, [r4, #44]	@ <variable>.stCB.pu8Data, <variable>.stCB.pu8Data
	add	r0, r8, lr	@, D.2230, <variable>.stCB.pu8Data
	add	r8, r8, r6	@ u32WritePos, D.2230, pretmp.118
	bl	memcpy	@
.L37:
	ldr	r2, [r4, #48]	@ <variable>.stCB.u32Lenght, <variable>.stCB.u32Lenght
	ldr	r1, [r4, #36]	@ temp.132, <variable>.stCB.pu32Write
	cmp	r2, r8	@ <variable>.stCB.u32Lenght, u32WritePos
	moveq	r8, #0	@ u32WritePos,
	adds	r5, r5, sl	@ Bytes, u3SrcLen.130, prephitmp.117
	str	r8, [r1, #0]	@ u32WritePos,* temp.132
	ldreq	r1, [r4, #76]	@ <variable>.stProc.uBufFullCnt, <variable>.stProc.uBufFullCnt
	mov	r2, r5	@ u3SrcLen, Bytes
	ldr	sl, [r4, #68]	@ <variable>.stProc.uTotalByteWrite, <variable>.stProc.uTotalByteWrite
	addeq	r1, r1, #1	@ tmp172, <variable>.stProc.uBufFullCnt,
	add	r5, sl, r5	@ tmp174, <variable>.stProc.uTotalByteWrite, Bytes
	streq	r1, [r4, #76]	@ tmp172, <variable>.stProc.uBufFullCnt
	str	r5, [r4, #68]	@ tmp174, <variable>.stProc.uTotalByteWrite
.L33:
	mov	r0, r2	@, u3SrcLen
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, sl, fp, sp, pc}
.L41:
	ldr	r5, [r4, #48]	@ D.2237, <variable>.stCB.u32Lenght
	add	r3, r2, r8	@ tmp156, u3SrcLen, u32WritePos
	cmp	r5, r3	@ D.2237, tmp156
	strcc	sl, [fp, #-48]	@ tmp153, u32WtPos
	rsbcc	r5, r8, r5	@ u3SrcLen.130, u32WritePos, D.2237
	strcs	r2, [fp, #-44]	@ u3SrcLen, u32WtLen
	rsbcc	sl, r5, r2	@ prephitmp.117, u3SrcLen.130, u3SrcLen
	movcs	r5, r2	@ u3SrcLen.130, u3SrcLen
	strcc	r5, [fp, #-44]	@ u3SrcLen.130, u32WtLen
	strcc	sl, [fp, #-40]	@ prephitmp.117, u32WtLen
	b	.L36	@
	.fnend
	.size	RBUF_WriteData, .-RBUF_WriteData
	.align	2
	.global	RBUF_ReadData
	.type	RBUF_ReadData, %function
RBUF_ReadData:
	.fnstart
.LFB16:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	.movsp ip
	mov	ip, sp	@,
	stmfd	sp!, {r4, r5, r6, r7, r8, sl, fp, ip, lr, pc}	@,
	.pad #4
	.save {r4, r5, r6, r7, r8, sl, fp, ip, lr}
	.setfp fp, ip, #-4
	sub	fp, ip, #4	@,,
	.pad #16
	sub	sp, sp, #16	@,,
	push	{lr}
	bl	__gnu_mcount_nc
	subs	r4, r0, #0	@ hRBUF, hRBUF
	mov	r8, r1	@ pu32Dest, pu32Dest
	moveq	r0, r4	@ Bytes, hRBUF
	beq	.L44	@,
	ldr	sl, [r4, #56]	@ <variable>.stProc.uTryReadCnt, <variable>.stProc.uTryReadCnt
	mov	r3, #0	@ tmp153,
	add	r6, sl, #1	@ tmp152, <variable>.stProc.uTryReadCnt,
	str	r6, [r4, #56]	@ tmp152, <variable>.stProc.uTryReadCnt
	str	r3, [fp, #-44]	@ tmp153, u32RdLen
	str	r3, [fp, #-40]	@ tmp153, u32RdLen
	str	r3, [fp, #-48]	@ tmp153, u32RdPos
	ldr	r0, [r4, #40]	@ D.2262, <variable>.stCB.pu32Read
	ldr	r1, [r4, #36]	@ D.2261, <variable>.stCB.pu32Write
	ldr	sl, [r0, #0]	@ u32ReadPos,* D.2262
	ldr	r6, [r1, #0]	@ u32WritePos,* D.2261
	cmp	sl, r6	@ u32ReadPos, u32WritePos
	str	sl, [fp, #-52]	@ u32ReadPos, u32RdPos
	bhi	.L45	@,
	add	ip, r2, sl	@ tmp156, u32DestSize, u32ReadPos
	cmp	r6, ip	@ u32WritePos, tmp156
	strcs	r2, [fp, #-44]	@ u32DestSize, u32RdLen
	rsbcc	r5, sl, r6	@ u32DestSize.166, u32ReadPos, u32WritePos
	strcc	r5, [fp, #-44]	@ u32DestSize.166, u32RdLen
	movcs	r5, r2	@ u32DestSize.166, u32DestSize
	movcs	r6, r3	@ u32WritePos, tmp153
	movcc	r6, r3	@ u32WritePos, tmp153
.L47:
	cmp	r5, #0	@ u32DestSize.166,
	beq	.L49	@,
	ldr	sl, [fp, #-52]	@ D.2252, u32RdPos
	mov	r0, r8	@, pu32Dest
	ldr	r3, [r4, #44]	@ <variable>.stCB.pu8Data, <variable>.stCB.pu8Data
	mov	r2, r5	@, u32DestSize.166
	add	r1, sl, r3	@, D.2252, <variable>.stCB.pu8Data
	add	sl, r5, sl	@ u32ReadPos, u32DestSize.166, D.2252
	bl	memcpy	@
	ldr	r7, [fp, #-40]	@ pretmp.154, u32RdLen
	add	r0, r8, r5	@ pu32Dest.174, pu32Dest, u32DestSize.166
	cmp	r7, #0	@ pretmp.154,
	beq	.L49	@,
	ldr	sl, [fp, #-48]	@ temp.170, u32RdPos
	mov	r2, r7	@, pretmp.154
	ldr	lr, [r4, #44]	@ <variable>.stCB.pu8Data, <variable>.stCB.pu8Data
	add	r1, sl, lr	@, temp.170, <variable>.stCB.pu8Data
	add	sl, sl, r7	@ u32ReadPos, temp.170, pretmp.154
	bl	memcpy	@
.L49:
	ldr	r0, [r4, #48]	@ <variable>.stCB.u32Lenght, <variable>.stCB.u32Lenght
	ldr	r3, [r4, #40]	@ temp.173, <variable>.stCB.pu32Read
	cmp	r0, sl	@ <variable>.stCB.u32Lenght, u32ReadPos
	moveq	sl, #0	@ u32ReadPos,
	adds	r0, r5, r6	@ Bytes, u32DestSize.166, u32WritePos
	str	sl, [r3, #0]	@ u32ReadPos,* temp.173
	ldreq	r3, [r4, #60]	@ <variable>.stProc.uBufEmptyCnt, <variable>.stProc.uBufEmptyCnt
	ldr	r1, [r4, #52]	@ <variable>.stProc.uTotalByteRead, <variable>.stProc.uTotalByteRead
	addeq	r3, r3, #1	@ tmp173, <variable>.stProc.uBufEmptyCnt,
	add	ip, r1, r0	@ tmp175, <variable>.stProc.uTotalByteRead, Bytes
	streq	r3, [r4, #60]	@ tmp173, <variable>.stProc.uBufEmptyCnt
	str	ip, [r4, #52]	@ tmp175, <variable>.stProc.uTotalByteRead
.L44:
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, sl, fp, sp, pc}
.L45:
	ldr	r5, [r4, #48]	@ D.2258, <variable>.stCB.u32Lenght
	add	r1, r2, sl	@ tmp157, u32DestSize, u32ReadPos
	cmp	r1, r5	@ tmp157, D.2258
	movls	r5, r2	@ u32DestSize.166, u32DestSize
	movls	r6, r3	@ u32WritePos, tmp153
	strls	r2, [fp, #-44]	@ u32DestSize, u32RdLen
	bls	.L47	@
.L48:
	rsb	r5, sl, r5	@ u32DestSize.166, u32ReadPos, D.2258
	str	r3, [fp, #-48]	@ tmp153, u32RdPos
	rsb	r2, r5, r2	@ u32WritePos.168, u32DestSize.166, u32DestSize
	str	r5, [fp, #-44]	@ u32DestSize.166, u32RdLen
	cmp	r6, r2	@ u32WritePos, u32WritePos.168
	str	r2, [fp, #-40]	@ u32WritePos.168, u32RdLen
	strcc	r6, [fp, #-40]	@ u32WritePos, u32RdLen
	movcs	r6, r2	@ u32WritePos, u32WritePos.168
	b	.L47	@
	.fnend
	.size	RBUF_ReadData, .-RBUF_ReadData
	.align	2
	.global	RBUF_Destroy
	.type	RBUF_Destroy, %function
RBUF_Destroy:
	.fnstart
.LFB15:
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
	subs	r4, r0, #0	@ hRBUF, hRBUF
	ldmeqfd	sp, {r3, r4, fp, sp, pc}	@
	ldr	r3, [r4, #0]	@ <variable>.stAttr.bExtMem, <variable>.stAttr.bExtMem
	cmp	r3, #0	@ <variable>.stAttr.bExtMem,
	beq	.L59	@,
	mov	r1, r4	@, hRBUF
	mov	r0, #1	@,
	bl	HI_FREE	@
	ldmfd	sp, {r3, r4, fp, sp, pc}	@
.L59:
	ldr	r1, [r4, #4]	@, <variable>.stAttr.uBufAddr
	mov	r0, #1	@,
	bl	HI_V_FREE_Aligned	@
	mov	r1, r4	@, hRBUF
	mov	r0, #1	@,
	bl	HI_FREE	@
	ldmfd	sp, {r3, r4, fp, sp, pc}	@
	.fnend
	.size	RBUF_Destroy, .-RBUF_Destroy
	.align	2
	.global	RBUF_Create_EXT
	.type	RBUF_Create_EXT, %function
RBUF_Create_EXT:
	.fnstart
.LFB14:
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
	mov	r6, r0	@ phRBUF, phRBUF
	mov	r5, r1	@ pstAttr, pstAttr
	mov	r0, #1	@,
	mov	r1, #84	@,
	bl	HI_MALLOC	@
	subs	r4, r0, #0	@ state,
	beq	.L61	@,
	mov	r1, #84	@,
	mov	r7, r4	@ state, state
	bl	__memzero	@
	mov	ip, r5	@ pstAttr, pstAttr
	ldmia	ip!, {r0, r1, r2, r3}	@ pstAttr,,,,
	stmia	r7!, {r0, r1, r2, r3}	@ state,,,,
	ldmia	ip!, {r0, r1, r2, r3}	@ pstAttr,,,,
	stmia	r7!, {r0, r1, r2, r3}	@ state,,,,
	ldr	r2, [ip, #0]	@ tmp143,* pstAttr
	str	r2, [r7, #0]	@ tmp143, <variable>.stAttr
	ldr	r3, [r5, #0]	@ <variable>.bExtMem, <variable>.bExtMem
	cmp	r3, #0	@ <variable>.bExtMem,
	beq	.L62	@,
	ldmib	r4, {r0, r3}	@ phole ldm
.L63:
	ldr	r1, [r4, #16]	@ D.2034, <variable>.stAttr.uBufRptr
	ldr	lr, [r4, #12]	@ <variable>.stAttr.uBufWptr, <variable>.stAttr.uBufWptr
	str	r0, [r4, #44]	@ prephitmp.217, <variable>.stCB.pu8Data
	str	r1, [r4, #40]	@ D.2034, <variable>.stCB.pu32Read
	str	lr, [r4, #36]	@ <variable>.stAttr.uBufWptr, <variable>.stCB.pu32Write
	str	r3, [r4, #48]	@ prephitmp.229, <variable>.stCB.u32Lenght
	str	r4, [r6, #0]	@ state,* phRBUF
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}	@
.L62:
	mov	r0, #1	@,
	ldr	r1, [r5, #8]	@, <variable>.uBufSize
	ldr	r2, [r5, #24]	@, <variable>.uFrameSize
	bl	HI_V_MALLOC_Aligned	@
	cmp	r0, #0	@ rbfbase
	ldrne	r3, [r5, #8]	@ prephitmp.229, <variable>.uBufSize
	stmneib	r4, {r0, r3}	@ phole stm
	bne	.L63	@
.L61:
	mov	r1, r4	@, state
	mov	r0, #1	@,
	bl	HI_FREE	@
	mov	r0, #1	@,
	mov	r1, #0	@,
	bl	HI_FREE_Aligned	@
	mov	r3, #0	@ tmp148,
	str	r3, [r6, #0]	@ tmp148,* phRBUF
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}	@
	.fnend
	.size	RBUF_Create_EXT, .-RBUF_Create_EXT
	.align	2
	.global	RBUF_Create
	.type	RBUF_Create, %function
RBUF_Create:
	.fnstart
.LFB13:
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
	mov	r6, r0	@ phRBUF, phRBUF
	mov	r5, r1	@ pstAttr, pstAttr
	mov	r0, #1	@,
	mov	r1, #84	@,
	bl	HI_MALLOC	@
	subs	r4, r0, #0	@ state,
	beq	.L67	@,
	mov	r1, #84	@,
	mov	r7, r4	@ state, state
	bl	__memzero	@
	mov	ip, r5	@ pstAttr, pstAttr
	ldmia	ip!, {r0, r1, r2, r3}	@ pstAttr,,,,
	stmia	r7!, {r0, r1, r2, r3}	@ state,,,,
	ldmia	ip!, {r0, r1, r2, r3}	@ pstAttr,,,,
	stmia	r7!, {r0, r1, r2, r3}	@ state,,,,
	ldr	r2, [ip, #0]	@ tmp145,* pstAttr
	str	r2, [r7, #0]	@ tmp145, <variable>.stAttr
	ldr	r3, [r5, #0]	@ <variable>.bExtMem, <variable>.bExtMem
	cmp	r3, #0	@ <variable>.bExtMem,
	beq	.L68	@,
	ldmib	r4, {r0, r1}	@ phole ldm
.L69:
	ldr	r2, [r4, #16]	@ D.2000, <variable>.stAttr.uBufRptr
	mov	lr, #0	@ tmp149,
	ldr	r3, [r4, #12]	@ pu32Write.3, <variable>.stAttr.uBufWptr
	str	r1, [r4, #48]	@ prephitmp.261, <variable>.stCB.u32Lenght
	str	r2, [r4, #40]	@ D.2000, <variable>.stCB.pu32Read
	str	r0, [r4, #44]	@ prephitmp.249, <variable>.stCB.pu8Data
	str	r3, [r4, #36]	@ pu32Write.3, <variable>.stCB.pu32Write
	str	lr, [r3, #0]	@ tmp149,* pu32Write.3
	ldr	r1, [r4, #40]	@ D.2277, <variable>.stCB.pu32Read
	str	lr, [r1, #0]	@ tmp149,* D.2277
	str	r4, [r6, #0]	@ state,* phRBUF
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}	@
.L68:
	mov	r0, #1	@,
	ldr	r1, [r5, #8]	@, <variable>.uBufSize
	ldr	r2, [r5, #24]	@, <variable>.uFrameSize
	bl	HI_V_MALLOC_Aligned	@
	cmp	r0, #0	@ rbfbase
	ldrne	r1, [r5, #8]	@ prephitmp.261, <variable>.uBufSize
	stmneib	r4, {r0, r1}	@ phole stm
	bne	.L69	@
.L67:
	mov	r1, r4	@, state
	mov	r0, #1	@,
	bl	HI_FREE	@
	mov	r0, #1	@,
	mov	r1, #0	@,
	bl	HI_FREE_Aligned	@
	mov	r0, #0	@ tmp151,
	str	r0, [r6, #0]	@ tmp151,* phRBUF
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}	@
	.fnend
	.size	RBUF_Create, .-RBUF_Create
	.ident	"GCC: (Hisilicon_v200(gcc4.4-290+glibc-2.11+eabi+nptl)) 4.4.1"
	.section	.note.GNU-stack,"",%progbits

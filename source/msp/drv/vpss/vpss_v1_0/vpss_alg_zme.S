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
	.file	"vpss_alg_zme.c"
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
@ -DKBUILD_STR(s)=#s -DKBUILD_BASENAME=KBUILD_STR(vpss_alg_zme)
@ -DKBUILD_MODNAME=KBUILD_STR(hi_vpss) -isystem
@ /opt/hisi-linux/x86-arm/arm-hisiv200-linux/bin/../lib/gcc/arm-hisiv200-linux-gnueabi/4.4.1/include
@ -include
@ /home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/include/linux/kconfig.h
@ -MD drivers/msp/vpss/vpss_v1_0/.vpss_alg_zme.o.d
@ /home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/drivers/msp/vpss/vpss_v1_0/vpss_alg_zme.c
@ -mlittle-endian -marm -mapcs -mno-sched-prolog -mabi=aapcs-linux
@ -mno-thumb-interwork -march=armv7-a -msoft-float -auxbase-strip
@ drivers/msp/vpss/vpss_v1_0/vpss_alg_zme.o -O2 -Wall -Wundef
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
	.global	GetVerticalScalerOffset
	.type	GetVerticalScalerOffset, %function
GetVerticalScalerOffset:
	.fnstart
.LFB1241:
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
	ldr	r3, [r1, #24]	@ <variable>.bZmeFrmFmtIn, <variable>.bZmeFrmFmtIn
	cmp	r3, #0	@ <variable>.bZmeFrmFmtIn,
	beq	.L2	@,
	ldr	r2, [r1, #28]	@ <variable>.bZmeFrmFmtOut, <variable>.bZmeFrmFmtOut
	cmp	r2, #0	@ <variable>.bZmeFrmFmtOut,
	bne	.L3	@,
.L7:
	ldr	ip, [r1, #36]	@ <variable>.bZmeBFOut, <variable>.bZmeBFOut
	cmp	ip, #0	@ <variable>.bZmeBFOut,
	movne	r0, r0, asr #1	@ s32VOffset, u32VRatio,
	ldmnefd	sp, {fp, sp, pc}	@
.L3:
	mov	r0, #0	@ s32VOffset,
	ldmfd	sp, {fp, sp, pc}	@
.L2:
	ldr	ip, [r1, #28]	@ <variable>.bZmeFrmFmtOut, <variable>.bZmeFrmFmtOut
	cmp	ip, #0	@ <variable>.bZmeFrmFmtOut,
	bne	.L12	@,
	ldr	r3, [r1, #32]	@ <variable>.bZmeBFIn, <variable>.bZmeBFIn
	cmp	r3, #0	@ <variable>.bZmeBFIn,
	beq	.L7	@,
	ldr	r2, [r1, #36]	@ <variable>.bZmeBFOut, <variable>.bZmeBFOut
	cmp	r2, #0	@ <variable>.bZmeBFOut,
	subne	r0, r0, #4096	@ tmp144, u32VRatio,
	movne	r0, r0, asr #1	@ s32VOffset, tmp144,
	ldmnefd	sp, {fp, sp, pc}	@
	b	.L6	@
.L12:
	ldr	r0, [r1, #32]	@ <variable>.bZmeBFIn, <variable>.bZmeBFIn
	cmp	r0, #0	@ <variable>.bZmeBFIn,
	beq	.L3	@,
.L6:
	movw	r0, #:lower16:-2048	@ s32VOffset,
	movt	r0, #:upper16:-2048	@ s32VOffset,
	ldmfd	sp, {fp, sp, pc}	@
	.fnend
	.size	GetVerticalScalerOffset, .-GetVerticalScalerOffset
	.global	__aeabi_uidiv
	.align	2
	.global	GetHLfirCoef
	.type	GetHLfirCoef, %function
GetHLfirCoef:
	.fnstart
.LFB1242:
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
	mov	r4, r0	@ pstCoefAddr, pstCoefAddr
	mov	r0, #16777216	@,
	bl	__aeabi_uidiv	@
	cmp	r0, #4096	@ tmp140,
	ldrhi	r0, [r4, #0]	@ u32CoefAddr, <variable>.u32ZmeCoefAddrHL8_1
	ldmhifd	sp, {r3, r4, fp, sp, pc}	@
	ldreq	r0, [r4, #4]	@ u32CoefAddr, <variable>.u32ZmeCoefAddrHL8_E1
	ldmeqfd	sp, {r3, r4, fp, sp, pc}	@
	cmp	r0, #3072	@ tmp140,
	ldrcs	r0, [r4, #8]	@ u32CoefAddr, <variable>.u32ZmeCoefAddrHL8_075
	ldmcsfd	sp, {r3, r4, fp, sp, pc}	@
	cmp	r0, #2048	@ tmp140,
	ldrcs	r0, [r4, #12]	@ u32CoefAddr, <variable>.u32ZmeCoefAddrHL8_05
	ldmcsfd	sp, {r3, r4, fp, sp, pc}	@
	movw	r3, #1364	@ tmp143,
	cmp	r0, r3	@ tmp140, tmp143
	ldrhi	r0, [r4, #16]	@ u32CoefAddr, <variable>.u32ZmeCoefAddrHL8_033
	ldmhifd	sp, {r3, r4, fp, sp, pc}	@
	cmp	r0, #1024	@ tmp140,
	ldrcs	r0, [r4, #20]	@ u32CoefAddr, <variable>.u32ZmeCoefAddrHL8_025
	ldrcc	r0, [r4, #24]	@ u32CoefAddr, <variable>.u32ZmeCoefAddrHL8_0
	ldmfd	sp, {r3, r4, fp, sp, pc}	@
	.fnend
	.size	GetHLfirCoef, .-GetHLfirCoef
	.align	2
	.global	GetHCfirCoef
	.type	GetHCfirCoef, %function
GetHCfirCoef:
	.fnstart
.LFB1243:
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
	mov	r4, r0	@ pstCoefAddr, pstCoefAddr
	mov	r0, #16777216	@,
	bl	__aeabi_uidiv	@
	cmp	r0, #4096	@ tmp140,
	ldrgt	r0, [r4, #28]	@ u32CoefAddr, <variable>.u32ZmeCoefAddrHC4_1
	ldmgtfd	sp, {r3, r4, fp, sp, pc}	@
	ldreq	r0, [r4, #32]	@ u32CoefAddr, <variable>.u32ZmeCoefAddrHC4_E1
	ldmeqfd	sp, {r3, r4, fp, sp, pc}	@
	cmp	r0, #3072	@ tmp140,
	ldrge	r0, [r4, #36]	@ u32CoefAddr, <variable>.u32ZmeCoefAddrHC4_075
	ldmgefd	sp, {r3, r4, fp, sp, pc}	@
	cmp	r0, #2048	@ tmp140,
	ldrge	r0, [r4, #40]	@ u32CoefAddr, <variable>.u32ZmeCoefAddrHC4_05
	ldmgefd	sp, {r3, r4, fp, sp, pc}	@
	movw	r3, #1364	@ tmp143,
	cmp	r0, r3	@ tmp140, tmp143
	ldrgt	r0, [r4, #44]	@ u32CoefAddr, <variable>.u32ZmeCoefAddrHC4_033
	ldmgtfd	sp, {r3, r4, fp, sp, pc}	@
	cmp	r0, #1024	@ tmp140,
	ldrge	r0, [r4, #48]	@ u32CoefAddr, <variable>.u32ZmeCoefAddrHC4_025
	ldrlt	r0, [r4, #52]	@ u32CoefAddr, <variable>.u32ZmeCoefAddrHC4_0
	ldmfd	sp, {r3, r4, fp, sp, pc}	@
	.fnend
	.size	GetHCfirCoef, .-GetHCfirCoef
	.align	2
	.global	GetVLfirCoef
	.type	GetVLfirCoef, %function
GetVLfirCoef:
	.fnstart
.LFB1244:
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
	mov	r4, r0	@ pstCoefAddr, pstCoefAddr
	mov	r0, #16777216	@,
	bl	__aeabi_uidiv	@
	cmp	r0, #4096	@ tmp140,
	ldrgt	r0, [r4, #56]	@ u32CoefAddr, <variable>.u32ZmeCoefAddrVL6_1
	ldmgtfd	sp, {r3, r4, fp, sp, pc}	@
	ldreq	r0, [r4, #60]	@ u32CoefAddr, <variable>.u32ZmeCoefAddrVL6_E1
	ldmeqfd	sp, {r3, r4, fp, sp, pc}	@
	cmp	r0, #3072	@ tmp140,
	ldrge	r0, [r4, #64]	@ u32CoefAddr, <variable>.u32ZmeCoefAddrVL6_075
	ldmgefd	sp, {r3, r4, fp, sp, pc}	@
	cmp	r0, #2048	@ tmp140,
	ldrge	r0, [r4, #68]	@ u32CoefAddr, <variable>.u32ZmeCoefAddrVL6_05
	ldmgefd	sp, {r3, r4, fp, sp, pc}	@
	movw	r3, #1364	@ tmp143,
	cmp	r0, r3	@ tmp140, tmp143
	ldrgt	r0, [r4, #72]	@ u32CoefAddr, <variable>.u32ZmeCoefAddrVL6_033
	ldmgtfd	sp, {r3, r4, fp, sp, pc}	@
	cmp	r0, #1024	@ tmp140,
	ldrge	r0, [r4, #76]	@ u32CoefAddr, <variable>.u32ZmeCoefAddrVL6_025
	ldrlt	r0, [r4, #80]	@ u32CoefAddr, <variable>.u32ZmeCoefAddrVL6_0
	ldmfd	sp, {r3, r4, fp, sp, pc}	@
	.fnend
	.size	GetVLfirCoef, .-GetVLfirCoef
	.align	2
	.global	GetVCfirCoef
	.type	GetVCfirCoef, %function
GetVCfirCoef:
	.fnstart
.LFB1245:
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
	mov	r4, r0	@ pstCoefAddr, pstCoefAddr
	mov	r0, #16777216	@,
	bl	__aeabi_uidiv	@
	cmp	r0, #4096	@ tmp140,
	ldrgt	r0, [r4, #84]	@ u32CoefAddr, <variable>.u32ZmeCoefAddrVC4_1
	ldmgtfd	sp, {r3, r4, fp, sp, pc}	@
	ldreq	r0, [r4, #88]	@ u32CoefAddr, <variable>.u32ZmeCoefAddrVC4_E1
	ldmeqfd	sp, {r3, r4, fp, sp, pc}	@
	cmp	r0, #3072	@ tmp140,
	ldrge	r0, [r4, #92]	@ u32CoefAddr, <variable>.u32ZmeCoefAddrVC4_075
	ldmgefd	sp, {r3, r4, fp, sp, pc}	@
	cmp	r0, #2048	@ tmp140,
	ldrge	r0, [r4, #96]	@ u32CoefAddr, <variable>.u32ZmeCoefAddrVC4_05
	ldmgefd	sp, {r3, r4, fp, sp, pc}	@
	movw	r3, #1364	@ tmp143,
	cmp	r0, r3	@ tmp140, tmp143
	ldrgt	r0, [r4, #100]	@ u32CoefAddr, <variable>.u32ZmeCoefAddrVC4_033
	ldmgtfd	sp, {r3, r4, fp, sp, pc}	@
	cmp	r0, #1024	@ tmp140,
	ldrge	r0, [r4, #104]	@ u32CoefAddr, <variable>.u32ZmeCoefAddrVC4_025
	ldrlt	r0, [r4, #108]	@ u32CoefAddr, <variable>.u32ZmeCoefAddrVC4_0
	ldmfd	sp, {r3, r4, fp, sp, pc}	@
	.fnend
	.size	GetVCfirCoef, .-GetVCfirCoef
	.align	2
	.global	GetHLCfirCoef
	.type	GetHLCfirCoef, %function
GetHLCfirCoef:
	.fnstart
.LFB1246:
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
	mov	r4, r0	@ pstCoefAddr, pstCoefAddr
	mov	r0, #16777216	@,
	bl	__aeabi_uidiv	@
	cmp	r0, #4096	@ tmp140,
	ldrhi	r0, [r4, #112]	@ u32CoefAddr, <variable>.u32ZmeCoefAddrHL8C4_1
	ldmhifd	sp, {r3, r4, fp, sp, pc}	@
	ldreq	r0, [r4, #116]	@ u32CoefAddr, <variable>.u32ZmeCoefAddrHL8C4_E1
	ldmeqfd	sp, {r3, r4, fp, sp, pc}	@
	cmp	r0, #3072	@ tmp140,
	ldrcs	r0, [r4, #120]	@ u32CoefAddr, <variable>.u32ZmeCoefAddrHL8C4_075
	ldmcsfd	sp, {r3, r4, fp, sp, pc}	@
	cmp	r0, #2048	@ tmp140,
	ldrcs	r0, [r4, #124]	@ u32CoefAddr, <variable>.u32ZmeCoefAddrHL8C4_05
	ldmcsfd	sp, {r3, r4, fp, sp, pc}	@
	movw	r3, #1364	@ tmp143,
	cmp	r0, r3	@ tmp140, tmp143
	ldrhi	r0, [r4, #128]	@ u32CoefAddr, <variable>.u32ZmeCoefAddrHL8C4_033
	ldmhifd	sp, {r3, r4, fp, sp, pc}	@
	cmp	r0, #1024	@ tmp140,
	ldrcs	r0, [r4, #132]	@ u32CoefAddr, <variable>.u32ZmeCoefAddrHL8C4_025
	ldrcc	r0, [r4, #136]	@ u32CoefAddr, <variable>.u32ZmeCoefAddrHL8C4_0
	ldmfd	sp, {r3, r4, fp, sp, pc}	@
	.fnend
	.size	GetHLCfirCoef, .-GetHLCfirCoef
	.align	2
	.global	GetVLCfirCoef
	.type	GetVLCfirCoef, %function
GetVLCfirCoef:
	.fnstart
.LFB1247:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	.movsp ip
	mov	ip, sp	@,
	stmfd	sp!, {r4, fp, ip, lr, pc}	@,
	.pad #4
	.save {r4, fp, ip, lr}
	.setfp fp, ip, #-4
	sub	fp, ip, #4	@,,
	.pad #20
	sub	sp, sp, #20	@,,
	push	{lr}
	bl	__gnu_mcount_nc
	sub	ip, fp, #32	@ tmp137,,
	mov	r4, r0	@ pstCoefAddr, pstCoefAddr
	mov	r0, #16777216	@,
	stmia	ip, {r1, r2, r3}	@ tmp137,,,
	bl	__aeabi_uidiv	@
	ldr	r3, [fp, #-28]	@ stPickCoef.u32TapL, stPickCoef.u32TapL
	cmp	r3, #6	@ stPickCoef.u32TapL,
	beq	.L74	@,
	cmp	r0, #4096	@ tmp140,
	ldrhi	r0, [r4, #168]	@ u32CoefAddr, <variable>.u32ZmeCoefAddrVL4C4_1
	bhi	.L61	@,
	ldreq	r0, [r4, #172]	@ u32CoefAddr, <variable>.u32ZmeCoefAddrVL4C4_E1
	beq	.L61	@,
	cmp	r0, #3072	@ tmp140,
	ldrcs	r0, [r4, #176]	@ u32CoefAddr, <variable>.u32ZmeCoefAddrVL4C4_075
	bcs	.L61	@,
	cmp	r0, #2048	@ tmp140,
	ldrcs	r0, [r4, #180]	@ u32CoefAddr, <variable>.u32ZmeCoefAddrVL4C4_05
	bcs	.L61	@,
	movw	r2, #1364	@ tmp148,
	cmp	r0, r2	@ tmp140, tmp148
	ldrhi	r0, [r4, #184]	@ u32CoefAddr, <variable>.u32ZmeCoefAddrVL4C4_033
	bhi	.L61	@,
	cmp	r0, #1024	@ tmp140,
	ldrcs	r0, [r4, #188]	@ u32CoefAddr, <variable>.u32ZmeCoefAddrVL4C4_025
	ldrcc	r0, [r4, #192]	@ u32CoefAddr, <variable>.u32ZmeCoefAddrVL4C4_0
.L61:
	sub	sp, fp, #16
	ldmfd	sp, {r4, fp, sp, pc}
.L74:
	cmp	r0, #4096	@ tmp140,
	ldrhi	r0, [r4, #140]	@ u32CoefAddr, <variable>.u32ZmeCoefAddrVL6C4_1
	bhi	.L61	@,
	ldreq	r0, [r4, #144]	@ u32CoefAddr, <variable>.u32ZmeCoefAddrVL6C4_E1
	beq	.L61	@,
	cmp	r0, #3072	@ tmp140,
	ldrcs	r0, [r4, #148]	@ u32CoefAddr, <variable>.u32ZmeCoefAddrVL6C4_075
	bcs	.L61	@,
	cmp	r0, #2048	@ tmp140,
	ldrcs	r0, [r4, #152]	@ u32CoefAddr, <variable>.u32ZmeCoefAddrVL6C4_05
	bcs	.L61	@,
	movw	r1, #1364	@ tmp144,
	cmp	r0, r1	@ tmp140, tmp144
	ldrhi	r0, [r4, #156]	@ u32CoefAddr, <variable>.u32ZmeCoefAddrVL6C4_033
	bhi	.L61	@,
	cmp	r0, #1024	@ tmp140,
	ldrcs	r0, [r4, #160]	@ u32CoefAddr, <variable>.u32ZmeCoefAddrVL6C4_025
	ldrcc	r0, [r4, #164]	@ u32CoefAddr, <variable>.u32ZmeCoefAddrVL6C4_0
	b	.L61	@
	.fnend
	.size	GetVLCfirCoef, .-GetVLCfirCoef
	.align	2
	.global	ALG_VZmeComnSet
	.type	ALG_VZmeComnSet, %function
ALG_VZmeComnSet:
	.fnstart
.LFB1248:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	.movsp ip
	mov	ip, sp	@,
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, ip, lr, pc}	@,
	.pad #4
	.save {r4, r5, r6, r7, r8, r9, sl, fp, ip, lr}
	.setfp fp, ip, #-4
	sub	fp, ip, #4	@,,
	.pad #20
	sub	sp, sp, #20	@,,
	push	{lr}
	bl	__gnu_mcount_nc
	mov	r7, #1	@ tmp151,
	mov	sl, #0	@ tmp155,
	str	r7, [r1, #12]	@ tmp151, <variable>.bZmeEnVC
	str	sl, [r1, #44]	@ tmp155, <variable>.bZmeMedVC
	mov	r4, r1	@ pstZmeRtlPara, pstZmeRtlPara
	str	sl, [r1, #32]	@ tmp155, <variable>.bZmeMedHL
	mov	r5, r0	@ pstZmeDrvPara, pstZmeDrvPara
	str	sl, [r1, #36]	@ tmp155, <variable>.bZmeMedHC
	str	sl, [r1, #40]	@ tmp155, <variable>.bZmeMedVL
	str	r7, [r1, #0]	@ tmp151, <variable>.bZmeEnHL
	str	r7, [r1, #4]	@ tmp151, <variable>.bZmeEnHC
	str	r7, [r1, #8]	@ tmp151, <variable>.bZmeEnVL
	ldr	r9, [r0, #0]	@ u32ZmeCWIn, <variable>.u32ZmeFrmWIn
	str	r9, [r1, #64]	@ u32ZmeCWIn, <variable>.u32ZmeWIn
	mov	sl, r9, asl #20	@ prephitmp.609, u32ZmeCWIn,
	ldr	r6, [r0, #8]	@ u32ZmeCWOut, <variable>.u32ZmeFrmWOut
	str	r6, [r1, #72]	@ u32ZmeCWOut, <variable>.u32ZmeWOut
	ldr	r3, [r0, #24]	@ <variable>.bZmeFrmFmtIn, <variable>.bZmeFrmFmtIn
	ldr	r8, [r0, #4]	@ u32ZmeCHIn, <variable>.u32ZmeFrmHIn
	cmp	r3, r7	@ <variable>.bZmeFrmFmtIn,
	movne	r8, r8, lsr #1	@ u32ZmeCHIn, <variable>.u32ZmeFrmHIn,
	str	r8, [r1, #68]	@ u32ZmeCHIn, <variable>.u32ZmeHIn
	ldr	lr, [r0, #28]	@ <variable>.bZmeFrmFmtOut, <variable>.bZmeFrmFmtOut
	ldr	r7, [r0, #12]	@ u32ZmeCHOut, <variable>.u32ZmeFrmHOut
	mov	ip, r8, asl #12	@, u32ZmeCHIn,
	cmp	lr, #1	@ <variable>.bZmeFrmFmtOut,
	str	ip, [fp, #-48]	@, %sfp
	mov	r0, sl	@, prephitmp.609
	movne	r7, r7, lsr #1	@ u32ZmeCHOut, <variable>.u32ZmeFrmHOut,
	str	r7, [r1, #76]	@ u32ZmeCHOut, <variable>.u32ZmeHOut
	mov	r1, r6	@, u32ZmeCWOut
	str	r8, [fp, #-56]	@,
	bl	__aeabi_uidiv	@
	mov	r1, r7	@, u32ZmeCHOut
	str	r0, [fp, #-52]	@, %sfp
	str	r0, [r4, #80]	@, <variable>.u32ZmeRatioHL
	ldr	r0, [fp, #-48]	@, %sfp
	bl	__aeabi_uidiv	@
	str	r0, [r4, #84]	@ D.26725, <variable>.u32ZmeRatioVL
	mov	r8, r0	@ D.26725,
	ldr	r1, [r5, #16]	@ D.26727, <variable>.eZmeYCFmtIn
	ldr	r2, [r5, #20]	@ D.26732, <variable>.eZmeYCFmtOut
	cmp	r1, #69	@ D.26727,
	ldr	r0, [fp, #-56]	@,
	movne	sl, r9, lsr #1	@ tmp169, u32ZmeCWIn,
	movne	sl, sl, asl #20	@ prephitmp.609, tmp169,
	cmp	r2, #69	@ D.26732,
	movne	r6, r6, lsr #1	@ u32ZmeCWOut, u32ZmeCWOut,
	cmp	r1, #65	@ D.26727,
	moveq	r0, r0, lsr #1	@ tmp170, u32ZmeCHIn,
	mov	r1, r6	@, u32ZmeCWOut
	moveq	r0, r0, asl #12	@, tmp170,
	streq	r0, [fp, #-48]	@, %sfp
	cmp	r2, #65	@ D.26732,
	mov	r0, sl	@, prephitmp.609
	moveq	r7, r7, lsr #1	@ u32ZmeCHOut, u32ZmeCHOut,
	bl	__aeabi_uidiv	@
	mov	r1, r7	@, u32ZmeCHOut
	mov	r7, #0	@ tmp177,
	str	r0, [r4, #88]	@ D.26745, <variable>.u32ZmeRatioHC
	mov	sl, r0	@ D.26745,
	ldr	r0, [fp, #-48]	@, %sfp
	bl	__aeabi_uidiv	@
	str	r7, [r4, #48]	@ tmp177, <variable>.s32ZmeOffsetHL
	mov	r1, r5	@, pstZmeDrvPara
	mov	r6, r0	@ D.26747,
	mov	r0, r8	@, D.26725
	str	r6, [r4, #92]	@ D.26747, <variable>.u32ZmeRatioVC
	bl	GetVerticalScalerOffset	@
	str	r0, [r4, #56]	@ D.26749, <variable>.s32ZmeOffsetVL
	mov	r9, r0	@ D.26749,
	ldr	r3, [r5, #16]	@ <variable>.eZmeYCFmtIn, <variable>.eZmeYCFmtIn
	cmp	r3, #62	@ <variable>.eZmeYCFmtIn,
	beq	.L93	@,
.L85:
	mov	r1, r5	@, pstZmeDrvPara
	mov	lr, #0	@ tmp181,
	mov	r0, r6	@, D.26747
	str	lr, [r4, #52]	@ tmp181, <variable>.s32ZmeOffsetHC
	bl	GetVerticalScalerOffset	@
	ldr	ip, [fp, #-52]	@, %sfp
	subs	sl, sl, #1048576	@ tmp184, D.26745,
	movne	sl, #1	@ tmp184
	str	sl, [r4, #20]	@ tmp184, <variable>.bZmeMdHC
	subs	r1, ip, #1048576	@ tmp183,,
	movne	r1, #1	@ tmp183
	cmp	r8, #4096	@ D.26725,
	str	r1, [r4, #16]	@ tmp183, <variable>.bZmeMdHL
	add	r7, r0, r7	@ tmp182,, s32VOffset
	str	r7, [r4, #60]	@ tmp182, <variable>.s32ZmeOffsetVC
	beq	.L94	@,
.L87:
	mov	r3, #1	@ tmp186,
	str	r3, [r4, #24]	@ tmp186, <variable>.bZmeMdVL
.L88:
	cmp	r6, #4096	@ D.26747,
	beq	.L95	@,
.L89:
	mov	r3, #1	@ tmp189,
.L92:
	ldr	r2, [r4, #80]	@ <variable>.u32ZmeRatioHL, <variable>.u32ZmeRatioHL
	mov	ip, #0	@ tmp190,
	str	r3, [r4, #28]	@ tmp189, <variable>.bZmeMdVC
	mov	r0, ip	@, tmp190
	cmp	r2, #1048576	@ <variable>.u32ZmeRatioHL,
	movcs	r1, #0	@, tmp193
	movcc	r1, #1	@, tmp193
	str	ip, [r4, #100]	@ tmp190, <variable>.bZmeTapVC
	str	r1, [r4, #96]	@ tmp193, <variable>.bZmeOrder
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, sl, fp, sp, pc}
.L95:
	ldr	r3, [r4, #60]	@ <variable>.s32ZmeOffsetVC, <variable>.s32ZmeOffsetVC
	cmp	r3, #0	@ <variable>.s32ZmeOffsetVC,
	bne	.L89	@,
	b	.L92	@
.L94:
	cmp	r9, #0	@ D.26749,
	streq	r9, [r4, #24]	@ D.26749, <variable>.bZmeMdVL
	beq	.L88	@,
	b	.L87	@
.L93:
	ldr	r2, [r5, #24]	@ <variable>.bZmeFrmFmtIn, <variable>.bZmeFrmFmtIn
	cmp	r2, r7	@ <variable>.bZmeFrmFmtIn,
	beq	.L96	@,
.L86:
	movw	r7, #:lower16:-512	@ s32VOffset,
	movt	r7, #:upper16:-512	@ s32VOffset,
	b	.L85	@
.L96:
	ldr	r0, [r5, #32]	@ <variable>.bZmeBFIn, <variable>.bZmeBFIn
	cmp	r0, r7	@ <variable>.bZmeBFIn,
	movwne	r7, #:lower16:-1536	@ s32VOffset,
	movtne	r7, #:upper16:-1536	@ s32VOffset,
	bne	.L85	@,
	b	.L86	@
	.fnend
	.size	ALG_VZmeComnSet, .-ALG_VZmeComnSet
	.align	2
	.global	ALG_VZmeVpssHQSet
	.type	ALG_VZmeVpssHQSet, %function
ALG_VZmeVpssHQSet:
	.fnstart
.LFB1249:
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
	mov	r4, r2	@ pstZmeRtlPara, pstZmeRtlPara
	mov	r5, r0	@ pstMem, pstMem
	mov	r0, r1	@, pstZmeDrvPara
	mov	r1, r2	@, pstZmeRtlPara
	add	r5, r5, #12	@ D.26788, pstMem,
	bl	ALG_VZmeComnSet	@
	ldr	r0, [r4, #80]	@ <variable>.u32ZmeRatioHL, <variable>.u32ZmeRatioHL
	mov	r1, r0, lsr #8	@, <variable>.u32ZmeRatioHL,
	mov	r0, r5	@, D.26788
	bl	GetHLfirCoef	@
	ldr	r1, [r4, #88]	@ <variable>.u32ZmeRatioHC, <variable>.u32ZmeRatioHC
	mov	r1, r1, lsr #8	@, <variable>.u32ZmeRatioHC,
	str	r0, [r4, #112]	@, <variable>.u32ZmeCoefAddrHL
	mov	r0, r5	@, D.26788
	bl	GetHCfirCoef	@
	ldr	r1, [r4, #84]	@, <variable>.u32ZmeRatioVL
	str	r0, [r4, #116]	@, <variable>.u32ZmeCoefAddrHC
	mov	r0, r5	@, D.26788
	bl	GetVLfirCoef	@
	ldr	r1, [r4, #92]	@, <variable>.u32ZmeRatioVC
	str	r0, [r4, #120]	@, <variable>.u32ZmeCoefAddrVL
	mov	r0, r5	@, D.26788
	bl	GetVCfirCoef	@
	str	r0, [r4, #124]	@, <variable>.u32ZmeCoefAddrVC
	mov	r0, #0	@,
	ldmfd	sp, {r4, r5, fp, sp, pc}	@
	.fnend
	.size	ALG_VZmeVpssHQSet, .-ALG_VZmeVpssHQSet
	.align	2
	.global	ALG_VZmeVpssComnDeInit
	.type	ALG_VZmeVpssComnDeInit, %function
ALG_VZmeVpssComnDeInit:
	.fnstart
.LFB1240:
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
	bl	HI_DRV_MMZ_UnmapAndRelease	@
	mov	r0, #0	@,
	ldmfd	sp, {fp, sp, pc}	@
	.fnend
	.size	ALG_VZmeVpssComnDeInit, .-ALG_VZmeVpssComnDeInit
	.align	2
	.type	VZmeTransVpssCoefAlign, %function
VZmeTransVpssCoefAlign:
	.fnstart
.LFB1233:
	@ args = 0, pretend = 0, frame = 152
	@ frame_needed = 1, uses_anonymous_args = 0
	.movsp ip
	mov	ip, sp	@,
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, ip, lr, pc}	@,
	.pad #4
	.save {r4, r5, r6, r7, r8, r9, sl, fp, ip, lr}
	.setfp fp, ip, #-4
	sub	fp, ip, #4	@,,
	.pad #156
	sub	sp, sp, #156	@,,
	push	{lr}
	bl	__gnu_mcount_nc
	cmp	r1, #3	@ enTap,
	mov	r4, r0	@ ps16Coef, ps16Coef
	str	r2, [fp, #-52]	@ pBitCoef, %sfp
	movhi	r0, #4	@,
	movhi	r9, #2	@ prephitmp.716,
	strhi	r0, [fp, #-56]	@, %sfp
	bhi	.L103	@,
	movw	r3, #:lower16:.LANCHOR0	@ tmp342,
	movt	r3, #:upper16:.LANCHOR0	@ tmp342,
	ldr	r3, [r3, r1, asl #2]	@, CSWTCH.414
	cmp	r3, #8	@,
	str	r3, [fp, #-56]	@, %sfp
	bne	.L126	@,
	ldrsh	r0, [r4, #0]	@ tmp350,* ps16Coef
	add	r2, r4, #2	@ ps16Coef.782, ps16Coef,
	ldrsh	r1, [r4, #2]	@ tmp343,
	add	r6, r2, #2	@ ps16Coef.785, ps16Coef.782,
	ldrsh	r5, [r2, #2]	@ tmp347,
	movw	r3, #:lower16:1047552	@ tmp345,
	ldr	r4, [fp, #-52]	@, %sfp
	mov	r7, r0, asl #22	@ tmp351, tmp350,
	mov	r8, r1, asl #10	@ tmp344, tmp343,
	movt	r3, #:upper16:1047552	@ tmp345,
	add	r0, r6, #2	@ ps16Coef.788, ps16Coef.785,
	and	ip, r8, r3	@ tmp345, tmp344, tmp345
	add	sl, ip, r5, asl #20	@, tmp349, tmp345, tmp347,
	mov	r9, r7, lsr #22	@ tmp351, tmp351,
	add	r2, sl, r9	@ tmp353, tmp349, tmp351
	add	r1, r0, #2	@ ps16Coef.797, ps16Coef.788,
	str	r2, [r4, #196]	@ tmp353, <variable>.s32CoefAttr
	movw	r3, #:lower16:1047552	@ tmp356,
	ldrsh	r8, [r0, #2]	@ tmp354,
	movt	r3, #:upper16:1047552	@ tmp356,
	ldrsh	r7, [r6, #2]	@ tmp361,
	ldrsh	ip, [r1, #2]	@ tmp358,
	add	r1, r1, #2	@ ps16Coef.800, ps16Coef.797,
	ldr	r0, [fp, #-52]	@, %sfp
	mov	r5, r8, asl #10	@ tmp355, tmp354,
	mov	r9, r7, asl #22	@ tmp362, tmp361,
	and	sl, r5, r3	@ tmp356, tmp355, tmp356
	add	r8, r1, #2	@ ps16Coef.803, ps16Coef.800,
	add	r4, sl, ip, asl #20	@, tmp360, tmp356, tmp358,
	mov	r2, r9, lsr #22	@ tmp362, tmp362,
	add	sl, r8, #2	@ ps16Coef.812, ps16Coef.803,
	add	r6, r4, r2	@ tmp364, tmp360, tmp362
	str	r6, [r0, #200]	@ tmp364, <variable>.s32CoefAttr
	ldrsh	r7, [r1, #2]	@ tmp369,
	add	ip, sl, #2	@ ps16Coef.815, ps16Coef.812,
	ldrsh	r5, [r8, #2]	@ tmp365,
	movw	r9, #:lower16:1047552	@ tmp367,
	ldr	r6, [fp, #-52]	@, %sfp
	movt	r9, #:upper16:1047552	@ tmp367,
	mov	r4, r7, asl #22	@ tmp370, tmp369,
	add	r8, ip, #2	@ ps16Coef.822, ps16Coef.815,
	mov	r3, r5, asl #10	@ tmp366, tmp365,
	mov	r7, r9	@ tmp375, tmp367
	and	r2, r3, r9	@ tmp367, tmp366, tmp367
	mov	r0, r4, lsr #22	@ tmp370, tmp370,
	add	r3, r2, r0	@ tmp372, tmp367, tmp370
	str	r3, [r6, #204]	@ tmp372, <variable>.s32CoefAttr
	ldrsh	r9, [ip, #2]	@ tmp373,
	add	r5, r8, #2	@ ps16Coef.825, ps16Coef.822,
	ldrsh	r1, [sl, #2]	@ tmp380,
	mov	r3, r7	@ tmp386, tmp375
	ldrsh	r0, [r8, #2]	@ tmp377,
	add	r6, r5, #2	@ ps16Coef.828, ps16Coef.825,
	mov	r4, r9, asl #10	@ tmp374, tmp373,
	add	sl, r6, #2	@ ps16Coef.837, ps16Coef.828,
	and	r2, r4, r7	@ tmp375, tmp374, tmp375
	mov	r8, r1, asl #22	@ tmp381, tmp380,
	ldr	r7, [fp, #-52]	@, %sfp
	add	r9, r2, r0, asl #20	@, tmp379, tmp375, tmp377,
	mov	ip, r8, lsr #22	@ tmp381, tmp381,
	add	r0, sl, #2	@ ps16Coef.840, ps16Coef.837,
	add	r1, r9, ip	@ tmp383, tmp379, tmp381
	add	r4, r0, #2	@ ps16Coef.843, ps16Coef.840,
	str	r1, [r7, #208]	@ tmp383, <variable>.s32CoefAttr
	mov	r1, r3	@ tmp397, tmp386
	ldrsh	r8, [r6, #2]	@ tmp384,
	add	r6, r4, #2	@ ps16Coef.852, ps16Coef.843,
	ldrsh	r2, [r5, #2]	@ tmp391,
	add	r7, r6, #2	@ ps16Coef.855, ps16Coef.852,
	ldrsh	ip, [sl, #2]	@ tmp388,
	mov	r9, r8, asl #10	@ tmp385, tmp384,
	and	r5, r9, r3	@ tmp386, tmp385, tmp386
	mov	sl, r2, asl #22	@ tmp392, tmp391,
	ldr	r3, [fp, #-52]	@, %sfp
	add	r8, r5, ip, asl #20	@, tmp390, tmp386, tmp388,
	mov	r9, sl, lsr #22	@ tmp392, tmp392,
	add	r5, r7, #2	@ ps16Coef.862, ps16Coef.855,
	add	r2, r8, r9	@ tmp394, tmp390, tmp392
	add	ip, r5, #2	@ ps16Coef.865, ps16Coef.862,
	str	r2, [r3, #212]	@ tmp394, <variable>.s32CoefAttr
	mov	r3, r1	@ tmp416, tmp405
	ldrsh	sl, [r0, #2]	@ tmp399,
	mov	r2, r1	@ tmp405, tmp397
	ldrsh	r8, [r4, #2]	@ tmp395,
	add	r4, ip, #2	@ ps16Coef.868, ps16Coef.865,
	mov	r9, sl, asl #22	@ tmp400, tmp399,
	add	sl, r4, #2	@ ps16Coef.877, ps16Coef.868,
	mov	r8, r8, asl #10	@ tmp396, tmp395,
	mov	r0, r9, lsr #22	@ tmp400, tmp400,
	ldr	r9, [fp, #-52]	@, %sfp
	and	r1, r8, r1	@ tmp397, tmp396, tmp397
	add	r1, r1, r0	@ tmp402, tmp397, tmp400
	add	r0, sl, #2	@ ps16Coef.880, ps16Coef.877,
	str	r1, [r9, #216]	@ tmp402, <variable>.s32CoefAttr
	add	r1, r0, #2	@ ps16Coef.883, ps16Coef.880,
	ldrsh	r7, [r7, #2]	@ tmp403,
	add	r9, r1, #2	@ ps16Coef.892, ps16Coef.883,
	ldrsh	r6, [r6, #2]	@ tmp410,
	ldrsh	r8, [r5, #2]	@ tmp407,
	mov	r5, r7, asl #10	@ tmp404, tmp403,
	add	r7, r9, #2	@ ps16Coef.895, ps16Coef.892,
	mov	r6, r6, asl #22	@ tmp411, tmp410,
	and	r2, r5, r2	@ tmp405, tmp404, tmp405
	add	r2, r2, r8, asl #20	@, tmp409, tmp405, tmp407,
	mov	r5, r3	@ tmp427, tmp416
	mov	r6, r6, lsr #22	@ tmp411, tmp411,
	add	r8, r2, r6	@ tmp413, tmp409, tmp411
	ldr	r6, [fp, #-52]	@, %sfp
	str	r8, [r6, #220]	@ tmp413, <variable>.s32CoefAttr
	add	r8, r7, #2	@ ps16Coef.902, ps16Coef.895,
	ldrsh	r4, [r4, #2]	@ tmp414,
	add	r2, r8, #2	@, ps16Coef.902,
	ldrsh	ip, [ip, #2]	@ tmp421,
	str	r2, [fp, #-56]	@, %sfp
	mov	r2, r3	@ tmp435, tmp427
	ldrsh	r6, [sl, #2]	@ tmp418,
	mov	sl, r4, asl #10	@ tmp415, tmp414,
	mov	ip, ip, asl #22	@ tmp422, tmp421,
	and	r3, sl, r3	@ tmp416, tmp415, tmp416
	ldr	r4, [fp, #-56]	@, %sfp
	add	sl, r3, r6, asl #20	@, tmp420, tmp416, tmp418,
	mov	ip, ip, lsr #22	@ tmp422, tmp422,
	add	r3, sl, ip	@ tmp424, tmp420, tmp422
	ldr	ip, [fp, #-52]	@, %sfp
	add	r6, r4, #2	@ ps16Coef.908,,
	add	r4, r6, #2	@ ps16Coef.917, ps16Coef.908,
	str	r3, [ip, #224]	@ tmp424, <variable>.s32CoefAttr
	mov	r3, r5	@ tmp446, tmp435
	ldrsh	sl, [r1, #2]	@,
	ldrsh	ip, [r0, #2]	@ tmp429,
	str	sl, [fp, #-60]	@, %sfp
	add	sl, r4, #2	@ ps16Coef.920, ps16Coef.917,
	add	r1, sl, #2	@, ps16Coef.920,
	str	r1, [fp, #-48]	@, %sfp
	ldr	r1, [fp, #-60]	@, %sfp
	mov	r0, ip, asl #22	@ tmp430, tmp429,
	mov	r1, r1, asl #10	@,,
	str	r1, [fp, #-60]	@, %sfp
	ldr	r1, [fp, #-48]	@, %sfp
	add	r1, r1, #2	@,,
	str	r1, [fp, #-76]	@, %sfp
	ldr	r1, [fp, #-60]	@, %sfp
	and	r5, r1, r5	@ tmp427,, tmp427
	mov	r1, r0, lsr #22	@ tmp430, tmp430,
	ldr	r0, [fp, #-52]	@, %sfp
	add	ip, r5, r1	@ tmp432, tmp427, tmp430
	str	ip, [r0, #228]	@ tmp432, <variable>.s32CoefAttr
	mov	r0, r2	@ tmp457, tmp446
	ldrsh	r5, [r9, #2]	@ tmp440,
	ldrsh	r7, [r7, #2]	@ tmp433,
	ldrsh	r1, [r8, #2]	@ tmp437,
	mov	r5, r5, asl #22	@ tmp441, tmp440,
	ldr	r9, [fp, #-76]	@, %sfp
	mov	r7, r7, asl #10	@ tmp434, tmp433,
	and	r2, r7, r2	@ tmp435, tmp434, tmp435
	mov	r5, r5, lsr #22	@ tmp441, tmp441,
	add	r2, r2, r1, asl #20	@, tmp439, tmp435, tmp437,
	add	ip, r9, #2	@ ps16Coef.935,,
	add	r7, r2, r5	@ tmp443, tmp439, tmp441
	ldr	r5, [fp, #-52]	@, %sfp
	add	r8, ip, #2	@ ps16Coef.942, ps16Coef.935,
	mov	r1, r3	@ tmp465, tmp457
	add	r9, r8, #2	@ ps16Coef.945, ps16Coef.942,
	str	r7, [r5, #232]	@ tmp443, <variable>.s32CoefAttr
	add	r5, r9, #2	@ ps16Coef.948, ps16Coef.945,
	ldr	r2, [fp, #-56]	@, %sfp
	ldrsh	r7, [r6, #2]	@ tmp444,
	ldrsh	r6, [r2, #2]	@ tmp451,
	add	r2, r5, #2	@, ps16Coef.948,
	str	r2, [fp, #-64]	@, %sfp
	mov	r7, r7, asl #10	@ tmp445, tmp444,
	ldrsh	r4, [r4, #2]	@ tmp448,
	mov	r2, r3	@ tmp476, tmp465
	mov	r6, r6, asl #22	@ tmp452, tmp451,
	and	r3, r7, r3	@ tmp446, tmp445, tmp446
	ldr	r7, [fp, #-64]	@, %sfp
	add	r3, r3, r4, asl #20	@, tmp450, tmp446, tmp448,
	mov	r6, r6, lsr #22	@ tmp452, tmp452,
	add	r3, r3, r6	@ tmp454, tmp450, tmp452
	ldr	r6, [fp, #-52]	@, %sfp
	add	r4, r7, #2	@ ps16Coef.960,,
	add	r7, r4, #2	@, ps16Coef.960,
	str	r7, [fp, #-60]	@, %sfp
	str	r3, [r6, #236]	@ tmp454, <variable>.s32CoefAttr
	add	r3, r7, #2	@,,
	ldrsh	r6, [sl, #2]	@ tmp459,
	ldr	sl, [fp, #-48]	@, %sfp
	str	r3, [fp, #-56]	@, %sfp
	mov	r6, r6, asl #22	@ tmp460, tmp459,
	ldrsh	sl, [sl, #2]	@,
	str	sl, [fp, #-68]	@, %sfp
	add	sl, r3, #2	@ ps16Coef.975,,
	add	r7, sl, #2	@, ps16Coef.975,
	str	r7, [fp, #-48]	@, %sfp
	ldr	r7, [fp, #-68]	@, %sfp
	mov	r3, r0	@ tmp487, tmp476
	mov	r7, r7, asl #10	@,,
	str	r7, [fp, #-68]	@, %sfp
	ldr	r7, [fp, #-48]	@, %sfp
	add	r7, r7, #2	@,,
	str	r7, [fp, #-72]	@, %sfp
	ldr	r7, [fp, #-68]	@, %sfp
	and	r0, r7, r0	@ tmp457,, tmp457
	mov	r7, r6, lsr #22	@ tmp460, tmp460,
	ldr	r6, [fp, #-52]	@, %sfp
	add	r0, r0, r7	@ tmp462, tmp457, tmp460
	str	r0, [r6, #240]	@ tmp462, <variable>.s32CoefAttr
	ldrsh	r0, [ip, #2]	@ tmp463,
	ldr	ip, [fp, #-76]	@, %sfp
	ldrsh	r7, [r8, #2]	@,
	mov	r0, r0, asl #10	@ tmp464, tmp463,
	ldr	r8, [fp, #-72]	@, %sfp
	ldrsh	r6, [ip, #2]	@ tmp470,
	mov	ip, r1	@ tmp495, tmp487
	str	r7, [fp, #-76]	@, %sfp
	and	r1, r0, r1	@ tmp465, tmp464, tmp465
	ldr	r0, [fp, #-76]	@, %sfp
	add	r7, r8, #2	@ ps16Coef.988,,
	mov	r6, r6, asl #22	@ tmp471, tmp470,
	add	r8, r7, #2	@ ps16Coef.997, ps16Coef.988,
	add	r1, r1, r0, asl #20	@, tmp469, tmp465,,
	mov	r6, r6, lsr #22	@ tmp471, tmp471,
	add	r1, r1, r6	@ tmp473, tmp469, tmp471
	ldr	r6, [fp, #-52]	@, %sfp
	mov	r0, r2	@ tmp506, tmp495
	str	r1, [r6, #244]	@ tmp473, <variable>.s32CoefAttr
	add	r6, r8, #2	@ ps16Coef.1000, ps16Coef.997,
	ldrsh	r9, [r9, #2]	@,
	ldr	r1, [fp, #-64]	@, %sfp
	ldrsh	r5, [r5, #2]	@ tmp474,
	str	r9, [fp, #-76]	@, %sfp
	add	r9, r6, #2	@ ps16Coef.1003, ps16Coef.1000,
	ldrsh	r1, [r1, #2]	@,
	mov	r5, r5, asl #10	@, tmp474,
	str	r5, [fp, #-80]	@, %sfp
	str	r1, [fp, #-64]	@, %sfp
	add	r1, r9, #2	@, ps16Coef.1003,
	add	r5, r1, #2	@,,
	str	r1, [fp, #-92]	@, %sfp
	ldr	r1, [fp, #-76]	@, %sfp
	str	r5, [fp, #-68]	@, %sfp
	mov	r5, r1, asl #22	@ tmp482,,
	ldr	r1, [fp, #-80]	@, %sfp
	and	r2, r1, r2	@ tmp476,, tmp476
	ldr	r1, [fp, #-64]	@, %sfp
	mov	r5, r5, lsr #22	@ tmp482, tmp482,
	add	r2, r2, r1, asl #20	@, tmp480, tmp476,,
	mov	r1, r3	@ tmp517, tmp506
	add	r2, r2, r5	@ tmp484, tmp480, tmp482
	ldr	r5, [fp, #-52]	@, %sfp
	str	r2, [r5, #248]	@ tmp484, <variable>.s32CoefAttr
	ldr	r2, [fp, #-68]	@, %sfp
	ldrsh	r4, [r4, #2]	@ tmp489,
	add	r2, r2, #2	@,,
	str	r2, [fp, #-112]	@, %sfp
	ldr	r5, [fp, #-60]	@, %sfp
	add	r2, r2, #2	@,,
	mov	r4, r4, asl #22	@ tmp490, tmp489,
	ldrsh	r5, [r5, #2]	@,
	mov	r4, r4, lsr #22	@ tmp490, tmp490,
	str	r2, [fp, #-80]	@, %sfp
	mov	r2, r3	@ tmp525, tmp517
	str	r5, [fp, #-60]	@, %sfp
	ldr	r5, [fp, #-80]	@, %sfp
	add	r5, r5, #2	@,,
	str	r5, [fp, #-76]	@, %sfp
	ldr	r5, [fp, #-60]	@, %sfp
	mov	r5, r5, asl #10	@,,
	str	r5, [fp, #-60]	@, %sfp
	ldr	r5, [fp, #-76]	@, %sfp
	add	r5, r5, #2	@,,
	str	r5, [fp, #-64]	@, %sfp
	ldr	r5, [fp, #-60]	@, %sfp
	and	r3, r5, r3	@ tmp487,, tmp487
	add	r5, r3, r4	@ tmp492, tmp487, tmp490
	ldr	r4, [fp, #-52]	@, %sfp
	str	r5, [r4, #252]	@ tmp492, <variable>.s32CoefAttr
	ldrsh	r4, [sl, #2]	@ tmp493,
	ldr	sl, [fp, #-56]	@, %sfp
	ldr	r5, [fp, #-48]	@, %sfp
	ldrsh	r3, [sl, #2]	@,
	str	r3, [fp, #-84]	@, %sfp
	mov	r3, ip	@ tmp536, tmp525
	ldrsh	sl, [r5, #2]	@,
	ldr	r5, [fp, #-64]	@, %sfp
	str	sl, [fp, #-60]	@, %sfp
	add	sl, r5, #2	@ ps16Coef.1040,,
	mov	r5, r4, asl #10	@, tmp493,
	str	r5, [fp, #-56]	@, %sfp
	ldr	r5, [fp, #-84]	@, %sfp
	add	r4, sl, #2	@, ps16Coef.1040,
	str	r4, [fp, #-48]	@, %sfp
	mov	r4, r5, asl #22	@ tmp501,,
	ldr	r5, [fp, #-56]	@, %sfp
	and	ip, r5, ip	@ tmp495,, tmp495
	ldr	r5, [fp, #-60]	@, %sfp
	mov	r4, r4, lsr #22	@ tmp501, tmp501,
	add	ip, ip, r5, asl #20	@, tmp499, tmp495,,
	ldr	r5, [fp, #-48]	@, %sfp
	add	r5, r5, #2	@,,
	str	r5, [fp, #-60]	@, %sfp
	add	r5, r5, #2	@,,
	str	r5, [fp, #-56]	@, %sfp
	add	r5, ip, r4	@ tmp503, tmp499, tmp501
	ldr	ip, [fp, #-52]	@, %sfp
	str	r5, [ip, #256]	@ tmp503, <variable>.s32CoefAttr
	ldr	r4, [fp, #-72]	@, %sfp
	ldrsh	ip, [r7, #2]	@ tmp504,
	ldrsh	r8, [r8, #2]	@,
	ldrsh	r7, [r4, #2]	@ tmp511,
	mov	r4, r0	@ tmp547, tmp536
	mov	ip, ip, asl #10	@ tmp505, tmp504,
	str	r8, [fp, #-72]	@, %sfp
	and	r0, ip, r0	@ tmp506, tmp505, tmp506
	ldr	ip, [fp, #-72]	@, %sfp
	mov	r7, r7, asl #22	@ tmp512, tmp511,
	ldr	r5, [fp, #-56]	@, %sfp
	mov	r7, r7, lsr #22	@ tmp512, tmp512,
	add	r0, r0, ip, asl #20	@, tmp510, tmp506,,
	add	r8, r5, #2	@ ps16Coef.1062,,
	add	r7, r0, r7	@ tmp514, tmp510, tmp512
	ldr	r0, [fp, #-52]	@, %sfp
	add	r5, r8, #2	@, ps16Coef.1062,
	str	r5, [fp, #-96]	@, %sfp
	mov	ip, r1	@ tmp555, tmp547
	str	r7, [r0, #260]	@ tmp514, <variable>.s32CoefAttr
	add	r0, r5, #2	@,,
	ldrsh	r6, [r6, #2]	@ tmp519,
	str	r0, [fp, #-88]	@, %sfp
	ldrsh	r7, [r9, #2]	@ tmp515,
	add	r9, r0, #2	@ ps16Coef.1077,,
	mov	r6, r6, asl #22	@ tmp520, tmp519,
	mov	r0, r1	@ tmp566, tmp555
	add	r5, r9, #2	@, ps16Coef.1077,
	str	r5, [fp, #-108]	@, %sfp
	mov	r7, r7, asl #10	@ tmp516, tmp515,
	add	r5, r5, #2	@,,
	and	r1, r7, r1	@ tmp517, tmp516, tmp517
	mov	r7, r6, lsr #22	@ tmp520, tmp520,
	ldr	r6, [fp, #-52]	@, %sfp
	add	r1, r1, r7	@ tmp522, tmp517, tmp520
	str	r5, [fp, #-104]	@, %sfp
	str	r1, [r6, #264]	@ tmp522, <variable>.s32CoefAttr
	add	r1, r5, #2	@,,
	ldr	r7, [fp, #-68]	@, %sfp
	ldr	r5, [fp, #-92]	@, %sfp
	ldrsh	r6, [r7, #2]	@ tmp523,
	str	r1, [fp, #-100]	@, %sfp
	add	r1, r1, #2	@,,
	ldrsh	r7, [r5, #2]	@ tmp530,
	ldr	r5, [fp, #-112]	@, %sfp
	mov	r6, r6, asl #10	@ tmp524, tmp523,
	str	r1, [fp, #-84]	@, %sfp
	ldrsh	r1, [r5, #2]	@,
	mov	r5, r7, asl #22	@ tmp531, tmp530,
	mov	r5, r5, lsr #22	@ tmp531, tmp531,
	str	r1, [fp, #-68]	@, %sfp
	mov	r1, r2	@ tmp577, tmp566
	ldr	r7, [fp, #-68]	@, %sfp
	and	r2, r6, r2	@ tmp525, tmp524, tmp525
	ldr	r6, [fp, #-84]	@, %sfp
	add	r2, r2, r7, asl #20	@, tmp529, tmp525,,
	add	r7, r6, #2	@,,
	add	r6, r7, #2	@,,
	str	r7, [fp, #-72]	@, %sfp
	ldr	r7, [fp, #-52]	@, %sfp
	add	r5, r2, r5	@ tmp533, tmp529, tmp531
	str	r6, [fp, #-92]	@, %sfp
	str	r5, [r7, #268]	@ tmp533, <variable>.s32CoefAttr
	ldr	r7, [fp, #-64]	@, %sfp
	ldr	r6, [fp, #-76]	@, %sfp
	ldr	r2, [fp, #-80]	@, %sfp
	ldrsh	r7, [r7, #2]	@,
	ldrsh	r5, [r6, #2]	@ tmp534,
	ldrsh	r6, [r2, #2]	@ tmp541,
	mov	r2, r3	@ tmp585, tmp577
	str	r7, [fp, #-64]	@, %sfp
	ldr	r7, [fp, #-92]	@, %sfp
	add	r7, r7, #2	@,,
	str	r7, [fp, #-68]	@, %sfp
	mov	r7, r5, asl #10	@ tmp535, tmp534,
	ldr	r5, [fp, #-68]	@, %sfp
	and	r3, r7, r3	@ tmp536, tmp535, tmp536
	ldr	r7, [fp, #-64]	@, %sfp
	add	r5, r5, #2	@,,
	str	r5, [fp, #-80]	@, %sfp
	mov	r5, r6, asl #22	@ tmp542, tmp541,
	add	r6, r3, r7, asl #20	@, tmp540, tmp536,,
	ldr	r7, [fp, #-52]	@, %sfp
	mov	r5, r5, lsr #22	@ tmp542, tmp542,
	mov	r3, r4	@ tmp596, tmp585
	add	r5, r6, r5	@ tmp544, tmp540, tmp542
	str	r5, [r7, #272]	@ tmp544, <variable>.s32CoefAttr
	ldr	r7, [fp, #-48]	@, %sfp
	ldr	r6, [fp, #-80]	@, %sfp
	ldrsh	r5, [sl, #2]	@ tmp549,
	add	sl, r6, #2	@ ps16Coef.1120,,
	ldrsh	r6, [r7, #2]	@ tmp545,
	add	r7, sl, #2	@, ps16Coef.1120,
	str	r7, [fp, #-76]	@, %sfp
	add	r7, r7, #2	@,,
	str	r7, [fp, #-64]	@, %sfp
	mov	r6, r6, asl #10	@, tmp545,
	str	r6, [fp, #-112]	@, %sfp
	ldr	r6, [fp, #-64]	@, %sfp
	mov	r7, r4	@ tmp607, tmp596
	mov	r5, r5, asl #22	@ tmp550, tmp549,
	add	r6, r6, #2	@,,
	str	r6, [fp, #-48]	@, %sfp
	ldr	r6, [fp, #-112]	@, %sfp
	mov	r5, r5, lsr #22	@ tmp550, tmp550,
	and	r4, r6, r4	@ tmp547,, tmp547
	ldr	r6, [fp, #-52]	@, %sfp
	add	r4, r4, r5	@ tmp552, tmp547, tmp550
	str	r4, [r6, #276]	@ tmp552, <variable>.s32CoefAttr
	ldr	r5, [fp, #-56]	@, %sfp
	ldr	r6, [fp, #-48]	@, %sfp
	ldrsh	r4, [r5, #2]	@ tmp553,
	add	r5, r6, #2	@,,
	ldr	r6, [fp, #-60]	@, %sfp
	str	r5, [fp, #-56]	@, %sfp
	ldrsh	r5, [r6, #2]	@ tmp560,
	ldr	r6, [fp, #-56]	@, %sfp
	add	r6, r6, #2	@,,
	str	r6, [fp, #-120]	@, %sfp
	ldrsh	r6, [r8, #2]	@,
	mov	r8, r4, asl #10	@ tmp554, tmp553,
	mov	r4, r5, asl #22	@ tmp561, tmp560,
	str	r6, [fp, #-60]	@, %sfp
	mov	r6, ip	@ tmp615, tmp607
	ldr	r5, [fp, #-60]	@, %sfp
	and	ip, r8, ip	@ tmp555, tmp554, tmp555
	ldr	r8, [fp, #-120]	@, %sfp
	mov	r4, r4, lsr #22	@ tmp561, tmp561,
	add	ip, ip, r5, asl #20	@, tmp559, tmp555,,
	add	r5, r8, #2	@,,
	add	r8, r5, #2	@,,
	str	r8, [fp, #-116]	@, %sfp
	add	r8, ip, r4	@ tmp563, tmp559, tmp561
	ldr	ip, [fp, #-52]	@, %sfp
	str	r5, [fp, #-60]	@, %sfp
	str	r8, [ip, #280]	@ tmp563, <variable>.s32CoefAttr
	ldr	r4, [fp, #-88]	@, %sfp
	ldr	r5, [fp, #-96]	@, %sfp
	ldrsh	r9, [r9, #2]	@,
	ldrsh	ip, [r4, #2]	@ tmp564,
	ldr	r8, [fp, #-116]	@, %sfp
	ldrsh	r4, [r5, #2]	@ tmp571,
	mov	r5, r0	@ tmp626, tmp615
	str	r9, [fp, #-88]	@, %sfp
	add	r9, r8, #2	@ ps16Coef.1160,,
	mov	r8, ip, asl #10	@ tmp565, tmp564,
	add	ip, r9, #2	@, ps16Coef.1160,
	and	r0, r8, r0	@ tmp566, tmp565, tmp566
	ldr	r8, [fp, #-88]	@, %sfp
	str	ip, [fp, #-140]	@, %sfp
	mov	ip, r4, asl #22	@ tmp572, tmp571,
	ldr	r4, [fp, #-140]	@, %sfp
	add	r0, r0, r8, asl #20	@, tmp570, tmp566,,
	mov	ip, ip, lsr #22	@ tmp572, tmp572,
	add	ip, r0, ip	@ tmp574, tmp570, tmp572
	ldr	r0, [fp, #-52]	@, %sfp
	add	r8, r4, #2	@,,
	str	r8, [fp, #-136]	@, %sfp
	add	r4, r8, #2	@,,
	str	r4, [fp, #-132]	@, %sfp
	str	ip, [r0, #284]	@ tmp574, <variable>.s32CoefAttr
	ldr	r8, [fp, #-108]	@, %sfp
	ldr	r4, [fp, #-104]	@, %sfp
	ldr	ip, [fp, #-132]	@, %sfp
	ldrsh	r0, [r8, #2]	@ tmp579,
	ldrsh	r8, [r4, #2]	@ tmp575,
	add	ip, ip, #2	@,,
	str	ip, [fp, #-112]	@, %sfp
	mov	r4, r1	@ tmp637, tmp626
	mov	r0, r0, asl #22	@ tmp580, tmp579,
	mov	ip, r1	@ tmp645, tmp637
	mov	r8, r8, asl #10	@, tmp575,
	str	r8, [fp, #-88]	@, %sfp
	ldr	r8, [fp, #-112]	@, %sfp
	add	r8, r8, #2	@,,
	str	r8, [fp, #-128]	@, %sfp
	ldr	r8, [fp, #-88]	@, %sfp
	and	r1, r8, r1	@ tmp577,, tmp577
	mov	r8, r0, lsr #22	@ tmp580, tmp580,
	ldr	r0, [fp, #-52]	@, %sfp
	add	r1, r1, r8	@ tmp582, tmp577, tmp580
	str	r1, [r0, #288]	@ tmp582, <variable>.s32CoefAttr
	ldr	r8, [fp, #-84]	@, %sfp
	ldr	r0, [fp, #-128]	@, %sfp
	ldrsh	r1, [r8, #2]	@ tmp583,
	add	r8, r0, #2	@,,
	ldr	r0, [fp, #-100]	@, %sfp
	str	r8, [fp, #-96]	@, %sfp
	ldrsh	r8, [r0, #2]	@ tmp590,
	ldr	r0, [fp, #-96]	@, %sfp
	add	r0, r0, #2	@,,
	str	r0, [fp, #-88]	@, %sfp
	ldr	r0, [fp, #-72]	@, %sfp
	ldrsh	r0, [r0, #2]	@,
	str	r0, [fp, #-84]	@, %sfp
	mov	r0, r1, asl #10	@, tmp583,
	str	r0, [fp, #-72]	@, %sfp
	mov	r1, r8, asl #22	@ tmp591, tmp590,
	ldr	r8, [fp, #-72]	@, %sfp
	mov	r0, r2	@ tmp656, tmp645
	mov	r1, r1, lsr #22	@ tmp591, tmp591,
	and	r2, r8, r2	@ tmp585,, tmp585
	ldr	r8, [fp, #-84]	@, %sfp
	add	r2, r2, r8, asl #20	@, tmp589, tmp585,,
	ldr	r8, [fp, #-88]	@, %sfp
	add	r1, r2, r1	@ tmp593, tmp589, tmp591
	ldr	r2, [fp, #-52]	@, %sfp
	add	r8, r8, #2	@,,
	str	r8, [fp, #-108]	@, %sfp
	add	r8, r8, #2	@,,
	str	r8, [fp, #-104]	@, %sfp
	str	r1, [r2, #292]	@ tmp593, <variable>.s32CoefAttr
	ldr	r8, [fp, #-68]	@, %sfp
	ldr	r1, [fp, #-104]	@, %sfp
	ldrsh	r2, [r8, #2]	@ tmp594,
	add	r8, r1, #2	@,,
	ldr	r1, [fp, #-92]	@, %sfp
	str	r8, [fp, #-100]	@, %sfp
	ldrsh	r8, [r1, #2]	@ tmp601,
	ldr	r1, [fp, #-100]	@, %sfp
	add	r1, r1, #2	@,,
	str	r1, [fp, #-84]	@, %sfp
	ldr	r1, [fp, #-80]	@, %sfp
	ldrsh	r1, [r1, #2]	@,
	str	r1, [fp, #-68]	@, %sfp
	mov	r1, r2, asl #10	@, tmp594,
	str	r1, [fp, #-80]	@, %sfp
	mov	r2, r8, asl #22	@ tmp602, tmp601,
	ldr	r8, [fp, #-80]	@, %sfp
	mov	r1, r3	@ tmp667, tmp656
	mov	r2, r2, lsr #22	@ tmp602, tmp602,
	and	r3, r8, r3	@ tmp596,, tmp596
	ldr	r8, [fp, #-68]	@, %sfp
	add	r3, r3, r8, asl #20	@, tmp600, tmp596,,
	ldr	r8, [fp, #-84]	@, %sfp
	add	r2, r3, r2	@ tmp604, tmp600, tmp602
	ldr	r3, [fp, #-52]	@, %sfp
	add	r8, r8, #2	@,,
	str	r8, [fp, #-72]	@, %sfp
	add	r8, r8, #2	@,,
	str	r8, [fp, #-92]	@, %sfp
	str	r2, [r3, #296]	@ tmp604, <variable>.s32CoefAttr
	ldr	r8, [fp, #-76]	@, %sfp
	ldrsh	sl, [sl, #2]	@ tmp609,
	ldrsh	r2, [r8, #2]	@,
	mov	sl, sl, asl #22	@ tmp610, tmp609,
	str	r2, [fp, #-76]	@, %sfp
	mov	sl, sl, lsr #22	@ tmp610, tmp610,
	ldr	r3, [fp, #-92]	@, %sfp
	mov	r2, r7	@ tmp675, tmp667
	add	r8, r3, #2	@,,
	str	r8, [fp, #-68]	@, %sfp
	ldr	r3, [fp, #-68]	@, %sfp
	add	r8, r3, #2	@,,
	ldr	r3, [fp, #-76]	@, %sfp
	str	r8, [fp, #-80]	@, %sfp
	mov	r8, r3, asl #10	@,,
	mov	r3, r7	@ tmp686, tmp675
	and	r7, r8, r7	@ tmp607,, tmp607
	str	r8, [fp, #-76]	@, %sfp
	add	r8, r7, sl	@ tmp612, tmp607, tmp610
	ldr	sl, [fp, #-52]	@, %sfp
	str	r8, [sl, #300]	@ tmp612, <variable>.s32CoefAttr
	ldr	sl, [fp, #-48]	@, %sfp
	ldr	r8, [fp, #-80]	@, %sfp
	ldrsh	r7, [sl, #2]	@,
	add	r8, r8, #2	@,,
	ldr	sl, [fp, #-64]	@, %sfp
	str	r8, [fp, #-76]	@, %sfp
	str	r7, [fp, #-124]	@, %sfp
	ldrsh	r7, [sl, #2]	@,
	str	r7, [fp, #-144]	@, %sfp
	add	r7, r8, #2	@,,
	ldr	r8, [fp, #-56]	@, %sfp
	str	r7, [fp, #-48]	@, %sfp
	ldrsh	sl, [r8, #2]	@,
	ldr	r8, [fp, #-124]	@, %sfp
	str	sl, [fp, #-148]	@, %sfp
	add	sl, r7, #2	@,,
	mov	r7, r8, asl #10	@ tmp614,,
	str	sl, [fp, #-64]	@, %sfp
	add	r8, sl, #2	@,,
	ldr	sl, [fp, #-144]	@, %sfp
	str	r8, [fp, #-56]	@, %sfp
	mov	r8, sl, asl #22	@ tmp621,,
	and	sl, r7, r6	@ tmp615, tmp614, tmp615
	ldr	r7, [fp, #-148]	@, %sfp
	add	r6, sl, r7, asl #20	@, tmp619, tmp615,,
	mov	sl, r8, lsr #22	@ tmp621, tmp621,
	add	r8, r6, sl	@ tmp623, tmp619, tmp621
	ldr	r6, [fp, #-52]	@, %sfp
	mov	r7, r5	@ tmp697, tmp686
	str	r8, [r6, #304]	@ tmp623, <variable>.s32CoefAttr
	ldr	sl, [fp, #-60]	@, %sfp
	ldr	r6, [fp, #-56]	@, %sfp
	ldrsh	r8, [sl, #2]	@ tmp624,
	add	sl, r6, #2	@,,
	ldr	r6, [fp, #-120]	@, %sfp
	str	sl, [fp, #-60]	@, %sfp
	ldrsh	sl, [r6, #2]	@ tmp631,
	ldr	r6, [fp, #-60]	@, %sfp
	add	r6, r6, #2	@,,
	str	r6, [fp, #-156]	@, %sfp
	ldr	r6, [fp, #-116]	@, %sfp
	ldrsh	r6, [r6, #2]	@,
	str	r6, [fp, #-120]	@, %sfp
	mov	r6, r8, asl #10	@, tmp624,
	str	r6, [fp, #-116]	@, %sfp
	mov	r8, sl, asl #22	@ tmp632, tmp631,
	ldr	sl, [fp, #-116]	@, %sfp
	mov	r6, r5	@ tmp705, tmp697
	mov	r8, r8, lsr #22	@ tmp632, tmp632,
	and	r5, sl, r5	@ tmp626,, tmp626
	ldr	sl, [fp, #-120]	@, %sfp
	add	r5, r5, sl, asl #20	@, tmp630, tmp626,,
	ldr	sl, [fp, #-156]	@, %sfp
	add	r8, r5, r8	@ tmp634, tmp630, tmp632
	add	sl, sl, #2	@,,
	str	sl, [fp, #-152]	@, %sfp
	add	sl, sl, #2	@,,
	str	sl, [fp, #-124]	@, %sfp
	ldr	sl, [fp, #-52]	@, %sfp
	str	r8, [sl, #308]	@ tmp634, <variable>.s32CoefAttr
	ldr	r5, [fp, #-140]	@, %sfp
	ldrsh	sl, [r9, #2]	@ tmp639,
	ldr	r9, [fp, #-124]	@, %sfp
	ldrsh	r8, [r5, #2]	@,
	add	r5, r9, #2	@,,
	str	r5, [fp, #-148]	@, %sfp
	ldr	r9, [fp, #-148]	@, %sfp
	mov	sl, sl, asl #22	@ tmp640, tmp639,
	str	r8, [fp, #-116]	@, %sfp
	mov	r8, r4	@ tmp716, tmp705
	add	r5, r9, #2	@,,
	ldr	r9, [fp, #-116]	@, %sfp
	str	r5, [fp, #-144]	@, %sfp
	mov	r5, r9, asl #10	@,,
	ldr	r9, [fp, #-144]	@, %sfp
	str	r5, [fp, #-116]	@, %sfp
	add	r5, r9, #2	@,,
	ldr	r9, [fp, #-116]	@, %sfp
	str	r5, [fp, #-180]	@, %sfp
	and	r5, r9, r4	@ tmp637,, tmp637
	mov	r9, sl, lsr #22	@ tmp640, tmp640,
	ldr	sl, [fp, #-52]	@, %sfp
	add	r4, r5, r9	@ tmp642, tmp637, tmp640
	str	r4, [sl, #312]	@ tmp642, <variable>.s32CoefAttr
	ldr	r9, [fp, #-136]	@, %sfp
	ldr	r5, [fp, #-132]	@, %sfp
	ldrsh	sl, [r9, #2]	@ tmp650,
	ldr	r9, [fp, #-180]	@, %sfp
	ldrsh	r4, [r5, #2]	@ tmp643,
	mov	r5, ip	@ tmp727, tmp716
	add	r9, r9, #2	@,,
	str	r9, [fp, #-168]	@, %sfp
	ldr	r9, [fp, #-112]	@, %sfp
	ldrsh	r9, [r9, #2]	@,
	str	r9, [fp, #-112]	@, %sfp
	ldr	r9, [fp, #-168]	@, %sfp
	add	r9, r9, #2	@,,
	str	r9, [fp, #-164]	@, %sfp
	mov	r9, r4, asl #10	@ tmp644, tmp643,
	ldr	r4, [fp, #-164]	@, %sfp
	and	ip, r9, ip	@ tmp645, tmp644, tmp645
	ldr	r9, [fp, #-112]	@, %sfp
	add	r4, r4, #2	@,,
	str	r4, [fp, #-176]	@, %sfp
	mov	r4, sl, asl #22	@ tmp651, tmp650,
	add	sl, ip, r9, asl #20	@, tmp649, tmp645,,
	ldr	r9, [fp, #-52]	@, %sfp
	mov	r4, r4, lsr #22	@ tmp651, tmp651,
	mov	ip, r0	@ tmp735, tmp727
	add	r4, sl, r4	@ tmp653, tmp649, tmp651
	str	r4, [r9, #316]	@ tmp653, <variable>.s32CoefAttr
	ldr	sl, [fp, #-96]	@, %sfp
	ldr	r9, [fp, #-176]	@, %sfp
	ldrsh	r4, [sl, #2]	@ tmp654,
	add	sl, r9, #2	@,,
	ldr	r9, [fp, #-128]	@, %sfp
	str	sl, [fp, #-172]	@, %sfp
	ldrsh	sl, [r9, #2]	@ tmp661,
	ldr	r9, [fp, #-172]	@, %sfp
	add	r9, r9, #2	@,,
	str	r9, [fp, #-160]	@, %sfp
	ldr	r9, [fp, #-88]	@, %sfp
	ldrsh	r9, [r9, #2]	@,
	str	r9, [fp, #-96]	@, %sfp
	mov	r9, r4, asl #10	@, tmp654,
	str	r9, [fp, #-88]	@, %sfp
	mov	r4, sl, asl #22	@ tmp662, tmp661,
	ldr	sl, [fp, #-88]	@, %sfp
	mov	r9, r0	@ tmp746, tmp735
	mov	r4, r4, lsr #22	@ tmp662, tmp662,
	and	r0, sl, r0	@ tmp656,, tmp656
	ldr	sl, [fp, #-96]	@, %sfp
	add	r0, r0, sl, asl #20	@, tmp660, tmp656,,
	ldr	sl, [fp, #-160]	@, %sfp
	add	r4, r0, r4	@ tmp664, tmp660, tmp662
	ldr	r0, [fp, #-52]	@, %sfp
	add	sl, sl, #2	@,,
	str	sl, [fp, #-120]	@, %sfp
	add	sl, sl, #2	@,,
	str	sl, [fp, #-116]	@, %sfp
	str	r4, [r0, #320]	@ tmp664, <variable>.s32CoefAttr
	add	sl, sl, #2	@,,
	ldr	r4, [fp, #-108]	@, %sfp
	ldrsh	r0, [r4, #2]	@ tmp669,
	ldr	r4, [fp, #-104]	@, %sfp
	str	sl, [fp, #-140]	@, %sfp
	add	sl, sl, #2	@,,
	mov	r0, r0, asl #22	@ tmp670, tmp669,
	ldrsh	r4, [r4, #2]	@,
	str	sl, [fp, #-136]	@, %sfp
	mov	sl, r1	@ tmp757, tmp746
	mov	r0, r0, lsr #22	@ tmp670, tmp670,
	str	r4, [fp, #-88]	@, %sfp
	ldr	r4, [fp, #-136]	@, %sfp
	add	r4, r4, #2	@,,
	str	r4, [fp, #-132]	@, %sfp
	ldr	r4, [fp, #-88]	@, %sfp
	mov	r4, r4, asl #10	@,,
	str	r4, [fp, #-88]	@, %sfp
	ldr	r4, [fp, #-132]	@, %sfp
	add	r4, r4, #2	@,,
	str	r4, [fp, #-112]	@, %sfp
	ldr	r4, [fp, #-88]	@, %sfp
	and	r1, r4, r1	@ tmp667,, tmp667
	ldr	r4, [fp, #-52]	@, %sfp
	add	r1, r1, r0	@ tmp672, tmp667, tmp670
	str	r1, [r4, #324]	@ tmp672, <variable>.s32CoefAttr
	ldr	r1, [fp, #-84]	@, %sfp
	ldr	r0, [fp, #-100]	@, %sfp
	ldrsh	r4, [r1, #2]	@ tmp673,
	ldrsh	r1, [r0, #2]	@,
	ldr	r0, [fp, #-72]	@, %sfp
	mov	r4, r4, asl #10	@, tmp673,
	str	r1, [fp, #-84]	@, %sfp
	mov	r1, r2	@ tmp765, tmp757
	ldrsh	r0, [r0, #2]	@,
	str	r4, [fp, #-88]	@, %sfp
	str	r0, [fp, #-72]	@, %sfp
	ldr	r0, [fp, #-112]	@, %sfp
	add	r0, r0, #2	@,,
	str	r0, [fp, #-128]	@, %sfp
	add	r4, r0, #2	@,,
	ldr	r0, [fp, #-84]	@, %sfp
	str	r4, [fp, #-96]	@, %sfp
	mov	r4, r0, asl #22	@ tmp681,,
	ldr	r0, [fp, #-88]	@, %sfp
	and	r2, r0, r2	@ tmp675,, tmp675
	ldr	r0, [fp, #-72]	@, %sfp
	mov	r4, r4, lsr #22	@ tmp681, tmp681,
	add	r2, r2, r0, asl #20	@, tmp679, tmp675,,
	mov	r0, r3	@ tmp776, tmp765
	add	r4, r2, r4	@ tmp683, tmp679, tmp681
	ldr	r2, [fp, #-52]	@, %sfp
	str	r4, [r2, #328]	@ tmp683, <variable>.s32CoefAttr
	ldr	r2, [fp, #-68]	@, %sfp
	ldrsh	r4, [r2, #2]	@,
	ldr	r2, [fp, #-96]	@, %sfp
	str	r4, [fp, #-72]	@, %sfp
	add	r4, r2, #2	@,,
	ldr	r2, [fp, #-92]	@, %sfp
	str	r4, [fp, #-88]	@, %sfp
	ldrsh	r4, [r2, #2]	@ tmp691,
	ldr	r2, [fp, #-88]	@, %sfp
	add	r2, r2, #2	@,,
	str	r2, [fp, #-108]	@, %sfp
	ldr	r2, [fp, #-80]	@, %sfp
	ldrsh	r2, [r2, #2]	@,
	str	r2, [fp, #-68]	@, %sfp
	ldr	r2, [fp, #-108]	@, %sfp
	add	r2, r2, #2	@,,
	str	r2, [fp, #-104]	@, %sfp
	ldr	r2, [fp, #-72]	@, %sfp
	mov	r2, r2, asl #10	@,,
	str	r2, [fp, #-80]	@, %sfp
	ldr	r2, [fp, #-104]	@, %sfp
	add	r2, r2, #2	@,,
	str	r2, [fp, #-100]	@, %sfp
	mov	r2, r4, asl #22	@ tmp692, tmp691,
	ldr	r4, [fp, #-80]	@, %sfp
	and	r3, r4, r3	@ tmp686,, tmp686
	ldr	r4, [fp, #-68]	@, %sfp
	mov	r2, r2, lsr #22	@ tmp692, tmp692,
	add	r3, r3, r4, asl #20	@, tmp690, tmp686,,
	mov	r4, r7	@ tmp787, tmp776
	add	r2, r3, r2	@ tmp694, tmp690, tmp692
	ldr	r3, [fp, #-52]	@, %sfp
	str	r2, [r3, #332]	@ tmp694, <variable>.s32CoefAttr
	ldr	r3, [fp, #-76]	@, %sfp
	ldrsh	r2, [r3, #2]	@,
	ldr	r3, [fp, #-100]	@, %sfp
	str	r2, [fp, #-76]	@, %sfp
	add	r3, r3, #2	@,,
	ldr	r2, [fp, #-48]	@, %sfp
	str	r3, [fp, #-84]	@, %sfp
	ldrsh	r2, [r2, #2]	@,
	str	r7, [fp, #-48]	@ tmp3, %sfp
	str	r2, [fp, #-68]	@, %sfp
	add	r2, r3, #2	@,,
	str	r2, [fp, #-72]	@, %sfp
	ldr	r2, [fp, #-76]	@, %sfp
	mov	r3, r2, asl #22	@ tmp700,,
	ldr	r2, [fp, #-72]	@, %sfp
	add	r2, r2, #2	@,,
	str	r2, [fp, #-92]	@, %sfp
	ldr	r2, [fp, #-68]	@, %sfp
	mov	r3, r3, lsr #22	@ tmp700, tmp700,
	mov	r2, r2, asl #10	@,,
	str	r2, [fp, #-76]	@, %sfp
	ldr	r2, [fp, #-92]	@, %sfp
	add	r2, r2, #2	@,,
	str	r2, [fp, #-68]	@, %sfp
	ldr	r2, [fp, #-76]	@, %sfp
	and	r7, r2, r7	@ tmp697,, tmp697
	ldr	r2, [fp, #-52]	@, %sfp
	add	r3, r7, r3	@ tmp702, tmp697, tmp700
	str	r3, [r2, #336]	@ tmp702, <variable>.s32CoefAttr
	movw	r2, #:lower16:1047552	@,
	ldr	r7, [fp, #-56]	@, %sfp
	ldrsh	r3, [r7, #2]	@ tmp703,
	ldr	r7, [fp, #-64]	@, %sfp
	str	r2, [fp, #-56]	@, %sfp
	ldrsh	r2, [r7, #2]	@ tmp710,
	str	r6, [fp, #-56]	@ tmp7, %sfp
	ldr	r7, [fp, #-60]	@, %sfp
	ldrsh	r7, [r7, #2]	@,
	str	r7, [fp, #-60]	@, %sfp
	ldr	r7, [fp, #-68]	@, %sfp
	add	r7, r7, #2	@,,
	str	r7, [fp, #-80]	@, %sfp
	mov	r7, r3, asl #10	@ tmp704, tmp703,
	ldr	r3, [fp, #-80]	@, %sfp
	and	r6, r7, r6	@ tmp705, tmp704, tmp705
	add	r3, r3, #2	@,,
	str	r3, [fp, #-76]	@, %sfp
	mov	r3, r2, asl #22	@ tmp711, tmp710,
	ldr	r2, [fp, #-60]	@, %sfp
	ldr	r7, [fp, #-76]	@, %sfp
	mov	r3, r3, lsr #22	@ tmp711, tmp711,
	add	r6, r6, r2, asl #20	@, tmp709, tmp705,,
	add	r2, r7, #2	@,,
	add	r3, r6, r3	@ tmp713, tmp709, tmp711
	ldr	r6, [fp, #-52]	@, %sfp
	str	r2, [fp, #-64]	@, %sfp
	add	r7, r2, #2	@,,
	str	r7, [fp, #-60]	@, %sfp
	str	r3, [r6, #340]	@ tmp713, <variable>.s32CoefAttr
	ldr	r6, [fp, #-124]	@, %sfp
	ldr	r2, [fp, #-152]	@, %sfp
	ldr	r7, [fp, #-156]	@, %sfp
	ldrsh	r6, [r6, #2]	@,
	ldrsh	r3, [r2, #2]	@ tmp714,
	ldrsh	r2, [r7, #2]	@ tmp721,
	mov	r7, r8	@ tmp817, tmp787
	str	r6, [fp, #-124]	@, %sfp
	ldr	r6, [fp, #-60]	@, %sfp
	add	r6, r6, #2	@,,
	str	r6, [fp, #-156]	@, %sfp
	mov	r6, r3, asl #10	@ tmp715, tmp714,
	ldr	r3, [fp, #-156]	@, %sfp
	add	r3, r3, #2	@,,
	str	r3, [fp, #-152]	@, %sfp
	mov	r3, r2, asl #22	@ tmp722, tmp721,
	and	r2, r6, r8	@ tmp716, tmp715, tmp716
	ldr	r6, [fp, #-124]	@, %sfp
	add	r8, r2, r6, asl #20	@, tmp720, tmp716,,
	mov	r2, r3, lsr #22	@ tmp722, tmp722,
	add	r3, r8, r2	@ tmp724, tmp720, tmp722
	ldr	r8, [fp, #-52]	@, %sfp
	mov	r6, r5	@ tmp825, tmp817
	str	r3, [r8, #344]	@ tmp724, <variable>.s32CoefAttr
	ldr	r2, [fp, #-148]	@, %sfp
	ldr	r3, [fp, #-152]	@, %sfp
	ldrsh	r8, [r2, #2]	@ tmp729,
	add	r3, r3, #2	@,,
	ldr	r2, [fp, #-144]	@, %sfp
	str	r3, [fp, #-124]	@, %sfp
	add	r3, r3, #2	@,,
	mov	r8, r8, asl #22	@, tmp729,
	ldrsh	r2, [r2, #2]	@,
	str	r2, [fp, #-184]	@, %sfp
	mov	r2, r5	@ tmp836, tmp825
	str	r8, [fp, #-192]	@, %sfp
	add	r8, r3, #2	@,,
	str	r8, [fp, #-144]	@, %sfp
	ldr	r8, [fp, #-184]	@, %sfp
	str	r3, [fp, #-148]	@, %sfp
	mov	r3, r8, asl #10	@,,
	str	r3, [fp, #-184]	@, %sfp
	ldr	r3, [fp, #-192]	@, %sfp
	ldr	r8, [fp, #-184]	@, %sfp
	and	r5, r8, r5	@ tmp727,, tmp727
	mov	r8, r3, lsr #22	@,,
	ldr	r3, [fp, #-52]	@, %sfp
	add	r5, r5, r8	@ tmp732, tmp727,
	str	r5, [r3, #348]	@ tmp732, <variable>.s32CoefAttr
	ldr	r8, [fp, #-168]	@, %sfp
	ldr	r5, [fp, #-180]	@, %sfp
	ldrsh	r3, [r8, #2]	@,
	str	r3, [fp, #-184]	@, %sfp
	mov	r3, ip	@ tmp847, tmp836
	ldrsh	r8, [r5, #2]	@ tmp740,
	ldr	r5, [fp, #-164]	@, %sfp
	ldrsh	r5, [r5, #2]	@,
	str	r5, [fp, #-168]	@, %sfp
	ldr	r5, [fp, #-184]	@, %sfp
	mov	r5, r5, asl #10	@,,
	str	r5, [fp, #-164]	@, %sfp
	mov	r5, r8, asl #22	@ tmp741, tmp740,
	ldr	r8, [fp, #-164]	@, %sfp
	and	ip, r8, ip	@ tmp735,, tmp735
	ldr	r8, [fp, #-168]	@, %sfp
	mov	r5, r5, lsr #22	@ tmp741, tmp741,
	add	ip, ip, r8, asl #20	@, tmp739, tmp735,,
	ldr	r8, [fp, #-52]	@, %sfp
	add	ip, ip, r5	@ tmp743, tmp739, tmp741
	str	ip, [r8, #352]	@ tmp743, <variable>.s32CoefAttr
	ldr	r5, [fp, #-172]	@, %sfp
	ldr	r8, [fp, #-176]	@, %sfp
	ldrsh	ip, [r5, #2]	@ tmp744,
	ldrsh	r5, [r8, #2]	@ tmp751,
	ldr	r8, [fp, #-160]	@, %sfp
	ldrsh	r8, [r8, #2]	@,
	str	r8, [fp, #-160]	@, %sfp
	mov	r8, ip, asl #10	@ tmp745, tmp744,
	mov	ip, r5, asl #22	@ tmp752, tmp751,
	ldr	r5, [fp, #-160]	@, %sfp
	and	r9, r8, r9	@ tmp746, tmp745, tmp746
	mov	r8, ip, lsr #22	@ tmp752, tmp752,
	add	r9, r9, r5, asl #20	@, tmp750, tmp746,,
	add	ip, r9, r8	@ tmp754, tmp750, tmp752
	ldr	r9, [fp, #-52]	@, %sfp
	str	ip, [r9, #356]	@ tmp754, <variable>.s32CoefAttr
	ldr	r8, [fp, #-116]	@, %sfp
	ldr	r5, [fp, #-120]	@, %sfp
	ldrsh	r9, [r8, #2]	@ tmp755,
	ldrsh	ip, [r5, #2]	@ tmp759,
	mov	r5, r9, asl #10	@ tmp756, tmp755,
	mov	r8, ip, asl #22	@ tmp760, tmp759,
	and	sl, r5, sl	@ tmp757, tmp756, tmp757
	ldr	r5, [fp, #-52]	@, %sfp
	mov	r9, r8, lsr #22	@ tmp760, tmp760,
	add	ip, sl, r9	@ tmp762, tmp757, tmp760
	str	ip, [r5, #360]	@ tmp762, <variable>.s32CoefAttr
	ldr	r8, [fp, #-136]	@, %sfp
	ldr	sl, [fp, #-140]	@, %sfp
	ldr	r9, [fp, #-132]	@, %sfp
	ldrsh	ip, [r8, #2]	@ tmp763,
	ldrsh	r5, [sl, #2]	@ tmp770,
	ldrsh	sl, [r9, #2]	@ tmp767,
	mov	r8, ip, asl #10	@ tmp764, tmp763,
	mov	ip, r5, asl #22	@ tmp771, tmp770,
	and	r5, r8, r1	@ tmp765, tmp764, tmp765
	add	r9, r5, sl, asl #20	@, tmp769, tmp765, tmp767,
	mov	sl, ip, lsr #22	@ tmp771, tmp771,
	add	r1, r9, sl	@ tmp773, tmp769, tmp771
	ldr	sl, [fp, #-52]	@, %sfp
	str	r1, [sl, #364]	@ tmp773, <variable>.s32CoefAttr
	ldr	r5, [fp, #-128]	@, %sfp
	ldr	r8, [fp, #-112]	@, %sfp
	ldr	r9, [fp, #-96]	@, %sfp
	ldrsh	r1, [r5, #2]	@ tmp774,
	ldrsh	ip, [r8, #2]	@ tmp781,
	ldrsh	r8, [r9, #2]	@ tmp778,
	mov	r5, r1, asl #10	@ tmp775, tmp774,
	mov	r1, ip, asl #22	@ tmp782, tmp781,
	and	r9, r5, r0	@ tmp776, tmp775, tmp776
	add	r0, r9, r8, asl #20	@, tmp780, tmp776, tmp778,
	mov	r8, r1, lsr #22	@ tmp782, tmp782,
	add	ip, r0, r8	@ tmp784, tmp780, tmp782
	str	ip, [sl, #368]	@ tmp784, <variable>.s32CoefAttr
	ldr	r9, [fp, #-88]	@, %sfp
	ldr	r5, [fp, #-108]	@, %sfp
	ldrsh	r1, [r9, #2]	@ tmp789,
	ldrsh	r0, [r5, #2]	@ tmp785,
	mov	ip, r1, asl #22	@ tmp790, tmp789,
	mov	r8, r0, asl #10	@ tmp786, tmp785,
	ldr	r0, [fp, #-52]	@, %sfp
	and	sl, r8, r4	@ tmp787, tmp786, tmp787
	mov	r9, ip, lsr #22	@ tmp790, tmp790,
	add	r5, sl, r9	@ tmp792, tmp787, tmp790
	str	r5, [r0, #372]	@ tmp792, <variable>.s32CoefAttr
	ldr	r4, [fp, #-100]	@, %sfp
	ldr	r1, [fp, #-104]	@, %sfp
	ldr	r8, [fp, #-84]	@, %sfp
	ldrsh	ip, [r4, #2]	@ tmp793,
	ldrsh	sl, [r1, #2]	@ tmp800,
	ldrsh	r4, [r8, #2]	@ tmp797,
	ldr	r5, [fp, #-48]	@, %sfp
	mov	r9, ip, asl #10	@ tmp794, tmp793,
	mov	r1, sl, asl #22	@ tmp801, tmp800,
	and	r0, r9, r5	@, tmp794,
	add	ip, r0, r4, asl #20	@, tmp799,, tmp797,
	mov	sl, r1, lsr #22	@ tmp801, tmp801,
	add	r8, ip, sl	@ tmp803, tmp799, tmp801
	ldr	sl, [fp, #-52]	@, %sfp
	str	r8, [sl, #376]	@ tmp803, <variable>.s32CoefAttr
	ldr	r9, [fp, #-92]	@, %sfp
	ldr	r5, [fp, #-72]	@, %sfp
	ldr	r4, [fp, #-68]	@, %sfp
	ldrsh	r0, [r9, #2]	@ tmp804,
	ldrsh	ip, [r5, #2]	@ tmp811,
	ldrsh	r9, [r4, #2]	@ tmp808,
	ldr	r8, [fp, #-56]	@, %sfp
	mov	r1, r0, asl #10	@ tmp805, tmp804,
	mov	r0, ip, asl #22	@ tmp812, tmp811,
	and	r5, r1, r8	@, tmp805,
	add	r4, r5, r9, asl #20	@, tmp810,, tmp808,
	mov	ip, r0, lsr #22	@ tmp812, tmp812,
	add	r9, r4, ip	@ tmp814, tmp810, tmp812
	str	r9, [sl, #380]	@ tmp814, <variable>.s32CoefAttr
	ldr	r8, [fp, #-80]	@, %sfp
	ldr	r5, [fp, #-76]	@, %sfp
	ldrsh	r1, [r8, #2]	@ tmp819,
	ldrsh	r4, [r5, #2]	@ tmp815,
	ldr	r8, [fp, #-52]	@, %sfp
	mov	sl, r1, asl #22	@ tmp820, tmp819,
	mov	r0, r4, asl #10	@ tmp816, tmp815,
	and	ip, r0, r7	@ tmp817, tmp816, tmp817
	mov	r9, sl, lsr #22	@ tmp820, tmp820,
	add	r5, ip, r9	@ tmp822, tmp817, tmp820
	str	r5, [r8, #384]	@ tmp822, <variable>.s32CoefAttr
	ldr	r4, [fp, #-60]	@, %sfp
	ldr	r5, [fp, #-156]	@, %sfp
	ldrsh	r7, [r4, #2]	@ tmp823,
	ldr	r4, [fp, #-64]	@, %sfp
	ldrsh	ip, [r5, #2]	@ tmp827,
	mov	r1, r7, asl #10	@ tmp824, tmp823,
	ldrsh	r0, [r4, #2]	@ tmp830,
	and	sl, r1, r6	@ tmp825, tmp824, tmp825
	add	r8, sl, ip, asl #20	@, tmp829, tmp825, tmp827,
	mov	r9, r0, asl #22	@ tmp831, tmp830,
	mov	r6, r9, lsr #22	@ tmp831, tmp831,
	add	r7, r8, r6	@ tmp833, tmp829, tmp831
	ldr	r6, [fp, #-52]	@, %sfp
	str	r7, [r6, #388]	@ tmp833, <variable>.s32CoefAttr
	ldr	r7, [fp, #-124]	@, %sfp
	ldr	r8, [fp, #-152]	@, %sfp
	ldr	r9, [fp, #-148]	@, %sfp
	ldrsh	sl, [r7, #2]	@ tmp834,
	ldrsh	r1, [r8, #2]	@ tmp841,
	ldrsh	r0, [r9, #2]	@ tmp838,
	mov	ip, sl, asl #10	@ tmp835, tmp834,
	mov	sl, r1, asl #22	@ tmp842, tmp841,
	and	r2, ip, r2	@ tmp836, tmp835, tmp836
	add	ip, r2, r0, asl #20	@, tmp840, tmp836, tmp838,
	mov	r0, #204	@ tmp853,
	mov	r1, sl, lsr #22	@ tmp842, tmp842,
	add	sl, ip, r1	@ tmp844, tmp840, tmp842
	str	sl, [r6, #392]	@ tmp844, <variable>.s32CoefAttr
	ldr	sl, [fp, #-144]	@, %sfp
	ldrsh	r2, [sl, #2]	@ tmp849,
	ldrsh	ip, [sl, #4]	@ tmp845,
	str	r0, [r6, #0]	@ tmp853, <variable>.u32Size
	mov	r0, r2, asl #22	@ tmp850, tmp849,
	mov	r1, ip, asl #10	@ tmp846, tmp845,
	and	ip, r1, r3	@ tmp847, tmp846, tmp847
	mov	r2, r0, lsr #22	@ tmp850, tmp850,
	add	r0, ip, r2	@ tmp852, tmp847, tmp850
	str	r0, [r6, #396]	@ tmp852, <variable>.s32CoefAttr
	b	.L106	@
.L126:
	mov	r9, r3, lsr #1	@ prephitmp.716,,
.L103:
	mov	r1, #0	@,
	str	r1, [fp, #-48]	@, %sfp
	mov	ip, r1	@ j,
.L105:
	cmp	r9, #0	@ prephitmp.716,
	beq	.L110	@,
	ldr	r2, [fp, #-56]	@, %sfp
	add	r6, ip, #48	@ tmp865, j,
	ldrsh	r1, [r4, #0]	@ s32CoefTmp1.777,* ps16Coef
	sub	r7, r9, #1	@ tmp876, prephitmp.716,
	cmp	r2, #16	@,
	ldr	r8, [fp, #-52]	@, %sfp
	ldr	r0, [fp, #-56]	@, %sfp
	and	r7, r7, #1	@ tmp877, tmp876,
	ldrsh	r2, [r4, #2]	@ s32CoefTmp2.779,
	add	sl, r8, r6, asl #2	@, tmp867,, tmp865,
	moveq	r3, #1	@ tmp906,
	movne	r3, #0	@ tmp906,
	cmp	r0, #16	@,
	movne	r8, #0	@, pretmp.717
	moveq	r8, #1	@, pretmp.717
	cmp	r3, #0	@ tmp906,
	uxth	r5, r1	@ tmp901, s32CoefTmp1.777
	add	r6, sl, #4	@ ivtmp.740, tmp867,
	add	r3, r5, r2, asl #16	@, D.26401, tmp901, s32CoefTmp2.779,
	add	r4, r4, #4	@ ps16Coef, ps16Coef,
	str	r3, [sl, #4]	@ D.26401, <variable>.s32CoefAttr
	add	sl, ip, #1	@ j.1340, j,
	bne	.L127	@,
.L113:
	mov	r3, #2	@ k,
	cmp	r3, r9	@ k, prephitmp.716
	add	r5, r6, #4	@ ivtmp.740, ivtmp.740,
	bhi	.L123	@,
	cmp	r7, #0	@ tmp877,
	beq	.L108	@,
	ldrsh	r0, [r4, #0]	@ s32CoefTmp1.777,* ps16Coef
	add	sl, r3, ip	@ j.1340, k, j
	ldrsh	r1, [r4, #2]	@ s32CoefTmp2.779,
	add	r3, r3, #1	@ k, k,
	cmp	r3, r9	@ k, prephitmp.716
	add	r4, r4, #4	@ ps16Coef, ps16Coef,
	uxth	r2, r0	@ tmp911, s32CoefTmp1.777
	add	r5, r5, #4	@ ivtmp.740, ivtmp.740,
	add	r0, r2, r1, asl #16	@, D.26401, tmp911, s32CoefTmp2.779,
	str	r0, [r6, #4]	@ D.26401, <variable>.s32CoefAttr
	bhi	.L123	@,
.L108:
	ldrsh	r1, [r4, #0]	@ s32CoefTmp1.777,* ps16Coef
	cmp	r3, #1	@ k,
	movne	r6, #0	@, tmp861
	andeq	r6, r8, #1	@,, tmp861, pretmp.717
	ldrsh	r2, [r4, #2]	@ s32CoefTmp2.779,
	cmp	r6, #0	@ tmp861,
	add	r6, r4, #4	@ tmp880, ps16Coef,
	add	r7, r3, #1	@ tmp878, k,
	uxth	sl, r1	@ tmp855, s32CoefTmp1.777
	add	r4, r6, #4	@ ps16Coef, tmp880,
	add	r3, sl, r2, asl #16	@, D.26401, tmp855, s32CoefTmp2.779,
	str	r3, [r5, #0]	@ D.26401, <variable>.s32CoefAttr
	bne	.L128	@,
.L107:
	ldrsh	r1, [r6, #0]	@ s32CoefTmp1.777,
	cmp	r7, #1	@ tmp878,
	movne	r0, #0	@, tmp894
	andeq	r0, r8, #1	@,, tmp894, pretmp.717
	ldrsh	r2, [r6, #2]	@ s32CoefTmp2.779,
	cmp	r0, #0	@ tmp894,
	add	r6, r5, #4	@ tmp879, ivtmp.740,
	add	sl, r7, ip	@ j.1340, tmp878, j
	uxth	r3, r1	@ tmp888, s32CoefTmp1.777
	add	r3, r3, r2, asl #16	@, D.26401, tmp888, s32CoefTmp2.779,
	str	r3, [r5, #4]	@ D.26401, <variable>.s32CoefAttr
	bne	.L129	@,
.L118:
	add	r3, r7, #1	@ k, tmp878,
	add	r5, r6, #4	@ ivtmp.740, tmp879,
	cmp	r3, r9	@ k, prephitmp.716
	bls	.L108	@,
.L123:
	mov	ip, sl	@ j, j.1340
.L110:
	ldr	r1, [fp, #-48]	@, %sfp
	add	r8, r1, #1	@,,
	str	r8, [fp, #-48]	@, %sfp
	cmp	r8, #17	@,
	bne	.L105	@,
	ldr	r8, [fp, #-52]	@, %sfp
	add	r9, r9, r9, asl #4	@, tmp870, prephitmp.716, prephitmp.716,
	mov	ip, r9, asl #2	@ tmp871, tmp870,
	str	ip, [r8, #0]	@ tmp871, <variable>.u32Size
.L106:
	mov	r0, #0	@,
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, sl, fp, sp, pc}
.L129:
	movw	r0, #:lower16:.LC0	@,
	movt	r0, #:upper16:.LC0	@,
	str	ip, [fp, #-188]	@,
	bl	printk	@
	ldr	ip, [fp, #-188]	@,
	b	.L118	@
.L128:
	movw	r0, #:lower16:.LC0	@,
	movt	r0, #:upper16:.LC0	@,
	str	ip, [fp, #-188]	@,
	bl	printk	@
	ldr	ip, [fp, #-188]	@,
	b	.L107	@
.L127:
	movw	r0, #:lower16:.LC0	@,
	movt	r0, #:upper16:.LC0	@,
	str	ip, [fp, #-188]	@,
	bl	printk	@
	ldr	ip, [fp, #-188]	@,
	b	.L113	@
	.fnend
	.size	VZmeTransVpssCoefAlign, .-VZmeTransVpssCoefAlign
	.align	2
	.type	VZmeLoadVpssCoefHL, %function
VZmeLoadVpssCoefHL:
	.fnstart
.LFB1234:
	@ args = 0, pretend = 0, frame = 400
	@ frame_needed = 1, uses_anonymous_args = 0
	.movsp ip
	mov	ip, sp	@,
	stmfd	sp!, {r4, r5, r6, r7, r8, fp, ip, lr, pc}	@,
	.pad #4
	.save {r4, r5, r6, r7, r8, fp, ip, lr}
	.setfp fp, ip, #-4
	sub	fp, ip, #4	@,,
	.pad #404
	sub	sp, sp, #404	@,,
	push	{lr}
	bl	__gnu_mcount_nc
	movw	r4, #:lower16:.LANCHOR1	@ tmp136,
	add	r3, r0, r0, asl #2	@, tmp139, enCoefRatio, enCoefRatio,
	sub	r5, fp, #436	@ tmp143,,
	movt	r4, #:upper16:.LANCHOR1	@ tmp136,
	mov	r6, r0	@ enCoefRatio, enCoefRatio
	add	r7, r5, #196	@ tmp147, tmp143,
	mov	r8, r1	@ pu32Addr, pu32Addr
	ldr	r0, [r4, r3, asl #2]	@, g_pVZmeCoef
	mov	r2, r5	@, tmp143
	mov	r1, #0	@,
	add	r4, r4, r6, asl #4	@, tmp155, tmp136, enCoefRatio,
	bl	VZmeTransVpssCoefAlign	@
	mov	r1, r7	@, tmp147
	ldr	r2, [fp, #-436]	@, stArray.u32Size
	mov	r0, r8	@, pu32Addr
	bl	memcpy	@
	mov	r2, r5	@, tmp143
	mov	r1, #0	@,
	ldr	r0, [r4, #140]	@, g_pVDtiZmeCoef
	bl	VZmeTransVpssCoefAlign	@
	add	r0, r8, #256	@, pu32Addr,
	mov	r1, r7	@, tmp147
	ldr	r2, [fp, #-436]	@, stArray.u32Size
	bl	memcpy	@
	mov	r0, #0	@,
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
	.fnend
	.size	VZmeLoadVpssCoefHL, .-VZmeLoadVpssCoefHL
	.align	2
	.type	VZmeLoadVpssCoefHC, %function
VZmeLoadVpssCoefHC:
	.fnstart
.LFB1235:
	@ args = 0, pretend = 0, frame = 400
	@ frame_needed = 1, uses_anonymous_args = 0
	.movsp ip
	mov	ip, sp	@,
	stmfd	sp!, {r4, r5, r6, r7, r8, fp, ip, lr, pc}	@,
	.pad #4
	.save {r4, r5, r6, r7, r8, fp, ip, lr}
	.setfp fp, ip, #-4
	sub	fp, ip, #4	@,,
	.pad #404
	sub	sp, sp, #404	@,,
	push	{lr}
	bl	__gnu_mcount_nc
	mov	r6, r0	@ enCoefRatio, enCoefRatio
	add	r0, r0, r0, asl #2	@, tmp140, enCoefRatio, enCoefRatio,
	movw	r4, #:lower16:.LANCHOR1	@ tmp136,
	movt	r4, #:upper16:.LANCHOR1	@ tmp136,
	sub	r5, fp, #436	@ tmp145,,
	add	r3, r4, r0, asl #2	@, tmp142, tmp136, tmp140,
	add	r7, r5, #196	@ tmp149, tmp145,
	mov	r8, r1	@ pu32Addr, pu32Addr
	mov	r2, r5	@, tmp145
	ldr	r0, [r3, #12]	@, g_pVZmeCoef
	mov	r1, #2	@,
	add	r4, r4, r6, asl #4	@, tmp158, tmp136, enCoefRatio,
	bl	VZmeTransVpssCoefAlign	@
	mov	r1, r7	@, tmp149
	ldr	r2, [fp, #-436]	@, stArray.u32Size
	mov	r0, r8	@, pu32Addr
	bl	memcpy	@
	mov	r2, r5	@, tmp145
	mov	r1, #2	@,
	ldr	r0, [r4, #148]	@, g_pVDtiZmeCoef
	bl	VZmeTransVpssCoefAlign	@
	add	r0, r8, #256	@, pu32Addr,
	mov	r1, r7	@, tmp149
	ldr	r2, [fp, #-436]	@, stArray.u32Size
	bl	memcpy	@
	mov	r0, #0	@,
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
	.fnend
	.size	VZmeLoadVpssCoefHC, .-VZmeLoadVpssCoefHC
	.align	2
	.type	VZmeLoadVpssCoefVL, %function
VZmeLoadVpssCoefVL:
	.fnstart
.LFB1236:
	@ args = 0, pretend = 0, frame = 400
	@ frame_needed = 1, uses_anonymous_args = 0
	.movsp ip
	mov	ip, sp	@,
	stmfd	sp!, {r4, r5, r6, r7, r8, fp, ip, lr, pc}	@,
	.pad #4
	.save {r4, r5, r6, r7, r8, fp, ip, lr}
	.setfp fp, ip, #-4
	sub	fp, ip, #4	@,,
	.pad #404
	sub	sp, sp, #404	@,,
	push	{lr}
	bl	__gnu_mcount_nc
	add	r2, r0, r0, asl #2	@, tmp139, enCoefRatio, enCoefRatio,
	add	r3, r2, #1	@ tmp140, tmp139,
	sub	r5, fp, #436	@ tmp142,,
	mov	r8, r0, asl #2	@ tmp138, enCoefRatio,
	movw	r4, #:lower16:.LANCHOR1	@ tmp136,
	movt	r4, #:upper16:.LANCHOR1	@ tmp136,
	add	r6, r5, #196	@ tmp146, tmp142,
	mov	r7, r1	@ pu32Addr, pu32Addr
	mov	r2, r5	@, tmp142
	ldr	r0, [r4, r3, asl #2]	@, g_pVZmeCoef
	mov	r1, #1	@,
	add	r4, r4, r8, asl #2	@, tmp156, tmp136, tmp138,
	bl	VZmeTransVpssCoefAlign	@
	mov	r1, r6	@, tmp146
	ldr	r2, [fp, #-436]	@, stArray.u32Size
	mov	r0, r7	@, pu32Addr
	bl	memcpy	@
	mov	r2, r5	@, tmp142
	mov	r1, #3	@,
	ldr	r0, [r4, #144]	@, g_pVDtiZmeCoef
	bl	VZmeTransVpssCoefAlign	@
	add	r0, r7, #256	@, pu32Addr,
	mov	r1, r6	@, tmp146
	ldr	r2, [fp, #-436]	@, stArray.u32Size
	bl	memcpy	@
	mov	r0, #0	@,
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
	.fnend
	.size	VZmeLoadVpssCoefVL, .-VZmeLoadVpssCoefVL
	.align	2
	.type	VZmeLoadVpssCoefVC, %function
VZmeLoadVpssCoefVC:
	.fnstart
.LFB1237:
	@ args = 0, pretend = 0, frame = 400
	@ frame_needed = 1, uses_anonymous_args = 0
	.movsp ip
	mov	ip, sp	@,
	stmfd	sp!, {r4, r5, r6, r7, r8, fp, ip, lr, pc}	@,
	.pad #4
	.save {r4, r5, r6, r7, r8, fp, ip, lr}
	.setfp fp, ip, #-4
	sub	fp, ip, #4	@,,
	.pad #404
	sub	sp, sp, #404	@,,
	push	{lr}
	bl	__gnu_mcount_nc
	mov	r6, r0	@ enCoefRatio, enCoefRatio
	add	r0, r0, r0, asl #2	@, tmp140, enCoefRatio, enCoefRatio,
	movw	r4, #:lower16:.LANCHOR1	@ tmp136,
	movt	r4, #:upper16:.LANCHOR1	@ tmp136,
	sub	r5, fp, #436	@ tmp145,,
	add	r3, r4, r0, asl #2	@, tmp142, tmp136, tmp140,
	add	r7, r5, #196	@ tmp149, tmp145,
	mov	r8, r1	@ pu32Addr, pu32Addr
	mov	r2, r5	@, tmp145
	ldr	r0, [r3, #16]	@, g_pVZmeCoef
	mov	r1, #2	@,
	add	r4, r4, r6, asl #4	@, tmp157, tmp136, enCoefRatio,
	bl	VZmeTransVpssCoefAlign	@
	mov	r1, r7	@, tmp149
	ldr	r2, [fp, #-436]	@, stArray.u32Size
	mov	r0, r8	@, pu32Addr
	bl	memcpy	@
	mov	r2, r5	@, tmp145
	mov	r1, #3	@,
	ldr	r0, [r4, #152]	@, g_pVDtiZmeCoef
	bl	VZmeTransVpssCoefAlign	@
	add	r0, r8, #256	@, pu32Addr,
	mov	r1, r7	@, tmp149
	ldr	r2, [fp, #-436]	@, stArray.u32Size
	bl	memcpy	@
	mov	r0, #0	@,
	sub	sp, fp, #32
	ldmfd	sp, {r4, r5, r6, r7, r8, fp, sp, pc}
	.fnend
	.size	VZmeLoadVpssCoefVC, .-VZmeLoadVpssCoefVC
	.align	2
	.global	VZmeLoadVpssCoefHV
	.type	VZmeLoadVpssCoefHV, %function
VZmeLoadVpssCoefHV:
	.fnstart
.LFB1238:
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
	mov	r4, r0	@ pstVZmeCoefMem, pstVZmeCoefMem
	ldr	r6, [r4, #0]	@ pu32CurAddr, <variable>.stMBuf.u32StartVirAddr
	mov	r0, #0	@,
	ldr	r5, [r4, #4]	@ u32PhyAddr, <variable>.stMBuf.u32StartPhyAddr
	add	r8, r6, #4048	@ pu32CurAddr.1561, pu32CurAddr,
	mov	r1, r6	@, pu32CurAddr
	add	r7, r5, #464	@ u32PhyAddr.1544, u32PhyAddr,
	bl	VZmeLoadVpssCoefHL	@
	add	r1, r6, #464	@, pu32CurAddr,
	str	r5, [r4, #12]	@ u32PhyAddr, <variable>.stZmeCoefAddr.u32ZmeCoefAddrHL8_1
	mov	r0, #1	@,
	bl	VZmeLoadVpssCoefHL	@
	add	r1, r6, #928	@, pu32CurAddr,
	str	r7, [r4, #16]	@ u32PhyAddr.1544, <variable>.stZmeCoefAddr.u32ZmeCoefAddrHL8_E1
	mov	r0, #2	@,
	bl	VZmeLoadVpssCoefHL	@
	add	ip, r5, #928	@ u32PhyAddr.1546, u32PhyAddr,
	add	r1, r6, #1392	@, pu32CurAddr,
	str	ip, [r4, #20]	@ u32PhyAddr.1546, <variable>.stZmeCoefAddr.u32ZmeCoefAddrHL8_075
	mov	r0, #3	@,
	add	sl, r5, #1392	@ u32PhyAddr.1548, u32PhyAddr,
	bl	VZmeLoadVpssCoefHL	@
	add	r1, r6, #1856	@, pu32CurAddr,
	str	sl, [r4, #24]	@ u32PhyAddr.1548, <variable>.stZmeCoefAddr.u32ZmeCoefAddrHL8_05
	mov	r0, #4	@,
	bl	VZmeLoadVpssCoefHL	@
	add	r1, r5, #1856	@ u32PhyAddr.1550, u32PhyAddr,
	mov	r0, #5	@,
	str	r1, [r4, #28]	@ u32PhyAddr.1550, <variable>.stZmeCoefAddr.u32ZmeCoefAddrHL8_033
	add	r1, r6, #2320	@, pu32CurAddr,
	add	r7, r5, #2320	@ u32PhyAddr.1552, u32PhyAddr,
	bl	VZmeLoadVpssCoefHL	@
	add	r1, r6, #2784	@, pu32CurAddr,
	str	r7, [r4, #32]	@ u32PhyAddr.1552, <variable>.stZmeCoefAddr.u32ZmeCoefAddrHL8_025
	mov	r0, #6	@,
	bl	VZmeLoadVpssCoefHL	@
	add	r2, r5, #2784	@ u32PhyAddr.1554, u32PhyAddr,
	add	r1, r6, #3248	@, pu32CurAddr,
	str	r2, [r4, #36]	@ u32PhyAddr.1554, <variable>.stZmeCoefAddr.u32ZmeCoefAddrHL8_0
	mov	r0, #0	@,
	add	sl, r5, #3248	@ u32PhyAddr.1556, u32PhyAddr,
	bl	VZmeLoadVpssCoefHC	@
	add	r1, r6, #3648	@, pu32CurAddr,
	str	sl, [r4, #40]	@ u32PhyAddr.1556, <variable>.stZmeCoefAddr.u32ZmeCoefAddrHC4_1
	mov	r0, #1	@,
	bl	VZmeLoadVpssCoefHC	@
	add	r7, r5, #3648	@ u32PhyAddr.1558, u32PhyAddr,
	mov	r1, r8	@, pu32CurAddr.1561
	str	r7, [r4, #44]	@ u32PhyAddr.1558, <variable>.stZmeCoefAddr.u32ZmeCoefAddrHC4_E1
	mov	r0, #2	@,
	add	r7, r5, #4048	@ u32PhyAddr.1560, u32PhyAddr,
	bl	VZmeLoadVpssCoefHC	@
	str	r7, [r4, #48]	@ u32PhyAddr.1560, <variable>.stZmeCoefAddr.u32ZmeCoefAddrHC4_075
	add	r1, r8, #400	@, pu32CurAddr.1561,
	mov	r0, #3	@,
	bl	VZmeLoadVpssCoefHC	@
	add	r0, r7, #400	@ u32PhyAddr.1562, u32PhyAddr.1560,
	add	r1, r8, #800	@, pu32CurAddr.1561,
	str	r0, [r4, #52]	@ u32PhyAddr.1562, <variable>.stZmeCoefAddr.u32ZmeCoefAddrHC4_05
	mov	r0, #4	@,
	add	sl, r7, #800	@ u32PhyAddr.1564, u32PhyAddr.1560,
	bl	VZmeLoadVpssCoefHC	@
	add	r1, r6, #5248	@, pu32CurAddr,
	str	sl, [r4, #56]	@ u32PhyAddr.1564, <variable>.stZmeCoefAddr.u32ZmeCoefAddrHC4_033
	mov	r0, #5	@,
	bl	VZmeLoadVpssCoefHC	@
	add	r3, r5, #5248	@ u32PhyAddr.1566, u32PhyAddr,
	add	r1, r8, #1600	@, pu32CurAddr.1561,
	str	r3, [r4, #60]	@ u32PhyAddr.1566, <variable>.stZmeCoefAddr.u32ZmeCoefAddrHC4_025
	mov	r0, #6	@,
	add	sl, r7, #1600	@ u32PhyAddr.1568, u32PhyAddr.1560,
	bl	VZmeLoadVpssCoefHC	@
	str	sl, [r4, #64]	@ u32PhyAddr.1568, <variable>.stZmeCoefAddr.u32ZmeCoefAddrHC4_0
	add	r1, r8, #2000	@, pu32CurAddr.1561,
	mov	r0, #0	@,
	bl	VZmeLoadVpssCoefVL	@
	add	ip, r7, #2000	@ u32PhyAddr.1570, u32PhyAddr.1560,
	add	r1, r8, #2336	@, pu32CurAddr.1561,
	str	ip, [r4, #68]	@ u32PhyAddr.1570, <variable>.stZmeCoefAddr.u32ZmeCoefAddrVL6_1
	mov	r0, #1	@,
	add	sl, r7, #2336	@ u32PhyAddr.1572, u32PhyAddr.1560,
	bl	VZmeLoadVpssCoefVL	@
	add	r1, r6, #6720	@, pu32CurAddr,
	str	sl, [r4, #72]	@ u32PhyAddr.1572, <variable>.stZmeCoefAddr.u32ZmeCoefAddrVL6_E1
	mov	r0, #2	@,
	bl	VZmeLoadVpssCoefVL	@
	add	r1, r5, #6720	@ u32PhyAddr.1574, u32PhyAddr,
	mov	r0, #3	@,
	str	r1, [r4, #76]	@ u32PhyAddr.1574, <variable>.stZmeCoefAddr.u32ZmeCoefAddrVL6_075
	add	r1, r8, #3008	@, pu32CurAddr.1561,
	add	sl, r7, #3008	@ u32PhyAddr.1576, u32PhyAddr.1560,
	bl	VZmeLoadVpssCoefVL	@
	str	sl, [r4, #80]	@ u32PhyAddr.1576, <variable>.stZmeCoefAddr.u32ZmeCoefAddrVL6_05
	add	r1, r8, #3344	@, pu32CurAddr.1561,
	mov	r0, #4	@,
	bl	VZmeLoadVpssCoefVL	@
	add	sl, r8, #4352	@ pu32CurAddr.1585, pu32CurAddr.1561,
	add	r2, r7, #3344	@ u32PhyAddr.1578, u32PhyAddr.1560,
	add	r1, r8, #3680	@, pu32CurAddr.1561,
	str	r2, [r4, #84]	@ u32PhyAddr.1578, <variable>.stZmeCoefAddr.u32ZmeCoefAddrVL6_033
	mov	r0, #5	@,
	bl	VZmeLoadVpssCoefVL	@
	add	r1, r6, #8064	@, pu32CurAddr,
	add	r0, r7, #3680	@ u32PhyAddr.1580, u32PhyAddr.1560,
	str	r0, [r4, #88]	@ u32PhyAddr.1580, <variable>.stZmeCoefAddr.u32ZmeCoefAddrVL6_025
	mov	r0, #6	@,
	add	r5, r5, #8064	@ u32PhyAddr.1582, u32PhyAddr,
	bl	VZmeLoadVpssCoefVL	@
	str	r5, [r4, #92]	@ u32PhyAddr.1582, <variable>.stZmeCoefAddr.u32ZmeCoefAddrVL6_0
	mov	r1, sl	@, pu32CurAddr.1585
	mov	r0, #0	@,
	add	r5, r7, #4352	@ u32PhyAddr.1584, u32PhyAddr.1560,
	bl	VZmeLoadVpssCoefVC	@
	str	r5, [r4, #96]	@ u32PhyAddr.1584, <variable>.stZmeCoefAddr.u32ZmeCoefAddrVC4_1
	add	r1, sl, #336	@, pu32CurAddr.1585,
	mov	r0, #1	@,
	add	r6, r5, #336	@ u32PhyAddr.1586, u32PhyAddr.1584,
	bl	VZmeLoadVpssCoefVC	@
	str	r6, [r4, #100]	@ u32PhyAddr.1586, <variable>.stZmeCoefAddr.u32ZmeCoefAddrVC4_E1
	add	r1, sl, #672	@, pu32CurAddr.1585,
	mov	r0, #2	@,
	bl	VZmeLoadVpssCoefVC	@
	add	r3, r5, #672	@ u32PhyAddr.1588, u32PhyAddr.1584,
	add	r1, sl, #1008	@, pu32CurAddr.1585,
	str	r3, [r4, #104]	@ u32PhyAddr.1588, <variable>.stZmeCoefAddr.u32ZmeCoefAddrVC4_075
	mov	r0, #3	@,
	add	r6, r5, #1008	@ u32PhyAddr.1590, u32PhyAddr.1584,
	bl	VZmeLoadVpssCoefVC	@
	str	r6, [r4, #108]	@ u32PhyAddr.1590, <variable>.stZmeCoefAddr.u32ZmeCoefAddrVC4_05
	add	r1, r8, #5696	@, pu32CurAddr.1561,
	mov	r0, #4	@,
	bl	VZmeLoadVpssCoefVC	@
	add	r7, r7, #5696	@ u32PhyAddr.1592, u32PhyAddr.1560,
	add	r1, sl, #1680	@, pu32CurAddr.1585,
	str	r7, [r4, #112]	@ u32PhyAddr.1592, <variable>.stZmeCoefAddr.u32ZmeCoefAddrVC4_033
	mov	r0, #5	@,
	add	r6, r5, #1680	@ u32PhyAddr.1594, u32PhyAddr.1584,
	bl	VZmeLoadVpssCoefVC	@
	str	r6, [r4, #116]	@ u32PhyAddr.1594, <variable>.stZmeCoefAddr.u32ZmeCoefAddrVC4_025
	add	r1, sl, #2016	@, pu32CurAddr.1585,
	mov	r0, #6	@,
	bl	VZmeLoadVpssCoefVC	@
	add	r5, r5, #2016	@ tmp189, u32PhyAddr.1584,
	str	r5, [r4, #120]	@ tmp189, <variable>.stZmeCoefAddr.u32ZmeCoefAddrVC4_0
	ldmfd	sp, {r4, r5, r6, r7, r8, sl, fp, sp, pc}	@
	.fnend
	.size	VZmeLoadVpssCoefHV, .-VZmeLoadVpssCoefHV
	.align	2
	.global	ALG_VZmeVpssComnInit
	.type	ALG_VZmeVpssComnInit, %function
ALG_VZmeVpssComnInit:
	.fnstart
.LFB1239:
	@ args = 0, pretend = 0, frame = 0
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
	mov	r1, #0	@,
	mov	r4, r0	@ pstVZmeCoefMem, pstVZmeCoefMem
	mov	r2, #10752	@,
	movw	r0, #:lower16:.LC1	@,
	mov	r3, r1	@,
	movt	r0, #:upper16:.LC1	@,
	str	r4, [sp, #0]	@ pstVZmeCoefMem,
	bl	HI_DRV_MMZ_AllocAndMap	@
	subs	r5, r0, #0	@ s32Ret,
	bne	.L144	@,
	mov	r0, r4	@, pstVZmeCoefMem
	bl	VZmeLoadVpssCoefHV	@
	mov	r0, r5	@ D.26475, s32Ret
.L142:
	sub	sp, fp, #20
	ldmfd	sp, {r4, r5, fp, sp, pc}
.L144:
	mov	r0, #0	@,
	mov	r1, #37	@,
	ldr	r2, .L145	@,
	movw	r3, #606	@,
	movw	ip, #:lower16:.LC2	@ tmp140,
	movt	ip, #:upper16:.LC2	@ tmp140,
	str	ip, [sp, #0]	@ tmp140,
	bl	HI_LogOut	@
	mvn	r0, #0	@ D.26475,
	b	.L142	@
.L146:
	.align	2
.L145:
	.word	.LANCHOR0+16
	.fnend
	.size	ALG_VZmeVpssComnInit, .-ALG_VZmeVpssComnInit
	.global	g_pVZmeCoef
	.global	g_pVDtiZmeCoef
	.section	.rodata
	.align	2
.LANCHOR0 = . + 0
	.type	CSWTCH.414, %object
	.size	CSWTCH.414, 16
CSWTCH.414:
	.word	8
	.word	6
	.word	4
	.word	2
	.type	__func__.26471, %object
	.size	__func__.26471, 21
__func__.26471:
	.ascii	"ALG_VZmeVpssComnInit\000"
	.data
	.align	2
.LANCHOR1 = . + 0
	.type	g_pVZmeCoef, %object
	.size	g_pVZmeCoef, 140
g_pVZmeCoef:
	.word	s16ZmeCoef_8T32P_Lanc3
	.word	s16ZmeCoef_6T32P_6M_a25
	.word	s16ZmeCoef_4T32P_5M_a15
	.word	s16ZmeCoef_4T32P_5M_a15
	.word	s16ZmeCoef_4T32P_5M_a15
	.word	s16ZmeCoef_8T32P_Cubic
	.word	s16ZmeCoef_6T32P_Cubic
	.word	s16ZmeCoef_4T32P_Cubic
	.word	s16ZmeCoef_4T32P_Cubic
	.word	s16ZmeCoef_4T32P_Cubic
	.word	s16ZmeCoef_8T32P_Lanc2
	.word	s16ZmeCoef_6T32P_5M_a25
	.word	s16ZmeCoef_4T32P_5M_a15
	.word	s16ZmeCoef_4T32P_5M_a15
	.word	s16ZmeCoef_4T32P_5M_a15
	.word	s16ZmeCoef_8T32P_Lanc2
	.word	s16ZmeCoef_6T32P_4M_a20
	.word	s16ZmeCoef_4T32P_5M_a15
	.word	s16ZmeCoef_4T32P_5M_a15
	.word	s16ZmeCoef_4T32P_5M_a15
	.word	s16ZmeCoef_8T32P_3M_a19
	.word	s16ZmeCoef_6T32P_3M_a15
	.word	s16ZmeCoef_4T32P_4M_a15
	.word	s16ZmeCoef_4T32P_4M_a15
	.word	s16ZmeCoef_4T32P_4M_a15
	.word	s16ZmeCoef_8T32P_3M_a19
	.word	s16ZmeCoef_6T32P_3M_a15
	.word	s16ZmeCoef_4T32P_4M_a15
	.word	s16ZmeCoef_4T32P_4M_a15
	.word	s16ZmeCoef_4T32P_4M_a15
	.word	s16ZmeCoef_8T32P_3M_a19
	.word	s16ZmeCoef_6T32P_3M_a15
	.word	s16ZmeCoef_4T32P_4M_a15
	.word	s16ZmeCoef_4T32P_4M_a15
	.word	s16ZmeCoef_4T32P_4M_a15
	.type	g_pVDtiZmeCoef, %object
	.size	g_pVDtiZmeCoef, 112
g_pVDtiZmeCoef:
	.word	s16ZmeCoef_8T32P_Lanc3
	.word	s16ZmeCoef_2T32P_Gus2_6M_a0_5
	.word	s16ZmeCoef_4T32P_5M_a15
	.word	s16ZmeCoef_2T32P_4M_0_1
	.word	s16ZmeCoef_8T32P_Cubic
	.word	s16ZmeCoef_2T32P_Gus2_6M_a0_5
	.word	s16ZmeCoef_4T32P_Cubic
	.word	s16ZmeCoef_2T32P_4M_0_1
	.word	s16ZmeCoef_8T32P_Lanc2
	.word	s16ZmeCoef_2T32P_Gus2_6M_a0_5
	.word	s16ZmeCoef_4T32P_5M_a15
	.word	s16ZmeCoef_2T32P_4M_0_1
	.word	s16ZmeCoef_8T32P_Lanc2
	.word	s16ZmeCoef_2T32P_Gus2_6M_a0_5
	.word	s16ZmeCoef_4T32P_5M_a15
	.word	s16ZmeCoef_2T32P_4M_0_1
	.word	s16ZmeCoef_8T32P_3M_a19
	.word	s16ZmeCoef_2T32P_Gus2_6M_a0_5
	.word	s16ZmeCoef_4T32P_4M_a15
	.word	s16ZmeCoef_2T32P_4M_0_1
	.word	s16ZmeCoef_8T32P_3M_a19
	.word	s16ZmeCoef_2T32P_Gus2_6M_a0_5
	.word	s16ZmeCoef_4T32P_4M_a15
	.word	s16ZmeCoef_2T32P_4M_0_1
	.word	s16ZmeCoef_8T32P_3M_a19
	.word	s16ZmeCoef_2T32P_Gus2_6M_a0_5
	.word	s16ZmeCoef_4T32P_4M_a15
	.word	s16ZmeCoef_2T32P_4M_0_1
	.type	s16ZmeCoef_8T32P_Lanc3, %object
	.size	s16ZmeCoef_8T32P_Lanc3, 288
s16ZmeCoef_8T32P_Lanc3:
	.short	4
	.short	-22
	.short	40
	.short	468
	.short	40
	.short	-22
	.short	4
	.short	0
	.short	3
	.short	-18
	.short	26
	.short	468
	.short	54
	.short	-26
	.short	5
	.short	0
	.short	2
	.short	-14
	.short	14
	.short	466
	.short	68
	.short	-30
	.short	6
	.short	0
	.short	2
	.short	-11
	.short	2
	.short	462
	.short	84
	.short	-34
	.short	7
	.short	0
	.short	1
	.short	-7
	.short	-9
	.short	457
	.short	100
	.short	-38
	.short	8
	.short	0
	.short	1
	.short	-4
	.short	-18
	.short	450
	.short	116
	.short	-42
	.short	9
	.short	0
	.short	1
	.short	-2
	.short	-27
	.short	443
	.short	133
	.short	-46
	.short	10
	.short	0
	.short	0
	.short	2
	.short	-35
	.short	434
	.short	151
	.short	-50
	.short	10
	.short	0
	.short	0
	.short	4
	.short	-42
	.short	425
	.short	168
	.short	-54
	.short	11
	.short	0
	.short	0
	.short	6
	.short	-49
	.short	414
	.short	186
	.short	-57
	.short	12
	.short	0
	.short	0
	.short	8
	.short	-54
	.short	401
	.short	204
	.short	-60
	.short	13
	.short	0
	.short	0
	.short	10
	.short	-58
	.short	387
	.short	222
	.short	-62
	.short	13
	.short	0
	.short	0
	.short	11
	.short	-62
	.short	374
	.short	240
	.short	-65
	.short	14
	.short	0
	.short	0
	.short	12
	.short	-65
	.short	359
	.short	258
	.short	-66
	.short	14
	.short	0
	.short	0
	.short	13
	.short	-67
	.short	344
	.short	276
	.short	-68
	.short	14
	.short	0
	.short	0
	.short	14
	.short	-68
	.short	327
	.short	293
	.short	-68
	.short	14
	.short	0
	.short	0
	.short	14
	.short	-68
	.short	310
	.short	310
	.short	-68
	.short	14
	.short	0
	.short	0
	.short	0
	.short	0
	.short	0
	.short	0
	.short	0
	.short	0
	.short	0
	.type	s16ZmeCoef_6T32P_6M_a25, %object
	.size	s16ZmeCoef_6T32P_6M_a25, 216
s16ZmeCoef_6T32P_6M_a25:
	.short	-34
	.short	78
	.short	415
	.short	78
	.short	-34
	.short	9
	.short	-32
	.short	66
	.short	415
	.short	90
	.short	-36
	.short	9
	.short	-29
	.short	54
	.short	413
	.short	103
	.short	-38
	.short	9
	.short	-26
	.short	43
	.short	411
	.short	116
	.short	-41
	.short	9
	.short	-23
	.short	33
	.short	406
	.short	129
	.short	-42
	.short	9
	.short	-21
	.short	24
	.short	401
	.short	143
	.short	-44
	.short	9
	.short	-18
	.short	14
	.short	396
	.short	157
	.short	-46
	.short	9
	.short	-16
	.short	6
	.short	389
	.short	171
	.short	-47
	.short	9
	.short	-14
	.short	-2
	.short	382
	.short	185
	.short	-48
	.short	9
	.short	-11
	.short	-9
	.short	374
	.short	199
	.short	-49
	.short	8
	.short	-9
	.short	-16
	.short	365
	.short	213
	.short	-49
	.short	8
	.short	-7
	.short	-21
	.short	354
	.short	228
	.short	-49
	.short	7
	.short	-5
	.short	-27
	.short	345
	.short	242
	.short	-49
	.short	6
	.short	-3
	.short	-32
	.short	333
	.short	256
	.short	-48
	.short	6
	.short	-1
	.short	-36
	.short	322
	.short	269
	.short	-46
	.short	4
	.short	0
	.short	-39
	.short	309
	.short	283
	.short	-44
	.short	3
	.short	2
	.short	-42
	.short	296
	.short	296
	.short	-42
	.short	2
	.short	0
	.short	0
	.short	0
	.short	0
	.short	0
	.short	0
	.type	s16ZmeCoef_4T32P_5M_a15, %object
	.size	s16ZmeCoef_4T32P_5M_a15, 144
s16ZmeCoef_4T32P_5M_a15:
	.short	103
	.short	335
	.short	103
	.short	-29
	.short	92
	.short	335
	.short	112
	.short	-27
	.short	84
	.short	335
	.short	121
	.short	-28
	.short	75
	.short	334
	.short	131
	.short	-28
	.short	67
	.short	332
	.short	141
	.short	-28
	.short	59
	.short	329
	.short	152
	.short	-28
	.short	51
	.short	326
	.short	162
	.short	-27
	.short	43
	.short	323
	.short	173
	.short	-27
	.short	36
	.short	319
	.short	183
	.short	-26
	.short	30
	.short	313
	.short	194
	.short	-25
	.short	23
	.short	308
	.short	204
	.short	-23
	.short	17
	.short	301
	.short	215
	.short	-21
	.short	12
	.short	295
	.short	225
	.short	-20
	.short	6
	.short	288
	.short	235
	.short	-17
	.short	2
	.short	280
	.short	244
	.short	-14
	.short	-3
	.short	271
	.short	254
	.short	-10
	.short	-7
	.short	263
	.short	263
	.short	-7
	.short	0
	.short	0
	.short	0
	.short	0
	.type	s16ZmeCoef_8T32P_Cubic, %object
	.size	s16ZmeCoef_8T32P_Cubic, 288
s16ZmeCoef_8T32P_Cubic:
	.short	0
	.short	0
	.short	0
	.short	511
	.short	0
	.short	0
	.short	0
	.short	0
	.short	-1
	.short	3
	.short	-12
	.short	511
	.short	14
	.short	-4
	.short	1
	.short	0
	.short	-2
	.short	6
	.short	-23
	.short	509
	.short	28
	.short	-8
	.short	2
	.short	0
	.short	-2
	.short	9
	.short	-33
	.short	503
	.short	44
	.short	-12
	.short	3
	.short	0
	.short	-3
	.short	11
	.short	-41
	.short	496
	.short	61
	.short	-16
	.short	4
	.short	0
	.short	-3
	.short	13
	.short	-48
	.short	488
	.short	79
	.short	-21
	.short	5
	.short	-1
	.short	-3
	.short	14
	.short	-54
	.short	477
	.short	98
	.short	-25
	.short	7
	.short	-2
	.short	-4
	.short	16
	.short	-59
	.short	465
	.short	118
	.short	-30
	.short	8
	.short	-2
	.short	-4
	.short	17
	.short	-63
	.short	451
	.short	138
	.short	-35
	.short	9
	.short	-1
	.short	-4
	.short	18
	.short	-66
	.short	437
	.short	158
	.short	-39
	.short	10
	.short	-2
	.short	-4
	.short	18
	.short	-68
	.short	421
	.short	180
	.short	-44
	.short	11
	.short	-2
	.short	-4
	.short	18
	.short	-69
	.short	404
	.short	201
	.short	-48
	.short	13
	.short	-3
	.short	-4
	.short	18
	.short	-70
	.short	386
	.short	222
	.short	-52
	.short	14
	.short	-2
	.short	-4
	.short	18
	.short	-70
	.short	368
	.short	244
	.short	-56
	.short	15
	.short	-3
	.short	-4
	.short	18
	.short	-69
	.short	348
	.short	265
	.short	-59
	.short	16
	.short	-3
	.short	-4
	.short	18
	.short	-67
	.short	329
	.short	286
	.short	-63
	.short	16
	.short	-3
	.short	-3
	.short	17
	.short	-65
	.short	307
	.short	307
	.short	-65
	.short	17
	.short	-3
	.short	0
	.short	0
	.short	0
	.short	0
	.short	0
	.short	0
	.short	0
	.short	0
	.type	s16ZmeCoef_6T32P_Cubic, %object
	.size	s16ZmeCoef_6T32P_Cubic, 216
s16ZmeCoef_6T32P_Cubic:
	.short	0
	.short	0
	.short	511
	.short	0
	.short	0
	.short	0
	.short	3
	.short	-12
	.short	511
	.short	13
	.short	-3
	.short	0
	.short	6
	.short	-22
	.short	507
	.short	28
	.short	-7
	.short	0
	.short	8
	.short	-32
	.short	502
	.short	44
	.short	-11
	.short	1
	.short	10
	.short	-40
	.short	495
	.short	61
	.short	-15
	.short	1
	.short	11
	.short	-47
	.short	486
	.short	79
	.short	-19
	.short	2
	.short	12
	.short	-53
	.short	476
	.short	98
	.short	-24
	.short	3
	.short	13
	.short	-58
	.short	464
	.short	117
	.short	-28
	.short	4
	.short	14
	.short	-62
	.short	451
	.short	137
	.short	-33
	.short	5
	.short	15
	.short	-65
	.short	437
	.short	157
	.short	-38
	.short	6
	.short	15
	.short	-67
	.short	420
	.short	179
	.short	-42
	.short	7
	.short	15
	.short	-68
	.short	404
	.short	200
	.short	-46
	.short	7
	.short	14
	.short	-68
	.short	386
	.short	221
	.short	-50
	.short	9
	.short	14
	.short	-68
	.short	367
	.short	243
	.short	-54
	.short	10
	.short	14
	.short	-67
	.short	348
	.short	264
	.short	-58
	.short	11
	.short	13
	.short	-66
	.short	328
	.short	286
	.short	-61
	.short	12
	.short	13
	.short	-63
	.short	306
	.short	306
	.short	-63
	.short	13
	.short	0
	.short	0
	.short	0
	.short	0
	.short	0
	.short	0
	.type	s16ZmeCoef_4T32P_Cubic, %object
	.size	s16ZmeCoef_4T32P_Cubic, 144
s16ZmeCoef_4T32P_Cubic:
	.short	0
	.short	511
	.short	0
	.short	0
	.short	-19
	.short	511
	.short	21
	.short	-1
	.short	-37
	.short	509
	.short	42
	.short	-2
	.short	-51
	.short	504
	.short	64
	.short	-5
	.short	-64
	.short	499
	.short	86
	.short	-9
	.short	-74
	.short	492
	.short	108
	.short	-14
	.short	-82
	.short	484
	.short	129
	.short	-19
	.short	-89
	.short	474
	.short	152
	.short	-25
	.short	-94
	.short	463
	.short	174
	.short	-31
	.short	-97
	.short	451
	.short	196
	.short	-38
	.short	-98
	.short	438
	.short	217
	.short	-45
	.short	-98
	.short	424
	.short	238
	.short	-52
	.short	-98
	.short	409
	.short	260
	.short	-59
	.short	-95
	.short	392
	.short	280
	.short	-65
	.short	-92
	.short	376
	.short	300
	.short	-72
	.short	-88
	.short	358
	.short	320
	.short	-78
	.short	-83
	.short	339
	.short	339
	.short	-83
	.short	0
	.short	0
	.short	0
	.short	0
	.type	s16ZmeCoef_8T32P_Lanc2, %object
	.size	s16ZmeCoef_8T32P_Lanc2, 288
s16ZmeCoef_8T32P_Lanc2:
	.short	-16
	.short	0
	.short	145
	.short	254
	.short	145
	.short	0
	.short	-16
	.short	0
	.short	-16
	.short	-2
	.short	140
	.short	253
	.short	151
	.short	3
	.short	-17
	.short	0
	.short	-15
	.short	-5
	.short	135
	.short	253
	.short	157
	.short	5
	.short	-18
	.short	0
	.short	-14
	.short	-7
	.short	129
	.short	252
	.short	162
	.short	8
	.short	-18
	.short	0
	.short	-13
	.short	-9
	.short	123
	.short	252
	.short	167
	.short	11
	.short	-19
	.short	0
	.short	-13
	.short	-11
	.short	118
	.short	250
	.short	172
	.short	15
	.short	-19
	.short	0
	.short	-12
	.short	-12
	.short	112
	.short	250
	.short	177
	.short	18
	.short	-20
	.short	-1
	.short	-11
	.short	-14
	.short	107
	.short	247
	.short	183
	.short	21
	.short	-20
	.short	-1
	.short	-10
	.short	-15
	.short	101
	.short	245
	.short	188
	.short	25
	.short	-21
	.short	-1
	.short	-9
	.short	-16
	.short	96
	.short	243
	.short	192
	.short	29
	.short	-21
	.short	-2
	.short	-8
	.short	-18
	.short	90
	.short	242
	.short	197
	.short	33
	.short	-22
	.short	-2
	.short	-8
	.short	-19
	.short	85
	.short	239
	.short	202
	.short	37
	.short	-22
	.short	-2
	.short	-7
	.short	-19
	.short	80
	.short	236
	.short	206
	.short	41
	.short	-22
	.short	-3
	.short	-7
	.short	-20
	.short	75
	.short	233
	.short	210
	.short	46
	.short	-22
	.short	-3
	.short	-6
	.short	-21
	.short	69
	.short	230
	.short	215
	.short	50
	.short	-22
	.short	-3
	.short	-5
	.short	-21
	.short	65
	.short	226
	.short	219
	.short	55
	.short	-22
	.short	-5
	.short	-5
	.short	-21
	.short	60
	.short	222
	.short	222
	.short	60
	.short	-21
	.short	-5
	.short	0
	.short	0
	.short	0
	.short	0
	.short	0
	.short	0
	.short	0
	.short	0
	.type	s16ZmeCoef_6T32P_5M_a25, %object
	.size	s16ZmeCoef_6T32P_5M_a25, 216
s16ZmeCoef_6T32P_5M_a25:
	.short	-31
	.short	104
	.short	362
	.short	104
	.short	-31
	.short	4
	.short	-30
	.short	94
	.short	362
	.short	114
	.short	-32
	.short	4
	.short	-29
	.short	84
	.short	361
	.short	125
	.short	-32
	.short	3
	.short	-28
	.short	75
	.short	359
	.short	136
	.short	-33
	.short	3
	.short	-27
	.short	66
	.short	356
	.short	147
	.short	-33
	.short	3
	.short	-25
	.short	57
	.short	353
	.short	158
	.short	-33
	.short	2
	.short	-24
	.short	49
	.short	349
	.short	169
	.short	-33
	.short	2
	.short	-22
	.short	41
	.short	344
	.short	180
	.short	-32
	.short	1
	.short	-20
	.short	33
	.short	339
	.short	191
	.short	-31
	.short	0
	.short	-19
	.short	26
	.short	333
	.short	203
	.short	-30
	.short	-1
	.short	-17
	.short	19
	.short	327
	.short	214
	.short	-29
	.short	-2
	.short	-16
	.short	13
	.short	320
	.short	225
	.short	-27
	.short	-3
	.short	-14
	.short	7
	.short	312
	.short	236
	.short	-25
	.short	-4
	.short	-13
	.short	1
	.short	305
	.short	246
	.short	-22
	.short	-5
	.short	-11
	.short	-4
	.short	295
	.short	257
	.short	-19
	.short	-6
	.short	-10
	.short	-8
	.short	286
	.short	267
	.short	-16
	.short	-7
	.short	-9
	.short	-12
	.short	277
	.short	277
	.short	-12
	.short	-9
	.short	0
	.short	0
	.short	0
	.short	0
	.short	0
	.short	0
	.type	s16ZmeCoef_6T32P_4M_a20, %object
	.size	s16ZmeCoef_6T32P_4M_a20, 216
s16ZmeCoef_6T32P_4M_a20:
	.short	-20
	.short	130
	.short	297
	.short	130
	.short	-20
	.short	-5
	.short	-21
	.short	122
	.short	298
	.short	138
	.short	-19
	.short	-6
	.short	-22
	.short	115
	.short	297
	.short	146
	.short	-17
	.short	-7
	.short	-22
	.short	108
	.short	296
	.short	153
	.short	-16
	.short	-7
	.short	-23
	.short	101
	.short	295
	.short	161
	.short	-14
	.short	-8
	.short	-23
	.short	93
	.short	294
	.short	169
	.short	-12
	.short	-9
	.short	-24
	.short	87
	.short	292
	.short	177
	.short	-10
	.short	-10
	.short	-24
	.short	80
	.short	289
	.short	185
	.short	-7
	.short	-11
	.short	-24
	.short	73
	.short	286
	.short	193
	.short	-4
	.short	-12
	.short	-23
	.short	66
	.short	283
	.short	200
	.short	-1
	.short	-13
	.short	-23
	.short	60
	.short	279
	.short	208
	.short	2
	.short	-14
	.short	-23
	.short	54
	.short	276
	.short	215
	.short	5
	.short	-15
	.short	-22
	.short	48
	.short	271
	.short	222
	.short	9
	.short	-16
	.short	-21
	.short	42
	.short	266
	.short	229
	.short	13
	.short	-17
	.short	-21
	.short	37
	.short	261
	.short	236
	.short	17
	.short	-18
	.short	-21
	.short	32
	.short	255
	.short	242
	.short	22
	.short	-18
	.short	-20
	.short	27
	.short	249
	.short	249
	.short	27
	.short	-20
	.short	0
	.short	0
	.short	0
	.short	0
	.short	0
	.short	0
	.type	s16ZmeCoef_8T32P_3M_a19, %object
	.size	s16ZmeCoef_8T32P_3M_a19, 288
s16ZmeCoef_8T32P_3M_a19:
	.short	-18
	.short	18
	.short	144
	.short	226
	.short	144
	.short	19
	.short	-17
	.short	-4
	.short	-17
	.short	16
	.short	139
	.short	226
	.short	148
	.short	21
	.short	-17
	.short	-4
	.short	-17
	.short	13
	.short	135
	.short	227
	.short	153
	.short	24
	.short	-18
	.short	-5
	.short	-17
	.short	11
	.short	131
	.short	226
	.short	157
	.short	27
	.short	-18
	.short	-5
	.short	-17
	.short	9
	.short	126
	.short	225
	.short	161
	.short	30
	.short	-17
	.short	-5
	.short	-16
	.short	6
	.short	122
	.short	225
	.short	165
	.short	33
	.short	-17
	.short	-6
	.short	-16
	.short	4
	.short	118
	.short	224
	.short	169
	.short	37
	.short	-17
	.short	-7
	.short	-16
	.short	2
	.short	113
	.short	224
	.short	173
	.short	40
	.short	-17
	.short	-7
	.short	-15
	.short	0
	.short	109
	.short	222
	.short	177
	.short	43
	.short	-17
	.short	-7
	.short	-15
	.short	-1
	.short	104
	.short	220
	.short	181
	.short	47
	.short	-16
	.short	-8
	.short	-14
	.short	-3
	.short	100
	.short	218
	.short	185
	.short	51
	.short	-16
	.short	-9
	.short	-14
	.short	-5
	.short	96
	.short	217
	.short	188
	.short	54
	.short	-15
	.short	-9
	.short	-14
	.short	-6
	.short	91
	.short	214
	.short	192
	.short	58
	.short	-14
	.short	-9
	.short	-13
	.short	-7
	.short	87
	.short	212
	.short	195
	.short	62
	.short	-14
	.short	-10
	.short	-13
	.short	-9
	.short	83
	.short	210
	.short	198
	.short	66
	.short	-13
	.short	-10
	.short	-12
	.short	-10
	.short	79
	.short	207
	.short	201
	.short	70
	.short	-12
	.short	-11
	.short	-12
	.short	-11
	.short	74
	.short	205
	.short	205
	.short	74
	.short	-11
	.short	-12
	.short	0
	.short	0
	.short	0
	.short	0
	.short	0
	.short	0
	.short	0
	.short	0
	.type	s16ZmeCoef_6T32P_3M_a15, %object
	.size	s16ZmeCoef_6T32P_3M_a15, 216
s16ZmeCoef_6T32P_3M_a15:
	.short	16
	.short	136
	.short	217
	.short	136
	.short	16
	.short	-9
	.short	13
	.short	132
	.short	217
	.short	141
	.short	18
	.short	-9
	.short	11
	.short	128
	.short	217
	.short	145
	.short	21
	.short	-10
	.short	9
	.short	124
	.short	216
	.short	149
	.short	24
	.short	-10
	.short	7
	.short	119
	.short	216
	.short	153
	.short	27
	.short	-10
	.short	5
	.short	115
	.short	216
	.short	157
	.short	30
	.short	-11
	.short	3
	.short	111
	.short	215
	.short	161
	.short	33
	.short	-11
	.short	1
	.short	107
	.short	214
	.short	165
	.short	36
	.short	-11
	.short	0
	.short	102
	.short	213
	.short	169
	.short	39
	.short	-11
	.short	-2
	.short	98
	.short	211
	.short	173
	.short	43
	.short	-11
	.short	-3
	.short	94
	.short	209
	.short	177
	.short	46
	.short	-11
	.short	-4
	.short	90
	.short	207
	.short	180
	.short	50
	.short	-11
	.short	-5
	.short	85
	.short	206
	.short	184
	.short	53
	.short	-11
	.short	-6
	.short	81
	.short	203
	.short	187
	.short	57
	.short	-10
	.short	-7
	.short	77
	.short	201
	.short	190
	.short	61
	.short	-10
	.short	-8
	.short	73
	.short	198
	.short	193
	.short	65
	.short	-9
	.short	-9
	.short	69
	.short	196
	.short	196
	.short	69
	.short	-9
	.short	0
	.short	0
	.short	0
	.short	0
	.short	0
	.short	0
	.type	s16ZmeCoef_4T32P_4M_a15, %object
	.size	s16ZmeCoef_4T32P_4M_a15, 144
s16ZmeCoef_4T32P_4M_a15:
	.short	120
	.short	281
	.short	120
	.short	-9
	.short	113
	.short	281
	.short	127
	.short	-9
	.short	106
	.short	280
	.short	134
	.short	-8
	.short	99
	.short	279
	.short	141
	.short	-7
	.short	92
	.short	277
	.short	148
	.short	-5
	.short	85
	.short	275
	.short	156
	.short	-4
	.short	79
	.short	273
	.short	162
	.short	-2
	.short	72
	.short	270
	.short	170
	.short	0
	.short	66
	.short	267
	.short	177
	.short	2
	.short	61
	.short	263
	.short	184
	.short	4
	.short	56
	.short	259
	.short	191
	.short	6
	.short	50
	.short	255
	.short	198
	.short	9
	.short	44
	.short	251
	.short	205
	.short	12
	.short	40
	.short	246
	.short	211
	.short	15
	.short	34
	.short	241
	.short	218
	.short	19
	.short	31
	.short	235
	.short	224
	.short	22
	.short	26
	.short	230
	.short	230
	.short	26
	.short	0
	.short	0
	.short	0
	.short	0
	.type	s16ZmeCoef_2T32P_Gus2_6M_a0_5, %object
	.size	s16ZmeCoef_2T32P_Gus2_6M_a0_5, 72
s16ZmeCoef_2T32P_Gus2_6M_a0_5:
	.short	462
	.short	50
	.short	449
	.short	63
	.short	437
	.short	75
	.short	424
	.short	88
	.short	411
	.short	101
	.short	398
	.short	114
	.short	386
	.short	126
	.short	373
	.short	139
	.short	360
	.short	152
	.short	347
	.short	165
	.short	334
	.short	178
	.short	321
	.short	191
	.short	308
	.short	204
	.short	295
	.short	217
	.short	282
	.short	230
	.short	269
	.short	243
	.short	256
	.short	256
	.short	0
	.short	0
	.type	s16ZmeCoef_2T32P_4M_0_1, %object
	.size	s16ZmeCoef_2T32P_4M_0_1, 72
s16ZmeCoef_2T32P_4M_0_1:
	.short	366
	.short	146
	.short	359
	.short	153
	.short	351
	.short	161
	.short	344
	.short	168
	.short	337
	.short	175
	.short	330
	.short	182
	.short	323
	.short	189
	.short	316
	.short	196
	.short	309
	.short	203
	.short	302
	.short	210
	.short	296
	.short	216
	.short	289
	.short	223
	.short	282
	.short	230
	.short	276
	.short	236
	.short	269
	.short	243
	.short	263
	.short	249
	.short	256
	.short	256
	.short	0
	.short	0
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
.LC0:
	.ascii	"\012s32CoefTmp1 %d s32CoefTmp2 %d  pBitCoef->s32Coe"
	.ascii	"fAttr[j++] %x \012 \000"
	.space	3
.LC1:
	.ascii	"VpssZmeCoef\000"
.LC2:
	.ascii	"Get zme_buf failed\012\000"
	.ident	"GCC: (Hisilicon_v200(gcc4.4-290+glibc-2.11+eabi+nptl)) 4.4.1"
	.section	.note.GNU-stack,"",%progbits

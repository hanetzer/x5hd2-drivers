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
	.file	"vpss_alg_ratio.c"
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
@ -DKBUILD_STR(s)=#s -DKBUILD_BASENAME=KBUILD_STR(vpss_alg_ratio)
@ -DKBUILD_MODNAME=KBUILD_STR(hi_vpss) -isystem
@ /opt/hisi-linux/x86-arm/arm-hisiv200-linux/bin/../lib/gcc/arm-hisiv200-linux-gnueabi/4.4.1/include
@ -include
@ /home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/include/linux/kconfig.h
@ -MD drivers/msp/vpss/vpss_v1_0/.vpss_alg_ratio.o.d
@ /home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/drivers/msp/vpss/vpss_v1_0/vpss_alg_ratio.c
@ -mlittle-endian -marm -mapcs -mno-sched-prolog -mabi=aapcs-linux
@ -mno-thumb-interwork -march=armv7-a -msoft-float -auxbase-strip
@ drivers/msp/vpss/vpss_v1_0/vpss_alg_ratio.o -O2 -Wall -Wundef
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

	.global	__aeabi_uidiv
	.text
	.align	2
	.global	ALG_RATIO_CorrectAspectRatioW_H
	.type	ALG_RATIO_CorrectAspectRatioW_H, %function
ALG_RATIO_CorrectAspectRatioW_H:
	.fnstart
.LFB580:
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
	ldr	r5, [r0, #0]	@ w,* pw
	ldr	r4, [r1, #0]	@ h,* ph
	mov	r6, r0	@ pw, pw
	mov	r7, r1	@ ph, ph
	cmp	r4, #0	@ h,
	cmpne	r5, #0	@, w,
	beq	.L11	@,
	mov	r0, r5, asl #10	@, w,
	mov	r1, r4	@, h
	bl	__aeabi_uidiv	@
	cmp	r0, #262144	@ tmp148,
	movhi	r4, #1	@ h,
	movhi	r5, #256	@ w,
	bhi	.L7	@,
	cmp	r0, #3	@ tmp148,
	movls	r4, #256	@ h,
	movls	r5, #1	@ w,
	bhi	.L11	@,
	str	r5, [r6, #0]	@ w,* pw
	str	r4, [r7, #0]	@ h,* ph
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}	@
.L9:
	mov	r5, r5, lsr #1	@ w, w,
	mov	r4, r4, lsr #1	@ h, h,
.L11:
	cmp	r5, #256	@ w,
	cmpls	r4, #256	@, h,
	bhi	.L9	@,
	cmp	r5, #0	@ w,
	moveq	r5, #1	@ w,
	cmp	r4, #0	@ h,
	moveq	r4, #1	@ h,
.L7:
	str	r5, [r6, #0]	@ w,* pw
	str	r4, [r7, #0]	@ h,* ph
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}	@
	.fnend
	.size	ALG_RATIO_CorrectAspectRatioW_H, .-ALG_RATIO_CorrectAspectRatioW_H
	.align	2
	.global	ALG_RATIO_CropedAspect
	.type	ALG_RATIO_CropedAspect, %function
ALG_RATIO_CropedAspect:
	.fnstart
.LFB581:
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
	mov	r4, r2	@ AspectRatioW, AspectRatioW
	ldr	r2, [r0, #12]	@ <variable>.s32Height, <variable>.s32Height
	mov	r6, r1	@ pCropedWnd, pCropedWnd
	ldr	lr, [r0, #8]	@ <variable>.s32Width, <variable>.s32Width
	mov	r5, r3	@ AspectRatioH, AspectRatioH
	ldr	r1, [r3, #0]	@ a_h.245,* AspectRatioH
	ldr	r0, [r4, #0]	@ a_w.244,* AspectRatioW
	mov	r3, r2, asl #10	@ tmp139, <variable>.s32Height,
	mul	r1, lr, r1	@, <variable>.s32Width, a_h.245
	mul	r0, r0, r3	@, a_w.244, tmp139
	bl	__aeabi_uidiv	@
	ldr	r3, [r6, #8]	@ <variable>.s32Width, <variable>.s32Width
	ldr	ip, [r6, #12]	@ <variable>.s32Height, <variable>.s32Height
	sub	r1, fp, #36	@,,
	mov	r2, ip, asl #10	@ tmp151, <variable>.s32Height,
	str	r2, [fp, #-36]	@ tmp151, a_h
	mul	ip, r3, r0	@ tmp149, <variable>.s32Width,
	sub	r0, fp, #32	@,,
	str	ip, [fp, #-32]	@ tmp149, a_w
	bl	ALG_RATIO_CorrectAspectRatioW_H	@
	ldr	r3, [fp, #-36]	@ a_h, a_h
	ldr	r2, [fp, #-32]	@ a_w, a_w
	str	r2, [r4, #0]	@ a_w,* AspectRatioW
	str	r3, [r5, #0]	@ a_h,* AspectRatioH
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
	.fnend
	.size	ALG_RATIO_CropedAspect, .-ALG_RATIO_CropedAspect
	.align	2
	.global	ALG_RATIO_LetterBox
	.type	ALG_RATIO_LetterBox, %function
ALG_RATIO_LetterBox:
	.fnstart
.LFB582:
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
	ldr	r5, [r2, #12]	@ D.13486, <variable>.s32Height
	mul	r1, r1, r3	@, AspectRatioH, pixr1_out
	mov	r4, r2	@ pOutWnd, pOutWnd
	mov	r3, r5, asl #10	@ tmp145, D.13486,
	mul	r0, r0, r3	@, AspectRatioW, tmp145
	bl	__aeabi_uidiv	@
	mov	r1, r0	@ tmp150,
	ldr	r0, [r4, #8]	@ D.13492, <variable>.s32Width
	cmp	r1, r0	@ tmp150, D.13492
	bls	.L20	@,
	mul	r0, r5, r0	@, D.13486, D.13492
	bl	__aeabi_uidiv	@
	ldr	r1, [r4, #4]	@ <variable>.s32Y, <variable>.s32Y
	bic	r2, r0, #3	@ v_h.350,,
	str	r2, [r4, #12]	@ v_h.350, <variable>.s32Height
	rsb	r5, r2, r5	@ tmp160, v_h.350, D.13486
	add	r0, r1, r5, lsr #1	@, tmp162, <variable>.s32Y, tmp160,
	bic	r3, r0, #3	@ tmp164, tmp162,
	str	r3, [r4, #4]	@ tmp164, <variable>.s32Y
	ldmfd	sp, {r4, r5, fp, sp, pc}	@
.L20:
	bic	lr, r1, #1	@ v_w.349, tmp150,
	ldr	ip, [r4, #0]	@ <variable>.s32X, <variable>.s32X
	rsb	r2, lr, r0	@ tmp151, v_w.349, D.13492
	str	lr, [r4, #8]	@ v_w.349, <variable>.s32Width
	add	r1, ip, r2, lsr #1	@, tmp153, <variable>.s32X, tmp151,
	bic	r0, r1, #1	@ tmp155, tmp153,
	str	r0, [r4, #0]	@ tmp155, <variable>.s32X
	ldmfd	sp, {r4, r5, fp, sp, pc}	@
	.fnend
	.size	ALG_RATIO_LetterBox, .-ALG_RATIO_LetterBox
	.global	__aeabi_idiv
	.align	2
	.global	ALG_RATIO_RatioProcess
	.type	ALG_RATIO_RatioProcess, %function
ALG_RATIO_RatioProcess:
	.fnstart
.LFB583:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	.movsp ip
	mov	ip, sp	@,
	stmfd	sp!, {r4, fp, ip, lr, pc}	@,
	.pad #4
	.save {r4, fp, ip, lr}
	.setfp fp, ip, #-4
	sub	fp, ip, #4	@,,
	.pad #28
	sub	sp, sp, #28	@,,
	push	{lr}
	bl	__gnu_mcount_nc
	ldr	r3, [r0, #16]	@ <variable>.eAspMode, <variable>.eAspMode
	mov	r4, r0	@ pstDrvPara, pstDrvPara
	cmp	r3, #7	@ <variable>.eAspMode,
	beq	.L22	@,
	ldr	r0, [r0, #20]	@ <variable>.stUsrAsp.bUserDefAspectRatio, <variable>.stUsrAsp.bUserDefAspectRatio
	cmp	r0, #1	@ <variable>.stUsrAsp.bUserDefAspectRatio,
	beq	.L28	@,
.L23:
	ldr	lr, [r4, #0]	@ ar_w.257, <variable>.AspectWidth
	ldr	ip, [r4, #4]	@ ar_h.258, <variable>.AspectHeight
	cmp	lr, #0	@ ar_w.257,
	cmpne	ip, #0	@, ar_h.258,
	str	lr, [fp, #-32]	@ ar_w.257, ar_w
	str	ip, [fp, #-36]	@ ar_h.258, ar_h
	ldreq	lr, [r4, #40]	@ <variable>.stInWnd.s32Width, <variable>.stInWnd.s32Width
	ldreq	ip, [r4, #44]	@ <variable>.stInWnd.s32Height, <variable>.stInWnd.s32Height
	streq	lr, [fp, #-32]	@ <variable>.stInWnd.s32Width, ar_w
	streq	ip, [fp, #-36]	@ <variable>.stInWnd.s32Height, ar_h
.L24:
	sub	r0, fp, #32	@,,
	sub	r1, fp, #36	@,,
	bl	ALG_RATIO_CorrectAspectRatioW_H	@
	ldr	r1, [r4, #8]	@ sr_w.263, <variable>.DeviceWidth
	ldr	r0, [r4, #12]	@ sr_h.264, <variable>.DeviceHeight
	cmp	r1, #0	@ sr_w.263,
	cmpne	r0, #0	@, sr_h.264,
	str	r1, [fp, #-24]	@ sr_w.263, sr_w
	str	r0, [fp, #-28]	@ sr_h.264, sr_h
	ldreq	r1, [r4, #72]	@ <variable>.stScreen.s32Width, <variable>.stScreen.s32Width
	ldreq	r0, [r4, #76]	@ <variable>.stScreen.s32Height, <variable>.stScreen.s32Height
	streq	r1, [fp, #-24]	@ <variable>.stScreen.s32Width, sr_w
	sub	r1, fp, #28	@,,
	streq	r0, [fp, #-28]	@ <variable>.stScreen.s32Height, sr_h
	sub	r0, fp, #24	@,,
	bl	ALG_RATIO_CorrectAspectRatioW_H	@
	ldr	r3, [r4, #16]	@ <variable>.eAspMode, <variable>.eAspMode
	ldr	r1, [r4, #72]	@ f_w, <variable>.stScreen.s32Width
	cmp	r3, #1	@ <variable>.eAspMode,
	ldr	r2, [r4, #76]	@ f_h, <variable>.stScreen.s32Height
	ldr	r0, [fp, #-24]	@ sr_w.265, sr_w
	ldr	r3, [fp, #-28]	@ sr_h.266, sr_h
	beq	.L29	@,
	movw	r2, #:lower16:.LANCHOR0	@,
	mov	r0, #0	@,
	movt	r2, #:upper16:.LANCHOR0	@,
	mov	r1, #37	@,
	mov	r3, #145	@,
	movw	lr, #:lower16:.LC0	@ tmp191,
	movt	lr, #:upper16:.LC0	@ tmp191,
	str	lr, [sp, #0]	@ tmp191,
	bl	HI_LogOut	@
.L22:
	mov	r0, #0	@,
	sub	sp, fp, #16
	ldmfd	sp, {r4, fp, sp, pc}
.L28:
	ldr	r2, [r4, #24]	@ ar_w.253, <variable>.stUsrAsp.u32UserAspectWidth
	ldr	r1, [r4, #28]	@ ar_h.254, <variable>.stUsrAsp.u32UserAspectHeight
	cmp	r2, #0	@ ar_w.253,
	cmpne	r1, #0	@, ar_h.254,
	str	r2, [fp, #-32]	@ ar_w.253, ar_w
	str	r1, [fp, #-36]	@ ar_h.254, ar_h
	bne	.L24	@,
	b	.L23	@
.L29:
	mul	ip, r0, r2	@ tmp183, sr_w.265, f_h
	add	r4, r4, #48	@ tmp182, pstDrvPara,
	mul	r1, r1, r3	@, f_w, sr_h.266
	mov	r0, ip, asl #10	@, tmp183,
	bl	__aeabi_idiv	@
	ldr	r1, [fp, #-36]	@, ar_h
	mov	r2, r4	@, tmp182
	mov	r3, r0	@ tmp189,
	ldr	r0, [fp, #-32]	@, ar_w
	bl	ALG_RATIO_LetterBox	@
	b	.L22	@
	.fnend
	.size	ALG_RATIO_RatioProcess, .-ALG_RATIO_RatioProcess
	.section	.rodata
	.align	2
.LANCHOR0 = . + 0
	.type	__func__.13526, %object
	.size	__func__.13526, 23
__func__.13526:
	.ascii	"ALG_RATIO_RatioProcess\000"
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
.LC0:
	.ascii	"eAspMode Error\012\000"
	.ident	"GCC: (Hisilicon_v200(gcc4.4-290+glibc-2.11+eabi+nptl)) 4.4.1"
	.section	.note.GNU-stack,"",%progbits

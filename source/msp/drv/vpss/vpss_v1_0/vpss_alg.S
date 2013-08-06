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
	.file	"vpss_alg.c"
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
@ -DKBUILD_STR(s)=#s -DKBUILD_BASENAME=KBUILD_STR(vpss_alg)
@ -DKBUILD_MODNAME=KBUILD_STR(hi_vpss) -isystem
@ /opt/hisi-linux/x86-arm/arm-hisiv200-linux/bin/../lib/gcc/arm-hisiv200-linux-gnueabi/4.4.1/include
@ -include
@ /home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/include/linux/kconfig.h
@ -MD drivers/msp/vpss/vpss_v1_0/.vpss_alg.o.d
@ /home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/drivers/msp/vpss/vpss_v1_0/vpss_alg.c
@ -mlittle-endian -marm -mapcs -mno-sched-prolog -mabi=aapcs-linux
@ -mno-thumb-interwork -march=armv7-a -msoft-float -auxbase-strip
@ drivers/msp/vpss/vpss_v1_0/vpss_alg.o -O2 -Wall -Wundef
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
	.global	VPSS_ALG_GetRwzbInfo
	.type	VPSS_ALG_GetRwzbInfo, %function
VPSS_ALG_GetRwzbInfo:
	.fnstart
.LFB1243:
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
	ldr	r0, [r0, #16]	@, <variable>.stDetInfo.isRWZB
	ldmfd	sp, {fp, sp, pc}	@
	.fnend
	.size	VPSS_ALG_GetRwzbInfo, .-VPSS_ALG_GetRwzbInfo
	.align	2
	.global	VPSS_ALG_GetDeiData
	.type	VPSS_ALG_GetDeiData, %function
VPSS_ALG_GetDeiData:
	.fnstart
.LFB1245:
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
	add	r3, r0, #548	@ tmp136, u32InfoAddr,
	mov	r2, #220	@,
	mov	r0, r1	@, pstDeiData
	mov	r1, r3	@, tmp136
	bl	memcpy	@
	mov	r0, #0	@,
	ldmfd	sp, {fp, sp, pc}	@
	.fnend
	.size	VPSS_ALG_GetDeiData, .-VPSS_ALG_GetDeiData
	.align	2
	.global	VPSS_ALG_StoreDeiData
	.type	VPSS_ALG_StoreDeiData, %function
VPSS_ALG_StoreDeiData:
	.fnstart
.LFB1244:
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
	mov	r2, #220	@,
	add	r0, r0, #548	@, u32InfoAddr,
	bl	memcpy	@
	mov	r0, #0	@,
	ldmfd	sp, {fp, sp, pc}	@
	.fnend
	.size	VPSS_ALG_StoreDeiData, .-VPSS_ALG_StoreDeiData
	.align	2
	.global	VPSS_ALG_SetFrameInfo
	.type	VPSS_ALG_SetFrameInfo, %function
VPSS_ALG_SetFrameInfo:
	.fnstart
.LFB1238:
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
	ldr	r2, [r1, #76]	@ <variable>.u32Width, <variable>.u32Width
	mov	ip, r1	@ pstFrm, pstFrm
	add	r5, r1, #4	@ D.27410, pstFrm,
	mov	lr, r0	@ pstFrmCfg, pstFrmCfg
	add	r6, r0, #4	@ D.27409, pstFrmCfg,
	str	r2, [r0, #76]	@ <variable>.u32Width, <variable>.u32Width
	ldr	r1, [r1, #80]	@ <variable>.u32Height, <variable>.u32Height
	str	r1, [r0, #80]	@ <variable>.u32Height, <variable>.u32Height
	ldr	r0, [ip, #104]	@ <variable>.ePixFormat, <variable>.ePixFormat
	str	r0, [lr, #92]	@ <variable>.ePixFormat, <variable>.ePixFormat
	ldr	r4, [ip, #112]	@ <variable>.enFieldMode, <variable>.enFieldMode
	str	r4, [lr, #100]	@ <variable>.enFieldMode, <variable>.enFieldMode
	mov	r4, r6	@ D.27409, D.27409
	ldr	r3, [ip, #108]	@ <variable>.bProgressive, <variable>.bProgressive
	str	r3, [lr, #96]	@ <variable>.bProgressive, <variable>.bProgressive
	ldr	r2, [ip, #116]	@ <variable>.bTopFieldFirst, <variable>.bTopFieldFirst
	str	r2, [lr, #104]	@ <variable>.bTopFieldFirst, <variable>.bTopFieldFirst
	ldr	r1, [ip, #96]	@ <variable>.u32AspectHeight, <variable>.u32AspectHeight
	strb	r1, [lr, #109]	@ <variable>.u32AspectHeight, <variable>.stDispPixAR.u8ARh
	ldr	r0, [ip, #92]	@ <variable>.u32AspectWidth, <variable>.u32AspectWidth
	strb	r0, [lr, #108]	@ <variable>.u32AspectWidth, <variable>.stDispPixAR.u8ARw
	ldmia	r5!, {r0, r1, r2, r3}	@ D.27410,,,,
	stmia	r4!, {r0, r1, r2, r3}	@ D.27409,,,,
	ldmia	r5!, {r0, r1, r2, r3}	@ D.27410,,,,
	stmia	r4!, {r0, r1, r2, r3}	@ D.27409,,,,
	mov	r3, r4	@ D.27409, D.27409
	ldr	r4, [r5, #0]	@ tmp150,* D.27410
	add	r0, lr, #40	@, pstFrmCfg,
	add	r1, ip, #40	@, pstFrm,
	mov	r2, #36	@,
	str	r4, [r3, #0]	@ tmp150,* D.27409
	bl	memcpy	@
	mov	r0, #0	@,
	ldmfd	sp, {r3, r4, r5, r6, fp, sp, pc}	@
	.fnend
	.size	VPSS_ALG_SetFrameInfo, .-VPSS_ALG_SetFrameInfo
	.align	2
	.global	VPSS_ALG_GetAspCfg
	.type	VPSS_ALG_GetAspCfg, %function
VPSS_ALG_GetAspCfg:
	.fnstart
.LFB1242:
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
	mov	r5, r0	@ pstAspDrvPara, pstAspDrvPara
	mov	r1, r3	@, pstAspCfg
	mov	r4, r3	@ pstAspCfg, pstAspCfg
	mov	r6, r2	@ pstScreen, pstScreen
	bl	ALG_RATIO_RatioProcess	@
	ldr	r2, [r5, #60]	@ <variable>.stOutWnd.s32Height, <variable>.stOutWnd.s32Height
	mov	r3, #0	@ tmp142,
	ldr	r7, [r5, #56]	@ <variable>.stOutWnd.s32Width, <variable>.stOutWnd.s32Width
	mov	r0, r3	@, tmp142
	ldr	r1, [r5, #52]	@ <variable>.stOutWnd.s32Y, <variable>.stOutWnd.s32Y
	ldr	ip, [r5, #48]	@ <variable>.stOutWnd.s32X, <variable>.stOutWnd.s32X
	str	r2, [r4, #12]	@ <variable>.stOutWnd.s32Height, <variable>.stOutWnd.s32Height
	str	r7, [r4, #8]	@ <variable>.stOutWnd.s32Width, <variable>.stOutWnd.s32Width
	ldr	r2, [r6, #12]	@ <variable>.s32Height, <variable>.s32Height
	ldr	r7, [r6, #8]	@ <variable>.s32Width, <variable>.s32Width
	str	ip, [r4, #0]	@ <variable>.stOutWnd.s32X, <variable>.stOutWnd.s32X
	str	r1, [r4, #4]	@ <variable>.stOutWnd.s32Y, <variable>.stOutWnd.s32Y
	str	r7, [r4, #24]	@ <variable>.s32Width, <variable>.stOutScreen.s32Width
	str	r2, [r4, #28]	@ <variable>.s32Height, <variable>.stOutScreen.s32Height
	str	r3, [r4, #16]	@ tmp142, <variable>.stOutScreen.s32X
	str	r3, [r4, #20]	@ tmp142, <variable>.stOutScreen.s32Y
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}	@
	.fnend
	.size	VPSS_ALG_GetAspCfg, .-VPSS_ALG_GetAspCfg
	.align	2
	.global	VPSS_ALG_GetZmeCfg
	.type	VPSS_ALG_GetZmeCfg, %function
VPSS_ALG_GetZmeCfg:
	.fnstart
.LFB1241:
	@ args = 0, pretend = 0, frame = 40
	@ frame_needed = 1, uses_anonymous_args = 0
	.movsp ip
	mov	ip, sp	@,
	stmfd	sp!, {r4, r5, r6, r7, r8, sl, fp, ip, lr, pc}	@,
	.pad #4
	.save {r4, r5, r6, r7, r8, sl, fp, ip, lr}
	.setfp fp, ip, #-4
	sub	fp, ip, #4	@,,
	.pad #40
	sub	sp, sp, #40	@,,
	push	{lr}
	bl	__gnu_mcount_nc
	sub	r7, fp, #76	@ tmp139,,
	mov	r6, r0	@ pstSrcImg, pstSrcImg
	mov	r5, r1	@ pstDstFrm, pstDstFrm
	mov	r0, r7	@, tmp139
	mov	r1, #40	@,
	mov	r4, r2	@ pstZmeCfg, pstZmeCfg
	mov	r8, r3	@ pstAlgCtrl, pstAlgCtrl
	bl	__memzero	@
	ldr	ip, [r6, #92]	@ D.27466, <variable>.ePixFormat
	ldr	r3, [r5, #92]	@ <variable>.ePixFormat, <variable>.ePixFormat
	mov	r0, r8	@, pstAlgCtrl
	ldr	sl, [r5, #84]	@ <variable>.u32ZmeWidth, <variable>.u32ZmeWidth
	mov	r1, r7	@, tmp139
	ldr	r8, [r6, #76]	@ <variable>.u32Width, <variable>.u32Width
	mov	r7, #0	@ tmp142,
	ldr	r6, [r6, #80]	@ <variable>.u32Height, <variable>.u32Height
	mov	r2, r4	@, pstZmeCfg
	ldr	r5, [r5, #88]	@ <variable>.u32ZmeHeight, <variable>.u32ZmeHeight
	mov	lr, #1	@ tmp140,
	str	ip, [r4, #104]	@ D.27466, <variable>.eZmeYCFmtIn
	str	r3, [r4, #108]	@ <variable>.ePixFormat, <variable>.eZmeYCFmtOut
	str	lr, [fp, #-48]	@ tmp140, stZmeDrvPara.bZmeFrmFmtOut
	str	r6, [fp, #-72]	@ <variable>.u32Height, stZmeDrvPara.u32ZmeFrmHIn
	str	r8, [fp, #-76]	@ <variable>.u32Width, stZmeDrvPara.u32ZmeFrmWIn
	str	r5, [fp, #-64]	@ <variable>.u32ZmeHeight, stZmeDrvPara.u32ZmeFrmHOut
	str	sl, [fp, #-68]	@ <variable>.u32ZmeWidth, stZmeDrvPara.u32ZmeFrmWOut
	str	lr, [fp, #-52]	@ tmp140, stZmeDrvPara.bZmeFrmFmtIn
	str	r7, [fp, #-44]	@ tmp142, stZmeDrvPara.bZmeBFIn
	str	r7, [fp, #-40]	@ tmp142, stZmeDrvPara.bZmeBFOut
	str	ip, [fp, #-60]	@ D.27466, stZmeDrvPara.eZmeYCFmtIn
	str	r3, [fp, #-56]	@ <variable>.ePixFormat, stZmeDrvPara.eZmeYCFmtOut
	bl	ALG_VZmeVpssHQSet	@
	mov	r0, r7	@, tmp142
	sub	sp, fp, #36
	ldmfd	sp, {r4, r5, r6, r7, r8, sl, fp, sp, pc}
	.fnend
	.size	VPSS_ALG_GetZmeCfg, .-VPSS_ALG_GetZmeCfg
	.align	2
	.global	VPSS_ALG_SetImageInfo
	.type	VPSS_ALG_SetImageInfo, %function
VPSS_ALG_SetImageInfo:
	.fnstart
.LFB1237:
	@ args = 16, pretend = 8, frame = 56
	@ frame_needed = 1, uses_anonymous_args = 0
	.movsp ip
	mov	ip, sp	@,
	.pad #8
	sub	sp, sp, #8	@,,
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, ip, lr, pc}	@,
	.pad #4
	.save {r4, r5, r6, r7, r8, r9, sl, fp, ip, lr}
	.setfp fp, ip, #-12
	sub	fp, ip, #12	@,,
	.pad #68
	sub	sp, sp, #68	@,,
	push	{lr}
	bl	__gnu_mcount_nc
	add	ip, fp, #4	@ tmp328,,
	cmp	r2, #0	@ stInRect$s32X,
	mov	r5, r0	@ pstImgCfg, pstImgCfg
	mov	r4, r1	@ pstImg, pstImg
	stmia	ip, {r2, r3}	@ tmp328,,
	ldr	r1, [fp, #16]	@ stInRect$s32Height, stInRect.s32Height
	ldr	r0, [fp, #8]	@ stInRect$s32Y, stInRect.s32Y
	ldr	lr, [fp, #12]	@ stInRect$s32Width, stInRect.s32Width
	ldr	r9, [r4, #136]	@ eFrmType, <variable>.eFrmType
	ldr	r6, [r5, #0]	@ eLReye, <variable>.eLReye
	bne	.L14	@,
	cmp	r0, #0	@ stInRect$s32Y,
	bne	.L14	@,
	cmp	r1, #0	@ stInRect$s32Height,
	bne	.L14	@,
	cmp	lr, #0	@ stInRect$s32Width,
	ldreq	ip, [r4, #76]	@ u32InWidth, <variable>.u32Width
	ldreq	r3, [r4, #80]	@ u32InHeight, <variable>.u32Height
	beq	.L15	@
.L14:
	ldr	ip, [r4, #76]	@ u32InWidth, <variable>.u32Width
	add	r3, lr, r2	@ tmp159, stInRect$s32Width, stInRect$s32X
	cmp	r3, ip	@ tmp159, u32InWidth
	ldrhi	r3, [r4, #80]	@ u32InHeight, <variable>.u32Height
	bhi	.L15	@,
	ldr	r3, [r4, #80]	@ u32InHeight, <variable>.u32Height
	add	r7, r1, r0	@ tmp161, stInRect$s32Height, stInRect$s32Y
	cmp	r7, r3	@ tmp161, u32InHeight
	bls	.L37	@,
.L15:
	mov	r7, r6, asl #3	@ tmp332, eLReye,
	sub	sl, fp, #80	@ tmp329,,
	add	r1, r7, r6	@ tmp180, tmp332, eLReye
	cmp	ip, #1920	@ u32InWidth,
	cmpeq	r3, #1088	@, u32InHeight,
	str	ip, [fp, #-88]	@ u32InWidth, %sfp
	add	ip, r4, #4	@ tmp177, pstImg,
	movw	lr, #1080	@ tmp333,
	add	r1, ip, r1, asl #2	@,, tmp177, tmp180,
	mov	r0, sl	@, tmp329
	mov	r2, #36	@,
	movne	lr, r3	@,, u32InHeight
	str	lr, [fp, #-100]	@, %sfp
	bl	memcpy	@
.L23:
	cmp	r9, #3	@ eFrmType,
	ldrls	pc, [pc, r9, asl #2]	@ eFrmType
	b	.L26	@
.L30:
	.word	.L27
	.word	.L28
	.word	.L29
	.word	.L27
.L29:
	ldr	lr, [fp, #-88]	@, %sfp
	add	r6, r7, r6	@ tmp309, tmp332, eLReye
	add	r2, r5, #4	@ tmp306, pstImgCfg,
	add	r0, r2, r6, asl #2	@,, tmp306, tmp309,
	mov	r2, #36	@,
	str	lr, [r5, #76]	@, <variable>.u32Width
	ldr	r1, [fp, #-100]	@, %sfp
	mov	r3, r1, lsr #1	@ tmp305,,
	mov	r1, sl	@, tmp329
	str	r3, [r5, #80]	@ tmp305, <variable>.u32Height
	bl	memcpy	@
.L31:
	ldr	r3, [r4, #104]	@ <variable>.ePixFormat, <variable>.ePixFormat
	mov	r0, #0	@,
	str	r3, [r5, #92]	@ <variable>.ePixFormat, <variable>.ePixFormat
	ldr	ip, [r4, #112]	@ <variable>.enFieldMode, <variable>.enFieldMode
	str	ip, [r5, #100]	@ <variable>.enFieldMode, <variable>.enFieldMode
	ldr	lr, [r4, #108]	@ <variable>.bProgressive, <variable>.bProgressive
	str	lr, [r5, #96]	@ <variable>.bProgressive, <variable>.bProgressive
	ldr	r1, [r4, #116]	@ <variable>.bTopFieldFirst, <variable>.bTopFieldFirst
	str	r1, [r5, #104]	@ <variable>.bTopFieldFirst, <variable>.bTopFieldFirst
	ldr	r2, [r4, #96]	@ <variable>.u32AspectHeight, <variable>.u32AspectHeight
	strb	r2, [r5, #109]	@ <variable>.u32AspectHeight, <variable>.stDispPixAR.u8ARh
	ldr	r3, [r4, #92]	@ <variable>.u32AspectWidth, <variable>.u32AspectWidth
	strb	r3, [r5, #108]	@ <variable>.u32AspectWidth, <variable>.stDispPixAR.u8ARw
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, sl, fp, sp, pc}
.L28:
	ldr	ip, [fp, #-100]	@, %sfp
	add	r6, r7, r6	@ tmp297, tmp332, eLReye
	add	lr, r5, #4	@ tmp294, pstImgCfg,
	mov	r1, sl	@, tmp329
	mov	r2, #36	@,
	str	ip, [r5, #80]	@, <variable>.u32Height
	ldr	r0, [fp, #-88]	@, %sfp
	mov	r3, r0, lsr #1	@ tmp293,,
	add	r0, lr, r6, asl #2	@,, tmp294, tmp297,
	str	r3, [r5, #76]	@ tmp293, <variable>.u32Width
	bl	memcpy	@
	b	.L31	@
.L27:
	ldr	r1, [fp, #-88]	@, %sfp
	add	r6, r7, r6	@ tmp285, tmp332, eLReye
	add	ip, r5, #4	@ tmp282, pstImgCfg,
	add	r0, ip, r6, asl #2	@,, tmp282, tmp285,
	str	r1, [r5, #76]	@, <variable>.u32Width
	mov	r1, sl	@, tmp329
	ldr	r2, [fp, #-100]	@, %sfp
	str	r2, [r5, #80]	@, <variable>.u32Height
	mov	r2, #36	@,
	bl	memcpy	@
	b	.L31	@
.L26:
	movw	r2, #:lower16:.LANCHOR0	@,
	mov	r0, #2	@,
	movt	r2, #:upper16:.LANCHOR0	@,
	mov	r1, #37	@,
	mov	r3, #174	@,
	str	r9, [sp, #4]	@ eFrmType,
	movw	ip, #:lower16:.LC1	@ tmp318,
	movt	ip, #:upper16:.LC1	@ tmp318,
	str	ip, [sp, #0]	@ tmp318,
	bl	HI_LogOut	@
	b	.L31	@
.L37:
	cmp	r2, #0	@ stInRect$s32X,
	blt	.L15	@,
	cmp	r0, #0	@ stInRect$s32Y,
	blt	.L15	@,
	cmp	lr, #0	@ stInRect$s32Width,
	bne	.L16	@,
	cmp	r1, #0	@ stInRect$s32Height,
	moveq	lr, r1	@ stInRect$s32Width, stInRect$s32Height
	bne	.L15	@,
.L17:
	cmp	r2, #0	@ stInRect$s32X,
	beq	.L38	@,
	adds	r3, r1, #7	@ pretmp.599, stInRect$s32Height,
	str	r2, [fp, #-92]	@ stInRect$s32X, %sfp
	addmi	r2, r1, #14	@, pretmp.599, stInRect$s32Height,
	movpl	r2, r3	@, pretmp.599, pretmp.599
	str	r0, [fp, #-96]	@ stInRect$s32Y, %sfp
	bic	r7, r2, #7	@, pretmp.599,
	str	r7, [fp, #-100]	@, %sfp
.L20:
	mov	r7, r6, asl #3	@ tmp332, eLReye,
	sub	sl, fp, #80	@ tmp329,,
	add	r8, r7, r6	@ tmp194, tmp332, eLReye
	add	r1, r4, #4	@ tmp191, pstImg,
	mov	r0, sl	@, tmp329
	mov	r2, #36	@,
	mov	r8, r8, asl #2	@ tmp195, tmp194,
	str	lr, [fp, #-88]	@ stInRect$s32Width, %sfp
	add	r1, r1, r8	@, tmp191, tmp195
	bl	memcpy	@
	ldr	ip, [r4, #104]	@ D.27358, <variable>.ePixFormat
	sub	r0, ip, #62	@ tmp201, D.27358,
	cmp	r0, #1	@ tmp201,
	bls	.L39	@,
	sub	r3, ip, #65	@ tmp241, D.27358,
	cmp	r3, #1	@ tmp241,
	bls	.L40	@,
	movw	r2, #:lower16:.LANCHOR0	@,
	mov	r0, #2	@,
	movt	r2, #:upper16:.LANCHOR0	@,
	mov	r1, #37	@,
	mov	r3, #143	@,
	str	ip, [sp, #4]	@ D.27358,
	movw	lr, #:lower16:.LC0	@ tmp281,
	movt	lr, #:upper16:.LC0	@ tmp281,
	str	lr, [sp, #0]	@ tmp281,
	bl	HI_LogOut	@
	b	.L23	@
.L40:
	add	r8, r8, r4	@ tmp247, tmp195, pstImg
	ldr	r0, [fp, #-92]	@, %sfp
	ldr	ip, [fp, #-92]	@, %sfp
	ldr	r3, [r8, #8]	@ <variable>.u32PhyAddr_Y, <variable>.u32PhyAddr_Y
	ldr	r1, [r8, #20]	@ <variable>.u32PhyAddr_C, <variable>.u32PhyAddr_C
	add	lr, r0, r3	@ tmp249,, <variable>.u32PhyAddr_Y
	ldr	r2, [r8, #24]	@ <variable>.u32Stride_C, <variable>.u32Stride_C
	add	r1, ip, r1	@ tmp268,, <variable>.u32PhyAddr_C
	ldr	r0, [r8, #12]	@ <variable>.u32Stride_Y, <variable>.u32Stride_Y
	ldr	ip, [fp, #-96]	@, %sfp
	mla	r2, r2, ip, r1	@ tmp279, <variable>.u32Stride_C,, tmp268
	mla	r1, r0, ip, lr	@ tmp260, <variable>.u32Stride_Y,, tmp249
	str	r2, [fp, #-64]	@ tmp279, stCfgAddr.u32PhyAddr_C
	str	r1, [fp, #-76]	@ tmp260, stCfgAddr.u32PhyAddr_Y
	b	.L23	@
.L39:
	add	r8, r8, r4	@ tmp207, tmp195, pstImg
	ldr	lr, [fp, #-92]	@, %sfp
	ldr	r1, [fp, #-96]	@, %sfp
	ldr	r0, [r8, #8]	@ <variable>.u32PhyAddr_Y, <variable>.u32PhyAddr_Y
	ldr	r2, [r8, #24]	@ <variable>.u32Stride_C, <variable>.u32Stride_C
	add	r0, lr, r0	@ tmp209,, <variable>.u32PhyAddr_Y
	ldr	ip, [fp, #-92]	@, %sfp
	ldr	lr, [r8, #20]	@ <variable>.u32PhyAddr_C, <variable>.u32PhyAddr_C
	mul	r2, r2, r1	@ tmp237, <variable>.u32Stride_C,
	ldr	r3, [r8, #12]	@ <variable>.u32Stride_Y, <variable>.u32Stride_Y
	add	r1, ip, lr	@ tmp228,, <variable>.u32PhyAddr_C
	ldr	ip, [fp, #-96]	@, %sfp
	mla	lr, r3, ip, r0	@ tmp220, <variable>.u32Stride_Y,, tmp209
	add	ip, r1, r2, lsr #1	@, tmp240, tmp228, tmp237,
	str	ip, [fp, #-64]	@ tmp240, stCfgAddr.u32PhyAddr_C
	str	lr, [fp, #-76]	@ tmp220, stCfgAddr.u32PhyAddr_Y
	b	.L23	@
.L16:
	cmp	r1, #0	@ stInRect$s32Height,
	bne	.L17	@,
	b	.L15	@
.L38:
	cmp	r0, #0	@ stInRect$s32Y,
	beq	.L19	@,
	adds	r3, r1, #7	@ pretmp.571, stInRect$s32Height,
	str	r0, [fp, #-96]	@ stInRect$s32Y, %sfp
	addmi	r0, r1, #14	@, pretmp.571, stInRect$s32Height,
	movpl	r0, r3	@, pretmp.571, pretmp.571
	str	r2, [fp, #-92]	@ stInRect$s32X, %sfp
	bic	r1, r0, #7	@, pretmp.571,
	str	r1, [fp, #-100]	@, %sfp
	b	.L20	@
.L19:
	cmp	r1, #0	@ stInRect$s32Height,
	beq	.L21	@,
	adds	sl, r1, #7	@ pretmp.608, stInRect$s32Height,
	str	r0, [fp, #-92]	@ stInRect$s32Y, %sfp
	addmi	r8, r1, #14	@, pretmp.608, stInRect$s32Height,
	movpl	r8, sl	@, pretmp.608, pretmp.608
	str	r0, [fp, #-96]	@ stInRect$s32Y, %sfp
	bic	r2, r8, #7	@, pretmp.608,
	str	r2, [fp, #-100]	@, %sfp
	b	.L20	@
.L21:
	cmp	lr, #0	@ stInRect$s32Width,
	strne	r1, [fp, #-100]	@ stInRect$s32Height, %sfp
	strne	r1, [fp, #-92]	@ stInRect$s32Height, %sfp
	strne	r1, [fp, #-96]	@ stInRect$s32Height, %sfp
	bne	.L20	@,
	b	.L15	@
	.fnend
	.size	VPSS_ALG_SetImageInfo, .-VPSS_ALG_SetImageInfo
	.align	2
	.global	VPSS_ALG_GetDeiCfg
	.type	VPSS_ALG_GetDeiCfg, %function
VPSS_ALG_GetDeiCfg:
	.fnstart
.LFB1240:
	@ args = 0, pretend = 0, frame = 0
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
	subs	ip, r0, #0	@ eDeiMode, eDeiMode
	mov	r6, r1	@ u32AuInfoAddr, u32AuInfoAddr
	mov	r4, r2	@ pstDeiCfg, pstDeiCfg
	mov	r5, r3	@ pstAlgCtrl, pstAlgCtrl
	streq	ip, [r2, #0]	@ eDeiMode, <variable>.bDei
	beq	.L48	@,
	cmp	ip, #4	@ eDeiMode,
	mov	r3, #1	@ tmp139,
	mov	r0, #0	@ tmp140,
	str	r3, [r2, #0]	@ tmp139, <variable>.bDei
	str	r3, [r2, #4]	@ tmp139, <variable>.stDeiPara.bDeiEnLum
	str	r0, [r2, #52]	@ tmp140, <variable>.stDeiPara.s32Repeat
	str	r3, [r2, #8]	@ tmp139, <variable>.stDeiPara.bDeiEnChr
	streq	r3, [r2, #16]	@ tmp139, <variable>.stDeiPara.s32DeiMdChr
	streq	r3, [r2, #12]	@ tmp139, <variable>.stDeiPara.s32DeiMdLum
	beq	.L47	@,
	cmp	ip, #5	@ eDeiMode,
	streq	r0, [r2, #16]	@ tmp140, <variable>.stDeiPara.s32DeiMdChr
	streq	r0, [r2, #12]	@ tmp140, <variable>.stDeiPara.s32DeiMdLum
	beq	.L47	@,
	cmp	ip, #3	@ eDeiMode,
	moveq	r3, #2	@ tmp143,
	streq	r3, [r2, #16]	@ tmp143, <variable>.stDeiPara.s32DeiMdChr
	streq	r3, [r2, #12]	@ tmp143, <variable>.stDeiPara.s32DeiMdLum
	beq	.L47	@,
	mov	r1, #37	@,
	ldr	r2, .L51	@,
	movw	r3, #275	@,
	movw	ip, #:lower16:.LC2	@ tmp151,
	movt	ip, #:upper16:.LC2	@ tmp151,
	str	ip, [sp, #0]	@ tmp151,
	bl	HI_LogOut	@
.L47:
	add	r5, r5, #208	@ D.27447, pstAlgCtrl,
	add	r0, r6, #20	@, u32AuInfoAddr,
	add	r1, r4, #4	@, pstDeiCfg,
	add	r3, r4, #520	@, pstDeiCfg,
	mov	r2, r5	@, D.27447
	bl	ALG_DeiSet	@
	str	r5, [r4, #548]	@ D.27447, <variable>.pstDeiDefaultPara
.L48:
	mov	r0, #0	@,
	sub	sp, fp, #24
	ldmfd	sp, {r4, r5, r6, fp, sp, pc}
.L52:
	.align	2
.L51:
	.word	.LANCHOR0+24
	.fnend
	.size	VPSS_ALG_GetDeiCfg, .-VPSS_ALG_GetDeiCfg
	.align	2
	.global	VPSS_ALG_GetRwzbCfg
	.type	VPSS_ALG_GetRwzbCfg, %function
VPSS_ALG_GetRwzbCfg:
	.fnstart
.LFB1239:
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
	ldr	ip, [r2, #100]	@ <variable>.enFieldMode, <variable>.enFieldMode
	mov	r3, r1	@ pstRwzbCfg, pstRwzbCfg
	cmp	ip, #0	@ <variable>.enFieldMode,
	beq	.L54	@,
	ldr	r1, [r2, #96]	@ <variable>.bProgressive, <variable>.bProgressive
	cmp	r1, #1	@ <variable>.bProgressive,
	movne	r2, #0	@ tmp143,
	strne	r2, [r3, #12]	@ tmp143, <variable>.u32Mode
	strne	r2, [r3, #8]	@ tmp143, <variable>.u32EnRwzb
	beq	.L54	@,
	mov	r0, #0	@,
	ldmfd	sp, {fp, sp, pc}	@
.L54:
	mov	ip, #0	@ tmp140,
	mov	r1, #1	@ tmp139,
	str	ip, [r3, #12]	@ tmp140, <variable>.u32Mode
	str	r1, [r3, #8]	@ tmp139, <variable>.u32EnRwzb
	mov	r1, r3	@, pstRwzbCfg
	ldr	lr, [r2, #76]	@ <variable>.u32Width, <variable>.u32Width
	str	lr, [r3, #4]	@ <variable>.u32Width, <variable>.u32Width
	ldr	r2, [r2, #80]	@ <variable>.u32Height, <variable>.u32Height
	str	r2, [r3, #0]	@ <variable>.u32Height, <variable>.u32Height
	bl	ALG_DetPic	@
	mov	r0, #0	@,
	ldmfd	sp, {fp, sp, pc}	@
	.fnend
	.size	VPSS_ALG_GetRwzbCfg, .-VPSS_ALG_GetRwzbCfg
	.align	2
	.global	VPSS_ALG_DeInitAuInfo
	.type	VPSS_ALG_DeInitAuInfo, %function
VPSS_ALG_DeInitAuInfo:
	.fnstart
.LFB1236:
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
	mov	r4, r0	@ u32InfoAddr, u32InfoAddr
	bl	ALG_DeInitRwzbInfo	@
	add	r0, r4, #20	@, u32InfoAddr,
	bl	ALG_DeiInfoDeInit	@
	mov	r0, #0	@,
	ldmfd	sp, {r3, r4, fp, sp, pc}	@
	.fnend
	.size	VPSS_ALG_DeInitAuInfo, .-VPSS_ALG_DeInitAuInfo
	.align	2
	.global	VPSS_ALG_InitAuInfo
	.type	VPSS_ALG_InitAuInfo, %function
VPSS_ALG_InitAuInfo:
	.fnstart
.LFB1235:
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
	mov	r4, r0	@ u32InfoAddr, u32InfoAddr
	bl	ALG_InitRwzbInfo	@
	add	r0, r4, #20	@, u32InfoAddr,
	bl	ALG_DeiInfoInit	@
	mov	r0, #0	@,
	ldmfd	sp, {r3, r4, fp, sp, pc}	@
	.fnend
	.size	VPSS_ALG_InitAuInfo, .-VPSS_ALG_InitAuInfo
	.align	2
	.global	VPSS_ALG_DelInit
	.type	VPSS_ALG_DelInit, %function
VPSS_ALG_DelInit:
	.fnstart
.LFB1234:
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
	bl	ALG_VZmeVpssComnDeInit	@
	bl	ALG_DeiDeInit	@
	mov	r0, #0	@,
	ldmfd	sp, {fp, sp, pc}	@
	.fnend
	.size	VPSS_ALG_DelInit, .-VPSS_ALG_DelInit
	.align	2
	.global	VPSS_ALG_Init
	.type	VPSS_ALG_Init, %function
VPSS_ALG_Init:
	.fnstart
.LFB1233:
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
	mov	r4, r0	@ pstAlgCtrl, pstAlgCtrl
	bl	ALG_VZmeVpssComnInit	@
	add	r0, r4, #208	@, pstAlgCtrl,
	bl	ALG_DeiInit	@
	mov	r0, #0	@,
	ldmfd	sp, {r3, r4, fp, sp, pc}	@
	.fnend
	.size	VPSS_ALG_Init, .-VPSS_ALG_Init
	.section	.rodata
	.align	2
.LANCHOR0 = . + 0
	.type	__func__.27309, %object
	.size	__func__.27309, 22
__func__.27309:
	.ascii	"VPSS_ALG_SetImageInfo\000"
	.space	2
	.type	__func__.27444, %object
	.size	__func__.27444, 19
__func__.27444:
	.ascii	"VPSS_ALG_GetDeiCfg\000"
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
.LC0:
	.ascii	"WrongInFormat %d \012\000"
	.space	1
.LC1:
	.ascii	"WrongInType  %d \012\000"
	.space	2
.LC2:
	.ascii	"\012Dei Mode Error\012\000"
	.ident	"GCC: (Hisilicon_v200(gcc4.4-290+glibc-2.11+eabi+nptl)) 4.4.1"
	.section	.note.GNU-stack,"",%progbits

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
	.file	"linear.c"
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
@ -DKBUILD_STR(s)=#s -DKBUILD_BASENAME=KBUILD_STR(linear)
@ -DKBUILD_MODNAME=KBUILD_STR(hi_adsp) -isystem
@ /opt/hisi-linux/x86-arm/arm-hisiv200-linux/bin/../lib/gcc/arm-hisiv200-linux-gnueabi/4.4.1/include
@ -include
@ /home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/include/linux/kconfig.h
@ -MD drivers/msp/adsp/adsp_v1_1/./hifi/common/linear/.linear.o.d
@ /home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/drivers/msp/adsp/adsp_v1_1/./hifi/common/linear/linear.c
@ -mlittle-endian -marm -mapcs -mno-sched-prolog -mabi=aapcs-linux
@ -mno-thumb-interwork -march=armv7-a -msoft-float -auxbase-strip
@ drivers/msp/adsp/adsp_v1_1/./hifi/common/linear/linear.o -O2 -Wall
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

	.global	__aeabi_idiv
	.text
	.align	2
	.global	LSRC_GetMaxOutputNum
	.type	LSRC_GetMaxOutputNum, %function
LSRC_GetMaxOutputNum:
	.fnstart
.LFB144:
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
	subs	r3, r0, #0	@ hLSRC, hLSRC
	mov	r2, r1	@ insamps, insamps
	mvneq	r0, #0	@ outsamps,
	ldmeqfd	sp, {fp, sp, pc}	@
	ldr	r0, [r3, #16]	@ <variable>.OutRate, <variable>.OutRate
	ldr	r1, [r3, #8]	@, <variable>.AdjustInRate
	mul	r0, r0, r2	@, <variable>.OutRate, insamps
	bl	__aeabi_idiv	@
	add	r0, r0, #1	@ outsamps,,
	ldmfd	sp, {fp, sp, pc}	@
	.fnend
	.size	LSRC_GetMaxOutputNum, .-LSRC_GetMaxOutputNum
	.global	__aeabi_idivmod
	.align	2
	.global	LSRC_GetMinInputNum
	.type	LSRC_GetMinInputNum, %function
LSRC_GetMinInputNum:
	.fnstart
.LFB145:
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
	cmp	r0, #0	@ hLSRC
	mvneq	r4, #0	@ insamps,
	beq	.L8	@,
	ldr	r6, [r0, #8]	@ <variable>.AdjustInRate, <variable>.AdjustInRate
	ldr	r5, [r0, #16]	@ D.3774, <variable>.OutRate
	mul	r6, r6, r1	@ D.3773, <variable>.AdjustInRate, outsamps
	mov	r1, r5	@, D.3774
	mov	r0, r6	@, D.3773
	bl	__aeabi_idiv	@
	mov	r1, r5	@, D.3774
	mov	r4, r0	@ insamps,
	mov	r0, r6	@, D.3773
	bl	__aeabi_idivmod	@
	cmp	r1, #0	@,
	addne	r4, r4, #1	@ insamps, insamps,
.L8:
	mov	r0, r4	@, insamps
	ldmfd	sp, {r3, r4, r5, r6, fp, sp, pc}	@
	.fnend
	.size	LSRC_GetMinInputNum, .-LSRC_GetMinInputNum
	.align	2
	.global	LSRC_CheckUpdate
	.type	LSRC_CheckUpdate, %function
LSRC_CheckUpdate:
	.fnstart
.LFB146:
	@ args = 4, pretend = 0, frame = 0
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
	ldr	r4, [r0, #12]	@ <variable>.InRate, <variable>.InRate
	ldr	ip, [r0, #16]	@ <variable>.OutRate, <variable>.OutRate
	ldr	r5, [r0, #0]	@ <variable>.BitPerSample, <variable>.BitPerSample
	subs	r4, r4, r1	@ tmp147, <variable>.InRate, inRate
	movne	r4, #1	@ tmp147
	cmp	ip, r2	@ <variable>.OutRate, outRate
	addne	r4, r4, #1	@,, change, tmp147
	ldr	r2, [r0, #4]	@ <variable>.Channels, <variable>.Channels
	cmp	r5, r3	@ <variable>.BitPerSample, BitPerSample
	addne	r4, r4, #1	@,, change.159, change
	ldr	r5, [fp, #4]	@ Channels, Channels
	cmp	r2, r5	@ <variable>.Channels, Channels
	moveq	r1, r4	@, tmp155, change.159
	addne	r1, r4, #1	@,, tmp155, change.159
	subs	r0, r1, #0	@, tmp155,
	movne	r0, #1	@
	ldmfd	sp, {r4, r5, fp, sp, pc}	@
	.fnend
	.size	LSRC_CheckUpdate, .-LSRC_CheckUpdate
	.align	2
	.global	LSRC_UpdateChange
	.type	LSRC_UpdateChange, %function
LSRC_UpdateChange:
	.fnstart
.LFB147:
	@ args = 4, pretend = 0, frame = 0
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
	ldr	r5, [r0, #12]	@ <variable>.InRate, <variable>.InRate
	ldr	r4, [r0, #16]	@ <variable>.OutRate, <variable>.OutRate
	subs	ip, r5, r1	@ tmp179, <variable>.InRate, inRate
	movne	ip, #1	@ tmp179
	ldr	r6, [r0, #0]	@ <variable>.BitPerSample, <variable>.BitPerSample
	cmp	r4, r2	@ <variable>.OutRate, outRate
	moveq	r4, ip	@, change, tmp179
	addne	r4, ip, #1	@,, change, tmp179
	ldr	r5, [r0, #4]	@ <variable>.Channels, <variable>.Channels
	ldr	ip, [fp, #4]	@ Channels, Channels
	cmp	r6, r3	@ <variable>.BitPerSample, BitPerSample
	addne	r4, r4, #1	@,, change.192, change
	cmp	r5, ip	@ <variable>.Channels, Channels
	addne	r4, r4, #1	@,, tmp186, change.192
	cmp	r4, #0	@ tmp186,
	ldmeqfd	sp, {r3, r4, r5, r6, fp, sp, pc}	@
	cmp	r2, #8000	@ outRate,
	str	r3, [r0, #0]	@ BitPerSample, <variable>.BitPerSample
	str	r1, [r0, #12]	@ inRate, <variable>.InRate
	mov	r3, #0	@ tmp187,
	str	ip, [r0, #4]	@ Channels, <variable>.Channels
	str	r1, [r0, #8]	@ inRate, <variable>.AdjustInRate
	str	r2, [r0, #16]	@ outRate, <variable>.OutRate
	str	r3, [r0, #20]	@ tmp187, <variable>.Remainder
	str	r3, [r0, #24]	@ tmp187, <variable>.DisContinuity
	str	r3, [r0, #28]	@ tmp187, <variable>.SpeedAdjust
	beq	.L15	@,
	movw	r3, #11025	@ tmp190,
	cmp	r2, r3	@ outRate, tmp190
	moveq	r3, #1	@ n,
	beq	.L15	@,
	movw	r3, #12000	@ tmp191,
	cmp	r2, r3	@ outRate, tmp191
	moveq	r3, #2	@ n,
	beq	.L15	@,
	cmp	r2, #16000	@ outRate,
	moveq	r3, #3	@ n,
	beq	.L15	@,
	movw	r3, #22050	@ tmp192,
	cmp	r2, r3	@ outRate, tmp192
	moveq	r3, #4	@ n,
	beq	.L15	@,
	movw	r3, #24000	@ tmp193,
	cmp	r2, r3	@ outRate, tmp193
	moveq	r3, #5	@ n,
	beq	.L15	@,
	cmp	r2, #32000	@ outRate,
	moveq	r3, #6	@ n,
	beq	.L15	@,
	movw	r3, #44100	@ tmp194,
	cmp	r2, r3	@ outRate, tmp194
	moveq	r3, #7	@ n,
	beq	.L15	@,
	movw	r3, #48000	@ tmp195,
	cmp	r2, r3	@ outRate, tmp195
	moveq	r3, #8	@ n,
	beq	.L15	@,
	movw	r3, #:lower16:88200	@ tmp196,
	movt	r3, #:upper16:88200	@ tmp196,
	cmp	r2, r3	@ outRate, tmp196
	moveq	r3, #9	@ n,
	beq	.L15	@,
	movw	r3, #:lower16:96000	@ tmp197,
	movt	r3, #:upper16:96000	@ tmp197,
	cmp	r2, r3	@ outRate, tmp197
	moveq	r3, #10	@ n,
	beq	.L15	@,
	movw	r3, #:lower16:176400	@ tmp198,
	movt	r3, #:upper16:176400	@ tmp198,
	cmp	r2, r3	@ outRate, tmp198
	moveq	r3, #11	@ n,
	beq	.L15	@,
	movw	r3, #:lower16:192000	@ tmp199,
	movt	r3, #:upper16:192000	@ tmp199,
	cmp	r2, r3	@ outRate, tmp199
	beq	.L36	@,
	cmp	ip, #0	@ Channels,
	ldmeqfd	sp, {r3, r4, r5, r6, fp, sp, pc}	@
.L37:
	mov	r3, #1	@ n.195,
	cmp	r3, ip	@ n.195, Channels
	sub	r4, ip, #1	@ tmp204, Channels,
	add	r2, r0, #4	@ ivtmp.190, hLSRC,
	and	r1, r4, r3	@ tmp206, tmp204,
	mov	r4, #0	@ tmp203,
	str	r4, [r0, #36]	@ tmp203, <variable>.PreSample
	ldmcsfd	sp, {r3, r4, r5, r6, fp, sp, pc}	@
	cmp	r1, #0	@ tmp206,
	beq	.L28	@,
	mov	r3, #2	@ n.195,
	cmp	r3, ip	@ n.195, Channels
	str	r4, [r2, #36]	@ tmp203, <variable>.PreSample
	add	r2, r2, #4	@ ivtmp.190, ivtmp.190,
	ldmcsfd	sp, {r3, r4, r5, r6, fp, sp, pc}	@
.L28:
	add	r3, r3, #2	@ n.195, n.195,
	add	r0, r2, #4	@ tmp208, ivtmp.190,
	cmp	r3, ip	@ n.195, Channels
	str	r4, [r2, #36]	@ tmp203, <variable>.PreSample
	str	r4, [r0, #36]	@ tmp203, <variable>.PreSample
	add	r2, r0, #4	@ ivtmp.190, tmp208,
	bcc	.L28	@,
	ldmfd	sp, {r3, r4, r5, r6, fp, sp, pc}	@
.L36:
	mov	r3, #12	@ n,
.L15:
	movw	r2, #:lower16:.LANCHOR0	@ tmp200,
	movt	r2, #:upper16:.LANCHOR0	@ tmp200,
	cmp	ip, #0	@ Channels,
	ldr	r1, [r2, r3, asl #2]	@ tmp201, g_s32IvtSfTab
	str	r1, [r0, #32]	@ tmp201, <variable>.IvtSFFactor
	bne	.L37	@,
	ldmfd	sp, {r3, r4, r5, r6, fp, sp, pc}	@
	.fnend
	.size	LSRC_UpdateChange, .-LSRC_UpdateChange
	.align	2
	.global	LSRC_Flush
	.type	LSRC_Flush, %function
LSRC_Flush:
	.fnstart
.LFB148:
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
	ldr	r3, [r0, #16]	@ pretmp.220, <variable>.OutRate
	mov	r2, #0	@ tmp141,
	str	r2, [r0, #20]	@ tmp141, <variable>.Remainder
	cmp	r3, #8000	@ pretmp.220,
	str	r2, [r0, #24]	@ tmp141, <variable>.DisContinuity
	beq	.L40	@,
	movw	r2, #11025	@ tmp143,
	cmp	r3, r2	@ pretmp.220, tmp143
	moveq	r2, #1	@ n.239,
	beq	.L40	@,
	movw	r2, #12000	@ tmp144,
	cmp	r3, r2	@ pretmp.220, tmp144
	moveq	r2, #2	@ n.239,
	beq	.L40	@,
	cmp	r3, #16000	@ pretmp.220,
	moveq	r2, #3	@ n.239,
	beq	.L40	@,
	movw	r2, #22050	@ tmp145,
	cmp	r3, r2	@ pretmp.220, tmp145
	moveq	r2, #4	@ n.239,
	beq	.L40	@,
	movw	r2, #24000	@ tmp146,
	cmp	r3, r2	@ pretmp.220, tmp146
	moveq	r2, #5	@ n.239,
	beq	.L40	@,
	cmp	r3, #32000	@ pretmp.220,
	moveq	r2, #6	@ n.239,
	beq	.L40	@,
	movw	r2, #44100	@ tmp147,
	cmp	r3, r2	@ pretmp.220, tmp147
	moveq	r2, #7	@ n.239,
	beq	.L40	@,
	movw	r2, #48000	@ tmp148,
	cmp	r3, r2	@ pretmp.220, tmp148
	moveq	r2, #8	@ n.239,
	beq	.L40	@,
	movw	r2, #:lower16:88200	@ tmp149,
	movt	r2, #:upper16:88200	@ tmp149,
	cmp	r3, r2	@ pretmp.220, tmp149
	moveq	r2, #9	@ n.239,
	beq	.L40	@,
	movw	r2, #:lower16:96000	@ tmp150,
	movt	r2, #:upper16:96000	@ tmp150,
	cmp	r3, r2	@ pretmp.220, tmp150
	moveq	r2, #10	@ n.239,
	beq	.L40	@,
	movw	r2, #:lower16:176400	@ tmp151,
	movt	r2, #:upper16:176400	@ tmp151,
	cmp	r3, r2	@ pretmp.220, tmp151
	moveq	r2, #11	@ n.239,
	beq	.L40	@,
	movw	r2, #:lower16:192000	@ tmp152,
	movt	r2, #:upper16:192000	@ tmp152,
	cmp	r3, r2	@ pretmp.220, tmp152
	beq	.L62	@,
	ldr	r4, [r0, #4]	@ D.3849, <variable>.Channels
	cmp	r4, #0	@ D.3849,
	ldmeqfd	sp, {r3, r4, fp, sp, pc}	@
.L63:
	cmp	r4, #1	@ D.3849,
	sub	ip, r4, #1	@ tmp157, D.3849,
	and	r1, ip, #1	@ tmp159, tmp157,
	add	r3, r0, #4	@ ivtmp.238, pLinearSrc,
	mov	ip, #0	@ tmp156,
	mov	r2, #1	@ n,
	str	ip, [r0, #36]	@ tmp156, <variable>.PreSample
	ldmlsfd	sp, {r3, r4, fp, sp, pc}	@
	cmp	r1, #0	@ tmp159,
	beq	.L54	@,
	mov	r2, #2	@ n,
	cmp	r4, r2	@ D.3849, n
	str	ip, [r3, #36]	@ tmp156, <variable>.PreSample
	add	r3, r3, #4	@ ivtmp.238, ivtmp.238,
	ldmlsfd	sp, {r3, r4, fp, sp, pc}	@
.L54:
	add	r2, r2, #2	@ n, n,
	add	r0, r3, #4	@ tmp161, ivtmp.238,
	cmp	r4, r2	@ D.3849, n
	str	ip, [r3, #36]	@ tmp156, <variable>.PreSample
	str	ip, [r0, #36]	@ tmp156, <variable>.PreSample
	add	r3, r0, #4	@ ivtmp.238, tmp161,
	bhi	.L54	@,
	ldmfd	sp, {r3, r4, fp, sp, pc}	@
.L62:
	mov	r2, #12	@ n.239,
.L40:
	movw	r1, #:lower16:.LANCHOR0	@ tmp153,
	movt	r1, #:upper16:.LANCHOR0	@ tmp153,
	ldr	r4, [r0, #4]	@ D.3849, <variable>.Channels
	ldr	r3, [r1, r2, asl #2]	@ tmp154, g_s32IvtSfTab
	cmp	r4, #0	@ D.3849,
	str	r3, [r0, #32]	@ tmp154, <variable>.IvtSFFactor
	bne	.L63	@,
	ldmfd	sp, {r3, r4, fp, sp, pc}	@
	.fnend
	.size	LSRC_Flush, .-LSRC_Flush
	.align	2
	.global	LSRC_CheckDoSRC
	.type	LSRC_CheckDoSRC, %function
LSRC_CheckDoSRC:
	.fnstart
.LFB149:
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
	ldr	r2, [r0, #8]	@ <variable>.AdjustInRate, <variable>.AdjustInRate
	ldr	r3, [r0, #16]	@ <variable>.OutRate, <variable>.OutRate
	cmp	r2, r3	@ <variable>.AdjustInRate, <variable>.OutRate
	movne	r0, #1	@ D.3861,
	ldmnefd	sp, {fp, sp, pc}	@
	ldr	r0, [r0, #24]	@ <variable>.DisContinuity, <variable>.DisContinuity
	subs	r0, r0, #0	@ D.3861, <variable>.DisContinuity,
	movne	r0, #1	@ D.3861
	ldmfd	sp, {fp, sp, pc}	@
	.fnend
	.size	LSRC_CheckDoSRC, .-LSRC_CheckDoSRC
	.align	2
	.global	LSRC_SetSpeedAdjust
	.type	LSRC_SetSpeedAdjust, %function
LSRC_SetSpeedAdjust:
	.fnstart
.LFB150:
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
	ldr	r3, [r0, #28]	@ prephitmp.291, <variable>.SpeedAdjust
	cmp	r3, r1	@ prephitmp.291, SpeedAdjust
	ldmeqfd	sp, {fp, sp, pc}	@
	cmp	r1, #100	@ SpeedAdjust,
	ldmgtfd	sp, {fp, sp, pc}	@
	cmn	r1, #100	@ SpeedAdjust,
	ldmltfd	sp, {fp, sp, pc}	@
	subs	r2, r1, #0	@ tmp143, SpeedAdjust,
	movne	r2, #1	@ tmp143
	cmp	r3, #0	@ prephitmp.291,
	ldr	ip, [r0, #12]	@ <variable>.InRate, <variable>.InRate
	movne	r2, #0	@, tmp143,
	cmp	r2, #0	@ tmp143,
	str	r1, [r0, #28]	@ SpeedAdjust, <variable>.SpeedAdjust
	movne	r2, #1	@ tmp144,
	strne	r2, [r0, #24]	@ tmp144, <variable>.DisContinuity
	add	r2, r1, #1000	@ tmp153, SpeedAdjust,
	subs	r3, r3, #0	@ tmp151, prephitmp.291,
	movne	r3, #1	@ tmp151
	cmp	r1, #0	@ SpeedAdjust,
	mov	r1, #0	@ tmp161,
	mul	r2, ip, r2	@ tmp154, <variable>.InRate, tmp153
	movne	r3, #0	@, tmp151,
	cmp	r3, #0	@ tmp151,
	str	r1, [r0, #20]	@ tmp161, <variable>.Remainder
	movne	r3, #1	@ tmp152,
	strne	r3, [r0, #24]	@ tmp152, <variable>.DisContinuity
	movw	r3, #:lower16:274877907	@ tmp157,
	movt	r3, #:upper16:274877907	@ tmp157,
	smull	r1, ip, r3, r2	@, tmp156, tmp157, tmp154
	mov	r2, r2, asr #31	@ tmp159, tmp154,
	rsb	r3, r2, ip, asr #6	@, tmp160, tmp159, tmp156,
	str	r3, [r0, #8]	@ tmp160, <variable>.AdjustInRate
	ldmfd	sp, {fp, sp, pc}	@
	.fnend
	.size	LSRC_SetSpeedAdjust, .-LSRC_SetSpeedAdjust
	.align	2
	.global	LSRC_ProcessFrame
	.type	LSRC_ProcessFrame, %function
LSRC_ProcessFrame:
	.fnstart
.LFB153:
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
	str	r1, [fp, #-92]	@ pInPcmBuf, %sfp
	ldr	r1, [r0, #0]	@ <variable>.BitPerSample, <variable>.BitPerSample
	str	r2, [fp, #-88]	@ pOutpcmBuf, %sfp
	cmp	r1, #16	@ <variable>.BitPerSample,
	str	r3, [fp, #-84]	@ InSamps, %sfp
	beq	.L228	@,
	ldr	r8, [r0, #24]	@ <variable>.DisContinuity, <variable>.DisContinuity
	ldr	r6, [fp, #-84]	@, %sfp
	ldr	r5, [r0, #4]	@, <variable>.Channels
	cmp	r8, #0	@ <variable>.DisContinuity,
	cmpne	r6, #0	@,,
	str	r5, [fp, #-48]	@, %sfp
	movle	r3, #0	@, tmp339
	movgt	r3, #1	@, tmp339
	bgt	.L104	@,
	str	r3, [fp, #-96]	@ tmp339, %sfp
	ldr	r7, [fp, #-96]	@, %sfp
	ldr	r3, [r0, #8]	@ <variable>.AdjustInRate, <variable>.AdjustInRate
	str	r7, [fp, #-64]	@, %sfp
	str	r7, [fp, #-68]	@, %sfp
	ldr	r8, [r0, #16]	@ temp.656, <variable>.OutRate
.L105:
	cmp	r3, r8	@ <variable>.AdjustInRate, temp.656
	beq	.L229	@,
	ldr	r2, [fp, #-84]	@, %sfp
	cmp	r2, #0	@,
	ble	.L117	@,
	ldr	r4, [r0, #20]	@ prephitmp.378, <variable>.Remainder
	cmp	r8, r4	@ temp.656, prephitmp.378
	ble	.L118	@,
	ldr	r5, [fp, #-48]	@, %sfp
	ldr	sl, [fp, #-64]	@, %sfp
	ldr	r6, [fp, #-96]	@, %sfp
	mov	r9, r5, asl #2	@,,
	str	r9, [fp, #-60]	@, %sfp
	mul	r9, r5, sl	@ tmp354,,
	ldr	r7, [fp, #-92]	@, %sfp
	mul	r2, r5, r6	@ pretmp.383,,
	ldr	ip, [fp, #-88]	@, %sfp
	movw	sl, #32767	@ tmp446,
	add	r1, r7, r2, asl #2	@,,, pretmp.383,
	add	r7, ip, r9, asl #2	@, ivtmp.577,, tmp354,
	mov	r9, r5	@ Channels,
	str	r1, [fp, #-56]	@, %sfp
	str	r7, [fp, #-52]	@ ivtmp.577, %sfp
.L119:
	cmp	r9, #0	@ Channels,
	ble	.L122	@,
	tst	r9, #1	@ Channels,
	ldr	r1, [fp, #-56]	@ ivtmp.569, %sfp
	mov	ip, r0	@ ivtmp.575, hLSRC
	ldr	r2, [fp, #-52]	@ ivtmp.652, %sfp
	mov	r7, #0	@ ch.637,
	strne	r9, [fp, #-48]	@ Channels, %sfp
	bne	.L123	@,
	ldr	r6, [r0, #32]	@ D.4503, <variable>.IvtSFFactor
	ldr	r1, [fp, #-56]	@, %sfp
	mul	ip, r6, r4	@ tmp713, D.4503, prephitmp.378
	ldr	r3, [r1, #0]	@ tmp712,
#APP
@ 584 "/home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/../../../source/msp/drv/adsp/adsp_v1_1/hifi/common/include/hi_dsp_basicop.h" 1
	smull r5,ip,r3,ip	@, sum, tmp712, sum
@ 0 "" 2
	rsb	r4, r4, r8	@ tmp717, prephitmp.378, temp.656
	ldr	r5, [r0, #36]	@ tmp716, <variable>.PreSample
	mul	r2, r6, r4	@ tmp718, D.4503, tmp717
#APP
@ 584 "/home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/../../../source/msp/drv/adsp/adsp_v1_1/hifi/common/include/hi_dsp_basicop.h" 1
	smull r6,r2,r5,r2	@, y, tmp716, y
@ 0 "" 2
	add	r8, r2, ip	@ tmp721, y, sum
	ldr	r2, [fp, #-52]	@ ivtmp.652, %sfp
	add	ip, r0, #4	@ ivtmp.575, hLSRC,
	mov	r7, r8, asr #31	@ sign, tmp721,
	mov	r1, r8, asr #14	@ x, tmp721,
	cmp	r7, r8, asr #29	@ sign, tmp721,
	eorne	r1, r7, sl	@ x, sign, tmp446
	mov	r7, #1	@ ch.637,
	mov	r3, r1, asl #16	@ tmp727, x,
	str	r3, [r2], #4	@ tmp727,
	ldr	r6, [fp, #-56]	@, %sfp
	ldr	r4, [r0, #20]	@ prephitmp.378, <variable>.Remainder
	ldr	r8, [r0, #16]	@ temp.656, <variable>.OutRate
	add	r1, r6, #4	@ ivtmp.569,,
	str	r9, [fp, #-48]	@ Channels, %sfp
	b	.L123	@
.L121:
	ldr	r6, [r0, #20]	@ prephitmp.378, <variable>.Remainder
	ldr	r4, [r0, #32]	@ D.4503, <variable>.IvtSFFactor
	ldr	r1, [r1, #4]	@,
	mul	r3, r4, r6	@ tmp357, D.4503, prephitmp.378
	str	r1, [fp, #-72]	@, %sfp
	ldr	r9, [r0, #16]	@ temp.656, <variable>.OutRate
#APP
@ 584 "/home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/../../../source/msp/drv/adsp/adsp_v1_1/hifi/common/include/hi_dsp_basicop.h" 1
	smull r8,r3,r1,r3	@, sum,, sum
@ 0 "" 2
	rsb	r8, r6, r9	@ tmp359, prephitmp.378, temp.656
	ldr	r1, [ip, #36]	@ tmp358, <variable>.PreSample
	mul	r6, r4, r8	@ tmp361, D.4503, tmp359
#APP
@ 584 "/home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/../../../source/msp/drv/adsp/adsp_v1_1/hifi/common/include/hi_dsp_basicop.h" 1
	smull r9,r6,r1,r6	@, y, tmp358, y
@ 0 "" 2
	add	r3, r6, r3	@ tmp362, y, sum
	add	r7, r7, #1	@ ch.637, tmp501,
	add	r1, r5, #4	@ ivtmp.569, tmp502,
	add	ip, ip, #4	@ ivtmp.575, tmp503,
	mov	r8, r3, asr #31	@ sign, tmp362,
	mov	r4, r3, asr #14	@ x, tmp362,
	cmp	r8, r3, asr #29	@ sign, tmp362,
	eorne	r4, r8, sl	@ x, sign, tmp446
	mov	r9, r4, asl #16	@ tmp730, x,
	str	r9, [r2], #4	@ tmp730,
	ldr	r4, [r0, #20]	@ prephitmp.378, <variable>.Remainder
	ldr	r8, [r0, #16]	@ temp.656, <variable>.OutRate
.L123:
	ldr	r5, [r0, #32]	@ D.4503, <variable>.IvtSFFactor
	ldr	r9, [r1, #0]	@,* ivtmp.569
	mul	r3, r5, r4	@ tmp610, D.4503, prephitmp.378
#APP
@ 584 "/home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/../../../source/msp/drv/adsp/adsp_v1_1/hifi/common/include/hi_dsp_basicop.h" 1
	smull r6,r3,r9,r3	@, sum,, sum
@ 0 "" 2
	rsb	r4, r4, r8	@ tmp615, prephitmp.378, temp.656
	ldr	r6, [ip, #36]	@ tmp614, <variable>.PreSample
	mul	r9, r5, r4	@ tmp616, D.4503, tmp615
#APP
@ 584 "/home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/../../../source/msp/drv/adsp/adsp_v1_1/hifi/common/include/hi_dsp_basicop.h" 1
	smull r5,r9,r6,r9	@, y, tmp614, y
@ 0 "" 2
	add	r5, r9, r3	@ tmp619, y, sum
	ldr	r6, [fp, #-48]	@, %sfp
	add	r7, r7, #1	@ tmp501, ch.637,
	add	ip, ip, #4	@ tmp503, ivtmp.575,
	mov	r4, r5, asr #31	@ sign, tmp619,
	mov	r8, r5, asr #14	@ x, tmp619,
	cmp	r4, r5, asr #29	@ sign, tmp619,
	add	r5, r1, #4	@ tmp502, ivtmp.569,
	eorne	r8, r4, sl	@ x, sign, tmp446
	cmp	r7, r6	@ tmp501,
	mov	r3, r8, asl #16	@ tmp366, x,
	str	r3, [r2], #4	@ tmp366,
	bne	.L121	@,
	ldr	r9, [fp, #-48]	@ Channels, %sfp
	ldr	r4, [r0, #20]	@ prephitmp.378, <variable>.Remainder
	ldr	r3, [r0, #8]	@ <variable>.AdjustInRate, <variable>.AdjustInRate
	ldr	r8, [r0, #16]	@ temp.656, <variable>.OutRate
.L122:
	ldr	ip, [fp, #-64]	@, %sfp
	add	r4, r4, r3	@ prephitmp.378, prephitmp.378, <variable>.AdjustInRate
	ldr	r7, [fp, #-68]	@, %sfp
	cmp	r4, r8	@ prephitmp.378, temp.656
	add	r5, ip, #1	@,,
	str	r5, [fp, #-64]	@, %sfp
	add	r7, r7, #1	@,,
	str	r7, [fp, #-68]	@, %sfp
	str	r4, [r0, #20]	@ prephitmp.378, <variable>.Remainder
	ldr	r1, [fp, #-52]	@, %sfp
	ldr	r2, [fp, #-60]	@, %sfp
	add	ip, r1, r2	@,,
	str	ip, [fp, #-52]	@, %sfp
	blt	.L119	@,
.L118:
	ldr	r5, [fp, #-84]	@, %sfp
	rsb	r4, r8, r4	@ prephitmp.639, temp.656, prephitmp.378
	str	r4, [r0, #20]	@ prephitmp.639, <variable>.Remainder
	cmp	r5, #1	@,
	beq	.L117	@,
	ldr	r6, [fp, #-48]	@, %sfp
	mov	r7, #0	@,
	ldr	sl, [fp, #-96]	@, %sfp
	str	r5, [fp, #-80]	@, %sfp
	mov	r9, r6, asl #2	@,,
	add	ip, sl, #1	@ tmp433,,
	mul	r1, r6, sl	@ tmp436,,
	ldr	r6, [fp, #-92]	@, %sfp
	str	r9, [fp, #-72]	@, %sfp
	str	r7, [fp, #-76]	@, %sfp
	mla	r2, ip, r9, r6	@, tmp433,,
	add	sl, r6, r1, asl #2	@,,, tmp436,
	str	sl, [fp, #-104]	@, %sfp
	str	r2, [fp, #-100]	@, %sfp
.L124:
	cmp	r4, r8	@ prephitmp.639, temp.656
	bge	.L127	@,
	ldr	sl, [fp, #-48]	@, %sfp
	ldr	r5, [fp, #-64]	@, %sfp
	ldr	r7, [fp, #-76]	@, %sfp
	ldr	r1, [fp, #-100]	@, %sfp
	mul	ip, sl, r5	@ tmp388,,
	ldr	r9, [fp, #-104]	@, %sfp
	ldr	sl, [fp, #-88]	@, %sfp
	add	r2, r1, r7	@,,
	add	r6, r9, r7	@,,
	str	r2, [fp, #-56]	@, %sfp
	str	r6, [fp, #-60]	@, %sfp
	add	r5, sl, ip, asl #2	@,,, tmp388,
	str	r5, [fp, #-52]	@, %sfp
.L130:
	ldr	r5, [fp, #-48]	@, %sfp
	cmp	r5, #0	@,
	ble	.L128	@,
	tst	r5, #1	@,
	ldr	r6, [fp, #-56]	@ ivtmp.540, %sfp
	ldr	r2, [fp, #-60]	@ ivtmp.544, %sfp
	mov	ip, #0	@ ch.650,
	ldr	r1, [fp, #-52]	@ ivtmp.660, %sfp
	movw	sl, #32767	@ tmp424,
	bne	.L129	@,
	ldr	r2, [r0, #32]	@ temp.640, <variable>.IvtSFFactor
	ldr	ip, [fp, #-56]	@, %sfp
	mul	r3, r2, r4	@ tmp693, temp.640, prephitmp.639
	ldr	r5, [ip, #0]	@ tmp692,
#APP
@ 584 "/home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/../../../source/msp/drv/adsp/adsp_v1_1/hifi/common/include/hi_dsp_basicop.h" 1
	smull ip,r3,r5,r3	@, sum.642, tmp692, sum.642
@ 0 "" 2
	rsb	r7, r4, r8	@ tmp697, prephitmp.639, temp.656
	ldr	r8, [fp, #-60]	@, %sfp
	mul	r9, r2, r7	@ tmp698, temp.640, tmp697
	ldr	r1, [r8, #0]	@ tmp696,
#APP
@ 584 "/home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/../../../source/msp/drv/adsp/adsp_v1_1/hifi/common/include/hi_dsp_basicop.h" 1
	smull r6,r9,r1,r9	@, y, tmp696, y
@ 0 "" 2
	add	r6, r9, r3	@ tmp701, y, sum.642
	ldr	r1, [fp, #-52]	@ ivtmp.660, %sfp
	mov	ip, r6, asr #31	@ sign, tmp701,
	mov	r4, r6, asr #14	@ x, tmp701,
	cmp	ip, r6, asr #29	@ sign, tmp701,
	eorne	r4, ip, sl	@ x, sign, tmp424
	mov	ip, #1	@ ch.650,
	mov	r7, r4, asl #16	@ tmp707, x,
	str	r7, [r1], #4	@ tmp707,
	ldr	r8, [fp, #-60]	@, %sfp
	ldr	r5, [fp, #-56]	@, %sfp
	add	r2, r8, #4	@ ivtmp.544,,
	ldr	r4, [r0, #20]	@ prephitmp.639, <variable>.Remainder
	add	r6, r5, #4	@ ivtmp.540,,
	ldr	r8, [r0, #16]	@ temp.656, <variable>.OutRate
	b	.L129	@
.L126:
	ldr	r5, [r0, #20]	@ prephitmp.639, <variable>.Remainder
	ldr	r4, [r0, #32]	@ temp.640, <variable>.IvtSFFactor
	ldr	r6, [r6, #4]	@,
	mul	r3, r4, r5	@ tmp370, temp.640, prephitmp.639
	str	r6, [fp, #-112]	@, %sfp
#APP
@ 584 "/home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/../../../source/msp/drv/adsp/adsp_v1_1/hifi/common/include/hi_dsp_basicop.h" 1
	smull r9,r3,r6,r3	@, sum.642,, sum.642
@ 0 "" 2
	ldr	r6, [r0, #16]	@, <variable>.OutRate
	ldr	r2, [r2, #4]	@ tmp371,
	rsb	r9, r5, r6	@ tmp372, prephitmp.639,
	mul	r5, r4, r9	@ tmp374, temp.640, tmp372
#APP
@ 584 "/home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/../../../source/msp/drv/adsp/adsp_v1_1/hifi/common/include/hi_dsp_basicop.h" 1
	smull r9,r5,r2,r5	@, y, tmp371, y
@ 0 "" 2
	add	r4, r5, r3	@ tmp375, y, sum.642
	add	r6, r8, #4	@ ivtmp.540, tmp495,
	add	ip, ip, #1	@ ch.650, tmp494,
	mov	r3, r4, asr #31	@ sign, tmp375,
	mov	r2, r4, asr #14	@ x, tmp375,
	cmp	r3, r4, asr #29	@ sign, tmp375,
	eorne	r2, r3, sl	@ x, sign, tmp424
	mov	r4, r2, asl #16	@ tmp710, x,
	str	r4, [r1], #4	@ tmp710,
	ldr	r4, [r0, #20]	@ prephitmp.639, <variable>.Remainder
	add	r2, r7, #4	@ ivtmp.544, tmp496,
	ldr	r8, [r0, #16]	@ temp.656, <variable>.OutRate
.L129:
	ldr	r5, [r0, #32]	@ temp.640, <variable>.IvtSFFactor
	ldr	r7, [r6, #0]	@ tmp629,* ivtmp.540
	mul	r3, r5, r4	@ tmp630, temp.640, prephitmp.639
#APP
@ 584 "/home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/../../../source/msp/drv/adsp/adsp_v1_1/hifi/common/include/hi_dsp_basicop.h" 1
	smull r9,r3,r7,r3	@, sum.642, tmp629, sum.642
@ 0 "" 2
	rsb	r4, r4, r8	@ tmp635, prephitmp.639, temp.656
	ldr	r9, [r2, #0]	@ tmp634,* ivtmp.544
	mul	r8, r5, r4	@ tmp636, temp.640, tmp635
#APP
@ 584 "/home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/../../../source/msp/drv/adsp/adsp_v1_1/hifi/common/include/hi_dsp_basicop.h" 1
	smull r5,r8,r9,r8	@, y, tmp634, y
@ 0 "" 2
	add	r7, r8, r3	@ tmp639, y, sum.642
	ldr	r9, [fp, #-48]	@, %sfp
	add	ip, ip, #1	@ tmp494, ch.650,
	add	r8, r6, #4	@ tmp495, ivtmp.540,
	mov	r5, r7, asr #31	@ sign, tmp639,
	mov	r4, r7, asr #14	@ x, tmp639,
	cmp	r5, r7, asr #29	@ sign, tmp639,
	add	r7, r2, #4	@ tmp496, ivtmp.544,
	eorne	r4, r5, sl	@ x, sign, tmp424
	cmp	ip, r9	@ tmp494,
	mov	r3, r4, asl #16	@ tmp379, x,
	str	r3, [r1], #4	@ tmp379,
	bne	.L126	@,
	ldr	r4, [r0, #20]	@ prephitmp.639, <variable>.Remainder
	ldr	r3, [r0, #8]	@ <variable>.AdjustInRate, <variable>.AdjustInRate
	ldr	r8, [r0, #16]	@ temp.656, <variable>.OutRate
.L128:
	ldr	r7, [fp, #-68]	@, %sfp
	add	r4, r4, r3	@ prephitmp.639, prephitmp.639, <variable>.AdjustInRate
	ldr	r6, [fp, #-64]	@, %sfp
	cmp	r4, r8	@ prephitmp.639, temp.656
	add	r9, r7, #1	@,,
	str	r9, [fp, #-68]	@, %sfp
	add	r2, r6, #1	@,,
	str	r2, [fp, #-64]	@, %sfp
	str	r4, [r0, #20]	@ prephitmp.639, <variable>.Remainder
	ldr	ip, [fp, #-52]	@, %sfp
	ldr	sl, [fp, #-72]	@, %sfp
	add	r1, ip, sl	@,,
	str	r1, [fp, #-52]	@, %sfp
	blt	.L130	@,
.L127:
	ldr	ip, [fp, #-80]	@, %sfp
	rsb	r4, r8, r4	@ prephitmp.639, temp.656, prephitmp.639
	sub	r6, ip, #1	@,,
	str	r6, [fp, #-80]	@, %sfp
	str	r4, [r0, #20]	@ prephitmp.639, <variable>.Remainder
	cmp	r6, #1	@,
	ldr	r1, [fp, #-76]	@, %sfp
	ldr	r9, [fp, #-72]	@, %sfp
	add	r7, r1, r9	@,,
	str	r7, [fp, #-76]	@, %sfp
	bne	.L124	@,
	ldr	r1, [fp, #-96]	@, %sfp
	ldr	sl, [fp, #-84]	@, %sfp
	add	r2, sl, r1	@ D.4636,,
	sub	r3, r2, #1	@, D.4636,
	str	r3, [fp, #-96]	@, %sfp
.L117:
	ldr	ip, [fp, #-48]	@, %sfp
	cmp	ip, #0	@,
	ble	.L86	@,
	ldr	r2, [fp, #-96]	@, %sfp
	mov	r1, #1	@ ch.654,
	ldr	r4, [fp, #-92]	@, %sfp
	cmp	r1, ip	@ ch.654,
	mul	r3, ip, r2	@ tmp390,,
	sub	ip, ip, #1	@ tmp485,,
	and	ip, ip, r1	@ tmp487, tmp485,
	add	r2, r0, #4	@ ivtmp.535, hLSRC,
	add	r3, r4, r3, asl #2	@, ivtmp.532,, tmp390,
	ldr	r4, [r3], #4	@ tmp392,
	str	r4, [r0, #36]	@ tmp392, <variable>.PreSample
	beq	.L86	@,
	cmp	ip, #0	@ tmp487,
	beq	.L223	@,
	ldr	r8, [fp, #-48]	@, %sfp
	mov	r1, #2	@ ch.654,
	ldr	r0, [r3], #4	@ tmp690,
	cmp	r1, r8	@ ch.654,
	str	r0, [r2, #36]	@ tmp690, <variable>.PreSample
	add	r2, r2, #4	@ ivtmp.535, ivtmp.535,
	beq	.L86	@,
.L223:
	ldr	r5, [fp, #-48]	@ Channels, %sfp
.L131:
	mov	r0, r3	@ tmp489, ivtmp.532
	add	r1, r1, #2	@ ch.654, ch.654,
	ldr	r4, [r0], #4	@ tmp648,
	add	ip, r2, #4	@ tmp490, ivtmp.535,
	cmp	r1, r5	@ ch.654, Channels
	str	r4, [r2, #36]	@ tmp648, <variable>.PreSample
	add	r2, ip, #4	@ ivtmp.535, tmp490,
	ldr	r4, [r3, #4]	@ tmp652,
	add	r3, r0, #4	@ ivtmp.532, tmp489,
	str	r4, [ip, #36]	@ tmp652, <variable>.PreSample
	bne	.L131	@,
.L86:
	ldr	r0, [fp, #-68]	@, %sfp
	sub	sp, fp, #40
	ldmfd	sp, {r4, r5, r6, r7, r8, r9, sl, fp, sp, pc}
.L104:
	ldr	r3, [r0, #8]	@ <variable>.AdjustInRate, <variable>.AdjustInRate
	ldr	r8, [r0, #16]	@ temp.656, <variable>.OutRate
	cmp	r3, r8	@ <variable>.AdjustInRate, temp.656
	beq	.L106	@,
	ldr	r9, [fp, #-48]	@, %sfp
	cmp	r9, #0	@,
	ble	.L108	@,
	ldr	r7, [fp, #-92]	@, %sfp
	mov	r2, #1	@ ch.626,
	ldr	r9, [fp, #-48]	@, %sfp
	ldr	r4, [r7, #0]	@ tmp346,
	cmp	r2, r9	@ ch.626,
	sub	r1, r9, #1	@ tmp510,,
	and	ip, r1, r2	@ tmp512, tmp510,
	add	r1, r0, #4	@ ivtmp.597, hLSRC,
	str	r4, [r0, #36]	@ tmp346, <variable>.PreSample
	beq	.L108	@,
	cmp	ip, #0	@ tmp512,
	beq	.L227	@,
	ldr	r2, [fp, #-92]	@, %sfp
	ldr	r4, [fp, #-48]	@, %sfp
	ldr	ip, [r2, #4]	@ tmp734,
	mov	r2, #2	@ ch.626,
	cmp	r2, r4	@ ch.626,
	str	ip, [r1, #36]	@ tmp734, <variable>.PreSample
	add	r1, r1, #4	@ ivtmp.597, ivtmp.597,
	beq	.L108	@,
.L227:
	ldr	r5, [fp, #-92]	@ pInPcmBuf, %sfp
.L111:
	ldr	r6, [r5, r2, asl #2]	@ tmp592,* pInPcmBuf
	add	r4, r2, #1	@ tmp513, ch.626,
	ldr	sl, [fp, #-48]	@, %sfp
	add	r2, r4, #1	@ ch.626, tmp513,
	add	ip, r1, #4	@ tmp514, ivtmp.597,
	str	r6, [r1, #36]	@ tmp592, <variable>.PreSample
	cmp	r2, sl	@ ch.626,
	ldr	r4, [r5, r4, asl #2]	@ tmp596,* pInPcmBuf
	add	r1, ip, #4	@ ivtmp.597, tmp514,
	str	r4, [ip, #36]	@ tmp596, <variable>.PreSample
	bne	.L111	@,
.L108:
	ldr	r1, [fp, #-84]	@, %sfp
	mov	r2, #0	@ tmp347,
	str	r2, [fp, #-64]	@ tmp347, %sfp
	sub	ip, r1, #1	@,,
	str	r2, [fp, #-68]	@ tmp347, %sfp
	str	ip, [fp, #-84]	@, %sfp
	mov	r1, #1	@,
	str	r1, [fp, #-96]	@, %sfp
	str	r2, [r0, #24]	@ tmp347, <variable>.DisContinuity
	b	.L105	@
.L229:
	ldr	r2, [fp, #-84]	@, %sfp
	cmp	r2, #0	@,
	ble	.L86	@,
	ldr	r3, [fp, #-48]	@, %sfp
	mov	r7, r2	@ InSamps.632,
	ldr	r4, [fp, #-96]	@, %sfp
	ldr	r9, [fp, #-64]	@, %sfp
	ldr	sl, [fp, #-88]	@, %sfp
	mov	r8, r3, asl #2	@ D.4850,,
	mul	r6, r3, r4	@ tmp349,,
	mul	r0, r3, r9	@ tmp351,,
	ldr	r9, [fp, #-92]	@, %sfp
	add	r6, r9, r6, asl #2	@, ivtmp.630,, tmp349,
	add	r5, sl, r0, asl #2	@, ivtmp.659,, tmp351,
	mov	sl, r3	@ Channels,
.L113:
	cmp	sl, #0	@ Channels,
	ble	.L116	@,
	mov	r0, r6	@ ivtmp.501, ivtmp.630
	mov	ip, #1	@ ch,
	ldr	r3, [r0], #4	@ tmp353,
	cmp	ip, sl	@ ch, Channels
	sub	r2, sl, #1	@ tmp479, Channels,
	mov	r1, r5	@ ivtmp.503, ivtmp.659
	and	r2, r2, ip	@ tmp481, tmp479,
	str	r3, [r1], #4	@ tmp353,
	beq	.L116	@,
	cmp	r2, #0	@ tmp481,
	beq	.L114	@,
	ldr	r2, [r0], #4	@ tmp689,
	mov	ip, #2	@ ch,
	cmp	ip, sl	@ ch, Channels
	str	r2, [r1], #4	@ tmp689,
	beq	.L116	@,
.L114:
	mov	r2, r0	@ tmp483, ivtmp.501
	mov	r3, r1	@ tmp484, ivtmp.503
	ldr	r4, [r2], #4	@ tmp600,
	add	ip, ip, #2	@ ch, ch,
	cmp	ip, sl	@ ch, Channels
	str	r4, [r3], #4	@ tmp600,
	ldr	r4, [r0, #4]	@ tmp604,
	add	r0, r2, #4	@ ivtmp.501, tmp483,
	str	r4, [r1, #4]	@ tmp604,
	add	r1, r3, #4	@ ivtmp.503, tmp484,
	bne	.L114	@,
.L116:
	subs	r7, r7, #1	@ InSamps.632, InSamps.632,
	add	r6, r6, r8	@ ivtmp.630, ivtmp.630, D.4850
	add	r5, r5, r8	@ ivtmp.659, ivtmp.659, D.4850
	bne	.L113	@,
	ldr	r1, [fp, #-68]	@, %sfp
	ldr	r0, [fp, #-84]	@, %sfp
	add	ip, r1, r0	@,,
	str	ip, [fp, #-68]	@, %sfp
	b	.L86	@
.L228:
	ldr	r3, [r0, #24]	@ <variable>.DisContinuity, <variable>.DisContinuity
	ldr	sl, [fp, #-84]	@, %sfp
	ldr	r6, [r0, #4]	@ Channels, <variable>.Channels
	cmp	r3, #0	@ <variable>.DisContinuity,
	cmpne	sl, #0	@,,
	movle	r3, #0	@, tmp281
	movgt	r3, #1	@, tmp281
	bgt	.L75	@,
	str	r3, [fp, #-76]	@ tmp281, %sfp
	ldr	r2, [fp, #-76]	@, %sfp
	ldr	r3, [r0, #8]	@ <variable>.AdjustInRate, <variable>.AdjustInRate
	str	r2, [fp, #-64]	@, %sfp
	str	r2, [fp, #-68]	@, %sfp
	ldr	sl, [r0, #16]	@ temp.656, <variable>.OutRate
.L76:
	cmp	r3, sl	@ <variable>.AdjustInRate, temp.656
	beq	.L230	@,
	ldr	r1, [fp, #-84]	@, %sfp
	cmp	r1, #0	@,
	ble	.L89	@,
	ldr	r2, [r0, #20]	@ prephitmp.609, <variable>.Remainder
	cmp	sl, r2	@ temp.656, prephitmp.609
	ble	.L90	@,
	ldr	r4, [fp, #-76]	@, %sfp
	mov	r1, r6, asl #1	@, Channels,
	ldr	r9, [fp, #-64]	@, %sfp
	ldr	r8, [fp, #-88]	@, %sfp
	mul	ip, r6, r4	@ D.4441, Channels,
	ldr	r7, [fp, #-92]	@, %sfp
	mul	r4, r6, r9	@ tmp297, Channels,
	movw	r9, #32767	@ tmp421,
	str	r1, [fp, #-56]	@, %sfp
	add	r5, r7, ip, asl #1	@,,, D.4441,
	str	r5, [fp, #-52]	@, %sfp
	add	ip, r8, r4, asl #1	@,,, tmp297,
	mov	r8, r6	@ Channels, Channels
	str	ip, [fp, #-48]	@, %sfp
.L91:
	cmp	r8, #0	@ Channels,
	ble	.L94	@,
	tst	r8, #1	@ Channels,
	ldr	ip, [fp, #-52]	@ ivtmp.459, %sfp
	mov	r4, r0	@ ivtmp.467, hLSRC
	ldr	r1, [fp, #-48]	@ ivtmp.469, %sfp
	mov	r7, #0	@ ch.607,
	strne	r8, [fp, #-72]	@ Channels, %sfp
	bne	.L95	@,
	ldr	r6, [r0, #32]	@ D.4434, <variable>.IvtSFFactor
	ldr	r3, [fp, #-52]	@, %sfp
	mul	r7, r6, r2	@ tmp673, D.4434, prephitmp.609
	ldrsh	r1, [r3, #0]	@ tmp672,
#APP
@ 584 "/home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/../../../source/msp/drv/adsp/adsp_v1_1/hifi/common/include/hi_dsp_basicop.h" 1
	smull r4,r7,r1,r7	@, sum, tmp672, sum
@ 0 "" 2
	rsb	r2, r2, sl	@ tmp677, prephitmp.609, temp.656
	ldr	ip, [r0, #36]	@ tmp676, <variable>.PreSample
	mul	r4, r6, r2	@ tmp678, D.4434, tmp677
#APP
@ 584 "/home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/../../../source/msp/drv/adsp/adsp_v1_1/hifi/common/include/hi_dsp_basicop.h" 1
	smull r5,r4,ip,r4	@, y, tmp676, y
@ 0 "" 2
	add	r5, r4, r7	@ tmp681, y, sum
	mov	r7, #1	@ ch.607,
	add	r4, r0, #4	@ ivtmp.467, hLSRC,
	mov	r3, r5, asl #2	@ x, tmp681,
	mov	r1, r3, asr #31	@ sign, x,
	cmp	r1, r3, asr #15	@ sign, x,
	eorne	r3, r1, r9	@ x, sign, tmp421
	ldr	r1, [fp, #-48]	@ ivtmp.469, %sfp
	strh	r3, [r1], #2	@ movhi	@ x,
	ldr	sl, [fp, #-52]	@, %sfp
	ldr	r2, [r0, #20]	@ prephitmp.609, <variable>.Remainder
	add	ip, sl, #2	@ ivtmp.459,,
	ldr	sl, [r0, #16]	@ temp.656, <variable>.OutRate
	str	r8, [fp, #-72]	@ Channels, %sfp
	b	.L95	@
.L93:
	ldr	r6, [r0, #20]	@ prephitmp.609, <variable>.Remainder
	ldr	r5, [r0, #32]	@ D.4434, <variable>.IvtSFFactor
	ldrsh	ip, [ip, #2]	@,
	mul	r3, r5, r6	@ tmp300, D.4434, prephitmp.609
	str	ip, [fp, #-60]	@, %sfp
	ldr	r8, [r0, #16]	@ temp.656, <variable>.OutRate
#APP
@ 584 "/home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/../../../source/msp/drv/adsp/adsp_v1_1/hifi/common/include/hi_dsp_basicop.h" 1
	smull sl,r3,ip,r3	@, sum,, sum
@ 0 "" 2
	rsb	r6, r6, r8	@ tmp302, prephitmp.609, temp.656
	ldr	ip, [r4, #36]	@ tmp301, <variable>.PreSample
	mul	r8, r5, r6	@ tmp304, D.4434, tmp302
#APP
@ 584 "/home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/../../../source/msp/drv/adsp/adsp_v1_1/hifi/common/include/hi_dsp_basicop.h" 1
	smull r6,r8,ip,r8	@, y, tmp301, y
@ 0 "" 2
	add	sl, r8, r3	@ tmp305, y, sum
	add	ip, r2, #2	@ ivtmp.459, tmp471,
	add	r7, r7, #1	@ ch.607, tmp470,
	add	r4, r4, #4	@ ivtmp.467, tmp472,
	mov	r3, sl, asl #2	@ x, tmp305,
	mov	r6, r3, asr #31	@ sign, x,
	cmp	r6, r3, asr #15	@ sign, x,
	eorne	r3, r6, r9	@ x, sign, tmp421
	strh	r3, [r1], #2	@ movhi	@ x,
	ldr	r2, [r0, #20]	@ prephitmp.609, <variable>.Remainder
	ldr	sl, [r0, #16]	@ temp.656, <variable>.OutRate
.L95:
	ldr	r5, [r0, #32]	@ D.4434, <variable>.IvtSFFactor
	ldrsh	r8, [ip, #0]	@,* ivtmp.459
	mul	r3, r5, r2	@ tmp536, D.4434, prephitmp.609
	str	r8, [fp, #-60]	@, %sfp
#APP
@ 584 "/home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/../../../source/msp/drv/adsp/adsp_v1_1/hifi/common/include/hi_dsp_basicop.h" 1
	smull r6,r3,r8,r3	@, sum,, sum
@ 0 "" 2
	rsb	r8, r2, sl	@ tmp541, prephitmp.609, temp.656
	ldr	sl, [r4, #36]	@ tmp540, <variable>.PreSample
	mul	r2, r5, r8	@ tmp542, D.4434, tmp541
#APP
@ 584 "/home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/../../../source/msp/drv/adsp/adsp_v1_1/hifi/common/include/hi_dsp_basicop.h" 1
	smull r8,r2,sl,r2	@, y, tmp540, y
@ 0 "" 2
	add	r6, r2, r3	@ tmp545, y, sum
	ldr	sl, [fp, #-72]	@, %sfp
	add	r7, r7, #1	@ tmp470, ch.607,
	add	r4, r4, #4	@ tmp472, ivtmp.467,
	mov	r3, r6, asl #2	@ x, tmp545,
	add	r2, ip, #2	@ tmp471, ivtmp.459,
	mov	r5, r3, asr #31	@ sign, x,
	cmp	r5, r3, asr #15	@ sign, x,
	eorne	r3, r5, r9	@ x, sign, tmp421
	cmp	r7, sl	@ tmp470,
	strh	r3, [r1], #2	@ movhi	@ x,
	bne	.L93	@,
	ldr	r8, [fp, #-72]	@ Channels, %sfp
	ldr	r2, [r0, #20]	@ prephitmp.609, <variable>.Remainder
	ldr	r3, [r0, #8]	@ <variable>.AdjustInRate, <variable>.AdjustInRate
	ldr	sl, [r0, #16]	@ temp.656, <variable>.OutRate
.L94:
	ldr	r4, [fp, #-64]	@, %sfp
	add	r2, r2, r3	@ prephitmp.609, prephitmp.609, <variable>.AdjustInRate
	ldr	r6, [fp, #-68]	@, %sfp
	cmp	r2, sl	@ prephitmp.609, temp.656
	add	r1, r4, #1	@,,
	str	r1, [fp, #-64]	@, %sfp
	add	ip, r6, #1	@,,
	str	ip, [fp, #-68]	@, %sfp
	str	r2, [r0, #20]	@ prephitmp.609, <variable>.Remainder
	ldr	r7, [fp, #-48]	@, %sfp
	ldr	r5, [fp, #-56]	@, %sfp
	add	r4, r7, r5	@,,
	str	r4, [fp, #-48]	@, %sfp
	blt	.L91	@,
	mov	r6, r8	@ Channels, Channels
.L90:
	ldr	r7, [fp, #-84]	@, %sfp
	rsb	r2, sl, r2	@ prephitmp.610, temp.656, prephitmp.609
	str	r2, [r0, #20]	@ prephitmp.610, <variable>.Remainder
	cmp	r7, #1	@,
	beq	.L89	@,
	ldr	r8, [fp, #-76]	@, %sfp
	mov	r5, #0	@,
	str	r5, [fp, #-80]	@, %sfp
	mov	r9, r6, asl #1	@, Channels,
	ldr	r5, [fp, #-92]	@, %sfp
	add	r1, r8, #1	@ tmp408,,
	mul	ip, r6, r8	@ tmp411, Channels,
	str	r9, [fp, #-72]	@, %sfp
	mla	r4, r1, r9, r5	@, tmp408,,
	str	r7, [fp, #-96]	@, %sfp
	str	r6, [fp, #-48]	@ Channels, %sfp
	add	r1, r5, ip, asl #1	@,,, tmp411,
	str	r1, [fp, #-104]	@, %sfp
	str	r4, [fp, #-100]	@, %sfp
	mov	r4, r2	@ prephitmp.610, prephitmp.610
.L96:
	cmp	sl, r4	@ temp.656, prephitmp.610
	ble	.L99	@,
	ldr	r9, [fp, #-48]	@, %sfp
	ldr	r7, [fp, #-64]	@, %sfp
	ldr	r2, [fp, #-80]	@, %sfp
	ldr	ip, [fp, #-100]	@, %sfp
	mul	r5, r9, r7	@ tmp327,,
	ldr	r1, [fp, #-104]	@, %sfp
	ldr	r7, [fp, #-88]	@, %sfp
	add	r6, ip, r2	@,,
	add	r8, r1, r2	@,,
	str	r6, [fp, #-56]	@, %sfp
	str	r8, [fp, #-60]	@, %sfp
	add	r9, r7, r5, asl #1	@,,, tmp327,
	str	r9, [fp, #-52]	@, %sfp
.L102:
	ldr	r9, [fp, #-48]	@, %sfp
	cmp	r9, #0	@,
	ble	.L100	@,
	tst	r9, #1	@,
	ldr	r6, [fp, #-56]	@ ivtmp.430, %sfp
	ldr	r2, [fp, #-60]	@ ivtmp.434, %sfp
	mov	ip, #0	@ ch.620,
	ldr	r1, [fp, #-52]	@ ivtmp.658, %sfp
	movw	r9, #32767	@ tmp399,
	bne	.L101	@,
	ldr	r2, [r0, #32]	@ temp.611, <variable>.IvtSFFactor
	ldr	r6, [fp, #-56]	@, %sfp
	mul	r3, r2, r4	@ tmp657, temp.611, prephitmp.610
	ldrsh	r7, [r6, #0]	@ tmp656,
#APP
@ 584 "/home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/../../../source/msp/drv/adsp/adsp_v1_1/hifi/common/include/hi_dsp_basicop.h" 1
	smull r5,r3,r7,r3	@, sum.613, tmp656, sum.613
@ 0 "" 2
	ldr	r8, [fp, #-60]	@, %sfp
	rsb	r4, r4, sl	@ tmp661, prephitmp.610, temp.656
	mul	r5, r2, r4	@ tmp662, temp.611, tmp661
	ldrsh	ip, [r8, #0]	@ tmp660,
#APP
@ 584 "/home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/../../../source/msp/drv/adsp/adsp_v1_1/hifi/common/include/hi_dsp_basicop.h" 1
	smull r7,r5,ip,r5	@, y, tmp660, y
@ 0 "" 2
	add	sl, r5, r3	@ tmp665, y, sum.613
	mov	ip, #1	@ ch.620,
	mov	r6, sl, asl #2	@ x, tmp665,
	mov	r1, r6, asr #31	@ sign, x,
	cmp	r1, r6, asr #15	@ sign, x,
	eorne	r6, r1, r9	@ x, sign, tmp399
	ldr	r1, [fp, #-52]	@ ivtmp.658, %sfp
	strh	r6, [r1], #2	@ movhi	@ x,
	ldr	r7, [fp, #-56]	@, %sfp
	ldr	r8, [fp, #-60]	@, %sfp
	add	r6, r7, #2	@ ivtmp.430,,
	ldr	r4, [r0, #20]	@ prephitmp.610, <variable>.Remainder
	add	r2, r8, #2	@ ivtmp.434,,
	ldr	sl, [r0, #16]	@ temp.656, <variable>.OutRate
	b	.L101	@
.L98:
	ldr	r5, [r0, #20]	@ prephitmp.610, <variable>.Remainder
	ldr	r4, [r0, #32]	@ temp.611, <variable>.IvtSFFactor
	ldrsh	r6, [r6, #2]	@,
	mul	r3, r4, r5	@ tmp311, temp.611, prephitmp.610
	str	r6, [fp, #-112]	@, %sfp
#APP
@ 584 "/home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/../../../source/msp/drv/adsp/adsp_v1_1/hifi/common/include/hi_dsp_basicop.h" 1
	smull sl,r3,r6,r3	@, sum.613,, sum.613
@ 0 "" 2
	ldr	r6, [r0, #16]	@, <variable>.OutRate
	ldrsh	r2, [r2, #2]	@ tmp312,
	rsb	sl, r5, r6	@ tmp313, prephitmp.610,
	mul	r6, r4, sl	@ tmp315, temp.611, tmp313
#APP
@ 584 "/home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/../../../source/msp/drv/adsp/adsp_v1_1/hifi/common/include/hi_dsp_basicop.h" 1
	smull sl,r6,r2,r6	@, y, tmp312, y
@ 0 "" 2
	add	r3, r6, r3	@ tmp316, y, sum.613
	add	ip, ip, #1	@ ch.620, tmp465,
	add	r6, r8, #2	@ ivtmp.430, tmp466,
	mov	r3, r3, asl #2	@ x, tmp316,
	mov	r2, r3, asr #31	@ sign, x,
	cmp	r2, r3, asr #15	@ sign, x,
	eorne	r3, r2, r9	@ x, sign, tmp399
	strh	r3, [r1], #2	@ movhi	@ x,
	ldr	r4, [r0, #20]	@ prephitmp.610, <variable>.Remainder
	add	r2, r7, #2	@ ivtmp.434, tmp463,
	ldr	sl, [r0, #16]	@ temp.656, <variable>.OutRate
.L101:
	ldr	r5, [r0, #32]	@ temp.611, <variable>.IvtSFFactor
	ldrsh	r7, [r6, #0]	@ tmp555,* ivtmp.430
	mul	r3, r5, r4	@ tmp556, temp.611, prephitmp.610
#APP
@ 584 "/home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/../../../source/msp/drv/adsp/adsp_v1_1/hifi/common/include/hi_dsp_basicop.h" 1
	smull r8,r3,r7,r3	@, sum.613, tmp555, sum.613
@ 0 "" 2
	rsb	r4, r4, sl	@ tmp561, prephitmp.610, temp.656
	ldrsh	r7, [r2, #0]	@ tmp560,* ivtmp.434
	mul	r8, r5, r4	@ tmp562, temp.611, tmp561
#APP
@ 584 "/home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/../../../source/msp/drv/adsp/adsp_v1_1/hifi/common/include/hi_dsp_basicop.h" 1
	smull sl,r8,r7,r8	@, y, tmp560, y
@ 0 "" 2
	add	r7, r8, r3	@ tmp565, y, sum.613
	ldr	sl, [fp, #-48]	@, %sfp
	add	ip, ip, #1	@ tmp465, ch.620,
	add	r8, r6, #2	@ tmp466, ivtmp.430,
	mov	r3, r7, asl #2	@ x, tmp565,
	add	r7, r2, #2	@ tmp463, ivtmp.434,
	mov	r4, r3, asr #31	@ sign, x,
	cmp	r4, r3, asr #15	@ sign, x,
	eorne	r3, r4, r9	@ x, sign, tmp399
	cmp	ip, sl	@ tmp465,
	strh	r3, [r1], #2	@ movhi	@ x,
	bne	.L98	@,
	ldr	r4, [r0, #20]	@ prephitmp.610, <variable>.Remainder
	ldr	r3, [r0, #8]	@ <variable>.AdjustInRate, <variable>.AdjustInRate
	ldr	sl, [r0, #16]	@ temp.656, <variable>.OutRate
.L100:
	ldr	r8, [fp, #-68]	@, %sfp
	add	r4, r4, r3	@ prephitmp.610, prephitmp.610, <variable>.AdjustInRate
	ldr	r2, [fp, #-64]	@, %sfp
	cmp	r4, sl	@ prephitmp.610, temp.656
	add	r6, r8, #1	@,,
	str	r6, [fp, #-68]	@, %sfp
	add	r1, r2, #1	@,,
	str	r1, [fp, #-64]	@, %sfp
	str	r4, [r0, #20]	@ prephitmp.610, <variable>.Remainder
	ldr	ip, [fp, #-52]	@, %sfp
	ldr	r8, [fp, #-72]	@, %sfp
	add	r7, ip, r8	@,,
	str	r7, [fp, #-52]	@, %sfp
	blt	.L102	@,
.L99:
	ldr	r6, [fp, #-96]	@, %sfp
	rsb	r4, sl, r4	@ prephitmp.610, temp.656, prephitmp.610
	sub	r2, r6, #1	@,,
	str	r2, [fp, #-96]	@, %sfp
	str	r4, [r0, #20]	@ prephitmp.610, <variable>.Remainder
	cmp	r2, #1	@,
	ldr	ip, [fp, #-80]	@, %sfp
	ldr	r1, [fp, #-72]	@, %sfp
	add	r5, ip, r1	@,,
	str	r5, [fp, #-80]	@, %sfp
	bne	.L96	@,
	ldr	r5, [fp, #-76]	@, %sfp
	ldr	r4, [fp, #-84]	@, %sfp
	ldr	r6, [fp, #-48]	@ Channels, %sfp
	add	ip, r4, r5	@ D.4632,,
	sub	r3, ip, #1	@, D.4632,
	str	r3, [fp, #-76]	@, %sfp
.L89:
	cmp	r6, #0	@ Channels,
	ble	.L86	@,
	ldr	r1, [fp, #-76]	@, %sfp
	sub	r2, r6, #1	@ tmp454, Channels,
	ldr	r4, [fp, #-92]	@, %sfp
	mul	r3, r6, r1	@ tmp329, Channels,
	mov	r1, #1	@ ch.624,
	cmp	r1, r6	@ ch.624, Channels
	and	ip, r2, r1	@ tmp456, tmp454,
	add	r2, r0, #4	@ ivtmp.425, hLSRC,
	add	r3, r4, r3, asl #1	@, ivtmp.421,, tmp329,
	ldrsh	r4, [r3], #2	@ tmp331,
	str	r4, [r0, #36]	@ tmp331, <variable>.PreSample
	beq	.L86	@,
	cmp	ip, #0	@ tmp456,
	beq	.L103	@,
	ldrsh	ip, [r3], #2	@ tmp654,
	mov	r1, #2	@ ch.624,
	cmp	r1, r6	@ ch.624, Channels
	str	ip, [r2, #36]	@ tmp654, <variable>.PreSample
	add	r2, r2, #4	@ ivtmp.425, ivtmp.425,
	beq	.L86	@,
.L103:
	mov	r0, r3	@ tmp458, ivtmp.421
	add	r1, r1, #2	@ ch.624, ch.624,
	ldrsh	r4, [r0], #2	@ tmp574,
	add	ip, r2, #4	@ tmp459, ivtmp.425,
	cmp	r1, r6	@ ch.624, Channels
	str	r4, [r2, #36]	@ tmp574, <variable>.PreSample
	add	r2, ip, #4	@ ivtmp.425, tmp459,
	ldrsh	r4, [r3, #2]	@ tmp578,
	add	r3, r0, #2	@ ivtmp.421, tmp458,
	str	r4, [ip, #36]	@ tmp578, <variable>.PreSample
	bne	.L103	@,
	b	.L86	@
.L75:
	ldr	r3, [r0, #8]	@ <variable>.AdjustInRate, <variable>.AdjustInRate
	ldr	sl, [r0, #16]	@ temp.656, <variable>.OutRate
	cmp	r3, sl	@ <variable>.AdjustInRate, temp.656
	moveq	r2, r0	@ ivtmp.486, hLSRC
	moveq	r3, #0	@ ch,
	beq	.L78	@,
	cmp	r6, #0	@ Channels,
	ble	.L80	@,
	ldr	r8, [fp, #-92]	@, %sfp
	mov	r2, #1	@ ch.598,
	cmp	r2, r6	@ ch.598, Channels
	sub	r9, r6, #1	@ tmp474, Channels,
	and	ip, r9, r2	@ tmp476, tmp474,
	add	r1, r0, #4	@ ivtmp.498, hLSRC,
	ldrsh	r4, [r8, #0]	@ tmp288,
	str	r4, [r0, #36]	@ tmp288, <variable>.PreSample
	beq	.L80	@,
	cmp	ip, #0	@ tmp476,
	beq	.L219	@,
	ldr	r7, [fp, #-92]	@, %sfp
	mov	r2, #2	@ tmp687,
	cmp	r2, r6	@ ch.598, Channels
	ldrsh	r9, [r7, r2]	@ tmp688,
	str	r9, [r1, #36]	@ tmp688, <variable>.PreSample
	add	r1, r1, #4	@ ivtmp.498, ivtmp.498,
	beq	.L80	@,
.L219:
	ldr	r5, [fp, #-92]	@ pInPcmBuf, %sfp
.L82:
	mov	r7, r2, asl #1	@ tmp517, ch.598,
	add	r9, r2, #1	@ tmp477, ch.598,
	add	r2, r9, #1	@ ch.598, tmp477,
	add	ip, r1, #4	@ tmp478, ivtmp.498,
	ldrsh	r7, [r5, r7]	@ tmp518,
	mov	r4, r9, asl #1	@ tmp521, tmp477,
	cmp	r2, r6	@ ch.598, Channels
	str	r7, [r1, #36]	@ tmp518, <variable>.PreSample
	add	r1, ip, #4	@ ivtmp.498, tmp478,
	ldrsh	r4, [r5, r4]	@ tmp522,
	str	r4, [ip, #36]	@ tmp522, <variable>.PreSample
	bne	.L82	@,
.L80:
	ldr	ip, [fp, #-84]	@, %sfp
	mov	r1, #0	@ tmp289,
	mov	r2, #1	@,
	str	r1, [fp, #-64]	@ tmp289, %sfp
	sub	r9, ip, #1	@,,
	str	r2, [fp, #-76]	@, %sfp
	str	r9, [fp, #-84]	@, %sfp
	str	r1, [fp, #-68]	@ tmp289, %sfp
	str	r1, [r0, #24]	@ tmp289, <variable>.DisContinuity
	b	.L76	@
.L78:
	cmp	r6, r3	@ Channels, ch
	mov	r1, r3, asl #1	@ tmp284, ch,
	add	r2, r2, #4	@ ivtmp.486, ivtmp.486,
	add	r3, r3, #1	@ ch, ch,
	ble	.L231	@,
	ldrh	r4, [r2, #32]	@, <variable>.PreSample
	ldr	r5, [fp, #-88]	@, %sfp
	strh	r4, [r5, r1]	@ movhi	@,
	b	.L78	@
.L106:
	ldr	sl, [fp, #-48]	@, %sfp
	cmp	sl, #0	@,
	ble	.L109	@,
	ldr	r6, [r0, #36]	@ tmp343, <variable>.PreSample
	mov	r3, #1	@ ch.625,
	ldr	r4, [fp, #-88]	@, %sfp
	cmp	r3, sl	@ ch.625,
	sub	r2, sl, #1	@ tmp505,,
	and	r1, r2, r3	@ tmp507, tmp505,
	add	r2, r0, #4	@ ivtmp.590, hLSRC,
	str	r6, [r4, #0]	@ tmp343,
	beq	.L202	@,
	cmp	r1, #0	@ tmp507,
	beq	.L225	@,
	ldr	r9, [fp, #-48]	@, %sfp
	mov	r3, #2	@ ch.625,
	ldr	r8, [r2, #36]	@ tmp732, <variable>.PreSample
	add	r2, r2, #4	@ ivtmp.590, ivtmp.590,
	ldr	sl, [fp, #-88]	@, %sfp
	cmp	r3, r9	@ ch.625,
	str	r8, [sl, #4]	@ tmp732,
	beq	.L202	@,
.L225:
	ldr	r4, [fp, #-88]	@ pOutpcmBuf, %sfp
.L110:
	ldr	r5, [r2, #36]	@ tmp584, <variable>.PreSample
	add	r8, r2, #4	@ tmp509, ivtmp.590,
	add	r1, r3, #1	@ tmp508, ch.625,
	add	r2, r8, #4	@ ivtmp.590, tmp509,
	str	r5, [r4, r3, asl #2]	@ tmp584,* pOutpcmBuf
	add	r3, r1, #1	@ ch.625, tmp508,
	ldr	r5, [fp, #-48]	@, %sfp
	ldr	ip, [r8, #36]	@ tmp588, <variable>.PreSample
	cmp	r3, r5	@ ch.625,
	str	ip, [r4, r1, asl #2]	@ tmp588,* pOutpcmBuf
	bne	.L110	@,
.L202:
	ldr	r3, [r0, #8]	@ <variable>.AdjustInRate, <variable>.AdjustInRate
	ldr	r8, [r0, #16]	@ temp.656, <variable>.OutRate
.L109:
	mov	r2, #0	@ tmp344,
	mov	r6, #1	@,
	str	r6, [fp, #-64]	@, %sfp
	str	r2, [r0, #24]	@ tmp344, <variable>.DisContinuity
	str	r2, [fp, #-96]	@ tmp344, %sfp
	str	r6, [fp, #-68]	@, %sfp
	b	.L105	@
.L230:
	ldr	r7, [fp, #-76]	@, %sfp
	mov	ip, r6, asl #1	@ D.4664, Channels,
	ldr	r3, [fp, #-64]	@, %sfp
	ldr	sl, [fp, #-92]	@, %sfp
	mul	r5, r6, r7	@ tmp291, Channels,
	ldr	r8, [fp, #-88]	@, %sfp
	mul	r0, r6, r3	@ tmp293, Channels,
	add	r5, sl, r5, asl #1	@, ivtmp.403,, tmp291,
	ldr	sl, [fp, #-68]	@ outsampls, %sfp
	add	r7, r8, r0, asl #1	@, ivtmp.657,, tmp293,
	ldr	r8, [fp, #-84]	@ InSamps, %sfp
.L84:
	cmp	r8, #0	@ InSamps,
	ble	.L207	@,
	cmp	r6, #0	@ Channels,
	ble	.L88	@,
	sub	r4, r6, #1	@ tmp448, Channels,
	mov	r2, r5	@ ivtmp.629, ivtmp.403
	mov	r1, #1	@ ch.602,
	and	r0, r4, r1	@ tmp450, tmp448,
	ldrh	r4, [r2], #2	@,
	cmp	r1, r6	@ ch.602, Channels
	mov	r3, r7	@ ivtmp.405, ivtmp.657
	strh	r4, [r3], #2	@ movhi	@,
	beq	.L88	@,
	cmp	r0, #0	@ tmp450,
	beq	.L85	@,
	ldrh	r9, [r2], #2	@,
	mov	r1, #2	@ ch.602,
	cmp	r1, r6	@ ch.602, Channels
	strh	r9, [r3], #2	@ movhi	@,
	beq	.L88	@,
.L85:
	mov	r4, r2	@ tmp452, ivtmp.629
	mov	r0, r3	@ tmp453, ivtmp.405
	ldrh	r9, [r4], #2	@,
	add	r1, r1, #2	@ ch.602, ch.602,
	cmp	r1, r6	@ ch.602, Channels
	strh	r9, [r0], #2	@ movhi	@,
	ldrh	r2, [r2, #2]	@,
	strh	r2, [r3, #2]	@ movhi	@,
	add	r2, r4, #2	@ ivtmp.629, tmp452,
	add	r3, r0, #2	@ ivtmp.405, tmp453,
	bne	.L85	@,
.L88:
	add	sl, sl, #1	@ outsampls, outsampls,
	sub	r8, r8, #1	@ InSamps, InSamps,
	add	r5, r5, ip	@ ivtmp.403, ivtmp.403, D.4664
	add	r7, r7, ip	@ ivtmp.657, ivtmp.657, D.4664
	b	.L84	@
.L207:
	str	sl, [fp, #-68]	@ outsampls, %sfp
	b	.L86	@
.L231:
	mov	ip, #0	@ tmp286,
	mov	r7, #1	@,
	str	r7, [fp, #-64]	@, %sfp
	str	ip, [r0, #24]	@ tmp286, <variable>.DisContinuity
	str	ip, [fp, #-76]	@ tmp286, %sfp
	str	r7, [fp, #-68]	@, %sfp
	ldr	r3, [r0, #8]	@ <variable>.AdjustInRate, <variable>.AdjustInRate
	ldr	sl, [r0, #16]	@ temp.656, <variable>.OutRate
	b	.L76	@
	.fnend
	.size	LSRC_ProcessFrame, .-LSRC_ProcessFrame
	.align	2
	.global	LSRC_Destroy
	.type	LSRC_Destroy, %function
LSRC_Destroy:
	.fnstart
.LFB143:
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
	subs	r1, r0, #0	@ hLSRC, hLSRC
	ldmeqfd	sp, {fp, sp, pc}	@
	mov	r0, #1	@,
	bl	HI_FREE	@
	ldmfd	sp, {fp, sp, pc}	@
	.fnend
	.size	LSRC_Destroy, .-LSRC_Destroy
	.align	2
	.global	LSRC_Create
	.type	LSRC_Create, %function
LSRC_Create:
	.fnstart
.LFB142:
	@ args = 4, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	.movsp ip
	mov	ip, sp	@,
	stmfd	sp!, {r3, r4, r5, r6, r7, r8, r9, sl, fp, ip, lr, pc}	@,
	.pad #4
	.save {r3, r4, r5, r6, r7, r8, r9, sl, fp, ip, lr}
	.setfp fp, ip, #-4
	sub	fp, ip, #4	@,,
	push	{lr}
	bl	__gnu_mcount_nc
	movw	ip, #:lower16:191999	@ tmp146,
	sub	lr, r1, #1	@ tmp145, inRate,
	movt	ip, #:upper16:191999	@ tmp146,
	cmp	lr, ip	@ tmp145, tmp146
	mov	r8, #0	@ tmp144,
	str	r8, [r0, #0]	@ tmp144,* phLSRC
	mov	r4, r1	@ inRate, inRate
	mov	r5, r0	@ phLSRC, phLSRC
	mov	r6, r2	@ outRate, outRate
	mov	sl, r3	@ BitPerSample, BitPerSample
	ldr	r7, [fp, #4]	@ Channels, Channels
	bhi	.L264	@,
	sub	r3, r2, #1	@ tmp148, outRate,
	cmp	r3, ip	@ tmp148, tmp146
	bhi	.L265	@,
	mov	r0, r2	@, outRate
	bl	__aeabi_idiv	@
	cmp	r0, #6	@,
	bgt	.L266	@,
	mov	r0, #1	@,
	mov	r1, #76	@,
	bl	HI_MALLOC	@
	subs	r9, r0, #0	@ pLinearSrc,
	beq	.L267	@,
	mov	r1, #76	@,
	bl	__memzero	@
	cmp	r6, #8000	@ outRate,
	beq	.L242	@,
	movw	r0, #11025	@ tmp157,
	cmp	r6, r0	@ outRate, tmp157
	moveq	r8, #1	@ n.715,
	beq	.L242	@,
	movw	r1, #12000	@ tmp158,
	cmp	r6, r1	@ outRate, tmp158
	moveq	r8, #2	@ n.715,
	beq	.L242	@,
	cmp	r6, #16000	@ outRate,
	moveq	r8, #3	@ n.715,
	beq	.L242	@,
	movw	r2, #22050	@ tmp159,
	cmp	r6, r2	@ outRate, tmp159
	moveq	r8, #4	@ n.715,
	beq	.L242	@,
	movw	ip, #24000	@ tmp160,
	cmp	r6, ip	@ outRate, tmp160
	moveq	r8, #5	@ n.715,
	beq	.L242	@,
	cmp	r6, #32000	@ outRate,
	moveq	r8, #6	@ n.715,
	beq	.L242	@,
	movw	lr, #44100	@ tmp161,
	cmp	r6, lr	@ outRate, tmp161
	moveq	r8, #7	@ n.715,
	beq	.L242	@,
	movw	r3, #48000	@ tmp162,
	cmp	r6, r3	@ outRate, tmp162
	moveq	r8, #8	@ n.715,
	beq	.L242	@,
	movw	r0, #:lower16:88200	@ tmp163,
	movt	r0, #:upper16:88200	@ tmp163,
	cmp	r6, r0	@ outRate, tmp163
	moveq	r8, #9	@ n.715,
	beq	.L242	@,
	movw	r1, #:lower16:96000	@ tmp164,
	movt	r1, #:upper16:96000	@ tmp164,
	cmp	r6, r1	@ outRate, tmp164
	moveq	r8, #10	@ n.715,
	beq	.L242	@,
	movw	r2, #:lower16:176400	@ tmp165,
	movt	r2, #:upper16:176400	@ tmp165,
	cmp	r6, r2	@ outRate, tmp165
	moveq	r8, #11	@ n.715,
	beq	.L242	@,
	movw	ip, #:lower16:192000	@ tmp166,
	movt	ip, #:upper16:192000	@ tmp166,
	cmp	r6, ip	@ outRate, tmp166
	beq	.L268	@,
.L254:
	cmp	r7, #0	@ Channels,
	beq	.L255	@,
	mov	r3, #1	@ n,
	cmp	r3, r7	@ n, Channels
	sub	r2, r7, #1	@ tmp171, Channels,
	mov	r0, #0	@ tmp170,
	and	r1, r2, r3	@ tmp173, tmp171,
	str	r0, [r9, #36]	@ tmp170, <variable>.PreSample
	add	r2, r9, #4	@ ivtmp.713, pLinearSrc,
	bcs	.L255	@,
	cmp	r1, #0	@ tmp173,
	beq	.L256	@,
	mov	r3, #2	@ n,
	cmp	r3, r7	@ n, Channels
	str	r0, [r2, #36]	@ tmp170, <variable>.PreSample
	add	r2, r2, #4	@ ivtmp.713, ivtmp.713,
	bcs	.L255	@,
.L256:
	add	r3, r3, #2	@ n, n,
	add	r1, r2, #4	@ tmp175, ivtmp.713,
	cmp	r3, r7	@ n, Channels
	str	r0, [r2, #36]	@ tmp170, <variable>.PreSample
	str	r0, [r1, #36]	@ tmp170, <variable>.PreSample
	add	r2, r1, #4	@ ivtmp.713, tmp175,
	bcc	.L256	@,
.L255:
	str	sl, [r9, #0]	@ BitPerSample, <variable>.BitPerSample
	str	r7, [r9, #4]	@ Channels, <variable>.Channels
	str	r4, [r9, #12]	@ inRate, <variable>.InRate
	str	r6, [r9, #16]	@ outRate, <variable>.OutRate
	str	r4, [r9, #8]	@ inRate, <variable>.AdjustInRate
	str	r9, [r5, #0]	@ pLinearSrc,* phLSRC
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, sl, fp, sp, pc}	@
.L268:
	mov	r8, #12	@ n.715,
.L242:
	movw	r3, #:lower16:.LANCHOR0	@ tmp167,
	movt	r3, #:upper16:.LANCHOR0	@ tmp167,
	ldr	lr, [r3, r8, asl #2]	@ tmp168, g_s32IvtSfTab
	str	lr, [r9, #32]	@ tmp168, <variable>.IvtSFFactor
	b	.L254	@
.L267:
	movw	r0, #:lower16:.LC2	@,
	movt	r0, #:upper16:.LC2	@,
	bl	printk	@
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, sl, fp, sp, pc}	@
.L265:
	mov	r1, r2	@, outRate
.L264:
	movw	r0, #:lower16:.LC0	@,
	movt	r0, #:upper16:.LC0	@,
	bl	printk	@
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, sl, fp, sp, pc}	@
.L266:
	mov	r1, r4	@, inRate
	mov	r2, r6	@, outRate
	movw	r0, #:lower16:.LC1	@,
	movt	r0, #:upper16:.LC1	@,
	bl	printk	@
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, sl, fp, sp, pc}	@
	.fnend
	.size	LSRC_Create, .-LSRC_Create
	.section	.rodata
	.align	2
.LANCHOR0 = . + 0
	.type	g_s32IvtSfTab, %object
	.size	g_s32IvtSfTab, 52
g_s32IvtSfTab:
	.word	134217
	.word	97391
	.word	89478
	.word	67109
	.word	48696
	.word	44739
	.word	33554
	.word	24348
	.word	22369
	.word	12174
	.word	11185
	.word	6087
	.word	5592
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
.LC0:
	.ascii	"invalid inRate(%d)\012\000"
.LC1:
	.ascii	"inRate(%d),outRated(%d)\012\000"
	.space	3
.LC2:
	.ascii	"HI_MALLOC SRC_linear failure!\012\000"
	.ident	"GCC: (Hisilicon_v200(gcc4.4-290+glibc-2.11+eabi+nptl)) 4.4.1"
	.section	.note.GNU-stack,"",%progbits

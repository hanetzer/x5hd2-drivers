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
	.file	"dsp_mem.c"
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
@ -DKBUILD_STR(s)=#s -DKBUILD_BASENAME=KBUILD_STR(dsp_mem)
@ -DKBUILD_MODNAME=KBUILD_STR(hi_adsp) -isystem
@ /opt/hisi-linux/x86-arm/arm-hisiv200-linux/bin/../lib/gcc/arm-hisiv200-linux-gnueabi/4.4.1/include
@ -include
@ /home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/include/linux/kconfig.h
@ -MD drivers/msp/adsp/adsp_v1_1/./hifi/common/mem/.dsp_mem.o.d
@ /home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/drivers/msp/adsp/adsp_v1_1/./hifi/common/mem/dsp_mem.c
@ -mlittle-endian -marm -mapcs -mno-sched-prolog -mabi=aapcs-linux
@ -mno-thumb-interwork -march=armv7-a -msoft-float -auxbase-strip
@ drivers/msp/adsp/adsp_v1_1/./hifi/common/mem/dsp_mem.o -O2 -Wall -Wundef
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
	.global	HI_V_FREE
	.type	HI_V_FREE, %function
HI_V_FREE:
	.fnstart
.LFB581:
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
	subs	r0, r1, #0	@ pMemAddr, pMemAddr
	ldmeqfd	sp, {fp, sp, pc}	@
	bl	vfree	@
	ldmfd	sp, {fp, sp, pc}	@
	.fnend
	.size	HI_V_FREE, .-HI_V_FREE
	.align	2
	.global	HI_V_FREE_Aligned
	.type	HI_V_FREE_Aligned, %function
HI_V_FREE_Aligned:
	.fnstart
.LFB586:
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
	cmp	r1, #0	@ pMemAddr
	ldmeqfd	sp, {fp, sp, pc}	@
	ldrb	r3, [r1, #-1]	@ zero_extendqisi2	@ tmp137,
	rsb	r1, r3, r1	@, tmp137, pMemAddr
	bl	HI_V_FREE	@
	ldmfd	sp, {fp, sp, pc}	@
	.fnend
	.size	HI_V_FREE_Aligned, .-HI_V_FREE_Aligned
	.align	2
	.global	HI_V_MALLOC
	.type	HI_V_MALLOC, %function
HI_V_MALLOC:
	.fnstart
.LFB580:
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
	mov	r0, r1	@, u32Size
	bl	vmalloc	@
	ldmfd	sp, {fp, sp, pc}	@
	.fnend
	.size	HI_V_MALLOC, .-HI_V_MALLOC
	.global	__aeabi_uidivmod
	.align	2
	.global	HI_V_MALLOC_Aligned
	.type	HI_V_MALLOC_Aligned, %function
HI_V_MALLOC_Aligned:
	.fnstart
.LFB585:
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
	cmp	r2, #3	@ iAlignToBytes,
	mov	r5, r2	@ iAlignToBytes, iAlignToBytes
	mov	r8, r0	@ u32ModuleID, u32ModuleID
	mov	r6, r1	@ u32Size, u32Size
	bls	.L20	@,
	cmp	r2, #256	@ iAlignToBytes,
	movls	r3, #0	@ iLog2,
	bhi	.L21	@,
.L14:
	add	r3, r3, #1	@ iLog2, iLog2,
	mov	r2, r5, lsr r3	@ tmp145, iAlignToBytes,
	cmp	r2, #1	@ tmp145,
	bhi	.L14	@,
	mvn	r7, #0	@ tmp146,
	mov	r7, r7, asl r3	@ mask, tmp146,
	bics	r4, r5, r7	@ tmp148, iAlignToBytes, mask
	bne	.L22	@,
	cmp	r5, r6	@ iAlignToBytes, u32Size
	bcs	.L16	@,
	mov	r0, r6	@, u32Size
	mov	r1, r5	@, iAlignToBytes
	bl	__aeabi_uidivmod	@
	cmp	r1, #0	@,
	bne	.L23	@,
.L16:
	add	r6, r5, r6	@ D.10744, iAlignToBytes, u32Size
	mov	r0, r8	@, u32ModuleID
	mov	r1, r6	@, D.10744
	bl	HI_V_MALLOC	@
	subs	r4, r0, #0	@ buffer,
	beq	.L24	@,
	add	r5, r4, r5	@ tmp158, buffer, iAlignToBytes
	and	r0, r7, r5	@ retBuffer, mask, tmp158
	rsb	r4, r4, r0	@ tmp161, buffer, retBuffer
	strb	r4, [r0, #-1]	@ tmp161,
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, fp, sp, pc}	@
.L22:
	movw	r0, #:lower16:.LC1	@,
	mov	r1, r5	@, iAlignToBytes
	movt	r0, #:upper16:.LC1	@,
	bl	printk	@
.L20:
	mov	r0, #0	@ retBuffer,
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, fp, sp, pc}	@
.L23:
	mov	r1, r6	@, u32Size
	mov	r2, r5	@, iAlignToBytes
	movw	r0, #:lower16:.LC2	@,
	movt	r0, #:upper16:.LC2	@,
	bl	printk	@
	mov	r0, r4	@ retBuffer, tmp148
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, fp, sp, pc}	@
.L24:
	mov	r1, r6	@, D.10744
	movw	r0, #:lower16:.LC3	@,
	movt	r0, #:upper16:.LC3	@,
	bl	printk	@
	mov	r0, r4	@ retBuffer, buffer
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, fp, sp, pc}	@
.L21:
	mov	r1, r2	@, iAlignToBytes
	movw	r0, #:lower16:.LC0	@,
	movt	r0, #:upper16:.LC0	@,
	bl	printk	@
	mov	r0, #0	@ retBuffer,
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, fp, sp, pc}	@
	.fnend
	.size	HI_V_MALLOC_Aligned, .-HI_V_MALLOC_Aligned
	.align	2
	.global	HI_REALLOC
	.type	HI_REALLOC, %function
HI_REALLOC:
	.fnstart
.LFB579:
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
	mov	r0, r1	@, pMemAddr
	mov	r1, r2	@, u32Size
	mov	r2, #208	@,
	bl	krealloc	@
	ldmfd	sp, {fp, sp, pc}	@
	.fnend
	.size	HI_REALLOC, .-HI_REALLOC
	.align	2
	.global	HI_CALLOC
	.type	HI_CALLOC, %function
HI_CALLOC:
	.fnstart
.LFB578:
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
	mul	r4, r1, r2	@ D.10632, u32MemBlock, u32Size
	mov	r1, #208	@,
	mov	r0, r4	@, D.10632
	bl	__kmalloc	@
	cmp	r4, #0	@ D.10632,
	mov	r5, r0	@ pMemAddr,
	bne	.L30	@,
	mov	r0, r5	@, pMemAddr
	ldmfd	sp, {r4, r5, fp, sp, pc}	@
.L30:
	mov	r1, r4	@, D.10632
	bl	__memzero	@
	mov	r0, r5	@, pMemAddr
	ldmfd	sp, {r4, r5, fp, sp, pc}	@
	.fnend
	.size	HI_CALLOC, .-HI_CALLOC
	.align	2
	.global	HI_MALLOC
	.type	HI_MALLOC, %function
HI_MALLOC:
	.fnstart
.LFB576:
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
	mov	r0, r1	@, u32Size
	mov	r1, #208	@,
	bl	__kmalloc	@
	ldmfd	sp, {fp, sp, pc}	@
	.fnend
	.size	HI_MALLOC, .-HI_MALLOC
	.align	2
	.global	HI_MALLOC_Aligned
	.type	HI_MALLOC_Aligned, %function
HI_MALLOC_Aligned:
	.fnstart
.LFB583:
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
	cmp	r2, #3	@ iAlignToBytes,
	mov	r5, r2	@ iAlignToBytes, iAlignToBytes
	mov	r8, r0	@ u32ModuleID, u32ModuleID
	mov	r6, r1	@ u32Size, u32Size
	bls	.L43	@,
	cmp	r2, #256	@ iAlignToBytes,
	movls	r3, #0	@ iLog2,
	bhi	.L44	@,
.L37:
	add	r3, r3, #1	@ iLog2, iLog2,
	mov	r2, r5, lsr r3	@ tmp144, iAlignToBytes,
	cmp	r2, #1	@ tmp144,
	bhi	.L37	@,
	mvn	r7, #0	@ tmp145,
	mov	r7, r7, asl r3	@ mask, tmp145,
	bics	r4, r5, r7	@ tmp147, iAlignToBytes, mask
	bne	.L45	@,
	cmp	r5, r6	@ iAlignToBytes, u32Size
	bcs	.L39	@,
	mov	r0, r6	@, u32Size
	mov	r1, r5	@, iAlignToBytes
	bl	__aeabi_uidivmod	@
	cmp	r1, #0	@,
	bne	.L46	@,
.L39:
	add	r6, r5, r6	@ D.10694, iAlignToBytes, u32Size
	mov	r0, r8	@, u32ModuleID
	mov	r1, r6	@, D.10694
	bl	HI_MALLOC	@
	subs	r4, r0, #0	@ buffer,
	beq	.L47	@,
	add	r5, r4, r5	@ tmp157, buffer, iAlignToBytes
	and	r0, r7, r5	@ retBuffer, mask, tmp157
	rsb	r4, r4, r0	@ tmp160, buffer, retBuffer
	strb	r4, [r0, #-1]	@ tmp160,
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, fp, sp, pc}	@
.L45:
	movw	r0, #:lower16:.LC1	@,
	mov	r1, r5	@, iAlignToBytes
	movt	r0, #:upper16:.LC1	@,
	bl	printk	@
.L43:
	mov	r0, #0	@ retBuffer,
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, fp, sp, pc}	@
.L46:
	mov	r1, r6	@, u32Size
	mov	r2, r5	@, iAlignToBytes
	movw	r0, #:lower16:.LC2	@,
	movt	r0, #:upper16:.LC2	@,
	bl	printk	@
	mov	r0, r4	@ retBuffer, tmp147
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, fp, sp, pc}	@
.L47:
	mov	r1, r6	@, D.10694
	movw	r0, #:lower16:.LC4	@,
	movt	r0, #:upper16:.LC4	@,
	bl	printk	@
	mov	r0, r4	@ retBuffer, buffer
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, fp, sp, pc}	@
.L44:
	mov	r1, r2	@, iAlignToBytes
	movw	r0, #:lower16:.LC0	@,
	movt	r0, #:upper16:.LC0	@,
	bl	printk	@
	mov	r0, #0	@ retBuffer,
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, fp, sp, pc}	@
	.fnend
	.size	HI_MALLOC_Aligned, .-HI_MALLOC_Aligned
	.align	2
	.global	HI_FREE
	.type	HI_FREE, %function
HI_FREE:
	.fnstart
.LFB577:
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
	subs	r0, r1, #0	@ pMemAddr, pMemAddr
	ldmeqfd	sp, {fp, sp, pc}	@
	bl	kfree	@
	ldmfd	sp, {fp, sp, pc}	@
	.fnend
	.size	HI_FREE, .-HI_FREE
	.align	2
	.global	HI_FREE_Aligned
	.type	HI_FREE_Aligned, %function
HI_FREE_Aligned:
	.fnstart
.LFB584:
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
	cmp	r1, #0	@ pMemAddr
	ldmeqfd	sp, {fp, sp, pc}	@
	ldrb	r3, [r1, #-1]	@ zero_extendqisi2	@ tmp137,
	rsb	r1, r3, r1	@, tmp137, pMemAddr
	bl	HI_FREE	@
	ldmfd	sp, {fp, sp, pc}	@
	.fnend
	.size	HI_FREE_Aligned, .-HI_FREE_Aligned
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
.LC0:
	.ascii	"iAlignToBytes(0x%x) should less than 256\012\000"
	.space	2
.LC1:
	.ascii	"iAlignToBytes(0x%x) should be power of 2\012\000"
	.space	2
.LC2:
	.ascii	" Size(0x%x) should align to(0x%x) \012\000"
.LC3:
	.ascii	"vmalloc(0x%x) failed \012\000"
	.space	1
.LC4:
	.ascii	"kmalloc(0x%x) failed \012\000"
	.ident	"GCC: (Hisilicon_v200(gcc4.4-290+glibc-2.11+eabi+nptl)) 4.4.1"
	.section	.note.GNU-stack,"",%progbits

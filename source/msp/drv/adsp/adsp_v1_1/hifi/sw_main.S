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
	.file	"sw_main.c"
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
@ -DKBUILD_STR(s)=#s -DKBUILD_BASENAME=KBUILD_STR(sw_main)
@ -DKBUILD_MODNAME=KBUILD_STR(hi_adsp) -isystem
@ /opt/hisi-linux/x86-arm/arm-hisiv200-linux/bin/../lib/gcc/arm-hisiv200-linux-gnueabi/4.4.1/include
@ -include
@ /home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/include/linux/kconfig.h
@ -MD drivers/msp/adsp/adsp_v1_1/./hifi/.sw_main.o.d
@ /home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/drivers/msp/adsp/adsp_v1_1/./hifi/sw_main.c
@ -mlittle-endian -marm -mapcs -mno-sched-prolog -mabi=aapcs-linux
@ -mno-thumb-interwork -march=armv7-a -msoft-float -auxbase-strip
@ drivers/msp/adsp/adsp_v1_1/./hifi/sw_main.o -O2 -Wall -Wundef
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
	.global	AOE_SwEngineDestory
	.type	AOE_SwEngineDestory, %function
AOE_SwEngineDestory:
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
	movw	r3, #:lower16:.LANCHOR0	@ tmp134,
	movt	r3, #:upper16:.LANCHOR0	@ tmp134,
	mov	r4, #0	@ tmp135,
	mov	r0, #10	@,
	str	r4, [r3, #0]	@ tmp135, g_AoeTaskstate
	bl	msleep	@
	mov	r0, r4	@, tmp135
	ldmfd	sp, {r3, r4, fp, sp, pc}	@
	.fnend
	.size	AOE_SwEngineDestory, .-AOE_SwEngineDestory
	.align	2
	.global	AOE_SwEngineCreate
	.type	AOE_SwEngineCreate, %function
AOE_SwEngineCreate:
	.fnstart
.LFB763:
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
	mov	r5, #0	@ tmp140,
	movw	r4, #:lower16:.LANCHOR0	@ tmp148,
	movt	r4, #:upper16:.LANCHOR0	@ tmp148,
	mov	r1, r5	@, tmp140
	mvn	r2, #0	@,
	movw	r0, #:lower16:AoEngineTask	@,
	movw	r3, #:lower16:.LC0	@,
	movt	r0, #:upper16:AoEngineTask	@,
	movt	r3, #:upper16:.LC0	@,
	mov	ip, #1	@ tmp138,
	str	r5, [r4, #4]	@ tmp140, g_AoeRunningFlag
	str	ip, [r4, #0]	@ tmp138, g_AoeTaskstate
	bl	kthread_create_on_node	@
	movw	r6, #:lower16:-1515870811	@ tmp150,
	movt	r6, #:upper16:-1515870811	@ tmp150,
	str	r0, [r4, #8]	@, g_pstAOEThread
	bl	wake_up_process	@
	b	.L5	@
.L11:
	bl	msleep	@
	ldr	r1, [r4, #4]	@ g_AoeRunningFlag, g_AoeRunningFlag
	mov	r0, #1	@,
	cmp	r1, r6	@ g_AoeRunningFlag, tmp150
	beq	.L4	@,
	bl	msleep	@
	cmp	r5, #512	@ cnt,
	beq	.L4	@,
.L5:
	ldr	r3, [r4, #4]	@ g_AoeRunningFlag, g_AoeRunningFlag
	mov	r0, #1	@,
	add	r5, r5, r0	@ tmp152, cnt,
	cmp	r3, r6	@ g_AoeRunningFlag, tmp150
	add	r5, r5, r0	@ cnt, tmp152,
	bne	.L11	@,
.L4:
	mov	r0, #0	@,
	ldmfd	sp, {r3, r4, r5, r6, fp, sp, pc}	@
	.fnend
	.size	AOE_SwEngineCreate, .-AOE_SwEngineCreate
	.align	2
	.type	AoEngineTask, %function
AoEngineTask:
	.fnstart
.LFB762:
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
	mov	ip, sp	@,
	bic	r0, ip, #8128	@ tmp144,,
	sub	r2, fp, #20	@ tmp147,,
	bic	r4, r0, #63	@ tmp144, tmp144,
	mov	lr, #99	@ tmp143,
	mov	r1, #1	@,
	str	lr, [r2, #-4]!	@ tmp143, param.sched_priority
	ldr	r0, [r4, #12]	@, <variable>.task
	bl	sched_setscheduler	@
	ldr	r3, [r4, #12]	@ D.14139, <variable>.task
	mov	r0, #1	@ tmp154,
	ldr	r1, [r3, #12]	@ <variable>.flags, <variable>.flags
	orr	r2, r1, #32768	@ tmp151, <variable>.flags,
	str	r2, [r3, #12]	@ tmp151, <variable>.flags
	ldr	ip, [r4, #12]	@ D.14145, <variable>.task
	str	r0, [ip, #0]	@ tmp154, <variable>.state
#APP
@ 97 "/home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/drivers/msp/adsp/adsp_v1_1/./hifi/sw_main.c" 1
	dmb
@ 0 "" 2
	movw	r4, #:lower16:.LANCHOR0	@ tmp168,
	movt	r4, #:upper16:.LANCHOR0	@ tmp168,
	bl	AOE_Init	@
	movw	r3, #:lower16:-1515870811	@ tmp158,
	ldr	r2, [r4, #0]	@ g_AoeTaskstate, g_AoeTaskstate
	mov	r1, #0	@ tmp156,
	movt	r3, #:upper16:-1515870811	@ tmp158,
	str	r1, [r4, #12]	@ tmp156, task_bug
	cmp	r2, #1	@ g_AoeTaskstate,
	str	r3, [r4, #4]	@ tmp158, g_AoeRunningFlag
	bne	.L13	@,
.L16:
	bl	AOE_ProcThread	@
	mov	r0, #3	@,
	bl	msecs_to_jiffies	@
	bl	schedule_timeout_interruptible	@
	ldr	r2, [r4, #0]	@ g_AoeTaskstate, g_AoeTaskstate
	cmp	r2, #1	@ g_AoeTaskstate,
	beq	.L16	@,
.L13:
	mov	r4, sp	@,
	bic	lr, r4, #8128	@ tmp163,,
	bic	r1, lr, #63	@ tmp163, tmp163,
	mov	r4, #0	@ tmp165,
	ldr	r3, [r1, #12]	@ D.14151, <variable>.task
	str	r4, [r3, #0]	@ tmp165, <variable>.state
#APP
@ 150 "/home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/drivers/msp/adsp/adsp_v1_1/./hifi/sw_main.c" 1
	dmb
@ 0 "" 2
	bl	AOE_DeInit	@
	mov	r0, r4	@, tmp165
	sub	sp, fp, #16
	ldmfd	sp, {r4, fp, sp, pc}
	.fnend
	.size	AoEngineTask, .-AoEngineTask
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
.LC0:
	.ascii	"AoEngineTask\000"
	.bss
	.align	2
.LANCHOR0 = . + 0
	.type	g_AoeTaskstate, %object
	.size	g_AoeTaskstate, 4
g_AoeTaskstate:
	.space	4
	.type	g_AoeRunningFlag, %object
	.size	g_AoeRunningFlag, 4
g_AoeRunningFlag:
	.space	4
	.type	g_pstAOEThread, %object
	.size	g_pstAOEThread, 4
g_pstAOEThread:
	.space	4
	.type	task_bug.14085, %object
	.size	task_bug.14085, 4
task_bug.14085:
	.space	4
	.ident	"GCC: (Hisilicon_v200(gcc4.4-290+glibc-2.11+eabi+nptl)) 4.4.1"
	.section	.note.GNU-stack,"",%progbits

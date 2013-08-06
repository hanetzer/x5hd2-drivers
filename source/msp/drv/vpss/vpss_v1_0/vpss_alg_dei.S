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
	.file	"vpss_alg_dei.c"
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
@ -DKBUILD_STR(s)=#s -DKBUILD_BASENAME=KBUILD_STR(vpss_alg_dei)
@ -DKBUILD_MODNAME=KBUILD_STR(hi_vpss) -isystem
@ /opt/hisi-linux/x86-arm/arm-hisiv200-linux/bin/../lib/gcc/arm-hisiv200-linux-gnueabi/4.4.1/include
@ -include
@ /home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/include/linux/kconfig.h
@ -MD drivers/msp/vpss/vpss_v1_0/.vpss_alg_dei.o.d
@ /home/release/HiSTBLinuxV100R002C00SPC011/source/kernel/linux-3.4.y/drivers/msp/vpss/vpss_v1_0/vpss_alg_dei.c
@ -mlittle-endian -marm -mapcs -mno-sched-prolog -mabi=aapcs-linux
@ -mno-thumb-interwork -march=armv7-a -msoft-float -auxbase-strip
@ drivers/msp/vpss/vpss_v1_0/vpss_alg_dei.o -O2 -Wall -Wundef
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
	.global	MadThdParaInit
	.type	MadThdParaInit, %function
MadThdParaInit:
	.fnstart
.LFB1233:
	@ args = 0, pretend = 0, frame = 0
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
	mov	r3, #0	@ tmp134,
	mov	r2, #8	@ tmp141,
	mov	r1, #1	@ tmp162,
	mov	ip, #2	@ tmp138,
	mov	r4, #4	@ tmp163,
	mov	r5, #6	@ tmp158,
	mov	r8, #5	@ tmp159,
	mov	r7, #192	@ tmp169,
	mov	r9, #64	@ tmp200,
	mov	r6, #128	@ tmp202,
	mov	sl, #30	@,
	str	r3, [r0, #0]	@ tmp134, <variable>.stMadLma2.s32SadThd
	str	r3, [r0, #8]	@ tmp134, <variable>.stMadLma2.bMfMaxLum
	str	r3, [r0, #4]	@ tmp134, <variable>.stMadLma2.bMfMaxChr
	str	r3, [r0, #12]	@ tmp134, <variable>.stMadLma2.bSceSdfMaxLum
	str	ip, [r0, #20]	@ tmp138, <variable>.stMadInten.s32DirIntenVer
	str	ip, [r0, #24]	@ tmp138, <variable>.stMadScl.s32RangeScale
	str	r2, [r0, #28]	@ tmp141, <variable>.stMadChk1.s32CkGain
	str	ip, [r0, #32]	@ tmp138, <variable>.stMadChk1.s32CkRangeGain
	str	r2, [r0, #40]	@ tmp141, <variable>.stMadChk2.s32CkGain
	str	ip, [r0, #44]	@ tmp138, <variable>.stMadChk2.s32CkRangeGain
	str	r2, [r0, #88]	@ tmp141, <variable>.stMadDirMult.s32MultDir
	str	r5, [r0, #96]	@ tmp158, <variable>.stMadDirMult.s32MultDir
	str	sl, [r0, #36]	@, <variable>.stMadChk1.s32CkMaxRange
	str	sl, [r0, #48]	@, <variable>.stMadChk2.s32CkMaxRange
	mov	sl, #32	@,
	str	r5, [r0, #136]	@ tmp158, <variable>.stMadCcrScl.s32NCcrDetBld
	str	sl, [r0, #60]	@, <variable>.stMadDirMult.s32MultDir
	mov	sl, #15	@,
	str	r2, [r0, #120]	@ tmp141, <variable>.stMadCcrScl.s32NCcrDetMax
	str	sl, [r0, #72]	@, <variable>.stMadDirMult.s32MultDir
	mov	sl, #7	@,
	str	r8, [r0, #100]	@ tmp159, <variable>.stMadDirMult.s32MultDir
	str	sl, [r0, #92]	@, <variable>.stMadDirMult.s32MultDir
	movw	sl, #:lower16:-320	@ tmp273,
	str	r8, [r0, #104]	@ tmp159, <variable>.stMadDirMult.s32MultDir
	movt	sl, #:upper16:-320	@ tmp273,
	str	r1, [r0, #112]	@ tmp162, <variable>.stMadCcrScl.bCcrEn
	str	sl, [r0, #16]	@ tmp273, <variable>.stMadInten.s32MinIntenVer
	mov	sl, #40	@ tmp147,
	str	r4, [r0, #116]	@ tmp163, <variable>.stMadCcrScl.s32ChrMaOffset
	str	sl, [r0, #52]	@ tmp147, <variable>.stMadDirMult.s32MultDir
	mov	sl, #24	@ tmp148,
	str	r4, [r0, #124]	@ tmp163, <variable>.stMadCcrScl.s32NccrDetThd
	str	sl, [r0, #56]	@ tmp148, <variable>.stMadDirMult.s32MultDir
	mov	sl, #27	@ tmp150,
	str	sl, [r0, #64]	@ tmp150, <variable>.stMadDirMult.s32MultDir
	mov	sl, #18	@ tmp151,
	str	sl, [r0, #68]	@ tmp151, <variable>.stMadDirMult.s32MultDir
	mov	sl, #12	@ tmp153,
	str	sl, [r0, #76]	@ tmp153, <variable>.stMadDirMult.s32MultDir
	mov	sl, #11	@ tmp154,
	str	sl, [r0, #80]	@ tmp154, <variable>.stMadDirMult.s32MultDir
	mov	sl, #9	@ tmp155,
	str	sl, [r0, #84]	@ tmp155, <variable>.stMadDirMult.s32MultDir
	mov	sl, #3	@ tmp161,
	str	sl, [r0, #108]	@ tmp161, <variable>.stMadDirMult.s32MultDir
	mov	sl, #7	@,
	str	r2, [r0, #128]	@ tmp141, <variable>.stMadCcrScl.s32SimlrMax
	str	r5, [r0, #160]	@ tmp158, <variable>.stMadIntpScl.s32IntpSclRat
	str	sl, [r0, #168]	@, <variable>.stMadIntpScl.s32IntpSclRat
	movw	sl, #5000	@ tmp187,
	str	r5, [r0, #144]	@ tmp158, <variable>.stMadIntpScl.s32IntpSclRat
	str	sl, [r0, #204]	@ tmp187, <variable>.stMadDirThd.s32StrengthThd
	mvn	sl, #0	@ tmp193,
	str	r5, [r0, #156]	@ tmp158, <variable>.stMadIntpScl.s32IntpSclRat
	mov	r5, #16	@,
	str	sl, [r0, #236]	@ tmp193, <variable>.stMadJitMtn.s32JitFlt
	mov	sl, #144	@ tmp198,
	str	r2, [r0, #172]	@ tmp141, <variable>.stMadIntpScl.s32IntpSclRat
	str	sl, [r0, #256]	@ tmp198, <variable>.stMadFldMtn.s32FldMtnThdH
	mov	sl, #7	@,
	str	r2, [r0, #176]	@ tmp141, <variable>.stMadIntpScl.s32IntpSclRat
	str	sl, [r0, #164]	@, <variable>.stMadIntpScl.s32IntpSclRat
	mov	sl, #32	@,
	str	r2, [r0, #180]	@ tmp141, <variable>.stMadIntpScl.s32IntpSclRat
	str	r2, [r0, #184]	@ tmp141, <variable>.stMadIntpScl.s32IntpSclRat
	str	r2, [r0, #188]	@ tmp141, <variable>.stMadIntpScl.s32IntpSclRat
	str	r2, [r0, #192]	@ tmp141, <variable>.stMadIntpScl.s32IntpSclRat
	str	r2, [r0, #196]	@ tmp141, <variable>.stMadIntpScl.s32IntpSclRat
	str	r2, [r0, #200]	@ tmp141, <variable>.stMadIntpScl.s32IntpSclRat
	str	sl, [r0, #212]	@, <variable>.stMadDirThd.s32BcGain
	str	r3, [r0, #224]	@ tmp134, <variable>.stMadJitMtn.s32JitCoring
	str	r3, [r0, #220]	@ tmp134, <variable>.stMadJitMtn.s32JitFactor
	str	r2, [r0, #228]	@ tmp141, <variable>.stMadJitMtn.s32JitGain
	str	r3, [r0, #232]	@ tmp134, <variable>.stMadJitMtn.s32JitFlt
	str	ip, [r0, #240]	@ tmp138, <variable>.stMadJitMtn.s32JitFlt
	str	r3, [r0, #244]	@ tmp134, <variable>.stMadFldMtn.s32FldMtnCoring
	str	ip, [r0, #252]	@ tmp138, <variable>.stMadFldMtn.s32FldMtnCrvSlp
	str	r2, [r0, #248]	@ tmp141, <variable>.stMadFldMtn.s32FldMtnGain
	str	r5, [r0, #260]	@, <variable>.stMadFldMtn.s32FldMtnThdL
	mov	r5, #96	@ tmp201,
	str	r8, [r0, #152]	@ tmp159, <variable>.stMadIntpScl.s32IntpSclRat
	str	r4, [r0, #132]	@ tmp163, <variable>.stMadCcrScl.s32SimlrThd
	str	r7, [r0, #140]	@ tmp169, <variable>.stMadCcrScl.s32XChrMax
	str	r8, [r0, #148]	@ tmp159, <variable>.stMadIntpScl.s32IntpSclRat
	str	r4, [r0, #208]	@ tmp163, <variable>.stMadDirThd.s32DirThd
	str	r1, [r0, #216]	@ tmp162, <variable>.stMadJitMtn.bJitMd
	str	r9, [r0, #264]	@ tmp200, <variable>.stMadMtnCrvThd.s32LumAvgThd
	str	r5, [r0, #268]	@ tmp201, <variable>.stMadMtnCrvThd.s32LumAvgThd
	mvn	r5, #7	@ tmp207,
	str	sl, [r0, #332]	@, <variable>.stMadMtnDiffThd.s32MtnDiffThd
	mov	sl, #160	@,
	str	r5, [r0, #292]	@ tmp207, <variable>.stMadMtnCrvSlp.s32MtnCrvSlp
	mov	r5, #16	@,
	str	sl, [r0, #276]	@, <variable>.stMadMtnCrvThd.s32LumAvgThd
	mov	sl, #255	@,
	str	r3, [r0, #280]	@ tmp134, <variable>.stMadMtnCrvSlp.s32MtnCrvSlp
	str	r2, [r0, #284]	@ tmp141, <variable>.stMadMtnCrvSlp.s32MtnCrvSlp
	str	r3, [r0, #288]	@ tmp134, <variable>.stMadMtnCrvSlp.s32MtnCrvSlp
	str	r2, [r0, #324]	@ tmp141, <variable>.stMadMtnCrvRat.s32MtnRatMax
	str	r5, [r0, #328]	@, <variable>.stMadMtnDiffThd.s32MtnDiffThd
	mov	r5, #15	@,
	str	sl, [r0, #348]	@, <variable>.stMadMtnDiffThd.s32MtnDiffThd
	str	sl, [r0, #352]	@, <variable>.stMadMtnDiffThd.s32MtnDiffThd
	str	sl, [r0, #356]	@, <variable>.stMadMtnDiffThd.s32MtnDiffThd
	mov	sl, #66	@ tmp237,
	str	r3, [r0, #360]	@ tmp134, <variable>.stMadMtnIirCrvSlp.s32MtnIirCrvSlp
	str	ip, [r0, #372]	@ tmp138, <variable>.stMadMtnIirCrvSlp.s32MtnIirCrvSlp
	str	ip, [r0, #376]	@ tmp138, <variable>.stMadMtnIirCrvSlp.s32MtnIirCrvSlp
	str	r2, [r0, #384]	@ tmp141, <variable>.stMadMtnIirCrvSlp.s32MtnIirCrvSlp
	str	r5, [r0, #388]	@, <variable>.stMadMtnIirCrvSlp.s32MtnIirCrvSlp
	mov	r5, #86	@ tmp239,
	str	ip, [r0, #432]	@ tmp138, <variable>.stMadMtnIirCrvRat.s32MtnIirRatMin
	str	r6, [r0, #340]	@ tmp202, <variable>.stMadMtnDiffThd.s32MtnDiffThd
	str	r7, [r0, #344]	@ tmp169, <variable>.stMadMtnDiffThd.s32MtnDiffThd
	str	r6, [r0, #272]	@ tmp202, <variable>.stMadMtnCrvThd.s32LumAvgThd
	str	r1, [r0, #296]	@ tmp162, <variable>.stMadMtnCrvRat.bMtnRatEn
	str	r4, [r0, #320]	@ tmp163, <variable>.stMadMtnCrvRat.s32MtnRatMin
	str	r4, [r0, #316]	@ tmp163, <variable>.stMadMtnCrvRat.s32MtnRatStart
	str	r6, [r0, #300]	@ tmp202, <variable>.stMadMtnCrvRat.s32MtnCrvRat
	str	r7, [r0, #304]	@ tmp169, <variable>.stMadMtnCrvRat.s32MtnCrvRat
	str	r7, [r0, #308]	@ tmp169, <variable>.stMadMtnCrvRat.s32MtnCrvRat
	str	r6, [r0, #312]	@ tmp202, <variable>.stMadMtnCrvRat.s32MtnCrvRat
	str	r9, [r0, #336]	@ tmp200, <variable>.stMadMtnDiffThd.s32MtnDiffThd
	str	r1, [r0, #364]	@ tmp162, <variable>.stMadMtnIirCrvSlp.s32MtnIirCrvSlp
	str	r1, [r0, #368]	@ tmp162, <variable>.stMadMtnIirCrvSlp.s32MtnIirCrvSlp
	str	r4, [r0, #380]	@ tmp163, <variable>.stMadMtnIirCrvSlp.s32MtnIirCrvSlp
	str	r1, [r0, #392]	@ tmp162, <variable>.stMadMtnIirCrvRat.bMtnIirEn
	str	r4, [r0, #436]	@ tmp163, <variable>.stMadMtnIirCrvRat.s32MtnIirRatMax
	str	ip, [r0, #428]	@ tmp138, <variable>.stMadMtnIirCrvRat.s32MtnIirRatStart
	mov	ip, #70	@ tmp238,
	str	sl, [r0, #400]	@ tmp237, <variable>.stMadMtnIirCrvRat.s32MtnIirCrvRat
	mov	sl, #102	@ tmp240,
	str	ip, [r0, #404]	@ tmp238, <variable>.stMadMtnIirCrvRat.s32MtnIirCrvRat
	mov	ip, #16	@,
	str	r5, [r0, #408]	@ tmp239, <variable>.stMadMtnIirCrvRat.s32MtnIirCrvRat
	mov	r5, #10	@ tmp268,
	str	sl, [r0, #412]	@ tmp240, <variable>.stMadMtnIirCrvRat.s32MtnIirCrvRat
	mov	sl, #133	@,
	str	r2, [r0, #456]	@ tmp141, <variable>.stMadRecMode.s32RecMdFldMtnGain
	mov	r2, #50	@,
	str	sl, [r0, #424]	@, <variable>.stMadMtnIirCrvRat.s32MtnIirCrvRat
	str	ip, [r0, #452]	@, <variable>.stMadRecMode.s32RecMdScl
	mov	ip, #15	@,
	str	r3, [r0, #520]	@ tmp134, <variable>.stMadMorFlt.s32MorFltSize
	str	r2, [r0, #524]	@, <variable>.stMadMorFlt.s32MorFltThd
	mov	r2, #30	@,
	str	r5, [r0, #536]	@ tmp268, <variable>.stMadCombChk.s32CombChkLowLmt
	mov	r5, #50	@,
	str	sl, [r0, #416]	@, <variable>.stMadMtnIirCrvRat.s32MtnIirCrvRat
	str	sl, [r0, #420]	@, <variable>.stMadMtnIirCrvRat.s32MtnIirCrvRat
	mov	sl, #160	@,
	str	r3, [r0, #448]	@ tmp134, <variable>.stMadRecMode.bRecMdMixMd
	str	r3, [r0, #440]	@ tmp134, <variable>.stMadRecMode.bRecMdWrMd
	str	r3, [r0, #460]	@ tmp134, <variable>.stMadRecMode.s32RecMdFldMtnCoring
	str	r3, [r0, #468]	@ tmp134, <variable>.stMadHisMtnMd.bHisMtnWrMd
	str	r3, [r0, #480]	@ tmp134, <variable>.stMadHisMtnMd.bPreInfoEn
	str	r3, [r0, #492]	@ tmp134, <variable>.stMadHisMtnMd.s32RecMdFrmMtnStp
	str	r3, [r0, #508]	@ tmp134, <variable>.stMadMorFlt.bDeflickerEn
	str	r3, [r0, #504]	@ tmp134, <variable>.stMadMorFlt.bMedBldEn
	str	r3, [r0, #516]	@ tmp134, <variable>.stMadMorFlt.s32AdjGain
	mov	r3, #255	@,
	str	r4, [r0, #496]	@ tmp163, <variable>.stMadHisMtnMd.s32RecMdFldMtnStp
	str	r1, [r0, #528]	@ tmp162, <variable>.stMadCombChk.bCombChkEn
	str	r9, [r0, #552]	@ tmp200, <variable>.stMadCombChk.s32CombChkEdgeThd
	str	r9, [r0, #396]	@ tmp200, <variable>.stMadMtnIirCrvRat.s32MtnIirCrvRat
	str	r1, [r0, #444]	@ tmp162, <variable>.stMadRecMode.bRecMdEn
	str	r5, [r0, #464]	@, <variable>.stMadRecMode.s32RecMdMtnThd
	str	r1, [r0, #476]	@ tmp162, <variable>.stMadHisMtnMd.bHisMtnEn
	str	r1, [r0, #472]	@ tmp162, <variable>.stMadHisMtnMd.bHisMtnUseMd
	str	r1, [r0, #484]	@ tmp162, <variable>.stMadHisMtnMd.bPpreInfoEn
	str	r1, [r0, #500]	@ tmp162, <variable>.stMadHisMtnMd.s32RecMdFldMtnStp
	str	r1, [r0, #488]	@ tmp162, <variable>.stMadHisMtnMd.s32RecMdFrmMtnStp
	str	r1, [r0, #512]	@ tmp162, <variable>.stMadMorFlt.bMorFltEn
	str	sl, [r0, #532]	@, <variable>.stMadCombChk.s32CombChkUpLmt
	str	ip, [r0, #540]	@, <variable>.stMadCombChk.s32CombChkMinThdV
	str	r2, [r0, #548]	@, <variable>.stMadCombChk.s32CombChkMdThd
	str	r3, [r0, #544]	@, <variable>.stMadCombChk.s32CombChkMinThdH
	ldmfd	sp, {r3, r4, r5, r6, r7, r8, r9, sl, fp, sp, pc}	@
	.fnend
	.size	MadThdParaInit, .-MadThdParaInit
	.align	2
	.global	ALG_MadSet
	.type	ALG_MadSet, %function
ALG_MadSet:
	.fnstart
.LFB1236:
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
	ldr	r3, [r0, #20]	@ <variable>.u32MadMvAddr, <variable>.u32MadMvAddr
	mov	ip, #960	@ tmp139,
	str	r3, [r2, #556]	@ <variable>.u32MadMvAddr, <variable>.u32MadSTBufAddrR
	ldr	r3, [r0, #12]	@ <variable>.u32MadMvAddr, <variable>.u32MadMvAddr
	str	ip, [r2, #564]	@ tmp139, <variable>.u32MadSTBufStride
	str	r3, [r2, #560]	@ <variable>.u32MadMvAddr, <variable>.u32MadSTBufAddrW
	ldr	r3, [r1, #48]	@ <variable>.s32Repeat, <variable>.s32Repeat
	cmp	r3, #0	@ <variable>.s32Repeat,
	ldmnefd	sp, {fp, sp, pc}	@
	ldr	r1, [r0, #20]	@ u32Addrtmp,
	ldr	r2, [r0, #16]	@ <variable>.u32MadMvAddr, <variable>.u32MadMvAddr
	ldr	ip, [r0, #12]	@ <variable>.u32MadMvAddr, <variable>.u32MadMvAddr
	str	r1, [r0, #12]	@ u32Addrtmp, <variable>.u32MadMvAddr
	str	r2, [r0, #20]	@ <variable>.u32MadMvAddr, <variable>.u32MadMvAddr
	str	ip, [r0, #16]	@ <variable>.u32MadMvAddr, <variable>.u32MadMvAddr
	ldmfd	sp, {fp, sp, pc}	@
	.fnend
	.size	ALG_MadSet, .-ALG_MadSet
	.align	2
	.global	ALG_DeiDeInit
	.type	ALG_DeiDeInit, %function
ALG_DeiDeInit:
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
	ldmfd	sp, {fp, sp, pc}	@
	.fnend
	.size	ALG_DeiDeInit, .-ALG_DeiDeInit
	.align	2
	.global	ALG_DeiRst
	.type	ALG_DeiRst, %function
ALG_DeiRst:
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
	bl	ALG_FmdReset	@
	ldmfd	sp, {fp, sp, pc}	@
	.fnend
	.size	ALG_DeiRst, .-ALG_DeiRst
	.align	2
	.global	ALG_DeiSet
	.type	ALG_DeiSet, %function
ALG_DeiSet:
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
	mov	r7, r3	@ pstDeiRtlOutPara, pstDeiRtlOutPara
	ldr	r3, [r1, #16]	@ <variable>.bDeiRst, <variable>.bDeiRst
	mov	r4, r1	@ pstDeiDrvPara, pstDeiDrvPara
	mov	r5, r0	@ pstDeiMem, pstDeiMem
	mov	r6, r2	@ pstDeiRtlPara, pstDeiRtlPara
	cmp	r3, #0	@ <variable>.bDeiRst,
	bne	.L15	@,
.L12:
	ldr	ip, [r5, #524]	@ <variable>.stMadMem.u32MadMvAddr, <variable>.stMadMem.u32MadMvAddr
	mov	r2, #960	@ tmp142,
	str	ip, [r6, #556]	@ <variable>.stMadMem.u32MadMvAddr, <variable>.stMadRtlPara.u32MadSTBufAddrR
	ldr	r1, [r5, #516]	@ <variable>.stMadMem.u32MadMvAddr, <variable>.stMadMem.u32MadMvAddr
	str	r2, [r6, #564]	@ tmp142, <variable>.stMadRtlPara.u32MadSTBufStride
	str	r1, [r6, #560]	@ <variable>.stMadMem.u32MadMvAddr, <variable>.stMadRtlPara.u32MadSTBufAddrW
	ldr	r0, [r4, #48]	@ <variable>.s32Repeat, <variable>.s32Repeat
	cmp	r0, #0	@ <variable>.s32Repeat,
	bne	.L13	@,
	ldr	r0, [r5, #524]	@ u32Addrtmp,
	ldr	r3, [r5, #520]	@ <variable>.stMadMem.u32MadMvAddr, <variable>.stMadMem.u32MadMvAddr
	ldr	lr, [r5, #516]	@ <variable>.stMadMem.u32MadMvAddr, <variable>.stMadMem.u32MadMvAddr
	str	r0, [r5, #516]	@ u32Addrtmp, <variable>.stMadMem.u32MadMvAddr
	str	r3, [r5, #524]	@ <variable>.stMadMem.u32MadMvAddr, <variable>.stMadMem.u32MadMvAddr
	str	lr, [r5, #520]	@ <variable>.stMadMem.u32MadMvAddr, <variable>.stMadMem.u32MadMvAddr
.L13:
	mov	ip, #1	@ tmp147,
	mov	r0, r5	@, pstDeiMem
	mov	r1, r4	@, pstDeiDrvPara
	mov	r5, #0	@ tmp146,
	mov	r2, r7	@, pstDeiRtlOutPara
	str	r5, [r6, #568]	@ tmp146, <variable>.stMadRtlPara.bMadMvInfoStp
	str	ip, [r4, #32]	@ tmp147, <variable>.EdgeSmoothEn
	str	ip, [r4, #20]	@ tmp147, <variable>.FodEnable
	str	ip, [r4, #24]	@ tmp147, <variable>.Pld22Enable
	str	ip, [r4, #28]	@ tmp147, <variable>.Pld32Enable
	bl	ALG_FmdSet	@
	mov	r0, r5	@, tmp146
	ldmfd	sp, {r4, r5, r6, r7, fp, sp, pc}	@
.L15:
	bl	ALG_DeiRst	@
	b	.L12	@
	.fnend
	.size	ALG_DeiSet, .-ALG_DeiSet
	.align	2
	.global	ALG_DeiInit
	.type	ALG_DeiInit, %function
ALG_DeiInit:
	.fnstart
.LFB1237:
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
	mov	r4, r0	@ pstDeiRtlPara, pstDeiRtlPara
	bl	MadThdParaInit	@
	add	r0, r4, #572	@, pstDeiRtlPara,
	bl	FmdThdParaInit	@
	ldmfd	sp, {r3, r4, fp, sp, pc}	@
	.fnend
	.size	ALG_DeiInit, .-ALG_DeiInit
	.align	2
	.global	ALG_MadDeInit
	.type	ALG_MadDeInit, %function
ALG_MadDeInit:
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
	ldr	r3, [r0, #0]	@ <variable>.stMBuf.u32StartVirAddr, <variable>.stMBuf.u32StartVirAddr
	mov	r4, r0	@ pstMadMem, pstMadMem
	cmp	r3, #0	@ <variable>.stMBuf.u32StartVirAddr,
	ldmeqfd	sp, {r3, r4, fp, sp, pc}	@
	bl	HI_DRV_MMZ_UnmapAndRelease	@
	mov	r0, #0	@ tmp135,
	str	r0, [r4, #0]	@ tmp135, <variable>.stMBuf.u32StartVirAddr
	ldmfd	sp, {r3, r4, fp, sp, pc}	@
	.fnend
	.size	ALG_MadDeInit, .-ALG_MadDeInit
	.align	2
	.global	ALG_DeiInfoDeInit
	.type	ALG_DeiInfoDeInit, %function
ALG_DeiInfoDeInit:
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
	add	r0, r0, #504	@, pstDeiMem,
	bl	ALG_MadDeInit	@
	mov	r0, #0	@,
	ldmfd	sp, {fp, sp, pc}	@
	.fnend
	.size	ALG_DeiInfoDeInit, .-ALG_DeiInfoDeInit
	.align	2
	.global	ALG_MadInit
	.type	ALG_MadInit, %function
ALG_MadInit:
	.fnstart
.LFB1234:
	@ args = 0, pretend = 0, frame = 0
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
	mov	r1, #0	@,
	mov	r4, r0	@ pstMadMem, pstMadMem
	movw	r2, #:lower16:1555200	@,
	movw	r0, #:lower16:.LC0	@,
	movt	r2, #:upper16:1555200	@,
	movt	r0, #:upper16:.LC0	@,
	mov	r3, r1	@,
	str	r4, [sp, #0]	@ pstMadMem,
	bl	HI_DRV_MMZ_AllocAndMap	@
	cmp	r0, #0	@ nRet
	bne	.L27	@,
	ldr	r3, [r4, #4]	@ D.26616, <variable>.stMBuf.u32StartPhyAddr
	add	lr, r3, #1036288	@ tmp145, D.26616,
	add	ip, r3, #516096	@ tmp143, D.26616,
	add	r1, ip, #2304	@ tmp143, tmp143,
	add	r2, lr, #512	@ tmp145, tmp145,
	str	r1, [r4, #16]	@ tmp143, <variable>.u32MadMvAddr
	str	r2, [r4, #20]	@ tmp145, <variable>.u32MadMvAddr
	str	r3, [r4, #12]	@ D.26616, <variable>.u32MadMvAddr
.L25:
	sub	sp, fp, #16
	ldmfd	sp, {r4, fp, sp, pc}
.L27:
	mov	r0, #0	@,
	mov	r1, #37	@,
	movw	r2, #:lower16:.LANCHOR0	@,
	mov	r3, #196	@,
	movt	r2, #:upper16:.LANCHOR0	@,
	movw	ip, #:lower16:.LC1	@ tmp141,
	movt	ip, #:upper16:.LC1	@ tmp141,
	str	ip, [sp, #0]	@ tmp141,
	bl	HI_LogOut	@
	mvn	r0, #0	@ D.26614,
	b	.L25	@
	.fnend
	.size	ALG_MadInit, .-ALG_MadInit
	.align	2
	.global	ALG_DeiInfoInit
	.type	ALG_DeiInfoInit, %function
ALG_DeiInfoInit:
	.fnstart
.LFB1238:
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
	mov	r4, r0	@ pstDeiMem, pstDeiMem
	add	r0, r0, #504	@, pstDeiMem,
	bl	ALG_MadInit	@
	cmp	r0, #0	@,
	bne	.L33	@,
	mov	r0, r4	@, pstDeiMem
	bl	ALG_FmdInit	@
	cmp	r0, #0	@ s32Ret.635
	ldmeqfd	sp, {r3, r4, fp, sp, pc}	@
	movw	r0, #:lower16:.LC2	@,
	movt	r0, #:upper16:.LC2	@,
	bl	printk	@
	mvn	r0, #0	@ D.26662,
	ldmfd	sp, {r3, r4, fp, sp, pc}	@
.L33:
	movw	r0, #:lower16:.LC1	@,
	movt	r0, #:upper16:.LC1	@,
	bl	printk	@
	mvn	r0, #0	@ D.26662,
	ldmfd	sp, {r3, r4, fp, sp, pc}	@
	.fnend
	.size	ALG_DeiInfoInit, .-ALG_DeiInfoInit
	.section	.rodata
	.align	2
.LANCHOR0 = . + 0
	.type	__func__.26606, %object
	.size	__func__.26606, 12
__func__.26606:
	.ascii	"ALG_MadInit\000"
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
.LC0:
	.ascii	"VPSS_MADMotionInfoBuf\000"
	.space	2
.LC1:
	.ascii	"Get VPSS_MADMotionInfoBuf failed\012\000"
	.space	2
.LC2:
	.ascii	"Get VPSS_FMDContextInfoBuf failed\012\000"
	.ident	"GCC: (Hisilicon_v200(gcc4.4-290+glibc-2.11+eabi+nptl)) 4.4.1"
	.section	.note.GNU-stack,"",%progbits

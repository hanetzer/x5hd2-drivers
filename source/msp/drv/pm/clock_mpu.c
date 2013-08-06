/*
 * hisilicon mpu clock management Routines
 *
 * Author: wangjian <stand.wang@huawei.com>
 *
 * Copyright (C) 2012 Hisilicon Instruments, Inc.
 * wangjian <stand.wang@huawei.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/err.h>
#include <linux/list.h>
#include <linux/spinlock.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <linux/clk.h>
#include <mach/platform.h>
#include <linux/clkdev.h>
#include <asm/clkdev.h>
#include <mach/clock.h>
#include <mach/early-debug.h>
#include <linux/device.h>

#include "hi_drv_pmoc.h"

struct device *mpu_dev;

/*APLL*/
#define PERI_CRG_PLL0 CRG_BASE
#define PERI_CRG_PLL1 CRG_BASE + 0x4

/*WPLL*/
#define PERI_CRG_PLL6 CRG_BASE + 0x18
#define PERI_CRG_PLL7 CRG_BASE + 0x1c

#define PLL_DEF_FREQ 24000 //24m
#define MAX_REFDIV 3
#define MAX_POSTDIV 3

#define TREND_PMC 18
#define PLLLOCK_PMC 19
#define SEL_CFG_MASK 0x7
#define DIV_CFG_MASK 0x30
#define REQ_PMC 21
#define SW_BEGIN_CFG_BIT 10

#define SW_OK_PMC_MASK 0x1000

static DEFINE_SPINLOCK(mpu_clock_lock);

#define DEFAULT_INIT_FREQ 800000
#define MAX_FREQ 1200000
#define MIN_FREQ 400000

#define PERI_PMC22  (PMC_BASE + 0x58)
#define PERI_PMC25  (PMC_BASE + 0x64)
#define PERI_PMC26  (PMC_BASE + 0x68)
#define PERI_PMC29  (PMC_BASE + 0x74)
#define PERI_PMC30  (PMC_BASE + 0x78)
#define PERI_PMC33  (PMC_BASE + 0x84)
#define PERI_PMC34  (PMC_BASE + 0x88)
#define PERI_PMC37  (PMC_BASE + 0x94)
#define PERI_PMC38  (PMC_BASE + 0x98)
#define PERI_PMC41  (PMC_BASE + 0xa4)


struct clk mpu_ck = {
    .name   = "mpu_ck",
    .parent = NULL,
};

int switch_pll(struct clk *clk, unsigned long foutpostdiv)
{
    return 0;
}

static int mpu_clk_set_div(unsigned long div)
{
    unsigned int v, tmp;

    HI_INFO_PM("enter %s\n", __FUNCTION__);

    v  = __raw_readl(PERI_CRG18);
    v &= ~(0x1 << SW_BEGIN_CFG_BIT);
    __raw_writel(v, PERI_CRG18);

    /* first set PERI_CRG18[2:0],select clock sel*/
    v = __raw_readl(PERI_CRG18);
    tmp = DIV_CFG_MASK;
    v &= ~tmp;
    v |= div << 4;
    __raw_writel(v, PERI_CRG18);

    v  = __raw_readl(PERI_CRG18);
    v |= 0x1 << SW_BEGIN_CFG_BIT;
    __raw_writel(v, PERI_CRG18);

    return 0;
}

static int mpu_init_hpm(unsigned long rate)
{
    unsigned int regval, div;

    div = (rate / 50000) - 1;

    if (div > 31)
    {
        HI_ERR_PM("\n Rate value is out of range \n");
        return -1;
    }

    /* hpm 0             */
    /* set time division */
    regval = __raw_readl(PERI_PMC22);
    regval &= 0xffffffc0;
    regval |= div;
    __raw_writel(regval, PERI_PMC22);

    /* set monitor period to 2ms */
    regval = __raw_readl(PERI_PMC25);
    regval &= 0x00ffffff;
    regval |= (1 << 24);
    __raw_writel(regval, PERI_PMC25);

    /* hpm enable */
    regval = __raw_readl(PERI_PMC22);
    regval |= (1 << 24);
    __raw_writel(regval, PERI_PMC22);

    /* hpm monitor enable */
    regval = __raw_readl(PERI_PMC22);
    regval |= (1 << 26);
    __raw_writel(regval, PERI_PMC22);

    /* hpm 1             */
    /* set time division */
    regval = __raw_readl(PERI_PMC26);
    regval &= 0xffffffc0;
    regval |= div;
    __raw_writel(regval, PERI_PMC26);

    /* set monitor period to 2ms */
    regval = __raw_readl(PERI_PMC29);
    regval &= 0x00ffffff;
    regval |= (1 << 24);
    __raw_writel(regval, PERI_PMC29);

    /* hpm enable */
    regval = __raw_readl(PERI_PMC26);
    regval |= (1 << 24);
    __raw_writel(regval, PERI_PMC26);

    /* hpm monitor enable */
    regval = __raw_readl(PERI_PMC26);
    regval |= (1 << 26);
    __raw_writel(regval, PERI_PMC26);

    /* hpm 2             */
    /* set time division */
    regval = __raw_readl(PERI_PMC30);
    regval &= 0xffffffc0;
    regval |= div;
    __raw_writel(regval, PERI_PMC30);

    /* set monitor period to 2ms */
    regval = __raw_readl(PERI_PMC33);
    regval &= 0x00ffffff;
    regval |= (1 << 24);
    __raw_writel(regval, PERI_PMC33);

    /* hpm enable */
    regval = __raw_readl(PERI_PMC30);
    regval |= (1 << 24);
    __raw_writel(regval, PERI_PMC30);

    /* hpm monitor enable */
    regval = __raw_readl(PERI_PMC30);
    regval |= (1 << 26);
    __raw_writel(regval, PERI_PMC30);

    /* hpm 3             */
    /* set time division */
    regval = __raw_readl(PERI_PMC34);
    regval &= 0xffffffc0;
    regval |= div;
    __raw_writel(regval, PERI_PMC34);

    /* set monitor period to 2ms */
    regval = __raw_readl(PERI_PMC37);
    regval &= 0x00ffffff;
    regval |= (1 << 24);
    __raw_writel(regval, PERI_PMC37);

    /* hpm enable */
    regval = __raw_readl(PERI_PMC34);
    regval |= (1 << 24);
    __raw_writel(regval, PERI_PMC34);

    /* hpm monitor enable */
    regval = __raw_readl(PERI_PMC34);
    regval |= (1 << 26);
    __raw_writel(regval, PERI_PMC34);

    /* hpm 4             */
    /* set time division */
    regval = __raw_readl(PERI_PMC38);
    regval &= 0xffffffc0;
    regval |= div;
    __raw_writel(regval, PERI_PMC38);

    /* set monitor period to 2ms */
    regval = __raw_readl(PERI_PMC41);
    regval &= 0x00ffffff;
    regval |= (1 << 24);
    __raw_writel(regval, PERI_PMC41);

    /* hpm enable */
    regval = __raw_readl(PERI_PMC38);
    regval |= (1 << 24);
    __raw_writel(regval, PERI_PMC38);

    /* hpm monitor enable */
    regval = __raw_readl(PERI_PMC38);
    regval |= (1 << 26);
    __raw_writel(regval, PERI_PMC38);

    return 0;
}

static int mpu_clk_set_sel(unsigned long sel)
{
    unsigned int v, tmp;

    HI_INFO_PM("enter %s\n", __FUNCTION__);

    v  = __raw_readl(PERI_CRG18);
    v &= ~(0x1 << SW_BEGIN_CFG_BIT);
    __raw_writel(v, PERI_CRG18);

    v = __raw_readl(PERI_CRG18);
    tmp = SEL_CFG_MASK;
    v &= ~tmp;
    v |= sel;
    __raw_writel(v, PERI_CRG18);

    v  = __raw_readl(PERI_CRG18);
    v |= 0x1 << SW_BEGIN_CFG_BIT;
    __raw_writel(v, PERI_CRG18);

    return 0;
}

/*
 * mpu_clksel_set_rate() - program clock rate in hardware
 * @clk: struct clk * to program rate
 * @rate: target rate to program
 *profile	freq		volt		sel
 *profile0	1.2GHz	1.3V		BPLL 1.2GHz
 *profile1	1GHz	1.25V	WPLL
 *profile2	750MHz	1.15V	APLL 750MHz
 *profile3	600MHz	1.1V		BPLL 1.2GHz DIV2
 *profile4	400MHz	1.05V	400MHz
 *000£ºAPLLÊä³ö£»
 *001£ºreserved£»
 *010£ºDPLLÊä³ö£»
 *011£ºWPLLÊä³ö£»
 *100£º24M£»
 *101£º1200M£»
 *110£º400M£»
 *111£º200M
 */
static int mpu_clk_set_rate(struct clk *clk, unsigned rate)
{
    unsigned int sel, div;
    unsigned long flag;

    HI_INFO_PM("mpu_clk_set_rate %dk\n", rate);
    div = 0;

    switch (rate)
    {
    case 400000:
        sel = 6; /*110*/
        break;
    case 800000:
        sel = 0;     /*apll*/
        break;
    case 1000000:
        sel = 3 /*1G wpll 011*/;
        break;
    case 1200000:
        sel = 5 /*1.2G bpll 101*/;
        break;
    case 600000:
        sel = 5 /*1.2G bpll 101*/;
        div = 1;
        break;
    case 24000:
        sel = 4;
        break;
    default:
        printk("mpu_clk_set_rate %dk not support\n", rate);
        return -1;
    }

    HI_INFO_PM("mpu_clk_set_rate sel=%u,div=%u\n", sel, div);

    /*only for s40v2 if div>1 first set div*/
    if (div > 0)
    {
        mpu_clk_set_div(div);
        mpu_clk_set_sel(sel);
    }
    else
    {
        mpu_clk_set_sel(sel);
        mpu_clk_set_div(div);
    }

    spin_lock_irqsave(&mpu_ck.spinlock, flag);
    clk->rate = rate;
    __raw_writel(rate, IO_ADDRESS(S40_REG_BASE + 0xE8));
    spin_unlock_irqrestore(&mpu_ck.spinlock, flag);

    /* After change clock, we need to reinitialize hpm */
    mpu_init_hpm(rate);

    return 0;
}

static int mpu_clk_get_sel(void)
{
    unsigned int v;

    v = __raw_readl(PERI_CRG18);
    v = v & SEL_CFG_MASK;

    return v;
}

static int mpu_clk_get_div(void)
{
    unsigned int v;

    v = __raw_readl(PERI_CRG18);
    v = (v & DIV_CFG_MASK) >> 4;

    return v;
}

static unsigned int __mpu_clk_get_rate(struct clk *clk)
{
    unsigned int sel, div;
    int rate;

    sel = mpu_clk_get_sel();
    switch (sel)
    {
    case 0:
        rate = 800000;
        break;
    case 3:
        rate = 1000000;
        break;
    case 4:
        rate = 24000;
        break;
    case 5:
        div = mpu_clk_get_div();
        rate = 1200000 / (div + 1);
        break;
    case 6:
        rate = 400000;
        break;
    default:
        return -1;      
    }

    return rate;
 
}

static unsigned int mpu_clk_get_rate(struct clk *clk)
{
    return __mpu_clk_get_rate(clk);
}

/*
 *995.328MHz		1.25V	WPLL
 *750MHz		1.15V	APLL
 */

/*if DSMPD = 1 (DSM is disabled, "integer mode")
*FOUTVCO = FREF / REFDIV * FBDIV
*FOUTPOSTDIV = FOUTVCO / POSTDIV1 / POSTDIV2
*If DSMPD = 0 (DSM is enabled, "fractional mode")
*FOUTVCO = FREF / REFDIV * (FBDIV + FRAC / 2^24)
*FOUTPOSTDIV = FOUTVCO / POSTDIV1 / POSTDIV2
*/
static unsigned int __mpu_pll_init(void)
{
#if 0
    /*set apll 750 mhz*/
    __raw_writel(0x21000000, PERI_CRG_PLL0);
    __raw_writel(0x200207d, PERI_CRG_PLL1);
#endif
    /*set wpll 1000 mhz*/
    __raw_writel(0x21000000, PERI_CRG_PLL6);
    __raw_writel(0x20020a7, PERI_CRG_PLL7);

    return 0;
}

static void mpu_clk_init(struct clk *clk)
{
    unsigned int v;

    v  = __raw_readl(PERI_CRG18);
    v |= 1 << 8;
    v |= 1 << 9;
    __raw_writel(v, PERI_CRG18);
}

static struct clk_ops mpu_clock_ops = {
    .set_rate = mpu_clk_set_rate,
    .get_rate = mpu_clk_get_rate,
    .init     = mpu_clk_init,
};

void  mpu_init_clocks(void)
{
    HI_INFO_PM("Enter %s\n", __FUNCTION__);
    mpu_ck.ops  = &mpu_clock_ops;
    mpu_ck.rate = DEFAULT_INIT_FREQ;
    mpu_ck.max_rate = MAX_FREQ;
    mpu_ck.min_rate = MIN_FREQ;

    mpu_ck.spinlock = mpu_clock_lock;
    clk_init(&mpu_ck);

    //mpu_clk_set_rate(&mpu_ck, mpu_ck.rate);
    __mpu_pll_init();
    return;
}

void  mpu_exit_clocks(void)
{
    clk_exit(&mpu_ck);
}

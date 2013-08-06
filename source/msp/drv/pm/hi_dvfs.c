/*
 * hisilicon DVFS Management Routines
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

#include <linux/err.h>
#include <linux/spinlock.h>
#include <linux/plist.h>
#include <linux/slab.h>
#include "opp.h"
#include <linux/debugfs.h>
#include <linux/seq_file.h>
#include <linux/io.h>
#include <mach/platform.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <mach/clock.h>
#include "hi_dvfs.h"
#include "hi_drv_pmoc.h"

extern struct clk mpu_ck;

#define CONFIG_DVFS_PWM

#define VMAX 1320 /*mv*/
#define VMIN 900 /*mv*/
#define PWM_STEP 5 /*mv*/
#define PWM_CLASS 2

#define PERI_PMC6 (PMC_BASE + 0x18)
#define PERI_PMC9 (PMC_BASE + 0x24)

#define PWM_CPU1 PERI_PMC6
#define PWM_CPU2 PERI_PMC9

#define PWM_DUTY_MASK 0xffff0000
#define PWM_PERIOD_MASK 0xffff
#define PWM_ENABLE_BIT 0x0

#define HPM_CPU1 PERI_PMC22
#define HPM_CPU2 PERI_PMC23
#define HPM_CPU3 PERI_PMC24
#define HPM_CPU4 PERI_PMC25

#define TSENSOR_CPU1 PERI_PMC10
#define TSENSOR_CPU2 PERI_PMC12
#define TSENSOR_CPU3 PERI_PMC14

#define PERI_PMC22 (PMC_BASE + 0x58)
#define PERI_PMC23 (PMC_BASE + 0x5c)
#define PERI_PMC24 (PMC_BASE + 0x60)
#define PERI_PMC25 (PMC_BASE + 0x64)

#define PERI_PMC10 (PMC_BASE + 0x28)
#define PERI_PMC11 (PMC_BASE + 0x2c)

#define PERI_PMC12 (PMC_BASE + 0x30)
#define PERI_PMC13 (PMC_BASE + 0x34)
#define PERI_PMC14 (PMC_BASE + 0x38)
#define PERI_PMC15 (PMC_BASE + 0x3c)

#define HPM_MONITOR_EN 26
#define HPM_EN 24
#define HPM_PC_RECORED0_MASK 0x3ff
#define HPM_PC_RECORED1_MASK 0x3ff000
#define HPM_PC_RECORED2_MASK 0x3ff
#define HPM_PC_RECORED3_MASK 0x3ff000

#define HPM_MONITOR_PERIOD_MASK

#define MIN_THRESHOLD 5
#define AVS_STEP 5 /*mv*/
#define BEST_VRING 333 /*fixme*/

unsigned long cur_volt = 1200;
int sub_volt = 0;

DEFINE_MUTEX(hi_dvfs_lock);

/**
 * struct hi_dvfs_info - The per vdd dvfs info
 * @user_lock:	spinlock for plist operations
 *
 * This is a fundamental structure used to store all the required
 * DVFS related information for a vdd.
 */
struct hi_dvfs_info
{
    unsigned long volt;
    unsigned long new_freq;
    unsigned long old_freq;
};

void mpu_init_volt(void)
{
    unsigned int period, duty, v, tmp;
    unsigned int vmax, vmin, pwc, pws;
    unsigned int volt = cur_volt;

    HI_INFO_PM("_volt_scale,volt=%d\n", volt);

    volt += sub_volt;
    vmax  = VMAX;
    vmin  = VMIN;
    pwc   = PWM_CLASS;
    pws   = PWM_STEP;

    period = (vmax - vmin) * pwc / 5 - 1;
    duty = (((vmax - volt) / pws) * pwc) - 1;

    HI_INFO_PM("period=%#x,duty=%#x\n", period, duty);

    v = __raw_readl(PWM_CPU1);
    tmp = PWM_PERIOD_MASK;
    v &= ~tmp;
    v |= period;

    tmp = PWM_DUTY_MASK;
    v &= ~tmp;
    v |= duty << 16;

    __raw_writel(v, PWM_CPU1);

    v  = __raw_readl(PWM_CPU2);
    v |= (0x1 << PWM_ENABLE_BIT);
    __raw_writel(v, PWM_CPU2);

    cur_volt = volt - sub_volt;

    return;
}

int _volt_scale(unsigned int volt)
{
    unsigned int duty, v, tmp;
    unsigned int vmax, vmin, pwc, pws;

    HI_INFO_PM("_volt_scale,volt=%d\n", volt);

    volt += sub_volt;
    vmax  = VMAX;
    vmin  = VMIN;
    pwc   = PWM_CLASS;
    pws   = PWM_STEP;

    duty = (((vmax - volt) / pws) * pwc) - 1;

    HI_INFO_PM("duty=%#x\n", duty);

    v = __raw_readl(PWM_CPU1);
    tmp = PWM_DUTY_MASK;
    v &= ~tmp;
    v |= duty << 16;

    __raw_writel(v, PWM_CPU1);
    cur_volt = volt - sub_volt;
    return 0;
}

unsigned long _get_best_vring(void)
{
    return BEST_VRING;
}

unsigned long _get_cur_vring(void)
{
    /*fixme*/
    unsigned long v, tmp, vring;

    v  = __raw_readl(HPM_CPU1);
    v &= ~(0x1 << HPM_MONITOR_EN);
    v |= (0x1 << HPM_EN);
    __raw_writel(v, HPM_CPU1);

    v = __raw_readl(HPM_CPU2);
    tmp = HPM_PC_RECORED0_MASK;
    v &= tmp;
    vring = v >> ffs(v);

    return vring;
}

void do_avs(void)
{
    unsigned long vring, vbest;

    vbest = _get_best_vring();
    vring = _get_cur_vring();

    while (vring != vbest)
    {
        if (vring > vbest)
        {
            if ((vring - vbest) < MIN_THRESHOLD)
            {
                break;
            }

            if (!cur_volt)
            {
                _volt_scale(cur_volt + AVS_STEP);
            }
        }
        else if (vring > vbest)
        {
            if ((vbest - vring) < MIN_THRESHOLD)
            {
                break;
            }

            if (!cur_volt)
            {
                _volt_scale(cur_volt - AVS_STEP);
            }
        }
    }
}

/**
 * _dvfs_scale() : Scale the devices associated with a voltage domain
 *
 * Returns 0 on success else the error value.
 */
static int _dvfs_scale(struct device *target_dev, struct hi_dvfs_info *tdvfs_info)
{
    struct clk * clk;
    int ret;

    HI_INFO_PM("%s rate=%ld\n", __FUNCTION__, tdvfs_info->new_freq);

    clk = &mpu_ck;
    if (tdvfs_info->new_freq == tdvfs_info->old_freq)
    {
        return 0;
    }
    else if (tdvfs_info->new_freq > tdvfs_info->old_freq)
    {
        ret = _volt_scale(tdvfs_info->volt);
        if (ret)
        {
            HI_ERR_PM("%s: scale volt to %ld falt\n",
                    __func__, tdvfs_info->volt);
            return ret;
        }

        msleep(10);
        ret = clk_set_rate(clk, tdvfs_info->new_freq);
        if (ret)
        {
            HI_ERR_PM("%s: scale freq to %ld falt\n",
                    __func__, tdvfs_info->new_freq);
            return ret;
        }
    }
    else
    {
        ret = clk_set_rate(clk, tdvfs_info->new_freq);
        if (ret)
        {
            HI_ERR_PM("%s: scale freq to %ld falt\n",
                    __func__, tdvfs_info->new_freq);
            return ret;
        }

        msleep(10);
        ret = _volt_scale(tdvfs_info->volt);
        if (ret)
        {
            HI_ERR_PM("%s: scale volt to %ld falt\n",
                    __func__, tdvfs_info->volt);
            return ret;
        }
    }

    return ret;
}

/**
 * hi_device_scale() - Set a new rate at which the devices is to operate
 * @rate:	the rnew rate for the device.
 *
 * This API gets the device opp table associated with this device and
 * tries putting the device to the requested rate and the voltage domain
 * associated with the device to the voltage corresponding to the
 * requested rate. Since multiple devices can be assocciated with a
 * voltage domain this API finds out the possible voltage the
 * voltage domain can enter and then decides on the final device
 * rate.
 *
 * Return 0 on success else the error value
 */
int hi_device_scale(struct device *target_dev, unsigned long old_freq, unsigned long new_freq)
{
    struct opp *opp;
    unsigned long volt, freq = new_freq;
    struct hi_dvfs_info dvfs_info;

    int ret = 0;

    HI_INFO_PM("hi_device_scale,oldfreq = %ld,newfreq = %ld\n", old_freq, new_freq);

    /* Lock me to ensure cross domain scaling is secure */
    mutex_lock(&hi_dvfs_lock);
    rcu_read_lock();

    opp = opp_find_freq_ceil(target_dev, &freq);

    /* If we dont find a max, try a floor at least */
    if (IS_ERR(opp))
    {
        opp = opp_find_freq_floor(target_dev, &freq);
    }

    if (IS_ERR(opp))
    {
        rcu_read_unlock();
        HI_ERR_PM("%s: Unable to find OPP for freq%ld\n",
                  __func__, freq);
        ret = -ENODEV;
        goto out;
    }

    volt = opp_get_voltage(opp);

    rcu_read_unlock();

    dvfs_info.old_freq = old_freq;

    dvfs_info.new_freq = freq;

    dvfs_info.volt = volt;

    /* Do the actual scaling */
    ret = _dvfs_scale( target_dev, &dvfs_info);

    if (ret)
    {
        HI_ERR_PM("%s: scale failed %d[f=%ld, v=%ld]\n",
                  __func__, ret, freq, volt);

        /* Fall through */
    }

    /* Fall through */
out:
    mutex_unlock(&hi_dvfs_lock);
    return ret;
}

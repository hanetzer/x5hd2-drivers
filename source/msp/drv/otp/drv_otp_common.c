#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/module.h>
#include <asm/delay.h>
#include <linux/kernel.h>
#include <mach/platform.h>
#include <mach/hardware.h>
#include <asm/io.h>
#include <asm/memory.h>
#include "hi_type.h"
#include "drv_otp.h"

HI_VOID otp_write_reg(HI_U32 addr, HI_U32 val)
{
	writel(val, IO_ADDRESS(addr));
	return;
}

HI_U32 otp_read_reg(HI_U32 addr)
{
	HI_U32 val;
	val = readl(IO_ADDRESS(addr));
	return val;
}

HI_VOID otp_wait(HI_U32 us)
{
	udelay(us);
}

/*---------------------------------------------END--------------------------------------*/

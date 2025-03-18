/*
**
**     
**     Copyright Copyright 2025 Mono Technologies Inc.
**     
**      SPDX-License-Identifier: BSD-3-Claus
**
**     
*/

#include <assert.h>
#include <errno.h>
#include <string.h>

#include <common/debug.h>
#include <ddr.h>
#include <lib/utils.h>

#include <errata.h>
#include <platform_def.h>

const struct ddr_cfg_regs static_1600 = {
	.cs[0].bnds = 0x01FF,
	.cs[0].config = 0x80840512,
	.cs[0].config_2 = 0x00,
	.timing_cfg[0] = 0x80550018,
	.timing_cfg[1] = 0xCDC60F44,
	.timing_cfg[2] = 0x0049111C,
	.timing_cfg[3] = 0x01161000,
	.timing_cfg[4] = 0x01,
	.timing_cfg[5] = 0x04001000,
	.timing_cfg[7] = 0x23300000,
	.timing_cfg[8] = 0x03336800,
	.sdram_cfg[0] = 0xE5040008,
	.sdram_cfg[1] = 0x00400050,
	.dq_map[0] = 0x00,
	.dq_map[1] = 0x00,
	.dq_map[2] = 0x00,
	.dq_map[3] = 0x00,
	.sdram_mode[0] = 0x01010210,
	.sdram_mode[8] = 0x0500,
	.sdram_mode[9] = 0x04C80000,
	.md_cntl = 0x00,
	.interval = 0x18600618,
	.data_init = 0xDEADBEEF,
	.clk_cntl = 0x02800000,
	.init_addr = 0x00,
	.ddr_sr_cntr = 0x0,
	.init_ext_addr = 0x00,
	.zq_cntl = 0x8A090705,
	.wrlvl_cntl[0] = 0x86750603,
	.wrlvl_cntl[1] = 0x03080809,
	.wrlvl_cntl[2] = 0x09090908,
	.cdr[0] = 0x80040000,
	.cdr[1] = 0x81,
};

long long board_static_ddr(struct ddr_info *priv)
{
	memcpy(&priv->ddr_reg, &static_1600, sizeof(static_1600));

	return 0x200000000;
}

long long init_ddr(void)
{
	int spd_addr[] = { NXP_SPD_EEPROM0 };
	struct ddr_info info;
	struct sysinfo sys;
	long long dram_size;

	zeromem(&sys, sizeof(sys));
	get_clocks(&sys);

	debug("platform clock %lu\n", sys.freq_platform);
	debug("DDR PLL1 %lu\n", sys.freq_ddr_pll0);
	debug("DDR PLL2 %lu\n", sys.freq_ddr_pll1);

	zeromem(&info, sizeof(struct ddr_info));
	info.num_ctlrs = 1;
	info.dimm_on_ctlr = 1;
	info.clk = get_ddr_freq(&sys, 0);
	info.spd_addr = spd_addr;
	info.ddr[0] = (void *)NXP_DDR_ADDR;

	dram_size = dram_init(&info);

	if (dram_size < 0)
		ERROR("DDR init failed.\n");

	erratum_a008850_post();

	return dram_size;
}
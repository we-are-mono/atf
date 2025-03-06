#
# Copyright 2025 Mono Technologies Inc.
#
# SPDX-License-Identifier: BSD-3-Clause
#

# Board-specific build parameters
BOOT_MODE			?=	qspi
BOARD				:=	gateway_dk
POVDD_ENABLE		:=	no
SUPPORTED_BOOT_MODE	:=	qspi

# DDR Compilation Configs
CONFIG_STATIC_DDR	:= 1
CONFIG_DDR_NODIMM	:= 1
DDRC_NUM_DIMM		:= 0
NUM_OF_DDRC			:= 1
DDRC_NUM_CS			:= 1
DDR_ECC_EN			:= yes
DDR_DEBUG			:= yes

# On-Board Flash Details
QSPI_FLASH_SZ		:=	0x20000000
NOR_FLASH_SZ		:=	0x20000000

# Platform specific features.
WARM_BOOT			:=	no

# Adding Platform files build files
BL2_SOURCES	+=	${BOARD_PATH}/ddr_init.c ${BOARD_PATH}/platform.c

# Adding platform board build info
include plat/nxp/common/plat_make_helper/plat_common_def.mk

# Adding SoC build info
include plat/nxp/soc-ls1046a/soc.mk

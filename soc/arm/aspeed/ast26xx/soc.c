/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Copyright (c) 2021 ASPEED Technology Inc.
 */

#include <init.h>
#include <kernel.h>
#include <stdint.h>
#include <linker/linker-defs.h>

/*WDT0 registers*/
#define WDT0_BASE 0x7e785000

#define WDT_SOFTWARE_RESET_MASK_REG 0x28

#define WDT_SOFTWARE_RESET_REG 0x24
#define WDT_TRIGGER_KEY 0xAEEDF123

/* secure boot header : provide image size to bootROM for SPI boot */
struct sb_header {
	uint32_t key_location;
	uint32_t enc_img_addr;
	uint32_t img_size;
	uint32_t sign_location;
	uint32_t header_rev[2];
	uint32_t patch_location;        /* address of the rom patch */
	uint32_t checksum;
};

struct sb_header sbh __attribute((used, section(".sboot"))) = {
	.img_size = (uint32_t)&_image_rom_end,
};

/* external reference */
void aspeed_cache_init(void);

void z_platform_init(void)
{
	uint32_t base = DT_REG_ADDR(DT_NODELABEL(syscon));

	/* init cache */
	aspeed_cache_init();
}

void sys_arch_reboot(int type)
{
	sys_write32(0x3fffff1, WDT0_BASE + WDT_SOFTWARE_RESET_MASK_REG);
	sys_write32(WDT_TRIGGER_KEY, WDT0_BASE + WDT_SOFTWARE_RESET_REG);
	ARG_UNUSED(type);
}

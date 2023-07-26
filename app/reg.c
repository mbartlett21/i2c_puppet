#include "reg.h"

#include "app_config.h"
#include "backlight.h"
#include "keyboard.h"
#include "touchpad.h"

#include <pico/stdlib.h>
#include <RP2040.h> // TODO: When there's more than one RP chip, change this to be more generic
#include <stdio.h>

// We don't enable this by default cause it spams quite a lot
//#define DEBUG_REGS

static struct
{
	uint8_t regs[REG_ID_LAST];
} self;

void reg_process_packet(uint8_t in_reg, uint8_t in_data, uint8_t *out_buffer, uint8_t *out_len)
{
	const bool is_write = (in_reg & PACKET_WRITE_MASK);
	const uint8_t reg = (in_reg & ~PACKET_WRITE_MASK);

//	printf("read complete, is_write: %d, reg: 0x%02X\r\n", is_write, reg);

	if (is_write) {
		*out_len = 0;
		reg_set_value(reg, in_data);

		if (reg == REG_ID_BKL)
			backlight_sync();
	} else {
		out_buffer[0] = reg_get_value(reg);
		*out_len = sizeof(uint8_t);
	}
}

uint8_t reg_get_value(enum reg_id reg)
{
	return self.regs[reg];
}

void reg_set_value(enum reg_id reg, uint8_t value)
{
#ifdef DEBUG_REGS
	printf("%s: reg: 0x%02X, val: 0x%02X (%d)\r\n", __func__, reg, value, value);
#endif

	self.regs[reg] = value;
}

bool reg_is_bit_set(enum reg_id reg, uint8_t bit)
{
	return self.regs[reg] & bit;
}

void reg_set_bit(enum reg_id reg, uint8_t bit)
{
#ifdef DEBUG_REGS
	printf("%s: reg: 0x%02X, bit: %d\r\n", __func__, reg, bit);
#endif

	self.regs[reg] |= bit;
}

void reg_clear_bit(enum reg_id reg, uint8_t bit)
{
#ifdef DEBUG_REGS
	printf("%s: reg: 0x%02X, bit: %d\r\n", __func__, reg, bit);
#endif

	self.regs[reg] &= ~bit;
}

void reg_init(void)
{
	reg_set_value(REG_ID_CFG, CFG_USE_MODS);
	reg_set_value(REG_ID_BKL, 255);
	reg_set_value(REG_ID_DEB, 10);
	reg_set_value(REG_ID_FRQ, 10);	// ms
	reg_set_value(REG_ID_CF2, CF2_USB_KEYB_ON | CF2_USB_MOUSE_ON);
}

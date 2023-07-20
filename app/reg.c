#include "reg.h"

#include "app_config.h"
#include "backlight.h"
#include "gpioexp.h"
#include "puppet_i2c.h"
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

	*out_len = 0;

	switch (reg) {

	// common R/W registers
	case REG_ID_CFG:
	case REG_ID_DEB:
	case REG_ID_FRQ:
	case REG_ID_BKL:
	case REG_ID_HLD:
	case REG_ID_ADR:
	case REG_ID_CF2:
	{
		if (is_write) {
			reg_set_value(reg, in_data);

			switch (reg) {
			case REG_ID_BKL:
				backlight_sync();
				break;

			case REG_ID_ADR:
				puppet_i2c_sync_address();
				break;

			default:
				break;
			}
		} else {
			out_buffer[0] = reg_get_value(reg);
			*out_len = sizeof(uint8_t);
		}
		break;
	}

	// special R/W registers
	case REG_ID_DIR: // gpio direction
	case REG_ID_PUE: // gpio input pull enable
	case REG_ID_PUD: // gpio input pull direction
	{
		if (is_write) {
			switch (reg) {
			case REG_ID_DIR:
				gpioexp_update_dir(in_data);
				break;
			case REG_ID_PUE:
				gpioexp_update_pue_pud(in_data, reg_get_value(REG_ID_PUD));
				break;
			case REG_ID_PUD:
				gpioexp_update_pue_pud(reg_get_value(REG_ID_PUE), in_data);
				break;
			}
		} else {
			out_buffer[0] = reg_get_value(reg);
			*out_len = sizeof(uint8_t);
		}
		break;
	}

	case REG_ID_RST:
		NVIC_SystemReset();
		break;
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
	reg_set_value(REG_ID_PUD, 0xFF);
	reg_set_value(REG_ID_HLD, 30);	// 10ms units
	reg_set_value(REG_ID_ADR, 0x1F);
	reg_set_value(REG_ID_CF2, CF2_USB_KEYB_ON | CF2_USB_MOUSE_ON);
}

#pragma once

#include <stdbool.h>
#include <stdint.h>

enum reg_id
{
	REG_ID_BKL = 0x05, // backlight (255)
	REG_ID_DEB = 0x06, // key debounce cfg (ms) (not implemented) (10)
	REG_ID_FRQ = 0x07, // key poll freq cfg (ms) (10)

	REG_ID_LAST,
};

#define CF2_USB_KEYB_ON		(1 << 1) // Should key events be sent over USB HID
#define CF2_USB_MOUSE_ON	(1 << 2) // Should touch events be sent over USB HID
// TODO? CF2_STICKY_MODS // Pressing and releasing a mod affects next key pressed

#define KEY_CAPSLOCK		(1 << 5) // Caps lock status
#define KEY_NUMLOCK			(1 << 6) // Num lock status
#define KEY_COUNT_MASK		0x1F

#define DIR_OUTPUT			0
#define DIR_INPUT			1

#define VER_VAL				((VERSION_MAJOR << 4) | (VERSION_MINOR << 0))

#define PACKET_WRITE_MASK	(1 << 7)

void reg_process_packet(uint8_t in_reg, uint8_t in_data, uint8_t *out_buffer, uint8_t *out_len);

uint8_t reg_get_value(enum reg_id reg);
void reg_set_value(enum reg_id reg, uint8_t value);

bool reg_is_bit_set(enum reg_id reg, uint8_t bit);
void reg_set_bit(enum reg_id reg, uint8_t bit);
void reg_clear_bit(enum reg_id reg, uint8_t bit);

void reg_init(void);

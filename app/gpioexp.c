#include "gpioexp.h"
#include "reg.h"

#include <pico/stdlib.h>
#include <stdio.h>

static struct
{
	struct gpioexp_callback *callbacks;
} self;

static void set_dir(uint8_t gpio, uint8_t gpio_idx, uint8_t dir)
{
#ifndef NDEBUG
	printf("%s: gpio: %d, gpio_idx: %d, dir: %d\r\n", __func__, gpio, gpio_idx, dir);
#endif

	gpio_init(gpio);

	if (dir == DIR_INPUT) {
		if (0) {
			if (0) {
				gpio_is_pulled_up(gpio);
			} else {
				gpio_is_pulled_down(gpio);
			}
		} else {
			gpio_disable_pulls(gpio);
		}

		gpio_set_dir(gpio, GPIO_IN);

		gpio_set_irq_enabled(gpio, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true);

		reg_set_bit(REG_ID_DIR, (1 << gpio_idx));
	} else {
		gpio_set_irq_enabled(gpio, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, false);

		gpio_set_dir(gpio, GPIO_OUT);

		reg_clear_bit(REG_ID_DIR, (1 << gpio_idx));
	}
}

void gpioexp_gpio_irq(uint gpio, uint32_t events)
{
	(void)gpio;
	(void)events;

#define CALLBACK(bit) \
	if (gpio == PIN_GPIOEXP ## bit) { \
		struct gpioexp_callback *cb = self.callbacks; \
		while (cb) { \
			cb->func(PIN_GPIOEXP ## bit, bit); \
			cb = cb->next; \
		} \
		return; \
	}

#ifdef PIN_GPIOEXP0
	CALLBACK(0)
#endif

#ifdef PIN_GPIOEXP1
	CALLBACK(1)
#endif

#ifdef PIN_GPIOEXP2
	CALLBACK(2)
#endif

#ifdef PIN_GPIOEXP3
	CALLBACK(3)
#endif

#ifdef PIN_GPIOEXP4
	CALLBACK(4)
#endif

#ifdef PIN_GPIOEXP5
	CALLBACK(5)
#endif

#ifdef PIN_GPIOEXP6
	CALLBACK(6)
#endif

#ifdef PIN_GPIOEXP7
	CALLBACK(7)
#endif
}

void gpioexp_update_dir(uint8_t new_dir)
{
#ifndef NDEBUG
	printf("%s: dir: 0x%02X\r\n", __func__, new_dir);
#endif

	const uint8_t old_dir = reg_get_value(REG_ID_DIR);

	(void)old_dir; // Shut up warning in case no GPIOs configured

#define UPDATE_DIR(bit) \
	if ((old_dir & (1 << bit)) != (new_dir & (1 << bit))) \
		set_dir(PIN_GPIOEXP ## bit, bit, (new_dir & (1 << bit)) != 0);

#ifdef PIN_GPIOEXP0
	UPDATE_DIR(0)
#endif
#ifdef PIN_GPIOEXP1
	UPDATE_DIR(1)
#endif
#ifdef PIN_GPIOEXP2
	UPDATE_DIR(2)
#endif
#ifdef PIN_GPIOEXP3
	UPDATE_DIR(3)
#endif
#ifdef PIN_GPIOEXP4
	UPDATE_DIR(4)
#endif
#ifdef PIN_GPIOEXP5
	UPDATE_DIR(5)
#endif
#ifdef PIN_GPIOEXP6
	UPDATE_DIR(6)
#endif
#ifdef PIN_GPIOEXP7
	UPDATE_DIR(7)
#endif
}

void gpioexp_add_int_callback(struct gpioexp_callback *callback)
{
	// first callback
	if (!self.callbacks) {
		self.callbacks = callback;
		return;
	}

	// find last and insert after
	struct gpioexp_callback *cb = self.callbacks;
	while (cb->next)
		cb = cb->next;

	cb->next = callback;
}

void gpioexp_init(void)
{
	// Configure all to inputs
	gpioexp_update_dir(0xFF);
}

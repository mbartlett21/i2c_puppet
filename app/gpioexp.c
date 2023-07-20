#include "gpioexp.h"
#include "reg.h"

#include <pico/stdlib.h>
#include <stdio.h>

static struct
{
	struct gpioexp_callback *callbacks;
} self;

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

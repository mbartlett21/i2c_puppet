#pragma once

#include <sys/types.h>

struct gpioexp_callback
{
	void (*func)(uint8_t gpio, uint8_t gpio_idx);
	struct gpioexp_callback *next;
};

void gpioexp_gpio_irq(uint gpio, uint32_t events);

void gpioexp_add_int_callback(struct gpioexp_callback *callback);

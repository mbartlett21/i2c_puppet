#include "backlight.h"

#include <hardware/pwm.h>
#include <pico/stdlib.h>

void backlight_init(void)
{
	gpio_set_function(PIN_BKL, GPIO_FUNC_PWM);

	const uint slice_num = pwm_gpio_to_slice_num(PIN_BKL);

	pwm_config config = pwm_get_default_config();
	pwm_init(slice_num, &config, true);

	pwm_set_gpio_level(PIN_BKL, 255 * 0x80);
}

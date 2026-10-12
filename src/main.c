#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>

#define BUTTON_POLL_INTERVAL_MS 20
#define LED_COUNT 3
#define LED_PATTERN_COUNT 8

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)
#define LED1_NODE DT_ALIAS(led1)
#define LED2_NODE DT_ALIAS(led2)
#define BUTTON0_NODE DT_ALIAS(sw0)
/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */
static const struct gpio_dt_spec leds[] = {
	GPIO_DT_SPEC_GET(LED0_NODE, gpios),
	GPIO_DT_SPEC_GET(LED1_NODE, gpios),
	GPIO_DT_SPEC_GET(LED2_NODE, gpios),
};
static const struct gpio_dt_spec button_0 = GPIO_DT_SPEC_GET(BUTTON0_NODE, gpios);

static void define_led_pattern(bool led_pattern[LED_COUNT][LED_PATTERN_COUNT])
{
	/* Each column is one of the 2^3 possible LED states. */
	for (int combination = 0; combination < LED_PATTERN_COUNT; combination++)
	{
		for (int led = 0; led < LED_COUNT; led++)
		{
			led_pattern[led][combination] =
				(combination & (1 << led)) != 0;
		}
	}
}

int main(void)
{
	int ret;
	bool led_pattern[LED_COUNT][LED_PATTERN_COUNT] = { 0 };
	int led_count = ARRAY_SIZE(leds);

	define_led_pattern(led_pattern);

	for (int led = 0; led < led_count; led++) {
		if (!gpio_is_ready_dt(&leds[led])) {
			return 0;
		}
	}

	if (!gpio_is_ready_dt(&button_0)) {
		return 0;
	}

	for (int led = 0; led < led_count; led++) {
		if (gpio_pin_configure_dt(&leds[led], GPIO_OUTPUT_INACTIVE) < 0) {
			return 0;
		}
	}

	if (gpio_pin_configure_dt(&button_0, GPIO_INPUT) < 0) {
		return 0;
	}

	int state = 0;
	bool button_was_pressed = false;

	while (1) {
		int button_value = gpio_pin_get_dt(&button_0);
		if (button_value < 0) {
			return 0;
		}

		bool button_is_pressed = button_value != 0;
		if (button_is_pressed && !button_was_pressed) {
			state = (state + 1) % LED_PATTERN_COUNT;
			for (int led = 0; led < led_count; led++) {
				ret = gpio_pin_set_dt(&leds[led], led_pattern[led][state]);
				if (ret < 0) {
					return 0;
				}
			}
		}
		button_was_pressed = button_is_pressed;
		k_msleep(BUTTON_POLL_INTERVAL_MS);
	}
	return 0;
}

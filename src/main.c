#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/gpio.h>

LOG_MODULE_REGISTER(main, CONFIG_LOG_DEFAULT_LEVEL);

static const struct gpio_dt_spec button =
	GPIO_DT_SPEC_GET(DT_ALIAS(button_on), gpios);
static const struct gpio_dt_spec led =
	GPIO_DT_SPEC_GET(DT_ALIAS(blinky_led), gpios);
static struct gpio_callback button_cb_data;
static struct k_timer blink_timer;
static bool next_blink_value = true;
static int blink_interval_ms = CONFIG_BLINK_INTERVAL_MS;

static int toggle_led() {
	int status;

	LOG_INF("Set LED %s", next_blink_value ? "on" : "off");
	status = gpio_pin_set_dt(&led, next_blink_value);
	if (status) {
		LOG_ERR("Could not set LED value: %i", status);
	}
	next_blink_value = !next_blink_value;

	return status;
}

static void blink_timer_expired(struct k_timer *timer_id) {
	toggle_led();
}

static void blink_start(void) {
	LOG_INF("Start blinkinking");
	next_blink_value = true;
	k_timer_start(&blink_timer, K_MSEC(0), K_MSEC(blink_interval_ms));
}

static void blink_stop(void) {
	LOG_INF("Stop blinking");
	next_blink_value = false;
	toggle_led();
	k_timer_stop(&blink_timer);
}

static void button_pressed(const struct device *dev, struct gpio_callback *cb,
						   uint32_t pins) {
	int status;

	status = gpio_pin_get_dt(&button);
	if (status < 0) {
		LOG_ERR("Could not get button pin status: %i", status);
		return;
	}

	LOG_INF("Button status %i", status);
	if (status) {
		blink_start();
	} else {
		blink_stop();
	}
}

static int init_button(void) {
	int status;

	status = gpio_pin_configure_dt(&button, GPIO_INPUT);
	if (status) {
		LOG_ERR("Could not configure button input: %i", status);
		return status;
	}

	status = gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_BOTH);
	if (status) {
		LOG_ERR("Could not configure button interrupt: %i", status);
		return status;
	}

	gpio_init_callback(&button_cb_data, button_pressed, BIT(button.pin));
	status = gpio_add_callback(button.port, &button_cb_data);
	if (status) {
		LOG_ERR("Could not add button callback: %i", status);
	}

	return status;
}

static int init_led(void) {
	int status;

	status = gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);
	if (status) {
		LOG_ERR("Could not configure button input: %i", status);
	}

	return status;
}

void main(void) {
	int status;

	LOG_INF("Starting blinky advanced");

	status = init_led();
	if (status) {
		LOG_WRN("Could not intialize LED: %i", status);
	}

	k_timer_init(&blink_timer, blink_timer_expired, NULL);

	status = init_button();
	if (status) {
		LOG_WRN("Could not intialize button ON: %i", status);
	}
}

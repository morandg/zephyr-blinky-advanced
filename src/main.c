#include <zephyr/logging/log.h>
#include <zephyr/drivers/gpio.h>

LOG_MODULE_REGISTER(main, CONFIG_LOG_DEFAULT_LEVEL);

static const struct gpio_dt_spec button =
	GPIO_DT_SPEC_GET(DT_ALIAS(button_on), gpios);
static struct gpio_callback button_cb_data;

static void button_pressed(const struct device *dev, struct gpio_callback *cb,
						   uint32_t pins) {
	int status;

	status = gpio_pin_get_dt(&button);
	if (status < 0) {
		LOG_ERR("Could not get button pin status: %i", status);
		return;
	}
	LOG_INF("Button status %i", status);
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

void main(void) {
	int status;

	LOG_INF("Starting blinky advanced");
	status = init_button();
	if (status) {
		LOG_WRN("Could not intialize button ON: %i", status);
	}
}

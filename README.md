Zephyr advanced blinky example.

# Initialize the project
```sh
west init -m https://github.com/morandg/zephyr-blinky-advanced.git blinky-advanced
cd blinky-advanced
west update 
source zephyr/zephyr-env.sh
```

# Build
```sh
west build -b lpcxpresso55s28 blinky-advanced/
```

# Flash
```sh
west flash
```

# Porting this app
To port this application to other boards, add an device tree overlay with the
following aliases

```c
/ {
	aliases{
		button-on = &some_gpio_button;
		blinky-led = &some_gpio_led;
	};
};
```

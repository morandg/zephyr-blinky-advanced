Zephyr advanced blinky example.

# Initialize the project
```sh
west init -m git@github.com:morandg/zephyr-blinky-advanced.git blinky-advanced
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

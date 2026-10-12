# Nucleo-F439ZI Blinky Button

A Zephyr application for the Nucleo-F439ZI that uses the three onboard LEDs to
display an 8-state pattern. Each press of the user button advances to the next
pattern.

## Requirements

- A configured Zephyr development workspace with `west` available in your
  terminal
- The Nucleo-F439ZI board connected to your computer

The board target is `nucleo_f439zi/stm32f439xx`.

## Build and flash

Run these commands from the project directory in a terminal where the Zephyr
workspace is initialized:

```powershell
west build -p always -b nucleo_f439zi/stm32f439xx .
west flash
```

If PowerShell cannot find `west`, make sure the Zephyr workspace's Python
environment is active or add its `Scripts` directory to `PATH`, then open a new
terminal.

## Behavior

The application reads the `led0`, `led1`, and `led2` devicetree aliases and the
`sw0` button alias. It polls the button every 20 ms and updates the LEDs when it
detects a new press. The three LEDs represent the bits of a three-bit value, so
each press cycles through all eight combinations.

## Project files

- `CMakeLists.txt` registers `src/main.c` as the application source.
- `prj.conf` enables GPIO support.
- `src/main.c` configures and polls the button and controls the LED pattern.

## References

- [Zephyr blinky sample](https://github.com/zephyrproject-rtos/zephyr/tree/main/samples/basic/blinky)
- [Zephyr GPIO button interrupt sample](https://github.com/zephyrproject-rtos/zephyr/tree/main/samples/drivers/gpio/button_interrupt)
- [Nucleo-F439ZI board documentation](https://docs.zephyrproject.org/latest/boards/st/nucleo_f439zi/doc/index.html)

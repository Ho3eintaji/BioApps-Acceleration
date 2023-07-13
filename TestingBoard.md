## Testing Board

The chip manufactured is hosted in the [Testing Board](https://eslgit.epfl.ch/heep/heepocrates-testing-board).

To boot the system with JTAG, set the `BOOT_SEL` switch in the board to `L` (0),

and then connect with `openocd` as:

```
openocd -f heepocrates-testing-board.cfg 
```

## See whether the board is alive

### Trying the gpio_toggle app

Set the Frequency to 50MHz from the waveform generator, and the switch to bypass the FLL to `H` (1).

Compile the application as:

```
make applications/gpio_toggle/gpio_toggle.dis TARGET=testing_board_bypass_fll
```

Then load it to the board as

```
$RISCV/bin/riscv32-unknown-elf-gdb ./sw/applications/gpio_toggle/gpio_toggle.elf
```

this application will run in a `while(1)` loop a GPIO toggle of the `GPIO0`.

If you connect the logic analyzer or the oscilloscope to the `GPIO0` on the board, you should see the GPIO toggling.

### Trying the while_1_printf app

Set the Frequency to 50MHz from the waveform generator, and the switch to bypass the FLL to `H` (1).

Compile the application as:

```
make applications/while_1_printf/while_1_printf.dis TARGET=testing_board_bypass_fll
```

Then load it to the board as

```
$RISCV/bin/riscv32-unknown-elf-gdb ./sw/applications/while_1_printf/while_1_printf.elf
```

Run the application that will print `hello` concatenated with incremental number `i` forever.

To see the output, connect to the `UART` terminal (e.g. `pyterm.py -b 9600 ftdi://ftdi:4232h/3`)

Use the `BAUDRATE` `9600` to visualize the correct output of the uart.



### Trying the matmul app


Set the Frequency to 50MHz from the waveform generator,

then compile the application as:


```
make clean applications/matmul/matmul.dis CUSTOM_GCC_FLAGS="-DMEASURE_POWER_APPLICATION -DTOGGLE_GPIO"
```

this application will run a matmul in a `while(1)` loop, and toggles the `GPIO0` every 1000 iterations.

Then load it to the board as

```
$RISCV/bin/riscv32-unknown-elf-gdb ./sw/applications/matmul/matmul.elf
```

If you connect the logic analyzer or the oscilloscope to the `GPIO0` on the board, you should see the GPIO toggling.
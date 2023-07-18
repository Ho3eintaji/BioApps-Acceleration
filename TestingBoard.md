## Testing Board

The chip manufactured is hosted in the [Testing Board](https://eslgit.epfl.ch/heep/heepocrates-testing-board).


### SETUP

When bypassing the FLL, make sure to set the frequency to 50MHz from the waveform generator, and the switch to bypass the FLL to `H` (1).
Otherwise, set the frequency to 32768Hz from the waveform generator, and the switch to bypass the FLL to `L` (0).

Do not forget to `reset` the chip with the `reset` button before launching openocd.

To connect with `openocd`, execute:

```
openocd -f heepocrates-testing-board.cfg 
```
## See whether the board is alive (JTAG mode)

To boot the system with JTAG, set the `BOOT_SEL` switch in the board to `L` (0),

### Trying the gpio_toggle app

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
make clean applications/matmul/matmul.dis TARGET=testing_board_bypass_fll CUSTOM_GCC_FLAGS="-DMEASURE_POWER_APPLICATION -DTOGGLE_GPIO"
```

this application will run a matmul in a `while(1)` loop, and toggles the `GPIO0` every 1000 iterations.

Then load it to the board as

```
$RISCV/bin/riscv32-unknown-elf-gdb ./sw/applications/matmul/matmul.elf
```

If you connect the logic analyzer or the oscilloscope to the `GPIO0` on the board, you should see the GPIO toggling.


### Trying the FLL


Set the Frequency to 32768Hz from the waveform generator, and the switch to bypass the FLL to `L` (0).


```
make applications/while_1_printf/while_1_printf.flash_load.dis TARGET=testing_board_bypass_fll
```

Then load it to the board as

```
$RISCV/bin/riscv32-unknown-elf-gdb ./sw/applications/while_1_printf/while_1_printf.elf
```

Run the application that will print `hello` concatenated with incremental number `i` forever.

To see the output, connect to the `UART` terminal (e.g. `pyterm.py -b 9600 ftdi://ftdi:4232h/3`)

Use the `BAUDRATE` `9600` to visualize the correct output of the uart. `WARNING` this is not currently not working.


## See whether the board is alive (Flash Load mode)


To boot the system from the Flash with `flash_load` mode, set the `BOOT_SEL` switch in the board to `H` (1),
and `EXEC FLASH` to `L` (0).

```
make applications/while_1_printf/while_1_printf.flash_load.hex TARGET=testing_board_bypass_fll
```

Check whether the flash is alive:

```
cd hw/vendor/esl_epfl_x_heep/sw/vendor/yosyshq_icestorm/iceprog
```

Compile `iceprog` if you haven't done it yet by executing `make`.

Then execute:

```
/iceprog -d i:0x0403:0x6011 -I B -t
```
you should see:

```
init..
cdone: high
reset..
cdone: high
flash ID: 0xEF 0x60 0x18 0x00
cdone: high
Bye.
```

Now we program the flash as:

```
./iceprog -d i:0x0403:0x6011 -I B ../../../../../../../sw/applications/while_1_printf/while_1_printf.flash_load.hex
```


Reset the chip and connect to the `UART` terminal (e.g. `pyterm.py -b 9600 ftdi://ftdi:4232h/3`)


## See whether the board is alive (Flash Exec mode)


To boot the system from the Flash with `flash_exec` mode, set the `BOOT_SEL` switch in the board to `H` (1),
and `EXEC FLASH` to `H` (1).

```
make applications/while_1_printf/while_1_printf.flash_exec.hex TARGET=testing_board_bypass_fll
```

Check whether the flash is alive:

```
cd hw/vendor/esl_epfl_x_heep/sw/vendor/yosyshq_icestorm/iceprog
```

Compile `iceprog` if you haven't done it yet by executing `make`.

Then execute:

```
/iceprog -d i:0x0403:0x6011 -I B -t
```
you should see:

```
init..
cdone: high
reset..
cdone: high
flash ID: 0xEF 0x60 0x18 0x00
cdone: high
Bye.
```

Now we program the flash as:

```
./iceprog -d i:0x0403:0x6011 -I B ../../../../../../../sw/applications/while_1_printf/while_1_printf.flash_exec.hex
```


Reset the chip and connect to the `UART` terminal (e.g. `pyterm.py -b 9600 ftdi://ftdi:4232h/3`)



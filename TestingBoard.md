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

### Trying the CGRA

To test the CGRA, two examples are available for now.

```
make applications/cgra_func_test/main.dis TARGET=testing_board_bypass_fll
```

or 

```
make applications/cgra_kernels_test/main.dis TARGET=testing_board_bypass_fll
```

Then load it to the board as

```
$RISCV/bin/riscv32-unknown-elf-gdb ./sw/applications/cgra_func_test/main.elf
```

To see the output, connect to the `UART` terminal (e.g. `pyterm.py -b 9600 ftdi://ftdi:4232h/3`)

### Trying the FLL


Set the Frequency to 32768 Hz from the waveform generator, and the switch to bypass the FLL to `L` (0). Make sure you use the write TARGET!

```
make applications/while_1_printf/while_1_printf.flash_load.dis TARGET=testing_board_use_fll
```

Then load it to the board as

```
$RISCV/bin/riscv32-unknown-elf-gdb ./sw/applications/while_1_printf/while_1_printf.elf
```

Run the application that will print `hello` concatenated with incremental number `i` forever.

To see the output, connect to the `UART` terminal (e.g. `pyterm.py -b 9600 ftdi://ftdi:4232h/3`)

Use the `BAUDRATE` `9600` to visualize the correct output of the `UART`.

Alternatively, you can launch the fll_test application (define the PRINT_FLL_DEFAULT_VAL_TEST macro to enable extra `UART` outputs).

```
make applications/fll_test/fll_test.hex TARGET=testing_board_use_fll
```

This test runs various experiments, the first one checks the FLL control register values (`WARNING` it gives error for now because the default values are changed by the external crt0 when the FLL is initialized to 100 MHz). The FLL has been tested up to 300 MHz for now and it seems to work for this application (although some printf are not working at the beginning when relaunching the application multiple times probably because the FLL clock frequency register is not updated properly so the `UART` fails, but the application runs and the last printf always work).

### Changing the FLL clock reference

To change the FLL clock reference you need to update the following files:

sw/device/lib/crt/external_crt0.S

The FLL output frequency in feedback mode (mode=1) is given by:

```
FLL_freq = clock_ref * mult / div
```

For a 32768 clock reference, the external_crt0.S set the mult parameter to 6103 and the div parameter to 2:

``` 
32768 * 6103 / 2 = 99991552 Hz
```

You also need to update the sw/device/target/testing_board_use_fll/x-heep.h file. For example, to use a 1 MHz clock reference change the following variables (always use power of two clock reference, so for 1 MHz it is actually 1.048576 MHz = 2^20). You can also change the `UART` baudrate if wanted.

```
#define REFERENCE_CLOCK_Hz 1048576
#define REFERENCE_CLOCK_Hz_LOG2 20
#define UART_BAUDRATE 9600
```

You need to update the external_crt0.S file accordingly to initialize the FLL to 100 MHz. With a 1.048576 MHz you need to set the mult parameter to 190 and the div parameter to 2:

```
1048576 * 190 / 2 = 99614720 Hz
```

Finally, make sure you change the clock generator frequency to 1048576 Hz.

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



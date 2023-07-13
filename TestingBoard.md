## Testing Board

The chip manufactured is hosted in the [Testing Board](https://eslgit.epfl.ch/heep/heepocrates-testing-board).

To boot the system with JTAG, set the `BOOT_SEL` switch in the board to `L` (0),

and then connect with `openocd` as:

```
openocd -f heepocrates-testing-board.cfg 
```

## Trying the matmul app


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
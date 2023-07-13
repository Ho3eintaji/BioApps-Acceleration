## Testing Board

The chip manufactured is hosted in the [Testing Board](https://eslgit.epfl.ch/heep/heepocrates-testing-board).

To boot the system with JTAG, set the `BOOT_SEL` switch in the board to `L` (0),

and then connect with `openocd` as:

```
openocd -f heepocrates-testing-board.cfg 
```

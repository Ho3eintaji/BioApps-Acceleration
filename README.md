# Application: Transformer
Running a full transformer model on HEEPocrates board

# Running applications on HEEPocrates


## Prerequisite

0. Only if you are running on eslsrv


On BASH shell:

```
scl enable devtoolset-10 bash
source env.sh
```

On TCSH shell:

```
scl enable devtoolset-10 tcsh
source env.csh
```

1. You should do all the prerequisite explained in [x-heep](https://github.com/esl-epfl/x-heep)

Don't forget to activate the conda enviroment

```
conda activate core-v-mini-mcu
```

## Compiling Software

Don't forget to set the `RISCV` env variable to the compiler folder (without the `/bin` included).

Then go to the `./sw` folder and type:

For jtag booting (loading memory from jtag):

```
make x_applications/hello_world/hello_world.hex
```

For executing from flash:
```
make x_applications/hello_world/hello_world.flash_exec.hex
```

For loading from flash to memory:
```
make x_applications/hello_world/hello_world.flash_load.hex
```

This will create the executable file to be loaded in your target system (ASIC, FPGA, Simulation).

### Bypassing the FLL

If you want to bypass the FLL, compile as:

```
make x_applications/hello_world/hello_world.hex TARGET=bypass_fll
```

or if you are using the Testing Board, do

```
make x_applications/hello_world/hello_world.hex TARGET=testing_board_bypass_fll
```

this assume your input Clock is `50MHz`.
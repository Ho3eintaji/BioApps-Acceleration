Minimal configuration of a core-v-mcu

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
$ conda activate core-v-mini-mcu
```

## Compiling Software

Don't forget to set the `RISCV` env variable to the compiler folder (without the `/bin` included).

Then go to the `./sw` folder and type:

```
$ make applications/hello_blade/hello_blade.hex
```

This will create the executable file to be loaded in your target system (ASIC, FPGA, Simulation).

## Simulating

This project supports simulation with Verilator, and Siemens Questasim.

### Compiling for Verilator

To simulate your application with Verilator, first compile the HDL:

```
$ fusesoc --cores-root . run --no-export --target=sim --tool=verilator --setup --build eslepfl::heepocrates | tee buildsim.log
```

then, go to your target system built folder

```
$ cd ./build/eslepfl__heepocrates_0/sim-verilator
```

and type to run your compiled software:

```
$ ./Vheepocrates_testharness +firmware=../../../sw/applications/hello_blade/hello_blade.hex
```

### Compiling for Questasim

To simulate your application with Questasim, first set the env variable `MODEL_TECH` to your Questasim bin folder, then compile the HDL:

```
$ fusesoc --cores-root . run --no-export --target=sim --tool=modelsim --setup --build eslepfl::heepocrates 2>&1 | tee buildsim.log
```

then, go to your target system built folder

```
$ cd ./build/eslepfl__heepocrates_0/sim-modelsim/
```

and type to run your compiled software:

```
$ make run PLUSARGS="c firmware=../../../sw/applications/hello_blade/hello_blade.hex"
```

### UART DPI

To simulate the UART, we use the LowRISC OpenTitan [UART DPI](https://github.com/lowRISC/opentitan/tree/master/hw/dv/dpi/uartdpi).
Read how to interact with it in the Section "Interact with the simulated UART" [here](https://docs.opentitan.org/doc/ug/getting_started_verilator/).
The output of the UART DPI module is printed in the `uart0.log` file in the simulation folder.

For example, to see the "hello world!" output of the Verilator simulation:

```
$ cd ./build/eslepfl__heepocrates_0/sim-verilator
$ ./Vheepocrates_testharness +firmware=../../../sw/applications/hello_blade/hello_blade.hex
$ cat uart0.log
```
## Debug

TODO

## Emulation

TODO

### Synthesis with Synopsys Design Compiler

First, you need to provide technology-dependent implementations of some of the cells which require specific instantiation.

Then, please provide a set_libs.tcl and set_constraints.tcl scripts to set link and target libraries, and constraints as the clock.

To generate and run synthesis scripts with DC, execute:

```
$ fusesoc --cores-root . run --no-export --target=asic_synthesis --setup --build eslepfl::heepocrates 2>&1 | tee buildsim.log
```

This relies on a fork of [edalize](https://github.com/davideschiavone/edalize) that contains templates for Design Compiler.

## Change Peripheral Memory Map

TODO: Create a template file to configure an heepocrates obi and periphearl bus that is external to x-heep.

## Files are formatted with Verible

We use version v0.0-1824-ga3b5bedf

See: [Install Verible](https://docs.opentitan.org/doc/ug/install_instructions/)

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

## Simulating

This project supports simulation only with Questasim as tapeouts should be signed-off with commercial EDA tools.
This way the developers are forced to used Questasim to validate their changes.
In addition, Verilator is not compatible with the FLL and the new UART DPI which measures the FLL frequency automatically.

### Compiling for Questasim

To simulate your application with Questasim compile the HDL:

```
make questasim-sim-opt
```

then, go to your target system built folder

```
cd ./build/eslepfl__heepocrates_0/sim-modelsim/
```

and type to run your compiled software:

```
make run RUN_OPT=1 PLUSARGS="c firmware=../../../sw/x_applications/hello_world/hello_world.hex"
```

or if you want to execute from flash:

```
make run RUN_OPT=1 PLUSARGS="c firmware=../../../sw/x_applications/hello_world/hello_world.flash_exec.hex boot_sel=1"
```

or if you want to load from flash:

```
make run RUN_OPT=1 PLUSARGS="c firmware=../../../sw/x_applications/hello_world/hello_world.flash_load.hex boot_sel=1 execute_from_flash=0"
```


If you want to bypass the FLL:

```
make run RUN_OPT=1 PLUSARGS="c firmware=../../../sw/x_applications/hello_world/hello_world.hex bypass_fll=1"
```


If you want to use the `UPF`

```
make questasim-sim-opt-upf FUSESOC_FLAGS="--flag=use_upf"
```

and then execute software as:

```
make run RUN_OPT=1 RUN_UPF=1 PLUSARGS="c firmware=../../../sw/x_applications/example_power_gating_core/example_power_gating_core.hex"
```

### UART DPI

To simulate the UART, we use the LowRISC OpenTitan [UART DPI](https://github.com/lowRISC/opentitan/tree/master/hw/dv/dpi/uartdpi).
Read how to interact with it in the Section "Interact with the simulated UART" [here](https://docs.opentitan.org/doc/ug/getting_started_verilator/).
The output of the UART DPI module is printed in the `uart0.log` file in the simulation folder.

For example, to see the "hello world!" output of the Verilator simulation:

```
cd ./build/eslepfl__heepocrates_0/sim-modelsim
make run PLUSARGS="c firmware=../../../sw/x_applications/hello_world/hello_world.hex"
cat uart0.log
```
### Synthesis with Synopsys Design Compiler

Only if you are running on eslsrv:

Please provide a set_libs.tcl and set_constraints.tcl scripts to set link and target libraries, and constraints as the clock.

To generate and run synthesis scripts with DC, execute:

```
make synthesis
```

This relies on a fork of [edalize](https://github.com/davideschiavone/edalize) that contains templates for Design Compiler.


### Postsynthesis Simulation for Questasim

To simulate your application with Questasim compile the `netlist` you generated before as:

```
make questasim-sim-postsynth-opt
```

then, go to your target system built folder

```
cd ./build/eslepfl__heepocrates_0/sim_postsynthesis-modelsim/
```

and type to run your compiled software:

to boot with jtag:

```
make run RUN_OPT=1 PLUSARGS="c firmware=../../../sw/x_applications/hello_world/hello_world.hex"
```

or to execute from flash:

```
make run RUN_OPT=1 PLUSARGS="c firmware=../../../sw/x_applications/hello_world/hello_world.flash_exec.hex boot_sel=1"
```

or to load from flash to memory:

```
make run RUN_OPT=1 PLUSARGS="c firmware=../../../sw/x_applications/hello_world/hello_world.flash_load.hex boot_sel=1 execute_from_flash=0"
```

If you want to debug it with the `gui`, as I strongly encourage, once inside `Questasim`


```
VSIM > do ../../../scripts/sim/waves_postsynth.do
```

This shows the most important post synthesis waveforms.


The simulation is very slow... `be patience`.

`UPF` simulation post-synthesis is not supported


### Place and Route with Innovus

Only if you are running on eslsrv:

```
make pnr_floorplan
```

for only creating the floorplan of `heepocrates`, otheriwise

```
make pnr
```

This relies on the [Google Sheet](https://docs.google.com/spreadsheets/d/1R42f33qJquhNsswMwyr-gp6fZFzjrIKRiU6ttj8dK-8/edit#gid=1605553209) that contains the IO and on the synthesized `netlist`.

### Postfloorplan Simulation for Questasim

To `verify` whether your power domains that you specified correctly, you must run at least post-floorplan simulation,
where the power switches are physically connected to your netlist.

To do so:

```
make questasim-sim-postfloorplan-opt
```
then, go to your target system built folder

```
cd ./build/eslepfl__heepocrates_0/sim_postlayout-modelsim/
```

and follow what was described above for the post-synthesis simulation.

### PostPnR Simulation for Questasim

To `verify` whether your your netlist has been placed and routed correctly, and no setup, nor hold violations are still present,
you must simulate the netlist with back-annotated `timing` (`SDF`) information.
Note that you are still simulating the `verilog` of the `hard` macros as:

- `memories from ARM`
- `FLL from ETH`
- `memory switch cells from us`
- `coubertin from us`

To do so:

```
make questasim-sim-postlayout-opt
```
then, go to your target system built folder

```
cd ./build/eslepfl__heepocrates_0/sim_postlayout-modelsim/
```

and follow what was described above for the post-synthesis simulation.
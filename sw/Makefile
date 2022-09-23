# Copyright 2019 Clifford Wolf
# Copyright 2019 Robert Balas
#
# Permission to use, copy, modify, and/or distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
# REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
# AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
# INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
# LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
# OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
# PERFORMANCE OF THIS SOFTWARE.

# Author: Robert Balas (balasr@iis.ee.ethz.ch)


MAKE			= make

# riscv toolchain install path
RISCV                    ?= ~/.riscv
RISCV_EXE_PREFIX         = $(RISCV)/bin/riscv32-unknown-elf-

TARGET                   ?= use_fll

# GCC configuration
CUSTOM_GCC_FLAGS           =

LIB_CRT                    = $(wildcard x_heep_device/lib/crt/*.S)
LIB_CRT_SPIFLASH           = $(wildcard x_heep_device/lib/crt_spiflash/*.S)
LIB_CRT_SPIHOST            = $(wildcard x_heep_device/lib/crt_spihost/*.S)
LIB_BASE                   = $(wildcard x_heep_device/lib/base/*.c)
LIB_BASE_FREESTD           = x_heep_device/lib/base/freestanding
LIB_RUNTIME                = $(wildcard x_heep_device/lib/runtime/*.c)
LIB_DRIVERS                = $(wildcard x_heep_device/lib/drivers/soc_ctrl/*.c)
LIB_DRIVERS                += $(wildcard x_heep_device/lib/drivers/uart/*.c)
LIB_DRIVERS                += $(wildcard x_heep_device/lib/drivers/rv_plic/*.c)
LIB_DRIVERS                += $(wildcard x_heep_device/lib/drivers/rv_timer/*.c)
LIB_DRIVERS                += $(wildcard x_heep_device/lib/drivers/gpio/*.c)
LIB_DRIVERS                += $(wildcard x_heep_device/lib/drivers/i2c/*.c)
LIB_DRIVERS                += $(wildcard x_heep_device/lib/drivers/dma/*.c)
LIB_DRIVERS                += $(wildcard x_heep_device/lib/drivers/power_manager/*.c)
LIB_DRIVERS                += $(wildcard x_heep_device/lib/drivers/spi_mem/*.c)
LIB_DRIVERS                += $(wildcard x_heep_device/lib/drivers/spi_host/*.c)
LIB_DRIVERS                += $(wildcard device/lib/drivers/cgra/*.c)
INC_FOLDERS                = $(sort $(dir $(wildcard device/target/$(TARGET)/)))
INC_FOLDERS                += $(sort $(dir $(wildcard x_heep_device/lib/drivers/*/)))
INC_FOLDERS                += $(sort $(dir $(wildcard x_heep_device/lib/base/)))
INC_FOLDERS                += $(sort $(dir $(wildcard x_heep_device/lib/runtime/)))
INC_FOLDERS                += $(sort $(dir $(wildcard device/lib/drivers/*/)))
INC_FOLDERS                += $(sort $(dir $(wildcard device/lib/runtime/)))
INC_FOLDERS_GCC             = $(addprefix -I ,$(INC_FOLDERS))

# rules to generate hex (loadable by simulators) from elf
%.hex: %.elf
	$(RISCV_EXE_PREFIX)objcopy -O verilog $< $@

%.flash.hex: %.flash.elf
	$(RISCV_EXE_PREFIX)objcopy -O verilog --adjust-vma=-0x40000000 $< $@

%.spihost.hex: %.spihost.elf
	$(RISCV_EXE_PREFIX)objcopy -O verilog --adjust-vma=-0x40000000 $< $@

%.dis: %.elf
	$(RISCV_EXE_PREFIX)objdump -S $^ > $@

%.dump: %.elf
	$(RISCV_EXE_PREFIX)objdump -xD $^ > $@


# Running custom programs:
# This is an example for running a Hello World in the testbench
# We link with our custom crt0.s and syscalls.c against newlib so that we can
# use the c standard library
x_applications/hello_world/hello_world.elf: x_applications/hello_world/hello_world.c
	$(RISCV_EXE_PREFIX)gcc -march=rv32imc -o $@ -w -Os -g -nostdlib \
		$(CUSTOM_GCC_FLAGS) \
		-DHOST_BUILD \
		-T x_heep_linker/link.ld \
		-I $(RISCV)/riscv32-unknown-elf/include \
		$(INC_FOLDERS_GCC) \
		-static \
		$(LIB_CRT) \
		$^ $(LIB_RUNTIME) \
		$(LIB_BASE) \
		$(LIB_DRIVERS) \
		-L $(RISCV)/riscv32-unknown-elf/lib \
		-lc -lm -lgcc -flto -ffunction-sections -fdata-sections -specs=nano.specs

x_applications/example_power_gating_core/example_power_gating_core.elf: x_applications/example_power_gating_core/example_power_gating_core.c
	$(RISCV_EXE_PREFIX)gcc -march=rv32imc -o $@ -w -Os -g -nostdlib \
		$(CUSTOM_GCC_FLAGS) \
		-DHOST_BUILD \
		-T x_heep_linker/link.ld \
		-I $(RISCV)/riscv32-unknown-elf/include \
		$(INC_FOLDERS_GCC) \
		-static \
		$(LIB_CRT) \
		$^ $(LIB_RUNTIME) \
		$(LIB_BASE) \
		$(LIB_DRIVERS) \
		-L $(RISCV)/riscv32-unknown-elf/lib \
		-lc -lm -lgcc -flto -ffunction-sections -fdata-sections -specs=nano.specs

x_applications/hello_world/hello_world.flash.elf: x_applications/hello_world/hello_world.c
	$(RISCV_EXE_PREFIX)gcc -march=rv32imc -o $@ -w -Os -g -nostdlib \
		$(CUSTOM_GCC_FLAGS) \
		-DHOST_BUILD \
		-T x_heep_linker/link_spiflash.ld \
		-I $(RISCV)/riscv32-unknown-elf/include \
		$(INC_FOLDERS_GCC) \
		-static \
		$(LIB_CRT_SPIFLASH) \
		$^ $(LIB_RUNTIME) \
		$(LIB_BASE) \
		$(LIB_DRIVERS) \
		-L $(RISCV)/riscv32-unknown-elf/lib \
		-lc -lm -lgcc -flto -ffunction-sections -fdata-sections -specs=nano.specs

x_applications/hello_world/hello_world.spihost.elf: x_applications/hello_world/hello_world.c
	$(RISCV_EXE_PREFIX)gcc -march=rv32imc -o $@ -w -Os -g -nostdlib \
		$(CUSTOM_GCC_FLAGS) \
		-DHOST_BUILD \
		-T x_heep_linker/link_spihost.ld \
		-I $(RISCV)/riscv32-unknown-elf/include \
		$(INC_FOLDERS_GCC) \
		-static \
		$(LIB_CRT_SPIHOST) \
		$^ $(LIB_RUNTIME) \
		$(LIB_BASE) \
		$(LIB_DRIVERS) \
		-Wl,-Map=x_applications/hello_world/hello_world.spihost.map \
		-L $(RISCV)/riscv32-unknown-elf/lib \
		-lc -lm -lgcc -flto -ffunction-sections -fdata-sections -specs=nano.specs


applications/blade_conv_16bits/blade_conv_16bits.elf: applications/blade_conv_16bits/blade_conv_16bits.c
	$(RISCV_EXE_PREFIX)gcc -march=rv32imc -o $@ -w -Os -g -nostdlib \
		$(CUSTOM_GCC_FLAGS) \
		-DHOST_BUILD \
		-T x_heep_linker/link.ld \
		-I $(RISCV)/riscv32-unknown-elf/include \
		$(INC_FOLDERS_GCC) \
		-static \
		$(LIB_CRT) \
		applications/blade_conv_16bits/blade_basic_op.c \
		applications/blade_conv_16bits/conv.c \
		$^ $(LIB_RUNTIME) \
		$(LIB_BASE) \
		$(LIB_DRIVERS) \
		-L $(RISCV)/riscv32-unknown-elf/lib \
		-lc -lm -lgcc -flto -ffunction-sections -fdata-sections -specs=nano.specs


applications/cgra_func_test/main.elf: applications/cgra_func_test/main.c applications/cgra_func_test/fxp.c
	$(RISCV_EXE_PREFIX)gcc -march=rv32imc -o $@ -w -Os -g -nostdlib \
		$(CUSTOM_GCC_FLAGS) \
		-DHOST_BUILD \
		-T x_heep_linker/link.ld \
		-I $(RISCV)/riscv32-unknown-elf/include \
		$(INC_FOLDERS_GCC) \
		-static \
		$(LIB_CRT) \
		$^ $(LIB_RUNTIME) \
		$(LIB_BASE) \
		$(LIB_DRIVERS) \
		-L $(RISCV)/riscv32-unknown-elf/lib \
		-lc -lm -lgcc -flto -ffunction-sections -fdata-sections -specs=nano.specs

applications/cgra_dbl_min/main.elf: applications/cgra_dbl_min/main.c
	$(RISCV_EXE_PREFIX)gcc -march=rv32imc -o $@ -w -Os -g -nostdlib \
		$(CUSTOM_GCC_FLAGS) \
		-DHOST_BUILD \
		-T x_heep_linker/link.ld \
		-I $(RISCV)/riscv32-unknown-elf/include \
		$(INC_FOLDERS_GCC) \
		-static \
		$(LIB_CRT) \
		$^ $(LIB_RUNTIME) \
		$(LIB_BASE) \
		$(LIB_DRIVERS) \
		-L $(RISCV)/riscv32-unknown-elf/lib \
		-lc -lm -lgcc -flto -ffunction-sections -fdata-sections -specs=nano.specs

clean:
	rm -rf applications/*/*.elf \
	rm -rf applications/*/*.hex \
	rm -rf applications/*/*.dis \
	rm -rf applications/*/*.dump \
	rm -rf x_applications/*/*.elf \
	rm -rf x_applications/*/*.hex \
	rm -rf x_applications/*/*.dis \
	rm -rf x_applications/*/*.dump
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


MAKE                       = make

# riscv toolchain install path
RISCV                      ?= ~/.riscv
RISCV_EXE_PREFIX           = $(RISCV)/bin/riscv32-unknown-elf-

TARGET                     ?= use_fll

# GCC configuration
CUSTOM_GCC_FLAGS           =

LIB_CRT                    = $(wildcard x_heep_device/lib/crt/*.S)
LIB_CRT_FLASH_EXEC         = $(wildcard x_heep_device/lib/crt_flash_exec/*.S)
LIB_CRT_FLASH_LOAD         = $(wildcard x_heep_device/lib/crt_flash_load/*.S)
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
LIB_DRIVERS                += $(wildcard x_heep_device/lib/drivers/fast_intr_ctrl/*.c)
LIB_DRIVERS                += $(wildcard x_heep_device/lib/drivers/spi_mem/*.c)
LIB_DRIVERS                += $(wildcard x_heep_device/lib/drivers/spi_host/*.c)
LIB_DRIVERS                += $(wildcard x_heep_device/lib/drivers/pad_control/*.c)
LIB_DRIVERS                += $(wildcard device/lib/drivers/cgra/*.c)
LIB_DRIVERS                += $(wildcard device/lib/drivers/fll/*.c)
LIB_DRIVERS                += $(wildcard device/lib/drivers/coubertin/*.c)
LIB_DRIVERS                += $(wildcard device/lib/drivers/heepocrates_ctrl/*.c)
INC_FOLDERS                = $(sort $(dir $(wildcard device/target/$(TARGET)/)))
INC_FOLDERS                += $(sort $(dir $(wildcard x_heep_device/lib/drivers/*/)))
INC_FOLDERS                += $(sort $(dir $(wildcard x_heep_device/lib/base/)))
INC_FOLDERS                += $(sort $(dir $(wildcard x_heep_device/lib/runtime/)))
INC_FOLDERS                += $(sort $(dir $(wildcard device/lib/crt/)))
INC_FOLDERS                += $(sort $(dir $(wildcard device/lib/drivers/*/)))
INC_FOLDERS                += $(sort $(dir $(wildcard device/lib/runtime/)))
INC_FOLDERS_GCC            = $(addprefix -I ,$(INC_FOLDERS))

# Rules to generate hex (loadable by simulators) from elf

# We keep the same name to be transparent but we create an Srecord file when booting with jtag
%.hex: %.elf
	$(RISCV_EXE_PREFIX)objcopy --srec-forceS3 --srec-len 1 -O srec $^ $@

%.flash_exec.hex: %.flash_exec.elf
	$(RISCV_EXE_PREFIX)objcopy -O verilog --adjust-vma=-0x40000000 $< $@

%.flash_load.hex: %.flash_load.elf
	$(RISCV_EXE_PREFIX)objcopy -O verilog --adjust-vma=-0x40000000 $< $@

%.dis: %.elf
	$(RISCV_EXE_PREFIX)objdump -S $^ > $@

%.dump: %.elf
	$(RISCV_EXE_PREFIX)objdump -xD $^ > $@

# Running custom programs:
# This is an example for running a Hello World in the testbench
# We link with our custom crt0.s and syscalls.c against newlib so that we can
# use the c standard library
applications/seiz_trans/seiz_trans.flash_load.elf: applications/seiz_trans/main.c
	$(RISCV_EXE_PREFIX)gcc -march=rv32imc -o $@ -w -Os -g -nostdlib \
		$(CUSTOM_GCC_FLAGS) \
		-DHOST_BUILD \
		-DEXTERNAL_CRTO \
		-T x_heep_linker/link_flash_load.ld \
		-I $(RISCV)/riscv32-unknown-elf/include \
		$(INC_FOLDERS_GCC) \
		-static \
		$(LIB_CRT_FLASH_LOAD) \
		$^ $(LIB_RUNTIME) \
		$ applications/seiz_trans/performance.c \
		$ applications/seiz_trans/multiply_cgra.c \
		$ applications/seiz_trans/weightsAndBiasesC.c \
		$ applications/seiz_trans/transformerBlockC.c \
		$ applications/seiz_trans/addNormC.c \
		$ applications/seiz_trans/dense_layerC.c \
		$ applications/seiz_trans/tokenPosEmbeddingC.c \
		$ applications/seiz_trans/selfattentionC.c \
		$ applications/seiz_trans/transposeC.c \
		$ applications/seiz_trans/matMulC.c \
		$ applications/seiz_trans/softmaxC.c \
		$(LIB_BASE) \
		$(LIB_DRIVERS) \
		-Wl,-Map=applications/seiz_trans/seiz_trans.spihost.map \
		-L $(RISCV)/riscv32-unknown-elf/lib \
		-lc -lm -lgcc -flto -ffunction-sections -fdata-sections -specs=nano.specs

# adding for seiz_trans_wo_cgra
applications/seiz_trans_wo_cgra/seiz_trans_wo_cgra.flash_load.elf: applications/seiz_trans_wo_cgra/main.c
	$(RISCV_EXE_PREFIX)gcc -march=rv32imc -o $@ -w -Os -g -nostdlib \
		$(CUSTOM_GCC_FLAGS) \
		-DHOST_BUILD \
		-DEXTERNAL_CRTO \
		-T x_heep_linker/link_flash_load.ld \
		-I $(RISCV)/riscv32-unknown-elf/include \
		$(INC_FOLDERS_GCC) \
		-static \
		$(LIB_CRT_FLASH_LOAD) \
		$^ $(LIB_RUNTIME) \
		$ applications/seiz_trans_wo_cgra/performance.c \
		$ applications/seiz_trans_wo_cgra/weightsAndBiasesC.c \
		$ applications/seiz_trans_wo_cgra/transformerBlockC.c \
		$ applications/seiz_trans_wo_cgra/addNormC.c \
		$ applications/seiz_trans_wo_cgra/dense_layerC.c \
		$ applications/seiz_trans_wo_cgra/tokenPosEmbeddingC.c \
		$ applications/seiz_trans_wo_cgra/selfattentionC.c \
		$ applications/seiz_trans_wo_cgra/transposeC.c \
		$ applications/seiz_trans_wo_cgra/matMulC.c \
		$ applications/seiz_trans_wo_cgra/softmaxC.c \
		$(LIB_BASE) \
		$(LIB_DRIVERS) \
		-Wl,-Map=applications/seiz_trans_wo_cgra/seiz_trans_wo_cgra.spihost.map \
		-L $(RISCV)/riscv32-unknown-elf/lib \
		-lc -lm -lgcc -flto -ffunction-sections -fdata-sections -specs=nano.specs


applications/hello_world/hello_world.flash_exec.elf: applications/hello_world/main.c
	$(RISCV_EXE_PREFIX)gcc -march=rv32imc -o $@ -w -Os -g -nostdlib \
		$(CUSTOM_GCC_FLAGS) \
		-DHOST_BUILD \
		-DEXTERNAL_CRTO \
		-T x_heep_linker/link_flash_exec.ld \
		-I $(RISCV)/riscv32-unknown-elf/include \
		$(INC_FOLDERS_GCC) \
		-static \
		$(LIB_CRT_FLASH_EXEC) \
		$^ $(LIB_RUNTIME) \
		$(LIB_BASE) \
		$(LIB_DRIVERS) \
		-L $(RISCV)/riscv32-unknown-elf/lib \
		-lc -lm -lgcc -flto -ffunction-sections -fdata-sections -specs=nano.specs


applications/hello_world/hello_world.flash_load.elf: applications/hello_world/main.c
	$(RISCV_EXE_PREFIX)gcc -march=rv32imc -o $@ -w -Os -g -nostdlib \
		$(CUSTOM_GCC_FLAGS) \
		-DHOST_BUILD \
		-DEXTERNAL_CRTO \
		-T x_heep_linker/link_flash_load.ld \
		-I $(RISCV)/riscv32-unknown-elf/include \
		$(INC_FOLDERS_GCC) \
		-static \
		$(LIB_CRT_FLASH_LOAD) \
		$^ $(LIB_RUNTIME) \
		$(LIB_BASE) \
		$(LIB_DRIVERS) \
		-Wl,-Map=applications/hello_world/hello_world.spihost.map \
		-L $(RISCV)/riscv32-unknown-elf/lib \
		-lc -lm -lgcc -flto -ffunction-sections -fdata-sections -specs=nano.specs

applications/hello_world/hello_world.elf: applications/hello_world/main.c
	$(RISCV_EXE_PREFIX)gcc -march=rv32imc -o $@ -w -Os -g -nostdlib \
		$(CUSTOM_GCC_FLAGS) \
		-DHOST_BUILD \
		-DEXTERNAL_CRTO \
		-T x_heep_linker/link.ld \
		-I $(RISCV)/riscv32-unknown-elf/include \
		$(INC_FOLDERS_GCC) \
		-static \
		$(LIB_CRT) \
		$^ $(LIB_RUNTIME) \
		$(LIB_BASE) \
		$(LIB_DRIVERS) \
		-L $(RISCV)/riscv32-unknown-elf/lib \
		-Wl,-Map=applications/hello_world/hello_world.map \
		-lc -lm -lgcc -flto -ffunction-sections -fdata-sections -specs=nano.specs


applications/matmul/matmul.flash_load.elf: applications/matmul/main.c
	$(RISCV_EXE_PREFIX)gcc -march=rv32imc -o $@ -w -Os -g -nostdlib \
		$(CUSTOM_GCC_FLAGS) \
		-DHOST_BUILD \
		-DEXTERNAL_CRTO \
		-T x_heep_linker/link_flash_load.ld \
		-I $(RISCV)/riscv32-unknown-elf/include \
		$(INC_FOLDERS_GCC) \
		-static \
		$(LIB_CRT_FLASH_LOAD) \
		$^ $(LIB_RUNTIME) \
		$(LIB_BASE) \
		$(LIB_DRIVERS) \
		-Wl,-Map=applications/matmul/matmul.spihost.map \
		-L $(RISCV)/riscv32-unknown-elf/lib \
		-lc -lm -lgcc -flto -ffunction-sections -fdata-sections -specs=nano.specs

clean:
	rm -rf applications/*/*.elf \
	rm -rf applications/*/*.hex \
	rm -rf applications/*/*.dis \
	rm -rf applications/*/*.map \
	rm -rf applications/*/*.dump \
	rm -rf x_applications/*/*.elf \
	rm -rf x_applications/*/*.hex \
	rm -rf x_applications/*/*.dis \
	rm -rf x_applications/*/*.map \
	rm -rf x_applications/*/*.dump
	rm -rf applications/power_apps/*/*.elf \
	rm -rf applications/power_apps/*/*.hex \
	rm -rf applications/power_apps/*/*.dis \
	rm -rf applications/power_apps/*/*.map \
	rm -rf applications/power_apps/*/*.dump \

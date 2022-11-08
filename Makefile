# Copyright EPFL contributors.
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0

# Makefile to generates core-v-mini-mcu files and build the design with fusesoc

.PHONY: clean help

# Generates mcu files
# every other target MUST also launch this one to be sure we are not missing it
# we are not configurint Heepocrates anyway as we hardwire the cpu and bus values

###############
###############
### CAREFUL ###
###############
###############
############################################################################################################################
### heepocrates_testharness heepocrates-gen is done with 2 external domains only as the 3rd ACK signal is handled in RTL ###
############################################################################################################################
mcu-gen:
	cd hw/vendor/esl_epfl_x_heep && \
	python util/mcu_gen.py --cfg mcu_cfg.hjson --outdir ../../../tb --external_domains 2 --pkg-sv ../../../tb/heepocrates_testharness.sv.tpl && \
	python util/mcu_gen.py --cfg mcu_cfg.hjson --outdir hw/core-v-mini-mcu/include --cpu cv32e20 --bus NtoM --memorybanks 10 --external_domains 3 --pkg-sv hw/core-v-mini-mcu/include/core_v_mini_mcu_pkg.sv.tpl  --external_pads ../../../heepocrates_pad.hjson && \
	python util/mcu_gen.py --cfg mcu_cfg.hjson --outdir hw/core-v-mini-mcu/ --memorybanks 10 --external_domains 3 --tpl-sv hw/core-v-mini-mcu/system_bus.sv.tpl  && \
	python util/mcu_gen.py --cfg mcu_cfg.hjson --outdir hw/core-v-mini-mcu/ --external_domains 3 --tpl-sv hw/core-v-mini-mcu/core_v_mini_mcu.sv.tpl && \
	python util/mcu_gen.py --cfg mcu_cfg.hjson --outdir ../../../hw/heepocrates/ --external_domains 3 --memorybanks 10  --tpl-sv ../../../hw/heepocrates/heepocrates_pad_ring.sv.tpl --external_pads ../../../heepocrates_pad.hjson && \
	python util/mcu_gen.py --cfg mcu_cfg.hjson --outdir ../../../hw/heepocrates/ --external_domains 3 --memorybanks 10  --tpl-sv ../../../hw/heepocrates/heepocrates.sv.tpl --external_pads ../../../heepocrates_pad.hjson && \
	python util/mcu_gen.py --cfg mcu_cfg.hjson --outdir tb/ --memorybanks 10 --external_domains 3 --tpl-sv tb/tb_util.svh.tpl  && \
	python util/mcu_gen.py --cfg mcu_cfg.hjson --outdir sw/device/lib/runtime --cpu cv32e20 --memorybanks 10 --external_domains 3 --pkg-sv sw/device/lib/runtime/core_v_mini_mcu.h.tpl  && \
	python util/mcu_gen.py --cfg mcu_cfg.hjson --outdir sw/linker --memorybanks 10 --linker_script sw/linker/link.ld.tpl  && \
	python util/mcu_gen.py --cfg mcu_cfg.hjson --outdir sw/linker --memorybanks 10 --linker_script sw/linker/link_flash_exec.ld.tpl && \
	python util/mcu_gen.py --cfg mcu_cfg.hjson --outdir sw/linker --memorybanks 10 --linker_script sw/linker/link_flash_load.ld.tpl && \
	python util/mcu_gen.py --cfg mcu_cfg.hjson --outdir hw/ip/power_manager/rtl --memorybanks 10 --external_domains 3 --pkg-sv hw/ip/power_manager/data/power_manager.sv.tpl && \
	python util/mcu_gen.py --cfg mcu_cfg.hjson --outdir hw/ip/power_manager/data --memorybanks 10 --external_domains 3 --pkg-sv hw/ip/power_manager/data/power_manager.hjson.tpl && \
	bash -c "cd hw/ip/power_manager; source power_manager_gen.sh; cd ../../../" && \
	python util/mcu_gen.py --cfg mcu_cfg.hjson --outdir sw/device/lib/drivers/power_manager --memorybanks 10 --external_domains 3 --pkg-sv sw/device/lib/drivers/power_manager/data/power_manager.h.tpl && \
	python util/mcu_gen.py --cfg mcu_cfg.hjson --outdir hw/system/pad_control/data --external_domains 3 --pkg-sv hw/system/pad_control/data/pad_control.hjson.tpl  --external_pads ../../../heepocrates_pad.hjson && \
	python util/mcu_gen.py --cfg mcu_cfg.hjson --outdir hw/system/pad_control/rtl --external_domains 3 --pkg-sv hw/system/pad_control/rtl/pad_control.sv.tpl  --external_pads ../../../heepocrates_pad.hjson && \
	bash -c "cd hw/system/pad_control; source pad_control_gen.sh; cd ../../../"

heepocrates-gen: mcu-gen
	python util/heepocrates_gen.py --cfg heepocrates_cfg.hjson --outdir hw/heepocrates/include --pkg-sv hw/heepocrates/include/heepocrates_pkg.sv.tpl;
	python util/heepocrates_gen.py --cfg heepocrates_cfg.hjson --outdir sw/device/lib/runtime --pkg-sv sw/device/lib/runtime/heepocrates.h.tpl;
	$(MAKE) verible

questasim-sim: heepocrates-gen
	fusesoc --cores-root . run --no-export --target=sim --tool=modelsim --flag=integrated_heep $(FUSESOC_FLAGS) --setup --build eslepfl::heepocrates 2>&1 | tee buildsim.log

questasim-sim-opt: questasim-sim
	$(MAKE) -C build/eslepfl__heepocrates_0/sim-modelsim/ opt

questasim-sim-tsmc65-opt: heepocrates-gen
	fusesoc --cores-root . run --no-export --target=sim_rtl_tsmc65 --tool=modelsim --flag=integrated_heep $(FUSESOC_FLAGS) --setup --build eslepfl::heepocrates 2>&1 | tee buildsim.log
	$(MAKE) -C build/eslepfl__heepocrates_0/sim_rtl_tsmc65-modelsim/ opt

questasim-sim-opt-upf: questasim-sim
	$(MAKE) -C build/eslepfl__heepocrates_0/sim-modelsim/ opt-upf

questasim-sim-postsynth-opt:
	fusesoc --cores-root . run --no-export --target=sim_postsynthesis --tool=modelsim $(FUSESOC_FLAGS) --setup --build eslepfl::heepocrates 2>&1 | tee buildsim.log
	$(MAKE) -C build/eslepfl__heepocrates_0/sim_postsynthesis-modelsim/ opt

questasim-sim-postsynth-opt-upf:
	fusesoc --cores-root . run --no-export --target=sim_postsynthesis --tool=modelsim $(FUSESOC_FLAGS) --setup --build eslepfl::heepocrates 2>&1 | tee buildsim.log
	$(MAKE) -C build/eslepfl__heepocrates_0/sim_postsynthesis-modelsim/ opt-upf

questasim-sim-postlayout-opt:
	fusesoc --cores-root . run --no-export --target=sim_postlayout --tool=modelsim $(FUSESOC_FLAGS) --setup --build eslepfl::heepocrates 2>&1 | tee buildsim.log
	$(MAKE) -C build/eslepfl__heepocrates_0/sim_postlayout-modelsim/ opt

synthesis: heepocrates-gen
	cd implementation/synthesis/lc_shell/ && lc_shell -f mem_lib2db.tcl -batch;
	cd implementation/synthesis/lc_shell/ && lc_shell -f fll_lib2db.tcl -batch;
	cd implementation/synthesis/lc_shell/ && lc_shell -f blade_lib2db.tcl -batch;
	cd hw/asic/mem_power_switches && tcsh compile_lib.csh;
	fusesoc --cores-root . run --no-export --target=asic_synthesis --setup --build eslepfl::heepocrates 2>&1 | tee builddc.log

synthesis_script_only: heepocrates-gen
	fusesoc --cores-root . run --no-export --target=asic_synthesis --setup eslepfl::heepocrates 2>&1 | tee builddc.log

synthesis_script_only: heepocrates-gen
	fusesoc --cores-root . run --no-export --target=asic_synthesis --setup eslepfl::heepocrates 2>&1 | tee buildsim.log

implementation/pnr/inputs/heepocrates.io:
	pushd implementation/pnr/inputs/ ; ./create_io_file_from_spreadsheet.py; popd;

pnr: implementation/pnr/inputs/heepocrates.io
	pushd implementation/pnr/ ; ./run_pnr_flow.csh; popd;

pnr_debug: implementation/pnr/inputs/heepocrates.io
	pushd implementation/pnr/ ; ./run_pnr_flow.csh debug; popd;

# Display mcu_gen.py help
mcu-gen-help:
	cd hw/vendor/esl_epfl_x_heep && \
	python util/mcu_gen.py -h;

heepocrates-gen-help:
	python util/heepocrates_gen.py -h

# Runs verible formating
verible:
	util/format-verible;

clean: clean-app clean-sim clean-pnr-io

clean-sim:
	@rm -rf build

clean-app:
	$(MAKE) -C sw clean

clean-pnr-io:
	rm -f implementation/pnr/inputs/heepocrates.io

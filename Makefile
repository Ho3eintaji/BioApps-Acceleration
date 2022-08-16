# Copyright EPFL contributors.
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0


# Makefile to generates core-v-mini-mcu files and build the design with fusesoc

.PHONY: clean help

# Generates mcu files
# every other target MUST also launch this one to be sure we are not missing it
# we are not configurint Heepocrates anyway as we hardwire the cpu and bus values
mcu-gen:
	cd hw/vendor/esl_epfl_x_heep && \
	python util/mcu_gen.py --cfg mcu_cfg.hjson --outdir hw/core-v-mini-mcu/include --cpu cv32e20 --bus NtoM --pkg-sv hw/core-v-mini-mcu/include/core_v_mini_mcu_pkg.sv.tpl;
	cd hw/vendor/esl_epfl_x_heep && \
	python util/mcu_gen.py --cfg mcu_cfg.hjson --outdir sw/device/lib/runtime --cpu cv32e20 --bus NtoM --pkg-sv sw/device/lib/runtime/core_v_mini_mcu.h.tpl;

heepocrates-gen: mcu-gen verible
	python util/heepocrates_gen.py --cfg heepocrates_cfg.hjson --outdir hw/heepocrates/include --pkg-sv hw/heepocrates/include/heepocrates_pkg.sv.tpl;
	python util/heepocrates_gen.py --cfg heepocrates_cfg.hjson --outdir sw/device/lib/runtime --pkg-sv sw/device/lib/runtime/heepocrates.h.tpl;

questasim-sim: heepocrates-gen
	fusesoc --cores-root . run --no-export --target=sim --tool=modelsim --setup --build eslepfl::heepocrates 2>&1 | tee buildsim.log

questasim-sim-opt: heepocrates-gen
	fusesoc --cores-root . run --no-export --target=sim_opt --tool=modelsim --setup --build eslepfl::heepocrates 2>&1 | tee buildsim.log

# Display mcu_gen.py help
mcu-gen-help:
	cd hw/vendor/esl_epfl_x_heep && \
	python util/mcu_gen.py -h;

heepocrates-gen-help:
	python util/heepocrates_gen.py -h

# Runs verible formating
verible:
	util/format-verible;

clean: clean-app clean-sim

clean-sim:
	@rm -rf build

clean-app:
	$(MAKE) -C sw clean

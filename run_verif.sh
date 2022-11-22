
# Init Conda
conda activate core-v-mini-mcu

# Run Fusesoc
if [[ $1 = "rtl" ]]
then
    type="rtl"
    fusesoc="questasim-sim-opt-upf"
    fusesoc_flags="--flag=use_upf"
    sim_folder="sim-modelsim"
    upf="RUN_UPF=1"
elif [[ $1 = "syn" ]]
then
    type="syn"
    fusesoc="questasim-sim-postsynth-opt"
    fusesoc_flags=""
    sim_folder="sim_postsynthesis-modelsim"
    upf=""
elif [[ $1 = "pnr" ]]
then
    type="pnr"
    fusesoc="questasim-sim-postlayout-opt"
    fusesoc_flags=""
    sim_folder="sim_postlayout_timing-modelsim"
    upf=""
else
    echo -e "ERROR: Wrong parameter!\n"
    return
fi

# Run Fusesoc
make $fusesoc FUSESOC_FLAGS="${fusesoc_flags}"

# Open output file
if test -f "run_verif_${type}_log.txt";
then
    rm run_verif_${type}_log.txt
fi
touch run_verif_${type}_log.txt
echo -e "APPLICATIONS OUTPUT:\n\n" >> ./run_verif_${type}_log.txt

# Run hello_world application
echo -e "Run hello_world application...\n" >> ./run_verif_${type}_log.txt
cd sw/
make clean x_applications/hello_world/hello_world.hex
cd ../build/eslepfl__heepocrates_0/${sim_folder}/
make run RUN_OPT=1 PLUSARGS="c firmware=../../../sw/x_applications/hello_world/hello_world.hex" ${upf}
cat uart0.log >> ../../../run_verif_${type}_log.txt
cd ../../../
echo -e "\n" >> ./run_verif_${type}_log.txt

# Run hello_world application (exec from flash)
echo -e "Run hello_world application (exec from flash)...\n" >> ./run_verif_${type}_log.txt
cd sw/
make clean x_applications/hello_world/hello_world.flash_exec.hex
cd ../build/eslepfl__heepocrates_0/${sim_folder}/
make run RUN_OPT=1 PLUSARGS="c firmware=../../../sw/x_applications/hello_world/hello_world.flash_exec.hex boot_sel=1 execute_from_flash=1" ${upf}
cat uart0.log >> ../../../run_verif_${type}_log.txt
cd ../../../
echo -e "\n" >> ./run_verif_${type}_log.txt

# Run hello_world application (load from flash)
echo -e "Run hello_world application (load from flash)...\n" >> ./run_verif_${type}_log.txt
cd sw/
make clean x_applications/hello_world/hello_world.flash_load.hex
cd ../build/eslepfl__heepocrates_0/${sim_folder}/
make run RUN_OPT=1 PLUSARGS="c firmware=../../../sw/x_applications/hello_world/hello_world.flash_load.hex boot_sel=1 execute_from_flash=0" ${upf}
cat uart0.log >> ../../../run_verif_${type}_log.txt
cd ../../../
echo -e "\n" >> ./run_verif_${type}_log.txt

# Run test_ram application
echo -e "Run test_ram application...\n" >> ./run_verif_${type}_log.txt
cd sw/
make clean applications/test_ram/test_ram.hex
cd ../build/eslepfl__heepocrates_0/${sim_folder}/
make run RUN_OPT=1 PLUSARGS="c firmware=../../../sw/applications/test_ram/test_ram.hex" ${upf}
cat uart0.log >> ../../../run_verif_${type}_log.txt
cd ../../../
echo -e "\n" >> ./run_verif_${type}_log.txt

# Run matadd application
echo -e "Run matadd application...\n" >> ./run_verif_${type}_log.txt
cd sw/
make clean x_applications/matadd/matadd.hex
cd ../build/eslepfl__heepocrates_0/${sim_folder}/
make run RUN_OPT=1 PLUSARGS="c firmware=../../../sw/x_applications/matadd/matadd.hex" ${upf}
cat uart0.log >> ../../../run_verif_${type}_log.txt
cd ../../../
echo -e "\n" >> ./run_verif_${type}_log.txt

# Run spi_host_example application (load from flash)
echo -e "Run spi_host_example application (load from flash)...\n" >> ./run_verif_${type}_log.txt
cd sw/
make clean x_applications/spi_host_example/spi_host_example.flash_load.hex
cd ../build/eslepfl__heepocrates_0/${sim_folder}/
make run RUN_OPT=1 PLUSARGS="c firmware=../../../sw/x_applications/spi_host_example/spi_host_example.flash_load.hex boot_sel=1 execute_from_flash=0" ${upf}
cat uart0.log >> ../../../run_verif_${type}_log.txt
cd ../../../
echo -e "\n" >> ./run_verif_${type}_log.txt

# Run dma_example application
echo -e "Run dma_example application...\n" >> ./run_verif_${type}_log.txt
cd sw/
make clean x_applications/dma_example/dma_example.hex
cd ../build/eslepfl__heepocrates_0/${sim_folder}/
make run RUN_OPT=1 PLUSARGS="c firmware=../../../sw/x_applications/dma_example/dma_example.hex" ${upf}
cat uart0.log >> ../../../run_verif_${type}_log.txt
cd ../../../
echo -e "\n" >> ./run_verif_${type}_log.txt

# Run spi_host_dma_example application (load from flash)
echo -e "Run spi_host_dma_example application (load from flash)...\n" >> ./run_verif_${type}_log.txt
cd sw/
make clean x_applications/spi_host_dma_example/spi_host_dma_example.flash_load.hex
cd ../build/eslepfl__heepocrates_0/${sim_folder}/
make run RUN_OPT=1 PLUSARGS="c firmware=../../../sw/x_applications/spi_host_dma_example/spi_host_dma_example.flash_load.hex boot_sel=1 execute_from_flash=0" ${upf}
cat uart0.log >> ../../../run_verif_${type}_log.txt
cd ../../../
echo -e "\n" >> ./run_verif_${type}_log.txt

# Run test_gpio_ao application
echo -e "Run test_gpio_ao application...\n" >> ./run_verif_${type}_log.txt
cd sw/
make clean applications/test_gpio_ao/test_gpio_ao.hex
cd ../build/eslepfl__heepocrates_0/${sim_folder}/
make run RUN_OPT=1 PLUSARGS="c firmware=../../../sw/applications/test_gpio_ao/test_gpio_ao.hex" ${upf}
cat uart0.log >> ../../../run_verif_${type}_log.txt
cd ../../../
echo -e "\n" >> ./run_verif_${type}_log.txt

# Run test_gpio application
echo -e "Run test_gpio application...\n" >> ./run_verif_${type}_log.txt
cd sw/
make clean applications/test_gpio/test_gpio.hex
cd ../build/eslepfl__heepocrates_0/${sim_folder}/
make run RUN_OPT=1 PLUSARGS="c firmware=../../../sw/applications/test_gpio/test_gpio.hex" ${upf}
cat uart0.log >> ../../../run_verif_${type}_log.txt
cd ../../../
echo -e "\n" >> ./run_verif_${type}_log.txt

# Run test_timers application
echo -e "Run test_timers application...\n" >> ./run_verif_${type}_log.txt
cd sw/
make clean applications/test_timers/test_timers.hex
cd ../build/eslepfl__heepocrates_0/${sim_folder}/
make run RUN_OPT=1 PLUSARGS="c firmware=../../../sw/applications/test_timers/test_timers.hex" ${upf}
cat uart0.log >> ../../../run_verif_${type}_log.txt
cd ../../../
echo -e "\n" >> ./run_verif_${type}_log.txt

# Run example_power_gating_core application
echo -e "Run example_power_gating_core application...\n" >> ./run_verif_${type}_log.txt
cd sw/
make clean x_applications/example_power_gating_core/example_power_gating_core.hex
cd ../build/eslepfl__heepocrates_0/${sim_folder}/
make run RUN_OPT=1 PLUSARGS="c firmware=../../../sw/x_applications/example_power_gating_core/example_power_gating_core.hex" ${upf}
cat uart0.log >> ../../../run_verif_${type}_log.txt
cd ../../../
echo -e "\n" >> ./run_verif_${type}_log.txt

# Run test_dma_power_gating application
echo -e "Run test_dma_power_gating application...\n" >> ./run_verif_${type}_log.txt
cd sw/
make clean applications/test_dma_power_gating/test_dma_power_gating.hex
cd ../build/eslepfl__heepocrates_0/${sim_folder}/
make run RUN_OPT=1 PLUSARGS="c firmware=../../../sw/applications/test_dma_power_gating/test_dma_power_gating.hex" ${upf}
cat uart0.log >> ../../../run_verif_${type}_log.txt
cd ../../../
echo -e "\n" >> ./run_verif_${type}_log.txt

# # Run spi_host_dma_power_gate_example application (load from flash)
# echo -e "Run spi_host_dma_power_gate_example application (load from flash)...\n" >> ./run_verif_${type}_log.txt
# cd sw/
# make clean x_applications/spi_host_dma_power_gate_example/spi_host_dma_power_gate_example.flash_load.hex
# cd ../build/eslepfl__heepocrates_0/${sim_folder}/
# make run RUN_OPT=1 PLUSARGS="c firmware=../../../sw/x_applications/spi_host_dma_power_gate_example/spi_host_dma_power_gate_example.flash_load.hex boot_sel=1 execute_from_flash=0" ${upf}
# cat uart0.log >> ../../../run_verif_${type}_log.txt
# cd ../../../
# echo -e "\n" >> ./run_verif_${type}_log.txt

# Run test_gpio_ao_power_gating application
echo -e "Run test_gpio_ao_power_gating application...\n" >> ./run_verif_${type}_log.txt
cd sw/
make clean applications/test_gpio_ao_power_gating/test_gpio_ao_power_gating.hex
cd ../build/eslepfl__heepocrates_0/${sim_folder}/
make run RUN_OPT=1 PLUSARGS="c firmware=../../../sw/applications/test_gpio_ao_power_gating/test_gpio_ao_power_gating.hex" ${upf}
cat uart0.log >> ../../../run_verif_${type}_log.txt
cd ../../../
echo -e "\n" >> ./run_verif_${type}_log.txt

# Run test_gpio_power_gating application
echo -e "Run test_gpio_power_gating application...\n" >> ./run_verif_${type}_log.txt
cd sw/
make clean applications/test_gpio_power_gating/test_gpio_power_gating.hex
cd ../build/eslepfl__heepocrates_0/${sim_folder}/
make run RUN_OPT=1 PLUSARGS="c firmware=../../../sw/applications/test_gpio_power_gating/test_gpio_power_gating.hex" ${upf}
cat uart0.log >> ../../../run_verif_${type}_log.txt
cd ../../../
echo -e "\n" >> ./run_verif_${type}_log.txt

# Run example_power_gating_periph application
echo -e "Run example_power_gating_periph application...\n" >> ./run_verif_${type}_log.txt
cd sw/
make clean x_applications/example_power_gating_periph/example_power_gating_periph.hex
cd ../build/eslepfl__heepocrates_0/${sim_folder}/
make run RUN_OPT=1 PLUSARGS="c firmware=../../../sw/x_applications/example_power_gating_periph/example_power_gating_periph.hex" ${upf}
cat uart0.log >> ../../../run_verif_${type}_log.txt
cd ../../../
echo -e "\n" >> ./run_verif_${type}_log.txt

# Run example_power_gating_ram_blocks application
echo -e "Run example_power_gating_ram_blocks application...\n" >> ./run_verif_${type}_log.txt
cd sw/
make clean x_applications/example_power_gating_ram_blocks/example_power_gating_ram_blocks.hex
cd ../build/eslepfl__heepocrates_0/${sim_folder}/
make run RUN_OPT=1 PLUSARGS="c firmware=../../../sw/x_applications/example_power_gating_ram_blocks/example_power_gating_ram_blocks.hex" ${upf}
cat uart0.log >> ../../../run_verif_${type}_log.txt
cd ../../../
echo -e "\n" >> ./run_verif_${type}_log.txt

# Run example_set_retentive_ram_blocks application
echo -e "Run example_set_retentive_ram_blocks application...\n" >> ./run_verif_${type}_log.txt
cd sw/
make clean x_applications/example_set_retentive_ram_blocks/example_set_retentive_ram_blocks.hex
cd ../build/eslepfl__heepocrates_0/${sim_folder}/
make run RUN_OPT=1 PLUSARGS="c firmware=../../../sw/x_applications/example_set_retentive_ram_blocks/example_set_retentive_ram_blocks.hex" ${upf}
cat uart0.log >> ../../../run_verif_${type}_log.txt
cd ../../../
echo -e "\n" >> ./run_verif_${type}_log.txt

# Print output file
echo -e "\n"
cat ./run_verif_${type}_log.txt

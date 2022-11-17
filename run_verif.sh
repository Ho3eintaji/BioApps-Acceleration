
# Init Conda
conda activate core-v-mini-mcu

# Run Fusesoc
if [[ $1 = "rtl" ]]
then
    type="rtl"
    fusesoc="questasim-sim-opt-upf"
    fusesoc_flags="--flag=use_upf --flag=use_external_device_example"
    sim_folder="sim-modelsim"
    upf="RUN_UPF=1"
elif [[ $1 = "syn" ]]
then
    type="syn"
    fusesoc="questasim-sim-postsynth-opt"
    fusesoc_flags="--flag=use_external_device_example"
    sim_folder="sim_postsynthesis-modelsim"
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

# Run matadd application
echo -e "Run matadd application...\n" >> ./run_verif_${type}_log.txt
cd sw/
make clean x_applications/matadd/matadd.hex
cd ../build/eslepfl__heepocrates_0/${sim_folder}/
make run RUN_OPT=1 PLUSARGS="c firmware=../../../sw/x_applications/matadd/matadd.hex" ${upf}
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

# Run example_gpio_cnt application
echo -e "Run example_gpio_cnt application...\n" >> ./run_verif_${type}_log.txt
cd sw/
make clean x_applications/example_gpio_cnt/example_gpio_cnt.hex
cd ../build/eslepfl__heepocrates_0/${sim_folder}/
make run RUN_OPT=1 PLUSARGS="c firmware=../../../sw/x_applications/example_gpio_cnt/example_gpio_cnt.hex" ${upf}
cat uart0.log >> ../../../run_verif_${type}_log.txt
cd ../../../
echo -e "\n" >> ./run_verif_${type}_log.txt

# Run gpio_pmw application
echo -e "Run gpio_pmw application...\n" >> ./run_verif_${type}_log.txt
cd sw/
make clean x_applications/gpio_pmw/gpio_pmw.hex
cd ../build/eslepfl__heepocrates_0/${sim_folder}/
make run RUN_OPT=1 PLUSARGS="c firmware=../../../sw/x_applications/gpio_pmw/gpio_pmw.hex" ${upf}
cat uart0.log >> ../../../run_verif_${type}_log.txt
cd ../../../
echo -e "\n" >> ./run_verif_${type}_log.txt

# Run spi_host_example application
echo -e "Run spi_host_example application...\n" >> ./run_verif_${type}_log.txt
cd sw/
make clean x_applications/spi_host_example/spi_host_example.hex
cd ../build/eslepfl__heepocrates_0/${sim_folder}/
make run RUN_OPT=1 PLUSARGS="c firmware=../../../sw/x_applications/spi_host_example/spi_host_example.hex" ${upf}
cat uart0.log >> ../../../run_verif_${type}_log.txt
cd ../../../
echo -e "\n" >> ./run_verif_${type}_log.txt

# Run spi_host_dma_example application
echo -e "Run spi_host_dma_example application...\n" >> ./run_verif_${type}_log.txt
cd sw/
make clean x_applications/spi_host_dma_example/spi_host_dma_example.hex
cd ../build/eslepfl__heepocrates_0/${sim_folder}/
make run RUN_OPT=1 PLUSARGS="c firmware=../../../sw/x_applications/spi_host_dma_example/spi_host_dma_example.hex" ${upf}
cat uart0.log >> ../../../run_verif_${type}_log.txt
cd ../../../
echo -e "\n" >> ./run_verif_${type}_log.txt

# Run example_external_peripheral application
echo -e "Run example_external_peripheral application...\n" >> ./run_verif_${type}_log.txt
cd sw/
make clean x_applications/example_external_peripheral/example_external_peripheral.hex
cd ../build/eslepfl__heepocrates_0/${sim_folder}/
make run RUN_OPT=1 PLUSARGS="c firmware=../../../sw/x_applications/example_external_peripheral/example_external_peripheral.hex" ${upf}
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

# Run spi_host_dma_power_gate_example application
echo -e "Run spi_host_dma_power_gate_example application...\n" >> ./run_verif_${type}_log.txt
cd sw/
make clean x_applications/spi_host_dma_power_gate_example/spi_host_dma_power_gate_example.hex
cd ../build/eslepfl__heepocrates_0/${sim_folder}/
make run RUN_OPT=1 PLUSARGS="c firmware=../../../sw/x_applications/spi_host_dma_power_gate_example/spi_host_dma_power_gate_example.hex" ${upf}
cat uart0.log >> ../../../run_verif_${type}_log.txt
cd ../../../
echo -e "\n" >> ./run_verif_${type}_log.txt

# Run example_power_gating_external application
echo -e "Run example_power_gating_external application...\n" >> ./run_verif_${type}_log.txt
cd sw/
make clean x_applications/example_power_gating_external/example_power_gating_external.hex
cd ../build/eslepfl__heepocrates_0/${sim_folder}/
make run RUN_OPT=1 PLUSARGS="c firmware=../../../sw/x_applications/example_power_gating_external/example_power_gating_external.hex" ${upf}
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

# Run example_set_retentive_external_ram_blocks application
echo -e "Run example_set_retentive_external_ram_blocks application...\n" >> ./run_verif_${type}_log.txt
cd sw/
make clean x_applications/example_set_retentive_external_ram_blocks/example_set_retentive_external_ram_blocks.hex
cd ../build/eslepfl__heepocrates_0/${sim_folder}/
make run RUN_OPT=1 PLUSARGS="c firmware=../../../sw/x_applications/example_set_retentive_external_ram_blocks/example_set_retentive_external_ram_blocks.hex" ${upf}
cat uart0.log >> ../../../run_verif_${type}_log.txt
cd ../../../
echo -e "\n" >> ./run_verif_${type}_log.txt

# Print output file
echo -e "\n"
cat ./run_verif_${type}_log.txt

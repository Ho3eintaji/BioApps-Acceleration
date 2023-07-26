// Copyright 2023 EPFL
// Solderpad Hardware License, Version 2.1, see LICENSE.md for details.
// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1

#include "heepocrates.h"
#include "core_v_mini_mcu.h"
#include "power_manager.h"
#include "matmul_data.h"
#include "gpio.h"
#include "soc_ctrl.h"
#include "rv_timer.h"

#define KEYWORD 0x0DA41DE

#define GPIO_TEST_OUT 0
#define GPIO_DEBUG_OUT 1
#define GPIO_DEBUG_MEM 2

#ifdef DEBUG_APPLICATION
    #pragma message ( "DEBUG_APPLICATION is defined, if the matmul fails it prints the expected value" )
#endif

#ifdef CHECK_RESULT_APPLICATION
    #pragma message ( "CHECK_RESULT_APPLICATION is defined, if the matmul fails it will let you know" )
#endif

#ifdef TOGGLE_GPIO
    #pragma message ( "TOGGLE_GPIO is defined, if MEASURE_POWER_APPLICATION is defined, then every 1000 matmul it toggles GPIO0" )
#endif

#ifdef MEASURE_POWER_APPLICATION
    #pragma message ( "MEASURE_POWER_APPLICATION is defined, the application never ends" )
#endif

#ifdef SWITCH_OFF_MEMORIES
    #pragma message ( "SWITCHING OFF BANKS 4-7" )
#endif

#ifdef MAKE_WFI
    #pragma message ( "Making WFI" )
#endif

#if POWER_GATE_CPU
    #pragma message ( "Power Gating CPU" )
#endif


int __attribute__ ((noinline)) matmul(int N, int* matrixM, int* matrixA, int * C, int* calculated_output)
{

    for(int i=0; i < N; i++){
        for(int j=0; j < N; j++){
            int c = 0;
            for(int k=0; k < N; k++){
                c += matrixM[i*N + k] * matrixA[k*N + j];
            }
            C[i*N + j] = c;
            #ifdef CHECK_RESULT_APPLICATION
            if(c != calculated_output[i*N + j]) {
		 #ifdef DEBUG_APPLICATION
                printf("Expected %d, got %d\n",c,calculated_output[i] );
                #endif //DEBUG_APPLICATION
                return -1;
            }
	    #endif
        }
    }

    return 0;
}

int output_C[OUTPUT_EXPECTED_C_SIZE];

static rv_timer_t timer_0_1;
static const uint64_t kTickFreqHz = 1000 * 1000; // 1 MHz

int main(int argc, char const *argv[])
{

    int matmul_result = 0;
    int gpio_val=0;
    int gpio_cnt = 0;

    gpio_t gpio;
    gpio_params_t gpio_params;
    gpio_params.base_addr = mmio_region_from_addr((uintptr_t)GPIO_AO_START_ADDRESS);

    gpio_init(gpio_params, &gpio);

    gpio_output_set_enabled(&gpio, GPIO_TEST_OUT, true);
    gpio_output_set_enabled(&gpio, GPIO_DEBUG_OUT, true);
    gpio_output_set_enabled(&gpio, GPIO_DEBUG_MEM, true);
    
    gpio_write(&gpio, GPIO_TEST_OUT, 0);
    gpio_write(&gpio, GPIO_DEBUG_OUT, 0);
    gpio_write(&gpio, GPIO_DEBUG_MEM, 0);

    volatile int * ptr_bank_off = 0x00028000;
    *ptr_bank_off = KEYWORD;


    power_manager_t power_manager;
    power_manager_counters_t power_manager_counters;
    mmio_region_t power_manager_reg = mmio_region_from_addr(POWER_MANAGER_START_ADDRESS);
    power_manager.base_addr = power_manager_reg;

    //counters
    uint32_t reset_off, reset_on, switch_off, switch_on, iso_off, iso_on;

    //Turn off: first, isolate the CPU outputs, then I reset it, then I switch it off (reset and switch off order does not really matter)
    iso_off = 10;
    reset_off = iso_off + 5000;
    switch_off = reset_off + 5;
    //Turn on: first, give back power by switching on, then deassert the reset, the unisolate the CPU outputs
    switch_on = 10;
    reset_on = switch_on + 20; //give 20 cycles to emulate the turn on time, this number depends on technology and here it is just a random number
    iso_on = 5;

    // Get current Frequency
    soc_ctrl_t soc_ctrl;
    soc_ctrl.base_addr = mmio_region_from_addr((uintptr_t)SOC_CTRL_START_ADDRESS);
    uint32_t freq_hz = soc_ctrl_get_frequency(&soc_ctrl);

    // Setup rv_timer_0_1
    mmio_region_t timer_0_1_reg = mmio_region_from_addr(RV_TIMER_AO_START_ADDRESS);
    rv_timer_init(timer_0_1_reg, (rv_timer_config_t){.hart_count = 1, .comparator_count = 1}, &timer_0_1);
    rv_timer_tick_params_t tick_params;
    rv_timer_approximate_tick_params(freq_hz, kTickFreqHz, &tick_params);

#ifdef SWITCH_OFF_MEMORIES

    if (power_gate_counters_init(&power_manager_counters, reset_off, reset_on, switch_off, switch_on, iso_off, iso_on, 0, 0) != kPowerManagerOk_e)
    {
	#ifdef DEBUG_APPLICATION
        printf("Error: power manager fail. Check the reset and powergate counters value\n");
	#endif //DEBUG_APPLICATION
        return -1;
    }

    if (power_gate_ram_block(&power_manager, 4, kOff_e, &power_manager_counters) != kPowerManagerOk_e)
    {
	#ifdef DEBUG_APPLICATION
        printf("Error: power manager fail.\n");
	#endif //DEBUG_APPLICATION
        return -1;
    }
    if (power_gate_ram_block(&power_manager, 5, kOff_e, &power_manager_counters) != kPowerManagerOk_e)
    {
	#ifdef DEBUG_APPLICATION
        printf("Error: power manager fail.\n");
	#endif //DEBUG_APPLICATION
        return -1;
    }
    if (power_gate_ram_block(&power_manager, 6, kOff_e, &power_manager_counters) != kPowerManagerOk_e)
    {
	#ifdef DEBUG_APPLICATION
        printf("Error: power manager fail.\n");
	#endif //DEBUG_APPLICATION
        return -1;
    }
    if (power_gate_ram_block(&power_manager, 7, kOff_e, &power_manager_counters) != kPowerManagerOk_e)
    {
	#ifdef DEBUG_APPLICATION
        printf("Error: power manager fail.\n");
	#endif //DEBUG_APPLICATION
        return -1;
    }
#endif


    int val = *ptr_bank_off;
#ifdef SWITCH_OFF_MEMORIES
    if (val == KEYWORD ) {
    	gpio_write(&gpio, GPIO_DEBUG_MEM, 1);
	asm volatile ("wfi");
    }
#else
    if (val != KEYWORD ) {
    	gpio_write(&gpio, GPIO_DEBUG_MEM, 1);
	asm volatile ("wfi");
    }
#endif

    #ifdef MEASURE_POWER_APPLICATION
    while(1) {
    #endif //MEASURE_POWER_APPLICATION
        gpio_cnt++;

	#ifdef MAKE_WFI
	asm volatile ("wfi");
	#elif POWER_GATE_CPU

	// Power-gate core and wake-up due to timer_0
	rv_timer_set_tick_params(&timer_0_1, 0, tick_params);
	rv_timer_irq_enable(&timer_0_1, 0, 0, kRvTimerEnabled);
	rv_timer_arm(&timer_0_1, 0, 0, 1024);
	rv_timer_counter_set_enabled(&timer_0_1, 0, kRvTimerEnabled);

	gpio_write(&gpio, GPIO_DEBUG_MEM, 0);

	power_gate_core(&power_manager, 0, &power_manager_counters);

	gpio_write(&gpio, GPIO_DEBUG_MEM, 1);
	
	matmul_result = matmul(MATMUL_RC_SIZE, input_M, input_A, output_C, output_expected_C);

	#else
        matmul_result = matmul(MATMUL_RC_SIZE, input_M, input_A, output_C, output_expected_C);
	#endif

	#ifdef CHECK_RESULT_APPLICATION
		if (matmul_result)
			gpio_write(&gpio, GPIO_DEBUG_OUT, 1);
	#endif

        #ifdef TOGGLE_GPIO
            if(gpio_cnt > 1000){
                gpio_val = !gpio_val;
                gpio_write(&gpio, GPIO_TEST_OUT, gpio_val);
                gpio_cnt = 0;
            }
        #endif // TOGGLE_GPIO


    #ifdef MEASURE_POWER_APPLICATION
    }
    #endif //MEASURE_POWER_APPLICATION


    return 0;
}

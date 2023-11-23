/*
 * File: example_dp_slp_mode.c
 * Author: Hossein Taji
 * Date: Oct, 2023
 * Description: This file put Heepocrates in deep sleep mode
 */
#include <stdio.h>
#include <stdlib.h>
#include "heepocrates.h"
#include "core_v_mini_mcu.h"
#include "power_manager.h"
#include "gpio.h"
#include "soc_ctrl.h"
#include "rv_timer.h"
#include "fll.h"
#include "gpio.h"
#include "power_manager.h"
#include "csr.h"
#include "hart.h"
#include "handler.h"

#define PIN_TRIGGER     4  //used for trigering and checking on oscilloscope

// #define PUT_SLEEP
#define PUT_DEEP_SLEEP
// #define PUT_IDLE

uint32_t fll_freq, fll_freq_real;
fll_t fll;
soc_ctrl_t soc_ctrl;
uint32_t fll_status;
const uint64_t TIMER_FREQ = 0.1*1000000; // 100 KHz
const uint64_t SYS_FREQ = 150*1000000; //MHz
gpio_t gpio;
rv_timer_t timer_0_1;

static power_manager_t power_manager;


int main(int argc, char const *argv[])
{
    /*
     * ==============
     * Configuration 
     * ==============
    */ 

    /*
     * Power Manager 
     */ 
    mmio_region_t power_manager_reg = mmio_region_from_addr(POWER_MANAGER_START_ADDRESS);
    power_manager.base_addr = power_manager_reg;
    power_manager_counters_t power_manager_counters;
    power_manager_counters_t power_manager_counters_cpu;
    power_gate_counters_init(&power_manager_counters, 0, 0, 0, 0, 0, 0, 0, 0);
    power_gate_counters_init(&power_manager_counters_cpu, 40, 40, 30, 30, 20, 20, 0, 0);

    /*
     * FLL and SoC controller 
     */ 
    // 2.1 FLL peripheral structure to access the registers
    fll.base_addr = mmio_region_from_addr((uintptr_t)FLL_START_ADDRESS);
    fll_init(&fll);
    fll_status = fll_status_get(&fll);
    soc_ctrl.base_addr = mmio_region_from_addr((uintptr_t)SOC_CTRL_START_ADDRESS);


    //2.4 Set default app frequency
    fll_set_freq(&fll, SYS_FREQ);
    for (int j = 0; j < 10000; j++) {
      asm volatile("nop");
    }
    fll_freq_real = fll_get_freq(&fll);
    soc_ctrl_set_frequency(&soc_ctrl, fll_freq_real);

    /*
     * Energy modes
     */
    init_ene_modes(&power_manager, &fll, &soc_ctrl, SYS_FREQ);

    /*
     * Setup GPIO 
     */ 
    gpio_params_t gpio_params;
    gpio_params.base_addr = mmio_region_from_addr((uintptr_t)GPIO_AO_START_ADDRESS);
    gpio_init(gpio_params, &gpio);
    gpio_output_set_enabled(&gpio, PIN_TRIGGER, true);
    gpio_write(&gpio, PIN_TRIGGER, false);

    /*
     * Initialize Wakeup Timer 
     */ 
    // Setup rv_timer_0_1
    mmio_region_t timer_0_1_reg = mmio_region_from_addr(RV_TIMER_AO_START_ADDRESS);
    rv_timer_init(timer_0_1_reg, (rv_timer_config_t){.hart_count = 1, .comparator_count = 1}, &timer_0_1);
    rv_timer_tick_params_t tick_params;
    rv_timer_approximate_tick_params(SYS_FREQ, TIMER_FREQ, &tick_params);
    if (tick_params.prescale==0){
        printf("Timer approximate function was not able to set a correct value prescale");
    }
    rv_timer_set_tick_params(&timer_0_1, 0, tick_params);
    rv_timer_irq_enable(&timer_0_1, 0, 0, kRvTimerEnabled);

    /* App */ 
    for (int j = 0; j < 10000000; j++) asm volatile("nop");

    /*
     * =========
     * Idle 
     * =========
    */ 
    #ifdef PUT_IDLE
    /* todo: its not working, I should check later */
    /* Wakeup time */
    rv_timer_arm(&timer_0_1, 0, 0, 33340);
    rv_timer_counter_set_enabled(&timer_0_1, 0, kRvTimerEnabled);
    /* Staying idle */   
    // wait_for_interrupt();
    while(1){
        asm volatile("nop");
    }
    #endif

    /*
     * ===========
     * Sleep mode
     * ===========
     */
    #ifdef PUT_SLEEP
    /* Wakeup time */
    rv_timer_approximate_tick_params(SYS_FREQ, SYS_FREQ, &tick_params); //add it because I lower the freq
    rv_timer_set_tick_params(&timer_0_1, 0, tick_params);
    // rv_timer_arm(&timer_0_1, 0, 0, 1);
    // rv_timer_counter_set_enabled(&timer_0_1, 0, kRvTimerEnabled);
    /* Sleep */
    en_sleep_mode(&power_manager, &power_manager_counters , &power_manager_counters_cpu);
    #endif

    /*
     * ===============
     * Deep sleep mode
     * ===============
     */
    #ifdef PUT_DEEP_SLEEP
    /* Setup Wakeup Timer */
    rv_timer_approximate_tick_params(SYS_FREQ, SYS_FREQ, &tick_params); //add it because I lower the freq
    rv_timer_set_tick_params(&timer_0_1, 0, tick_params);
    rv_timer_irq_enable(&timer_0_1, 0, 0, kRvTimerEnabled);
    rv_timer_arm(&timer_0_1, 0, 0, 1000);   //changing timer parameter because frequency has changed(?!)
    rv_timer_counter_set_enabled(&timer_0_1, 0, kRvTimerEnabled); //start timer here
    /* Deep sleep mode*/
    gpio_write(&gpio, PIN_TRIGGER, true);
    en_deep_sleep_mode(&power_manager, &power_manager_counters , &power_manager_counters_cpu, &fll, &soc_ctrl);
    gpio_write(&gpio, PIN_TRIGGER, false);
    #endif

    /* App */ 
    for (int j = 0; j < 10000000; j++) asm volatile("nop");

    /* write something to stdout */
    printf("Success.\n");
    return EXIT_SUCCESS;
}
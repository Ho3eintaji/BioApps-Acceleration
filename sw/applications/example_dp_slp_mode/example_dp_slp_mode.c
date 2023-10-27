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



#define SYS_FREQ        150*1000000 //MHz
#define TIMER_FREQ      0.1*1000000 // 100 KHz
#define PIN_TRIGGER     4  //used for trigering and checking on oscilloscope

// #define PUT_SLEEP
#define PUT_DEEP_SLEEP
// #define PUT_IDLE


gpio_t gpio;
static power_manager_t power_manager;

static rv_timer_t timer_0_1;

int main(int argc, char const *argv[])
{
    /*
     * ===================================================================
     * =================== Configuration =================================
     * ===================================================================
    */ 

    /*
     * ======================
     * == Power Manager =====
     * ======================
    */ 
    // 1. Setting up power manager
    mmio_region_t power_manager_reg = mmio_region_from_addr(POWER_MANAGER_START_ADDRESS);
    power_manager.base_addr = power_manager_reg;
    power_manager_counters_t power_manager_counters;
    power_manager_counters_t power_manager_counters_cpu;
    power_gate_counters_init(&power_manager_counters, 0, 0, 0, 0, 0, 0, 0, 0);
    power_gate_counters_init(&power_manager_counters_cpu, 40, 40, 30, 30, 20, 20, 0, 0);

    /*
     * ============
     * == FLL =====
     * ============
    */ 
    // 2.1 FLL peripheral structure to access the registers
    uint32_t fll_freq, fll_freq_real;
    fll_t fll;
    fll.base_addr = mmio_region_from_addr((uintptr_t)FLL_START_ADDRESS);
    fll_init(&fll);
    uint32_t fll_status = fll_status_get(&fll);
    soc_ctrl_t soc_ctrl;
    soc_ctrl.base_addr = mmio_region_from_addr((uintptr_t)SOC_CTRL_START_ADDRESS);

    // 2.2. Enable open loop mode :)
    fll_conf1_reg_t fll_conf1 = fll_conf1_get(&fll);
    fll_conf1.op_mode = 0;
    // fll_conf1_set(&fll, fll_conf1.raw);


    // 2.3 Values for openloop switching
    const uint32_t config_fll_ol_low = fll_create_config_1((fll_conf1_reg_t){
        .mult_factor = fll_conf1.mult_factor,
        .dco_input   = 170,
        .clk_div     = 0x2,
        .lock_enable = fll_conf1.lock_enable,
        .op_mode     = fll_conf1.op_mode
    });
    const uint32_t config_fll_ol_high = fll_create_config_1((fll_conf1_reg_t){
        .mult_factor = fll_conf1.mult_factor,
        .dco_input   = 807,
        .clk_div     = 0x2,
        .lock_enable = fll_conf1.lock_enable,
        .op_mode     = fll_conf1.op_mode
    });

    //2.4 Set default app frequency
    fll_set_freq(&fll, SYS_FREQ);
    for (int j = 0; j < 10000; j++) {
      asm volatile("nop");
    }
    fll_freq_real = fll_get_freq(&fll);
    soc_ctrl_set_frequency(&soc_ctrl, fll_freq_real);

    /*
     * ============
     * == GPIO ====
     * ============
    */ 
    // 3. Setup GPIO
    gpio_params_t gpio_params;
    gpio_params.base_addr = mmio_region_from_addr((uintptr_t)GPIO_AO_START_ADDRESS);
    gpio_init(gpio_params, &gpio);
    gpio_output_set_enabled(&gpio, PIN_TRIGGER, true);
    gpio_write(&gpio, PIN_TRIGGER, false);

    /*
     * =============================
     * == Initialize Wakeup Timer ==
     * =============================
    */ 
    // Setup rv_timer_0_1
    mmio_region_t timer_0_1_reg = mmio_region_from_addr(RV_TIMER_AO_START_ADDRESS);
    rv_timer_init(timer_0_1_reg, (rv_timer_config_t){.hart_count = 1, .comparator_count = 1}, &timer_0_1);
    rv_timer_tick_params_t tick_params;
    rv_timer_approximate_tick_params(SYS_FREQ, TIMER_FREQ, &tick_params);
    if (tick_params.prescale==0){
        printf("Timer approximate function was not able to set a correct value prescale");
    }

    /*
     * ===========================================================================
     * =================== Staying idle ==========================================
     * ===========================================================================
    */ 
    #ifdef PUT_IDLE

    //todo: its not working, I should check later

    /*
     * ==========================
     * == Setup Wakeup Timer ====
     * ==========================
    */
    rv_timer_set_tick_params(&timer_0_1, 0, tick_params);
    rv_timer_irq_enable(&timer_0_1, 0, 0, kRvTimerEnabled);
    rv_timer_arm(&timer_0_1, 0, 0, 33340);
    rv_timer_counter_set_enabled(&timer_0_1, 0, kRvTimerEnabled);
    /*
     * ============
     * == App =====
     * ============
    */ 
    for (int j = 0; j < 10000000; j++) {
      asm volatile("nop");
    }
    /*
     * =====================
     * == Staying idle =====
     * =====================
    */   
    gpio_write(&gpio, PIN_TRIGGER, true);
    // wait_for_interrupt();
    while(1){
        asm volatile("nop");
    }
    gpio_write(&gpio, PIN_TRIGGER, false);
    /*
     * ============
     * == App =====
     * ============
    */ 
    for (int j = 0; j < 10000000; j++) {
      asm volatile("nop");
    }

    #endif

    /*
     * ===========================================================================
     * =================== Enabling Sleep Mode ===================================
     * ===========================================================================
    */ 
    #ifdef PUT_SLEEP

    /*
     * ==========================
     * == Setup Wakeup Timer ====
     * ==========================
    */
    rv_timer_set_tick_params(&timer_0_1, 0, tick_params);
    rv_timer_irq_enable(&timer_0_1, 0, 0, kRvTimerEnabled);
    rv_timer_arm(&timer_0_1, 0, 0, 33340);
    rv_timer_counter_set_enabled(&timer_0_1, 0, kRvTimerEnabled);

    /*
     * ============
     * == App =====
     * ============
    */ 
    for (int j = 0; j < 10000000; j++) {
      asm volatile("nop");
    }

    /*
     * ===================
     * == Power Gate =====
     * ===================
    */
    // gpio_write(&gpio, PIN_TRIGGER, true);
    // 1.1. Power gating peripheral domain
    power_gate_periph(&power_manager, kOff_e, &power_manager_counters);
    // 1.2. Power gating memory banks
    power_gate_ram_block(&power_manager, 2, kOff_e, &power_manager_counters);
    power_gate_ram_block(&power_manager, 3, kOff_e, &power_manager_counters);
    power_gate_ram_block(&power_manager, 4, kOff_e, &power_manager_counters);
    power_gate_ram_block(&power_manager, 5, kOff_e, &power_manager_counters);
    power_gate_ram_block(&power_manager, 6, kOff_e, &power_manager_counters);
    power_gate_ram_block(&power_manager, 7, kOff_e, &power_manager_counters);
    // 1.3.1. Power gating CGRA logic
    power_gate_external(&power_manager, 0, kOff_e, &power_manager_counters);
    // 1.3.2. Power gating CGRA memories
    power_gate_external(&power_manager, 1, kOff_e, &power_manager_counters);
    // 1.3.3. Power gating Coubertin
    power_gate_external(&power_manager, 2, kOff_e, &power_manager_counters);
    gpio_write(&gpio, PIN_TRIGGER, true); 

    /*
     * =======================
     * == Power Gate Core ====
     * =======================
    */
    CSR_CLEAR_BITS(CSR_REG_MSTATUS, 0x8);
    power_gate_core(&power_manager, kTimer_0_pm_e, &power_manager_counters_cpu); //todo: remove
    CSR_SET_BITS(CSR_REG_MSTATUS, 0x8);

    /*
     * ===================
     * == Power On =======
     * ===================
    */
    gpio_write(&gpio, PIN_TRIGGER, false);
    // 1.1. Power on peripheral domain
    power_gate_periph(&power_manager, kOn_e, &power_manager_counters);
    // 1.2. Power on memory banks
    power_gate_ram_block(&power_manager, 2, kOn_e, &power_manager_counters);
    power_gate_ram_block(&power_manager, 3, kOn_e, &power_manager_counters);
    power_gate_ram_block(&power_manager, 4, kOn_e, &power_manager_counters);
    power_gate_ram_block(&power_manager, 5, kOn_e, &power_manager_counters);
    power_gate_ram_block(&power_manager, 6, kOn_e, &power_manager_counters);
    power_gate_ram_block(&power_manager, 7, kOn_e, &power_manager_counters);
    // 1.3.1. Power on CGRA logic
    power_gate_external(&power_manager, 0, kOn_e, &power_manager_counters);
    // 1.3.2. Power on CGRA memories
    power_gate_external(&power_manager, 1, kOn_e, &power_manager_counters);
    // 1.3.3. Power on Coubertin
    power_gate_external(&power_manager, 2, kOn_e, &power_manager_counters);
    // gpio_write(&gpio, PIN_TRIGGER, false);

    /*
     * ============
     * == App =====
     * ============
    */ 
    for (int j = 0; j < 10000000; j++) {
      asm volatile("nop");
    }

    #endif


    /*
     * ===========================================================================
     * =================== Enabling Deep Sleep Mode ==============================
     * ===========================================================================
    */ 
    #ifdef PUT_DEEP_SLEEP
    /*
     * ==========================
     * == Setup Wakeup Timer ====
     * ==========================
    */
    rv_timer_approximate_tick_params(SYS_FREQ, SYS_FREQ, &tick_params); //add it because I lower the freq
    rv_timer_set_tick_params(&timer_0_1, 0, tick_params);
    rv_timer_irq_enable(&timer_0_1, 0, 0, kRvTimerEnabled);
    // rv_timer_arm(&timer_0_1, 0, 0, 33340);
    // rv_timer_arm(&timer_0_1, 0, 0, 100000);
    // rv_timer_counter_set_enabled(&timer_0_1, 0, kRvTimerEnabled); //todo: remove because timer freq is getting different
    /*
     * ============
     * == App =====
     * ============
    */ 
    for (int j = 0; j < 10000000; j++) {
      asm volatile("nop");
    }
    /*
     * ===================
     * == Power Gate =====
     * ===================
    */
    gpio_write(&gpio, PIN_TRIGGER, true);
    power_gate_periph(&power_manager, kOff_e, &power_manager_counters); // 1.1. Power gating peripheral domain
    power_gate_ram_block(&power_manager, 2, kOff_e, &power_manager_counters); // 1.2. Power gating memory banks
    power_gate_ram_block(&power_manager, 3, kOff_e, &power_manager_counters);
    power_gate_ram_block(&power_manager, 4, kOff_e, &power_manager_counters);
    power_gate_ram_block(&power_manager, 5, kOff_e, &power_manager_counters);
    power_gate_ram_block(&power_manager, 6, kOff_e, &power_manager_counters);
    power_gate_ram_block(&power_manager, 7, kOff_e, &power_manager_counters);
    power_gate_external(&power_manager, 0, kOff_e, &power_manager_counters); // 1.3.1. Power gating CGRA logic
    power_gate_external(&power_manager, 1, kOff_e, &power_manager_counters); // 1.3.2. Power gating CGRA memories
    power_gate_external(&power_manager, 2, kOff_e, &power_manager_counters); // 1.3.3. Power gating Coubertin
    /*
     * =====================
     * == FLL, Low Freq ====
     * =====================
    */
    fll_conf1_set(&fll, config_fll_ol_low);     //open loop
    fll_freq = fll_get_freq(&fll);
    soc_ctrl_set_frequency(&soc_ctrl, fll_freq);
    /*
     * =======================
     * == Power Gate Core ====
     * =======================
    */
    rv_timer_arm(&timer_0_1, 0, 0, 3000);   //changing timer parameter because frequency has changed(?!)
    rv_timer_counter_set_enabled(&timer_0_1, 0, kRvTimerEnabled); //start timer here
    CSR_CLEAR_BITS(CSR_REG_MSTATUS, 0x8);
    power_gate_core(&power_manager, kTimer_0_pm_e, &power_manager_counters_cpu); //todo: remove
    CSR_SET_BITS(CSR_REG_MSTATUS, 0x8);
    // gpio_write(&gpio, PIN_TRIGGER, false);
    /*
     * =====================
     * == FLL, High Freq ===
     * =====================
    */
    fll_conf1_set(&fll, config_fll_ol_high);    // open loop step
    fll_set_freq(&fll, SYS_FREQ);               // feedback step
    fll_freq_real = fll_get_freq(&fll);
    soc_ctrl_set_frequency(&soc_ctrl, fll_freq_real);
    /*
     * ===================
     * == Power On =======
     * ===================
    */
    power_gate_periph(&power_manager, kOn_e, &power_manager_counters); // 1.1. Power on peripheral domain
    power_gate_ram_block(&power_manager, 2, kOn_e, &power_manager_counters); // 1.2. Power on memory banks
    power_gate_ram_block(&power_manager, 3, kOn_e, &power_manager_counters);
    power_gate_ram_block(&power_manager, 4, kOn_e, &power_manager_counters);
    power_gate_ram_block(&power_manager, 5, kOn_e, &power_manager_counters);
    power_gate_ram_block(&power_manager, 6, kOn_e, &power_manager_counters);
    power_gate_ram_block(&power_manager, 7, kOn_e, &power_manager_counters);
    power_gate_external(&power_manager, 0, kOn_e, &power_manager_counters); // 1.3.1. Power on CGRA logic
    power_gate_external(&power_manager, 1, kOn_e, &power_manager_counters); // 1.3.2. Power on CGRA memories
    power_gate_external(&power_manager, 2, kOn_e, &power_manager_counters); // 1.3.3. Power on Coubertin
    /*
     * ============
     * == App =====
     * ============
    */ 
    for (int j = 0; j < 10000000; j++) {
      asm volatile("nop");
    }

    #endif


    /* write something to stdout */
    printf("Success.\n");
    return EXIT_SUCCESS;
}
#include <stdio.h>
#include <stdlib.h>

#include "core_v_mini_mcu.h"
#include "heepocrates.h"
#include "power_manager.h"
#include "heepocrates_ctrl.h"

#include "heepocrates_ctrl_regs.h"

int main(void) {

  power_manager_t power_manager;
  heepocrates_ctrl_t heepocrates_ctrl;

  // Setup power_manager
  power_manager.base_addr = mmio_region_from_addr(POWER_MANAGER_START_ADDRESS);


  heepocrates_ctrl.base_addr = mmio_region_from_addr(HEEPOCRATES_CTRL_START_ADDRESS);


  power_manager_counters_t power_manager_ram_blocks_counters;

  //read monitors
  uint32_t sw_mem4, sw_cgra, sw_coubertin;

  sw_mem4 = 0x0;
  sw_cgra = 0x0;
  sw_coubertin = 0x0;

  //check ON
  while(sw_mem4!=0xF) {  sw_mem4 = heepocrates_ctrl_read_sw_monitor(&heepocrates_ctrl, (ptrdiff_t)(HEEPOCRATES_CTRL_MEM4_SW_MONITOR_REG_OFFSET)); }

  while(sw_cgra!=0xF) {  sw_cgra = heepocrates_ctrl_read_sw_monitor(&heepocrates_ctrl, (ptrdiff_t)(HEEPOCRATES_CTRL_CGRA_MEM_SW_MONITOR_REG_OFFSET)); }

  while(sw_coubertin!=0x3) {  sw_coubertin = heepocrates_ctrl_read_sw_monitor(&heepocrates_ctrl, (ptrdiff_t)(HEEPOCRATES_CTRL_COUBERTIN_SW_MONITOR_REG_OFFSET)); }

  printf("sw: mem4 %x memcgra %x coubertin %x\n", sw_mem4, sw_cgra, sw_coubertin);

  // Init ram block 4's counters
  if (power_gate_counters_init(&power_manager_ram_blocks_counters, 40, 40, 30, 30, 20, 20, 0, 0) != kPowerManagerOk_e)
  {
      printf("Error: power manager fail. Check the reset and powergate counters value\n");
      return EXIT_FAILURE;
  }

  // Power off ram block 4 domain
  if (power_gate_ram_block(&power_manager, 4, kOff_e, &power_manager_ram_blocks_counters) != kPowerManagerOk_e)
  {
      printf("Error: power manager fail.\n");
      return EXIT_FAILURE;
  }

  // Power off cgra ram domain
  if (power_gate_external(&power_manager, 1, kOff_e, &power_manager_ram_blocks_counters) != kPowerManagerOk_e)
  {
      printf("Error: power manager fail.\n");
      return EXIT_FAILURE;
  }

    // Power off coubertin macro domain
  if (power_gate_external(&power_manager, 2, kOff_e, &power_manager_ram_blocks_counters) != kPowerManagerOk_e)
  {
      printf("Error: power manager fail.\n");
      return EXIT_FAILURE;
  }

  // Check that the ram block 4 domain is actually OFF
  while(!ram_block_power_domain_is_off(&power_manager, 4));

  //check OFF
  while(sw_mem4!=0x0) {  sw_mem4 = heepocrates_ctrl_read_sw_monitor(&heepocrates_ctrl, (ptrdiff_t)(HEEPOCRATES_CTRL_MEM4_SW_MONITOR_REG_OFFSET)); }

  while(sw_cgra!=0x0) {  sw_cgra = heepocrates_ctrl_read_sw_monitor(&heepocrates_ctrl, (ptrdiff_t)(HEEPOCRATES_CTRL_CGRA_MEM_SW_MONITOR_REG_OFFSET)); }

  while(sw_coubertin!=0x0) {  sw_coubertin = heepocrates_ctrl_read_sw_monitor(&heepocrates_ctrl, (ptrdiff_t)(HEEPOCRATES_CTRL_COUBERTIN_SW_MONITOR_REG_OFFSET)); }

  printf("sw: mem4 %x memcgra %x coubertin %x\n", sw_mem4, sw_cgra, sw_coubertin);

  // Wait some time
  for (int i=0; i<100; i++) asm volatile("nop");

  // Power on ram block 2 domain
  if (power_gate_ram_block(&power_manager, 4, kOn_e, &power_manager_ram_blocks_counters) != kPowerManagerOk_e)
  {
      printf("Error: power manager fail.\n");
      return EXIT_FAILURE;
  }
  // Power on cgra ram domain
  if (power_gate_external(&power_manager, 1, kOn_e, &power_manager_ram_blocks_counters) != kPowerManagerOk_e)
  {
      printf("Error: power manager fail.\n");
      return EXIT_FAILURE;
  }

    // Power on coubertin macro domain
  if (power_gate_external(&power_manager, 2, kOn_e, &power_manager_ram_blocks_counters) != kPowerManagerOk_e)
  {
      printf("Error: power manager fail.\n");
      return EXIT_FAILURE;
  }
  //check ON
  while(sw_mem4!=0xF) {  sw_mem4 = heepocrates_ctrl_read_sw_monitor(&heepocrates_ctrl, (ptrdiff_t)(HEEPOCRATES_CTRL_MEM4_SW_MONITOR_REG_OFFSET)); }

  while(sw_cgra!=0xF) {  sw_cgra = heepocrates_ctrl_read_sw_monitor(&heepocrates_ctrl, (ptrdiff_t)(HEEPOCRATES_CTRL_CGRA_MEM_SW_MONITOR_REG_OFFSET)); }

  while(sw_coubertin!=0x3) {  sw_coubertin = heepocrates_ctrl_read_sw_monitor(&heepocrates_ctrl, (ptrdiff_t)(HEEPOCRATES_CTRL_COUBERTIN_SW_MONITOR_REG_OFFSET)); }

  printf("sw: mem4 %x memcgra %x coubertin %x\n", sw_mem4, sw_cgra, sw_coubertin);

  return EXIT_SUCCESS;

}

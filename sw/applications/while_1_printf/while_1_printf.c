/*
 * Copyright 2020 ETH Zurich
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Author: Robert Balas <balasr@iis.ee.ethz.ch>
 */

#include <stdio.h>
#include <stdlib.h>
#include "core_v_mini_mcu.h"
#include "power_manager.h"
#include "gpio.h"
#include "soc_ctrl.h"
#include "rv_timer.h"
#include "fll.h"
#include "csr.h"
#include "hart.h"
#include "handler.h"
#include "heepocrates.h"



const uint64_t SYS_FREQ = 80*1000000; //MHz

void fll_cfg(uint64_t freq);


int main(int argc, char *argv[])
{

  // Set app frequency
  fll_cfg(SYS_FREQ);
    
  /* write something to stdout */
  int i = 0, k;
  while(1){

      printf("hello!! %x!\n", i);
      i++;
      for(k = 0; k < 50000; k++) asm volatile("nop");
  }

  return EXIT_SUCCESS;
}


void fll_cfg(uint64_t freq) {
  uint32_t fll_freq_real;
  fll_t fll;
  soc_ctrl_t soc_ctrl;
  uint32_t fll_status;
  // 2.1 FLL peripheral structure to access the registers
  fll.base_addr = mmio_region_from_addr((uintptr_t)FLL_START_ADDRESS);
  fll_init(&fll);
  fll_status = fll_status_get(&fll);
  soc_ctrl.base_addr = mmio_region_from_addr((uintptr_t)SOC_CTRL_START_ADDRESS);
  //2.4 Set default app frequency
  fll_set_freq(&fll, freq);
  for (int j = 0; j < 10000; j++) {
    asm volatile("nop");
  }
  fll_freq_real = fll_get_freq(&fll);
  soc_ctrl_set_frequency(&soc_ctrl, fll_freq_real);
}

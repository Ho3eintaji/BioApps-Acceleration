#include "heepocrates.h"
#include "fll_regs.h"

#ifdef TARGET_USE_FLL
    // Compute FFL frequency: (fll_status*REFERENCE_CLOCK_Hz)/fll_conf1.clk_div;
   li a1, FLL_START_ADDRESS
   lw a3, FLL_STATUS_REG_OFFSET(a1)
   mul a2, a2, a3
   lw a3, FLL_CONFIG_1_REG_OFFSET(a1)
   srli a3, a3, FLL_CONFIG_1_CLK_DIV_OFFSET
   andi a3, a3, FLL_CONFIG_1_CLK_DIV_MASK
   divu a2 ,a2, a3
   sw a2, SOC_CTRL_SYSTEM_FREQUENCY_HZ_REG_OFFSET(a0)
#endif // TARGET_USE_FLL


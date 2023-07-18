#include "heepocrates.h"
#include "fll_regs.h"

#ifdef TARGET_USE_FLL
    // Init the FLL to 100MHz in lock mode
    li    a1, FLL_START_ADDRESS
    lw    a2, FLL_CONFIG_1_REG_OFFSET(a1)
    lw    a3, FLL_CONFIG_2_REG_OFFSET(a1)
    // Set assert_cycles to 6
    li    a5, FLL_CONFIG_2_ASSERT_CYCLES_MASK
    slli  a5, a5, FLL_CONFIG_2_ASSERT_CYCLES_OFFSET
    xori  a5, a5, -1 // bitwise not
    and   a3, a3, a5
    li    a4, 6
    slli  a4, a4, FLL_CONFIG_2_ASSERT_CYCLES_OFFSET
    or    a3, a3, a4
    // Set lock_tolerance to 0x50
    li    a5, FLL_CONFIG_2_LOCK_TOLERANCE_MASK
    slli  a5, a5, FLL_CONFIG_2_LOCK_TOLERANCE_OFFSET
    xori  a5, a5, -1 // bitwise not
    and   a3, a3, a5
    li    a4, 0x50
    slli  a4, a4, FLL_CONFIG_2_LOCK_TOLERANCE_OFFSET
    or    a3, a3, a4
    sw    a3, FLL_CONFIG_2_REG_OFFSET(a1)
    // Set dco to 395 (~100MHz) to speed up FLL lock
    li    a5, FLL_CONFIG_1_DCO_CODE_STA_MASK
    slli  a5, a5, FLL_CONFIG_1_DCO_CODE_STA_OFFSET
    xori  a5, a5, -1 // bitwise not
    and   a2, a2, a5
    li    a4, 395
    slli  a4, a4, FLL_CONFIG_1_DCO_CODE_STA_OFFSET
    or    a2, a2, a4
    sw    a2, FLL_CONFIG_1_REG_OFFSET(a1)
#ifndef USE_TESTING_BOARD
    // Set lock_enable to 1
    li    a4, 1
    slli  a4, a4, FLL_CONFIG_1_LOCK_ENABLE_BIT
#else
    // Set lock_enable to 0 in the ASIC version
    li    a4, 1
    slli  a4, a4, FLL_CONFIG_1_LOCK_ENABLE_BIT
    not   a4, a4 #(this one is ~0100_0000_0000_0000)
    and   a2, a2, a4
#endif
    or    a2, a2, a4
    // Set op_mode to 1
    li    a4, 1
    slli  a4, a4, FLL_CONFIG_1_OP_MODE_BIT
    or    a2, a2, a4
    // Set mult_factor
    li    a5, FLL_CONFIG_1_CLK_MULT_MASK
    slli  a5, a5, FLL_CONFIG_1_CLK_MULT_OFFSET
    xori  a5, a5, -1 // bitwise not
    and   a2, a2, a5
    // Create 6103 immediate for mult
    lui   a4, 1
    addi  a4, a4, 2007
    or    a2, a2, a4
    // Set clk_div
    li    a5, FLL_CONFIG_1_CLK_DIV_MASK
    slli  a5, a5, FLL_CONFIG_1_CLK_DIV_OFFSET
    xori  a5, a5, -1 // bitwise not
    and   a2, a2, a5
    li    a4, 2
    slli  a4, a4, FLL_CONFIG_1_CLK_DIV_OFFSET
    or    a2, a2, a4
    sw    a2, FLL_CONFIG_1_REG_OFFSET(a1)
    // Compute FFL frequency: (fll_status*REFERENCE_CLOCK_Hz)/fll_conf1.clk_div;
    li    a0, SOC_CTRL_START_ADDRESS
    li    a2, REFERENCE_CLOCK_Hz
    lw    a3, FLL_STATUS_REG_OFFSET(a1)
    mul   a2, a2, a3
    lw    a3, FLL_CONFIG_1_REG_OFFSET(a1)
    srli  a3, a3, FLL_CONFIG_1_CLK_DIV_OFFSET
    andi  a3, a3, FLL_CONFIG_1_CLK_DIV_MASK
    divu  a2, a2, a3
    // We round the FLL frequency number before storing it to the soc_ctrl as the library
    // requires round values for calculating the absolute time.
    li    a3, 1000000
    divu  a2, a2, a3
    mul   a2, a2, a3
    sw    a2, SOC_CTRL_SYSTEM_FREQUENCY_HZ_REG_OFFSET(a0)
#endif // TARGET_USE_FLL

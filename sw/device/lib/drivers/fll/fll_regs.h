// Generated register defines for fll

// Copyright information found in source file:
// Copyright EPFL contributors.

// Licensing information found in source file:
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

#ifndef _FLL_REG_DEFS_
#define _FLL_REG_DEFS_

#ifdef __cplusplus
extern "C" {
#endif
// Register width
#define FLL_PARAM_REG_WIDTH 32

// FLL 1 frequency.
#define FLL_STATUS_REG_OFFSET 0x0
#define FLL_STATUS_MULT_FACTOR_MASK 0xffff
#define FLL_STATUS_MULT_FACTOR_OFFSET 0
#define FLL_STATUS_MULT_FACTOR_FIELD \
  ((bitfield_field32_t) { .mask = FLL_STATUS_MULT_FACTOR_MASK, .index = FLL_STATUS_MULT_FACTOR_OFFSET })

// FLL 1 Configuration register 1.
#define FLL_CONFIG_1_REG_OFFSET 0x4
#define FLL_CONFIG_1_CLK_MULT_MASK 0xffff
#define FLL_CONFIG_1_CLK_MULT_OFFSET 0
#define FLL_CONFIG_1_CLK_MULT_FIELD \
  ((bitfield_field32_t) { .mask = FLL_CONFIG_1_CLK_MULT_MASK, .index = FLL_CONFIG_1_CLK_MULT_OFFSET })
#define FLL_CONFIG_1_DCO_CODE_STA_MASK 0x3ff
#define FLL_CONFIG_1_DCO_CODE_STA_OFFSET 16
#define FLL_CONFIG_1_DCO_CODE_STA_FIELD \
  ((bitfield_field32_t) { .mask = FLL_CONFIG_1_DCO_CODE_STA_MASK, .index = FLL_CONFIG_1_DCO_CODE_STA_OFFSET })
#define FLL_CONFIG_1_CLK_DIV_MASK 0xf
#define FLL_CONFIG_1_CLK_DIV_OFFSET 26
#define FLL_CONFIG_1_CLK_DIV_FIELD \
  ((bitfield_field32_t) { .mask = FLL_CONFIG_1_CLK_DIV_MASK, .index = FLL_CONFIG_1_CLK_DIV_OFFSET })
#define FLL_CONFIG_1_LOCK_ENABLE_BIT 30
#define FLL_CONFIG_1_OP_MODE_BIT 31

// FLL 1 Configuration register 2.
#define FLL_CONFIG_2_REG_OFFSET 0x8
#define FLL_CONFIG_2_LOOP_GAIN_MASK 0xf
#define FLL_CONFIG_2_LOOP_GAIN_OFFSET 0
#define FLL_CONFIG_2_LOOP_GAIN_FIELD \
  ((bitfield_field32_t) { .mask = FLL_CONFIG_2_LOOP_GAIN_MASK, .index = FLL_CONFIG_2_LOOP_GAIN_OFFSET })
#define FLL_CONFIG_2_DEASSERT_CYCLES_MASK 0x3f
#define FLL_CONFIG_2_DEASSERT_CYCLES_OFFSET 4
#define FLL_CONFIG_2_DEASSERT_CYCLES_FIELD \
  ((bitfield_field32_t) { .mask = FLL_CONFIG_2_DEASSERT_CYCLES_MASK, .index = FLL_CONFIG_2_DEASSERT_CYCLES_OFFSET })
#define FLL_CONFIG_2_ASSERT_CYCLES_MASK 0x3f
#define FLL_CONFIG_2_ASSERT_CYCLES_OFFSET 10
#define FLL_CONFIG_2_ASSERT_CYCLES_FIELD \
  ((bitfield_field32_t) { .mask = FLL_CONFIG_2_ASSERT_CYCLES_MASK, .index = FLL_CONFIG_2_ASSERT_CYCLES_OFFSET })
#define FLL_CONFIG_2_LOCK_TOLERANCE_MASK 0xfff
#define FLL_CONFIG_2_LOCK_TOLERANCE_OFFSET 16
#define FLL_CONFIG_2_LOCK_TOLERANCE_FIELD \
  ((bitfield_field32_t) { .mask = FLL_CONFIG_2_LOCK_TOLERANCE_MASK, .index = FLL_CONFIG_2_LOCK_TOLERANCE_OFFSET })
#define FLL_CONFIG_2_CLK_STA_MODE_BIT 29
#define FLL_CONFIG_2_OPEN_LOOP_ENABLE_BIT 30
#define FLL_CONFIG_2_DITHERING_ENABLE_BIT 31

// FLL 1 Integrator register.
#define FLL_INTEGRATOR_REG_OFFSET 0xc
#define FLL_INTEGRATOR_FRAC_MASK 0x3ff
#define FLL_INTEGRATOR_FRAC_OFFSET 6
#define FLL_INTEGRATOR_FRAC_FIELD \
  ((bitfield_field32_t) { .mask = FLL_INTEGRATOR_FRAC_MASK, .index = FLL_INTEGRATOR_FRAC_OFFSET })
#define FLL_INTEGRATOR_INT_MASK 0x3ff
#define FLL_INTEGRATOR_INT_OFFSET 16
#define FLL_INTEGRATOR_INT_FIELD \
  ((bitfield_field32_t) { .mask = FLL_INTEGRATOR_INT_MASK, .index = FLL_INTEGRATOR_INT_OFFSET })

// FLL 2 frequency.
#define FLL_UNUSED0_REG_OFFSET 0x10

// FLL 2 Configuration register 1.
#define FLL_UNUSED1_REG_OFFSET 0x14

// FLL 2 Configuration register 2.
#define FLL_UNUSED2_REG_OFFSET 0x18

// FLL 2 Integrator register
#define FLL_UNUSED3_REG_OFFSET 0x1c

// FLL 3 frequency.
#define FLL_UNUSED4_REG_OFFSET 0x20

// FLL 3 Configuration register 1.
#define FLL_UNUSED5_REG_OFFSET 0x24

// FLL 3 Configuration register 2.
#define FLL_UNUSED6_REG_OFFSET 0x28

// FLL 3 Integrator register
#define FLL_UNUSED7_REG_OFFSET 0x2c

// Unused
#define FLL_UNUSED8_REG_OFFSET 0x30

// Unused
#define FLL_UNUSED9_REG_OFFSET 0x34

// Unused
#define FLL_UNUSED10_REG_OFFSET 0x38

// FLLs lock signals.
#define FLL_LOCK_SIGNALS_REG_OFFSET 0x3c
#define FLL_LOCK_SIGNALS_FLL_1_BIT 0
#define FLL_LOCK_SIGNALS_FLL_2_BIT 1
#define FLL_LOCK_SIGNALS_FLL_3_BIT 2

#ifdef __cplusplus
}  // extern "C"
#endif
#endif  // _FLL_REG_DEFS_
// End generated register defines for fll
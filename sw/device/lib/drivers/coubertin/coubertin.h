#ifndef _COUBERTIN_H_
#define _COUBERTIN_H_

#include <stdint.h>

#include "mmio.h"
#include "heepocrates.h"
#include "core_v_mini_mcu.h"
#include "coubertin_regs.h" // generated

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initialization parameters for CGRA peripheral control registers..
 *
 */
typedef struct coubertin {
  /**
   * The base address for the soc_ctrl hardware registers.
   */
  mmio_region_t base_addr;
} coubertin_t;

volatile void coubertin_write_r0(const coubertin_t *coubertin, int32_t reg_val);
volatile void coubertin_write_r1(const coubertin_t *coubertin, int32_t reg_val);
volatile uint32_t coubertin_read_r2(const coubertin_t *coubertin);
volatile void coubertin_write_saen(const coubertin_t *coubertin, int32_t reg_val);
volatile void coubertin_write_latency(const coubertin_t *coubertin, int32_t reg_val);
volatile uint32_t coubertin_read_latency(const coubertin_t *coubertin);
volatile bool coubertin_read_ready(const coubertin_t *coubertin);

#ifdef __cplusplus
}
#endif

#endif // _COUBERTIN_H_
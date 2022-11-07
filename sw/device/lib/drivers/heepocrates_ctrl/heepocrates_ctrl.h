#ifndef _HEEPOCRATES_CTRL_H_
#define _HEEPOCRATES_CTRL_H_

#include <stdint.h>

#include "mmio.h"
#include "heepocrates.h"
#include "core_v_mini_mcu.h"
#include "heepocrates_ctrl_regs.h" // generated

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initialization parameters for CGRA peripheral control registers..
 *
 */
typedef struct heepocrates_ctrl {
  /**
   * The base address for the soc_ctrl hardware registers.
   */
  mmio_region_t base_addr;
} heepocrates_ctrl_t;

volatile uint32_t heepocrates_ctrl_read_sw_monitor(const heepocrates_ctrl_t *heepocrates_ctrl, ptrdiff_t reg_offset);


#ifdef __cplusplus
}
#endif

#endif // _HEEPOCRATES_CTRL_H_
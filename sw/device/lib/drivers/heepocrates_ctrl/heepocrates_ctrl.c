#include <stdint.h>

#include "heepocrates_ctrl.h"

volatile uint32_t heepocrates_ctrl_read_sw_monitor(const heepocrates_ctrl_t *heepocrates_ctrl, ptrdiff_t reg_offset) {
  return mmio_region_read32(heepocrates_ctrl->base_addr, reg_offset);
}

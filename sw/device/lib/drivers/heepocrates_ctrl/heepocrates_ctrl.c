#include <stdint.h>

#include "heepocrates_ctrl.h"

volatile uint32_t heepocrates_ctrl_read_sw_monitor(const heepocrates_ctrl_t *heepocrates_ctrl, ptrdiff_t reg_offset) {
  return mmio_region_read32(heepocrates_ctrl->base_addr, reg_offset);
}

void heepocrates_ctrl_cgra_disable(const heepocrates_ctrl_t *heepocrates_ctrl, bool disable) {
  mmio_region_write32(heepocrates_ctrl->base_addr, (ptrdiff_t)(HEEPOCRATES_CTRL_CGRA_ENABLE_REG_OFFSET), !disable);
}

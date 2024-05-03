#include <stdint.h>

#include "coubertin.h"

volatile void coubertin_write_r0(const coubertin_t *coubertin, int32_t reg_val) {
  mmio_region_write32(coubertin->base_addr, (ptrdiff_t)(COUBERTIN_CONTROLLER_R0_REG_OFFSET), reg_val);
}

volatile void coubertin_write_r1(const coubertin_t *coubertin, int32_t reg_val) {
  mmio_region_write32(coubertin->base_addr, (ptrdiff_t)(COUBERTIN_CONTROLLER_R1_REG_OFFSET), reg_val);
}

volatile uint32_t coubertin_read_r2(const coubertin_t *coubertin) {
  return mmio_region_read32(coubertin->base_addr, (ptrdiff_t)(COUBERTIN_CONTROLLER_R2_REG_OFFSET));
}

volatile void coubertin_write_saen(const coubertin_t *coubertin, int32_t reg_val) {
  mmio_region_write32(coubertin->base_addr, (ptrdiff_t)(COUBERTIN_CONTROLLER_SAEN_REG_OFFSET), reg_val);
}

volatile void coubertin_write_latency(const coubertin_t *coubertin, int32_t reg_val) {
  mmio_region_write32(coubertin->base_addr, (ptrdiff_t)(COUBERTIN_CONTROLLER_LATENCY_REG_OFFSET), reg_val);
}

volatile uint32_t coubertin_read_latency(const coubertin_t *coubertin) {
  return mmio_region_read32(coubertin->base_addr, (ptrdiff_t)(COUBERTIN_CONTROLLER_LATENCY_REG_OFFSET));
}

volatile bool coubertin_read_ready(const coubertin_t *coubertin) {
  return mmio_region_get_bit32(coubertin->base_addr, (ptrdiff_t)(COUBERTIN_CONTROLLER_READY_REG_OFFSET), COUBERTIN_CONTROLLER_READY_READY_BIT);
}

volatile void coubertin_cg_disable(const coubertin_t *coubertin, bool disable) {
  mmio_region_write32(coubertin->base_addr, (ptrdiff_t)(COUBERTIN_CONTROLLER_COUBERTIN_CLK_GATE_REG_OFFSET), !disable);
}

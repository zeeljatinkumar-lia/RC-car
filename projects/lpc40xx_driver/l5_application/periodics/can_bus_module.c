#include "can_bus_module.h"
#include "can_bus.h"
#include <stddef.h>

static uint32_t baudrate_kbps_max = 500;
static uint16_t rxq_size_max = 10;
static uint16_t txq_size_max = 10;

void can_bus_module__reset(can__num_e can) {
  if (can__is_bus_off(can)) {
    can__reset_bus(can);
  }
}

bool can_bus_module__init(can__num_e can, uint32_t baudrate_kbps, uint16_t rxq_size, uint16_t txq_size) {
  bool isCanInitialized = false;
  if (can >= can_max || baudrate_kbps > baudrate_kbps_max || rxq_size > rxq_size_max || txq_size > txq_size_max) {
    isCanInitialized = false;
  } else {
    isCanInitialized = can__init(can, baudrate_kbps, rxq_size, txq_size, NULL, NULL);
    can__bypass_filter_accept_all_msgs();
    can_bus_module__reset(can);
  }
  return isCanInitialized;
}
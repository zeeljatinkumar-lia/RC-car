#include "can_bus_module.h"
#include "can_bus.h"
#include <stddef.h>

void can_bus_module__reset(can__num_e can) {
  if (can__is_bus_off(can)) {
    can__reset_bus(can);
  }
}

bool can_bus_module__init(can__num_e can) {
  bool isCanInitialized = false;
  uint32_t baudrate_kbps = 100;
  uint16_t rxq_size = 10;
  uint16_t txq_size = 10;
  isCanInitialized = can__init(can, baudrate_kbps, rxq_size, txq_size, NULL, NULL);
  can__bypass_filter_accept_all_msgs();
  can_bus_module__reset(can);
  return isCanInitialized;
}
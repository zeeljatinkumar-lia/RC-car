#pragma once
#include "can_bus.h"
#include <stdbool.h>

void can_bus_module__reset(can__num_e can);
bool can_bus_module__init(can__num_e can, uint32_t baudrate_kbps, uint16_t rxq_size, uint16_t txq_size);
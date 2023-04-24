#pragma once
#include "can_bus.h"
#include <stdbool.h>

bool can_bus_module__check_for_bus_off(can__num_e can);
bool can_bus_module__init(can__num_e can);
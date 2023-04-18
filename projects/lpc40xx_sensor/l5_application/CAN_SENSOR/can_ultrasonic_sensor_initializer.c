#include "can_ultrasonic_sensor_initializer.h"
#include "can_bus.h"
bool can_ultrasonic_init(void) {
  bool can_init_status = false;
  can_init_status = can__init(can1, 100, 5, 5, NULL, NULL);
  can__bypass_filter_accept_all_msgs();
  can__reset_bus(can1);

  return can_init_status;
}

bool can_ultrasonic_reset() {
  bool can_reset_status = false;
  if (can__is_bus_off(can1))
    can__reset_bus(can1);
  can_reset_status = true;

  return can_reset_status;
}

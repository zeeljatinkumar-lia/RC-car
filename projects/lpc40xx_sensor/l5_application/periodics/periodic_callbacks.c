#include "periodic_callbacks.h"

#include "board_io.h"
#include "gpio.h"
// newly added header
#include "bridge.h"
#include "bridge_init.h"
#include "can_ultrasonic_sensor_handler.h"
#include "can_ultrasonic_sensor_initializer.h"
#include "sensor_pin_init.h"

/******************************************************************************
 * Your board will reset if the periodic function does not return within its deadline
 * For 1Hz, the function must return within 1000ms
 * For 1000Hz, the function must return within 1ms
 */

void periodic_callbacks__initialize(void) {

  can_ultrasonic_init();
  ultrasonic__init_all_sensors();
  bridge_init_led();

  // This method is invoked once when the periodic tasks are created
}

void periodic_callbacks__1Hz(uint32_t callback_count) {
  // get sensor data every 1s
  ultrasonic__update_all_sensors();
  ultrasonic__get_distance_from_all_sensors(&sensor_data);
  print_dest_coords();

  can_ultrasonic_reset();
  // Add your code here
}

void periodic_callbacks__10Hz(uint32_t callback_count) {
  // Transmit sensor data
  can_handler__transmit_ultrasonic_sensor_messages_10hz();
  dbg_set_fake_dest_coords();
  bridge__read_destination_coordinates_from_app();
  bridge__send_destination_coordinates_over_can();

  // Add your code here
}
void periodic_callbacks__100Hz(uint32_t callback_count) {}

/**
 * @warning
 * This is a very fast 1ms task and care must be taken to use this
 * This may be disabled based on intialization of periodic_scheduler__initialize()
 */
void periodic_callbacks__1000Hz(uint32_t callback_count) {

  // Add your code here
}
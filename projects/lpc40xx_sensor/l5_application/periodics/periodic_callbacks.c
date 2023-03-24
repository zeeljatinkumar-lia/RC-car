#include "periodic_callbacks.h"

#include "board_io.h"
#include "gpio.h"
// newly added header
#include "can_dbc_initializer.h"
#include "can_handler.h"
#include "ultrasonic_sensor.h"

/******************************************************************************
 * Your board will reset if the periodic function does not return within its deadline
 * For 1Hz, the function must return within 1000ms
 * For 1000Hz, the function must return within 1ms
 */
// dbc_ULTRASONIC_TO_DRIVER_s sensor_struct = {};
void periodic_callbacks__initialize(void) {
  can_dbc_init();
  // This method is invoked once when the periodic tasks are created
}

void periodic_callbacks__1Hz(uint32_t callback_count) {
  // get sensor data every 1s
  get_ultrasonic_sensor_data();
  can_dbc_reset();
  // Add your code here
}

void periodic_callbacks__10Hz(uint32_t callback_count) {
  // Transmit sensor data
  can_handler__transmit_messages_10hz();

  // Add your code here
}
void periodic_callbacks__100Hz(uint32_t callback_count) {
  // can_handler__manage_mia_10hz();
  // Add your code here
}

/**
 * @warning
 * This is a very fast 1ms task and care must be taken to use this
 * This may be disabled based on intialization of periodic_scheduler__initialize()
 */
void periodic_callbacks__1000Hz(uint32_t callback_count) {

  // Add your code here
}
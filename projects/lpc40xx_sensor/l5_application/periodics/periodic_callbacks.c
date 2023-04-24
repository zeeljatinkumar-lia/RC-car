#include "periodic_callbacks.h"

#include "board_io.h"
#include "gpio.h"

#include "LV_sensor_controller.h"
#include "LV_sensor_pin_init.h"
#include "bridge_controller.h"
#include "can_ultrasonic_sensor_handler.h"
#include "can_ultrasonic_sensor_initializer.h"

/******************************************************************************
 * Your board will reset if the periodic function does not return within its deadline
 * For 1Hz, the function must return within 1000ms
 * For 1000Hz, the function must return within 1ms
 */

void periodic_callbacks__initialize(void) {
  gpio__set(board_io__get_led0());
  gpio__set(board_io__get_led1());
  gpio__set(board_io__get_led2());
  gpio__set(board_io__get_led3());

  can_ultrasonic_init();
  Bridge_Controller_init();
  Sensor_Controller_init();
}

void periodic_callbacks__1Hz(uint32_t callback_count) {
  bridge_can_mia_handler();
  Sensor_Controller__print_sensor_values();
  can_ultrasonic_reset();
}

void periodic_callbacks__10Hz(uint32_t callback_count) {

  Bridge_Controller__10hz_handler();
  can_bridge_controller__Sending_dest_location();
  bridge_controller_transmit_value_to_app();
}

void periodic_callbacks__100Hz(uint32_t callback_count) {
  CAN_RX_MSGS_FOR_BRIDGE();
  Sensor_Controller__100hz_handler(callback_count);
  can_ultrasonic_sensor_transmit_messages();
}

/**
 * @warning
 * This is a very fast 1ms task and care must be taken to use this
 * This may be disabled based on intialization of periodic_scheduler__initialize()
 */
void periodic_callbacks__1000Hz(uint32_t callback_count) {

  // Add your code here
}
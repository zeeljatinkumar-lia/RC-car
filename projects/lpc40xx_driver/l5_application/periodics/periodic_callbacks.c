#include "periodic_callbacks.h"

#include "board_io.h"
#include "can_bus.h"
#include "can_bus_module.h"
#include "driver_controller.h"
#include "gpio.h"

/******************************************************************************
 * Your board will reset if the periodic function does not return within its deadline
 * For 1Hz, the function must return within 1000ms
 * For 1000Hz, the function must return within 1ms
 */
void periodic_callbacks__initialize(void) {
  // This method is invoked once when the periodic tasks are created
  // turn OFF all LEDs at the beginning
  gpio__set(board_io__get_led0());
  gpio__set(board_io__get_led1());
  gpio__set(board_io__get_led2());
  gpio__set(board_io__get_led3());
  can_bus_module__init(can1);
}

void periodic_callbacks__1Hz(uint32_t callback_count) {
  // gpio__toggle(board_io__get_led0());
  can_bus_module__check_for_bus_off(can1);
  print_heading_and_motor_cmds();
}

void periodic_callbacks__10Hz(uint32_t callback_count) {
}

void periodic_callbacks__100Hz(uint32_t callback_count) {
  // gpio__toggle(board_io__get_led2());
  driver_controller__read_all_can_messages();
  driver_controller__send_cmd_to_motor_over_can();
}

/**
 * @warning
 * This is a very fast 1ms task and care must be taken to use this
 * This may be disabled based on intialization of periodic_scheduler__initialize()
 */
void periodic_callbacks__1000Hz(uint32_t callback_count) {
  gpio__toggle(board_io__get_led3());
  // Add your code here
}
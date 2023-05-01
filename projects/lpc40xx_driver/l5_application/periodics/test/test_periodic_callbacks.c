#include <stdio.h>
#include <string.h>

#include "unity.h"

// Include the Mocks
// - This will not pull the REAL source code of these modules (such as board_io.c)
// - This will auto-generate "Mock" versions based on the header file
#include "Mockboard_io.h"
#include "Mockcan_bus.h"
#include "Mockcan_bus_module.h"
#include "Mockdriver_controller.h"
#include "Mockgpio.h"
#include "Mocksteer_processor.h"

#include "MockI2C_init.h"
#include "MockLCD_init.h"
#include "MockLCD_process.h"

// Include the source we wish to test
#include "periodic_callbacks.h"

void setUp(void) {}

void tearDown(void) {}

void test__periodic_callbacks__initialize(void) {
  gpio_s gpio;
  gpio__set_Expect(gpio);
  board_io__get_led0_ExpectAndReturn(gpio);
  gpio__set_Expect(gpio);
  board_io__get_led1_ExpectAndReturn(gpio);
  gpio__set_Expect(gpio);
  board_io__get_led2_ExpectAndReturn(gpio);
  gpio__set_Expect(gpio);
  board_io__get_led3_ExpectAndReturn(gpio);

  can_bus_module__init_ExpectAndReturn(can1, true);
  steer_processor__obstacle_LEDs_init_Expect();
  I2C_init_Expect();

  init__LCD_Expect();

  periodic_callbacks__initialize();
}

void test_periodic_callbacks__1Hz(uint32_t callback_count) {
  // gpio__toggle(board_io__get_led0());
  can_bus_module__check_for_bus_off_IgnoreAndReturn(can1);
  print_heading_and_motor_cmds_Ignore();
  LCD_display_1Hz_Expect();
  periodic_callbacks__1Hz(0);
}

void test__periodic_callbacks__100Hz(void) {
  // gpio_s gpio;
  // gpio__toggle_Expect(gpio);
  // board_io__get_led1_ExpectAndReturn(gpio);
  driver_controller__read_all_can_messages_Expect();
  driver_controller__send_cmd_to_motor_over_can_ExpectAndReturn(true);
  periodic_callbacks__100Hz(0);
}
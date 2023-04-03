#include <stdio.h>
#include <string.h>

#include "unity.h"

// Include the Mocks
// - This will not pull the REAL source code of these modules (such as board_io.c)
// - This will auto-generate "Mock" versions based on the header file
#include "Mockboard_io.h"
#include "Mockcan_bus.h"
#include "Mockcan_bus_module.h"
#include "Mockgeo_controller.h"
#include "Mockgpio.h"
#include "Mockgps.h"

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
  gps__init_Expect();
  periodic_callbacks__initialize();
}

void test__periodic_callbacks__1Hz(void) { periodic_callbacks__1Hz(0); }

void test__periodic_callbacks__100Hz(void) {
  // geo_controller__read_all_can_messages_Expect();
  geo_controller__read_current_coordinates_Expect();
  geo_controller__calculate_heading_Expect();
  // geo_controller__send_heading_to_driver_over_can_ExpectAndReturn(true);
  // gps__get_gps_data_and_parse_coordinates_Expect();
  periodic_callbacks__100Hz(0);
}
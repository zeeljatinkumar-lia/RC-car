#include <stdio.h>
#include <string.h>

#include "unity.h"

// Include the Mocks
// - This will not pull the REAL source code of these modules (such as board_io.c)
// - This will auto-generate "Mock" versions based on the header file
#include "Mockboard_io.h"
#include "Mockgpio.h"

// SR04 sensor Mock
#include "MockSR04_sensor_pin_init.h"
#include "MockSR04_ultrasonic_sensor.h"
// CAN_bus Mock
#include "Mockcan_ultrasonic_sensor_handler.h"
#include "Mockcan_ultrasonic_sensor_initializer.h"
// bridge Mock
#include "Mockbridge_controller.h"
// LV_sensor Mocks
#include "MockLV_sensor_controller.h"

// Include the source we wish to test
#include "periodic_callbacks.h"

void setUp(void) {}

void tearDown(void) {}

void test__periodic_callbacks__initialize(void) {
  gpio_s gpio;
  gpio__set_Expect(gpio);
  board_io__get_led0_ExpectAndReturn(gpio);
  can_ultrasonic_init_ExpectAndReturn(true);
  gpio__set_Expect(gpio);
  board_io__get_led1_ExpectAndReturn(gpio);
  gpio__set_Expect(gpio);
  board_io__get_led2_ExpectAndReturn(gpio);
  gpio__set_Expect(gpio);
  board_io__get_led3_ExpectAndReturn(gpio);

  Bridge_Controller_init_Expect();
  Sensor_Controller_init_Expect();
  periodic_callbacks__initialize();
}

void test__periodic_callbacks__1Hz(void) {

  can_ultrasonic_reset_ExpectAndReturn(true);
  Sensor_Controller__print_sensor_values_Expect();
  periodic_callbacks__1Hz(0);
}

void test__periodic_callbacks__10Hz(void) {
  Bridge_Controller__10hz_handler_Expect();
  Sensor_Controller__10hz_handler_ExpectAnyArgs();
  can_ultrasonic_sensor_transmit_messages_10hz_Expect();
  can_bridge_controller__Sending_dest_location_Expect();
  bridge_controller_transmit_sensor_value_to_app_Expect();
  periodic_callbacks__10Hz(0);
}

void test__periodic_callbacks__100Hz(void) { periodic_callbacks__100Hz(0); }

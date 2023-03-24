#include <stdio.h>
#include <string.h>

#include "unity.h"

// Include the Mocks
// - This will not pull the REAL source code of these modules (such as board_io.c)
// - This will auto-generate "Mock" versions based on the header file
#include "Mockboard_io.h"
#include "Mockgpio.h"

// newly added Mock
#include "Mockcan_dbc_initializer.h"
#include "Mockcan_handler.h"
//#include "Mockultrasonic_sensor.h"

// Include the source we wish to test
#include "periodic_callbacks.h"

void setUp(void) {}

void tearDown(void) {}

void test__periodic_callbacks__initialize(void) {
  can_dbc_init_ExpectAndReturn(true);
  periodic_callbacks__initialize();
}

void test__periodic_callbacks__1Hz(void) {
  // dbc_ULTRASONIC_TO_DRIVER_s test_sensor_struct;
  get_ultrasonic_sensor_data_Expect();
  can_dbc_reset_ExpectAndReturn(true);
  periodic_callbacks__1Hz(0);
}

void test__periodic_callbacks__10Hz(void) {
  can_handler__transmit_messages_10hz_Expect();
  periodic_callbacks__10Hz(0);
}

/*void test__periodic_callbacks__100Hz(void) {
  can_handler__manage_mia_10hz_Expect();
  periodic_callbacks__100Hz(0);
}*/
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

// Include the source we wish to test
#include "periodic_callbacks.h"

void setUp(void) {}

void tearDown(void) {}

void test__periodic_callbacks__initialize(void) {
  can_bus_module__init_ExpectAndReturn(can1, true);
  periodic_callbacks__initialize();
}

void test__periodic_callbacks__10Hz(void) {
  driver_controller__read_all_can_messages_Expect();
  driver_controller__send_cmd_to_motor_over_can_ExpectAndReturn(true);
  periodic_callbacks__10Hz(0);
}
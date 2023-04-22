
#include "Mockboard_io.h"
#include "Mockcan_bus.h"
#include "Mockgpio.h"
#include "can_handler.c"

#include "project.h"
#include "unity.h"

/*void test_can_handler__manage_mia_10hz(void) {
  sensor_struct_mia.mia_info.mia_counter = 500;
  gpio_s led1 = {0};
  board_io__get_led3_ExpectAndReturn(led1);
  gpio__toggle_Expect(led1);

  can_handler__manage_mia_10hz();
}*/

void test_can_handler__transmit_messages_10hz(void) {
  dbc_ULTRASONIC_TO_DRIVER_s test_sensor_struct = {};
  can__tx_ExpectAndReturn(can1, &can_msg, 0, true);
  can_handler__transmit_messages_10hz();
}

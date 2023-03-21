#include "project.h"
#include "unity.h"

#include "Mockboard_io.h"
#include "Mockcan_bus.h"
#include "Mockgpio.h"
#include "Mocksteer_processor.h"

#include "driver_controller.c"

void setUp() { memset(&sensor_val, 0, sizeof(sensor_val)); }

void tearDown() {}

void test_driver_controller__mia_happens() {
  gpio_s gpio;
  sensor_val.mia_info.mia_counter = 90;
  board_io__get_led3_ExpectAndReturn(gpio);
  gpio__reset_Expect(gpio);
  driver_controller__manage_mia();
}

static void helper_function_to_encode_a_sensor_value(can__msg_t *msg, dbc_ULTRASONIC_TO_DRIVER_s val) {
  dbc_message_header_t header = dbc_encode_ULTRASONIC_TO_DRIVER(msg->data.bytes, &val);
  msg->frame_fields.data_len = header.message_dlc;
  msg->msg_id = header.message_id;
}

void test_driver_controller__decode_sensor_message() {
  can__msg_t can;
  dbc_ULTRASONIC_TO_DRIVER_s val = {
      .ULTRASONIC_TO_DRIVER_front = 10, .ULTRASONIC_TO_DRIVER_left = 20, .ULTRASONIC_TO_DRIVER_right = 30};
  helper_function_to_encode_a_sensor_value(&can, val);
  driver_controller__decode_sensor_message(&can);
  TEST_ASSERT_EQUAL(val.ULTRASONIC_TO_DRIVER_front, sensor_val.ULTRASONIC_TO_DRIVER_front);
  TEST_ASSERT_EQUAL(val.ULTRASONIC_TO_DRIVER_left, sensor_val.ULTRASONIC_TO_DRIVER_left);
  TEST_ASSERT_EQUAL(val.ULTRASONIC_TO_DRIVER_right, sensor_val.ULTRASONIC_TO_DRIVER_right);
}

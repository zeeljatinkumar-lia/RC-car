#include "project.h"
#include "unity.h"

#include "Mockboard_io.h"
#include "Mockcan_bus.h"
#include "Mockgpio.h"
#include "Mocksteer_processor.h"

#include "MockLCD_process.h"

#include "driver_controller.c"

void setUp() { memset(&sensor_val, 0, sizeof(sensor_val)); }

void tearDown() {}

void ignore_mia_led() {
  gpio_s gpio;
  gpio__set_Ignore();
  gpio__reset_Ignore();
  board_io__get_led3_IgnoreAndReturn(gpio);
}

void test_driver_controller__mia_happens() {
  ignore_mia_led();
  sensor_val.mia_info.mia_counter = 90;
  driver_controller__manage_mia();
}

static void helper_function_to_encode_a_sensor_value(can__msg_t *msg, dbc_ULTRASONIC_TO_DRIVER_s val) {
  dbc_message_header_t header = dbc_encode_ULTRASONIC_TO_DRIVER(msg->data.bytes, &val);
  msg->frame_fields.data_len = header.message_dlc;
  msg->msg_id = header.message_id;
}

void test_driver_controller__decode_sensor_message() {
  can__msg_t msg;
  dbc_ULTRASONIC_TO_DRIVER_s val = {
      .ULTRASONIC_TO_DRIVER_front = 10, .ULTRASONIC_TO_DRIVER_left = 20, .ULTRASONIC_TO_DRIVER_right = 30};
  helper_function_to_encode_a_sensor_value(&msg, val);
  driver_controller__decode_sensor_message(&msg);
  TEST_ASSERT_EQUAL(val.ULTRASONIC_TO_DRIVER_front, sensor_val.ULTRASONIC_TO_DRIVER_front);
  TEST_ASSERT_EQUAL(val.ULTRASONIC_TO_DRIVER_left, sensor_val.ULTRASONIC_TO_DRIVER_left);
  TEST_ASSERT_EQUAL(val.ULTRASONIC_TO_DRIVER_right, sensor_val.ULTRASONIC_TO_DRIVER_right);
}

void test_driver_controller__read_zero_messages() {
  can__rx_ExpectAndReturn(can1, NULL, 0, false);
  can__rx_IgnoreArg_can_message_ptr();
  driver_controller__read_all_can_messages();
}

void test_driver_controller__read_one_messages() {
  ignore_mia_led();
  can__rx_ExpectAndReturn(can1, NULL, 0, true);
  can__rx_IgnoreArg_can_message_ptr();
  can__rx_ExpectAndReturn(can1, NULL, 0, false);
  can__rx_IgnoreArg_can_message_ptr();

  steer_processor_Expect(&motor_val, sensor_val, geo_heading);
  update_sensor_for_LCD_Expect(&sensor_val);
  update_compass_for_LCD_Expect(&geo_heading);
  update_motor_for_LCD_Expect(&motor_val);

  driver_controller__read_all_can_messages();
}

void test_driver_controller__can_send_fail() {
  gpio_s gpio;
  can__tx_ExpectAndReturn(can1, NULL, 0, false);
  can__tx_IgnoreArg_can_message_ptr();
  gpio__set_Expect(gpio);
  board_io__get_led0_ExpectAndReturn(gpio);
  TEST_ASSERT_FALSE(driver_controller__send_cmd_to_motor_over_can());
}

void test_driver_controller__can_send_successful() {
  gpio_s gpio;
  can__tx_ExpectAndReturn(can1, NULL, 0, true);
  can__tx_IgnoreArg_can_message_ptr();
  gpio__reset_Expect(gpio);
  board_io__get_led0_ExpectAndReturn(gpio);
  TEST_ASSERT_TRUE(driver_controller__send_cmd_to_motor_over_can());
}

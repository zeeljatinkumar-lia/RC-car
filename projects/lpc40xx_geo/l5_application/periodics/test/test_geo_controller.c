#include "Mockboard_io.h"
#include "Mockcan_bus.h"
#include "Mockcompass.h"
#include "Mockgpio.h"
#include "Mockgps.h"

#include "haversine.h"
#include "project.h"

#include "geo_controller.c"

void setUp(void) {}

void tearDown(void) {}

void test_geo_controller__mia_happens() {
  gpio_s gpio;
  dest_coord.mia_info.mia_counter = 90;
  board_io__get_led3_ExpectAndReturn(gpio);
  gpio__reset_Expect(gpio);
  geo_controller__manage_mia();
}

static void helper_function_to_encode_a_bridge_value(can__msg_t *msg, dbc_GPS_DESTINATION_s val) {
  dbc_message_header_t header = dbc_encode_GPS_DESTINATION(msg->data.bytes, &val);
  msg->frame_fields.data_len = header.message_dlc;
  msg->msg_id = header.message_id;
}

void test_geo_controller__decode_bridge_message() {
  can__msg_t msg;
  dbc_GPS_DESTINATION_s val = {.GPS_DEST_LATITUDE_SCALED_100000 = 37 * 100000,
                               .GPS_DEST_LONGITUDE_SCALED_100000 = -121 * 100000};
  helper_function_to_encode_a_bridge_value(&msg, val);
  geo_controller__decode_bridge_message(&msg);
  TEST_ASSERT_EQUAL(val.GPS_DEST_LATITUDE_SCALED_100000, dest_coord.GPS_DEST_LATITUDE_SCALED_100000);
  TEST_ASSERT_EQUAL(val.GPS_DEST_LONGITUDE_SCALED_100000, dest_coord.GPS_DEST_LONGITUDE_SCALED_100000);
}

void test_geo_controller__read_zero_messages() {
  can__rx_ExpectAndReturn(can1, NULL, 0, false);
  can__rx_IgnoreArg_can_message_ptr();
  geo_controller__read_all_can_messages();
}

void test_geo_controller__read_one_messages() {
  gpio_s gpio;
  can__rx_ExpectAndReturn(can1, NULL, 0, true);
  can__rx_IgnoreArg_can_message_ptr();
  can__rx_ExpectAndReturn(can1, NULL, 0, false);
  can__rx_IgnoreArg_can_message_ptr();
  board_io__get_led3_ExpectAndReturn(gpio);
  gpio__set_Expect(gpio);
  geo_controller__read_all_can_messages();
}

void test_geo_controller__can_send_fail() {
  can__tx_ExpectAndReturn(can1, NULL, 0, false);
  can__tx_IgnoreArg_can_message_ptr();
  TEST_ASSERT_FALSE(geo_controller__send_heading_to_driver_over_can());
}

void test_geo_controller__can_send_successful() {
  gpio_s gpio;
  can__tx_ExpectAndReturn(can1, NULL, 0, true);
  can__tx_IgnoreArg_can_message_ptr();
  gpio__toggle_Expect(gpio);
  board_io__get_led0_ExpectAndReturn(gpio);
  TEST_ASSERT_TRUE(geo_controller__send_heading_to_driver_over_can());
}
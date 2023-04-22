#include "driver_controller.h"
#include "board_io.h"
#include "can_bus.h"
#include "gpio.h"
#include "project.h"
#include "steer_processor.h"

#include "stdio.h"

#define MIA_LED board_io__get_led3()

const dbc_ULTRASONIC_TO_DRIVER_s dbc_mia_replacement_ULTRASONIC_TO_DRIVER = {
    .ULTRASONIC_TO_DRIVER_front = 100, .ULTRASONIC_TO_DRIVER_left = 100, .ULTRASONIC_TO_DRIVER_right = 100};
const uint32_t dbc_mia_threshold_ULTRASONIC_TO_DRIVER = 100;

static dbc_ULTRASONIC_TO_DRIVER_s sensor_val;
static dbc_GEO_STATUS_s geo_heading;
static dbc_DRIVER_TO_MOTOR_s motor_val;

static void driver_controller__manage_mia() {
  const uint32_t mia_increment_value = 10;
  if (dbc_service_mia_ULTRASONIC_TO_DRIVER(&sensor_val, mia_increment_value)) {
    // gpio__reset(MIA_LED); // turn ON to indicate MIA
  }
}

static void driver_controller__decode_sensor_message(can__msg_t *msg) {
  const dbc_message_header_t header = {.message_dlc = msg->frame_fields.data_len, .message_id = msg->msg_id};
  dbc_decode_ULTRASONIC_TO_DRIVER(&sensor_val, header, msg->data.bytes);
}

static void driver_controller__decode_geo_message(can__msg_t *msg) {
  const dbc_message_header_t header = {.message_dlc = msg->frame_fields.data_len, .message_id = msg->msg_id};
  dbc_decode_GEO_STATUS(&geo_heading, header, msg->data.bytes);
}

static void driver_controller__encode_motor_message(can__msg_t *msg) {
  dbc_message_header_t header = {0};
  header = dbc_encode_DRIVER_TO_MOTOR(msg->data.bytes, &motor_val);
  msg->frame_fields.data_len = header.message_dlc;
  msg->msg_id = header.message_id;
}

void print_heading_and_motor_cmds() {
  printf("heading=%d, dist=%f, steer=%d, speed=%d\n", geo_heading.GEO_STATUS_COMPASS_HEADING,
         geo_heading.GEO_STATUS_DISTANCE_TO_DESTINATION, motor_val.DRIVER_TO_MOTOR_steer,
         motor_val.DRIVER_TO_MOTOR_speed);
}

void driver_controller__read_all_can_messages() {
  can__msg_t msg = {0};
  while (can__rx(can1, &msg, 0)) {
    driver_controller__decode_sensor_message(&msg);
    driver_controller__decode_geo_message(&msg);
    steer_processor(&motor_val, sensor_val, geo_heading);
    // gpio__set(MIA_LED); // turn OFF since we received the CAN message
  }
  driver_controller__manage_mia();
}

bool driver_controller__send_cmd_to_motor_over_can() {
  bool tx_status = false;
  can__msg_t msg = {0};
  driver_controller__encode_motor_message(&msg);
  tx_status = can__tx(can1, &msg, 0);
  if (tx_status) {
    // Toggle LED0 for each successful transmission
    gpio__toggle(board_io__get_led0());
  }
  return tx_status;
}
#include "motor_controller.h"
#include "board_io.h"
#include "can_bus.h"
#include "can_bus_module.h"
#include "gpio.h"
#include "motor.h"
#include "project.h"
#include "rpm.h"
#include <stdio.h>

static gpio_s MIA_LED;

const dbc_DRIVE_STATUS_CMD_s dbc_mia_replacement_DRIVE_STATUS_CMD = {.DRIVE_STATUS_CMD_start = 0};
const dbc_DRIVER_TO_MOTOR_s dbc_mia_replacement_DRIVER_TO_MOTOR = {.DRIVER_TO_MOTOR_speed = 0,
                                                                   .DRIVER_TO_MOTOR_steer = 0};

const uint32_t dbc_mia_threshold_DRIVE_STATUS_CMD = 1500;
const uint32_t dbc_mia_threshold_DRIVER_TO_MOTOR = 1500;

static dbc_DRIVE_STATUS_CMD_s car_on_off_command;
static dbc_DRIVER_TO_MOTOR_s motor_val;
static dbc_MOTOR_TO_APP_DBG_s speed_val;

static bool motor_controller__manage_start_stop_mia() {
  bool went_into_mia = false;
  const uint32_t mia_increment_value = 100;
  if (dbc_service_mia_DRIVE_STATUS_CMD(&car_on_off_command, mia_increment_value)) {
    went_into_mia = true;
  }
  return went_into_mia;
}

static bool motor_controller__manage_driver_cmd_mia() {
  bool went_into_mia = false;
  const uint32_t mia_increment_value = 100;
  if (dbc_service_mia_DRIVER_TO_MOTOR(&motor_val, mia_increment_value)) {
    went_into_mia = true;
  }
  return went_into_mia;
}

static void motor_controller__manage_mia() {
  bool a = false, b = false;
  a = motor_controller__manage_start_stop_mia();
  b = motor_controller__manage_driver_cmd_mia();
  if (a || b) {
    gpio__set(MIA_LED); // turn ON to indicate MIA
  }
}

void motor_controller__run_motor() {
  //printf("%d\n", car_on_off_command.DRIVE_STATUS_CMD_start);
  if (car_on_off_command.DRIVE_STATUS_CMD_start == 1) {
    motor__run_dc_motor_by_speed((double)motor_val.DRIVER_TO_MOTOR_speed / 1000.0);
    motor__turn_servo_by_angle(motor_val.DRIVER_TO_MOTOR_steer);
  } else {
    motor__run_dc_motor_by_speed(0);
    motor__turn_servo_by_angle(0);
  }
}

/*static int speed_sign = 1, steer_sign = 1;
void fake_motor_values() {
  if (motor_val.DRIVER_TO_MOTOR_speed <= -30 || motor_val.DRIVER_TO_MOTOR_speed >= 30) {
    motor_val.DRIVER_TO_MOTOR_speed = 0;
    speed_sign = -1 * speed_sign;
  } else {
    motor_val.DRIVER_TO_MOTOR_speed += (speed_sign * 5);
  }
  if (motor_val.DRIVER_TO_MOTOR_steer <= -60 || motor_val.DRIVER_TO_MOTOR_steer >= 60) {
    motor_val.DRIVER_TO_MOTOR_steer = 0;
    steer_sign = -1 * steer_sign;
  } else {
    motor_val.DRIVER_TO_MOTOR_steer += (steer_sign * 10);
  }
  printf("speed = %d, steer = %d,\n", motor_val.DRIVER_TO_MOTOR_speed, motor_val.DRIVER_TO_MOTOR_steer);
}*/

void motor_controller__print_motor_cmd_values() {
  printf("speed=%ld,steer=%d,curr_speed=%ld\n", motor_val.DRIVER_TO_MOTOR_speed, motor_val.DRIVER_TO_MOTOR_steer,
         speed_val.MOTOR_TO_APP_DBG_current_speed);
}

static void motor_controller__decode_start_stop_message(can__msg_t *msg) {
  const dbc_message_header_t header = {.message_dlc = msg->frame_fields.data_len, .message_id = msg->msg_id};
  dbc_decode_DRIVE_STATUS_CMD(&car_on_off_command, header, msg->data.bytes);
}

static void motor_controller__decode_driver_message(can__msg_t *msg) {
  const dbc_message_header_t header = {.message_dlc = msg->frame_fields.data_len, .message_id = msg->msg_id};
  dbc_decode_DRIVER_TO_MOTOR(&motor_val, header, msg->data.bytes);
}

void motor_controller__read_all_can_messages() {
  can__msg_t msg = {0};
  // fake_motor_values();
  while (can__rx(can1, &msg, 0)) {
    motor_controller__decode_start_stop_message(&msg);
    motor_controller__decode_driver_message(&msg);
    gpio__reset(MIA_LED); // turn OFF since we received the CAN message
  }
  motor_controller__manage_mia();
}

static void motor_controller__encode_speed_message(can__msg_t *msg) {
  dbc_message_header_t header = {0};

  speed_val.MOTOR_TO_APP_DBG_current_speed = (int)(1000.0 * rpm_sensor__get_current_speed());
  speed_val.MOTOR_TO_APP_DBG_current_steer = motor_val.DRIVER_TO_MOTOR_steer;

  header = dbc_encode_MOTOR_TO_APP_DBG(msg->data.bytes, &speed_val);
  msg->frame_fields.data_len = header.message_dlc;
  msg->msg_id = header.message_id;
}

bool motor_controller__send_motor_speed_over_can() {
  bool tx_status = false;
  can__msg_t msg = {0};
  motor_controller__encode_speed_message(&msg);
  tx_status = can__tx(can1, &msg, 0);
  if (tx_status) {
    // Turn ON LED0 if successful transmission
    gpio__reset(board_io__get_led0());
  } else {
    gpio__set(board_io__get_led0());
  }
  return tx_status;
}

void motor_controller__init() { MIA_LED = gpio__construct_as_output(GPIO__PORT_0, 17); }
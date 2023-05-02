#include "motor_controller.h"
#include "board_io.h"
#include "can_bus.h"
#include "can_bus_module.h"
#include "gpio.h"
#include "motor.h"
#include "project.h"
#include "rpm.h"
#include <stdio.h>

#define MIA_LED board_io__get_led3()

const dbc_DRIVER_TO_MOTOR_s dbc_mia_replacement_DRIVER_TO_MOTOR = {.DRIVER_TO_MOTOR_speed = 9,
                                                                   .DRIVER_TO_MOTOR_steer = 0};

const uint32_t dbc_mia_threshold_DRIVER_TO_MOTOR = 100;

static dbc_DRIVER_TO_MOTOR_s motor_val;
static dbc_MOTOR_TO_APP_DBG_s speed_val;

static void motor_controller__manage_mia() {
  const uint32_t mia_increment_value = 10;
  if (dbc_service_mia_DRIVER_TO_MOTOR(&motor_val, mia_increment_value)) {
    gpio__reset(MIA_LED); // turn ON to indicate MIA
  }
}

static void motor_controller__run_motor() {
  motor__run_dc_motor_by_speed(motor_val.DRIVER_TO_MOTOR_speed);
  motor__turn_servo_by_angle(motor_val.DRIVER_TO_MOTOR_steer);
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
  printf("speed=%d,steer=%d\n", motor_val.DRIVER_TO_MOTOR_speed, motor_val.DRIVER_TO_MOTOR_steer);
}

void motor_controller__read_all_can_messages() {
  can__msg_t msg = {0};
  dbc_message_header_t header = {0};
  // fake_motor_values();
  // motor_controller__run_motor();
  rpm_sensor__update_speed_value(&speed_val);
  while (can__rx(can1, &msg, 0)) {
    gpio__set(MIA_LED); // turn OFF since we received the CAN message
    header.message_dlc = msg.frame_fields.data_len;
    header.message_id = msg.msg_id;
    dbc_decode_DRIVER_TO_MOTOR(&motor_val, header, msg.data.bytes);
    motor_controller__run_motor();
  }
  motor_controller__manage_mia();
}

static void motor_controller__encode_speed_message(can__msg_t *msg) {
  dbc_message_header_t header = {0};

  // TODO: remove this once we start getting actual speed from RPM sensor
  speed_val.MOTOR_TO_APP_DBG_current_speed = motor_val.DRIVER_TO_MOTOR_speed;
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
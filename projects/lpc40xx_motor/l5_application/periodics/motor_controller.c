#include "motor_controller.h"
#include "board_io.h"
#include "can_bus.h"
#include "can_bus_module.h"
#include "gpio.h"
#include "project.h"

#define MIA_LED board_io__get_led3()

const dbc_DRIVER_TO_MOTOR_s dbc_mia_replacement_DRIVER_TO_MOTOR = {
    .DRIVER_TO_MOTOR_reverse = 0, .DRIVER_TO_MOTOR_speed = 9, .DRIVER_TO_MOTOR_steer = DRIVER_TO_MOTOR_steer__LEFT};

const uint32_t dbc_mia_threshold_DRIVER_TO_MOTOR = 100;

static dbc_DRIVER_TO_MOTOR_s motor_val;

static void motor_controller__run_motor() {
  DRIVER_TO_MOTOR_steer_e steer = motor_val.DRIVER_TO_MOTOR_steer;
  if (steer == DRIVER_TO_MOTOR_steer__RIGHT) {
    gpio__set(board_io__get_led0());
    gpio__set(board_io__get_led1());
    gpio__reset(board_io__get_led2());
  } else if (steer == DRIVER_TO_MOTOR_steer__STRAIGHT) {
    gpio__set(board_io__get_led0());
    gpio__reset(board_io__get_led1());
    gpio__set(board_io__get_led2());
  } else if (steer == DRIVER_TO_MOTOR_steer__LEFT) {
    gpio__reset(board_io__get_led0());
    gpio__set(board_io__get_led1());
    gpio__set(board_io__get_led2());
  }
}

void motor_controller__read_all_can_messages() {
  can__msg_t msg = {0};
  dbc_message_header_t header = {0};
  while (can__rx(can1, &msg, 0)) {
    gpio__set(MIA_LED); // turn OFF since we received the CAN message
    header.message_dlc = msg.frame_fields.data_len;
    header.message_id = msg.msg_id;
    dbc_decode_DRIVER_TO_MOTOR(&motor_val, header, msg.data.bytes);
    motor_controller__run_motor();
  }
  if (dbc_service_mia_DRIVER_TO_MOTOR(&motor_val, 10)) {
    gpio__reset(MIA_LED); // turn ON to indicate MIA
  }
}
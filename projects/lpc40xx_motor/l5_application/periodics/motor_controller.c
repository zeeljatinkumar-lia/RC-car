#include "motor_controller.h"
#include "board_io.h"
#include "can_bus.h"
#include "can_bus_module.h"
#include "gpio.h"
#include "motor.h"
#include "project.h"
#include <stdio.h>

#define MIA_LED board_io__get_led3()

const dbc_DRIVER_TO_MOTOR_s dbc_mia_replacement_DRIVER_TO_MOTOR = {
    .DRIVER_TO_MOTOR_reverse = 0, .DRIVER_TO_MOTOR_speed = 9, .DRIVER_TO_MOTOR_steer = 0};

const uint32_t dbc_mia_threshold_DRIVER_TO_MOTOR = 100;

static dbc_DRIVER_TO_MOTOR_s motor_val;

static void motor_controller__run_motor() {
  motor__run_dc_motor_by_speed(motor_val.DRIVER_TO_MOTOR_speed, motor_val.DRIVER_TO_MOTOR_reverse);
  motor__turn_servo_by_angle(motor_val.DRIVER_TO_MOTOR_steer);
}

/*
static int sign = 1;
void fake_motor_values() {
  if (motor_val.DRIVER_TO_MOTOR_speed >= 15) {
    motor_val.DRIVER_TO_MOTOR_speed = 0;
    sign = -1 * sign;
    if (sign > 0) {
      motor_val.DRIVER_TO_MOTOR_reverse = 0;
    } else {
      motor_val.DRIVER_TO_MOTOR_reverse = 1;
    }
  } else {
    motor_val.DRIVER_TO_MOTOR_speed += 2.5f;
  }
  if (motor_val.DRIVER_TO_MOTOR_steer <= -60 || motor_val.DRIVER_TO_MOTOR_steer >= 60) {
    motor_val.DRIVER_TO_MOTOR_steer = 0;
  } else {
    motor_val.DRIVER_TO_MOTOR_steer += (sign * 10);
  }
  printf("speed = %d, steer = %d, revere = %d\n", motor_val.DRIVER_TO_MOTOR_speed, motor_val.DRIVER_TO_MOTOR_steer,
         motor_val.DRIVER_TO_MOTOR_reverse);
}*/

void motor_controller__read_all_can_messages() {
  can__msg_t msg = {0};
  dbc_message_header_t header = {0};
  // fake_motor_values();
  // motor_controller__run_motor();
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
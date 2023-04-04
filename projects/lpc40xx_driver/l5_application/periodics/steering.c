#include "steering.h"
#include "project.h"

void steer_right(dbc_DRIVER_TO_MOTOR_s *motor_val) {
  motor_val->DRIVER_TO_MOTOR_speed = 15;
  motor_val->DRIVER_TO_MOTOR_steer = -60;
  motor_val->DRIVER_TO_MOTOR_reverse = 0;
}

void steer_left(dbc_DRIVER_TO_MOTOR_s *motor_val) {
  motor_val->DRIVER_TO_MOTOR_speed = 15;
  motor_val->DRIVER_TO_MOTOR_steer = 60;
  motor_val->DRIVER_TO_MOTOR_reverse = 0;
}

void drive_straight(dbc_DRIVER_TO_MOTOR_s *motor_val) {
  motor_val->DRIVER_TO_MOTOR_speed = 15;
  motor_val->DRIVER_TO_MOTOR_steer = 0;
  motor_val->DRIVER_TO_MOTOR_reverse = 0;
}

void steer_reverse_and_turn_right(dbc_DRIVER_TO_MOTOR_s *motor_val) {
  motor_val->DRIVER_TO_MOTOR_speed = 15;
  motor_val->DRIVER_TO_MOTOR_steer = 0;
  motor_val->DRIVER_TO_MOTOR_reverse = 1;
}
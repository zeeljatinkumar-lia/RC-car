#include "steering.h"
#include "project.h"

void steer_right(dbc_DRIVER_TO_MOTOR_s *motor_val) {
  motor_val->DRIVER_TO_MOTOR_speed = 10;
  motor_val->DRIVER_TO_MOTOR_steer = -90.0; // 0 degrees
  motor_val->DRIVER_TO_MOTOR_reverse = 0;
}

void steer_left(dbc_DRIVER_TO_MOTOR_s *motor_val) {
  motor_val->DRIVER_TO_MOTOR_speed = 10;
  motor_val->DRIVER_TO_MOTOR_steer = 90.0; // 0 degrees
  motor_val->DRIVER_TO_MOTOR_reverse = 0;
}

void drive_straight(dbc_DRIVER_TO_MOTOR_s *motor_val) {
  motor_val->DRIVER_TO_MOTOR_speed = 10;
  motor_val->DRIVER_TO_MOTOR_steer = 0; // 0 degrees
  motor_val->DRIVER_TO_MOTOR_reverse = 0;
}
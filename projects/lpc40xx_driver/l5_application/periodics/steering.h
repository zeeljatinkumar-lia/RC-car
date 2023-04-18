#pragma once

#include "project.h"

void steer_left(dbc_DRIVER_TO_MOTOR_s *motor_val);
void steer_right(dbc_DRIVER_TO_MOTOR_s *motor_val);
void drive_straight(dbc_DRIVER_TO_MOTOR_s *motor_val);
void steer_reverse_and_turn_right(dbc_DRIVER_TO_MOTOR_s *motor_val);
void stop_driving(dbc_DRIVER_TO_MOTOR_s *motor_val);

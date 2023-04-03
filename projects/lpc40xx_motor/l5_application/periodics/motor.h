#pragma once

#include <stdint.h>

void motor__init(void);
void motor__turn_servo_by_angle(int degrees);
void motor__run_dc_motor_by_speed(float speed, bool reverse);
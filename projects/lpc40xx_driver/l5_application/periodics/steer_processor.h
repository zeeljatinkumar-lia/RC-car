#pragma once

#include "project.h"
#include <stdint.h>

#define STEER_THRESHOLD 25

void steer_processor(dbc_DRIVER_TO_MOTOR_s *motor_val, dbc_ULTRASONIC_TO_DRIVER_s sensor_val);

#pragma once

#include "project.h"
#include <stdint.h>

typedef enum speed_throttle_level {
  THROTTLE_LEVEL_0 = 0,
  THROTTLE_LEVEL_1 = 4,
  THROTTLE_LEVEL_2 = 5,
  THROTTLE_LEVEL_3 = 6,
  THROTTLE_LEVEL_4 = 7,
} speed_throttle_level_t;

typedef enum steer_angle_level {
  STEER_SHARP_RIGHT = -60,
  STEER_MEDIUM_RIGHT = -50,
  STEER_SOFT_RIGHT = -45,
  STEER_STRAIGHT = 0,
  STEER_SOFT_LEFT = 45,
  STEER_MEDIUM_LEFT = 50,
  STEER_SHARP_LEFT = 60,
} steer_angle_level_t;

typedef enum obstacle_threshold_level {
  OBSTACLE_THRESHOLD_LEVEL_0 = 0,
  OBSTACLE_THRESHOLD_LEVEL_1 = 50,  // unit cm
  OBSTACLE_THRESHOLD_LEVEL_2 = 75,  // unit cm
  OBSTACLE_THRESHOLD_LEVEL_3 = 100, // unit cm
  NO_OBSTACLE = 150,                // unit cm
} obstacle_threshold_level_t;

void steer_processor(dbc_DRIVER_TO_MOTOR_s *motor_val, dbc_ULTRASONIC_TO_DRIVER_s sensor_val,
                     dbc_GEO_STATUS_s geo_heading);
void steer_processor__obstacle_LEDs_init();
#pragma once

#include "project.h"
#include <stdint.h>

#define SPEED_FULL_THROTTLE 25                     // unit kmph
#define THROTTLE_RESOUTION SPEED_FULL_THROTTLE / 4 // unit kmph

typedef enum speed_throttle_level {
  THROTTLE_LEVEL_0 = 0,
  THROTTLE_LEVEL_1 = THROTTLE_RESOUTION * 1,
  THROTTLE_LEVEL_2 = THROTTLE_RESOUTION * 2,
  THROTTLE_LEVEL_3 = THROTTLE_RESOUTION * 3,
  THROTTLE_LEVEL_4 = THROTTLE_RESOUTION * 4,
} speed_throttle_level_t;

typedef enum steer_angle_level {
  STEER_SHARP_RIGHT = -60,
  STEER_MEDIUM_RIGHT = -45,
  STEER_SOFT_RIGHT = -30,
  STEER_STRAIGHT = 0,
  STEER_SOFT_LEFT = 30,
  STEER_MEDIUM_LEFT = 45,
  STEER_SHARP_LEFT = 60,
} steer_angle_level_t;

typedef enum obstacle_threshold_level {
  OBSTACLE_THRESHOLD_LEVEL_1 = 50,  // unit cm
  OBSTACLE_THRESHOLD_LEVEL_2 = 75,  // unit cm
  OBSTACLE_THRESHOLD_LEVEL_3 = 100, // unit cm
  OBSTACLE_THRESHOLD_LEVEL_4 = 150, // unit cm
} obstacle_threshold_level_t;

void steer_processor(dbc_DRIVER_TO_MOTOR_s *motor_val, dbc_ULTRASONIC_TO_DRIVER_s sensor_val,
                     dbc_GEO_STATUS_s geo_heading);
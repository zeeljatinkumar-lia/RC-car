#include "steer_processor.h"
#include "project.h"
#include "steering.h"

static bool front_path_is_clear(uint16_t front_sensor_cm) { return (front_sensor_cm > STEER_THRESHOLD); }

static bool right_path_is_clear(uint16_t right_sensor_cm) { return (right_sensor_cm > STEER_THRESHOLD); }

static bool left_path_is_clear(uint16_t left_sensor_cm) { return (left_sensor_cm > STEER_THRESHOLD); }

void steer_processor(dbc_DRIVER_TO_MOTOR_s *motor_val, dbc_ULTRASONIC_TO_DRIVER_s sensor_val) {
  uint16_t front_sensor_cm = sensor_val.ULTRASONIC_TO_DRIVER_front;
  uint16_t left_sensor_cm = sensor_val.ULTRASONIC_TO_DRIVER_left;
  uint16_t right_sensor_cm = sensor_val.ULTRASONIC_TO_DRIVER_right;

  if (front_path_is_clear(front_sensor_cm)) {
    drive_straight(motor_val);
  } else {
    if (right_path_is_clear(right_sensor_cm)) {
      steer_right(motor_val);
    } else if (left_path_is_clear(left_sensor_cm)) {
      steer_left(motor_val);
    }
  }
}

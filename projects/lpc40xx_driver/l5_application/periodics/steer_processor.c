#include "steer_processor.h"
#include "project.h"
#include "steering.h"

static bool front_path_is_clear(uint16_t front_sensor_cm) { return (front_sensor_cm > STEER_THRESHOLD); }

static bool right_path_is_clear(uint16_t right_sensor_cm) { return (right_sensor_cm > STEER_THRESHOLD); }

static bool left_path_is_clear(uint16_t left_sensor_cm) { return (left_sensor_cm > STEER_THRESHOLD); }

static bool check_for_obstacle(dbc_DRIVER_TO_MOTOR_s *motor_val, dbc_ULTRASONIC_TO_DRIVER_s sensor_val) {
  bool path_clear_status = false;
  uint16_t front_sensor_cm = sensor_val.ULTRASONIC_TO_DRIVER_front;
  uint16_t left_sensor_cm = sensor_val.ULTRASONIC_TO_DRIVER_left;
  uint16_t right_sensor_cm = sensor_val.ULTRASONIC_TO_DRIVER_right;
  if (front_path_is_clear(front_sensor_cm) && right_path_is_clear(right_sensor_cm) &&
      left_path_is_clear(left_sensor_cm)) {
    path_clear_status = true;
  } else {
    if (front_path_is_clear(front_sensor_cm) == false) {
      if (right_path_is_clear(right_sensor_cm)) {
        steer_right(motor_val);
      } else if (left_path_is_clear(left_sensor_cm)) {
        steer_left(motor_val);
      } else {
        // steer_reverse_and_turn_right(motor_val);
      }
    }
  }
  return path_clear_status;
}

static void drive_towards_destination(dbc_DRIVER_TO_MOTOR_s *motor_val, dbc_GEO_STATUS_s geo_heading) {
  int heading_diff = geo_heading.GEO_STATUS_COMPASS_BEARING - geo_heading.GEO_STATUS_COMPASS_HEADING;
  if (geo_heading.GEO_STATUS_DISTANCE_TO_DESTINATION) {
    if (heading_diff == 0) {
      drive_straight(motor_val);
    } else if (heading_diff > 0 && heading_diff <= 180) {
      // actual logic TBD
      steer_right(motor_val);
    } else {
      // actual logic TBD
      steer_left(motor_val);
    }
  }
}

void steer_processor(dbc_DRIVER_TO_MOTOR_s *motor_val, dbc_ULTRASONIC_TO_DRIVER_s sensor_val,
                     dbc_GEO_STATUS_s geo_heading) {
  bool is_path_clear = check_for_obstacle(motor_val, sensor_val);
  if (is_path_clear) {
    drive_towards_destination(motor_val, geo_heading);
  }
}

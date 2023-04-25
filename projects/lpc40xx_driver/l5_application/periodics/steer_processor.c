#include "steer_processor.h"
#include "board_io.h"
#include "gpio.h"
#include "project.h"
#include "steering.h"

#define CAR_STOP_DISTANCE_THRESHOLD 10 // unit meters

#define FRONT_OBSTACLE_LED board_io__get_led2()
#define LEFT_OBSTACLE_LED board_io__get_led1()
#define RIGHT_OBSTACLE_LED board_io__get_led3()

// static const gpio_s back_obstacle_led = {.port_number = 2, .pin_number = 40};

struct obstacle_status {
  obstacle_threshold_level_t front_status;
  obstacle_threshold_level_t right_status;
  obstacle_threshold_level_t left_status;
  obstacle_threshold_level_t back_status;
};

static struct obstacle_status current_obstacle_status;
static obstacle_threshold_level_t closest_obstacle_threshold_ahead;
static obstacle_threshold_level_t back_obstacle_threshold;

// Function to determine the threshold level of the obstacle
static obstacle_threshold_level_t get_obstacle_threshold(uint16_t sensor_cm) {
  obstacle_threshold_level_t level = OBSTACLE_THRESHOLD_LEVEL_1;
  if (sensor_cm > OBSTACLE_THRESHOLD_LEVEL_4) {
    level = OBSTACLE_THRESHOLD_LEVEL_4;
  } else if (sensor_cm > OBSTACLE_THRESHOLD_LEVEL_3) {
    level = OBSTACLE_THRESHOLD_LEVEL_3;
  } else if (sensor_cm > OBSTACLE_THRESHOLD_LEVEL_2) {
    level = OBSTACLE_THRESHOLD_LEVEL_2;
  } else if (sensor_cm > OBSTACLE_THRESHOLD_LEVEL_1) {
    level = OBSTACLE_THRESHOLD_LEVEL_1;
  }
  return level;
}

// Function to detect an obstacle and glow the obstacle LED
static bool check_sensor_value_and_glow_obstacle_led(uint16_t sensor_cm, gpio_s obstacle_led) {
  obstacle_threshold_level_t level = get_obstacle_threshold(sensor_cm);
  if (closest_obstacle_threshold_ahead > level) {
    closest_obstacle_threshold_ahead = level;
  }
  if (level == OBSTACLE_THRESHOLD_LEVEL_4) {
    gpio__set(obstacle_led);
    return true;
  } else {
    gpio__reset(obstacle_led);
    return false;
  }
}

// Function to check if the front path is clear
static bool front_path_is_clear(uint16_t front_sensor_cm) {
  return check_sensor_value_and_glow_obstacle_led(front_sensor_cm, FRONT_OBSTACLE_LED);
}

static bool right_path_is_clear(uint16_t right_sensor_cm) {
  return check_sensor_value_and_glow_obstacle_led(right_sensor_cm, RIGHT_OBSTACLE_LED);
}

static bool left_path_is_clear(uint16_t left_sensor_cm) {
  return check_sensor_value_and_glow_obstacle_led(left_sensor_cm, LEFT_OBSTACLE_LED);
}

static bool back_path_is_clear(uint16_t back_sensor_cm) {
  back_obstacle_threshold = get_obstacle_threshold(back_sensor_cm);
  if (back_obstacle_threshold == OBSTACLE_THRESHOLD_LEVEL_4) {
    // gpio__set(back_obstacle_led);
    return true;
  } else {
    // gpio__reset(back_obstacle_led);
    return false;
  }
}

static bool check_for_obstacles(dbc_DRIVER_TO_MOTOR_s *motor_val, dbc_ULTRASONIC_TO_DRIVER_s sensor_val) {
  bool path_clear_status = false;

  uint16_t front_sensor_cm = sensor_val.ULTRASONIC_TO_DRIVER_front;
  uint16_t left_sensor_cm = sensor_val.ULTRASONIC_TO_DRIVER_left;
  uint16_t right_sensor_cm = sensor_val.ULTRASONIC_TO_DRIVER_right;

  closest_obstacle_threshold_ahead = OBSTACLE_THRESHOLD_LEVEL_4;
  if (front_path_is_clear(front_sensor_cm) && right_path_is_clear(right_sensor_cm) &&
      left_path_is_clear(left_sensor_cm)) {
    path_clear_status = true;
  }
  return path_clear_status;
}

// Function to determine the speed level based on the obstacle threshold
int get_speed_level_based_on_obstacle_distance(void) {
  speed_throttle_level_t speed_level;
  switch (closest_obstacle_threshold_ahead) {
  case OBSTACLE_THRESHOLD_LEVEL_1:
    speed_level = THROTTLE_LEVEL_1;
    break;
  case OBSTACLE_THRESHOLD_LEVEL_2:
    speed_level = THROTTLE_LEVEL_2;
    break;
  case OBSTACLE_THRESHOLD_LEVEL_3:
    speed_level = THROTTLE_LEVEL_3;
    break;
  case OBSTACLE_THRESHOLD_LEVEL_4:
    speed_level = THROTTLE_LEVEL_4;
    break;
  default:
    speed_level = THROTTLE_LEVEL_1;
    break;
  }
  return (int)speed_level;
}

static bool is_front_completely_blocked() {
  if (current_obstacle_status.front_status == OBSTACLE_THRESHOLD_LEVEL_1 &&
      current_obstacle_status.left_status == OBSTACLE_THRESHOLD_LEVEL_1 &&
      current_obstacle_status.right_status == OBSTACLE_THRESHOLD_LEVEL_1) {
    return true;
  } else {
    return false;
  }
}

// Function to determine the steer angle based on the direction of the obstacles
static int get_steer_angle_based_on_obstacle_closeness(bool *reverse) {
  int angle = STEER_STRAIGHT;
  if (is_front_completely_blocked() == true) {
    if (back_obstacle_threshold == OBSTACLE_THRESHOLD_LEVEL_1) {
      // we are surrounded from all sides by obstacles. stay put.
      // do nothing
    } else {
      // reverse and turn right
      *reverse = true;
      angle = STEER_MEDIUM_RIGHT;
    }
  } else {
    switch (current_obstacle_status.front_status) {
    case OBSTACLE_THRESHOLD_LEVEL_4:
    case OBSTACLE_THRESHOLD_LEVEL_3:
      angle = STEER_STRAIGHT;
      break;
    case OBSTACLE_THRESHOLD_LEVEL_2:
    case OBSTACLE_THRESHOLD_LEVEL_1:
      // nested switch case used here
      switch (current_obstacle_status.left_status) {
      case OBSTACLE_THRESHOLD_LEVEL_3:
        angle += STEER_SOFT_RIGHT;
        break;
      case OBSTACLE_THRESHOLD_LEVEL_2:
        angle += STEER_MEDIUM_RIGHT;
        break;
      case OBSTACLE_THRESHOLD_LEVEL_1:
        angle += STEER_SHARP_RIGHT;
        break;
      }
      switch (current_obstacle_status.right_status) {
      case OBSTACLE_THRESHOLD_LEVEL_3:
        angle += STEER_SOFT_LEFT;
        break;
      case OBSTACLE_THRESHOLD_LEVEL_2:
        angle += STEER_MEDIUM_LEFT;
        break;
      case OBSTACLE_THRESHOLD_LEVEL_1:
        angle += STEER_SHARP_LEFT;
        break;
      }
      break;
    }
  }
}

static void avoid_obstacles(dbc_DRIVER_TO_MOTOR_s *motor_val) {
  bool reverse = false;
  int speed = 0;
  motor_val->DRIVER_TO_MOTOR_steer = get_steer_angle_based_on_obstacle_closeness(&reverse);
  speed = get_speed_level_based_on_obstacle_distance();
  if (reverse) {
    speed = -1 * speed;
  }
  motor_val->DRIVER_TO_MOTOR_speed = speed;
}

// Function to determine the speed level based on the distance to the destination
int get_speed_level_based_on_distance_to_destination(int destination_distance_meters) {
  speed_throttle_level_t speed_level;
  if (destination_distance_meters > 100) {
    speed_level = THROTTLE_LEVEL_4;
  } else if (destination_distance_meters > 50) {
    speed_level = THROTTLE_LEVEL_2;
  } else if (destination_distance_meters <= CAR_STOP_DISTANCE_THRESHOLD) {
    speed_level = THROTTLE_LEVEL_0;
  }
  return (int)speed_level;
}

// Function to determine the steer angle based on the direction of the destination
int get_steer_angle_based_on_destination_direction(int dest, int curr) {
  steer_angle_level_t steer = 0;
  // TODO : actual steer logic
  return (int)steer;
}

// Function to drive towards the destination when there are no obstacles
// The drive speed is based on the distance to the destination.
// Farther the destination, more is the speed
static void drive_towards_destination(dbc_DRIVER_TO_MOTOR_s *motor_val, dbc_GEO_STATUS_s geo_heading) {
  int dest_angle = geo_heading.GEO_STATUS_COMPASS_HEADING;
  int curr_angle = geo_heading.GEO_STATUS_COMPASS_BEARING;
  int dist_to_dest = geo_heading.GEO_STATUS_DISTANCE_TO_DESTINATION;

  motor_val->DRIVER_TO_MOTOR_steer = get_steer_angle_based_on_destination_direction(dest_angle, curr_angle);
  motor_val->DRIVER_TO_MOTOR_speed = get_speed_level_based_on_distance_to_destination(dist_to_dest);
}

// Function to check and avoid obstacles and go towards destination
void steer_processor(dbc_DRIVER_TO_MOTOR_s *motor_val, dbc_ULTRASONIC_TO_DRIVER_s sensor_val,
                     dbc_GEO_STATUS_s geo_heading) {
  bool is_path_clear = check_for_obstacles(motor_val, sensor_val);
  if (is_path_clear) {
    drive_towards_destination(motor_val, geo_heading);
  } else {
    avoid_obstacles(motor_val);
  }
}

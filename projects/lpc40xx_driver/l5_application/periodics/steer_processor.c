#include "steer_processor.h"
#include "board_io.h"
#include "gpio.h"
#include "project.h"
#include "steering.h"

#define CAR_STOP_DISTANCE_THRESHOLD 10 // unit meters

static int REVERSE_SPEED = -1 * THROTTLE_LEVEL_3;

static gpio_s FRONT_OBSTACLE_LED;
static gpio_s LEFT_OBSTACLE_LED;
static gpio_s RIGHT_OBSTACLE_LED;
static gpio_s BACK_OBSTACLE_LED;

struct obstacle_status {
  obstacle_threshold_level_t front_status;
  obstacle_threshold_level_t right_status;
  obstacle_threshold_level_t left_status;
  obstacle_threshold_level_t back_status;
};

static struct obstacle_status current_obstacle_status;
static obstacle_threshold_level_t closest_obstacle_threshold_ahead;

// Function to determine the threshold level of the obstacle
static obstacle_threshold_level_t get_obstacle_threshold(uint16_t sensor_cm) {
  obstacle_threshold_level_t level;
  if (sensor_cm >= NO_OBSTACLE) {
    level = NO_OBSTACLE; // 150 and above
  } else if (sensor_cm >= OBSTACLE_THRESHOLD_LEVEL_3) {
    level = OBSTACLE_THRESHOLD_LEVEL_3; // 100-149
  } else if (sensor_cm >= OBSTACLE_THRESHOLD_LEVEL_2) {
    level = OBSTACLE_THRESHOLD_LEVEL_2; // 75-99
  } else if (sensor_cm >= OBSTACLE_THRESHOLD_LEVEL_1) {
    level = OBSTACLE_THRESHOLD_LEVEL_1; // 50-74
  } else {
    level = OBSTACLE_THRESHOLD_LEVEL_0; // 0-49
  }
  return level;
}

// Function to detect an obstacle and glow the obstacle LED
static obstacle_threshold_level_t check_sensor_value_and_glow_obstacle_led(uint16_t sensor_cm, gpio_s obstacle_led) {
  obstacle_threshold_level_t level = get_obstacle_threshold(sensor_cm);
  if (level == NO_OBSTACLE) { // no obstacle detected. Turn OFF LED
    gpio__set(obstacle_led);
  } else {
    gpio__reset(obstacle_led); // obstacle detected. Turn ON LED
  }
  return level;
}

// Function to check if the front path is clear
static bool front_path_is_clear(uint16_t front_sensor_cm) {
  obstacle_threshold_level_t level = check_sensor_value_and_glow_obstacle_led(front_sensor_cm, FRONT_OBSTACLE_LED);
  current_obstacle_status.front_status = level;
  return (level == NO_OBSTACLE);
}

static bool right_path_is_clear(uint16_t right_sensor_cm) {
  obstacle_threshold_level_t level = check_sensor_value_and_glow_obstacle_led(right_sensor_cm, RIGHT_OBSTACLE_LED);
  current_obstacle_status.right_status = level;
  return (level == NO_OBSTACLE);
}

static bool left_path_is_clear(uint16_t left_sensor_cm) {
  obstacle_threshold_level_t level = check_sensor_value_and_glow_obstacle_led(left_sensor_cm, LEFT_OBSTACLE_LED);
  current_obstacle_status.left_status = level;
  return (level == NO_OBSTACLE);
}

static bool back_path_is_clear(uint16_t back_sensor_cm) {
  obstacle_threshold_level_t level = check_sensor_value_and_glow_obstacle_led(back_sensor_cm, BACK_OBSTACLE_LED);
  current_obstacle_status.back_status = level;
  return (level == NO_OBSTACLE);
}

static bool check_if_path_is_clear_of_obstacles(dbc_ULTRASONIC_TO_DRIVER_s sensor_val) {
  bool path_clear_status = true;

  uint16_t front_sensor_cm = sensor_val.ULTRASONIC_TO_DRIVER_front;
  uint16_t left_sensor_cm = sensor_val.ULTRASONIC_TO_DRIVER_left;
  uint16_t right_sensor_cm = sensor_val.ULTRASONIC_TO_DRIVER_right;
  uint16_t back_sensor_cm = sensor_val.ULTRASONIC_TO_DRIVER_back;

  path_clear_status &= front_path_is_clear(front_sensor_cm);
  path_clear_status &= right_path_is_clear(right_sensor_cm);
  path_clear_status &= left_path_is_clear(left_sensor_cm);
  back_path_is_clear(back_sensor_cm); // just to update current_obstacle_status

  closest_obstacle_threshold_ahead =
      MIN_OF(current_obstacle_status.front_status,
             MIN_OF(current_obstacle_status.left_status, current_obstacle_status.right_status));
  return path_clear_status;
}

static bool is_front_completely_blocked() {
  if (current_obstacle_status.front_status <= OBSTACLE_THRESHOLD_LEVEL_1 &&
      current_obstacle_status.left_status <= OBSTACLE_THRESHOLD_LEVEL_1 &&
      current_obstacle_status.right_status <= OBSTACLE_THRESHOLD_LEVEL_1) {
    return true;
  } else {
    return false;
  }
}

// Function to determine the speed level based on the obstacle threshold
static int get_speed_level_based_on_obstacle_distance(void) {
  speed_throttle_level_t speed_level;
  switch (closest_obstacle_threshold_ahead) {
  case OBSTACLE_THRESHOLD_LEVEL_0:
    speed_level = THROTTLE_LEVEL_1;
    break;
  case OBSTACLE_THRESHOLD_LEVEL_1:
    speed_level = THROTTLE_LEVEL_1;
    break;
  case OBSTACLE_THRESHOLD_LEVEL_2:
    speed_level = THROTTLE_LEVEL_2;
    break;
  case OBSTACLE_THRESHOLD_LEVEL_3:
    speed_level = THROTTLE_LEVEL_3;
    break;
  case NO_OBSTACLE:
    speed_level = THROTTLE_LEVEL_4;
    break;
  default:
    speed_level = THROTTLE_LEVEL_0;
    break;
  }
  return (int)speed_level;
}

// Function to determine the steer angle based on the direction of the obstacles
// if closest obstacle is NO_OBSTACLE or OBSTACLE_THRESHOLD_LEVEL_3 => straight
// if closest obstacle is OBSTACLE_THRESHOLD_LEVEL_2 => soft. direction = max(front, left, right)
// if closest obstacle is OBSTACLE_THRESHOLD_LEVEL_1 => medium. direction = max(front, left, right)
// if closest obstacle is OBSTACLE_THRESHOLD_LEVEL_0 => sharp. direction = max(front, left, right)
static int get_steer_angle_based_on_obstacle_closeness() {
  int angle = 0;
  obstacle_threshold_level_t f = current_obstacle_status.front_status;
  obstacle_threshold_level_t r = current_obstacle_status.right_status;
  obstacle_threshold_level_t l = current_obstacle_status.left_status;

  switch (closest_obstacle_threshold_ahead) {
  case NO_OBSTACLE:
  case OBSTACLE_THRESHOLD_LEVEL_3:
    angle = STEER_STRAIGHT;
    break;
  case OBSTACLE_THRESHOLD_LEVEL_2:
    if (r >= f && r >= l) {
      angle = STEER_SOFT_RIGHT;
    } else if (l >= f && l >= r) {
      angle = STEER_SOFT_LEFT;
    } else {
      angle = STEER_STRAIGHT;
    }
    break;
  case OBSTACLE_THRESHOLD_LEVEL_1:
    if (r >= f && r >= l) {
      angle = STEER_MEDIUM_RIGHT;
    } else if (l >= f && l >= r) {
      angle = STEER_MEDIUM_LEFT;
    } else {
      angle = STEER_STRAIGHT;
    }
    break;
  case OBSTACLE_THRESHOLD_LEVEL_0:
    if (r >= f && r >= l) {
      angle = STEER_SHARP_RIGHT;
    } else if (l >= f && l >= r) {
      angle = STEER_SHARP_LEFT;
    } else {
      angle = STEER_STRAIGHT;
    }
    break;
  }
  return angle;
}

static void avoid_obstacles(dbc_DRIVER_TO_MOTOR_s *motor_val) {
  if (is_front_completely_blocked()) {
    if (current_obstacle_status.back_status > OBSTACLE_THRESHOLD_LEVEL_2) {
      motor_val->DRIVER_TO_MOTOR_speed = REVERSE_SPEED;
      motor_val->DRIVER_TO_MOTOR_steer = STEER_MEDIUM_RIGHT;
    } else {
      motor_val->DRIVER_TO_MOTOR_speed = THROTTLE_LEVEL_0;
      motor_val->DRIVER_TO_MOTOR_steer = STEER_STRAIGHT;
    }
  } else {
    motor_val->DRIVER_TO_MOTOR_speed = get_speed_level_based_on_obstacle_distance();
    motor_val->DRIVER_TO_MOTOR_steer = get_steer_angle_based_on_obstacle_closeness();
  }
}

// Function to determine the speed level based on the distance to the destination
int get_speed_level_based_on_distance_to_destination(int destination_distance_meters) {
  speed_throttle_level_t speed_level = THROTTLE_LEVEL_0;
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
  steer_angle_level_t steer = STEER_STRAIGHT;
  int angle_difference = dest - curr;

  if (angle_difference > 180) {
    angle_difference -= 360;
  } else if (angle_difference < -180) {
    angle_difference += 360;
  }

  if (angle_difference > 60) {
    return STEER_SHARP_RIGHT;
  } else if (angle_difference > 45) {
    return STEER_MEDIUM_RIGHT;
  } else if (angle_difference > 30) {
    return STEER_SOFT_RIGHT;
  } else if (angle_difference < -60) {
    return STEER_SHARP_LEFT;
  } else if (angle_difference < -45) {
    return STEER_MEDIUM_LEFT;
  } else if (angle_difference < -30) {
    return STEER_SOFT_LEFT;
  } else {
    return 0;
  }
  return (int)steer;
}

// Function to drive towards the destination when there are no obstacles
// The drive speed is based on the distance to the destination.
// Farther the destination, more is the speed
static void drive_towards_destination(dbc_DRIVER_TO_MOTOR_s *motor_val, dbc_GEO_STATUS_s geo_heading) {
  int dest_angle = geo_heading.GEO_STATUS_COMPASS_HEADING;
  int curr_angle = geo_heading.GEO_STATUS_COMPASS_BEARING;
  int dist_to_dest = geo_heading.GEO_STATUS_DISTANCE_TO_DESTINATION;

  // TODO: uncomment this actual logic when the mobile app is ready
  // motor_val->DRIVER_TO_MOTOR_steer = get_steer_angle_based_on_destination_direction(dest_angle, curr_angle);
  // motor_val->DRIVER_TO_MOTOR_speed = get_speed_level_based_on_distance_to_destination(dist_to_dest);

  // TODO: this is only for obstacle detection testing. delete this when the mobile app is ready
  motor_val->DRIVER_TO_MOTOR_steer = STEER_STRAIGHT;
  motor_val->DRIVER_TO_MOTOR_speed = THROTTLE_LEVEL_4;
}

// Function to check and avoid obstacles and go towards destination
void steer_processor(dbc_DRIVER_TO_MOTOR_s *motor_val, dbc_ULTRASONIC_TO_DRIVER_s sensor_val,
                     dbc_GEO_STATUS_s geo_heading) {
  bool is_path_clear = check_if_path_is_clear_of_obstacles(sensor_val);
  if (is_path_clear) {
    drive_towards_destination(motor_val, geo_heading);
  } else {
    avoid_obstacles(motor_val);
  }
}

void steer_processor__obstacle_LEDs_init() {
  FRONT_OBSTACLE_LED = gpio__construct_as_output(GPIO__PORT_2, 2);
  LEFT_OBSTACLE_LED = gpio__construct_as_output(GPIO__PORT_2, 0);
  RIGHT_OBSTACLE_LED = gpio__construct_as_output(GPIO__PORT_2, 1);
  BACK_OBSTACLE_LED = gpio__construct_as_output(GPIO__PORT_2, 4);
}

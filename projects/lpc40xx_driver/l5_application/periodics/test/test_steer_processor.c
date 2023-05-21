#include "Mockboard_io.h"
#include "Mockgpio.h"
#include "Mocksteering.h"
#include "project.h"
#include "steer_processor.c"
#include "unity.h"

void setUp() {
  current_obstacle_status.back_status = 0;
  current_obstacle_status.front_status = 0;
  current_obstacle_status.left_status = 0;
  current_obstacle_status.right_status = 0;
  closest_obstacle_threshold_ahead = NO_OBSTACLE;
}

void ignore_obstacle_leds() {
  gpio_s gpio;
  gpio__set_Ignore();
  gpio__reset_Ignore();
  board_io__get_led1_IgnoreAndReturn(gpio);
  board_io__get_led2_IgnoreAndReturn(gpio);
  board_io__get_led3_IgnoreAndReturn(gpio);
}

void test_get_obstacle_threshold() {
  TEST_ASSERT_EQUAL(OBSTACLE_THRESHOLD_LEVEL_0, get_obstacle_threshold(30));
  TEST_ASSERT_EQUAL(OBSTACLE_THRESHOLD_LEVEL_1, get_obstacle_threshold(50));
  TEST_ASSERT_EQUAL(OBSTACLE_THRESHOLD_LEVEL_1, get_obstacle_threshold(70));
  TEST_ASSERT_EQUAL(OBSTACLE_THRESHOLD_LEVEL_2, get_obstacle_threshold(75));
  TEST_ASSERT_EQUAL(OBSTACLE_THRESHOLD_LEVEL_2, get_obstacle_threshold(90));
  TEST_ASSERT_EQUAL(OBSTACLE_THRESHOLD_LEVEL_3, get_obstacle_threshold(100));
  TEST_ASSERT_EQUAL(OBSTACLE_THRESHOLD_LEVEL_3, get_obstacle_threshold(130));
  TEST_ASSERT_EQUAL(NO_OBSTACLE, get_obstacle_threshold(150));
  TEST_ASSERT_EQUAL(NO_OBSTACLE, get_obstacle_threshold(160));
}

void test_front_path_is_clear() {
  ignore_obstacle_leds();
  TEST_ASSERT_TRUE(front_path_is_clear(NO_OBSTACLE + 10));
  TEST_ASSERT_EQUAL(NO_OBSTACLE, current_obstacle_status.front_status);
  TEST_ASSERT_FALSE(front_path_is_clear(OBSTACLE_THRESHOLD_LEVEL_3 + 10));
  TEST_ASSERT_EQUAL(OBSTACLE_THRESHOLD_LEVEL_3, current_obstacle_status.front_status);
  TEST_ASSERT_FALSE(front_path_is_clear(OBSTACLE_THRESHOLD_LEVEL_3 - 10));
  TEST_ASSERT_EQUAL(OBSTACLE_THRESHOLD_LEVEL_2, current_obstacle_status.front_status);
}

void test_right_path_is_clear() {
  ignore_obstacle_leds();
  TEST_ASSERT_TRUE(right_path_is_clear(NO_OBSTACLE + 10));
  TEST_ASSERT_EQUAL(NO_OBSTACLE, current_obstacle_status.right_status);
  TEST_ASSERT_FALSE(right_path_is_clear(OBSTACLE_THRESHOLD_LEVEL_2 + 10));
  TEST_ASSERT_EQUAL(OBSTACLE_THRESHOLD_LEVEL_2, current_obstacle_status.right_status);
  TEST_ASSERT_FALSE(right_path_is_clear(OBSTACLE_THRESHOLD_LEVEL_2 - 10));
  TEST_ASSERT_EQUAL(OBSTACLE_THRESHOLD_LEVEL_1, current_obstacle_status.right_status);
}

void test_left_path_is_clear() {
  ignore_obstacle_leds();
  TEST_ASSERT_TRUE(left_path_is_clear(NO_OBSTACLE + 10));
  TEST_ASSERT_EQUAL(NO_OBSTACLE, current_obstacle_status.left_status);
  TEST_ASSERT_FALSE(left_path_is_clear(OBSTACLE_THRESHOLD_LEVEL_1 + 10));
  TEST_ASSERT_EQUAL(OBSTACLE_THRESHOLD_LEVEL_1, current_obstacle_status.left_status);
  TEST_ASSERT_FALSE(left_path_is_clear(OBSTACLE_THRESHOLD_LEVEL_1 - 10));
  TEST_ASSERT_EQUAL(OBSTACLE_THRESHOLD_LEVEL_0, current_obstacle_status.left_status);
}

void test_back_path_is_clear() {
  ignore_obstacle_leds();
  TEST_ASSERT_TRUE(back_path_is_clear(NO_OBSTACLE + 10));
  TEST_ASSERT_EQUAL(NO_OBSTACLE, current_obstacle_status.back_status);
  TEST_ASSERT_FALSE(back_path_is_clear(OBSTACLE_THRESHOLD_LEVEL_1 + 10));
  TEST_ASSERT_EQUAL(OBSTACLE_THRESHOLD_LEVEL_1, current_obstacle_status.back_status);
  TEST_ASSERT_FALSE(back_path_is_clear(OBSTACLE_THRESHOLD_LEVEL_1 - 10));
  TEST_ASSERT_EQUAL(OBSTACLE_THRESHOLD_LEVEL_0, current_obstacle_status.back_status);
}

void test_check_if_path_is_clear_of_obstacles() {
  ignore_obstacle_leds();
  dbc_ULTRASONIC_TO_DRIVER_s s;
  s.ULTRASONIC_TO_DRIVER_front = 150;
  s.ULTRASONIC_TO_DRIVER_left = 150;
  s.ULTRASONIC_TO_DRIVER_right = 150;
  s.ULTRASONIC_TO_DRIVER_back = 150;
  TEST_ASSERT_TRUE(check_if_path_is_clear_of_obstacles(s));
  TEST_ASSERT_EQUAL(NO_OBSTACLE, closest_obstacle_threshold_ahead);

  s.ULTRASONIC_TO_DRIVER_back = 10;
  TEST_ASSERT_TRUE(check_if_path_is_clear_of_obstacles(s));
  TEST_ASSERT_EQUAL(NO_OBSTACLE, closest_obstacle_threshold_ahead);

  s.ULTRASONIC_TO_DRIVER_right = 10;
  TEST_ASSERT_FALSE(check_if_path_is_clear_of_obstacles(s));
  TEST_ASSERT_EQUAL(OBSTACLE_THRESHOLD_LEVEL_0, closest_obstacle_threshold_ahead);

  s.ULTRASONIC_TO_DRIVER_left = 60;
  TEST_ASSERT_FALSE(check_if_path_is_clear_of_obstacles(s));
  TEST_ASSERT_EQUAL(OBSTACLE_THRESHOLD_LEVEL_0, closest_obstacle_threshold_ahead);

  s.ULTRASONIC_TO_DRIVER_right = 100;
  TEST_ASSERT_FALSE(check_if_path_is_clear_of_obstacles(s));
  TEST_ASSERT_EQUAL(OBSTACLE_THRESHOLD_LEVEL_1, closest_obstacle_threshold_ahead);
}

void test_get_speed_level_based_on_obstacle_distance() {
  closest_obstacle_threshold_ahead = NO_OBSTACLE;
  TEST_ASSERT_EQUAL(SPEED_MULTIPLIER_FACTOR * THROTTLE_LEVEL_4, get_speed_level_based_on_obstacle_distance());

  closest_obstacle_threshold_ahead = OBSTACLE_THRESHOLD_LEVEL_3;
  TEST_ASSERT_EQUAL(SPEED_MULTIPLIER_FACTOR * THROTTLE_LEVEL_3, get_speed_level_based_on_obstacle_distance());

  closest_obstacle_threshold_ahead = OBSTACLE_THRESHOLD_LEVEL_2;
  TEST_ASSERT_EQUAL(SPEED_MULTIPLIER_FACTOR * THROTTLE_LEVEL_2, get_speed_level_based_on_obstacle_distance());

  closest_obstacle_threshold_ahead = OBSTACLE_THRESHOLD_LEVEL_1;
  TEST_ASSERT_EQUAL(SPEED_MULTIPLIER_FACTOR * THROTTLE_LEVEL_1, get_speed_level_based_on_obstacle_distance());

  closest_obstacle_threshold_ahead = OBSTACLE_THRESHOLD_LEVEL_0;
  TEST_ASSERT_EQUAL(SPEED_MULTIPLIER_FACTOR * THROTTLE_LEVEL_1 * 0.5, get_speed_level_based_on_obstacle_distance());

  dbc_ULTRASONIC_TO_DRIVER_s sensor_val;
  ignore_obstacle_leds();
  sensor_val.ULTRASONIC_TO_DRIVER_front = 30;
  sensor_val.ULTRASONIC_TO_DRIVER_left = 30;
  sensor_val.ULTRASONIC_TO_DRIVER_right = 30;
  check_if_path_is_clear_of_obstacles(sensor_val);
  TEST_ASSERT_EQUAL(SPEED_MULTIPLIER_FACTOR * THROTTLE_LEVEL_1 * 0.5, get_speed_level_based_on_obstacle_distance());

  sensor_val.ULTRASONIC_TO_DRIVER_front = 30;
  sensor_val.ULTRASONIC_TO_DRIVER_left = 100;
  sensor_val.ULTRASONIC_TO_DRIVER_right = 200;
  check_if_path_is_clear_of_obstacles(sensor_val);
  TEST_ASSERT_EQUAL(SPEED_MULTIPLIER_FACTOR * THROTTLE_LEVEL_1 * 0.5, get_speed_level_based_on_obstacle_distance());

  sensor_val.ULTRASONIC_TO_DRIVER_front = 80;
  sensor_val.ULTRASONIC_TO_DRIVER_left = 100;
  sensor_val.ULTRASONIC_TO_DRIVER_right = 200;
  check_if_path_is_clear_of_obstacles(sensor_val);
  TEST_ASSERT_EQUAL(SPEED_MULTIPLIER_FACTOR * THROTTLE_LEVEL_2, get_speed_level_based_on_obstacle_distance());
}

void test_is_front_completely_blocked() {
  current_obstacle_status.front_status = 30;
  current_obstacle_status.left_status = 30;
  current_obstacle_status.right_status = 30;
  TEST_ASSERT_TRUE(is_front_completely_blocked());

  current_obstacle_status.front_status = 130;
  current_obstacle_status.left_status = 130;
  current_obstacle_status.right_status = 130;
  TEST_ASSERT_FALSE(is_front_completely_blocked());
}

void test_get_steer_angle_based_on_obstacle_closeness() {
  dbc_ULTRASONIC_TO_DRIVER_s sensor_val;
  ignore_obstacle_leds();

  sensor_val.ULTRASONIC_TO_DRIVER_front = 150;
  sensor_val.ULTRASONIC_TO_DRIVER_left = 150;
  sensor_val.ULTRASONIC_TO_DRIVER_right = 150;
  TEST_ASSERT_TRUE(check_if_path_is_clear_of_obstacles(sensor_val));
  TEST_ASSERT_EQUAL(STEER_STRAIGHT, get_steer_angle_based_on_obstacle_closeness());

  sensor_val.ULTRASONIC_TO_DRIVER_front = 150;
  sensor_val.ULTRASONIC_TO_DRIVER_left = 100;
  sensor_val.ULTRASONIC_TO_DRIVER_right = 100;
  TEST_ASSERT_FALSE(check_if_path_is_clear_of_obstacles(sensor_val));
  TEST_ASSERT_EQUAL(STEER_STRAIGHT, get_steer_angle_based_on_obstacle_closeness());

  sensor_val.ULTRASONIC_TO_DRIVER_front = 150;
  sensor_val.ULTRASONIC_TO_DRIVER_left = 100;
  sensor_val.ULTRASONIC_TO_DRIVER_right = 100;
  TEST_ASSERT_FALSE(check_if_path_is_clear_of_obstacles(sensor_val));
  TEST_ASSERT_EQUAL(STEER_STRAIGHT, get_steer_angle_based_on_obstacle_closeness());

  sensor_val.ULTRASONIC_TO_DRIVER_front = 40;
  sensor_val.ULTRASONIC_TO_DRIVER_left = 50;
  sensor_val.ULTRASONIC_TO_DRIVER_right = 60;
  TEST_ASSERT_FALSE(check_if_path_is_clear_of_obstacles(sensor_val));
  TEST_ASSERT_EQUAL(STEER_SHARP_RIGHT, get_steer_angle_based_on_obstacle_closeness());
}
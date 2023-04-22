#include "Mockboard_io.h"
#include "Mockgpio.h"
#include "Mocksteering.h"
#include "project.h"
#include "steer_processor.c"
#include "unity.h"

static int steer_threshold = 50;

void ignore_obstacle_leds() {
  gpio_s gpio;
  gpio__set_Ignore();
  gpio__reset_Ignore();
  board_io__get_led1_IgnoreAndReturn(gpio);
  board_io__get_led2_IgnoreAndReturn(gpio);
  board_io__get_led3_IgnoreAndReturn(gpio);
}

void test_front_path_is_clear() {
  ignore_obstacle_leds();
  TEST_ASSERT_TRUE(front_path_is_clear(steer_threshold + 10));
  TEST_ASSERT_FALSE(front_path_is_clear(steer_threshold));
  TEST_ASSERT_FALSE(front_path_is_clear(steer_threshold - 10));
}

void test_right_path_is_clear() {
  ignore_obstacle_leds();
  TEST_ASSERT_TRUE(right_path_is_clear(steer_threshold + 10));
  TEST_ASSERT_FALSE(right_path_is_clear(steer_threshold));
  TEST_ASSERT_FALSE(right_path_is_clear(steer_threshold - 10));
}

void test_left_path_is_clear() {
  ignore_obstacle_leds();
  TEST_ASSERT_TRUE(left_path_is_clear(steer_threshold + 10));
  TEST_ASSERT_FALSE(left_path_is_clear(steer_threshold));
  TEST_ASSERT_FALSE(left_path_is_clear(steer_threshold - 10));
}

void test_steer_processor_drive_straight() {}
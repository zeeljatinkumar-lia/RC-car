#include "Mocksteering.h"
#include "project.h"
#include "unity.h"

#include "steer_processor.c"

void test_front_path_is_clear() {
  TEST_ASSERT_TRUE(front_path_is_clear(100));
  TEST_ASSERT_FALSE(front_path_is_clear(50));
  TEST_ASSERT_FALSE(front_path_is_clear(0));
}

void test_right_path_is_clear() {
  TEST_ASSERT_TRUE(right_path_is_clear(100));
  TEST_ASSERT_FALSE(right_path_is_clear(50));
  TEST_ASSERT_FALSE(right_path_is_clear(0));
}

void test_left_path_is_clear() {
  TEST_ASSERT_TRUE(left_path_is_clear(100));
  TEST_ASSERT_FALSE(left_path_is_clear(50));
  TEST_ASSERT_FALSE(left_path_is_clear(0));
}

void test_steer_processor_drive_straight() {}
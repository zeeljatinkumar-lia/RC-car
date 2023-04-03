#include "unity.h"

#include "Mockdelay.h"
#include "Mockgpio.h"
#include "Mockpwm1.h"

#include "motor.c"

void test_motor__convert_angle_to_pwm() {
  TEST_ASSERT_EQUAL(10.0f, motor__convert_angle_to_pwm(-60));
  TEST_ASSERT_EQUAL(15.0f, motor__convert_angle_to_pwm(0));
  TEST_ASSERT_EQUAL(20.0f, motor__convert_angle_to_pwm(60));
  TEST_ASSERT_EQUAL(17.5f, motor__convert_angle_to_pwm(30));
  TEST_ASSERT_EQUAL(12.5f, motor__convert_angle_to_pwm(-30));
}

void test_motor__convert_speed_to_pwm() {
  TEST_ASSERT_EQUAL(10.0f, motor__convert_speed_to_pwm(50, true));
  TEST_ASSERT_EQUAL(15.0f, motor__convert_speed_to_pwm(0, true));
  TEST_ASSERT_EQUAL(15.0f, motor__convert_speed_to_pwm(0, false));
  TEST_ASSERT_EQUAL(20.0f, motor__convert_speed_to_pwm(50, false));
  TEST_ASSERT_EQUAL(17.5f, motor__convert_speed_to_pwm(25, false));
  TEST_ASSERT_EQUAL(12.5f, motor__convert_speed_to_pwm(25, true));
}
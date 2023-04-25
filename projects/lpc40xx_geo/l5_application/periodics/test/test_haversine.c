#include "Mockgps.h"
#include "math.h"
#include "unity.h"

#include "haversine.h"

void setUp() {}

void tearDown() {}

void test_calculate_distance() {
  TEST_ASSERT_EQUAL(111, calculate_distance(37, -121, 38, -121));
  TEST_ASSERT_EQUAL(111, calculate_distance(37, -121, 36, -121));
  TEST_ASSERT_EQUAL(88, calculate_distance(37, -121, 37, -120));
  TEST_ASSERT_EQUAL(88, calculate_distance(37, -121, 37, -122));
}

void test_calculate_heading() {
  TEST_ASSERT_EQUAL(0, calculate_heading(37, -121, 38, -121));
  TEST_ASSERT_EQUAL(180, calculate_heading(37, -121, 36, -121));
  TEST_ASSERT_EQUAL(89, calculate_heading(37, -121, 37, -120));
  TEST_ASSERT_EQUAL(270, calculate_heading(37, -121, 37, -122));
}

void test_distance_and_heading() {
  gps_coordinates_t curr, dest;
  curr.latitude = 37;
  curr.longitude = -121;

  dest.latitude = 38;
  dest.longitude = -122;
  TEST_ASSERT_EQUAL(141.9, calculate_distance(curr.latitude, curr.longitude, dest.latitude, dest.longitude));
  TEST_ASSERT_EQUAL(321.5, calculate_heading(curr.latitude, curr.longitude, dest.latitude, dest.longitude));

  dest.latitude = 36;
  dest.longitude = -120;
  TEST_ASSERT_EQUAL(142.7, calculate_distance(curr.latitude, curr.longitude, dest.latitude, dest.longitude));
  TEST_ASSERT_EQUAL(140.5, calculate_heading(curr.latitude, curr.longitude, dest.latitude, dest.longitude));
}
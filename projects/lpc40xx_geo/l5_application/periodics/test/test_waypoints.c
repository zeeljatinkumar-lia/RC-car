#include "Mockgps.h"
#include "haversine.h"
#include "unity.h"

#include "waypoints.c"

void test_waypoints__calculate_distance_between_two_points() {
  gps_coordinates_t x = {.latitude = 37, .longitude = -121};
  gps_coordinates_t y = {0};

  y.latitude = 38;
  y.longitude = -121;
  TEST_ASSERT_EQUAL(111, waypoints__calculate_distance_between_two_points(x, y) / 1000);

  y.latitude = 36;
  y.longitude = -121;
  TEST_ASSERT_EQUAL(111, waypoints__calculate_distance_between_two_points(x, y) / 1000);

  y.latitude = 37;
  y.longitude = -120;
  TEST_ASSERT_EQUAL(88, waypoints__calculate_distance_between_two_points(x, y) / 1000);

  y.latitude = 37;
  y.longitude = -122;
  TEST_ASSERT_EQUAL(88, waypoints__calculate_distance_between_two_points(x, y) / 1000);
}

void test_waypoints__get_next_coordinate() {
  gps_coordinates_t curr = {0};
  gps_coordinates_t dest = {0};
  gps_coordinates_t next = {0};

  // choosing dest as sightly away from the last waypoint
  // choosing curr as slightly close to each of the waypoint
  // and verying that that waypoint comes out to be the next coordinate
  dest.latitude = waypoints_array[MAX_WAYPOINTS - 1].latitude + 0.0001;   // 37.3400;
  dest.longitude = waypoints_array[MAX_WAYPOINTS - 1].longitude - 0.0002; //-121.8820;
  for (int i = 0; i < MAX_WAYPOINTS; i++) {
    curr.latitude = waypoints_array[i].latitude - 0.00005;
    curr.longitude = waypoints_array[i].longitude + 0.0001;
    next = waypoints__get_next_coordinate(curr, dest);
    printf("way%d.lat = %f, way%d.long = %f\n", i, waypoints_array[i].latitude, i, waypoints_array[i].longitude);
    printf("curr.lat = %f, curr.long = %f\n", curr.latitude, curr.longitude);
    printf("dest.lat = %f, dest.long = %f\n", dest.latitude, dest.longitude);
    printf("%d. next.lat = %f, next.long = %f\n\n", i, next.latitude, next.longitude);
    TEST_ASSERT_EQUAL(next.latitude, waypoints_array[i].latitude);
    TEST_ASSERT_EQUAL(next.longitude, waypoints_array[i].longitude);
  }

  // choosing curr between dest and the last waypoint, verifying that dest is the next coordinate
  dest.latitude = waypoints_array[MAX_WAYPOINTS - 1].latitude + 0.0002;   // 37.3401;
  dest.longitude = waypoints_array[MAX_WAYPOINTS - 1].longitude + 0.0004; //-121.8822;
  dest.latitude = waypoints_array[MAX_WAYPOINTS - 1].latitude + 0.0001;   // 37.3400;
  dest.longitude = waypoints_array[MAX_WAYPOINTS - 1].longitude + 0.0002; //-121.8820;
  printf("dest.lat = %f, dest.long = %f\n", dest.latitude, dest.longitude);
  next = waypoints__get_next_coordinate(curr, dest);
  TEST_ASSERT_EQUAL(next.latitude, dest.latitude);
  TEST_ASSERT_EQUAL(next.longitude, dest.longitude);
}
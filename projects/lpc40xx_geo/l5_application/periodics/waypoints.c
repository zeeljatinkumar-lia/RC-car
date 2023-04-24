#include <stdint.h>

#include "gps.h"
#include "haversine.h"
#include "waypoints.h"

#define MAX_WAYPOINTS 10
// taken an approx point of 37.3390, -121.8800 and increased latitide values by 0.0001 and longitude values by 0.0002
// need to add actual coordinates by going to the test site
static gps_coordinates_t waypoints_array[MAX_WAYPOINTS] = {
    {37.3390, -121.8800}, {37.3391, -121.8802}, {37.3392, -121.8804}, {37.3393, -121.8806}, {37.3394, -121.8808},
    {37.3395, -121.8810}, {37.3396, -121.8812}, {37.3397, -121.8814}, {37.3398, -121.8816}, {37.3399, -121.8818}};

static int waypoints__calculate_distance_between_two_points(gps_coordinates_t x, gps_coordinates_t y) {
  int dist = calculate_distance_in_meters(x.latitude, x.longitude, y.latitude, y.longitude);
  return dist;
}

static gps_coordinates_t waypoints__get_next_coordinate(gps_coordinates_t current_coords,
                                                        gps_coordinates_t dest_coords) {
  int dist_between_curr_and_dest = 0;
  int dist_between_curr_and_waypoint = 0;
  int dist_between_waypoint_and_dest = 0;
  int closest_distance_so_far = 0;
  gps_coordinates_t next_point_coordinates = {0};

  dist_between_curr_and_dest = waypoints__calculate_distance_between_two_points(current_coords, dest_coords);
  closest_distance_so_far = dist_between_curr_and_dest;

  for (int i = 0; i < MAX_WAYPOINTS; i++) {
    dist_between_curr_and_waypoint =
        waypoints__calculate_distance_between_two_points(current_coords, waypoints_array[i]);
    dist_between_waypoint_and_dest = waypoints__calculate_distance_between_two_points(waypoints_array[i], dest_coords);

    if (dist_between_curr_and_waypoint < 10) {
      // we are already within 10 meter radius of a waypoint. time to go to the next one.
      continue;
    } else {
      if (dist_between_curr_and_waypoint < dist_between_curr_and_dest) {
        // this waypoint is closer to us than the destination. but are we going in the right direction?
        if (dist_between_waypoint_and_dest < dist_between_curr_and_dest) {
          // yes we are going in the right direction. but is this the closest waypoint to us?
          if (dist_between_curr_and_waypoint < closest_distance_so_far) {
            // well, this one is the closest waypoint to us so far. but lets check all the points to be sure
            closest_distance_so_far = dist_between_curr_and_waypoint;
            next_point_coordinates = waypoints_array[i];
          }
        }
      }
    }
  }
  if (closest_distance_so_far == dist_between_curr_and_dest) {
    // we exhausted the entire array and didn't find a single waypoint that is closer to us than the destination
    next_point_coordinates = dest_coords;
  }
  return next_point_coordinates;
}

uint16_t waypoints__calculate_heading_to_next_point(gps_coordinates_t current_coords, gps_coordinates_t dest_coords) {
  uint16_t next_point_heading = 0;
  gps_coordinates_t next_coords = waypoints__get_next_coordinate(current_coords, dest_coords);
  next_point_heading = (uint16_t)calculate_heading(current_coords.latitude, current_coords.longitude,
                                                   next_coords.latitude, next_coords.longitude);
  return next_point_heading;
}

int waypoints__calculate_distance_to_dest(gps_coordinates_t current_coords, gps_coordinates_t dest_coords) {
  int dist = waypoints__calculate_distance_between_two_points(current_coords, dest_coords);
  return dist;
}
#pragma once

#include "gps.h"
#include <stdint.h>

uint16_t waypoints__calculate_heading_to_next_point(gps_coordinates_t current_coords, gps_coordinates_t dest_coords);
int waypoints__calculate_distance_to_dest(gps_coordinates_t current_coords, gps_coordinates_t dest_coords);
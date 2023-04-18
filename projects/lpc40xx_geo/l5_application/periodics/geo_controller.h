#pragma once
#include <stdbool.h>

void geo_controller__read_all_can_messages();
void geo_controller__read_current_coordinates();
void geo_controller__calculate_heading();
bool geo_controller__send_heading_to_driver_over_can();
bool geo_controller__send_current_coord_to_bridge_over_can();
void geo_controller__print_coord_and_heading_values();
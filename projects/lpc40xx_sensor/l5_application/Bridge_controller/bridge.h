#pragma once
#include "project.h"

void bridge__read_destination_coordinates_from_app();
dbc_GPS_DESTINATION_s bridge__get_destination_coordinates();
void bridge__send_destination_coordinates_over_can();
void print_dest_coords();
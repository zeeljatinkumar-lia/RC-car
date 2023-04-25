// gps.h
#pragma once
#include "stdbool.h"
// Note:
// South means negative latittude
// West means negative longitutde
typedef struct {
  float latitude;
  float longitude;
} gps_coordinates_t;

void gps__init(void);
void gps__get_gps_data_and_parse_coordinates(void);

gps_coordinates_t gps__get_coordinates(void);
bool gps__get_satellite_lock_status(void);
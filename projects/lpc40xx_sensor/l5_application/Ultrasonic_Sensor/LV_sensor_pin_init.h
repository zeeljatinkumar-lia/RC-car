#pragma once

#include "project.h"
#include <stdint.h>
typedef enum {
  // FRONT_ULTRA_SONIC = 0,
  BACK_ULTRA_SONIC = 1,
  LEFT_ULTRA_SONIC = 2,
  RIGHT_ULTRA_SONIC = 3,
} ultra_sonic_sensor_position;

void initialize_adc_for_ultra_sonic_sensors(void);
void initialize_pins_for_ultra_sonic_sensor_triggers(void);
void fill_left_ultra_sonic_distance_buffer(void);
void fill_right_ultra_sonic_distance_buffer(void);
void fill_back_ultra_sonic_distance_buffer(void);
uint16_t get_median_distance(ultra_sonic_sensor_position sensor_position);
uint16_t sort_sensor_buffer_data_and_get_median(ultra_sonic_sensor_position sensor_position);

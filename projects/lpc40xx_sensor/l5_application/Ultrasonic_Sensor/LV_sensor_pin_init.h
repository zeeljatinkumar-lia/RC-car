#pragma once

#include "project.h"
#include <stdint.h>
typedef enum {
  FRONT_ULTRA_SONIC = 0,
  BACK_ULTRA_SONIC = 1,
  LEFT_ULTRA_SONIC = 2,
  RIGHT_ULTRA_SONIC = 3,
} ultra_sonic_sensor_position;

void adc_init_for_LV_sensors(void);
void trigger_pins_for_LV_sensors(void);
void collect_left_LV_sensor_values_buffer(void);
void collect_right_LV_sensor_values_buffer(void);
void collect_back_LV_sensor_values_buffer(void);
void collect_front_LV_sensor_values_buffer(void);
uint16_t get_median_distance(ultra_sonic_sensor_position sensor_position);
uint16_t sort_sensor_buffer_data_and_get_median(ultra_sonic_sensor_position sensor_position);

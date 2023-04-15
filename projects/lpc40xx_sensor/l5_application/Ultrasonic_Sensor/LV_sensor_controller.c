#include "board_io.h"
#include "can_bus.h"
#include "gpio.h"
#include "project.h"
#include <stdio.h>
#include <string.h>

//#include "can_bus_initializer.h"
#include "LV_sensor_controller.h"

#include "LV_sensor_pin_init.h"
#include "SR04_sensor_pin_init.h"

static dbc_ULTRASONIC_TO_DRIVER_s ultra_sonic_data;

void Sensor_Controller_init(void) {
  initialize_adc_for_ultra_sonic_sensors();
  initialize_pins_for_ultra_sonic_sensor_triggers();
  ultrasonic__init_front_sensor();
}

void Sensor_Controller__10hz_handler(uint32_t callback_count) {
  // const uint32_t callback_count_modulo_val = 5;
  // const uint32_t callback_count_remainder = callback_count % callback_count_modulo_val;

  fill_left_ultra_sonic_distance_buffer();
  ultra_sonic_data.ULTRASONIC_TO_DRIVER_left = sort_sensor_buffer_data_and_get_median(LEFT_ULTRA_SONIC);

  fill_right_ultra_sonic_distance_buffer();
  ultra_sonic_data.ULTRASONIC_TO_DRIVER_right = sort_sensor_buffer_data_and_get_median(RIGHT_ULTRA_SONIC);

  ultrasonic__update_front_sensor();
  ultrasonic__get_distance_from_front_sensors(&ultra_sonic_data);

  printf("left-sensor value: %d\n", ultra_sonic_data.ULTRASONIC_TO_DRIVER_left);
  printf("right-sensor value: %d\n", ultra_sonic_data.ULTRASONIC_TO_DRIVER_right);
  printf("front-sensor value: %d\n", ultra_sonic_data.ULTRASONIC_TO_DRIVER_front);
}
dbc_ULTRASONIC_TO_DRIVER_s get_ultra_sonic_data(void) { return ultra_sonic_data; }

#include "board_io.h"
#include "can_bus.h"
#include "gpio.h"
#include "project.h"
#include <stdio.h>
#include <string.h>

#include "LV_sensor_controller.h"
#include "LV_sensor_pin_init.h"

static dbc_ULTRASONIC_TO_DRIVER_s ultra_sonic_data;

void Sensor_Controller_init(void) {
  adc_init_for_LV_sensors();
  trigger_pins_for_LV_sensors();
}

void Sensor_Controller__print_sensor_values() {
  printf("L:%d F:%d R:%d B:%d\n", ultra_sonic_data.ULTRASONIC_TO_DRIVER_left,
         ultra_sonic_data.ULTRASONIC_TO_DRIVER_front, ultra_sonic_data.ULTRASONIC_TO_DRIVER_right,
         ultra_sonic_data.ULTRASONIC_TO_DRIVER_back);
}

void Sensor_Controller__100hz_handler(uint32_t callback_count) {
  if (callback_count % 2 == 0) {
    collect_left_LV_sensor_values_buffer();
    ultra_sonic_data.ULTRASONIC_TO_DRIVER_left = sort_sensor_buffer_data_and_get_median(LEFT_ULTRA_SONIC);

    collect_right_LV_sensor_values_buffer();
    ultra_sonic_data.ULTRASONIC_TO_DRIVER_right = sort_sensor_buffer_data_and_get_median(RIGHT_ULTRA_SONIC);

  }

  else {

    collect_back_LV_sensor_values_buffer();
    ultra_sonic_data.ULTRASONIC_TO_DRIVER_back = sort_sensor_buffer_data_and_get_median(BACK_ULTRA_SONIC);

    collect_front_LV_sensor_values_buffer();
    ultra_sonic_data.ULTRASONIC_TO_DRIVER_front = sort_sensor_buffer_data_and_get_median(FRONT_ULTRA_SONIC);
  }
}
dbc_ULTRASONIC_TO_DRIVER_s get_ultra_sonic_data(void) { return ultra_sonic_data; }

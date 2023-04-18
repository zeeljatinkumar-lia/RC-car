#include "unity.h"

#include "LV_sensor_controller.h"

#include "MockLV_sensor_pin_init.h"
#include "MockSR04_sensor_pin_init.h"
#include "Mockcan_bus.h"

void test_Sensor_Controller_init(void) {
  initialize_adc_for_ultra_sonic_sensors_Expect();
  initialize_pins_for_ultra_sonic_sensor_triggers_Expect();
  ultrasonic__init_front_sensor_Expect();
  Sensor_Controller_init();
}

void test_Sensor_Controller__100hz_handler(void) {
  uint16_t front_sensor_median, back_sensor_median, left_sensor_median, right_sensor_median;
  dbc_ULTRASONIC_TO_DRIVER_s ultra_sonic_data;
  fill_left_ultra_sonic_distance_buffer_Expect();
  sort_sensor_buffer_data_and_get_median_ExpectAndReturn(LEFT_ULTRA_SONIC, left_sensor_median);

  fill_right_ultra_sonic_distance_buffer_Expect();
  sort_sensor_buffer_data_and_get_median_ExpectAndReturn(RIGHT_ULTRA_SONIC, right_sensor_median);

  fill_back_ultra_sonic_distance_buffer_Expect();
  sort_sensor_buffer_data_and_get_median_ExpectAndReturn(BACK_ULTRA_SONIC, right_sensor_median);

  ultrasonic__update_front_sensor_Expect();
  ultrasonic__get_distance_from_front_sensors_ExpectAnyArgs();
  Sensor_Controller__10hz_handler(0);
}
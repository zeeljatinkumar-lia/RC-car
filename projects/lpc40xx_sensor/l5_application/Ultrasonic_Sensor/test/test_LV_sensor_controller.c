#include "unity.h"

#include "LV_sensor_controller.h"

#include "MockLV_sensor_pin_init.h"
#include "Mockcan_bus.h"

void test_Sensor_Controller_init(void) {
  adc_init_for_LV_sensors_Expect();
  trigger_pins_for_LV_sensors_Expect();
  Sensor_Controller_init();
}

void test_Sensor_Controller__100hz_handler(void) {
  uint16_t sensor_median;
  collect_left_LV_sensor_values_buffer_Expect();
  sort_sensor_buffer_data_and_get_median_ExpectAndReturn(LEFT_ULTRA_SONIC, sensor_median);
  collect_right_LV_sensor_values_buffer_Expect();
  sort_sensor_buffer_data_and_get_median_ExpectAndReturn(RIGHT_ULTRA_SONIC, sensor_median);
  Sensor_Controller__100hz_handler(0);

  collect_back_LV_sensor_values_buffer_Expect();
  sort_sensor_buffer_data_and_get_median_ExpectAndReturn(BACK_ULTRA_SONIC, sensor_median);
  collect_front_LV_sensor_values_buffer_Expect();
  sort_sensor_buffer_data_and_get_median_ExpectAndReturn(FRONT_ULTRA_SONIC, sensor_median);
  Sensor_Controller__100hz_handler(1);
}
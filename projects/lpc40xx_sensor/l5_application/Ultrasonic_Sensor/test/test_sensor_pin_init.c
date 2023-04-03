#include "unity.h"

#include "Mockultrasonic_sensor.h"

#include "Mockgpio.h"

#include "sensor_pin_init.c"

void setUp() {}
void tearDown() {}

void test_ultrasonic__init_all_sensors(void) {
  ultrasonic__init_Expect(&sensor_left);
  ultrasonic__init_all_sensors();
}

void test_ultrasonic__update_all_sensors(void) {

  sensor_left.distance_from_obstacle = 45.31;
  ultrasonic__calculate_distance_from_obstacle_Expect(NULL);
  ultrasonic__calculate_distance_from_obstacle_IgnoreArg_u_sensor();
  ultrasonic__calculate_distance_from_obstacle_ReturnThruPtr_u_sensor(&sensor_left);

  ultrasonic__update_all_sensors();
  // TEST_ASSERT_EQUAL_FLOAT(sensor_middle.distance_from_obstacle, 12.27);
}

void test_ultrasonic__init_ports_and_pins_left() {
  ultrasonic__init_ports_and_pins_left();

  TEST_ASSERT_EQUAL_UINT8(sensor_left.echo_input.port_number, ULTRASONIC_LEFT_SENSOR_INPUT_PORT);
  TEST_ASSERT_EQUAL_UINT8(sensor_left.echo_input.pin_number, ULTRASONIC_LEFT_ECHO_INPUT_PIN);
  TEST_ASSERT_EQUAL_UINT8(sensor_left.trig_output.port_number, ULTRASONIC_LEFT_SENSOR_OUTPUT_PORT);
  TEST_ASSERT_EQUAL_UINT8(sensor_left.trig_output.pin_number, ULTRASONIC_LEFT_TRIG_OUTPUT_PIN);
}

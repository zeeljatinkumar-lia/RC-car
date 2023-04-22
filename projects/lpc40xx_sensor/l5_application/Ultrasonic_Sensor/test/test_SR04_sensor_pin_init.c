#include "unity.h"

#include "MockSR04_ultrasonic_sensor.h"

#include "Mockgpio.h"

#include "SR04_sensor_pin_init.c"

void setUp() {}
void tearDown() {}

void test_ultrasonic__init_front_sensors(void) {
  ultrasonic__init_Expect(&sensor_front);
  ultrasonic__init_front_sensor();
}

void test_ultrasonic__update_front_sensors(void) {

  sensor_front.distance_from_obstacle = 45.31;
  ultrasonic__calculate_distance_from_obstacle_Expect(NULL);
  ultrasonic__calculate_distance_from_obstacle_IgnoreArg_u_sensor();
  ultrasonic__calculate_distance_from_obstacle_ReturnThruPtr_u_sensor(&sensor_front);

  ultrasonic__update_front_sensor();
  // TEST_ASSERT_EQUAL_FLOAT(sensor_middle.distance_from_obstacle, 12.27);
}

void test_ultrasonic__init_ports_and_pins_front() {
  ultrasonic__init_ports_and_pins_front();

  TEST_ASSERT_EQUAL_UINT8(sensor_front.echo_input.port_number, ULTRASONIC_FRONT_SENSOR_INPUT_PORT);
  TEST_ASSERT_EQUAL_UINT8(sensor_front.echo_input.pin_number, ULTRASONIC_FRONT_ECHO_INPUT_PIN);
  TEST_ASSERT_EQUAL_UINT8(sensor_front.trig_output.port_number, ULTRASONIC_FRONT_SENSOR_OUTPUT_PORT);
  TEST_ASSERT_EQUAL_UINT8(sensor_front.trig_output.pin_number, ULTRASONIC_FRONT_TRIG_OUTPUT_PIN);
}

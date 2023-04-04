#include "sensor_pin_init.h"
#include "stdio.h"

#include "gpio.h"

/**

 * SENSOR_LEFT - Echo: 0.6, Trigger: 0.7
 **/

// PINS
static const uint8_t ULTRASONIC_LEFT_ECHO_INPUT_PIN = 6, ULTRASONIC_LEFT_TRIG_OUTPUT_PIN = 7;

// PORTS
static const gpio__port_e COMMON_PORT = GPIO__PORT_0;

static const gpio__port_e ULTRASONIC_LEFT_SENSOR_INPUT_PORT = COMMON_PORT,
                          ULTRASONIC_LEFT_SENSOR_OUTPUT_PORT = COMMON_PORT;

/**
 * STATIC FUNCTIONS
 */
static void ultrasonic__init_ports_and_pins(void);
static void ultrasonic__init_ports_and_pins_left(void);

/**
 * STATIC VARIABLES
 */
static ultrasonic_sensor_s sensor_left;

/**
 * NON-STATIC FUNCTION DEFINITIONS
 */
void ultrasonic__init_all_sensors(void) {
  ultrasonic__init_ports_and_pins();
  ultrasonic__init(&sensor_left);
}

void ultrasonic__update_all_sensors(void) { ultrasonic__calculate_distance_from_obstacle(&sensor_left); }

void ultrasonic__get_distance_from_all_sensors(ultrasonic_distance_s *output) {
  output->left = sensor_left.distance_from_obstacle;
  printf("sensor value: %lf\n", output->left);
}

static void ultrasonic__init_ports_and_pins(void) { ultrasonic__init_ports_and_pins_left(); }
static void ultrasonic__init_ports_and_pins_left() {
  sensor_left.echo_input.port_number = ULTRASONIC_LEFT_SENSOR_INPUT_PORT;
  sensor_left.echo_input.pin_number = ULTRASONIC_LEFT_ECHO_INPUT_PIN;
  sensor_left.trig_output.port_number = ULTRASONIC_LEFT_SENSOR_OUTPUT_PORT;
  sensor_left.trig_output.pin_number = ULTRASONIC_LEFT_TRIG_OUTPUT_PIN;
}

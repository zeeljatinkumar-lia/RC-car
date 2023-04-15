#include "SR04_sensor_pin_init.h"
#include "SR04_ultrasonic_sensor.h"
#include "stdio.h"

#include "gpio.h"

/**

 * SENSOR_FRONT - Echo: 0.6, Trigger: 0.7
 **/

// PINS
static const uint8_t ULTRASONIC_FRONT_ECHO_INPUT_PIN = 6, ULTRASONIC_FRONT_TRIG_OUTPUT_PIN = 7;

// PORTS
static const gpio__port_e COMMON_PORT = GPIO__PORT_0;

static const gpio__port_e ULTRASONIC_FRONT_SENSOR_INPUT_PORT = COMMON_PORT,
                          ULTRASONIC_FRONT_SENSOR_OUTPUT_PORT = COMMON_PORT;

/**
 * STATIC FUNCTIONS
 */
static void ultrasonic__init_ports_and_pins_front(void);
// static void ultrasonic__init_ports_and_pins_front(void);

/**
 * STATIC VARIABLES
 */
static ultrasonic_sensor_s sensor_front;
// static ultrasonic_distance_s sensor_data;

/**
 * NON-STATIC FUNCTION DEFINITIONS
 */
void ultrasonic__init_front_sensor(void) {
  ultrasonic__init_ports_and_pins_front();
  ultrasonic__init(&sensor_front);
}

void ultrasonic__update_front_sensor(void) { ultrasonic__calculate_distance_from_obstacle(&sensor_front); }

// void get_sensor_data(void) { ultrasonic__get_distance_from_all_sensors(&sensor_data); }

void ultrasonic__get_distance_from_front_sensors(dbc_ULTRASONIC_TO_DRIVER_s *output) {
  output->ULTRASONIC_TO_DRIVER_front = sensor_front.distance_from_obstacle;
}
// ultrasonic_distance_s *can_get_sensor_data(void) { return (&sensor_data); }
// static void ultrasonic__init_ports_and_pins(void) { ultrasonic__init_ports_and_pins_front(); }
static void ultrasonic__init_ports_and_pins_front() {
  sensor_front.echo_input.port_number = ULTRASONIC_FRONT_SENSOR_INPUT_PORT;
  sensor_front.echo_input.pin_number = ULTRASONIC_FRONT_ECHO_INPUT_PIN;
  sensor_front.trig_output.port_number = ULTRASONIC_FRONT_SENSOR_OUTPUT_PORT;
  sensor_front.trig_output.pin_number = ULTRASONIC_FRONT_TRIG_OUTPUT_PIN;
}

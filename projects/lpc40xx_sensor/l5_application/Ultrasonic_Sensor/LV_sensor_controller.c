#include "board_io.h"
#include "can_bus.h"
#include "gpio.h"
#include "project.h"
#include <stdio.h>
#include <string.h>

#include "LV_sensor_controller.h"
#include "LV_sensor_pin_init.h"

static dbc_ULTRASONIC_TO_DRIVER_s ultra_sonic_data;

static gpio_s FRONT_OBSTACLE_LED;
static gpio_s LEFT_OBSTACLE_LED;
static gpio_s RIGHT_OBSTACLE_LED;
static gpio_s BACK_OBSTACLE_LED;

void Sensor_Controller_init(void) {
  FRONT_OBSTACLE_LED = gpio__construct_as_output(GPIO__PORT_2, 2);
  LEFT_OBSTACLE_LED = gpio__construct_as_output(GPIO__PORT_2, 0);
  RIGHT_OBSTACLE_LED = gpio__construct_as_output(GPIO__PORT_2, 1);
  BACK_OBSTACLE_LED = gpio__construct_as_output(GPIO__PORT_2, 4);
  adc_init_for_LV_sensors();
  trigger_pins_for_LV_sensors();
}

void Sensor_Controller__print_sensor_values() {
  printf("L:%d F:%d R:%d B:%d\n", ultra_sonic_data.ULTRASONIC_TO_DRIVER_left,
         ultra_sonic_data.ULTRASONIC_TO_DRIVER_front, ultra_sonic_data.ULTRASONIC_TO_DRIVER_right,
         ultra_sonic_data.ULTRASONIC_TO_DRIVER_back);
}

static void update_obstacle_LED(int sensor_value, gpio_s obstacle_led) {
  if (sensor_value > 150) { // no obstacle detected. Turn OFF LED
    gpio__set(obstacle_led);
  } else {
    gpio__reset(obstacle_led); // obstacle detected. Turn ON LED
  }
}

void Sensor_Controller__100hz_handler(uint32_t callback_count) {
  if (callback_count % 2 == 0) {
    collect_left_LV_sensor_values_buffer();
    ultra_sonic_data.ULTRASONIC_TO_DRIVER_left = sort_sensor_buffer_data_and_get_median(LEFT_ULTRA_SONIC);
    update_obstacle_LED(ultra_sonic_data.ULTRASONIC_TO_DRIVER_left, LEFT_OBSTACLE_LED);

    collect_right_LV_sensor_values_buffer();
    ultra_sonic_data.ULTRASONIC_TO_DRIVER_right = sort_sensor_buffer_data_and_get_median(RIGHT_ULTRA_SONIC);
    update_obstacle_LED(ultra_sonic_data.ULTRASONIC_TO_DRIVER_right, RIGHT_OBSTACLE_LED);
  }

  else {

    collect_back_LV_sensor_values_buffer();
    ultra_sonic_data.ULTRASONIC_TO_DRIVER_back = sort_sensor_buffer_data_and_get_median(BACK_ULTRA_SONIC);
    update_obstacle_LED(ultra_sonic_data.ULTRASONIC_TO_DRIVER_back, BACK_OBSTACLE_LED);

    collect_front_LV_sensor_values_buffer();
    ultra_sonic_data.ULTRASONIC_TO_DRIVER_front = sort_sensor_buffer_data_and_get_median(FRONT_ULTRA_SONIC);
    update_obstacle_LED(ultra_sonic_data.ULTRASONIC_TO_DRIVER_front, FRONT_OBSTACLE_LED);
  }
}
dbc_ULTRASONIC_TO_DRIVER_s get_ultra_sonic_data(void) { return ultra_sonic_data; }

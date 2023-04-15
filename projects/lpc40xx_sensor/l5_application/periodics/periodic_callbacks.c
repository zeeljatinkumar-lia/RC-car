#include "periodic_callbacks.h"

#include "board_io.h"
#include "gpio.h"

#include "SR04_sensor_pin_init.h"

// newly added header
#include "LV_sensor_controller.h"
#include "LV_sensor_pin_init.h"
#include "bridge_controller.h"
#include "can_ultrasonic_sensor_handler.h"
#include "can_ultrasonic_sensor_initializer.h"

/******************************************************************************
 * Your board will reset if the periodic function does not return within its deadline
 * For 1Hz, the function must return within 1000ms
 * For 1000Hz, the function must return within 1ms
 */

void periodic_callbacks__initialize(void) {
  can_ultrasonic_init();
  Bridge_Controller_init();
  Sensor_Controller_init();
}

void periodic_callbacks__1Hz(uint32_t callback_count) { can_ultrasonic_reset(); }

void periodic_callbacks__10Hz(uint32_t callback_count) {
  // //TODO:zeel fetch all the sensor data and pass it on the can bus
  Bridge_Controller__10hz_handler();
  Sensor_Controller__10hz_handler(callback_count);
  can_ultrasonic_sensor_transmit_messages_10hz();
}

void periodic_callbacks__100Hz(uint32_t callback_count) {}

/**
 * @warning
 * This is a very fast 1ms task and care must be taken to use this
 * This may be disabled based on intialization of periodic_scheduler__initialize()
 */
void periodic_callbacks__1000Hz(uint32_t callback_count) {

  // Add your code here
}
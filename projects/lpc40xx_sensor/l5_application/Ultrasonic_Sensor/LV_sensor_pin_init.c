
#include "adc.h"
#include "board_io.h"
#include "clock.h"
#include "delay.h"
#include "gpio.h"
#include "sys_time.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "LV_sensor_pin_init.h"
#define BUFFER_SIZE 32
#define SCALING_FACTOR 0.333 // divide by 3 to get cm values, as per experimentation

static const uint8_t sensor_trigger_delay = 20;

static gpio_s front_ultra_sonic_trigger;
static gpio_s back_ultra_sonic_trigger;
static gpio_s left_ultra_sonic_trigger;
static gpio_s right_ultra_sonic_trigger;

static uint16_t front_sensor_distance_cm[BUFFER_SIZE];
static uint16_t left_sensor_distance_cm[BUFFER_SIZE];
static uint16_t right_sensor_distance_cm[BUFFER_SIZE];
static uint16_t back_sensor_distance_cm[BUFFER_SIZE];

void adc_init_for_LV_sensors(void) { adc__initialize(); }

void trigger_pins_for_LV_sensors(void) {
  front_ultra_sonic_trigger = gpio__construct_as_output(GPIO__PORT_0, 6);
  back_ultra_sonic_trigger = gpio__construct_as_output(GPIO__PORT_0, 7);
  right_ultra_sonic_trigger = gpio__construct_as_output(GPIO__PORT_0, 8);
  left_ultra_sonic_trigger = gpio__construct_as_output(GPIO__PORT_0, 9);

  gpio__set_function(front_ultra_sonic_trigger, GPIO__FUNCITON_0_IO_PIN);
  gpio__set_function(back_ultra_sonic_trigger, GPIO__FUNCITON_0_IO_PIN);
  gpio__set_function(right_ultra_sonic_trigger, GPIO__FUNCITON_0_IO_PIN);
  gpio__set_function(left_ultra_sonic_trigger, GPIO__FUNCITON_0_IO_PIN);

  gpio__set(front_ultra_sonic_trigger);
  gpio__set(back_ultra_sonic_trigger);
  gpio__set(right_ultra_sonic_trigger);
  gpio__set(left_ultra_sonic_trigger);
  delay__ms(500);
  gpio__reset(front_ultra_sonic_trigger);
  gpio__reset(back_ultra_sonic_trigger);
  gpio__reset(right_ultra_sonic_trigger);
  gpio__reset(left_ultra_sonic_trigger);
}

void collect_left_LV_sensor_values_buffer(void) {
  uint16_t left_sensor_raw_data;
  float distance_in_cm;
  size_t i;

  memset(left_sensor_distance_cm, 0, BUFFER_SIZE * sizeof(uint16_t));

  gpio__set(left_ultra_sonic_trigger);
  delay__us(sensor_trigger_delay);
  for (i = 0; i < BUFFER_SIZE; i++) {
    left_sensor_raw_data = adc__get_adc_value(ADC__CHANNEL_2);
    distance_in_cm = left_sensor_raw_data * SCALING_FACTOR;
    left_sensor_distance_cm[i] = ((uint32_t)distance_in_cm);
  }
  gpio__reset(left_ultra_sonic_trigger);
}

void collect_right_LV_sensor_values_buffer(void) {
  uint16_t right_sensor_raw_data;
  float distance_in_cm;
  size_t i;
  memset(right_sensor_distance_cm, 0, BUFFER_SIZE * sizeof(uint16_t));

  gpio__set(right_ultra_sonic_trigger);
  delay__us(sensor_trigger_delay);
  for (i = 0; i < BUFFER_SIZE; i++) {
    right_sensor_raw_data = adc__get_adc_value(ADC__CHANNEL_5);
    distance_in_cm = right_sensor_raw_data * SCALING_FACTOR;
    right_sensor_distance_cm[i] = ((uint32_t)distance_in_cm);
  }

  gpio__reset(right_ultra_sonic_trigger);
}

void collect_back_LV_sensor_values_buffer(void) {
  uint16_t back_sensor_raw_data;
  float distance_in_cm;
  size_t i;
  memset(back_sensor_distance_cm, 0, BUFFER_SIZE * sizeof(uint16_t));

  gpio__set(back_ultra_sonic_trigger);
  delay__us(sensor_trigger_delay);
  for (i = 0; i < BUFFER_SIZE; i++) {
    back_sensor_raw_data = adc__get_adc_value(ADC__CHANNEL_3);
    distance_in_cm = back_sensor_raw_data * SCALING_FACTOR;
    back_sensor_distance_cm[i] = ((uint32_t)distance_in_cm);
  }

  gpio__reset(back_ultra_sonic_trigger);
}
void collect_front_LV_sensor_values_buffer(void) {
  uint16_t front_sensor_raw_data;
  float distance_in_cm;
  size_t i;
  memset(front_sensor_distance_cm, 0, BUFFER_SIZE * sizeof(uint16_t));

  gpio__set(front_ultra_sonic_trigger);
  delay__us(sensor_trigger_delay);
  for (i = 0; i < BUFFER_SIZE; i++) {
    front_sensor_raw_data = adc__get_adc_value(ADC__CHANNEL_4);
    distance_in_cm = front_sensor_raw_data * SCALING_FACTOR;
    front_sensor_distance_cm[i] = ((uint32_t)distance_in_cm);
  }
  // printf("\n");
  gpio__reset(front_ultra_sonic_trigger);
}

static int compare(const void *a, const void *b) { return (*(uint16_t *)a - *(uint16_t *)b); }

uint16_t sort_sensor_buffer_data_and_get_median(ultra_sonic_sensor_position sensor_position) {
  size_t median_position = (BUFFER_SIZE) / 2;
  uint16_t distance_val_to_return = 0;

  switch (sensor_position) {
  case LEFT_ULTRA_SONIC:
    qsort(left_sensor_distance_cm, BUFFER_SIZE, sizeof(uint16_t), compare);
    distance_val_to_return = left_sensor_distance_cm[median_position];
    break;

  case RIGHT_ULTRA_SONIC:
    qsort(right_sensor_distance_cm, BUFFER_SIZE, sizeof(uint16_t), compare);
    distance_val_to_return = right_sensor_distance_cm[median_position];
    break;

  case BACK_ULTRA_SONIC:
    qsort(back_sensor_distance_cm, BUFFER_SIZE, sizeof(uint16_t), compare);
    distance_val_to_return = back_sensor_distance_cm[median_position];
    break;

  case FRONT_ULTRA_SONIC:
    qsort(front_sensor_distance_cm, BUFFER_SIZE, sizeof(uint16_t), compare);
    distance_val_to_return = front_sensor_distance_cm[median_position];
    break;
  }
  return distance_val_to_return;
}

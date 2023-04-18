#pragma once

//#include "can_dbc_send_msg.h"
#include "project.h"
#include <stdbool.h>
#include <stdint.h>
void Sensor_Controller_init(void);

bool can__Sensor_Controller__routine_send(dbc_ULTRASONIC_TO_DRIVER_s *sensor_send_data);
void Sensor_Controller__10hz_handler(uint32_t callback_count);
dbc_ULTRASONIC_TO_DRIVER_s get_ultra_sonic_data(void);
void Sensor_Controller__print_sensor_values();

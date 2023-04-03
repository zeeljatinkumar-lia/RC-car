#pragma once

#include "project.h"
#include "sensor_pin_init.h"
dbc_ULTRASONIC_TO_DRIVER_s sensor_struct;
ultrasonic_distance_s sensor_data;
void can_ultrasonic_sensor_handler__transmit_messages_10hz(void);
// void can_handler__manage_mia_10hz(void);

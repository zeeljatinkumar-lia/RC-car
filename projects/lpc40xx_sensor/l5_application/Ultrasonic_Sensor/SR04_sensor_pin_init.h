#pragma once

//#include "SR04_ultrasonic_sensor.h"
#include "project.h"

void ultrasonic__init_front_sensor(void);
void ultrasonic__update_front_sensor(void);

void ultrasonic__get_distance_from_front_sensors(dbc_ULTRASONIC_TO_DRIVER_s *output);

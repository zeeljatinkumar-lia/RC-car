#pragma once

#include "project.h"

void update_sensor_for_LCD(dbc_ULTRASONIC_TO_DRIVER_s *sensor_data);
void update_compass_for_LCD(dbc_GEO_STATUS_s *compass_data);
void update_motor_for_LCD(dbc_DRIVER_TO_MOTOR_s *motor_data_s);

void LCD_display_1Hz(void);
// void LCD_status__display_handle_1hz(void);

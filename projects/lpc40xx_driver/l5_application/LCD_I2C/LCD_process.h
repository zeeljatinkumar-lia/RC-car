#pragma once

#include "project.h"

void update_sensor_for_LCD(dbc_ULTRASONIC_TO_DRIVER_s *sensor_data);
void update_compass_for_LCD(dbc_GEO_STATUS_s *compass_data);
// void LCD_status__update_motor_debug(dbc_MOTOR_DATA_s *motor_data_s);

void LCD_display_1Hz(void);
// void LCD_status__display_handle_1hz(void);

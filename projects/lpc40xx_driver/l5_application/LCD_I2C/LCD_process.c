#include <stdio.h>

#include "LCD_init.h"
#include "LCD_process.h"
#include "string.h"

static dbc_ULTRASONIC_TO_DRIVER_s lcd_sensor = {0};
static dbc_GEO_STATUS_s lcd_compass = {0};
static dbc_MOTOR_TO_APP_DBG_s lcd_motor = {0};

void update_sensor_for_LCD(dbc_ULTRASONIC_TO_DRIVER_s *sensor_data) { lcd_sensor = *sensor_data; }

void update_compass_for_LCD(dbc_GEO_STATUS_s *compass_data) { lcd_compass = *compass_data; }

// void LCD_status__update_motor_debug(dbc_MOTOR_TO_APP_DBG_s *motor_data_s) { lcd_motor = *motor_data_s; }

void LCD_display_1Hz(void) {
  char lcd_buffer[20];
  send_LCD_command(0x01);
  // Clear Screen before update
  send_LCD_command(0x80);
  sprintf(lcd_buffer, "F: %i", lcd_sensor.ULTRASONIC_TO_DRIVER_front);
  send_LCD_string(lcd_buffer);
  printf("front: %s\n", lcd_buffer);

  send_LCD_command(0x8A);
  sprintf(lcd_buffer, "L: %i", lcd_sensor.ULTRASONIC_TO_DRIVER_left);
  send_LCD_string(lcd_buffer);

  send_LCD_command(0xC0);
  sprintf(lcd_buffer, "Rr: %i", lcd_sensor.ULTRASONIC_TO_DRIVER_back);
  send_LCD_string(lcd_buffer);

  send_LCD_command(0xCA);
  sprintf(lcd_buffer, "R: %i", lcd_sensor.ULTRASONIC_TO_DRIVER_right);
  send_LCD_string(lcd_buffer);

  send_LCD_command(0x94);
  sprintf(lcd_buffer, "CMP: %d", lcd_compass.GEO_STATUS_COMPASS_HEADING);
  send_LCD_string(lcd_buffer);

  send_LCD_command(0x9F);
  sprintf(lcd_buffer, "m: %f", lcd_compass.GEO_STATUS_DISTANCE_TO_DESTINATION);
  send_LCD_string(lcd_buffer);

  send_LCD_command(0xD4);
  sprintf(lcd_buffer, "BRG: %d", lcd_compass.GEO_STATUS_COMPASS_BEARING);
  send_LCD_string(lcd_buffer);
}

// void LCD_status__display_handle_1hz(void) { LCD_status__display_on_screen(); }

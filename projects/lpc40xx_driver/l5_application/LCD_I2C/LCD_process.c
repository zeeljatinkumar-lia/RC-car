#include <stdio.h>

#include "LCD_init.h"
#include "LCD_process.h"
#include "string.h"

#include "delay.h"

#define LCD_LINE1 0x80
#define LCD_LINE2 0xC0
#define LCD_LINE3 0x94
#define LCD_LINE4 0xD4

static dbc_ULTRASONIC_TO_DRIVER_s lcd_sensor = {0};
static dbc_GEO_STATUS_s lcd_compass = {0};
static dbc_DRIVER_TO_MOTOR_s lcd_motor = {0};

void update_sensor_for_LCD(dbc_ULTRASONIC_TO_DRIVER_s *sensor_data) { lcd_sensor = *sensor_data; }

void update_compass_for_LCD(dbc_GEO_STATUS_s *compass_data) { lcd_compass = *compass_data; }

void update_motor_for_LCD(dbc_DRIVER_TO_MOTOR_s *motor_data_s) { lcd_motor = *motor_data_s; }

void LCD_display_1Hz(void) {
  char lcd_buffer[30];
  send_LCD_command(0x01); // Clear Screen before update

  send_LCD_command(LCD_LINE1);
  sprintf(lcd_buffer, "Front: %d", lcd_sensor.ULTRASONIC_TO_DRIVER_front);
  send_LCD_string(lcd_buffer);
  printf("front: %s\n", lcd_buffer);

  send_LCD_command(LCD_LINE2);
  sprintf(lcd_buffer, "Back: %d", lcd_sensor.ULTRASONIC_TO_DRIVER_back);
  send_LCD_string(lcd_buffer);

  send_LCD_command(LCD_LINE3);

  sprintf(lcd_buffer, "Left: %d", lcd_sensor.ULTRASONIC_TO_DRIVER_left);
  send_LCD_string(lcd_buffer);

  send_LCD_command(LCD_LINE4);
  sprintf(lcd_buffer, "Right: %d", lcd_sensor.ULTRASONIC_TO_DRIVER_right);
  send_LCD_string(lcd_buffer);

  send_LCD_command(0x8A);
  sprintf(lcd_buffer, "Steer: %d", lcd_motor.DRIVER_TO_MOTOR_steer);
  send_LCD_string(lcd_buffer);

  send_LCD_command(0xCA);

  sprintf(lcd_buffer, "Speed: %d", lcd_motor.DRIVER_TO_MOTOR_speed);
  send_LCD_string(lcd_buffer);

  send_LCD_command(0x9F);

  sprintf(lcd_buffer, "CB: %d", lcd_compass.GEO_STATUS_COMPASS_BEARING);
  send_LCD_string(lcd_buffer);

  send_LCD_command(0xDF);

  sprintf(lcd_buffer, "CH: %d", lcd_compass.GEO_STATUS_COMPASS_HEADING);
  send_LCD_string(lcd_buffer);
}

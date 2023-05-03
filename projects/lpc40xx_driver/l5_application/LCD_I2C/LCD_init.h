#pragma once

#include <stdint.h>

void init__LCD(void);

void reset__LCD(void);

void send_LCD_char(uint8_t character);

void send_LCD_string(char *input_string);

void send_LCD_command(uint8_t command);

void set_LCD_to_4_bit_mode(void);

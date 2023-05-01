#include "LCD_init.h"
#include "delay.h"
#include "i2c.h"
#include "string.h"

#define LCD_ID 0x4E // New device ID(New LCD Poggers)
/*Command shortcuts*/
#define LINE1 0x80 // Starting address for screen ROW 0, Column 0
#define LINE2 0xC0 // Starting address for screen ROW 1, Column 0
#define LINE3 0x94 // Starting address for screen ROW 1, Column 0
#define LINE4 0xD4 // Starting address for screen ROW 1, Column 0

#define FOUR_BIT_MODE 0x20
#define EIGHT_BIT_MODE 0x30
#define DISPLAY_AND_CURSOR 0x0C

#define CLEAR_SCREEN 0x01 // Clear contents on screen and reset cursor to ROW0, Column 0.
#define AUTOINCREMENT_CURSOR 0x06

/*Define Control bit registers*/
#define RS 0x01 // D0 - Read(0)/Write(1) mode to internal DDRAM
#define RW 0x02 // D1 - Read Busy Flag
#define E 0x04  // D2 - Enable for pulsing
#define BL 0x08 // D3 - Backlight of LCD (1)ON (0) OFF

static const i2c_e LCD_I2C_bus = I2C__2;

void init__LCD(void) {
  /*Set function to 4-bit interfacing mode*/

  delay__ms(50);
  send_LCD_command(EIGHT_BIT_MODE);
  delay__ms(5);
  send_LCD_command(EIGHT_BIT_MODE);
  delay__ms(1);
  send_LCD_command(EIGHT_BIT_MODE);
  delay__ms(10);
  send_LCD_command(FOUR_BIT_MODE); // 4bit mode
  delay__ms(10);

  send_LCD_command(0x28); // Function set
  delay__ms(1);
  send_LCD_command(DISPLAY_AND_CURSOR);
  delay__ms(1);
  send_LCD_command(CLEAR_SCREEN);
  ; // clear
  delay__ms(1);
  delay__ms(1);
  send_LCD_command(AUTOINCREMENT_CURSOR);
  ; // Entry mode set

  // return i2c__detect(I2C__2,0x27);
}
/*
void LCD__reset(void) {
  uint8_t Reg = 0x00;

  i2c__write_single(LCD_I2C_bus, LCD_ID, Reg, 0x3C); // 0x30
  i2c__write_single(LCD_I2C_bus, LCD_ID, Reg, 0x38);

  i2c__write_single(LCD_I2C_bus, LCD_ID, Reg, 0x3C); // 0x30
  i2c__write_single(LCD_I2C_bus, LCD_ID, Reg, 0x38);

  i2c__write_single(LCD_I2C_bus, LCD_ID, Reg, 0x3C); // 0x30
  i2c__write_single(LCD_I2C_bus, LCD_ID, Reg, 0x38);

  i2c__write_single(LCD_I2C_bus, LCD_ID, Reg, 0x2C); // 0x20
  i2c__write_single(LCD_I2C_bus, LCD_ID, Reg, 0x28);

  delay__ms(10);
}
*/

void send_LCD_command(uint8_t command) {
  uint8_t byte, Reg = 0x00;
  /*Start with the upper nibble*/
  byte = (command & 0xF0);
  byte |= 0x08;
  byte |= 0x04;
  i2c__write_single(LCD_I2C_bus, LCD_ID, Reg, byte);
  byte &= ~0x04;
  byte |= 0x08;
  i2c__write_single(LCD_I2C_bus, LCD_ID, Reg, byte);
  /*Now do the lower nibble*/
  byte = (command & 0x0F) << 4;
  byte |= 0x08;
  byte |= 0x04;
  i2c__write_single(LCD_I2C_bus, LCD_ID, Reg, byte);
  byte &= ~0x04;
  byte |= 0x08;
  i2c__write_single(LCD_I2C_bus, LCD_ID, Reg, byte);
  // Delay to process commands
  delay__ms(3);
}

void send_LCD_char(uint8_t character) {
  uint8_t byte, Reg = 0x00;
  /*Start with the upper nibble*/
  byte = (character & 0xF0);
  byte |= 0x08;
  byte |= (0x04 | 0x01);
  i2c__write_single(LCD_I2C_bus, LCD_ID, Reg, byte);
  byte &= ~(0x04 | 0x01);
  byte |= 0x08;
  i2c__write_single(LCD_I2C_bus, LCD_ID, Reg, byte);
  delay__ms(3);
  /*Now do the lower nibble*/
  byte = (character & 0x0F) << 4;
  byte |= 0x08;
  byte |= (0x04 | 0x01);
  i2c__write_single(LCD_I2C_bus, LCD_ID, Reg, byte);
  byte &= ~(0x04 | 0x01);
  byte |= 0x08;
  i2c__write_single(LCD_I2C_bus, LCD_ID, Reg, byte);
  delay__ms(3);
}

void send_LCD_string(char *input_string) {
  uint8_t i = 0;
  while (i != strnlen(input_string, 16)) {
    send_LCD_char(input_string[i]);
    i++;
  }
}

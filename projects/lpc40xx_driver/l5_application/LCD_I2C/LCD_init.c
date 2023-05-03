#include "LCD_init.h"
#include "delay.h"
#include "i2c.h"
#include "string.h"

#define LCD_ID 0x4E // New device ID(New LCD Poggers)

static const i2c_e LCD_I2C_bus = I2C__2;

void set_LCD_to_4_bit_mode(void) {
  uint8_t Reg = 0x00;
  delay__ms(50);
  i2c__write_single(LCD_I2C_bus, LCD_ID, Reg, 0x3C); // 0x30
  i2c__write_single(LCD_I2C_bus, LCD_ID, Reg, 0x38);

  delay__ms(5);

  i2c__write_single(LCD_I2C_bus, LCD_ID, Reg, 0x3C); // 0x30
  i2c__write_single(LCD_I2C_bus, LCD_ID, Reg, 0x38);
  delay__ms(1);
  i2c__write_single(LCD_I2C_bus, LCD_ID, Reg, 0x3C); // 0x30
  i2c__write_single(LCD_I2C_bus, LCD_ID, Reg, 0x38);
  delay__ms(10);
  i2c__write_single(LCD_I2C_bus, LCD_ID, Reg, 0x2C); // 0x20
  i2c__write_single(LCD_I2C_bus, LCD_ID, Reg, 0x28);

  delay__ms(10);
}

void init__LCD(void) {
  /* delays in between commands to let commands process*/

  send_LCD_command(0x28); // Function set
  delay__ms(1);
  send_LCD_command(0x0C); // Display on
  delay__ms(1);
  send_LCD_command(0x01); // clear the screen
  delay__ms(1);
  send_LCD_command(0x06); // turn on auto-increment
  delay__ms(2);
  send_LCD_command(0x80); // start of LCD
  delay__ms(2);
}

void send_LCD_command(uint8_t command) {
  uint8_t byte, Reg = 0x00;
  /*Start with the upper nibble*/
  byte = (command & 0xF0);
  byte |= 0x08; // backlight of LCD
  byte |= 0x04; // Enable bit LCD
  i2c__write_single(LCD_I2C_bus, LCD_ID, Reg, byte);
  byte &= ~0x04;
  byte |= 0x08;
  i2c__write_single(LCD_I2C_bus, LCD_ID, Reg, byte);
  /*lower nibble*/
  byte = (command & 0x0F) << 4;
  byte |= 0x08;
  byte |= 0x04;
  i2c__write_single(LCD_I2C_bus, LCD_ID, Reg, byte);
  byte &= ~0x04;
  byte |= 0x08;
  i2c__write_single(LCD_I2C_bus, LCD_ID, Reg, byte);
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

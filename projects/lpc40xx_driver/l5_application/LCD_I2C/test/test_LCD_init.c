#include "unity.h"

#include "Mockdelay.h"
#include "Mockgpio.h"
#include "Mocki2c.h"

#include "LCD_init.h"

static const uint8_t lcd_id = 0x4E;

void test_set_LCD_to_4_bit_mode(void) {
  delay__ms_Expect(50);
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x3C, true);
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x38, true);
  delay__ms_Expect(5);
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x3C, true);
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x38, true);
  delay__ms_Expect(1);
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x3C, true);
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x38, true);
  delay__ms_Expect(10);
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x2C, true);
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x28, true);

  delay__ms_Expect(10);
  set_LCD_to_4_bit_mode();
}

void test_init__LCD(void) {

  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x2C, true);
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x28, true);
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x8C, true);
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x88, true);

  delay__ms_Expect(3);
  delay__ms_Expect(1);

  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x0C, true);
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x08, true);
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0xCC, true);
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0xC8, true);
  delay__ms_Expect(3);
  delay__ms_Expect(1);

  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x0C, true);
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x08, true);
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x1C, true);
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x18, true);
  delay__ms_Expect(3);
  delay__ms_Expect(1);

  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x0C, true);
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x08, true);
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x6C, true);
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x68, true);
  delay__ms_Expect(3);
  delay__ms_Expect(2);

  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x8C, true);
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x88, true);
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x0C, true);
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x08, true);
  delay__ms_Expect(3);
  delay__ms_Expect(2);
  init__LCD();
}

void test_send_LCD_command(void) {
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x8C, true);
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x88, true);
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x0C, true);
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x08, true);
  delay__ms_Expect(3);
  send_LCD_command(0x80);
}

void test_send_LCD_char(void) {
  uint8_t test_char = 'A';
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x4D, true);
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x48, true);
  delay__ms_Expect(3);

  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x1D, true);
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x18, true);
  delay__ms_Expect(3);

  send_LCD_char(test_char);
}

void test_send_LCD_string(void) {
  char test_char[] = "ABC";
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x4D, true);
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x48, true);
  delay__ms_Expect(3);

  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x1D, true);
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x18, true);
  delay__ms_Expect(3);

  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x4D, true);
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x48, true);
  delay__ms_Expect(3);

  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x2D, true);
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x28, true);
  delay__ms_Expect(3);

  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x4D, true);
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x48, true);
  delay__ms_Expect(3);

  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x3D, true);
  i2c__write_single_ExpectAndReturn(I2C__2, lcd_id, 0x00, 0x38, true);
  delay__ms_Expect(3);

  send_LCD_string(test_char);
}

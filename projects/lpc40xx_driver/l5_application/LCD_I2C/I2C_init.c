#include <stdio.h>

#include "I2C_init.h"
#include "clock.h"
#include "gpio.h"
#include "i2c.h"

static const uint8_t lcd_ID = 0x4E;
void I2C_init(void) {
  gpio__construct_with_function(GPIO__PORT_0, 10, GPIO__FUNCTION_2);
  gpio__construct_with_function(GPIO__PORT_0, 11, GPIO__FUNCTION_2);
  // i2c__initialize(I2C__2, 100000U, clock__get_peripheral_clock_hz(), 0, 0);

  if (i2c__detect(I2C__2, lcd_ID)) {
    printf("LCD Slave Addr(0x4E) detected!\n");
  }
}

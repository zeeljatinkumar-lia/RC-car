#include "unity.h"

#include "Mockgpio.h"
#include "Mocki2c.h"

#include "i2c_init.h"

void test_I2C_init(void) {
  gpio_s gpio = {};
  gpio__construct_with_function_ExpectAndReturn(GPIO__PORT_0, 10, GPIO__FUNCTION_2, gpio);
  gpio__construct_with_function_ExpectAndReturn(GPIO__PORT_0, 11, GPIO__FUNCTION_2, gpio);
  i2c__detect_ExpectAndReturn(I2C__2, 0x4E, true);
  I2C_init();
}

#include "unity.h"

// Mock

#include "Mockadc.h"
#include "Mockdelay.h"
#include "Mockgpio.h"

#include "LV_sensor_pin_init.c"

void test_adc_init_for_LV_sensors(void) {
  adc__initialize_Expect();
  adc_init_for_LV_sensors();
}

void test_trigger_pins_for_LV_sensors(void) {

  gpio_s gpio = {};
  gpio__construct_as_output_ExpectAndReturn(GPIO__PORT_0, 6, gpio);
  gpio__construct_as_output_ExpectAndReturn(GPIO__PORT_0, 7, gpio);
  gpio__construct_as_output_ExpectAndReturn(GPIO__PORT_0, 8, gpio);
  gpio__construct_as_output_ExpectAndReturn(GPIO__PORT_0, 9, gpio);

  gpio__set_function_Expect(front_ultra_sonic_trigger, GPIO__FUNCITON_0_IO_PIN);

  gpio__set_function_Expect(back_ultra_sonic_trigger, GPIO__FUNCITON_0_IO_PIN);

  gpio__set_function_Expect(right_ultra_sonic_trigger, GPIO__FUNCITON_0_IO_PIN);
  gpio__set_function_Expect(left_ultra_sonic_trigger, GPIO__FUNCITON_0_IO_PIN);

  gpio__set_Expect(front_ultra_sonic_trigger);
  gpio__set_Expect(back_ultra_sonic_trigger);
  gpio__set_Expect(right_ultra_sonic_trigger);
  gpio__set_Expect(left_ultra_sonic_trigger);
  delay__ms_Expect(500);
  gpio__reset_Expect(front_ultra_sonic_trigger);
  gpio__reset_Expect(back_ultra_sonic_trigger);
  gpio__reset_Expect(right_ultra_sonic_trigger);
  gpio__reset_Expect(left_ultra_sonic_trigger);

  trigger_pins_for_LV_sensors();
}

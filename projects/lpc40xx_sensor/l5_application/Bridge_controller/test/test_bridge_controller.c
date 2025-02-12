#include "unity.h"

// Mock functions
#include "Mockcan_bus.h"
#include "Mockclock.h"
#include "Mockgpio.h"
#include "Mockline_buffer.h"
#include "Mockqueue.h"
#include "Mockuart.h"

#include "bridge_controller.c"

static char line_buffer[100];
static line_buffer_s line;

void setUp(void) {
  gpio_s test_gpio;
  uint32_t test_clock;
  QueueHandle_t rxq_handle;
  QueueHandle_t txq_handle;
  line_buffer__init_Expect(&line, line_buffer, sizeof(line_buffer));
  gpio__construct_with_function_ExpectAndReturn(GPIO__PORT_4, 28, GPIO__FUNCTION_2, test_gpio);
  gpio__construct_with_function_ExpectAndReturn(GPIO__PORT_4, 29, GPIO__FUNCTION_2, test_gpio);
  clock__get_peripheral_clock_hz_ExpectAndReturn(test_clock);
  uart__init_Expect(bridge_uart, test_clock, 9600);
  xQueueCreate_ExpectAndReturn(60, sizeof(char), rxq_handle);
  xQueueCreate_ExpectAndReturn(60, sizeof(char), txq_handle);
  uart__enable_queues_ExpectAndReturn(bridge_uart, rxq_handle, txq_handle, true);
  Bridge_Controller_init();
}

void tearDown(void) {}

// void test__bridge_controller(void) {}

void test__Bridge_Controller__transfer_data_from_uart_driver_to_line_buffer(void) {
  char byte;
  const uint32_t zero_timeout = 0;
  uart__get_ExpectAndReturn(bridge_uart, &byte, zero_timeout, true);
  line_buffer__add_byte_ExpectAndReturn(&line, byte, true);
  uart__get_ExpectAndReturn(bridge_uart, &byte, zero_timeout, false);
  Bridge_Controller__transfer_data_from_uart_driver_to_line_buffer();
}

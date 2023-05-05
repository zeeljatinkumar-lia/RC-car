// @file test_gps.c
#include "unity.h"

// Mocks
#include "Mockclock.h"
#include "Mockgpio.h"
#include "Mockqueue.h"
#include "Mockuart.h"
#include "string.h"

// We can choose to use real implementation (not Mock) for line_buffer.h
// because this is a relatively trivial module
#include "app_queue.h"
#include "line_buffer.h"

// Include the source we wish to test
#include "gps.c"

void setUp(void) {}
void tearDown(void) {}

static void set_init_expectations() {
  clock__get_peripheral_clock_hz_ExpectAndReturn(0);
  uart__init_Expect(gps_uart, 0, 9600);
  gpio_s gpio;
  gpio__construct_with_function_ExpectAndReturn(GPIO__PORT_4, 28, GPIO__FUNCTION_2, gpio);
  gpio__construct_with_function_ExpectAndReturn(GPIO__PORT_4, 29, GPIO__FUNCTION_2, gpio);

  QueueHandle_t rxq_handle, txq_handle;
  xQueueCreate_ExpectAndReturn(100, sizeof(char), rxq_handle);
  xQueueCreate_ExpectAndReturn(100, sizeof(char), txq_handle);
  uart__enable_queues_ExpectAndReturn(gps_uart, rxq_handle, txq_handle, true);
  uart__polled_put_IgnoreAndReturn(true);
  gps__init();
}

void test_init(void) { set_init_expectations(); }
void test_gps__coordinate_parser() {
  char str[] = "$GPGGA,230612.015,115.15,S,12102.4634,W,0,04,5.7,508.3,M,,,,0000*13\r\n";
  gps__coordinate_parser(str);
  gps_coordinates_t coord = gps__get_coordinates();
  TEST_ASSERT_EQUAL_FLOAT(-1.1515, coord.latitude);
  TEST_ASSERT_EQUAL_FLOAT(-121.024634, coord.longitude);
}

void test_gps__transfer_data_from_uart_driver_to_line_buffer() {
  char str[] = "abc";
  int i = 0;
  uart__get_ExpectAndReturn(UART__3, NULL, 0, true);
  uart__get_IgnoreArg_input_byte();
  uart__get_ReturnThruPtr_input_byte(&str[i++]);

  uart__get_ExpectAndReturn(UART__3, NULL, 0, true);
  uart__get_IgnoreArg_input_byte();
  uart__get_ReturnThruPtr_input_byte(&str[i++]);

  uart__get_ExpectAndReturn(UART__3, NULL, 0, true);
  uart__get_IgnoreArg_input_byte();
  uart__get_ReturnThruPtr_input_byte(&str[i++]);

  uart__get_ExpectAndReturn(UART__3, NULL, 0, false);
  uart__get_IgnoreArg_input_byte();
  gps__transfer_data_from_uart_driver_to_line_buffer();
}

static void send_a_test_line_over_uart(char *uart_driver_returned_data) {
  for (size_t index = 0; index <= strlen(uart_driver_returned_data); index++) {
    char *the_char_to_return = &uart_driver_returned_data[index];

    bool last_char = (index < strlen(uart_driver_returned_data));
    uart__get_ExpectAndReturn(UART__3, NULL, 0, last_char);
    uart__get_IgnoreArg_input_byte();
    uart__get_ReturnThruPtr_input_byte(the_char_to_return);
  }
}

void test_GPGGA_coordinates_are_parsed(void) {
  set_init_expectations();
  char uart_driver_returned_data[] = "$GPGGA,230612.015,215.15,S,12102.4634,W,0,04,5.7,508.3,M,,,,0000*13\r\n";
  send_a_test_line_over_uart(uart_driver_returned_data);
  gps__get_gps_data_and_parse_coordinates();
  gps_coordinates_t coord = gps__get_coordinates();
  TEST_ASSERT_EQUAL_FLOAT(-2.1515, coord.latitude);
  TEST_ASSERT_EQUAL_FLOAT(-121.024634, coord.longitude);
}

void test_GPGLL_line_is_ignored(void) {
  set_init_expectations();
  char uart_driver_returned_data[] = "$GPLL,230612.015,1.15,S,12102.4634,W,0,04,5.7,508.3,M,,,,0000*13\r\n";
  send_a_test_line_over_uart(uart_driver_returned_data);
  gps__get_gps_data_and_parse_coordinates();
  gps_coordinates_t coord = gps__get_coordinates();
  TEST_ASSERT_EQUAL_FLOAT(0, coord.latitude);
  TEST_ASSERT_EQUAL_FLOAT(0, coord.longitude);
}

void test_GPGGA_coordinates_North_and_South_parsed(void) {
  set_init_expectations();
  char uart_driver_returned_data_N[] = "$GPGGA,230612.015,123.456,N,12102.4634,W,0,04,5.7,508.3,M,,,,0000*13\r\n";
  send_a_test_line_over_uart(uart_driver_returned_data_N);
  gps__get_gps_data_and_parse_coordinates();
  gps_coordinates_t coord = gps__get_coordinates();
  TEST_ASSERT_EQUAL_FLOAT(1.23456, coord.latitude);

  char uart_driver_returned_data_S[] = "$GPGGA,230612.015,123.456,S,12102.4634,W,0,04,5.7,508.3,M,,,,0000*13\r\n";
  send_a_test_line_over_uart(uart_driver_returned_data_S);
  gps__get_gps_data_and_parse_coordinates();
  coord = gps__get_coordinates();
  TEST_ASSERT_EQUAL_FLOAT(-1.23456, coord.latitude);
}

void test_GPGGA_coordinates_East_and_West_parsed(void) {
  set_init_expectations();

  char uart_driver_returned_data_E[] = "$GPGGA,230612.015,123.4,N,567.8,E,0,04,5.7,508.3,M,,,,0000*13\r\n";
  send_a_test_line_over_uart(uart_driver_returned_data_E);
  gps__get_gps_data_and_parse_coordinates();
  gps_coordinates_t coord = gps__get_coordinates();
  TEST_ASSERT_EQUAL_FLOAT(5.678, coord.longitude);

  char uart_driver_returned_data_W[] = "$GPGGA,230612.015,123.4,S,567.8,W,0,04,5.7,508.3,M,,,,0000*13\r\n";
  send_a_test_line_over_uart(uart_driver_returned_data_W);
  gps__get_gps_data_and_parse_coordinates();
  coord = gps__get_coordinates();
  TEST_ASSERT_EQUAL_FLOAT(-5.678, coord.longitude);
}

void test_GPGGA_incomplete_line(void) {
  set_init_expectations();

  // set coords as 0 first
  char uart_driver_returned_data_0[] = "$GPGGA,230612.015,0,N,0,E,0,04,5.7,508.3,M,,,,0000*13\r\n";
  send_a_test_line_over_uart(uart_driver_returned_data_0);
  gps__get_gps_data_and_parse_coordinates();

  // test incomplete line
  char uart_driver_returned_data_1[] = "$GPGGA,230612.015,123.4,N,567.8,E,";
  send_a_test_line_over_uart(uart_driver_returned_data_1);
  gps__get_gps_data_and_parse_coordinates();
  gps_coordinates_t coord = gps__get_coordinates();
  TEST_ASSERT_EQUAL_FLOAT(0, coord.latitude);
  TEST_ASSERT_EQUAL_FLOAT(0, coord.longitude);

  char uart_driver_returned_data_2[] = "0,04,5.7,508.3,M,,,,0000*13\r\n";
  send_a_test_line_over_uart(uart_driver_returned_data_2);
  gps__get_gps_data_and_parse_coordinates();
  coord = gps__get_coordinates();
  TEST_ASSERT_EQUAL_FLOAT(1.234, coord.latitude);
  TEST_ASSERT_EQUAL_FLOAT(5.678, coord.longitude);
}
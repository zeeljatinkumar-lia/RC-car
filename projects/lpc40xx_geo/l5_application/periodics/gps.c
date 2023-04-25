// gps.c

#include "gps.h"

// GPS module dependency
#include "line_buffer.h"
#include "string.h"
#include "uart.h"

#include <board_io.h>
#include <gpio.h>
#include <stdio.h>

#include "clock.h" // needed for UART initialization

#define SENTENCE_IDENTIFIER "$GPGGA"

const uart_e gps_uart = UART__3;

// Space for the line buffer, and the line buffer data structure instance
static char line_buffer[200];
static line_buffer_s line;

static gps_coordinates_t parsed_coordinates;
static bool satellite_lock_status;

static void gps__transfer_data_from_uart_driver_to_line_buffer(void) {
  char byte = 0;
  const uint32_t zero_timeout = 0;
  while (uart__get(gps_uart, &byte, zero_timeout)) {
    line_buffer__add_byte(&line, byte);
  }
}

/* The format of the GPS coordinates string of type "Global Positioning System Fix Data" is as follows:
"$GPGGA,hhmmss.ss,llll.ll,a,yyyyy.yy,a,x,xx,x.x,x.x,M,x.x,M,x.x,xxxx*hh\r\n"
Each word is separated by a comma and has a special meaning. Of these we are currently interested only in:
0: $GPGGA : Ignore the line if it does not start with this identifier
1: hhmmss.ss : Time
2: llll.ll : Latitude
3: a: Can be either N or S. South means negative latittude
4: yyyy.yy : Longitude
5: a : Can be either E or W. West means negative longitutde
We will ignore all the other fields.
*/
static void gps__coordinate_parser(char *str) {
  char *word;
  float latitude = 1, longitude = 1;
  int i = 0, lock = 0;
  word = strtok(str, ",");
  if (word && strcmp(word, "$GPGGA") == 0) {
    parsed_coordinates.latitude = latitude;

    while (word != NULL) {
      i++;
      word = strtok(NULL, ",");
      switch (i) {
      case 2:
        sscanf(word, "%f", &latitude);
        break;
      case 3:
        if (word[0] == 'S' || word[0] == 's') {
          latitude = -1 * latitude;
        }
        break;
      case 4:
        sscanf(word, "%f", &longitude);
        break;
      case 5:
        if (word[0] == 'W' || word[0] == 'w') {
          longitude = -1 * longitude;
        }
        break;
      case 6:
        sscanf(word, "%d", &lock);
        if (lock == 1 || lock == 2) {
          satellite_lock_status = true;
        } else {
          satellite_lock_status = false;
        }
        break;
      default:
        break;
      }
    }
    parsed_coordinates.latitude = latitude / 100;
    parsed_coordinates.longitude = longitude / 100;
    printf("lat %f, long %f\n", parsed_coordinates.latitude, parsed_coordinates.longitude);
  }
}

static void gps__parse_coordinates_from_line(void) {
  char gps_line[200] = {0};

  if (line_buffer__remove_line(&line, gps_line, sizeof(gps_line))) {
    // printf("gps_line=%s", gps_line);
    gps__coordinate_parser(gps_line);
  }
}

void gps__init(void) {
  memset(line_buffer, 0, sizeof(line_buffer));
  parsed_coordinates.latitude = 0;
  parsed_coordinates.longitude = 0;
  line_buffer__init(&line, line_buffer, sizeof(line_buffer));
  uart__init(gps_uart, clock__get_peripheral_clock_hz(), 9600);
  gpio__construct_with_function(GPIO__PORT_4, 28, GPIO__FUNCTION_2); // P4.28 - Uart-3 Tx
  gpio__construct_with_function(GPIO__PORT_4, 29, GPIO__FUNCTION_2); // P4.29 - Uart-3 Rx

  // RX queue should be sized such that can buffer data in UART driver until gps__run_once() is called
  // Note: Assuming 38400bps, we can get 4 chars per ms, and 40 chars per 10ms (100Hz)
  QueueHandle_t rxq_handle = xQueueCreate(100, sizeof(char));
  QueueHandle_t txq_handle = xQueueCreate(100, sizeof(char)); // We are using tx queue for transmitting in fake_gps
  uart__enable_queues(gps_uart, rxq_handle, txq_handle);
}

void gps__get_gps_data_and_parse_coordinates(void) {
  gps__transfer_data_from_uart_driver_to_line_buffer();
  gps__parse_coordinates_from_line();
}

gps_coordinates_t gps__get_coordinates(void) { return parsed_coordinates; }

bool gps__get_satellite_lock_status(void) { return satellite_lock_status; }

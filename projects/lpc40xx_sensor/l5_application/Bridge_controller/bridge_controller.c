#include "bridge_controller.h"
#include "FreeRTOS.h"
#include "clock.h"

#include "board_io.h"
#include "can_bus.h"

#include "gpio.h"
#include "line_buffer.h"
#include "project.h"
#include "queue.h"

#include "sl_string.h"
#include "uart.h"
#include <gpio.h>
#include <stdio.h>

static dbc_GPS_DESTINATION_s gps_destination_location;
static dbc_GPS_DESTINATION_s gps_destination_location_last_sent;

static char line_buffer[100];
static line_buffer_s line;
static bool gps_dest_data_latched = false;
static uart_e bridge_uart = UART__3;

/* STATIC FUNCTION*/
static void Bridge_Controller__transfer_data_from_uart_driver_to_line_buffer(void);
static void Bridge_Controller__get_line_from_buffer(void);
/* NON-STATIC FUNCTION*/

void bridge_controller_handler__parse_gps_data(void);

void Bridge_Controller_init(void) {
  gps_destination_location.GPS_DEST_LONGITUDE_SCALED_100000 = 0.0;
  gps_destination_location.GPS_DEST_LATITUDE_SCALED_100000 = 0.0;
  gps_destination_location_last_sent.GPS_DEST_LONGITUDE_SCALED_100000 = 0.0;
  gps_destination_location_last_sent.GPS_DEST_LATITUDE_SCALED_100000 = 0.0;

  /*uart setup*/
  line_buffer__init(&line, line_buffer, sizeof(line_buffer));
  gpio__construct_with_function(GPIO__PORT_4, 28, GPIO__FUNCTION_2);
  gpio__construct_with_function(GPIO__PORT_4, 29, GPIO__FUNCTION_2);
  uart__init(bridge_uart, clock__get_peripheral_clock_hz(), 9600);
  QueueHandle_t rxq_handle = xQueueCreate(60, sizeof(char));
  QueueHandle_t txq_handle = xQueueCreate(60, sizeof(char));
  uart__enable_queues(bridge_uart, rxq_handle, txq_handle);
}

void Bridge_Controller__10hz_handler(void) {

  Bridge_Controller__transfer_data_from_uart_driver_to_line_buffer();
  Bridge_Controller__get_line_from_buffer();
  bridge_controller_handler__parse_gps_data();
}

void Bridge_Controller__transfer_data_from_uart_driver_to_line_buffer(void) {
  char byte;
  const uint32_t zero_timeout = 0;

  while (uart__get(bridge_uart, &byte, zero_timeout)) {

    line_buffer__add_byte(&line, byte);
  }
}

static void Bridge_Controller__get_line_from_buffer(void) {

  line_buffer__remove_line(&line, &line_buffer, sizeof(line_buffer));
}

void can_bridge_controller__Sending_dest_location(void) {
  can__msg_t can_msg = {0};
  dbc_message_header_t header = {
      .message_id = 0x0,
      .message_dlc = 0,
  };
  dbc_GPS_DESTINATION_s BRIDGE_TO_GPS_DATA = {};
  dbc_GPS_DESTINATION_s BRIDGE_DEFAULT_DATA = {};

  if (gps_dest_data_latched) {
    BRIDGE_TO_GPS_DATA.GPS_DEST_LATITUDE_SCALED_100000 =
        gps_destination_location_last_sent.GPS_DEST_LATITUDE_SCALED_100000;
    BRIDGE_TO_GPS_DATA.GPS_DEST_LONGITUDE_SCALED_100000 =
        gps_destination_location_last_sent.GPS_DEST_LONGITUDE_SCALED_100000;

    header = dbc_encode_GPS_DESTINATION(can_msg.data.bytes, &gps_destination_location_last_sent);

    can_msg.msg_id = header.message_id;
    can_msg.frame_fields.data_len = header.message_dlc;
    can__tx(can1, &can_msg, 0);
  } else {
    BRIDGE_DEFAULT_DATA.GPS_DEST_LATITUDE_SCALED_100000 = 0.0;
    BRIDGE_DEFAULT_DATA.GPS_DEST_LONGITUDE_SCALED_100000 = 0.0;

    header = dbc_encode_GPS_DESTINATION(can_msg.data.bytes, &BRIDGE_DEFAULT_DATA);

    can_msg.msg_id = header.message_id;
    can_msg.frame_fields.data_len = header.message_dlc;
    can__tx(can1, &can_msg, 0);
  }
}

void bridge_controller_handler__parse_gps_data(void) {
  static char temp_line_buffer[] = "GPS17.291291589287276,-32.724082209169865#";
  sscanf(temp_line_buffer, "GPS%ld,%ld#", &gps_destination_location.GPS_DEST_LATITUDE_SCALED_100000,
         &gps_destination_location.GPS_DEST_LONGITUDE_SCALED_100000);

  // sscanf(line_buffer, "GPS%f,%f#", &gps_destination_location.GPS_DEST_LATITUDE_SCALED_100000,
  //  &gps_destination_location.GPS_DEST_LONGITUDE_SCALED_100000);

  if (gps_destination_location.GPS_DEST_LATITUDE_SCALED_100000 != 0 &&
      gps_destination_location.GPS_DEST_LONGITUDE_SCALED_100000 != 0 && !gps_dest_data_latched) {

    printf("Latitude is %ld and longitude is %ld \n", gps_destination_location.GPS_DEST_LATITUDE_SCALED_100000,
           gps_destination_location.GPS_DEST_LONGITUDE_SCALED_100000);
    // printf("latched \n");
    gps_destination_location_last_sent.GPS_DEST_LONGITUDE_SCALED_100000 =
        gps_destination_location.GPS_DEST_LONGITUDE_SCALED_100000;
    gps_destination_location_last_sent.GPS_DEST_LATITUDE_SCALED_100000 =
        gps_destination_location.GPS_DEST_LATITUDE_SCALED_100000;
    gps_dest_data_latched = true;

    for (int i = 0; i < sizeof(line_buffer); i++) {
      line_buffer[i] = '\0';
    }

    line_buffer__init(&line, line_buffer, sizeof(line_buffer));
  }
}
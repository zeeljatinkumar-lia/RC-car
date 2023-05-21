#include "bridge_controller.h"
#include "FreeRTOS.h"
#include "clock.h"

#include "can_bus.h"

#include "gpio.h"
#include "line_buffer.h"
#include "project.h"
#include "queue.h"

#include "sl_string.h"
#include "uart.h"
#include "uart_printf.h"
#include <gpio.h>
#include <stdio.h>

#include "LV_sensor_controller.h"

static gpio_s MIA_LED;

static dbc_GPS_DESTINATION_s gps_destination_location;
static dbc_GPS_DESTINATION_s gps_destination_location_last_sent;
static dbc_GEO_STATUS_s compass_value_to_app;
static dbc_GEO_CURRENT_COORDS_s geo_coordinates_to_app;
static dbc_MOTOR_TO_APP_DBG_s motor_speed_to_app;
dbc_DRIVE_STATUS_CMD_s bridge_app_commands;

static char line_buffer[100];
static line_buffer_s line;
static bool gps_dest_data_latched = false;
static uart_e bridge_uart = UART__3;
static char app_command[10] = {0};

const uint32_t dbc_mia_threshold_GEO_STATUS = 1500;
const dbc_GEO_STATUS_s dbc_mia_replacement_GEO_STATUS;
const uint32_t dbc_mia_threshold_GEO_CURRENT_COORDS = 1500;
const dbc_GEO_CURRENT_COORDS_s dbc_mia_replacement_GEO_CURRENT_COORDS;
const uint32_t dbc_mia_threshold_MOTOR_TO_APP_DBG = 1500;
const dbc_MOTOR_TO_APP_DBG_s dbc_mia_replacement_MOTOR_TO_APP_DBG;

/* STATIC FUNCTION*/
static void Bridge_Controller__transfer_data_from_uart_driver_to_line_buffer(void);
static void Bridge_Controller__get_line_from_buffer(void);
static bool bridge_controller__decode_geo_message(can__msg_t *msg);
static bool bridge_controller__decode_geo_coordinates(can__msg_t *msg);
static bool bridge_controller__decode_motor_msg(can__msg_t *msg);

void bridge_controller_handler__parse_gps_data(void);

void app_to_bridge_command(void);

void Bridge_Controller_init(void) {
  MIA_LED = gpio__construct_as_output(GPIO__PORT_0, 17);

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

  Bridge_Controller__transfer_data_from_uart_driver_to_line_buffer();
  Bridge_Controller__get_line_from_buffer();
  app_to_bridge_command();
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
  static char temp_line_buffer[] = "GPS17291291,-32724082#";
  sscanf(temp_line_buffer, "GPS%ld,%ld#", &gps_destination_location.GPS_DEST_LATITUDE_SCALED_100000,
         &gps_destination_location.GPS_DEST_LONGITUDE_SCALED_100000);

  if (gps_destination_location.GPS_DEST_LATITUDE_SCALED_100000 != 0 &&
      gps_destination_location.GPS_DEST_LONGITUDE_SCALED_100000 != 0 && !gps_dest_data_latched) {

    printf("Latitude is %ld and longitude is %ld \n", gps_destination_location.GPS_DEST_LATITUDE_SCALED_100000,
           gps_destination_location.GPS_DEST_LONGITUDE_SCALED_100000);
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

void bridge_controller_transmit_value_to_app(void) {
  char sensor_msg[100] = {0};
  dbc_ULTRASONIC_TO_DRIVER_s sensor_values = get_ultra_sonic_data();
  int dummy_value = 0;
  snprintf(sensor_msg, 100, "%d,%d,%d,%d,%f,%f,%d,%f,%d,%d", sensor_values.ULTRASONIC_TO_DRIVER_left,
           sensor_values.ULTRASONIC_TO_DRIVER_front, sensor_values.ULTRASONIC_TO_DRIVER_right,
           sensor_values.ULTRASONIC_TO_DRIVER_back, (double)geo_coordinates_to_app.CURR_LATITUDE_SCALED_100000,
           (double)geo_coordinates_to_app.CURR_LONGITUDE_SCALED_100000, compass_value_to_app.GEO_STATUS_COMPASS_BEARING,
           (double)motor_speed_to_app.MOTOR_TO_APP_DBG_current_speed / 1000,
           motor_speed_to_app.MOTOR_TO_APP_DBG_current_steer, compass_value_to_app.GEO_STATUS_DISTANCE_TO_DESTINATION);

  uart_printf(bridge_uart, "%s", sensor_msg);
}

void CAN_RX_MSGS_FOR_BRIDGE(void) {
  can__msg_t can_msg = {0};
  bool geo_msg_received, geo_cordinates_received, motor_msg_recieved = 0;
  while (can__rx(can1, &can_msg, 0)) {
    geo_msg_received = bridge_controller__decode_geo_message(&can_msg);

    // TODO:zeel receive current geo coord from Geo_current_cord and divide them by 100000 before sending it to app
    geo_cordinates_received = bridge_controller__decode_geo_coordinates(&can_msg);
    // TODO:zeel recieve speed and steer from MOTOR_TO_APP
    motor_msg_recieved = bridge_controller__decode_motor_msg(&can_msg); // speed and steer

    if (geo_msg_received && geo_cordinates_received && motor_msg_recieved) {
      gpio__reset(MIA_LED); // turn OFF since we received the CAN message
    }
  }
}

bool bridge_controller__decode_geo_message(can__msg_t *msg) {
  bool status;
  const dbc_message_header_t header = {
      .message_id = msg->msg_id,
      .message_dlc = msg->frame_fields.data_len,
  };

  if (dbc_decode_GEO_STATUS(&compass_value_to_app, header, msg->data.bytes)) {
    status = true;
  } else {
    status = false;
  }
  return status;
}
bool bridge_controller__decode_geo_coordinates(can__msg_t *msg) {
  bool status;
  const dbc_message_header_t header = {
      .message_id = msg->msg_id,
      .message_dlc = msg->frame_fields.data_len,
  };
  if (dbc_decode_GEO_CURRENT_COORDS(&geo_coordinates_to_app, header, msg->data.bytes)) {
    geo_coordinates_to_app.CURR_LATITUDE_SCALED_100000 = geo_coordinates_to_app.CURR_LATITUDE_SCALED_100000 / 100000;
    geo_coordinates_to_app.CURR_LONGITUDE_SCALED_100000 = geo_coordinates_to_app.CURR_LONGITUDE_SCALED_100000 / 100000;
    status = true;
  } else {
    status = false;
  }
  return status;
}

bool bridge_controller__decode_motor_msg(can__msg_t *msg) {
  bool status;
  const dbc_message_header_t header = {
      .message_id = msg->msg_id,
      .message_dlc = msg->frame_fields.data_len,
  };
  if (dbc_decode_MOTOR_TO_APP_DBG(&motor_speed_to_app, header, msg->data.bytes)) {
    status = true;
  } else {
    status = false;
  }
  return status;
}

void bridge_can_mia_handler(void) {
  const uint32_t mia_increment_value = 100;
  if (dbc_service_mia_GEO_STATUS(&compass_value_to_app, mia_increment_value)) {
    gpio__set(MIA_LED); // turn ON to indicate MIA
  }
  if (dbc_service_mia_GEO_CURRENT_COORDS(&geo_coordinates_to_app, mia_increment_value)) {
    gpio__set(MIA_LED); // turn ON to indicate MIA
  }
  if (dbc_service_mia_MOTOR_TO_APP_DBG(&motor_speed_to_app, mia_increment_value)) {
    gpio__set(MIA_LED); // turn ON to indicate MIA
  }
}

void app_to_bridge_command(void) {

  const int BRIDGE_APP_DEFAULT_DATA = 0;
  const int BRIDGE_APP_STOP_DATA = 3;
  const int BRIDGE_APP_START_DATA = 1;
  // printf("%s \n", line_buffer);
  sscanf(line_buffer, "a%s", &app_command);

  if ((strcmp(app_command, "Start"))) {
    bridge_app_commands.DRIVE_STATUS_CMD_start = BRIDGE_APP_STOP_DATA;

  } else if (strcmp(app_command, "Stop")) {
    gps_dest_data_latched = false;
    bridge_app_commands.DRIVE_STATUS_CMD_start = BRIDGE_APP_START_DATA;
  } else {
    bridge_app_commands.DRIVE_STATUS_CMD_start = BRIDGE_APP_DEFAULT_DATA;
  }
}

void send_app_command_on_can(void) {
  can__msg_t can_msg = {0};
  dbc_message_header_t header = {
      .message_id = 0x0,
      .message_dlc = 0,
  };

  header = dbc_encode_DRIVE_STATUS_CMD(can_msg.data.bytes, &bridge_app_commands);

  can_msg.msg_id = header.message_id;
  can_msg.frame_fields.data_len = header.message_dlc;
  can__tx(can1, &can_msg, 0);
}

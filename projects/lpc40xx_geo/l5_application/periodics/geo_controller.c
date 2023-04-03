#include "geo_controller.h"
#include "board_io.h"
#include "can_bus.h"
#include "gpio.h"
#include "gps.h"
#include "project.h"

#define MIA_LED board_io__get_led3()

const dbc_GPS_DESTINATION_s dbc_mia_replacement_GPS_DESTINATION = {.GPS_DEST_LATITUDE_SCALED_100000 = 0,
                                                                   .GPS_DEST_LONGITUDE_SCALED_100000 = 0};
const uint32_t dbc_mia_threshold_GPS_DESTINATION = 100;

static gps_coordinates_t current_coord;
static dbc_GEO_STATUS_s geo_status;
static dbc_GPS_DESTINATION_s dest_coord;

static void geo_controller__manage_mia() {
  const uint32_t mia_increment_value = 10;
  if (dbc_service_mia_GPS_DESTINATION(&dest_coord, mia_increment_value)) {
    gpio__reset(MIA_LED); // turn ON to indicate MIA
  }
}

static void geo_controller__decode_sensor_message(can__msg_t *msg) {
  const dbc_message_header_t header = {.message_dlc = msg->frame_fields.data_len, .message_id = msg->msg_id};
  dbc_decode_GPS_DESTINATION(&dest_coord, header, msg->data.bytes);
}

void geo_controller__read_all_can_messages() {
  can__msg_t msg = {0};
  while (can__rx(can1, &msg, 0)) {
    geo_controller__decode_sensor_message(&msg);
    gpio__set(MIA_LED); // turn OFF since we received the CAN message
  }
  geo_controller__manage_mia();
}

void geo_controller__read_current_coordinates() {
  gps__get_gps_data_and_parse_coordinates();
  current_coord = gps__get_coordinates();
}

void geo_controller__calculate_heading() {
  geo_status.GEO_STATUS_COMPASS_BEARING = 0;
  geo_status.GEO_STATUS_COMPASS_HEADING = 0;
  geo_status.GEO_STATUS_DISTANCE_TO_DESTINATION = 0;
}

static void geo_controller__encode_driver_message(can__msg_t *msg) {
  dbc_message_header_t header = {0};
  header = dbc_encode_GEO_STATUS(msg->data.bytes, &geo_status);
  msg->frame_fields.data_len = header.message_dlc;
  msg->msg_id = header.message_id;
}

bool geo_controller__send_heading_to_driver_over_can() {
  bool tx_status = false;
  can__msg_t msg = {0};
  geo_controller__encode_driver_message(&msg);
  tx_status = can__tx(can1, &msg, 0);
  if (tx_status) {
    // Toggle LED0 for each successful transmission
    gpio__toggle(board_io__get_led0());
  }
  return tx_status;
}
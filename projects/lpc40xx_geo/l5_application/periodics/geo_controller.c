#include "geo_controller.h"
#include "board_io.h"
#include "can_bus.h"
#include "compass.h"
#include "gpio.h"
#include "gps.h"
#include "project.h"
#include "waypoints.h"
#include <stdio.h>

static gpio_s MIA_LED;
static gpio_s GPS_LOCK_LED;

const dbc_GPS_DESTINATION_s dbc_mia_replacement_GPS_DESTINATION = {.GPS_DEST_LATITUDE_SCALED_100000 = 0,
                                                                   .GPS_DEST_LONGITUDE_SCALED_100000 = 0};
const uint32_t dbc_mia_threshold_GPS_DESTINATION = 1500;

static gps_coordinates_t current_coord;
static dbc_GEO_CURRENT_COORDS_s current_coord_to_bridge;
static dbc_GEO_STATUS_s geo_status;
static dbc_GPS_DESTINATION_s dest_coord;

static void geo_controller__manage_mia() {
  const uint32_t mia_increment_value = 100;
  if (dbc_service_mia_GPS_DESTINATION(&dest_coord, mia_increment_value)) {
    gpio__set(MIA_LED); // turn ON to indicate MIA
  }
}

static void geo_controller__decode_bridge_message(can__msg_t *msg) {
  const dbc_message_header_t header = {.message_dlc = msg->frame_fields.data_len, .message_id = msg->msg_id};
  dbc_decode_GPS_DESTINATION(&dest_coord, header, msg->data.bytes);
}

void geo_controller__print_coord_and_heading_values() {
  gps_coordinates_t scaled_dest_coord = {0};
  scaled_dest_coord.latitude = (float)dest_coord.GPS_DEST_LATITUDE_SCALED_100000 / 100000;
  scaled_dest_coord.longitude = (float)dest_coord.GPS_DEST_LONGITUDE_SCALED_100000 / 100000;
  printf("cur lat %f, cur long %f, dest lat %f, dest long %f\n", current_coord.latitude, current_coord.longitude,
         scaled_dest_coord.latitude, scaled_dest_coord.longitude);
  printf("dist: %f, heading: %d, bearing: %d\n\n", geo_status.GEO_STATUS_DISTANCE_TO_DESTINATION,
         geo_status.GEO_STATUS_COMPASS_HEADING, geo_status.GEO_STATUS_COMPASS_BEARING);
}

void geo_controller__read_all_can_messages() {
  can__msg_t msg = {0};
  while (can__rx(can1, &msg, 0)) {
    geo_controller__decode_bridge_message(&msg);
    gpio__reset(MIA_LED); // turn OFF since we received the CAN message
  }
  geo_controller__manage_mia();
}

void geo_controller__read_current_coordinates() {
  gps__get_gps_data_and_parse_coordinates();
  current_coord = gps__get_coordinates();
}

void geo_controller__calculate_heading() {
  gps_coordinates_t scaled_dest_coord = {0};

  scaled_dest_coord.latitude = (float)dest_coord.GPS_DEST_LATITUDE_SCALED_100000 / 100000;
  scaled_dest_coord.longitude = (float)dest_coord.GPS_DEST_LONGITUDE_SCALED_100000 / 100000;

  geo_status.GEO_STATUS_COMPASS_BEARING = get_current_compass_bearing();

  geo_status.GEO_STATUS_COMPASS_HEADING =
      (uint16_t)waypoints__calculate_heading_to_next_point(current_coord, scaled_dest_coord);
  geo_status.GEO_STATUS_DISTANCE_TO_DESTINATION =
      waypoints__calculate_distance_to_dest(current_coord, scaled_dest_coord);
  geo_status.GEO_STATUS_SATELLITE_LOCKED = gps__get_satellite_lock_status();
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

static void geo_controller__encode_current_coord_message(can__msg_t *msg) {
  dbc_message_header_t header = {0};

  current_coord_to_bridge.CURR_LATITUDE_SCALED_100000 = current_coord.latitude * 100000;
  current_coord_to_bridge.CURR_LONGITUDE_SCALED_100000 = current_coord.longitude * 100000;

  header = dbc_encode_GEO_CURRENT_COORDS(msg->data.bytes, &current_coord_to_bridge);
  msg->frame_fields.data_len = header.message_dlc;
  msg->msg_id = header.message_id;
}

bool geo_controller__send_current_coord_to_bridge_over_can() {
  bool tx_status = false;
  can__msg_t msg = {0};
  geo_controller__encode_current_coord_message(&msg);
  tx_status = can__tx(can1, &msg, 0);
  if (tx_status) {
    // Toggle LED0 for each successful transmission
    // gpio__toggle(board_io__get_led0());
  }
  return tx_status;
}

static void geo_controller__LED_init() {
  GPS_LOCK_LED = gpio__construct_as_output(GPIO__PORT_2, 0);
  MIA_LED = gpio__construct_as_output(GPIO__PORT_0, 17);
}

void geo_controller__init() {
  gps__init();
  compass_init();
  geo_controller__LED_init();
}

void geo_controller__gps_lock_LED_update() {
  if (geo_status.GEO_STATUS_SATELLITE_LOCKED) {
    gpio__reset(GPS_LOCK_LED);         // turn ON to indicate GPS is locked
    gpio__reset(board_io__get_led2()); // TODO: remove after LED is soldered
  } else {
    gpio__toggle(GPS_LOCK_LED);         // toggle to indicate GPS is still waiting for a fix
    gpio__toggle(board_io__get_led2()); // TODO: remove after LED is soldered
  }
}
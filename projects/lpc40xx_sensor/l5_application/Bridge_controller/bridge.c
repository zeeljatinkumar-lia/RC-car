#include "bridge.h"
#include "board_io.h"
#include "can_bus.h"
#include "project.h"
#include <stdbool.h>
#include <stdio.h>

extern bool dest_is_towards_north;
extern bool dest_is_towards_south;
extern bool dest_is_towards_east;
extern bool dest_is_towards_west;

static dbc_GPS_DESTINATION_s dest_coord = {.GPS_DEST_LATITUDE_SCALED_100000 = 37,
                                           .GPS_DEST_LONGITUDE_SCALED_100000 = -121};

void bridge__read_destination_coordinates_from_app() {
  // current coordinates will be approx these lat = 37, long = -121
  dest_coord.GPS_DEST_LATITUDE_SCALED_100000 = 37 * 100000;
  dest_coord.GPS_DEST_LONGITUDE_SCALED_100000 = -121 * 100000;
  if (dest_is_towards_north) {
    dest_coord.GPS_DEST_LATITUDE_SCALED_100000 += 5 * 100000; // dest is towards north by 5 degrees
  }
  if (dest_is_towards_south) {
    dest_coord.GPS_DEST_LATITUDE_SCALED_100000 -= 5 * 100000; // dest is towards south by 5 degrees
  }
  if (dest_is_towards_east) {
    dest_coord.GPS_DEST_LONGITUDE_SCALED_100000 += 5 * 100000; // dest is towards east by 5 degrees
  }
  if (dest_is_towards_west) {
    dest_coord.GPS_DEST_LONGITUDE_SCALED_100000 -= 5 * 100000; // dest is towards west by 5 degrees
  }
}

void print_dest_coords() {
  printf("north %d, east %d, lat = %ld, long = %ld\n", dest_is_towards_north, dest_is_towards_east,
         dest_coord.GPS_DEST_LATITUDE_SCALED_100000 / 100000, dest_coord.GPS_DEST_LONGITUDE_SCALED_100000 / 100000);
}

dbc_GPS_DESTINATION_s bridge__get_destination_coordinates() { return dest_coord; }

static void driver_controller__encode_bridge_message(can__msg_t *msg) {
  dbc_message_header_t header = {0};
  header = dbc_encode_GPS_DESTINATION(msg->data.bytes, &dest_coord);
  msg->frame_fields.data_len = header.message_dlc;
  msg->msg_id = header.message_id;
}

void bridge__send_destination_coordinates_over_can() {
  bool tx_status = false;
  can__msg_t msg = {0};
  // test_stub();
  driver_controller__encode_bridge_message(&msg);
  tx_status = can__tx(can1, &msg, 0);
  if (tx_status) {
    // Toggle LED0 for each successful transmission
    gpio__toggle(board_io__get_led0());
  }
  return tx_status;
}

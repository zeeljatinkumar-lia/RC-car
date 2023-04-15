
#include "board_io.h"
#include "can_bus.h"

#include "can_ultrasonic_sensor_handler.h"
#include "gpio.h"

#include "LV_sensor_controller.h"
//#include "SR04_ultrasonic_sensor.h"
#include "stdio.h"

static can__msg_t can_msg;
static dbc_ULTRASONIC_TO_DRIVER_s sensor_struct;
// ultrasonic_distance_s *SR04_sensor_data;
dbc_ULTRASONIC_TO_DRIVER_s LV_sensor_data;

// We are assuming that we have a 10hz function in which we wish
// to transmit all messages that should be sent at 10x per second
void can_ultrasonic_sensor_transmit_messages_10hz(void) {
  // TODO_zeel: we need sensor_data from external file
  // SR04_sensor_data = can_get_sensor_data();
  sensor_struct = get_ultra_sonic_data();

  // sensor_struct.ULTRASONIC_TO_DRIVER_front = SR04_sensor_data->front;
  // sensor_struct.ULTRASONIC_TO_DRIVER_right = LV_sensor_data.ULTRASONIC_TO_DRIVER_right;
  // sensor_struct.ULTRASONIC_TO_DRIVER_left = LV_sensor_data.ULTRASONIC_TO_DRIVER_left;

  // Encode struct to bytes of the CAN message
  const dbc_message_header_t header = dbc_encode_ULTRASONIC_TO_DRIVER(can_msg.data.bytes, &sensor_struct);

  can_msg.msg_id = header.message_id;
  can_msg.frame_fields.data_len = header.message_dlc;
  can__tx(can1, &can_msg, 0);
}

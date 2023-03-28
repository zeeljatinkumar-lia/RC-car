
#include "board_io.h"
#include "can_bus.h"

#include "gpio.h"
#include "stdio.h"
#include "ultrasonic_sensor.h"
#include "can_ultrasonic_sensor_handler.h"

static can__msg_t can_msg;

// We are assuming that we have a 10hz function in which we wish
// to transmit all messages that should be sent at 10x per second
void can_handler__transmit_ultrasonic_sensor_messages_10hz(void) {
  sensor_struct.ULTRASONIC_TO_DRIVER_left = sensor_data.left;

  // Encode struct to bytes of the CAN message
  const dbc_message_header_t header = dbc_encode_ULTRASONIC_TO_DRIVER(can_msg.data.bytes, &sensor_struct);

  can_msg.msg_id = header.message_id;
  can_msg.frame_fields.data_len = header.message_dlc;
  can__tx(can1, &can_msg, 0);
}

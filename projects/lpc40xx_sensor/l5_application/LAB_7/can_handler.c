#include "can_handler.h"
#include "board_io.h"
#include "can_bus.h"

#include "gpio.h"
#include "stdio.h"
#include "ultrasonic_sensor.h"

static can__msg_t can_msg;

void get_ultrasonic_sensor_data() {
  static uint16_t front_value = 0, left_value = 0, right_value = 0;
  if (front_value < 50) {
    sensor_struct.ULTRASONIC_TO_DRIVER_front = front_value++;
    sensor_struct.ULTRASONIC_TO_DRIVER_left = left_value++;
    sensor_struct.ULTRASONIC_TO_DRIVER_right = right_value++;
  } else {
    front_value = 0;
    left_value = 0;
    right_value = 0;
  }
}

// We are assuming that we have a 10hz function in which we wish
// to transmit all messages that should be sent at 10x per second
void can_handler__transmit_messages_10hz(void) {

  // Encode struct to bytes of the CAN message
  const dbc_message_header_t header = dbc_encode_ULTRASONIC_TO_DRIVER(can_msg.data.bytes, &sensor_struct);

  can_msg.msg_id = header.message_id;
  can_msg.frame_fields.data_len = header.message_dlc;
  can__tx(can1, &can_msg, 0);
}

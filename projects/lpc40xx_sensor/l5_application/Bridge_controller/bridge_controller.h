#pragma once

//#include "can_dbc_send_msg.h"
#include "project.h"
//#include "sensor_controller.h"
#include <stdbool.h>
#include <stdint.h>

void Bridge_Controller_init(void);
void Bridge_Controller__10hz_handler(void);
void can_bridge_controller__Sending_dest_location(void);

void bridge_controller_transmit_value_to_app(void);

void CAN_RX_MSGS_FOR_BRIDGE(void);
void bridge_can_mia_handler(void);

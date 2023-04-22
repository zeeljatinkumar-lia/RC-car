#pragma once

//#include "can_dbc_send_msg.h"
#include "project.h"
//#include "sensor_controller.h"
#include <stdbool.h>
#include <stdint.h>

void Bridge_Controller_init(void);
void Bridge_Controller__10hz_handler(void);
void can_bridge_controller__Sending_dest_location(void);

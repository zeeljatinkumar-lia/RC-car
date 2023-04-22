#pragma once

#include "stdbool.h"

void driver_controller__read_all_can_messages();
bool driver_controller__send_cmd_to_motor_over_can();
void print_heading_and_motor_cmds();
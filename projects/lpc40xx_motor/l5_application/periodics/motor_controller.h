#pragma once
#include <stdbool.h>

void motor_controller__read_all_can_messages();
bool motor_controller__send_motor_speed_over_can();
void motor_controller__print_motor_cmd_values();
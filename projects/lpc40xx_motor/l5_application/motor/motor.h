#pragma once

#include <stdint.h>

void motor__init(void);

// Apply brake by setting the ESC throttle to neutral
void brake();

// Set the throttle to go forward
void go_forward(float dc_speed);

// Set the throttle to go in reverse
void go_reveser();

// Set the servo motor to go straight
void go_straight();

// Set the servo motor to turn left
void go_left(float servo_speed);

// Set the servo motor to turn right
void go_right(float servo_speed);

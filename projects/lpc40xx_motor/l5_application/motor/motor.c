#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

#include "delay.h"
#include "gpio.h"
#include "math.h"
#include "pwm1.h"
#include <stdint.h>

#include "motor.h"

/*servo motor only be used when go forward*/
bool servo_in_use = false;
static const uint32_t pwm_frequency_in_hz = 100;

/*init both DC and servo motor
  set the frequency to 100Hz
  set the motors to neutral*/
void motor__init(void) {
  gpio__construct_with_function(GPIO__PORT_2, 1, GPIO__FUNCTION_1);
  gpio__construct_with_function(GPIO__PORT_2, 4, GPIO__FUNCTION_1);
  delay__ms(100);
  pwm1__init_single_edge(pwm_frequency_in_hz);
  pwm1__set_duty_cycle(PWM1__2_1, 15.0f);
  pwm1__set_duty_cycle(PWM1__2_4, 15.0f);
  delay__ms(2000);
}

/*driver logic for dc motor
  accept the next behavior instructions：
  msg_id: forward, msg_data: dc_speed
  msg_id: brake
  msg_id: reveser*/

/*DC motor forward
  get target speed value and apply to DC driver
  msg_id: forward, msg_data: dc_speed
  msg_data range: 15.5f < dc_speed <= 17.0f*/
void go_forward(float dc_speed) {
  servo_in_use = true;
  pwm1__set_duty_cycle(PWM1__2_1, dc_speed);
  delay__ms(500);
}

/*DC motor brake
  msg_id: brake*/
void brake() {
  servo_in_use = false;
  pwm1__set_duty_cycle(PWM1__2_1, 15.0f);
  delay__ms(200);
}

/*DC motor reveser in Fixed speed
  due to ESC control principle
  change acceleration direction to backward first
  msg_id: reveser*/
void go_reveser() {
  brake();
  pwm1__set_duty_cycle(PWM1__2_1, 13.5f);
  delay__ms(200);
  pwm1__set_duty_cycle(PWM1__2_1, 15.0f);
  delay__ms(200);
  pwm1__set_duty_cycle(PWM1__2_1, 13.5f);
  delay__ms(500);
}

/*driver logic for servo motor
  only be used when go forward
  accept the next behavior instructions:
  msg_id: straight
  msg_id: left, msg_data: sevor_speed
  msg_id: right, msg_data: sevor_speed*/

/*servo motor go straight
  msg_id: straight*/
void go_straight() {
  if (servo_in_use == true) {
    pwm1__set_duty_cycle(PWM1__2_4, 15.0f);
    delay__ms(500);
  }
}

/*servo motor turn left
  msg_id: left, msg_data: sevor_speed
  msg_data range: 15.0f < servo_speed <= 20.0f*/
void go_left(float servo_speed) {
  if (servo_in_use == true) {
    pwm1__set_duty_cycle(PWM1__2_4, servo_speed);
    delay__ms(500);
  }
}

/*servo motor turn right
  msg_id: right, msg_data: sevor_speed
  msg_data range: 10.0f <= servo_speed < 15.0f*/
void go_right(float servo_speed) {
  if (servo_in_use == true) {
    pwm1__set_duty_cycle(PWM1__2_4, servo_speed);
    delay__ms(500);
  }
}

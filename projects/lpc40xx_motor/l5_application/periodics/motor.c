#include <stdio.h>

#include "gpio.h"
#include "pwm1.h"
#include <stdint.h>

#include "motor.h"

#define DC_MOTOR PWM1__2_0
#define SERVO_MOTOR PWM1__2_4
#define MOTOR__MAX_SPEED 50
#define MOTOR__MIN_SPEED -50
#define MOTOR__MAX_ANGLE 60
#define MOTOR__MIN_ANGLE -60
#define PWM_CENTRAL_VALUE 15.0f

static float previous_speed;

/*servo motor only be used when go forward*/
// bool servo_in_use = false;

/*init both DC and servo motor
  set the frequency to 100Hz
  set the motors to neutral*/
void motor__init(void) {
  gpio__construct_with_function(GPIO__PORT_2, 0, GPIO__FUNCTION_1);
  gpio__construct_with_function(GPIO__PORT_2, 4, GPIO__FUNCTION_1);
  pwm1__init_single_edge(100);
  pwm1__set_duty_cycle(DC_MOTOR, PWM_CENTRAL_VALUE);
  pwm1__set_duty_cycle(SERVO_MOTOR, PWM_CENTRAL_VALUE);
}

/* pwm value 15.0f = straight (0 degrees)
   pwm value 10.0f to 14.99f = right (-60 to -1 degrees)
   pwm value 15.01f to 20.0f = left (1 to 60 degrees)
*/
static float motor__convert_angle_to_pwm(int angle) {
  float pwm = 0.0f, fraction = 0.0f;
  fraction = 5.0f / MOTOR__MAX_ANGLE;
  if (angle >= MOTOR__MIN_ANGLE && angle <= MOTOR__MAX_ANGLE) {
    if (angle == 0) {
      pwm = PWM_CENTRAL_VALUE;
    } else {
      pwm = PWM_CENTRAL_VALUE + (angle * fraction);
    }
  }
  return pwm;
}

static float motor__convert_speed_to_pwm(float speed) {
  float pwm = 0.0f, fraction = 0.0f;
  fraction = 5.0f / MOTOR__MAX_SPEED;
  if (speed >= MOTOR__MIN_SPEED && speed <= MOTOR__MAX_SPEED) {
    if (speed == 0.0f) {
      pwm = PWM_CENTRAL_VALUE;
    } else {
      pwm = PWM_CENTRAL_VALUE + (speed * fraction);
    }
  }
  return pwm;
}

void motor__turn_servo_by_angle(int degrees) {
  float pwm = motor__convert_angle_to_pwm(degrees);
  pwm1__set_duty_cycle(SERVO_MOTOR, pwm);
}

static void motor__brake() { pwm1__set_duty_cycle(DC_MOTOR, PWM_CENTRAL_VALUE); }

static bool motor__if_direction_reversed(float speed) {
  // if both prev and curr speeds are -ve or +ve, their product will be +ve
  // if one is +ve and other is -ve, their product will be -ve, meaning direction changed
  return (speed * previous_speed < 0);
}

void motor__run_dc_motor_by_speed(float speed) {
  float pwm = 0.0f;
  if (motor__if_direction_reversed(speed)) {
    // we need to change the direction
    motor__brake();
    float reversing_pwm = motor__convert_speed_to_pwm(previous_speed * -1.0);
    pwm1__set_duty_cycle(DC_MOTOR, reversing_pwm);
    motor__brake();
  }
  pwm = motor__convert_speed_to_pwm(speed);
  pwm1__set_duty_cycle(DC_MOTOR, pwm);
  previous_speed = speed;
}

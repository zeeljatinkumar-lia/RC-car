#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "gpio.h"
#include "motor.h"
#include "pwm1.h"
#include "rpm.h"

#define DC_MOTOR PWM1__2_0
#define SERVO_MOTOR PWM1__2_4
#define MOTOR__MAX_SPEED 200
#define MOTOR__MIN_SPEED -200
#define MOTOR__MAX_ANGLE 60
#define MOTOR__MIN_ANGLE -60

bool is_car_in_reverse = false;
static double motor_speed_pwm;
// static uint8_t car_on_off = 0;
static const double PWM_CENTRAL_VALUE = 15.0;
static const double PWM_MAX_OFFSET = 5.0; // pwm goes from 10.0 - 15.0 - 20.0
static const double SPEED_ATTENUATION_FACTOR = 0.4;
static float TILT_OFFSET_PWM_VALUE = 0.6; // car is going slightly right at 15.0 so need to offset the tilt

static double motor__calculate_proportional_speed(double expected_speed, double current_speed) {
  return (expected_speed + (expected_speed - current_speed)); // proportional control
}

static double motor__calculate_motor_pwm(double percent_speed) {
  return PWM_CENTRAL_VALUE + ((PWM_MAX_OFFSET * percent_speed * SPEED_ATTENUATION_FACTOR) / 100);
}

void motor__init(void) {
  gpio__construct_with_function(GPIO__PORT_2, 0, GPIO__FUNCTION_1);
  gpio__construct_with_function(GPIO__PORT_2, 4, GPIO__FUNCTION_1);
  pwm1__init_single_edge(100);
  pwm1__set_duty_cycle(DC_MOTOR, PWM_CENTRAL_VALUE);
  pwm1__set_duty_cycle(SERVO_MOTOR, PWM_CENTRAL_VALUE);
  delay__ms(3000); // neutral signals for first 3 seconds is required for ESC calibration
}

/* pwm value 15.0f = straight (0 degrees)
   pwm value 10.0f to 14.99f = right (-60 to -1 degrees)
   pwm value 15.01f to 20.0f = left (1 to 60 degrees)
*/
static double motor__convert_angle_to_pwm(int angle) {
  double pwm = 0.0, fraction = 0.0f;
  fraction = PWM_MAX_OFFSET / MOTOR__MAX_ANGLE;
  if (angle >= MOTOR__MIN_ANGLE && angle <= MOTOR__MAX_ANGLE) {
    if (angle == 0) {
      pwm = PWM_CENTRAL_VALUE;
    } else {
      pwm = PWM_CENTRAL_VALUE + (angle * fraction);
    }
  }
  return pwm;
}

void motor__turn_servo_by_angle(int degrees) {
  float pwm = motor__convert_angle_to_pwm(degrees);
  pwm1__set_duty_cycle(SERVO_MOTOR, pwm + TILT_OFFSET_PWM_VALUE);
}

// void motor__set_on_off_state(dbc_BRIDGE_APP_COMMANDS_s *bridge_app_cmd) { car_on_off = bridge_app_cmd->APP_COMMAND; }

void motor__run_dc_motor_by_speed(double expected_speed) {
  if (expected_speed >= 0.0) {
    is_car_in_reverse = false;
    double percent_speed = 0.0;
    double current_speed = rpm_sensor__get_current_speed();
    double target_motor_speed = motor__calculate_proportional_speed(expected_speed, current_speed);
    percent_speed = (target_motor_speed * 100) / 15;
    motor_speed_pwm = motor__calculate_motor_pwm(percent_speed);
    pwm1__set_duty_cycle(DC_MOTOR, motor_speed_pwm);
    printf("expected: %f, actual: %f, target: %f, motor_pwm: %f\n", expected_speed, current_speed, target_motor_speed,
           motor_speed_pwm);
  } else {
    printf("expected: %f\n", expected_speed);
    if (is_car_in_reverse != true) {
      is_car_in_reverse = true;
      pwm1__set_duty_cycle(DC_MOTOR, 14.0);
      delay__ms(30);
      pwm1__set_duty_cycle(DC_MOTOR, 15.0);
      delay__ms(30);
    }
    pwm1__set_duty_cycle(DC_MOTOR, 14.0);
  }
}
// 16.0 => 3.3 to 3.5kmph

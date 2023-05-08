#include <stdio.h>

#include "gpio.h"
#include "pwm1.h"
#include <stdint.h>

#include "motor.h"
#include "rpm.h"

#include <stddef.h>

#define HISTORY_BUF_SIZE 5

#define DC_MOTOR PWM1__2_0
#define SERVO_MOTOR PWM1__2_4
#define MOTOR__MAX_SPEED 200
#define MOTOR__MIN_SPEED -200
#define MOTOR__MAX_ANGLE 60
#define MOTOR__MIN_ANGLE -60
#define PWM_CENTRAL_VALUE 15.0f

static float previous_speed;

// PID code

static const double Kp = 0.5;
static const double Ki = 0.05;
static const double Kd = 0;

static double error_history[HISTORY_BUF_SIZE] = {0};

static double dt = 0.5f; // Sample rate of 2Hz (maybe faster is needed)

static void update_error_buffer(double latest_error) {
  for (size_t i = HISTORY_BUF_SIZE - 1; i > 0; --i) {
    error_history[i] = error_history[i - 1];
  }
  error_history[0] = latest_error;
}

static double error_central_difference(void) { return (error_history[0] - error_history[2]) / 2; }

static double squared_error_integrate(void) {
  double integral = 0.0;
  for (size_t i = 0; i < HISTORY_BUF_SIZE; ++i) {
    integral += error_history[i] * error_history[i];
  }
  return integral;
}

double motor_pid__compute_pid(double target_speed, double actual_speed) {
  update_error_buffer(target_speed - actual_speed);
  return Kp * error_history[0] + Ki * squared_error_integrate() + Kd * error_central_difference();
}

double motor_pid__compute_pid_run(double target_speed, double actual_speed) {
  if ((target_speed - actual_speed) > 5)
    return 2; // increase the speed
  else
    return -2; // decrease the speed
}

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
  delay__ms(3000); // neutral signals for first 3 seconds is required for ESC calibration
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

float pwm_g = 0;
static float motor__convert_speed_to_pwm(float speed) {
  float pwm = 0.0f, fraction = 0.0f;
  fraction = 5.0f / MOTOR__MAX_SPEED;
  if (speed < 0.0f) {
    pwm = 14.4;
  } else if (speed == 0.0f) {
    pwm = PWM_CENTRAL_VALUE;
  } else {
    /*positive speed starts 15.8, 15.85, 15.9, 15.95. negative speed starts 14.5, 14.4, */
    // double pid_speed = speed + motor_pid__compute_pid(speed, rpm_sensor__get_current_speed());
    // pwm = PWM_CENTRAL_VALUE + (pid_speed * fraction);
    // crawling at 15.9. running at 15.95, 16.0, 16.05, 16.1. works at 15.9 base and no obstacle in college
    pwm = 16.0 + (speed * fraction);
  }
  pwm_g = pwm;
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
    // float reversing_pwm = motor__convert_speed_to_pwm(previous_speed * -1.0);
    // pwm1__set_duty_cycle(DC_MOTOR, reversing_pwm);
    pwm1__set_duty_cycle(DC_MOTOR, 13.5);
    motor__brake();
  }
  pwm = motor__convert_speed_to_pwm(speed);
  pwm1__set_duty_cycle(DC_MOTOR, pwm);
  if (speed != 0.0) {
    previous_speed = speed;
  }
}

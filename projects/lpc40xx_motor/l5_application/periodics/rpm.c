#include "RPM.h"

#include <stddef.h>
#include <stdio.h>

#include "clock.h"
#include "gpio.h"
#include "lpc40xx.h"
#include "lpc_peripherals.h"
#include "project.h"

static const uint32_t timer_counter_upper_bound = 10000;

static const float circumference_of_wheel_cm = 34.56f;
static const float wheel_to_gear_ratio = 2.125f;
static float kph_scalar = (circumference_of_wheel_cm * 3600) / (wheel_to_gear_ratio * 100000);
static float speed_in_kph;
static float speed_in_mph;

static void timer2_isr(void) {
  LPC_TIM2->TC = 0;
  LPC_TIM2->IR |= 0x10;
}

void rpm_sensor__init(void) {
  uint32_t prescalar_divider = 959;
  lpc_peripheral__turn_on_power_to(LPC_PERIPHERAL__TIMER2);
  LPC_TIM2->IR |= 0x3F;
  LPC_TIM2->CCR = (1 << 0) | (1 << 2);
  lpc_peripheral__enable_interrupt(LPC_PERIPHERAL__TIMER2, timer2_isr, NULL);
  LPC_TIM2->PR = prescalar_divider;
  LPC_TIM2->TCR |= (1 << 0);
  gpio__construct_with_function(GPIO__PORT_2, 6, GPIO__FUNCTION_3);
  kph_scalar *= clock__get_peripheral_clock_hz() / (prescalar_divider + 1);
}

void rpm_sensor__update_speed_value(dbc_MOTOR_TO_APP_DBG_s *speed_val) {
  uint32_t timer_capture_value = LPC_TIM2->CR0;
  uint32_t timer_counter_value = LPC_TIM2->TC;
  float offset = 44500.0f;
  speed_in_kph = (kph_scalar - offset) / timer_capture_value;
  if (timer_counter_value > timer_counter_upper_bound || timer_capture_value == 0) {
    speed_in_kph = 0.0f;
  }
  speed_in_mph = speed_in_kph * 0.6214;
  speed_val->MOTOR_TO_APP_DBG_current_speed = speed_in_mph;
  // printf("speed_in_kph %f kph_scalar=%f timer_capture_value=%d \n ", speed_in_kph, kph_scalar, timer_capture_value);
}

float rpm_sensor__get_current_speed() { return speed_in_mph; }

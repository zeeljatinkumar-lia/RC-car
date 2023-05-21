#include "rpm.h"
#include <stddef.h>
#include <stdio.h>

#include "clock.h"
#include "gpio.h"
#include "lpc40xx.h"
#include "lpc_peripherals.h"
#include "project.h"

extern bool is_car_in_reverse;

static const double circumference_of_wheel_cm = 34.56;
static const double wheel_to_gear_ratio = 2.125;
static double kph_scalar = (circumference_of_wheel_cm * 3600) / (wheel_to_gear_ratio * 100000);
static double speed_in_kph;

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

void rpm_sensor__update_speed_value() {
  uint32_t timer_capture_value = LPC_TIM2->CR0;
  if (timer_capture_value == 0) {
    timer_capture_value = 1;
  }
  double const offset = 25000;

  speed_in_kph = (kph_scalar - offset) / timer_capture_value;
  if ((speed_in_kph < 0.5) || (speed_in_kph > 100)) {
    speed_in_kph = 0.0;
  }
  if (is_car_in_reverse) {
    speed_in_kph = -1 * speed_in_kph;
  }
}

double rpm_sensor__get_current_speed() { return speed_in_kph; }

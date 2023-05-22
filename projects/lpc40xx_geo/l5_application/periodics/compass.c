#include "FreeRTOS.h"
#include "math.h"
#include "task.h"
#include <stdint.h>
#include <stdio.h>

#include "gpio.h"
#include "i2c.h"

#include "compass.h"

#define LSM303AGR_MAG_CTRL_REG1 0x60  // LSM303AGR magnetometer control register 1 address
#define LSM303AGR_MAG_CTRL_REG2 0x61  // LSM303AGR magnetometer control register 2 address
#define LSM303AGR_MAG_CTRL_REG3 0x62  // LSM303AGR magnetometer control register 3 address
#define LSM303AGR_MAG_CTRL_REG4 0x63  // LSM303AGR magnetometer control register 4 address
#define LSM303AGR_MAG_OUTX_L_REG 0x68 // LSM303AGR magnetometer output X-axis LSB register address
#define LSM303AGR_MAG_OUTX_H_REG 0x69 // LSM303AGR magnetometer output X-axis MSB register address
#define LSM303AGR_MAG_OUTY_L_REG 0x6A // LSM303AGR magnetometer output Y-axis LSB register address
#define LSM303AGR_MAG_OUTY_H_REG 0x6B // LSM303AGR magnetometer output Y-axis MSB register address
#define LSM303AGR_MAG_OUTZ_L_REG 0x6C // LSM303AGR magnetometer output Z-axis LSB register address
#define LSM303AGR_MAG_OUTZ_H_REG 0x6D // LSM303AGR magnetometer output Z-axis MSB register address

#define magnetometer_read 0x3D
#define magnetometer_write 0x3C

static const i2c_e current_i2c = I2C__2;
const uint32_t i2c_speed_hz = UINT32_C(400) * 1000;
static StaticSemaphore_t binary_semaphore_struct;
static StaticSemaphore_t mutex_struct;
static float current_compass_bearing;
float mag[3];

#define PI 3.14159265358979323846;

void compass__init() {
  i2c__initialize(current_i2c, i2c_speed_hz, clock__get_peripheral_clock_hz(), &binary_semaphore_struct, &mutex_struct);

  uint8_t ctrl_reg1_value = 0x00;
  uint8_t ctrl_reg2_value = 0x00;
  uint8_t ctrl_reg3_value = 0x00;

  i2c__write_single(current_i2c, magnetometer_write, LSM303AGR_MAG_CTRL_REG1, ctrl_reg1_value);
  i2c__write_single(current_i2c, magnetometer_write, LSM303AGR_MAG_CTRL_REG2, ctrl_reg2_value);
  i2c__write_single(current_i2c, magnetometer_write, LSM303AGR_MAG_CTRL_REG3, ctrl_reg3_value);
}

static void compass__transformation_mag(float uncalibrated_values[3]) {
  float calibrated_mag_values[3];
  float matrix[3][3] = {{2.024, 0.034, -0.021}, {0.181, 2.003, 0.024}, {0.056, 0.205, 1.858}};
  float bias[3] = {-15.138, -17.842, 43.116};
  for (int i = 0; i < 3; ++i)
    uncalibrated_values[i] = uncalibrated_values[i] - bias[i];
  float result[3] = {0, 0, 0};
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
      result[i] += matrix[i][j] * uncalibrated_values[j];
  for (int i = 0; i < 3; ++i)
    calibrated_mag_values[i] = result[i];

  mag[0] = calibrated_mag_values[0];
  mag[1] = calibrated_mag_values[1];
  mag[2] = calibrated_mag_values[2];
}

static void compass__calculate_current_bearing(float mag_arr[3]) {
  float magnitude = sqrtf(mag_arr[0] * mag_arr[0] + mag_arr[1] * mag_arr[1] + mag_arr[2] * mag_arr[2]);
  float Mxz = mag_arr[2] / magnitude;
  float Mxy = mag_arr[1] / magnitude;
  float Mxx = mag_arr[0] / magnitude;
  // printf("Without magnitude angle: %f\n", (atan2(mag[1], mag[0])) * 180 / 3.1415926535);
  current_compass_bearing = (atan2(Mxy, Mxx)) * 180 / PI;
  if (current_compass_bearing < 0) {
    current_compass_bearing = 360 + current_compass_bearing;
  }
  current_compass_bearing = (360 - (int)current_compass_bearing) % 360;
  // printf("current_compass_bearing: %f\n", current_compass_bearing);
}

static void compass__read() {
  uint8_t buffer[6];
  if (i2c__read_slave_data(current_i2c, magnetometer_read, LSM303AGR_MAG_OUTX_L_REG, buffer, 6) == true) {
    mag[0] = (int16_t)(buffer[1] << 8 | buffer[0]);
    mag[1] = (int16_t)(buffer[3] << 8 | buffer[2]);
    mag[2] = (int16_t)(buffer[5] << 8 | buffer[4]);

    compass__transformation_mag(mag);
    // printf("x %f, y %f, z %f\n", mag[0], mag[1], mag[2]);
    compass__calculate_current_bearing(mag);
  }
}

float compass__get_current_bearing() {
  compass__read();
  return current_compass_bearing;
}
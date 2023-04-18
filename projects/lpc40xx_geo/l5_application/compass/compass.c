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
static float current_compass_heading;

#define PI 3.14159265358979323846;

void lsm303agr_mag_init() {
  i2c__initialize(current_i2c, i2c_speed_hz, clock__get_peripheral_clock_hz(), &binary_semaphore_struct, &mutex_struct);

  uint8_t ctrl_reg1_value = 0x00;
  uint8_t ctrl_reg2_value = 0x00;
  uint8_t ctrl_reg3_value = 0x00;

  i2c__write_single(current_i2c, magnetometer_write, LSM303AGR_MAG_CTRL_REG1, ctrl_reg1_value);
  i2c__write_single(current_i2c, magnetometer_write, LSM303AGR_MAG_CTRL_REG2, ctrl_reg2_value);
  i2c__write_single(current_i2c, magnetometer_write, LSM303AGR_MAG_CTRL_REG3, ctrl_reg3_value);
}

void lsm303agr_mag_read() {
  uint8_t buffer[6];
  float mag[3];
  i2c__read_slave_data(current_i2c, magnetometer_read, LSM303AGR_MAG_OUTX_L_REG, buffer, 6);

  mag[0] = (int16_t)(buffer[1] << 8 | buffer[0]);
  mag[1] = (int16_t)(buffer[3] << 8 | buffer[2]);
  mag[2] = (int16_t)(buffer[5] << 8 | buffer[4]);

  printf("x %f, y %f, z %f\n", mag[0], mag[1], mag[2]);
  get_current_compass_heading(mag);
}

void get_current_compass_heading(float mag[3]) {

  float magnitude = sqrtf(mag[0] * mag[0] + mag[1] * mag[1] + mag[2] * mag[2]);
  float Mxz = mag[2] / magnitude;
  float Mxy = mag[1] / magnitude;
  float Mxx = mag[0] / magnitude;
  printf("Without magnitude angle: %f\n", (atan2(mag[1], mag[0])) * 180 / 3.1415926535);
  current_compass_heading = (atan2(Mxy, Mxx)) * 180 / PI;
  if (current_compass_heading < 0) {
    current_compass_heading = 360 + current_compass_heading;
  }
  printf("With magitude angle: %f\n", current_compass_heading);
}

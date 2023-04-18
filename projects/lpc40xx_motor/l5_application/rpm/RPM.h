#pragma once

#include <stdint.h>

/**
 *  Initialize the rpm_sensor by setting up pin P2.6
 *        for T2_CAP0.
 */
void rpm_sensor__init(void);

/**
 *Update speed value
 */
void rpm_sensor__update_speed_value(void);

/**
 * Return the value from the RPM sensor
 * speed_in_kph
 */
float rpm_sensor__get_speed(void);

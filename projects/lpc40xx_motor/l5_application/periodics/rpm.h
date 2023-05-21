#pragma once

#include "project.h"
#include <stdint.h>

/**
 *  Initialize the rpm_sensor by setting up pin P2.6
 *        for T2_CAP0.
 */
void rpm_sensor__init(void);

/**
 *Update speed value
 */
void rpm_sensor__update_speed_value();
double rpm_sensor__get_current_speed();

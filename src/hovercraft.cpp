/**
 * @file main.cpp
 * @brief Main application file.
 * @author Robotics Society
 * @date March 11, 2024
 *
 * @license GNU General Public License Version 3
 */

#include <Arduino.h>
#include <crsf/crsf.h>
#include <sbus/sbus.h>
#include <serialIO.h>
#include <esp32-hal-rmt.h>

crsf crsfRx(Serial1, 16, 17);
serialIO *receiver = &crsfRx;

void setup() { receiver->begin(); }

void loop() {}

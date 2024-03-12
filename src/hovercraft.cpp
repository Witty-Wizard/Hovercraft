/**
 * @file main.cpp
 * @brief Main application file.
 * @author Robotics Society
 * @date March 11, 2024
 *
 * @license GNU General Public License Version 3
 */

#include <Arduino.h>
#include <esp32-hal-rmt.h>

#include <serialIO.h>
#include <crsf/crsf.h>
#include <sbus/sbus.h>

crsf_channels_t channelData;

serialIO *receiver = new sbus(Serial1, 16, 17);;

void setup() { 
    receiver->begin();
 }

void loop() {
    receiver->processIncoming();
    receiver->getChannel(&channelData);
}

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

#include <crsf.h>
#include <sbus.h>
#include <serialIO.h>

crsf_channels_t channelData;

serialIO *receiver = NULL;

void setup() {
  Serial.begin(115200);
  receiver = new crsf(Serial1,5,10);
  receiver->begin();
}

void loop() {
  receiver->processIncoming();
  receiver->getChannel(&channelData);
  Serial.println(channelData.channel1);
}

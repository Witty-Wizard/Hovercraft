/**
 * @file main.cpp
 * @brief Main application file.
 * @author Robotics Society
 * @date April , 2024
 *
 * @license GNU General Public License Version 3
 */

#include <Arduino.h>
#include <DriveMaster.h>
#include <SerialIO.h>

crsf_channels_t channelData;

SerialIO *receiver = nullptr;
DriveMaster *motorInflate = nullptr;
DriveMaster *motorThrust = nullptr;

void setup() {
  Serial.begin(115200);
  receiver = new crsf(Serial1, 5);

  motorInflate = new dshot(18);
  motorThrust = new dshot(5);

  receiver->begin();
  motorInflate->begin();
  motorThrust->begin();
}

void loop() {
}